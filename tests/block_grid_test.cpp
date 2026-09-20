// The block-grid tests: the partition of unity, the trimming and
// re-batching properties, and the end-to-end integral accuracy.

#include "excgrid/grid.hpp"

#include <cmath>
#include <gtest/gtest.h>

namespace {

constexpr double kPi = 3.14159265358979323846;

excgrid::Geometry TwoAtomGeometry() {
    // H2-like pair: two hydrogens at 1.4 Bohr separation.
    excgrid::Geometry geometry;
    geometry.atoms.push_back({1, {0.0, 0.0, -0.7}});
    geometry.atoms.push_back({1, {0.0, 0.0, 0.7}});

    return geometry;
}

TEST(BlockGridTest, BuildsForPair) {
    auto grid = excgrid::BlockGrid::Create(TwoAtomGeometry());

    ASSERT_TRUE(grid.has_value());
    EXPECT_GT(grid->BlockCount(), 0);
    EXPECT_GT(grid->TotalPointCount(), 0);
}

TEST(BlockGridTest, BlocksWithinTarget) {
    excgrid::GridParams params;
    params.radialPoints = 30;
    params.angularPoints = 110;
    params.blockTarget = 512;

    auto grid = excgrid::BlockGrid::Create(TwoAtomGeometry(), params);

    ASSERT_TRUE(grid.has_value());

    for (const excgrid::Block& block : grid->Blocks())
    {
        EXPECT_LE(block.pointCount, params.blockTarget);
        EXPECT_EQ(block.pointCount, block.points.size());
        EXPECT_EQ(block.pointCount, block.weights.size());
        EXPECT_EQ(block.pointCount, block.atomIndex.size());
    }
}

TEST(BlockGridTest, IntegratesGaussian) {
    // The full molecular grid integrates a unit-normalized Gaussian:
    // int exp(-a r^2) d^3r = (pi / a)^(3/2).  The point set is the
    // one-atom product grid (no partition complication), built through
    // the same public BlockGrid path.
    excgrid::Geometry geometry;
    geometry.atoms.push_back({1, {0.0, 0.0, 0.0}});
    excgrid::GridParams params;
    params.radialPoints = 75;
    params.angularPoints = 302;
    params.trimWeight = 0.0;

    auto grid = excgrid::BlockGrid::Create(geometry, params);

    ASSERT_TRUE(grid.has_value());
    const double a = 1.3;
    double sum = 0.0;

    for (const excgrid::Block& block : grid->Blocks())
    {
        for (std::size_t i = 0; i < block.pointCount; ++i)
        {
            const std::array<double, 3>& p = block.points[i];
            const double r2 = p[0] * p[0] + p[1] * p[1] + p[2] * p[2];
            sum += block.weights[i] * std::exp(-a * r2);
        }
    }

    EXPECT_NEAR(sum, std::pow(kPi / a, 1.5), 1e-6);
}

TEST(BlockGridTest, TrimmingDropsOnlyTinyWeights) {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({1, {0.0, 0.0, 0.0}});
    excgrid::GridParams params;
    params.radialPoints = 50;
    params.angularPoints = 194;
    params.trimWeight = 1e-10;

    auto grid = excgrid::BlockGrid::Create(geometry, params);

    ASSERT_TRUE(grid.has_value());

    for (const excgrid::Block& block : grid->Blocks())
    {
        for (std::size_t i = 0; i < block.pointCount; ++i)
        {
            EXPECT_GE(std::abs(block.weights[i]), params.trimWeight);
        }
    }
}

TEST(BlockGridTest, SpatialCompactness) {
    // Every block's diameter is a small fraction of the point cloud's
    // extent (the re-batching contract): the cloud extent is the
    // bounding-box diagonal (one pass over all points - an exact
    // all-pairs diameter is O(n^2) on a ~10^4-point grid).
    excgrid::GridParams params;
    params.radialPoints = 40;
    params.angularPoints = 194;
    params.blockTarget = 256;

    auto grid = excgrid::BlockGrid::Create(TwoAtomGeometry(), params);

    ASSERT_TRUE(grid.has_value());
    std::array<double, 3> minimum = {1e99, 1e99, 1e99};
    std::array<double, 3> maximum = {-1e99, -1e99, -1e99};
    double maxBlockDiameter = 0.0;

    for (const excgrid::Block& block : grid->Blocks())
    {
        for (std::size_t i = 0; i < block.pointCount; ++i)
        {
            for (int k = 0; k < 3; ++k)
            {
                minimum[k] = std::min(minimum[k], block.points[i][k]);
                maximum[k] = std::max(maximum[k], block.points[i][k]);
            }

            for (std::size_t j = i + 1; j < block.pointCount; ++j)
            {
                const double dx = block.points[i][0] - block.points[j][0];
                const double dy = block.points[i][1] - block.points[j][1];
                const double dz = block.points[i][2] - block.points[j][2];
                maxBlockDiameter =
                    std::max(maxBlockDiameter, std::sqrt(dx * dx + dy * dy + dz * dz));
            }
        }
    }

    const double extent = std::sqrt((maximum[0] - minimum[0]) * (maximum[0] - minimum[0]) +
                                    (maximum[1] - minimum[1]) * (maximum[1] - minimum[1]) +
                                    (maximum[2] - minimum[2]) * (maximum[2] - minimum[2]));

    EXPECT_GT(grid->TotalPointCount(), 0);
    EXPECT_LT(maxBlockDiameter, 0.6 * extent);
}

} // namespace
