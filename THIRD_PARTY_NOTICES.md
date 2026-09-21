# Third-party notices

Licenses and attribution obligations for every third-party component that
ships in excgrid builds or vendored data.  When distributing binaries,
this file must travel with the distribution, together with the license
texts it names (the vendored Yacas texts live in `tools/yacas/`; the
BSD-3-Clause text is `LICENSE`).

| Component | Version | Purpose | License |
|---|---|---|---|
| Yacas | 1.8.0 (win64, vendored subset) | the symbolic-codegen maintainer tool (`tools/yacas/`) | LGPL-2.1-or-later — full text in `tools/yacas/COPYING.LESSER` (with the GPL-2.0 terms it incorporates by reference, `tools/yacas/COPYING`) |
| `xc_defs/excgrid_cpp_form.ys` | — | the maintainer printer the codegen step drives | LGPL-2.1-or-later — its rule frame derives from Yacas' `c_form.rep/code.ys`; see "The excgrid printer" below |
| GoogleTest | 1.17.0 | test harness (FetchContent, tests only) | BSD-3-Clause |
| Lebedev-Laikov quadrature tables | 16 sizes, re-derived and certified | angular quadrature (`include/excgrid/internal/lebedev_tables.hpp`) | published mathematical data; the orbit structure per size and the values that seed the solve are the published tables of Lebedev & Laikov, Dokl. Math. 59 (1999) 477 (the citation `CITATION.bib` records), as distributed through the CCL, and are carried as data in `tools/gen_lebedev.py`; the header itself is this repository's own solve of the moment conditions, certified by that script. No third-party source code is shipped, linked, or used at build or run time |
| DFT-D3 parameter data | Grimme 2010 | C6/r0/r2r4/rcov tables (`tools/data/`, transcribed into `src/d3_tables.inc`) | published scientific parameter data; numeric values only, transcribed from the GPL-1.0-or-later `loriab/dftd3` distribution — no code taken; see "DFT-D3 data" below |

## Vendored Yacas

Yacas is the excgrid codegen maintainer tool, vendored as a plain
directory (`tools/yacas/`) — the pinned 1.8.0-win64 release's console
subset (see `tools/yacas/YACAS_PROVENANCE.md` for version, hash, trim,
and license).  Yacas is invoked exactly once per functional addition or
`.ey` change, never by any build; only its OUTPUT (the committed
`generated/*.cpp`, produced from this project's own `.ey` input scripts)
enters the library.

Yacas is licensed LGPL-2.1-or-later: upstream declares version 2.1 or, at
your discretion, any later version, and the vendored `bin/yacas.exe` prints
the same grant in its own startup banner.  The vendored subset IS
redistributed - it is committed to this repository, `bin/yacas.exe` and
`share/yacas/scripts/` alike - so LGPL-2.1 section 4 governs it, and the
notices below are stated against the source distribution as well as
against binaries:

- **License text.** `tools/yacas/COPYING.LESSER` carries the LGPL-2.1 in
  full.  LGPL-2.1 incorporates the terms of the GPL-2.0 by reference, so
  `tools/yacas/COPYING` carries that text as well.
- **Corresponding source.** The vendored files are an unmodified subset of
  the upstream 1.8.0-win64 release; `tools/yacas/YACAS_PROVENANCE.md`
  records the upstream repository, the tag, the SHA-256 of the release
  archive, and exactly which parts were trimmed.  The script sources that
  make up the vendored standard library ship here in full; for
  `bin/yacas.exe`, the complete corresponding source is the upstream
  `v1.8.0` source tree, and this project offers equivalent access to it
  from the same place the object code is distributed from: a written
  request to the maintainer (`.github/CODEOWNERS`) is answered with that
  source tree, at no more than the cost of distribution, for three years
  from the release that carried this notice.
- **What excgrid does not do.** The library never links Yacas and never
  calls it at build or run time; the dependency is one-way, from the
  maintainer's codegen step to the tool.  Nothing in `generated/` is a
  Yacas derivative in the LGPL sense - those files are this project's own
  output, emitted from this project's own `.ey` scripts, and are
  BSD-3-Clause like the rest of excgrid.

## The excgrid printer

`xc_defs/excgrid_cpp_form.ys` is the rule-based printer the codegen step
drives to turn a Yacas expression into C++ source text.  Its rule FRAME is
derived from Yacas' own `CForm` printer - `tools/yacas/share/yacas/scripts/
c_form.rep/code.ys`, part of the vendored subset above - following that
file's `RuleBase` pair, its never-bracketed precedence constant, its guard
function over `IsBoolean`/`IsString`, and its rule numbers and predicate
names for integers, zero, numbers, atoms and strings.  It is therefore a
derivative of an LGPL-2.1-or-later work and carries those terms rather than
excgrid's BSD-3-Clause; the file's own header says so, and also identifies
the parts that are excgrid's own (the bracketing policy, the `std::` names,
the ordering-comparison and assignment rules, and the generator hooks).
Like Yacas itself, this file is maintainer codegen tooling: it is never
linked into the library and never invoked by a build, and the C++ text it
emits is excgrid's own output.

## Verification tooling that is not shipped

The point checks (`tools/verify_pyscf.py`, Linux CI lane only) run under
**pyscf** (Apache-2.0), which reaches **Libxc** (LGPL-3.0-or-later) for its
functional values.  Libxc is used here strictly as a *verification oracle*:
the shipped kernels are evaluated and their values compared against Libxc's
at sampled points.  Neither pyscf nor Libxc is vendored in this repository,
linked into the library, or present at build or run time, and nothing of
either is redistributed here, so no distribution obligation attaches and
neither appears in the table above.  They are named because the test
tooling names them, because CI installs them, and because a deliberate,
cited difference from Libxc's values is documented for two kernels
(`pw91_c` and `p86`, `docs/kernel-api.md`).

This is the oracle's whole role: it is how the kernels are *checked*, not
what they are *derived from*.  Every shipped kernel is implemented from the
published literature `docs/mainpage.md` and `CITATION.bib` name, and where a
constant is carried at full precision, its closed form is stated next to it
in `xc_defs/` so that it can be re-derived from the published expression
without reference to any implementation.

## DFT-D3 data

The numeric DFT-D3 parameter tables were transcribed from the published
scheme (Grimme, Antony, Ehrlich, Krieg, J. Chem. Phys. 132 (2010)
154104) as distributed numerically in the classic dftd3 program; they
are scientific parameter data, not code.  The excgrid D3 implementation
is fresh code.

The distribution transcribed from is the `loriab/dftd3` mirror, which is
licensed **GPL-1.0-or-later** - its `LICENSE` is the GPL version 1
(February 1989) text, and `dftd3.f`'s own notice ("Copyright (C) 2009 -
2011 Stefan Grimme, University of Muenster, Germany") grants version 1
"or (at your option) any later version".  What was taken is **numeric
values only**, from the data file `pars.f`: no line of `pars.f` or
`dftd3.f` is reproduced, translated, or adapted anywhere in this
repository, and neither file is linked, at build time or run time.  The
transcription is mechanical (`tools/gen_d3_tables.py` -> the committed
`src/d3_tables.inc`), and the D3 energy, gradient, and
coordination-number code around those values is independently written.
`CITATION.bib` carries the 2010 paper, which is where the values are
published as scientific data.

## DFT-D4 data

The numeric DFT-D4 reference-state tables were transcribed, **values only**,
from the `dftd4` distribution (`github.com/dftd4/dftd4`), which is licensed
**LGPL-3.0-or-later** by its per-file SPDX headers (its root `COPYING` is
GPL-3.0).  The values were taken from `src/dftd4/reference.inc` - the
per-reference-state reference charge, reference coordination number, and the
23 dynamic polarizabilities `alpha(iw)` of each state.

What was taken is **numeric values only**.  No statement, comment, structure
or expression of the source is reproduced, translated or adapted anywhere in
this repository, and no file of that distribution is linked, at build time or
run time.  The transcription is mechanical (`tools/gen_d4_tables.py` -> the
committed `src/d4_tables.inc`) and is `--check`-verifiable, and the D4 charge
model, damping and three-body code around those values is independently
written.  The 23 frequencies the model's Casimir-Polder relation integrates
over were transcribed the same way and by the same generator, values only,
from the `freq` data of `src/dftd4/model/utils.f90` of the same distribution.

The method damping parameter sets for the 107 method families were transcribed
the same way, values only, from `assets/parameters.toml` of the same
distribution; every D4 entry of that file names its source publication, and
`CITATION.bib` carries the D4 paper (Caldeweyher, Ehlert, Hansen, Neugebauer,
Spicher, Bannwarth, Grimme, J. Chem. Phys. 150 (2019) 154122), which is where
the values are published as scientific data.

The element-specific reference states the model's C6 step interpolates over
were transcribed the same way, values only, from the same `reference.inc`: per
element and per reference state, the reference system it is drawn from, its
reference coordination number, its reference charge and the 23 dynamic
polarizabilities of that state (`tools/gen_d4_reference.py` -> the committed
`src/d4_reference.inc`, likewise `--check`-verifiable).  That transcription
covers the elements the published data covers completely, which is Z = 1 to 85:
the distribution's last element carries reference coordination numbers but no
reference polarizabilities, so it has nothing for that step to interpolate and
the table stops one short of the parameter tables' own range of 86.

Those polarizabilities are those of a whole reference MOLECULE and not of the
atom the state belongs to, so the transcription also carries the terms the
model's own partitioning needs to reduce one to the other: each state's share
of its reference system, the count and charge of that system's other atoms, and
which reference system of that other element to take those atoms'
polarizabilities from, together with the secondary-reference entries those
lookups name.  The share is checked at transcription time against the atom
count of the formula the source names each state by, and disagrees with it
nowhere; the count is carried as published, and the one state where it differs
from that formula's other-atom count says so in the generated file.  `main` of
the source distribution carried two more of these states than an earlier
transcription of it did, for which this repository's table has been regenerated
whole; the source's own generated file carries no revision marker to cite.

The D4 element parameters were transcribed the same way, values only.  The
electron-equilibration set (electronegativity, hardness, coordination scale
and radius) comes from Table A1 of that paper's Supplementary Material; the
coordination number's covalent radii (Pyykko and Atsumi, Chem. Eur. J. 15
(2009) 186-197, the paper's ref 48) and Pauling electronegativities (the
paper's ref 47) come from the `covrad.f90` and `en.f90` data modules of the
same distribution; and the charge-scaling function's element-specific
chemical hardnesses (Ghosh and Islam, Int. J. Quantum Chem. 110 (2010)
1206-1213, the paper's ref 42) come from its `hardness.f90` module.  Those
radii are carried in both the convention the module publishes and the
4/3-scaled counting convention, and the coordination-number accessor names
the one it returns.  The transcription is mechanical (`tools/gen_d4_eeq.py`,
`tools/gen_d4_cn_tables.py` and `tools/gen_d4_hardness.py` -> the committed
`src/d4_eeq_tables.inc`, `src/d4_cn_tables.inc` and
`src/d4_hardness_tables.inc`) and is `--check`-verifiable.

The effective nuclear charges the charge scaling function works in come from
the paper rather than from the distribution.  Its summary table gives the
subtracted core for the elements beyond krypton, four ranges at a time, and
its discussion of the reference calculations names the effective core
potentials and the elements they cover: 28 core electrons for Rb, Sr, Y-Cd,
In-Sb, Te-Xe and Ce-Lu; 46 for Cs, Ba and La; 60 for Hf-Hg, Tl-Bi and Po-Rn.
The table prints 18 for the Rb-Xe range, which matches no potential the same
paper names and would charge bromine as chlorine; the element-by-element
statement is the one this repository carries.

One number of the model is taken from neither, and is carried as a parameter
rather than as a constant: the count `N^s` of Gaussian functions each
reference system contributes to the interpolation of equation 8.  It is
absent from the data - every array of `reference.inc` was enumerated and
searched for a width or count array, `src/dftd4/data/` carries none, and the
`weight_factors` of `wfpair.f90` are a per-element-pair weighting of a
different structure - so it is read from the paper's own worked example,
Figure 4, with its caption and the text around it.  What that reading yields
is a rule and not a value: panel (a) labels the three reference systems of the
example with one common count, panel (b) makes the well-separated system's
count differ from that of the two that lie close together, the caption states
that a well-separated pair "are easily distinguishable ... with N^s = 1" and
that "the set of Gaussian functions is enlarged for A,ref2 and A,ref3 ... by
varying the N^s value in equation 8 dynamically", and the text says the
procedure "is exemplified in Figure 4 to explain the principle of the
weighting scheme".  No numeral is printed for the enlarged count, in the
figure or anywhere else in the paper, so `D4ReferenceWeights` takes the counts
from its caller and states the rule in its documentation instead of fixing a
default this repository would have invented.  The counts the library ships as
its own default (`src/d4_reference_counts.inc`) are therefore **derived data,
not transcribed data**: they are this project's own, and they must not be
cited as the authors' values.  **They are not fitted values either**, and the
file says so: a fit of the whole family against the published two-body energy
of the caffeine fixture reaches that energy, and is not taken.  Over every
uniform count from one to a million the ratio of this implementation's energy
to the published one runs from 0.997913 to 0.999281, and a mixed assignment of
the same family reaches 1.000000 to six decimals; the counts are held at the
model's neutral baseline of one Gaussian function per reference state because
a vector picked from that family is a fit to the fixture and not a value of the
model, and the baseline is what the tests pin.

One further number of the model is the second global of its charge-scaling
function, the factor its element-specific chemical hardnesses are multiplied
by before equation 2's exponential takes them.  Equation 2 as printed carries
the hardness unmultiplied and the paper names only the other global, so this
one is read from the model's own published output: the per-atom
polarizabilities the same distribution's app test set records for its
lenalidomide fixture select it, at 2.8 per cent root mean square against 18
per cent without it, and the file's own parameter defines it.  It is carried
as `kChargeScalingSteepness` in `src/d4.cpp` rather than folded into the
transcribed hardness table, which stays faithful to its source.

**Each reference state's own charge, and the charge of the atoms equation 5
subtracts, are taken from the model's classical charge set and not from the
`ref` set the source places beside it.**  The source carries six charge sets
per state, and the model's own reference-polarizability routine of the EEQ
scheme reads the classical pair (`clsq`/`clsh`), as its companion routine for
the reference charges does, while its GFN2 routine reads the `ref` pair and
its newer EEQ-BC routine the `eeqbc` pair.  The choice is settled the same way
as the other readings above, by the model's own published output: against the
32 published per-atom polarizabilities of the lenalidomide fixture, the
classical pair leaves this project's values 0.1 per cent out in root mean
square, the `ref` pair 2.8 per cent, the EEQ-BC pair 2.6, the DFT pair 2.9 and
the PBC pair 2.8.  The sets are not interchangeable elsewhere in the table:
the GFN-FF pair coincides with the classical one over the four elements the
fixture carries, and differs from it over 17 of the 261 reference states the
table covers.  `tools/gen_d4_reference.py` reads `clsq` and `clsh` and records
why; the `ref` pair appears nowhere in the table it generates.

The two-body energy's pair sum is read from the same published data and not
from the paper's notation.  Equation 18 sums over `AB`, which is ambiguous
between the unordered pairs and the ordered ones, and the two readings differ
by a factor of two.  What settles it is the arithmetic of the pair matrix the
same distribution publishes for its caffeine app test: the matrix's entries
sum over **every ordered pair of atoms** to exactly the `energy` field the
same file reports, less only the non-additive term.  That total is the model's
own two-body energy, so each entry is half of the contribution of the pair it
belongs to, and the energy is the sum over the unordered pairs and nothing
more; summing over ordered pairs is what recovers it.  A second check agrees:
with the polarizabilities the charge scaling above produces, this project's
per-pair terms come out within a few per cent of twice those entries for every
one of the ten element pairs the molecule contains.  This repository therefore
sums the unordered pairs once, and the equation's printed form is not what it
follows here - one more place where this publication's text and its own data
disagree, as its equations 1 and 3 already do and as the reference count N^s
already does by being absent.
