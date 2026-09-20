# Contributing to excgrid

Thanks for considering a contribution. Read this whole file before opening a
pull request — a few of the rules here are unusual, and they exist because
this library is a numerical artifact whose numbers are its contract.

## What this project is

A standalone C++23 library for density-functional integrations: molecular
block-grid construction, per-point exchange-correlation kernels, and D3
dispersion. It has no dependencies beyond the standard library, and it
deliberately does not know about basis sets, density matrices, or
self-consistent fields. See the [README](README.md) and the frozen boundary
contract in [docs/kernel-api.md](docs/kernel-api.md).

## Ground rules

1. **The API contract is frozen.** [docs/kernel-api.md](docs/kernel-api.md)
   specifies the units, the signatures, and the shipped recipes. A change to
   it is a versioned release, not a pull request — open an issue first.
2. **Generated code is never edited by hand.** Every kernel in `generated/`
   comes from a `xc_defs/*.ey` source through `tools/regenerate.py`. Edit the
   source, regenerate, and commit both together. A hand-edited generated file
   is invisible to every check this repo has.
3. **Do not weaken a test or a gate.** The Doxygen gate, the codegen freshness
   check, and the numerical tests are the reasons the numbers can be trusted.
   If a change makes one go red, the change is the suspect — not the check.
4. **State your evidence.** If you add a functional, say in the pull request
   where its formula came from and what verified it (a published paper, a
   comparison against Libxc, a finite-difference check). An addition with no
   stated evidence will be asked for one.
5. **Small, reviewable changes.** One logical change per pull request.

## Build and test

Requirements: CMake 3.25 or newer, and a C++23 compiler (MSVC, GCC, or Clang).
No vcpkg, no submodules. The test suite fetches GoogleTest 1.17.0 through
CMake's FetchContent, so the first configure needs network access.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

Please run the suite in Debug as well before opening a pull request.

## Style rules

- **Naming:** PascalCase types and functions, camelCase variables,
  `_camelCase` private members, kPascalCase constants and enums, short
  lowercase namespaces. No snake_case anywhere.
- **Formatting:** [.clang-format](.clang-format) is not advisory — the
  committed generated kernels are formatter output, so a change to it changes
  the library's bytes. Run clang-format on your changes; do not edit the
  config to suit a change.
- **Headers:** `#pragma once`; Doxygen `///` on every public declaration, with
  `\param` and `\returns`, and an `\ingroup` naming the documented group it
  belongs to. The Doxygen build gates on warnings (see below).
- **Errors:** no exceptions. Fallible construction returns
  `excgrid::Result<T>` (a `std::expected`); consumers translate the codes at
  their own boundary.
- **Raw arrays:** 3-vector coordinates use `std::array<double, 3>`; pointer +
  count APIs use `std::span`; raw arrays only where an ABI mandates them.
- **Comments:** brief, self-contained, why-focused. Cite published work by its
  citation, never an internal document.

## Tests

- A new kernel lands with tests: its entry in the finite-difference list, and
  its entry in the registry surface test. Both enumerate kernels explicitly, so
  a kernel you do not add is silently uncovered.
- Numerical expectations belong in the test with the reasoning that produced
  them, not as a magic constant.
- Adding a functional touches more than the test file — the maintainer guide
  lists every file, and skipping one is usually a red build or a silently
  uncovered kernel rather than a visible failure.

## The Doxygen gate

`cmake --build build --target excgrid-docs` builds the API reference and fails
on any Doxygen warning. What it catches is a misspelled or missing `\param`, an
undocumented struct member, and an undocumented class or enum. What it does
**not** catch is a public function with no doc comment and no `\ingroup` —
such a function belongs to no documented group, so there is nothing to warn
about. Put every public entry in a documented group and the gap closes.

## What never goes in

- References to the development repository this code was cut from: internal
  paths, decision numbers, stage or track names, person-specific notes.
- Vendored code beyond what is already here (the Yacas codegen tool and the
  D3 parameter data), or generated output edited by hand.
- Third-party code or data without its license recorded in
  [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## License

BSD-3-Clause — see [LICENSE](LICENSE). By contributing, you agree your
contribution is licensed under the same terms. Third-party components and their
licenses are listed in [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## Versioning

The project follows Semantic Versioning; the version identifier lives in
`CMakeLists.txt`, the [Doxyfile](Doxyfile), and [CITATION.cff](CITATION.cff),
and is checked for agreement in CI.

- **MAJOR** — an incompatible public API change (removal, rename, signature,
  layout, namespace), a changed unit or domain, or a change to the stability
  promise in [docs/kernel-api.md](docs/kernel-api.md).
- **MINOR** — additive public API, or any internal numerical change that leaves
  every documented signature, unit, and domain intact. A reconstructed kernel
  whose numbers move in the last digits is this class.
- **PATCH** — no intended public-API or numerical change.

Within a major version, public function signatures, units, and the shipped name
set are stable. Exact bitwise output is **not** promised across releases: it
depends on the compiler, the flags, and the build configuration. Pin the release
tag if you need reproducibility.

## Getting help

Open an issue for questions, bug reports, or proposals to extend the contract.
The maintainer reviews and merges every change to `main`.
