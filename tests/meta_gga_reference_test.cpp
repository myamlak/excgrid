// The tau tier's hand-written reference, verified by its exact conditions.
//
// The functional under test is the second-order gradient expansion of exchange
// written in the iso-orbital variables (src/meta_gga_reference.cpp):
//
//     e_sigma = e_x^LSDA(rho_sigma) (1 + (1 - tau_P/tau_unif)/12),
//     tau_P = tau_sigma - |grad rho_sigma|^2/(8 rho_sigma),
//     tau_unif = (3/10)(6 pi^2)^(2/3) rho_sigma^(5/3).
//
// What it is checked against is a set of EXACT CONDITIONS rather than another
// implementation's numbers: the uniform-gas limit seen through both the energy
// density and the density-functional derivative along the gas's own manifold,
// the von Weizsacker relation tau_W = |grad rho|^2/(8 rho) that ties the sigma
// slot to the tau slot, the exact 10/81 of the exchange gradient expansion, the
// schema's mask semantics on the new inputs, and the finite-difference channel
// over tau.  A point-value comparison against a reference sharing this code's
// approximations is not among them on purpose.

#include "excgrid/components.hpp"
#include "excgrid/kernel.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>
#include <limits>
#include <string_view>
#include <utility>

// Declared here at namespace scope, OUTSIDE the anonymous namespace below: a
// namespace nested inside an unnamed namespace has internal linkage, so a
// declaration there would not bind to the definition in src/meta_gga_reference.cpp.
namespace excgrid {
const XcFunctional& MetaGgaReferenceFunctional() noexcept;
} // namespace excgrid

namespace {

using excgrid::Component;
using excgrid::ComponentMask;
using excgrid::IndexOf;
using excgrid::PointInputs;
using excgrid::PointResult;
using excgrid::XcFunctional;

constexpr double kPi = 3.14159265358979323846;

/// The shipped name of the tau tier's reference.
constexpr std::string_view kReferenceName = "tau_x";

/// (3/4)(6/pi)^(1/3): the spin-scaled Slater constant, so the LSDA exchange
/// energy density is -(3/4)(6/pi)^(1/3) rho^(4/3) per spin channel.
const double kSlaterSpinPrefactor = 0.75 * std::cbrt(6.0 / kPi);

/// The cube root of 6 pi^2, carried separately so the Thomas-Fermi constant and
/// the reduced gradient below come from one number.
const double kSixPiSquaredCbrt = std::cbrt(6.0 * kPi * kPi);

/// (3/10)(6 pi^2)^(2/3): the spin-scaled Thomas-Fermi constant, so a uniform gas
/// has tau_sigma = this * rho_sigma^(5/3) in that channel.
const double kFermiConstant = 0.3 * kSixPiSquaredCbrt * kSixPiSquaredCbrt;

/// The exact-condition tolerance, relative.  Every check below compares two
/// routes to the same expression, so the residual is round-off only: a handful
/// of roundings at ~1e-16 each, and at most a 1.3x cancellation in the sums
/// involved.  1e-14 sits above that floor and ten orders below the smallest
/// coefficient error any of these checks exists to catch - a wrong von
/// Weizsacker coefficient of 1/4 in place of 1/8, for instance, moves the sigma
/// derivative by 100%.
constexpr double kIdentityTolerance = 1e-14;

/// One grid point's components, before a mask is put on them.
struct Point {
    double rhoA = 0.0;
    double rhoB = 0.0;
    double sigmaAa = 0.0;
    double sigmaAb = 0.0;
    double sigmaBb = 0.0;
    double tauA = 0.0;
    double tauB = 0.0;
};

/// The point's values with exactly the mask named.
///
/// The values go straight into the buffer and the mask is set afterwards, so a
/// caller can put a value in the buffer and leave it out of the mask - which is
/// the state the mask tests need, and the only way to reach it (PointInputs::Set
/// always adds what it writes).
PointInputs MaskedInputs(const Point& point, const ComponentMask& mask) {
    PointInputs inputs;
    inputs.mask = mask;
    inputs.values[IndexOf(Component::RhoA)] = point.rhoA;
    inputs.values[IndexOf(Component::RhoB)] = point.rhoB;
    inputs.values[IndexOf(Component::SigmaAa)] = point.sigmaAa;
    inputs.values[IndexOf(Component::SigmaAb)] = point.sigmaAb;
    inputs.values[IndexOf(Component::SigmaBb)] = point.sigmaBb;
    inputs.values[IndexOf(Component::TauA)] = point.tauA;
    inputs.values[IndexOf(Component::TauB)] = point.tauB;
    return inputs;
}

class MetaGgaReferenceTest : public ::testing::Test {
protected:
    /// The hand-written functional itself, not the registry's name for the tier.
    ///
    /// The registry publishes the GENERATED tau-tier kernel under that name, so
    /// resolving through it here would test the generator's output rather than
    /// the reference this file is about - and would leave the hand-written
    /// implementation with no coverage at all.  The agreement between the two is
    /// checked where the registry is (tests/registry_test.cpp).
    [[nodiscard]] const XcFunctional& Reference() const {
        return excgrid::MetaGgaReferenceFunctional();
    }

    /// The point through the reference, with the collinear mask unless one is named.
    [[nodiscard]] PointResult Evaluate(
        const Point& point, const ComponentMask& mask = ComponentMask::Collinear()) const {
        const PointInputs inputs = MaskedInputs(point, mask);
        PointResult result;
        EXPECT_EQ(Reference().EvaluatePoint(inputs, result), excgrid::KernelStatus::kOk);
        return result;
    }
};

/// The uniform electron gas in both channels: no gradient, and the channel's
/// kinetic-energy density at its Thomas-Fermi value.
Point UniformGasPoint(double rhoA, double rhoB) {
    Point point;
    point.rhoA = rhoA;
    point.rhoB = rhoB;
    point.tauA = kFermiConstant * std::pow(rhoA, 5.0 / 3.0);
    point.tauB = kFermiConstant * std::pow(rhoB, 5.0 / 3.0);
    return point;
}

// The exact condition, in the form every meta-GGA owes: a uniform electron gas
// is described exactly as LSDA describes it.  Its gradient vanishes and its
// kinetic-energy density is the Thomas-Fermi value above, and both channels must
// then carry the spin-scaled Slater energy density with no enhancement at all.
//
// This is the check that pins the tau NORMALIZATION: the enhancement is 1 only
// if tau/tau_unif is exactly 1, so a kernel carrying the unpolarized constant
// (3/10)(3 pi^2)^(2/3) in place of the spin-scaled (3/10)(6 pi^2)^(2/3) lands 3.1%
// away from the LSDA value here and fails.  Unequal spins are used because a
// spin-blind normalization would still cancel at rhoA = rhoB.
TEST_F(MetaGgaReferenceTest, UniformGasReproducesTheLsdaExchange) {
    const std::array<std::pair<double, double>, 3> gases = {
        {{0.26, 0.14}, {0.05, 0.11}, {0.4, 0.4}}};

    for (const auto& [rhoA, rhoB] : gases)
    {
        const PointResult result = Evaluate(UniformGasPoint(rhoA, rhoB));
        const double expected =
            -kSlaterSpinPrefactor * (std::pow(rhoA, 4.0 / 3.0) + std::pow(rhoB, 4.0 / 3.0));

        EXPECT_NEAR(result.exc, expected, kIdentityTolerance * std::abs(expected))
            << "rhoA = " << rhoA << ", rhoB = " << rhoB << ": the enhancement must be exactly 1";
    }
}

// The same exact condition through the derivative the schema's new component
// supplies.  A meta-GGA's density-functional derivative along the uniform gas's
// own manifold is the total derivative of the energy density with tau_sigma
// following rho_sigma at the Thomas-Fermi rate,
//
//     de/d rho = @e/@rho|tau + @e/@tau * (5/3) kFermi rho^(2/3),
//
// and it must equal the LSDA exchange potential -(4/3)(3/4)(6/pi)^(1/3) rho^(1/3).
// This is the condition tying the tau slot to the density slot: the energy check
// above is passed by a kernel that reads tau with any weight that happens to
// cancel, while this one is passed only by the right one.  At the point checked
// the density derivative alone is 8% away from the LSDA potential, so the tau
// term is load-bearing rather than a rounding correction.
TEST_F(MetaGgaReferenceTest, UniformGasReproducesTheLsdaPotentialAlongTheGasManifold) {
    const std::array<std::pair<double, double>, 2> gases = {{{0.26, 0.14}, {0.05, 0.11}}};

    for (const auto& [rhoA, rhoB] : gases)
    {
        const PointResult result = Evaluate(UniformGasPoint(rhoA, rhoB));

        const double tauRate = (5.0 / 3.0) * kFermiConstant;
        const double totalA = result.first[IndexOf(Component::RhoA)] +
                              result.first[IndexOf(Component::TauA)] * tauRate *
                                  std::pow(rhoA, 2.0 / 3.0);
        const double totalB = result.first[IndexOf(Component::RhoB)] +
                              result.first[IndexOf(Component::TauB)] * tauRate *
                                  std::pow(rhoB, 2.0 / 3.0);
        const double expectedA = -(4.0 / 3.0) * kSlaterSpinPrefactor * std::pow(rhoA, 1.0 / 3.0);
        const double expectedB = -(4.0 / 3.0) * kSlaterSpinPrefactor * std::pow(rhoB, 1.0 / 3.0);

        EXPECT_NEAR(totalA, expectedA, kIdentityTolerance * std::abs(expectedA))
            << "rhoA = " << rhoA;
        EXPECT_NEAR(totalB, expectedB, kIdentityTolerance * std::abs(expectedB))
            << "rhoB = " << rhoB;
    }
}

// The von Weizsacker relation and the consistency of the kinetic-energy
// derivative.  A spin channel's von Weizsacker kinetic-energy density is
//
//     tau_W,sigma = |grad rho_sigma|^2 / (8 rho_sigma) = sigma_sigma/(8 rho_sigma),
//
// with tau_sigma >= tau_W,sigma for every physical density and equality for a
// single orbital.  The reference's gradient enters through tau_P = tau - tau_W
// and nothing else, so differentiating that identity gives the tie between the
// two derivative slots the schema publishes:
//
//     @e/@sigma_sigma = -(1/(8 rho_sigma)) @e/@tau_sigma.
//
// The tie is exact at every point and is the consistency statement for the
// kinetic-energy derivative: what it catches is exactly the slip that matters
// here - the coefficient 8, its reciprocal, or the sign - and it is deliberately
// a relation between two returned numbers rather than a point value, so no
// external reference can satisfy it by sharing an approximation.
TEST_F(MetaGgaReferenceTest, VonWeizsackerRelationTiesSigmaToTau) {
    const std::array<Point, 3> points = {{
        {0.42, 0.31, 0.021, 0.004, 0.017, 0.09, 0.07},
        {0.08, 0.61, 0.3, 0.0, 0.0014, 0.02, 0.9},
        {1.9, 0.045, 0.7, 0.02, 0.13, 2.4, 0.06},
    }};

    for (const Point& point : points)
    {
        const PointResult result = Evaluate(point);

        const double sigmaDerivativeA = result.first[IndexOf(Component::SigmaAa)];
        const double sigmaDerivativeB = result.first[IndexOf(Component::SigmaBb)];
        const double tauDerivativeA = result.first[IndexOf(Component::TauA)];
        const double tauDerivativeB = result.first[IndexOf(Component::TauB)];

        EXPECT_NEAR(sigmaDerivativeA, -tauDerivativeA / (8.0 * point.rhoA),
                    kIdentityTolerance * std::abs(tauDerivativeA) / (8.0 * point.rhoA))
            << "rhoA = " << point.rhoA;
        EXPECT_NEAR(sigmaDerivativeB, -tauDerivativeB / (8.0 * point.rhoB),
                    kIdentityTolerance * std::abs(tauDerivativeB) / (8.0 * point.rhoB))
            << "rhoB = " << point.rhoB;

        // The cross invariant is outside the relation: it carries no channel's
        // own von Weizsacker density, so a channel-separable functional has no
        // derivative with respect to it.
        EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::SigmaAb)], 0.0);
    }
}

// The von Weizsacker relation at the level of values rather than derivatives: at
// fixed rho_sigma the pair (sigma_sigma, tau_sigma) may be moved together by
// (8 rho_sigma delta, delta) without moving tau_P, so the energy density and the
// two partials it is built from must not move - however far the move goes, and
// whichever way it takes sigma_sigma.  The density partial is deliberately NOT
// among them: it is taken at fixed sigma, and the move changes sigma.
//
// The equality case of the relation is the physical realization of tau_P = 0: a
// single orbital has tau_sigma = tau_W,sigma exactly, which is the state the
// alpha channel is anchored on below.
TEST_F(MetaGgaReferenceTest, GradientEntersOnlyThroughTheVonWeizsackerDensity) {
    Point point;
    point.rhoA = 0.42;
    point.rhoB = 0.31;
    point.sigmaAa = 0.021;
    point.sigmaBb = 0.017;
    point.tauA = 0.021 / (8.0 * point.rhoA); // the single-orbital value: tau = tau_W
    point.tauB = 0.017 / (8.0 * point.rhoB) + 0.2;

    const PointResult anchored = Evaluate(point);

    for (const double delta : {1e-6, 1e-2, 1.0})
    {
        Point moved = point;
        moved.sigmaAa += 8.0 * point.rhoA * delta;
        moved.tauA += delta;
        moved.sigmaBb += 8.0 * point.rhoB * delta;
        moved.tauB += delta;

        const PointResult result = Evaluate(moved);

        // tau_P is invariant in exact arithmetic; what is left is the round-off
        // of the two moves, a couple of ulps of sigma/(8 rho) that reach the
        // energy density scaled by de/d tau, so a few ulps of exc - hence a
        // tolerance rather than an equality, and a tight one.
        EXPECT_NEAR(result.exc, anchored.exc, 1e-15 * std::abs(anchored.exc)) << "delta = " << delta;

        // These two are untouched by construction: the move leaves them exactly
        // where they were.
        EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::SigmaAa)],
                         anchored.first[IndexOf(Component::SigmaAa)])
            << "delta = " << delta;
        EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::TauA)],
                         anchored.first[IndexOf(Component::TauA)])
            << "delta = " << delta;
    }
}

// The exact coefficient of the exchange gradient expansion.  For a slowly
// varying density the exact second-order expansion of exchange is
//
//     e_x = e_x^LSDA (1 + (10/81) s_sigma^2 + O(grad^4)),
//
// with s_sigma = |grad rho_sigma|/(2 (6 pi^2)^(1/3) rho_sigma^(4/3)) the
// spin-scaled reduced gradient, and 10/81 is exact rather than fitted - it is
// the coefficient PBEsol carries for the same reason.  In the iso-orbital
// variables the slowly varying gas has tau_W/tau_unif = (5/3) s^2 (an algebraic
// identity of the two definitions) and tau = tau_unif (1 + (5/27) s^2), the
// second-order expansion of the kinetic-energy density itself (coefficient 1/72
// in |grad rho|^2/rho, smaller than the von Weizsacker 1/8 by the well-known
// factor nine).  Together those give tau_P/tau_unif = 1 - (40/27) s^2, so the
// enhancement's leading coefficient must come out as (40/27)/12 = 10/81.
//
// The point below is built from that physics rather than from the kernel: s and
// rho determine sigma and tau independently.  The round-off in the ratio
// (e_x/e_x^LSDA - 1)/s^2 is the energy density's ~1e-15 relative divided by s^2,
// which is 1e-12 at s^2 = 1e-3 and 1e-11 at 1e-4 - three orders inside the
// tolerance, and the same tolerance is four orders below the error a wrong
// coefficient would produce (the bare 1/8 of the von Weizsacker term would give
// 0.2778 in place of 0.1235).
TEST_F(MetaGgaReferenceTest, GradientExpansionCoefficientIsTheExactTenOverEightyOne) {
    constexpr double kCoefficientTolerance = 1e-9;
    constexpr double kExactCoefficient = 10.0 / 81.0;

    for (const double rho : {0.3, 1.4})
    {
        for (const double sSquared : {1e-3, 1e-4})
        {
            Point point;
            point.rhoA = rho;
            // sigma = |grad rho|^2 from the definition of s, and tau from the
            // second-order expansion of the kinetic-energy density.
            point.sigmaAa = 4.0 * kSixPiSquaredCbrt * kSixPiSquaredCbrt * std::pow(rho, 8.0 / 3.0) *
                            sSquared;
            point.tauA = kFermiConstant * std::pow(rho, 5.0 / 3.0) * (1.0 + (5.0 / 27.0) * sSquared);

            const PointResult result = Evaluate(point);
            const double lsda =
                -kSlaterSpinPrefactor * std::pow(point.rhoA, 4.0 / 3.0);
            const double measured = (result.exc / lsda - 1.0) / sSquared;

            EXPECT_NEAR(measured, kExactCoefficient, kCoefficientTolerance * kExactCoefficient)
                << "rho = " << rho << ", s^2 = " << sSquared;
        }
    }
}

// The schema's mask, on the new input.  The two evaluations below carry the SAME
// buffer - a large tau in both channels - and differ only in whether the tau bits
// are in the mask.  An input outside the mask contributes exactly zero, so the
// results must be bit-identical to the same point with tau zeroed.
TEST_F(MetaGgaReferenceTest, AnInputOutsideTheMaskContributesExactlyZero) {
    Point uncounted;
    uncounted.rhoA = 0.42;
    uncounted.rhoB = 0.31;
    uncounted.sigmaAa = 0.021;
    uncounted.sigmaBb = 0.017;
    uncounted.tauA = 3.7;
    uncounted.tauB = 2.9;

    ComponentMask withoutTau = ComponentMask::Collinear();
    withoutTau.Clear(Component::TauA);
    withoutTau.Clear(Component::TauB);

    const PointInputs unmasked = MaskedInputs(uncounted, withoutTau);
    EXPECT_DOUBLE_EQ(unmasked.Get(Component::TauA), 0.0);
    EXPECT_DOUBLE_EQ(unmasked.Get(Component::TauB), 0.0);

    Point zeroed = uncounted;
    zeroed.tauA = 0.0;
    zeroed.tauB = 0.0;

    const PointResult fromUnmasked = Evaluate(uncounted, withoutTau);
    const PointResult fromZeroed = Evaluate(zeroed, withoutTau);

    EXPECT_DOUBLE_EQ(fromUnmasked.exc, fromZeroed.exc);
    EXPECT_DOUBLE_EQ(fromUnmasked.first[IndexOf(Component::RhoA)],
                     fromZeroed.first[IndexOf(Component::RhoA)]);
    EXPECT_DOUBLE_EQ(fromUnmasked.first[IndexOf(Component::SigmaAa)],
                     fromZeroed.first[IndexOf(Component::SigmaAa)]);
    EXPECT_DOUBLE_EQ(fromUnmasked.first[IndexOf(Component::TauA)],
                     fromZeroed.first[IndexOf(Component::TauA)]);
}

// The other half of the same rule: an input INSIDE the mask is consumed and moves
// the result.  The two points below share a density and a gradient and differ
// only in tau - the iso-orbital value tau = tau_W (a single orbital, the equality
// case of the von Weizsacker relation) against the uniform-gas value - and the
// energy density must move by the enhancement's own difference, 0.0404
// Ha/Bohr^3 here.  The check is the order of magnitude, so that a kernel ignoring
// tau cannot pass by accident.
//
// The slot the result publishes follows the caller's mask rather than tau's
// value, and the reference makes that visible: de/d tau is a constant in tau
// here, so the slot is identical at both taus and is exactly zero when the bit is
// clear.
TEST_F(MetaGgaReferenceTest, AnInputInsideTheMaskIsConsumedAndMovesTheResult) {
    Point isoOrbital;
    isoOrbital.rhoA = 0.42;
    isoOrbital.rhoB = 0.31;
    isoOrbital.sigmaAa = 0.021;
    isoOrbital.sigmaBb = 0.017;
    isoOrbital.tauA = isoOrbital.sigmaAa / (8.0 * isoOrbital.rhoA); // tau = tau_W: one orbital
    isoOrbital.tauB = isoOrbital.sigmaBb / (8.0 * isoOrbital.rhoB);

    Point uniform = isoOrbital;
    uniform.tauA = kFermiConstant * std::pow(uniform.rhoA, 5.0 / 3.0);
    uniform.tauB = kFermiConstant * std::pow(uniform.rhoB, 5.0 / 3.0);

    const PointResult isoResult = Evaluate(isoOrbital);
    const PointResult uniformResult = Evaluate(uniform);

    // The two points differ in tau_P by tau_unif - tau_W in each channel, so the
    // energy densities differ by the enhancement's own difference: (1/12) of the
    // channel's LSDA energy density less the von Weizsacker correction, which at
    // this point comes to 0.0404 Ha/Bohr^3.  The assert is on the order of
    // magnitude, with four times that as the floor, so a kernel that reads no tau
    // at all (difference exactly zero) cannot pass.
    EXPECT_GT(std::abs(uniformResult.exc - isoResult.exc), 1e-2)
        << "iso-orbital " << isoResult.exc << " against uniform " << uniformResult.exc;

    EXPECT_NE(uniformResult.first[IndexOf(Component::TauA)], 0.0);
    EXPECT_DOUBLE_EQ(uniformResult.first[IndexOf(Component::TauA)],
                     isoResult.first[IndexOf(Component::TauA)]);

    ComponentMask withoutTau = ComponentMask::Collinear();
    withoutTau.Clear(Component::TauA);
    withoutTau.Clear(Component::TauB);
    EXPECT_DOUBLE_EQ(Evaluate(uniform, withoutTau).first[IndexOf(Component::TauA)], 0.0);
}

// The finite-difference channel over the new input.
//
// The reference is linear in tau, so a central difference carries NO truncation
// error and any step is exact in exact arithmetic: what the step has to clear is
// the round-off floor, whose relative size is about ulp(exc)/(2 h |de/d tau|) -
// 2.5e-15/h at the point below, where |de/d tau| is 2.3e-2 in the alpha channel
// and 2.5e-2 in the beta one and exc is -5.3e-1.  That puts the floor at 2.5e-12
// for h = 1e-3 and 2.5e-6 for h = 1e-9.  The step used is 1e-3, three orders above
// the floor at the 1e-9 tolerance, and the ladder MEASURES both ends rather than
// trusting the model: the same difference at 1e-9 must break the tolerance, and
// at 1e-2 must still hold it, so the step sits in a range rather than on a knife
// edge.
TEST_F(MetaGgaReferenceTest, TauDerivativeMatchesCentralFiniteDifferences) {
    constexpr double kChosenStep = 1e-3;
    constexpr double kWiderStep = 1e-2;
    constexpr double kBelowFloorStep = 1e-9;
    constexpr double kRelativeTolerance = 1e-9;

    Point point;
    point.rhoA = 0.42;
    point.rhoB = 0.31;
    point.sigmaAa = 0.021;
    point.sigmaAb = 0.004;
    point.sigmaBb = 0.017;
    point.tauA = 0.09;
    point.tauB = 0.07;

    const PointResult result = Evaluate(point);
    const std::array<Component, 2> channels = {Component::TauA, Component::TauB};

    for (const Component channel : channels)
    {
        const double analytic = result.first[IndexOf(channel)];
        ASSERT_NE(analytic, 0.0) << "the kernel returned no tau derivative to difference";

        const auto differenceAt = [&](double step) {
            Point ahead = point;
            Point behind = point;
            if (channel == Component::TauA)
            {
                ahead.tauA += step;
                behind.tauA -= step;
            }
            else
            {
                ahead.tauB += step;
                behind.tauB -= step;
            }

            return (Evaluate(ahead).exc - Evaluate(behind).exc) / (2.0 * step);
        };

        const double relativeFloor = std::numeric_limits<double>::epsilon() *
                                     std::abs(result.exc) /
                                     (2.0 * kBelowFloorStep * std::abs(analytic));

        EXPECT_NEAR(differenceAt(kChosenStep), analytic, kRelativeTolerance * std::abs(analytic))
            << IndexOf(channel) << ": h = " << kChosenStep;
        EXPECT_NEAR(differenceAt(kWiderStep), analytic, kRelativeTolerance * std::abs(analytic))
            << IndexOf(channel) << ": h = " << kWiderStep;

        const double collapsed = differenceAt(kBelowFloorStep);
        EXPECT_GT(std::abs(collapsed - analytic), kRelativeTolerance * std::abs(analytic))
            << IndexOf(channel) << ": the difference at h = " << kBelowFloorStep << " is "
            << collapsed << " against the analytic " << analytic
            << " - the round-off floor it sits under is " << relativeFloor << " relative";
    }
}

// The registry surface: the reference resolves by name, reports the two kinetic-
// energy densities as required along with the collinear set, and no shipped
// LDA/GGA functional requires them.
TEST_F(MetaGgaReferenceTest, TheRegistryResolvesTheReferenceAndRequiresTau) {
    const ComponentMask mask = Reference().RequiredMask();

    EXPECT_TRUE(mask.Test(Component::RhoA));
    EXPECT_TRUE(mask.Test(Component::RhoB));
    EXPECT_TRUE(mask.Test(Component::SigmaAa));
    EXPECT_TRUE(mask.Test(Component::TauA));
    EXPECT_TRUE(mask.Test(Component::TauB));
    EXPECT_EQ(mask.ActiveCount(), 7U);
    EXPECT_TRUE(Reference().UsesGradient());
    EXPECT_DOUBLE_EQ(Reference().ExchangeFraction(), 0.0);

    int checked = 0;

    for (const std::string_view name : excgrid::FunctionalNames())
    {
        if (name == kReferenceName)
        {
            continue;
        }

        const XcFunctional* functional = excgrid::FindFunctional(name);
        ASSERT_NE(functional, nullptr) << name;
        EXPECT_FALSE(functional->RequiredMask().Test(Component::TauA)) << name;
        EXPECT_FALSE(functional->RequiredMask().Test(Component::TauB)) << name;
        ++checked;
    }

    EXPECT_GT(checked, 0) << "no shipped LDA/GGA functional was examined";
}

/// The density derivative at (rho, sigma, tau), hand-derived from the definition
/// at the top of this file rather than from the source's term decomposition.
///
/// With u = tau_P/tau_unif, e = e_x^LSDA(rho)(1 + (1 - u)/12), so the product and
/// quotient rules give de/drho = e_x^LSDA'(rho)(1 + (1 - u)/12) - e_x^LSDA(rho) u'/12,
/// where u' = (sigma/(8 rho^2) tau_unif - tau_P (5/3) kFermiConstant rho^(2/3))/tau_unif^2.
///
/// The sigma term of u' is what makes this helper worth having: it carries the
/// gradient, so it vanishes at sigma = 0, and every other check in this file
/// evaluates either there or on a slot that term does not reach.
/// \param rho The channel's density.
/// \param sigma grad(rho) . grad(rho) for this channel.
/// \param tau The channel's kinetic-energy density.
/// \returns de/drho at fixed sigma and tau, Hartree.
[[nodiscard]] double HandDerivedVrho(double rho, double sigma, double tau) {
    const double tauPauli = tau - sigma / (8.0 * rho);
    const double tauUniform = kFermiConstant * std::pow(rho, 5.0 / 3.0);
    const double lsda = -kSlaterSpinPrefactor * std::pow(rho, 4.0 / 3.0);
    const double lsdaPrime = -(4.0 / 3.0) * kSlaterSpinPrefactor * std::cbrt(rho);
    const double tauUniformPrime = (5.0 / 3.0) * kFermiConstant * std::pow(rho, 2.0 / 3.0);
    const double tauPauliPrime = sigma / (8.0 * rho * rho);
    const double uPrime =
        (tauPauliPrime * tauUniform - tauPauli * tauUniformPrime) / (tauUniform * tauUniform);
    const double enhancement = 1.0 + (1.0 - tauPauli / tauUniform) / 12.0;

    return lsdaPrime * enhancement - lsda * uPrime / 12.0;
}

// The density derivative at a gradient that is not zero, against the derivative
// derived by hand above.  Every other gate in this file evaluates either at
// sigma = 0 or on a slot the gradient's density term does not reach, so a wrong
// coefficient on that term passes all of them; this is the case that separates
// it.  Both channels carry a gradient and neither is at its uniform tau.
TEST_F(MetaGgaReferenceTest, DensityDerivativeMatchesTheHandDerivedOneAtNonzeroGradient) {
    Point point;
    point.rhoA = 1.3;
    point.rhoB = 0.31;
    point.sigmaAa = 2.7;
    point.sigmaBb = 1.1;
    point.tauA = 1.7;
    point.tauB = 0.5;

    const PointResult result = Evaluate(point);

    const double alphaAnalytic = HandDerivedVrho(point.rhoA, point.sigmaAa, point.tauA);
    const double betaAnalytic = HandDerivedVrho(point.rhoB, point.sigmaBb, point.tauB);

    EXPECT_NEAR(result.first[IndexOf(Component::RhoA)], alphaAnalytic,
                kIdentityTolerance * std::abs(alphaAnalytic));
    EXPECT_NEAR(result.first[IndexOf(Component::RhoB)], betaAnalytic,
                kIdentityTolerance * std::abs(betaAnalytic));

    // The independent route: a central difference of the kernel's own energy
    // density in rho, at fixed sigma and tau.  It shares no algebra with either
    // expression above, which is what makes the two a check rather than one
    // restating the other.
    constexpr double kStep = 1e-5;
    Point ahead = point;
    Point behind = point;
    ahead.rhoA += kStep;
    behind.rhoA -= kStep;

    const double difference = (Evaluate(ahead).exc - Evaluate(behind).exc) / (2.0 * kStep);

    EXPECT_NEAR(result.first[IndexOf(Component::RhoA)], difference, 1e-8 * std::abs(difference));
}

} // namespace
