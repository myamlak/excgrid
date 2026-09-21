// The grid's derivatives as a CONSUMER sees them: through the block-indexed
// interface, over the blocks a build hands out.
//
// The computation behind the interface is checked against fresh builds at
// displaced geometries - the same channel the derivative pass is checked on -
// and against the pass's own routines called directly, which is what says the
// interface adds nothing of its own to the numbers.
//
// The refusals are checked as refusals: the status the caller gets, and the
// buffers the caller is left with.  A zero where a second derivative was asked
// for is the failure the interface exists to prevent, so the tests here assert
// that the refusal happens and that nothing was written in its place.

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

// The step the consumer's finite differences run at.  A displacement of this
// size leaves the product grid's point set alone - the switch stays on its
// plateau at every point and every atom pair - so two builds separated by it
// carry the same points and their difference is a difference of one smooth
// function.
constexpr double kStep = 1e-5;

// The step the moved-grid refusals are probed with: five orders of magnitude
// past the point where a point's position is recognisably not this grid's.
constexpr double kWideStep = 1e-2;

// A three-atom molecule with no symmetry.  The jitter keeps a check from
// passing because two directions happen to be equal, and the two hydrogens
// differ enough that no cell is at a plateau by accident.
[[nodiscard]] Geometry Triatomic() {
    Geometry geometry;
    geometry.atoms.push_back({8, {0.0, 0.0, 0.0}});
    geometry.atoms.push_back({1, {0.24, 1.43, 1.11}});
    geometry.atoms.push_back({1, {-0.17, -1.35, 1.02}});
    return geometry;
}

// Small quadrature counts, so a block walk is a handful of blocks and a
// (3N)^2 buffer stays trivial.  The trim is off: the build then keeps every
// product-grid point, so two builds of the same molecule carry exactly the same
// points and a difference over them is a difference of the weights and
// positions alone.
[[nodiscard]] GridParams BlockParams() {
    GridParams params;
    params.radialPoints = 24;
    params.angularPoints = 26;
    params.alpha = 0.05;
    params.trimWeight = 0.0;
    params.blockTarget = 128;
    return params;
}

// The same build at the shipped threshold, so the pass drops the points whose
// weight is exactly zero.
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

// Both parts at once: DerivativePart names one bit each.
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

// The caller's arrays for one block, sized by SizeOf and filled with a
// sentinel, so that a refusal can be told from an answer of zeros by reading
// the buffers rather than by trusting the status.
struct Buffers {
    std::vector<double> positionFirst;
    std::vector<double> weightFirst;
    std::vector<double> positionSecond;
    std::vector<double> weightSecond;

    // The spans the interface is handed, into this object's own arrays.  Taken
    // after the object is in its final place: a span into a vector that is then
    // moved would point at freed storage.
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

// The product-grid identity of a block point, recovered from the point itself.
//
// A block carries its points, its weights and its owning atoms, and nothing
// that names which radial point and which angular node each came from, so this
// is the reconstruction any consumer of the interface has to make as well: the
// radial point nearest the distance to the owning atom, the angular node the
// direction is most nearly along, and then the rebuild that turns the pair into
// an answer.
//
// The margins are asserted rather than assumed.  A fixture in which the nearest
// radial point or the best-aligned node were ambiguous would make every
// comparison below pass for the wrong reason, which is a failure mode a
// tolerance cannot see.
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

// The product grid's own enumeration order: atom, then radial point, then
// angular node.  The build re-batches its blocks away from this order, so the
// order is a name for a point that does not depend on which block it landed in.
[[nodiscard]] std::size_t FlatIndex(const GridBasis& basis, const PointId& id) {
    return (id.atom * basis.radial.Size() + id.radial) * basis.angular.Size() + id.angular;
}

// The consumer's per-point field.  Broad enough that the outer shells still
// carry it, and centred away from the origin so that no symmetry of the fixture
// can cancel the total it is integrated to.
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

// The consumer's quantity: sum over the grid's points of w_g f(x_g), taken on
// the weights the build hands out rather than on any formula beside them.
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

// The magnitude of the consumer's quantity, taken over the terms that make it
// up rather than over their sum: a grid's weights are not all of one sign, so
// the sum can cancel and it is the terms that fix how finely it can be read.
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

// A number in scientific notation, which is what a measured figure wants:
// std::to_string would round the small ones to zero and report nothing.
[[nodiscard]] std::string Scientific(double value) {
    std::ostringstream text;
    text << std::scientific << std::setprecision(3) << value;

    return text.str();
}

// The same total's derivative, assembled entirely from the interface: the
// weight derivative times the field plus the field's own gradient carried by
// the point's motion.  The provider is taken by its base reference, because a
// consumer holds a GridDerivativeProvider and not this library's own type.
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

// The interface answers the question the pass answers.  The comparison is
// bit for bit, because both sides call the same routine with the same identity:
// a difference here is the interface introducing one, and nothing else.
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

            // A point is a rigid translate of its owning atom, so the
            // coordinate derivative is the owner's own identity block and
            // nothing else.
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

            // The value the pass produces is the weight the build handed out,
            // and the build is a different translation unit: it is the same
            // expression over the same point, so the two agree to a rounding
            // and not necessarily bit for bit.
            EXPECT_NEAR(reference.value, block.weights[index],
                        1e-12 * (1.0 + std::abs(block.weights[index])))
                << "point " << index;
            ++visited;
        }
    }

    EXPECT_EQ(visited, grid->TotalPointCount());
    EXPECT_EQ(visited, FlatCount(geometry, *basis));
}

// The second order, the same way.  The coordinate half is identically zero and
// the weight half is the pass's own matrix, mirrored rather than recomputed, so
// the comparison is exact on both.
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

// A consumer's own call, through the base class, against a difference of the
// build it built: the interface is usable, and what it is usable for agrees
// with the geometry it describes.
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

    // A total that is not small: a gradient of zero would compare well against
    // any difference and prove nothing.
    EXPECT_GT(Magnitude(*analytic), 1e-3);

    // Two steps per coordinate, combined into the estimate that cancels the
    // difference's own h^2 term.  What is left is the difference's rounding,
    // which for a sum of this size over a step this small is the resolution of
    // the comparison: the bound below is that rounding and not a magnitude
    // picked to fit.
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

    // The measured numbers, so that a reader of a red run knows how far the two
    // sides were apart and not only that they were.
    RecordProperty("worst_error", Scientific(worst));
    RecordProperty("resolution", Scientific(resolution));
    RecordProperty("gradient_magnitude", Scientific(Magnitude(*analytic)));
    RecordProperty("term_magnitude", Scientific(terms));
}

// The build's trim and its re-batching, seen from the interface.  Every point
// the build kept is one this construction differentiates, the kept points are
// exactly the product grid's points that survived the trim - each of them once,
// wherever the re-batching put them - and the trim really did drop some.
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

    // The premise, as the pass measured it: the trim removes the points whose
    // weight is exactly zero, and nothing else.
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

        // The trim has not made the grid's own points undifferentiable: every
        // block the build kept is answered, none of them refused.
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

    // The blocks are re-batched, not enumerated: without this the identity
    // recovery above would be reading points out of the order they were built
    // in, and would prove nothing about a re-batched block.
    EXPECT_GT(reordered, 0u);
}

// The order the provider was not built for is refused by name, and the caller's
// arrays are left holding the sentinel: the refusal is not answered with zeros,
// which is the failure this interface exists to prevent.
TEST(GridDerivativeProviderTest, SecondOrderIsRefusedByNameAndLeavesTheBuffersAlone) {
    const Geometry geometry = Triatomic();
    const GridParams params = BlockParams();

    const excgrid::Result<BlockGrid> grid = BlockGrid::Create(geometry, params);
    ASSERT_TRUE(grid.has_value());
    const excgrid::Result<BlockGridDerivativeProvider> provider =
        BlockGridDerivativeProvider::Create(geometry, params, DerivativeOrder::kFirst);
    ASSERT_TRUE(provider.has_value());
    ASSERT_EQ(provider->MaxOrder(), DerivativeOrder::kFirst);

    // A request sized exactly as the contract says to size one: a caller that
    // asked for the second order and got silence would be reading these.
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

    // The same block, at the order the provider does supply, is answered.
    const DerivativeRequest first = MakeRequest(geometry, BothParts(), DerivativeOrder::kFirst);
    const Block& block = grid->Blocks().front();
    Buffers buffers = MakeBuffers(block, first, sentinel);
    EXPECT_EQ(provider->Evaluate(block, first, buffers.View(block)), DerivativeStatus::kOk);
    EXPECT_FALSE(buffers.AllHold(sentinel));
}

// A block from another geometry is refused rather than differentiated.  The
// points of an atom that did not move are on both grids and a point on the
// switch's plateau does not feel the moved atom either, so the count of
// refusals is what the fixture gives - but a swept coordinate moves its own
// atom's whole sub-grid, and that sub-grid is somewhere in every sweep.
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
                        // A block can survive: a point owned by an atom that did
                        // not move, and far enough from the one that did, is on
                        // both grids with the weight the block carries, so there
                        // is nothing about it to refuse.  What cannot survive is
                        // the sweep - the atom that moved has a sub-grid of its
                        // own, and it is in some block.
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

// A block carrying a point this geometry's build would have trimmed is refused
// by name too.  The two builds here differ only in the threshold, so every
// point is on the grid and every position matches: what is refused is the point
// set, which is the condition the status names.
TEST(GridDerivativeProviderTest, TrimmedPointIsRefusedByName) {
    const Geometry geometry = Triatomic();

    // The grid as it would be built with the trim off, and a provider for the
    // same molecule with the shipped threshold.  The point sets differ and
    // nothing else does.
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

    // The fixture has to contain the case, and the pass has to have found it.
    EXPECT_GT(expectedRefused, 0u);
    EXPECT_EQ(refusedPoints, expectedRefused);
    EXPECT_GT(refusedBlocks, 0u);
}

// A request whose frame is not this geometry's cannot be filled at all: the
// arrays are indexed by nuclear coordinate, and the wrong count is not a
// smaller answer.
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

    // A frame one coordinate short.
    DerivativeRequest wrongFrame = MakeRequest(geometry, BothParts(), DerivativeOrder::kFirst);
    --wrongFrame.nuclearCoordinateCount;
    {
        Buffers buffers = MakeBuffers(block, wrongFrame, sentinel);
        EXPECT_EQ(provider->Evaluate(block, wrongFrame, buffers.View(block)),
                  DerivativeStatus::kRefusedBufferTooSmall);
        EXPECT_TRUE(buffers.AllHold(sentinel));
    }

    // Right frame, buffers one element short of what SizeOf asks for.
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

// A consumer reaches the provider through the public header and nothing else.
//
// Every construction above goes through the library's own internal header,
// which a consumer cannot include, so none of them shows that the seam is open
// at all - a provider can be perfectly tested and still be unobtainable.  This
// builds one the way a consumer would, from the public declaration alone, and
// checks that what comes back answers.
TEST(GridDerivativeProviderTest, AConsumerBuildsOneThroughThePublicHeader) {
    const Geometry geometry = Triatomic();
    const GridParams params = BlockParams();

    excgrid::Result<std::unique_ptr<GridDerivativeProvider>> built =
        excgrid::CreateGridDerivativeProvider(geometry, params, DerivativeOrder::kFirst);
    ASSERT_TRUE(built.has_value()) << "a consumer cannot obtain a provider at all";

    const GridDerivativeProvider& provider = **built;
    EXPECT_EQ(provider.MaxOrder(), DerivativeOrder::kFirst);

    // The order asked for is the order answered, so the depth reaches the
    // provider rather than defaulting.
    excgrid::Result<std::unique_ptr<GridDerivativeProvider>> deeper =
        excgrid::CreateGridDerivativeProvider(geometry, params, DerivativeOrder::kSecond);
    ASSERT_TRUE(deeper.has_value());
    EXPECT_EQ((*deeper)->MaxOrder(), DerivativeOrder::kSecond);
}
