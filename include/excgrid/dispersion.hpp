#pragma once

#include "excgrid/error.hpp"
#include "excgrid/geometry.hpp"

#include <array>
#include <cstddef>
#include <string_view>
#include <vector>

namespace excgrid {

/// \defgroup excgrid-d3 Grimme D3 dispersion
///
/// The DFT-D3 two-body dispersion correction (Grimme, Antony, Ehrlich,
/// Krieg, J. Chem. Phys. 132 (2010) 154104), zero-damping scheme:
/// geometry-only closed forms - energy plus analytic coordinate
/// gradients, hand-written (no codegen), over the library's own
/// Geometry type.  Coordination-number-dependent C6 interpolation,
/// the C8 estimate, and the counting-function coordination numbers
/// follow the published scheme; the parameter data is committed under
/// tools/data/ (see tools/gen_d3_tables.py).
/// \{

/// The D3 zero-damping parameters of one functional.
/// \ingroup excgrid-d3
struct D3Parameters {
    double s6 = 1.0; ///< Global C6 scale.
    double s8 = 1.0; ///< C8 scale.
    double rs6 = 1.0; ///< The a1 damping scale of the R^6 term.
    double alpha6 = 14.0; ///< The a2 exponent of the R^6 term.
    double rs8 = 1.0; ///< The a1 damping scale of the R^8 term.
    double alpha8 = 16.0; ///< The a2 exponent of the R^8 term (alpha6 + 2).
};

/// The standard D3 zero-damping parameter set of one method family
/// (transcribed from the published D3 parameter data).
/// \param name One of "b3lyp", "pbe", "pbe0", "bp86" (the b-p set),
/// "blyp", "revpbe", "pbesol", "b97d", "tpss", "bpbe", "bhandhlyp",
/// "b3pw91", "hf".
/// \returns The parameter set, or kInvalidArgument for an unknown name.
/// \ingroup excgrid-d3
Result<D3Parameters> D3Preset(std::string_view name);

/// The D3 dispersion energy of the geometry, in Hartree.
/// \param geometry The molecule.
/// \param parameters The functional's parameter set.
/// \returns The energy (negative, attractive), or kUnsupported for an
/// element above the table's range.
/// \ingroup excgrid-d3
Result<double> GrimmeD3Energy(const Geometry& geometry, const D3Parameters& parameters);

/// The D3 energy and its analytic coordinate gradient.
/// \param geometry The molecule.
/// \param parameters The functional's parameter set.
/// \returns The energy (Hartree) and the gradient (one 3-vector per atom,
/// Hartree/Bohr), or kUnsupported for an out-of-table element.
/// The energy plus the analytic coordinate gradient, together.
/// \ingroup excgrid-d3
struct D3Result {
    double energy = 0.0; ///< The dispersion energy (Hartree).
    std::vector<std::array<double, 3>> gradient; ///< One 3-vector per atom (Hartree/Bohr).
};

/// The D3 energy and its analytic coordinate gradient in one pass.
/// \param geometry The molecule.
/// \param parameters The functional's parameter set.
/// \returns The energy and the gradient, or kUnsupported for an
/// out-of-table element.
/// \ingroup excgrid-d3
Result<D3Result> GrimmeD3(const Geometry& geometry, const D3Parameters& parameters);

} // namespace excgrid
