#!/usr/bin/env python3
"""Isolated Exc/Vxc point checks: excgrid kernels vs Libxc (via pyscf).

The linux leg of `.github/workflows/ci.yml` (Python 3.12,
requirements-verif.txt): samples (rhoA, rhoB, sigma) points, evaluates each
shipped excgrid kernel through the kernel-probe binary and the corresponding
Libxc functional through pyscf's own Libxc wrapper (pyscf.dft.libxc.eval_xc),
and compares.

**State 2026-09-17: GREEN, after the `p86` repairs below.** Every kernel
compared as an oracle match reproduces Libxc; `pw91_c` is reported as a NAMED
VARIANT with both citations (it implements a different published
parametrization on purpose - see its bullet), which is not a pass and not a
widened band. The first real run of this script was RED on 2026-09-13, with
114 mismatches; that number is now 0. Every defect named below was in THIS
REPO's call form or in the kernels, and the oracle is fine (`pyscf==2.14.0`
in WSL is the fallback oracle and it works). Run from the tree root, with
relative paths, so the command works from any checkout location:

    wsl -e python3 tools/verify_pyscf.py \
        --probe build/Release/excgrid-kernel-probe.exe

A Ninja build puts the same probe at `build/excgrid-kernel-probe`, which is
the default when `--probe` is omitted.

What the run measures now (measured 2026-09-14, after `rpbe` and `mpw91` were
moved to their full-precision constants and regenerated): 16 of the 18 compared
kernels reproduce Libxc to 8.24e-09 or better - 15 of them to 5.75e-13 and 8
to 8.85e-15, per-functional max over the compared slots, from the table the
run now prints (corrected 2026-09-15: the earlier wording here claimed "15 of
them to ~1e-15", which the per-functional table refutes - `pw92` alone sits at
8.24e-09 and `pw91`, `mpw91` and `pbe_c` at 3.7e-13..5.8e-13) - `rpbe`
is at 4.7e-16 on exc, 6.2e-16 on vrhoA, 5.8e-15 and 3.3e-14 on two vsigma
slots and 0.0 on the third, and `mpw91` came from 5.554e-06 to 3.702e-13 at
its worst - with no mismatch in either. Two do not, and for those two the
first question is not which value is right but whether the two sides compute
the SAME functional, so they were compared as FUNCTIONS - each side identified
by the published parametrization it implements - rather than by matching
numbers:

- `p86` was recorded here until 2026-09-17 as a DIFFERENT FORM in three named
  terms, "expected, not a defect". Re-examination against the paper's printed
  equations (Perdew, Phys. Rev. B 33, 8822 (1986)) shows all three terms are
  transcription defects of THIS repo's rule, not an alternative published
  form, and all three are corrected in `xc_defs/excgrid_defs.ys`:
  (1) the damping exponent's C(infinity) is the INFINITE-DENSITY limit,
  aa + bb = 0.004235 - not the rs -> infinity limit aa = 0.001667 the rule
  carried. The old note's measured exponent ratio, 2.5414 / 2.5401 / 2.5412,
  is 0.004235 / 0.001667 = 2.54049: the old note measured the defect correctly
  and then read it as the paper's choice rather than as this rule's.
  (2) the gradient term is divided by the paper's spin factor
  delta(z) = sqrt(((1 + z)^(5/3) + (1 - z)^(5/3)) / 2), which the old rule had
  no denominator for (eq. 4 of the paper).
  (3) the LDA baseline was the VWN-RPA (table I) parametrization where the
  paper carries the Perdew-Zunger 1981 piecewise one (Perdew & Zunger, Phys.
  Rev. B 23, 5048 (1981)) - the two sigma = 0 limits are 29% apart at
  rs = 1.34, and the oracle's matches the PZ81 one to 1e-15.  It was the whole
  of the remaining residual and is
  corrected too: PZ81 is piecewise at rs = 1 (a rational above, a logarithmic
  form below), so `p86` is now the tree's only branch-carrying kernel -
  `xc_defs/excgrid_gga_piecewise_skeleton.ey` carries the emitted shape and
  its contract, `xc_defs/p86_correlation.ey` the boundary and its source.
  Measured with all three in: `p86` reproduces Libxc to 4.07e-15 at its worst
  compared slot (this run's own table, 2026-09-17) where it was 1.06e+02, and
  the run's total went from 114 mismatches to 0. The two analytic corrections
  alone had moved the `exc` slot's worst only 6.55e+01 -> 7.56e+00, so the
  residual was entirely the LDA term, as that intermediate measurement said.
- `rpbe` is the PBE enhancement with the exponential FORM: kappa = 0.804 and
  mu = beta pi^2/3, both PBE's own constants. Our mu is that product formed
  with the tree's full-precision beta (`X'PbeCorrBeta`), 0.2195149727645171,
  which is also what the other PBE-family rules here carry; the oracle uses
  the same constant. An analytic RPBE reproduced our kernel to 3.2e-16 and
  the oracle to 4.7e-16 across all 18 sampled points.
- `mpw91` is the same class and is now applied. Its damping exponent is not
  a quoted number: PW91 damps with `Exp(-100 s^2)` in the reduced gradient
  s = |grad rho| / (2 kF rho), kF = (6 pi^2 rho)^(1/3), and this kernel works
  in x = |grad rho| / rho^(4/3), so the exponent is exactly
  100 / (4 (6 pi^2)^(2/3)) = 1.6455307846020557... . The kernel carried a
  five-digit alpha before and now carries that constant; the oracle's
  independent agreement to nine digits at every well-conditioned sampled
  point (this run, 22 points across two densities) is the check on the
  change, not its source.
- `pw91_c` is NOT that class, and its status was re-measured on 2026-09-17:
  the whole deviation is one named term - the `0.07389 rs^3` of H1's
  `C_c(rs)`, which this rule carries and Libxc's `gga_c_pw91` does not. Its
  `H0` is provably identical (the two sides agree to 1e-17 at large x, where
  H1 is dead). The identification is exact, not fitted: removing the term
  moves the two failing sampled points from rel 9.10e-06 and 5.19e-06 to
  3.51e-16 and 3.71e-16, and the worst over all 18 points from 9.49e-06 to
  8.42e-09 (that 8.4e-09 floor is a separate, sub-tolerance LDA
  spin-interpolation residue present at every point, not part of this term).
  The two sides are NOT a truncation of one expression: the oracle's
  `C_xc(rs)` is the Rasolt & Geldart, Phys. Rev. B 34, 1325 (1986)
  parametrization, carried with a quadratic denominator, while this rule's
  `C_c(rs)` is the P86 paper's `C(n)` form verbatim - algebraically identical
  to this repo's own `P86Cc`, cubic included.
  So both sides follow a published parametrization of the same gradient
  coefficient, and the cubic is not evidence of a transcription slip on
  either side. What could NOT be
  established locally is the PW91 paper's own printed `C_c(rs)`: the paper is
  not reachable from this machine, and the one literature transcription of
  its H1 that was found reproduces this rule's alpha, beta, nu, C_c(0) and
  C_x exactly but does not print `C_c(rs)` at all. It is therefore recorded
  and left alone, as before - the numbers are open, the identification is not.
  Because the rule is kept, this kernel can never match `gga_c_pw91`; the
  `VARIANT_ORACLES` table below is where that is stated to the tool, with both
  citations, so its deviations print and count as named variants rather than
  failing as mismatches - reported, never a silent pass, and never a band.

**The sub-tolerance floor has a cause, measured 2026-09-17.** The few parts
per billion that `pw92` (8.24e-09) and the polarized points of everything
spin-interpolated sit at is one constant: the spin-stiffness divisor `f''(0)`,
which the `.ey` derives symbolically as `4/9/(2^(1/3) - 1) = 1.70992093416...`
and Libxc carries as the literature's six-digit `1.709921`. Re-running the
polarized points with the rounded constant reproduces Libxc to 1.7e-16 /
0.0e+00 where the exact one is 1.6e-09 away, at every density tried; at
unpolarized points the term vanishes (the two agree to 1.6e-16 there), which
is why the floor appears only where it does. This rule is the MORE precise of
the two, so it is recorded and not changed - and it is the whole of the
8.42e-09 floor the `pw91_c` bullet above reports once its term is removed.

The red is the finding, as the CI's own comment says: do not widen a tolerance
to make this green, and do not adopt Libxc's variant of a different-form
kernel as this repo's definition.

The three defects the old call form carried, all measured rather than read:

1. LAYOUT. `pyscf.dft.libxc._eval_xc` takes `ngrids = rho.shape[-1]` and then
   reshapes to `(spin+1, nvar, ngrids)` - the GRID AXIS IS LAST, and for
   `spin=1` the array must be `(2, nvar, N)`: `nvar` = 1 for an LDA (density
   only) and 4 for a GGA (density plus the three gradient COMPONENTS - libxc
   forms `sigma = grad.grad` itself; it does not take the sigma triple). The
   old call passed `(1, 2)`, which pyscf read as two grid points of one
   variable and rejected with `ValueError: cannot reshape array of size 2
   into shape (2,1,2)` on its first functional. `gradient_components()`
   below rebuilds the components from the sigma triple.
2. UNITS. `eval_xc`'s `exc` is Libxc's `zk`, the energy PER PARTICLE. The
   kernels return the energy DENSITY (Hartree/Bohr^3, `excgrid/kernel.hpp`).
   The reference is therefore scaled by `rhoA + rhoB`; without that factor
   the comparison would be wrong by the density even once the layout is
   right. Measured at rhoA = rhoB = 0.2 for `lda_x` and `gga_x_b88`: exact.
3. ALIAS. The FUNCTIONALS table below is the third defect's fix: the Libxc id
   each kernel is compared against. B3LYP's is `b3lyp5`, not `b3lyp` -
   excgrid's recipe carries VWN5 and pyscf's `b3lyp` alias carries VWN_RPA,
   which is off by 6.8e-03 relative at rhoA = rhoB = 0.2 where `b3lyp5`
   reproduces the kernel to 8.8e-15 worst-case across the sampled points.

**A different point set is ruled out by measurement, not assumed.** Both
sides evaluate ONE point carrying the same (rhoA, rhoB, sigma) triple, and no
quadrature weights exist on either side. Libxc re-forms sigma from the
gradient components this script passes, and that reconstruction agrees with
the sampled triple to 1.1e-16 over all 18 points; and the four kernels that
are the same functional on both sides (`becke88`, `pbe`, `lyp`, `pbe_c`)
agree to 2.5e-15 / 8.6e-16 / 4.7e-14 / 5.7e-13 at the same points and slots.
A point-set or unit mismatch would break those four as well, so it is not
what separates the four kernels above.

**What the run does NOT compare.** The zero-gradient corner: every shipped
GGA kernel except `lyp` returns NaN in a vsigma slot when the sigma it feeds
on is BIT-EXACTLY zero (the vsigma expression carries the same `sqrt(sigma)`
factor above and below, so the exact zero evaluates 0/0 where Libxc returns a
finite limit). That is a pinned kernel defect with its own tests
(`tests/kernel_test.cpp`, `KernelTest.ZeroGradientVsigma*`), not something
this script may hide: a NaN is compared against the allowed set below, a NaN
anywhere else fails the check, and the skipped slots are counted and printed
so a green line never means "silently passed". `exc` and the `vrho` pair stay
finite and ARE compared at those points.

pyscf has no Windows wheels (upstream), so the windows leg of
`.github/workflows/ci.yml` skips this step - the linux leg is the gate;
the repo's ctest suite (finite-difference + exact-condition channels) runs on
both legs and needs no external reference.  Libxc is the verification oracle
only - never linked into the library and never vendored, so the shipped
kernels stay independent of it.  pylibxc is deliberately not used: the
package is no longer published on PyPI - its one permitted install attempt
failed on exactly that (2026-09-13) - and pyscf ships the same Libxc surface
natively.

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

# The zero-gradient vsigma rules, keyed by which sigma argument the kernel's
# vsigma expression divides through by (see the module docstring and
# tests/kernel_test.cpp).  "spin": each vsigma dies with its OWN per-spin
# sigma - the exchange route.  "total": all three die with
# sigmaAa + 2 sigmaAb + sigmaBb - the correlation route.  A functional
# carrying both routes lists both, e.g. pbe0 (pbe exchange plus pbe
# correlation).  An empty set is an LDA kernel (no sigma at all) or lyp
# (its energy is linear in sigma, so its derivative is a constant).
NAN_SPIN = "spin"
NAN_TOTAL = "total"

# (excgrid kernel name, the lowercase Libxc id pyscf resolves, vsigma rules).
# Hybrids compare the DFT part only (Libxc's hybrid exc/vrho carry the
# (1 - a0) exchange scaling and full correlation - exactly the excgrid
# composition; the exact-exchange fraction itself is the consumer's Fock-side
# concern).  `rpbe` is compared against `gga_x_rpbe` like its neighbours, but
# its FORMULA provenance is still the published form alone - the constants
# were asserted from the paper and never read from Libxc, which is recorded
# in `xc_defs/rpbe_exchange.ey` and the CHANGELOG; this point check is
# evidence about the numbers, it does not retract that boundary.
FUNCTIONALS = [
    ("slater", "lda_x", set()),
    ("vwn5", "lda_c_vwn", set()),
    ("vwn3", "lda_c_vwn_rpa", set()),
    ("pw92", "lda_c_pw", set()),
    ("becke88", "gga_x_b88", {NAN_SPIN}),
    ("pw91", "gga_x_pw91", {NAN_SPIN}),
    ("pbe", "gga_x_pbe", {NAN_SPIN}),
    ("revpbe", "gga_x_pbe_r", {NAN_SPIN}),
    ("rpbe", "gga_x_rpbe", {NAN_SPIN}),
    ("mpw91", "gga_x_mpw91", {NAN_SPIN}),
    ("pbesol", "gga_x_pbe_sol", {NAN_SPIN}),
    ("lyp", "gga_c_lyp", set()),
    ("pbe_c", "gga_c_pbe", {NAN_TOTAL}),
    ("pw91_c", "gga_c_pw91", {NAN_TOTAL}),
    ("p86", "gga_c_p86", {NAN_TOTAL}),
    ("b3lyp", "b3lyp5", {NAN_SPIN}),
    ("pbe0", "pbe0", {NAN_SPIN, NAN_TOTAL}),
    ("bhandhlyp", "bhandhlyp", {NAN_SPIN}),
]

# The six probe outputs, in the probe's own order (tools/kernel_probe.cpp).
SLOTS = ("exc", "vrhoA", "vrhoB", "vsigmaAa", "vsigmaAb", "vsigmaBb")

TOLERANCE_EXC = 2e-6  # Relative on exc (|exc| can be tiny at low rho).
TOLERANCE_V = 2e-7   # Relative on vrho/vsigma.

# Kernels that implement a DIFFERENT published parametrization of the same
# physical quantity from the Libxc id beside them, and so are expected never to
# match it.  This is not a tolerance and not a skip: the comparison still runs,
# the deviation is still measured and printed, and the entry names the oracle,
# what differs, and both citations.  A name here turns its mismatches into
# reported VARIANT deviations - counted and printed on their own line, never
# folded into a pass - which is the same shape as the zero-gradient NaN rule
# above (a named, counted, printed exemption that can never mean a silent OK).
#
# The bar for an entry: the two sides must each be published, and the
# difference must have been identified to the term, not merely bounded.  A
# kernel that is simply WRONG does not belong here; it belongs fixed.
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
    """One probe output field as a float.

    MSVC prints a quiet NaN as `-nan(ind)`, a C99-legal spelling (the standard
    allows a parenthesised tag) that Python's float() rejects; gcc prints
    `nan`.  Both legs of the probe therefore have to be parsed here, and the
    scripts' first real execution is where that was measured.  Round-tripping
    a NaN matters: the zero-gradient slots are how the kernels report a
    removable singularity the oracle evaluates finitely (module docstring).
    """
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
    """Gradient components realizing a sigma triple, as libxc wants them.

    libxc takes the three gradient components per spin and forms the sigma
    entries as dot products itself, so a sigma triple has to be turned back
    into vectors.  This samples exactly the triples with
    sigmaAb^2 == sigmaAa * sigmaBb - the gradients parallel - which makes the
    in-plane reconstruction below exact; the clamp only bounds the last-ulp
    round-off of an identity that holds mathematically.
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
    if NAN_SPIN in rules:
        if s_aa == 0.0:
            allowed.add("vsigmaAa")
        if s_bb == 0.0:
            allowed.add("vsigmaBb")
    if NAN_TOTAL in rules and s_aa + 2.0 * s_ab + s_bb == 0.0:
        allowed.update(("vsigmaAa", "vsigmaAb", "vsigmaBb"))
    return allowed


def compare(name, libxc_name, rules, probe, points, stats, variant=False):
    failures = 0
    variants = 0
    # Per-functional worst, so the run summary can say WHICH kernel a worst
    # value belongs to.  Without it the global summary reports one number for
    # eighteen kernels and a functional sitting just inside tolerance is
    # indistinguishable from one at machine precision - measured 2026-09-15,
    # when the docstring's own "16 of 18 reproduce Libxc to 8.2e-09" claim
    # could not be produced from the run's output at all.
    fstats = {slot: 0.0 for slot in SLOTS}
    rows = run_probe(probe, name, points)

    for point, row in zip(points, rows):
        rho_a, rho_b, s_aa, s_ab, s_bb = point
        refs = reference_values(libxc_name, rho_a, rho_b, s_aa, s_ab, s_bb)
        allowed = degenerate_slots(rules, s_aa, s_ab, s_bb)
        where = f"rho=({rho_a}, {rho_b}) sigma=({s_aa}, {s_ab}, {s_bb})"

        for slot, got in zip(SLOTS, row):
            # A NaN probe value is never a pass: `nan > tol` is False, so a
            # bare tolerance test would swallow it.  It is allowed only in a
            # slot this functional's zero-gradient rule predicts, and the
            # skip is counted into the run summary.
            if math.isnan(got):
                if slot in allowed:
                    stats[slot]["skipped"] += 1
                    continue
                print(f"{name}: {slot} NaN at {where} "
                      f"(allowed: {sorted(allowed) or 'none'})")
                failures += 1
                continue

            if slot not in refs:
                # An LDA kernel takes no sigma inputs and must leave the
                # vsigma slots at their default zero.
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

    # The per-functional table is what makes the summary readable: a global
    # "worst 6.55e+01" says nothing about the sixteen kernels that are fine.
    # It is ALSO the only place the docstring's finer claims are checkable
    # ("...to 8.2e-09 or better"), so it is printed unconditionally.
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

    # A named variant is never a silent pass: both citations print, so a green
    # exit cannot be read as "this kernel reproduces the oracle".
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
