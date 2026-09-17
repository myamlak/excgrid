// The D3 dispersion tests: the analytic gradient vs finite differences
// (the strong internal check - no external reference needed), the
// distance-decay shape, and the preset table.

#include "excgrid/dispersion.hpp"

#include <cmath>
#include <gtest/gtest.h>

namespace {

excgrid::Geometry WaterGeometry() {
    // A water-like three-atom geometry (O + 2 H), arbitrary but
    // physical coordinates in Bohr.
    excgrid::Geometry geometry;
    geometry.atoms.push_back({8, {0.0, 0.0, 0.0}});
    geometry.atoms.push_back({1, {1.4, 0.0, 0.9}});
    geometry.atoms.push_back({1, {-1.4, 0.0, 0.9}});

    return geometry;
}

TEST(D3Test, EnergyIsNegativeAndDecays) {
    const excgrid::D3Parameters p = excgrid::D3Preset("b3lyp").value();

    auto near = excgrid::GrimmeD3Energy(WaterGeometry(), p);
    ASSERT_TRUE(near.has_value());
    EXPECT_LT(*near, 0.0);

    // Doubling every distance must reduce |E| substantially (the R^-6
    // dominance).
    excgrid::Geometry far = WaterGeometry();

    for (excgrid::Atom& atom : far.atoms)
    {
        atom.position = {atom.position[0] * 2.0, atom.position[1] * 2.0, atom.position[2] * 2.0};
    }

    auto farEnergy = excgrid::GrimmeD3Energy(far, p);
    ASSERT_TRUE(farEnergy.has_value());
    EXPECT_LT(std::abs(*farEnergy), std::abs(*near) * 0.1);
}

TEST(D3Test, GradientMatchesFiniteDifference) {
    const excgrid::D3Parameters p = excgrid::D3Preset("pbe").value();
    const excgrid::Geometry geometry = WaterGeometry();
    const double h = 1e-4;

    auto result = excgrid::GrimmeD3(geometry, p);
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->gradient.size(), geometry.atoms.size());

    for (std::size_t a = 0; a < geometry.atoms.size(); ++a)
    {
        for (int k = 0; k < 3; ++k)
        {
            excgrid::Geometry plus = geometry;
            excgrid::Geometry minus = geometry;
            plus.atoms[a].position[k] += h;
            minus.atoms[a].position[k] -= h;

            auto ePlus = excgrid::GrimmeD3Energy(plus, p);
            auto eMinus = excgrid::GrimmeD3Energy(minus, p);
            ASSERT_TRUE(ePlus.has_value());
            ASSERT_TRUE(eMinus.has_value());

            const double numerical = (*ePlus - *eMinus) / (2.0 * h);
            const double analytic = result->gradient[a][k];

            EXPECT_NEAR(analytic, numerical, 1e-6) << "atom " << a << " axis " << k;
        }
    }
}

TEST(D3Test, TranslationInvariance) {
    const excgrid::D3Parameters p = excgrid::D3Preset("b3lyp").value();
    excgrid::Geometry shifted = WaterGeometry();

    for (excgrid::Atom& atom : shifted.atoms)
    {
        atom.position = {atom.position[0] + 2.5, atom.position[1] - 1.2, atom.position[2] + 0.7};
    }

    auto here = excgrid::GrimmeD3Energy(WaterGeometry(), p);
    auto there = excgrid::GrimmeD3Energy(shifted, p);

    ASSERT_TRUE(here.has_value());
    ASSERT_TRUE(there.has_value());
    EXPECT_NEAR(*here, *there, 1e-12);
}

TEST(D3Test, GradientSumsToZero) {
    const excgrid::D3Parameters p = excgrid::D3Preset("b3lyp").value();
    auto result = excgrid::GrimmeD3(WaterGeometry(), p);

    ASSERT_TRUE(result.has_value());
    std::array<double, 3> sum = {0.0, 0.0, 0.0};

    for (const std::array<double, 3>& g : result->gradient)
    {
        sum = {sum[0] + g[0], sum[1] + g[1], sum[2] + g[2]};
    }

    EXPECT_NEAR(sum[0], 0.0, 1e-12);
    EXPECT_NEAR(sum[1], 0.0, 1e-12);
    EXPECT_NEAR(sum[2], 0.0, 1e-12);
}

TEST(D3Test, PresetsResolve) {
    for (const char* name : {"b3lyp",
                             "pbe",
                             "pbe0",
                             "bp86",
                             "blyp",
                             "revpbe",
                             "pbesol",
                             "b97d",
                             "tpss",
                             "bpbe",
                             "bhandhlyp",
                             "b3pw91",
                             "hf"})
    {
        auto preset = excgrid::D3Preset(name);
        EXPECT_TRUE(preset.has_value()) << name;
    }

    EXPECT_FALSE(excgrid::D3Preset("nosuch").has_value());
}

TEST(D3Test, RejectsOutOfTableElement) {
    const excgrid::D3Parameters p = excgrid::D3Preset("b3lyp").value();
    excgrid::Geometry geometry;
    geometry.atoms.push_back({95, {0.0, 0.0, 0.0}});
    geometry.atoms.push_back({1, {1.4, 0.0, 0.0}});

    auto result = excgrid::GrimmeD3Energy(geometry, p);

    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), excgrid::ErrorCode::kUnsupported);
}

} // namespace
