// The generated-kernel verification suite: closed forms, symmetry,
// the finite-difference channel (symbolic derivatives vs numerical
// derivatives of the same energy expression - catches transcription
// and differentiation errors with no external reference), exact
// conditions, and the Vxc-integrates-to-Exc consistency identity.

#include "excgrid/kernel.hpp"
#include "excgrid/kernels.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
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
// WHY a shipped GGA kernel used to answer NaN in vsigma at sigma == 0: the
// generated expression carried the same sqrt(sigma) factor in its numerator
// and its denominator - the chain rule's ds/dsigma = 1 / (2 s) multiplied
// through by s - so an exactly zero sigma evaluated 0 / 0.  The derivative has
// a finite, NONZERO limit there, which is why the smallest positive subnormal
// (5e-324) already returned it: a single REMOVABLE point, not an overflow and
// not a divergence.
//
// THE FIX IS THE CANCELLATION, in the generator and not here.  The shared
// sqrt(gamma) is an exact subtree of both products, so `ExCancelRadical` in
// xc_defs/excgrid_generate.ys divides it out before the CSE and the emitted
// quotient is the analytic derivative, defined at the corner.  A clamp to zero
// would have put an invented number there; a limit branch would have left the
// indeterminate quotient in place.  This is the cause repaired, not the
// symptom hidden.
//
// THE FIX IS PARTIAL: four of the nine affected kernels.  `pbe`, `revpbe`,
// `rpbe` and `pbesol` return the limit now; the other five still return NaN
// at a bit-exactly zero sigma, and their tests still pin that:
//
//   * `becke88`, `pw91`, `mpw91` carry a SECOND singular site of a different
//     shape - `asinh(x) / sqrt(sigma)`, whose numerator vanishes like x but is
//     not a product carrying the radical, so there is no common FACTOR to
//     cancel.  Reaching it needs `asinh(x)/x` written as a function of x^2, or
//     a limit branch: a different change from this one.
//   * `pbe_c`, `pw91_c`, `p86` carry the same radical inside a sum, where the
//     numerator and denominator do not hold it the same number of times.  A
//     recursive cancellation was built for them and REJECTED on measurement -
//     it returned their vsigma exactly 4x too large and `GgaFiniteDifference`
//     failed on it while `exc` and the `vrho` channels stayed bit-identical.
//     It is a correctness trap, not a missing feature.
//
// The tests below were written to PIN the defect and have been INVERTED where
// the fix landed - the EXPECT_TRUE(std::isnan(...)) lines became assertions of
// the limit for the four kernels that hold it, and stay as pins for the six
// that do not.  The inversions are the evidence that the fix did something.

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

/// The PART of the affected set the radical cancellation actually fixes: the
/// four exchange kernels whose derivative carries the shared sqrt(gamma) as a
/// factor of its single top-level quotient.  Kept as its own list so the
/// tests below cannot quietly claim the whole family - five kernels still
/// return NaN at the exact zero (becke88, pw91, mpw91, pbe_c, pw91_c, p86).
const std::array<NamedGgaKernel, 4> kRadicalCancelledKernels = {{
    {"pbe", &excgrid::PbeExchange},
    {"revpbe", &excgrid::RevPbeExchange},
    {"rpbe", &excgrid::RpbeExchange},
    {"pbesol", &excgrid::PbeSolExchange},
}};

/// The two exchange kernels left on the OTHER shape: `asinh(x) / sqrt(sigma)`,
/// whose numerator vanishes like x but is not a product carrying the radical,
/// so there is no common factor to cancel.  becke88 and pw91 and mpw91 all
/// take it; they are named here so the tests assert their held state rather
/// than skipping them.
const std::array<NamedGgaKernel, 3> kAsinhShapedKernels = {{
    {"becke88", &excgrid::Becke88Exchange},
    {"pw91", &excgrid::Pw91Exchange},
    {"mpw91", &excgrid::MPw91Exchange},
}};

/// sigmaAa + 2 sigmaAb + sigmaBb, the combination the correlation kernels
/// reduce the gradient to.  Mathematically |grad(rhoA) + grad(rhoB)|^2, so
/// it is a perfect square and the only physically reachable boundary is its
/// exact zero.
double TotalSigma(double sigmaAa, double sigmaAb, double sigmaBb) {
    return sigmaAa + 2.0 * sigmaAb + sigmaBb;
}

TEST(KernelTest, ZeroGradientVsigmaExchangeReturnsTheLimit) {
    // rhoA = rhoB = 0.4 with all three sigma inputs bit-exactly zero, the
    // configuration that used to return NaN in the same-spin vsigma slots.
    // The energy and the vrho pair were always finite and correct here, which
    // is what made it a 0 / 0 in the derivative rather than a broken energy;
    // the derivative is finite too now.
    constexpr double kSmallestSubnormal = 4.9406564584124654e-324;
    const double rhoA = 0.4;
    const double rhoB = 0.4;

    for (const NamedGgaKernel& entry : kRadicalCancelledKernels)
    {
        const excgrid::XcKernelValue value = entry.kernel(rhoA, rhoB, 0.0, 0.0, 0.0);

        EXPECT_TRUE(std::isfinite(value.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vrhoA)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vrhoB)) << entry.name;
        // Exchange is same-spin only.  Each vsigma is finite at its own
        // sigma's zero and NEGATIVE - the exchange enhancement falls away
        // with the gradient, so dE/dsigma < 0 is the physical sign.
        EXPECT_TRUE(std::isfinite(value.vsigmaAa)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vsigmaBb)) << entry.name;
        EXPECT_LT(value.vsigmaAa, 0.0) << entry.name;
        EXPECT_LT(value.vsigmaBb, 0.0) << entry.name;
        EXPECT_EQ(value.vsigmaAb, 0.0) << entry.name;

        // The value AT the corner is the value one representable step off it,
        // bit for bit: the cancellation defines the one point the 0 / 0 left
        // undefined and moves nothing else.  This equality is what the fix
        // bought; before it the left side was NaN and the right side was not.
        const excgrid::XcKernelValue limit =
            entry.kernel(rhoA, rhoB, kSmallestSubnormal, kSmallestSubnormal, kSmallestSubnormal);

        EXPECT_EQ(std::bit_cast<std::uint64_t>(value.vsigmaAa),
                  std::bit_cast<std::uint64_t>(limit.vsigmaAa))
            << entry.name;
        EXPECT_EQ(std::bit_cast<std::uint64_t>(value.vsigmaBb),
                  std::bit_cast<std::uint64_t>(limit.vsigmaBb))
            << entry.name;
    }

    // The other three exchange kernels are HELD, not skipped.  Their second
    // singular site is asinh(x) / sqrt(sigma) - the numerator vanishes but is
    // not a product carrying the radical, so no factor cancellation reaches
    // it.  They must be asserted as still-NaN here, so that a later change
    // which fixes them is a deliberate inversion of this line and not a
    // silent difference.
    for (const NamedGgaKernel& entry : kAsinhShapedKernels)
    {
        const excgrid::XcKernelValue value = entry.kernel(rhoA, rhoB, 0.0, 0.0, 0.0);

        EXPECT_TRUE(std::isfinite(value.exc)) << entry.name;
        EXPECT_TRUE(std::isnan(value.vsigmaAa)) << entry.name;
        EXPECT_TRUE(std::isnan(value.vsigmaBb)) << entry.name;
        EXPECT_TRUE(std::isfinite(entry.kernel(rhoA, rhoB, kSmallestSubnormal,
                                               kSmallestSubnormal, kSmallestSubnormal)
                                      .vsigmaAa))
            << entry.name;
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
    // The boundary the defect lived on was bit-exactness, not smallness: while
    // the NaN was there, sigma = 0 answered -nan and the smallest positive
    // subnormal 5e-324 (denorm_min) already answered the finite limit, eight
    // orders of magnitude below any significance threshold a grid could use.
    // That is still the boundary the FIX defines - exactly one point - and it
    // is why the "rho vanishes and sigma underflows to zero" route was always
    // closed: an underflowed sigma is a nonzero subnormal, not a zero.
    const double rhoA = 0.4;
    const double rhoB = 0.4;
    // 2^-1074, the smallest positive subnormal - spelled as a literal because the
    // stdlib name for it is snake_case and the G6 scan reads whole blobs.
    constexpr double kSmallestSubnormal = 4.9406564584124654e-324;

    EXPECT_NE(kSmallestSubnormal, 0.0);
    EXPECT_TRUE(std::isfinite(excgrid::PbeExchange(rhoA, rhoB, 0.0, 0.0, 0.0).vsigmaAa));
    EXPECT_TRUE(std::isfinite(
        excgrid::PbeExchange(rhoA, rhoB, kSmallestSubnormal, kSmallestSubnormal, kSmallestSubnormal)
            .vsigmaAa));

    // Per-spin, not all-or-nothing: each spin's term is differentiated against
    // its own sigma, so zeroing ONE sigma leaves the other spin's value
    // untouched.  rhoA = rhoB = 0.4 with sigmaAa = 0 only.
    const excgrid::XcKernelValue split = excgrid::PbeExchange(rhoA, rhoB, 0.0, 0.03, 0.05);
    const excgrid::XcKernelValue splitLimit = excgrid::PbeExchange(
        rhoA, rhoB, kSmallestSubnormal, 0.03, 0.05);

    EXPECT_TRUE(std::isfinite(split.vsigmaAa));
    EXPECT_TRUE(std::isfinite(split.vsigmaBb));
    EXPECT_NEAR(split.vsigmaBb, -1.1345974574693928e-02, 1e-16);
    EXPECT_EQ(std::bit_cast<std::uint64_t>(split.vsigmaAa),
              std::bit_cast<std::uint64_t>(splitLimit.vsigmaAa));
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
    // any system whose beta density matrix is the zero matrix - and two of
    // the sites it exposes are still live.
    //
    // Two distinct sites, both measured at rhoA = 0.4, sigmaAa = 0.05:
    //   - vsigmaBb = bit-exactly zero in all six, which was the zero-sigma
    //     defect above; FIXED there, and asserted finite below.  The two are
    //     worth keeping in one test because a fully spin-polarized density is
    //     what makes the sigma corner reachable on a real grid at all;
    //   - vrhoB = NaN in pbe, revpbe, pbesol and pw91 only, INDEPENDENT of
    //     sigmaBb - a std::pow(<rhoB-derived>, -2/3) that overflows to +inf
    //     and is then multiplied by rhoB = 0, giving 0 * inf.  becke88 and
    //     mpw91 have no such term and return a finite vrhoB.  STILL LIVE: it
    //     is a different mechanism (0 * inf, not 0 / 0) reached through rho
    //     rather than sigma, so the radical cancellation does not reach it.
    constexpr double kSmallestSubnormal = 4.9406564584124654e-324;
    const double rhoA = 0.4;
    const double sigmaAa = 0.05;

    for (const NamedGgaKernel& entry : kRadicalCancelledKernels)
    {
        const excgrid::XcKernelValue polarized = entry.kernel(rhoA, 0.0, sigmaAa, 0.0, 0.0);

        EXPECT_TRUE(std::isfinite(polarized.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(polarized.vrhoA)) << entry.name;
        // The alpha channel is ordinary - a normal density with a normal
        // gradient - so only the beta channel is at the corner.
        EXPECT_TRUE(std::isfinite(polarized.vsigmaAa)) << entry.name;
        EXPECT_LT(polarized.vsigmaAa, 0.0) << entry.name;
        // The beta sigma is bit-exactly zero here, which is the corner the
        // cancellation removed: finite now, and equal to the value one
        // representable step off it.
        EXPECT_TRUE(std::isfinite(polarized.vsigmaBb)) << entry.name;
        EXPECT_EQ(std::bit_cast<std::uint64_t>(polarized.vsigmaBb),
                  std::bit_cast<std::uint64_t>(
                      entry.kernel(rhoA, 0.0, sigmaAa, 0.0, kSmallestSubnormal).vsigmaBb))
            << entry.name;
    }

    // The three asinh-shaped kernels hold the same sigma corner they always
    // did, for the second reason above rather than the first.
    for (const NamedGgaKernel& entry : kAsinhShapedKernels)
    {
        const excgrid::XcKernelValue polarized = entry.kernel(rhoA, 0.0, sigmaAa, 0.0, 0.0);

        EXPECT_TRUE(std::isfinite(polarized.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(polarized.vsigmaAa)) << entry.name;
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

TEST(KernelTest, ZeroSpinDensityIsSymmetricInTheTwoSpins) {
    // The mirror of the test above, and the half it does not reach: the same
    // four kernels answer NaN for vrhoA at rhoA = 0.  Measured 2026-09-18 at
    // rhoB = 0.4, sigmaBb = 0.05.  A density matrix that is zero in the ALPHA
    // channel is as legal as one zero in beta - it is the same high-spin
    // system written with the axes swapped - and the generated vrhoA carries
    // the same pow(<rhoA-derived>, -2/3) term that overflows to +inf and is
    // then multiplied by rhoA = 0.  Reading the defect off one spin alone
    // would leave half of it unpinned.
    const double rhoB = 0.4;
    const double sigmaBb = 0.05;

    EXPECT_TRUE(std::isnan(excgrid::PbeExchange(0.0, rhoB, 0.0, 0.0, sigmaBb).vrhoA));
    EXPECT_TRUE(std::isnan(excgrid::RevPbeExchange(0.0, rhoB, 0.0, 0.0, sigmaBb).vrhoA));
    EXPECT_TRUE(std::isnan(excgrid::PbeSolExchange(0.0, rhoB, 0.0, 0.0, sigmaBb).vrhoA));
    EXPECT_TRUE(std::isnan(excgrid::Pw91Exchange(0.0, rhoB, 0.0, 0.0, sigmaBb).vrhoA));

    // The same two kernels that stay finite on the beta side stay finite here.
    EXPECT_FALSE(std::isnan(excgrid::Becke88Exchange(0.0, rhoB, 0.0, 0.0, sigmaBb).vrhoA));
    EXPECT_FALSE(std::isnan(excgrid::MPw91Exchange(0.0, rhoB, 0.0, 0.0, sigmaBb).vrhoA));

    // Everything away from the zero channel of the same call is ordinary.
    for (const NamedGgaKernel& entry : kGgaExchangeKernels)
    {
        const excgrid::XcKernelValue value = entry.kernel(0.0, rhoB, 0.0, 0.0, sigmaBb);

        EXPECT_TRUE(std::isfinite(value.exc)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vrhoB)) << entry.name;
        EXPECT_TRUE(std::isfinite(value.vsigmaBb)) << entry.name;
    }
}

TEST(KernelTest, ZeroGradientNanIsTheIndefiniteQuietNan) {
    // The two sites that still answer NaN both do it by an INVALID OPERATION -
    // a 0 / 0 in the correlation sigma derivative and a 0 * inf in the density
    // derivative - and on x64 SSE2 both answer with the hardware's single
    // "indefinite"
    // form: a quiet NaN with a zero payload.  Pinning that payload and not
    // merely isnan() is what separates this class from its neighbours: a NaN
    // carrying a payload, or a signalling one, would have to come from
    // somewhere else.
    //
    // The SIGN is deliberately NOT asserted here, and that is a measurement
    // rather than a simplification.  It comes from the instruction the
    // compiler happens to emit, not from the operands, so it is not the same
    // everywhere for the same source: both of these sites answer -nan(ind) on
    // this x64 build and both answer +nan(ind) on linux-arm64 gcc, macos x64
    // and windows-arm64.  An earlier revision of this test pinned the two
    // signs after measuring them on one machine, and CI went red on all three
    // of those platforms - which is why the assertion is gone rather than
    // widened.  The payload and the quiet bit are the portable part, and they
    // are what this test exists to pin.
    constexpr std::uint64_t kQuietPayloadZero = 0x0008000000000000ULL;
    constexpr std::uint64_t kPayloadMask = 0x000FFFFFFFFFFFFFULL;
    const double rho = 0.4;

    const double densityZero = excgrid::PbeExchange(rho, 0.0, 0.05, 0.0, 0.0).vrhoB;
    const double totalZero = excgrid::PbeCorrelation(rho, rho, 0.0, 0.0, 0.0).vsigmaAa;

    for (const double value : {densityZero, totalZero})
    {
        EXPECT_TRUE(std::isnan(value));
        EXPECT_EQ(std::bit_cast<std::uint64_t>(value) & kPayloadMask, kQuietPayloadZero);
    }

    // The third site this test used to carry - the exchange 0 / 0 at an
    // exactly zero sigma - is gone, and that is the point of keeping the two
    // survivors in the same test: same hardware NaN, different mechanisms,
    // and only one of them was the removable singularity.
    EXPECT_TRUE(std::isfinite(excgrid::PbeExchange(rho, rho, 0.0, 0.0, 0.0).vsigmaAa));
}

TEST(KernelTest, ZeroGradientVsigmaAtExactZeroIsTheAcceptanceTarget) {
    // What the fix had to produce, stated so it can be checked rather than
    // argued about.  PbeExchange's generated vsigmaAa used to multiply the
    // numerator AND the denominator by C97 = sqrt(4 sigmaAa), which is the
    // chain rule's ds/dsigma = 1 / (2 s) written through by s.  Cancelling
    // exactly that one factor - the only change the repair makes - leaves a
    // quotient with no sigma radical in it at all:
    //
    //   numerator   = -rhoA * 3 * C99 * 1.4119203048213739
    //   denominator = 1.608 * C101 * 2 * C102 * (mu s^2 / kappa + 1)^2 * 4 pi
    //
    // That expression is defined at sigmaAa = 0, and its value there is the
    // limit the kernel already returns one representable step away, at
    // sigmaAa = 5e-324.  The two agreeing BIT FOR BIT is the acceptance
    // criterion: after the fix, vsigmaAa at exactly zero must equal the
    // reduced form evaluated at exactly zero, and must equal the line below.
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

    // The reduced form is finite at the exact zero, and its numerator is a
    // plain product - the cancelled factor really is gone.
    EXPECT_TRUE(std::isfinite(reducedLimit));
    EXPECT_NE(reducedNumerator, 0.0);

    // THE ACCEPTANCE ASSERTION.  Before the fix this failed with the left side
    // NaN and the right side a finite number: the kernel had no value at the
    // corner at all.  It passes now, and what it says is that the corner is an
    // ordinary point of the emitted expression - the value there is the value
    // one representable step off it, bit for bit, so the cancellation defined
    // exactly one point and moved nothing.
    const double cornerValue = excgrid::PbeExchange(rhoA, rhoA, 0.0, 0.0, 0.0).vsigmaAa;
    const double shippedLimit = excgrid::PbeExchange(rhoA, rhoA, kSmallestSubnormal,
                                                     kSmallestSubnormal, kSmallestSubnormal)
                                    .vsigmaAa;

    EXPECT_TRUE(std::isfinite(cornerValue));
    EXPECT_LT(cornerValue, 0.0);
    EXPECT_EQ(std::bit_cast<std::uint64_t>(cornerValue),
              std::bit_cast<std::uint64_t>(shippedLimit));

    // And the value is the right number, not merely a number: the independent
    // hand-derived reduced form above agrees to well inside the tolerance the
    // published point check is run at.  It is not asserted bit-for-bit because
    // the generator folds the emitted constants differently from the hand
    // derivation (3 * 1.4119203048213739 against 4.2357609144641219), and that
    // regrouping is worth the last bit or two - measured at up to 7 ulp across
    // a 300k-point sweep, 1.5e-15 relative at worst.
    EXPECT_NEAR(cornerValue, reducedLimit, 1e-13 * std::abs(reducedLimit));
}

} // namespace
