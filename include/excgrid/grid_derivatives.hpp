#pragma once

#include <cstddef>
#include <memory>
#include <span>

#include "excgrid/components.hpp"
#include "excgrid/grid.hpp"

namespace excgrid {

/// \defgroup excgrid-gridderivs The grid's geometric derivative interface
///
/// How the grid points and their weights move when the nuclei move; indexed by
/// nuclear coordinate rather than by density component, and walked block by block.
/// \{

/// How deep a derivative request goes.
/// \ingroup excgrid-gridderivs
enum class DerivativeOrder : std::uint8_t {
    kFirst = 1, ///< The coordinate and weight derivatives.
    kSecond = 2, ///< Additionally their second derivatives.
};

/// Which quantities a request asks for.
/// \ingroup excgrid-gridderivs
enum class DerivativePart : std::uint8_t {
    kCoordinates = 1U << 0U, ///< The derivatives of the point positions.
    kWeights = 1U << 1U, ///< The derivatives of the quadrature weights.
};

/// The derivatives of one block, in the caller's own arrays: points in block
/// order, then the caller's 3N nuclear coordinates.
/// \ingroup excgrid-gridderivs
struct BlockDerivatives {
    /// d(position)/dR, shape {pointCount, 3, 3N}: the derivative of point p's
    /// Cartesian direction c with respect to nuclear coordinate q.
    std::span<double> positionFirst;
    /// d(weight)/dR, shape {pointCount, 3N}, when the weights were asked for.
    std::span<double> weightFirst;
    /// d2(position)/dR2, shape {pointCount, 3, 3N, 3N}, when second order was
    /// asked for.  Symmetric in the last two indices.
    std::span<double> positionSecond;
    /// d2(weight)/dR2, shape {pointCount, 3N, 3N}, when second order was asked
    /// for.  Symmetric.
    std::span<double> weightSecond;
    /// One owning-atom index per point, as the block itself carries.
    std::span<const std::size_t> atomIndex;
};

/// How a derivative request ended.
/// \ingroup excgrid-gridderivs
enum class DerivativeStatus : std::uint8_t {
    kOk = 0, ///< The values were produced.
    /// Second-order derivatives were asked for and this build does not supply them.
    kRefusedSecondOrderUnavailable = 1,
    /// The caller's buffers are too small for the block.
    kRefusedBufferTooSmall = 2,
    /// The block carries a point this geometry's grid does not produce.
    kRefusedUndifferentiableGrid = 3,
};

/// One request's parameters.
/// \ingroup excgrid-gridderivs
struct DerivativeRequest {
    DerivativePart parts = DerivativePart::kCoordinates; ///< What to produce.
    DerivativeOrder order = DerivativeOrder::kFirst; ///< How deep.
    std::size_t nuclearCoordinateCount = 0; ///< 3N, the caller's ordering.
};

/// The element counts BlockDerivatives' arrays need.
/// \ingroup excgrid-gridderivs
struct BlockDerivativeSizes {
    std::size_t positionFirst = 0; ///< Elements in positionFirst.
    std::size_t weightFirst = 0; ///< Elements in weightFirst.
    std::size_t positionSecond = 0; ///< Elements in positionSecond.
    std::size_t weightSecond = 0; ///< Elements in weightSecond.
};

/// The size of each buffer a block's derivatives need.
/// \param request The request.
/// \param pointCount The block's point count.
/// \returns The element counts, in the order of BlockDerivatives.
/// \ingroup excgrid-gridderivs
[[nodiscard]] BlockDerivativeSizes SizeOf(const DerivativeRequest& request,
                                          std::size_t pointCount) noexcept;

/// Produces a block's geometric derivatives, tied to the grid it was made for.
/// \ingroup excgrid-gridderivs
class GridDerivativeProvider {
public:
    virtual ~GridDerivativeProvider() = default;

    /// The deepest order this provider supplies.
    /// \returns The order.
    [[nodiscard]] virtual DerivativeOrder MaxOrder() const noexcept = 0;

    /// Fills a caller's buffers with one block's derivatives.
    /// \param block The block, from the grid this provider was made for.
    /// \param request What to produce.
    /// \param derivatives The caller's buffers, sized by SizeOf.
    /// \returns kOk, or a named refusal.
    [[nodiscard]] virtual DerivativeStatus Evaluate(const Block& block,
                                                    const DerivativeRequest& request,
                                                    const BlockDerivatives& derivatives) const = 0;
};

/// Builds the provider for a geometry and a set of grid parameters.
/// \param geometry The molecule the caller's grid was built for.
/// \param params The parameters the caller's grid was built with.
/// \param order The deepest order wanted; a request past it is refused by name,
/// never answered with zeros.
/// \returns The provider, or a named error when the grid cannot be built for
/// \p params.
/// \ingroup excgrid-gridderivs
[[nodiscard]] Result<std::unique_ptr<GridDerivativeProvider>> CreateGridDerivativeProvider(
    const Geometry& geometry,
    const GridParams& params,
    DerivativeOrder order = DerivativeOrder::kFirst);

} // namespace excgrid
