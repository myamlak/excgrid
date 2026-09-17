# excgrid

A C++23 library for density-functional integrations: it builds molecular
integration grids and evaluates exchange-correlation energy densities on them.

It has no dependencies beyond the standard library, and it is deliberately
narrow. It gives you points, weights, and per-point XC energy densities —
not basis sets, not density matrices, not a self-consistent field.

- **Molecular block grids** — Murray-Handy-Laming radial quadrature,
  Lebedev-Laikov angular quadrature, Becke/SSF fuzzy-cell partition,
  weight trimming, and spatially compact block re-batching.
- **XC kernels** — the energy density and its first derivatives, for 23
  LDA/GGA/hybrid recipes ([full list below](#functionals)).
- **D3 dispersion** — the Grimme DFT-D3 zero-damping energy and analytic
  coordinate gradients.

**Full API documentation: <https://myamlak.github.io/excgrid/>**

## Quick start

```sh
git clone https://github.com/myamlak/excgrid.git
cd excgrid
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

## Using the library

Build a grid for a molecule, then evaluate a functional at its points. The
library supplies the grid and the kernel; the density is yours.

```cpp
#include "excgrid/grid.hpp"
#include "excgrid/kernel.hpp"

#include <cmath>
#include <cstdio>

int main() {
    // Water, in Bohr.
    excgrid::Geometry water;
    water.atoms = {
        {8, {0.0, 0.0, 0.0}},
        {1, {0.0, 1.4309, 1.1078}},
        {1, {0.0, -1.4309, 1.1078}},
    };

    excgrid::GridParams params;
    params.radialPoints = 75;
    params.angularPoints = 302;

    excgrid::Result<excgrid::BlockGrid> grid = excgrid::BlockGrid::Create(water, params);
    if (!grid) {
        std::printf("grid build failed\n");
        return 1;
    }

    const excgrid::XcFunctional* slater = excgrid::FindFunctional("slater");
    if (slater == nullptr) {
        std::printf("unknown functional\n");
        return 1;
    }

    // Integrating a hydrogenic 1s density, exp(-2r)/pi, on the first atom.
    // A real calculation supplies rho and sigma per point from a basis set;
    // only this line changes.  The exact LDA exchange energy of that
    // density is -0.212742 Ha, and this prints -0.212742.
    constexpr double kPi = 3.14159265358979323846;
    double energy = 0.0;
    for (const excgrid::Block& block : grid->Blocks()) {
        for (std::size_t i = 0; i < block.pointCount; ++i) {
            const std::array<double, 3>& p = block.points[i];
            const double r = std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
            const double rho = std::exp(-2.0 * r) / kPi;
            const excgrid::XcKernelValue k = slater->Evaluate(0.5 * rho, 0.5 * rho, 0.0, 0.0, 0.0);
            energy += block.weights[i] * k.exc;
        }
    }

    std::printf("E_x = %.6f Ha over %zu points\n", energy, grid->TotalPointCount());
    return 0;
}
```

Functionals are resolved by name, never by an enum — your schema carries the
string, so a new release cannot renumber your choices. `FunctionalNames()`
lists every shipped name.

## Consuming it from CMake

There is no packaged `find_package(excgrid)` yet. Embed the source tree and
link the target:

```cmake
add_subdirectory(excgrid)          # or FetchContent_Declare + MakeAvailable
target_link_libraries(myapp PRIVATE excgrid::excgrid)
```

The target requires C++23 and carries its own include directory, so
`#include "excgrid/grid.hpp"` works without further setup.

## Functionals

| Name | Kind | Notes |
|---|---|---|
| `slater` | LDA exchange | Slater/Dirac |
| `vwn5`, `vwn3` | LDA correlation | interpolation-V and RPA parameters |
| `pw92` | LDA correlation | |
| `svwn`, `spw92` | LDA composite | Slater + VWN5, Slater + PW92 |
| `becke88`, `pw91`, `pbe`, `revpbe`, `rpbe`, `mpw91`, `pbesol` | GGA exchange | |
| `lyp`, `pbe_c`, `pw91_c`, `p86` | GGA correlation | |
| `b3lyp`, `pbe0`, `b3pw91`, `mpw1pw91`, `bhandhlyp`, `b3p86` | hybrid | see `ExchangeFraction()` |

A hybrid's exact-exchange fraction is reported by `ExchangeFraction()`, which
the caller routes through its own Hartree-Fock path — excgrid evaluates only
the density-functional part.

## Documentation

| Document | What it covers |
|---|---|
| [docs/kernel-api.md](docs/kernel-api.md) | The frozen boundary contract: types, units, preconditions |
| [docs/maintainer-guide.md](docs/maintainer-guide.md) | Adding a functional; the codegen pipeline |
| [docs/mainpage.md](docs/mainpage.md) | The published formulas the kernels implement |
| [CITATION.bib](CITATION.bib) | BibTeX for those references |
| API reference | Built by Doxygen; published to GitHub Pages on each release tag |

To build the API reference locally (this is also a gate — see below):

```sh
cmake --build build --target excgrid-docs   # writes build/doxygen/html
```

## Verification

Three things are checked, and they check different things:

- **The test suite** (`ctest`) — quadrature exactness, partition and trimming
  behaviour, D3 energies and gradients, and per-kernel accuracy against
  finite differences and exact conditions that need no external reference.
- **Libxc point checks** (Linux leg) — every shipped kernel is compared
  against pyscf/Libxc at sampled points.
- **Codegen freshness** (Windows leg) — the committed `generated/*.cpp` are
  re-derived from their `xc_defs/*.ey` sources and diffed byte-for-byte, so
  committed generated code cannot drift from the sources it claims to come
  from.
- **The zero-warning Doxygen gate** — a Doxygen warning fails the build, so
  the API reference cannot silently rot. It catches a misspelled or missing
  `\param`, an undocumented struct member, and an undocumented class or enum.
  It does not catch a public function that carries no doc comment and no
  `\ingroup`, which is why every public entry belongs to a documented group.

See [.github/workflows/ci.yml](.github/workflows/ci.yml) for the exact matrix.

## Codegen (maintainers)

The kernels are generated, not hand-written: Yacas expands `xc_defs/*.ey`
into `generated/*.cpp`, and the result is committed. Regeneration runs once
per functional change, never per build:

```sh
python tools/regenerate.py           # rewrite generated/ + the input manifest
python tools/regenerate.py --check   # verify the committed output is current
```

Only the `.ey` sources are edited by hand. See
[docs/maintainer-guide.md](docs/maintainer-guide.md) and
[tools/yacas/YACAS_PROVENANCE.md](tools/yacas/YACAS_PROVENANCE.md).

## Citation

If excgrid contributes to work you publish, please cite it — see
[CITATION.cff](CITATION.cff), or [CITATION.bib](CITATION.bib) for the BibTeX
of the formulas and data the kernels implement.

## License

BSD-3-Clause — see [LICENSE](LICENSE).

Third-party components and data are listed in
[THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md), which also carries the
license texts and provenance for the vendored Yacas codegen tool.
