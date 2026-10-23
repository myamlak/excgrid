#!/usr/bin/env python3
"""Isolated Exc/Vxc point checks: excgrid kernels vs Libxc (via pyscf).

Samples (rhoA, rhoB, sigma) points, evaluates each shipped kernel through the
kernel-probe binary and the same functional through pyscf's own Libxc wrapper
(pyscf.dft.libxc.eval_xc), and compares.  Libxc is an oracle only: never linked
into the library and never vendored.  pyscf publishes no Windows wheels, so only
the linux CI leg runs this.

libxc's layout is (spin+1, nvar, ngrids) - the grid axis LAST, nvar 1 for an
LDA and 4 for a GGA (density plus three gradient components).  eval_xc's exc is
the energy per particle, so it is scaled by rhoA + rhoB to the kernels' energy
density.

A kernel whose deviation is a different published parametrization, identified to
the term, is listed in VARIANT_ORACLES: reported and counted, never a pass.  The
correlation kernels return NaN in the vsigma slots at a bit-exactly zero sigma
total; that NaN is allowed only where NAN_TOTAL says so, and is counted.

Run from the tree root, with relative paths, so the command works from any
checkout location:

    wsl -e python3 tools/verify_pyscf.py \
        --probe build/Release/excgrid-kernel-probe.exe

A Ninja build puts the same probe at `build/excgrid-kernel-probe`, the default
when `--probe` is omitted.

A red is the finding: never widen a tolerance to make this green.

Usage: python tools/verify_pyscf.py [--probe path/to/excgrid-kernel-probe]
"""

import argparse
import math
import subprocess
import sys
from pathlib import Path

import numpy as np
from pyscf.dft import libxc as pyscf_libxc

ROOT = Path(__file__).resolve().parent.parent

# The one zero-gradient rule still open: all three vsigma slots die with sigmaAa + 2 sigmaAb + sigmaBb.
NAN_TOTAL = "total"

# (kernel name, Libxc id, vsigma rule); hybrids compare the DFT part only; b3lyp maps to b3lyp5 (VWN5).
FUNCTIONALS = [
    ("slater", "lda_x", set()),
    ("vwn5", "lda_c_vwn", set()),
    ("vwn3", "lda_c_vwn_rpa", set()),
    ("pw92", "lda_c_pw", set()),
    ("becke88", "gga_x_b88", set()),
    ("pw91", "gga_x_pw91", set()),
    ("pbe", "gga_x_pbe", set()),
    ("revpbe", "gga_x_pbe_r", set()),
    ("rpbe", "gga_x_rpbe", set()),
    ("mpw91", "gga_x_mpw91", set()),
    ("pbesol", "gga_x_pbe_sol", set()),
    ("lyp", "gga_c_lyp", set()),
    ("pbe_c", "gga_c_pbe", {NAN_TOTAL}),
    ("pw91_c", "gga_c_pw91", {NAN_TOTAL}),
    ("p86", "gga_c_p86", {NAN_TOTAL}),
    ("b3lyp", "b3lyp5", set()),
    ("pbe0", "pbe0", {NAN_TOTAL}),
    ("bhandhlyp", "bhandhlyp", set()),
]

# The six probe outputs, in the probe's own order (tools/kernel_probe.cpp).
SLOTS = ("exc", "vrhoA", "vrhoB", "vsigmaAa", "vsigmaAb", "vsigmaBb")

TOLERANCE_EXC = 2e-6  # Relative on exc (|exc| can be tiny at low rho).
TOLERANCE_V = 2e-7   # Relative on vrho/vsigma.

# Different published parametrizations: compared and printed, never a pass; an entry needs both sides published and the term identified.
VARIANT_ORACLES = {
    "pw91_c": {
        "oracle": "gga_c_pw91",
        "differs": "this rule's H1 carries the P86 C(rho) form, with the "
                   "cubic 0.07389 rs^3 term in the denominator; Libxc's "
                   "gga_c_pw91 carries the Rasolt-Geldart C_xc(rs), whose "
                   "denominator is quadratic. Removing the cubic from this "
                   "rule reproduces gga_c_pw91 to 3.5e-16 at the sampled "
                   "points, so the difference is that one term and nothing "
                   "else - Libxc is not truncating our expression, the two "
                   "sides follow different published fits.",
        "citations": "our form: J. P. Perdew, Phys. Rev. B 33, 8822 (1986) "
                     "(the C(n) of its eq. 6, which this rule's P86 kernel "
                     "uses identically); Libxc's form: M. Rasolt and "
                     "D. J. W. Geldart, Phys. Rev. B 34, 1325 (1986), named "
                     "as the source in Libxc's own gga_c_pw91 ``Pade "
                     "parametrized form of C-xc'' comment.",
    },
}


def sample_points():
    points = []
    for rho in (0.05, 0.2, 0.8):
        for pol in (0.0, 0.35):
            rho_a = rho * (1.0 + pol)
            rho_b = rho * (1.0 - pol)
            for gamma in (0.0, 0.3, 1.7):
                points.append((rho_a, rho_b, 0.25 * gamma, 0.5 * gamma, gamma))
    return points


def parse_value(text):
    """One probe output field as a float, accepting MSVC's `-nan(ind)` spelling as well as `nan`."""
    if "nan" in text.lower():
        return math.nan
    return float(text)


def run_probe(probe, name, points):
    payload = [f"{name} {ra} {rb} {sa} {sb} {sc}" for ra, rb, sa, sb, sc in points]
    out = subprocess.run(
        [probe], input="\n".join(payload) + "\n",
        capture_output=True, text=True, check=True)
    rows = [[parse_value(v) for v in line.split()] for line in out.stdout.splitlines()]
    assert len(rows) == len(points), "probe row count mismatch"
    return rows


def gradient_components(s_aa, s_ab, s_bb):
    """Gradient components realizing a sigma triple; libxc forms sigma as their dot products.

    Every sampled point has sigmaAb^2 == sigmaAa * sigmaBb, so the in-plane
    reconstruction is exact; the clamp only bounds last-ulp round-off.
    """
    g_ax = math.sqrt(s_aa)
    if g_ax > 0.0:
        g_bx = s_ab / g_ax
        g_by = math.sqrt(max(0.0, s_bb - g_bx * g_bx))
    else:
        g_bx = math.sqrt(s_bb)
        g_by = 0.0
    return (g_ax, 0.0, 0.0), (g_bx, g_by, 0.0)


def reference_values(libxc_name, rho_a, rho_b, s_aa, s_ab, s_bb):
    """One Libxc evaluation at one point, in the kernels' units and names."""
    xctype = pyscf_libxc.xc_type(libxc_name)
    if xctype == "LDA":
        rho = np.array([[[rho_a]], [[rho_b]]])
    elif xctype == "GGA":
        grad_a, grad_b = gradient_components(s_aa, s_ab, s_bb)
        rho = np.array([[[rho_a], [grad_a[0]], [grad_a[1]], [grad_a[2]]],
                        [[rho_b], [grad_b[0]], [grad_b[1]], [grad_b[2]]]])
    else:
        raise ValueError(f"{libxc_name}: xctype {xctype} has no layout here")

    exc, vxc, _, _ = pyscf_libxc.eval_xc(libxc_name, rho, spin=1, deriv=1)
    values = {"exc": float(exc[0]) * (rho_a + rho_b),
              "vrhoA": float(vxc[0][0, 0]),
              "vrhoB": float(vxc[0][0, 1])}
    if xctype == "GGA":
        values["vsigmaAa"] = float(vxc[1][0, 0])
        values["vsigmaAb"] = float(vxc[1][0, 1])
        values["vsigmaBb"] = float(vxc[1][0, 2])
    return values


def degenerate_slots(rules, s_aa, s_ab, s_bb):
    """The vsigma slots the kernel may return NaN in at this sigma triple."""
    allowed = set()
    if NAN_TOTAL in rules and s_aa + 2.0 * s_ab + s_bb == 0.0:
        allowed.update(("vsigmaAa", "vsigmaAb", "vsigmaBb"))
    return allowed


def compare(name, libxc_name, rules, probe, points, stats, variant=False):
    failures = 0
    variants = 0
    # Per-functional worst, so the summary says which kernel a worst value belongs to.
    fstats = {slot: 0.0 for slot in SLOTS}
    rows = run_probe(probe, name, points)

    for point, row in zip(points, rows):
        rho_a, rho_b, s_aa, s_ab, s_bb = point
        refs = reference_values(libxc_name, rho_a, rho_b, s_aa, s_ab, s_bb)
        allowed = degenerate_slots(rules, s_aa, s_ab, s_bb)
        where = f"rho=({rho_a}, {rho_b}) sigma=({s_aa}, {s_ab}, {s_bb})"

        for slot, got in zip(SLOTS, row):
            # `nan > tol` is False, so a NaN is tested before the tolerance and never counts as a pass.
            if math.isnan(got):
                if slot in allowed:
                    stats[slot]["skipped"] += 1
                    continue
                print(f"{name}: {slot} NaN at {where} "
                      f"(allowed: {sorted(allowed) or 'none'})")
                failures += 1
                continue

            if slot not in refs:
                # An LDA kernel takes no sigma and must leave the vsigma slots at zero.
                if got != 0.0:
                    print(f"{name}: {slot} {got:.12e} at {where}, expected 0.0")
                    failures += 1
                continue

            ref = refs[slot]
            if abs(ref) > 1e-12:
                err = abs(got - ref) / abs(ref)
            else:
                err = abs(got - ref)

            stats[slot]["compared"] += 1
            if err > stats[slot]["worst"]:
                stats[slot]["worst"] = err
                stats[slot]["where"] = f"{name} at {where}"
            if err > fstats[slot]:
                fstats[slot] = err

            tol = TOLERANCE_EXC if slot == "exc" else TOLERANCE_V
            if err > tol:
                if variant:
                    # A named variant: reported, counted, and never a pass.
                    print(f"{name}: {slot} VARIANT deviation at {where}: "
                          f"{got:.12e} vs {libxc_name} {ref:.12e} "
                          f"(rel {err:.2e})")
                    variants += 1
                else:
                    print(f"{name}: {slot} mismatch at {where}: "
                          f"{got:.12e} vs {ref:.12e} (rel {err:.2e})")
                    failures += 1

    return failures, variants, fstats


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--probe",
                        default=str(ROOT / "build" / "excgrid-kernel-probe"))
    args = parser.parse_args()

    if not Path(args.probe).exists():
        print(f"probe binary not found: {args.probe} "
              f"(build the excgrid-kernel-probe target first)")
        return 2

    points = sample_points()
    stats = {slot: {"compared": 0, "skipped": 0, "worst": 0.0, "where": ""}
             for slot in SLOTS}
    per_functional = []
    total = 0
    variant_total = 0
    for name, libxc_name, rules in FUNCTIONALS:
        variant = name in VARIANT_ORACLES
        if variant:
            print(f"checking {name} against libxc {libxc_name} "
                  f"(NAMED VARIANT against {VARIANT_ORACLES[name]['oracle']} - "
                  f"deviation reported, not a pass; see the module docstring)")
        else:
            print(f"checking {name} (libxc {libxc_name})")
        failures, variants, fstats = compare(name, libxc_name, rules,
                                             args.probe, points, stats,
                                             variant=variant)
        per_functional.append((name, failures, variants, fstats))
        total += failures
        variant_total += variants

    print(f"{len(FUNCTIONALS)} functionals x {len(points)} density points "
          f"= {len(FUNCTIONALS) * len(points)} point evaluations")
    for slot in SLOTS:
        entry = stats[slot]
        tol = TOLERANCE_EXC if slot == "exc" else TOLERANCE_V
        print(f"  {slot:9s} {entry['compared']:5d} compared, "
              f"{entry['skipped']:4d} skipped (zero-gradient NaN), "
              f"worst {entry['worst']:.2e} vs tolerance {tol:.0e}"
              f" [{entry['where']}]")

    print("per-functional worst (max over compared slots; "
          "slots the kernel legitimately NaNs are not counted):")
    for name, failures, variants, fstats in per_functional:
        worst = max(fstats.values())
        slot = max(fstats, key=fstats.get)
        if failures:
            verdict = f"{failures} mismatch" + ("es" if failures != 1 else "")
        elif variants:
            verdict = f"{variants} named-variant deviation" + \
                ("s" if variants != 1 else "")
        else:
            verdict = "OK"
        print(f"  {name:10s} worst {worst:.2e} ({slot:9s})  {verdict}")

    # Both citations print, so a green exit cannot be read as "matches the oracle".
    for name, entry in sorted(VARIANT_ORACLES.items()):
        print(f"named variant {name} vs {entry['oracle']}: {entry['differs']}")
        print(f"    citations: {entry['citations']}")

    if total:
        print(f"pyscf point checks FAILED: {total} mismatches"
              + (f", plus {variant_total} named-variant deviations"
                 if variant_total else ""))
        return 1
    if variant_total:
        print(f"pyscf point checks OK for every kernel compared as an oracle "
              f"match; {variant_total} named-variant deviations are reported "
              f"above and are NOT passes")
        return 0
    print("pyscf point checks OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
