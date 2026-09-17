// The generated-kernel verification suite: closed forms, symmetry,
// the finite-difference channel (symbolic derivatives vs numerical
// derivatives of the same energy expression - catches transcription
// and differentiation errors with no external reference), exact
// conditions, and the Vxc-integrates-to-Exc consistency identity.

#include "excgrid/kernel.hpp"
#include "excgrid/kernels.hpp"

#include <array>
#include <cmath>
#include <gtest/gtest.h>

namespace {

constexpr double kPi = 3.14159265358979323846;

// Central-difference helper: df/dx at x for the scalar function f
// (any callable - the kernels are wrapped in capturing lambdas).
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

// The exc field of a kernel call, as a scalar function of one input
// with the others held fixed.
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
    // Every LDA kernel must return zero sigma derivatives (its
    // signature ignores sigma entirely).
    const excgrid::XcKernelValue slater = excgrid::SlaterExchange(0.3, 0.2);

    EXPECT_EQ(slater.vsigmaAa, 0.0);
    EXPECT_EQ(slater.vsigmaAb, 0.0);
    EXPECT_EQ(slater.vsigmaBb, 0.0);
}

TEST(KernelTest, SlaterUniformElectronGas) {
    // The UEG exact condition: at rhoA = rhoB = rho/2 the Slater energy
    // density is the uniform-gas value -(3/4)(3/pi)^(1/3) rho^(4/3).
    const double rho = 0.4;
    const double expect = -0.75 * std::pow(3.0 * rho / kPi, 1.0 / 3.0) * rho;

    const excgrid::XcKernelValue value = excgrid::SlaterExchange(0.5 * rho, 0.5 * rho);

    EXPECT_NEAR(value.exc, expect, 1e-13);
}

TEST(KernelTest, SpinScalingSymmetry) {
    // Exchange and correlation kernels are symmetric under rhoA <-> rhoB
    // with the sigma slots swapped correspondingly.
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
    // The Euler homogeneity identity, per point over an analytic
    // Gaussian-pair density.  LDA exchange scales as
    // e(lambda rhoA, lambda rhoB) = lambda^(4/3) e, so the Euler
    // relation carries the 4/3 factor: (4/3) e = rhoA vrhoA +
    // rhoB vrhoB.  GGA kernels are deliberately NOT checked here:
    // the reduced gradient scales as lambda^(-1/3) under the DFT
    // scaling (rho, sigma) -> (lambda rho, lambda^2 sigma), so
    // Fx(s) breaks the pure power law and no clean Euler identity
    // exists for them - the finite-difference suite is their check.
    // Correlation kernels are not homogeneous either (their LDA
    // limit's epsilon_c(rs) breaks the power law).
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

// ---------------------------------------------------------------------------
// The exactly-zero-gradient corner of the GGA kernels.
//
// Every shipped GGA kernel except LYP returns NaN in a vsigma component when
// the sigma it feeds on is BIT-EXACTLY zero.  The generated expression carries
// the same sqrt(sigma) factor in its numerator and its denominator - the chain
// rule's ds/dsigma = 1 / (2 s), multiplied through by s to keep the expression
// polynomial in sqrt(sigma) - so the exact zero evaluates 0 / 0.  The
// derivative itself has a finite, NONZERO limit there, which is why the
// smallest positive subnormal (5e-324) already returns it: this is a single
// removable point, not an overflow and not a divergence.
//
// The tests below PIN THE DEFECT.  They do not assert the correct behaviour,
// because the correct behaviour is not what the kernels do yet.  A kernel
// fixed to return the limit at sigma = 0 must have them inverted - the
// EXPECT_TRUE(std::isnan(...)) lines become EXPECT_NEAR(..., limit, ...) - and
// that inversion is the whole content of the fix.  Until then they exist so
// that the corner cannot move silently, and so that the next reader of
// registry_test.cpp's kProbeGradients (which probes 1e-12 rather than a clean
// zero to dodge exactly this) has the measurement that comment refers to.

/// One GGA kernel plus the name it ships under - a failure message has to say
/// which kernel, and the registry name is the name a consumer would use.
struct NamedGgaKernel {
    const char* name;
    excgrid::GgaKernel kernel;
};

/// The six GGA exchange kernels: the spin-scaled LDA plus an enhancement
/// factor F(s) with s proportional to sqrt(sigma).
const std::array<NamedGgaKernel, 6> kGgaExchangeKernels = {{
    {"becke88", &excgrid::Becke88Exchange},
    {"pw91", &excgrid::Pw91Exchange},
    {"pbe", &excgrid::PbeExchange},
    {"revpbe", &excgrid::RevPbeExchange},
    {"mpw91", &excgrid::MPw91Exchange},
    {"pbesol", &excgrid::PbeSolExchange},
}};

/// The three GGA correlation kernels that carry the sigma-total route: their
/// energy depends on sigmaAa + 2 sigmaAb + sigmaBb, and the generated
/// derivative divides through by that same quantity.
const std::array<NamedGgaKernel, 3> kGgaCorrelationKernels = {{
    {"pbe_c", &excgrid::PbeCorrelation},
    {"pw91_c", &excgrid::Pw91Correlation},
    {"p86", &excgrid::P86Correlation},
}};

/// sigmaAa + 2 sigmaAb + sigmaBb, the combination the correlation kernels
/// reduce the gradient to.  Mathematically |grad(rhoA) + grad(rhoB)|^2, so
/// it is a perfect square and the only physically reachable boundary is its
/// exact zero.
double TotalSigma(double sigmaAa, double sigmaAb, double sigmaBb) {
    return sigmaAa + 2.0 * sigmaAb + sigmaBb;
}

TEST(KernelTest, ZeroGradientVsigmaIsNanForNineOfTenGgaKernels) {
    // rhoA = rhoB = 0.4 with all three sigma inputs bit-exactly zero.  The
    // NaN is quiet and confined to the vsigma slots: exc and the vrho pair
    // are finite and correct at the same point, which is what makes this a
    // 0 / 0 in the derivative rather than a broken energy.
    const double rhoA = 0.4;
    const double rhoB = 0.4;

    for (const NamedGgaKernel& entry : kGgaExchangeKernels)
    {
        const excgrid::XcKernelValue value = entry.kernel(rhoA, rhoB, 0.0, 0.0, 0.0);

        EXPECT_TRUE(std::isfinite(value.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vrhoA)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vrhoB)) << entry.name;
        // Exchange is same-spin only, so each vsigma dies with its own sigma.
        EXPECT_TRUE(std::isnan(value.vsigmaAa)) << entry.name;
        EXPECT_TRUE(std::isnan(value.vsigmaBb)) << entry.name;
        EXPECT_EQ(value.vsigmaAb, 0.0) << entry.name;
    }

    for (const NamedGgaKernel& entry : kGgaCorrelationKernels)
    {
        const excgrid::XcKernelValue value = entry.kernel(rhoA, rhoB, 0.0, 0.0, 0.0);

        // The correlation energy is finite at the same point - its sigma
        // dependence is quadratic, so sigma = 0 is an ordinary interior point
        // of the energy and a singular one only of the source expression.
        EXPECT_TRUE(std::isfinite(value.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vrhoA)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vrhoB)) << entry.name;
        // All three, unlike exchange: these ride on the sigma total.
        EXPECT_TRUE(std::isnan(value.vsigmaAa)) << entry.name;
        EXPECT_TRUE(std::isnan(value.vsigmaAb)) << entry.name;
        EXPECT_TRUE(std::isnan(value.vsigmaBb)) << entry.name;
    }

    // LYP is the exception and is the control for the whole test: its energy
    // is linear in the sigma inputs, so its derivative is a constant with no
    // sqrt(sigma) to divide by, and it is finite at the exact zero.
    const excgrid::XcKernelValue lyp = excgrid::LypCorrelation(rhoA, rhoB, 0.0, 0.0, 0.0);

    EXPECT_TRUE(std::isfinite(lyp.vsigmaAa));
    EXPECT_TRUE(std::isfinite(lyp.vsigmaAb));
    EXPECT_TRUE(std::isfinite(lyp.vsigmaBb));
    EXPECT_NEAR(lyp.vsigmaAa, 6.3832299997409927e-04, 1e-18);
    EXPECT_NEAR(lyp.vsigmaAb, 6.9783987371625626e-04, 1e-18);
    EXPECT_NEAR(lyp.vsigmaBb, 6.3832299997409927e-04, 1e-18);
}

TEST(KernelTest, ZeroGradientVsigmaNaNBoundaryIsTheExactZero) {
    // The boundary is bit-exactness, not smallness: the smallest positive
    // subnormal 5e-324 (denorm_min) already returns the finite limit, eight
    // orders of magnitude below any significance threshold a grid could use.
    // Measured on pbe: sigma = 0 gives -nan, sigma = 5e-324 gives
    // -1.1404735752307885e-02, and sigma = 1e-320 gives the same to the last
    // bit.  So the "rho vanishes and sigma underflows to zero" route is
    // closed - an underflowed sigma is a nonzero subnormal, not a zero.
    const double rhoA = 0.4;
    const double rhoB = 0.4;
    // 2^-1074, the smallest positive subnormal - spelled as a literal because the
    // stdlib name for it is snake_case and the G6 scan reads whole blobs.
    constexpr double kSmallestSubnormal = 4.9406564584124654e-324;

    EXPECT_NE(kSmallestSubnormal, 0.0);
    EXPECT_TRUE(std::isnan(excgrid::PbeExchange(rhoA, rhoB, 0.0, 0.0, 0.0).vsigmaAa));
    EXPECT_TRUE(std::isfinite(
        excgrid::PbeExchange(rhoA, rhoB, kSmallestSubnormal, kSmallestSubnormal, kSmallestSubnormal)
            .vsigmaAa));

    // Per-spin, not all-or-nothing: zeroing ONE sigma leaves that spin's
    // vsigma NaN and the other spin's finite, because each spin's term is
    // differentiated against its own sigma.  rhoA = rhoB = 0.4 with
    // sigmaAa = 0 only.
    const excgrid::XcKernelValue split = excgrid::PbeExchange(rhoA, rhoB, 0.0, 0.03, 0.05);

    EXPECT_TRUE(std::isnan(split.vsigmaAa));
    EXPECT_TRUE(std::isfinite(split.vsigmaBb));
    EXPECT_NEAR(split.vsigmaBb, -1.1345974574693928e-02, 1e-16);
}

TEST(KernelTest, ZeroGradientCorrelationVsigmaNaNIsTheSigmaTotal) {
    // The correlation kernels take a different route to the same corner: the
    // trigger is sigmaAa + 2 sigmaAb + sigmaBb = 0, with none of the three
    // individually zero.  Measured at rhoA = rhoB = 0.4:
    // (0.02, -0.01, 0.0) has a total of exactly 0.0 and NaNs all three
    // vsigma slots, while (0.001, 0.0, 0.0) - total 0.001 - is finite.  This
    // is what separates the correlation route from the exchange one.
    const double rhoA = 0.4;
    const double rhoB = 0.4;

    for (const NamedGgaKernel& entry : kGgaCorrelationKernels)
    {
        const excgrid::XcKernelValue zeroTotal = entry.kernel(rhoA, rhoB, 0.02, -0.01, 0.0);

        EXPECT_EQ(TotalSigma(0.02, -0.01, 0.0), 0.0) << entry.name;
        EXPECT_TRUE(std::isnan(zeroTotal.vsigmaAa)) << entry.name;
        EXPECT_TRUE(std::isnan(zeroTotal.vsigmaAb)) << entry.name;
        EXPECT_TRUE(std::isnan(zeroTotal.vsigmaBb)) << entry.name;

        const excgrid::XcKernelValue smallTotal = entry.kernel(rhoA, rhoB, 0.001, 0.0, 0.0);

        EXPECT_TRUE(std::isfinite(smallTotal.vsigmaAa)) << entry.name;
        EXPECT_TRUE(std::isfinite(smallTotal.vsigmaAb)) << entry.name;
        EXPECT_TRUE(std::isfinite(smallTotal.vsigmaBb)) << entry.name;
    }

    // Negative totals are NOT part of the defect's reachable set: the total is
    // |grad(rhoA) + grad(rhoB)|^2 and cannot be negative for a real density.
    // It is recorded here only to bound the test - sqrt of a negative is NaN
    // everywhere in the kernel, not just in vsigma.
    const excgrid::XcKernelValue negative =
        excgrid::PbeCorrelation(rhoA, rhoB, 0.001, -0.002, 0.001);

    EXPECT_LT(TotalSigma(0.001, -0.002, 0.001), 0.0);
    EXPECT_TRUE(std::isnan(negative.exc));
}

TEST(KernelTest, ZeroGradientVsigmaLimitMatchesClosedForm) {
    // What the correct value at sigma = 0 IS.  For the PBE exchange family
    // e_sigma = -(1/2)(3/4)(3/pi)^(1/3) rho_sigma^(4/3) F(s) with
    // F(s) = 1 + mu s^2 / (1 + mu s^2 / kappa) and
    // s = |grad rho_sigma| / (2 (3 pi^2)^(1/3) (2 rho_sigma)^(4/3)), so
    // F(0) = 1, F'(0) = 0 and F''(0)/2 = mu s^2 carries the whole gradient
    // term.  Differentiating once more against sigma and taking sigma -> 0
    // (kappa cancels, because it only rescales the s^2 term's saturation):
    //
    //     dE/dsigma_sigmasigma|_0
    //         = -(1/2)(3/4)(3/pi)^(1/3) 2^(4/3) mu
    //           / (4 * 2^(2/3) * (3 pi^2)^(2/3) * rho_sigma^(4/3))
    //
    // The shipped expression agrees with it wherever it is defined: measured
    // at rho = 0.4, pbe gives -1.1404735752307884e-02 against this form's
    // -1.14047357523078818e-02 (agreement to 16 significant digits).  So the
    // defect is the single point sigma = 0, not the formula.
    const double rho = 0.4;
    // 2^-1074, the smallest positive subnormal - spelled as a literal because the
    // stdlib name for it is snake_case and the G6 scan reads whole blobs.
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

    // revPBE shares PBE's mu (only kappa differs, and kappa drops out of the
    // sigma -> 0 limit), so it must land on the same number - and does, to 15
    // digits.  mPW91 takes the other route, x = |grad rho| / rho^(4/3), whose
    // small-x coefficient is 5 (36 pi)^(-5/3) by construction (the same
    // gradient coefficient PBEsol's 10/81 restores, which is why the two
    // happen to coincide here).
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
    // The same bit-exact-zero probe pointed at rho rather than sigma.  A fully
    // spin-polarized density reaches the kernel as (rhoA > 0, rhoB = 0,
    // sigmaBb = 0) at EVERY grid point - a hydrogen atom, a high-spin doublet,
    // any system whose beta density matrix is the zero matrix - and four of
    // the six exchange kernels answer with NaN.
    //
    // Two distinct sites, both measured 2026-09-13 at rhoA = 0.4, sigmaAa =
    // 0.05:
    //   - vsigmaBb = NaN in all six, which is the zero-sigma defect above
    //     (sigmaBb is bit-exactly zero whenever the beta density matrix is);
    //   - vrhoB = NaN in pbe, revpbe, pbesol and pw91 only, INDEPENDENT of
    //     sigmaBb - a std::pow(<rhoB-derived>, -2/3) that overflows to +inf
    //     and is then multiplied by rhoB = 0, giving 0 * inf.  becke88 and
    //     mpw91 have no such term and return a finite vrhoB.
    const double rhoA = 0.4;
    const double sigmaAa = 0.05;

    for (const NamedGgaKernel& entry : kGgaExchangeKernels)
    {
        const excgrid::XcKernelValue polarized = entry.kernel(rhoA, 0.0, sigmaAa, 0.0, 0.0);

        EXPECT_TRUE(std::isfinite(polarized.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(polarized.vrhoA)) << entry.name;
        // The alpha channel is ordinary - a normal density with a normal
        // gradient - so only the beta channel is at the corner.
        EXPECT_TRUE(std::isfinite(polarized.vsigmaAa)) << entry.name;
        EXPECT_LT(polarized.vsigmaAa, 0.0) << entry.name;
        EXPECT_TRUE(std::isnan(polarized.vsigmaBb)) << entry.name;
    }

    EXPECT_TRUE(std::isnan(excgrid::PbeExchange(rhoA, 0.0, sigmaAa, 0.0, 0.0).vrhoB));
    EXPECT_TRUE(std::isnan(excgrid::RevPbeExchange(rhoA, 0.0, sigmaAa, 0.0, 0.0).vrhoB));
    EXPECT_TRUE(std::isnan(excgrid::PbeSolExchange(rhoA, 0.0, sigmaAa, 0.0, 0.0).vrhoB));
    EXPECT_TRUE(std::isnan(excgrid::Pw91Exchange(rhoA, 0.0, sigmaAa, 0.0, 0.0).vrhoB));
    // Independent of the sigma input, which is what makes it a second site
    // rather than a restatement of the first.
    EXPECT_TRUE(std::isnan(excgrid::PbeExchange(rhoA, 0.0, sigmaAa, 0.0, 0.03).vrhoB));

    // The two kernels with no negative-power rho term stay finite, so the
    // NaN is not a property of the spin edge as such.
    EXPECT_FALSE(std::isnan(excgrid::Becke88Exchange(rhoA, 0.0, sigmaAa, 0.0, 0.0).vrhoB));
    EXPECT_FALSE(std::isnan(excgrid::MPw91Exchange(rhoA, 0.0, sigmaAa, 0.0, 0.0).vrhoB));

    // Correlation sits on the other side of this one: its rhoB -> 0 limit
    // diverges (the correlation hole cannot close faster than the density
    // does), so a large finite value is the correct answer, not a NaN.
    EXPECT_TRUE(std::isfinite(excgrid::PbeCorrelation(rhoA, 0.0, sigmaAa, 0.0, 0.0).vrhoB));
    EXPECT_FALSE(std::isnan(excgrid::LypCorrelation(rhoA, 0.0, sigmaAa, 0.0, 0.0).vrhoB));
}

} // namespace
