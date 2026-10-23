// excgrid codegen source: the revPBE GGA exchange functional (the PBE
// exchange form with kappa = 1.245).  Fresh authorship; formula from the
// revPBE paper (docs/mainpage.md references).  Regenerate with
// tools/regenerate.py.

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

// The excgrid GGA kernel skeleton (fresh authorship; BSD-3-Clause).
// Emits one excgrid::XcKernelValue function per GGA functional: the energy
// density, the spin-density derivatives, and the three sigma derivatives.
// It also emits the functional's SECOND-DERIVATIVE tier: one function filling
// the contract's materialised matrix, from the same expression as the kernel.
// The matrix is the contract's upper triangle, row-major over the five
// components the kernel takes, in identifier order - the GGA tier's whole
// span.  Which of them a caller's matrix is over is the caller's mask, and the
// registry re-packs the two; the kinetic-energy-density rows and columns are
// not emitted at all, because a GGA energy density does not read tau.
// No spin-edge branches: the definitions' Tiny guards keep every expression
// finite at the spin and gamma edges (the LDA skeleton's exact-limit
// machinery is not needed for the gradient-corrected forms, whose edge
// contributions vanish with the spin density).
// The banner and the kernel.hpp include are emitted by the calling .ey, which
// always precedes this skeleton, so they are not repeated here.

#include <cmath>
#include <limits>

namespace excgrid {

XcKernelValue RevPbeExchange(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
    (void)sigmaAb;

    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;
    // The sigma-edge guard the generator adds to a vanishing gradient radical
    // (ExGuardSigmaRadical in excgrid_generate.ys carries the measurement).
    // Its value has to be written HERE rather than injected from the
    // definitions: the printer renders a number below 1e-16 as "0.", so a
    // constant small enough to be absorbed by every normal sigma cannot reach
    // the expression through yacas at all.  1e-300 is absorbed by every sigma
    // above ~1e-284 and leaves the sum at exactly sigma, which is what keeps
    // the guarded arithmetic bit-identical to the unguarded one.
    [[maybe_unused]] constexpr double SigmaGuard = 1e-300;

    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        return result;
    }

    const double C70 = 2. * rhoA;
    const double C71 = 2. * rhoB;
    const double C72 = 1. / 3.;
    const double C73 = std::pow(Pi, 2);
    const double C74 = C73 * C70;
    const double C75 = C73 * C71;
    const double C76 = 3. * C74;
    const double C77 = 3. * C75;
    const double C78 = std::pow(C76, C72);
    const double C79 = std::pow(C77, C72);

    result.exc =
        -((2.245 -
           1.245 / (0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaBb) / (2. * (C79 * C71 + 1e-16)), 2) / 1.245 +
                    1.)) *
              rhoB * 3. * C79 +
          (2.245 -
           1.245 / (0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaAa) / (2. * (C78 * C70 + 1e-16)), 2) / 1.245 +
                    1.)) *
              rhoA * 3. * C78) /
        (4. * Pi);

    const double C81 = 2.;
    const double C82 = C73 * C81;
    const double C83 = 3. * C82;
    const double C84 = C72 * std::pow(C76, C72 - 1) * C83;

    result.vrhoA = -(
        C81 * rhoA * 3. * C78 * -1.0931845643672951 * C78 * 4. * sigmaAa /
            (2.49 * (C78 * C70 + 1e-16) * std::pow(2. * (C78 * C70 + 1e-16), 2) *
             std::pow(0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaAa) / (2. * (C78 * C70 + 1e-16)), 2) /
                              1.245 +
                          1.,
                      2) *
             4. * Pi) +
        (C84 * ((2.245 -
                 1.245 / (0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaAa) / (2. * (C78 * C70 + 1e-16)), 2) /
                              1.245 +
                          1.)) *
                    3. * rhoA +
                rhoA * 3. * C78 * -1.0931845643672951 * C70 * 4. * sigmaAa /
                    (2.49 * (C78 * C70 + 1e-16) * std::pow(2. * (C78 * C70 + 1e-16), 2) *
                     std::pow(
                         0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C78 * C70 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2))) +
         (2.245 -
          1.245 / (0.2195149727645171 *
                       std::pow(std::sqrt(4. * sigmaAa) / (2. * (C78 * C70 + 1e-16)), 2) / 1.245 +
                   1.)) *
             3. * C78) /
            (4. * Pi));

    const double C85 = 2.;
    const double C86 = C73 * C85;
    const double C87 = 3. * C86;
    const double C88 = C72 * std::pow(C77, C72 - 1) * C87;

    result.vrhoB = -(
        C85 * rhoB * 3. * C79 * -1.0931845643672951 * C79 * 4. * sigmaBb /
            (2.49 * (C79 * C71 + 1e-16) * std::pow(2. * (C79 * C71 + 1e-16), 2) *
             std::pow(0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaBb) / (2. * (C79 * C71 + 1e-16)), 2) /
                              1.245 +
                          1.,
                      2) *
             4. * Pi) +
        (C88 * ((2.245 -
                 1.245 / (0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaBb) / (2. * (C79 * C71 + 1e-16)), 2) /
                              1.245 +
                          1.)) *
                    3. * rhoB +
                rhoB * 3. * C79 * -1.0931845643672951 * C71 * 4. * sigmaBb /
                    (2.49 * (C79 * C71 + 1e-16) * std::pow(2. * (C79 * C71 + 1e-16), 2) *
                     std::pow(
                         0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C79 * C71 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2))) +
         (2.245 -
          1.245 / (0.2195149727645171 *
                       std::pow(std::sqrt(4. * sigmaBb) / (2. * (C79 * C71 + 1e-16)), 2) / 1.245 +
                   1.)) *
             3. * C79) /
            (4. * Pi));

    result.vsigmaAa = -2.1863691287345903 * C78 * 3. * rhoA /
                      (Pi * 4. *
                       std::pow(0.2195149727645171 *
                                        std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) /
                                                     (2. * (C78 * C70 + 1e-16)),
                                                 2) /
                                        1.245 +
                                    1.,
                                2) *
                       (C78 * C70 + 1e-16) * 2. * 2 * (C78 * C70 + 1e-16) * 2.49);

    result.vsigmaAb = 0;

    result.vsigmaBb = -2.1863691287345903 * C79 * 3. * rhoB /
                      (Pi * 4. *
                       std::pow(0.2195149727645171 *
                                        std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) /
                                                     (2. * (C79 * C71 + 1e-16)),
                                                 2) /
                                        1.245 +
                                    1.,
                                2) *
                       (C79 * C71 + 1e-16) * 2. * 2 * (C79 * C71 + 1e-16) * 2.49);

    return result;
}

void RevPbeExchangeSecondDerivatives(double rhoA,
                                     double rhoB,
                                     double sigmaAa,
                                     double sigmaAb,
                                     double sigmaBb,
                                     PointSecondDerivativeMatrix& matrix) {
    (void)sigmaAb;

    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    if (rhoA + rhoB < 2. * std::numeric_limits<double>::epsilon())
    {
        return;
    }

    const double C46 = 2. * rhoA;
    const double C48 = 1. / 3.;
    const double C49 = std::pow(Pi, 2);
    const double C50 = C49 * C46;
    const double C52 = 3. * C50;
    const double C54 = std::pow(C52, C48);
    const double C60 = 2.;
    const double C61 = C49 * C60;
    const double C62 = 3. * C61;
    const double C63 = C48 * std::pow(C52, C48 - 1) * C62;
    const double C68 = C48 * (C48 - 1) * std::pow(C52, C48 - 2) * std::pow(C62, 2);
    matrix.upper[0] =
        ((C60 * rhoA * 3. * C54 * -1.0931845643672951 * C54 * 4. * sigmaAa * 2.49 *
              ((C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                   (0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) / 1.245 +
                    1.) *
                   -1.7561197821161368 * C54 * 4. * sigmaAa /
                   (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2)) +
               ((C54 * C46 + 1e-16) * 4. * C54 * 2. * (C54 * C46 + 1e-16) +
                C54 * std::pow(2. * (C54 * C46 + 1e-16), 2)) *
                   std::pow(
                       0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                               1.245 +
                           1.,
                       2)) *
              4. * Pi /
              std::pow(2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                           std::pow(0.2195149727645171 *
                                            std::pow(std::sqrt(4. * sigmaAa) /
                                                         (2. * (C54 * C46 + 1e-16)),
                                                     2) /
                                            1.245 +
                                        1.,
                                    2) *
                           4. * Pi,
                       2) -
          (C63 *
               (3. * rhoA * -1.0931845643672951 * C54 * 4. * sigmaAa /
                    (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                     std::pow(
                         0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2)) +
                (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                     std::pow(
                         0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2) *
                     rhoA * 3. * C54 * -4.3727382574691806 * sigmaAa -
                 rhoA * 3. * C54 * -1.0931845643672951 * C46 * 4. * sigmaAa * 2.49 *
                     ((C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                          (0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                               1.245 +
                           1.) *
                          -1.7561197821161368 * C54 * 4. * sigmaAa /
                          (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2)) +
                      ((C54 * C46 + 1e-16) * 4. * C54 * 2. * (C54 * C46 + 1e-16) +
                       C54 * std::pow(2. * (C54 * C46 + 1e-16), 2)) *
                          std::pow(
                              0.2195149727645171 *
                                      std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)),
                                               2) /
                                      1.245 +
                                  1.,
                              2))) /
                    std::pow(2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                                 std::pow(0.2195149727645171 *
                                                  std::pow(std::sqrt(4. * sigmaAa) /
                                                               (2. * (C54 * C46 + 1e-16)),
                                                           2) /
                                                  1.245 +
                                              1.,
                                          2),
                             2)) +
           3. * C54 * -1.0931845643672951 * C54 * 4. * sigmaAa /
               (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                std::pow(0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2))) /
              (4. * Pi)) *
             C60 -
         (((2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                std::pow(0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2) *
                4. * Pi * C60 *
                (rhoA * 3. * C54 * -4.3727382574691806 * sigmaAa +
                 3. * rhoA * -1.0931845643672951 * C54 * 4. * sigmaAa) -
            C60 * rhoA * 3. * C54 * -1.0931845643672951 * C54 * 4. * sigmaAa * 2.49 *
                ((C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                     (0.2195149727645171 *
                          std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                          1.245 +
                      1.) *
                     -1.7561197821161368 * C46 * 4. * sigmaAa /
                     (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2)) +
                 ((C54 * C46 + 1e-16) * 4. * C46 * 2. * (C54 * C46 + 1e-16) +
                  C46 * std::pow(2. * (C54 * C46 + 1e-16), 2)) *
                     std::pow(
                         0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2)) *
                4. * Pi) /
               std::pow(2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                            std::pow(0.2195149727645171 *
                                             std::pow(std::sqrt(4. * sigmaAa) /
                                                          (2. * (C54 * C46 + 1e-16)),
                                                      2) /
                                             1.245 +
                                         1.,
                                     2) *
                            4. * Pi,
                        2) +
           (C63 *
                (3. * rhoA * -1.0931845643672951 * C46 * 4. * sigmaAa /
                     (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                      std::pow(
                          0.2195149727645171 *
                                  std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)),
                                           2) /
                                  1.245 +
                              1.,
                          2)) +
                 (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                      std::pow(
                          0.2195149727645171 *
                                  std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)),
                                           2) /
                                  1.245 +
                              1.,
                          2) *
                      3. * rhoA * -1.0931845643672951 * C46 * 4. * sigmaAa -
                  rhoA * 3. * C54 * -1.0931845643672951 * C46 * 4. * sigmaAa * 2.49 *
                      ((C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                           (0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                1.245 +
                            1.) *
                           -1.7561197821161368 * C46 * 4. * sigmaAa /
                           (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2)) +
                       ((C54 * C46 + 1e-16) * 4. * C46 * 2. * (C54 * C46 + 1e-16) +
                        C46 * std::pow(2. * (C54 * C46 + 1e-16), 2)) *
                           std::pow(0.2195149727645171 *
                                            std::pow(std::sqrt(4. * sigmaAa) /
                                                         (2. * (C54 * C46 + 1e-16)),
                                                     2) /
                                            1.245 +
                                        1.,
                                    2))) /
                     std::pow(2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                                  std::pow(0.2195149727645171 *
                                                   std::pow(std::sqrt(4. * sigmaAa) /
                                                                (2. * (C54 * C46 + 1e-16)),
                                                            2) /
                                                   1.245 +
                                               1.,
                                           2),
                              2)) +
            (3. * (2.245 -
                   1.245 / (0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                1.245 +
                            1.)) +
             3. * C54 * -1.0931845643672951 * C46 * 4. * sigmaAa /
                 (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                  std::pow(
                      0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                              1.245 +
                          1.,
                      2)))) /
               (4. * Pi)) *
              C63 +
          (C60 * 3. * C54 * -1.0931845643672951 * C54 * 4. * sigmaAa /
               (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                std::pow(0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2) *
                4. * Pi) +
           C63 *
               (3. * (2.245 -
                      1.245 /
                          (0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                               1.245 +
                           1.)) +
                3. * C54 * -1.0931845643672951 * C46 * 4. * sigmaAa /
                    (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                     std::pow(
                         0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2))) /
               (4. * Pi)))) -
        C68 *
            ((2.245 -
              1.245 /
                  (0.2195149727645171 *
                       std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) / 1.245 +
                   1.)) *
                 3. * rhoA +
             rhoA * 3. * C54 * -1.0931845643672951 * C46 * 4. * sigmaAa /
                 (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                  std::pow(
                      0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                              1.245 +
                          1.,
                      2))) /
            (4. * Pi);

    matrix.upper[1] = 0;

    matrix.upper[2] =
        C60 * 2.1863691287345903 * C54 * 3. * rhoA * 2.49 *
            (Pi * 4. *
                 std::pow(
                     0.2195149727645171 *
                             std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                             1.245 +
                         1.,
                     2) *
                 (C54 * C46 + 1e-16) * 2. * 2 * C54 +
             4. *
                 (Pi * 4. *
                      std::pow(
                          0.2195149727645171 *
                                  std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)),
                                           2) /
                                  1.245 +
                              1.,
                          2) *
                      C54 +
                  (C54 * C46 + 1e-16) * Pi * 4. *
                      (0.2195149727645171 *
                           std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                           1.245 +
                       1.) *
                      -1.7561197821161368 * C54 * 4. * sigmaAa /
                      (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2))) *
                 (C54 * C46 + 1e-16)) /
            std::pow(
                Pi * 4. *
                    std::pow(
                        0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                1.245 +
                            1.,
                        2) *
                    (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 2.49,
                2) -
        (C63 *
             (Pi * 4. *
                  std::pow(
                      0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                              1.245 +
                          1.,
                      2) *
                  (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 2.49 * 6.5591073862037709 *
                  rhoA -
              2.1863691287345903 * C54 * 3. * rhoA * 2.49 *
                  (Pi * 4. *
                       std::pow(
                           0.2195149727645171 *
                                   std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)),
                                            2) /
                                   1.245 +
                               1.,
                           2) *
                       (C54 * C46 + 1e-16) * 2. * 2 * C46 +
                   4. *
                       (Pi * 4. *
                            std::pow(0.2195149727645171 *
                                             std::pow(std::sqrt(4. * sigmaAa) /
                                                          (2. * (C54 * C46 + 1e-16)),
                                                      2) /
                                             1.245 +
                                         1.,
                                     2) *
                            C46 +
                        (C54 * C46 + 1e-16) * Pi * 4. *
                            (0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 1.245 +
                             1.) *
                            -1.7561197821161368 * C46 * 4. * sigmaAa /
                            (2.49 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2))) *
                       (C54 * C46 + 1e-16))) /
             std::pow(
                 Pi * 4. *
                     std::pow(
                         0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2) *
                     (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 2.49,
                 2) +
         6.5591073862037709 * C54 /
             (Pi * 4. *
              std::pow(0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                               1.245 +
                           1.,
                       2) *
              (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 2.49));

    matrix.upper[3] = 0;

    matrix.upper[4] = 0;

    const double C47 = 2. * rhoB;
    const double C51 = C49 * C47;
    const double C53 = 3. * C51;
    const double C55 = std::pow(C53, C48);
    const double C64 = 2.;
    const double C65 = C49 * C64;
    const double C66 = 3. * C65;
    const double C67 = C48 * std::pow(C53, C48 - 1) * C66;
    const double C69 = C48 * (C48 - 1) * std::pow(C53, C48 - 2) * std::pow(C66, 2);
    matrix.upper[5] =
        ((C64 * rhoB * 3. * C55 * -1.0931845643672951 * C55 * 4. * sigmaBb * 2.49 *
              ((C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                   (0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) / 1.245 +
                    1.) *
                   -1.7561197821161368 * C55 * 4. * sigmaBb /
                   (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2)) +
               ((C55 * C47 + 1e-16) * 4. * C55 * 2. * (C55 * C47 + 1e-16) +
                C55 * std::pow(2. * (C55 * C47 + 1e-16), 2)) *
                   std::pow(
                       0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                               1.245 +
                           1.,
                       2)) *
              4. * Pi /
              std::pow(2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                           std::pow(0.2195149727645171 *
                                            std::pow(std::sqrt(4. * sigmaBb) /
                                                         (2. * (C55 * C47 + 1e-16)),
                                                     2) /
                                            1.245 +
                                        1.,
                                    2) *
                           4. * Pi,
                       2) -
          (C67 *
               (3. * rhoB * -1.0931845643672951 * C55 * 4. * sigmaBb /
                    (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                     std::pow(
                         0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2)) +
                (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                     std::pow(
                         0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2) *
                     rhoB * 3. * C55 * -4.3727382574691806 * sigmaBb -
                 rhoB * 3. * C55 * -1.0931845643672951 * C47 * 4. * sigmaBb * 2.49 *
                     ((C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                          (0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                               1.245 +
                           1.) *
                          -1.7561197821161368 * C55 * 4. * sigmaBb /
                          (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2)) +
                      ((C55 * C47 + 1e-16) * 4. * C55 * 2. * (C55 * C47 + 1e-16) +
                       C55 * std::pow(2. * (C55 * C47 + 1e-16), 2)) *
                          std::pow(
                              0.2195149727645171 *
                                      std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)),
                                               2) /
                                      1.245 +
                                  1.,
                              2))) /
                    std::pow(2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                                 std::pow(0.2195149727645171 *
                                                  std::pow(std::sqrt(4. * sigmaBb) /
                                                               (2. * (C55 * C47 + 1e-16)),
                                                           2) /
                                                  1.245 +
                                              1.,
                                          2),
                             2)) +
           3. * C55 * -1.0931845643672951 * C55 * 4. * sigmaBb /
               (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                std::pow(0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2))) /
              (4. * Pi)) *
             C64 -
         (((2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                std::pow(0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2) *
                4. * Pi * C64 *
                (rhoB * 3. * C55 * -4.3727382574691806 * sigmaBb +
                 3. * rhoB * -1.0931845643672951 * C55 * 4. * sigmaBb) -
            C64 * rhoB * 3. * C55 * -1.0931845643672951 * C55 * 4. * sigmaBb * 2.49 *
                ((C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                     (0.2195149727645171 *
                          std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                          1.245 +
                      1.) *
                     -1.7561197821161368 * C47 * 4. * sigmaBb /
                     (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2)) +
                 ((C55 * C47 + 1e-16) * 4. * C47 * 2. * (C55 * C47 + 1e-16) +
                  C47 * std::pow(2. * (C55 * C47 + 1e-16), 2)) *
                     std::pow(
                         0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2)) *
                4. * Pi) /
               std::pow(2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                            std::pow(0.2195149727645171 *
                                             std::pow(std::sqrt(4. * sigmaBb) /
                                                          (2. * (C55 * C47 + 1e-16)),
                                                      2) /
                                             1.245 +
                                         1.,
                                     2) *
                            4. * Pi,
                        2) +
           (C67 *
                (3. * rhoB * -1.0931845643672951 * C47 * 4. * sigmaBb /
                     (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                      std::pow(
                          0.2195149727645171 *
                                  std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)),
                                           2) /
                                  1.245 +
                              1.,
                          2)) +
                 (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                      std::pow(
                          0.2195149727645171 *
                                  std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)),
                                           2) /
                                  1.245 +
                              1.,
                          2) *
                      3. * rhoB * -1.0931845643672951 * C47 * 4. * sigmaBb -
                  rhoB * 3. * C55 * -1.0931845643672951 * C47 * 4. * sigmaBb * 2.49 *
                      ((C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                           (0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                1.245 +
                            1.) *
                           -1.7561197821161368 * C47 * 4. * sigmaBb /
                           (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2)) +
                       ((C55 * C47 + 1e-16) * 4. * C47 * 2. * (C55 * C47 + 1e-16) +
                        C47 * std::pow(2. * (C55 * C47 + 1e-16), 2)) *
                           std::pow(0.2195149727645171 *
                                            std::pow(std::sqrt(4. * sigmaBb) /
                                                         (2. * (C55 * C47 + 1e-16)),
                                                     2) /
                                            1.245 +
                                        1.,
                                    2))) /
                     std::pow(2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                                  std::pow(0.2195149727645171 *
                                                   std::pow(std::sqrt(4. * sigmaBb) /
                                                                (2. * (C55 * C47 + 1e-16)),
                                                            2) /
                                                   1.245 +
                                               1.,
                                           2),
                              2)) +
            (3. * (2.245 -
                   1.245 / (0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                1.245 +
                            1.)) +
             3. * C55 * -1.0931845643672951 * C47 * 4. * sigmaBb /
                 (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                  std::pow(
                      0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                              1.245 +
                          1.,
                      2)))) /
               (4. * Pi)) *
              C67 +
          (C64 * 3. * C55 * -1.0931845643672951 * C55 * 4. * sigmaBb /
               (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                std::pow(0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2) *
                4. * Pi) +
           C67 *
               (3. * (2.245 -
                      1.245 /
                          (0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                               1.245 +
                           1.)) +
                3. * C55 * -1.0931845643672951 * C47 * 4. * sigmaBb /
                    (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                     std::pow(
                         0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2))) /
               (4. * Pi)))) -
        C69 *
            ((2.245 -
              1.245 /
                  (0.2195149727645171 *
                       std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) / 1.245 +
                   1.)) *
                 3. * rhoB +
             rhoB * 3. * C55 * -1.0931845643672951 * C47 * 4. * sigmaBb /
                 (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                  std::pow(
                      0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                              1.245 +
                          1.,
                      2))) /
            (4. * Pi);

    matrix.upper[6] = 0;

    matrix.upper[7] = 0;

    matrix.upper[8] =
        C64 * 2.1863691287345903 * C55 * 3. * rhoB * 2.49 *
            (Pi * 4. *
                 std::pow(
                     0.2195149727645171 *
                             std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                             1.245 +
                         1.,
                     2) *
                 (C55 * C47 + 1e-16) * 2. * 2 * C55 +
             4. *
                 (Pi * 4. *
                      std::pow(
                          0.2195149727645171 *
                                  std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)),
                                           2) /
                                  1.245 +
                              1.,
                          2) *
                      C55 +
                  (C55 * C47 + 1e-16) * Pi * 4. *
                      (0.2195149727645171 *
                           std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                           1.245 +
                       1.) *
                      -1.7561197821161368 * C55 * 4. * sigmaBb /
                      (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2))) *
                 (C55 * C47 + 1e-16)) /
            std::pow(
                Pi * 4. *
                    std::pow(
                        0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                1.245 +
                            1.,
                        2) *
                    (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 2.49,
                2) -
        (C67 *
             (Pi * 4. *
                  std::pow(
                      0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                              1.245 +
                          1.,
                      2) *
                  (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 2.49 * 6.5591073862037709 *
                  rhoB -
              2.1863691287345903 * C55 * 3. * rhoB * 2.49 *
                  (Pi * 4. *
                       std::pow(
                           0.2195149727645171 *
                                   std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)),
                                            2) /
                                   1.245 +
                               1.,
                           2) *
                       (C55 * C47 + 1e-16) * 2. * 2 * C47 +
                   4. *
                       (Pi * 4. *
                            std::pow(0.2195149727645171 *
                                             std::pow(std::sqrt(4. * sigmaBb) /
                                                          (2. * (C55 * C47 + 1e-16)),
                                                      2) /
                                             1.245 +
                                         1.,
                                     2) *
                            C47 +
                        (C55 * C47 + 1e-16) * Pi * 4. *
                            (0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 1.245 +
                             1.) *
                            -1.7561197821161368 * C47 * 4. * sigmaBb /
                            (2.49 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2))) *
                       (C55 * C47 + 1e-16))) /
             std::pow(
                 Pi * 4. *
                     std::pow(
                         0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 1.245 +
                             1.,
                         2) *
                     (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 2.49,
                 2) +
         6.5591073862037709 * C55 /
             (Pi * 4. *
              std::pow(0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                               1.245 +
                           1.,
                       2) *
              (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 2.49));

    matrix.upper[9] =
        3.5122395642322736 *
        (0.2195149727645171 * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
             1.245 +
         1.) *
        4. * Pi * 4. * rhoA * 3. * C54 * 2.1863691287345903 /
        (std::pow(
             Pi * 4. *
                 std::pow(
                     0.2195149727645171 *
                             std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                             1.245 +
                         1.,
                     2) *
                 (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 2.49,
             2) *
         2. * 2);

    matrix.upper[10] = 0;

    matrix.upper[11] = 0;

    matrix.upper[12] = 0;

    matrix.upper[13] = 0;

    matrix.upper[14] =
        3.5122395642322736 *
        (0.2195149727645171 * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
             1.245 +
         1.) *
        4. * Pi * 4. * rhoB * 3. * C55 * 2.1863691287345903 /
        (std::pow(
             Pi * 4. *
                 std::pow(
                     0.2195149727645171 *
                             std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                             1.245 +
                         1.,
                     2) *
                 (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 2.49,
             2) *
         2. * 2);
}

} // namespace excgrid
