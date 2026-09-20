// excgrid codegen source: the Perdew-Burke-Ernzerhof 1996 GGA exchange
// functional.  Fresh authorship; formula from the PBE paper
// (docs/mainpage.md references).  Regenerate with tools/regenerate.py.

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

XcKernelValue PbeExchange(
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
        -((1.804 -
           0.804 / (0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) / 0.804 +
                    1.)) *
              rhoB * 3. * C55 +
          (1.804 -
           0.804 / (0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) / 0.804 +
                    1.)) *
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
    const double C74 = C73 + 1.;

    result.vrhoA = -((1.804 - 0.804 / C74) * (rhoA * 3. * C69 / 3. + C60) +
                     rhoA * C60 * -0.705960152410687 * (2. * C54 + 2. * rhoA * C69 / 3.) * C61 /
                         (1.608 * C65 * std::pow(C68, 2) * std::pow(C74, 2))) /
                   (4. * Pi);

    const double C76 = 3. * C55;
    const double C77 = 4. * sigmaBb;
    const double C78 = 6. * C49;
    const double C79 = C55 * C47;
    const double C80 = -2. / 3.;
    const double C81 = C79 + 1e-16;
    const double C82 = std::sqrt(C77);
    const double C83 = std::pow(C53, C80);
    const double C84 = 2. * C81;
    const double C85 = C83 * C78;
    const double C86 = C82 / C84;
    const double C87 = std::pow(C86, 2);
    const double C88 = 0.2195149727645171 * C87;
    const double C89 = C88 / 0.804;
    const double C90 = C89 + 1.;

    result.vrhoB = -((1.804 - 0.804 / C90) * (rhoB * 3. * C85 / 3. + C76) +
                     rhoB * C76 * -0.705960152410687 * (2. * C55 + 2. * rhoB * C85 / 3.) * C77 /
                         (1.608 * C81 * std::pow(C84, 2) * std::pow(C90, 2))) /
                   (4. * Pi);

    const double C92 = 2. * rhoA;
    const double C93 = 1. / 3.;
    const double C94 = std::pow(Pi, 2);
    const double C95 = C94 * C92;
    const double C96 = 3. * C95;
    const double C97 = std::pow(C96, C93);
    const double C98 = C97 * C92;
    const double C99 = C98 + 1e-16;

    result.vsigmaAa =
        -4.2357609144641219 * C97 * rhoA /
        (0.25728e2 * Pi *
         std::pow(0.2195149727645171 * std::pow(std::sqrt(4. * sigmaAa) / (2. * C99), 2) / 0.804 +
                      1.,
                  2) *
         std::pow(C99, 2));

    result.vsigmaAb = 0;

    const double C102 = 2. * rhoB;
    const double C103 = 1. / 3.;
    const double C104 = std::pow(Pi, 2);
    const double C105 = C104 * C102;
    const double C106 = 3. * C105;
    const double C107 = std::pow(C106, C103);
    const double C108 = C107 * C102;
    const double C109 = C108 + 1e-16;

    result.vsigmaBb =
        -4.2357609144641219 * C107 * rhoB /
        (0.25728e2 * Pi *
         std::pow(0.2195149727645171 * std::pow(std::sqrt(4. * sigmaBb) / (2. * C109), 2) / 0.804 +
                      1.,
                  2) *
         std::pow(C109, 2));

    return result;
}

} // namespace excgrid
