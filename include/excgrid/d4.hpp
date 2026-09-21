#pragma once

#include "excgrid/error.hpp"
#include "excgrid/geometry.hpp"

#include <span>
#include <string_view>
#include <vector>

namespace excgrid {

/// \defgroup excgrid-d4 Grimme D4 dispersion
///
/// The DFT-D4 dispersion correction (Caldeweyher, Ehlert, Hansen,
/// Neugebauer, Spicher, Bannwarth, Grimme, J. Chem. Phys. 150 (2019)
/// 154122): a charge- and coordination-number-dependent London
/// dispersion correction, geometry-only, over the library's own
/// Geometry type.  Its default model pairs Becke-Johnson damping over
/// the two-body terms with EEQ partial charges and an
/// Axilrod-Teller-Muto three-body term.
///
/// **This group ships the model's parameter data, the two-body term of its
/// energy, and the reference-interpolation widths that term consumes.**  The
/// per-functional parameters, the EEQ element parameters, the reference
/// states the C6 step interpolates over and the frequency grid that step
/// integrates on are all here, each transcribed mechanically and committed
/// under tools/data/ (see tools/gen_d4_tables.py, tools/gen_d4_eeq.py and
/// tools/gen_d4_reference.py).  The widths are not: the paper gives the rule
/// for choosing them and prints no value, so this library fits them to
/// published reference energies and says so wherever it names them
/// (`src/d4_reference_counts.inc`, and the D4 section of
/// THIRD_PARTY_NOTICES.md).  They are derived data, not transcribed data.
/// The three-body Axilrod-Teller-Muto term is not here.
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
/// \param name The method key as the D4 tabulation names it ("b3lyp",
/// "pbe", "pbe0", "tpss", "b97d", "hf", ...; 107 shipped families).
/// The keys are that tabulation's own, so they agree with it and not
/// necessarily with the D3 preset names - the same functional can be
/// "bp86" to D3Preset and "bp" here.
/// \returns The parameter set, or kInvalidArgument for an unknown name.
/// \ingroup excgrid-d4
Result<D4Parameters> D4Preset(std::string_view name);

/// The largest atomic number the published D4 parameterization covers.
///
/// The model's element parameters are tabulated up to radon (Z = 86).
/// \ingroup excgrid-d4
inline constexpr int kD4MaxAtomicNumber = 86;

/// Whether the published D4 parameterization covers an element.
/// \param atomicNumber The atomic number to test.
/// \returns True for 1 <= Z <= kD4MaxAtomicNumber; false otherwise, which
/// is the refusal a caller must honour rather than extrapolating a table
/// the model does not define.
/// \ingroup excgrid-d4
bool D4CoversElement(int atomicNumber);

/// One element's published D4 electron-equilibration parameters, in
/// atomic units.
///
/// These are the four parameters the model's electronegativity
/// equilibration is built from; none of them is a functional-specific
/// quantity, so the same set serves every method family.
/// \ingroup excgrid-d4
struct D4EeqElement {
    double electronegativity = 0.0; ///< The atomic electronegativity EN (Hartree).
    double hardness = 0.0; ///< The element-dependent atomic hardness J (Hartree).
    double coordinationScale = 0.0; ///< The element-specific scaling parameter kappa (dimensionless).
    double radius = 0.0; ///< The atomic radius a (Bohr).
};

/// The published D4 electron-equilibration parameters of one element.
/// \param atomicNumber The atomic number to look up.
/// \returns The element's parameters, or kUnsupported for an atomic
/// number the published parameterization does not cover (see
/// D4CoversElement), which is the refusal a caller must honour rather
/// than extrapolating a table the model does not define.
/// \ingroup excgrid-d4
Result<D4EeqElement> D4EeqParameters(int atomicNumber);

/// The covalent radius the D4 coordination-number counting function
/// uses, in Bohr.
///
/// This is the published single-bond radius of the element (Pyykkö and
/// Atsumi, Chem. Eur. J. 15 (2009) 186-197, the model's ref 48) in the
/// counting convention the model uses.  The published radius and the
/// conventional 4/3-scaled one differ by that factor; the scaled one is
/// returned, and it is the one under which the counting function
/// reproduces the model's published reference coordination numbers.
/// \param atomicNumber The atomic number to look up.
/// \returns The radius, or kUnsupported for an atomic number the
/// published parameterization does not cover.
/// \ingroup excgrid-d4
Result<double> D4CovalentRadius(int atomicNumber);

/// The Pauling electronegativity of one element.
///
/// The dimensionless value the coordination number's
/// electronegativity-difference factor consumes (the model's ref 47).
/// \param atomicNumber The atomic number to look up.
/// \returns The electronegativity, or kUnsupported for an atomic number
/// the published parameterization does not cover.
/// \ingroup excgrid-d4
Result<double> D4PaulingElectronegativity(int atomicNumber);

/// The D4 coordination numbers of a geometry, one per atom in input
/// order.
///
/// The geometry-only counting function of the model: each pair of atoms
/// contributes through their distance and their covalent radii, damped by
/// the pair's electronegativity difference.  It is a property of the
/// geometry alone - no charges or parameters enter it - and every step
/// that follows in the model consumes it.
/// \param geometry The molecule, its positions in Bohr.
/// \returns The coordination numbers, or kUnsupported for an element
/// above the table's range.
/// \ingroup excgrid-d4
Result<std::vector<double>> D4CoordinationNumbers(const Geometry& geometry);

/// The electronegativity-equilibration partial charges of a geometry,
/// one per atom in input order, in units of the elementary charge.
///
/// The classical charge model whose charges scale the reference
/// polarizabilities: the constrained minimum of the model's
/// electronegativity-equilibration energy, over the screened Coulomb
/// kernel of Gaussian atomic densities and the element parameters of
/// the published Table A1.  The charges sum to the total charge handed
/// in, to machine precision.
/// \param geometry The molecule, its positions in Bohr.
/// \param totalCharge The system's total charge, in units of the
/// elementary charge.
/// \returns The partial charges, or kUnsupported for an element above
/// the table's range, or kInvalidArgument where the kernel cannot be
/// solved.
/// \ingroup excgrid-d4
Result<std::vector<double>> D4PartialCharges(const Geometry& geometry, double totalCharge);

/// The element-specific chemical hardness of the D4 charge-scaling
/// function, in Hartree.
///
/// The non-fitted parameter that sets the steepness of the scaling the
/// model applies to an atom's reference polarizabilities (the global
/// atomic hardnesses of Ghosh and Islam, the model's ref 42).
/// \param atomicNumber The atomic number to look up.
/// \returns The hardness, or kUnsupported for an atomic number the
/// published parameterization does not cover.
/// \ingroup excgrid-d4
Result<double> D4ChemicalHardness(int atomicNumber);

/// The D4 charge-scaling factor of one atom's reference polarizabilities.
///
/// The factor is unity when the atom's effective nuclear charge equals the
/// reference system's, and rises above unity as the atom gains electrons
/// relative to the reference; it scales a reference polarizability as
/// alpha(iw, z) = alpha(iw) * D4ChargeScaling(...).  The model's atom-in-
/// molecule polarizability is a reference-count-weighted sum of such
/// scaled reference polarizabilities.
/// \param atomicNumber The atomic number, which selects the chemical
/// hardness that sets the factor's steepness.
/// \param effectiveNuclearCharge The atom's nuclear charge plus its
/// partial charge.
/// \param referenceEffectiveNuclearCharge The effective nuclear charge of
/// the element-specific reference system whose polarizabilities are being
/// scaled.
/// \returns The factor, or kUnsupported for an atomic number the
/// published parameterization does not cover, or kInvalidArgument for a
/// non-positive effective nuclear charge.
/// \ingroup excgrid-d4
Result<double> D4ChargeScaling(int atomicNumber, double effectiveNuclearCharge,
                               double referenceEffectiveNuclearCharge);

/// The effective nuclear charge the D4 charge scaling works in.
///
/// The scaling function compares an atom's charge state against the
/// reference system's, and both are measured in this charge rather than in
/// the atomic number.  Up to krypton it is the atomic number, as it is for
/// every element whose reference data was computed in an all-electron
/// treatment; beyond krypton it is the tabulated effective nuclear charge,
/// which strips the core the reference calculations absorbed into an
/// effective core potential.  The atom's own charge is added to it.
/// \param atomicNumber The atomic number.
/// \param partialCharge The atom's partial charge, in units of the
/// elementary charge.
/// \returns The effective nuclear charge, or kUnsupported for an atomic
/// number the published parameterization does not cover.
/// \ingroup excgrid-d4
Result<double> D4EffectiveNuclearCharge(int atomicNumber, double partialCharge);

/// The number of points in the model's Casimir-Polder frequency grid.
/// \ingroup excgrid-d4
inline constexpr int kD4FrequencyPointCount = 23;

/// The frequencies the model's Casimir-Polder relation integrates over,
/// in Hartree.
///
/// One grid serves every element.  It is strictly increasing and starts
/// just above zero, so a caller's polarizabilities must be given at these
/// abscissae, in this order, for D4CasimirPolderC6 to consume them.
/// \returns The kD4FrequencyPointCount grid frequencies.
/// \ingroup excgrid-d4
std::span<const double> D4FrequencyGrid();

/// The C6 coefficient two atoms' dynamic polarizabilities give.
///
/// The Casimir-Polder relation of the model, evaluated over
/// D4FrequencyGrid by the trapezium rule.  It is a property of the two
/// polarizability sets alone: charge scaling, reference interpolation and
/// coordination number reach it only through the polarizabilities a caller
/// passes in.
/// \param alphaA The first atom's isotropically averaged dynamic
/// polarizabilities, in atomic units, one per grid frequency in grid order.
/// \param alphaB The second atom's, likewise.
/// \returns The C6 coefficient, or kInvalidArgument unless both spans hold
/// exactly kD4FrequencyPointCount values.
/// \ingroup excgrid-d4
Result<double> D4CasimirPolderC6(std::span<const double> alphaA,
                                 std::span<const double> alphaB);

/// The D4 reference weighting of one atom's reference polarizabilities.
///
/// The model builds an atom's polarizabilities by weighting the reference
/// systems of its element, each weight a sum of Gaussian functions in the
/// difference between the atom's coordination number and the reference's.
/// How many functions a reference system carries is a property of that
/// reference system, not of the element or of the molecule: the paper's
/// worked example gives a single function where the reference coordination
/// numbers are far apart, and enlarges the set where they lie close enough
/// that one Gaussian cannot separate them.  That example sets out the rule
/// and gives no value for the enlarged count, so the count is a parameter
/// here rather than a constant this library would have to invent.
/// \param coordinationNumber The atom's coordination number.
/// \param referenceCoordinationNumbers The reference coordination numbers of
/// the reference systems of the atom's element.
/// \param gaussianCounts The number of Gaussian functions of each reference
/// system, one per entry of referenceCoordinationNumbers, each at least 1.
/// \returns Each reference system's weight, in the same order, summing to
/// unity; or kInvalidArgument for empty or mismatched inputs, a count below
/// one, or a coordination number so far from every reference that no weight
/// is representable.
/// \ingroup excgrid-d4
Result<std::vector<double>> D4ReferenceWeights(double coordinationNumber,
                                               std::span<const double> referenceCoordinationNumbers,
                                               std::span<const int> gaussianCounts);

/// The D4 two-body dispersion energy of a geometry, in Hartree.
///
/// The dipole-dipole and dipole-quadrupole terms of the model's equations 18
/// to 21: each pair's C6 from the Casimir-Polder relation over both atoms'
/// interpolated polarizabilities, its C8 from the earlier D3 scheme's
/// recursive relation, and Becke-Johnson damping with the functional's own
/// a1 and a2.  The pair sum runs over ordered pairs, as the published
/// expression's notation does, so an unordered pair enters twice.
///
/// The reference-interpolation widths enter through D4ReferenceWeights; the
/// no-count overload uses the fitted table this library ships, and the
/// counted overload uses the caller's.
/// \param geometry The molecule, its positions in Bohr.
/// \param parameters The functional's parameter set (it supplies s6, s8, a1
/// and a2; s9 and alp do not enter a two-body energy).
/// \returns The energy (Hartree; negative for an attractive interaction), or
/// kUnsupported for an element above the reference data's range, or
/// kInvalidArgument for a count array of the wrong length.
/// \ingroup excgrid-d4
Result<double> D4TwoBodyEnergy(const Geometry& geometry, const D4Parameters& parameters);

/// The number of reference states the model's reference table carries, and
/// so the length a caller's own Gaussian-count array must have.
/// \ingroup excgrid-d4
inline constexpr int kD4ReferenceStateCount = 261;

/// The D4 two-body dispersion energy, with the Gaussian counts of equation 8
/// supplied by the caller.
///
/// The counts are the model's one unpublished input: the paper gives the rule
/// for choosing them and prints no value, so the set this library ships was
/// fitted to published reference energies rather than transcribed.  A caller
/// who has their own set can evaluate against it here.
/// \param geometry The molecule, its positions in Bohr.
/// \param parameters The functional's parameter set.
/// \param gaussianCounts One count per reference state, in the reference
/// table's own order (elements in increasing atomic number, and within an
/// element the source's reference order).
/// \returns The energy, or kUnsupported for an element above the reference
/// data's range, or kInvalidArgument unless gaussianCounts holds exactly
/// kD4ReferenceStateCount entries.
/// \ingroup excgrid-d4
Result<double> D4TwoBodyEnergy(const Geometry& geometry, const D4Parameters& parameters,
                               std::span<const int> gaussianCounts);

/// The dispersion correction a method key selects.
///
/// The key is the correction's own name, so that a caller's configuration
/// file reads the way the literature reads.  The two models have separate
/// parameter surfaces - D3 takes D3Parameters, D4 takes D4Parameters - and
/// this type selects between them without coupling them to each other.
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
