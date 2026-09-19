// excgrid codegen source: the Becke 1988 GGA exchange functional.
// Fresh authorship; formula from the B88 paper (docs/mainpage.md
// references).  Regenerate with tools/regenerate.py.

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

XcKernelValue Becke88Exchange(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
    (void)sigmaAb;

    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        return result;
    }

    const double C46 = 4. * Pi;
    const double C47 = rhoA + 1e-16;
    const double C48 = rhoB + 1e-16;
    const double C49 = 1. / 3.;
    const double C50 = 4. / 3.;
    const double C51 = std::sqrt(sigmaAa);
    const double C52 = std::sqrt(sigmaBb);
    const double C53 = 3. / C46;
    const double C54 = std::pow(C47, C50);
    const double C55 = std::pow(C48, C50);
    const double C56 = C51 / C54;
    const double C57 = C52 / C55;
    const double C58 = std::pow(C53, C49);
    const double C59 = 3. * C58;
    const double C60 = std::pow(C56, 2);
    const double C61 = std::pow(C57, 2);
    const double C62 = C59 / 2.;

    result.exc = -(
        std::pow(rhoB, C50) *
            (C62 + 0.0042 * C61 / (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * C52 / C55 + 1.)) +
        std::pow(rhoA, C50) *
            (C62 + 0.0042 * C60 / (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * C51 / C54 + 1.)));

    const double C67 = 0.0252 * C51;
    const double C68 = C60 + 1.;
    const double C69 = 8. / 3.;
    const double C70 = std::pow(C47, C49);
    const double C71 = 4. * C70;
    const double C72 = C70 * sigmaAa;
    const double C73 = std::sqrt(C68);
    const double C74 = std::pow(C47, C69);
    const double C75 = 3. * C74;
    const double C76 = C56 + C73;
    const double C77 = C54 * C75;
    const double C78 = std::log(C76);
    const double C79 = C78 * C67;
    const double C80 = C79 / C54;
    const double C81 = C80 + 1.;

    result.vrhoA =
        -(std::pow(rhoA, C50) *
              (C81 * -0.0336 * C72 / C77 -
               0.0042 * C60 *
                   (C54 * 0.0252 * C51 * (-8. * C72 / (C77 * 2 * C73) - C51 * C71 / C75) / C76 -
                    C79 * C71 / 3.) /
                   C74) /
              std::pow(C81, 2) +
          (C62 + 0.0042 * C60 / C81) * 4. * std::pow(rhoA, C49) / 3.);

    const double C83 = 0.0252 * C52;
    const double C84 = C61 + 1.;
    const double C85 = 8. / 3.;
    const double C86 = std::pow(C48, C49);
    const double C87 = 4. * C86;
    const double C88 = C86 * sigmaBb;
    const double C89 = std::sqrt(C84);
    const double C90 = std::pow(C48, C85);
    const double C91 = 3. * C90;
    const double C92 = C57 + C89;
    const double C93 = C55 * C91;
    const double C94 = std::log(C92);
    const double C95 = C94 * C83;
    const double C96 = C95 / C55;
    const double C97 = C96 + 1.;

    result.vrhoB =
        -(std::pow(rhoB, C50) *
              (C97 * -0.0336 * C88 / C93 -
               0.0042 * C61 *
                   (C55 * 0.0252 * C52 * (-8. * C88 / (C93 * 2 * C89) - C52 * C87 / C91) / C92 -
                    C95 * C87 / 3.) /
                   C90) /
              std::pow(C97, 2) +
          (C62 + 0.0042 * C61 / C97) * 4. * std::pow(rhoB, C49) / 3.);

    const double C99 = rhoA + 1e-16;
    const double C100 = 4. / 3.;
    const double C101 = 8. / 3.;
    const double C102 = std::sqrt(sigmaAa);
    const double C103 = 0.0252 * C102;
    const double C104 = std::pow(C99, C100);
    const double C105 = std::pow(C99, C101);
    const double C106 = C102 * C105;
    const double C107 = C102 / C104;
    const double C108 = std::pow(C107, 2);
    const double C109 = C108 + 1.;
    const double C110 = std::sqrt(C109);
    const double C111 = C107 + C110;
    const double C112 = std::log(C111);
    const double C113 = C112 * C103;
    const double C114 = C113 / C104;
    const double C115 = C114 + 1.;

    result.vsigmaAa =
        -std::pow(rhoA, C100) *
        (C115 * 0.0042 * C102 / C106 -
         0.0042 * C108 *
             (0.0252 * C112 / (2 * C102) +
              0.0252 * C102 * (1 / (2 * C102 * C104) + C102 / (C106 * 2 * C110)) / C111) /
             C104) /
        std::pow(C115, 2);

    result.vsigmaAb = 0;

    const double C118 = rhoB + 1e-16;
    const double C119 = 4. / 3.;
    const double C120 = 8. / 3.;
    const double C121 = std::sqrt(sigmaBb);
    const double C122 = 0.0252 * C121;
    const double C123 = std::pow(C118, C119);
    const double C124 = std::pow(C118, C120);
    const double C125 = C121 * C124;
    const double C126 = C121 / C123;
    const double C127 = std::pow(C126, 2);
    const double C128 = C127 + 1.;
    const double C129 = std::sqrt(C128);
    const double C130 = C126 + C129;
    const double C131 = std::log(C130);
    const double C132 = C131 * C122;
    const double C133 = C132 / C123;
    const double C134 = C133 + 1.;

    result.vsigmaBb =
        -std::pow(rhoB, C119) *
        (C134 * 0.0042 * C121 / C125 -
         0.0042 * C127 *
             (0.0252 * C131 / (2 * C121) +
              0.0252 * C121 * (1 / (2 * C121 * C123) + C121 / (C125 * 2 * C129)) / C130) /
             C123) /
        std::pow(C134, 2);

    return result;
}

} // namespace excgrid
