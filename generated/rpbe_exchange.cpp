// excgrid codegen source: the Hammer-Hansen-Norskov 1999 revised PBE GGA
// exchange functional (RPBE).  Fresh authorship.
//
// The enhancement factor Fx(s) = 1 + kappa (1 - exp(-mu s^2 / kappa)) has
// kappa = 0.804 and mu = beta pi^2 / 3, the PBE relation, evaluated with
// this tree's own full-precision PBE beta (X'PbeCorrBeta).  That product is
// 0.2195149727645171 - the standard PBE mu, and the same value the tree's
// pbe and revpbe rules carry - and it is what RPBE uses: RPBE keeps PBE's
// kappa and mu and changes only the enhancement's FORM, from PBE's rational
// expression to this exponential one.
// Verified against the oracle by excgrid/tools/verify_pyscf.py.
// Regenerate with tools/regenerate.py.

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

// The excgrid GGA kernel skeleton (fresh authorship; BSD-3-Clause).
// Emits one excgrid::XcKernelValue function per GGA functional: the energy
// density, the spin-density derivatives, and the three sigma derivatives.
// No spin-edge branches: the definitions' Tiny guards keep every expression
// finite at the spin and gamma edges (the LDA skeleton's exact-limit
// machinery is not needed for the gradient-corrected forms, whose edge
// contributions vanish with the spin density).
// The banner and the kernel.hpp include are emitted by the calling .ey, which
// always precedes this skeleton, so they are not repeated here.

#include <cmath>
#include <limits>

namespace excgrid {

XcKernelValue RpbeExchange(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
    (void)sigmaAb;

    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        return result;
    }

    const double C46 = 2. * rhoA;
    const double C47 = 2. * rhoB;
    const double C48 = 1. / 3.;
    const double C49 = std::pow(Pi, 2);
    const double C50 = C49 * C46;
    const double C51 = C49 * C47;
    const double C52 = 3. * C50;
    const double C53 = 3. * C51;
    const double C54 = std::pow(C52, C48);
    const double C55 = std::pow(C53, C48);

    result.exc =
        -((0.804 *
               (1. - std::exp(-0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                              0.804)) +
           1.) *
              rhoB * 3. * C55 +
          (0.804 *
               (1. - std::exp(-0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                              0.804)) +
           1.) *
              rhoA * 3. * C54) /
        (4. * Pi);

    const double C60 = 3. * C54;
    const double C61 = 4. * sigmaAa;
    const double C62 = 6. * C49;
    const double C63 = C54 * C46;
    const double C64 = -2. / 3.;
    const double C65 = C63 + 1e-16;
    const double C66 = std::sqrt(C61);
    const double C67 = std::pow(C52, C64);
    const double C68 = 2. * C65;
    const double C69 = C67 * C62;
    const double C70 = C66 / C68;
    const double C71 = std::pow(C70, 2);
    const double C72 = 0.2195149727645171 * C71;
    const double C73 = C72 / 0.804;
    const double C74 = -C73;
    const double C75 = std::exp(C74);

    result.vrhoA =
        -((0.804 * (1. - C75) + 1.) * (rhoA * 3. * C69 / 3. + C60) +
          rhoA * C60 * 0.804 * C75 * -0.8780598910580684 * (2. * C54 + 2. * rhoA * C69 / 3.) * C61 /
              (1.608 * C65 * std::pow(C68, 2))) /
        (4. * Pi);

    const double C77 = 3. * C55;
    const double C78 = 4. * sigmaBb;
    const double C79 = 6. * C49;
    const double C80 = C55 * C47;
    const double C81 = -2. / 3.;
    const double C82 = C80 + 1e-16;
    const double C83 = std::sqrt(C78);
    const double C84 = std::pow(C53, C81);
    const double C85 = 2. * C82;
    const double C86 = C84 * C79;
    const double C87 = C83 / C85;
    const double C88 = std::pow(C87, 2);
    const double C89 = 0.2195149727645171 * C88;
    const double C90 = C89 / 0.804;
    const double C91 = -C90;
    const double C92 = std::exp(C91);

    result.vrhoB =
        -((0.804 * (1. - C92) + 1.) * (rhoB * 3. * C86 / 3. + C77) +
          rhoB * C77 * 0.804 * C92 * -0.8780598910580684 * (2. * C55 + 2. * rhoB * C86 / 3.) * C78 /
              (1.608 * C82 * std::pow(C85, 2))) /
        (4. * Pi);

    const double C94 = 2. * rhoA;
    const double C95 = 1. / 3.;
    const double C96 = std::pow(Pi, 2);
    const double C97 = C96 * C94;
    const double C98 = 3. * C97;
    const double C99 = std::pow(C98, C95);
    const double C100 = C99 * C94;
    const double C101 = C100 + 1e-16;

    result.vsigmaAa =
        -4.2357609144641219 *
        std::exp(-0.2195149727645171 * std::pow(std::sqrt(4. * sigmaAa) / (2. * C101), 2) / 0.804) *
        C99 * rhoA / (0.25728e2 * Pi * std::pow(C101, 2));

    result.vsigmaAb = 0;

    const double C104 = 2. * rhoB;
    const double C105 = 1. / 3.;
    const double C106 = std::pow(Pi, 2);
    const double C107 = C106 * C104;
    const double C108 = 3. * C107;
    const double C109 = std::pow(C108, C105);
    const double C110 = C109 * C104;
    const double C111 = C110 + 1e-16;

    result.vsigmaBb =
        -4.2357609144641219 *
        std::exp(-0.2195149727645171 * std::pow(std::sqrt(4. * sigmaBb) / (2. * C111), 2) / 0.804) *
        C109 * rhoB / (0.25728e2 * Pi * std::pow(C111, 2));

    return result;
}

} // namespace excgrid
