# Third-party notices

Licenses and attribution obligations for every third-party component that
ships in excgrid builds or vendored data.  When distributing binaries,
this file must travel with the distribution, together with the license
texts it names (the vendored Yacas texts live in `tools/yacas/`; the
BSD-3-Clause text is `LICENSE`).

| Component | Version | Purpose | License |
|---|---|---|---|
| Yacas | 1.8.0 (win64, vendored subset) | the symbolic-codegen maintainer tool (`tools/yacas/`) | LGPL-2.1 — full text in `tools/yacas/COPYING.LESSER` (with the GPL-2.0 terms it incorporates by reference, `tools/yacas/COPYING`) |
| GoogleTest | 1.17.0 | test harness (FetchContent, tests only) | BSD-3-Clause |
| Lebedev-Laikov quadrature tables | 16 sizes, solved-for | angular quadrature (`include/excgrid/internal/lebedev_tables.hpp`) | published mathematical data; tables solved for this project by `tools/gen_lebedev.py` (this author's Lebedev-grid recovery) |
| DFT-D3 parameter data | Grimme 2010 | C6/r0/r2r4/rcov tables (`tools/data/`, transcribed into `src/d3_tables.inc`) | published scientific parameter data; provenance in `tools/data/D3_DATA_PROVENANCE.md` |

## Vendored Yacas

Yacas is the excgrid codegen maintainer tool, vendored as a plain
directory (`tools/yacas/`) — the pinned 1.8.0-win64 release's console
subset (see `tools/yacas/YACAS_PROVENANCE.md` for version, hash, trim,
and license).  Yacas is invoked exactly once per functional addition or
`.ey` change, never by any build and never redistributed with excgrid
binaries; only its OUTPUT (the committed `generated/*.cpp`, produced from
this project's own `.ey` input scripts) enters the library.

Yacas is licensed LGPL-2.1.  The vendored subset IS redistributed - it is
committed to this repository, `bin/yacas.exe` and
`share/yacas/scripts/` alike - so the notices below are stated against
the source distribution as well as against binaries:

- **License text.** `tools/yacas/COPYING.LESSER` carries the LGPL-2.1 in
  full.  LGPL-2.1 incorporates the terms of the GPL-2.0 by reference, so
  `tools/yacas/COPYING` carries that text as well.
- **Corresponding source.** The vendored files are an unmodified subset of
  the upstream 1.8.0-win64 release; `tools/yacas/YACAS_PROVENANCE.md`
  records the upstream repository, the tag, the SHA-256 of the release
  archive, and exactly which parts were trimmed.  Upstream is the source
  location for the complete corresponding source of this component.
- **What excgrid does not do.** The library never links Yacas and never
  calls it at build or run time; the dependency is one-way, from the
  maintainer's codegen step to the tool.  Nothing in `generated/` is a
  Yacas derivative in the LGPL sense - those files are this project's own
  output, emitted from this project's own `.ey` scripts, and are
  BSD-3-Clause like the rest of excgrid.

## DFT-D3 data

The numeric DFT-D3 parameter tables were transcribed from the published
scheme (Grimme, Antony, Ehrlich, Krieg, J. Chem. Phys. 132 (2010)
154104) as distributed numerically in the classic dftd3 program; they
are scientific parameter data, not code.  The excgrid D3 implementation
is fresh code.
