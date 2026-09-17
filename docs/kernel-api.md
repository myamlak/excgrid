# The excgrid kernel API — the frozen seam

This document is the **frozen contract** between the excgrid library (grid +
XC kernels + D3) and its consumers. The header of record is
[`include/excgrid/kernel.hpp`](../include/excgrid/kernel.hpp); this document is
its specification, and where the two disagree the header is what compiles.

Change procedure: a new version of this document plus a release-tag bump.
Consumers pin a version and move only by an explicit bump.

## 1. Inputs and units

Every per-point kernel consumes the spin-resolved density inputs

| Name | Meaning | Unit |
|---|---|---|
| `rhoA`, `rhoB` | spin densities | electrons / Bohr^3 |
| `sigmaAa` = grad(rhoA).grad(rhoA) | gamma_aa | Bohr^-8 |
| `sigmaAb` = grad(rhoA).grad(rhoB) | gamma_ab | Bohr^-8 |
| `sigmaBb` = grad(rhoB).grad(rhoB) | gamma_bb | Bohr^-8 |
| `tauA`, `tauB` | kinetic-energy densities | **reserved** — accepted by the struct, zero from every shipped LDA/GGA kernel |

These are exactly Libxc's conventions (`n`, `sigma` in the same units, as
surfaced by pyscf's wrapper), so isolated point checks against Libxc are a
direct equality test with no conversion.

## 2. Outputs

One struct per evaluation (`excgrid::XcKernelValue`):

| Field | Meaning | Unit |
|---|---|---|
| `exc` | exchange-correlation energy **density** e(r) (e_total = integral of exc d^3r) | Hartree / Bohr^3 |
| `vrhoA`, `vrhoB` | de/d rhoA, de/d rhoB | Hartree |
| `vsigmaAa`, `vsigmaAb`, `vsigmaBb` | de/d sigma_* | Hartree * Bohr^5 |
| `vtauA`, `vtauB` | de/d tau_* | **reserved** — always 0.0 from LDA/GGA kernels |

The struct is a plain aggregate (public fields, default-initialized to zero) and
supports the algebra `+`, `+=`, scalar `*`, `*=` — functionals compose by these
operators (the hybrid recipes in
[`src/kernels_registry.cpp`](../src/kernels_registry.cpp) are nothing but such
weighted sums).

## 3. Kernel signatures (order-1, the only shipped tier)

```cpp
namespace excgrid {
    using LdaKernel = XcKernelValue (*)(double rhoA, double rhoB);
    using GgaKernel = XcKernelValue (*)(double rhoA, double rhoB,
                                         double sigmaAa, double sigmaAb, double sigmaBb);
    class XcFunctional;   // abstract: UsesGradient(), Evaluate(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb), ExchangeFraction()
    const XcFunctional* FindFunctional(std::string_view name);
    std::span<const std::string_view> FunctionalNames();
}
```

- **LDA kernels** take densities only and return `exc`, `vrhoA`, `vrhoB`
  (`vsigma_*`, `vtau_*` zero).
- **GGA kernels** additionally consume the three sigma inputs and return the three
  `vsigma_*`.
- **Order-1 is the shipped tier**: plain Vxc assembly consumes first
  derivatives only. Second/third derivative tiers exist solely for
  energy-gradient machinery; they are a documented, mechanical generator
  extension and are not shipped.
- **Tau slots are reserved, never shipped** until a meta-GGA tier lands:
  the struct and the abstract `Evaluate` always carry them so the extension is
  additive, not a breaking change.

## 4. Functional naming and coverage

`FindFunctional` resolves a **name string** (never an enum — enums drift with
repo releases; the consumer's schema carries the string). The shipped set:

LDA: `slater`, `vwn5`, `vwn3`, `pw92`, `svwn` (= slater + vwn5), `spw92` (= slater + pw92).
GGA exchange: `becke88`, `pw91`, `pbe`, `revpbe`, `rpbe`, `mpw91`, `pbesol`.
GGA correlation: `lyp`, `pbe_c`, `pw91_c`, `p86`.
Hybrids (composition + an exact-exchange fraction; the HF part is the CONSUMER's
Fock-side concern — `ExchangeFraction()` states the fraction): `b3lyp`, `pbe0`,
`b3pw91`, `mpw1pw91`, `bhandhlyp`, `b3p86`.

**`pw91_c` and `p86` each carry a cited provenance difference from Libxc, and
both are deliberate.** `pw91_c` implements the PW91 correlation with the
gradient coefficient `C(rho)` of the P86 paper (its eq. 6, cubic
`0.07389 rs^3` denominator term included), where Libxc's `gga_c_pw91` follows
the parametrization Libxc's own source attributes to M. Rasolt and
D. J. W. Geldart, Phys. Rev. B 34, 1325 (1986), whose denominator is
quadratic. Those are two published fits of the same coefficient, and the
difference is that one term and nothing else: removing our cubic reproduces
`gga_c_pw91` to 3.5e-16 at the verification points, so neither side is
truncating the other. `p86` is the Perdew 1986 correlation (Phys. Rev. B 33,
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

`excgrid::GrimmeD3` over the same `Geometry` type: energy + analytic coordinate
gradients (closed forms, hand-written — the one coordinate-gradient tier shipped,
because gradients are the unit any optimizer needs). Parameters are the caller's
(`s6`, `s8`, `rs6` = a1, `alpha6` = a2) per the DFT-D3 (Grimme 2010) zero-damping
scheme; a `D3Parameters` struct carries the standard presets. The preset
table follows the published parameter data, so it names method families the
kernel registry does not ship (`blyp`, `bp86`, `bpbe`, `b97d`, `tpss`, `hf`,
for instance): the two API surfaces are independent, and a consumer may pair
D3 with a method it brings from elsewhere.

## 7. Stability promise

- Aggregate field names above are API; adding fields is a minor release, changing
  semantics a major one (SemVer + CHANGELOG).
- Kernel numerics may change only via the regeneration discipline
  ([`tools/regenerate.py`](../tools/regenerate.py); committed generated output;
  CI freshness check) and are pin-tested by the Libxc point checks via pyscf plus
  the finite-difference and exact-condition tests in this repo's own suite.
- No Eigen, no basis sets, no density matrices, no foreign framework types anywhere
  in the public surface — the boundary test (anything needing them belongs to the
  consumer).
