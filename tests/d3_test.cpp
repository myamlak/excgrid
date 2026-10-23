
#include "excgrid/dispersion.hpp"

#include <cmath>
#include <gtest/gtest.h>

namespace {

excgrid::Geometry WaterGeometry() {
    // Water-like O + 2 H fixture, in Bohr.
    excgrid::Geometry geometry;
    geometry.atoms.push_back({8, {0.0, 0.0, 0.0}});
    geometry.atoms.push_back({1, {1.4, 0.0, 0.9}});
    geometry.atoms.push_back({1, {-1.4, 0.0, 0.9}});

    return geometry;
}

excgrid::Geometry ScaledWaterGeometry(double scale) {
    excgrid::Geometry geometry = WaterGeometry();

    for (excgrid::Atom& atom : geometry.atoms)
    {
        atom.position = {atom.position[0] * scale, atom.position[1] * scale,
                         atom.position[2] * scale};
    }

    return geometry;
}

TEST(D3Test, EnergyIsNegativeAndDecays) {
    const excgrid::D3Parameters p = excgrid::D3Preset("b3lyp").value();

    auto near = excgrid::GrimmeD3Energy(WaterGeometry(), p);
    ASSERT_TRUE(near.has_value());
    EXPECT_LT(*near, 0.0);

    // Asymptotic R^-6 tail: doubling every distance cuts |E| by about 2^6 = 64.
    auto far4 = excgrid::GrimmeD3Energy(ScaledWaterGeometry(4.0), p);
    auto far8 = excgrid::GrimmeD3Energy(ScaledWaterGeometry(8.0), p);
    ASSERT_TRUE(far4.has_value());
    ASSERT_TRUE(far8.has_value());
    EXPECT_LT(std::abs(*far8), std::abs(*far4) * 0.05);
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

TEST(D3Test, ReferenceEnergyB3lypWater) {
    // dftd3 V3.2 Rev 1, `dftd3 water.coord -zero -func b3-lyp`: Edisp = -0.00000634 au.
    const excgrid::D3Parameters p = excgrid::D3Preset("b3lyp").value();
    auto energy = excgrid::GrimmeD3Energy(WaterGeometry(), p);

    ASSERT_TRUE(energy.has_value());
    EXPECT_NEAR(*energy, -0.00000634, 5e-8);
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
