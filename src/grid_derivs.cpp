// The grid's derivatives with respect to nuclear position.  The mathematics,
// the naming of every intermediate and the reason the pass is structured this
// way are in grid_derivs.hpp; this file is the implementation.

#include "grid_derivs.hpp"

#include "bragg_slater.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <vector>

namespace excgrid::derivs {

namespace {

// The switch window: |mu| <= a is the only region where the SSF step is off its
// plateau.  Mirrors the build's own kSwitchWindow (src/block_grid.cpp).
constexpr double kSwitchWindow = 0.64;

// Two of the nine entries of a second-derivative block are the accumulator
// slots the walk over the cells fills: the block's (0, 0) holds the sum of the
// raw cells' second derivatives over every live cell, and its (1, 1) holds the
// same sum over the owner's own cell alone.  Both are overwritten when the
// block is assembled into the nuclear-coordinate Hessian.  They are named as
// entries of the block's own 3 x 3 frame, not as raw offsets into the buffer:
// the buffer IS the (3N)^2 matrix, whose blocks are `width` entries apart, so a
// raw offset of four would land inside a neighbouring block's row whenever the
// geometry has more than one atom.
constexpr std::size_t kSecondSumRow = 0;
constexpr std::size_t kSecondSumColumn = 0;
constexpr std::size_t kSecondOwnRow = 1;
constexpr std::size_t kSecondOwnColumn = 1;

// The two smallest of a cell's upper bounds, and the atom carrying the
// smallest.  A cell's switch sits on the zero plateau exactly when its lower
// bound reaches the smallest upper bound over the OTHER atoms, and the two
// smallest answer that for every cell at once, in O(1) per cell.  The second
// smallest of a one-atom geometry is infinite, which is the whole of why a lone
// atom's cell is always live.
struct CellBounds {
    double smallest = 0.0; ///< The smallest upper bound.
    double secondSmallest = 0.0; ///< The next one up, infinite for one atom.
    std::size_t carrier = 0; ///< The atom the smallest belongs to.
};

CellBounds TwoSmallest(const std::vector<double>& hi) noexcept {
    CellBounds out;
    out.smallest = std::numeric_limits<double>::infinity();
    out.secondSmallest = std::numeric_limits<double>::infinity();

    for (std::size_t a = 0; a < hi.size(); ++a)
    {
        if (hi[a] < out.smallest)
        {
            out.secondSmallest = out.smallest;
            out.smallest = hi[a];
            out.carrier = a;
        } else if (hi[a] < out.secondSmallest)
        {
            out.secondSmallest = hi[a];
        }
    }

    return out;
}

} // namespace

SwitchValue SsfSwitch(double mu) noexcept {
    if (mu >= kSwitchWindow)
    {
        return {0.0, 0.0, 0.0};
    }

    if (mu <= -kSwitchWindow)
    {
        return {1.0, 0.0, 0.0};
    }

    // z(t) = t (35 - t^2 (35 - t^2 (21 - 5 t^2))) / 16, and its derivatives
    // collapsed to z'(t) = 35 (1 - t^2)^3 / 16 and
    // z''(t) = -105 t (1 - t^2)^2 / 8.  Both vanish at |t| = 1, so the switch
    // and its first two derivatives are continuous at the joins.
    const double t = mu / kSwitchWindow;
    const double t2 = t * t;
    const double z = t * (35.0 - t2 * (35.0 - t2 * (21.0 - 5.0 * t2))) / 16.0;
    const double window = 1.0 - t2;
    const double dz = 35.0 * window * window * window / 16.0;
    const double ddz = -105.0 * t * window * window / 8.0;
    const double inverseWindow = 1.0 / kSwitchWindow;
    SwitchValue out;
    out.value = 0.5 * (1.0 - z);
    out.first = -0.5 * dz * inverseWindow;
    out.second = -0.5 * ddz * inverseWindow * inverseWindow;

    return out;
}

double BraggSlaterRadius(int atomicNumber) noexcept {
    return internal::BraggSlaterRadius(atomicNumber);
}

std::size_t DerivativeScratch::DoubleCount() const noexcept {
    // capacity(), not size(): the question this answers is how much memory the
    // pass holds, and `active` and `member` are the arrays whose size moves per
    // point.
    return radius.capacity() + distance.capacity() + lo.capacity() + hi.capacity() + u.capacity() +
           sumFirst.capacity() + ownFirst.capacity() + gradient.capacity() + chain.capacity() +
           member.capacity() + active.capacity() + mark.capacity() + switchValue.capacity() +
           switchFirst.capacity() + switchSecond.capacity() + prefix.capacity() + suffix.capacity();
}

std::size_t DerivativeScratch::PerPointDoubles(std::size_t atomCount) noexcept {
    // Thirteen N-sized slots (radius, distance, lo, hi, sumFirst, ownFirst,
    // gradient, member, active, mark, switchValue, switchFirst and
    // switchSecond), the 3N unit vector and the 3N chain, the two N + 1 running
    // products, and the reserved capacity of the per-cell lists.  The
    // footprint test asserts that DoubleCount() equals this expression for a
    // scratch built for the same atom count, so the formula and the arrays
    // cannot drift apart.  The output buffer's own `dirty` list is part of the
    // output, not of the pass's work arrays, and the (3N)^2 matrix beside it is
    // the caller's.
    return 21 * atomCount + 2;
}

DerivativeScratch MakeDerivativeScratch(const Geometry& geometry, DerivativeOrder order) {
    DerivativeScratch scratch;
    const std::size_t atomCount = geometry.atoms.size();
    scratch.atomCount = atomCount;
    scratch.order = order;
    scratch.radius.reserve(atomCount);

    for (const Atom& atom : geometry.atoms)
    {
        scratch.radius.push_back(BraggSlaterRadius(atom.atomicNumber));
    }

    scratch.distance.assign(atomCount, 0.0);
    scratch.lo.assign(atomCount, 0.0);
    scratch.hi.assign(atomCount, 0.0);
    scratch.u.assign(3 * atomCount, 0.0);
    scratch.sumFirst.assign(atomCount, 0.0);
    scratch.ownFirst.assign(atomCount, 0.0);
    scratch.gradient.assign(atomCount, 0.0);
    scratch.chain.assign(3 * atomCount, 0.0);
    scratch.member.assign(atomCount, 0);
    scratch.mark.assign(atomCount, 0u);
    scratch.switchValue.assign(atomCount, 0.0);
    scratch.switchFirst.assign(atomCount, 0.0);
    scratch.switchSecond.assign(atomCount, 0.0);
    scratch.prefix.assign(atomCount + 1, 0.0);
    scratch.suffix.assign(atomCount + 1, 0.0);
    scratch.active.reserve(atomCount);

    return scratch;
}

Result<GridBasis> MakeGridBasis(const GridParams& params) {
    auto radial = RadialGrid::Create(params.radialPoints, params.alpha, params.radialExponent);

    if (!radial)
    {
        return std::unexpected(radial.error());
    }

    auto angular = AngularGrid::Create(params.angularPoints);

    if (!angular)
    {
        return std::unexpected(angular.error());
    }

    return GridBasis{std::move(*radial), std::move(*angular)};
}

std::array<double, 3> PointPosition(const Geometry& geometry,
                                    const GridBasis& basis,
                                    const PointId& id) {
    const std::array<double, 3>& center = geometry.atoms[id.atom].position;
    const double radius = basis.radial.Points()[id.radial];
    const std::array<double, 3> node = basis.angular.Point(id.angular);

    return {center[0] + radius * node[0],
            center[1] + radius * node[1],
            center[2] + radius * node[2]};
}

double PointQuadrature(const GridBasis& basis, const PointId& id) {
    const double radius = basis.radial.Points()[id.radial];

    return basis.radial.Weights()[id.radial] * radius * radius * basis.angular.Weight(id.angular);
}

void EvaluatePointCoordinates(std::size_t owner,
                              std::size_t atomCount,
                              DerivativeOrder order,
                              PointCoordinateDerivatives& out) {
    const std::size_t width = 3 * atomCount;
    out.first.assign(3 * width, 0.0);

    // The owner's own block is the identity; every other block is zero.
    for (std::size_t k = 0; k < 3; ++k)
    {
        out.first[k * width + 3 * owner + k] = 1.0;
    }

    if (order == DerivativeOrder::kSecond)
    {
        out.second.assign(width * width, 0.0);
    } else
    {
        out.second.clear();
    }
}

void EvaluatePointWeight(const Geometry& geometry,
                         const GridBasis& basis,
                         const PointId& id,
                         DerivativeScratch& scratch,
                         PointWeightDerivatives& out) {
    EvaluatePartitionWeight(geometry, PointPosition(geometry, basis, id), id.atom, scratch, out);

    // The quadrature factor is constant in the geometry, so it scales the value
    // and every derivative alike.  The partition leaves its second derivatives
    // sparse - only the active atoms' blocks are anything but zero - and the
    // buffer's own `dirty` is exactly which those were, so the scaling is as
    // sparse as the assembly was and never a (3N)^2 sweep per point.
    const double quadrature = PointQuadrature(basis, id);
    out.value *= quadrature;

    for (double& entry : out.first)
    {
        entry *= quadrature;
    }

    if (scratch.order != DerivativeOrder::kSecond)
    {
        return;
    }

    const std::size_t width = 3 * geometry.atoms.size();

    for (const std::size_t b : out.dirty)
    {
        for (const std::size_t c : out.dirty)
        {
            for (std::size_t l = 0; l < 3; ++l)
            {
                for (std::size_t m = 0; m < 3; ++m)
                {
                    out.second[(3 * b + l) * width + 3 * c + m] *= quadrature;
                }
            }
        }
    }
}

void EvaluatePartitionWeight(const Geometry& geometry,
                             const std::array<double, 3>& point,
                             std::size_t owner,
                             DerivativeScratch& scratch,
                             PointWeightDerivatives& out) {
    const std::size_t atomCount = geometry.atoms.size();

    if (scratch.atomCount != atomCount)
    {
        scratch = MakeDerivativeScratch(geometry, scratch.order);
    }

    std::vector<double>& distance = scratch.distance;
    std::vector<double>& lo = scratch.lo;
    std::vector<double>& hi = scratch.hi;
    std::vector<double>& u = scratch.u;
    const std::vector<double>& radius = scratch.radius;
    const std::size_t width = 3 * atomCount;
    const bool secondOrder = scratch.order == DerivativeOrder::kSecond;

    if (secondOrder)
    {
        // The (3N)^2 output is dense in the contract and sparse in the work:
        // only the active atoms have a non-zero block, so clearing the blocks
        // the previous point left set is the whole of what a point pays, and
        // never a (3N)^2 sweep.  A buffer this pass has not seen before is the
        // one exception, and it is cleared once.  Which blocks the buffer holds
        // is recorded beside them, so a buffer whose footprint is right but
        // whose contents came from somewhere else cannot be mis-cleared.
        if (out.second.size() != width * width)
        {
            out.second.assign(width * width, 0.0);
            out.dirty.clear();
        } else
        {
            for (const std::size_t b : out.dirty)
            {
                for (const std::size_t c : out.dirty)
                {
                    for (std::size_t l = 0; l < 3; ++l)
                    {
                        for (std::size_t m = 0; m < 3; ++m)
                        {
                            out.second[(3 * b + l) * width + 3 * c + m] = 0.0;
                        }
                    }
                }
            }
        }
    } else
    {
        out.second.clear();
    }

    // One pass over the atoms: the distances, the unit vectors the chain rule
    // needs, and the two smallest upper bounds.  Every cell test below asks for
    // the smallest hi over the OTHER atoms, which the two smallest values
    // answer in O(1) for every cell at once.
    for (std::size_t a = 0; a < atomCount; ++a)
    {
        const std::array<double, 3>& center = geometry.atoms[a].position;
        const double dx = point[0] - center[0];
        const double dy = point[1] - center[1];
        const double dz = point[2] - center[2];
        const double r = std::sqrt(dx * dx + dy * dy + dz * dz);
        const double inverse = r > 0.0 ? 1.0 / r : 0.0;
        distance[a] = r;
        u[3 * a] = dx * inverse;
        u[3 * a + 1] = dy * inverse;
        u[3 * a + 2] = dz * inverse;
        const double window = kSwitchWindow * radius[a];
        lo[a] = r - window;
        hi[a] = r + window;
    }

    const CellBounds bounds = TwoSmallest(hi);

    // The live cells: every cell whose switch is not on the zero plateau.  A
    // cell that is not live has raw_c = 0 and every derivative zero, because
    // the vanishing factor's derivative vanishes with it at the plateau edge.
    // `active` collects the atoms that end up with a non-zero row; the mark
    // array is stamped by a generation counter, so the collection clears
    // without an O(N) sweep per point.
    if (++scratch.generation == 0u)
    {
        std::fill(scratch.mark.begin(), scratch.mark.end(), 0u);
        scratch.generation = 1u;
    }

    std::vector<std::size_t>& active = scratch.active;
    std::vector<unsigned>& mark = scratch.mark;
    std::vector<std::size_t>& member = scratch.member;
    std::vector<double>& sumFirst = scratch.sumFirst;
    std::vector<double>& ownFirst = scratch.ownFirst;
    std::vector<double>& switchValue = scratch.switchValue;
    std::vector<double>& switchFirst = scratch.switchFirst;
    std::vector<double>& switchSecond = scratch.switchSecond;
    std::vector<double>& prefix = scratch.prefix;
    std::vector<double>& suffix = scratch.suffix;

    active.clear();
    std::fill(sumFirst.begin(), sumFirst.end(), 0.0);
    std::fill(ownFirst.begin(), ownFirst.end(), 0.0);

    const auto addActive = [&](std::size_t a) {
        if (mark[a] != scratch.generation)
        {
            mark[a] = scratch.generation;
            active.push_back(a);
        }
    };

    // The second-order accumulator: while the cells are walked, the (x, y)
    // block of out.second carries two scalars in two of its nine entries - the
    // sum of the raw cells' second derivatives d2(raw_c)/dr_x dr_y over every
    // live cell, and the same sum over the owner's own cell alone.  The
    // quotient rule weights the two differently, and holding both here is what
    // keeps the walk over the cells a single one: the assembly below reads the
    // two slots and overwrites the whole block with the nuclear-coordinate
    // Hessian.  Both slots are entries of the (x, y) block itself, so a write to
    // any other block leaves them alone and the block's own assembly reads them
    // before it overwrites them.
    const auto slotAt = [&](std::size_t x,
                            std::size_t y,
                            std::size_t row,
                            std::size_t column) -> double& {
        return out.second[(3 * x + row) * width + 3 * y + column];
    };

    const auto addSecond = [&](std::size_t x, std::size_t y, double value, bool ownCell) {
        slotAt(x, y, kSecondSumRow, kSecondSumColumn) += value;

        if (ownCell)
        {
            slotAt(x, y, kSecondOwnRow, kSecondOwnColumn) += value;
        }

        if (x != y)
        {
            slotAt(y, x, kSecondSumRow, kSecondSumColumn) += value;

            if (ownCell)
            {
                slotAt(y, x, kSecondOwnRow, kSecondOwnColumn) += value;
            }
        }
    };

    double sumValue = 0.0;
    double ownValue = 0.0;

    for (std::size_t c = 0; c < atomCount; ++c)
    {
        const double otherSmallestHi = (c == bounds.carrier) ? bounds.secondSmallest : bounds.smallest;

        if (lo[c] >= otherSmallestHi)
        {
            continue; // raw_c = 0, with every derivative
        }

        std::size_t memberCount = 0;

        for (std::size_t d = 0; d < atomCount; ++d)
        {
            if (d == c || lo[d] >= hi[c])
            {
                continue; // the factor is exactly one, so it is left out
            }

            member[memberCount] = d;

            const double pairRadius = radius[c] + radius[d];
            const SwitchValue step = SsfSwitch((distance[c] - distance[d]) / pairRadius);
            switchValue[memberCount] = step.value;
            switchFirst[memberCount] = step.first / pairRadius;
            switchSecond[memberCount] = step.second / (pairRadius * pairRadius);
            ++memberCount;
        }

        // The running products, so a member's cofactor costs one multiply:
        // prefix[i] * suffix[i + 1] is the product over every other member.
        prefix[0] = 1.0;

        for (std::size_t i = 0; i < memberCount; ++i)
        {
            prefix[i + 1] = prefix[i] * switchValue[i];
        }

        suffix[memberCount] = 1.0;

        for (std::size_t i = memberCount; i > 0; --i)
        {
            suffix[i - 1] = suffix[i] * switchValue[i - 1];
        }

        const double cellValue = prefix[memberCount];
        double cellFirst = 0.0;

        // d(raw_c)/dr_c = sum_i cofactor_i * S'(mu) / pairRadius, and the
        // derivative with respect to a member's distance is the negative of
        // that member's own term - mu is linear in the distances.
        for (std::size_t i = 0; i < memberCount; ++i)
        {
            const double term = prefix[i] * suffix[i + 1] * switchFirst[i];
            cellFirst += term;
            sumFirst[member[i]] -= term;
        }

        sumValue += cellValue;
        sumFirst[c] += cellFirst;
        addActive(c);

        for (std::size_t i = 0; i < memberCount; ++i)
        {
            addActive(member[i]);
        }

        if (c == owner)
        {
            ownValue = cellValue;
            ownFirst[c] = cellFirst;

            for (std::size_t i = 0; i < memberCount; ++i)
            {
                ownFirst[member[i]] -= prefix[i] * suffix[i + 1] * switchFirst[i];
            }
        }

        if (secondOrder)
        {
            // The cell's own block of the distance Hessian, in the two
            // accumulator slots.  With s_i = S(mu_i), a_i = ds_i/dr_c and
            // b_i = d2s_i/dr_c2, the cell's raw product obeys
            //     d2(raw_c)/dr_c2       = sum_i [ cof_i b_i + sum_{j != i} cof_ij a_i a_j ]
            //     d2(raw_c)/dr_c dr_m_i = -cof_i b_i - sum_{j != i} cof_ij a_i a_j
            //     d2(raw_c)/dr_m_i2     =  cof_i b_i
            //     d2(raw_c)/dr_m_i dr_m_j = cof_ij a_i a_j,
            // where cof_i = prod_{j != i} s_j and cof_ij = prod_{l != i, j} s_l.
            const bool ownCell = c == owner;

            for (std::size_t i = 0; i < memberCount; ++i)
            {
                const double single = prefix[i] * suffix[i + 1] * switchSecond[i];
                addSecond(c, c, single, ownCell);
                addSecond(member[i], member[i], single, ownCell);
                addSecond(c, member[i], -single, ownCell);
            }

            for (std::size_t i = 0; i < memberCount; ++i)
            {
                double middle = prefix[i];

                for (std::size_t j = i + 1; j < memberCount; ++j)
                {
                    // middle is the product over the members strictly between
                    // i and j, so middle * suffix[j + 1] is cof_ij.
                    const double pair = middle * suffix[j + 1] * switchFirst[i] * switchFirst[j];
                    addSecond(c, c, 2.0 * pair, ownCell);
                    addSecond(member[i], member[j], pair, ownCell);
                    addSecond(c, member[i], -pair, ownCell);
                    addSecond(c, member[j], -pair, ownCell);
                    middle *= switchValue[j];
                }
            }
        }
    }

    // The quotient rule, then the chain rule that takes the distance
    // derivatives to nuclear-coordinate ones (3).  Every atom outside `active`
    // has g_c = 0, so its row is exactly zero and the assembly leaves it alone.
    const double weight = sumValue > 0.0 ? ownValue / sumValue : ownValue;
    out.value = weight;
    out.activeAtoms = active.size();
    out.first.assign(3 * atomCount, 0.0);

    // Every live cell's product is strictly positive - each of its members sits
    // strictly inside the switch window - so a vanishing sum means there was no
    // live cell at all: an empty active set, a weight of zero, and every
    // derivative of that weight zero.  The loops below are then empty, and the
    // output stays as the clear above left it.
    std::vector<double>& gradient = scratch.gradient;
    std::array<double, 3> pointGradient = {0.0, 0.0, 0.0};

    for (const std::size_t e : active)
    {
        gradient[e] = (ownFirst[e] - weight * sumFirst[e]) / sumValue;

        for (std::size_t k = 0; k < 3; ++k)
        {
            pointGradient[k] += gradient[e] * u[3 * e + k];
        }
    }

    for (const std::size_t b : active)
    {
        for (std::size_t k = 0; k < 3; ++k)
        {
            out.first[3 * b + k] =
                -gradient[b] * u[3 * b + k] + (b == owner ? pointGradient[k] : 0.0);
        }
    }

    if (!secondOrder)
    {
        return;
    }

    if (!active.empty())
    {
        const double inverseSquare = 1.0 / (sumValue * sumValue);

        // The distance Hessian at one pair, out of the two accumulator slots:
        //     H_xy = [ N (D_xy - W N_xy) - D_x N_y - D_y N_x + 2 W N_x N_y ] / N^2,
        // with D_xy the owner's own cell's second derivative and N_xy the sum
        // over every live cell.
        const auto pairHessian = [&](std::size_t x, std::size_t y) {
            const double sumSecond = slotAt(x, y, kSecondSumRow, kSecondSumColumn);
            const double ownSecond = slotAt(x, y, kSecondOwnRow, kSecondOwnColumn);
            const double numerator = sumValue * (ownSecond - weight * sumSecond) -
                                     ownFirst[x] * sumFirst[y] - ownFirst[y] * sumFirst[x] +
                                     2.0 * weight * sumFirst[x] * sumFirst[y];

            return numerator * inverseSquare;
        };

        // v_c = sum_d H_cd u_d for every active atom, read out of the accumulator
        // blocks while they are all still the accumulator's.  The assembly below
        // then reads the chains and never a slot: both the row's curvature and
        // the column's come from here, and a block an earlier row has already
        // overwritten could no longer be asked for either.
        std::vector<double>& chain = scratch.chain;

        // P_c[p,q] = g_c T_c[p,q] + u_c[p] v_c[q] with T_c = (I - u_c u_c^T)/r_c:
        // the curvature of the weight in the point's own coordinates at atom c.
        // (4) mixes it into the nuclear blocks three ways - the (b, owner) block
        // takes the ROW's, the (owner, c) block takes the COLUMN's, and the
        // (owner, owner) block takes the sum over every active atom, which is the
        // point-space Hessian d2W/dx2.  Only an active atom has a gradient, so an
        // owner outside `active` contributes nothing to any of the three.
        const auto curvatureAt = [&](std::size_t c, std::size_t p, std::size_t q) {
            const double t = ((p == q ? 1.0 : 0.0) - u[3 * c + p] * u[3 * c + q]) / distance[c];

            return gradient[c] * t + u[3 * c + p] * chain[3 * c + q];
        };

        for (const std::size_t c : active)
        {
            for (std::size_t k = 0; k < 3; ++k)
            {
                chain[3 * c + k] = 0.0;
            }

            for (const std::size_t d : active)
            {
                const double h = pairHessian(c, d);

                for (std::size_t k = 0; k < 3; ++k)
                {
                    chain[3 * c + k] += h * u[3 * d + k];
                }
            }
        }

        std::array<double, 9> curvatureSum = {};

        for (const std::size_t c : active)
        {
            for (std::size_t p = 0; p < 3; ++p)
            {
                for (std::size_t q = 0; q < 3; ++q)
                {
                    curvatureSum[3 * p + q] += curvatureAt(c, p, q);
                }
            }
        }

        // The blocks themselves, upper triangle first: H_bc is read before the
        // block that holds it is overwritten, and a lower-triangle block is never
        // written here, so every H is still the accumulator's while the rows are
        // walked - and the chains above have already taken everything else out of
        // the slots.
        for (std::size_t bi = 0; bi < active.size(); ++bi)
        {
            const std::size_t b = active[bi];

            for (std::size_t ci = bi; ci < active.size(); ++ci)
            {
                const std::size_t c = active[ci];
                const double distanceHessian = pairHessian(b, c);

                for (std::size_t l = 0; l < 3; ++l)
                {
                    for (std::size_t m = 0; m < 3; ++m)
                    {
                        double entry = distanceHessian * u[3 * b + l] * u[3 * c + m];

                        if (b == c)
                        {
                            entry += gradient[b] *
                                     ((l == m ? 1.0 : 0.0) - u[3 * b + l] * u[3 * b + m]) /
                                     distance[b];
                        }

                        if (c == owner)
                        {
                            entry -= curvatureAt(b, l, m);
                        }

                        if (b == owner)
                        {
                            entry -= curvatureAt(c, m, l);
                        }

                        if (b == c && b == owner)
                        {
                            entry += curvatureSum[3 * l + m];
                        }

                        out.second[(3 * b + l) * width + 3 * c + m] = entry;
                    }
                }
            }
        }

        // The lower triangle is the upper one transposed, and inside a diagonal
        // block the lower half is the upper half's.  Mirroring rather than
        // recomputing is what makes the Hessian symmetric bit for bit: an entry
        // is a sum of terms that is symmetric in (l, m) only as a whole - the
        // row's curvature and the column's are different pieces - so the two
        // halves of a diagonal block disagree in the last bit when each is
        // assembled on its own.
        for (std::size_t bi = 0; bi < active.size(); ++bi)
        {
            const std::size_t b = active[bi];

            for (std::size_t l = 0; l < 3; ++l)
            {
                for (std::size_t m = l + 1; m < 3; ++m)
                {
                    out.second[(3 * b + m) * width + 3 * b + l] =
                        out.second[(3 * b + l) * width + 3 * b + m];
                }
            }

            for (std::size_t ci = bi + 1; ci < active.size(); ++ci)
            {
                const std::size_t c = active[ci];

                for (std::size_t l = 0; l < 3; ++l)
                {
                    for (std::size_t m = 0; m < 3; ++m)
                    {
                        out.second[(3 * c + l) * width + 3 * b + m] =
                            out.second[(3 * b + m) * width + 3 * c + l];
                    }
                }
            }
        }
    }

    out.dirty.assign(active.begin(), active.end());
}

} // namespace excgrid::derivs
