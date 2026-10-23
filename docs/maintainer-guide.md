# The excgrid maintainer guide

## Why this is a codegen project

Adding a new functional stays tractable because the hard, bug-prone step — the
partial derivatives — is symbolic differentiation rather than hand work. Yacas
expands a small `.ey` script into a C++ kernel, and the output is committed.

## The add-a-functional procedure

### 1. Get the definition

For a literature functional, take the energy-density formula from the published
paper. For a novel functional, the researcher's own derivation.

Never take a formula from another program's source. Another implementation is
evidence that a formula can be evaluated, never a statement of what the formula
is — and it carries its own licence. Where a published paper is genuinely
unreachable, say so in the commit message and carry the numeric evidence
instead; do not substitute a reader of someone else's code.

The **source** of the formula and the **numeric oracle** you check it against
are two separate channels, and they can be reachable independently. Say in the
commit message which channel supplied the definition and which supplied the
evidence. An oracle-less addition is allowed; a silent one is not.

- **Both reachable** — read the source for the formula, then confirm it
  numerically in step 4. Nothing replaces the numeric check.
- **Source unreachable, oracle reachable** — take the formula from the
  published paper or the derivation, and let step 4's point check carry the
  numeric evidence. This is the stronger of the two: the source states what the
  formula is, while the oracle states whether your kernel reproduces it.
- **Neither reachable** — fall back to step 4's finite-difference channel, and
  say in the commit message that no external oracle was consulted.

### 2. Add the rule to `xc_defs/excgrid_defs.ys` FIRST

Write the functional's own rule as an `X'<Name>(args) := ...` line beside the
existing kernel definitions, and only then write the thin `.ey` driver that
calls it.

This order is load-bearing. The `.ey` is nothing but `e := X'<Name>(...)` plus
`ExKernelGenerate`, so a `.ey` written before its rule is added does not fail
loudly: it reaches Yacas with the call still unevaluated, and you get a broken
`.cpp`. Yacas reports expansion errors on **stdout** and still exits `0`, so a
zero exit is not evidence the step worked.

`tools/regenerate.py` refuses to write when the child's output carries one of
Yacas' own error banners (`Error in file`, `File not found`, `bad argument
number`, ...) or lacks the kernel shape (an `excgrid` namespace line plus a
`result.` assignment), and it aborts before blessing the manifest.
`tools/regenerate_test.py` drives both reproduced exit-0 failures through it.

Before assuming a functional's form fits, check which skeleton it needs. There
are three: the LDA and GGA shapes, and — for a functional that reads the
reserved tau slots — `xc_defs/excgrid_meta_gga_skeleton.ey`, which emits the
seven-argument order-1 kernel where the other two emit a narrower one. All
three emit the functional's second-derivative matrix from the same expression.
The tau tier's own example is `xc_defs/tau_x.ey`, and it is four lines: the rule
is in `excgrid_defs.ys`, the `.ey` is the call plus `ExKernelGenerate` with its
seven named components.

### 3. Run `tools/regenerate.py` once

One invocation per addition, never per build, never in the ordinary build path.
The vendored Yacas lives in `tools/yacas/`; see
[YACAS_PROVENANCE.md](../tools/yacas/YACAS_PROVENANCE.md) for its version and
trim.

**Check the artifact anyway**, whatever the exit status said: `grep -c 'Error
in file\|bad argument'` on the new `generated/<name>.cpp` must be 0, the file
must be non-trivial in size against its neighbours, and `python
tools/regenerate.py --check` must be green before you commit — which is also
what proves the other kernels were left alone.

Two guards make an interrupted run safe and a hanging one loud. Generation
builds into a sibling `<name>.cpp.partial` and renames it into place only once
it is complete, formatted, and pruned, so an interrupted generation cannot
leave a truncated file where a committed kernel used to be. And the child is
killed at the per-kernel bound — `KERNEL_TIMEOUT_SECONDS` (1800) unless
`--kernel-timeout SECONDS` says otherwise, and `0` means no bound at all. The
default is deliberately far above the slowest healthy ORDER-1 kernel:
`pbe_correlation` legitimately takes **about 12.5 minutes**, `vwn5_correlation`
about 3, and most kernels under one. The bound exists to stop a
non-terminating expansion, not to police the pace — do not "fix" a slow kernel
by lowering it.

The second-derivative tier is a second full symbolic differentiation, and it
can cost far more than that default (the measured table is under
[The measured cost of the tiers](#the-measured-cost-of-the-tiers)). A run that
needs more time passes `--kernel-timeout 0` and is a deliberate long run: the
driver prints each kernel's wall time as it goes, on the failure path too,
because a timed-out kernel's actual cost is the number the next bound has to
rest on. The 0.4.0 tier generation was such a run, and it is the one that
produced the table.

### 4. Verify before committing

Three independent channels, and they cover different things:

- **The Libxc point check** (`tools/verify_pyscf.py`) for literature functionals.
  It compares each kernel against pyscf/Libxc at sampled points, and it does
  **not** currently report a clean sweep: 16 of the 18 compared kernels
  reproduce Libxc to 8.24e-09 or better, while `p86` and `pw91_c` do not, each
  for a cause its own docstring names and each kept by decision. Run it, read
  its per-slot summary, and put the new kernel's numbers in the commit message
  rather than implying a pass. A new kernel does not inherit coverage: add it
  to the script's `FUNCTIONALS` table or it is silently uncompared.
- **The finite-difference tests** for novel functionals — no external
  reference needed. They enumerate their kernels explicitly
  ([`tests/kernel_test.cpp`](../tests/kernel_test.cpp)), so a new kernel is
  silently uncovered until you add it to the matching list.
- **The exact-condition identities.** Note that the UEG exact-condition and
  `VxcIntegratesToExc` identities are Slater-exchange identities only — their
  own comments say why — so they are not a channel that covers a new GGA
  kernel.

A kernel whose constants were transcribed from a paper at fewer digits than the
oracle carries will disagree with the oracle in the last digits by
construction. That is a decision about which constants ship, not a wiring
fault. A kernel whose form deliberately differs from Libxc's keeps that
deviation — do not "fix" it to match.

### 5. Commit the `.ey` source and the freshly generated `.cpp` together

In the same commit. Two checks then keep them in sync, and they prove different
things:

- `tools/check_freshness.py` is fast and needs no Yacas. It reads
  `xc_defs/freshness.json`, the hash manifest `regenerate.py` rewrites on a
  successful run, and fails when a codegen input has moved since the blessing.
  Run it before every commit that touches codegen inputs.
- `python tools/regenerate.py --check` re-expands every kernel — about half an
  hour, `pbe_correlation` alone being most of it — and diffs the result against
  the committed bytes. That is the deliberate run, not a hook. CI runs it on the
  Windows leg.

### The complete list of what an addition touches

Skipping any of the first three leaves the tree visibly broken rather than
merely untidy:

1. `tools/regenerate.py` — the `FUNCTIONALS` table. Without the entry the new
   `.ey` is never expanded, so step 3 reports success and produces nothing.
2. `tests/registry_test.cpp` — **two edits, not one.** `RegistryTest.ShippedSurface`
   pins the registry count and the name list. And if the addition is a new
   *kernel* rather than a composition of existing ones, `kKernelUniverse` /
   `kKernelCount` too: the folding guard
   (`RegistryFoldingGuard.EveryExchangePartitionCloses`) recovers each entry as
   a weighted sum over that universe, so a kernel missing from it is fitted by
   a spurious ill-conditioned combination and fails the folding rule. Note that
   the guard's own header comment claims an added functional "is checked
   without touching this test" — true for a hybrid, not for a new kernel.
   A kernel that reads tau is the exception to that second edit: it is exempted
   by `RequiredMask` before the fit, and the exemption is paid for with the
   response check beside it, so do not add it to the universe.
3. `CMakeLists.txt` — the `generated/<name>.cpp` entry in the library sources.
   Miss it and the kernel is never compiled, so the probe reports
   `unknown functional`. Add it **after** step 3 has produced the file, not
   with the `.ey`: CMake validates the source list at configure time, so an
   entry whose file does not exist yet fails the next configure hard with
   `Cannot find source file`, which reads like a broken tree rather than a step
   done out of order.
4. `include/excgrid/kernels.hpp` — the declaration, documented like its
   neighbours. A tau-tier kernel has none here: it is wider than the two
   kernel-pointer types the header's functions return, so the registry names it
   where it is built (`src/kernels_registry.cpp`) and consumers reach it through
   the registry name like any other functional. The header publishes the
   per-point contract, not the tier's internals.
5. `src/kernels_registry.cpp` — for a pure functional, three edits: the
   `PureFunctional` object, the `kRegistry` array (whose size template argument
   is written out), and `kNames`. A hybrid adds one edit before its object: the
   terms go in a named `constexpr` array with a `TermRole` on every term and a
   `static_assert(FoldingRuleHolds(...))` directly above the construction — the
   shape every shipped hybrid has. The roles are what the folding rule reads:
   the published LSDA coefficient against a full GGA kernel double counts the
   LSDA exchange, so the LSDA weight is the published coefficient minus the
   GGA-exchange coefficient, and the two exchange roles must sum to
   `1 - exchangeFraction`. A violation is a build failure. A tau-tier kernel is
   the third shape: the registry declares the generated kernel and its
   second-derivative function at namespace scope (an anonymous-namespace
   declaration would give the definitions in `generated/tau_x.cpp` internal
   linkage and fail at link time) and one class over them supplies the two
   second-derivative entry points, re-packing the generated matrix onto the
   caller's mask.
6. `tests/kernel_test.cpp` — **two edits, not one**: the finite-difference
   list (step 4), and, for a new kernel, the size-pinned
   `std::array<NamedGgaKernel, 6>` (`kGgaExchangeKernels`) or
   `std::array<NamedGgaKernel, 3>` (`kGgaCorrelationKernels`) that the list
   iterates. Leave it and MSVC reports C2078 at the added entry.
7. `tools/verify_pyscf.py` — its `FUNCTIONALS` table, one entry per compared
   kernel. **A literature addition missing here has no point-check coverage
   and nothing says so**: the run stays green for the kernels it does compare.
   A novel functional has no Libxc id to compare against and is covered by the
   finite-difference channel instead, which is why this item is on the
   literature path.
8. `docs/kernel-api.md` §4 — the name list in the shipped set.
9. `xc_defs/freshness.json` — rewritten by step 3; commit it.
10. `docs/maintainer-guide.md` — this list, if the addition taught you
    something it does not already say.
11. Then the closing checks: `python tools/check_freshness.py` fast and green,
    `python tools/regenerate.py --check` slow and green,
    `python tools/regenerate_test.py` green (5 seconds, and the only check that
    would notice the driver silently accepting a failed expansion again), and
    the test suite passing.

## The derivative tiers

The derivative tiers every kernel skeleton emits are the same two.
`xc_defs/excgrid_lda_skeleton.ey`, `xc_defs/excgrid_gga_skeleton.ey`,
`xc_defs/excgrid_gga_piecewise_skeleton.ey` and
`xc_defs/excgrid_meta_gga_skeleton.ey` each expand one expression into the
functional's **materialised second derivatives** beside its order-1 kernel,
because the contract already crosses that tier (`PointSecondDerivativeMatrix`)
and this is the tier an analytic gradient reaches for. The order-1 tier (energy
density + first derivatives) is what plain Vxc assembly consumes.

**A source can decline the tier.** The second differentiation is taken
symbolically, in full, once per upper-triangle entry, and it costs seconds for
the exchange forms and minutes for some correlation forms; the measured
per-kernel times are under
[The measured cost of the tiers](#the-measured-cost-of-the-tiers). A driver
that passes `False` as the last argument of `ExKernelGenerate` — or as the last
element of the argument list `ExPiecewiseKernelGenerate` takes — emits its
order-1 kernel alone. The three LDA/GGA skeletons wrap their whole tier
function in `ExTierEmissionGuard`, so a declined tier produces **no function at
all** rather than one that would answer zeros; the cost of declining is that
callers get the tier's own refusal, not a silently empty matrix. No shipped
source declines any more — the owner ruling of 2026-10-19 funded the tier for
every kernel — but the mechanism stays for a source whose differentiation is
genuinely unaffordable. `tau_x` is not on this path — the meta-GGA generator
takes no such flag and always emits its tier.

**The tier mode chooses which repair the differentiation needs.** The last
element of `ExKernelGenerate`'s argument list is the mode:

- `True` leaves the entry as the differentiation produced it, with only the
  top-level shared-radical cancellation every tier gets. The LDA tiers, the tau
  tier and the kernels whose expressions carry no vanishing radical stay
  byte-identical under it.
- `TierGuard` shifts every applicable radical by `SigmaGuard` in the radicand —
  the same `ExGuardSigmaRadical` the order-1 derivatives carry, applied to the
  first derivative before the second differentiation — and leaves the
  arithmetic above it bit-identical. The correlation forms (`pbe_c`, `pw91_c`,
  `p86`) use it for their total-gradient edge, whose radicand is not a factor
  the cancellation can reach. `becke88` and `pw91` also pass it for their
  entries other than the two hand-derived ones.
- `TierCancelSum` cancels the nested quotients of the numerator sum
  (`ExCancelSumQuotients`). `pbesol` is the one source that needs it: its `mu`
  crosses as the exact division `10/81` rather than as one decimal, so the
  singularity sits at the sum's level and a per-factor cancellation cannot
  reach it.
- `False` declines the tier altogether, above.

**Some entries are supplied rather than differentiated.** A source may write an
entry's expression in the driver through the global `ahandDerivedEntries`, a
list of `{variable, variable, expression}`. A matching pair — in either order —
is taken verbatim: no differentiation and none of the automatic repairs,
because the expression is the derivative already. It then reaches CSE and the
printer like any other entry. `becke88` and `pw91` use this for their two
sigma-sigma entries: the plain second derivative cancels two terms of order
`1/sigma`, and no guard is simultaneously representable in double and invisible
in the answer. The closed forms are written in the drivers against
`AsinhOverX` and `AsinhOverXDerivX2`
(`include/excgrid/internal/asinh_over_x.hpp`), which answer their exact values
at zero and a series below `z = 1e-4`. The emitted tier is finite at the corner
and agrees with the true limit — `0.0229209731018569` and `0.0903626801447989` for
the two channels at `rhoA = 0.31`, `rhoB = 0.22`. `mpw91` is not on this path
and does not get a guard: its
`x^3.72` term makes the sigma-sigma curvature diverge as `sigma^(-0.14)`, so
the truth there is an infinity and a shifted radicand would report a large
finite number where the truth is infinite. That leaves the one kernel with a
corner hole, measured as four non-finite entries: the two divergent diagonals
and the two `(rho, sigma)` channel entries, whose limits are finite but whose
only repair here is that same shift; because the mode guards every
differentiated entry of the kernel, the whole tier stays unguarded. The one
recipe that contains it, `mpw1pw91`, inherits those four entries through the
weighted sum, so `tests/second_derivative_test.cpp` pins both names rather
than the entries.

Two passes in `xc_defs/excgrid_generate.ys` close the ORDER-1 holes this tree
has measured; both are invoked on the first-derivative expressions, and the
first is also the function the tier's `TierGuard` mode calls:

- `ExGuardSigmaRadical` shifts the radicand of a vanishing gradient radical by
  a constant chosen so the shift rounds out of every sigma above about
  `1e-284`: the arithmetic is bit-identical there, and at the corner the
  expression answers the limit (the changed region is sigma below that scale).
  It covers the `asinh` exchanges' channel radicals (`becke88`, `pw91`,
  `mpw91`) and, since the owner ruling of 2026-10-19, the correlation forms'
  total gradient (`pbe_c`, `pw91_c`, `p86`); `ExSigmaRadicalScale` states the
  test that keeps the wider scope safe;
- `ExCancelDensityPower` cancels the removable `rho * (k rho)^(-2/3)` the chain
  rule leaves in a spin-density derivative (`pbe`, `revpbe`, `rpbe`, `pbesol`,
  `pw91`), by the exponent identity rather than by a guard.

Each carries its own measurements in its comment, including the repairs that
were built and rejected on measurement. Neither pass touches the energy
expression, so the tiers the modes above do not repair regenerate byte for
byte. The tier's own singularities are closed by its own mode — the guard, the
sum-level cancellation, or a hand-derived entry. `TierGuard` applies
`ExGuardSigmaRadical` to the first derivative and then differentiates that, so
the tier is the derivative of exactly the guarded expression the order-1
kernel emits. Guarding the finished second derivative instead was measured and
rejected: the walk recursed over `pbe_correlation`'s `(rhoA,rhoA)` entry —
6,447 `Sqrt` nodes at depth 74 — and exhausted Yacas' evaluation stack, while
`d(Sqrt(u + c SigmaGuard))` already carries its shifted `1/Sqrt` before the
differentiation duplicates the radical.

Declining a tier means the declaration must go with it, and the second fact is
not in the source: the tier functions are declared by hand — in
`include/excgrid/kernels.hpp`, or for the tau tier where the registry builds
it, `src/kernels_registry.cpp` — while their definitions are generated, and a
declaration without a definition compiles and then fails at link time on the
registry. `regenerate.py` therefore reads both sides and refuses — in `--check`
as well as on the regenerating path — when the declared set and the defined set
differ, and it does not rewrite the blessing manifest when they do.

Two pieces of the generator carry the second derivatives, both in
`xc_defs/excgrid_generate.ys`: `ExSecondDerivativeEntries` differentiates the
expression over its arguments and folds the entries that are identically zero,
and `ExUpperTriangleIndex` places each survivor in the contract's upper
triangle over the active components in identifier order.
`tools/regenerate.py` prunes per emitted assignment, so a
`matrix.upper[<k>] = ...` line is a root exactly as a `result.<field> = ...`
line is.

A GGA skeleton passes its three sigma arguments as radical-carrying variables, and
that is what the third parameter of `ExSecondDerivativeEntries` is for. A
gradient invariant enters the expression through `Sqrt`, and the expansion
squares it back — `pow(sqrt(sigma)/(2 kF rho), 2)` — so by the time the
enhancement is written the radical is no longer a free factor and no
factor cancellation reaches it. Each differentiation with respect to a
radical-carrying variable therefore produces a quotient with a shared `sqrt` at
the top of the tree, and the generator applies the same top-level cancellation
the order-1 path applies to `vsigma` after the inner derivative and again after
the outer one. Without it the tier's sigma entries are `NaN` at an exactly zero
gradient. The mixed partials commute, so a mixed pair is differentiated with
respect to the radical-carrying variable first; a pair with no radical variable
is differentiated exactly as before, which is why `tau_x` — the one kernel whose
argument list carries no radical — regenerates byte for byte.

Two things to know when reading that output. The skeleton emits the entries in
the order of the components its kernel takes; which of them a caller's matrix is
actually over is the caller's mask, and `src/kernels_registry.cpp` re-packs the
two — an input outside the mask has no row and no column rather than a zero one.
And the contracted tier is computed from the materialised matrix rather than
emitted beside it, so the schema's two second-derivative entry points cannot
disagree.

A recipe takes a tier only when every one of its terms has one, so a hybrid
built on a declined correlation refuses as a whole rather than returning the sum
of the terms it happens to have. Every shipped recipe's terms all carry tiers
now, and `tests/second_derivative_test.cpp` checks b3lyp's materialised matrix
against the weighted sum of its four terms rather than trusting the
composition.

What is not built: the third derivative, which has no contract type to cross in
— that addition is a schema change before it is a generator change.

## Regenerating the angular tables

`tools/gen_lebedev.py` re-derives the Lebedev-Laikov tables in
`include/excgrid/internal/lebedev_tables.hpp` from the moment conditions
(octahedral symmetry + polynomial exactness). The published orbit structure and
the values seeding the solve are carried in the script itself, so it needs no
input beyond a checkout. The solve runs in 80-digit arithmetic (mpmath) and
prints a per-size certificate — max |moment residual| at the published
positions — which is a statement about the *solve*, not about the shipped
table: the committed values are the double-precision rounding of a
high-precision result, so their own moment residuals sit at double precision.
Regenerate and re-certify only when a new size is added.

## Regenerating the D3 tables

`tools/gen_d3_tables.py` transcribes the committed DFT-D3 parameter data
(`tools/data/`, provenance in `D3_DATA_PROVENANCE.md`) into
`src/d3_tables.inc` — a one-time transcription step per data change, never per
build.
