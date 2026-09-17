#!/usr/bin/env python3
"""The excgrid codegen driver: Yacas -> committed generated kernels.

Invoked exactly once per functional addition or .ey-script change (the
maintainer step; never by any build).  Each dft-xc/xc_defs/<name>.ey is
expanded by the vendored Yacas (tools/yacas/, pinned 1.8.0-win64 - see
YACAS_PROVENANCE.md) with the repo's own skeleton machinery, the child's
output is INSPECTED (not just its exit status - yacas exits 0 on an
expansion error), normalized to LF and clang-format, and written to
generated/<name>.cpp.

Usage:
    python tools/regenerate.py             # regenerate all committed outputs,
                                           # then rewrite xc_defs/freshness.json
    python tools/regenerate.py --check     # regenerate to a temp dir, diff
                                           # against the committed files (the
                                           # deliberate freshness gate)

The manifest rewritten on the first path is what tools/check_freshness.py
reads at commit time: it hashes the xc_defs sources and fails when one has
moved, in milliseconds and without Yacas, so the hook stays usable.  That
checker proves the SOURCES are unchanged since the blessing; this script's
--check mode is what proves the generated code matches them.

Yacas discovery order: --yacas, $EXCGRID_YACAS, tools/yacas/bin/yacas.exe,
then PATH.  clang-format discovery: --clang-format, $EXCGRID_CLANG_FORMAT,
then PATH.
"""

import argparse
import difflib
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

import check_freshness

# The generation manifest: (xc_defs source, generated output), one per
# shipped functional.  Adding a functional means appending here AND adding
# the generated file to the library sources (see the maintainer guide,
# docs/maintainer-guide.md).
FUNCTIONALS = [
    ("slater_exchange.ey", "slater_exchange.cpp"),
    ("vwn5_correlation.ey", "vwn5_correlation.cpp"),
    ("vwn3_correlation.ey", "vwn3_correlation.cpp"),
    ("pw92_correlation.ey", "pw92_correlation.cpp"),
    ("becke88_exchange.ey", "becke88_exchange.cpp"),
    ("pw91_exchange.ey", "pw91_exchange.cpp"),
    ("pbe_exchange.ey", "pbe_exchange.cpp"),
    ("revpbe_exchange.ey", "revpbe_exchange.cpp"),
    ("rpbe_exchange.ey", "rpbe_exchange.cpp"),
    ("mpw91_exchange.ey", "mpw91_exchange.cpp"),
    ("pbesol_exchange.ey", "pbesol_exchange.cpp"),
    ("lyp_correlation.ey", "lyp_correlation.cpp"),
    ("pbe_correlation.ey", "pbe_correlation.cpp"),
    ("pw91_correlation.ey", "pw91_correlation.cpp"),
    ("p86_correlation.ey", "p86_correlation.cpp"),
]

ROOT = Path(__file__).resolve().parent.parent
DEFS_DIR = ROOT / "xc_defs"
GENERATED_DIR = ROOT / "generated"
YACAS_CANDIDATES = [
    ROOT / "tools" / "yacas" / "bin" / "yacas.exe",
    ROOT / "tools" / "yacas" / "bin" / "yacas",
]

CLANG_FORMAT_CANDIDATES = [
    r"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang-format.exe",
]

# The per-kernel bound.  Deliberately far above the slowest HEALTHY kernel -
# pbe_correlation at about 12.5 minutes, timed from the child process itself,
# against about 3 minutes for the next slowest, vwn5_correlation, read off
# the per-kernel output timestamps of a full regeneration.  The bound exists
# to stop a NON-TERMINATING expansion, not to police the pace, so do not
# lower it to make a run feel faster.  See the 2026-09-13 note in
# docs/maintainer-guide.md.
KERNEL_TIMEOUT_SECONDS = 1800


def find_tool(args_path, env_var, candidates, path_name):
    if args_path:
        if not Path(args_path).exists():
            raise SystemExit(f"tool not found: {args_path}")
        return args_path
    if env_var:
        if not Path(env_var).exists():
            raise SystemExit(f"tool not found (env): {env_var}")
        return env_var
    for candidate in candidates:
        if Path(candidate).exists():
            return str(candidate)
    found = shutil.which(path_name)
    if found:
        return found
    raise SystemExit(
        f"{path_name} not found (arguments, environment, {candidates[0]}, or PATH)")


def yacas_scripts_dir(yacas):
    """The yacas standard-library path (`yacas -d`)."""
    out = subprocess.run(
        [yacas, "-d"], check=True, capture_output=True, text=True, encoding="utf-8", errors="replace"
    ).stdout.strip()
    return out


#: Yacas' own error banners, read out of the pinned binary
#: (`tools/yacas/bin/yacas.exe`, 1.8.0-win64) rather than guessed at - every
#: one of these is a literal in its message table.  Both reproduced failures
#: below EXIT 0 while printing one: a missing `scripts_root` entry writes
#: `Error in file` + `File not found` (472 bytes on stdout) and a `.ey`
#: calling a rule that does not exist writes `Error in file` +
#: `bad argument number 1` (1087 bytes).  See inspect_expansion().
YACAS_ERROR_MARKERS = (
    "Error in file",
    "Error reading file",
    "File not found",
    "bad argument number",
    "Invalid argument",
    "Wrong number of arguments",
    "Unknown exception",
)

#: What the captured text must carry to be a kernel: the excgrid namespace and
#: at least one `result.<field> = ` assignment.  15 of 15 committed generated
#: files carry both.  This is the half of the check that does not depend on
#: yacas' wording, so it catches an empty or unrelated output; it does NOT
#: catch the undefined-rule failure, which emits a plausible kernel with the
#: error text spliced into one assignment, and that is why the markers above
#: are the load-bearing half.
KERNEL_SHAPE = (
    re.compile(r"^namespace excgrid", re.MULTILINE),
    re.compile(r"^[ \t]*result\.[A-Za-z]+ =", re.MULTILINE),
)


def inspect_expansion(stdout_text, stderr_text):
    """The reasons a captured expansion is not a kernel; an empty list means it is.

    The child's EXIT STATUS is not evidence.  Yacas reports an expansion error
    on STDOUT and still exits 0, so the driver used to rename that stdout into
    `generated/<name>.cpp` and then rewrite `xc_defs/freshness.json` - a
    committed kernel destroyed by 472 bytes of error text, and a manifest
    blessing the tree that had just broken.  Reproduced end to end 2026-09-15;
    the maintainer guide carried the warning and the code did not.
    """
    problems = []
    for marker in YACAS_ERROR_MARKERS:
        for stream_name, stream in (("stdout", stdout_text), ("stderr", stderr_text)):
            for line in stream.splitlines():
                if marker in line:
                    problems.append(
                        f"yacas wrote {marker!r} to {stream_name}: {line.strip()}")
                    break
    for shape in KERNEL_SHAPE:
        if not shape.search(stdout_text):
            problems.append(
                f"the captured output has no line matching {shape.pattern!r}, "
                f"so it is not a kernel")
    return problems


def generate_one(yacas, scripts_root, ey_path, out_path, clang_format=None,
                 timeout=KERNEL_TIMEOUT_SECONDS):
    """One yacas invocation: the `-pc --patchload` console-expansion pattern.

    The kernel is built beside `out_path` and RENAMED into place only once it
    is complete, INSPECTED, formatted and pruned, so `out_path` is never
    observable in a half-written state and never receives an error string.
    Both halves of that sentence are repairs: the old shape opened `out_path`
    with "w" BEFORE starting the child, so a crash, a kill, a timeout or a
    power cut left a 0-byte file where a committed kernel used to be; and it
    piped the child's stdout straight into the file while trusting only its
    exit status, so an expansion error - which yacas prints to stdout and
    exits 0 on - REPLACED a committed kernel and then blessed the manifest
    (inspect_expansion() carries the measurement).

    The child is killed at `timeout` and the kernel is named in the error.
    Without a bound, one non-terminating expansion stalls the whole serial
    run - every other kernel, and the manifest rewrite at the end of it.
    """
    # Yacas parses Windows paths with forward slashes (the smoke-tested
    # form); backslashes get mangled in its argument handling.
    yacas_ey = str(ey_path).replace("\\", "/")
    temporary = out_path.with_name(out_path.name + ".partial")
    try:
        completed = subprocess.run(
            [yacas, "--rootdir", scripts_root, "-pc", "--patchload", yacas_ey],
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
            timeout=timeout,
        )
        if completed.returncode != 0:
            raise SystemExit(
                f"{ey_path.name}: yacas exited {completed.returncode}, so {out_path.name} "
                f"was NOT rewritten and the freshness manifest has NOT been blessed. "
                f"stderr: {completed.stderr.strip()[:400]}")
        problems = inspect_expansion(completed.stdout, completed.stderr)
        if problems:
            raise SystemExit(
                f"{ey_path.name}: yacas exited 0 but produced no kernel, so {out_path.name} "
                f"was NOT rewritten and the freshness manifest has NOT been blessed "
                f"({len(problems)} finding(s)):\n  " + "\n  ".join(problems))
        if completed.stderr.strip():
            print(f"  note: {ey_path.name}: yacas wrote {len(completed.stderr)} bytes to "
                  f"stderr while producing a kernel: "
                  f"{completed.stderr.strip().splitlines()[0]}")
        temporary.write_text(completed.stdout, encoding="utf-8", newline="\n")
        normalize_and_format(temporary, clang_format)
        prune_in_place(temporary)
    except subprocess.TimeoutExpired:
        temporary.unlink(missing_ok=True)
        raise SystemExit(
            f"{ey_path.name}: yacas did not finish within {timeout}s and was killed. "
            f"The committed {out_path.name} is untouched. The slowest HEALTHY kernel "
            f"timed on this tree is pbe_correlation at about 12.5 minutes, so a larger "
            f"bound needs a measurement, not a guess.")
    except BaseException:
        temporary.unlink(missing_ok=True)
        raise
    os.replace(temporary, out_path)


def normalize_and_format(path, clang_format):
    """LF line endings, then the repository clang-format on the output.

    The config is named EXPLICITLY rather than left to clang-format's
    directory search.  That search starts at the file being formatted, and
    on the --check path this file lives in a temporary directory: an
    in-tree .clang-format is not an ancestor of it, so the search would
    miss it and fall back to the LLVM defaults, which reproduce none of the
    committed bytes.  The gate would then report all fifteen kernels as
    drift on a clean tree.  Naming the config makes the formatter the same
    one on both paths, and a missing config is a hard error rather than a
    silent restyle.
    """
    text = path.read_text(encoding="utf-8").replace("\r\n", "\n")
    path.write_text(text, encoding="utf-8", newline="\n")
    if clang_format:
        style = f"file:{ROOT / '.clang-format'}"
        subprocess.run([clang_format, "--style", style, "-i", str(path)], check=True)


# One emitted CSE temporary: `const double C17 = <expr>;`, always one line
# (checked over the committed corpus: 4772 of 4772).
DECLARATION = re.compile(r"^(?P<indent>[ \t]*)const double (?P<tag>C\d+) = (?P<rhs>.*);\s*$")
# The assignment whose expression the group's declarations feed.
RESULT_START = re.compile(r"^[ \t]*result\.[A-Za-z]+ =")
TAG = re.compile(r"\bC\d+\b")


def prune_unreachable_temporaries(text):
    """Drop CSE temporaries that nothing printed in their own C++ scope reaches.

    The skeleton emits each branch's whole CSE list, and the LDA flow
    substitutes the spin-zero limit subexpressions into the derivative before
    re-running the CSE - which leaves the limit entries whose only consumer was
    an expanded occurrence referenced by nothing at all.  MSVC reports those one
    tip at a time, because C4189 fires only on a local with ZERO references: a
    dead chain, whose entries reference each other, produces no warning at all.

    Reachability is computed from every `result.*` assignment in the SAME C++
    scope, because that is what the declarations are visible to: the LDA
    skeleton puts each branch's list inside its own braces, while the GGA
    skeleton declares all six blocks in one function scope, where a later
    block's expression may reference an earlier block's temporaries.  Survivors
    keep their emission order, so declarations still precede their uses.
    """
    lines = text.split("\n")
    kept = [True] * len(lines)

    # One entry per open brace scope, innermost last; the file itself is scope 0
    # so a file with no braces at all (none today) still prunes.
    scopes = [{"declarations": [], "roots": []}]
    pending_root = None

    index = 0
    while index < len(lines):
        line = lines[index]

        if pending_root is not None:
            pending_root[1].append(index)
            if ";" in line:
                scopes[-1]["roots"].append("\n".join(lines[i] for i in pending_root[1]))
                pending_root = None
        else:
            declaration = DECLARATION.match(line)
            if declaration is not None:
                scopes[-1]["declarations"].append(
                    (index, declaration.group("tag"), declaration.group("rhs"))
                )
            elif RESULT_START.match(line):
                # The assignment may wrap; it ends at the first line with ';'.
                pending_root = (index, [index])
                if ";" in line:
                    scopes[-1]["roots"].append(line)
                    pending_root = None

        for _ in range(line.count("}")):
            if len(scopes) > 1:
                _prune_scope(scopes.pop(), kept, lines)

        for _ in range(line.count("{")):
            scopes.append({"declarations": [], "roots": []})

        index += 1

    for scope in reversed(scopes):
        _prune_scope(scope, kept, lines)

    return "\n".join(line for line, keep in zip(lines, kept) if keep)


def _prune_scope(scope, kept, lines):
    """Mark a scope's unreachable declarations for removal."""
    if not scope["declarations"]:
        return

    live = set()
    for root in scope["roots"]:
        live.update(TAG.findall(root))

    # Reachability runs DOWN the dependency chain: a value the printed
    # expression needs makes the values its own right-hand side uses needed in
    # turn.  (The opposite rule - an entry is live because it *uses* something
    # live - keeps consumers of live values and drops the values themselves.)
    entries = {tag: rhs for _, tag, rhs in scope["declarations"]}
    changed = True
    while changed:
        changed = False
        for tag, rhs in entries.items():
            if tag not in live:
                continue
            for used in TAG.findall(rhs):
                if used not in live:
                    live.add(used)
                    changed = True

    for line_index, tag, _ in scope["declarations"]:
        if tag not in live:
            kept[line_index] = False


def prune_in_place(path):
    """Apply the reachability prune to a generated file, if it changes anything."""
    text = path.read_text(encoding="utf-8")
    pruned = prune_unreachable_temporaries(text)
    if pruned != text:
        path.write_text(pruned, encoding="utf-8", newline="\n")


def regenerate(output_dir, yacas_arg=None, clang_format_arg=None):
    yacas = find_tool(yacas_arg, os.environ.get("EXCGRID_YACAS"),
                      [str(p) for p in YACAS_CANDIDATES], "yacas")
    clang_format = find_tool(clang_format_arg, os.environ.get("EXCGRID_CLANG_FORMAT"),
                             [str(p) for p in CLANG_FORMAT_CANDIDATES], "clang-format")

    # Forward slashes throughout (yacas mangles backslashes in paths).
    scripts_root = os.pathsep.join(
        [str(DEFS_DIR).replace("\\", "/"), yacas_scripts_dir(yacas)])
    for ey_name, cpp_name in FUNCTIONALS:
        ey_path = DEFS_DIR / ey_name
        out_path = output_dir / cpp_name
        print(f"generating {cpp_name}")
        generate_one(yacas, scripts_root, ey_path, out_path, clang_format)


def main():
    parser = argparse.ArgumentParser(description="excgrid Yacas codegen driver")
    parser.add_argument("--check", action="store_true",
                        help="regenerate to a temp dir and diff against the committed outputs")
    parser.add_argument("--yacas", help="path to the yacas binary")
    parser.add_argument("--clang-format", help="path to clang-format")
    args = parser.parse_args()

    if args.check:
        with tempfile.TemporaryDirectory(prefix="excgrid-codegen-") as tmp:
            tmp_dir = Path(tmp)
            regenerate(tmp_dir, args.yacas, args.clang_format)
            mismatches = []
            for _, cpp_name in FUNCTIONALS:
                committed = (GENERATED_DIR / cpp_name).read_text(encoding="utf-8")
                fresh = (tmp_dir / cpp_name).read_text(encoding="utf-8")
                if committed != fresh:
                    mismatches.append(cpp_name)
                    diff = "\n".join(difflib.unified_diff(
                        committed.splitlines(), fresh.splitlines(),
                        fromfile=f"generated/{cpp_name}", tofile="regenerated",
                        lineterm=""))
                    print(diff)
            if mismatches:
                print(f"freshness check FAILED: {len(mismatches)} generated file(s) out of sync")
                return 1
            print("freshness check OK: every generated file matches its .ey source")
            return 0

    regenerate(GENERATED_DIR, args.yacas, args.clang_format)

    # The manifest is written only here, on the path that actually regenerated:
    # the commit-time checker (tools/check_freshness.py) reads it to decide
    # whether any codegen input has moved since this run, so blessing the
    # sources from a path that did not regenerate would be a lie the hook
    # cannot see.
    blessed = check_freshness.write_manifest()
    print(f"freshness manifest rewritten: {blessed} source(s)")

    return 0


if __name__ == "__main__":
    sys.exit(main())
