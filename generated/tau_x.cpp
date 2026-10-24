// excgrid codegen source: the tau tier's exchange functional - the second-order
// gradient expansion of exchange in the iso-orbital (Pauli) variables.
// Formula from the published expansion in its iso-orbital form.

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

// The excgrid meta-GGA kernel skeleton (fresh authorship; BSD-3-Clause).  Emits the
// order-1 kernel and the second-derivative tier, the contract's upper triangle over the
// caller's mask; the Tiny guards keep every expression finite, so no edge branches.

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

    const double C59 = rhoA + 1e-16;
    const double C60 = rhoB + 1e-16;
    const double C61 = 2. / 3.;
    const double C62 = 4. / 3.;
    const double C63 = 5. / 3.;
    const double C64 = std::pow(Pi, 2);
    const double C65 = 6. * C64;
    const double C66 = std::pow(C65, C61);
    const double C67 = 3. * C66;

    result.exc =
        -(0.9305257363491 * std::pow(rhoB, C62) *
              ((1. - 10. * (tauB - sigmaBb / (8. * C60)) / (std::pow(C60, C63) * C67)) / 12. + 1.) +
          0.9305257363491 * std::pow(rhoA, C62) *
              ((1. - 10. * (tauA - sigmaAa / (8. * C59)) / (std::pow(C59, C63) * C67)) / 12. + 1.));

    const double C69 = 1;

    result.vrhoA = -(
        (-0.9305257363491 * std::pow(rhoA, C62) *
             (std::pow(C59, C63) * C67 * 80. * sigmaAa / std::pow(8. * C59, 2) -
              10. * (tauA - sigmaAa / (8. * C59)) * C63 * std::pow(C59, C63 - 1) * C67) /
             (12. * std::pow(std::pow(C59, C63) * C67, 2)) +
         0.) *
            C69 +
        (0.9305257363491 * C62 * std::pow(rhoA, C62 - 1) *
             ((1. - 10. * (tauA - sigmaAa / (8. * C59)) / (std::pow(C59, C63) * C67)) / 12. + 1.) +
         0.));

    const double C76 = 1;

    result.vrhoB = -(
        (-0.9305257363491 * std::pow(rhoB, C62) *
             (std::pow(C60, C63) * C67 * 80. * sigmaBb / std::pow(8. * C60, 2) -
              10. * (tauB - sigmaBb / (8. * C60)) * C63 * std::pow(C60, C63 - 1) * C67) /
             (12. * std::pow(std::pow(C60, C63) * C67, 2)) +
         0.) *
            C76 +
        (0.9305257363491 * C62 * std::pow(rhoB, C62 - 1) *
             ((1. - 10. * (tauB - sigmaBb / (8. * C60)) / (std::pow(C60, C63) * C67)) / 12. + 1.) +
         0.));

    result.vsigmaAa =
        -(9.3052573634910001 * std::pow(rhoA, C62) / (96. * C59 * std::pow(C59, C63) * C67) + 0.);

    result.vsigmaAb = 0.;

    result.vsigmaBb =
        -(9.3052573634910001 * std::pow(rhoB, C62) / (96. * C60 * std::pow(C60, C63) * C67) + 0.);

    result.vtauA =
        -(-9.3052573634910001 * std::pow(rhoA, C62) / (12. * std::pow(C59, C63) * C67) + 0.);

    result.vtauB =
        -(-9.3052573634910001 * std::pow(rhoB, C62) / (12. * std::pow(C60, C63) * C67) + 0.);

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

    const double C44 = rhoA + 1e-16;
    const double C46 = 2. / 3.;
    const double C47 = 4. / 3.;
    const double C48 = 5. / 3.;
    const double C49 = std::pow(Pi, 2);
    const double C50 = 6. * C49;
    const double C51 = std::pow(C50, C46);
    const double C52 = 3. * C51;
    const double C57 = 1;
    matrix.upper[0] = -(
        (C57 *
             (12. * std::pow(std::pow(C44, C48) * C52, 2) * -0.9305257363491 * std::pow(rhoA, C47) *
                  ((std::pow(8. * C44, 2) * C48 * std::pow(C44, C48 - 1) * C52 * 80. * sigmaAa -
                    std::pow(C44, C48) * C52 * 80. * sigmaAa * 128. * C44) /
                       std::pow(8. * C44, 4) -
                   10. * ((tauA - sigmaAa / (8. * C44)) * C48 * (C48 - 1) * std::pow(C44, C48 - 2) *
                              C52 +
                          C48 * std::pow(C44, C48 - 1) * C52 * 8. * sigmaAa /
                              std::pow(8. * C44, 2))) -
              -0.9305257363491 * std::pow(rhoA, C47) *
                  (std::pow(C44, C48) * C52 * 80. * sigmaAa / std::pow(8. * C44, 2) -
                   10. * (tauA - sigmaAa / (8. * C44)) * C48 * std::pow(C44, C48 - 1) * C52) *
                  24. * C48 * std::pow(C44, C48 - 1) * C52 * std::pow(C44, C48) * C52) /
             std::pow(12. * std::pow(std::pow(C44, C48) * C52, 2), 2) +
         -0.9305257363491 * C47 * std::pow(rhoA, C47 - 1) *
             (std::pow(C44, C48) * C52 * 80. * sigmaAa / std::pow(8. * C44, 2) -
              10. * (tauA - sigmaAa / (8. * C44)) * C48 * std::pow(C44, C48 - 1) * C52) /
             (12. * std::pow(std::pow(C44, C48) * C52, 2))) *
            C57 +
        (C57 * -0.9305257363491 * C47 * std::pow(rhoA, C47 - 1) *
             (std::pow(C44, C48) * C52 * 80. * sigmaAa / std::pow(8. * C44, 2) -
              10. * (tauA - sigmaAa / (8. * C44)) * C48 * std::pow(C44, C48 - 1) * C52) /
             (12. * std::pow(std::pow(C44, C48) * C52, 2)) +
         0.9305257363491 * C47 * (C47 - 1) * std::pow(rhoA, C47 - 2) *
             ((1. - 10. * (tauA - sigmaAa / (8. * C44)) / (std::pow(C44, C48) * C52)) / 12. + 1.)));

    matrix.upper[1] = 0.;

    matrix.upper[2] = -(C57 * -0.9305257363491 * std::pow(rhoA, C47) *
                            (80. * std::pow(C44, C48) * C52 / std::pow(8. * C44, 2) -
                             -10. * C48 * std::pow(C44, C48 - 1) * C52 / (8. * C44)) /
                            (12. * std::pow(std::pow(C44, C48) * C52, 2)) +
                        9.3052573634910001 * C47 * std::pow(rhoA, C47 - 1) /
                            (96. * C44 * std::pow(C44, C48) * C52));

    matrix.upper[3] = 0.;

    matrix.upper[4] = 0.;

    matrix.upper[5] =
        -(C57 * 0.9305257363491 * std::pow(rhoA, C47) * 10. * C48 * std::pow(C44, C48 - 1) * C52 /
              (12. * std::pow(std::pow(C44, C48) * C52, 2)) +
          -9.3052573634910001 * C47 * std::pow(rhoA, C47 - 1) / (12. * std::pow(C44, C48) * C52));

    matrix.upper[6] = 0.;

    const double C45 = rhoB + 1e-16;
    const double C58 = 1;
    matrix.upper[7] = -(
        (C58 *
             (12. * std::pow(std::pow(C45, C48) * C52, 2) * -0.9305257363491 * std::pow(rhoB, C47) *
                  ((std::pow(8. * C45, 2) * C48 * std::pow(C45, C48 - 1) * C52 * 80. * sigmaBb -
                    std::pow(C45, C48) * C52 * 80. * sigmaBb * 128. * C45) /
                       std::pow(8. * C45, 4) -
                   10. * ((tauB - sigmaBb / (8. * C45)) * C48 * (C48 - 1) * std::pow(C45, C48 - 2) *
                              C52 +
                          C48 * std::pow(C45, C48 - 1) * C52 * 8. * sigmaBb /
                              std::pow(8. * C45, 2))) -
              -0.9305257363491 * std::pow(rhoB, C47) *
                  (std::pow(C45, C48) * C52 * 80. * sigmaBb / std::pow(8. * C45, 2) -
                   10. * (tauB - sigmaBb / (8. * C45)) * C48 * std::pow(C45, C48 - 1) * C52) *
                  24. * C48 * std::pow(C45, C48 - 1) * C52 * std::pow(C45, C48) * C52) /
             std::pow(12. * std::pow(std::pow(C45, C48) * C52, 2), 2) +
         -0.9305257363491 * C47 * std::pow(rhoB, C47 - 1) *
             (std::pow(C45, C48) * C52 * 80. * sigmaBb / std::pow(8. * C45, 2) -
              10. * (tauB - sigmaBb / (8. * C45)) * C48 * std::pow(C45, C48 - 1) * C52) /
             (12. * std::pow(std::pow(C45, C48) * C52, 2))) *
            C58 +
        (C58 * -0.9305257363491 * C47 * std::pow(rhoB, C47 - 1) *
             (std::pow(C45, C48) * C52 * 80. * sigmaBb / std::pow(8. * C45, 2) -
              10. * (tauB - sigmaBb / (8. * C45)) * C48 * std::pow(C45, C48 - 1) * C52) /
             (12. * std::pow(std::pow(C45, C48) * C52, 2)) +
         0.9305257363491 * C47 * (C47 - 1) * std::pow(rhoB, C47 - 2) *
             ((1. - 10. * (tauB - sigmaBb / (8. * C45)) / (std::pow(C45, C48) * C52)) / 12. + 1.)));

    matrix.upper[8] = 0.;

    matrix.upper[9] = 0.;

    matrix.upper[10] = -(C58 * -0.9305257363491 * std::pow(rhoB, C47) *
                             (80. * std::pow(C45, C48) * C52 / std::pow(8. * C45, 2) -
                              -10. * C48 * std::pow(C45, C48 - 1) * C52 / (8. * C45)) /
                             (12. * std::pow(std::pow(C45, C48) * C52, 2)) +
                         9.3052573634910001 * C47 * std::pow(rhoB, C47 - 1) /
                             (96. * C45 * std::pow(C45, C48) * C52));

    matrix.upper[11] = 0.;

    matrix.upper[12] =
        -(C58 * 0.9305257363491 * std::pow(rhoB, C47) * 10. * C48 * std::pow(C45, C48 - 1) * C52 /
              (12. * std::pow(std::pow(C45, C48) * C52, 2)) +
          -9.3052573634910001 * C47 * std::pow(rhoB, C47 - 1) / (12. * std::pow(C45, C48) * C52));

    matrix.upper[13] = 0;

    matrix.upper[14] = 0;

    matrix.upper[15] = 0;

    matrix.upper[16] = 0;

    matrix.upper[17] = 0;

    matrix.upper[18] = 0;

    matrix.upper[19] = 0;

    matrix.upper[20] = 0;

    matrix.upper[21] = 0;

    matrix.upper[22] = 0;

    matrix.upper[23] = 0;

    matrix.upper[24] = 0;

    matrix.upper[25] = 0;

    matrix.upper[26] = 0;

    matrix.upper[27] = 0;
}

} // namespace excgrid
