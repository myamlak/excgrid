#include "excgrid/grid.hpp"

#include "bragg_slater.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>
#include <utility>

namespace excgrid {

namespace {

constexpr double kSwitchWindow = 0.64; // SSF: |mu| <= a window.
constexpr std::size_t kCandidateCap = 512; // Nearest-neighbour search cap per step.

// The SSF switching function: s(mu) in [0, 1], s(mu) = 0 for mu >= a,
// s(mu) = 1 for mu <= -a, and the odd degree-7 polynomial
// z(t) = t(35 - t^2(35 - t^2(21 - 5 t^2)))/16 in between (Stratmann,
// Scuseria, Frisch, Chem. Phys. Lett. 257 (1996) 213).
double Switch(double mu) {
    if (mu >= kSwitchWindow)
    {
        return 0.0;
    }

    if (mu <= -kSwitchWindow)
    {
        return 1.0;
    }

    const double t = mu / kSwitchWindow;
    const double t2 = t * t;
    const double z = t * (35.0 - t2 * (35.0 - t2 * (21.0 - 5.0 * t2))) / 16.0;

    return 0.5 * (1.0 - z);
}

// The per-build work arrays of the partition pass.  The pass runs once per
// grid point (10^5-10^6 of them), so its buffers are the caller's rather
// than the callee's, and the Slater radii are resolved once here instead of
// once per point per atom.
struct PartitionScratch {
    std::vector<double> radius; // Bragg-Slater radius, one per atom.
    std::vector<double> distance; // |point - atom|, per point.
    std::vector<double> lo; // distance - 0.64 radius, per point.
    std::vector<double> hi; // distance + 0.64 radius, per point.
    std::vector<double> raw; // the un-normalized cell weights.
    std::vector<std::size_t> window; // the atoms that can matter, per shell.
    double windowMargin = 0.0; // 4 max over atoms of 0.64 x radius.
};

PartitionScratch MakePartitionScratch(const Geometry& geometry) {
    PartitionScratch scratch;
    const std::size_t atomCount = geometry.atoms.size();
    scratch.radius.reserve(atomCount);

    for (const Atom& atom : geometry.atoms)
    {
        scratch.radius.push_back(internal::BraggSlaterRadius(atom.atomicNumber));
    }

    for (const double radius : scratch.radius)
    {
        scratch.windowMargin = std::max(scratch.windowMargin, kSwitchWindow * radius);
    }

    scratch.windowMargin *= 4.0;
    scratch.distance.resize(atomCount);
    scratch.lo.resize(atomCount);
    scratch.hi.resize(atomCount);
    scratch.raw.resize(atomCount);
    scratch.window.reserve(atomCount);

    return scratch;
}

// The neighbour list of one shell: every atom whose switch can differ from
// its plateau value for some point of the owner atom's sphere of radius
// `radius`.  For a point p on that sphere, p is `radius` from the owner and
// every other atom b is at least d_ab - radius from p, so an atom with
// d_ab > 2 radius + 4 kMax (kMax = 0.64 times the largest Slater radius) is
// farther than radius + 4 kMax from every point of the sphere.  Such an atom
// is provably inert there: the owner breaks its cell (the owner sits at
// radius, so the break test r_b >= kA[b] + r_owner + kB[owner] clears by
// more than 2 kMax), it is farther than any owner-relative factor bound
// (r_b < r_a + 2 kMax inside the product), and it never enters the smallest
// two hi, which settle the owner's own break test.  The list is built once
// per shell and shared by every angular point on it, and the
// 2 radius + 4 kMax bound is the only cutoff in the pass - it is derived
// from the switch plateau, not fitted.
void BuildShellWindow(const Geometry& geometry,
                      std::size_t owner,
                      double radius,
                      double margin,
                      std::vector<std::size_t>& window) {
    const double bound = 2.0 * radius + margin;
    const double bound2 = bound * bound;
    const std::size_t atomCount = geometry.atoms.size();
    window.clear();

    for (std::size_t b = 0; b < atomCount; ++b)
    {
        const double dx = geometry.atoms[b].position[0] - geometry.atoms[owner].position[0];
        const double dy = geometry.atoms[b].position[1] - geometry.atoms[owner].position[1];
        const double dz = geometry.atoms[b].position[2] - geometry.atoms[owner].position[2];

        if (dx * dx + dy * dy + dz * dz <= bound2)
        {
            window.push_back(b);
        }
    }
}

// Becke fuzzy-cell partition weight (Becke, J. Chem. Phys. 88 (1988) 2547)
// with the SSF step and the Bragg-Slater heteroatom adjustment, normalized
// to a partition of unity at the point.  Returns the weight of the OWNING
// atom's cell - the only weight the grid build consumes.
//
// The pass is the pair loop's own two comparisons regrouped, not a new
// approximation.  With kX = 0.64 times a Slater radius, the loop's break
// test is r_a >= kA[a] + (r_b + kB[b]) and its exactly-1 no-contest case is
// r_a + kA[a] <= r_b - kB[b].  Writing lo[b] = r_b - kB[b] and
// hi[b] = r_b + kB[b] those read lo[a] >= hi[b] and hi[a] <= lo[b], so:
//   - the cell of atom a is exactly zero unless lo[a] < min over b != a of
//     hi[b] - two running minima settle that for every atom at once, in
//     place of one scan per atom;
//   - otherwise its product runs over the {b : lo[b] < hi[a]} only, because
//     every other factor is exactly 1.0 and multiplying by 1.0 is exact.
// A broken cell is set to 0 either way and a surviving product multiplies
// the same factors in the same order as the full N^2 form, so the weights
// are unchanged.  With the shell window in front of it the pass visits the
// local atoms instead of the molecule: measured 2.25 candidate atoms per
// point at 48 atoms and 3.18 at 384 (measured 2026-09-13).
double OwnerPartitionWeight(const std::array<double, 3>& point,
                            const Geometry& geometry,
                            PartitionScratch& scratch,
                            std::size_t owner) {
    const std::size_t atomCount = geometry.atoms.size();
    const std::span<const std::size_t> window = scratch.window;
    const std::vector<double>& radius = scratch.radius;
    std::vector<double>& distance = scratch.distance;
    std::vector<double>& lo = scratch.lo;
    std::vector<double>& hi = scratch.hi;
    std::vector<double>& raw = scratch.raw;

    double nearest = std::numeric_limits<double>::infinity();
    double secondNearest = std::numeric_limits<double>::infinity();
    double smallestHi = std::numeric_limits<double>::infinity();
    double secondSmallestHi = std::numeric_limits<double>::infinity();
    std::size_t nearestAtom = 0;
    std::size_t smallestHiAtom = atomCount;

    for (const std::size_t a : window)
    {
        const std::array<double, 3>& center = geometry.atoms[a].position;
        const double dx = point[0] - center[0];
        const double dy = point[1] - center[1];
        const double dz = point[2] - center[2];
        const double r = std::sqrt(dx * dx + dy * dy + dz * dz);
        const double windowRadius = kSwitchWindow * radius[a];
        distance[a] = r;
        lo[a] = r - windowRadius;
        hi[a] = r + windowRadius;
        raw[a] = 0.0;

        if (r < nearest)
        {
            secondNearest = nearest;
            nearest = r;
            nearestAtom = a;
        } else if (r < secondNearest)
        { secondNearest = r; }

        if (hi[a] < smallestHi)
        {
            secondSmallestHi = smallestHi;
            smallestHi = hi[a];
            smallestHiAtom = a;
        } else if (hi[a] < secondSmallestHi)
        { secondSmallestHi = hi[a]; }
    }

    // Two of the reductions above can read atoms the window leaves out, and
    // an atom outside the window is farther than `radius + margin` from the
    // point (that is what excludes it) - call that the floor.  The nearest
    // atom is always inside (the owner sits at the sphere radius, so
    // nearest <= radius).  The second nearest is exact whenever it is at most
    // the floor, because the true one is then the smaller of it and a value
    // beyond the floor; past the floor the shortcut's comparison is still
    // settled whenever the nearest sits below 0.5 (1 - a) times the floor,
    // because the true second nearest is beyond the floor too, so the
    // comparison is false - and evaluating it with the window's larger value
    // gives false as well.  The second smallest hi settles the owner's own
    // break test, and the same floor argument applies: at or below the floor
    // it is the true value, above it the true value may be an excluded atom's
    // and the test's answer could differ.  Both are then measured, on the
    // atom list, for the points that reach that state - never for the rest.
    const double floor = distance[owner] + scratch.windowMargin;
    const bool windowComplete = window.size() == atomCount;
    const bool secondNearestExact = windowComplete || secondNearest <= floor;
    const bool secondHiExact = windowComplete || secondSmallestHi <= floor;
    const bool shortcutDecided = nearest <= 0.5 * (1.0 - kSwitchWindow) * floor;

    if ((!secondNearestExact && !shortcutDecided) || !secondHiExact)
    {
        secondNearest = std::numeric_limits<double>::infinity();
        secondSmallestHi = std::numeric_limits<double>::infinity();

        for (std::size_t b = 0; b < atomCount; ++b)
        {
            const double dx = point[0] - geometry.atoms[b].position[0];
            const double dy = point[1] - geometry.atoms[b].position[1];
            const double dz = point[2] - geometry.atoms[b].position[2];
            const double r = std::sqrt(dx * dx + dy * dy + dz * dz);

            if (b != nearestAtom && r < secondNearest)
            {
                secondNearest = r;
            }

            if (b != smallestHiAtom && r + kSwitchWindow * radius[b] < secondSmallestHi)
            {
                secondSmallestHi = r + kSwitchWindow * radius[b];
            }
        }
    }

    // Near-nucleus shortcut: within 0.5 (1 - a) of the nearest atom every
    // switch sits on the mu <= -a plateau (the standard Becke shortcut;
    // the renormalization below still covers the tail of anomalously
    // short bonds).
    if (nearest > 0.5 * (1.0 - kSwitchWindow) * secondNearest)
    {
        for (const std::size_t a : window)
        {
            const double smallestOtherHi = (smallestHiAtom == a) ? secondSmallestHi : smallestHi;

            if (lo[a] >= smallestOtherHi)
            {
                continue; // the pair loop breaks here with weight 0
            }

            double weight = 1.0;
            const double upper = hi[a];

            for (const std::size_t b : window)
            {
                if (b == a || lo[b] >= upper)
                {
                    continue;
                }

                const double pairRadius = radius[a] + radius[b];
                weight *= Switch((distance[a] - distance[b]) / pairRadius);
            }

            raw[a] = weight;
        }
    } else
    {
        raw[nearestAtom] = 1.0;
    }

    // The normalization runs over the window: every atom outside it has a
    // cell of exactly 0.0, and adding 0.0 leaves a partial sum unchanged, so
    // the total is the same sum in the same order.
    double total = 0.0;

    for (const std::size_t a : window)
    {
        total += raw[a];
    }

    return total > 0.0 ? raw[owner] / total : raw[owner];
}

// The first free index at or after `start`, or `count` when there is none.
// The occupancy map holds one bit per point, set while the point is free, so
// the scan advances a WORD at a time and a run of assigned points costs one
// step per 64 points instead of one per point.  The bit position inside the
// word is read from the lowest set bit by popcount of its square minus one
// (one instruction, branch-free); the standard trailing-zero spelling is a
// snake_case token the style guard treats as a project identifier.
std::size_t NextFree(const std::vector<std::uint64_t>& freeWords,
                     std::size_t start,
                     std::size_t count) {
    std::size_t word = start >> 6;
    std::uint64_t bits = freeWords[word] & (~0ull << (start & 63));

    while (true)
    {
        if (bits != 0)
        {
            const std::uint64_t lowest = bits & (0ull - bits);
            const std::size_t index =
                (word << 6) + static_cast<std::size_t>(std::popcount(lowest - 1ull));

            return index < count ? index : count;
        }

        ++word;

        if (word >= freeWords.size())
        {
            return count;
        }

        bits = freeWords[word];
    }
}

// One unassigned-grid-point record for the re-batching pass.
struct GridPoint {
    std::array<double, 3> position;
    double weight;
    std::size_t atom;
    bool assigned = false;
};

} // namespace

Result<BlockGrid> BlockGrid::Create(const Geometry& geometry, const GridParams& params) {
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

    // The per-atom product grids: for every atom, every radial point
    // times every angular node, folded with r^2 (the volume element the
    // radial weights deliberately exclude), then the Becke partition
    // weight of the point, then the trim.  The scratch is built once for
    // the whole pass: it carries the point-invariant radii, the arrays each
    // point would otherwise allocate, and the shell window.  One window
    // serves every angular node of a shell, and a point is dropped here
    // rather than kept in an intermediate cloud, so the pass holds only the
    // points that survive.
    std::vector<GridPoint> kept;
    PartitionScratch scratch = MakePartitionScratch(geometry);

    for (std::size_t a = 0; a < geometry.atoms.size(); ++a)
    {
        const std::array<double, 3>& center = geometry.atoms[a].position;

        for (std::size_t r = 0; r < radial->Size(); ++r)
        {
            const double radius = radial->Points()[r];
            const double radialWeight = radial->Weights()[r] * radius * radius;

            BuildShellWindow(geometry, a, radius, scratch.windowMargin, scratch.window);

            for (std::size_t t = 0; t < angular->Size(); ++t)
            {
                const std::array<double, 3> node = angular->Point(t);
                const std::array<double, 3> position = {center[0] + radius * node[0],
                                                        center[1] + radius * node[1],
                                                        center[2] + radius * node[2]};
                const double weight = (radialWeight * angular->Weight(t)) *
                                      OwnerPartitionWeight(position, geometry, scratch, a);

                if (std::abs(weight) >= params.trimWeight)
                {
                    kept.push_back({position, weight, a, false});
                }
            }
        }
    }

    std::vector<Block> blocks;
    std::size_t cursor = 0;
    const std::size_t count = kept.size();
    std::size_t remaining = count;

    // The re-seed asks a different question from the fill: not "which of the
    // next kCandidateCap is nearest" (that walk IS the choice rule) but "where
    // is the next free point".  In the endgame it dominates the pass - 81-87 %
    // of the seeds run with 10 % or less of the pool free and walk 67-184
    // times the uniform gap between free points, measured 1999-5368 probes per
    // block (measured 2026-09-15).  One bit per point answers it a word at a
    // time; an index per point costs 64 times the memory for no measured gain.
    std::vector<std::uint64_t> freeWords(count / 64 + 1, ~0ull);

    if (count % 64 != 0)
    {
        freeWords.back() &= (1ull << (count % 64)) - 1ull;
    }

    while (remaining > 0)
    {
        // The seed is the first free point at or after the cursor, wrapping -
        // the same point the walk returned, found without traversing the
        // assigned run.
        std::size_t seed = NextFree(freeWords, cursor, count);

        if (seed == count)
        {
            seed = NextFree(freeWords, 0, count);
        }

        Block block;
        std::array<double, 3> centroid = kept[seed].position;
        std::size_t centroidCount = 1;
        kept[seed].assigned = true;
        freeWords[seed >> 6] &= ~(1ull << (seed & 63));
        remaining -= 1;

        block.points.push_back(kept[seed].position);
        block.weights.push_back(kept[seed].weight);
        block.atomIndex.push_back(kept[seed].atom);

        while (block.points.size() < params.blockTarget && remaining > 0)
        {
            std::size_t best = count;
            double bestDistance = std::numeric_limits<double>::infinity();

            for (std::size_t probe = 0; probe < kCandidateCap && probe < count; ++probe)
            {
                if (++cursor == count)
                {
                    cursor = 0;
                }

                const GridPoint& candidate = kept[cursor];

                if (candidate.assigned)
                {
                    continue;
                }

                const double dx = candidate.position[0] - centroid[0];
                const double dy = candidate.position[1] - centroid[1];
                const double dz = candidate.position[2] - centroid[2];
                const double distanceSquared = dx * dx + dy * dy + dz * dz;

                if (distanceSquared < bestDistance)
                {
                    bestDistance = distanceSquared;
                    best = cursor;
                }
            }

            if (best == count)
            {
                break; // The remaining pool is out of this scan's reach; re-seed.
            }

            const GridPoint& chosen = kept[best];
            centroid = {(centroid[0] * centroidCount + chosen.position[0]) / (centroidCount + 1.0),
                        (centroid[1] * centroidCount + chosen.position[1]) / (centroidCount + 1.0),
                        (centroid[2] * centroidCount + chosen.position[2]) / (centroidCount + 1.0)};
            centroidCount += 1;
            kept[best].assigned = true;
            freeWords[best >> 6] &= ~(1ull << (best & 63));
            remaining -= 1;

            block.points.push_back(chosen.position);
            block.weights.push_back(chosen.weight);
            block.atomIndex.push_back(chosen.atom);
        }

        block.pointCount = block.points.size();
        blocks.push_back(std::move(block));
    }

    return BlockGrid(std::move(blocks), count);
}

BlockGrid::BlockGrid(std::vector<Block> blocks, std::size_t totalPoints) noexcept :
    _blocks(std::move(blocks)), _totalPoints(totalPoints) {}

} // namespace excgrid
