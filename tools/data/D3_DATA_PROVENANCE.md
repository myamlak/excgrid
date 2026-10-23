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
- `d3_r0ab.txt` — the 4465 pair cut-off radii r0 (Angstrom), in
  `setr0ab`'s fill order: larger element outer, smaller inner, no
  padding entry.  The pair i <= j therefore lives at `j(j-1)/2 + i-1`.
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

## The external reference (2026-10-19)

The classic `dftd3` program was compiled and run once as an independent
reference for the two-body zero-damping energy, in a scratch directory
outside this repository; nothing from it is vendored, linked, or shipped.

The sources were fetched from
`https://raw.githubusercontent.com/loriab/dftd3/master/` and recorded by
SHA-256:

| File | SHA-256 |
| --- | --- |
| `dftd3.f` | `d8653170b2e13ce28059880e861d729026f7a41cb566866cd58f417aa947392c` |
| `copyc6.f` | `047b6f02d6b34312303d1cff4b227f286065d877b690f1bf9e087869984d120f` |
| `pars.f` | `d3e4abda619d9c481e52f4197f0fb22a952d7feaf13869c53a1206aaa7dfff8f` |
| `param` | `370ffb6325a6b01ec501e26f3c6c86576c7d3732223d4bac499525b7b48c5333` |

Compiled with GNU Fortran (Ubuntu 15.2.0-16ubuntu1 15.2.0):
`gfortran -O2 -o dftd3 dftd3.f copyc6.f`.

The fixture `water.coord` is in Bohr; the reader takes the first line
`bohr` as the unit selector:

```
bohr
O 0.0 0.0 0.0
H 1.4 0.0 0.9
H -1.4 0.0 0.9
```

The run `dftd3 water.coord -zero -func b3-lyp` uses the zero-damping
scheme and the default (def2-QZVP) b3-lyp parameters s6 = 1,
rs6 = 1.261, s8 = 1.703, alpha6 = 14, alpha8 = 16; the three-body term
is off by default.  It printed

```
 Edisp /kcal,au:    -0.0040 -0.00000634
```

so the reference two-body energy is **-6.34e-6 Ha**.  The same run
printed coordination numbers O 1.998, H 1.000, which confirms the two
conventions the kernel had wrong: `ncoord` uses `rco = rcov(i) +
rcov(j)` (`dftd3.f` line 2628), and `setr0ab` converts the r0 table
with `r0ab(k)/autoang`, `autoang = 0.52917726` (lines 3495-3496).
Both are corrected in `src/d3.cpp`.

The reference also exposed two further defects, independent of those
conventions:

1. **The flat r0 index.** `setr0ab` fills the table with the larger
   element outer (`do i=1,max_elem; do j=1,i; k=k+1`), so the flat index
   is `j(j-1)/2 + i-1`, and the committed `kR0Angstrom` has no padding
   entry.  The kernel's former formula assumed a padded row-major
   layout: for O-H it read 2.9802 Angstrom instead of 2.1768, and for
   the last pair (94, 94) it read one element past the array.  Corrected
   in `src/d3.cpp`.
2. **The C6 table generator and layout.** `tools/gen_d3_tables.py`
   built each reference tuple as `(iza, ib, ia, ib, c6, cna, cnb)` - the
   B element slot held B's reference index - so the committed
   `src/d3_tables.inc` carried 12880 of the 32385 C6 values, with blocks
   keyed by (element A, B reference index) and contaminated reference
   CNs (element 1 read `[0.9118, 1.9209]` instead of `[0.9118, 0.0]`).
   It also emitted each pair's present rows sparsely, while `C6Of` read
   a dense `kRefCount[a] * kRefCount[b]` block, so every block ran into
   the next pair's values.

   The repair (2026-10-19) fixes the tuple; mirrors each row as
   `copyc6` does (`c6ab[b][a][j][i] = c6ab[a][b][i][j]`); and emits each
   ordered pair's dense block in `getc6`'s CN-a-major order, with 0 for
   an absent combination.  `C6Of` now skips non-positive entries with
   the source's own test (`if(c6.gt.0)`, `dftd3.f` line 2578).  The
   source rows are sparse per stored orientation - of 8836 ordered
   element pairs, 401 carry every reference combination, 8041 carry only
   some, and 394 carry none - but mirroring completes every pair: the
   regenerated `src/d3_tables.inc` carries 64516 values (254^2, the
   square of the total reference-state count), all positive, so the
   sentinel path is present but unexercised by this data.  Diagonal
   blocks reproduce the program's printed `C6(AA)`: H 3.0267/7.5916,
   O 15.5059/12.8161/10.3708, and every ordered block transposes into
   its mirror.  Recomputing the reference from the emitted file gives
   -6.338494e-6 Ha; `D3Test.ReferenceEnergyB3lypWater` pins it to the
   printed -0.00000634 within the 5e-8 print resolution.
