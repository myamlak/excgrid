// The block grid's implementation of GridDerivativeProvider.

#pragma once

#include <memory>

#include "excgrid/geometry.hpp"
#include "excgrid/grid.hpp"
#include "excgrid/grid_derivatives.hpp"

namespace excgrid {

/// The grid's geometric derivatives, over its blocks.  One instance holds one
/// geometry's work arrays, so it is not safe to share between threads.
class BlockGridDerivativeProvider final : public GridDerivativeProvider {
public:
    /// Builds the provider for a geometry and a set of build parameters.
    /// \param order The deepest order this provider supplies; a request past it
    /// is refused rather than answered with zeros.
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
    /// \param block A block of the grid this provider was built for.
    /// \param request What to produce; its nuclearCoordinateCount must be 3N.
    /// \param derivatives The caller's buffers, sized by SizeOf; a refusal leaves
    /// the points before it written, so they may be read only when the status is
    /// kOk.
    [[nodiscard]] DerivativeStatus Evaluate(const Block& block,
                                            const DerivativeRequest& request,
                                            const BlockDerivatives& derivatives) const override;

private:
    /// The geometry, the quadratures and the work arrays.
    struct State;

    explicit BlockGridDerivativeProvider(std::unique_ptr<State> state) noexcept;

    std::unique_ptr<State> _state;
};

} // namespace excgrid
