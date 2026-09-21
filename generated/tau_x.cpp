// excgrid codegen source: the tau tier's exchange functional - the second-order
// gradient expansion of exchange in the iso-orbital (Pauli) variables, the
// library's first functional to read the kinetic-energy densities.  Fresh
// authorship; the formula is the published expansion in its iso-orbital form
// (docs/mainpage.md references).  Regenerate with tools/regenerate.py
// (Yacas is a maintainer-only tool; the committed generated/ output is the
// build input).  This is also the tier's worked example of the generator's
// second-derivative emission: the kernel below is emitted together with the
// functional's materialised second-derivative matrix.

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

// The excgrid meta-GGA kernel skeleton (fresh authorship; BSD-3-Clause).
// Emits, per meta-GGA functional, the order-1 kernel - the energy density, the
// two spin-density derivatives, the three sigma derivatives and the two
// kinetic-energy-density derivatives, the contract's reserved slots 5 and 6 -
// and, beside it, the functional's SECOND-DERIVATIVE tier: one function filling
// the contract's materialised matrix.
//
// The second-derivative function's argument list is the kernel's, and the matrix
// it fills is the contract's upper triangle, row-major over the ACTIVE components
// in identifier order.  Which components those are is the caller's mask, so the
// order is pinned by the caller and not here: this skeleton emits the entries in
// the identifier order of the seven collinear inputs it is generated over.
//
// No spin-edge branches: as in the GGA skeleton, the definitions' Tiny guards
// keep every expression finite at the spin and gamma edges, and the density
// prefactor carries the zero-density corner.
// The banner and the kernel.hpp include are emitted by the calling .ey, which
// always precedes this skeleton, so they are not repeated here.

#include <cmath>
#include <limits>

namespace excgrid {

XcKernelValue TauXExchange(double rhoA,
                           double rhoB,
                           double sigmaAa,
                           double sigmaAb,
                           double sigmaBb,
                           double tauA,
                           double tauB) {
    (void)sigmaAb;

    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        return result;
    }

    const double C140 = rhoA + 1e-16;
    const double C141 = rhoB + 1e-16;
    const double C142 = 2. / 3.;
    const double C143 = 4. / 3.;
    const double C144 = 5. / 3.;
    const double C145 = std::pow(Pi, 2);
    const double C146 = 6. * C145;
    const double C147 = std::pow(C146, C142);
    const double C148 = 3. * C147;

    result.exc =
        -(0.9305257363491 * std::pow(rhoB, C143) *
              ((1. - 10. * (tauB - sigmaBb / (8. * C141)) / (std::pow(C141, C144) * C148)) / 12. +
               1.) +
          0.9305257363491 * std::pow(rhoA, C143) *
              ((1. - 10. * (tauA - sigmaAa / (8. * C140)) / (std::pow(C140, C144) * C148)) / 12. +
               1.));

    const double C153 = 8. * C140;
    const double C154 = std::pow(C140, C144);
    const double C155 = C154 * C148;
    const double C156 = sigmaAa / C153;
    const double C157 = tauA - C156;

    result.vrhoA =
        -(0.9305257363491 *
              (((1. - 10. * C157 / C155) / 12. + 1.) * 4. * std::pow(rhoA, 1. / 3.) / 3. -
               std::pow(rhoA, C143) *
                   (C155 * 80. * sigmaAa / std::pow(C153, 2) -
                    10. * C157 * 3. * C147 * 5. * std::pow(C140, C142) / 3.) /
                   (12. * std::pow(C155, 2))) +
          0.);

    const double C159 = 8. * C141;
    const double C160 = std::pow(C141, C144);
    const double C161 = C160 * C148;
    const double C162 = sigmaBb / C159;
    const double C163 = tauB - C162;

    result.vrhoB =
        -(0.9305257363491 *
              (((1. - 10. * C163 / C161) / 12. + 1.) * 4. * std::pow(rhoB, 1. / 3.) / 3. -
               std::pow(rhoB, C143) *
                   (C161 * 80. * sigmaBb / std::pow(C159, 2) -
                    10. * C163 * 3. * C147 * 5. * std::pow(C141, C142) / 3.) /
                   (12. * std::pow(C161, 2))) +
          0.);

    const double C165 = rhoA + 1e-16;

    result.vsigmaAa = -(
        9.3052573634910001 * std::pow(rhoA, 4. / 3.) /
            (96. * C165 * std::pow(C165, 5. / 3.) * 3. * std::pow(6. * std::pow(Pi, 2), 2. / 3.)) +
        0.);

    result.vsigmaAb = 0.;

    const double C168 = rhoB + 1e-16;

    result.vsigmaBb = -(
        9.3052573634910001 * std::pow(rhoB, 4. / 3.) /
            (96. * C168 * std::pow(C168, 5. / 3.) * 3. * std::pow(6. * std::pow(Pi, 2), 2. / 3.)) +
        0.);

    result.vtauA =
        -(-9.3052573634910001 * std::pow(rhoA, C143) / (12. * std::pow(C140, C144) * C148) + 0.);

    result.vtauB =
        -(-9.3052573634910001 * std::pow(rhoB, C143) / (12. * std::pow(C141, C144) * C148) + 0.);

    return result;
}

void TauXExchangeSecondDerivatives(double rhoA,
                                   double rhoB,
                                   double sigmaAa,
                                   double sigmaAb,
                                   double sigmaBb,
                                   double tauA,
                                   double tauB,
                                   PointSecondDerivativeMatrix& matrix) {
    (void)sigmaAb;

    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    if (rhoA + rhoB < 2. * std::numeric_limits<double>::epsilon())
    {
        return;
    }

    matrix.upper[27] = 0;

    matrix.upper[26] = 0;

    matrix.upper[25] = 0;

    matrix.upper[24] = 0;

    matrix.upper[23] = 0;

    matrix.upper[22] = 0;

    matrix.upper[21] = 0;

    matrix.upper[20] = 0;

    matrix.upper[19] = 0;

    matrix.upper[18] = 0;

    matrix.upper[17] = 0;

    matrix.upper[16] = 0;

    matrix.upper[15] = 0;

    matrix.upper[14] = 0;

    matrix.upper[13] = 0;

    const double C131 = rhoB + 1e-16;
    const double C132 = 2. / 3.;
    const double C133 = 5. / 3.;
    const double C134 = std::pow(Pi, 2);
    const double C135 = 6. * C134;
    const double C136 = std::pow(C131, C133);
    const double C137 = std::pow(C135, C132);
    const double C138 = 3. * C137;
    const double C139 = C136 * C138;
    matrix.upper[12] = -0.9305257363491 *
                       (-40. * std::pow(rhoB, 1. / 3.) / (36. * C139) -
                        (-std::pow(rhoB, 4. / 3.) * 30. * C137 * 5. * std::pow(C131, C132) / 3.) /
                            (12. * std::pow(C139, 2)));

    matrix.upper[11] = 0.;

    const double C122 = rhoB + 1e-16;
    const double C123 = 2. / 3.;
    const double C124 = 5. / 3.;
    const double C125 = std::pow(Pi, 2);
    const double C126 = 6. * C125;
    const double C127 = std::pow(C122, C124);
    const double C128 = std::pow(C126, C123);
    const double C129 = 3. * C128;
    const double C130 = C127 * C129;
    matrix.upper[10] =
        -0.9305257363491 * (40. * std::pow(rhoB, 1. / 3.) / (288. * C122 * C130) -
                            std::pow(rhoB, 4. / 3.) *
                                (80. * C130 / std::pow(8. * C122, 2) -
                                 -30. * C128 * 5. * std::pow(C122, C123) / (24. * C122)) /
                                (12. * std::pow(C130, 2)));

    matrix.upper[9] = 0.;

    matrix.upper[8] = 0.;

    const double C92 = 80. * sigmaBb;
    const double C93 = rhoB + 1e-16;
    const double C94 = 1. / 3.;
    const double C95 = 2. / 3.;
    const double C96 = 4. / 3.;
    const double C97 = 5. / 3.;
    const double C98 = std::pow(Pi, 2);
    const double C99 = 6. * C98;
    const double C100 = 8. * C93;
    const double C101 = std::pow(C93, C95);
    const double C102 = std::pow(C93, C97);
    const double C103 = std::pow(rhoB, C94);
    const double C104 = std::pow(rhoB, C96);
    const double C105 = 5. * C101;
    const double C106 = sigmaBb / C100;
    const double C107 = std::pow(C100, 2);
    const double C108 = std::pow(C99, C95);
    const double C109 = 3. * C108;
    const double C110 = C108 * C105;
    const double C111 = tauB - C106;
    const double C112 = 3. * C110;
    const double C113 = C102 * C109;
    const double C114 = C111 * C112;
    const double C115 = C113 * C92;
    const double C116 = std::pow(C113, 2);
    const double C117 = 10. * C114;
    const double C118 = 12. * C116;
    const double C119 = C115 / C107;
    const double C120 = C117 / 3.;
    const double C121 = C119 - C120;
    matrix.upper[7] =
        -0.9305257363491 *
        ((((1. - 10. * C111 / C113) / 12. + 1.) * 4. * std::pow(rhoB, -2. / 3.) / 3. +
          -4. * C103 * C121 / C118) /
             3. -
         (12. * C116 *
              (C104 * ((C107 * 80. * sigmaBb * C112 / 3. - C115 * 128. * C93) / std::pow(C100, 4) -
                       10. *
                           (C111 * 3. * C108 * 10. * std::pow(C93, -1. / 3.) / 3. +
                            3. * C110 * 8. * sigmaBb / C107) /
                           3.) +
               C121 * 4. * C103 / 3.) -
          C104 * C121 * 12. * C113 * 6. * C110 / 3.) /
             std::pow(C118, 2));

    matrix.upper[6] = 0.;

    const double C83 = rhoA + 1e-16;
    const double C84 = 2. / 3.;
    const double C85 = 5. / 3.;
    const double C86 = std::pow(Pi, 2);
    const double C87 = 6. * C86;
    const double C88 = std::pow(C83, C85);
    const double C89 = std::pow(C87, C84);
    const double C90 = 3. * C89;
    const double C91 = C88 * C90;
    matrix.upper[5] =
        -0.9305257363491 * (-40. * std::pow(rhoA, 1. / 3.) / (36. * C91) -
                            (-std::pow(rhoA, 4. / 3.) * 30. * C89 * 5. * std::pow(C83, C84) / 3.) /
                                (12. * std::pow(C91, 2)));

    matrix.upper[4] = 0.;

    matrix.upper[3] = 0.;

    const double C74 = rhoA + 1e-16;
    const double C75 = 2. / 3.;
    const double C76 = 5. / 3.;
    const double C77 = std::pow(Pi, 2);
    const double C78 = 6. * C77;
    const double C79 = std::pow(C74, C76);
    const double C80 = std::pow(C78, C75);
    const double C81 = 3. * C80;
    const double C82 = C79 * C81;
    matrix.upper[2] = -0.9305257363491 * (40. * std::pow(rhoA, 1. / 3.) / (288. * C74 * C82) -
                                          std::pow(rhoA, 4. / 3.) *
                                              (80. * C82 / std::pow(8. * C74, 2) -
                                               -30. * C80 * 5. * std::pow(C74, C75) / (24. * C74)) /
                                              (12. * std::pow(C82, 2)));

    matrix.upper[1] = 0.;

    const double C44 = 80. * sigmaAa;
    const double C45 = rhoA + 1e-16;
    const double C46 = 1. / 3.;
    const double C47 = 2. / 3.;
    const double C48 = 4. / 3.;
    const double C49 = 5. / 3.;
    const double C50 = std::pow(Pi, 2);
    const double C51 = 6. * C50;
    const double C52 = 8. * C45;
    const double C53 = std::pow(C45, C47);
    const double C54 = std::pow(C45, C49);
    const double C55 = std::pow(rhoA, C46);
    const double C56 = std::pow(rhoA, C48);
    const double C57 = 5. * C53;
    const double C58 = sigmaAa / C52;
    const double C59 = std::pow(C51, C47);
    const double C60 = std::pow(C52, 2);
    const double C61 = 3. * C59;
    const double C62 = C59 * C57;
    const double C63 = tauA - C58;
    const double C64 = 3. * C62;
    const double C65 = C54 * C61;
    const double C66 = C63 * C64;
    const double C67 = C65 * C44;
    const double C68 = std::pow(C65, 2);
    const double C69 = 10. * C66;
    const double C70 = 12. * C68;
    const double C71 = C67 / C60;
    const double C72 = C69 / 3.;
    const double C73 = C71 - C72;
    matrix.upper[0] =
        -0.9305257363491 *
        ((((1. - 10. * C63 / C65) / 12. + 1.) * 4. * std::pow(rhoA, -2. / 3.) / 3. +
          -4. * C55 * C73 / C70) /
             3. -
         (12. * C68 *
              (C56 * ((C60 * 80. * sigmaAa * C64 / 3. - C67 * 128. * C45) / std::pow(C52, 4) -
                      10. *
                          (C63 * 3. * C59 * 10. * std::pow(C45, -1. / 3.) / 3. +
                           3. * C62 * 8. * sigmaAa / C60) /
                          3.) +
               C73 * 4. * C55 / 3.) -
          C56 * C73 * 12. * C65 * 6. * C62 / 3.) /
             std::pow(C70, 2));
}

} // namespace excgrid
