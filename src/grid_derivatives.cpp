#include "excgrid/grid_derivatives.hpp"

#include "block_grid_derivatives.hpp"
#include "grid_derivs.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

namespace excgrid {

BlockDerivativeSizes SizeOf(const DerivativeRequest& request, std::size_t pointCount) noexcept {
    const bool coordinates =
        (static_cast<std::uint8_t>(request.parts) &
         static_cast<std::uint8_t>(DerivativePart::kCoordinates)) != 0U;
    const bool weights = (static_cast<std::uint8_t>(request.parts) &
                          static_cast<std::uint8_t>(DerivativePart::kWeights)) != 0U;
    const bool second = request.order == DerivativeOrder::kSecond;
    const std::size_t n = request.nuclearCoordinateCount;

    BlockDerivativeSizes sizes;
    if (coordinates) {
        sizes.positionFirst = pointCount * 3U * n;
        if (second) {
            sizes.positionSecond = pointCount * 3U * n * n;
        }
    }
    if (weights) {
        sizes.weightFirst = pointCount * n;
        if (second) {
            sizes.weightSecond = pointCount * n * n;
        }
    }
    return sizes;
}

namespace {

/// Whether a request's part mask asks for \p part.
[[nodiscard]] bool AsksFor(DerivativePart parts, DerivativePart part) noexcept {
    return (static_cast<std::uint8_t>(parts) & static_cast<std::uint8_t>(part)) != 0U;
}

/// How far a rebuilt position may sit from the block's before the point is
/// called a different point.
///
/// The build and this pass both evaluate R_atom + rho * n, but they are separate
/// translation units and a compiler is free to contract that multiply-add in one
/// and not in the other, so the two agree only to a rounding and never bit for
/// bit.  The bound is relative as well as absolute because a point's own
/// coordinates grow with the molecule's distance from the origin.
constexpr double kIdentityTolerance = 1e-12;

/// How far this pass's weight may sit from the one the block carries before the
/// block is called a different grid's.
///
/// The build and this pass compute q * W from the same quadrature and the same
/// partition, so a weight that does not match to a rounding is not a rounding:
/// the block's weights came from a geometry whose cells sit somewhere else.
/// The bound is far above the few ulp the two translation units can differ by
/// and far below the change any step a caller would take produces.
constexpr double kWeightTolerance = 1e-10;

/// Recovers the product-grid identity of a block point from its position.
///
/// The radial index is the radial point nearest the distance to the owning atom
/// and the angular index is the node the direction is most nearly along; both
/// are candidates rather than answers, so the position is rebuilt from them and
/// the identity is returned only when the rebuild reproduces the point.  A block
/// point that is not on this geometry's product grid - the state a build leaves
/// when the point set moves under a finite-difference step - cannot reproduce
/// it.
///
/// The search is over the whole node set, because a re-batched block orders its
/// points by proximity and not by the order the build enumerated them, so no
/// point's node says anything about its neighbour's.  The cost is therefore
/// |A| alignments per point where the weight pass itself is O(N + |A|^2): it is
/// the larger of the two only for a molecule small enough that neither is
/// large.
/// \param geometry The molecule.
/// \param basis The quadrature half of the grid.
/// \param nodes The angular nodes, flat, three per node.
/// \param block The block.
/// \param index Which of the block's points.
/// \param out Receives the identity, when there is one.
/// \returns Whether the point is one of this grid's.
[[nodiscard]] bool RecoverIdentity(const Geometry& geometry,
                                   const derivs::GridBasis& basis,
                                   std::span<const double> nodes,
                                   const Block& block,
                                   std::size_t index,
                                   derivs::PointId& out) {
    const std::size_t atom = block.atomIndex[index];
    const std::array<double, 3>& point = block.points[index];
    const std::array<double, 3>& center = geometry.atoms[atom].position;
    const double dx = point[0] - center[0];
    const double dy = point[1] - center[1];
    const double dz = point[2] - center[2];
    const double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (!(distance > 0.0))
    {
        return false;
    }

    const std::span<const double> radii = basis.radial.Points();
    const auto above = std::lower_bound(radii.begin(), radii.end(), distance);
    std::size_t radial = 0;
    if (above == radii.begin())
    {
        radial = 0;
    }
    else if (above == radii.end())
    {
        radial = radii.size() - 1;
    }
    else
    {
        const std::size_t high = static_cast<std::size_t>(above - radii.begin());
        radial = (distance - radii[high - 1] <= radii[high] - distance) ? high - 1 : high;
    }

    out.atom = atom;
    out.radial = radial;

    const auto reproduces = [&](std::size_t angular) {
        out.angular = angular;
        const std::array<double, 3> rebuilt = derivs::PointPosition(geometry, basis, out);
        for (std::size_t k = 0; k < 3; ++k)
        {
            if (std::abs(rebuilt[k] - point[k]) > kIdentityTolerance * (1.0 + std::abs(point[k])))
            {
                return false;
            }
        }
        return true;
    };

    const double inverse = 1.0 / distance;
    std::size_t angular = 0;
    double best = -2.0;
    for (std::size_t node = 0; node < basis.angular.Size(); ++node)
    {
        const double* direction = nodes.data() + (3U * node);
        const double alignment = (dx * direction[0] + dy * direction[1] + dz * direction[2]) * inverse;
        if (alignment > best)
        {
            best = alignment;
            angular = node;
        }
    }

    return reproduces(angular);
}

} // namespace

/// The geometry, the quadratures, the trim threshold and the work arrays.
struct BlockGridDerivativeProvider::State {
    State(Geometry molecule, derivs::GridBasis quadratures, double threshold,
          derivs::DerivativeOrder depth) :
        geometry(std::move(molecule)),
        basis(std::move(quadratures)),
        trimWeight(threshold),
        order(depth),
        scratch(derivs::MakeDerivativeScratch(geometry, depth)) {
        // The angular nodes are geometry-independent, so the identity search
        // walks a flat copy of them once rather than the quadrature's own
        // accessor per node.
        nodes.resize(3U * basis.angular.Size());
        for (std::size_t node = 0; node < basis.angular.Size(); ++node)
        {
            const std::array<double, 3> direction = basis.angular.Point(node);
            std::copy(direction.begin(), direction.end(), nodes.begin() + (3U * node));
        }
    }

    Geometry geometry; ///< The molecule, as the grid was built for it.
    derivs::GridBasis basis; ///< The quadrature half, as the grid was built with it.
    std::vector<double> nodes; ///< The angular nodes, flat, three per node.
    double trimWeight = 0.0; ///< The build's threshold for keeping a point.
    derivs::DerivativeOrder order = derivs::DerivativeOrder::kFirst; ///< What to compute.

    derivs::DerivativeScratch scratch; ///< The derivative pass's work arrays.
    derivs::PointWeightDerivatives weight; ///< One point's weight derivatives.
    derivs::PointCoordinateDerivatives coordinates; ///< One point's coordinate derivatives.
    std::vector<derivs::PointId> identity; ///< One recovered identity per block point.
};

Result<BlockGridDerivativeProvider> BlockGridDerivativeProvider::Create(const Geometry& geometry,
                                                                        const GridParams& params,
                                                                        DerivativeOrder order) {
    Result<derivs::GridBasis> basis = derivs::MakeGridBasis(params);
    if (!basis)
    {
        return std::unexpected(basis.error());
    }

    const derivs::DerivativeOrder depth = order == DerivativeOrder::kSecond
                                              ? derivs::DerivativeOrder::kSecond
                                              : derivs::DerivativeOrder::kFirst;

    return BlockGridDerivativeProvider(
        std::make_unique<State>(geometry, std::move(*basis), params.trimWeight, depth));
}

BlockGridDerivativeProvider::BlockGridDerivativeProvider(std::unique_ptr<State> state) noexcept :
    _state(std::move(state)) {
}

BlockGridDerivativeProvider::~BlockGridDerivativeProvider() = default;
BlockGridDerivativeProvider::BlockGridDerivativeProvider(BlockGridDerivativeProvider&& other) noexcept =
    default;
BlockGridDerivativeProvider& BlockGridDerivativeProvider::operator=(
    BlockGridDerivativeProvider&& other) noexcept = default;

DerivativeOrder BlockGridDerivativeProvider::MaxOrder() const noexcept {
    return _state->order == derivs::DerivativeOrder::kSecond ? DerivativeOrder::kSecond
                                                             : DerivativeOrder::kFirst;
}

DerivativeStatus BlockGridDerivativeProvider::Evaluate(const Block& block,
                                                       const DerivativeRequest& request,
                                                       const BlockDerivatives& derivatives) const {
    State& state = *_state;
    const std::size_t atomCount = state.geometry.atoms.size();
    const std::size_t width = 3U * atomCount;

    const bool coordinates = AsksFor(request.parts, DerivativePart::kCoordinates);
    const bool weights = AsksFor(request.parts, DerivativePart::kWeights);
    const bool second = request.order == DerivativeOrder::kSecond;

    // The order the provider was built for decides what it can answer.  Asking
    // past it is a refusal and never a buffer of zeros: a caller that did not
    // build for the second order must not silently receive a Hessian-shaped
    // array of nothing.
    if (second && (state.order != derivs::DerivativeOrder::kSecond))
    {
        return DerivativeStatus::kRefusedSecondOrderUnavailable;
    }

    // The request's framework is the caller's 3N, and it must be this
    // geometry's: the arrays are indexed by nuclear coordinate, so a request
    // for a different count cannot be filled at all.
    if (request.nuclearCoordinateCount != width)
    {
        return DerivativeStatus::kRefusedBufferTooSmall;
    }

    const BlockDerivativeSizes needed = SizeOf(request, block.pointCount);
    if ((needed.positionFirst > derivatives.positionFirst.size()) ||
        (needed.positionSecond > derivatives.positionSecond.size()) ||
        (needed.weightFirst > derivatives.weightFirst.size()) ||
        (needed.weightSecond > derivatives.weightSecond.size()) ||
        (block.points.size() < block.pointCount) || (block.atomIndex.size() < block.pointCount))
    {
        return DerivativeStatus::kRefusedBufferTooSmall;
    }

    // Every point's identity, before a single value is written.  The buffers
    // therefore hold either a whole answer or none of one.
    if (weights)
    {
        state.identity.resize(block.pointCount);
        for (std::size_t index = 0; index < block.pointCount; ++index)
        {
            if (!RecoverIdentity(state.geometry, state.basis, state.nodes, block, index,
                                 state.identity[index]))
            {
                return DerivativeStatus::kRefusedUndifferentiableGrid;
            }
        }
    }

    for (std::size_t index = 0; index < block.pointCount; ++index)
    {
        const std::size_t owner = block.atomIndex[index];

        if (coordinates)
        {
            // A point is its owner's own quadrature point, so it is a rigid
            // translate of that atom: the first derivative is the owner's own
            // identity block and the second is identically zero.
            derivs::EvaluatePointCoordinates(owner, atomCount, derivs::DerivativeOrder::kFirst,
                                             state.coordinates);
            std::copy(state.coordinates.first.begin(), state.coordinates.first.end(),
                      derivatives.positionFirst.data() + index * 3U * width);

            if (second)
            {
                std::fill_n(derivatives.positionSecond.data() + index * 3U * width * width,
                            3U * width * width, 0.0);
            }
        }

        if (weights)
        {
            derivs::EvaluatePointWeight(state.geometry, state.basis, state.identity[index],
                                        state.scratch, state.weight);
            const double carried = block.weights[index];

            // The block's own weight is the grid's statement about this point,
            // and the pass has just computed what this geometry's build would
            // state instead.  A point of an atom that did not move is on both
            // grids, so the position check above cannot see a partial move; the
            // weight can, and this is where the two geometries part company.
            if (std::abs(state.weight.value - carried) >
                kWeightTolerance * std::max(std::abs(state.weight.value), std::abs(carried)))
            {
                return DerivativeStatus::kRefusedUndifferentiableGrid;
            }

            // The build keeps a point when its weight reaches the threshold and
            // drops it otherwise, so a block carrying a point this geometry's
            // build would have dropped is a block from a grid whose point set
            // moved.  Its weights sum over a different set of points than any
            // this geometry has, and no derivative of that sum describes this
            // geometry.
            if (std::abs(state.weight.value) < state.trimWeight)
            {
                return DerivativeStatus::kRefusedUndifferentiableGrid;
            }

            std::copy(state.weight.first.begin(), state.weight.first.end(),
                      derivatives.weightFirst.data() + index * width);

            if (second)
            {
                std::copy(state.weight.second.begin(), state.weight.second.end(),
                          derivatives.weightSecond.data() + index * width * width);
            }
        }
    }

    return DerivativeStatus::kOk;
}

Result<std::unique_ptr<GridDerivativeProvider>> CreateGridDerivativeProvider(
    const Geometry& geometry,
    const GridParams& params,
    DerivativeOrder order) {
    Result<BlockGridDerivativeProvider> provider =
        BlockGridDerivativeProvider::Create(geometry, params, order);
    if (!provider)
    {
        return std::unexpected(provider.error());
    }

    return std::make_unique<BlockGridDerivativeProvider>(std::move(*provider));
}

} // namespace excgrid
