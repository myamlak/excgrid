#pragma once

#include "excgrid/error.hpp"
#include "excgrid/geometry.hpp"
#include "excgrid/internal/lebedev_tables.hpp"

#include <array>
#include <cstddef>
#include <span>
#include <vector>

namespace excgrid {

/// \defgroup excgrid-grid Molecular block-grid construction
///
/// The radial and angular quadratures, the Becke fuzzy-cell partition,
/// and the trimmed, spatially re-batched molecular blocks the kernel
/// consumers walk (docs/kernel-api.md section 5).
/// \{

/// A radial quadrature for the atomic radial coordinate, using the
/// Murray-Handy-Laming substitution (Mol. Phys. 78 (1993) 997) with
/// Euler-Maclaurin trapezoidal points:
///     r(q) = alpha * (q / (1 - q))^m
/// mapping q in [0, 1) onto r in [0, inf).  Points sit at q_i = i / N
/// with N = pointCount + 1, so the outermost finite point is
/// r_max = alpha * pointCount^m.  Weights carry the substitution
/// Jacobian dr/dq * (1/N) but NOT the r^2 of the volume element - the
/// atomic product grid folds r^2 in when it combines radial and angular
/// quadratures.
/// \ingroup excgrid-grid
class RadialGrid {
public:
    /// Creates the quadrature.
    /// \param pointCount Radial points; the outermost lies at r_max.
    /// \param alpha Length scale of the mapping (Bohr).
    /// \param exponent The mapping exponent m; 2 is the standard choice.
    /// \returns The grid, or kInvalidArgument for a zero point count or a
    /// zero exponent.
    static Result<RadialGrid> Create(std::size_t pointCount,
                                     double alpha,
                                     std::size_t exponent = 2);

    /// The radial point count.
    /// \returns The point count.
    [[nodiscard]] std::size_t Size() const noexcept {
        return _points.size();
    }

    /// The radial points, innermost first.
    /// \returns The point span (Bohr).
    [[nodiscard]] std::span<const double> Points() const noexcept {
        return _points;
    }

    /// The radial weights (Jacobian included, no r^2 factor).
    /// \returns The weight span.
    [[nodiscard]] std::span<const double> Weights() const noexcept {
        return _weights;
    }

private:
    RadialGrid(std::vector<double> points, std::vector<double> weights) noexcept;

    std::vector<double> _points;
    std::vector<double> _weights;
};

/// A Lebedev-Laikov angular quadrature on the unit sphere.
///
/// The point set is closed under the octahedral group and the weights
/// integrate every polynomial of total degree <= Degree() exactly.  The
/// tables are SOLVED for from these two defining conditions by
/// tools/gen_lebedev.py (no copied tables) and certified per size to
/// max |moment residual| < 1e-60.  Weights are normalized so that they
/// sum to 4 pi, the area of the unit sphere.
/// \ingroup excgrid-grid
class AngularGrid {
public:
    /// The available point counts, smallest first.
    static constexpr std::span<const std::size_t, internal::kLebedevSizeCount> kAvailableSizes =
        internal::kLebedevSizes;

    /// Creates the quadrature with the given point count.
    /// \param pointCount A size from kAvailableSizes.
    /// \returns The grid, or kInvalidArgument for a non-Lebedev count.
    static Result<AngularGrid> Create(std::size_t pointCount);

    /// The number of quadrature nodes.
    /// \returns The node count.
    [[nodiscard]] std::size_t Size() const noexcept {
        return _size;
    }

    /// The algebraic degree of exactness of the quadrature.
    /// \returns The exactness degree.
    [[nodiscard]] std::size_t Degree() const noexcept {
        return _degree;
    }

    /// The unit-sphere node at \p index (Bohr-free unit vector).
    /// \param index The node index in [0, Size()).
    /// \returns The node triple.
    [[nodiscard]] std::array<double, 3> Point(std::size_t index) const noexcept;

    /// The node weight at \p index (weights sum to 4 pi).
    /// \param index The node index in [0, Size()).
    /// \returns The weight.
    [[nodiscard]] double Weight(std::size_t index) const noexcept;

private:
    AngularGrid(std::size_t size, std::size_t degree, std::size_t offset) noexcept;

    std::size_t _size;
    std::size_t _degree;
    std::size_t _offset;
};

/// The molecular grid build parameters.
/// \ingroup excgrid-grid
struct GridParams {
    std::size_t radialPoints = 75; ///< Radial points per atom.
    std::size_t angularPoints = 302; ///< A Lebedev size (AngularGrid::kAvailableSizes).
    double alpha = 0.5; ///< Radial mapping scale, Bohr.
    std::size_t radialExponent = 2; ///< The MHL mapping exponent m.
    double trimWeight = 1e-15; ///< Points with |weight| below this are dropped.
    std::size_t blockTarget = 1024; ///< Spatial re-batching target (points per block).
};

/// One spatial block of the molecular grid: the parallel and cache unit
/// every consumer walks.  Points are Bohr coordinates; weights include
/// the radial and angular quadrature weights, the r^2 volume element,
/// and the normalized Becke partition weight.
/// \ingroup excgrid-grid
struct Block {
    std::size_t pointCount = 0; ///< Points in the block.
    std::vector<std::array<double, 3>> points; ///< pointCount positions, Bohr.
    std::vector<double> weights; ///< pointCount quadrature weights.
    std::vector<std::size_t> atomIndex; ///< pointCount owning atoms.
};

/// The molecular integration grid: per-atom radial x angular product
/// grids combined into one point set, each point weighted by its Becke
/// partition weight, trimmed (near-zero weights dropped) and re-batched
/// into spatially compact blocks (~GridParams::blockTarget points each).
///
/// The integral of a function f over R^3 is approximated by
///     sum over blocks, points: weight_i f(point_i).
/// \ingroup excgrid-grid
class BlockGrid {
public:
    /// Builds the grid for the geometry.
    /// \param geometry The molecule whose atoms center the sub-grids.
    /// \param params The build parameters.
    /// \returns The grid, or an Error from the underlying quadratures.
    static Result<BlockGrid> Create(const Geometry& geometry, const GridParams& params = {});

    /// The number of blocks.
    /// \returns The block count.
    [[nodiscard]] std::size_t BlockCount() const noexcept {
        return _blocks.size();
    }

    /// The blocks, in iteration order.
    /// \returns The block span.
    [[nodiscard]] std::span<const Block> Blocks() const noexcept {
        return _blocks;
    }

    /// The total point count across all blocks.
    /// \returns The total point count.
    [[nodiscard]] std::size_t TotalPointCount() const noexcept {
        return _totalPoints;
    }

private:
    BlockGrid(std::vector<Block> blocks, std::size_t totalPoints) noexcept;

    std::vector<Block> _blocks;
    std::size_t _totalPoints;
};

} // namespace excgrid
