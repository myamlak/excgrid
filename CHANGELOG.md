# Changelog

All notable changes to this project are documented in this file.  The
format follows Keep a Changelog, and the project adheres to Semantic
Versioning.

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
- **`mpw91` moved to full precision**: its damping exponent is now
  `1.6455307846` rather than the published form's truncated `1.6455` (decided
  2026-09-14, the same transcription-precision class as `rpbe`; the
  value was solved out of the reference's own numbers, identically at every
  well-conditioned sample point, and the solve's inability to resolve the two
  values at spin-polarised points is recorded in the verification notes).  This moves the kernel's numbers at the sampled points —
  `vsigmaAa` 5.554e-06, `vsigmaBb` 4.303e-06, `vrhoA` 4.126e-08, `exc`
  8.569e-09 — and the accepted consequence is that mpw91 now reproduces the
  oracle instead of sitting 5.6e-06 away from it.
- **`rpbe` moved to full precision**: its enhancement constant is now
  `mu = 0.2195149727645171` rather than the published spelling's truncated
  `0.21951` (decided 2026-09-14 — the truncation was a transcription
  precision choice, not a distinct variant, and Libxc's kernel is the same
  expression with `mu = MU_PBE`).  This moves the kernel's numbers: against
  the Libxc point check the largest movements are `vsigmaBb` 4.874e-04,
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
  branch** (decided 2026-09-17, from a red Libxc point check on the published
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
  where Libxc's `gga_c_pw91` follows the fit its own source attributes to
  Rasolt & Geldart (Phys. Rev. B 34, 1325 (1986)) with a quadratic
  denominator.  Both are published fits of the same coefficient and the
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
