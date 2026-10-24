
#include "excgrid/kernel.hpp"
#include "excgrid/kernels.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>
#include <string_view>
#include <gtest/gtest.h>

namespace {

constexpr double kPi = 3.14159265358979323846;

template <typename F> double FiniteDifference(const F& f, double x, double h) {
    return (f(x + h) - f(x - h)) / (2.0 * h);
}

struct DensityPoint {
    double rhoA;
    double rhoB;
    double sigmaAa;
    double sigmaAb;
    double sigmaBb;
};

double ExcOfRhoA(excgrid::GgaKernel kernel, const DensityPoint& p, double x) {
    return kernel(x, p.rhoB, p.sigmaAa, p.sigmaAb, p.sigmaBb).exc;
}

double ExcOfSigmaAa(excgrid::GgaKernel kernel, const DensityPoint& p, double x) {
    return kernel(p.rhoA, p.rhoB, x, p.sigmaAb, p.sigmaBb).exc;
}

void CheckFiniteDifferenceGga(excgrid::GgaKernel kernel, const DensityPoint& p) {
    const excgrid::XcKernelValue value = kernel(p.rhoA, p.rhoB, p.sigmaAa, p.sigmaAb, p.sigmaBb);

    const double h = 1e-4 * std::max(p.rhoA, 0.1);
    const double dRhoA =
        FiniteDifference([&](double x) { return ExcOfRhoA(kernel, p, x); }, p.rhoA, h);
    const double dRhoB = FiniteDifference(
        [&](double x) { return kernel(p.rhoA, x, p.sigmaAa, p.sigmaAb, p.sigmaBb).exc; },
        p.rhoB,
        h);
    const double hSigma = 1e-4 * std::max(p.sigmaAa, 0.1);
    const double dSigmaAa =
        FiniteDifference([&](double x) { return ExcOfSigmaAa(kernel, p, x); }, p.sigmaAa, hSigma);
    const double dSigmaBb = FiniteDifference(
        [&](double x) { return kernel(p.rhoA, p.rhoB, p.sigmaAa, p.sigmaAb, x).exc; },
        p.sigmaBb,
        hSigma);

    EXPECT_NEAR(value.vrhoA, dRhoA, 1e-7 * std::max(1.0, std::abs(dRhoA)));
    EXPECT_NEAR(value.vrhoB, dRhoB, 1e-7 * std::max(1.0, std::abs(dRhoB)));
    EXPECT_NEAR(value.vsigmaAa, dSigmaAa, 1e-7 * std::max(1.0, std::abs(dSigmaAa)));
    EXPECT_NEAR(value.vsigmaBb, dSigmaBb, 1e-7 * std::max(1.0, std::abs(dSigmaBb)));
}

TEST(KernelTest, SlaterClosedForm) {
    // e_x(rhoA, rhoB) = -(3/4)(6/pi)^(1/3) (rhoA^(4/3) + rhoB^(4/3)).
    const double c = 0.75 * std::pow(6.0 / kPi, 1.0 / 3.0);
    const double rhoA = 0.3;
    const double rhoB = 0.2;

    const excgrid::XcKernelValue value = excgrid::SlaterExchange(rhoA, rhoB);

    const double expect = -c * (std::pow(rhoA, 4.0 / 3.0) + std::pow(rhoB, 4.0 / 3.0));

    EXPECT_NEAR(value.exc, expect, 1e-14);
    EXPECT_NEAR(value.vrhoA, -(4.0 / 3.0) * c * std::pow(rhoA, 1.0 / 3.0), 1e-13);
    EXPECT_NEAR(value.vrhoB, -(4.0 / 3.0) * c * std::pow(rhoB, 1.0 / 3.0), 1e-13);
    EXPECT_EQ(value.vsigmaAa, 0.0);
    EXPECT_EQ(value.vtauA, 0.0);
}

TEST(KernelTest, LdaKernelsAreGradientIndependent) {
    const excgrid::XcKernelValue slater = excgrid::SlaterExchange(0.3, 0.2);

    EXPECT_EQ(slater.vsigmaAa, 0.0);
    EXPECT_EQ(slater.vsigmaAb, 0.0);
    EXPECT_EQ(slater.vsigmaBb, 0.0);
}

TEST(KernelTest, SlaterUniformElectronGas) {
    // UEG: at rhoA = rhoB = rho/2, e = -(3/4)(3/pi)^(1/3) rho^(4/3).
    const double rho = 0.4;
    const double expect = -0.75 * std::pow(3.0 * rho / kPi, 1.0 / 3.0) * rho;

    const excgrid::XcKernelValue value = excgrid::SlaterExchange(0.5 * rho, 0.5 * rho);

    EXPECT_NEAR(value.exc, expect, 1e-13);
}

TEST(KernelTest, SpinScalingSymmetry) {
    const excgrid::XcKernelValue ab = excgrid::PbeExchange(0.4, 0.1, 0.5, 0.2, 0.05);
    const excgrid::XcKernelValue ba = excgrid::PbeExchange(0.1, 0.4, 0.05, 0.2, 0.5);

    EXPECT_NEAR(ab.exc, ba.exc, 1e-14);
    EXPECT_NEAR(ab.vrhoA, ba.vrhoB, 1e-14);
    EXPECT_NEAR(ab.vrhoB, ba.vrhoA, 1e-14);
    EXPECT_NEAR(ab.vsigmaAa, ba.vsigmaBb, 1e-14);
    EXPECT_NEAR(ab.vsigmaBb, ba.vsigmaAa, 1e-14);
}

TEST(KernelTest, GgaFiniteDifference) {
    const DensityPoint points[] = {
        {0.3, 0.25, 0.6, 0.2, 0.4},
        {0.08, 0.02, 0.12, 0.05, 0.01},
    };

    for (const DensityPoint& p : points)
    {
        CheckFiniteDifferenceGga(excgrid::Becke88Exchange, p);
        CheckFiniteDifferenceGga(excgrid::Pw91Exchange, p);
        CheckFiniteDifferenceGga(excgrid::PbeExchange, p);
        CheckFiniteDifferenceGga(excgrid::RevPbeExchange, p);
        CheckFiniteDifferenceGga(excgrid::RpbeExchange, p);
        CheckFiniteDifferenceGga(excgrid::MPw91Exchange, p);
        CheckFiniteDifferenceGga(excgrid::PbeSolExchange, p);
        CheckFiniteDifferenceGga(excgrid::LypCorrelation, p);
        CheckFiniteDifferenceGga(excgrid::PbeCorrelation, p);
        CheckFiniteDifferenceGga(excgrid::Pw91Correlation, p);
        CheckFiniteDifferenceGga(excgrid::P86Correlation, p);
    }
}

TEST(KernelTest, LdaFiniteDifference) {
    const double rhoA = 0.35;
    const double rhoB = 0.15;
    const double h = 1e-5;

    for (excgrid::LdaKernel kernel : {excgrid::SlaterExchange,
                                      excgrid::Vwn5Correlation,
                                      excgrid::Vwn3Correlation,
                                      excgrid::Pw92Correlation})
    {
        const excgrid::XcKernelValue value = kernel(rhoA, rhoB);
        const double dRhoA =
            FiniteDifference([&](double x) { return kernel(x, rhoB).exc; }, rhoA, h);
        const double dRhoB =
            FiniteDifference([&](double x) { return kernel(rhoA, x).exc; }, rhoB, h);

        EXPECT_NEAR(value.vrhoA, dRhoA, 1e-7 * std::max(1.0, std::abs(dRhoA)));
        EXPECT_NEAR(value.vrhoB, dRhoB, 1e-7 * std::max(1.0, std::abs(dRhoB)));
    }
}

TEST(KernelTest, VxcIntegratesToExc) {
    // LDA exchange: e(lambda rho) = lambda^(4/3) e, so (4/3) e = rhoA vrhoA + rhoB vrhoB.
    const double width = 0.7;
    const double center = 1.3;
    const double amplitude = 0.25;

    auto densityOf = [&](double x) {
        const double g = amplitude * std::exp(-width * (x - center) * (x - center));
        const double h = 0.6 * amplitude * std::exp(-width * x * x);
        return g + h;
    };

    for (double x = -2.0; x <= 4.0; x += 0.25)
    {
        const double rho = densityOf(x);

        const excgrid::XcKernelValue lda = excgrid::SlaterExchange(0.5 * rho, 0.5 * rho);
        EXPECT_NEAR(1.3333333333333333 * lda.exc,
                    0.5 * rho * (lda.vrhoA + lda.vrhoB),
                    1e-12 * std::max(1.0, std::abs(lda.exc)));
    }
}


/// One GGA kernel plus the name it ships under - a failure message has to say
/// which kernel, and the registry name is the name a consumer would use.
struct NamedGgaKernel {
    const char* name;
    excgrid::GgaKernel kernel;
};

const std::array<NamedGgaKernel, 6> kGgaExchangeKernels = {{
    {"becke88", &excgrid::Becke88Exchange},
    {"pw91", &excgrid::Pw91Exchange},
    {"pbe", &excgrid::PbeExchange},
    {"revpbe", &excgrid::RevPbeExchange},
    {"mpw91", &excgrid::MPw91Exchange},
    {"pbesol", &excgrid::PbeSolExchange},
}};

/// Correlation kernels reduce the gradient to sigmaAa + 2 sigmaAb + sigmaBb.
const std::array<NamedGgaKernel, 3> kGgaCorrelationKernels = {{
    {"pbe_c", &excgrid::PbeCorrelation},
    {"pw91_c", &excgrid::Pw91Correlation},
    {"p86", &excgrid::P86Correlation},
}};

const std::array<NamedGgaKernel, 4> kRadicalCancelledKernels = {{
    {"pbe", &excgrid::PbeExchange},
    {"revpbe", &excgrid::RevPbeExchange},
    {"rpbe", &excgrid::RpbeExchange},
    {"pbesol", &excgrid::PbeSolExchange},
}};

/// asinh(x) / sqrt(sigma) shape: no common factor for the cancellation to remove.
const std::array<NamedGgaKernel, 3> kAsinhShapedKernels = {{
    {"becke88", &excgrid::Becke88Exchange},
    {"pw91", &excgrid::Pw91Exchange},
    {"mpw91", &excgrid::MPw91Exchange},
}};

/// The Dirac constant as the generated sources' literal prints it.
constexpr double kCx = 0.9305257363491;

/// dE/dsigma at sigma = 0: the s^2 coefficient over the reduced gradient's denominator squared.
double AsinhShapedVsigmaAtZeroGradient(std::string_view name, double rho) {
    if (name == "becke88")
    {
        // F = beta x^2 / (1 + 6 beta x asinh(x)); dE/dsigma -> -beta / rho^(4/3).
        return -0.0042 / std::pow(rho, 4.0 / 3.0);
    }
    if (name == "mpw91")
    {
        // mPW91: x^2 coefficient 5 (36 pi)^(-5/3); dE/dsigma -> -5 (36 pi)^(-5/3) / rho^(4/3).
        return -5.0 * std::pow(36.0 * kPi, -5.0 / 3.0) / std::pow(rho, 4.0 / 3.0);
    }
    if (name == "pw91")
    {
        // F = 1 + (0.2743 - 0.1508) s^2 + O(s^4); dE/dsigma -> -kCx rho^(4/3) (0.2743 - 0.1508) / radial^2.
        const double radial =
            std::pow(3.0 * kPi * kPi, 1.0 / 3.0) * std::pow(2.0 * rho, 4.0 / 3.0);

        return -kCx * std::pow(rho, 4.0 / 3.0) * (0.2743 - 0.1508) / (radial * radial);
    }
    return std::numeric_limits<double>::quiet_NaN();
}

/// sigmaAa + 2 sigmaAb + sigmaBb = |grad(rhoA) + grad(rhoB)|^2, a perfect square.
double TotalSigma(double sigmaAa, double sigmaAb, double sigmaBb) {
    return sigmaAa + 2.0 * sigmaAb + sigmaBb;
}

TEST(KernelTest, ZeroGradientVsigmaExchangeReturnsTheLimit) {
    // rhoA = rhoB = 0.4 with all three sigma inputs bit-exactly zero.
    constexpr double kSmallestSubnormal = 4.9406564584124654e-324;
    const double rhoA = 0.4;
    const double rhoB = 0.4;

    for (const NamedGgaKernel& entry : kRadicalCancelledKernels)
    {
        const excgrid::XcKernelValue value = entry.kernel(rhoA, rhoB, 0.0, 0.0, 0.0);

        EXPECT_TRUE(std::isfinite(value.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vrhoA)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vrhoB)) << entry.name;
        // Exchange enhancement falls with the gradient: dE/dsigma < 0.
        EXPECT_TRUE(std::isfinite(value.vsigmaAa)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vsigmaBb)) << entry.name;
        EXPECT_LT(value.vsigmaAa, 0.0) << entry.name;
        EXPECT_LT(value.vsigmaBb, 0.0) << entry.name;
        EXPECT_EQ(value.vsigmaAb, 0.0) << entry.name;

        // The corner value is the value one representable step off it, bit for bit.
        const excgrid::XcKernelValue limit =
            entry.kernel(rhoA, rhoB, kSmallestSubnormal, kSmallestSubnormal, kSmallestSubnormal);

        EXPECT_EQ(std::bit_cast<std::uint64_t>(value.vsigmaAa),
                  std::bit_cast<std::uint64_t>(limit.vsigmaAa))
            << entry.name;
        EXPECT_EQ(std::bit_cast<std::uint64_t>(value.vsigmaBb),
                  std::bit_cast<std::uint64_t>(limit.vsigmaBb))
            << entry.name;
    }

    // asinh-shaped kernels: the corner answers the published closed form.
    for (const NamedGgaKernel& entry : kAsinhShapedKernels)
    {
        const excgrid::XcKernelValue value = entry.kernel(rhoA, rhoB, 0.0, 0.0, 0.0);

        EXPECT_TRUE(std::isfinite(value.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vsigmaAa)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vsigmaBb)) << entry.name;
        EXPECT_LT(value.vsigmaAa, 0.0) << entry.name;
        EXPECT_LT(value.vsigmaBb, 0.0) << entry.name;
        EXPECT_EQ(value.vsigmaAb, 0.0) << entry.name;

        const double limitAa = AsinhShapedVsigmaAtZeroGradient(entry.name, rhoA);

        EXPECT_NEAR(value.vsigmaAa, limitAa, 1e-13 * std::abs(limitAa)) << entry.name;

        const double limitBb = AsinhShapedVsigmaAtZeroGradient(entry.name, rhoB);

        EXPECT_NEAR(value.vsigmaBb, limitBb, 1e-13 * std::abs(limitBb)) << entry.name;

        // The corner is one point here too: sigma = 0 equals one representable step off.
        const excgrid::XcKernelValue step =
            entry.kernel(rhoA, rhoB, kSmallestSubnormal, kSmallestSubnormal, kSmallestSubnormal);

        EXPECT_EQ(std::bit_cast<std::uint64_t>(value.vsigmaAa),
                  std::bit_cast<std::uint64_t>(step.vsigmaAa))
            << entry.name;
        EXPECT_EQ(std::bit_cast<std::uint64_t>(value.vsigmaBb),
                  std::bit_cast<std::uint64_t>(step.vsigmaBb))
            << entry.name;
    }

    for (const NamedGgaKernel& entry : kGgaCorrelationKernels)
    {
        const excgrid::XcKernelValue value = entry.kernel(rhoA, rhoB, 0.0, 0.0, 0.0);

        // The correlation energy is quadratic in the sigma total: its zero is an interior point for
        // exc, and the guarded first derivative answers the finite limit there.
        EXPECT_TRUE(std::isfinite(value.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vrhoA)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vrhoB)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vsigmaAa)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vsigmaAb)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vsigmaBb)) << entry.name;

        // The corner is one point here too: sigma = 0 equals one representable step off.
        const excgrid::XcKernelValue step =
            entry.kernel(rhoA, rhoB, kSmallestSubnormal, kSmallestSubnormal, kSmallestSubnormal);

        EXPECT_EQ(std::bit_cast<std::uint64_t>(value.vsigmaAa),
                  std::bit_cast<std::uint64_t>(step.vsigmaAa))
            << entry.name;
        EXPECT_EQ(std::bit_cast<std::uint64_t>(value.vsigmaAb),
                  std::bit_cast<std::uint64_t>(step.vsigmaAb))
            << entry.name;
        EXPECT_EQ(std::bit_cast<std::uint64_t>(value.vsigmaBb),
                  std::bit_cast<std::uint64_t>(step.vsigmaBb))
            << entry.name;
    }

    // LYP is linear in sigma, so it is the finite control.
    const excgrid::XcKernelValue lyp = excgrid::LypCorrelation(rhoA, rhoB, 0.0, 0.0, 0.0);

    EXPECT_TRUE(std::isfinite(lyp.vsigmaAa));
    EXPECT_TRUE(std::isfinite(lyp.vsigmaAb));
    EXPECT_TRUE(std::isfinite(lyp.vsigmaBb));
    EXPECT_NEAR(lyp.vsigmaAa, 6.3832299997409927e-04, 1e-18);
    EXPECT_NEAR(lyp.vsigmaAb, 6.9783987371625626e-04, 1e-18);
    EXPECT_NEAR(lyp.vsigmaBb, 6.3832299997409927e-04, 1e-18);
}

TEST(KernelTest, ZeroGradientVsigmaNaNBoundaryIsTheExactZero) {
    // The defect's boundary is bit-exact zero, not smallness: the smallest subnormal is already finite.
    const double rhoA = 0.4;
    const double rhoB = 0.4;
    // 2^-1074, the smallest positive subnormal.
    constexpr double kSmallestSubnormal = 4.9406564584124654e-324;

    EXPECT_NE(kSmallestSubnormal, 0.0);
    EXPECT_TRUE(std::isfinite(excgrid::PbeExchange(rhoA, rhoB, 0.0, 0.0, 0.0).vsigmaAa));
    EXPECT_TRUE(std::isfinite(
        excgrid::PbeExchange(rhoA, rhoB, kSmallestSubnormal, kSmallestSubnormal, kSmallestSubnormal)
            .vsigmaAa));

    const excgrid::XcKernelValue split = excgrid::PbeExchange(rhoA, rhoB, 0.0, 0.03, 0.05);
    const excgrid::XcKernelValue splitLimit = excgrid::PbeExchange(
        rhoA, rhoB, kSmallestSubnormal, 0.03, 0.05);

    EXPECT_TRUE(std::isfinite(split.vsigmaAa));
    EXPECT_TRUE(std::isfinite(split.vsigmaBb));
    EXPECT_NEAR(split.vsigmaBb, -1.1345974574693928e-02, 1e-16);
    EXPECT_EQ(std::bit_cast<std::uint64_t>(split.vsigmaAa),
              std::bit_cast<std::uint64_t>(splitLimit.vsigmaAa));
}

TEST(KernelTest, ZeroGradientCorrelationVsigmaAnswersAtTheZeroTotal) {
    // The correlation trigger is the sigma total: (0.02, -0.01, 0.0) totals exactly zero, and the
    // guarded radicand answers the finite limit there, independent of how the total is split.
    const double rhoA = 0.4;
    const double rhoB = 0.4;

    for (const NamedGgaKernel& entry : kGgaCorrelationKernels)
    {
        const excgrid::XcKernelValue zeroTotal = entry.kernel(rhoA, rhoB, 0.02, -0.01, 0.0);
        const excgrid::XcKernelValue allZero = entry.kernel(rhoA, rhoB, 0.0, 0.0, 0.0);

        EXPECT_EQ(TotalSigma(0.02, -0.01, 0.0), 0.0) << entry.name;
        EXPECT_TRUE(std::isfinite(zeroTotal.vsigmaAa)) << entry.name;
        EXPECT_TRUE(std::isfinite(zeroTotal.vsigmaAb)) << entry.name;
        EXPECT_TRUE(std::isfinite(zeroTotal.vsigmaBb)) << entry.name;
        EXPECT_EQ(std::bit_cast<std::uint64_t>(zeroTotal.vsigmaAa),
                  std::bit_cast<std::uint64_t>(allZero.vsigmaAa))
            << entry.name;
        EXPECT_EQ(std::bit_cast<std::uint64_t>(zeroTotal.vsigmaAb),
                  std::bit_cast<std::uint64_t>(allZero.vsigmaAb))
            << entry.name;
        EXPECT_EQ(std::bit_cast<std::uint64_t>(zeroTotal.vsigmaBb),
                  std::bit_cast<std::uint64_t>(allZero.vsigmaBb))
            << entry.name;

        const excgrid::XcKernelValue smallTotal = entry.kernel(rhoA, rhoB, 0.001, 0.0, 0.0);

        EXPECT_TRUE(std::isfinite(smallTotal.vsigmaAa)) << entry.name;
        EXPECT_TRUE(std::isfinite(smallTotal.vsigmaAb)) << entry.name;
        EXPECT_TRUE(std::isfinite(smallTotal.vsigmaBb)) << entry.name;
    }

    // Negative totals are unreachable physically; checked only to bound the test.
    const excgrid::XcKernelValue negative =
        excgrid::PbeCorrelation(rhoA, rhoB, 0.001, -0.002, 0.001);

    EXPECT_LT(TotalSigma(0.001, -0.002, 0.001), 0.0);
    EXPECT_TRUE(std::isnan(negative.exc));
}

TEST(KernelTest, ZeroGradientVsigmaLimitMatchesClosedForm) {
    // dE/dsigma|0 = -(1/2)(3/4)(3/pi)^(1/3) 2^(4/3) mu / (4 * 2^(2/3) (3 pi^2)^(2/3) rho^(4/3)); kappa cancels.
    const double rho = 0.4;
    // 2^-1074, the smallest positive subnormal.
    constexpr double kSmallestSubnormal = 4.9406564584124654e-324;

    auto pbeFamilyLimit = [rho](double mu) {
        const double ldaPrefactor =
            0.5 * 0.75 * std::pow(3.0 / kPi, 1.0 / 3.0) * std::pow(2.0, 4.0 / 3.0);

        return -ldaPrefactor * mu /
               (4.0 * std::pow(2.0, 2.0 / 3.0) * std::pow(3.0 * kPi * kPi, 2.0 / 3.0) *
                std::pow(rho, 4.0 / 3.0));
    };

    const double pbeMu = 0.2195149727645171;

    EXPECT_NEAR(
        excgrid::PbeExchange(rho, rho, kSmallestSubnormal, kSmallestSubnormal, kSmallestSubnormal)
            .vsigmaAa,
        pbeFamilyLimit(pbeMu),
        1e-13 * std::abs(pbeFamilyLimit(pbeMu)));
    EXPECT_NEAR(excgrid::PbeSolExchange(
                    rho, rho, kSmallestSubnormal, kSmallestSubnormal, kSmallestSubnormal)
                    .vsigmaAa,
                pbeFamilyLimit(10.0 / 81.0),
                1e-13 * std::abs(pbeFamilyLimit(10.0 / 81.0)));

    // revPBE shares PBE's mu; kappa drops out of the sigma -> 0 limit.
    EXPECT_NEAR(excgrid::RevPbeExchange(
                    rho, rho, kSmallestSubnormal, kSmallestSubnormal, kSmallestSubnormal)
                    .vsigmaAa,
                pbeFamilyLimit(pbeMu),
                1e-13 * std::abs(pbeFamilyLimit(pbeMu)));

    const double mpw91Limit = -5.0 * std::pow(36.0 * kPi, -5.0 / 3.0) / std::pow(rho, 4.0 / 3.0);

    EXPECT_NEAR(
        excgrid::MPw91Exchange(rho, rho, kSmallestSubnormal, kSmallestSubnormal, kSmallestSubnormal)
            .vsigmaAa,
        mpw91Limit,
        1e-13 * std::abs(mpw91Limit));
}

TEST(KernelTest, ZeroSpinDensityIsAClosedCornerToo) {
    // Fully spin-polarized densities reach (rhoB = 0, sigmaBb = 0) at every point; the exact vrhoB there is 0.
    constexpr double kSmallestSubnormal = 4.9406564584124654e-324;
    const double rhoA = 0.4;
    const double sigmaAa = 0.05;

    for (const NamedGgaKernel& entry : kRadicalCancelledKernels)
    {
        const excgrid::XcKernelValue polarized = entry.kernel(rhoA, 0.0, sigmaAa, 0.0, 0.0);

        EXPECT_TRUE(std::isfinite(polarized.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(polarized.vrhoA)) << entry.name;
        EXPECT_TRUE(std::isfinite(polarized.vsigmaAa)) << entry.name;
        EXPECT_LT(polarized.vsigmaAa, 0.0) << entry.name;
        EXPECT_TRUE(std::isfinite(polarized.vsigmaBb)) << entry.name;
        EXPECT_EQ(std::bit_cast<std::uint64_t>(polarized.vsigmaBb),
                  std::bit_cast<std::uint64_t>(
                      entry.kernel(rhoA, 0.0, sigmaAa, 0.0, kSmallestSubnormal).vsigmaBb))
            << entry.name;
    }

    // Empty channel: the beta term carries rhoB^(4/3) = 0, and the guard keeps the radical's 0/0 out.
    for (const NamedGgaKernel& entry : kAsinhShapedKernels)
    {
        const excgrid::XcKernelValue polarized = entry.kernel(rhoA, 0.0, sigmaAa, 0.0, 0.0);

        EXPECT_TRUE(std::isfinite(polarized.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(polarized.vsigmaAa)) << entry.name;
        EXPECT_DOUBLE_EQ(polarized.vsigmaBb, 0.0) << entry.name;
        EXPECT_DOUBLE_EQ(polarized.vsigmaAb, 0.0) << entry.name;
    }

    // Empty channel's vrhoB is exactly zero, independent of the sigma inputs.
    const auto vrhoBOfEmptyBeta = [&](excgrid::GgaKernel kernel) {
        return kernel(rhoA, 0.0, sigmaAa, 0.0, 0.0).vrhoB;
    };

    EXPECT_DOUBLE_EQ(vrhoBOfEmptyBeta(&excgrid::PbeExchange), 0.0);
    EXPECT_DOUBLE_EQ(vrhoBOfEmptyBeta(&excgrid::RevPbeExchange), 0.0);
    EXPECT_DOUBLE_EQ(vrhoBOfEmptyBeta(&excgrid::RpbeExchange), 0.0);
    EXPECT_DOUBLE_EQ(vrhoBOfEmptyBeta(&excgrid::PbeSolExchange), 0.0);
    EXPECT_DOUBLE_EQ(vrhoBOfEmptyBeta(&excgrid::Pw91Exchange), 0.0);
    EXPECT_DOUBLE_EQ(excgrid::PbeExchange(rhoA, 0.0, sigmaAa, 0.0, 0.03).vrhoB, 0.0);

    for (const NamedGgaKernel& entry : kGgaExchangeKernels)
    {
        const excgrid::XcKernelValue polarized = entry.kernel(rhoA, 0.0, sigmaAa, 0.0, 0.0);

        EXPECT_TRUE(std::isfinite(polarized.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(polarized.vrhoA)) << entry.name;
        EXPECT_TRUE(std::isfinite(polarized.vrhoB)) << entry.name;
        EXPECT_TRUE(std::isfinite(polarized.vsigmaAa)) << entry.name;
        EXPECT_TRUE(std::isfinite(polarized.vsigmaBb)) << entry.name;
    }

    // Correlation diverges as rhoB -> 0: finite, not NaN, is the correct answer.
    EXPECT_TRUE(std::isfinite(excgrid::PbeCorrelation(rhoA, 0.0, sigmaAa, 0.0, 0.0).vrhoB));
    EXPECT_FALSE(std::isnan(excgrid::LypCorrelation(rhoA, 0.0, sigmaAa, 0.0, 0.0).vrhoB));
}

TEST(KernelTest, ZeroSpinDensityIsSymmetricInTheTwoSpins) {
    const double rhoB = 0.4;
    const double sigmaBb = 0.05;

    EXPECT_DOUBLE_EQ(excgrid::PbeExchange(0.0, rhoB, 0.0, 0.0, sigmaBb).vrhoA, 0.0);
    EXPECT_DOUBLE_EQ(excgrid::RevPbeExchange(0.0, rhoB, 0.0, 0.0, sigmaBb).vrhoA, 0.0);
    EXPECT_DOUBLE_EQ(excgrid::RpbeExchange(0.0, rhoB, 0.0, 0.0, sigmaBb).vrhoA, 0.0);
    EXPECT_DOUBLE_EQ(excgrid::PbeSolExchange(0.0, rhoB, 0.0, 0.0, sigmaBb).vrhoA, 0.0);
    EXPECT_DOUBLE_EQ(excgrid::Pw91Exchange(0.0, rhoB, 0.0, 0.0, sigmaBb).vrhoA, 0.0);

    EXPECT_TRUE(std::isfinite(excgrid::Becke88Exchange(0.0, rhoB, 0.0, 0.0, sigmaBb).vrhoA));
    EXPECT_TRUE(std::isfinite(excgrid::MPw91Exchange(0.0, rhoB, 0.0, 0.0, sigmaBb).vrhoA));

    for (const NamedGgaKernel& entry : kGgaExchangeKernels)
    {
        const excgrid::XcKernelValue value = entry.kernel(0.0, rhoB, 0.0, 0.0, sigmaBb);

        EXPECT_TRUE(std::isfinite(value.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vrhoB)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vsigmaBb)) << entry.name;
    }
}

TEST(KernelTest, ZeroGradientVsigmaAtExactZeroIsTheAcceptanceTarget) {
    // numerator = -rhoA * 3 * c99 * 1.4119203048213739; denominator = 1.608 * c101 * 2 * c102 * (mu s^2 / kappa + 1)^2 * 4 pi.
    constexpr double kSmallestSubnormal = 4.9406564584124654e-324;
    const double rhoA = 0.4;
    const double sigmaAa = 0.0;
    const double c92 = 2.0 * rhoA;
    const double c93 = 4.0 * sigmaAa;
    const double c95 = kPi * kPi;
    const double c96 = c95 * c92;
    const double c97 = std::sqrt(c93);
    const double c99 = std::pow(3.0 * c96, 1.0 / 3.0);
    const double c101 = c99 * c92 + 1e-16;
    const double c102 = 2.0 * c101;
    const double reducedNumerator = -rhoA * 3.0 * c99 * 1.4119203048213739;
    const double reducedDenominator =
        1.608 * c101 * 2 * c102 *
        std::pow(0.2195149727645171 * std::pow(c97 / c102, 2) / 0.804 + 1., 2) * 4. * kPi;
    const double reducedLimit = reducedNumerator / reducedDenominator;

    EXPECT_TRUE(std::isfinite(reducedLimit));
    EXPECT_NE(reducedNumerator, 0.0);

    const double cornerValue = excgrid::PbeExchange(rhoA, rhoA, 0.0, 0.0, 0.0).vsigmaAa;
    const double shippedLimit = excgrid::PbeExchange(rhoA, rhoA, kSmallestSubnormal,
                                                     kSmallestSubnormal, kSmallestSubnormal)
                                    .vsigmaAa;

    EXPECT_TRUE(std::isfinite(cornerValue));
    EXPECT_LT(cornerValue, 0.0);
    EXPECT_EQ(std::bit_cast<std::uint64_t>(cornerValue),
              std::bit_cast<std::uint64_t>(shippedLimit));

    // The generator regroups the constants: up to 7 ulp, hence 1e-13 relative.
    EXPECT_NEAR(cornerValue, reducedLimit, 1e-13 * std::abs(reducedLimit));
}

} // namespace
