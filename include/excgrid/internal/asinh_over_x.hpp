// The removable 0/0 the asinh exchange channels carry at a zero gradient.
//
// The Becke88 and PW91 reduced gradients x = sqrt(sigma)/rho^(4/3) enter
// through asinh(x)/x and through x^(-1) times an odd function.  Both ratios
// have finite limits at x = 0, but the emitted arithmetic is 0/0 there, and
// the plain second derivative of the asinh channel also cancels two terms of
// order sigma^(-1): no radicand guard is both representable in double and
// invisible in the result.  The generated tiers therefore write the two
// ratios through these helpers, which carry the exact limits at x = 0 and a
// series for the derivative near it.  This is generator support, not part of
// the public contract.
#pragma once

#include <cmath>

namespace excgrid::internal {

// asinh(x) / x, with the exact limit 1 at x = 0.
inline double AsinhOverX(double x) noexcept
{
    return x == 0.0 ? 1.0 : std::asinh(x) / x;
}

// d/d(x^2) [asinh(x) / x], with the exact limit -1/6 at x = 0.
//
// The closed form (1/sqrt(1+x^2) - asinh(x)/x) / (2 x^2) subtracts two
// numbers that agree to O(x^2), so below x^2 = 1e-4 it is replaced by its
// series, whose first omitted term is about 1.1e-17 at the 1e-4 boundary:
//   -1/6 + 3 x^2/20 - 15 x^4/112 + 35 x^6/288.
inline double AsinhOverXDerivX2(double x) noexcept
{
    const double z = x * x;
    if (z < 1e-4)
    {
        return -1.0 / 6.0 + z * (3.0 / 20.0 - z * (15.0 / 112.0 - z * (35.0 / 288.0)));
    }
    return (1.0 / std::sqrt(1.0 + z) - std::asinh(x) / x) / (2.0 * z);
}

} // namespace excgrid::internal
