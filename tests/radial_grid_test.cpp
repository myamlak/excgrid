// The MHL/Euler-Maclaurin radial quadrature tests: the mapping shape
// and integral accuracy on a known radial function.

#include "excgrid/grid.hpp"

#include <cmath>
#include <gtest/gtest.h>

namespace {

TEST(RadialGridTest, MappingShape) {
    // r_i = alpha (q_i / (1 - q_i))^m with q_i = i / (N + 1); the
    // outermost point is alpha * pointCount^m, innermost is smallest.
    auto grid = excgrid::RadialGrid::Create(50, 0.5, 2);

    ASSERT_TRUE(grid.has_value());
    EXPECT_EQ(grid->Size(), 50);
    EXPECT_NEAR(grid->Points()[49], 0.5 * 50.0 * 50.0, 1e-12 * 1250.0);
    EXPECT_GT(grid->Points()[1], grid->Points()[0]);

    for (std::size_t i = 0; i < grid->Size(); ++i)
    {
        EXPECT_GT(grid->Points()[i], 0.0);
        EXPECT_GT(grid->Weights()[i], 0.0);
    }
}

TEST(RadialGridTest, IntegratesExponential) {
    // int_0^inf r^2 exp(-2 r) dr = 1/4; the grid folds r^2 in on the
    // consumer side, so the check sums w_i * r_i^2 * exp(-2 r_i).
    auto grid = excgrid::RadialGrid::Create(75, 0.5, 2);

    ASSERT_TRUE(grid.has_value());
    double sum = 0.0;

    for (std::size_t i = 0; i < grid->Size(); ++i)
    {
        const double r = grid->Points()[i];
        sum += grid->Weights()[i] * r * r * std::exp(-2.0 * r);
    }

    EXPECT_NEAR(sum, 0.25, 1e-4);
}

TEST(RadialGridTest, RejectsZeroSize) {
    EXPECT_FALSE(excgrid::RadialGrid::Create(0, 0.5).has_value());
    EXPECT_FALSE(excgrid::RadialGrid::Create(10, 0.5, 0).has_value());
}

} // namespace
