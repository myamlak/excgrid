// The Lebedev angular quadrature tests: the normalization (sum w = 4 pi)
// and the moment exactness the tables are certified for.

#include "excgrid/grid.hpp"

#include <cmath>
#include <gtest/gtest.h>

namespace {

constexpr double kPi = 3.14159265358979323846;

TEST(AngularGridTest, Normalization) {
    // Every shipped size sums its weights to 4 pi.
    for (std::size_t size : excgrid::AngularGrid::kAvailableSizes)
    {
        auto grid = excgrid::AngularGrid::Create(size);

        ASSERT_TRUE(grid.has_value());
        double sum = 0.0;

        for (std::size_t i = 0; i < grid->Size(); ++i)
        {
            sum += grid->Weight(i);
        }

        EXPECT_NEAR(sum, 4.0 * kPi, 1e-12) << "size " << size;
    }
}

TEST(AngularGridTest, MomentExactness) {
    // The odd monomials and low even moments integrate exactly on every
    // size's certified degree: int x dOmega = 0, int x^2 dOmega = 4pi/3,
    // int x^2 y^2 dOmega = 4pi/15, int x^4 dOmega = 4pi/5.
    for (std::size_t size : excgrid::AngularGrid::kAvailableSizes)
    {
        auto grid = excgrid::AngularGrid::Create(size);

        ASSERT_TRUE(grid.has_value());
        double mX = 0.0;
        double mX2 = 0.0;
        double mX2Y2 = 0.0;
        double mX4 = 0.0;

        for (std::size_t i = 0; i < grid->Size(); ++i)
        {
            const std::array<double, 3> p = grid->Point(i);
            const double w = grid->Weight(i);

            mX += w * p[0];
            mX2 += w * p[0] * p[0];
            mX2Y2 += w * p[0] * p[0] * p[1] * p[1];
            mX4 += w * p[0] * p[0] * p[0] * p[0];
        }

        // The odd/even moment checks apply within the certified degree:
        // x^2 y^2 and x^4 are degree-4 moments (the smallest shipped
        // size, 6, is degree 3 - its x^2 y^2 is identically zero on the
        // axis nodes).
        EXPECT_NEAR(mX, 0.0, 1e-12) << "size " << size;
        EXPECT_NEAR(mX2, 4.0 * kPi / 3.0, 1e-12) << "size " << size;

        if (grid->Degree() >= 4)
        {
            EXPECT_NEAR(mX2Y2, 4.0 * kPi / 15.0, 1e-12) << "size " << size;
            EXPECT_NEAR(mX4, 4.0 * kPi / 5.0, 1e-12) << "size " << size;
        }
    }
}

TEST(AngularGridTest, RejectsUnknownSize) {
    auto grid = excgrid::AngularGrid::Create(313);

    EXPECT_FALSE(grid.has_value());
    EXPECT_EQ(grid.error(), excgrid::ErrorCode::kInvalidArgument);
}

TEST(AngularGridTest, NodesOnUnitSphere) {
    auto grid = excgrid::AngularGrid::Create(110);

    ASSERT_TRUE(grid.has_value());

    for (std::size_t i = 0; i < grid->Size(); ++i)
    {
        const std::array<double, 3> p = grid->Point(i);
        const double r2 = p[0] * p[0] + p[1] * p[1] + p[2] * p[2];

        EXPECT_NEAR(r2, 1.0, 1e-14);
    }
}

} // namespace
