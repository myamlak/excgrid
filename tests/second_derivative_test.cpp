
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

// The Dirac constant as the generated sources print it.
constexpr double kCx = 0.9305257363491;

// e = -Cx rho^(4/3) => d2e/drho2 = -(4/9) Cx rho^(-2/3).
double SlaterCurvature(double rho) { return -(4.0 / 9.0) * kCx * std::pow(rho, -2.0 / 3.0); }

// (3/10)(6 pi^2)^(2/3) rho^(5/3), as the tier's definitions use.
double TauUniform(double rho) {
    return 0.3 * std::pow(6.0 * kPi * kPi, 2.0 / 3.0) * std::pow(rho, 5.0 / 3.0);
}

// Upper-triangle index: i*active - i*(i-1)/2 + (j-i).
constexpr std::size_t Packed(std::size_t i, std::size_t j, std::size_t active) {
    return i * active - i * (i - 1) / 2 + (j - i);
}

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

// Reached through the registry: no kernel-pointer type covers the wider tau kernel.
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

    const excgrid::KernelStatus status = excgrid::FindFunctional("tau_x")->EvaluatePointMaterialising(
        TauPoint(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb, tauA, tauB), result, matrix);
    EXPECT_EQ(status, excgrid::KernelStatus::kOk);

    return matrix;
}

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

        // Value-initialized past the supplied entries: scanning the whole array would overwrite component zero.
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

// Blind to the energy being wrong; the step is scaled off the perturbed value and every invariant stays positive.
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

// Every shipped name carries a second-derivative tier; mpw91, and mpw1pw91 with it, is the one analytically divergent corner.
const std::array<std::string_view, 24> kEveryTieredName = {
    "slater", "vwn5",   "vwn3", "pw92",   "svwn",     "spw92",     "becke88", "pw91",
    "pbe",    "revpbe", "rpbe", "mpw91",  "pbesol",   "lyp",       "pbe_c",   "pw91_c",
    "p86",    "b3lyp",  "pbe0", "b3pw91", "mpw1pw91", "bhandhlyp", "b3p86",   "tau_x",
};

const std::array<std::string_view, 0> kEveryUntieredName = {};

} // namespace


TEST(SecondDerivativeTest, SlaterDensityCurvatureIsTheClosedForm) {
    const excgrid::PointSecondDerivativeMatrix matrix =
        LdaMatrix(excgrid::SlaterExchangeSecondDerivatives, kRhoA, kRhoB);

    // Two independent spin channels: the cross curvature is exactly zero.
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(0, 0, 2)], SlaterCurvature(kRhoA));
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(1, 1, 2)], SlaterCurvature(kRhoB));
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(0, 1, 2)], 0.0);

    EXPECT_LT(matrix.upper[Packed(0, 0, 2)], 0.0);
    EXPECT_NE(matrix.upper[Packed(1, 1, 2)], 0.0);
}

// Enhancement flat to first order in the gradient: the density curvature at zero gradient is the LSDA one.
TEST(SecondDerivativeTest, PbeDensityCurvatureAtZeroGradientIsTheLsdaOne) {
    const excgrid::PointSecondDerivativeMatrix matrix =
        GgaMatrix(excgrid::PbeExchangeSecondDerivatives, kRhoA, kRhoB, 0.0, 0.0, 0.0);

    const double tolerance = 1e-12 * std::abs(SlaterCurvature(kRhoA));
    EXPECT_NEAR(matrix.upper[Packed(0, 0, 5)], SlaterCurvature(kRhoA), tolerance);
    EXPECT_NEAR(matrix.upper[Packed(1, 1, 5)], SlaterCurvature(kRhoB), tolerance);
}

// Linear in the gradient invariant and in tau: both curvatures vanish; the density block is LSDA.
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

    EXPECT_NEAR(matrix.upper[Packed(rhoASlot, rhoASlot, kActive)], SlaterCurvature(rhoA), 1e-12);
    EXPECT_NEAR(matrix.upper[Packed(rhoBSlot, rhoBSlot, kActive)], SlaterCurvature(rhoB), 1e-12);
    EXPECT_DOUBLE_EQ(matrix.upper[Packed(rhoASlot, rhoBSlot, kActive)], 0.0);

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

    // e = -Cx rho^(4/3)(1 + (1 - tauP/tauUnif)/12); d2e/drho dsigma = (4/3) Cx / (28.8 (6 pi^2)^(2/3)) rho^(-7/3), d2e/drho dtau = -(1/3) Cx / (3.6 (6 pi^2)^(2/3)) rho^(-4/3).
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

// de/dsigma = -(1/(8 rho)) de/dtau, so d2e/drho dsigma + (1/(8 rho)) d2e/drho dtau - (1/(8 rho^2)) de/dtau = 0.
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

// Each functional over its own required mask, against differences of its own first derivatives.
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

// At an exactly zero gradient the tier must be finite wherever the kernel is, and equal the
// limit. Pinned: mpw91's x^3.72 curvature diverges there; p86's gamma*exp(-a*sqrt(gamma)) has a
// -3a/(4 sqrt(gamma)) curvature, so its sigma-sigma entries (9-14) diverge, while its rho-sigma
// entries (2,3,4,6,7,8) are singular in the unguarded source but have finite limits; mpw1pw91 and
// b3p86 inherit their component's holes.
const std::array<std::string_view, 4> kTierHolesAtZeroGradient = {"mpw91", "mpw1pw91", "p86",
                                                                  "b3p86"};

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

        EXPECT_TRUE(kernelFinite) << name;

        const bool pinnedTierHole =
            std::find(kTierHolesAtZeroGradient.begin(), kTierHolesAtZeroGradient.end(), name) !=
            kTierHolesAtZeroGradient.end();

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

        bool anyHole = false;
        for (std::size_t k = 0; k < atZero.upper.size(); ++k)
        {
            if (!std::isfinite(atZero.upper[k]))
            {
                anyHole = true;
                EXPECT_TRUE(pinnedTierHole) << name << " entry " << k;
                continue;
            }

            EXPECT_NEAR(atZero.upper[k], adjacent.upper[k],
                        1e-9 * std::max(1.0, std::abs(adjacent.upper[k])))
                << name << " entry " << k;
        }
        EXPECT_EQ(anyHole, pinnedTierHole) << name;
    }
}

// ---------------------------------------------------------------------------
// The uniform-gas condition where it does NOT hold, stated as a measurement

// F(s) = 1 + kappa - kappa/(1 + mu s^2/kappa), s^2 = sigmaAa / radial^2, radial = kF(2 rhoA) 2 rhoA; d2e/dsigma^2|0 = -2 mu^2 rhoA epsX(2 rhoA) / (kappa radial^4).
TEST(SecondDerivativeTest, PbeGradientCurvatureAtAUniformGasIsItsAnalyticValue) {
    constexpr double kMu = 0.2195149727645171;
    constexpr double kKappa = 0.804;

    const excgrid::PointSecondDerivativeMatrix matrix =
        GgaMatrix(excgrid::PbeExchangeSecondDerivatives, kRhoA, kRhoB, 0.0, 0.0, 0.0);

    const double kFermi = std::pow(3.0 * kPi * kPi * 2.0 * kRhoA, 1.0 / 3.0);
    const double radial = kFermi * 2.0 * kRhoA;

    const double channelEnergy = -kCx * std::pow(kRhoA, 4.0 / 3.0);
    const double analytic =
        -2.0 * channelEnergy * kMu * kMu / (kKappa * std::pow(radial, 4.0));

    EXPECT_GT(analytic, 0.0);
    EXPECT_NE(analytic, 0.0);

    EXPECT_NEAR(matrix.upper[Packed(2, 2, 5)], analytic, 1e-10 * std::abs(analytic));
}

// d2E/dsigma^2 at sigma = 0 = 12 beta^2 / rho^4.
TEST(SecondDerivativeTest, TheBecke88CornerIsTheClosedFormCurvature) {
    constexpr double kBeta = 0.0042;

    const excgrid::PointSecondDerivativeMatrix matrix =
        GgaMatrix(excgrid::Becke88ExchangeSecondDerivatives, kRhoA, kRhoB, 0.0, 0.0, 0.0);

    const double alphaClosedForm = 12.0 * kBeta * kBeta / std::pow(kRhoA, 4.0);
    const double betaClosedForm = 12.0 * kBeta * kBeta / std::pow(kRhoB, 4.0);

    EXPECT_NEAR(matrix.upper[Packed(2, 2, 5)], 0.0229209731018569, 1e-15);
    EXPECT_NEAR(matrix.upper[Packed(4, 4, 5)], 0.0903626801447989, 1e-15);

    EXPECT_NEAR(matrix.upper[Packed(2, 2, 5)], alphaClosedForm, 1e-12 * alphaClosedForm);
    EXPECT_NEAR(matrix.upper[Packed(4, 4, 5)], betaClosedForm, 1e-12 * betaClosedForm);
}


TEST(SecondDerivativeTest, TheCoverageReportNamesWhatEachTierSpans) {
    // The hand-written lists are pinned to the registry first, so a new functional cannot fall outside every loop.
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

    // Widths are the families' own: 2 for LDA, 5 for GGA, 7 for the tau tier.
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

    // A component the functional does not read is not part of the request.
    EXPECT_EQ(excgrid::UnspannedRequestComponents(requested, spanned, spanned).bits, 0u);
    EXPECT_EQ(excgrid::SecondDerivativeStatus(requested, spanned, spanned),
              excgrid::KernelStatus::kOk);

    EXPECT_EQ(excgrid::SecondDerivativeStatus(requested, required, excgrid::ComponentMask{}),
              excgrid::KernelStatus::kRefusedUnsupportedCapability);
}

// ---------------------------------------------------------------------------
// Composition

TEST(SecondDerivativeTest, ARecipeWithEveryTermTieredIsTheWeightedSumOfThem) {
    const excgrid::XcFunctional* hybrid = excgrid::FindFunctional("b3lyp");
    ASSERT_NE(hybrid, nullptr);

    const excgrid::PointInputs inputs =
        InputsOf(MakePoint(excgrid::ComponentMask::Collinear(), GenericValues()));

    EXPECT_EQ(hybrid->SecondDerivativeMask().bits, hybrid->RequiredMask().bits);

    const excgrid::PointSecondDerivativeMatrix slater =
        LdaMatrix(excgrid::SlaterExchangeSecondDerivatives, kRhoA, kRhoB);
    const excgrid::PointSecondDerivativeMatrix becke88 = GgaMatrix(
        excgrid::Becke88ExchangeSecondDerivatives, kRhoA, kRhoB, kSigmaAa, kSigmaAb, kSigmaBb);
    const excgrid::PointSecondDerivativeMatrix vwn5 =
        LdaMatrix(excgrid::Vwn5CorrelationSecondDerivatives, kRhoA, kRhoB);
    const excgrid::PointSecondDerivativeMatrix lyp = GgaMatrix(
        excgrid::LypCorrelationSecondDerivatives, kRhoA, kRhoB, kSigmaAa, kSigmaAb, kSigmaBb);

    excgrid::PointSecondDerivativeMatrix expected{};
    constexpr std::size_t kEntries = Packed(4, 4, 5) + 1;

    // An LDA term occupies the densities' block and contributes zeros elsewhere.
    const auto addLda = [&expected](double weight,
                                    const excgrid::PointSecondDerivativeMatrix& narrow) {
        expected.upper[Packed(0, 0, 5)] += weight * narrow.upper[Packed(0, 0, 2)];
        expected.upper[Packed(0, 1, 5)] += weight * narrow.upper[Packed(0, 1, 2)];
        expected.upper[Packed(1, 1, 5)] += weight * narrow.upper[Packed(1, 1, 2)];
    };
    const auto addGga = [&expected](double weight,
                                    const excgrid::PointSecondDerivativeMatrix& matrix) {
        for (std::size_t k = 0; k < kEntries; ++k)
        {
            expected.upper[k] += weight * matrix.upper[k];
        }
    };

    addLda(0.08, slater);
    addGga(0.72, becke88);
    addLda(0.19, vwn5);
    addGga(0.81, lyp);

    excgrid::PointResult result;
    excgrid::PointSecondDerivativeMatrix matrix;
    ASSERT_EQ(hybrid->EvaluatePointMaterialising(inputs, result, matrix),
              excgrid::KernelStatus::kOk);

    for (std::size_t k = 0; k < kEntries; ++k)
    {
        EXPECT_DOUBLE_EQ(matrix.upper[k], expected.upper[k]) << "entry " << k;
    }

    // The contracted entry point is the materialised matrix contracted with the caller's right-hand side (upper triangle only).
    excgrid::PointSecondDerivative second;
    const std::array<double, 5> rightHandSide{1.0, 0.5, 0.25, -0.125, 2.0};
    ASSERT_EQ(hybrid->EvaluatePointWithSecondDerivatives(inputs, rightHandSide, result, second),
              excgrid::KernelStatus::kOk);

    EXPECT_EQ(second.rightHandSides, 1u);
    for (std::size_t i = 0; i < 5; ++i)
    {
        double contraction = 0.0;
        for (std::size_t j = 0; j < 5; ++j)
        {
            const std::size_t row = std::min(i, j);
            const std::size_t column = std::max(i, j);
            contraction += matrix.upper[Packed(row, column, 5)] * rightHandSide[j];
        }
        EXPECT_DOUBLE_EQ(second.contracted[i], contraction) << "component " << i;
    }

    excgrid::PointResult plain;
    ASSERT_EQ(hybrid->EvaluatePoint(inputs, plain), excgrid::KernelStatus::kOk);
    for (std::size_t i = 0; i < 5; ++i)
    {
        const auto id = static_cast<excgrid::Component>(i);
        EXPECT_DOUBLE_EQ(plain.first[excgrid::IndexOf(id)], result.first[excgrid::IndexOf(id)])
            << "component " << i;
    }
}
