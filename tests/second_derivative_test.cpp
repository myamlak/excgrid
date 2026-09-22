// The second-derivative tier's verification suite.
//
// The standing rule is that every functional family passes at least one named
// exact condition, and the tier does not inherit the energy's.  The derivative
// of an exact relation is exact, so an energy-level condition says nothing
// about a tier unless the tier is asked for a condition of its own - and the
// obvious check, finite differences of the kernel's OWN first derivatives,
// verifies the differentiation and is blind to the functional being wrong,
// because both sides come from the same expression.
//
// So three channels, and this file carries all three:
//
//   * the tier's own exact condition at a uniform electron gas, where the
//     answer is known in closed form - the density curvature is non-zero, and
//     the curvature in the gradient and the kinetic-energy density vanishes for
//     a functional that is linear in them, which the second-order gradient
//     expansion is;
//   * comparison against analytic second derivatives derived OUTSIDE the
//     generator - the LDA exchange curvature, and the closed-form curvatures of
//     the second-order gradient expansion, including the differentiated von
//     Weizsaecker relation between its sigma and tau entries;
//   * finite differences of the kernel's own first derivatives, kept because it
//     is the only channel that covers a functional this file has no closed form
//     for.

#include "excgrid/kernel.hpp"
#include "excgrid/kernels.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <string_view>
#include <gtest/gtest.h>

namespace {

constexpr double kPi = 3.14159265358979323846;

// The Dirac exchange constant as the library's own definitions print it.  The
// generated sources carry this literal, so a closed-form expectation written
// with it is comparable at the last digit the generator emits.
constexpr double kCx = 0.9305257363491;

// The second derivative of the LSDA exchange energy density
//   e = -Cx rho^(4/3)  =>  d2e/drho2 = -(4/9) Cx rho^(-2/3).
double SlaterCurvature(double rho) { return -(4.0 / 9.0) * kCx * std::pow(rho, -2.0 / 3.0); }

// The uniform-gas kinetic-energy density of one spin channel, the same
// (3/10)(6 pi^2)^(2/3) rho^(5/3) the tier's definitions use.
double TauUniform(double rho) {
    return 0.3 * std::pow(6.0 * kPi * kPi, 2.0 / 3.0) * std::pow(rho, 5.0 / 3.0);
}

// The contract's upper-triangle index over `active` components in identifier
// order, restated here so the expectations below are written against the
// packing the header documents rather than against a helper out of the library.
constexpr std::size_t Packed(std::size_t i, std::size_t j, std::size_t active) {
    return i * active - i * (i - 1) / 2 + (j - i);
}

// One tier call's matrix, by the kernel's own argument order.
excgrid::PointSecondDerivativeMatrix LdaMatrix(excgrid::LdaSecondDerivative tier,
                                               double rhoA,
                                               double rhoB) {
    excgrid::PointSecondDerivativeMatrix matrix;
    tier(rhoA, rhoB, matrix);
    return matrix;
}

excgrid::PointSecondDerivativeMatrix GgaMatrix(excgrid::GgaSecondDerivative tier,
                                               double rhoA,
                                               double rhoB,
                                               double sigmaAa,
                                               double sigmaAb,
                                               double sigmaBb) {
    excgrid::PointSecondDerivativeMatrix matrix;
    tier(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb, matrix);
    return matrix;
}

// The tau tier is reached through the registry rather than through a named
// kernel: its kernel is wider than the two kernel-pointer types the header
// publishes, so the generated function is named where the registry builds it
// and not in excgrid/kernels.hpp.
excgrid::PointInputs TauPoint(double rhoA,
                              double rhoB,
                              double sigmaAa,
                              double sigmaAb,
                              double sigmaBb,
                              double tauA,
                              double tauB) {
    excgrid::PointInputs inputs;
    inputs.Set(excgrid::Component::RhoA, rhoA);
    inputs.Set(excgrid::Component::RhoB, rhoB);
    inputs.Set(excgrid::Component::SigmaAa, sigmaAa);
    inputs.Set(excgrid::Component::SigmaAb, sigmaAb);
    inputs.Set(excgrid::Component::SigmaBb, sigmaBb);
    inputs.Set(excgrid::Component::TauA, tauA);
    inputs.Set(excgrid::Component::TauB, tauB);
    return inputs;
}

excgrid::PointSecondDerivativeMatrix TauMatrix(double rhoA,
                                               double rhoB,
                                               double sigmaAa,
                                               double sigmaAb,
                                               double sigmaBb,
                                               double tauA,
                                               double tauB) {
    excgrid::PointSecondDerivativeMatrix matrix;
    excgrid::PointResult result;

    // The caller's own expectations are what fails if this did not answer; the
    // status is consumed here only so a refusal is not silently a zero matrix.
    const excgrid::KernelStatus status = excgrid::FindFunctional("tau_x")->EvaluatePointMaterialising(
        TauPoint(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb, tauA, tauB), result, matrix);
    EXPECT_EQ(status, excgrid::KernelStatus::kOk);

    return matrix;
}

// A point's components in identifier order, and their values.
struct TierPoint {
    excgrid::ComponentMask mask{};
    std::array<excgrid::Component, excgrid::kComponentCapacity> ids{};
    std::array<double, excgrid::kComponentCapacity> values{};
    std::size_t active = 0;
};

TierPoint MakePoint(const excgrid::ComponentMask& mask,
                    const std::array<std::pair<excgrid::Component, double>,
                                     excgrid::kComponentCapacity>& supplied) {
    TierPoint point;
    point.mask = mask;

    for (std::size_t component = 0; component < excgrid::kComponentCapacity; ++component)
    {
        const auto id = static_cast<excgrid::Component>(component);

        if (!mask.Test(id))
        {
            continue;
        }

        point.ids[point.active] = id;
        point.values[point.active] = 0.0;

        // The first match wins.  The supplied array is value-initialized past its
        // own entries, so every trailing slot names component zero with a zero
        // value - scanning the whole array would overwrite the first component's
        // value with that zero.
        for (const auto& [named, value] : supplied)
        {
            if (named == id)
            {
                point.values[point.active] = value;
                break;
            }
        }

        ++point.active;
    }

    return point;
}

excgrid::PointInputs InputsOf(const TierPoint& point) {
    excgrid::PointInputs inputs;

    for (std::size_t i = 0; i < point.active; ++i)
    {
        inputs.Set(point.ids[i], point.values[i]);
    }

    return inputs;
}

// The channel that is kept: the emitted matrix against central differences of
// the kernel's OWN first derivatives, through the registry - so it covers the
// composed functionals too, and their terms' weights with them.  It is blind to
// the energy expression being wrong, which is why it is not the only channel.
//
// The step is scaled off the perturbed value, and the sampled points keep every
// gradient invariant well away from zero: the kernels take the square root of
// the invariants, so a central difference through a negative one is not a
// number at all.
void CheckTierAgainstFiniteDifference(const excgrid::XcFunctional& functional,
                                      const TierPoint& point,
                                      std::string_view label) {
    const excgrid::PointInputs inputs = InputsOf(point);

    excgrid::PointResult result;
    excgrid::PointSecondDerivativeMatrix matrix;
    ASSERT_EQ(functional.EvaluatePointMaterialising(inputs, result, matrix),
              excgrid::KernelStatus::kOk)
        << label;

    const auto firstAt = [&](std::size_t slot, double value) {
        excgrid::PointInputs shifted = inputs;
        shifted.Set(point.ids[slot], value);

        excgrid::PointResult shiftedResult;
        EXPECT_EQ(functional.EvaluatePoint(shifted, shiftedResult), excgrid::KernelStatus::kOk)
            << label;
        return shiftedResult;
    };

    for (std::size_t i = 0; i < point.active; ++i)
    {
        for (std::size_t j = i; j < point.active; ++j)
        {
            const double scale = std::max(1.0, std::abs(point.values[j]));
            const double step = 1e-5 * scale;

            const double plus = firstAt(j, point.values[j] + step).first[excgrid::IndexOf(point.ids[i])];
            const double minus =
                firstAt(j, point.values[j] - step).first[excgrid::IndexOf(point.ids[i])];
            const double difference = (plus - minus) / (2.0 * step);

            const double emitted = matrix.upper[Packed(i, j, point.active)];
            const double tolerance = 1e-5 * std::max(1.0, std::abs(difference));

            EXPECT_NEAR(emitted, difference, tolerance)
                << label << " entry (" << i << "," << j << ") of " << matrix.mask.bits;
        }
    }
}

// A generic, well-conditioned point for every family.
constexpr double kRhoA = 0.31;
constexpr double kRhoB = 0.22;
constexpr double kSigmaAa = 0.7;
constexpr double kSigmaAb = 0.15;
constexpr double kSigmaBb = 0.45;

std::array<std::pair<excgrid::Component, double>, excgrid::kComponentCapacity> GenericValues() {
    return {{{excgrid::Component::RhoA, kRhoA},
             {excgrid::Component::RhoB, kRhoB},
             {excgrid::Component::SigmaAa, kSigmaAa},
             {excgrid::Component::SigmaAb, kSigmaAb},
             {excgrid::Component::SigmaBb, kSigmaBb},
             {excgrid::Component::TauA, TauUniform(kRhoA)},
             {excgrid::Component::TauB, TauUniform(kRhoB)}}};
}

const std::array<std::string_view, 24> kEveryShippedName = {
    "slater", "vwn5",   "vwn3", "pw92",   "svwn",     "spw92",     "becke88", "pw91",
    "pbe",    "revpbe", "rpbe", "mpw91",  "pbesol",   "lyp",       "pbe_c",   "pw91_c",
    "p86",    "b3lyp",  "pbe0", "b3pw91", "mpw1pw91", "bhandhlyp", "b3p86",   "tau_x",
};

// The names whose kernels ship a second-derivative tier.  A name is absent when
// the symbolic second differentiation of its energy density is too expensive to
// carry, and for the exchange forms that turns on the enhancement: the five that
// are rational functions of s^2 alone generate in under two minutes on the
// development machine, mpw91's - which carries Exp and Asinh - in about three
// and a half, and pw91's, which carries both inside a rational function, was
// still running at 22 minutes and was abandoned.  For the correlation forms it
// is every one of them.  A recipe takes a tier only when every one of its
// terms has one, so a hybrid of a tiered exchange with an untiered correlation is
// absent too.
// `pbesol` is absent for a different reason from the ones above it: its tier
// generates quickly but is not finite at an exactly zero gradient in the two
// sigma-sigma entries, where the kernel itself is.  A tier that has a hole at a
// point its own kernel answers is not shippable, and the shape that would need
// repairing is a sum at the top of the differentiated tree, which the
// generator's cancellation deliberately does not rewrite.
const std::array<std::string_view, 7> kEveryTieredName = {
    "slater", "becke88", "pbe", "revpbe", "rpbe", "mpw91", "tau_x",
};

// Every other shipped name, which is where the refusal grain has to hold: a
// caller asking one of these for a derivative gets "no tier", not a matrix.
const std::array<std::string_view, 17> kEveryUntieredName = {
    "vwn5",  "vwn3",   "pw92",     "svwn",  "spw92",    "pw91",  "lyp",   "pbe_c",
    "pw91_c", "p86",   "b3lyp",    "pbe0",  "b3pw91",   "mpw1pw91", "bhandhlyp", "b3p86",
    "pbesol",
};

} // namespace

// ---------------------------------------------------------------------------
// The tier's own exact condition: the uniform electron gas
// ---------------------------------------------------------------------------

// The second derivative with respect to the density, at a uniform gas, for the
// functional whose curvature is known in closed form.  This is the tier's
// anchor: the number below is the analytic one, not a remembered measurement.
TEST(SecondDerivativeTest, SlaterDensityCurvatureIsTheClosedForm) {
    const excgrid::PointSecondDerivativeMatrix matrix =
        LdaMatrix(excgrid::SlaterExchangeSecondDerivatives, kRhoA, kRhoB);

    // The LSDA exchange energy density is a sum of two independent spin
    // channels, so the cross curvature is exactly zero - not nearly zero.
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(0, 0, 2)], SlaterCurvature(kRhoA));
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(1, 1, 2)], SlaterCurvature(kRhoB));
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(0, 1, 2)], 0.0);

    EXPECT_LT(matrix.upper[Packed(0, 0, 2)], 0.0);
    EXPECT_NE(matrix.upper[Packed(1, 1, 2)], 0.0);
}

// A GGA does not inherit that: its enhancement is flat to first order in the
// gradient but not to second, so the density curvature at a zero gradient is
// the LSDA one exactly, the correction contributing nothing at order zero.  A
// functional whose tier got this wrong would still pass the finite-difference
// channel, because that channel differentiates the same expression the kernel
// returns.
TEST(SecondDerivativeTest, PbeDensityCurvatureAtZeroGradientIsTheLsdaOne) {
    const excgrid::PointSecondDerivativeMatrix matrix =
        GgaMatrix(excgrid::PbeExchangeSecondDerivatives, kRhoA, kRhoB, 0.0, 0.0, 0.0);

    const double tolerance = 1e-12 * std::abs(SlaterCurvature(kRhoA));
    EXPECT_NEAR(matrix.upper[Packed(0, 0, 5)], SlaterCurvature(kRhoA), tolerance);
    EXPECT_NEAR(matrix.upper[Packed(1, 1, 5)], SlaterCurvature(kRhoB), tolerance);
}

// The tier's own exact condition, asserted where it is true and in the form it
// is true in.
//
// The tier's own functional is the second-order gradient expansion of exchange
// in the iso-orbital variables: it is LINEAR in the gradient invariant and in
// the kinetic-energy density, so its curvature in both vanishes identically,
// while its density curvature does not.  At a uniform gas it reduces to the
// LSDA exchange exactly, so its density block is the LSDA curvature above.
TEST(SecondDerivativeTest, TheGradientExpansionIsFlatInTheGradientAndTheKineticDensity) {
    const double rhoA = 0.4;
    const double rhoB = 0.4;
    const double tauA = TauUniform(rhoA);
    const double tauB = TauUniform(rhoB);

    const excgrid::PointSecondDerivativeMatrix matrix =
        TauMatrix(rhoA, rhoB, 0.0, 0.0, 0.0, tauA, tauB);

    constexpr std::size_t kActive = 7;
    constexpr std::size_t rhoASlot = 0;
    constexpr std::size_t rhoBSlot = 1;
    constexpr std::size_t sigmaAaSlot = 2;
    constexpr std::size_t sigmaAbSlot = 3;
    constexpr std::size_t sigmaBbSlot = 4;
    constexpr std::size_t tauASlot = 5;
    constexpr std::size_t tauBSlot = 6;

    // NON-ZERO: the density curvature, and it is the LSDA one.
    EXPECT_NEAR(matrix.upper[Packed(rhoASlot, rhoASlot, kActive)], SlaterCurvature(rhoA), 1e-12);
    EXPECT_NEAR(matrix.upper[Packed(rhoBSlot, rhoBSlot, kActive)], SlaterCurvature(rhoB), 1e-12);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(rhoASlot, rhoBSlot, kActive)], 0.0);

    // VANISHING: every curvature that is second order in the gradient, or in
    // the kinetic-energy density, or mixed between them.  Exactly zero, because
    // the functional is linear in each of those variables.
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(sigmaAaSlot, sigmaAaSlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(sigmaAbSlot, sigmaAbSlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(sigmaBbSlot, sigmaBbSlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(sigmaAaSlot, sigmaAbSlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(sigmaAaSlot, sigmaBbSlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(sigmaAbSlot, sigmaBbSlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(tauASlot, tauASlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(tauBSlot, tauBSlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(tauASlot, tauBSlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(sigmaAaSlot, tauASlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(sigmaAbSlot, tauASlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(sigmaBbSlot, tauASlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(sigmaAaSlot, tauBSlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(sigmaAbSlot, tauBSlot, kActive)], 0.0);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(sigmaBbSlot, tauBSlot, kActive)], 0.0);

    // NON-ZERO: the mixed density-gradient and density-kinetic entries, whose
    // closed forms follow from  e = -Cx rho^(4/3) (1 + (1 - tauP/tauUnif)/12)
    // with tauP = tau - sigma/(8 rho) and tauUnif = (3/10)(6 pi^2)^(2/3) rho^(5/3):
    //
    //   de/dsigma = -Cx rho^(4/3) / (96 rho tauUnif),
    //   de/dtau   = +Cx rho^(4/3) / (12 tauUnif),
    //
    // so d2e/drho dsigma = (4/3) Cx / (28.8 (6 pi^2)^(2/3)) rho^(-7/3) and
    //    d2e/drho dtau   = -(1/3) Cx / (3.6 (6 pi^2)^(2/3)) rho^(-4/3).
    const double kSixPiSquared = 6.0 * kPi * kPi;
    const double densityGradient =
        (4.0 / 3.0) * kCx / (28.8 * std::pow(kSixPiSquared, 2.0 / 3.0)) * std::pow(rhoA, -7.0 / 3.0);
    const double densityKinetic = -(1.0 / 3.0) * kCx / (3.6 * std::pow(kSixPiSquared, 2.0 / 3.0)) *
                                  std::pow(rhoA, -4.0 / 3.0);

    EXPECT_NE(matrix.upper[Packed(rhoASlot, sigmaAaSlot, kActive)], 0.0);
    EXPECT_NE(matrix.upper[Packed(rhoASlot, tauASlot, kActive)], 0.0);
    EXPECT_NEAR(matrix.upper[Packed(rhoASlot, sigmaAaSlot, kActive)], densityGradient, 1e-12);
    EXPECT_NEAR(matrix.upper[Packed(rhoASlot, tauASlot, kActive)], densityKinetic, 1e-12);

    EXPECT_NE(matrix.upper[Packed(rhoBSlot, sigmaBbSlot, kActive)], 0.0);
    EXPECT_NE(matrix.upper[Packed(rhoBSlot, tauBSlot, kActive)], 0.0);
    EXPECT_NEAR(matrix.upper[Packed(rhoBSlot, sigmaBbSlot, kActive)],
                (4.0 / 3.0) * kCx / (28.8 * std::pow(kSixPiSquared, 2.0 / 3.0)) *
                    std::pow(rhoB, -7.0 / 3.0),
                1e-12);
    EXPECT_NEAR(matrix.upper[Packed(rhoBSlot, tauBSlot, kActive)],
                -(1.0 / 3.0) * kCx / (3.6 * std::pow(kSixPiSquared, 2.0 / 3.0)) *
                    std::pow(rhoB, -4.0 / 3.0),
                1e-12);
}

// The differentiated von Weizsaecker relation, which is a second-derivative
// statement the first-derivative layer cannot make.
//
// The functional reads sigma and tau only through tauP = tau - sigma/(8 rho),
// so de/dsigma = -(1/(8 rho)) de/dtau identically.  Differentiating THAT with
// respect to the density relates three MATRIX entries to one first derivative:
//
//   d2e/drho dsigma + (1/(8 rho)) d2e/drho dtau - (1/(8 rho^2)) de/dtau = 0.
//
// It holds at any point, not only at a uniform gas, because it follows from how
// the functional is built rather than from a limit - so it is asserted at a
// point with every variable off its own edge.
TEST(SecondDerivativeTest, TheDifferentiatedVonWeizsaeckerRelationHolds) {
    constexpr std::size_t kActive = 7;
    constexpr std::size_t rhoASlot = 0;
    constexpr std::size_t sigmaAaSlot = 2;
    constexpr std::size_t tauASlot = 5;

    const double rhoA = 0.37;
    const double rhoB = 0.21;
    const double sigmaAa = 0.42;
    const double sigmaBb = 0.18;
    const double tauA = 0.9 * TauUniform(rhoA);
    const double tauB = 0.9 * TauUniform(rhoB);

    const excgrid::PointSecondDerivativeMatrix matrix =
        TauMatrix(rhoA, rhoB, sigmaAa, 0.0, sigmaBb, tauA, tauB);

    excgrid::PointResult first;
    const excgrid::KernelStatus status = excgrid::FindFunctional("tau_x")->EvaluatePoint(
        TauPoint(rhoA, rhoB, sigmaAa, 0.0, sigmaBb, tauA, tauB), first);
    EXPECT_EQ(status, excgrid::KernelStatus::kOk);
    const double vtauA = first.first[excgrid::IndexOf(excgrid::Component::TauA)];

    const double residual =
        matrix.upper[Packed(rhoASlot, sigmaAaSlot, kActive)] +
        matrix.upper[Packed(rhoASlot, tauASlot, kActive)] / (8.0 * rhoA) -
        vtauA / (8.0 * rhoA * rhoA);

    EXPECT_NE(vtauA, 0.0);
    EXPECT_NEAR(residual, 0.0, 1e-12 * std::abs(vtauA / (8.0 * rhoA * rhoA)));
}

// ---------------------------------------------------------------------------
// The channel that is kept: finite differences of the kernel's own derivatives
// ---------------------------------------------------------------------------

// Every tiered functional, on the tier, against central differences of the first
// derivatives the same functional returns.  Each functional is sampled over its
// OWN required mask, so an LDA functional is checked on its two densities, a GGA
// one on its five components and `tau_x` on its seven - which is also what makes
// this the test that catches a tier packed at the wrong width.  This is the check
// that the emitted matrix really is the derivative of the kernel; it is not the
// only check, because both sides of it come from the same energy expression.
TEST(SecondDerivativeTest, TheMatrixIsTheDerivativeOfTheKernelsOwnFirstDerivatives) {
    const auto supplied = GenericValues();

    for (const std::string_view name : kEveryTieredName)
    {
        const excgrid::XcFunctional* functional = excgrid::FindFunctional(name);
        ASSERT_NE(functional, nullptr);

        const TierPoint point = MakePoint(functional->RequiredMask(), supplied);
        EXPECT_EQ(point.active, functional->RequiredMask().ActiveCount()) << name;

        CheckTierAgainstFiniteDifference(*functional, point, name);
    }
}

// The entries that span a gradient invariant are the ones the differentiation
// makes removable at an exactly zero gradient, so the corner gets its own
// check: the matrix there must be finite, and it must be the limit rather than
// merely near it - a value that only approaches the limit has a hole at the
// point a caller is most likely to hand over.
//
// The requirement is placed where it can hold: the tier must be finite at the
// corner wherever the kernel it differentiates is, and no further.  The two
// names below are the exception, and they are the order-1 layer's exception
// rather than the tier's - their enhancement's derivative carries
// asinh(x)/sqrt(sigma), a quotient whose numerator vanishes like x without
// being a product that carries the radical, so there is no common factor for
// the generator's cancellation to remove.  That list is checked against the
// kernels instead of trusted, so it cannot go stale in either direction.
const std::array<std::string_view, 2> kOrderOneSingularAtZeroGradient = {"becke88", "mpw91"};

TEST(SecondDerivativeTest, TheZeroGradientCornerIsAnOrdinaryPointOfTheTier) {
    constexpr double kNearlyZeroGradient = 1e-30;

    for (const std::string_view name : kEveryTieredName)
    {
        const excgrid::XcFunctional* functional = excgrid::FindFunctional(name);
        ASSERT_NE(functional, nullptr);

        const auto cornerInputs = [](double sigma) {
            excgrid::PointInputs inputs;
            inputs.Set(excgrid::Component::RhoA, kRhoA);
            inputs.Set(excgrid::Component::RhoB, kRhoB);
            inputs.Set(excgrid::Component::SigmaAa, sigma);
            inputs.Set(excgrid::Component::SigmaAb, sigma);
            inputs.Set(excgrid::Component::SigmaBb, sigma);
            return inputs;
        };

        // Whether the kernel itself answers at the corner, measured through the
        // registry over exactly the components the mask carries.
        excgrid::PointResult kernelAtZero;
        ASSERT_EQ(functional->EvaluatePoint(cornerInputs(0.0), kernelAtZero),
                  excgrid::KernelStatus::kOk)
            << name;

        bool kernelFinite = true;
        for (std::size_t i = 0; i < excgrid::kComponentCapacity; ++i)
        {
            const auto id = static_cast<excgrid::Component>(i);
            if (functional->RequiredMask().Test(id))
            {
                kernelFinite =
                    kernelFinite && std::isfinite(kernelAtZero.first[excgrid::IndexOf(id)]);
            }
        }

        const bool pinnedSingular =
            std::find(kOrderOneSingularAtZeroGradient.begin(), kOrderOneSingularAtZeroGradient.end(),
                      name) != kOrderOneSingularAtZeroGradient.end();
        EXPECT_EQ(kernelFinite, !pinnedSingular) << name;

        const auto corner = [&](double sigma) {
            excgrid::PointResult result;
            excgrid::PointSecondDerivativeMatrix matrix;
            EXPECT_EQ(functional->EvaluatePointMaterialising(cornerInputs(sigma), result, matrix),
                      excgrid::KernelStatus::kOk)
                << name;
            return matrix;
        };

        const excgrid::PointSecondDerivativeMatrix atZero = corner(0.0);
        const excgrid::PointSecondDerivativeMatrix adjacent = corner(kNearlyZeroGradient);

        // An entry the tier does answer must be the limit and not merely near
        // it; an entry it does not answer is permitted only where the kernel has
        // already stopped answering, and never silently - so the singular case
        // has to show at least one entry carrying the kernel's own hole.
        bool anyHole = false;
        for (std::size_t k = 0; k < atZero.upper.size(); ++k)
        {
            if (!std::isfinite(atZero.upper[k]))
            {
                anyHole = true;
                EXPECT_FALSE(kernelFinite) << name << " entry " << k;
                continue;
            }

            EXPECT_NEAR(atZero.upper[k], adjacent.upper[k],
                        1e-9 * std::max(1.0, std::abs(adjacent.upper[k])))
                << name << " entry " << k;
        }
        EXPECT_EQ(anyHole, !kernelFinite) << name;
    }
}

// ---------------------------------------------------------------------------
// The uniform-gas condition where it does NOT hold, stated as a measurement
// ---------------------------------------------------------------------------

// A full GGA is not linear in the gradient invariant, so its curvature there
// does not vanish and the condition stated for the energy density of a uniform
// gas does not carry over to it.  The number below is the analytic one, derived
// from the published enhancement alone:
//
//   F(s) = 1 + kappa - kappa / (1 + mu s^2 / kappa),  s^2 = sigmaAa / (kF(2 rhoA) 2 rhoA)^2,
//
// on the alpha channel's own term rhoA epsX(2 rhoA) F(s).  F'(s) = 2 mu s - 4 mu^2 s^3 / kappa
// near the origin, so de/dsigmaAa = rhoA epsX (mu - 2 mu^2 s^2 / kappa) / radial^2, and
// the curvature at sigmaAa = 0 is
//
//   -2 mu^2 rhoA epsX(2 rhoA) / (kappa radial^4) = +2 mu^2 Cx rhoA^(4/3) / (kappa radial^4),
//
// positive, because the energy density it multiplies is negative.  The Cx is
// already the spin-summed coefficient the tree's other exchange kernels carry -
// rho epsX(2 rho) = -Cx rho^(4/3) with it - so no further spin factor enters
// here.  It is asserted so the difference from the second-order gradient
// expansion is a fact in the suite rather than an assumption.
TEST(SecondDerivativeTest, PbeGradientCurvatureAtAUniformGasIsItsAnalyticValue) {
    constexpr double kMu = 0.2195149727645171;
    constexpr double kKappa = 0.804;

    const excgrid::PointSecondDerivativeMatrix matrix =
        GgaMatrix(excgrid::PbeExchangeSecondDerivatives, kRhoA, kRhoB, 0.0, 0.0, 0.0);

    const double kFermi = std::pow(3.0 * kPi * kPi * 2.0 * kRhoA, 1.0 / 3.0);
    const double radial = kFermi * 2.0 * kRhoA;

    // The alpha channel's own contribution at a zero gradient, rhoA epsX(2 rhoA),
    // which is the LSDA exchange energy density of a channel holding all of rhoA.
    const double channelEnergy = -kCx * std::pow(kRhoA, 4.0 / 3.0);
    const double analytic =
        -2.0 * channelEnergy * kMu * kMu / (kKappa * std::pow(radial, 4.0));

    EXPECT_GT(analytic, 0.0);
    EXPECT_NE(analytic, 0.0);

    EXPECT_NEAR(matrix.upper[Packed(2, 2, 5)], analytic, 1e-10 * std::abs(analytic));
}

// ---------------------------------------------------------------------------
// The capability report
// ---------------------------------------------------------------------------

// What a caller can ask before it asks for numbers: which components a tier
// covers.  A tiered functional's report equals its required mask - asserted
// rather than assumed, because it is the fact that keeps the two refusals
// distinguishable - and an untiered one reports that it spans nothing at all.
TEST(SecondDerivativeTest, TheCoverageReportNamesWhatEachTierSpans) {
    // The three lists below are hand-written, so they are pinned to the registry
    // first: a functional added later has to be placed in one of them
    // deliberately rather than falling outside every loop here.
    {
        std::size_t shipped = 0;
        for (const std::string_view name : excgrid::FunctionalNames())
        {
            ++shipped;
            EXPECT_NE(std::find(kEveryShippedName.begin(), kEveryShippedName.end(), name),
                      kEveryShippedName.end())
                << name;
        }
        EXPECT_EQ(shipped, kEveryShippedName.size());
    }

    for (const std::string_view name : kEveryTieredName)
    {
        const excgrid::XcFunctional* functional = excgrid::FindFunctional(name);
        ASSERT_NE(functional, nullptr);

        const excgrid::ComponentMask spanned = functional->SecondDerivativeMask();
        const excgrid::ComponentMask required = functional->RequiredMask();

        EXPECT_EQ(spanned.bits, required.bits) << name;
        EXPECT_NE(spanned.ActiveCount(), 0u) << name;
        EXPECT_EQ(excgrid::SecondDerivativeStatus(required, required, spanned),
                  excgrid::KernelStatus::kOk)
            << name;
    }

    // A functional with no tier says so by spanning nothing, and every
    // component it reads then reads as uncovered rather than as answered.
    for (const std::string_view name : kEveryUntieredName)
    {
        const excgrid::XcFunctional* functional = excgrid::FindFunctional(name);
        ASSERT_NE(functional, nullptr);

        const excgrid::ComponentMask spanned = functional->SecondDerivativeMask();
        const excgrid::ComponentMask required = functional->RequiredMask();

        EXPECT_EQ(spanned.ActiveCount(), 0u) << name;
        EXPECT_EQ(excgrid::UnspannedRequestComponents(required, required, spanned).bits,
                  required.bits)
            << name;
        EXPECT_EQ(excgrid::SecondDerivativeStatus(required, required, spanned),
                  excgrid::KernelStatus::kRefusedUnsupportedCapability)
            << name;
    }

    // And the widths are the families' own, which is what the packing depends
    // on: two components for an LDA kernel, five for a GGA, seven for the tau
    // tier.  The tiered sets are disjoint and cover every shipped name, so a
    // functional that gains a tier cannot be silently left out of both.
    EXPECT_EQ(excgrid::FindFunctional("slater")->SecondDerivativeMask().ActiveCount(), 2u);
    EXPECT_EQ(excgrid::FindFunctional("pbe")->SecondDerivativeMask().ActiveCount(), 5u);
    EXPECT_EQ(excgrid::FindFunctional("tau_x")->SecondDerivativeMask().ActiveCount(), 7u);

    EXPECT_EQ(kEveryTieredName.size() + kEveryUntieredName.size(), kEveryShippedName.size());

    for (const std::string_view name : kEveryShippedName)
    {
        const bool tiered = std::find(kEveryTieredName.begin(), kEveryTieredName.end(), name)
                            != kEveryTieredName.end();
        const bool untiered = std::find(kEveryUntieredName.begin(), kEveryUntieredName.end(), name)
                              != kEveryUntieredName.end();

        EXPECT_NE(tiered, untiered) << name;
    }
}

// The refusal's finer grain, on the seam itself: a tier that spans less than the
// functional reads reports the uncovered components rather than pretending to
// answer, and it is a DIFFERENT refusal from having no tier at all.
TEST(SecondDerivativeTest, AnUncoveredComponentIsRefusedByItsOwnName) {
    excgrid::ComponentMask spanned;
    spanned.Set(excgrid::Component::RhoA);
    spanned.Set(excgrid::Component::RhoB);

    const excgrid::ComponentMask required = excgrid::ComponentMask::Collinear();
    const excgrid::ComponentMask requested = excgrid::ComponentMask::Collinear();

    const excgrid::ComponentMask uncovered =
        excgrid::UnspannedRequestComponents(requested, required, spanned);
    EXPECT_EQ(uncovered.ActiveCount(), 5u);
    EXPECT_TRUE(uncovered.Test(excgrid::Component::SigmaAa));
    EXPECT_TRUE(uncovered.Test(excgrid::Component::TauB));
    EXPECT_FALSE(uncovered.Test(excgrid::Component::RhoA));

    EXPECT_EQ(excgrid::SecondDerivativeStatus(requested, required, spanned),
              excgrid::KernelStatus::kRefusedSecondDerivativeCoverage);

    // A component the functional does not read is not part of the request, so a
    // wide caller mask against a narrow functional is not a refusal: the tier
    // spans everything that functional reads.
    EXPECT_EQ(excgrid::UnspannedRequestComponents(requested, spanned, spanned).bits, 0u);
    EXPECT_EQ(excgrid::SecondDerivativeStatus(requested, spanned, spanned),
              excgrid::KernelStatus::kOk);

    // And no tier at all is the other refusal, not this one.
    EXPECT_EQ(excgrid::SecondDerivativeStatus(requested, required, excgrid::ComponentMask{}),
              excgrid::KernelStatus::kRefusedUnsupportedCapability);
}

// ---------------------------------------------------------------------------
// Composition
// ---------------------------------------------------------------------------

// A composed functional's Hessian would be the weighted sum of its terms', so a
// recipe can only carry a tier when every term carries its own.  b3lyp mixes a
// tiered exchange with untiered correlations, so it must refuse rather than
// return the sum of the terms it happens to have - a matrix missing a term's
// contribution is a wrong number, not a smaller one.
TEST(SecondDerivativeTest, ARecipeWithAnUntieredTermRefusesRatherThanSummingPartOfItself) {
    const excgrid::XcFunctional* hybrid = excgrid::FindFunctional("b3lyp");
    ASSERT_NE(hybrid, nullptr);

    const excgrid::PointInputs inputs =
        InputsOf(MakePoint(excgrid::ComponentMask::Collinear(), GenericValues()));

    EXPECT_EQ(hybrid->SecondDerivativeMask().ActiveCount(), 0u);

    excgrid::PointResult result;
    excgrid::PointSecondDerivativeMatrix matrix;
    EXPECT_EQ(hybrid->EvaluatePointMaterialising(inputs, result, matrix),
              excgrid::KernelStatus::kRefusedUnsupportedCapability);

    excgrid::PointSecondDerivative second;
    const std::array<double, 5> rightHandSide{1.0, 0.0, 0.0, 0.0, 0.0};
    EXPECT_EQ(hybrid->EvaluatePointWithSecondDerivatives(inputs, rightHandSide, result, second),
              excgrid::KernelStatus::kRefusedUnsupportedCapability);

    // The order-1 answer is untouched by any of this: the refusal is the tier's
    // alone, and a caller that never asks for a derivative still gets its value.
    excgrid::PointResult plain;
    EXPECT_EQ(hybrid->EvaluatePoint(inputs, plain), excgrid::KernelStatus::kOk);
    EXPECT_NE(plain.first[excgrid::IndexOf(excgrid::Component::RhoA)], 0.0);
}
