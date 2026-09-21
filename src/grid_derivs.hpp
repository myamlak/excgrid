#pragma once

// The grid's derivatives with respect to nuclear position: the point
// coordinates, and the Becke partition weights, in the first and the second
// order.
//
// WHY THIS HEADER IS IN src/.  The public seam of this library is frozen
// (docs/kernel-api.md): the derivative surface reaches a consumer through a
// separate, later change that owns the seam, and until then this header is
// library-internal.  The library's own tests include it through a relative
// path; `include/` is untouched and nothing here is installed.
//
// THE MATHEMATICS, IN ONE PLACE.
//
// The build generates, for atom a, radial index r and angular index t, the
// point
//     x = R_a + rho_r * n_t
// with rho_r (Bohr) from the radial quadrature and n_t a unit vector from the
// angular one.  Neither rho_r nor n_t depends on the geometry, so a grid point
// is a rigid translate of its owning atom:
//     dx/dR_b = delta_ab * I3,        d2x/dR_b dR_c = 0.                (1)
// The point's weight is
//     w = q * W_a(x),     q = w_rad * rho^2 * w_ang,
// with q geometry-independent and W_a the Becke partition weight of the owning
// atom's cell at x.  That is why the coordinate's derivative is nearly trivial
// and the weight's is the whole job: W_a depends on the distance from x to
// EVERY atom.
//
// The partition is Becke's (J. Chem. Phys. 88 (1988) 2547) with the SSF step
// and the Bragg-Slater heteroatom adjustment, normalized at the point:
//     W_a(x) = raw_a / sum_c raw_c,    raw_c = prod_{d != c} S(mu_cd),
//     mu_cd = (r_c - r_d) / (rad_c + rad_d),     r_c = |x - R_c|,
// with rad the Bragg-Slater covalent radius (Slater, J. Chem. Phys. 41 (1964)
// 3199) and S the odd degree-7 switch (Stratmann, Scuseria, Frisch, Chem.
// Phys. Lett. 257 (1996) 213):
//     S(mu) = 0.5 * (1 - z(t)),   t = mu / a,
//     z(t)  = t * (35 - t^2 * (35 - t^2 * (21 - 5 * t^2))) / 16,
// zero for mu >= a and one for mu <= -a, with a = 0.64.
//
// S is C^2 at the joins, because z'(+/-1) = 0 and z''(+/-1) = 0 (both derived
// from z'(t) = 35 (1 - t^2)^3 / 16 and z''(t) = -105 t (1 - t^2)^2 / 8 in
// SsfSwitch below).  A cell whose switch is on the zero plateau for some pair
// has raw_c = 0 and all of its derivatives zero; on every other cell the
// plateau factors are exactly one, so the partition is C^2 in the geometry
// wherever no cell changes its live-pair set, and
//     raw_c = prod_{d in A_c} S(mu_cd),   A_c = {d != c : -a < mu_cd < a}. (2)
// The work per point is therefore O(N) distances plus O(|A_c|^2) for each live
// cell, and only O(1) cells are live.  The ARRAYS are another matter: the
// contract must carry 3N first derivatives and (3N)^2 second derivatives per
// point, which is a separate and measured cost.
//
// The weight derivative with respect to a NUCLEAR COORDINATE is the total,
// including the point's own motion with its owner (1).  That total is what a
// finite difference of a grid build returns and what a gradient or Hessian
// assembly consumes.  It is assembled from the derivatives with respect to the
// distances.  With
//     g_c = dW/dr_c,   H_cd = d2W/dr_c dr_d,   u_c = (x - R_c)/r_c,
//     T_c = (I - u_c u_c^T)/r_c,               v_c = sum_d H_cd u_d,
// the chain rule gives
//     dW/dx        = sum_c g_c u_c,
//     dW/dR_b      = -g_b u_b,
//     d2W/dx2      = sum_c (g_c T_c + u_c v_c^T),
//     d2W/dxdR_b   = -(g_b T_b + u_b v_b^T),
//     d2W/dR_bdR_c = delta_bc g_b T_b + H_bc u_b u_c^T,
// and (1) then gives the totals
//     dW/dR_b|tot      = dW/dR_b + delta_ba dW/dx,                       (3)
//     d2W/dR_bdR_c|tot = d2W/dR_bdR_c + delta_ca d2W/dxdR_b
//                      + delta_ba d2W/dxdR_c^T + delta_ba delta_ca d2W/dx2.(4)
// A row or column of (3)-(4) is exactly zero unless its atom is the owner or a
// member of some live cell's A_c, so a dense array carrying the contract's
// footprint is filled sparsely and the cost per point stays O(N + |A|^2).
//
// All displacements and radii are in Bohr; a derivative is per Bohr and a
// second derivative per Bohr^2.

#include "excgrid/geometry.hpp"
#include "excgrid/grid.hpp"

#include <array>
#include <cstddef>
#include <vector>

namespace excgrid::derivs {

/// The SSF switch at one argument, with its first two derivatives.
///
/// S(mu) sits on a plateau outside |mu| <= a = 0.64 and is the odd degree-7
/// polynomial above inside it.  Its derivatives vanish at the joins - z'(t) =
/// 35 (1 - t^2)^3 / 16 and z''(t) = -105 t (1 - t^2)^2 / 8 are both zero at
/// t = +/-1 - so the switch, and every product of switches in (2), is C^2 in
/// the geometry.
struct SwitchValue {
    double value = 0.0; ///< S(mu).
    double first = 0.0; ///< dS/dmu.
    double second = 0.0; ///< d2S/dmu2.
};

/// Evaluates the switch and its derivatives.
/// \param mu The switch argument, (r_c - r_d) / (rad_c + rad_d).
/// \returns The value and the two derivatives.
[[nodiscard]] SwitchValue SsfSwitch(double mu) noexcept;

/// The Bragg-Slater covalent radius of an element, in Bohr.
///
/// The derivative pass must partition with exactly the radii the build
/// partitions with, so both read the one table in src/bragg_slater.hpp.
/// \param atomicNumber The element's Z.
/// \returns The radius in Bohr, or the 1.0 A fallback for an untabulated
/// element.
[[nodiscard]] double BraggSlaterRadius(int atomicNumber) noexcept;

/// How many derivative orders an evaluation produces.
enum class DerivativeOrder {
    kFirst = 1, ///< First derivatives only: 3N doubles per point.
    kSecond = 2, ///< First and second: 3N + (3N)^2 doubles per point.
};

/// The work arrays of the derivative pass.
///
/// The pass runs once per grid point (10^5-10^6 of them), so its buffers are
/// the caller's rather than the callee's: one scratch, sized once for a
/// geometry, serves every point.  Its footprint is the working-set figure
/// DoubleCount() reports, and nothing else does.
///
/// A point writes only the rows and columns of its active atoms, and `active`
/// is the list of those, so a point whose active set has moved can clear
/// exactly what the previous one left.  The clearing itself is the output
/// buffer's business and is recorded there: one scratch may serve a run of
/// points into any number of buffers, and a buffer whose contents a stale list
/// did not describe could not be cleared correctly.
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

    /// 3N: v_c = sum_d H_cd u_d per active atom, taken out of the accumulator
    /// blocks before the assembly overwrites any of them.  Both the row's
    /// curvature and the column's in (4) are read from here, so the assembly
    /// never has to ask an already-written block for a value.
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
    /// \returns The sum of every work array above, member by member.
    [[nodiscard]] std::size_t DoubleCount() const noexcept;

    /// The scratch's footprint per point, in doubles, excluding the O(N^2)
    /// output the caller owns.
    /// \returns The number of doubles one point's evaluation touches.
    [[nodiscard]] static std::size_t PerPointDoubles(std::size_t atomCount) noexcept;
};

/// Builds the scratch for a geometry.
/// \param geometry The molecule the derivative pass will walk.
/// \param order The derivative order the caller will ask for.
/// \returns The scratch, with the radii resolved once.
[[nodiscard]] DerivativeScratch MakeDerivativeScratch(const Geometry& geometry,
                                                      DerivativeOrder order);

/// One point's weight and its derivatives with respect to the nuclear
/// coordinates.
///
/// The coordinate order is the library's atom-major one: the derivative with
/// respect to component k of atom b sits at index 3b + k, and the second
/// derivative with respect to (b, k) and (c, l) at index
/// (3b + k) * (3N) + (3c + l) of a row-major (3N) x (3N) matrix.
struct PointWeightDerivatives {
    /// The differentiated quantity: the partition factor W_a, or the point's
    /// grid weight q * W_a, as the evaluating routine's own comment states.
    double value = 0.0;

    /// The number of atoms with a non-zero row: the owner plus the members of
    /// every live cell's A_c.  It is O(1) in practice and is what the assembly
    /// actually touches, against a footprint of 3N.
    std::size_t activeAtoms = 0;

    /// 3N entries: d(value)/dR.  Every entry outside an active row is exactly
    /// zero.
    std::vector<double> first;

    /// (3N)^2 entries, row-major, when the scratch's order is kSecond: the
    /// second derivative d2(value)/dR dR.  Empty at kFirst.  Every entry
    /// outside an active row and column is exactly zero, and the two halves are
    /// mirrored rather than recomputed, so the matrix is symmetric bit for bit.
    std::vector<double> second;

    /// The active atoms the previous point left blocks at in \c second: this
    /// point clears exactly those before writing its own, which is why no point
    /// pays for clearing a matrix the contract sizes at (3N)^2.  The record
    /// lives with the buffer it describes rather than in the scratch, so a
    /// caller that keeps two buffers cannot have one of them cleared from the
    /// other's list.
    std::vector<std::size_t> dirty;
};

/// The quadrature half of a grid: the radial and angular nodes, which do not
/// depend on the geometry and are therefore built once for a whole pass.
struct GridBasis {
    RadialGrid radial; ///< The radial quadrature, innermost point first.
    AngularGrid angular; ///< The angular quadrature on the unit sphere.
};

/// Builds the quadrature half of the grid for a set of build parameters.
/// \param params The build parameters.
/// \returns The basis, or the underlying quadratures' error.
[[nodiscard]] Result<GridBasis> MakeGridBasis(const GridParams& params);

/// One point of the un-trimmed product grid.
///
/// The build enumerates its points atom by atom, radial index within the atom
/// and angular index within the shell, so a triple is a stable name for a point
/// across geometries - which is what lets a displaced build be compared with
/// the point it came from.
struct PointId {
    std::size_t atom = 0; ///< The owning atom.
    std::size_t radial = 0; ///< Its radial index, in [0, RadialGrid::Size()).
    std::size_t angular = 0; ///< Its angular index, in [0, AngularGrid::Size()).
};

/// The point a build would generate for an identity, in Bohr.
/// \param geometry The molecule.
/// \param basis The quadrature half of the grid.
/// \param id The point's identity.
/// \returns The position, R_atom + radial point * angular node.
[[nodiscard]] std::array<double, 3> PointPosition(const Geometry& geometry,
                                                  const GridBasis& basis,
                                                  const PointId& id);

/// The quadrature factor of a point: its radial weight times rho^2 times its
/// angular weight, all of it independent of the geometry.
/// \param basis The quadrature half of the grid.
/// \param id The point's identity.
/// \returns The factor q, so that the point's grid weight is q * W_owner.
[[nodiscard]] double PointQuadrature(const GridBasis& basis, const PointId& id);

/// The coordinate derivatives of one grid point.
///
/// A point is its owner's own radial point times its own angular node, so it is
/// a rigid translate of that atom and (1) applies exactly: the derivative is
/// the owner's 3 x 3 identity block inside a 3 x 3N frame and the second
/// derivative is identically zero.  Nothing here is approximated, which is why
/// a grid-motion term can be assembled from the weight derivatives alone.
struct PointCoordinateDerivatives {
    /// 3 x 3N entries: dx_k/dR_{(b,l)} at index k * 3N + 3b + l.
    std::vector<double> first;

    /// (3N)^2 entries, all zero, when the order is kSecond.  Empty at kFirst.
    std::vector<double> second;
};

/// Fills the coordinate derivatives of a point.
/// \param owner The owning atom.
/// \param atomCount N.
/// \param order Which orders to fill; kSecond fills \c second with zeros.
/// \param out Receives the derivatives, resized to the order's footprint.
void EvaluatePointCoordinates(std::size_t owner,
                              std::size_t atomCount,
                              DerivativeOrder order,
                              PointCoordinateDerivatives& out);

/// Evaluates one point of the product grid: its grid weight, with the
/// quadrature factor folded in, and the derivatives of that weight with respect
/// to the 3N nuclear coordinates.
///
/// The point is the one the build generates for \p id, so it moves with
/// \p id.atom and the derivatives are the totals (3) and (4).  This is the
/// quantity a gradient or Hessian assembly consumes: the point's weight is
/// q * W, and every derivative of the constant q comes along with it.
/// \param geometry The molecule.
/// \param basis The quadrature half of the grid.
/// \param id The point's identity.
/// \param scratch The work arrays, sized for \p geometry.
/// \param out Receives the weight and its derivatives.
void EvaluatePointWeight(const Geometry& geometry,
                         const GridBasis& basis,
                         const PointId& id,
                         DerivativeScratch& scratch,
                         PointWeightDerivatives& out);

/// Evaluates the partition weight of one atom's cell at a point, and its first
/// two derivatives with respect to the 3N nuclear coordinates.
///
/// The point is taken to move with \p owner - it is the point that atom's own
/// sub-grid generates - so the derivatives returned are the totals (3) and (4),
/// not the derivatives at a fixed point.  The returned value is the normalized
/// partition weight W_owner, without the quadrature factor q.  At
/// DerivativeOrder::kSecond the second derivatives are assembled sparsely:
/// only the active atoms' rows and columns are written, and the blocks the
/// previous point left behind are cleared, so the buffer is a dense (3N)^2
/// matrix whose every entry is current.  Which blocks those were is recorded
/// in \c out.dirty, so the next point into the same buffer clears exactly them.
/// \param geometry The molecule.
/// \param point The point, in Bohr.
/// \param owner The atom whose cell's weight is wanted.
/// \param scratch The work arrays, sized for \p geometry.
/// \param out Receives the weight and, in \c first and \c second, its 3N and
/// (3N)^2 derivatives.  The buffers are sized once and reused across a run of
/// points; each point leaves exactly its own active blocks in \c second, and
/// \c out.dirty records which those were.
void EvaluatePartitionWeight(const Geometry& geometry,
                             const std::array<double, 3>& point,
                             std::size_t owner,
                             DerivativeScratch& scratch,
                             PointWeightDerivatives& out);

} // namespace excgrid::derivs
