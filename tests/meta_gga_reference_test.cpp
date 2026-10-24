// e_sigma = e_x^LSDA(rho_sigma)(1 + (1 - tau_P/tau_unif)/12), tau_P = tau_sigma - |grad rho_sigma|^2/(8 rho_sigma), tau_unif = (3/10)(6 pi^2)^(2/3) rho_sigma^(5/3).

#include "excgrid/components.hpp"
#include "excgrid/kernel.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>
#include <limits>
#include <string_view>
#include <utility>

// At namespace scope: a declaration in the anonymous namespace would not bind to the library's definition.
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

constexpr std::string_view kReferenceName = "tau_x";

/// (3/4)(6/pi)^(1/3): the spin-scaled Slater constant, e_x^LSDA = -this * rho^(4/3) per channel.
const double kSlaterSpinPrefactor = 0.75 * std::cbrt(6.0 / kPi);

const double kSixPiSquaredCbrt = std::cbrt(6.0 * kPi * kPi);

/// (3/10)(6 pi^2)^(2/3): tau_sigma = this * rho_sigma^(5/3) for a uniform gas.
const double kFermiConstant = 0.3 * kSixPiSquaredCbrt * kSixPiSquaredCbrt;

/// Round-off only; 1e-14 is above the floor and far below any coefficient error.
constexpr double kIdentityTolerance = 1e-14;

struct Point {
    double rhoA = 0.0;
    double rhoB = 0.0;
    double sigmaAa = 0.0;
    double sigmaAb = 0.0;
    double sigmaBb = 0.0;
    double tauA = 0.0;
    double tauB = 0.0;
};

/// Writes values directly: PointInputs::Set would add what it writes to the mask.
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
    /// The hand-written reference, not the registry's generated kernel under the same name.
    [[nodiscard]] const XcFunctional& Reference() const {
        return excgrid::MetaGgaReferenceFunctional();
    }

    [[nodiscard]] PointResult Evaluate(
        const Point& point, const ComponentMask& mask = ComponentMask::Collinear()) const {
        const PointInputs inputs = MaskedInputs(point, mask);
        PointResult result;
        EXPECT_EQ(Reference().EvaluatePoint(inputs, result), excgrid::KernelStatus::kOk);
        return result;
    }
};

Point UniformGasPoint(double rhoA, double rhoB) {
    Point point;
    point.rhoA = rhoA;
    point.rhoB = rhoB;
    point.tauA = kFermiConstant * std::pow(rhoA, 5.0 / 3.0);
    point.tauB = kFermiConstant * std::pow(rhoB, 5.0 / 3.0);
    return point;
}

// Uniform gas at tau_unif must give LSDA exactly; unequal spins catch a spin-blind normalization.
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

// de/drho = de/drho|tau + (5/3) kFermi rho^(2/3) de/dtau = -(4/3)(3/4)(6/pi)^(1/3) rho^(1/3).
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

// tau_W = sigma/(8 rho); tau_P = tau - tau_W, so de/dsigma_sigma = -(1/(8 rho_sigma)) de/dtau_sigma.
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

        // The cross invariant carries no channel's tau_W: its derivative is exactly zero.
        EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::SigmaAb)], 0.0);
    }
}

// (sigma_sigma, tau_sigma) -> (sigma_sigma + 8 rho delta, tau_sigma + delta) leaves tau_P fixed.
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

        // tau_P is invariant; only the two moves' round-off remains, a few ulps.
        EXPECT_NEAR(result.exc, anchored.exc, 1e-15 * std::abs(anchored.exc)) << "delta = " << delta;

        EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::SigmaAa)],
                         anchored.first[IndexOf(Component::SigmaAa)])
            << "delta = " << delta;
        EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::TauA)],
                         anchored.first[IndexOf(Component::TauA)])
            << "delta = " << delta;
    }
}

// e_x = e_x^LSDA (1 + (10/81) s^2 + O(grad^4)), s = |grad rho|/(2 (6 pi^2)^(1/3) rho^(4/3)); tau_P/tau_unif = 1 - (40/27) s^2, so (40/27)/12 = 10/81.
TEST_F(MetaGgaReferenceTest, GradientExpansionCoefficientIsTheExactTenOverEightyOne) {
    constexpr double kCoefficientTolerance = 1e-9;
    constexpr double kExactCoefficient = 10.0 / 81.0;

    for (const double rho : {0.3, 1.4})
    {
        for (const double sSquared : {1e-3, 1e-4})
        {
            Point point;
            point.rhoA = rho;
            // sigma = |grad rho|^2 from s; tau from the second-order kinetic-energy expansion.
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

// Same buffer, tau bits differ: an input outside the mask contributes exactly zero.
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

// Tau inside the mask moves exc; the returned tau slot follows the mask, not tau's value.
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

    // exc differs by (1/12) of the channel's LSDA energy density less tau_W: 0.0404 here.
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

// Linear in tau, so the central difference has no truncation error; 1e-3 sits above the round-off floor, 1e-9 below it.
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

/// de/drho = e_x^LSDA'(rho)(1 + (1-u)/12) - e_x^LSDA(rho) u'/12, u = tau_P/tau_unif.
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

// Neither channel at sigma = 0 or uniform tau: this separates the gradient term of de/drho.
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

    // Central difference of the kernel's own energy: shares no algebra with the hand-derived expression.
    constexpr double kStep = 1e-5;
    Point ahead = point;
    Point behind = point;
    ahead.rhoA += kStep;
    behind.rhoA -= kStep;

    const double difference = (Evaluate(ahead).exc - Evaluate(behind).exc) / (2.0 * kStep);

    EXPECT_NEAR(result.first[IndexOf(Component::RhoA)], difference, 1e-8 * std::abs(difference));
}

} // namespace
