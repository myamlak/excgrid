#pragma once

// The grid's derivatives with respect to nuclear position: the point coordinates
// and the Becke partition weights, in the first and the second order.
//
// Totals, the point moving with its owner a:
//     dW/dR_b|tot      = dW/dR_b + delta_ba dW/dx,
//     d2W/dR_bdR_c|tot = d2W/dR_bdR_c + delta_ca d2W/dxdR_b
//                      + delta_ba d2W/dxdR_c^T + delta_ba delta_ca d2W/dx2.
// Indices: first derivative of component k of atom b at 3b + k; second of
// (b, k) and (c, l) at (3b + k) * 3N + (3c + l), row-major (3N) x (3N).
// Units: Bohr; first derivative per Bohr, second per Bohr^2.

#include "excgrid/geometry.hpp"
#include "excgrid/grid.hpp"

#include <array>
#include <cstddef>
#include <vector>

namespace excgrid::derivs {

/// The SSF switch at one argument, with its first two derivatives.
struct SwitchValue {
    double value = 0.0; ///< S(mu).
    double first = 0.0; ///< dS/dmu.
    double second = 0.0; ///< d2S/dmu2.
};

/// Evaluates the switch and its derivatives.
/// \param mu The switch argument, (r_c - r_d) / (rad_c + rad_d).
[[nodiscard]] SwitchValue SsfSwitch(double mu) noexcept;

/// The Bragg-Slater covalent radius of an element, in Bohr.
/// \returns The radius in Bohr, or the 1.0 A fallback for an untabulated
/// element.
[[nodiscard]] double BraggSlaterRadius(int atomicNumber) noexcept;

/// How many derivative orders an evaluation produces.
enum class DerivativeOrder {
    kFirst = 1, ///< First derivatives only: 3N doubles per point.
    kSecond = 2, ///< First and second: 3N + (3N)^2 doubles per point.
};

/// The work arrays of the derivative pass, sized once for a geometry.
struct DerivativeScratch {
    std::size_t atomCount = 0; ///< N: atoms in the geometry.
    DerivativeOrder order = DerivativeOrder::kFirst; ///< Which orders to build.

    std::vector<double> radius; ///< N: Bragg-Slater radius per atom, Bohr.
    std::vector<double> distance; ///< N: |point - atom|.
    std::vector<double> lo; ///< N: distance - a * radius.
    std::vector<double> hi; ///< N: distance + a * radius.
    std::vector<double> u; ///< 3N: the unit vectors (point - atom)/|point - atom|.

    std::vector<double> sumFirst; ///< N: d(sum_c raw_c)/dr_e, accumulated per point.
    std::vector<double> ownFirst; ///< N: d(raw_owner)/dr_e, accumulated per point.
    std::vector<double> gradient; ///< N: dW/dr_e after the quotient rule.

    /// 3N: v_c = sum_d H_cd u_d per active atom.
    std::vector<double> chain;

    std::vector<std::size_t> member; ///< N: the live cell's A_c, built per cell.
    std::vector<std::size_t> active; ///< N: the atoms with a non-zero row.
    std::vector<unsigned> mark; ///< N: generation stamps, so `active` clears in O(1).
    unsigned generation = 0; ///< The stamp `mark` is compared against.
    std::vector<double> switchValue; ///< N: S(mu) per member of the live cell.
    std::vector<double> switchFirst; ///< N: S'(mu) per member.
    std::vector<double> switchSecond; ///< N: S''(mu) per member.
    std::vector<double> prefix; ///< N + 1: running product of switchValue.
    std::vector<double> suffix; ///< N + 1: the same, from the far end.

    /// The scratch's whole footprint, in doubles.
    [[nodiscard]] std::size_t DoubleCount() const noexcept;

    /// The scratch's footprint per point, in doubles, excluding the O(N^2)
    /// output the caller owns.
    [[nodiscard]] static std::size_t PerPointDoubles(std::size_t atomCount) noexcept;
};

/// Builds the scratch for a geometry, with the radii resolved once.
[[nodiscard]] DerivativeScratch MakeDerivativeScratch(const Geometry& geometry,
                                                      DerivativeOrder order);

/// One point's weight and its derivatives with respect to the nuclear
/// coordinates.
struct PointWeightDerivatives {
    /// The differentiated quantity: W_a, or the point's grid weight q * W_a.
    double value = 0.0;

    /// The atoms with a non-zero row: the owner plus every live cell's members.
    std::size_t activeAtoms = 0;

    /// 3N entries: d(value)/dR, zero outside an active row.
    std::vector<double> first;

    /// (3N)^2 entries, row-major, when the order is kSecond: d2(value)/dR dR;
    /// empty at kFirst, zero outside active rows and columns, mirrored bit for bit.
    std::vector<double> second;

    /// The active atoms whose blocks the previous point left set in \c second.
    std::vector<std::size_t> dirty;
};

/// The quadrature half of a grid: radial and angular nodes, built once per pass.
struct GridBasis {
    RadialGrid radial; ///< The radial quadrature, innermost point first.
    AngularGrid angular; ///< The angular quadrature on the unit sphere.
};

/// Builds the quadrature half of the grid for a set of build parameters.
/// \returns The basis, or the underlying quadratures' error.
[[nodiscard]] Result<GridBasis> MakeGridBasis(const GridParams& params);

/// One point of the un-trimmed product grid.
struct PointId {
    std::size_t atom = 0; ///< The owning atom.
    std::size_t radial = 0; ///< Its radial index, in [0, RadialGrid::Size()).
    std::size_t angular = 0; ///< Its angular index, in [0, AngularGrid::Size()).
};

/// The point a build would generate for an identity, in Bohr.
/// \returns The position, R_atom + radial point * angular node.
[[nodiscard]] std::array<double, 3> PointPosition(const Geometry& geometry,
                                                  const GridBasis& basis,
                                                  const PointId& id);

/// The quadrature factor of a point: its radial weight times rho^2 times its
/// angular weight, all of it independent of the geometry.
/// \returns The factor q, so that the point's grid weight is q * W_owner.
[[nodiscard]] double PointQuadrature(const GridBasis& basis, const PointId& id);

/// The coordinate derivatives of one grid point: its owner's 3 x 3 identity
/// block inside a 3 x 3N frame, and a second derivative identically zero.
struct PointCoordinateDerivatives {
    /// 3 x 3N entries: dx_k/dR_{(b,l)} at index k * 3N + 3b + l.
    std::vector<double> first;

    /// (3N)^2 entries, all zero, when the order is kSecond.  Empty at kFirst.
    std::vector<double> second;
};

/// Fills the coordinate derivatives of a point.
/// \param order Which orders to fill; kSecond fills \c second with zeros.
/// \param out Receives the derivatives, resized to the order's footprint.
void EvaluatePointCoordinates(std::size_t owner,
                              std::size_t atomCount,
                              DerivativeOrder order,
                              PointCoordinateDerivatives& out);

/// Evaluates one point of the product grid: its grid weight, with the
/// quadrature factor folded in, and the derivatives of that weight with respect
/// to the 3N nuclear coordinates.
/// \param scratch The work arrays, sized for \p geometry.
/// \param out Receives the weight and its derivatives.
void EvaluatePointWeight(const Geometry& geometry,
                         const GridBasis& basis,
                         const PointId& id,
                         DerivativeScratch& scratch,
                         PointWeightDerivatives& out);

/// Evaluates the partition weight of one atom's cell at a point, and its first
/// two derivatives with respect to the 3N nuclear coordinates.  The point moves
/// with \p owner, so the derivatives are the totals at the top of this header,
/// and the value is the normalized W_owner without the quadrature factor q.
/// \param point The point, in Bohr.
/// \param scratch The work arrays, sized for \p geometry.
/// \param out Receives the weight and, in \c first and \c second, its 3N and
/// (3N)^2 derivatives; \c out.dirty records the blocks left set.
void EvaluatePartitionWeight(const Geometry& geometry,
                             const std::array<double, 3>& point,
                             std::size_t owner,
                             DerivativeScratch& scratch,
                             PointWeightDerivatives& out);

} // namespace excgrid::derivs
