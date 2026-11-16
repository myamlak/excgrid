#include "excgrid/internal/asinh_over_x.hpp"
#include "excgrid/kernel.hpp"
#include "excgrid/kernels.hpp"

#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>

namespace {

// Upper-triangle index: i*active - i*(i-1)/2 + (j-i).
constexpr std::size_t Packed(std::size_t i, std::size_t j, std::size_t active) {
    return i * active - i * (i - 1) / 2 + (j - i);
}

// ---------------------------------------------------------------------------
// 1. The helpers' exact limits at a zero gradient

// asinh(x) = x - x^3/6 + 3 x^5/40 - 5 x^7/112 + ...
// => asinh(x)/x = 1 - x^2/6 + 3 x^4/40 - ..., whose x -> 0 limit is 1.
// With u = x^2 the same series is 1 - u/6 + 3 u^2/40 - ..., and differentiating
// it gives d/du [asinh(x)/x] = -1/6 + 3 u/20 - 15 u^2/112 + ..., whose
// u -> 0 limit is -1/6.  The two limits are therefore exactly 1 and -1/6.
TEST(AsinhOverXTest, TheHelpersCarryTheirExactLimitsAtZero) {
    EXPECT_DOUBLE_EQ(excgrid::internal::AsinhOverX(0.0), 1.0);
    EXPECT_DOUBLE_EQ(excgrid::internal::AsinhOverXDerivX2(0.0), -1.0 / 6.0);

    // Continuation: at x = 1e-3 (u = 1e-6) the next omitted term of each series is
    // below 5e-20, so the truncated series above is exact to the last-place rounding
    // of the two evaluations (one ulp at 1.0 is 2.2e-16; 1e-15 is a few ulp).
    constexpr double kX = 1e-3;
    constexpr double kU = kX * kX;
    const double asinhOverXSeries = 1.0 - kU / 6.0 + 3.0 * kU * kU / 40.0;
    const double derivX2Series = -1.0 / 6.0 + 3.0 * kU / 20.0 - 15.0 * kU * kU / 112.0;

    EXPECT_NEAR(excgrid::internal::AsinhOverX(kX), asinhOverXSeries, 1e-15);
    EXPECT_NEAR(excgrid::internal::AsinhOverXDerivX2(kX), derivX2Series, 1e-15);
}

// ---------------------------------------------------------------------------
// 2. The becke88 corner at the plan's input

// The hand-derived sigma-sigma entries (generated/becke88_exchange.cpp, upper[9]
// and upper[14]) are, with x = sqrt(sigma)/(rho + Tiny)^(4/3), w = asinh(x)/x,
// r = 1/sqrt(1 + x^2) and d = 1 + 6 beta x asinh(x):
//
//   E''(rho, sigma) = rho^(4/3) / (4 (rho + Tiny)^(16/3))
//                     * 6 beta^2 * (d (2 + x^2) r^3 + 3 d (w + r)
//                                   - 12 beta x^2 (w + r)^2) / d^3.
//
// At sigma = 0: x = 0, w = 1 (the first test), r = 1 and d = 1, so
//
//   E''(rho, 0) = rho^(4/3) * 6 beta^2 * (2 + 6) / (4 (rho + Tiny)^(16/3))
//               = 12 beta^2 rho^(4/3) / (rho + Tiny)^(16/3)      (emitted)
//               = 12 beta^2 / rho^4 * (rho/(rho + Tiny))^(16/3),
//
// whose guard-free (Tiny -> 0) limit is 12 beta^2 / rho^4.  The plan's acceptance
// constants are that limit rounded to 15 significant digits: an 80-digit
// evaluation of 12 beta^2/rho^4 with beta = 0.0042 gives 0.022920973101856915...
// at rhoA and 0.090362680144798852... at rhoB.  Both constants are therefore
// guard-free limits, not emitted values; the emitted value is the guarded form
// above, which the in-test derivation pins and the assertions below evaluate.
constexpr double kTiny = 1e-16;
constexpr double kBecke88Beta = 0.0042;
constexpr double kRhoA = 0.31;
constexpr double kRhoB = 0.22;

// Plan section 2 item 5.
constexpr double kGuardFreeSigmaAaAa = 0.0229209731018569;
constexpr double kGuardFreeSigmaBbBb = 0.0903626801447989;

double GuardedSigmaSigmaCurvature(double rho) {
    return 12.0 * kBecke88Beta * kBecke88Beta * std::pow(rho, 4.0 / 3.0) /
           std::pow(rho + kTiny, 16.0 / 3.0);
}

double GuardFreeSigmaSigmaCurvature(double rho) {
    return 12.0 * kBecke88Beta * kBecke88Beta / std::pow(rho, 4.0);
}

TEST(AsinhOverXTest, TheBecke88CornerIsTheGuardedClosedFormCurvature) {
    constexpr std::size_t kActive = 5;
    constexpr std::size_t kSigmaAaSlot = 2;
    constexpr std::size_t kSigmaBbSlot = 4;

    excgrid::PointSecondDerivativeMatrix matrix;
    excgrid::Becke88ExchangeSecondDerivatives(kRhoA, kRhoB, 0.0, 0.0, 0.0, matrix);

    const double emittedAaAa = matrix.upper[Packed(kSigmaAaSlot, kSigmaAaSlot, kActive)];
    const double emittedBbBb = matrix.upper[Packed(kSigmaBbSlot, kSigmaBbSlot, kActive)];

    // The emitted entries are the guarded closed form: the pow's and the division
    // round to a few ulp of it (1e-15 relative is about three ulp at these values).
    EXPECT_NEAR(emittedAaAa, GuardedSigmaSigmaCurvature(kRhoA), 1e-15 * emittedAaAa);
    EXPECT_NEAR(emittedBbBb, GuardedSigmaSigmaCurvature(kRhoB), 1e-15 * emittedBbBb);

    // The plan's constants are the guard-free limits: 15 significant digits carry at
    // most half an ulp (5e-16 relative), and evaluating 12 beta^2/rho^4 in double adds
    // a few ulp; 2e-15 relative covers both.
    EXPECT_NEAR(kGuardFreeSigmaAaAa, GuardFreeSigmaSigmaCurvature(kRhoA),
                2e-15 * kGuardFreeSigmaAaAa);
    EXPECT_NEAR(kGuardFreeSigmaBbBb, GuardFreeSigmaSigmaCurvature(kRhoB),
                2e-15 * kGuardFreeSigmaBbBb);

    // Against the acceptance constants the emitted entry carries the rho + Tiny guard's
    // relative shift, bounded by (16/3) Tiny/rho in the limit: 1.72e-15 at rhoA and
    // 2.42e-15 at rhoB.  The 15-digit rounding of the constant (up to 5e-16) and the
    // emitted chain's few ulp add to that; 5e-15 relative covers the measured total
    // (3.4e-15 at rhoB) without admitting a different closed form.
    EXPECT_NEAR(emittedAaAa, kGuardFreeSigmaAaAa, 5e-15 * kGuardFreeSigmaAaAa);
    EXPECT_NEAR(emittedBbBb, kGuardFreeSigmaBbBb, 5e-15 * kGuardFreeSigmaBbBb);

    // The guard lowers the curvature, so the emitted value sits below its guard-free
    // limit at both channels.
    EXPECT_LT(emittedAaAa, kGuardFreeSigmaAaAa);
    EXPECT_LT(emittedBbBb, kGuardFreeSigmaBbBb);
}

} // namespace
