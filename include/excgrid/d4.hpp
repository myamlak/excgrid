#pragma once

#include "excgrid/error.hpp"
#include "excgrid/geometry.hpp"

#include <span>
#include <string_view>
#include <vector>

namespace excgrid {

/// \defgroup excgrid-d4 Grimme D4 dispersion
///
/// The DFT-D4 charge- and coordination-number-dependent London dispersion
/// correction, geometry-only (Caldeweyher, Ehlert, Hansen, Neugebauer, Spicher,
/// Bannwarth, Grimme, J. Chem. Phys. 150 (2019) 154122).  This group ships the
/// two-body term and the parameter data it consumes; the Axilrod-Teller-Muto
/// three-body term is not here.  The reference-interpolation widths are fitted
/// to published reference energies, not transcribed from the paper.
/// \{

/// The D4 parameters of one functional, in its "bj-eeq-atm" form.
/// \ingroup excgrid-d4
struct D4Parameters {
    double s6 = 1.0; ///< The dipole-dipole scale (1.0 in the default model).
    double s8 = 0.0; ///< The dipole-quadrupole scale (fitted per functional).
    double a1 = 0.0; ///< The Becke-Johnson damping scale (fitted per functional).
    double a2 = 0.0; ///< The Becke-Johnson damping offset (Bohr; fitted per functional).
    double s9 = 1.0; ///< The Axilrod-Teller-Muto three-body scale.
    double alp = 16.0; ///< The three-body zero-damping exponent.
};

/// The published D4 parameter set of one method family.
/// \param name The method key as the D4 tabulation names it ("b3lyp", "pbe",
/// "pbe0", "tpss", "b97d", "hf", ...; 107 shipped families); the keys need not
/// match the D3 preset names.
/// \returns The parameter set, or kInvalidArgument for an unknown name.
/// \ingroup excgrid-d4
Result<D4Parameters> D4Preset(std::string_view name);

/// The largest atomic number the published D4 parameterization covers.
/// \ingroup excgrid-d4
inline constexpr int kD4MaxAtomicNumber = 86;

/// Whether the published D4 parameterization covers an element.
/// \param atomicNumber The atomic number to test.
/// \returns True for 1 <= Z <= kD4MaxAtomicNumber, false otherwise.
/// \ingroup excgrid-d4
bool D4CoversElement(int atomicNumber);

/// One element's published D4 electron-equilibration parameters, in atomic
/// units; the same set serves every method family.
/// \ingroup excgrid-d4
struct D4EeqElement {
    double electronegativity = 0.0; ///< The atomic electronegativity EN (Hartree).
    double hardness = 0.0; ///< The element-dependent atomic hardness J (Hartree).
    double coordinationScale = 0.0; ///< The element-specific scaling parameter kappa (dimensionless).
    double radius = 0.0; ///< The atomic radius a (Bohr).
};

/// The published D4 electron-equilibration parameters of one element.
/// \param atomicNumber The atomic number to look up.
/// \returns The element's parameters, or kUnsupported for an uncovered atomic
/// number (see D4CoversElement).
/// \ingroup excgrid-d4
Result<D4EeqElement> D4EeqParameters(int atomicNumber);

/// The covalent radius the D4 coordination-number counting function uses, in
/// Bohr: the published single-bond radius (Pyykkö, Atsumi, Chem. Eur. J. 15
/// (2009) 186) scaled by the model's 4/3 factor.
/// \param atomicNumber The atomic number to look up.
/// \returns The radius, or kUnsupported for an uncovered atomic number.
/// \ingroup excgrid-d4
Result<double> D4CovalentRadius(int atomicNumber);

/// The Pauling electronegativity of one element, dimensionless: the factor the
/// coordination number's electronegativity difference consumes.
/// \param atomicNumber The atomic number to look up.
/// \returns The electronegativity, or kUnsupported for an uncovered atomic number.
/// \ingroup excgrid-d4
Result<double> D4PaulingElectronegativity(int atomicNumber);

/// The D4 coordination numbers of a geometry, one per atom in input order: the
/// geometry-only counting function, each pair damped by its electronegativity
/// difference.
/// \param geometry The molecule, its positions in Bohr.
/// \returns The coordination numbers, or kUnsupported for an element above the
/// table's range.
/// \ingroup excgrid-d4
Result<std::vector<double>> D4CoordinationNumbers(const Geometry& geometry);

/// The electronegativity-equilibration partial charges of a geometry, one per
/// atom in input order, in units of the elementary charge; they sum to
/// \p totalCharge to machine precision.
/// \param geometry The molecule, its positions in Bohr.
/// \param totalCharge The system's total charge, in units of the elementary charge.
/// \returns The partial charges, or kUnsupported for an element above the
/// table's range, or kInvalidArgument where the kernel cannot be solved.
/// \ingroup excgrid-d4
Result<std::vector<double>> D4PartialCharges(const Geometry& geometry, double totalCharge);

/// The element-specific chemical hardness of the D4 charge-scaling function, in
/// Hartree: it sets the steepness of the scaling.
/// \param atomicNumber The atomic number to look up.
/// \returns The hardness, or kUnsupported for an uncovered atomic number.
/// \ingroup excgrid-d4
Result<double> D4ChemicalHardness(int atomicNumber);

/// The D4 charge-scaling factor of one atom's reference polarizabilities: unity
/// at the reference charge, rising as the atom gains electrons.
/// \param atomicNumber The atomic number, which selects the chemical hardness.
/// \param effectiveNuclearCharge The atom's nuclear charge plus its partial charge.
/// \param referenceEffectiveNuclearCharge The effective nuclear charge of the
/// reference system being scaled.
/// \returns The factor, or kUnsupported for an uncovered atomic number, or
/// kInvalidArgument for a non-positive effective nuclear charge.
/// \ingroup excgrid-d4
Result<double> D4ChargeScaling(int atomicNumber, double effectiveNuclearCharge,
                               double referenceEffectiveNuclearCharge);

/// The effective nuclear charge the D4 charge scaling works in: Z up to krypton,
/// the tabulated ECP-stripped charge beyond it, plus the atom's partial charge.
/// \param atomicNumber The atomic number.
/// \param partialCharge The atom's partial charge, in units of the elementary charge.
/// \returns The effective nuclear charge, or kUnsupported for an uncovered
/// atomic number.
/// \ingroup excgrid-d4
Result<double> D4EffectiveNuclearCharge(int atomicNumber, double partialCharge);

/// The number of points in the model's Casimir-Polder frequency grid.
/// \ingroup excgrid-d4
inline constexpr int kD4FrequencyPointCount = 23;

/// The frequencies the model's Casimir-Polder relation integrates over, in
/// Hartree, strictly increasing from just above zero; one grid serves every
/// element.
/// \returns The kD4FrequencyPointCount grid frequencies, in this order.
/// \ingroup excgrid-d4
std::span<const double> D4FrequencyGrid();

/// The C6 coefficient two atoms' dynamic polarizabilities give: the model's
/// Casimir-Polder relation, trapezium-rule over D4FrequencyGrid.
/// \param alphaA The first atom's isotropically averaged dynamic
/// polarizabilities, in atomic units, one per grid frequency in grid order.
/// \param alphaB The second atom's, likewise.
/// \returns The C6 coefficient, or kInvalidArgument unless both spans hold
/// exactly kD4FrequencyPointCount values.
/// \ingroup excgrid-d4
Result<double> D4CasimirPolderC6(std::span<const double> alphaA,
                                 std::span<const double> alphaB);

/// The D4 reference weighting of one atom's reference polarizabilities: a
/// Gaussian sum in the coordination-number difference per reference system.
/// \param coordinationNumber The atom's coordination number.
/// \param referenceCoordinationNumbers The reference coordination numbers of
/// the reference systems of the atom's element.
/// \param gaussianCounts The number of Gaussian functions of each reference
/// system, one per entry of referenceCoordinationNumbers, each at least 1.
/// \returns Each reference system's weight, in the same order, summing to
/// unity; or kInvalidArgument for empty or mismatched inputs, a count below
/// one, or a coordination number too far from every reference.
/// \ingroup excgrid-d4
Result<std::vector<double>> D4ReferenceWeights(double coordinationNumber,
                                               std::span<const double> referenceCoordinationNumbers,
                                               std::span<const int> gaussianCounts);

/// The D4 two-body dispersion energy of a geometry, in Hartree: equations 18 to
/// 21 of the model, summed over the unordered pairs, each once.
/// \param geometry The molecule, its positions in Bohr.
/// \param parameters The functional's parameter set (it supplies s6, s8, a1
/// and a2; s9 and alp do not enter a two-body energy).
/// \returns The energy (negative for an attractive interaction), or kUnsupported
/// for an element above the reference data's range.
/// \ingroup excgrid-d4
Result<double> D4TwoBodyEnergy(const Geometry& geometry, const D4Parameters& parameters);

/// The number of reference states: the length a caller's Gaussian-count array
/// must have.
/// \ingroup excgrid-d4
inline constexpr int kD4ReferenceStateCount = 261;

/// The D4 two-body dispersion energy, with equation 8's Gaussian counts supplied
/// by the caller.  The counts are the model's one unpublished input, so the set
/// this library ships was fitted to reference energies rather than transcribed.
/// \param geometry The molecule, its positions in Bohr.
/// \param parameters The functional's parameter set.
/// \param gaussianCounts One count per reference state, in the reference
/// table's order (increasing atomic number, then the source's reference order).
/// \returns The energy, or kUnsupported for an element above the reference
/// data's range, or kInvalidArgument unless gaussianCounts holds exactly
/// kD4ReferenceStateCount entries.
/// \ingroup excgrid-d4
Result<double> D4TwoBodyEnergy(const Geometry& geometry, const D4Parameters& parameters,
                               std::span<const int> gaussianCounts);

/// The dispersion correction a method key selects.
/// \ingroup excgrid-d4
enum class DispersionModel {
    kD3, ///< The D3 zero-damping scheme of GrimmeD3.
    kD4, ///< The D4 default model of D4Preset.
};

/// The dispersion correction a method key selects.
/// \param key "d3" for the D3 zero-damping scheme, "d4" for the D4
/// default model.
/// \returns The model, or kInvalidArgument for an unknown key.
/// \ingroup excgrid-d4
Result<DispersionModel> DispersionModelFromKey(std::string_view key);

} // namespace excgrid
