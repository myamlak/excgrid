#!/usr/bin/env python3
"""The excgrid codegen driver: Yacas -> committed generated kernels.

Each xc_defs/<name>.ey is expanded by the vendored Yacas, inspected (yacas
exits 0 on an expansion error, so its exit status is not evidence), normalized
to LF and clang-format, and written to generated/<name>.cpp.

Usage:
    python tools/regenerate.py             # regenerate all committed outputs,
                                           # then rewrite xc_defs/freshness.json
    python tools/regenerate.py --check     # regenerate to a temp dir, diff
                                           # against the committed files (the
                                           # deliberate freshness gate)
    python tools/regenerate.py --check --kernel-timeout 0
                                           # the same gate for a tree whose
                                           # committed tiers cost more than the
                                           # default 1800 s per kernel

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
import time
from pathlib import Path

import check_freshness

# (xc_defs source, generated output); a new functional also adds its output to the library sources.
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
    ("tau_x.ey", "tau_x.cpp"),
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

#: Both places tier functions are declared; the tau tier is named in the registry, not the header.
TIER_DECLARATION_SITES = (
    ROOT / "include" / "excgrid" / "kernels.hpp",
    ROOT / "src" / "kernels_registry.cpp",
)
TIER_SUFFIX = "SecondDerivatives"

# Stops a NON-TERMINATING expansion, not a slow healthy one; --kernel-timeout opts a long run in.
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


#: Yacas' own error banners, literals in the pinned binary's message table; yacas exits 0 on these.
YACAS_ERROR_MARKERS = (
    "Error in file",
    "Error reading file",
    "File not found",
    "bad argument number",
    "Invalid argument",
    "Wrong number of arguments",
    "Unknown exception",
)

#: What a captured expansion must carry to be a kernel; it cannot see an error spliced into one value.
KERNEL_SHAPE = (
    re.compile(r"^namespace excgrid", re.MULTILINE),
    re.compile(r"^[ \t]*result\.[A-Za-z]+ =", re.MULTILINE),
)


def inspect_expansion(stdout_text, stderr_text):
    """The reasons a captured expansion is not a kernel; an empty list means it is.

    The child's EXIT STATUS is not evidence: yacas reports an expansion error on
    STDOUT and still exits 0.
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


def error_digest(stdout_text, stderr_text, limit=400):
    """The error lines of a failed child, or the tail of its output when it printed none.

    Bounded: a half-expanded kernel can be megabytes of text.
    """
    lines = [line.strip()
             for line in (stdout_text + "\n" + stderr_text).splitlines()
             if any(marker in line for marker in YACAS_ERROR_MARKERS)]
    if lines:
        return "yacas wrote: " + " | ".join(lines[:3])[:limit]
    tail = (stderr_text.strip() or stdout_text.strip())
    return f"stdout/stderr tail: {tail[-limit:]!r}"


def generate_one(yacas, scripts_root, ey_path, out_path, clang_format=None,
                 timeout=KERNEL_TIMEOUT_SECONDS):
    """One yacas invocation: the `-pc --patchload` console-expansion pattern.

    The kernel is built beside `out_path` and renamed into place only once it is
    complete, inspected, formatted and pruned, so `out_path` is never observable
    half-written and never receives an error string.
    """
    # Yacas mangles backslashes in arguments; forward slashes are the smoke-tested form.
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
            # A nonzero exit does not say which stream holds the reason; error_digest reads both.
            raise SystemExit(
                f"{ey_path.name}: yacas exited {completed.returncode}, so {out_path.name} "
                f"was NOT rewritten and the freshness manifest has NOT been blessed. "
                f"{error_digest(completed.stdout, completed.stderr)}")
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
            f"The committed {out_path.name} is untouched. A slower kernel may be "
            f"healthy, but a larger bound needs a measurement, not a guess; pass "
            f"--kernel-timeout SECONDS (0 = no bound) only for a deliberate long run.")
    except BaseException:
        temporary.unlink(missing_ok=True)
        raise
    os.replace(temporary, out_path)


def normalize_and_format(path, clang_format):
    """LF line endings, then the repository clang-format on the output.

    The config is named explicitly because on the --check path the file lives in
    a temporary directory, where clang-format's directory search would not find it.
    """
    text = path.read_text(encoding="utf-8").replace("\r\n", "\n")
    path.write_text(text, encoding="utf-8", newline="\n")
    if clang_format:
        style = f"file:{ROOT / '.clang-format'}"
        subprocess.run([clang_format, "--style", style, "-i", str(path)], check=True)


# One emitted CSE temporary: `const double C17 = <expr>;`.
DECLARATION = re.compile(r"^(?P<indent>[ \t]*)const double (?P<tag>C\d+) =(?P<rest>.*)$")
# The output assignment a group's declarations feed: a field or an indexed member of an output object.
ASSIGNMENT_START = re.compile(r"^[ \t]*[A-Za-z_][A-Za-z0-9_]*(?:\.[A-Za-z_][A-Za-z0-9_]*|\[[0-9]+\])* =")
TAG = re.compile(r"\bC\d+\b")


def prune_unreachable_temporaries(text):
    """Drop CSE temporaries that nothing printed in their own C++ scope reaches.

    Reachability starts at every output assignment in the SAME C++ scope, because
    that is what the declarations are visible to.  Survivors keep emission order.
    """
    lines = text.split("\n")
    kept = [True] * len(lines)

    # One entry per open brace scope, innermost last; the file itself is scope 0.
    scopes = [{"declarations": [], "roots": []}]
    pending_root = None
    pending_decl = None

    index = 0
    while index < len(lines):
        line = lines[index]

        if pending_decl is not None:
            pending_decl[3].append(line)
            if ";" in line:
                scopes[-1]["declarations"].append(
                    (pending_decl[0], index, pending_decl[1],
                     "\n".join(pending_decl[3]))
                )
                pending_decl = None
        elif pending_root is not None:
            pending_root[1].append(index)
            if ";" in line:
                scopes[-1]["roots"].append("\n".join(lines[i] for i in pending_root[1]))
                pending_root = None
        else:
            declaration = DECLARATION.match(line)
            if declaration is not None:
                if ";" in declaration.group("rest"):
                    scopes[-1]["declarations"].append(
                        (index, index, declaration.group("tag"), declaration.group("rest"))
                    )
                else:
                    # The declaration wraps; it ends at the first line with ';'.
                    pending_decl = (index, declaration.group("tag"), None, [line])
            elif ASSIGNMENT_START.match(line):
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

    # Reachability runs down the chain: a live value makes the values its own right-hand side uses live.
    entries = {tag: rhs for _, _, tag, rhs in scope["declarations"]}
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

    for start, end, tag, _ in scope["declarations"]:
        if tag not in live:
            for line_index in range(start, end + 1):
                kept[line_index] = False


def prune_in_place(path):
    """Apply the reachability prune to a generated file, if it changes anything."""
    text = path.read_text(encoding="utf-8")
    pruned = prune_unreachable_temporaries(text)
    if pruned != text:
        path.write_text(pruned, encoding="utf-8", newline="\n")


def check_tier_declarations(generated_dir):
    """Cross-check the second-derivative tiers' declarations against their definitions.

    A declaration is hand-written while its definition is generated, so a
    disagreement would otherwise reach link time on the registry.

    Returns a list of complaint strings; empty means the two sides agree.
    """
    declaration = re.compile(rf"^void\s+(\w+{TIER_SUFFIX})\s*\(", re.MULTILINE)
    declared = set()
    for site in TIER_DECLARATION_SITES:
        declared.update(declaration.findall(site.read_text(encoding="utf-8")))
    defined = set()
    for _, cpp_name in FUNCTIONALS:
        text = (generated_dir / cpp_name).read_text(encoding="utf-8")
        defined.update(declaration.findall(text))

    where = " or ".join(site.relative_to(ROOT).as_posix() for site in TIER_DECLARATION_SITES)
    complaints = []
    for name in sorted(declared - defined):
        complaints.append(
            f"{name} is declared in {where} but no generated kernel defines it - "
            "either its .ey asks for a tier or the declaration goes")
    for name in sorted(defined - declared):
        complaints.append(
            f"{name} is defined by a generated kernel but not declared in {where}")
    return complaints


def regenerate(output_dir, yacas_arg=None, clang_format_arg=None,
               kernel_timeout=KERNEL_TIMEOUT_SECONDS):
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
        print(f"generating {cpp_name}", flush=True)
        started = time.monotonic()
        try:
            generate_one(yacas, scripts_root, ey_path, out_path, clang_format,
                         kernel_timeout)
        finally:
            # Printed on the failure path too: a timed-out kernel's cost is what the next bound needs.
            print(f"  {cpp_name}: {time.monotonic() - started:.1f}s", flush=True)


def main():
    parser = argparse.ArgumentParser(description="excgrid Yacas codegen driver")
    parser.add_argument("--check", action="store_true",
                        help="regenerate to a temp dir and diff against the committed outputs")
    parser.add_argument("--yacas", help="path to the yacas binary")
    parser.add_argument("--clang-format", help="path to clang-format")
    parser.add_argument("--kernel-timeout", type=float, default=KERNEL_TIMEOUT_SECONDS,
                        metavar="SECONDS",
                        help="per-kernel yacas bound; 0 means no bound (default: %(default)s)")
    args = parser.parse_args()

    # subprocess treats timeout=0 as "kill immediately", not "no bound".
    kernel_timeout = None if args.kernel_timeout == 0 else args.kernel_timeout

    if args.check:
        with tempfile.TemporaryDirectory(prefix="excgrid-codegen-") as tmp:
            tmp_dir = Path(tmp)
            regenerate(tmp_dir, args.yacas, args.clang_format, kernel_timeout)
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
            tier_complaints = check_tier_declarations(tmp_dir)
            for complaint in tier_complaints:
                print(complaint)
            if mismatches or tier_complaints:
                print(f"freshness check FAILED: {len(mismatches)} generated file(s) out of sync, "
                      f"{len(tier_complaints)} tier declaration(s) unaccounted for")
                return 1
            print("freshness check OK: every generated file matches its .ey source, and every "
                  "declared tier has a definition")
            return 0

    regenerate(GENERATED_DIR, args.yacas, args.clang_format, kernel_timeout)

    # Before the manifest: a tree whose tiers do not link has not earned a blessing.
    tier_complaints = check_tier_declarations(GENERATED_DIR)
    for complaint in tier_complaints:
        print(complaint)
    if tier_complaints:
        print(f"regeneration wrote generated/, but {len(tier_complaints)} tier declaration(s) "
              "are unaccounted for; the manifest is NOT rewritten")
        return 1

    # Only the path that regenerated may bless; check_freshness.py reads this at commit time.
    blessed = check_freshness.write_manifest()
    print(f"freshness manifest rewritten: {blessed} source(s)")

    return 0


if __name__ == "__main__":
    sys.exit(main())
