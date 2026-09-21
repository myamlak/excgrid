// The derivative provider the block grid's own construction backs.
//
// This is the library's implementation of GridDerivativeProvider: it produces
// the derivatives of the points and the weights of the blocks that
// BlockGrid::Create hands out for the same geometry and the same build
// parameters.  It carries no state the grid carries and the grid carries no
// state it does; the two are held together by the geometry, which is why the
// caller must build it for the geometry and parameters it built its grid with.

#pragma once

#include <memory>

#include "excgrid/geometry.hpp"
#include "excgrid/grid.hpp"
#include "excgrid/grid_derivatives.hpp"

namespace excgrid {

/// The grid's geometric derivatives, over its blocks.
///
/// Every call recovers each block point's identity from the point itself - the
/// block carries its owning atom and its position, and the position is the
/// geometry's own quadrature point of that atom - so the derivatives and the
/// grid cannot disagree about which point is being differentiated.  A block
/// this construction did not produce is refused rather than answered.
///
/// One instance holds the work arrays of one geometry and is therefore not
/// safe to share between threads; a consumer running blocks in parallel builds
/// one per thread.  Evaluate() is const and may be called repeatedly, and does
/// not retain anything about the block it was last given.
class BlockGridDerivativeProvider final : public GridDerivativeProvider {
public:
    /// Builds the provider for a geometry and a set of build parameters.
    /// \param geometry The molecule the caller's grid was built for.
    /// \param params The parameters the caller's grid was built with.
    /// \param order The deepest order this provider supplies.  A request past
    /// it is refused by name rather than answered with zeros.
    /// \returns The provider, or kInvalidArgument when a quadrature fails to
    /// build for \p params.
    [[nodiscard]] static Result<BlockGridDerivativeProvider> Create(
        const Geometry& geometry,
        const GridParams& params,
        DerivativeOrder order = DerivativeOrder::kFirst);

    ~BlockGridDerivativeProvider() override;

    BlockGridDerivativeProvider(BlockGridDerivativeProvider&& other) noexcept;
    BlockGridDerivativeProvider& operator=(BlockGridDerivativeProvider&& other) noexcept;
    BlockGridDerivativeProvider(const BlockGridDerivativeProvider&) = delete;
    BlockGridDerivativeProvider& operator=(const BlockGridDerivativeProvider&) = delete;

    /// \returns The order Create was asked for.
    [[nodiscard]] DerivativeOrder MaxOrder() const noexcept override;

    /// Fills a caller's buffers with one block's derivatives.
    ///
    /// The buffers are checked and every point's identity is recovered before
    /// anything is written, so a block this geometry's grid does not produce is
    /// refused before it is answered.  A refusal past that point - a weight this
    /// geometry does not reproduce, or one its trim threshold would have dropped
    /// - leaves the points before it written, so the buffers may be read only
    /// when the status is kOk.
    /// \param block A block of the grid this provider was built for.
    /// \param request What to produce; its nuclearCoordinateCount must be 3N.
    /// \param derivatives The caller's buffers, sized by SizeOf.
    /// \returns kOk, or a named refusal.
    [[nodiscard]] DerivativeStatus Evaluate(const Block& block,
                                            const DerivativeRequest& request,
                                            const BlockDerivatives& derivatives) const override;

private:
    /// The geometry, the quadratures and the work arrays.  Held by pointer so
    /// that this header reaches neither the derivative pass's internals nor the
    /// quadrature tables.
    struct State;

    explicit BlockGridDerivativeProvider(std::unique_ptr<State> state) noexcept;

    std::unique_ptr<State> _state;
};

} // namespace excgrid
