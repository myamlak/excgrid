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
