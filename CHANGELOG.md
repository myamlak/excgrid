# Changelog

All notable changes to this project are documented in this file.  The
format follows Keep a Changelog, and the project adheres to Semantic
Versioning.

## [Unreleased]

### Fixed
- **`vsigma` no longer returns NaN at an exactly zero gradient.** Every shipped
  GGA reduced gradient is proportional to `Sqrt(gamma)` (`X'S`, `X'X`, `X'T` in
  `xc_defs/excgrid_defs.ys`), so differentiating one leaves
  `(dF/ds) * (ds/dgamma)`: the first factor carries one `Sqrt(gamma)` because
  the enhancement is even in `s`, the second carries `1/Sqrt(gamma)`.  The
  generator multiplied them into a single fraction and did **not** cancel the
  shared radical, so the emitted quotient was `0/0` whenever the sigma it fed
  on was bit-exactly zero — a removable point, not a divergence: the smallest
  positive subnormal already returned the finite limit.
  The new `ExCancelRadical` in `xc_defs/excgrid_generate.ys` divides the exact
  shared factor out before the CSE, so the emitted quotient is the analytic
  derivative and is defined at the corner.  Nothing is invented there: the
  cancellation defines the one point the `0/0` left undefined and the value at
  `sigma == 0` is bit-identical to the value one representable step away.
  **This fix is PARTIAL: four of the ten affected kernels.**  `pbe`,
  `revpbe`, `rpbe` and `pbesol` now return the limit at an exactly zero sigma.
  The other six still return NaN there, by two distinct routes, and neither is
  reachable from this change:
  - `becke88`, `pw91`, `mpw91` carry a **second singular site of a different
    shape**: `asinh(x) / sqrt(sigma)`, whose numerator vanishes like `x` but is
    not a product carrying the radical, so there is no common *factor* to
    cancel.  Reaching it needs `asinh(x)/x` written as a function of `x^2`, or
    a limit branch.
  - `pbe_c`, `pw91_c`, `p86` carry the same radical inside a **sum**, where the
    numerator and denominator do not hold it the same number of times.  A
    recursive cancellation was built and **rejected on measurement**: it
    returned their `vsigma` exactly 4x too large at every sampled sigma
    (ratio 4.000 from sigma_total 0.001 to 0.1) and
    `KernelTest.GgaFiniteDifference` failed on it, while `exc` and both `vrho`
    channels stayed bit-identical everywhere.  A syntactic factor cancellation
    is invalid at those nodes; it is a correctness trap, not a missing feature.
  - the four kernels with a `pow(<rho-derived>, -2/3)` term (`pbe`, `revpbe`,
    `pbesol`, `pw91`) still return NaN in `vrhoA`/`vrhoB` at a bit-exactly zero
    spin density.  That is a different mechanism — `0 * inf`, not `0 / 0`,
    reached through `rho` rather than sigma — and it is symmetric in the two
    spins, which an earlier test pinned only on the beta side.
  - the kernel-API contract is unchanged; this is a value fix at one input.
  The cancellation is not free: it regroups the emitted quotient, and
  floating-point multiplication is not associative, so finite values move.
  Measured over a 300k-point sweep, 67% of samples move and the largest move is
  7 ulp (1.5e-15 relative), 1-2 ulp at typical points.  `Clamp`, a limit branch
  and demoting the functional names were each rejected: a clamp invents a value
  the expression already computes, a branch leaves the indeterminate quotient
  in the emitted code, and the expression is not wrong away from the corner.
- **The generated kernels emit their banner and include once, not twice.**
  Every file in `generated/` opened with the `Auto-generated file, do not
  modify` banner and `#include "excgrid/kernel.hpp"` twice — the calling
  `.ey` emits both and the LDA/GGA skeletons emitted them again.  The
  duplicate is removed in the skeleton sources; the emitted code is
  otherwise unchanged.

### Changed
- **Provenance statements and licence notices corrected.**  Two constants
  whose codegen headers described them as precision choices are now stated
  as what they are, with closed forms re-derivable from the published
  expressions: `mpw91`'s damping exponent is `100 / (4 (6 pi^2)^(2/3))`
  (PW91's `Exp(-100 s^2)` transformed into `x = |grad rho| / rho^(4/3)`),
  and `rpbe`'s `mu` is the PBE relation `beta pi^2/3` formed with this
  tree's own full-precision `beta`.  No shipped kernel's value changes; this
  is the provenance of constants already released.
- `THIRD_PARTY_NOTICES.md` now records the source licence for the DFT-D3
  parameter data (GPL-1.0-or-later, numeric values only, no code taken),
  states the licence of the maintainer printer `xc_defs/excgrid_cpp_form.ys`
  (LGPL-2.1-or-later, its rule frame derived from Yacas' `c_form.rep`), and
  records the role of the verification oracle (pyscf/Libxc, CI only, not
  vendored, not linked, not present at build or run time).
- Yacas' licence is given as LGPL-2.1-**or-later** throughout, which is the
  grant the vendored binary's own startup banner and the upstream texts both
  state.
- `p86`'s gradient coefficient documents why the printed `1.745` prefactor
  and the printed `10^4 C4` cubic term are carried as printed rather than
  sharpened to the radicals they approximate.

## [0.1.0] - 2026-09-17

### Added
- The excgrid scaffold: CMake project, public headers, tests, CI,
  Doxygen gate, license, and notices.
- The frozen kernel-API contract (`docs/kernel-api.md`).
- Molecular block-grid construction: MHL/Euler-Maclaurin radial,
  Lebedev-Laikov angular, Becke/SSF partition, trimming, spatial
  re-batching.
- The order-1 XC kernel family (15 generated LDA/GGA kernels + 2 LDA
  composites + 6 hybrids) via the vendored-Yacas codegen pipeline
  (`xc_defs/` → `generated/`, `tools/regenerate.py`).
- `rpbe`, the Hammer-Hansen-Norskov 1999 revised-PBE GGA exchange — the
  first functional added through the documented add-a-functional procedure
  (`docs/maintainer-guide.md`), which the exercise corrected. Its arithmetic
  and first derivatives are verified to machine precision against an
  independently hand-written reference (worst relative error 2e-14), and
  **its formula is asserted from the published enhancement factor rather
  than read from Libxc** — no oracle was reachable when it landed. The point
  check against Libxc now runs in CI, so that boundary has a measured
  verdict instead of a pending one: rpbe differed from Libxc by at most
  4.9e-04 relative, and the whole of the difference was the enhancement
  constant — the paper's truncated `mu = 0.21951` against the full-precision
  `0.2195149727645171` that Libxc carries as `MU_PBE` and that this repo's
  own pbe/revpbe rules already used, with an analytic RPBE reproducing the
  kernel to 3.2e-16 and Libxc to 4.7e-16 over all 18 sampled points. The
  constant is now carried at full precision (see Changed), so the numbers are cross-checked and the truncation is gone; the
  form itself is still the published enhancement factor.
- The Grimme D3 zero-damping dispersion energy + analytic gradients
  over the committed DFT-D3 parameter data.
- The verification suite: finite-difference across the shipped set, the
  exact-condition and Vxc-integrates-to-Exc identities (**Slater exchange
  only**, as their own comments say), and the Libxc point checks
  (`tools/verify_pyscf.py`, the CI linux leg).
- The commit-time codegen freshness gate: `tools/check_freshness.py` and the
  `xc_defs/freshness.json` input-hash manifest that `regenerate.py` rewrites
  on a successful run.  It fails the next commit when a codegen input has
  moved since the blessing — milliseconds, no Yacas — while the CI leg's
  `regenerate.py --check` stays the slower proof that the committed generated
  code matches its sources.

### Changed
- **The hybrid recipes carry the folding rule as a checked contract**: each
  hybrid's terms are a named `constexpr` list with a `TermRole` on every term
  and a `static_assert` that the DFT exchange weights sum to
  `1 - exchangeFraction`, and the hybrid constructor refuses a list that
  reaches it without one.  The rule is the one the B3LYP family needed and
  did not have — the published LSDA coefficient multiplied onto a FULL GGA
  kernel double counts the LSDA exchange (measured at 5.953 Ha on `b3lyp`,
  2026-09-13) — so a future entry that ships the same mismatch now fails the
  build instead of converging to a wrong number.  No shipped recipe's weights
  or numbers change; the registry's own suite
  (`RegistryFoldingGuard.EveryExchangePartitionCloses`) checks the same rule
  a second time, from behaviour.
- **`mpw91`'s damping exponent is derived, not quoted.**  It is now
  `1.6455307846` where the kernel carried five digits, and no digit of it is
  a free parameter: PW91 damps with `Exp(-100 s^2)` in the reduced gradient
  `s = |grad rho| / (2 kF rho)` with `kF = (6 pi^2 rho)^(1/3)`, and the
  kernel works in `x = |grad rho| / rho^(4/3)`, so the exponent is exactly
  `100 / (4 (6 pi^2)^(2/3)) = 1.6455307846020557...`.  This moves the
  kernel's numbers at the sampled points — `vsigmaAa` 5.554e-06,
  `vsigmaBb` 4.303e-06, `vrhoA` 4.126e-08, `exc` 8.569e-09 — and the
  accepted consequence is that mpw91 now reproduces the oracle instead of
  sitting 5.6e-06 away from it.
- **`rpbe` carries the PBE constants at full precision.**  Its enhancement
  constant is `mu = 0.2195149727645171` rather than the five-digit `0.21951`:
  RPBE keeps PBE's `kappa` and `mu` and changes only the enhancement's form,
  and `mu` is the PBE relation `beta pi^2/3` formed with this tree's own
  full-precision `beta` (`X'PbeCorrBeta`), which is the same value the other
  PBE-family rules here carry.  This moves the kernel's numbers: against the
  oracle point check the largest movements are `vsigmaBb` 4.874e-04,
  `vsigmaAa` 1.048e-04 and `vrhoB` 1.179e-05, all at the sampled points, and
  the accepted consequence is that rpbe now reproduces the oracle instead of
  sitting 4.9e-04 away from it.
- The generated kernels no longer declare unreachable CSE temporaries:
  `regenerate.py` prunes the temporaries no printed result depends on, so the
  committed output is warning-free under /W4 without suppressions.  The
  emitted arithmetic is unchanged.
- The generated GGA exchange kernels annotate the arguments their functional
  does not read, so a new parameter cannot hide behind an unused-parameter
  warning.
- **`p86` corrected on all three terms, and the codegen gains its first
  branch** (found 2026-09-17, from a red oracle point check on the published
  tree).  Every term the point check had been told to expect as a "different
  form" was this rule's own defect against the paper, not an alternative
  reading: the LDA baseline was the VWN table-I (RPA) parametrization where
  the P86 paper carries the Perdew-Zunger 1981 piecewise one (29% apart at
  rs = 1.34); the damping exponent's `C(infinity)` was the `rs -> infinity`
  limit `0.001667` where the paper's `C(infinity)` is the INFINITE-DENSITY
  limit `0.004235`; and the gradient term was missing the paper's `delta(z)`
  divisor.  PZ81 is piecewise at `rs = 1`, so `p86` is the tree's **first
  branch-carrying kernel**: `xc_defs/excgrid_gga_piecewise_skeleton.ey` is a
  new skeleton emitting two complete branch bodies inside
  `if (cond) { ... } else { ... }`, each in its OWN brace scope (the
  temporary pruner already computes reachability per C++ scope, so the
  branches' CSE tag names may collide), and `excgrid_generate.ys` gains
  `ExPiecewiseKernelGenerate`, which takes the condition and then the two
  expressions - the condition selecting the first.  The printer gains the two
  rules a branch condition needs (`<`, and `Not`, which is how yacas 1.8
  parses every `>=`).  Measured against the same oracle the CI uses: `p86`
  goes from 90 mismatches and a 1.06e+02 worst relative deviation to 0
  mismatches and 4.07e-15, and the run's total from 114 to 0.
- **`pw91_c`'s difference from Libxc is now stated rather than implicit.**
  The kernel implements PW91 correlation with the P86 paper's `C(rho)`
  gradient coefficient (cubic `0.07389 rs^3` denominator term included),
  while the PW91 correlation's own published fit is the Rasolt & Geldart
  (Phys. Rev. B 34, 1325 (1986)) parametrization, whose denominator is
  quadratic.  Both are published fits of the same coefficient and the
  difference is that one term and nothing else - removing it reproduces
  Libxc to 3.5e-16 at the sampled points - so the rule keeps its form and
  `tools/verify_pyscf.py` gains a `VARIANT_ORACLES` entry naming the oracle,
  the term and both citations.  Its 24 deviations now print and count as
  NAMED VARIANTS: reported, with the citations, never a pass, and no
  tolerance touched.  `docs/kernel-api.md` records the same difference for
  consumers.
- **The point check's sub-tolerance floor is identified, not mysterious.**
  The few parts per billion `pw92` sits at (8.24e-09), and the polarized
  points of every spin-interpolated kernel, is one constant: `f''(0)` is
  derived here symbolically as `4/9/(2^(1/3) - 1) = 1.70992093416...`, while
  Libxc divides by the literature's six-digit `1.709921`.  The exact value is
  the more precise of the two, so it is recorded where the constant lives
  (`xc_defs/excgrid_defs.ys`) and not changed.
