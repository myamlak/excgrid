// excgrid codegen source: the Slater (Dirac) LDA exchange functional.
// Fresh authorship; the formula is the published spin-scaled Dirac exchange
// (docs/mainpage.md references).  Regenerate with tools/regenerate.py
// (Yacas is a maintainer-only tool; the committed generated/ output is the
// build input).

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

// The excgrid LDA kernel skeleton (fresh authorship; BSD-3-Clause).
// Emits one excgrid::XcKernelValue function per LDA functional: the energy
// density and its spin-density derivatives, with the exact one-spin-zero
// limit branches (the generator's limit substitution supplies the limit
// expressions).

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

#include <cmath>
#include <limits>

namespace excgrid {

XcKernelValue SlaterExchange(double rhoA, double rhoB) {
    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        return result;
    }

    if (rhoA >= eps && rhoB >= eps)
    {
        const double C44 = 4. / 3.;

        result.exc = -0.9305257363491 * (std::pow(rhoA, C44) + std::pow(rhoB, C44));
    } else if (rhoA < eps)
    {
        result.exc = -0.9305257363491 * std::pow(rhoB, 4. / 3.);
    } else
    {
        result.exc = -0.9305257363491 * std::pow(rhoA, 4. / 3.);
    }

    if (rhoA >= eps)
    {
        result.vrhoA = -3.7221029453964 * std::pow(rhoA, 1. / 3.) / 3.;
    }

    if (rhoB >= eps)
    {
        result.vrhoB = -3.7221029453964 * std::pow(rhoB, 1. / 3.) / 3.;
    }

    return result;
}

} // namespace excgrid
