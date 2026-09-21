// The grid's derivatives with respect to nuclear position, checked against
// central finite differences of the same quantities.
//
// The finite difference is taken on the WEIGHT THE BUILD WOULD HAND OUT, not on
// a formula beside it: a displacement moves the owning atom, the point carries
// with it (a grid point is that atom's own radial point times its own angular
// node), and the weight is re-evaluated at the displaced geometry.  The
// analytic derivative is the total that includes the point's own motion, which
// is what a gradient or Hessian assembly consumes.
//
// Every fixture here is small on purpose: the second-derivative checks are
// O((3N)^2) evaluations, and a small system is what proves the algebra before a
// production-size one is measured.

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

// The step the central differences run at.  Errors are O(h^2) truncation plus
// O(eps / h) round-off; at h = 1e-5 both terms sit near 1e-10 for a weight of
// order one, which is the tolerance the pair checks below are written to.
constexpr double kStep = 1e-5;

// The same difference at half the step.  The two steps together say whether the
// disagreement between an analytic derivative and a difference is the
// difference's own error or the analytic value's, which a fixed tolerance
// cannot - see ExpectMatchesDifference.
constexpr double kHalfStep = 0.5 * kStep;

// The step the SECOND differences run at, a hundred times the first difference's.
// A second difference's round-off scales as value / h^2, so its optimal step is
// far larger: at kStep a whole-grid check of a point whose quadrature factor is
// 1e4 would carry a round-off floor of order one, on top of a second derivative
// whose own magnitude is that factor times the curvature of a shell tens of Bohr
// out.  The step is still small enough that no cell leaves its live set.
constexpr double kSecondStep = 1e-3;
constexpr double kSecondHalfStep = 0.5 * kSecondStep;

// A displacement small enough that no switch leaves its branch and no cell
// changes its live set, but not so small that the difference loses digits.
constexpr double kDerivativeTolerance = 1e-8;

excgrid::Geometry HydrogenPair() {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({1, {0.0, 0.0, -0.7}});
    geometry.atoms.push_back({1, {0.0, 0.0, 0.7}});

    return geometry;
}

// The same pair, distorted by fixed amounts that take every symmetry and every
// exact tie out of the arrangement: no coordinate of either atom is zero, no
// two coordinates are equal, and the two atoms are not mirror images of each
// other.  The bond still runs along z, so the window arithmetic the pair's
// point cases rely on is unchanged.
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

/// A chain of \p atomCount hydrogen atoms 2.4 Bohr apart, jittered so that
/// nothing in it is symmetric.  A geometry of a chosen size: the scratch's
/// allocation has to match the stated formula at every atom count, not only at
/// the two the fixtures above happen to carry.
excgrid::Geometry ChainOfAtoms(std::size_t atomCount) {
    excgrid::Geometry geometry;

    for (std::size_t index = 0; index < atomCount; ++index)
    {
        const double jitter = 0.013 * static_cast<double>(index % 7) - 0.05;
        geometry.atoms.push_back({1, {2.4 * static_cast<double>(index), jitter, -0.5 * jitter}});
    }

    return geometry;
}

// One point of the product grid: an offset in the owning atom's own frame.  The
// point therefore moves with its owner, which is what the total derivative
// (3) is defined against.
struct PointCase {
    std::size_t owner;
    std::array<double, 3> offset;
    std::string label;
};

// The live cases: on the 1.4 Bohr bond of the fixture, a point has both cells
// off their plateaus exactly when the two distances differ by less than
// a * (rad_0 + rad_1) = 0.85 Bohr, so every offset below sits between the two
// atoms or shadowed by one of them.  An offset beyond an atom puts the far cell
// on the zero plateau; that case has a test of its own.
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

// The partition weight of the owning atom's cell at the attached point.
double WeightAt(const excgrid::Geometry& geometry,
                const PointCase& pointCase,
                DerivativeScratch& scratch,
                PointWeightDerivatives& out) {
    EvaluatePartitionWeight(
        geometry, AttachedPoint(geometry, pointCase), pointCase.owner, scratch, out);

    return out.value;
}

// A displaced geometry.  Every difference below compares derivatives named by
// an ATOM INDEX, so the comparison is only meaningful while an atom keeps its
// index across the displacement: a geometry whose ordering changed would have
// the difference compare one atom's derivative against another's, and it would
// fail - or worse, agree - for reasons that have nothing to do with the
// derivative.  The check costs one comparison over the atoms and is the reason
// no fixture here can silently reorder: it also stands as the library's own
// contract ("A molecular geometry: atoms in input order", include/excgrid/
// geometry.hpp) being exercised rather than assumed.
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

// The same, displaced in two coordinates at once - one corner of a cross
// difference.  The coordinates are named the way the derivative arrays name
// them, index 3 * atom + component.  Each half goes through the checked
// overload above, so a corner carries the same ordering guarantee a single
// displacement does.
excgrid::Geometry Displaced(const excgrid::Geometry& geometry,
                            std::size_t firstCoordinate,
                            std::size_t secondCoordinate,
                            double firstAmount,
                            double secondAmount) {
    const excgrid::Geometry once =
        Displaced(geometry, firstCoordinate / 3, firstCoordinate % 3, firstAmount);

    return Displaced(once, secondCoordinate / 3, secondCoordinate % 3, secondAmount);
}

// The analytic first derivative against a central difference of the value, for
// every nuclear coordinate of the fixture.
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

    // A fixture whose derivatives are all zero would pass vacuously.
    EXPECT_GT(largest, 1e-3) << pointCase.label << " has no live pair";
}

/// Four evaluations of one quantity, along one nuclear coordinate: a central
/// difference's two sides, and the same pair at half the step.
///
/// The pair fixtures above carry unit-scale weights and can be checked against
/// a fixed tolerance.  A whole product grid cannot: its quadrature factor spans
/// nine orders of magnitude - the outermost radial shell of the block fixture
/// carries q = 3.0e7 - and a difference of a large value carries a
/// proportionally large round-off, so no single absolute or relative tolerance
/// serves every point.  The samples therefore carry, beside the four values,
/// the scale the difference's round-off rides on.
struct DifferenceSamples {
    double ahead = 0.0; ///< The quantity at +kStep.
    double behind = 0.0; ///< The quantity at -kStep.
    double aheadHalf = 0.0; ///< The quantity at +kHalfStep.
    double behindHalf = 0.0; ///< The quantity at -kHalfStep.
    double reference = 0.0; ///< The quantity at the undisplaced geometry.
    double scale = 0.0; ///< The magnitude the difference's round-off rides on.
};

// The multiple of the two steps' gap the analytic value may sit outside.  A
// central difference's truncation is three quarters of that gap, so anything
// beyond a few times it is not the difference's own error.
constexpr double kGapMultiple = 8.0;

// The difference's round-off, in ulp of `scale`.  A difference of values of
// magnitude s carries an error of order eps * s, and dividing by 2h magnifies
// it; the factor beyond the ulp covers the conditioning of the distance a
// weight is built from - a square root of a sum of squares of coordinates up to
// 288 Bohr, whose difference between two atoms is the switch argument.  It is
// measured, not fitted: the two extreme points of the block fixture need 37 and
// 0.4 of it respectively.
constexpr double kRoundOffUlp = 64.0;

// What a difference at this step resolves as a fraction of the derivative: six
// digits, which is what a central difference at h = 1e-5 with a well-scaled
// value delivers.  An analytic derivative that is wrong is wrong by far more.
constexpr double kResolution = 1e-6;

// Checks an analytic derivative against a DifferenceSamples set.
//
// Three things bound what a difference can be held to, and all three are in
// the tolerance: its truncation, which the gap between the two steps measures;
// its round-off, which scales with the value the steps subtract; and the
// resolution the step itself delivers.  A fixed tolerance cannot say this for
// a product grid, because a difference of the outer shell's 1e7-sized weight
// is worth about 1e-2 however small the derivative beside it is - the check
// holds the analytic value to what the difference is worth, and no more.
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

// Checks an analytic second derivative against the second difference of the
// same quantity: (f(+h) - 2 f(0) + f(-h)) / h^2, at h and at h/2.
//
// The three bounds are the same three - truncation through the gap, round-off
// through the value, resolution through the difference - with the step's square
// in place of the step, which is why a second difference needs a far larger step
// than a first one to resolve the same number of digits.
[[nodiscard]] double SecondDifference(const DifferenceSamples& samples, double step) {
    return (samples.ahead - 2.0 * samples.reference + samples.behind) / (step * step);
}

/// The tolerance the second difference above can be held to.  It is a function
/// of its own because the acceptance has to hold a WRONG analytic derivative to
/// the same bound the right one is held to, and to say by what multiple it
/// misses.
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

// The four corners of a two-coordinate difference, at both steps: the quantity
// at (+h, +h), (+h, -h), (-h, +h) and (-h, -h) for the first four, and the same
// four at h/2 for the rest.  A single-coordinate displacement cannot reach a
// mixed second derivative, and this is what does.
struct CrossSamples {
    std::array<double, 4> corner = {0.0, 0.0, 0.0, 0.0}; ///< The four at the step.
    std::array<double, 4> cornerHalf = {0.0, 0.0, 0.0, 0.0}; ///< The four at half of it.
    double reference = 0.0; ///< The quantity at the undisplaced geometry.
    double scale = 0.0; ///< The magnitude the difference's round-off rides on.
};

// Checks an analytic mixed second derivative against the four-corner cross
// difference of the two coordinates it belongs to, at two steps.
[[nodiscard]] double CrossDifference(const CrossSamples& samples, double step) {
    return (samples.corner[0] - samples.corner[1] - samples.corner[2] + samples.corner[3]) /
           (4.0 * step * step);
}

/// The tolerance of the cross difference above, for the same reasons and on the
/// same terms as the second difference's.
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

// The single-atom limit: the partition is one at every point, so the weight is
// exactly one and every derivative is exactly zero.  A missing or doubled
// normalization term breaks this before it breaks anything with a partner.
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
        // Both cells are live on every case above, so the fixture is exercising
        // the pair term rather than a plateau.
        EvaluatePartitionWeight(
            geometry, AttachedPoint(geometry, pointCase), pointCase.owner, scratch, out);
        ASSERT_EQ(out.activeAtoms, 2u) << pointCase.label;
        CheckFirstDerivativesAgainstDifference(geometry, pointCase, scratch, out);
    }
}

// A point past the partner's switch window has one cell on each plateau: the
// owning or the partner cell is exactly one and the other exactly zero, so
// nothing about the weight depends on the geometry and every derivative is
// exactly zero - bitwise, not to a tolerance.
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

// Translating the whole molecule translates the point with it, so the weight is
// unchanged: every component of the derivative must sum to zero over the atoms.
// A missing delta_ba term in the total (3) breaks nothing else these tests
// check.
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

// The heteroatom adjustment: two species with different radii, so the pair
// radius is not twice one radius and the radius table is on the path the
// derivative takes.
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

// The scratch's stated per-point footprint and its actual allocation are the
// same number, so the stated working-set figure cannot drift away from the
// arrays it describes.
//
// That figure is the one F1.7 was asked for: measured, the scratch holds
// 21 N + 2 doubles - 168 N + 16 bytes - which is 352 bytes at 2 atoms, 5.4 kB
// at 32 and 21.5 kB at 128.  It is linear in the atom count and is not what the
// contract has to worry about: the caller's own array is the (3N)^2 second
// derivative, which is 73 kB per point at 32 atoms and 18 MB at 500.  The
// identity below is checked at several counts rather than at one, because a
// single count cannot tell a linear allocation from an accident of rounding.
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

// ---------------------------------------------------------------------------
// The product grid: identity, the un-trimmed enumeration, and the table of what
// a built grid actually holds.
// ---------------------------------------------------------------------------

// The number of points an un-trimmed build would generate.
std::size_t FlatCount(const excgrid::Geometry& geometry, const GridBasis& basis) {
    return geometry.atoms.size() * basis.radial.Size() * basis.angular.Size();
}

// A stable name for an identity, atom-major: any fixed order would do, and this
// one mirrors the one the build enumerates in.
std::size_t FlatIndex(const GridBasis& basis, const PointId& id) {
    return (id.atom * basis.radial.Size() + id.radial) * basis.angular.Size() + id.angular;
}

PointId FromFlat(const GridBasis& basis, std::size_t flat) {
    const std::size_t angular = flat % basis.angular.Size();
    const std::size_t radial = (flat / basis.angular.Size()) % basis.radial.Size();

    return {flat / (basis.angular.Size() * basis.radial.Size()), radial, angular};
}

// The identity of a point the build handed out, recovered from its own position:
// the radial point whose radius matches the distance to the owning atom, and the
// angular node the point lies along.
//
// The recovery is exact - the build computes the position the same way
// PointPosition does - so a wrong recovery is an assertion failure and never a
// silent skip.  The runner-up gaps are checked so that a point lying between two
// nodes cannot be quietly attributed to the wrong one.
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

// What a built grid holds, indexed by the un-trimmed enumeration: the weight of
// every point that survived the trim, and `present` marking which those are.
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

// A displaced geometry and the grid it builds, tabulated by identity.  A failed
// build is reported by the caller: this helper has no voice of its own, and a
// grid that did not build must stop the test rather than be read as zeros.
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

// The grid's totals: the weight sum, then the three first moments.  Both are
// accumulated here rather than per point, because both are what a consumer
// accumulates and neither is a per-point quantity.
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

// A three-atom fixture for the whole-grid checks.  It is deliberately NOT
// symmetric: a molecule with a mirror plane makes some of the grid's totals
// vanish there exactly, and a central difference of a quantity that is zero by
// symmetry measures the round-off of the terms that cancel, not the derivative.
excgrid::Geometry Triatomic() {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({8, {0.0, 0.0, 0.0}});
    geometry.atoms.push_back({1, {0.24, 1.43, 1.11}});
    geometry.atoms.push_back({1, {-0.17, -1.35, 1.02}});

    return geometry;
}

// The block fixture's build parameters.  The trim is OFF on purpose: F1.3 is
// about the whole product grid, so every identity must survive at every
// displaced geometry and a point that comes and goes must not be able to blame
// the derivative.  The trim's own behaviour is F1.6's subject.  The radial
// scale is small so that the outermost shell's quadrature factor is of order
// 1e4 rather than the 1e7 a 288 Bohr box would carry: the partition does not
// care, and a difference of a 1e7-sized weight is worth far less than one of a
// 1e4-sized weight.
excgrid::GridParams BlockParams() {
    excgrid::GridParams params;
    params.radialPoints = 24;
    params.angularPoints = 26;
    params.alpha = 0.05;
    params.trimWeight = 0.0;
    params.blockTarget = 128;

    return params;
}

// ---------------------------------------------------------------------------
// F1.1 - one atom, one point: the coordinate's derivative.
// ---------------------------------------------------------------------------

// A grid point is its owner's own radial point times its own angular node, so it
// translates rigidly with that atom: the derivative is the owner's identity
// block and nothing else.  The check is against a central difference of the very
// position the build would generate.
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

// ---------------------------------------------------------------------------
// F1.2 - one atom, one point: the weight's derivative.
// ---------------------------------------------------------------------------

// With one atom the partition is one everywhere, so the weight is exactly the
// quadrature factor - constant in the geometry - and every derivative is
// exactly zero.  A missing or doubled normalization shows up here first.
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

// The same statement with a partner: the weight is no longer constant, its
// derivative is the partition's, and the quadrature factor scales both.
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

// ---------------------------------------------------------------------------
// F1.3 - a whole block, points and totals.
// ---------------------------------------------------------------------------

// The whole product grid of a three-atom molecule, point by point against the
// weight the build hands out at the displaced geometry, and then the grid's own
// totals: the weight sum and the first moment, both of which a consumer
// accumulates and neither of which is a per-point quantity.
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

    // The value tie to the build: every point the build hands out is the point
    // this pass evaluates, and the two agree on its weight.  This is the guard
    // on the partition itself and on the radius table repeated beside it.
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

    // The derivatives of the grid's totals, accumulated point by point from the
    // same per-point derivatives a consumer would use: T = sum_i w_i, and the
    // first moment M_j = sum_i w_i x_ij.
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

                    // The point is a rigid translate of its owner, so its own
                    // position contributes delta_(b, owner) delta_(l, row).
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

    // Each nuclear coordinate in turn: the four displaced builds, then every
    // point of the grid against them, then the four totals.
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
                // The trim is off for this fixture, so a point missing from a
                // displaced build is a failure and never a silent zero.
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
                // The total's own magnitude is the scale its round-off rides
                // on, and for a moment that already carries the |x| of the sum.
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

    // The fixture is asymmetric, so no total is zero by symmetry and every one
    // of them carries a real derivative; the largest is what says the totals
    // were checked at all.
    EXPECT_GT(largestTotal, 1e-3);
}

// A block with more points than the build's per-block target does not exist, so
// the "whole block" claim is measured on the fixture rather than assumed: at
// least one block is exactly full.
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

// ---------------------------------------------------------------------------
// F1.4 - the second derivatives.
// ---------------------------------------------------------------------------

// The coordinate's second derivative is identically zero - not small, exactly
// zero - because a grid point is its owner's own radial point times its own
// angular node and neither factor depends on the geometry: the point is a rigid
// translate of its atom.  Owner 1 of a three-atom frame puts the identity block
// at a non-zero offset, where a wrong offset shows up.
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

    // dx/dR is a constant, so nothing assembled at second order receives a
    // contribution from the coordinate's own motion beyond the first order one.
    for (const double entry : analytic.second)
    {
        EXPECT_EQ(entry, 0.0);
    }
}

// The weight's second derivative at one point of a two-atom fixture, both kinds
// at once: the diagonal entries against a second difference, the mixed ones -
// which no single-coordinate displacement can reach - against the four-corner
// cross difference, and the sparsity the header promises audited entry by entry
// rather than trusted.
//
// The fixture is the caller's, so the identical check runs on the collinear
// pair - where some entries are zero by symmetry, and a difference of those
// measures the round-off of the terms that cancel rather than the derivative -
// and on a distorted one, where nothing vanishes for a reason the derivative
// had no part in.  The two together are the reason the agreement is evidence.
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

                // The audit, before anything overwrites the buffer: the rows
                // and columns of the active atoms are the only entries that are
                // anything but zero, and they are mirrored rather than
                // recomputed, so the matrix is symmetric bit for bit.
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

                // Everything the differences below are compared against is read
                // out now, because they reuse the buffer: an output buffer
                // carries its own record of the blocks it holds, so a run of
                // points into one buffer is the contract, and a second buffer
                // would need a second record.
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

                // The mixed entries: the four corners of the two-coordinate
                // displacement, which is the only stencil that reaches d2W/dR_k
                // dR_l for k != l.
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

                // The audit and the differences cover every entry of the row:
                // the first derivatives come along for the ride, and the second
                // order's `first` is the same array the first order would give.
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

// The same check on a fixture with no symmetry left in it.  The pair above is
// invariant under z -> -z together with an exchange of its two atoms, so it has
// coordinates that sit exactly at zero and entries that vanish exactly; on a
// point of such a fixture a difference of a vanishing entry measures the
// round-off of the terms that cancel, which is agreement bought at no price.
// Every coordinate here is a fixed small displacement away from that layout, so
// no entry is zero for a reason the derivative had no part in, and no two
// distances tie.
TEST(GridDerivSecondTest, JitteredPairPointSecondDerivativeMatchesCentralDifference) {
    excgrid::GridParams params;
    params.radialPoints = 8;
    params.angularPoints = 6;
    auto basis = MakeGridBasis(params);
    ASSERT_TRUE(basis.has_value());

    CheckOnePointHessian("the jittered pair", JitteredPair(), *basis);
}

// The whole product grid at second order: the diagonal entries against the same
// displaced builds F1.3 uses, the grid's totals one order up from the same
// builds, and the mixed entries - the cross-cell blocks between two atoms, which
// no single-coordinate displacement reaches - against a four-corner difference.
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

    // Pass one: walk the enumeration and accumulate the second derivatives of
    // the two quantities a consumer accumulates - the weight sum and the first
    // moment - out of the same per-point derivatives it would use.  Which mixed
    // entries the fixture gives content to is measured here too, rather than
    // assumed by the pass that checks them.
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
                    // d2(M_row)/dR_k dR_l = sum_i [ d2w_i x_row + dw_i/dR_k dx_row/dR_l
                    //                                  + dw_i/dR_l dx_row/dR_k ],
                    // and the point is a rigid translate of its owner, so its
                    // own position's derivative is one exactly when the
                    // coordinate's atom is that owner and its component is row.
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

    // Pass two: every nuclear coordinate in turn, the four displaced builds, the
    // grid's own totals out of them, then every point's diagonal entry and the
    // four totals against them.
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
                // The trim is off for this fixture, so a point missing from a
                // displaced build is a failure and never a silent zero.
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

    // Pass three: the mixed entries.  A pair the fixture gives content to is
    // checked point by point against the four corners of a two-coordinate
    // displacement, and the weight sum and the moments against the sums of those
    // same corners - which is what the grid's totals are, the trim being off.
    // The displacement moves two coordinates at once, so the geometry is not one
    // a single build walks; the evaluations are the ones a displaced build's
    // weights were shown to agree with in F1.3 and in pass two.
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

    // The fixture is not allowed to be one where the checks above had nothing to
    // do: entries with content, on both a single atom's two components and a
    // pair of different atoms, whose blocks are the reason the partition's
    // Hessian has off-diagonal structure at all.
    EXPECT_GT(largestEntry, 1e-6);
    EXPECT_GT(largestDiagonal, 1e-6);
    EXPECT_GT(largestTotal, 1e-3);
    EXPECT_GT(largestMixed, 1e-6);
    EXPECT_GT(largestMixedTotal, 1e-6);
    EXPECT_GT(sameAtomPairs, 0u);
    EXPECT_GT(crossAtomPairs, 0u);
    EXPECT_EQ(asymmetric, 0u);
}

// ---------------------------------------------------------------------------
// F1.5 - the grid-motion term at second order: the acceptance.
// ---------------------------------------------------------------------------

/// The smooth field the acceptance integrates: a Gaussian of variance `width`,
/// standing in for whatever a consumer's grid carries - a density, an
/// integrand - and with its gradient and its curvature in closed form.
///
/// It is deliberately a function of the POINT alone, with no atom anywhere in
/// it: f(x) = exp(-|x|^2 / width),
/// grad f = -2 x f / width,  d2f = (4 x x^T / width^2 - 2 I / width) f.
/// A displaced geometry therefore moves the integral only through the grid -
/// the points' motion and the weights' - which is exactly the term under test,
/// and none of the second derivative belongs to the field's own atoms.
struct TestField {
    double width = 2.5; ///< The Gaussian's variance, in Bohr^2.

    /// The field at one point.
    [[nodiscard]] double Value(const std::array<double, 3>& x) const {
        return std::exp(-(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]) / width);
    }

    /// The field's gradient at one point.
    [[nodiscard]] std::array<double, 3> Gradient(const std::array<double, 3>& x) const {
        const double value = Value(x);

        return {-2.0 * x[0] * value / width,
                -2.0 * x[1] * value / width,
                -2.0 * x[2] * value / width};
    }

    /// The field's curvature at one point, row-major.
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

/// The energy of a BUILT grid, integrated with the field.
///
/// This is the side of the acceptance that must not be the derivative pass: two
/// sides sharing a weight would be the weight checked against itself.  Every
/// displaced value comes from a fresh excgrid::BlockGrid::Create, which the
/// derivative pass has no part in.  A build that fails counts itself and
/// contributes nothing, so the caller can report it rather than read zeros.
/// \param geometry The molecule.
/// \param params The build parameters.
/// \param field The field to integrate.
/// \param failed Incremented when a build does not succeed.
/// \returns The energy, or zero from a build that failed.
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

/// The same energy at one displaced nuclear coordinate.
double EnergyAt(const excgrid::Geometry& geometry,
                const excgrid::GridParams& params,
                const TestField& field,
                std::size_t atom,
                std::size_t component,
                double amount,
                std::size_t& failed) {
    return EnergyOfBuild(Displaced(geometry, atom, component, amount), params, field, failed);
}

/// The same energy at a corner of two displaced coordinates: what a mixed
/// second derivative needs and a single-coordinate displacement cannot reach.
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

// THE ACCEPTANCE: one molecule's grid-motion term at second order, against the
// energies of slightly displaced geometries.
//
// The quantity is a whole-grid integral of the fixture,
//     E(R) = sum_g w_g(R) f(x_g(R)),
// with f the smooth field above and w the weight the build hands out.  Every
// point moves with its owner - a point is that atom's own radial point times its
// own angular node - and every weight moves with every atom, so the second
// derivative of E has three parts:
//     d2E/dR_b dR_c = sum_g (d2w_g/dR_b dR_c) f_g                        (a)
//                   + [owner(g) == c] sum_g (dw_g/dR_b) (grad f_g)_m      (b)
//                   + [owner(g) == b] sum_g (dw_g/dR_c) (grad f_g)_l      (c)
//                   + [b == c] sum_{g: owner b} w_g (d2f_g)_lm            (d)
// of which (a) is the weights' own second derivative - the term F1.4 built and
// this test exists to prove is NEEDED - (b) and (c) are the two cross terms in
// which a weight's first derivative meets the field's gradient through the
// owner's motion, and (d) is the frozen-grid term carrying no derivative of a
// weight at all.  (a)-(c) are the grid's motion.
//
// A Hessian assembled without (a) is wrong and plausible: it is the exact
// Hessian of an integral over a grid nobody built.  That is why this is the
// acceptance rather than a tolerance, and why the check runs twice - with (a) in
// place, where it must agree with the difference, and with (a) dropped, where it
// must not.  The second is what says the term is measurable, and that the
// agreement above is not something any Hessian would have given.
//
// The trim is off in this fixture, so a displaced build carries exactly the
// points of the base geometry and none switches in or out; the trimming's own
// differentiability is F1.6's subject.  The fixture is asymmetric and every
// displacement goes through Displaced, which asserts that no atom reordered:
// a reordered pair would compare two different molecules, which is not a
// derivative of anything.
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

    // The analytic side: one walk over the product grid, each point adding its
    // three parts to the entries they belong to.  Two matrices, so that the term
    // under test can be dropped from one and the two answers compared.
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

        // (a) the weights' own second derivative, the term the extension exists
        // for.  It rides on every entry the point's active atoms name, which is
        // why it goes to both matrices' entries at once - only the term itself
        // is left out of the second.
        for (std::size_t row = 0; row < width; ++row)
        {
            for (std::size_t column = 0; column < width; ++column)
            {
                withSecond[row * width + column] += out.second[row * width + column] * value;
                withoutSecond[row * width + column] += 0.0;
            }
        }

        // (b) and (c): a weight's first derivative against the field's gradient,
        // reaching the owner's row and the owner's column of every entry.
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

        // (d) the frozen-grid term: the field's curvature, on the owner's own
        // block, with no derivative of a weight anywhere in it.
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

    // Both sides enumerate the same product grid - one integrated from the build,
    // one from the derivative pass - so their energies are the same number before
    // any derivative is taken.
    EXPECT_NEAR(analyticEnergy, reference, 1e-12 * analyticScale);

    std::size_t entries = 0;
    std::size_t moved = 0;
    std::size_t missed = 0;
    double largest = 0.0;
    double worstMiss = 0.0;

    for (std::size_t k = 0; k < width; ++k)
    {
        // The diagonal entries: one coordinate, a second difference.
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

        // The mixed entries: two coordinates, a four-corner difference.
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

    // The term has to be there to be tested.  The first says it moves entries of
    // this fixture at all - measured, it moves every one of the 45 the frame
    // names - and the second says an entry it moves is missed by far more than
    // the difference's own tolerance, which is what makes the agreement above
    // evidence for the term rather than for arithmetic in general.  Measured, the
    // worst entry is missed by 5e5 times its tolerance: the term is four orders
    // of magnitude clear of the bound the right answer is held to.
    EXPECT_GT(moved, entries / 2u)
        << "the weights' second derivative moves only " << moved << " of " << entries
        << " entries: this fixture cannot tell its presence from its absence";
    EXPECT_GT(missed, 0u) << "removing the weights' second derivative left every entry inside the "
                             "difference's own tolerance";
    EXPECT_GT(worstMiss, 100.0)
        << "dropping the weights' second derivative misses the difference by only " << worstMiss
        << " times its tolerance, so the agreement above would have been obtained without it";
}

// ---------------------------------------------------------------------------
// F1.6 - the trimming is differentiable, or it fails loudly.
// ---------------------------------------------------------------------------

/// The F1.3 fixture with the production trim switched on: the grid these checks
/// walk is a trimmed one, which is the whole point of them.
excgrid::GridParams TrimmedParams() {
    excgrid::GridParams params = BlockParams();
    params.trimWeight = 1e-15;

    return params;
}

/// Whether two tabulations hold exactly the same identities, entry by entry.
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

/// The first identity on which two tabulations disagree, named, or "none".
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

// The precondition every derivative check in this file rests on, checked
// directly and on a TRIMMED grid rather than a trim-off fixture.
//
// A displacement moves each weight, and a point whose weight crosses the trim
// threshold changes the SET of points the grid carries.  When that happens,
// "the derivative of the trimmed sum" is not the sum of its points'
// derivatives, and no error in a derivative formula can see it: the formula is
// right about a sum nobody is computing.  This test sweeps every nuclear
// coordinate, both ways, at both steps the checks above take their differences
// at, and asserts the identity set is the same at every geometry in the sweep.
//
// The margin is measured rather than assumed, and it is what makes the sweep a
// statement instead of a coincidence: the points this build drops are dropped
// for having a weight of EXACTLY zero - they contribute nothing to any sum, so
// the trim is exactly neutral here - and the smallest weight it keeps is
// 5.5e-13, five hundred and fifty times the 1e-15 threshold.  Nothing in this
// fixture sits near the trim at the step a derivative is taken at.
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

    // The trim has to have done something, or the sweep below is a sweep of the
    // untrimmed grid and F1.3 already covers that.
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
            // Dropped, and dropped for a weight of exactly zero: the whole
            // margin this fixture has, and the reason the sweep below is clean.
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

// The other side of F1.6: the switch is reachable, and it is not a
// negligible-weight event, so a derivative pass that ignored it would be
// silently wrong rather than approximately right.
//
// The sweep here is deliberately coarser than a derivative step, because the
// switch is a property of the geometry and sits where it sits: on this fixture
// the first one takes a displacement of 1e-2, a hundred times the second
// difference's step and a thousand times the first's.  The two tests together
// are the statement - clean at the step a derivative is taken at, and a real
// event not far beyond it.
//
// What makes it loud is not the arithmetic but the witness: the identity set.
// A point this grid drops at the base geometry carries 3.1e-6 at a geometry
// 1e-2 away - nine orders of magnitude ABOVE the 1e-15 threshold that dropped
// it - while the lightest weight it keeps sits five hundred and fifty times
// above the same threshold.  The trim is a property of the geometry and not of
// a point, and two builds are enough to see it: this test uses nothing else,
// which is itself the evidence that the condition is detectable without taking
// a single derivative.
//
// That condition is what the contract's DerivativeStatus::kRefusedUndifferentiableGrid
// names.  Raising it is the provider's business and not this file's; what is
// established here is that it is real, reachable, and that a provider which
// returned kOk across a switch would be reporting a derivative of a sum that
// no geometry has.
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

    // The loud side is the one that leaves.  A point that ENTERS does so by
    // rising through the threshold and carries barely more than it - the
    // lightest measured here is 2.2e-13, two hundred and twenty times the
    // threshold - which is what a crossing is supposed to look like.  A point
    // that LEAVES does not: it was kept at 3.1e-6, three million times the same
    // threshold, and that weight is gone from the grid one displacement later.
    // What the trim removes there is not a point becoming negligible; it is a
    // point the grid no longer has.
    EXPECT_GT(lightestVanished, 1000.0 * params.trimWeight)
        << "the lightest point this trim drops carries " << lightestVanished << ", only "
        << lightestVanished / params.trimWeight << " times the threshold";
}

/// A deliberately symmetric cluster: four identical atoms at the alternate
/// corners of a cube.  Every atom is the same element, every atom sits at the
/// same distance from every other, and the arrangement is closed under the
/// octahedral group - so anything that sorted, deduplicated or canonicalised
/// atoms by position would have to break a tie to do it, and would break it
/// differently at the base geometry and at a displaced one.
excgrid::Geometry SymmetricCluster() {
    excgrid::Geometry geometry;
    geometry.atoms.push_back({1, {1.0, 1.0, 1.0}});
    geometry.atoms.push_back({1, {1.0, -1.0, -1.0}});
    geometry.atoms.push_back({1, {-1.0, 1.0, -1.0}});
    geometry.atoms.push_back({1, {-1.0, -1.0, 1.0}});

    return geometry;
}

// The guard behind every finite-difference comparison in this file, driven over
// the fixture built to have the ties.
//
// A reordered pair of atoms is the same class of silent failure F1.6 is about -
// a discontinuity nobody asked for - and it is worse than a switched point,
// because the comparison it corrupts is between two different molecules and the
// difference still looks like a number.  Displaced asserts the atom count, the
// element of every index and the exact position of every component on every
// call, so the sweep below drives that assertion over a cluster whose symmetry
// is the hazard, at three steps including the smallest a derivative is taken
// at.  The same facts are asserted a second time here, so this test states its
// own claim rather than resting on a helper's.
//
// Nothing in the library sorts today: Geometry is documented as atoms in input
// order and the build has no ordering step anywhere.  This is the test that
// fails if one is ever added, and the grid's identity set is checked beside it
// because a fixture can also lose its points to a discontinuity rather than its
// atoms to a reordering.
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

                // And the grid built from it carries the same points at the
                // steps a difference is taken at: no point switching in or out,
                // which is the other discontinuity this fixture could hide.
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
