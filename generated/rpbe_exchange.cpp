// excgrid codegen source: the Hammer-Hansen-Norskov 1999 revised PBE GGA
// exchange functional (RPBE).  Fresh authorship.  The enhancement factor
// Fx(s) = 1 + kappa (1 - exp(-mu s^2 / kappa)) has kappa = 0.804 and
// mu = 0.2195149727645171 - the FULL-PRECISION value, not the 0.21951 the
// paper prints.  Owner ruling 2026-09-13: the truncation was a transcription
// precision choice, not a distinct variant of RPBE, libxc's kernel is
// algebraically the same expression with mu = MU_PBE, and this tree's own
// pbe and revpbe rules already carry that value - so matching it is internal
// consistency, not adopting another code's definition.  The form itself is
// still the published enhancement factor.  Verified against the oracle by
// excgrid/tools/verify_pyscf.py.  Regenerate with tools/regenerate.py.

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

// The excgrid GGA kernel skeleton (fresh authorship; BSD-3-Clause).
// Emits one excgrid::XcKernelValue function per GGA functional: the energy
// density, the spin-density derivatives, and the three sigma derivatives.
// No spin-edge branches: the definitions' Tiny guards keep every expression
// finite at the spin and gamma edges (the LDA skeleton's exact-limit
// machinery is not needed for the gradient-corrected forms, whose edge
// contributions vanish with the spin density).

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

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
    const double C95 = 4. * sigmaAa;
    const double C96 = 1. / 3.;
    const double C97 = std::pow(Pi, 2);
    const double C98 = C97 * C94;
    const double C99 = std::sqrt(C95);
    const double C100 = 3. * C98;
    const double C101 = std::pow(C100, C96);
    const double C102 = C101 * C94;
    const double C103 = C102 + 1e-16;
    const double C104 = 2. * C103;

    result.vsigmaAa = -rhoA * 3. * C101 * 0.804 *
                      std::exp(-0.2195149727645171 * std::pow(C99 / C104, 2) / 0.804) *
                      1.7561197821161368 * C99 / (1.608 * C103 * 2 * C99 * C104 * 4. * Pi);

    result.vsigmaAb = 0;

    const double C107 = 2. * rhoB;
    const double C108 = 4. * sigmaBb;
    const double C109 = 1. / 3.;
    const double C110 = std::pow(Pi, 2);
    const double C111 = C110 * C107;
    const double C112 = std::sqrt(C108);
    const double C113 = 3. * C111;
    const double C114 = std::pow(C113, C109);
    const double C115 = C114 * C107;
    const double C116 = C115 + 1e-16;
    const double C117 = 2. * C116;

    result.vsigmaBb = -rhoB * 3. * C114 * 0.804 *
                      std::exp(-0.2195149727645171 * std::pow(C112 / C117, 2) / 0.804) *
                      1.7561197821161368 * C112 / (1.608 * C116 * 2 * C112 * C117 * 4. * Pi);

    return result;
}

} // namespace excgrid
