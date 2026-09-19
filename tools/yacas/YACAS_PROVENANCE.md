# Vendored Yacas — provenance and trim

## What this is

The Yacas computer algebra system, vendored as the excgrid codegen
maintainer tool. Yacas is invoked exactly once per functional addition or
`.ey`-script change (never by any build, never in CI's ordinary build
path — the CI freshness check runs it on the windows leg only, see
`.github/workflows/ci.yml`).

## Version

**1.8.0, win64** — the official GitHub release asset
`yacas-1.8.0-win64.zip` from `https://github.com/grzegorzmazur/yacas`
(tag `v1.8.0`), SHA-256 of the zip: `ea03a14a6f882542498f099c5e28deb1c4e567d0a8d8ba255000f12d1497463b`
(computed at vendoring time, 2026-09-08).

Why 1.8.0 and not 1.9.1: 1.9.1 (July 2020) ships only a Java jar; 1.8.0 is
the newest release with a native win64 console binary, and it is the
version the codegen smoke test passed on — re-running a pre-existing
2018-era `.ey` script **reproduced that script's own committed output
byte-identically**, and the generated C++ compiled and linked under MSVC.

## Trim

Only the console runtime is vendored — `bin/yacas.exe` (its only imports
are the Win32/CRT system DLLs) plus `share/yacas/scripts/` (the standard
library `-d`/`--rootdir` needs). Excluded from the release zip: the Qt GUI
(`yacas-gui.exe`, the Qt5/QtWebEngine DLLs, `bin/resources/`), `lib/` and
`include/` (building against libyacas), `share/yacas/tests/` (self-tests).

## License

Yacas is **LGPL-2.1-or-later**: upstream declares version 2.1 or, at your
discretion, any later version, and the vendored `bin/yacas.exe` prints the
same grant in its own startup banner.  As a maintainer-side
code-generation tool, Yacas is never linked into the library and never
invoked by a build; only its OUTPUT (the committed `excgrid/generated/*.cpp`,
generated from this project's own `.ey` input scripts) enters the library.
The vendored subset itself - this directory - is redistributed as part of
this repository, so LGPL-2.1 section 4 governs it and the corresponding-source
offer recorded in `THIRD_PARTY_NOTICES.md` applies.  See that file, and
`COPYING.LESSER` / `COPYING` here for the license texts.

## Reproducibility

`tools/regenerate.py` locates the binary here first; the committed
`generated/` output and the CI freshness check pin the toolchain.
