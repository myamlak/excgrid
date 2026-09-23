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
// It also emits the functional's SECOND-DERIVATIVE tier: one function filling
// the contract's materialised matrix, from the same expression as the kernel.
// The matrix is the contract's upper triangle, row-major over the two
// densities in identifier order, which is the LDA tier's whole span - an LDA
// energy density reads no other component, so the gradient and
// kinetic-energy-density rows and columns are not emitted at all rather than
// emitted as zeros.  The tier is taken from the guarded expression, not from
// the limit branches above: the Tiny guards already keep it finite at the spin
// edges, which is what makes it a straight-line form where the order-1 kernel
// is not.
// The banner and the kernel.hpp include are emitted by the calling .ey, which
// always precedes this skeleton, so they are not repeated here.

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

void SlaterExchangeSecondDerivatives(double rhoA,
                                     double rhoB,
                                     PointSecondDerivativeMatrix& matrix) {
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    if (rhoA + rhoB < 2. * std::numeric_limits<double>::epsilon())
    {
        return;
    }

    matrix.upper[2] = -3.7221029453964 * std::pow(rhoB, -2. / 3.) / 9.;

    matrix.upper[1] = 0;

    matrix.upper[0] = -3.7221029453964 * std::pow(rhoA, -2. / 3.) / 9.;
}

} // namespace excgrid
