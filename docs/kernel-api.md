# The excgrid kernel API — the frozen seam

This document is the **frozen contract** between the excgrid library and its
consumers. The header of record is
[`include/excgrid/kernel.hpp`](../include/excgrid/kernel.hpp); this document is
its specification, and where the two disagree the header is what compiles.

Change procedure: a new version of this document plus a release-tag bump.
Consumers pin a version and move only by an explicit bump.

Two public groups sit outside this document's sections and are documented by
their own headers: the D4 parameter data
([`include/excgrid/d4.hpp`](../include/excgrid/d4.hpp) — the published
parameters, an element-coverage test, and deliberately no energy) and the grid's
geometric derivative interface
([`include/excgrid/grid_derivatives.hpp`](../include/excgrid/grid_derivatives.hpp)).
Neither is part of the contract below.

## 1. Inputs and units

A point's inputs cross as one record (`excgrid::PointInputs`): a value per
**identifier**, plus the `ComponentMask` saying which of them the caller
supplied. The identifiers are the component table's
([`include/excgrid/components.hpp`](../include/excgrid/components.hpp)), numbered
`0` to `31` and named, never positional. Identifiers `0` to `6` are the inputs
below and keep the positions they arrived in; `7` to `31` are reserved with fixed
meanings. **The seven below are active today:**

| Name | Meaning | Unit |
|---|---|---|
| `rhoA`, `rhoB` | spin densities | electrons / Bohr^3 |
| `sigmaAa` = grad(rhoA).grad(rhoA) | gamma_aa | Bohr^-8 |
| `sigmaAb` = grad(rhoA).grad(rhoB) | gamma_ab | Bohr^-8 |
| `sigmaBb` = grad(rhoB).grad(rhoB) | gamma_bb | Bohr^-8 |
| `tauA`, `tauB` | kinetic-energy densities | Bohr^-5 |

The first five are the LDA/GGA kernels' inputs — an LDA kernel reads the two
densities, a GGA kernel adds the three gradient invariants. The two
kinetic-energy densities are what the tau tier reads (section 4) — **zero from
every other shipped functional**, and part of the mask only there.

An identifier the caller leaves out of the mask reads as zero, so a point's
value is a function of its masked inputs alone.

These are exactly Libxc's conventions (`n`, `sigma` and `tau` in the same units,
as surfaced by pyscf's wrapper), so isolated point checks against Libxc are a
direct equality test with no conversion.

## 2. Outputs

The energy density and its derivatives are carried two ways, and they are not
the same type.

`excgrid::XcKernelValue` is what a **generated kernel returns** and what
composition is written in — one named field per derivative:

| Field | Meaning | Unit |
|---|---|---|
| `exc` | exchange-correlation energy **density** e(r) (e_total = integral of exc d^3r) | Hartree / Bohr^3 |
| `vrhoA`, `vrhoB` | de/d rhoA, de/d rhoB | Hartree |
| `vsigmaAa`, `vsigmaAb`, `vsigmaBb` | de/d sigma_* | Hartree * Bohr^5 |
| `vtauA`, `vtauB` | de/d tau_* | Hartree * Bohr^5 — zero from every shipped functional except `tau_x` |

A plain aggregate (public fields, default-initialized to zero), and it supports
the algebra `+`, `+=`, scalar `*`, `*=` — functionals compose by these operators
(the hybrid recipes in [`src/kernels_registry.cpp`](../src/kernels_registry.cpp)
are nothing but such weighted sums).

`excgrid::PointResult` is what a **consumer receives** from a functional's
per-point entry point: the schema version that produced it, the mask the result
is over, `exc`, and `first` — the first derivatives in an array indexed by
`Component`, in identifier order. `FoldIntoResult` is the one mapping between the
two, and the second-derivative tiers carry their own result types beside it
(section 3).

## 3. Kernel signatures and the derivative tiers

```cpp
namespace excgrid {
    using LdaKernel = XcKernelValue (*)(double rhoA, double rhoB);
    using GgaKernel = XcKernelValue (*)(double rhoA, double rhoB,
                                         double sigmaAa, double sigmaAb, double sigmaBb);
    class XcFunctional;   // abstract: RequiredMask(), UsesGradient(), ExchangeFraction(),
                          // EvaluatePoint(...), EvaluatePointWithSecondDerivatives(...),
                          // EvaluatePointMaterialising(...)
    const XcFunctional* FindFunctional(std::string_view name);
    std::span<const std::string_view> FunctionalNames();
    SchemaVersion ContractVersion();
    std::string_view DescribeStatus(KernelStatus status);
}
```

- **LDA kernels** take densities only and return `exc`, `vrhoA`, `vrhoB`
  (`vsigma_*`, `vtau_*` zero).
- **GGA kernels** additionally consume the three sigma inputs and return the three
  `vsigma_*`.
- **The tau tier's kernel is neither of those two types.** It takes the seven
  active components in identifier order and returns the two `vtau_*` as well
  (section 4); it is reached through the registry name like every other
  functional.
- **A functional is evaluated per point through a record, not positionally**:
  `EvaluatePoint(inputs, result)` over `PointInputs` and `PointResult`.
  `RequiredMask()` states the components it reads, `UsesGradient()` whether the
  sigma slots are among them, and `ExchangeFraction()` the exact-exchange
  fraction the consumer routes through its own Fock path.
- **Three derivative tiers cross the contract, and `Request` names them**:
  `kFirstDerivatives` — the energy density and the first derivatives, in
  `PointResult`; `kSecondDerivativeContraction` — those plus
  `PointSecondDerivative`, the second-derivative matrix multiplied by the
  caller's right-hand side (`contracted`, one value per active component per
  right-hand side in identifier order, `rightHandSides` the count supplied);
  `kSecondDerivativeMatrix` — those plus `PointSecondDerivativeMatrix`, the
  matrix itself, upper triangle only, row-major over the active components in
  identifier order, entry (i, j), i <= j, at `i*active - i*(i-1)/2 + (j-i)`. The
  materialised form is a debug path. Each tier is asked for by calling its own
  entry point; the two above the first default to
  `kRefusedUnsupportedCapability`.
- **Order-1 is shipped for every functional; the second-derivative tiers are
  shipped for `tau_x` alone.** Its kernel and the matrix are emitted from one
  symbolic definition, and its contraction is taken from that same matrix, so the
  tier's two entry points cannot disagree. Nothing above them is built: the third
  derivative has no contract type to cross in.
- A second-derivative request is refused with `kRefusedUnsupportedCombination`
  when the right-hand side is not a whole number of active-component vectors (or
  no component is active), and with `kRefusedExhaustedCapacity` beyond
  `kSecondDerivativeCapacity` (16).
- **`Evaluate(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb)` is a temporary adapter** for
  consumers that have not moved to `EvaluatePoint`. It fills the five
  densities/gradient invariants and cannot express the tau inputs, and a refusal
  comes back as a zero-filled `XcKernelValue` with the status lost.
- **Every refusal is named.** `KernelStatus` enumerates them and `DescribeStatus`
  renders one; none of them is answered with zeros, except on that adapter.

## 4. Functional naming and coverage

`FindFunctional` resolves a **name string** (never an enum — enums drift with
repo releases; the consumer's schema carries the string). The shipped set:

LDA: `slater`, `vwn5`, `vwn3`, `pw92`, `svwn` (= slater + vwn5), `spw92` (= slater + pw92).
GGA exchange: `becke88`, `pw91`, `pbe`, `revpbe`, `rpbe`, `mpw91`, `pbesol`.
GGA correlation: `lyp`, `pbe_c`, `pw91_c`, `p86`.
Tau tier (meta-GGA): `tau_x`.
Hybrids (composition + an exact-exchange fraction; the HF part is the CONSUMER's
Fock-side concern — `ExchangeFraction()` states the fraction): `b3lyp`, `pbe0`,
`b3pw91`, `mpw1pw91`, `bhandhlyp`, `b3p86`.

**`tau_x` is the tier's first kernel and the only shipped functional that reads
the kinetic-energy densities.** It is the second-order gradient expansion of
exchange in the iso-orbital (Pauli) variables, per spin channel: with
tau_P = tau - |grad rho|^2 / (8 rho) and
tau_unif = (3/10)(6 pi^2)^(2/3) rho^(5/3),
e = e_x^LSDA(rho) * (1 + (1 - tau_P/tau_unif)/12). Two limits follow from the
form rather than from a fit: the uniform gas is the spin-scaled Slater kernel,
and the slowly varying gas carries the exact 10/81 coefficient of the exchange
gradient expansion. Its mask is all seven active components; of them the formula
reads six — `sigmaAb` is not among them. The bare expansion's
enhancement is unbounded above — the known failure of the second-order form where
tau runs high — so the entry is the tier's shape reference rather than a
production functional. Its evidence is internal, not a Libxc point check: the
finite differences and exact conditions that travel with the tier, and the
hand-written derivation of the same expansion
([`src/meta_gga_reference.cpp`](../src/meta_gga_reference.cpp)) that the
registry's tests cross-check the generated kernel against.

**`pw91_c` and `p86` each differ from the point-check oracle, and both
differences are deliberate and cited.** `pw91_c` implements the PW91
correlation with the gradient coefficient `C(rho)` of the P86 paper (its
eq. 6, cubic `0.07389 rs^3` denominator term included), while the PW91
correlation's own published fit is the M. Rasolt and D. J. W. Geldart,
Phys. Rev. B 34, 1325 (1986) parametrization, whose denominator is
quadratic. Those are two published fits of the same coefficient, and the
difference is that one term and nothing else: removing our cubic reproduces
the oracle's `gga_c_pw91` to 3.5e-16 at the verification points, so neither
side is truncating the other. `p86` is the Perdew 1986 correlation (Phys. Rev. B 33,
8822) on the Perdew–Zunger 1981 LDA piece (Phys. Rev. B 23, 5048), which the
P86 paper specifies; PZ81 is piecewise at `rs = 1`, so `p86` is the tree's only
branch-carrying kernel — `tools/verify_pyscf.py` and
`xc_defs/excgrid_gga_piecewise_skeleton.ey` carry the detail, and the point
check reports `pw91_c`'s difference as a named, cited variant rather than as a
pass.

**VWN3/VWN5 are two separately-named kernels, never conflated.** Every hybrid
that uses VWN states which one in its documentation — **B3LYP uses VWN5** (the
modern majority convention).

| Name | Recipe | ExchangeFraction |
|---|---|---|
| `svwn` | slater + vwn5 | 0 |
| `spw92` | slater + pw92 | 0 |
| `b3lyp` | 0.08 slater + 0.72 becke88 + 0.19 vwn5 + 0.81 lyp | 0.20 |
| `pbe0` | 0.75 pbe + pbe_c | 0.25 |
| `b3pw91` | 0.08 slater + 0.72 becke88 + 0.19 vwn5 + 0.81 pw91_c | 0.20 |
| `mpw1pw91` | 0.75 mpw91 + pw91_c | 0.25 |
| `bhandhlyp` | 0.50 becke88 + 1.00 lyp | 0.50 |
| `b3p86` | 0.08 slater + 0.72 becke88 + 0.19 vwn5 + 0.81 p86 | 0.20 |

**Read the 0.08 as arithmetic, not as a typo.** The B3LYP family is published as
`0.80 E_x^LSDA + 0.72 dE_x^B88`, where `dE_x^B88 = E_x^B88 - E_x^LSDA` is the B88
*correction*. Every kernel in this table is the **full** functional instead
(`becke88` on its own is a usable functional, and it agrees with libxc's
`gga_x_b88` as a whole-SCF energy), so the published 0.72 needs the Slater term
folded in: `0.80 - 0.72 = 0.08`. This is how libxc writes the identical recipe
(its B3LYP resolves to `0.08 LDA_X + 0.72 GGA_X_B88`). The published `0.80`
against a full kernel double-counts `0.72 E_x^LSDA`, which was measured at
**5.953 Ha** on H2O/STO-3G against pyscf's `b3lyp5`. `bhandhlyp` follows the
same conversion and its two LSDA terms cancel outright, leaving
`0.50 becke88`.

## 5. Grid blocks (the other half of the boundary)

The library also owns molecular **block-grid construction** over its own minimal
geometry type (`excgrid::Geometry`, `excgrid::Atom` — plain aggregates: species +
Bohr positions, so a consumer is not obliged to adopt any other library's
molecule type). Output contract ([`include/excgrid/grid.hpp`](../include/excgrid/grid.hpp)):

- A `BlockGrid` is a flat sequence of `Block`s; each `Block` owns `pointCount`,
  `points` (3 * pointCount, Bohr), `weights` (pointCount), and `atomIndex`
  (pointCount — the owning atom, for consumer-side bookkeeping).
- Blocks are **spatially compact** (greedy nearest-neighbour re-batching after
  per-atom construction, ~1024 points per block) and are **trimmed**: points
  whose weighted contribution falls below the trimming threshold are dropped at
  build time.
- The consumer walks blocks, evaluates its AO values per point once, assembles
  rho/sigma per point, calls the frozen kernel API per point, and contracts Vxc
  per point — the block is the parallel unit.
- Per-point **significant-shell screening is the consumer's concern**, not the
  library's (the library has no basis set); the block's spatial compactness is
  what makes a per-block shell union useful.

## 6. D3 dispersion

`excgrid::GrimmeD3` (the energy and its gradient, a `D3Result`) and
`excgrid::GrimmeD3Energy` (the energy alone) over the same `Geometry` type:
closed forms, hand-written — the one energy gradient the library ships, because
gradients are the unit any optimizer needs. Parameters are the caller's (`s6`,
`s8`, and `rs6`/`alpha6` = a1/a2 of the R^6 damping with `rs8`/`alpha8` the same
pair for R^8) per the DFT-D3 (Grimme 2010) zero-damping scheme; `D3Parameters`
is that struct, and `D3Preset(name)` returns the published set of a method
family. An element above the table's range is `kUnsupported` — a refusal, never
an extrapolation. The preset table follows the published parameter data, so it
names method families the kernel registry does not ship (`blyp`, `bp86`, `bpbe`,
`b97d`, `tpss`, `hf`, for instance): the two API surfaces are independent, and a
consumer may pair D3 with a method it brings from elsewhere.

## 7. Stability promise

- The value schema is versioned separately from the library. A `SchemaVersion`
  rides with every result, `ContractVersion()` reports the one this build speaks
  (1.0 today), and two versions exchange values only while their major numbers
  agree (`SchemaVersion::CompatibleWith`). A major change alters an identifier's
  meaning, reorders the component table, exhausts its capacity or changes the
  composition semantics; a minor change activates a reserved identifier or adds a
  functional over reserved ones. Identifiers `0` to `6` keep the positions they
  arrived in.
- Aggregate field names above are API; adding fields is a minor release, changing
  semantics a major one (SemVer).
- Kernel numerics may change only via the regeneration discipline
  ([`tools/regenerate.py`](../tools/regenerate.py); committed generated output;
  CI freshness check) and are pin-tested by the Libxc point checks via pyscf plus
  the finite-difference and exact-condition tests in this repo's own suite.
- No Eigen, no basis sets, no density matrices, no foreign framework types anywhere
  in the public surface — the boundary test (anything needing them belongs to the
  consumer).
