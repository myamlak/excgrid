// The removable 0/0 the asinh exchange channels carry at a zero gradient: the
// generated tiers write asinh(x)/x and x^-1 times an odd function through these
// helpers, which carry the exact limits at x = 0.
#pragma once

#include <cmath>

namespace excgrid::internal {

// asinh(x) / x, with the exact limit 1 at x = 0.
inline double AsinhOverX(double x) noexcept
{
    return x == 0.0 ? 1.0 : std::asinh(x) / x;
}

// d/d(x^2) [asinh(x) / x], exact limit -1/6 at x = 0; below x^2 = 1e-4 the
// cancelling closed form is replaced by its series
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
