
#include "../src/grid_derivs.hpp"

#include "excgrid/geometry.hpp"
#include "excgrid/grid.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>
#include <limits>
#include <string>
#include <vector>

namespace {

using excgrid::derivs::DerivativeOrder;
using excgrid::derivs::DerivativeScratch;
using excgrid::derivs::EvaluatePartitionWeight;
using excgrid::derivs::EvaluatePointCoordinates;
using excgrid::derivs::EvaluatePointWeight;
using excgrid::derivs::GridBasis;
using excgrid::derivs::MakeDerivativeScratch;
using excgrid::derivs::MakeGridBasis;
using excgrid::derivs::PointCoordinateDerivatives;
using excgrid::derivs::PointId;
using excgrid::derivs::PointPosition;
using excgrid::derivs::PointQuadrature;
using excgrid::derivs::PointWeightDerivatives;

// h = 1e-5: O(h^2) truncation and O(eps/h) round-off both near 1e-10 for unit-scale weights.
constexpr double kStep = 1e-5;

// Two steps separate the difference's own error from the analytic value's.
constexpr double kHalfStep = 0.5 * kStep;

// Second-difference round-off scales as value / h^2, so the second step is 1e-3.
constexpr double kSecondStep = 1e-3;
constexpr double kSecondHalfStep = 0.5 * kSecondStep;

constexpr double kDerivativeTolerance = 1e-8;

excgrid::Geometry HydrogenPair() {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({1, {0.0, 0.0, -0.7}});
    geometry.atoms.push_back({1, {0.0, 0.0, 0.7}});

    return geometry;
}

// Distorted pair: no coordinate zero, no ties, no mirror symmetry; the bond still runs along z.
excgrid::Geometry JitteredPair() {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({1, {0.031, -0.107, -0.7}});
    geometry.atoms.push_back({1, {-0.023, 0.088, 0.7}});

    return geometry;
}

excgrid::Geometry CarbonOxygen() {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({6, {0.0, 0.0, 0.0}});
    geometry.atoms.push_back({8, {0.0, 0.0, 2.3}});

    return geometry;
}

excgrid::Geometry SingleAtom() {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({1, {0.35, -0.2, 0.9}});

    return geometry;
}

/// A jittered chain of atomCount hydrogens 2.4 Bohr apart.
excgrid::Geometry ChainOfAtoms(std::size_t atomCount) {
    excgrid::Geometry geometry;

    for (std::size_t index = 0; index < atomCount; ++index)
    {
        const double jitter = 0.013 * static_cast<double>(index % 7) - 0.05;
        geometry.atoms.push_back({1, {2.4 * static_cast<double>(index), jitter, -0.5 * jitter}});
    }

    return geometry;
}

// An offset in the owning atom's frame: the point moves with its owner.
struct PointCase {
    std::size_t owner;
    std::array<double, 3> offset;
    std::string label;
};

// Both cells are live when the two distances differ by less than a (rad_0 + rad_1) = 0.85 Bohr.
std::vector<PointCase> PairPointCases() {
    return {
        {0, {0.0, 0.0, 1.0}, "toward the partner"},
        {0, {0.0, 0.0, 0.5}, "inside the window"},
        {0, {1.2, 0.0, 0.0}, "side on"},
        {0, {0.4, 0.4, 0.6}, "off axis"},
        {1, {0.0, 0.0, -0.3}, "mirror of the window case"},
        {1, {0.0, 0.0, -1.0}, "mirror of the toward case"},
    };
}

std::array<double, 3> AttachedPoint(const excgrid::Geometry& geometry, const PointCase& pointCase) {
    const std::array<double, 3>& center = geometry.atoms[pointCase.owner].position;

    return {center[0] + pointCase.offset[0],
            center[1] + pointCase.offset[1],
            center[2] + pointCase.offset[2]};
}

double WeightAt(const excgrid::Geometry& geometry,
                const PointCase& pointCase,
                DerivativeScratch& scratch,
                PointWeightDerivatives& out) {
    EvaluatePartitionWeight(
        geometry, AttachedPoint(geometry, pointCase), pointCase.owner, scratch, out);

    return out.value;
}

// Atom indices name the derivative entries, so an atom must keep its index across a displacement.
excgrid::Geometry Displaced(const excgrid::Geometry& geometry,
                            std::size_t atom,
                            std::size_t component,
                            double amount) {
    excgrid::Geometry moved = geometry;
    moved.atoms[atom].position[component] += amount;

    EXPECT_EQ(moved.atoms.size(), geometry.atoms.size())
        << "a displacement changed the number of atoms";

    for (std::size_t index = 0; index < geometry.atoms.size(); ++index)
    {
        EXPECT_EQ(moved.atoms[index].atomicNumber, geometry.atoms[index].atomicNumber)
            << "a displacement reordered the atoms: index " << index << " changed element";

        for (std::size_t k = 0; k < 3; ++k)
        {
            const double expected = geometry.atoms[index].position[k] +
                                    ((index == atom && k == component) ? amount : 0.0);
            EXPECT_DOUBLE_EQ(moved.atoms[index].position[k], expected)
                << "a displacement moved atom " << index << " component " << k
                << " by something other than the one coordinate asked for";
        }
    }

    return moved;
}

// Coordinates are named 3 * atom + component, as the derivative arrays name them.
excgrid::Geometry Displaced(const excgrid::Geometry& geometry,
                            std::size_t firstCoordinate,
                            std::size_t secondCoordinate,
                            double firstAmount,
                            double secondAmount) {
    const excgrid::Geometry once =
        Displaced(geometry, firstCoordinate / 3, firstCoordinate % 3, firstAmount);

    return Displaced(once, secondCoordinate / 3, secondCoordinate % 3, secondAmount);
}

void CheckFirstDerivativesAgainstDifference(const excgrid::Geometry& geometry,
                                            const PointCase& pointCase,
                                            DerivativeScratch& scratch,
                                            PointWeightDerivatives& out) {
    const std::size_t atomCount = geometry.atoms.size();
    const double analyticValue = WeightAt(geometry, pointCase, scratch, out);
    const std::vector<double> analytic = out.first;

    ASSERT_EQ(analytic.size(), 3 * atomCount);

    double largest = 0.0;

    for (std::size_t atom = 0; atom < atomCount; ++atom)
    {
        for (std::size_t component = 0; component < 3; ++component)
        {
            const excgrid::Geometry plus = Displaced(geometry, atom, component, kStep);
            const excgrid::Geometry minus = Displaced(geometry, atom, component, -kStep);
            const double difference = (WeightAt(plus, pointCase, scratch, out) -
                                       WeightAt(minus, pointCase, scratch, out)) /
                                      (2.0 * kStep);
            const std::size_t index = 3 * atom + component;
            const double scale = std::max(std::abs(analytic[index]), std::abs(difference));

            EXPECT_NEAR(analytic[index], difference, kDerivativeTolerance + 1e-7 * scale)
                << pointCase.label << " atom " << atom << " component " << component << " weight "
                << analyticValue;
            largest = std::max(largest, std::abs(analytic[index]));
        }
    }

    // Guard against a vacuous pass: the fixture must have live pairs.
    EXPECT_GT(largest, 1e-3) << pointCase.label << " has no live pair";
}

/// Four evaluations at two steps; `scale` is the magnitude the difference's round-off rides on.
struct DifferenceSamples {
    double ahead = 0.0; ///< The quantity at +kStep.
    double behind = 0.0; ///< The quantity at -kStep.
    double aheadHalf = 0.0; ///< The quantity at +kHalfStep.
    double behindHalf = 0.0; ///< The quantity at -kHalfStep.
    double reference = 0.0; ///< The quantity at the undisplaced geometry.
    double scale = 0.0; ///< The magnitude the difference's round-off rides on.
};

// Truncation is three quarters of the two steps' gap; a few times that is not the difference's error.
constexpr double kGapMultiple = 8.0;

// eps * scale / (2h), times 64 for the distance's conditioning; measured, not fitted.
constexpr double kRoundOffUlp = 64.0;

// Six digits: what a central difference at h = 1e-5 with a well-scaled value resolves.
constexpr double kResolution = 1e-6;

// Tolerance combines truncation (gap), round-off (scale) and the step's resolution.
void ExpectMatchesDifference(double analytic,
                             const DifferenceSamples& samples,
                             const std::string& label) {
    const double coarse = (samples.ahead - samples.behind) / (2.0 * kStep);
    const double fine = (samples.aheadHalf - samples.behindHalf) / (2.0 * kHalfStep);
    const double gap = std::abs(coarse - fine);
    const double error = std::abs(analytic - coarse);
    const double tolerance = kGapMultiple * gap +
                             kRoundOffUlp * std::numeric_limits<double>::epsilon() *
                                 std::abs(samples.scale) / kStep +
                             kResolution * std::abs(coarse);

    EXPECT_LE(error, tolerance)
        << label << ": analytic " << analytic << ", difference " << coarse << " (at " << kHalfStep
        << ": " << fine << "), value " << samples.reference << ", tolerance " << tolerance;
}

// (f(+h) - 2 f(0) + f(-h)) / h^2; truncation via the gap, round-off via scale, resolution.
[[nodiscard]] double SecondDifference(const DifferenceSamples& samples, double step) {
    return (samples.ahead - 2.0 * samples.reference + samples.behind) / (step * step);
}

/// Separate so a wrong analytic derivative is held to the same bound and its miss reported.
[[nodiscard]] double SecondDifferenceTolerance(const DifferenceSamples& samples, double step) {
    const double coarse = SecondDifference(samples, step);
    const double fine = (samples.aheadHalf - 2.0 * samples.reference + samples.behindHalf) /
                        (step * step * 0.25);
    const double gap = std::abs(coarse - fine);

    return kGapMultiple * gap +
           kRoundOffUlp * std::numeric_limits<double>::epsilon() * std::abs(samples.scale) /
               (step * step) +
           kResolution * std::abs(coarse);
}

void ExpectMatchesSecondDifference(double analytic,
                                   double step,
                                   const DifferenceSamples& samples,
                                   const std::string& label) {
    const double coarse = SecondDifference(samples, step);
    const double fine = (samples.aheadHalf - 2.0 * samples.reference + samples.behindHalf) /
                        (step * step * 0.25);
    const double error = std::abs(analytic - coarse);
    const double tolerance = SecondDifferenceTolerance(samples, step);

    EXPECT_LE(error, tolerance)
        << label << ": analytic " << analytic << ", difference " << coarse << " (at half the step: "
        << fine << "), value " << samples.reference << ", tolerance " << tolerance;
}

// Four corners (+h,+h), (+h,-h), (-h,+h), (-h,-h) at both steps.
struct CrossSamples {
    std::array<double, 4> corner = {0.0, 0.0, 0.0, 0.0}; ///< The four at the step.
    std::array<double, 4> cornerHalf = {0.0, 0.0, 0.0, 0.0}; ///< The four at half of it.
    double reference = 0.0; ///< The quantity at the undisplaced geometry.
    double scale = 0.0; ///< The magnitude the difference's round-off rides on.
};

[[nodiscard]] double CrossDifference(const CrossSamples& samples, double step) {
    return (samples.corner[0] - samples.corner[1] - samples.corner[2] + samples.corner[3]) /
           (4.0 * step * step);
}

[[nodiscard]] double CrossDifferenceTolerance(const CrossSamples& samples, double step) {
    const double coarse = CrossDifference(samples, step);
    const double fine = (samples.cornerHalf[0] - samples.cornerHalf[1] - samples.cornerHalf[2] +
                         samples.cornerHalf[3]) /
                        (step * step);
    const double gap = std::abs(coarse - fine);

    return kGapMultiple * gap +
           kRoundOffUlp * std::numeric_limits<double>::epsilon() * std::abs(samples.scale) /
               (step * step) +
           kResolution * std::abs(coarse);
}

void ExpectMatchesCrossDifference(double analytic,
                                  double step,
                                  const CrossSamples& samples,
                                  const std::string& label) {
    const double coarse = CrossDifference(samples, step);
    const double fine = (samples.cornerHalf[0] - samples.cornerHalf[1] - samples.cornerHalf[2] +
                         samples.cornerHalf[3]) /
                        (step * step);
    const double error = std::abs(analytic - coarse);
    const double tolerance = CrossDifferenceTolerance(samples, step);

    EXPECT_LE(error, tolerance)
        << label << ": analytic " << analytic << ", difference " << coarse << " (at half the step: "
        << fine << "), value " << samples.reference << ", tolerance " << tolerance;
}

// One atom: the partition is 1 everywhere, so the weight is exactly 1 and derivatives exactly 0.
TEST(GridDerivPairTest, OneAtomWeightIsOneWithZeroDerivative) {
    const excgrid::Geometry geometry = SingleAtom();
    DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kFirst);
    PointWeightDerivatives out;

    for (const PointCase& pointCase :
         std::vector<PointCase>{{0, {0.0, 0.0, 1.0}, "one atom"}, {0, {2.5, 1.0, -0.5}, "far out"}})
    {
        const double weight = WeightAt(geometry, pointCase, scratch, out);

        EXPECT_DOUBLE_EQ(weight, 1.0) << pointCase.label;
        ASSERT_EQ(out.first.size(), 3u);

        for (const double entry : out.first)
        {
            EXPECT_EQ(entry, 0.0) << pointCase.label;
        }
    }
}

TEST(GridDerivPairTest, WeightDerivativeMatchesCentralDifference) {
    const excgrid::Geometry geometry = HydrogenPair();
    DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kFirst);
    PointWeightDerivatives out;

    for (const PointCase& pointCase : PairPointCases())
    {
        EvaluatePartitionWeight(
            geometry, AttachedPoint(geometry, pointCase), pointCase.owner, scratch, out);
        ASSERT_EQ(out.activeAtoms, 2u) << pointCase.label;
        CheckFirstDerivativesAgainstDifference(geometry, pointCase, scratch, out);
    }
}

// Past the switch window: one cell 1, the other 0, so every derivative is exactly zero.
TEST(GridDerivPairTest, PlateauPointHasNoDerivative) {
    const excgrid::Geometry geometry = HydrogenPair();
    DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kFirst);
    PointWeightDerivatives out;

    struct PlateauCase {
        PointCase pointCase;
        double weight;
    };

    const std::vector<PlateauCase> cases = {
        {{0, {0.0, 0.0, -6.0}, "owning side, far out"}, 1.0},
        {{1, {0.0, 0.0, 6.0}, "owning side, far out, mirror"}, 1.0},
        {{0, {0.0, 0.0, 3.0}, "past the partner"}, 0.0},
        {{1, {0.0, 0.0, -3.0}, "past the partner, mirror"}, 0.0},
    };

    for (const PlateauCase& entry : cases)
    {
        const double weight = WeightAt(geometry, entry.pointCase, scratch, out);

        EXPECT_DOUBLE_EQ(weight, entry.weight) << entry.pointCase.label;
        ASSERT_EQ(out.activeAtoms, 1u) << entry.pointCase.label;

        for (const double derivative : out.first)
        {
            EXPECT_EQ(derivative, 0.0) << entry.pointCase.label;
        }
    }
}

// A rigid translation leaves the weight unchanged: every component sums to zero over the atoms.
TEST(GridDerivPairTest, DerivativeSumsToZeroOverAtoms) {
    const excgrid::Geometry geometry = HydrogenPair();
    DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kFirst);
    PointWeightDerivatives out;

    for (const PointCase& pointCase : PairPointCases())
    {
        WeightAt(geometry, pointCase, scratch, out);

        for (std::size_t component = 0; component < 3; ++component)
        {
            double sum = 0.0;

            for (std::size_t atom = 0; atom < geometry.atoms.size(); ++atom)
            {
                sum += out.first[3 * atom + component];
            }

            EXPECT_NEAR(sum, 0.0, 1e-12) << pointCase.label << " component " << component;
        }
    }
}

// Different radii: the pair radius is not twice one radius, so the radius table is exercised.
TEST(GridDerivPairTest, WeightDerivativeMatchesCentralDifferenceForAHeteroatomPair) {
    const excgrid::Geometry geometry = CarbonOxygen();
    DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kFirst);
    PointWeightDerivatives out;

    for (const PointCase& pointCase : std::vector<PointCase>{
             {0, {0.0, 0.0, 0.9}, "carbon side, toward oxygen"},
             {0, {0.0, 0.0, 1.6}, "carbon side, inside the window"},
             {1, {0.0, 0.0, -0.9}, "oxygen side, toward carbon"},
         })
    {
        CheckFirstDerivativesAgainstDifference(geometry, pointCase, scratch, out);
    }
}

// The stated per-point footprint (21N + 2 doubles) must equal the actual allocation.
TEST(GridDerivPairTest, ScratchFootprintMatchesItsFormula) {
    const excgrid::Geometry geometry = HydrogenPair();
    const DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kFirst);

    EXPECT_EQ(scratch.DoubleCount(), DerivativeScratch::PerPointDoubles(geometry.atoms.size()));

    for (const std::size_t atomCount : {1u, 3u, 8u, 32u, 128u})
    {
        const excgrid::Geometry chain = ChainOfAtoms(atomCount);
        const DerivativeScratch built = MakeDerivativeScratch(chain, DerivativeOrder::kFirst);

        EXPECT_EQ(built.DoubleCount(), DerivativeScratch::PerPointDoubles(atomCount))
            << "at " << atomCount << " atoms the scratch holds " << built.DoubleCount()
            << " doubles, not " << DerivativeScratch::PerPointDoubles(atomCount);
    }
}


std::size_t FlatCount(const excgrid::Geometry& geometry, const GridBasis& basis) {
    return geometry.atoms.size() * basis.radial.Size() * basis.angular.Size();
}

// A stable name for a point, atom-major.
std::size_t FlatIndex(const GridBasis& basis, const PointId& id) {
    return (id.atom * basis.radial.Size() + id.radial) * basis.angular.Size() + id.angular;
}

PointId FromFlat(const GridBasis& basis, std::size_t flat) {
    const std::size_t angular = flat % basis.angular.Size();
    const std::size_t radial = (flat / basis.angular.Size()) % basis.radial.Size();

    return {flat / (basis.angular.Size() * basis.radial.Size()), radial, angular};
}

// Recovered from the point's position: the matching radial point and the angular node it lies along.
PointId Identify(const excgrid::Geometry& geometry,
                 const GridBasis& basis,
                 const excgrid::Block& block,
                 std::size_t index) {
    const std::size_t atom = block.atomIndex[index];
    const std::array<double, 3>& point = block.points[index];
    const std::array<double, 3>& center = geometry.atoms[atom].position;
    const double dx = point[0] - center[0];
    const double dy = point[1] - center[1];
    const double dz = point[2] - center[2];
    const double distance = std::sqrt(dx * dx + dy * dy + dz * dz);

    std::size_t radial = 0;
    double bestGap = std::numeric_limits<double>::infinity();
    double secondGap = std::numeric_limits<double>::infinity();

    for (std::size_t r = 0; r < basis.radial.Size(); ++r)
    {
        const double gap = std::abs(basis.radial.Points()[r] - distance);

        if (gap < bestGap)
        {
            secondGap = bestGap;
            bestGap = gap;
            radial = r;
        } else if (gap < secondGap)
        {
            secondGap = gap;
        }
    }

    std::size_t angular = 0;
    double bestAlignment = -2.0;
    double secondAlignment = -2.0;

    for (std::size_t t = 0; t < basis.angular.Size(); ++t)
    {
        const std::array<double, 3> node = basis.angular.Point(t);
        const double alignment = (dx * node[0] + dy * node[1] + dz * node[2]) / distance;

        if (alignment > bestAlignment)
        {
            secondAlignment = bestAlignment;
            bestAlignment = alignment;
            angular = t;
        } else if (alignment > secondAlignment)
        {
            secondAlignment = alignment;
        }
    }

    const PointId id{atom, radial, angular};
    const std::array<double, 3> rebuilt = PointPosition(geometry, basis, id);

    for (std::size_t k = 0; k < 3; ++k)
    {
        EXPECT_NEAR(rebuilt[k], point[k], 1e-12) << "point " << index << " component " << k;
    }

    EXPECT_LT(bestGap, 1e-12) << "point " << index << " is not on a radial point";
    EXPECT_GT(secondGap, 1e-6) << "point " << index << " sits between two radial points";
    EXPECT_LT(bestAlignment, 1.0 + 1e-12) << "point " << index;
    EXPECT_GT(bestAlignment - secondAlignment, 1e-6) << "point " << index << " sits between nodes";

    return id;
}

struct GridTable {
    std::vector<double> weight; ///< weight[flat], meaningful where present[flat].
    std::vector<char> present; ///< 1 where the point survived the trim.
    std::size_t kept = 0; ///< How many points survived.
    std::size_t seen = 0; ///< How many distinct identities the grid named.
};

GridTable Tabulate(const excgrid::Geometry& geometry,
                   const GridBasis& basis,
                   const excgrid::BlockGrid& grid) {
    GridTable table;
    table.weight.assign(FlatCount(geometry, basis), 0.0);
    table.present.assign(FlatCount(geometry, basis), 0);

    for (const excgrid::Block& block : grid.Blocks())
    {
        for (std::size_t index = 0; index < block.pointCount; ++index)
        {
            const PointId id = Identify(geometry, basis, block, index);
            const std::size_t flat = FlatIndex(basis, id);

            EXPECT_EQ(table.present[flat], 0) << "two block points share one identity";
            table.present[flat] = 1;
            table.weight[flat] = block.weights[index];
            ++table.kept;
        }
    }

    for (const char present : table.present)
    {
        table.seen += (present != 0) ? 1u : 0u;
    }

    EXPECT_EQ(table.kept, grid.TotalPointCount());
    EXPECT_EQ(table.kept, table.seen);

    return table;
}

struct DisplacedGrid {
    excgrid::Geometry geometry; ///< The geometry, displaced.
    GridTable table; ///< Its grid's weights, by identity.
    bool built = false; ///< Whether the build succeeded.
};

DisplacedGrid BuildDisplaced(const excgrid::Geometry& geometry,
                             const excgrid::GridParams& params,
                             const GridBasis& basis,
                             std::size_t atom,
                             std::size_t component,
                             double amount) {
    DisplacedGrid out;
    out.geometry = Displaced(geometry, atom, component, amount);
    auto grid = excgrid::BlockGrid::Create(out.geometry, params);

    if (!grid)
    {
        return out;
    }

    out.table = Tabulate(out.geometry, basis, *grid);
    out.built = true;

    return out;
}

std::array<double, 4> GridTotals(const GridTable& table,
                                 const excgrid::Geometry& geometry,
                                 const GridBasis& basis) {
    std::array<double, 4> totals = {0.0, 0.0, 0.0, 0.0};

    for (std::size_t flat = 0; flat < table.present.size(); ++flat)
    {
        if (table.present[flat] == 0)
        {
            continue;
        }

        const std::array<double, 3> position =
            PointPosition(geometry, basis, FromFlat(basis, flat));
        totals[0] += table.weight[flat];

        for (std::size_t row = 0; row < 3; ++row)
        {
            totals[row + 1] += table.weight[flat] * position[row];
        }
    }

    return totals;
}

// Deliberately asymmetric: symmetric totals vanish and their differences measure cancellation.
excgrid::Geometry Triatomic() {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({8, {0.0, 0.0, 0.0}});
    geometry.atoms.push_back({1, {0.24, 1.43, 1.11}});
    geometry.atoms.push_back({1, {-0.17, -1.35, 1.02}});

    return geometry;
}

// Trim off: every identity survives at every displaced geometry. The small radial scale keeps the outer quadrature factor near 1e4.
excgrid::GridParams BlockParams() {
    excgrid::GridParams params;
    params.radialPoints = 24;
    params.angularPoints = 26;
    params.alpha = 0.05;
    params.trimWeight = 0.0;
    params.blockTarget = 128;

    return params;
}


// The point translates rigidly with its owner: the derivative is the owner's identity block.
TEST(GridDerivPointTest, OneAtomOnePointCoordinateMatchesCentralDifference) {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({1, {0.4, -0.3, 0.2}});
    excgrid::GridParams params;
    params.radialPoints = 2;
    params.angularPoints = 6;
    auto basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());

    PointCoordinateDerivatives analytic;
    EvaluatePointCoordinates(0, 1, DerivativeOrder::kFirst, analytic);
    ASSERT_EQ(analytic.first.size(), 9u); // three rows of 3N, N = 1.

    for (const std::size_t radial : {0u, 1u})
    {
        const PointId id{0, radial, 3};

        for (std::size_t component = 0; component < 3; ++component)
        {
            const excgrid::Geometry plus = Displaced(geometry, 0, component, kStep);
            const excgrid::Geometry minus = Displaced(geometry, 0, component, -kStep);
            const std::array<double, 3> ahead = PointPosition(plus, *basis, id);
            const std::array<double, 3> behind = PointPosition(minus, *basis, id);

            for (std::size_t row = 0; row < 3; ++row)
            {
                const double difference = (ahead[row] - behind[row]) / (2.0 * kStep);
                const double expected = (row == component) ? 1.0 : 0.0;

                EXPECT_NEAR(analytic.first[row * 3 + component], difference, 1e-9)
                    << "radial " << radial << " row " << row << " component " << component;
                EXPECT_DOUBLE_EQ(analytic.first[row * 3 + component], expected);
            }
        }
    }
}


// With one atom the weight is the quadrature factor, constant in the geometry: every derivative is zero.
TEST(GridDerivPointTest, OneAtomOnePointWeightMatchesCentralDifference) {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({1, {0.4, -0.3, 0.2}});
    excgrid::GridParams params;
    params.radialPoints = 3;
    params.angularPoints = 6;
    auto basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());

    DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kFirst);
    PointWeightDerivatives out;

    for (std::size_t radial = 0; radial < basis->radial.Size(); ++radial)
    {
        for (const std::size_t angular : {0u, 2u, 5u})
        {
            const PointId id{0, radial, angular};
            EvaluatePointWeight(geometry, *basis, id, scratch, out);

            EXPECT_DOUBLE_EQ(out.value, PointQuadrature(*basis, id)) << "radial " << radial;

            for (std::size_t component = 0; component < 3; ++component)
            {
                const excgrid::Geometry plus = Displaced(geometry, 0, component, kStep);
                const excgrid::Geometry minus = Displaced(geometry, 0, component, -kStep);
                PointWeightDerivatives ahead;
                PointWeightDerivatives behind;
                EvaluatePointWeight(plus, *basis, id, scratch, ahead);
                EvaluatePointWeight(minus, *basis, id, scratch, behind);
                const double difference = (ahead.value - behind.value) / (2.0 * kStep);

                EXPECT_EQ(out.first[component], 0.0) << "radial " << radial;
                EXPECT_NEAR(difference, 0.0, 1e-20) << "radial " << radial;
            }
        }
    }
}

TEST(GridDerivPointTest, TwoAtomOnePointWeightMatchesCentralDifference) {
    const excgrid::Geometry geometry = HydrogenPair();
    excgrid::GridParams params;
    params.radialPoints = 8;
    params.angularPoints = 6;
    auto basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());

    DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kFirst);
    PointWeightDerivatives out;
    std::size_t checked = 0;
    double largest = 0.0;

    for (const std::size_t atom : {0u, 1u})
    {
        for (std::size_t radial = 0; radial < basis->radial.Size(); ++radial)
        {
            for (const std::size_t angular : {0u, 1u, 4u})
            {
                const PointId id{atom, radial, angular};
                EvaluatePointWeight(geometry, *basis, id, scratch, out);

                if (out.value == 0.0 || out.activeAtoms < 2u)
                {
                    continue; // on a plateau, where the derivative is exactly zero
                }

                for (std::size_t partner = 0; partner < 2; ++partner)
                {
                    for (std::size_t component = 0; component < 3; ++component)
                    {
                        DifferenceSamples samples;
                        const auto sample = [&](double amount) {
                            const excgrid::Geometry moved =
                                Displaced(geometry, partner, component, amount);
                            PointWeightDerivatives displaced;
                            EvaluatePointWeight(moved, *basis, id, scratch, displaced);

                            return displaced.value;
                        };
                        samples.ahead = sample(kStep);
                        samples.behind = sample(-kStep);
                        samples.aheadHalf = sample(kHalfStep);
                        samples.behindHalf = sample(-kHalfStep);
                        samples.reference = out.value;
                        samples.scale = PointQuadrature(*basis, id);
                        const std::size_t index = 3 * partner + component;
                        const std::string label = "atom " + std::to_string(atom) + " radial " +
                                                  std::to_string(radial) + " angular " +
                                                  std::to_string(angular);

                        ExpectMatchesDifference(out.first[index], samples, label);
                        largest = std::max(largest, std::abs(out.first[index]));
                    }
                }

                ++checked;
            }
        }
    }

    EXPECT_GT(checked, 10u);
    EXPECT_GT(largest, 1e-6); // a fixture whose derivatives are all zero proves nothing
}


// Every point against the displaced build's weight, then the weight sum and first moment.
TEST(GridDerivBlockTest, PointsAndTotalsMatchTheDisplacedBuild) {
    const excgrid::Geometry geometry = Triatomic();
    const excgrid::GridParams params = BlockParams();
    auto basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());
    auto grid = excgrid::BlockGrid::Create(geometry, params);
    ASSERT_TRUE(grid.has_value());

    const GridTable base = Tabulate(geometry, *basis, *grid);
    const std::size_t atomCount = geometry.atoms.size();
    const std::size_t width = 3 * atomCount;
    DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kFirst);
    PointWeightDerivatives out;

    ASSERT_GT(base.kept, 1000u);
    ASSERT_GT(grid->BlockCount(), 1u);

    for (const excgrid::Block& block : grid->Blocks())
    {
        for (std::size_t index = 0; index < block.pointCount; ++index)
        {
            const PointId id = Identify(geometry, *basis, block, index);
            EvaluatePointWeight(geometry, *basis, id, scratch, out);

            EXPECT_NEAR(out.value, block.weights[index], 1e-12 * std::abs(block.weights[index]))
                << "atom " << id.atom << " radial " << id.radial << " angular " << id.angular;
        }
    }

    // T = sum_i w_i; M_j = sum_i w_i x_ij.
    std::vector<double> totalFirst(width, 0.0);
    std::vector<double> momentFirst(3 * width, 0.0);

    for (std::size_t flat = 0; flat < base.present.size(); ++flat)
    {
        ASSERT_EQ(base.present[flat], 1) << "point " << flat << " is not in the build";

        const PointId id = FromFlat(*basis, flat);
        const std::array<double, 3> position = PointPosition(geometry, *basis, id);
        EvaluatePointWeight(geometry, *basis, id, scratch, out);

        for (std::size_t index = 0; index < width; ++index)
        {
            totalFirst[index] += out.first[index];
        }

        for (std::size_t b = 0; b < atomCount; ++b)
        {
            for (std::size_t l = 0; l < 3; ++l)
            {
                const std::size_t index = 3 * b + l;

                for (std::size_t row = 0; row < 3; ++row)
                {
                    momentFirst[3 * index + row] += out.first[index] * position[row];

                    if (b == id.atom && row == l)
                    {
                        momentFirst[3 * index + row] += out.value;
                    }
                }
            }
        }
    }

    const std::array<double, 4> baseTotals = GridTotals(base, geometry, *basis);
    double largestTotal = 0.0;

    for (std::size_t atom = 0; atom < atomCount; ++atom)
    {
        for (std::size_t component = 0; component < 3; ++component)
        {
            const std::size_t index = 3 * atom + component;
            const std::string where =
                ", displaced along atom " + std::to_string(atom) + " component " +
                std::to_string(component);
            const DisplacedGrid ahead =
                BuildDisplaced(geometry, params, *basis, atom, component, kStep);
            const DisplacedGrid behind =
                BuildDisplaced(geometry, params, *basis, atom, component, -kStep);
            const DisplacedGrid aheadHalf =
                BuildDisplaced(geometry, params, *basis, atom, component, kHalfStep);
            const DisplacedGrid behindHalf =
                BuildDisplaced(geometry, params, *basis, atom, component, -kHalfStep);
            ASSERT_TRUE(ahead.built) << where;
            ASSERT_TRUE(behind.built) << where;
            ASSERT_TRUE(aheadHalf.built) << where;
            ASSERT_TRUE(behindHalf.built) << where;

            for (std::size_t flat = 0; flat < base.present.size(); ++flat)
            {
                ASSERT_EQ(ahead.table.present[flat], 1) << "point " << flat << " is missing" << where;
                ASSERT_EQ(behind.table.present[flat], 1) << "point " << flat << " is missing" << where;
                ASSERT_EQ(aheadHalf.table.present[flat], 1)
                    << "point " << flat << " is missing" << where;
                ASSERT_EQ(behindHalf.table.present[flat], 1)
                    << "point " << flat << " is missing" << where;

                const PointId id = FromFlat(*basis, flat);
                EvaluatePointWeight(geometry, *basis, id, scratch, out);

                DifferenceSamples samples;
                samples.ahead = ahead.table.weight[flat];
                samples.behind = behind.table.weight[flat];
                samples.aheadHalf = aheadHalf.table.weight[flat];
                samples.behindHalf = behindHalf.table.weight[flat];
                samples.reference = out.value;
                samples.scale = PointQuadrature(*basis, id);

                ExpectMatchesDifference(out.first[index],
                                        samples,
                                        "the weight of atom " + std::to_string(id.atom) +
                                            " radial " + std::to_string(id.radial) + " angular " +
                                            std::to_string(id.angular) + where);
            }

            const std::array<double, 4> aheadTotals =
                GridTotals(ahead.table, ahead.geometry, *basis);
            const std::array<double, 4> behindTotals =
                GridTotals(behind.table, behind.geometry, *basis);
            const std::array<double, 4> aheadHalfTotals =
                GridTotals(aheadHalf.table, aheadHalf.geometry, *basis);
            const std::array<double, 4> behindHalfTotals =
                GridTotals(behindHalf.table, behindHalf.geometry, *basis);

            for (std::size_t row = 0; row < 4; ++row)
            {
                DifferenceSamples samples;
                samples.ahead = aheadTotals[row];
                samples.behind = behindTotals[row];
                samples.aheadHalf = aheadHalfTotals[row];
                samples.behindHalf = behindHalfTotals[row];
                samples.reference = baseTotals[row];
                // The total's own magnitude is the scale its round-off rides on.
                samples.scale = baseTotals[row];
                const double analytic =
                    (row == 0) ? totalFirst[index] : momentFirst[3 * index + row - 1];
                const std::string label =
                    (row == 0) ? std::string("the weight sum")
                               : "the first moment, row " + std::to_string(row - 1);

                ExpectMatchesDifference(analytic, samples, label + where);
                largestTotal = std::max(largestTotal, std::abs(analytic));
            }
        }
    }

    // The fixture is asymmetric: no total vanishes by symmetry, so the check has content.
    EXPECT_GT(largestTotal, 1e-3);
}

TEST(GridDerivBlockTest, FixtureCarriesAFullBlock) {
    const excgrid::Geometry geometry = Triatomic();
    const excgrid::GridParams params = BlockParams();
    auto grid = excgrid::BlockGrid::Create(geometry, params);
    ASSERT_TRUE(grid.has_value());

    std::size_t full = 0;

    for (const excgrid::Block& block : grid->Blocks())
    {
        full += (block.pointCount == params.blockTarget) ? 1u : 0u;
    }

    EXPECT_GT(full, 0u);
    EXPECT_GT(grid->TotalPointCount(), 100u);
}


// A rigid translate: the coordinate's second derivative is identically zero.
TEST(GridDerivSecondTest, CoordinateSecondDerivativeIsExactlyZero) {
    PointCoordinateDerivatives analytic;
    EvaluatePointCoordinates(1, 3, DerivativeOrder::kSecond, analytic);

    const std::size_t width = 9;
    ASSERT_EQ(analytic.first.size(), 3u * width);
    ASSERT_EQ(analytic.second.size(), width * width);

    for (std::size_t row = 0; row < 3; ++row)
    {
        for (std::size_t column = 0; column < width; ++column)
        {
            const double expected = (column == 3u + row) ? 1.0 : 0.0;

            EXPECT_DOUBLE_EQ(analytic.first[row * width + column], expected)
                << "row " << row << " column " << column;
        }
    }

    for (const double entry : analytic.second)
    {
        EXPECT_EQ(entry, 0.0);
    }
}

// Diagonal entries against a second difference, mixed against a four-corner cross; both fixtures.
void CheckOnePointHessian(const std::string& label,
                          const excgrid::Geometry& geometry,
                          const GridBasis& basis) {
    const std::size_t atomCount = geometry.atoms.size();
    const std::size_t width = 3 * atomCount;
    DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kSecond);
    PointWeightDerivatives out;
    std::size_t points = 0;
    std::size_t pairs = 0;
    std::size_t asymmetric = 0;
    std::size_t outside = 0;
    double largestDiagonal = 0.0;
    double largestMixed = 0.0;

    for (const std::size_t atom : {0u, 1u})
    {
        for (std::size_t radial = 0; radial < basis.radial.Size(); ++radial)
        {
            for (const std::size_t angular : {0u, 1u, 4u})
            {
                const PointId id{atom, radial, angular};
                EvaluatePointWeight(geometry, basis, id, scratch, out);

                if (out.value == 0.0 || out.activeAtoms < 2u)
                {
                    continue; // on a plateau, where every derivative is exactly zero
                }

                ASSERT_EQ(out.second.size(), width * width);

                // Audit first: only active rows and columns are non-zero, and the matrix is symmetric bit for bit.
                std::vector<char> live(atomCount, 0);

                for (const std::size_t a : out.dirty)
                {
                    live[a] = 1;
                }

                for (std::size_t index = 0; index < width; ++index)
                {
                    for (std::size_t other = 0; other < width; ++other)
                    {
                        const double entry = out.second[index * width + other];

                        if (entry != out.second[other * width + index])
                        {
                            ++asymmetric;
                        }

                        if (live[index / 3] == 0 || live[other / 3] == 0)
                        {
                            outside += (entry != 0.0) ? 1u : 0u;
                        }
                    }
                }

                // Copy the values out: the differences below reuse the buffer.
                const std::vector<double> analytic = out.second;
                const std::vector<double> analyticFirst = out.first;
                const double reference = out.value;
                const double quadrature = PointQuadrature(basis, id);
                const std::string where = label + ", atom " + std::to_string(atom) + " radial " +
                                          std::to_string(radial) + " angular " +
                                          std::to_string(angular);
                const auto weightAt = [&](const excgrid::Geometry& moved) {
                    EvaluatePointWeight(moved, basis, id, scratch, out);

                    return out.value;
                };

                for (std::size_t index = 0; index < width; ++index)
                {
                    DifferenceSamples samples;
                    samples.ahead =
                        weightAt(Displaced(geometry, index / 3, index % 3, kSecondStep));
                    samples.behind =
                        weightAt(Displaced(geometry, index / 3, index % 3, -kSecondStep));
                    samples.aheadHalf =
                        weightAt(Displaced(geometry, index / 3, index % 3, kSecondHalfStep));
                    samples.behindHalf =
                        weightAt(Displaced(geometry, index / 3, index % 3, -kSecondHalfStep));
                    samples.reference = reference;
                    samples.scale = quadrature;

                    const double analyticEntry = analytic[index * width + index];
                    ExpectMatchesSecondDifference(analyticEntry,
                                                  kSecondStep,
                                                  samples,
                                                  where + " coordinate " + std::to_string(index));
                    largestDiagonal = std::max(largestDiagonal, std::abs(analyticEntry));
                }

                for (std::size_t k = 0; k < width; ++k)
                {
                    for (std::size_t l = k + 1; l < width; ++l)
                    {
                        CrossSamples samples;
                        samples.corner[0] = weightAt(Displaced(geometry, k, l, kSecondStep, kSecondStep));
                        samples.corner[1] = weightAt(Displaced(geometry, k, l, kSecondStep, -kSecondStep));
                        samples.corner[2] = weightAt(Displaced(geometry, k, l, -kSecondStep, kSecondStep));
                        samples.corner[3] = weightAt(Displaced(geometry, k, l, -kSecondStep, -kSecondStep));
                        samples.cornerHalf[0] =
                            weightAt(Displaced(geometry, k, l, kSecondHalfStep, kSecondHalfStep));
                        samples.cornerHalf[1] =
                            weightAt(Displaced(geometry, k, l, kSecondHalfStep, -kSecondHalfStep));
                        samples.cornerHalf[2] =
                            weightAt(Displaced(geometry, k, l, -kSecondHalfStep, kSecondHalfStep));
                        samples.cornerHalf[3] =
                            weightAt(Displaced(geometry, k, l, -kSecondHalfStep, -kSecondHalfStep));
                        samples.reference = reference;
                        samples.scale = quadrature;

                        const double analyticEntry = analytic[k * width + l];
                        ExpectMatchesCrossDifference(analyticEntry,
                                                     kSecondStep,
                                                     samples,
                                                     where + " coordinates " + std::to_string(k) +
                                                         " and " + std::to_string(l));
                        largestMixed = std::max(largestMixed, std::abs(analyticEntry));
                        ++pairs;
                    }
                }

                EXPECT_EQ(analyticFirst.size(), width);
                ++points;
            }
        }
    }

    EXPECT_GT(points, 10u);
    EXPECT_GT(pairs, 10u * width);
    EXPECT_GT(largestDiagonal, 1e-6); // a fixture whose second derivatives are all zero proves nothing
    EXPECT_GT(largestMixed, 1e-6);
    EXPECT_EQ(asymmetric, 0u);
    EXPECT_EQ(outside, 0u);
}

TEST(GridDerivSecondTest, TwoAtomOnePointSecondDerivativeMatchesCentralDifference) {
    excgrid::GridParams params;
    params.radialPoints = 8;
    params.angularPoints = 6;
    auto basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());

    CheckOnePointHessian("the collinear pair", HydrogenPair(), *basis);
}

// No symmetry left: no entry vanishing by symmetry, no two distances tied.
TEST(GridDerivSecondTest, JitteredPairPointSecondDerivativeMatchesCentralDifference) {
    excgrid::GridParams params;
    params.radialPoints = 8;
    params.angularPoints = 6;
    auto basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());

    CheckOnePointHessian("the jittered pair", JitteredPair(), *basis);
}

TEST(GridDerivSecondTest, BlockSecondDerivativesMatchTheDisplacedBuild) {
    const excgrid::Geometry geometry = Triatomic();
    const excgrid::GridParams params = BlockParams();
    auto basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());
    auto grid = excgrid::BlockGrid::Create(geometry, params);
    ASSERT_TRUE(grid.has_value());

    const GridTable base = Tabulate(geometry, *basis, *grid);
    const std::size_t atomCount = geometry.atoms.size();
    const std::size_t width = 3 * atomCount;
    DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kSecond);
    PointWeightDerivatives out;

    ASSERT_GT(base.kept, 1000u);

    // Pass one: accumulate the totals' second derivatives from the per-point ones.
    std::vector<double> totalSecond(width * width, 0.0);
    std::vector<double> momentSecond(3 * width * width, 0.0);
    std::vector<double> mixedMagnitude(width * width, 0.0);
    std::size_t asymmetric = 0;
    double largestEntry = 0.0;

    for (std::size_t flat = 0; flat < base.present.size(); ++flat)
    {
        ASSERT_EQ(base.present[flat], 1) << "point " << flat << " is not in the build";

        const PointId id = FromFlat(*basis, flat);
        const std::array<double, 3> position = PointPosition(geometry, *basis, id);
        EvaluatePointWeight(geometry, *basis, id, scratch, out);

        for (std::size_t k = 0; k < width; ++k)
        {
            for (std::size_t l = 0; l < width; ++l)
            {
                const std::size_t index = k * width + l;
                const double second = out.second[index];

                asymmetric += (second != out.second[l * width + k]) ? 1u : 0u;
                totalSecond[index] += second;
                largestEntry = std::max(largestEntry, std::abs(second));

                if (k != l)
                {
                    mixedMagnitude[index] = std::max(mixedMagnitude[index], std::abs(second));
                }

                for (std::size_t row = 0; row < 3; ++row)
                {
                    // d2(M_row)/dR_k dR_l = sum_i [ d2w_i x_row + dw_i/dR_k dx_row/dR_l + dw_i/dR_l dx_row/dR_k ].
                    double term = second * position[row];

                    if (k / 3 == id.atom && k % 3 == row)
                    {
                        term += out.first[l];
                    }

                    if (l / 3 == id.atom && l % 3 == row)
                    {
                        term += out.first[k];
                    }

                    momentSecond[3 * index + row] += term;
                }
            }
        }
    }

    const std::array<double, 4> baseTotals = GridTotals(base, geometry, *basis);
    double largestDiagonal = 0.0;
    double largestTotal = 0.0;

    for (std::size_t atom = 0; atom < atomCount; ++atom)
    {
        for (std::size_t component = 0; component < 3; ++component)
        {
            const std::size_t index = 3 * atom + component;
            const std::string where =
                ", along atom " + std::to_string(atom) + " component " + std::to_string(component);
            const DisplacedGrid ahead =
                BuildDisplaced(geometry, params, *basis, atom, component, kSecondStep);
            const DisplacedGrid behind =
                BuildDisplaced(geometry, params, *basis, atom, component, -kSecondStep);
            const DisplacedGrid aheadHalf =
                BuildDisplaced(geometry, params, *basis, atom, component, kSecondHalfStep);
            const DisplacedGrid behindHalf =
                BuildDisplaced(geometry, params, *basis, atom, component, -kSecondHalfStep);
            ASSERT_TRUE(ahead.built) << where;
            ASSERT_TRUE(behind.built) << where;
            ASSERT_TRUE(aheadHalf.built) << where;
            ASSERT_TRUE(behindHalf.built) << where;

            for (std::size_t flat = 0; flat < base.present.size(); ++flat)
            {
                ASSERT_EQ(ahead.table.present[flat], 1) << "point " << flat << " is missing" << where;
                ASSERT_EQ(behind.table.present[flat], 1)
                    << "point " << flat << " is missing" << where;
                ASSERT_EQ(aheadHalf.table.present[flat], 1)
                    << "point " << flat << " is missing" << where;
                ASSERT_EQ(behindHalf.table.present[flat], 1)
                    << "point " << flat << " is missing" << where;

                const PointId id = FromFlat(*basis, flat);
                EvaluatePointWeight(geometry, *basis, id, scratch, out);

                DifferenceSamples samples;
                samples.ahead = ahead.table.weight[flat];
                samples.behind = behind.table.weight[flat];
                samples.aheadHalf = aheadHalf.table.weight[flat];
                samples.behindHalf = behindHalf.table.weight[flat];
                samples.reference = out.value;
                samples.scale = PointQuadrature(*basis, id);

                const double analytic = out.second[index * width + index];
                ExpectMatchesSecondDifference(analytic,
                                              kSecondStep,
                                              samples,
                                              "the second derivative of the weight of atom " +
                                                  std::to_string(id.atom) + " radial " +
                                                  std::to_string(id.radial) + " angular " +
                                                  std::to_string(id.angular) + where);
                largestDiagonal = std::max(largestDiagonal, std::abs(analytic));
            }

            const std::array<double, 4> aheadTotals = GridTotals(ahead.table, ahead.geometry, *basis);
            const std::array<double, 4> behindTotals =
                GridTotals(behind.table, behind.geometry, *basis);
            const std::array<double, 4> aheadHalfTotals =
                GridTotals(aheadHalf.table, aheadHalf.geometry, *basis);
            const std::array<double, 4> behindHalfTotals =
                GridTotals(behindHalf.table, behindHalf.geometry, *basis);

            for (std::size_t row = 0; row < 4; ++row)
            {
                DifferenceSamples samples;
                samples.ahead = aheadTotals[row];
                samples.behind = behindTotals[row];
                samples.aheadHalf = aheadHalfTotals[row];
                samples.behindHalf = behindHalfTotals[row];
                samples.reference = baseTotals[row];
                samples.scale = baseTotals[row];
                const double analytic =
                    (row == 0) ? totalSecond[index * width + index]
                               : momentSecond[3 * (index * width + index) + row - 1];
                const std::string label =
                    (row == 0) ? std::string("the weight sum, second derivative")
                               : "the second derivative of moment row " + std::to_string(row - 1);

                ExpectMatchesSecondDifference(analytic, kSecondStep, samples, label + where);
                largestTotal = std::max(largestTotal, std::abs(analytic));
            }
        }
    }

    // Pass three: mixed entries against the four corners of a two-coordinate displacement.
    std::size_t sameAtomPairs = 0;
    std::size_t crossAtomPairs = 0;
    double largestMixed = 0.0;
    double largestMixedTotal = 0.0;

    for (std::size_t k = 0; k < width; ++k)
    {
        for (std::size_t l = k + 1; l < width; ++l)
        {
            if (mixedMagnitude[k * width + l] == 0.0)
            {
                continue; // the fixture gives this pair nothing to check
            }

            sameAtomPairs += (k / 3 == l / 3) ? 1u : 0u;
            crossAtomPairs += (k / 3 != l / 3) ? 1u : 0u;
            const std::string where =
                " coordinates " + std::to_string(k) + " and " + std::to_string(l);
            std::array<std::array<double, 4>, 4> cornerTotals = {};
            std::array<std::array<double, 4>, 4> cornerHalfTotals = {};

            for (std::size_t flat = 0; flat < base.present.size(); ++flat)
            {
                const PointId id = FromFlat(*basis, flat);
                const excgrid::Geometry corners[4] = {
                    Displaced(geometry, k, l, kSecondStep, kSecondStep),
                    Displaced(geometry, k, l, kSecondStep, -kSecondStep),
                    Displaced(geometry, k, l, -kSecondStep, kSecondStep),
                    Displaced(geometry, k, l, -kSecondStep, -kSecondStep)};
                const excgrid::Geometry halves[4] = {
                    Displaced(geometry, k, l, kSecondHalfStep, kSecondHalfStep),
                    Displaced(geometry, k, l, kSecondHalfStep, -kSecondHalfStep),
                    Displaced(geometry, k, l, -kSecondHalfStep, kSecondHalfStep),
                    Displaced(geometry, k, l, -kSecondHalfStep, -kSecondHalfStep)};
                EvaluatePointWeight(geometry, *basis, id, scratch, out);
                const double reference = out.value;
                const double analytic = out.second[k * width + l];
                const double quadrature = PointQuadrature(*basis, id);
                const auto weightAt = [&](const excgrid::Geometry& moved) {
                    EvaluatePointWeight(moved, *basis, id, scratch, out);

                    return out.value;
                };

                CrossSamples samples;

                for (std::size_t corner = 0; corner < 4; ++corner)
                {
                    samples.corner[corner] = weightAt(corners[corner]);
                    samples.cornerHalf[corner] = weightAt(halves[corner]);
                    const std::array<double, 3> position =
                        PointPosition(corners[corner], *basis, id);
                    const std::array<double, 3> halfPosition =
                        PointPosition(halves[corner], *basis, id);
                    cornerTotals[corner][0] += samples.corner[corner];
                    cornerHalfTotals[corner][0] += samples.cornerHalf[corner];

                    for (std::size_t row = 0; row < 3; ++row)
                    {
                        cornerTotals[corner][row + 1] += samples.corner[corner] * position[row];
                        cornerHalfTotals[corner][row + 1] +=
                            samples.cornerHalf[corner] * halfPosition[row];
                    }
                }

                samples.reference = reference;
                samples.scale = quadrature;
                ExpectMatchesCrossDifference(analytic,
                                             kSecondStep,
                                             samples,
                                             "the mixed second derivative of the weight of atom " +
                                                 std::to_string(id.atom) + " radial " +
                                                 std::to_string(id.radial) + " angular " +
                                                 std::to_string(id.angular) + where);
                largestMixed = std::max(largestMixed, std::abs(analytic));
            }

            for (std::size_t row = 0; row < 4; ++row)
            {
                CrossSamples samples;

                for (std::size_t corner = 0; corner < 4; ++corner)
                {
                    samples.corner[corner] = cornerTotals[corner][row];
                    samples.cornerHalf[corner] = cornerHalfTotals[corner][row];
                }

                samples.reference = baseTotals[row];
                samples.scale = baseTotals[row];
                const double analytic = (row == 0)
                                            ? totalSecond[k * width + l]
                                            : momentSecond[3 * (k * width + l) + row - 1];
                const std::string label =
                    (row == 0) ? std::string("the weight sum, mixed second derivative")
                               : "the mixed second derivative of moment row " + std::to_string(row - 1);

                ExpectMatchesCrossDifference(analytic, kSecondStep, samples, label + where);
                largestMixedTotal = std::max(largestMixedTotal, std::abs(analytic));
            }
        }
    }

    // The fixture must have content on same-atom and cross-atom pairs.
    EXPECT_GT(largestEntry, 1e-6);
    EXPECT_GT(largestDiagonal, 1e-6);
    EXPECT_GT(largestTotal, 1e-3);
    EXPECT_GT(largestMixed, 1e-6);
    EXPECT_GT(largestMixedTotal, 1e-6);
    EXPECT_GT(sameAtomPairs, 0u);
    EXPECT_GT(crossAtomPairs, 0u);
    EXPECT_EQ(asymmetric, 0u);
}


/// The smooth field the acceptance integrates: a Gaussian of variance `width`,
/// f = exp(-|x|^2 / width), grad f = -2 x f / width, d2f = (4 x x^T / width^2 - 2 I / width) f.
struct TestField {
    double width = 2.5; ///< The Gaussian's variance, in Bohr^2.

    [[nodiscard]] double Value(const std::array<double, 3>& x) const {
        return std::exp(-(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]) / width);
    }

    [[nodiscard]] std::array<double, 3> Gradient(const std::array<double, 3>& x) const {
        const double value = Value(x);

        return {-2.0 * x[0] * value / width,
                -2.0 * x[1] * value / width,
                -2.0 * x[2] * value / width};
    }

    [[nodiscard]] std::array<double, 9> Curvature(const std::array<double, 3>& x) const {
        const double value = Value(x);
        std::array<double, 9> out = {};

        for (std::size_t row = 0; row < 3; ++row)
        {
            for (std::size_t column = 0; column < 3; ++column)
            {
                out[3 * row + column] = 4.0 * x[row] * x[column] * value / (width * width);

                if (row == column)
                {
                    out[3 * row + column] -= 2.0 * value / width;
                }
            }
        }

        return out;
    }
};

/// The energy of a fresh build, independent of the derivative pass; a failed build counts and contributes zero.
double EnergyOfBuild(const excgrid::Geometry& geometry,
                     const excgrid::GridParams& params,
                     const TestField& field,
                     std::size_t& failed) {
    auto grid = excgrid::BlockGrid::Create(geometry, params);

    if (!grid)
    {
        ++failed;

        return 0.0;
    }

    double energy = 0.0;

    for (const excgrid::Block& block : grid->Blocks())
    {
        for (std::size_t index = 0; index < block.pointCount; ++index)
        {
            energy += block.weights[index] * field.Value(block.points[index]);
        }
    }

    return energy;
}

double EnergyAt(const excgrid::Geometry& geometry,
                const excgrid::GridParams& params,
                const TestField& field,
                std::size_t atom,
                std::size_t component,
                double amount,
                std::size_t& failed) {
    return EnergyOfBuild(Displaced(geometry, atom, component, amount), params, field, failed);
}

double EnergyAt(const excgrid::Geometry& geometry,
                const excgrid::GridParams& params,
                const TestField& field,
                std::size_t firstCoordinate,
                std::size_t secondCoordinate,
                double firstAmount,
                double secondAmount,
                std::size_t& failed) {
    const excgrid::Geometry moved =
        Displaced(geometry, firstCoordinate, secondCoordinate, firstAmount, secondAmount);

    return EnergyOfBuild(moved, params, field, failed);
}

// E(R) = sum_g w_g(R) f(x_g(R)); d2E/dR_b dR_c = sum_g d2w_g f_g + [owner==c] sum_g dw_g/dR_b (grad f)_m + [owner==b] sum_g dw_g/dR_c (grad f)_l + [b==c] sum_{owner b} w_g d2f_lm.
TEST(GridDerivSecondTest, GridMotionTermMatchesEnergiesAtDisplacedGeometries) {
    const excgrid::Geometry geometry = Triatomic();
    const excgrid::GridParams params = BlockParams();
    const TestField field;
    auto basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());

    const std::size_t atomCount = geometry.atoms.size();
    const std::size_t width = 3 * atomCount;
    DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kSecond);
    PointWeightDerivatives out;
    std::size_t failed = 0;

    const double reference = EnergyOfBuild(geometry, params, field, failed);
    ASSERT_EQ(failed, 0u);
    ASSERT_GT(std::abs(reference), 1e-3);

    std::vector<double> withSecond(width * width, 0.0);
    std::vector<double> withoutSecond(width * width, 0.0);
    double analyticEnergy = 0.0;
    double analyticScale = 0.0;

    for (std::size_t flat = 0; flat < FlatCount(geometry, *basis); ++flat)
    {
        const PointId id = FromFlat(*basis, flat);
        EvaluatePointWeight(geometry, *basis, id, scratch, out);

        const std::array<double, 3> position = PointPosition(geometry, *basis, id);
        const double value = field.Value(position);
        const std::array<double, 3> gradient = field.Gradient(position);
        const std::array<double, 9> curvature = field.Curvature(position);
        const std::size_t owner = id.atom;

        analyticEnergy += out.value * value;
        analyticScale += std::abs(out.value * value);

        // (a) the weights' own second derivative, on every entry the point's active atoms name.
        for (std::size_t row = 0; row < width; ++row)
        {
            for (std::size_t column = 0; column < width; ++column)
            {
                withSecond[row * width + column] += out.second[row * width + column] * value;
                withoutSecond[row * width + column] += 0.0;
            }
        }

        for (std::size_t row = 0; row < width; ++row)
        {
            for (std::size_t k = 0; k < 3; ++k)
            {
                withSecond[row * width + 3 * owner + k] += out.first[row] * gradient[k];
                withSecond[(3 * owner + k) * width + row] += out.first[row] * gradient[k];
                withoutSecond[row * width + 3 * owner + k] += out.first[row] * gradient[k];
                withoutSecond[(3 * owner + k) * width + row] += out.first[row] * gradient[k];
            }
        }

        for (std::size_t row = 0; row < 3; ++row)
        {
            for (std::size_t column = 0; column < 3; ++column)
            {
                const double frozen = out.value * curvature[3 * row + column];
                withSecond[(3 * owner + row) * width + 3 * owner + column] += frozen;
                withoutSecond[(3 * owner + row) * width + 3 * owner + column] += frozen;
            }
        }
    }

    EXPECT_NEAR(analyticEnergy, reference, 1e-12 * analyticScale);

    std::size_t entries = 0;
    std::size_t moved = 0;
    std::size_t missed = 0;
    double largest = 0.0;
    double worstMiss = 0.0;

    for (std::size_t k = 0; k < width; ++k)
    {
        DifferenceSamples samples;
        samples.ahead = EnergyAt(geometry, params, field, k / 3, k % 3, kSecondStep, failed);
        samples.behind = EnergyAt(geometry, params, field, k / 3, k % 3, -kSecondStep, failed);
        samples.aheadHalf = EnergyAt(geometry, params, field, k / 3, k % 3, kSecondHalfStep, failed);
        samples.behindHalf =
            EnergyAt(geometry, params, field, k / 3, k % 3, -kSecondHalfStep, failed);
        samples.reference = reference;
        samples.scale = analyticScale;

        const double analyticEntry = withSecond[k * width + k];
        const std::string label =
            "the grid-motion term of nuclear coordinate " + std::to_string(k);

        ExpectMatchesSecondDifference(analyticEntry, kSecondStep, samples, label);

        const double difference = SecondDifference(samples, kSecondStep);
        const double tolerance = SecondDifferenceTolerance(samples, kSecondStep);
        const double gap = std::abs(withoutSecond[k * width + k] - difference);

        moved += (std::abs(analyticEntry - withoutSecond[k * width + k]) > 100.0 * tolerance) ? 1u
                                                                                              : 0u;
        missed += (gap > tolerance) ? 1u : 0u;
        largest = std::max(largest, std::abs(analyticEntry));
        worstMiss = std::max(worstMiss, gap / tolerance);
        ++entries;

        for (std::size_t l = k + 1; l < width; ++l)
        {
            CrossSamples corner;
            corner.corner[0] =
                EnergyAt(geometry, params, field, k, l, kSecondStep, kSecondStep, failed);
            corner.corner[1] =
                EnergyAt(geometry, params, field, k, l, kSecondStep, -kSecondStep, failed);
            corner.corner[2] =
                EnergyAt(geometry, params, field, k, l, -kSecondStep, kSecondStep, failed);
            corner.corner[3] =
                EnergyAt(geometry, params, field, k, l, -kSecondStep, -kSecondStep, failed);
            corner.cornerHalf[0] =
                EnergyAt(geometry, params, field, k, l, kSecondHalfStep, kSecondHalfStep, failed);
            corner.cornerHalf[1] =
                EnergyAt(geometry, params, field, k, l, kSecondHalfStep, -kSecondHalfStep, failed);
            corner.cornerHalf[2] =
                EnergyAt(geometry, params, field, k, l, -kSecondHalfStep, kSecondHalfStep, failed);
            corner.cornerHalf[3] =
                EnergyAt(geometry, params, field, k, l, -kSecondHalfStep, -kSecondHalfStep, failed);
            corner.reference = reference;
            corner.scale = analyticScale;

            const double analyticMixed = withSecond[k * width + l];
            const std::string mixedLabel = "the grid-motion term of nuclear coordinates " +
                                           std::to_string(k) + " and " + std::to_string(l);

            ExpectMatchesCrossDifference(analyticMixed, kSecondStep, corner, mixedLabel);

            const double crossDifference = CrossDifference(corner, kSecondStep);
            const double crossTolerance = CrossDifferenceTolerance(corner, kSecondStep);
            const double crossGap = std::abs(withoutSecond[k * width + l] - crossDifference);

            moved +=
                (std::abs(analyticMixed - withoutSecond[k * width + l]) > 100.0 * crossTolerance)
                    ? 1u
                    : 0u;
            missed += (crossGap > crossTolerance) ? 1u : 0u;
            largest = std::max(largest, std::abs(analyticMixed));
            worstMiss = std::max(worstMiss, crossGap / crossTolerance);
            ++entries;
        }
    }

    EXPECT_EQ(failed, 0u) << "a displaced build did not succeed";
    EXPECT_GT(entries, width * width / 2u);
    EXPECT_GT(largest, 1e-6);

    // The term must move entries, and dropping it must miss by far more than the tolerance.
    EXPECT_GT(moved, entries / 2u)
        << "the weights' second derivative moves only " << moved << " of " << entries
        << " entries: this fixture cannot tell its presence from its absence";
    EXPECT_GT(missed, 0u) << "removing the weights' second derivative left every entry inside the "
                             "difference's own tolerance";
    EXPECT_GT(worstMiss, 100.0)
        << "dropping the weights' second derivative misses the difference by only " << worstMiss
        << " times its tolerance, so the agreement above would have been obtained without it";
}


/// The whole-grid fixture with the production trim switched on.
excgrid::GridParams TrimmedParams() {
    excgrid::GridParams params = BlockParams();
    params.trimWeight = 1e-15;

    return params;
}

[[nodiscard]] bool SameIdentities(const GridTable& base, const GridTable& moved) {
    if (base.present.size() != moved.present.size())
    {
        return false;
    }

    for (std::size_t flat = 0; flat < base.present.size(); ++flat)
    {
        if (base.present[flat] != moved.present[flat])
        {
            return false;
        }
    }

    return true;
}

[[nodiscard]] std::string FirstIdentityDifference(const GridBasis& basis,
                                                 const GridTable& base,
                                                 const GridTable& moved) {
    for (std::size_t flat = 0; flat < base.present.size(); ++flat)
    {
        if (base.present[flat] != moved.present[flat])
        {
            const PointId id = FromFlat(basis, flat);

            return "atom " + std::to_string(id.atom) + " radial " + std::to_string(id.radial) +
                   " angular " + std::to_string(id.angular) +
                   ((moved.present[flat] != 0) ? " appeared" : " vanished");
        }
    }

    return "none";
}

// The identity set must be the same at every displaced geometry; a point crossing the trim threshold would make the sum's derivative uncomputable.
TEST(GridDerivSecondTest, TrimmedGridKeepsItsIdentitySetAtTheDerivativeSteps) {
    const excgrid::Geometry geometry = Triatomic();
    const excgrid::GridParams params = TrimmedParams();
    auto basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());
    auto grid = excgrid::BlockGrid::Create(geometry, params);
    ASSERT_TRUE(grid.has_value());

    const GridTable base = Tabulate(geometry, *basis, *grid);
    DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kFirst);
    PointWeightDerivatives out;

    const std::size_t all = FlatCount(geometry, *basis);
    ASSERT_LT(base.kept, all);

    std::size_t dropped = 0;
    double smallestKept = std::numeric_limits<double>::infinity();
    double largestDropped = 0.0;

    for (std::size_t flat = 0; flat < all; ++flat)
    {
        const PointId id = FromFlat(*basis, flat);
        EvaluatePointWeight(geometry, *basis, id, scratch, out);

        if (base.present[flat] == 0)
        {
            // Dropped points carry a weight of exactly zero, so the trim is neutral here.
            EXPECT_EQ(out.value, 0.0) << "a dropped point carries a non-zero weight";
            largestDropped = std::max(largestDropped, std::abs(out.value));
            ++dropped;
        }
        else
        {
            smallestKept = std::min(smallestKept, std::abs(out.value));
        }
    }

    EXPECT_EQ(dropped, all - base.kept);
    EXPECT_EQ(largestDropped, 0.0);
    EXPECT_GT(base.kept, all / 2u);
    EXPECT_GT(smallestKept, 100.0 * params.trimWeight)
        << "the lightest weight this build keeps is " << smallestKept << ", only "
        << smallestKept / params.trimWeight
        << " times the threshold: a displacement can push it out";

    std::size_t builds = 0;

    for (std::size_t atom = 0; atom < geometry.atoms.size(); ++atom)
    {
        for (std::size_t component = 0; component < 3; ++component)
        {
            for (const double amount : {kStep, -kStep, kSecondStep, -kSecondStep})
            {
                const DisplacedGrid moved =
                    BuildDisplaced(geometry, params, *basis, atom, component, amount);
                const std::string where = ", displaced by " + std::to_string(amount) +
                                          " along atom " + std::to_string(atom) + " component " +
                                          std::to_string(component);

                ASSERT_TRUE(moved.built) << where;
                EXPECT_TRUE(SameIdentities(base, moved.table))
                    << "a point switched in or out of the trimmed grid" << where << ": "
                    << FirstIdentityDifference(*basis, base, moved.table);
                EXPECT_EQ(moved.table.kept, base.kept) << where;
                ++builds;
            }
        }
    }

    EXPECT_EQ(builds, 4u * 3u * geometry.atoms.size());
}

// The switch is reachable at 1e-2: a point the base grid drops carries 3.1e-6 there.
TEST(GridDerivSecondTest, TheTrimCanDropAPointFarAboveItsThreshold) {
    const excgrid::Geometry geometry = Triatomic();
    const excgrid::GridParams params = TrimmedParams();
    constexpr double kCoarseStep = 1e-2;
    auto basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());
    auto grid = excgrid::BlockGrid::Create(geometry, params);
    ASSERT_TRUE(grid.has_value());

    const GridTable base = Tabulate(geometry, *basis, *grid);
    DerivativeScratch scratch = MakeDerivativeScratch(geometry, DerivativeOrder::kFirst);
    PointWeightDerivatives out;
    std::size_t switches = 0;
    std::size_t appeared = 0;
    std::size_t vanished = 0;
    double lightestVanished = std::numeric_limits<double>::infinity();

    for (std::size_t atom = 0; atom < geometry.atoms.size(); ++atom)
    {
        for (std::size_t component = 0; component < 3; ++component)
        {
            for (const double amount : {kCoarseStep, -kCoarseStep})
            {
                const DisplacedGrid moved =
                    BuildDisplaced(geometry, params, *basis, atom, component, amount);
                ASSERT_TRUE(moved.built);

                if (SameIdentities(base, moved.table))
                {
                    continue;
                }

                ++switches;

                for (std::size_t flat = 0; flat < base.present.size(); ++flat)
                {
                    if (base.present[flat] == 0 && moved.table.present[flat] != 0)
                    {
                        ++appeared;
                    }

                    if (base.present[flat] != 0 && moved.table.present[flat] == 0)
                    {
                        ++vanished;
                        EvaluatePointWeight(
                            geometry, *basis, FromFlat(*basis, flat), scratch, out);
                        lightestVanished = std::min(lightestVanished, std::abs(out.value));
                    }
                }
            }
        }
    }

    EXPECT_GT(switches, 0u) << "the coarse sweep crossed nothing at all: this fixture cannot "
                               "show that the trimmed set depends on the geometry";
    EXPECT_GT(appeared, 0u);
    EXPECT_GT(vanished, 0u);

    // A point that enters crosses just above the threshold; one that leaves was kept far above it.
    EXPECT_GT(lightestVanished, 1000.0 * params.trimWeight)
        << "the lightest point this trim drops carries " << lightestVanished << ", only "
        << lightestVanished / params.trimWeight << " times the threshold";
}

/// Four identical atoms at alternate cube corners: every distance tied, so ordering has no natural answer.
excgrid::Geometry SymmetricCluster() {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({1, {1.0, 1.0, 1.0}});
    geometry.atoms.push_back({1, {1.0, -1.0, -1.0}});
    geometry.atoms.push_back({1, {-1.0, 1.0, -1.0}});
    geometry.atoms.push_back({1, {-1.0, -1.0, 1.0}});

    return geometry;
}

// Drives Displaced's ordering assertions over a fixture built to invite a reordering tie.
TEST(GridDerivSecondTest, ASymmetricFixtureKeepsItsAtomOrderUnderADisplacement) {
    const excgrid::Geometry geometry = SymmetricCluster();
    const excgrid::GridParams params = TrimmedParams();
    auto basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());
    auto grid = excgrid::BlockGrid::Create(geometry, params);
    ASSERT_TRUE(grid.has_value());

    const GridTable base = Tabulate(geometry, *basis, *grid);
    std::size_t displaced = 0;

    for (std::size_t atom = 0; atom < geometry.atoms.size(); ++atom)
    {
        for (std::size_t component = 0; component < 3; ++component)
        {
            for (const double amount : {kStep, -kStep, kSecondStep, -kSecondStep, 1e-2, -1e-2})
            {
                const excgrid::Geometry moved = Displaced(geometry, atom, component, amount);
                ASSERT_EQ(moved.atoms.size(), geometry.atoms.size());
                std::size_t changed = 0;

                for (std::size_t index = 0; index < moved.atoms.size(); ++index)
                {
                    EXPECT_EQ(moved.atoms[index].atomicNumber, geometry.atoms[index].atomicNumber)
                        << "atom " << index << " changed element under a displacement";

                    for (std::size_t k = 0; k < 3; ++k)
                    {
                        const double expected = geometry.atoms[index].position[k] +
                                                ((index == atom && k == component) ? amount : 0.0);

                        EXPECT_DOUBLE_EQ(moved.atoms[index].position[k], expected)
                            << "atom " << index << " component " << k
                            << " is not where the one requested displacement puts it";

                        if (moved.atoms[index].position[k] != geometry.atoms[index].position[k])
                        {
                            ++changed;
                        }
                    }
                }

                EXPECT_EQ(changed, 1u)
                    << "the displacement moved " << changed << " coordinates, not one";

                // And the built grid keeps the same points at the difference steps: no point switches in or out.
                if (std::abs(amount) <= kSecondStep)
                {
                    const DisplacedGrid built =
                        BuildDisplaced(geometry, params, *basis, atom, component, amount);

                    ASSERT_TRUE(built.built);
                    EXPECT_TRUE(SameIdentities(base, built.table))
                        << "a point switched in or out of the symmetric fixture: "
                        << FirstIdentityDifference(*basis, base, built.table);
                }

                ++displaced;
            }
        }
    }

    EXPECT_EQ(displaced, 6u * 3u * geometry.atoms.size());
}

} // namespace
