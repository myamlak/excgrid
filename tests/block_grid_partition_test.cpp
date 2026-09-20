// The Becke partition's value contract, checked through the public grid
// rather than against stored numbers: every block weight the build hands out
// is the quadrature weight of the point it was generated from times the
// Becke/SSF partition weight of that point's OWNING atom.  Both halves of
// that product are reconstructible from the public API - the radial point,
// the angular node, and the owned atom all come back out of the block - so
// the partition weight can be extracted and compared against the pairwise
// reference the library's candidate-restricted pass has to agree with.
//
// This is the gate the candidate restriction is answerable to: the pass skips
// every pair whose SSF factor is exactly 1 and every cell whose weight is
// exactly 0, and this test is what says those skips cost nothing.

#include "excgrid/grid.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>
#include <limits>
#include <vector>

namespace {

constexpr double kSwitchWindow = 0.64;

excgrid::Geometry Chain(std::size_t atomCount) {
    excgrid::Geometry geometry;

    for (std::size_t index = 0; index < atomCount; ++index)
    {
        geometry.atoms.push_back({1, {1.4 * static_cast<double>(index), 0.0, 0.0}});
    }

    return geometry;
}

// A heteroatom set: four species with four different Slater radii, so the
// radius table, the heteroatom adjustment and the fallback all get walked.
excgrid::Geometry MixedSpecies() {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({6, {0.0, 0.0, 0.0}});
    geometry.atoms.push_back({1, {0.0, 0.0, 2.1}});
    geometry.atoms.push_back({8, {0.0, 0.0, 4.2}});
    geometry.atoms.push_back({7, {1.1, 0.0, -1.1}});

    return geometry;
}

// The same radius table the library resolves (Bragg-Slater, Slater 1964),
// in Bohr, with the untabulated-element fallback.
struct SlaterEntry {
    int atomicNumber;
    double radiusAngstrom;
};

constexpr std::array<SlaterEntry, 46> kSlaterRadii = {{
    {1, 0.35},  {2, 1.0},   {3, 1.45},  {4, 1.05},  {5, 0.85},  {6, 0.70},  {7, 0.65},  {8, 0.60},
    {9, 0.50},  {10, 1.0},  {11, 1.80}, {12, 1.50}, {13, 1.25}, {14, 1.10}, {15, 1.00}, {16, 1.00},
    {17, 1.00}, {18, 1.0},  {19, 2.20}, {20, 1.80}, {21, 1.60}, {22, 1.40}, {23, 1.35}, {24, 1.40},
    {25, 1.40}, {26, 1.40}, {27, 1.35}, {28, 1.35}, {29, 1.35}, {30, 1.35}, {31, 1.30}, {32, 1.25},
    {33, 1.15}, {34, 1.15}, {35, 1.15}, {36, 1.0},  {37, 2.35}, {38, 2.00}, {39, 1.80}, {40, 1.55},
    {41, 1.45}, {42, 1.45}, {43, 1.35}, {44, 1.30}, {45, 1.35}, {46, 1.40},
}};

double RadiusOf(int atomicNumber) {
    for (const SlaterEntry& entry : kSlaterRadii)
    {
        if (entry.atomicNumber == atomicNumber)
        {
            return entry.radiusAngstrom * 1.8897261246257702;
        }
    }

    return 1.0 * 1.8897261246257702;
}

double Switch(double mu) {
    if (mu >= kSwitchWindow)
    {
        return 0.0;
    }

    if (mu <= -kSwitchWindow)
    {
        return 1.0;
    }

    const double t = mu / kSwitchWindow;
    const double t2 = t * t;
    const double z = t * (35.0 - t2 * (35.0 - t2 * (21.0 - 5.0 * t2))) / 16.0;

    return 0.5 * (1.0 - z);
}

// The pairwise reference: the owning atom's normalized Becke weight, with no
// candidate restriction and no early cell rejection - the full N^2 walk.
double ReferenceOwnerWeight(const std::array<double, 3>& point,
                            const excgrid::Geometry& geometry,
                            std::size_t owner) {
    const std::size_t atomCount = geometry.atoms.size();
    std::vector<double> distance(atomCount);
    std::vector<double> radius(atomCount);

    for (std::size_t a = 0; a < atomCount; ++a)
    {
        const double dx = point[0] - geometry.atoms[a].position[0];
        const double dy = point[1] - geometry.atoms[a].position[1];
        const double dz = point[2] - geometry.atoms[a].position[2];
        distance[a] = std::sqrt(dx * dx + dy * dy + dz * dz);
        radius[a] = RadiusOf(geometry.atoms[a].atomicNumber);
    }

    std::size_t nearest = 0;

    for (std::size_t b = 1; b < atomCount; ++b)
    {
        if (distance[b] < distance[nearest])
        {
            nearest = b;
        }
    }

    double secondNearest = std::numeric_limits<double>::infinity();

    for (std::size_t b = 0; b < atomCount; ++b)
    {
        if (b != nearest && distance[b] < secondNearest)
        {
            secondNearest = distance[b];
        }
    }

    std::vector<double> raw(atomCount, 1.0);

    if (distance[nearest] > 0.5 * (1.0 - kSwitchWindow) * secondNearest)
    {
        for (std::size_t a = 0; a < atomCount; ++a)
        {
            double weight = 1.0;

            for (std::size_t b = 0; b < atomCount; ++b)
            {
                if (b == a)
                {
                    continue;
                }

                const double pairRadius = radius[a] + radius[b];

                if (distance[a] >= kSwitchWindow * pairRadius + distance[b])
                {
                    weight = 0.0;
                    break;
                }

                weight *= Switch((distance[a] - distance[b]) / pairRadius);
            }

            raw[a] = weight;
        }
    } else
    {
        for (std::size_t a = 0; a < atomCount; ++a)
        {
            raw[a] = 0.0;
        }

        raw[nearest] = 1.0;
    }

    double total = 0.0;

    for (const double weight : raw)
    {
        total += weight;
    }

    return total > 0.0 ? raw[owner] / total : raw[owner];
}

// The quadrature weight the point was generated with, rebuilt from the
// public tables: the radial point and the angular node are matched to the
// block point's own geometry.
double QuadratureWeight(const excgrid::Block& block,
                        std::size_t index,
                        const excgrid::Geometry& geometry,
                        const excgrid::RadialGrid& radial,
                        const excgrid::AngularGrid& angular) {
    const std::array<double, 3>& center = geometry.atoms[block.atomIndex[index]].position;
    const double dx = block.points[index][0] - center[0];
    const double dy = block.points[index][1] - center[1];
    const double dz = block.points[index][2] - center[2];
    const double distance = std::sqrt(dx * dx + dy * dy + dz * dz);

    std::size_t radialIndex = 0;
    double bestRadialGap = std::numeric_limits<double>::infinity();

    for (std::size_t r = 0; r < radial.Size(); ++r)
    {
        const double gap = std::abs(radial.Points()[r] - distance);

        if (gap < bestRadialGap)
        {
            bestRadialGap = gap;
            radialIndex = r;
        }
    }

    std::size_t angularIndex = 0;
    double bestAlignment = -2.0;

    for (std::size_t t = 0; t < angular.Size(); ++t)
    {
        const std::array<double, 3> node = angular.Point(t);
        const double alignment = (dx * node[0] + dy * node[1] + dz * node[2]) / distance;

        if (alignment > bestAlignment)
        {
            bestAlignment = alignment;
            angularIndex = t;
        }
    }

    const double radius = radial.Points()[radialIndex];

    return radial.Weights()[radialIndex] * radius * radius * angular.Weight(angularIndex);
}

struct Fixture {
    excgrid::Geometry geometry;
    excgrid::GridParams params;
};

std::vector<Fixture> MakeFixtures() {
    std::vector<Fixture> fixtures;

    // The resolution the engine runs at, on a small system: the candidate
    // set is exactly what a real DFT grid build exercises.
    excgrid::GridParams engine;
    engine.radialPoints = 30;
    engine.angularPoints = 50;

    excgrid::GridParams coarse;
    coarse.radialPoints = 20;
    coarse.angularPoints = 26;

    excgrid::Geometry pair;
    pair.atoms.push_back({1, {0.0, 0.0, -0.7}});
    pair.atoms.push_back({1, {0.0, 0.0, 0.7}});

    excgrid::Geometry single;
    single.atoms.push_back({1, {0.0, 0.0, 0.0}});

    excgrid::Geometry untabulated;
    untabulated.atoms.push_back({94, {0.0, 0.0, 0.0}});
    untabulated.atoms.push_back({1, {0.0, 0.0, 3.0}});

    // Sparse geometries.  The per-shell neighbour window leaves most of the
    // molecule out here, so these are the fixtures that exercise the second
    // nearest and second smallest hi fallbacks the restricted pass needs to
    // stay exact - dense sets never reach them.
    excgrid::Geometry sparsePair;
    sparsePair.atoms.push_back({6, {0.0, 0.0, 0.0}});
    sparsePair.atoms.push_back({6, {0.0, 0.0, 20.0}});

    excgrid::Geometry sparseTriangle;
    sparseTriangle.atoms.push_back({1, {0.0, 0.0, 0.0}});
    sparseTriangle.atoms.push_back({8, {15.0, 0.0, 0.0}});
    sparseTriangle.atoms.push_back({7, {0.0, 15.0, 0.0}});

    fixtures.push_back({single, engine});
    fixtures.push_back({pair, engine});
    fixtures.push_back({Chain(3), engine});
    fixtures.push_back({Chain(5), engine});
    fixtures.push_back({MixedSpecies(), engine});
    fixtures.push_back({untabulated, coarse});
    fixtures.push_back({Chain(9), coarse});
    fixtures.push_back({sparsePair, engine});
    fixtures.push_back({sparseTriangle, engine});

    return fixtures;
}

TEST(BlockGridPartitionTest, OwnerWeightMatchesThePairwiseReference) {
    std::size_t checked = 0;
    std::size_t nonzero = 0;

    for (const Fixture& fixture : MakeFixtures())
    {
        auto grid = excgrid::BlockGrid::Create(fixture.geometry, fixture.params);

        ASSERT_TRUE(grid.has_value());
        ASSERT_GT(grid->TotalPointCount(), 0u);

        auto radial = excgrid::RadialGrid::Create(
            fixture.params.radialPoints, fixture.params.alpha, fixture.params.radialExponent);
        auto angular = excgrid::AngularGrid::Create(fixture.params.angularPoints);

        ASSERT_TRUE(radial.has_value());
        ASSERT_TRUE(angular.has_value());

        for (const excgrid::Block& block : grid->Blocks())
        {
            ASSERT_EQ(block.pointCount, block.weights.size());

            for (std::size_t index = 0; index < block.pointCount; ++index)
            {
                const double quadrature =
                    QuadratureWeight(block, index, fixture.geometry, *radial, *angular);
                const double extracted = block.weights[index] / quadrature;
                const double reference = ReferenceOwnerWeight(
                    block.points[index], fixture.geometry, block.atomIndex[index]);
                const double tolerance = std::max(1e-12 * std::abs(reference), 1e-300);

                ASSERT_GT(quadrature, 0.0);
                EXPECT_LE(std::abs(extracted - reference), tolerance)
                    << "atoms=" << fixture.geometry.atoms.size() << " point=" << index;
                ++checked;

                if (reference != 0.0)
                {
                    ++nonzero;
                }
            }
        }
    }

    // The coverage claim: the fixtures above are only a gate if they carry a
    // real point count and real non-zero cells (a pass built entirely of
    // trimmed cells would be vacuously green).
    EXPECT_GT(checked, 20000u);
    EXPECT_GT(nonzero, 20000u);
}

} // namespace
