#!/usr/bin/env python3
"""Self-test for tools/regenerate.py: the guard that refuses a failed expansion.

WHY THIS FILE EXISTS.  The defect this covers was found on 2026-09-13 by
walking the add-a-functional guide, and the GUIDE was fixed - it now says a
zero exit from `regenerate.py` is not evidence that the step worked.  The CODE
was not: `generate_one()` piped yacas' stdout into `generated/<name>.cpp` and
trusted only the exit status, so an expansion error REPLACED a committed kernel
(errors go to stdout, and yacas exits 0 on them) and the run then rewrote
`xc_defs/freshness.json`, blessing the tree it had just broken.  A warning in a
document is a warning nobody's code enforces, which is exactly how it survived
one fix; this file is the enforcement.

The checks, and the direction each one protects:

1. HEALTHY CONTROL (the vendored yacas, a real expansion): `generate_one()`
   writes a file carrying the kernel shape and does NOT refuse.  A guard that
   refused everything would satisfy 2 and 3, so this is the check that keeps
   the guard from being a wall.
2. THE ORIGINAL SILENT REWRITE, LIVE - a `scripts_root` missing the defs
   directory (yacas: `Error in file ... File not found`, 472 bytes on stdout,
   exit 0): `generate_one()` raises SystemExit, the target file is byte-identical
   to what it was, no `.partial` is left behind, and the message names the
   kernel and quotes the error.
3. THE SAME, for the other reproduced exit-0 failure - a `.ey` calling a rule
   that does not exist (yacas: `Error in file ... bad argument number 1`, 1087
   bytes, exit 0).  Distinct from 2 because the output is a PLAUSIBLE kernel
   with the error text spliced into one assignment: no shape check can see it,
   only the marker scan can.
4. THE PREDICATE, without yacas: `inspect_expansion()` on the two payloads
   captured above returns a finding naming the marker, and on a captured
   healthy payload returns none.  This is the half that runs on a machine with
   no vendored binary, and the half that catches a marker list edited into a
   typo.

Exit status: 0 every check passed, 1 a check failed, 2 the vendored yacas is
absent so checks 1-3 CANNOT RUN - which is a third answer, not a pass, and it
is what keeps this file from reporting a green it did not measure.

Run: python excgrid/tools/regenerate_test.py
"""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import regenerate  # noqa: E402  (the tool under test, beside this file)

SENTINEL = b"// a committed kernel that must survive a failed regeneration\n"

#: The two exit-0 failure payloads, captured from the pinned yacas on
#: 2026-09-15 by running `generate_one`'s own command line by hand (the byte
#: counts are the ones on stdout, and the exit status was 0 both times).
FAILED_PAYLOADS = (
    ("missing defs directory", "Error in file", 472, """// excgrid codegen source, a comment header
// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

Error in file xc_defs/slater_exchange.ey
excgrid_defs.ys(1) : File not found
"""),
    ("undefined rule", "bad argument number", 1087, """// probe
// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

namespace excgrid {

XcKernelValue NoSuch(double rhoA, double rhoB) {
    XcKernelValue result;

        result.exc = Error in file broken.ey
String(1) : Invalid argument

In function "WriteString" :
bad argument number 1 (counting from 1)
The offending argument ExCppForm(e) evaluated to ExCppForm(X'NoSuch(rhoA,rhoB))
    return result;
}

} // namespace excgrid
"""),
)

#: A captured HEALTHY payload (the slater expansion, same day, 1805 bytes).
HEALTHY_PAYLOAD = """// excgrid codegen source: the Slater (Dirac) LDA exchange functional.

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

namespace excgrid {

XcKernelValue SlaterExchange(double rhoA, double rhoB) {
    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        result.exc = 0.;
    }
    else
    {
        result.exc = - 0.9305257363491 * std::pow(rhoA, 4. / 3.);
    }
    result.vrhoA = - 3.7221029453964 * std::pow(rhoA, 1. / 3.) / 3.;
    return result;
}

} // namespace excgrid
"""


def _yacas_and_root():
    """(yacas path, scripts_root) as regenerate.regenerate() builds them, or None."""
    try:
        yacas = regenerate.find_tool(None, None, [str(p) for p in regenerate.YACAS_CANDIDATES],
                                     "yacas")
        scripts_root = regenerate.DEFS_DIR.as_posix() + ";" + regenerate.yacas_scripts_dir(yacas)
    except SystemExit as exc:
        print(f"  the vendored yacas is not usable here: {exc}")
        return None
    return yacas, scripts_root


def _drive(generate, yacas, scripts_root, ey_source, target):
    """Run generate_one on `target` and return (refused, message) or (False, '')."""
    try:
        generate(yacas, scripts_root, ey_source, target, clang_format=None, timeout=120)
    except SystemExit as exc:
        return True, str(exc)
    return False, ""


def _run():
    failures = []
    checks = 0

    # 4. THE PREDICATE, yacas-free - always runs.
    checks += 1
    for label, marker, _, payload in FAILED_PAYLOADS:
        findings = regenerate.inspect_expansion(payload, "")
        if not findings:
            failures.append(f"4. PREDICATE: the {label} payload (exit 0) was accepted as a kernel")
        elif not any(marker in finding for finding in findings):
            failures.append(f"4. PREDICATE: the {label} payload's finding names no {marker!r}: "
                            f"{findings}")
    if regenerate.inspect_expansion(HEALTHY_PAYLOAD, ""):
        failures.append("4. PREDICATE: the healthy payload was refused: "
                        f"{regenerate.inspect_expansion(HEALTHY_PAYLOAD, '')}")
    if not regenerate.inspect_expansion("", ""):
        failures.append("4. PREDICATE: EMPTY output was accepted as a kernel")

    live = _yacas_and_root()
    if live is None:
        print("regenerate self-test: CANNOT STATE - the vendored yacas "
              "(tools/yacas/bin/yacas.exe) is not present, so checks 1-3 did not run "
              f"({checks} of 4 checks run, {len(failures)} failed)")
        return 2
    yacas, scripts_root = live

    with tempfile.TemporaryDirectory(prefix="excgrid-regen-test-") as tmp:
        tmp_dir = Path(tmp)

        # 1. HEALTHY CONTROL.
        checks += 1
        healthy_target = tmp_dir / "slater_exchange.cpp"
        refused, message = _drive(regenerate.generate_one, yacas, scripts_root,
                                  regenerate.DEFS_DIR / "slater_exchange.ey", healthy_target)
        if refused:
            failures.append(f"1. HEALTHY: the real expansion was refused: {message}")
        elif not healthy_target.exists():
            failures.append("1. HEALTHY: nothing was written")
        elif healthy_target.stat().st_size < 1024:
            failures.append(f"1. HEALTHY: the kernel is {healthy_target.stat().st_size} "
                            "bytes, which is not a kernel")

        # 2. and 3. THE ORIGINAL SILENT REWRITE, both reproduced exit-0 failures.
        broken_ey = tmp_dir / "undefined_rule.ey"
        broken_ey.write_text(
            '// probe\n<?\n    Builtin\'Precision\'Set(16);\n    Use("excgrid_defs.ys");\n'
            '    Use("excgrid_generate.ys");\n\n    e := X\'NoSuchFunctional(rhoA, rhoB);\n\n'
            '    ExKernelGenerate("NoSuch", e, rhoA, rhoB);\n?>\n',
            encoding="utf-8", newline="\n")
        cases = (
            ("2. ERROR-WITH-EXIT-0 (missing defs directory)",
             # scripts_root WITHOUT the defs dir: yacas cannot load the rules.
             regenerate.yacas_scripts_dir(yacas),
             regenerate.DEFS_DIR / "slater_exchange.ey",
             "Error in file"),
            ("3. ERROR-WITH-EXIT-0 (undefined rule)",
             scripts_root, broken_ey, "bad argument number"),
        )
        for label, root, ey_source, marker in cases:
            checks += 1
            target = tmp_dir / f"case{checks}.cpp"
            target.write_bytes(SENTINEL)
            refused, message = _drive(regenerate.generate_one, yacas, root, ey_source, target)
            if not refused:
                failures.append(f"{label}: generate_one() ACCEPTED it - the target is now "
                                f"{target.stat().st_size} bytes and holds the error string "
                                f"({target.read_bytes()[:60]!r})")
                continue
            if target.read_bytes() != SENTINEL:
                failures.append(f"{label}: refused, but the target was still rewritten")
            if marker not in message:
                failures.append(f"{label}: the refusal does not quote {marker!r}: {message}")
            if ey_source.name not in message:
                failures.append(f"{label}: the refusal does not name {ey_source.name}")
            if list(tmp_dir.glob("*.partial")):
                failures.append(f"{label}: a .partial file was left behind")

    if failures:
        print("regenerate self-test FAILED:")
        for line in failures:
            print(f"  {line}")
        return 1
    print(f"regenerate self-test: {checks}/{checks} checks pass "
          "(healthy-control, error-with-exit-0 x2, predicate)")
    return 0


if __name__ == "__main__":
    sys.exit(_run())
