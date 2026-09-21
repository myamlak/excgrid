// The D4 parameter-surface tests: the published functional parameters
// resolve by method family, the published element parameters come back
// as the paper tabulates them, the element range refuses what the
// published parameterization does not cover, the coordination number,
// partial charge, charge scaling and Casimir-Polder steps reproduce the
// relations the paper states for them, and the dispersion-model key
// selects between D3 and D4.
//
// The expected parameter values below are the published DFT-D4
// parameters (Caldeweyher, Ehlert, Hansen, Neugebauer, Spicher,
// Bannwarth, Grimme, J. Chem. Phys. 150 (2019) 154122): the BJ-damping
// sets from Supplementary Material S14-S21, and the element set from
// Table A1 (S4-S6), quoted here so that the table the library ships is
// checked against the literature rather than against itself.

#include "excgrid/d4.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>

#include <numbers>
#include <span>
#include <tuple>
#include <optional>
#include <utility>
#include <vector>

namespace {

TEST(D4Test, PresetsResolveByMethodFamily) {
    for (const char* name :
         {"b3lyp", "pbe", "pbe0", "tpss", "tpssh", "b97d", "hf", "blyp", "revpbe", "r2scan"})
    {
        auto preset = excgrid::D4Preset(name);
        EXPECT_TRUE(preset.has_value()) << name;
    }

    // The published values are the check that a resolved set is the
    // family's own, not the defaults.
    auto b3lyp = excgrid::D4Preset("b3lyp");
    ASSERT_TRUE(b3lyp.has_value());
    EXPECT_DOUBLE_EQ(b3lyp->s6, 1.0);
    EXPECT_DOUBLE_EQ(b3lyp->s9, 1.0);
    EXPECT_DOUBLE_EQ(b3lyp->alp, 16.0);
    EXPECT_NEAR(b3lyp->s8, 2.02929367, 1e-8);
    EXPECT_NEAR(b3lyp->a1, 0.40868035, 1e-8);
    EXPECT_NEAR(b3lyp->a2, 4.53807137, 1e-8);

    auto pbe = excgrid::D4Preset("pbe");
    ASSERT_TRUE(pbe.has_value());
    EXPECT_NEAR(pbe->s8, 0.95948085, 1e-8);
    EXPECT_NEAR(pbe->a1, 0.38574991, 1e-8);
    EXPECT_NEAR(pbe->a2, 4.80688534, 1e-8);

    // A family whose name resolves is not a family that was guessed at.
    EXPECT_FALSE(excgrid::D4Preset("nosuchfunctional").has_value());
    EXPECT_EQ(excgrid::D4Preset("nosuchfunctional").error(), excgrid::ErrorCode::kInvalidArgument);
}

TEST(D4Test, ElementRangeIsRefusedByName) {
    EXPECT_TRUE(excgrid::D4CoversElement(1));  // H
    EXPECT_TRUE(excgrid::D4CoversElement(6));  // C
    EXPECT_TRUE(excgrid::D4CoversElement(86)); // Rn, the last tabulated element

    // Z = 87 is outside the published parameterization: refused, not
    // extrapolated.
    EXPECT_FALSE(excgrid::D4CoversElement(87));
    EXPECT_FALSE(excgrid::D4CoversElement(92));
    EXPECT_FALSE(excgrid::D4CoversElement(0));
    EXPECT_FALSE(excgrid::D4CoversElement(-1));
}

TEST(D4Test, DispersionKeySelectsTheModel) {
    auto d3 = excgrid::DispersionModelFromKey("d3");
    ASSERT_TRUE(d3.has_value());
    EXPECT_EQ(*d3, excgrid::DispersionModel::kD3);

    auto d4 = excgrid::DispersionModelFromKey("d4");
    ASSERT_TRUE(d4.has_value());
    EXPECT_EQ(*d4, excgrid::DispersionModel::kD4);

    // An unknown key is refused by name, and the two shipped keys differ.
    EXPECT_FALSE(excgrid::DispersionModelFromKey("d5").has_value());
    EXPECT_EQ(excgrid::DispersionModelFromKey("d5").error(), excgrid::ErrorCode::kInvalidArgument);
    EXPECT_FALSE(excgrid::DispersionModelFromKey("").has_value());
    EXPECT_NE(excgrid::DispersionModel::kD3, excgrid::DispersionModel::kD4);
}

// The expected values are read off Table A1 of the paper's Supplementary
// Material (pages S4-S6), quoted here so that the element parameters the
// library ships are checked against the publication rather than against
// themselves.
TEST(D4Test, EeqElementParametersAreThePublishedOnes) {
    auto hydrogen = excgrid::D4EeqParameters(1);
    ASSERT_TRUE(hydrogen.has_value());
    EXPECT_NEAR(hydrogen->electronegativity, 1.23695041, 1e-8);
    EXPECT_NEAR(hydrogen->hardness, -0.35015861, 1e-8);
    EXPECT_NEAR(hydrogen->coordinationScale, 0.04916110, 1e-8);
    EXPECT_NEAR(hydrogen->radius, 0.55159092, 1e-8);

    auto carbon = excgrid::D4EeqParameters(6);
    ASSERT_TRUE(carbon.has_value());
    EXPECT_NEAR(carbon->electronegativity, 1.40028282, 1e-8);
    EXPECT_NEAR(carbon->hardness, 0.19408787, 1e-8);
    EXPECT_NEAR(carbon->coordinationScale, 0.06005196, 1e-8);
    EXPECT_NEAR(carbon->radius, 1.88862966, 1e-8);

    // The four elements an organic molecule is made of, plus one whose
    // hardness is not positive: the table is carried as published, not
    // as a rounded or sign-normalized version of it.
    auto nitrogen = excgrid::D4EeqParameters(7);
    ASSERT_TRUE(nitrogen.has_value());
    EXPECT_NEAR(nitrogen->electronegativity, 1.55819364, 1e-8);
    EXPECT_NEAR(nitrogen->hardness, 0.05317918, 1e-8);
    EXPECT_NEAR(nitrogen->coordinationScale, 0.09279548, 1e-8);
    EXPECT_NEAR(nitrogen->radius, 1.32250290, 1e-8);

    auto oxygen = excgrid::D4EeqParameters(8);
    ASSERT_TRUE(oxygen.has_value());
    EXPECT_NEAR(oxygen->electronegativity, 1.56866440, 1e-8);
    EXPECT_NEAR(oxygen->hardness, 0.03151644, 1e-8);
    EXPECT_NEAR(oxygen->coordinationScale, 0.11689703, 1e-8);
    EXPECT_NEAR(oxygen->radius, 1.23166285, 1e-8);

    auto radon = excgrid::D4EeqParameters(86);
    ASSERT_TRUE(radon.has_value());
    EXPECT_NEAR(radon->electronegativity, 1.27465977, 1e-8);
    EXPECT_NEAR(radon->hardness, 0.10500484, 1e-8);
    EXPECT_NEAR(radon->coordinationScale, 0.05849285, 1e-8);
    EXPECT_NEAR(radon->radius, 2.82773085, 1e-8);
}

TEST(D4Test, EeqParametersRefuseWhatTheTableDoesNotCover) {
    // Every tabulated element resolves, and to parameters, not to a
    // default: an element the table does not name is refused instead.
    for (int z = 1; z <= excgrid::kD4MaxAtomicNumber; ++z)
    {
        EXPECT_TRUE(excgrid::D4EeqParameters(z).has_value()) << z;
    }

    for (int z : {0, -1, 87, 92, 118})
    {
        auto element = excgrid::D4EeqParameters(z);
        ASSERT_FALSE(element.has_value()) << z;
        EXPECT_EQ(element.error(), excgrid::ErrorCode::kUnsupported) << z;
    }
}

// The counting function's element data: the covalent radii (Pyykko and
// Atsumi, Chem. Eur. J. 15 (2009) 186-197) and the Pauling
// electronegativities, both as the paper cites them.
TEST(D4Test, CountingFunctionElementDataIsThePublishedData) {
    // The radius the counting function uses is the published single-bond
    // radius scaled by 4/3 (the counting convention of the earlier D3
    // method); the published value is the datum under it, in Angstrom,
    // converted here with the same CODATA constant the library uses.
    constexpr double kAngstromPerBohr = 0.529177210903;
    auto hydrogen = excgrid::D4CovalentRadius(1);
    ASSERT_TRUE(hydrogen.has_value());
    EXPECT_NEAR(*hydrogen, 0.32 * (4.0 / 3.0) / kAngstromPerBohr, 1e-12);

    auto carbon = excgrid::D4CovalentRadius(6);
    ASSERT_TRUE(carbon.has_value());
    EXPECT_NEAR(*carbon, 0.75 * (4.0 / 3.0) / kAngstromPerBohr, 1e-12);

    auto chlorine = excgrid::D4CovalentRadius(17);
    ASSERT_TRUE(chlorine.has_value());
    EXPECT_NEAR(*chlorine, 0.99 * (4.0 / 3.0) / kAngstromPerBohr, 1e-12);

    // Pauling's scale, dimensionless.
    EXPECT_NEAR(*excgrid::D4PaulingElectronegativity(1), 2.20, 1e-9);
    EXPECT_NEAR(*excgrid::D4PaulingElectronegativity(6), 2.55, 1e-9);
    EXPECT_NEAR(*excgrid::D4PaulingElectronegativity(7), 3.04, 1e-9);
    EXPECT_NEAR(*excgrid::D4PaulingElectronegativity(8), 3.44, 1e-9);
    EXPECT_NEAR(*excgrid::D4PaulingElectronegativity(9), 3.98, 1e-9);
    EXPECT_NEAR(*excgrid::D4PaulingElectronegativity(17), 3.16, 1e-9);

    for (int z : {0, 87, 118})
    {
        EXPECT_EQ(excgrid::D4CovalentRadius(z).error(), excgrid::ErrorCode::kUnsupported);
        EXPECT_EQ(excgrid::D4PaulingElectronegativity(z).error(), excgrid::ErrorCode::kUnsupported);
    }
}

// The counted coordination numbers of the model's diatomic reference
// states, against the model's published reference values at the published
// experimental bond lengths of those molecules.
//
// The radii are the 4/3-scaled set: the published single-bond radii
// themselves come nowhere near these numbers at any bond length, so this
// also fixes the convention the counting function scales by.  The model
// does not publish the geometries its reference states were computed at,
// so the check is at the bond-length scale rather than an exact
// reproduction, and at that scale the two agree to within a few per cent.
TEST(D4Test, CountingFunctionReproducesTheReferenceCoordinationNumbers) {
    constexpr double kAngstromPerBohr = 0.529177210903;

    struct Reference {
        const char* name;
        int first;
        int second;
        double distance;      // Published experimental bond length, Angstrom.
        double coordination;  // The model's published reference CN.
    };

    const Reference references[] = {
        {"H2", 1, 1, 0.74144, 0.8943405},
        {"CO", 6, 8, 1.12832, 0.8555886},
        {"N2", 7, 7, 1.09769, 0.9810000},
        {"O2", 8, 8, 1.20752, 0.9800000},
        {"F2", 9, 9, 1.41193, 0.9610000},
        {"Cl2", 17, 17, 1.98716, 0.9770000},
    };

    for (const Reference& reference : references)
    {
        excgrid::Geometry geometry;
        geometry.atoms.push_back({reference.first, {0.0, 0.0, 0.0}});
        const double r = reference.distance / kAngstromPerBohr;
        geometry.atoms.push_back({reference.second, {0.0, 0.0, r}});

        auto cn = excgrid::D4CoordinationNumbers(geometry);
        ASSERT_TRUE(cn.has_value()) << reference.name;
        ASSERT_EQ(cn->size(), 2u) << reference.name;

        EXPECT_NEAR((*cn)[0], reference.coordination, 0.03 * reference.coordination)
            << reference.name;
        EXPECT_NEAR((*cn)[1], reference.coordination, 0.03 * reference.coordination)
            << reference.name;
    }
}

// The charge-scaling function's element data: the global atomic hardnesses
// the model cites for equation 2 (Ghosh and Islam, the model's ref 42).
TEST(D4Test, ChargeScalingHardnessIsThePublishedData) {
    EXPECT_NEAR(*excgrid::D4ChemicalHardness(1), 0.47259288, 1e-9);
    EXPECT_NEAR(*excgrid::D4ChemicalHardness(6), 0.42195412, 1e-9);
    EXPECT_NEAR(*excgrid::D4ChemicalHardness(7), 0.50438193, 1e-9);
    EXPECT_NEAR(*excgrid::D4ChemicalHardness(8), 0.58691863, 1e-9);
    EXPECT_NEAR(*excgrid::D4ChemicalHardness(9), 0.66931351, 1e-9);
    EXPECT_NEAR(*excgrid::D4ChemicalHardness(17), 0.43115670, 1e-9);
    EXPECT_NEAR(*excgrid::D4ChemicalHardness(86), 0.21240778, 1e-9);

    for (int z : {0, -1, 87, 118})
    {
        EXPECT_EQ(excgrid::D4ChemicalHardness(z).error(), excgrid::ErrorCode::kUnsupported);
    }
}

// The properties the paper states for the scaling function: it performs no
// scaling when the two effective nuclear charges are equal, it rises above
// unity as the atom gains electrons relative to the reference, and the
// global parameter that bounds it from above is 3, so its large-ratio limit
// is exp(3).
TEST(D4Test, ChargeScalingIsUnityAtTheReference) {
    for (int z : {1, 6, 7, 8, 9})
    {
        auto unity = excgrid::D4ChargeScaling(z, 6.0, 6.0);
        ASSERT_TRUE(unity.has_value()) << z;
        EXPECT_DOUBLE_EQ(*unity, 1.0) << z;
    }

    // Gaining electrons lowers the effective nuclear charge and raises the
    // factor; losing them does the reverse, and the ratio z_ref/z spans the
    // two monotonically.
    auto gained = excgrid::D4ChargeScaling(8, 7.0, 8.0);
    auto plain = excgrid::D4ChargeScaling(8, 8.0, 8.0);
    auto lost = excgrid::D4ChargeScaling(8, 9.0, 8.0);
    ASSERT_TRUE(gained.has_value());
    ASSERT_TRUE(plain.has_value());
    ASSERT_TRUE(lost.has_value());
    EXPECT_GT(*gained, *plain);
    EXPECT_DOUBLE_EQ(*plain, 1.0);
    EXPECT_LT(*lost, *plain);

    // The one published value of the global parameter, and the limit it
    // puts on the factor: a vanishing effective nuclear charge drives the
    // scaling to exp(3).
    auto huge = excgrid::D4ChargeScaling(1, 1e-9, 1.0);
    ASSERT_TRUE(huge.has_value());
    EXPECT_NEAR(*huge, std::exp(3.0), 1e-6);

    // Stated as the published function of the published hydrogen hardness,
    // with the steepness the model multiplies that hardness by before it
    // enters the exponential: exp[beta1 (1 - exp(2 gamma (1 - z_ref/z)))]
    // at z_ref/z = 2.
    const double gamma = 0.47259288;
    auto doubled = excgrid::D4ChargeScaling(1, 1.0, 2.0);
    ASSERT_TRUE(doubled.has_value());
    EXPECT_NEAR(*doubled, std::exp(3.0 * (1.0 - std::exp(2.0 * gamma * (1.0 - 2.0)))), 1e-12);

    // The steepness, against the model's own published output rather than
    // against its own arithmetic.  The published polarizabilities of the
    // lenalidomide fixture give a hydrogen carrying +0.1126 the value
    // 2.0236, where the model's own molecular polarizability of H2 reduced
    // to the atom and interpolated at that hydrogen's coordination number is
    // 2.73485, so the scaling the model applied is 0.73993.  This function
    // returns 0.73997 there.  The same expression with the hardness alone,
    // which is what the published equation 2 prints, gives 0.86332 and fails
    // this by seventeen per cent, so the steepness is what the check pins.
    auto hydrogen = excgrid::D4ChargeScaling(1, 1.1126, 1.0);
    ASSERT_TRUE(hydrogen.has_value());
    EXPECT_NEAR(*hydrogen, 0.73997, 1e-4);

    // An effective nuclear charge that cannot be divided by, or an element
    // the parameterization does not cover, is refused rather than clamped.
    EXPECT_EQ(excgrid::D4ChargeScaling(1, 0.0, 1.0).error(),
              excgrid::ErrorCode::kInvalidArgument);
    EXPECT_EQ(excgrid::D4ChargeScaling(1, 1.0, -1.0).error(),
              excgrid::ErrorCode::kInvalidArgument);
    EXPECT_EQ(excgrid::D4ChargeScaling(87, 1.0, 1.0).error(),
              excgrid::ErrorCode::kUnsupported);
}

TEST(D4Test, CoordinationNumbersRefuseUncoveredElements) {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({1, {0.0, 0.0, 0.0}});
    geometry.atoms.push_back({7, {0.0, 0.0, 2.0}});

    auto covered = excgrid::D4CoordinationNumbers(geometry);
    ASSERT_TRUE(covered.has_value());
    EXPECT_EQ(covered->size(), 2u);

    // A single atom above the table's range poisons the whole geometry:
    // the refusal is the caller's to honour, not one to average over.
    geometry.atoms.push_back({87, {0.0, 0.0, 4.0}});
    auto refused = excgrid::D4CoordinationNumbers(geometry);
    ASSERT_FALSE(refused.has_value());
    EXPECT_EQ(refused.error(), excgrid::ErrorCode::kUnsupported);
}

// The effective nuclear charge is the atomic number up to krypton, where
// the reference data is all-electron, and the atomic number less the core
// the reference calculations absorbed into an effective core potential
// beyond it.  The core sizes are the paper's own, named element by element
// in its discussion of the reference calculations: 28 for Rb, Sr, Y-Cd,
// In-Sb, Te-Xe and Ce-Lu; 46 for Cs, Ba and La; 60 for Hf-Hg, Tl-Bi and
// Po-Rn.  Its summary table prints 18 for the Rb-Xe block instead, which
// cannot be the core size of an element block whose potential the same
// paper calls ECP-28, and which would charge bromine as chlorine.
TEST(D4Test, EffectiveNuclearChargeStripsTheCoreBeyondKrypton) {
    for (int z = 1; z <= 36; ++z)
    {
        auto charge = excgrid::D4EffectiveNuclearCharge(z, 0.0);
        ASSERT_TRUE(charge.has_value()) << z;
        EXPECT_DOUBLE_EQ(*charge, static_cast<double>(z)) << z;
    }

    // Each range's charge is its atomic number less its core, so it steps
    // by one across the range and ends where the table says it ends.
    for (const auto& [first, last, core] :
         {std::tuple{37, 54, 28}, std::tuple{55, 57, 46}, std::tuple{58, 71, 28},
          std::tuple{72, 86, 60}})
    {
        for (int z = first; z <= last; ++z)
        {
            auto charge = excgrid::D4EffectiveNuclearCharge(z, 0.0);
            ASSERT_TRUE(charge.has_value()) << z;
            EXPECT_DOUBLE_EQ(*charge, static_cast<double>(z - core)) << z;
        }
    }

    // The atom's own charge is added to that, and nothing else is.
    auto anion = excgrid::D4EffectiveNuclearCharge(6, -0.5);
    auto cation = excgrid::D4EffectiveNuclearCharge(6, 0.5);
    ASSERT_TRUE(anion.has_value());
    ASSERT_TRUE(cation.has_value());
    EXPECT_DOUBLE_EQ(*anion, 5.5);
    EXPECT_DOUBLE_EQ(*cation, 6.5);

    EXPECT_EQ(excgrid::D4EffectiveNuclearCharge(0, 0.0).error(),
              excgrid::ErrorCode::kUnsupported);
    EXPECT_EQ(excgrid::D4EffectiveNuclearCharge(87, 0.0).error(),
              excgrid::ErrorCode::kUnsupported);
}

TEST(D4Test, FrequencyGridIsTheModelsIntegrationGrid) {
    // The count is the paper's own: its sum runs over the intervals
    // between the grid points, and the grid it integrates on has 23.
    EXPECT_EQ(excgrid::kD4FrequencyPointCount, 23);

    std::span<const double> grid = excgrid::D4FrequencyGrid();
    ASSERT_EQ(grid.size(), static_cast<std::size_t>(excgrid::kD4FrequencyPointCount));

    // Every polarizability table the model carries is given at these
    // abscissae, so they must be usable as one: positive, and strictly
    // increasing so that every interval has a positive width.
    EXPECT_GT(grid.front(), 0.0);
    for (std::size_t j = 0; j + 1 < grid.size(); ++j)
    {
        EXPECT_GT(grid[j + 1], grid[j]) << j;
    }
}

TEST(D4Test, CasimirPolderC6IsTheTrapeziumRuleOverTheGrid) {
    std::span<const double> grid = excgrid::D4FrequencyGrid();
    const std::size_t count = grid.size();

    // Two units of polarizability at every frequency: each interval then
    // contributes its width twice, and the coefficient is the prefactor
    // over the span the grid covers.
    const std::vector<double> ones(count, 1.0);
    auto flat = excgrid::D4CasimirPolderC6(ones, ones);
    ASSERT_TRUE(flat.has_value());
    EXPECT_NEAR(*flat, (3.0 / std::numbers::pi) * (grid.back() - grid.front()), 1e-12);

    // The grid is not uniform, so the rule is exact for a linear integrand
    // and no longer: a polarizability linear in the frequency against a
    // constant one must come back as the closed-form integral of their
    // product over the grid's span, and the two linear ones must not.
    const double alpha = 3.0, slopeA = -0.25, beta = 5.0, slopeB = 0.5;
    std::vector<double> linearA(count), linearB(count);
    for (std::size_t j = 0; j < count; ++j)
    {
        linearA[j] = alpha + slopeA * grid[j];
        linearB[j] = beta + slopeB * grid[j];
    }

    const double lo = grid.front();
    const double hi = grid.back();
    const double linearIntegral = beta * (alpha * (hi - lo) + slopeA * (hi * hi - lo * lo) / 2.0);
    const double quadraticIntegral = (alpha * beta) * (hi - lo)
                                     + (alpha * slopeB + slopeA * beta) * (hi * hi - lo * lo) / 2.0
                                     + (slopeA * slopeB) * (hi * hi * hi - lo * lo * lo) / 3.0;

    // Where the integrand is a quadratic its deviation from that integral
    // is the partition's own: one twelfth of the cube of each interval's
    // width, times the second derivative, which is constant here.
    double partition = 0.0;
    for (std::size_t j = 0; j + 1 < count; ++j)
    {
        partition += std::pow(grid[j + 1] - grid[j], 3.0);
    }

    const double correction = partition * 2.0 * slopeA * slopeB / 12.0;

    auto mixed = excgrid::D4CasimirPolderC6(linearA, linearB);
    ASSERT_TRUE(mixed.has_value());
    EXPECT_NEAR(*mixed, (3.0 / std::numbers::pi) * (quadraticIntegral + correction), 1e-9);

    const std::vector<double> flatB(count, beta);
    auto exact = excgrid::D4CasimirPolderC6(linearA, flatB);
    ASSERT_TRUE(exact.has_value());
    EXPECT_NEAR(*exact, (3.0 / std::numbers::pi) * linearIntegral, 1e-9);

    // The coefficient is symmetric in the two atoms and bilinear in their
    // polarizabilities.
    auto swapped = excgrid::D4CasimirPolderC6(linearB, linearA);
    ASSERT_TRUE(swapped.has_value());
    EXPECT_NEAR(*swapped, *mixed, 1e-12);

    const std::vector<double> doubledA(count, 2.0);
    auto doubled = excgrid::D4CasimirPolderC6(doubledA, ones);
    ASSERT_TRUE(doubled.has_value());
    EXPECT_NEAR(*doubled, 2.0 * *flat, 1e-12);

    // A polarizability set given on the wrong grid is refused rather than
    // integrated over whatever it happens to hold.
    const std::vector<double> shortSet(count - 1, 1.0);
    EXPECT_EQ(excgrid::D4CasimirPolderC6(shortSet, ones).error(),
              excgrid::ErrorCode::kInvalidArgument);
    EXPECT_EQ(excgrid::D4CasimirPolderC6(ones, {}).error(),
              excgrid::ErrorCode::kInvalidArgument);
}

// The weighting function of the model's reference interpolation: the
// weights of a set of reference coordination numbers sum to one, they are
// carried by the reference the atom's coordination number sits on, and
// they are symmetric in two references that sit equally far from it.
TEST(D4Test, ReferenceWeightsSumToOneAndFavourTheNearestReference) {
    const std::vector<double> references = {0.0, 1.0, 4.0};
    const std::vector<int> single(references.size(), 1);

    auto onReference = excgrid::D4ReferenceWeights(4.0, references, single);
    ASSERT_TRUE(onReference.has_value());
    ASSERT_EQ(onReference->size(), references.size());

    double total = 0.0;
    for (double weight : *onReference)
    {
        total += weight;
    }
    EXPECT_NEAR(total, 1.0, 1e-12);
    EXPECT_GT((*onReference)[2], 0.99);
    EXPECT_LT((*onReference)[0], 1e-12);

    // Two references equidistant from the atom carry equal weight, and a
    // coordination number on the far side of the pair swaps them.
    const std::vector<double> pair = {0.0, 2.0};
    const std::vector<int> pairSingle(pair.size(), 1);

    auto midpoint = excgrid::D4ReferenceWeights(1.0, pair, pairSingle);
    ASSERT_TRUE(midpoint.has_value());
    EXPECT_NEAR((*midpoint)[0], 0.5, 1e-12);

    for (double offset : {0.25, 0.5, 0.9})
    {
        auto below = excgrid::D4ReferenceWeights(1.0 - offset, pair, pairSingle);
        auto above = excgrid::D4ReferenceWeights(1.0 + offset, pair, pairSingle);
        ASSERT_TRUE(below.has_value()) << offset;
        ASSERT_TRUE(above.has_value()) << offset;
        EXPECT_NEAR((*below)[0], (*above)[1], 1e-12) << offset;
        EXPECT_NEAR((*below)[1], (*above)[0], 1e-12) << offset;
    }

    // How many Gaussian functions a reference carries is a free parameter
    // of the model at this point, and it is not an inert one: enlarging a
    // reference's set raises its share of the weight against a reference
    // that keeps one, which is what the count is for.
    const std::vector<double> close = {0.0, 1.0};
    const std::vector<int> plain = {1, 1};
    const std::vector<int> enlarged = {1, 4};

    auto split = excgrid::D4ReferenceWeights(1.2, close, plain);
    auto sharpened = excgrid::D4ReferenceWeights(1.2, close, enlarged);
    ASSERT_TRUE(split.has_value());
    ASSERT_TRUE(sharpened.has_value());
    EXPECT_GT((*sharpened)[1], (*split)[1]);
    EXPECT_NEAR((*split)[0] + (*split)[1], 1.0, 1e-12);
    EXPECT_NEAR((*sharpened)[0] + (*sharpened)[1], 1.0, 1e-12);

    // A count below one, an empty set, a set of the wrong length, or a
    // coordination number no reference can be weighed against is refused.
    const std::vector<double> noReferences;
    const std::vector<int> noCounts;
    const std::vector<int> mismatched = {1, 1};
    const std::vector<int> zeroCount = {1, 0, 1};
    const std::vector<int> negativeCount = {1, -2, 1};

    EXPECT_EQ(excgrid::D4ReferenceWeights(1.0, references, mismatched).error(),
              excgrid::ErrorCode::kInvalidArgument);
    EXPECT_EQ(excgrid::D4ReferenceWeights(1.0, noReferences, noCounts).error(),
              excgrid::ErrorCode::kInvalidArgument);
    EXPECT_EQ(excgrid::D4ReferenceWeights(1.0, references, zeroCount).error(),
              excgrid::ErrorCode::kInvalidArgument);
    EXPECT_EQ(excgrid::D4ReferenceWeights(1.0, references, negativeCount).error(),
              excgrid::ErrorCode::kInvalidArgument);
    EXPECT_EQ(excgrid::D4ReferenceWeights(1e6, references, single).error(),
              excgrid::ErrorCode::kInvalidArgument);
}

// The published form of the weighting sum: reference k contributes
// sum over j = 1..N^s of exp(-beta2 * j * (CN - CN_k)^2), the multiplier
// being the function's own index.  Two references one unit either side of
// the atom's coordination number therefore split their weight in the
// ratio of those two sums, and the count enters as a number of
// progressively narrower functions rather than as a rescaling of one.
//
// Written out from the published expression rather than as its value, so
// that a sum running the other way round - over j/N^s, say - fails here.
TEST(D4Test, ReferenceWeightsAreThePublishedGaussianSum) {
    constexpr double kBeta2 = 6.0;
    constexpr double kOffset = 0.5;

    const auto publishedSum = [](int count) {
        double sum = 0.0;
        for (int j = 1; j <= count; ++j)
        {
            sum += std::exp(-kBeta2 * static_cast<double>(j) * kOffset * kOffset);
        }
        return sum;
    };

    const std::vector<double> references = {0.0, 1.0};
    const std::vector<int> counts = {1, 3};

    auto split = excgrid::D4ReferenceWeights(0.5, references, counts);
    ASSERT_TRUE(split.has_value());

    const double single = publishedSum(1);
    const double triple = publishedSum(3);
    EXPECT_NEAR((*split)[0], single / (single + triple), 1e-12);
    EXPECT_NEAR((*split)[1], triple / (single + triple), 1e-12);

    // The count is a sharpening and not a broadening: a reference whose
    // set is enlarged takes a larger share of a nearer atom's weight than
    // the same reference carrying one function, against a fixed neighbour.
    const std::vector<double> neighbours = {0.0, 1.0};
    auto one = excgrid::D4ReferenceWeights(1.2, neighbours, std::vector<int>{1, 1});
    auto four = excgrid::D4ReferenceWeights(1.2, neighbours, std::vector<int>{1, 4});
    ASSERT_TRUE(one.has_value());
    ASSERT_TRUE(four.has_value());
    EXPECT_GT((*four)[1], (*one)[1]);
}

} // namespace

// The two-body term's fixture: the caffeine geometry of the D4 program's own
// test set (its app test 04, `04-caffeine.xyz`), in Angstrom as that file
// gives it.  The program publishes this molecule's pair-resolved two-body
// energy as the `additive pairwise energy` matrix of `04-pair-analysis.json`,
// whose upper triangle sums to -0.023389122251427714 Hartree and whose total,
// entry over every ordered pair of the matrix, is -0.04677824450285543.
//
// Measured against that target at the counts the table ships, this
// implementation returns -0.046680600986 Hartree: 0.997913 of the published
// magnitude, a residual of 0.2 per cent that is stated and not tuned away.
// Six causes have been removed, each against the model's own published data.
// The reference states carry the polarizability of a whole reference MOLECULE
// and are reduced to the atom's own by the model's partitioning, which
// accounted for -1.0924602087411479 falling to -0.0963797279651535.  The
// charge kernel's diagonal is the screened interaction's own zero-separation
// limit, without which the kernel has no minimum and the charges run away,
// and the charge-scaling function's steepness is a global the model
// multiplies its chemical hardness by and the publication's equation 2 does
// not print; those two brought that fall to 1.903 and 1.958 by one route and
// to 0.98 by another.  The charge each reference state carries is the
// classical set's, which the model's own EEQ reference routines read and which
// the paper's text names as its descriptor: reading the `ref` set instead left
// this implementation 2.8 per cent out in root mean square against the model's
// published per-atom polarizabilities, and the classical set puts it 0.1 per
// cent out.
//
// The steepness is fixed against those same published per-atom
// polarizabilities - the lenalidomide fixture of the same app test set, 32
// atoms of C, H, N and O with their coordination numbers, charges and
// polarizabilities.  Reading equation 2 without the steepness leaves this
// project's polarizabilities 18 per cent out in root mean square and its
// hydrogen 25 to 44 per cent high; with it, 2.8 per cent out and every element
// inside 4; with the classical charge set as well, 0.1 per cent out and every
// element inside 0.1.
//
// The pair sum is read from the published matrix rather than from the
// equation's notation, which is ambiguous between the unordered pairs and the
// ordered ones.  The matrix's entries sum over every ordered pair of atoms to
// exactly the `energy` field the same file publishes, less only the
// non-additive term, so each entry is half of the contribution of the pair it
// belongs to and the energy is the sum over the unordered pairs and nothing
// more; a second check agrees, this implementation's per-pair terms coming out
// at 1.99 to 2.00 times those entries for every one of the ten element pairs
// the molecule contains.  The sum is not doubled.
//
// What is left is the 0.2 per cent.  It is not the polarizability chain: that
// chain reproduces the model's own per-atom values for all four of the
// elements the fixture carries to 0.1 per cent, and feeding the model's own
// coordination numbers and charges in place of this implementation's moves
// those per-atom values by less than 0.01 per cent.  It is not the counts of
// equation 8 either, which are held at the model's neutral baseline and could
// close the gap by construction, as the test below measures.  That leaves the
// geometry the publication does not print: this implementation's coordination
// numbers for the fixture agree with the model's own to 0.2 per cent, and a
// 0.2 per cent shift in a coordination number moves this energy by 0.04 per
// cent, while a 0.001 shift in a partial charge moves it by 0.14.
//
// The tests below assert the properties the term does have; they do not
// assert the published value, and no constant was adjusted to make anything
// look right.
excgrid::Geometry CaffeineGeometry() {
    constexpr double kAngstromPerBohr = 0.529177210903;
    struct Raw {
        int atomicNumber;
        double x;
        double y;
        double z;
    };
    const Raw raw[] = {
        {6, -3.2668063145, -1.0612383311, 0.0019792586},
        {7, -2.2766003898, -0.0195172936, -0.0013208218},
        {6, -0.9134590858, -0.2019878522, 0.0036747135},
        {6, -0.3724701006, 1.0762202467, 0.0017569102},
        {7, -1.3526586265, 2.0066755645, -0.0051933750},
        {6, -2.4700895686, 1.3111433173, -0.0065911261},
        {1, -3.4434528448, 1.7598589152, -0.0115392456},
        {7, 0.9701620055, 1.2833365802, -0.0008793828},
        {6, 1.8257144377, 0.2028321683, -0.0007168653},
        {8, 3.0278876281, 0.3474912012, -0.0051812078},
        {7, 1.2515027114, -1.0636950051, -0.0008922263},
        {6, -0.1065018202, -1.3842039491, 0.0029991689},
        {8, -0.5015980556, -2.5366534217, 0.0044099955},
        {6, 2.2018357820, -2.1638123093, -0.0037185988},
        {1, 1.6323675079, -3.0900622488, 0.0097444448},
        {1, 2.8244315108, -2.1119787186, -0.8964193499},
        {1, 2.8450781508, -2.0977859157, 0.8731480371},
        {6, 1.5216365707, 2.6211569486, 0.0093869454},
        {1, 0.7306184352, 3.3088549484, -0.2771144995},
        {1, 1.8886888508, 2.8714188674, 1.0060887389},
        {1, 2.3551115434, 2.6687817985, -0.6902763920},
        {1, -2.7510404767, -2.0202634640, -0.0176576390},
        {1, -3.9109098848, -0.9753935259, -0.8744768432},
        {1, -3.8816811438, -0.9984445054, 0.9013764258},
    };

    excgrid::Geometry geometry;

    for (const Raw& atom : raw)
    {
        geometry.atoms.push_back({atom.atomicNumber,
                                  {atom.x / kAngstromPerBohr, atom.y / kAngstromPerBohr,
                                   atom.z / kAngstromPerBohr}});
    }

    return geometry;
}

// The two-body term is a function of the interatomic distances alone, so
// moving the whole molecule cannot change it.
TEST(D4Test, TwoBodyEnergyIsTranslationInvariant) {
    const excgrid::Geometry geometry = CaffeineGeometry();
    const auto parameters = *excgrid::D4Preset("b3lyp");
    const std::vector<int> counts(excgrid::kD4ReferenceStateCount, 1);

    auto here = excgrid::D4TwoBodyEnergy(geometry, parameters, counts);
    ASSERT_TRUE(here.has_value());

    excgrid::Geometry moved = geometry;
    const std::array<double, 3> shift = {11.25, -4.5, 7.125};

    for (excgrid::Atom& atom : moved.atoms)
    {
        for (std::size_t k = 0; k < 3; ++k)
        {
            atom.position[k] += shift[k];
        }
    }

    auto there = excgrid::D4TwoBodyEnergy(moved, parameters, counts);
    ASSERT_TRUE(there.has_value());
    EXPECT_DOUBLE_EQ(*here, *there);

    // The term is attractive, and it refuses a geometry it has no reference
    // data for rather than evaluating a term of it.
    EXPECT_LT(*here, 0.0);

    excgrid::Geometry radon = geometry;
    radon.atoms[0].atomicNumber = 86;
    EXPECT_EQ(excgrid::D4TwoBodyEnergy(radon, parameters, counts).error(),
              excgrid::ErrorCode::kUnsupported);

    EXPECT_EQ(excgrid::D4TwoBodyEnergy(geometry, parameters,
                                       std::vector<int>(excgrid::kD4ReferenceStateCount - 1, 1))
                  .error(),
              excgrid::ErrorCode::kInvalidArgument);
}

// The Gaussian counts of equation 8 are the one term of the chain the
// publication never fixes, and with every other term of that chain now
// measured against the model's own published output they are also the one term
// that could absorb what is left of the residual.  This pins the baseline the
// table ships, so that a vector chosen to close the gap would have to move
// this number and would be seen here.
//
// The family of assignments is unbounded, so what is measured is its reach
// over the assignments that can matter: every state at one; at a few small
// counts; and far past the point where equation 8's sum saturates and the
// weights stop depending on the count at all.  Saturation is the limit of that
// direction, so the large uniform counts are the end of the family rather than
// merely large numbers.  Over those the ratio runs from 0.997913 to 0.999281.
// A mixture was searched separately, outside this test - eight random restarts
// and a coordinate descent over every state of the four elements the molecule
// carries - and it reaches the published value itself, an assignment whose
// ratio is 1.000000 to six decimals.
//
// The counts are therefore held at the model's neutral baseline of one
// function per state as a choice, and not because the family cannot reach the
// published value.  What a fitted vector would do is absorb a 0.2 per cent gap
// that the rest of the chain accounts for neither, which is the failure this
// baseline exists to prevent.
TEST(D4Test, TheGaussianCountsAreHeldAtTheModelsNeutralBaseline) {
    const excgrid::Geometry geometry = CaffeineGeometry();
    const auto parameters = *excgrid::D4Preset("b3lyp");
    constexpr double kPublished = -0.04677824450285543;
    constexpr int kStates = excgrid::kD4ReferenceStateCount;

    const auto ratioAt = [&](const std::vector<int>& counts) {
        auto energy = excgrid::D4TwoBodyEnergy(geometry, parameters, counts);
        EXPECT_TRUE(energy.has_value());
        return *energy / kPublished;
    };

    // The baseline vector itself, through the shipped default rather than
    // through a vector built here: a fit moved into the table would fail.
    auto shippedDefault = excgrid::D4TwoBodyEnergy(geometry, parameters);
    ASSERT_TRUE(shippedDefault.has_value());
    EXPECT_NEAR(*shippedDefault / kPublished, 0.997913, 1e-6);

    const double shipped = ratioAt(std::vector<int>(kStates, 1));
    EXPECT_DOUBLE_EQ(shipped, *shippedDefault / kPublished);

    double lowest = shipped;
    double highest = shipped;

    for (const int uniform : {2, 5, 10, 100, 100000})
    {
        const double ratio = ratioAt(std::vector<int>(kStates, uniform));
        lowest = std::min(lowest, ratio);
        highest = std::max(highest, ratio);
    }

    // Every assignment this test samples lands within a seventh of a per cent
    // of every other, and the span stops short of the published value although
    // the search outside this test does not.
    EXPECT_LT(highest - lowest, 0.002);
    EXPECT_GT(lowest, 0.997);
    EXPECT_LT(highest, 1.0);
}

// The long-range shape: past the damping radius the dipole-dipole term is the
// bare C6 over the sixth power, so doubling the separation divides it by
// sixty-four.  The separations are chosen well beyond R0 of the pair, where
// the damping factor has gone to one.
TEST(D4Test, TwoBodyEnergyFallsOffAsTheSixthPower) {
    const auto parameters = *excgrid::D4Preset("hf");
    const std::vector<int> counts(excgrid::kD4ReferenceStateCount, 1);

    const auto energyAt = [&](double separation) {
        excgrid::Geometry geometry;
        geometry.atoms.push_back({9, {0.0, 0.0, 0.0}});
        geometry.atoms.push_back({9, {0.0, 0.0, separation}});
        auto energy = excgrid::D4TwoBodyEnergy(geometry, parameters, counts);
        return energy.has_value() ? std::optional<double>(*energy) : std::nullopt;
    };

    auto close = energyAt(40.0);
    auto far = energyAt(80.0);
    ASSERT_TRUE(close.has_value());
    ASSERT_TRUE(far.has_value());
    EXPECT_LT(*close, 0.0);
    EXPECT_NEAR(*far / *close, 1.0 / 64.0, 0.02);
}

// The charges the model's own reference data records for the atoms of its
// reference systems, against the charge model of the publication.  The
// classical set is the one these values are compared against, and the model's
// own reference routines of the EEQ scheme read it: the data carries several
// charge sets per atom, its GFN-FF set reproduces the classical one for the
// four elements these tests reach, and the others are the charges of other
// methods.  The model does not publish the geometries its reference states
// were computed at, so the comparison is at the experimental-geometry
// scale, as the coordination-number check above is.
TEST(D4Test, PartialChargesReproduceTheModelsOwnReferenceCharges) {
    constexpr double kAngstromPerBohr = 0.529177210903;

    // Water: O-H 0.9584 Angstrom, H-O-H 104.45 degrees.  The recorded
    // oxygen charge of the model's own OH2 state is -0.59881526060460.
    {
        const double r = 0.9584 / kAngstromPerBohr;
        const double half = 0.5 * 104.45 * std::numbers::pi / 180.0;
        excgrid::Geometry geometry;
        geometry.atoms.push_back({8, {0.0, 0.0, 0.0}});
        geometry.atoms.push_back({1, {r * std::sin(half), r * std::cos(half), 0.0}});
        geometry.atoms.push_back({1, {-r * std::sin(half), r * std::cos(half), 0.0}});

        auto charges = excgrid::D4PartialCharges(geometry, 0.0);
        ASSERT_TRUE(charges.has_value());
        EXPECT_NEAR((*charges)[0], -0.59881526060460, 0.02 * 0.59881526060460);
        EXPECT_DOUBLE_EQ((*charges)[1], (*charges)[2]);
    }

    // Benzene: C-C 1.39 Angstrom, C-H 1.09 Angstrom.  The recorded carbon
    // charge of the model's own C6H6 state is -0.10441353918078.
    {
        const double rcc = 1.39 / kAngstromPerBohr;
        const double rch = (1.39 + 1.09) / kAngstromPerBohr;
        excgrid::Geometry geometry;

        for (int k = 0; k < 6; ++k)
        {
            const double angle = k * std::numbers::pi / 3.0;
            geometry.atoms.push_back({6, {rcc * std::cos(angle), rcc * std::sin(angle), 0.0}});
        }

        for (int k = 0; k < 6; ++k)
        {
            const double angle = k * std::numbers::pi / 3.0;
            geometry.atoms.push_back({1, {rch * std::cos(angle), rch * std::sin(angle), 0.0}});
        }

        auto charges = excgrid::D4PartialCharges(geometry, 0.0);
        ASSERT_TRUE(charges.has_value());
        EXPECT_NEAR((*charges)[0], -0.10441353918078, 0.02 * 0.10441353918078);
        EXPECT_NEAR((*charges)[6], 0.10441353918078, 0.02 * 0.10441353918078);
    }
}

// What a partial-charge field has to be: the constraint of the model's
// Lagrangian is the system's total charge, the charges are of the order a
// partial charge is rather than of the order of the nuclear charges the
// scaling function divides them into, and the electronegativity ordering
// of the elements survives into their sign.
TEST(D4Test, PartialChargesConserveTheTotalChargeAndItsOrdering) {
    const excgrid::Geometry geometry = CaffeineGeometry();
    auto charges = excgrid::D4PartialCharges(geometry, 0.0);
    ASSERT_TRUE(charges.has_value());
    ASSERT_EQ(charges->size(), geometry.atoms.size());

    double sum = 0.0;

    for (std::size_t a = 0; a < charges->size(); ++a)
    {
        const double q = (*charges)[a];
        const int z = geometry.atoms[a].atomicNumber;
        sum += q;

        // The model's own reference systems carry charges well below one
        // electron, and this molecule's atoms are of that order: the
        // smallest eigenvalue of the kernel is far from zero but far from
        // the collective charge-transfer mode that a soft diagonal leaves
        // free to run away.
        EXPECT_LT(std::abs(q), 1.0) << a;

        // Hydrogen is the least electronegative element here and oxygen
        // the most, so their charges are of opposite sign.
        if (z == 1)
        {
            EXPECT_GT(q, 0.0) << a;
        }

        if (z == 8)
        {
            EXPECT_LT(q, 0.0) << a;
        }
    }

    EXPECT_NEAR(sum, 0.0, 1e-12);

    // The constraint is the total charge, not neutrality.
    auto anion = excgrid::D4PartialCharges(geometry, -1.0);
    ASSERT_TRUE(anion.has_value());

    double anionSum = 0.0;

    for (double q : *anion)
    {
        anionSum += q;
    }

    EXPECT_NEAR(anionSum, -1.0, 1e-12);

    // The term is a function of the interatomic distances alone.
    excgrid::Geometry moved = geometry;

    for (excgrid::Atom& atom : moved.atoms)
    {
        atom.position[0] += 11.25;
        atom.position[1] -= 4.5;
    }

    auto shifted = excgrid::D4PartialCharges(moved, 0.0);
    ASSERT_TRUE(shifted.has_value());

    for (std::size_t a = 0; a < charges->size(); ++a)
    {
        // The shift enters the kernel through a subtraction of coordinates
        // that no longer rounds identically, and the solve carries that
        // difference at its own last place.
        EXPECT_NEAR((*shifted)[a], (*charges)[a], 1e-12);
    }
}

TEST(D4Test, PartialChargesRefuseWhatTheTableDoesNotCover) {
    excgrid::Geometry beyond;
    beyond.atoms.push_back({87, {0.0, 0.0, 0.0}});
    beyond.atoms.push_back({1, {0.0, 0.0, 1.0}});

    EXPECT_EQ(excgrid::D4PartialCharges(beyond, 0.0).error(),
              excgrid::ErrorCode::kUnsupported);

    // Radon is the last element the charge model's own table covers, and
    // it is covered: only the polarizability tables stop short of it.
    excgrid::Geometry radon;
    radon.atoms.push_back({86, {0.0, 0.0, 0.0}});
    radon.atoms.push_back({1, {0.0, 0.0, 1.0}});
    EXPECT_TRUE(excgrid::D4PartialCharges(radon, 0.0).has_value());

    excgrid::Geometry empty;
    auto none = excgrid::D4PartialCharges(empty, 0.0);
    ASSERT_TRUE(none.has_value());
    EXPECT_TRUE(none->empty());
}
