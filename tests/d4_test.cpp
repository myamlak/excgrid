// Published DFT-D4 parameters: Caldeweyher, Ehlert, Hansen, Neugebauer, Spicher, Bannwarth, Grimme, J. Chem. Phys. 150 (2019) 154122 (S14-S21, Table A1).

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

    EXPECT_FALSE(excgrid::D4Preset("nosuchfunctional").has_value());
    EXPECT_EQ(excgrid::D4Preset("nosuchfunctional").error(), excgrid::ErrorCode::kInvalidArgument);
}

TEST(D4Test, ElementRangeIsRefusedByName) {
    EXPECT_TRUE(excgrid::D4CoversElement(1));  // H
    EXPECT_TRUE(excgrid::D4CoversElement(6));  // C
    EXPECT_TRUE(excgrid::D4CoversElement(86)); // Rn, the last tabulated element

    // 87 is outside the published parameterization: refused, not extrapolated.
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

    EXPECT_FALSE(excgrid::DispersionModelFromKey("d5").has_value());
    EXPECT_EQ(excgrid::DispersionModelFromKey("d5").error(), excgrid::ErrorCode::kInvalidArgument);
    EXPECT_FALSE(excgrid::DispersionModelFromKey("").has_value());
    EXPECT_NE(excgrid::DispersionModel::kD3, excgrid::DispersionModel::kD4);
}

// Table A1 of the paper's Supplementary Material (S4-S6).
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

    // Includes an element whose hardness is not positive: the table is carried as published.
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

// Covalent radii and Pauling electronegativities: Pyykko and Atsumi, Chem. Eur. J. 15 (2009) 186.
TEST(D4Test, CountingFunctionElementDataIsThePublishedData) {
    // The published single-bond radius scaled by 4/3 (the D3 counting convention), in Angstrom.
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

// Reference coordination numbers at experimental bond lengths, with the 4/3-scaled radii.
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

// Atomic hardnesses: Ghosh and Islam (the model's reference 42).
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

// No scaling when the effective charges are equal; the large-ratio limit is exp(3).
TEST(D4Test, ChargeScalingIsUnityAtTheReference) {
    for (int z : {1, 6, 7, 8, 9})
    {
        auto unity = excgrid::D4ChargeScaling(z, 6.0, 6.0);
        ASSERT_TRUE(unity.has_value()) << z;
        EXPECT_DOUBLE_EQ(*unity, 1.0) << z;
    }

    auto gained = excgrid::D4ChargeScaling(8, 7.0, 8.0);
    auto plain = excgrid::D4ChargeScaling(8, 8.0, 8.0);
    auto lost = excgrid::D4ChargeScaling(8, 9.0, 8.0);
    ASSERT_TRUE(gained.has_value());
    ASSERT_TRUE(plain.has_value());
    ASSERT_TRUE(lost.has_value());
    EXPECT_GT(*gained, *plain);
    EXPECT_DOUBLE_EQ(*plain, 1.0);
    EXPECT_LT(*lost, *plain);

    auto huge = excgrid::D4ChargeScaling(1, 1e-9, 1.0);
    ASSERT_TRUE(huge.has_value());
    EXPECT_NEAR(*huge, std::exp(3.0), 1e-6);

    // exp[beta1 (1 - exp(2 gamma (1 - z_ref/z)))] at z_ref/z = 2.
    const double gamma = 0.47259288;
    auto doubled = excgrid::D4ChargeScaling(1, 1.0, 2.0);
    ASSERT_TRUE(doubled.has_value());
    EXPECT_NEAR(*doubled, std::exp(3.0 * (1.0 - std::exp(2.0 * gamma * (1.0 - 2.0)))), 1e-12);

    // The model's published output for this hydrogen is 0.73993; the equation without the steepness misses by 17 per cent.
    auto hydrogen = excgrid::D4ChargeScaling(1, 1.1126, 1.0);
    ASSERT_TRUE(hydrogen.has_value());
    EXPECT_NEAR(*hydrogen, 0.73997, 1e-4);

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

    geometry.atoms.push_back({87, {0.0, 0.0, 4.0}});
    auto refused = excgrid::D4CoordinationNumbers(geometry);
    ASSERT_FALSE(refused.has_value());
    EXPECT_EQ(refused.error(), excgrid::ErrorCode::kUnsupported);
}

// Beyond krypton the charge is Z less the ECP core: 28 (Rb-Xe, Y-Cd, In-Sb, Ce-Lu), 46 (Cs, Ba, La), 60 (Hf-Hg, Tl-Bi, Po-Rn).
TEST(D4Test, EffectiveNuclearChargeStripsTheCoreBeyondKrypton) {
    for (int z = 1; z <= 36; ++z)
    {
        auto charge = excgrid::D4EffectiveNuclearCharge(z, 0.0);
        ASSERT_TRUE(charge.has_value()) << z;
        EXPECT_DOUBLE_EQ(*charge, static_cast<double>(z)) << z;
    }

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
    // The paper's frequency grid has 23 points.
    EXPECT_EQ(excgrid::kD4FrequencyPointCount, 23);

    std::span<const double> grid = excgrid::D4FrequencyGrid();
    ASSERT_EQ(grid.size(), static_cast<std::size_t>(excgrid::kD4FrequencyPointCount));

    EXPECT_GT(grid.front(), 0.0);
    for (std::size_t j = 0; j + 1 < grid.size(); ++j)
    {
        EXPECT_GT(grid[j + 1], grid[j]) << j;
    }
}

TEST(D4Test, CasimirPolderC6IsTheTrapeziumRuleOverTheGrid) {
    std::span<const double> grid = excgrid::D4FrequencyGrid();
    const std::size_t count = grid.size();

    // Flat alpha: the coefficient is (3/pi) times the grid span.
    const std::vector<double> ones(count, 1.0);
    auto flat = excgrid::D4CasimirPolderC6(ones, ones);
    ASSERT_TRUE(flat.has_value());
    EXPECT_NEAR(*flat, (3.0 / std::numbers::pi) * (grid.back() - grid.front()), 1e-12);

    // Exact for one linear factor against a constant; not for two linear factors.
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

    // Quadratic error: sum over intervals of width^3 / 12 times the second derivative.
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

    auto swapped = excgrid::D4CasimirPolderC6(linearB, linearA);
    ASSERT_TRUE(swapped.has_value());
    EXPECT_NEAR(*swapped, *mixed, 1e-12);

    const std::vector<double> doubledA(count, 2.0);
    auto doubled = excgrid::D4CasimirPolderC6(doubledA, ones);
    ASSERT_TRUE(doubled.has_value());
    EXPECT_NEAR(*doubled, 2.0 * *flat, 1e-12);

    const std::vector<double> shortSet(count - 1, 1.0);
    EXPECT_EQ(excgrid::D4CasimirPolderC6(shortSet, ones).error(),
              excgrid::ErrorCode::kInvalidArgument);
    EXPECT_EQ(excgrid::D4CasimirPolderC6(ones, {}).error(),
              excgrid::ErrorCode::kInvalidArgument);
}

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

    // The Gaussian count is not inert: enlarging a reference raises its share of the weight.
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

// w_k = sum_{j=1..N_k^s} exp(-beta2 j (CN - CN_k)^2); two references split in the ratio of their sums.
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

    const std::vector<double> neighbours = {0.0, 1.0};
    auto one = excgrid::D4ReferenceWeights(1.2, neighbours, std::vector<int>{1, 1});
    auto four = excgrid::D4ReferenceWeights(1.2, neighbours, std::vector<int>{1, 4});
    ASSERT_TRUE(one.has_value());
    ASSERT_TRUE(four.has_value());
    EXPECT_GT((*four)[1], (*one)[1]);
}

} // namespace

// Caffeine fixture and pair energies from the D4 program's app test 04 (`04-caffeine.xyz`, `04-pair-analysis.json`).
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

    // Through the shipped default: a fit moved into the table would fail here.
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

    EXPECT_LT(highest - lowest, 0.002);
    EXPECT_GT(lowest, 0.997);
    EXPECT_LT(highest, 1.0);
}

// Bare C6/r^6 past R0: doubling the separation divides the term by 64.
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

// The model's recorded reference charges (the classical EEQ set), compared at experimental-geometry scale.
TEST(D4Test, PartialChargesReproduceTheModelsOwnReferenceCharges) {
    constexpr double kAngstromPerBohr = 0.529177210903;

    // Water O-H 0.9584 Angstrom, H-O-H 104.45 degrees; recorded oxygen charge -0.59881526060460.
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

    // Benzene C-C 1.39 Angstrom, C-H 1.09 Angstrom; recorded carbon charge -0.10441353918078.
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

// Total charge conserved, charges of partial-charge order, electronegativity ordering in the signs.
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

        // Charges well below one electron, as the model's reference systems carry.
        EXPECT_LT(std::abs(q), 1.0) << a;

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
        // Translation re-rounds the coordinate subtraction; the solve carries it at 1e-12.
        EXPECT_NEAR((*shifted)[a], (*charges)[a], 1e-12);
    }
}

TEST(D4Test, PartialChargesRefuseWhatTheTableDoesNotCover) {
    excgrid::Geometry beyond;
    beyond.atoms.push_back({87, {0.0, 0.0, 0.0}});
    beyond.atoms.push_back({1, {0.0, 0.0, 1.0}});

    EXPECT_EQ(excgrid::D4PartialCharges(beyond, 0.0).error(),
              excgrid::ErrorCode::kUnsupported);

    // Radon is in the charge table; only the polarizability tables stop short.
    excgrid::Geometry radon;
    radon.atoms.push_back({86, {0.0, 0.0, 0.0}});
    radon.atoms.push_back({1, {0.0, 0.0, 1.0}});
    EXPECT_TRUE(excgrid::D4PartialCharges(radon, 0.0).has_value());

    excgrid::Geometry empty;
    auto none = excgrid::D4PartialCharges(empty, 0.0);
    ASSERT_TRUE(none.has_value());
    EXPECT_TRUE(none->empty());
}
