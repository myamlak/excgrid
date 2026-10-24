
#include "../src/block_grid_derivatives.hpp"
#include "../src/grid_derivs.hpp"

#include "excgrid/geometry.hpp"
#include "excgrid/grid.hpp"
#include "excgrid/grid_derivatives.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <gtest/gtest.h>
#include <iomanip>
#include <limits>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace {

using excgrid::Block;
using excgrid::BlockDerivativeSizes;
using excgrid::BlockDerivatives;
using excgrid::BlockGrid;
using excgrid::BlockGridDerivativeProvider;
using excgrid::DerivativeOrder;
using excgrid::DerivativePart;
using excgrid::DerivativeRequest;
using excgrid::DerivativeStatus;
using excgrid::Geometry;
using excgrid::GridDerivativeProvider;
using excgrid::GridParams;
using excgrid::SizeOf;

using excgrid::derivs::DerivativeScratch;
using excgrid::derivs::EvaluatePointWeight;
using excgrid::derivs::GridBasis;
using excgrid::derivs::MakeDerivativeScratch;
using excgrid::derivs::MakeGridBasis;
using excgrid::derivs::PointId;
using excgrid::derivs::PointPosition;
using excgrid::derivs::PointWeightDerivatives;

// Leaves the product grid's point set unchanged: every switch stays on its plateau.
constexpr double kStep = 1e-5;

// Wide enough to move a point recognisably off this grid.
constexpr double kWideStep = 1e-2;

// Three atoms, no symmetry: the jitter keeps directions and cells from coinciding.
[[nodiscard]] Geometry Triatomic() {
    Geometry geometry;
    geometry.atoms.push_back({8, {0.0, 0.0, 0.0}});
    geometry.atoms.push_back({1, {0.24, 1.43, 1.11}});
    geometry.atoms.push_back({1, {-0.17, -1.35, 1.02}});
    return geometry;
}

// Trim off: two builds carry the same points, so their difference is weights and positions alone.
[[nodiscard]] GridParams BlockParams() {
    GridParams params;
    params.radialPoints = 24;
    params.angularPoints = 26;
    params.alpha = 0.05;
    params.trimWeight = 0.0;
    params.blockTarget = 128;
    return params;
}

[[nodiscard]] GridParams TrimmedParams() {
    GridParams params = BlockParams();
    params.trimWeight = 1e-15;
    return params;
}

[[nodiscard]] Geometry Displaced(const Geometry& geometry,
                                 std::size_t atom,
                                 std::size_t component,
                                 double amount) {
    Geometry moved = geometry;
    moved.atoms[atom].position[component] += amount;
    return moved;
}

[[nodiscard]] DerivativePart BothParts() noexcept {
    return static_cast<DerivativePart>(static_cast<std::uint8_t>(DerivativePart::kCoordinates) |
                                       static_cast<std::uint8_t>(DerivativePart::kWeights));
}

[[nodiscard]] DerivativeRequest MakeRequest(const Geometry& geometry,
                                            DerivativePart parts,
                                            DerivativeOrder order) {
    DerivativeRequest request;
    request.parts = parts;
    request.order = order;
    request.nuclearCoordinateCount = 3U * geometry.atoms.size();
    return request;
}

// Sentinel-filled, so a refusal is told from an answer of zeros by the buffers.
struct Buffers {
    std::vector<double> positionFirst;
    std::vector<double> weightFirst;
    std::vector<double> positionSecond;
    std::vector<double> weightSecond;

    // Spans taken after the object is final: a moved vector would leave them dangling.
    [[nodiscard]] BlockDerivatives View(const Block& block) {
        BlockDerivatives view;
        view.positionFirst = positionFirst;
        view.weightFirst = weightFirst;
        view.positionSecond = positionSecond;
        view.weightSecond = weightSecond;
        view.atomIndex = block.atomIndex;
        return view;
    }

    [[nodiscard]] bool AllHold(double sentinel) const {
        const auto holds = [sentinel](const std::vector<double>& values) {
            return std::all_of(values.begin(), values.end(),
                               [sentinel](double entry) { return entry == sentinel; });
        };

        return holds(positionFirst) && holds(weightFirst) && holds(positionSecond) &&
               holds(weightSecond);
    }
};

[[nodiscard]] Buffers MakeBuffers(const Block& block,
                                  const DerivativeRequest& request,
                                  double sentinel = 0.0) {
    const BlockDerivativeSizes sizes = SizeOf(request, block.pointCount);

    Buffers buffers;
    buffers.positionFirst.assign(sizes.positionFirst, sentinel);
    buffers.weightFirst.assign(sizes.weightFirst, sentinel);
    buffers.positionSecond.assign(sizes.positionSecond, sentinel);
    buffers.weightSecond.assign(sizes.weightSecond, sentinel);
    return buffers;
}

// A block carries no point identity: recover it by nearest radial point and best-aligned node.
[[nodiscard]] PointId Identify(const Geometry& geometry,
                               const GridBasis& basis,
                               const Block& block,
                               std::size_t index) {
    const std::size_t atom = block.atomIndex[index];
    const std::array<double, 3>& point = block.points[index];
    const std::array<double, 3>& center = geometry.atoms[atom].position;
    const double dx = point[0] - center[0];
    const double dy = point[1] - center[1];
    const double dz = point[2] - center[2];
    const double distance = std::sqrt(dx * dx + dy * dy + dz * dz);

    std::size_t radial = 0;
    double bestGap = std::numeric_limits<double>::infinity();
    double secondGap = std::numeric_limits<double>::infinity();
    for (std::size_t r = 0; r < basis.radial.Size(); ++r)
    {
        const double gap = std::abs(basis.radial.Points()[r] - distance);
        if (gap < bestGap)
        {
            secondGap = bestGap;
            bestGap = gap;
            radial = r;
        } else if (gap < secondGap)
        {
            secondGap = gap;
        }
    }

    const double inverse = 1.0 / distance;
    std::size_t angular = 0;
    double bestAlignment = -2.0;
    double secondAlignment = -2.0;
    for (std::size_t t = 0; t < basis.angular.Size(); ++t)
    {
        const std::array<double, 3> node = basis.angular.Point(t);
        const double alignment = (dx * node[0] + dy * node[1] + dz * node[2]) * inverse;
        if (alignment > bestAlignment)
        {
            secondAlignment = bestAlignment;
            bestAlignment = alignment;
            angular = t;
        } else if (alignment > secondAlignment)
        {
            secondAlignment = alignment;
        }
    }

    EXPECT_LT(bestGap, 1e-12) << "radial point " << radial << " of point " << index;
    EXPECT_GT(secondGap, 1e-9) << "radial point " << radial << " of point " << index;
    EXPECT_GT(bestAlignment - secondAlignment, 1e-6) << "angular node " << angular;

    const PointId id{atom, radial, angular};
    const std::array<double, 3> rebuilt = PointPosition(geometry, basis, id);
    for (std::size_t k = 0; k < 3; ++k)
    {
        EXPECT_NEAR(rebuilt[k], point[k], 1e-12 * (1.0 + std::abs(point[k])))
            << "component " << k << " of point " << index;
    }

    return id;
}

[[nodiscard]] std::size_t FlatCount(const Geometry& geometry, const GridBasis& basis) {
    return geometry.atoms.size() * basis.radial.Size() * basis.angular.Size();
}

// The product-grid enumeration order: a point's name independent of its block.
[[nodiscard]] std::size_t FlatIndex(const GridBasis& basis, const PointId& id) {
    return (id.atom * basis.radial.Size() + id.radial) * basis.angular.Size() + id.angular;
}

// Broad enough to reach outer shells, and off-origin so no symmetry cancels the integral.
constexpr double kFieldScale = 0.02;
constexpr std::array<double, 3> kFieldCenter = {0.7, -0.3, 0.4};

[[nodiscard]] double Field(const std::array<double, 3>& point) {
    const double dx = point[0] - kFieldCenter[0];
    const double dy = point[1] - kFieldCenter[1];
    const double dz = point[2] - kFieldCenter[2];

    return std::exp(-kFieldScale * (dx * dx + dy * dy + dz * dz));
}

[[nodiscard]] std::array<double, 3> FieldGradient(const std::array<double, 3>& point) {
    const double value = Field(point);

    return {-2.0 * kFieldScale * (point[0] - kFieldCenter[0]) * value,
            -2.0 * kFieldScale * (point[1] - kFieldCenter[1]) * value,
            -2.0 * kFieldScale * (point[2] - kFieldCenter[2]) * value};
}

[[nodiscard]] double QuadratureTotal(const BlockGrid& grid) {
    double total = 0.0;

    for (const Block& block : grid.Blocks())
    {
        for (std::size_t index = 0; index < block.pointCount; ++index)
        {
            total += block.weights[index] * Field(block.points[index]);
        }
    }

    return total;
}

// Sum of |terms|, not |sum|: weights change sign, so the sum can cancel.
[[nodiscard]] double QuadratureMagnitude(const BlockGrid& grid) {
    double total = 0.0;

    for (const Block& block : grid.Blocks())
    {
        for (std::size_t index = 0; index < block.pointCount; ++index)
        {
            total += std::abs(block.weights[index] * Field(block.points[index]));
        }
    }

    return total;
}

[[nodiscard]] double Magnitude(const std::vector<double>& values) {
    double largest = 0.0;

    for (const double value : values)
    {
        largest = std::max(largest, std::abs(value));
    }

    return largest;
}

[[nodiscard]] std::string Scientific(double value) {
    std::ostringstream text;
    text << std::scientific << std::setprecision(3) << value;

    return text.str();
}

// d(sum w f)/dR = sum (dw/dR f + w grad f . dx/dR).
[[nodiscard]] std::optional<std::vector<double>> InterfaceGradient(
    const Geometry& geometry,
    const GridDerivativeProvider& provider,
    const BlockGrid& grid) {
    const std::size_t width = 3U * geometry.atoms.size();
    const DerivativeRequest request = MakeRequest(geometry, BothParts(), DerivativeOrder::kFirst);

    std::vector<double> gradient(width, 0.0);

    for (const Block& block : grid.Blocks())
    {
        Buffers buffers = MakeBuffers(block, request);
        if (provider.Evaluate(block, request, buffers.View(block)) != DerivativeStatus::kOk)
        {
            return std::nullopt;
        }

        for (std::size_t index = 0; index < block.pointCount; ++index)
        {
            const double value = Field(block.points[index]);
            const std::array<double, 3> fieldGradient = FieldGradient(block.points[index]);
            const double* weightFirst = buffers.weightFirst.data() + index * width;
            const double* positionFirst = buffers.positionFirst.data() + index * 3U * width;

            for (std::size_t q = 0; q < width; ++q)
            {
                double term = weightFirst[q] * value;

                for (std::size_t k = 0; k < 3; ++k)
                {
                    term += block.weights[index] * fieldGradient[k] * positionFirst[k * width + q];
                }

                gradient[q] += term;
            }
        }
    }

    return gradient;
}

} // namespace

// Bit for bit: both sides call the same routine with the same identity.
TEST(GridDerivativeProviderTest, FirstOrderAgreesWithTheDirectComputation) {
    const Geometry geometry = Triatomic();
    const GridParams params = BlockParams();
    const std::size_t width = 3U * geometry.atoms.size();

    const excgrid::Result<GridBasis> basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());
    const excgrid::Result<BlockGrid> grid = BlockGrid::Create(geometry, params);
    ASSERT_TRUE(grid.has_value());
    const excgrid::Result<BlockGridDerivativeProvider> provider =
        BlockGridDerivativeProvider::Create(geometry, params, DerivativeOrder::kFirst);
    ASSERT_TRUE(provider.has_value());

    EXPECT_EQ(provider->MaxOrder(), DerivativeOrder::kFirst);

    const DerivativeRequest request = MakeRequest(geometry, BothParts(), DerivativeOrder::kFirst);
    DerivativeScratch scratch = MakeDerivativeScratch(geometry, excgrid::derivs::DerivativeOrder::kFirst);
    PointWeightDerivatives reference;

    std::size_t visited = 0;
    for (const Block& block : grid->Blocks())
    {
        Buffers buffers = MakeBuffers(block, request);
        ASSERT_EQ(provider->Evaluate(block, request, buffers.View(block)), DerivativeStatus::kOk);

        for (std::size_t index = 0; index < block.pointCount; ++index)
        {
            const PointId id = Identify(geometry, *basis, block, index);
            EvaluatePointWeight(geometry, *basis, id, scratch, reference);

            // A point rigidly translates with its owner: the coordinate derivative is the owner's identity block.
            const std::size_t owner = block.atomIndex[index];
            const double* position = buffers.positionFirst.data() + index * 3U * width;
            for (std::size_t row = 0; row < 3; ++row)
            {
                for (std::size_t column = 0; column < width; ++column)
                {
                    const double expected = (column == 3U * owner + row) ? 1.0 : 0.0;
                    EXPECT_EQ(position[row * width + column], expected)
                        << "point " << index << " of a block";
                }
            }

            const double* weight = buffers.weightFirst.data() + index * width;
            for (std::size_t column = 0; column < width; ++column)
            {
                EXPECT_EQ(weight[column], reference.first[column])
                    << "coordinate " << column << " of point " << index;
            }

            // Same expression in a different translation unit: equal to a rounding, not bit for bit.
            EXPECT_NEAR(reference.value, block.weights[index],
                        1e-12 * (1.0 + std::abs(block.weights[index])))
                << "point " << index;
            ++visited;
        }
    }

    EXPECT_EQ(visited, grid->TotalPointCount());
    EXPECT_EQ(visited, FlatCount(geometry, *basis));
}

// Coordinate half is identically zero; the weight half is the pass's matrix, mirrored.
TEST(GridDerivativeProviderTest, SecondOrderAgreesWithTheDirectComputation) {
    const Geometry geometry = Triatomic();
    const GridParams params = BlockParams();
    const std::size_t width = 3U * geometry.atoms.size();

    const excgrid::Result<GridBasis> basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());
    const excgrid::Result<BlockGrid> grid = BlockGrid::Create(geometry, params);
    ASSERT_TRUE(grid.has_value());
    const excgrid::Result<BlockGridDerivativeProvider> provider =
        BlockGridDerivativeProvider::Create(geometry, params, DerivativeOrder::kSecond);
    ASSERT_TRUE(provider.has_value());

    EXPECT_EQ(provider->MaxOrder(), DerivativeOrder::kSecond);

    const DerivativeRequest request = MakeRequest(geometry, BothParts(), DerivativeOrder::kSecond);
    DerivativeScratch scratch = MakeDerivativeScratch(geometry, excgrid::derivs::DerivativeOrder::kSecond);
    PointWeightDerivatives reference;

    std::size_t visited = 0;
    for (const Block& block : grid->Blocks())
    {
        Buffers buffers = MakeBuffers(block, request);
        ASSERT_EQ(provider->Evaluate(block, request, buffers.View(block)), DerivativeStatus::kOk);

        for (std::size_t index = 0; index < block.pointCount; ++index)
        {
            const PointId id = Identify(geometry, *basis, block, index);
            EvaluatePointWeight(geometry, *basis, id, scratch, reference);

            const double* position = buffers.positionSecond.data() + index * 3U * width * width;
            for (std::size_t entry = 0; entry < 3U * width * width; ++entry)
            {
                EXPECT_EQ(position[entry], 0.0) << "point " << index << " entry " << entry;
            }

            const double* weight = buffers.weightSecond.data() + index * width * width;
            for (std::size_t entry = 0; entry < width * width; ++entry)
            {
                EXPECT_EQ(weight[entry], reference.second[entry])
                    << "point " << index << " entry " << entry;
            }
            ++visited;
        }
    }

    EXPECT_EQ(visited, grid->TotalPointCount());
}

TEST(GridDerivativeProviderTest, ConsumerAssemblesTheTotalDerivative) {
    const Geometry geometry = Triatomic();
    const GridParams params = BlockParams();

    const excgrid::Result<BlockGrid> grid = BlockGrid::Create(geometry, params);
    ASSERT_TRUE(grid.has_value());
    const excgrid::Result<BlockGridDerivativeProvider> provider =
        BlockGridDerivativeProvider::Create(geometry, params, DerivativeOrder::kFirst);
    ASSERT_TRUE(provider.has_value());

    const GridDerivativeProvider& interface = *provider;
    const std::optional<std::vector<double>> analytic = InterfaceGradient(geometry, interface, *grid);
    ASSERT_TRUE(analytic.has_value());

    // A non-zero gradient, or the difference would compare well against anything.
    EXPECT_GT(Magnitude(*analytic), 1e-3);

    // Bound is the difference's rounding: 300 eps * terms / kStep.
    const double terms = QuadratureMagnitude(*grid);
    ASSERT_GT(terms, 1e-3);
    const double resolution = 300.0 * std::numeric_limits<double>::epsilon() * terms / kStep;

    double worst = 0.0;
    for (std::size_t atom = 0; atom < geometry.atoms.size(); ++atom)
    {
        for (std::size_t component = 0; component < 3; ++component)
        {
            const auto sample = [&](double amount) {
                const excgrid::Result<BlockGrid> built =
                    BlockGrid::Create(Displaced(geometry, atom, component, amount), params);
                EXPECT_TRUE(built.has_value());
                return built.has_value() ? QuadratureTotal(*built) : 0.0;
            };

            const double coarse = (sample(kStep) - sample(-kStep)) / (2.0 * kStep);
            const double fine = (sample(0.5 * kStep) - sample(-0.5 * kStep)) / kStep;
            const double extrapolated = (4.0 * coarse - fine) / 3.0;
            const std::size_t coordinate = 3U * atom + component;
            const double error = std::abs((*analytic)[coordinate] - extrapolated);

            worst = std::max(worst, error);
            EXPECT_LE(error, resolution)
                << "coordinate " << coordinate << ": interface " << (*analytic)[coordinate]
                << ", difference " << coarse << " (at half the step: " << fine << ")";
        }
    }

    RecordProperty("worst_error", Scientific(worst));
    RecordProperty("resolution", Scientific(resolution));
    RecordProperty("gradient_magnitude", Scientific(Magnitude(*analytic)));
    RecordProperty("term_magnitude", Scientific(terms));
}

TEST(GridDerivativeProviderTest, TrimmedRebatchedBlocksKeepTheirPoints) {
    const Geometry geometry = Triatomic();
    const GridParams untrimmed = BlockParams();
    const GridParams trimmed = TrimmedParams();

    const excgrid::Result<GridBasis> basis = MakeGridBasis(untrimmed);
    ASSERT_TRUE(basis.has_value());
    const excgrid::Result<BlockGrid> full = BlockGrid::Create(geometry, untrimmed);
    ASSERT_TRUE(full.has_value());
    const excgrid::Result<BlockGrid> kept = BlockGrid::Create(geometry, trimmed);
    ASSERT_TRUE(kept.has_value());

    std::size_t zeroWeighted = 0;
    for (const Block& block : full->Blocks())
    {
        for (std::size_t index = 0; index < block.pointCount; ++index)
        {
            if (block.weights[index] == 0.0)
            {
                ++zeroWeighted;
            }
        }
    }
    EXPECT_GT(zeroWeighted, 0u);
    EXPECT_EQ(zeroWeighted, full->TotalPointCount() - kept->TotalPointCount());

    const excgrid::Result<BlockGridDerivativeProvider> provider =
        BlockGridDerivativeProvider::Create(geometry, trimmed, DerivativeOrder::kFirst);
    ASSERT_TRUE(provider.has_value());

    const DerivativeRequest request = MakeRequest(geometry, BothParts(), DerivativeOrder::kFirst);
    const std::size_t flat = FlatCount(geometry, *basis);
    std::vector<char> seen(flat, 0);
    std::size_t visited = 0;
    std::size_t reordered = 0;

    for (const Block& block : kept->Blocks())
    {
        Buffers buffers = MakeBuffers(block, request);

        ASSERT_EQ(provider->Evaluate(block, request, buffers.View(block)), DerivativeStatus::kOk);

        std::size_t previous = flat;
        bool ascending = true;
        for (std::size_t index = 0; index < block.pointCount; ++index)
        {
            const std::size_t position = FlatIndex(*basis, Identify(geometry, *basis, block, index));

            EXPECT_EQ(seen[position], 0) << "product-grid point " << position << " appears twice";
            seen[position] = 1;

            if (position < previous)
            {
                ascending = false;
            }
            previous = position;
            ++visited;
        }

        if (!ascending)
        {
            ++reordered;
        }
    }

    EXPECT_EQ(visited, kept->TotalPointCount());
    EXPECT_LT(visited, flat);

    // The fixture must actually re-batch, or the identity recovery proves nothing.
    EXPECT_GT(reordered, 0u);
}

TEST(GridDerivativeProviderTest, SecondOrderIsRefusedByNameAndLeavesTheBuffersAlone) {
    const Geometry geometry = Triatomic();
    const GridParams params = BlockParams();

    const excgrid::Result<BlockGrid> grid = BlockGrid::Create(geometry, params);
    ASSERT_TRUE(grid.has_value());
    const excgrid::Result<BlockGridDerivativeProvider> provider =
        BlockGridDerivativeProvider::Create(geometry, params, DerivativeOrder::kFirst);
    ASSERT_TRUE(provider.has_value());
    ASSERT_EQ(provider->MaxOrder(), DerivativeOrder::kFirst);

    const DerivativeRequest request = MakeRequest(geometry, BothParts(), DerivativeOrder::kSecond);
    const double sentinel = -1234.5;

    ASSERT_GT(grid->BlockCount(), 0u);
    for (const Block& block : grid->Blocks())
    {
        Buffers buffers = MakeBuffers(block, request, sentinel);

        EXPECT_EQ(provider->Evaluate(block, request, buffers.View(block)),
                  DerivativeStatus::kRefusedSecondOrderUnavailable);
        EXPECT_TRUE(buffers.AllHold(sentinel));
    }

    const DerivativeRequest first = MakeRequest(geometry, BothParts(), DerivativeOrder::kFirst);
    const Block& block = grid->Blocks().front();
    Buffers buffers = MakeBuffers(block, first, sentinel);
    EXPECT_EQ(provider->Evaluate(block, first, buffers.View(block)), DerivativeStatus::kOk);
    EXPECT_FALSE(buffers.AllHold(sentinel));
}

// A swept coordinate moves its own atom's whole sub-grid, so some block is always refused.
TEST(GridDerivativeProviderTest, MovedGridIsRefusedByName) {
    const Geometry geometry = Triatomic();
    const GridParams params = BlockParams();

    const excgrid::Result<BlockGrid> grid = BlockGrid::Create(geometry, params);
    ASSERT_TRUE(grid.has_value());
    const DerivativeRequest request = MakeRequest(geometry, BothParts(), DerivativeOrder::kFirst);
    const double sentinel = -1234.5;

    std::size_t sweeps = 0;
    std::size_t refusals = 0;
    for (std::size_t atom = 0; atom < geometry.atoms.size(); ++atom)
    {
        for (std::size_t component = 0; component < 3; ++component)
        {
            for (const double amount : {kStep, -kStep, kWideStep, -kWideStep})
            {
                const excgrid::Result<BlockGridDerivativeProvider> provider =
                    BlockGridDerivativeProvider::Create(Displaced(geometry, atom, component, amount),
                                                        params, DerivativeOrder::kFirst);
                ASSERT_TRUE(provider.has_value());
                ++sweeps;

                std::size_t refusedHere = 0;
                for (const Block& block : grid->Blocks())
                {
                    Buffers buffers = MakeBuffers(block, request, sentinel);
                    const DerivativeStatus status =
                        provider->Evaluate(block, request, buffers.View(block));

                    if (status == DerivativeStatus::kRefusedUndifferentiableGrid)
                    {
                        ++refusedHere;
                    } else
                    {
                        // Points far from the moved atom are on both grids unchanged; the moved atom's sub-grid is always somewhere.
                        EXPECT_EQ(status, DerivativeStatus::kOk);
                    }
                }

                EXPECT_GT(refusedHere, 0u)
                    << "atom " << atom << " component " << component << " by " << amount;
                refusals += refusedHere;
            }
        }
    }

    EXPECT_EQ(sweeps, 4U * 3U * geometry.atoms.size());
    EXPECT_GT(refusals, 0u);
}

// Same molecule, same positions; only the point set differs, which is what the refusal names.
TEST(GridDerivativeProviderTest, TrimmedPointIsRefusedByName) {
    const Geometry geometry = Triatomic();

    const excgrid::Result<BlockGrid> loose = BlockGrid::Create(geometry, BlockParams());
    ASSERT_TRUE(loose.has_value());
    const excgrid::Result<BlockGridDerivativeProvider> provider =
        BlockGridDerivativeProvider::Create(geometry, TrimmedParams(), DerivativeOrder::kFirst);
    ASSERT_TRUE(provider.has_value());

    const DerivativeRequest request = MakeRequest(geometry, BothParts(), DerivativeOrder::kFirst);
    const double sentinel = -1234.5;
    const double threshold = TrimmedParams().trimWeight;

    std::size_t refusedBlocks = 0;
    std::size_t refusedPoints = 0;
    std::size_t expectedRefused = 0;
    for (const Block& block : loose->Blocks())
    {
        std::size_t unthresholded = 0;
        for (std::size_t index = 0; index < block.pointCount; ++index)
        {
            if (std::abs(block.weights[index]) < threshold)
            {
                ++unthresholded;
            }
        }

        Buffers buffers = MakeBuffers(block, request, sentinel);
        const DerivativeStatus status = provider->Evaluate(block, request, buffers.View(block));

        if (unthresholded > 0)
        {
            EXPECT_EQ(status, DerivativeStatus::kRefusedUndifferentiableGrid);
            ++refusedBlocks;
            refusedPoints += unthresholded;
        } else
        {
            EXPECT_EQ(status, DerivativeStatus::kOk);
        }

        expectedRefused += unthresholded;
    }

    EXPECT_GT(expectedRefused, 0u);
    EXPECT_EQ(refusedPoints, expectedRefused);
    EXPECT_GT(refusedBlocks, 0u);
}

// Arrays are indexed by nuclear coordinate; the wrong count is not a smaller answer.
TEST(GridDerivativeProviderTest, WrongFrameAndShortBuffersAreRefusedByName) {
    const Geometry geometry = Triatomic();
    const GridParams params = BlockParams();

    const excgrid::Result<BlockGrid> grid = BlockGrid::Create(geometry, params);
    ASSERT_TRUE(grid.has_value());
    const excgrid::Result<BlockGridDerivativeProvider> provider =
        BlockGridDerivativeProvider::Create(geometry, params, DerivativeOrder::kFirst);
    ASSERT_TRUE(provider.has_value());

    const double sentinel = -1234.5;
    const Block& block = grid->Blocks().front();

    DerivativeRequest wrongFrame = MakeRequest(geometry, BothParts(), DerivativeOrder::kFirst);
    --wrongFrame.nuclearCoordinateCount;
    {
        Buffers buffers = MakeBuffers(block, wrongFrame, sentinel);
        EXPECT_EQ(provider->Evaluate(block, wrongFrame, buffers.View(block)),
                  DerivativeStatus::kRefusedBufferTooSmall);
        EXPECT_TRUE(buffers.AllHold(sentinel));
    }

    const DerivativeRequest request = MakeRequest(geometry, BothParts(), DerivativeOrder::kFirst);
    {
        Buffers buffers = MakeBuffers(block, request, sentinel);
        Buffers truncated = buffers;
        truncated.weightFirst.pop_back();
        EXPECT_EQ(provider->Evaluate(block, request, truncated.View(block)),
                  DerivativeStatus::kRefusedBufferTooSmall);
        EXPECT_TRUE(truncated.AllHold(sentinel));
    }
}

// Built the way a consumer would: from the public declaration alone.
TEST(GridDerivativeProviderTest, AConsumerBuildsOneThroughThePublicHeader) {
    const Geometry geometry = Triatomic();
    const GridParams params = BlockParams();

    excgrid::Result<std::unique_ptr<GridDerivativeProvider>> built =
        excgrid::CreateGridDerivativeProvider(geometry, params, DerivativeOrder::kFirst);
    ASSERT_TRUE(built.has_value()) << "a consumer cannot obtain a provider at all";

    const GridDerivativeProvider& provider = **built;
    EXPECT_EQ(provider.MaxOrder(), DerivativeOrder::kFirst);

    excgrid::Result<std::unique_ptr<GridDerivativeProvider>> deeper =
        excgrid::CreateGridDerivativeProvider(geometry, params, DerivativeOrder::kSecond);
    ASSERT_TRUE(deeper.has_value());
    EXPECT_EQ((*deeper)->MaxOrder(), DerivativeOrder::kSecond);
}
