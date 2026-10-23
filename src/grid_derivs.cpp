// The grid's derivatives with respect to nuclear position.

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

// The SSF switch window a; must match the build's own kSwitchWindow in block_grid.cpp.
constexpr double kSwitchWindow = 0.64;

// The block-frame accumulator slots the cell walk fills: (0, 0) sums every live
// cell, (1, 1) the owner's own; block entries, never raw buffer offsets.
constexpr std::size_t kSecondSumRow = 0;
constexpr std::size_t kSecondSumColumn = 0;
constexpr std::size_t kSecondOwnRow = 1;
constexpr std::size_t kSecondOwnColumn = 1;

// The two smallest upper bounds: the smallest over the other atoms answers every
// cell at once, and a lone atom's cell is always live.
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

    // z(t) = t (35 - t^2 (35 - t^2 (21 - 5 t^2))) / 16, z'(t) = 35 (1 - t^2)^3 / 16,
    // z''(t) = -105 t (1 - t^2)^2 / 8; both vanish at |t| = 1, so the joins are C^2.
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
    // capacity(), not size(): this answers how much memory the pass holds.
    return radius.capacity() + distance.capacity() + lo.capacity() + hi.capacity() + u.capacity() +
           sumFirst.capacity() + ownFirst.capacity() + gradient.capacity() + chain.capacity() +
           member.capacity() + active.capacity() + mark.capacity() + switchValue.capacity() +
           switchFirst.capacity() + switchSecond.capacity() + prefix.capacity() + suffix.capacity();
}

std::size_t DerivativeScratch::PerPointDoubles(std::size_t atomCount) noexcept {
    // Thirteen N-sized slots, the 3N unit vector and 3N chain, two N + 1 products: 21 N + 2.
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

    // q is constant in the geometry: it scales the value and every derivative alike.
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
        // Clear only the blocks `dirty` records; a fresh buffer is cleared once here.
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

    // Distances, unit vectors and the two smallest upper bounds, in one pass.
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

    // The live cells; `active` collects the atoms that end up with a non-zero row.
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

    // Two of the (x, y) block's nine entries accumulate while the cells are
    // walked; the assembly below reads both and overwrites the whole block.
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

        // Running products: prefix[i] * suffix[i + 1] is the product over every other member.
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

        // d(raw_c)/dr_c = sum_i cof_i S'(mu_i) / pairRadius; a member's own distance
        // takes the negative of its term, mu being linear in the distances.
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
            // The cell's distance Hessian, with s_i = S(mu_i), a_i = ds_i/dr_c,
            // b_i = d2s_i/dr_c2, cof_i = prod_{j != i} s_j, cof_ij = prod_{l != i,j} s_l:
            //     d2(raw_c)/dr_c2         = sum_i [ cof_i b_i + sum_{j != i} cof_ij a_i a_j ]
            //     d2(raw_c)/dr_c dr_m_i   = -cof_i b_i - sum_{j != i} cof_ij a_i a_j
            //     d2(raw_c)/dr_m_i2       =  cof_i b_i
            //     d2(raw_c)/dr_m_i dr_m_j =  cof_ij a_i a_j.
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
                    // middle * suffix[j + 1] is cof_ij: the product over the members between.
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

    // Quotient rule, then the chain rule to nuclear coordinates; atoms outside `active`
    // have g_c = 0, so their rows are exactly zero.
    const double weight = sumValue > 0.0 ? ownValue / sumValue : ownValue;
    out.value = weight;
    out.activeAtoms = active.size();
    out.first.assign(3 * atomCount, 0.0);

    // A vanishing sum means no live cell: weight zero and every derivative zero.
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

        // The distance Hessian at one pair:
        //     H_xy = [ N (D_xy - W N_xy) - D_x N_y - D_y N_x + 2 W N_x N_y ] / N^2,
        // D_xy the owner's own cell's second derivative, N_xy the sum over every live cell.
        const auto pairHessian = [&](std::size_t x, std::size_t y) {
            const double sumSecond = slotAt(x, y, kSecondSumRow, kSecondSumColumn);
            const double ownSecond = slotAt(x, y, kSecondOwnRow, kSecondOwnColumn);
            const double numerator = sumValue * (ownSecond - weight * sumSecond) -
                                     ownFirst[x] * sumFirst[y] - ownFirst[y] * sumFirst[x] +
                                     2.0 * weight * sumFirst[x] * sumFirst[y];

            return numerator * inverseSquare;
        };

        // v_c = sum_d H_cd u_d, read out of the accumulator blocks before the assembly
        // overwrites any of them.
        std::vector<double>& chain = scratch.chain;

        // P_c[p,q] = g_c T_c[p,q] + u_c[p] v_c[q], T_c = (I - u_c u_c^T)/r_c: the
        // curvature in the point's coordinates; only an active atom contributes.
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

        // Upper triangle first: every H_bc is still the accumulator's when it is read.
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

        // Mirror, never recompute: the two halves must agree bit for bit.
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
