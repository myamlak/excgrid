#pragma once

#include <array>
#include <cstddef>
#include <vector>

namespace excgrid {

/// \defgroup excgrid-geometry The minimal geometry aggregate
///
/// The library's own atom/geometry types: plain data (species + Bohr
/// positions), deliberately NOT any consumer's type - the standalone
/// boundary (docs/kernel-api.md section 5).

/// One atom: the element and its position.
/// \ingroup excgrid-geometry
struct Atom {
    int atomicNumber; ///< Z.
    std::array<double, 3> position; ///< Bohr coordinates.
};

/// A molecular geometry: atoms in input order.
/// \ingroup excgrid-geometry
struct Geometry {
    std::vector<Atom> atoms; ///< The atoms.
};

} // namespace excgrid
