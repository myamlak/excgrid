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

// The SSF switching function (Stratmann, Scuseria, Frisch, Chem. Phys. Lett. 257
// (1996) 213): 0 for mu >= a, 1 for mu <= -a, else 0.5 (1 - z(t)), t = mu / a,
//     z(t) = t(35 - t^2(35 - t^2(21 - 5 t^2)))/16.
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

// The per-build work arrays of the partition pass.
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

// The neighbour list of one shell: the atoms with d_ab <= 2 radius + 4 kMax,
// kMax = 0.64 max_a radius_a.  An atom outside is farther than radius + 4 kMax
// from every point of the shell and is provably inert there.
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

// Becke fuzzy-cell partition weight (Becke, J. Chem. Phys. 88 (1988) 2547),
// normalized to unity at the point; returns the OWNING atom's cell.  With
// lo[b] = r_b - kB[b] and hi[b] = r_b + kB[b], a cell is exactly zero unless
// lo[a] < min_{b != a} hi[b], and its product runs over {b : lo[b] < hi[a]} only.
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

    // The window's reductions can miss an atom, so the three flags below are
    // conservative tests for whether the values are the true ones; the scan
    // recomputes them in full when any fails.
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

    // Near-nucleus shortcut: within 0.5 (1 - a) of the nearest atom every switch
    // sits on the mu <= -a plateau; the renormalization below covers the tail.
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

    // Atoms outside the window have a cell of exactly 0.0, so the total is unchanged.
    double total = 0.0;

    for (const std::size_t a : window)
    {
        total += raw[a];
    }

    return total > 0.0 ? raw[owner] / total : raw[owner];
}

// The first free index at or after `start`, or `count` when there is none.
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

    // One bit per point, set while the point is free.
    std::vector<std::uint64_t> freeWords(count / 64 + 1, ~0ull);

    if (count % 64 != 0)
    {
        freeWords.back() &= (1ull << (count % 64)) - 1ull;
    }

    while (remaining > 0)
    {
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
