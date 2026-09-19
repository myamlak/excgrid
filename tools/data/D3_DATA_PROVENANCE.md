# DFT-D3 data provenance

The four committed files here hold the published DFT-D3 parameter data
(Grimme, Antony, Ehrlich, Krieg, J. Chem. Phys. 132 (2010) 154104),
transcribed numerically from the classic dftd3 program's distribution
(the `loriab/dftd3` mirror of `pars.f`/`dftd3.f`, commit at
transcription time 2026-09-08: master), machine-extracted - no manual
retyping:

- `d3_c6_ref.txt` — 32385 reference rows: (C6, Z'_a, Z'_b, CN_a, CN_b),
  where a primed Z slot carries the CN-reference index in the hundreds
  digit (Z' = z + 100*i means element z, reference state i+1) — the
  classic dftd3 "limit" encoding.
- `d3_r0ab.txt` — the 4465 pair cut-off radii r0 (Angstrom),
  upper-triangle order, from `setr0ab`.
- `d3_r2r4.txt` — the 94 per-element sqrt(<r^4>/<r^2>) values (the
  pre-scaled dftd3 convention).
- `d3_rcov.txt` — the 94 covalent radii (Pyykko-Atsumi values scaled by
  k2 = 4/3, in Bohr), for the coordination-number counting function.

`tools/gen_d3_tables.py` transcribes these into the committed
`src/d3_tables.inc` (run once per data change; never per build).  The
parameter data are published scientific facts; the excgrid D3
implementation is fresh code and carries no dftd3 code.

## The source's license

The `loriab/dftd3` distribution is licensed **GPL-1.0-or-later**: its
`LICENSE` is the verbatim GNU General Public License, version 1
(February 1989) text, and `dftd3.f` carries its own notice,
"Copyright (C) 2009 - 2011 Stefan Grimme, University of Muenster,
Germany", granting "either version 1, or (at your option) any later
version" - so the later-version option is exercised by the program's own
terms, not merely by the bare license file.  `pars.f`, the file the
numeric transcription actually reads, is pure data: it is a list of
`real*8` array initializers with no comment header and no code, which is
exactly the shape of what was taken from it.

What excgrid takes from that distribution is **numeric parameter values
only** - the published Grimme-2010 constants and reference data, as
enumerated above.  No line of `pars.f` or `dftd3.f` is reproduced,
translated, or adapted anywhere in this repository; the transcription is
mechanical (`tools/gen_d3_tables.py` reads the numbers and writes
`src/d3_tables.inc`), and the excgrid D3 energy, gradient, and
coordination-number machinery is independently written.  The values
themselves are the scientific data the 2010 paper defines and publishes,
which is why `CITATION.bib` carries that paper as the citation for this
component.
