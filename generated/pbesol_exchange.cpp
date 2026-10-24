// excgrid codegen source: the PBEsol GGA exchange functional (the PBE exchange
// form with mu = 10/81).
// J. P. Perdew et al., Phys. Rev. Lett. 100 (2008) 136406.

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

XcKernelValue PbeSolExchange(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
    (void)sigmaAb;

    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;
    // The generator's sigma-edge guard.  Written here, not injected: the printer renders
    // a number below 1e-16 as "0.", and 1e-300 leaves sigma + guard == sigma.
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

    result.exc = -((1.804 - 0.804 / (10. *
                                         std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) /
                                                      (2. * (C79 * C71 + 1e-16)),
                                                  2) /
                                         0.65124e2 +
                                     1.)) *
                       rhoB * 3. * C79 +
                   (1.804 - 0.804 / (10. *
                                         std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) /
                                                      (2. * (C78 * C70 + 1e-16)),
                                                  2) /
                                         0.65124e2 +
                                     1.)) *
                       rhoA * 3. * C78) /
                 (4. * Pi);

    const double C81 = 2.;
    const double C82 = C73 * C81;
    const double C83 = 3. * C82;
    const double C84 = C72 * std::pow(C76 + 6. * std::pow(Pi, 2) * SigmaGuard, C72 - 1) * C83;

    result.vrhoA =
        -(C81 * rhoA * 3. * C78 * -0.3216e2 * C78 * (4. * sigmaAa + 4. * SigmaGuard) /
              (0.130248e3 * (C78 * C70 + 1e-16) * std::pow(2. * (C78 * C70 + 1e-16), 2) *
               std::pow(10. *
                                std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) /
                                             (2. * (C78 * C70 + 1e-16)),
                                         2) /
                                0.65124e2 +
                            1.,
                        2) *
               4. * Pi) +
          (C84 * ((1.804 - 0.804 / (10. *
                                        std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) /
                                                     (2. * (C78 * C70 + 1e-16)),
                                                 2) /
                                        0.65124e2 +
                                    1.)) *
                      3. * rhoA +
                  rhoA * 3. * C78 * -0.3216e2 * C70 * (4. * sigmaAa + 4. * SigmaGuard) /
                      (0.130248e3 * (C78 * C70 + 1e-16) * std::pow(2. * (C78 * C70 + 1e-16), 2) *
                       std::pow(10. *
                                        std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) /
                                                     (2. * (C78 * C70 + 1e-16)),
                                                 2) /
                                        0.65124e2 +
                                    1.,
                                2))) +
           (1.804 - 0.804 / (10. *
                                 std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) /
                                              (2. * (C78 * C70 + 1e-16)),
                                          2) /
                                 0.65124e2 +
                             1.)) *
               3. * C78) /
              (4. * Pi));

    const double C87 = 2.;
    const double C88 = C73 * C87;
    const double C89 = 3. * C88;
    const double C90 = C72 * std::pow(C77 + 6. * std::pow(Pi, 2) * SigmaGuard, C72 - 1) * C89;

    result.vrhoB =
        -(C87 * rhoB * 3. * C79 * -0.3216e2 * C79 * (4. * sigmaBb + 4. * SigmaGuard) /
              (0.130248e3 * (C79 * C71 + 1e-16) * std::pow(2. * (C79 * C71 + 1e-16), 2) *
               std::pow(10. *
                                std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) /
                                             (2. * (C79 * C71 + 1e-16)),
                                         2) /
                                0.65124e2 +
                            1.,
                        2) *
               4. * Pi) +
          (C90 * ((1.804 - 0.804 / (10. *
                                        std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) /
                                                     (2. * (C79 * C71 + 1e-16)),
                                                 2) /
                                        0.65124e2 +
                                    1.)) *
                      3. * rhoB +
                  rhoB * 3. * C79 * -0.3216e2 * C71 * (4. * sigmaBb + 4. * SigmaGuard) /
                      (0.130248e3 * (C79 * C71 + 1e-16) * std::pow(2. * (C79 * C71 + 1e-16), 2) *
                       std::pow(10. *
                                        std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) /
                                                     (2. * (C79 * C71 + 1e-16)),
                                                 2) /
                                        0.65124e2 +
                                    1.,
                                2))) +
           (1.804 - 0.804 / (10. *
                                 std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) /
                                              (2. * (C79 * C71 + 1e-16)),
                                          2) /
                                 0.65124e2 +
                             1.)) *
               3. * C79) /
              (4. * Pi));

    result.vsigmaAa = -0.6432e2 * C78 * 3. * rhoA /
                      (Pi * 4. *
                       std::pow(10. *
                                        std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) /
                                                     (2. * (C78 * C70 + 1e-16)),
                                                 2) /
                                        0.65124e2 +
                                    1.,
                                2) *
                       (C78 * C70 + 1e-16) * 2. * 2 * (C78 * C70 + 1e-16) * 0.130248e3);

    result.vsigmaAb = 0;

    result.vsigmaBb = -0.6432e2 * C79 * 3. * rhoB /
                      (Pi * 4. *
                       std::pow(10. *
                                        std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) /
                                                     (2. * (C79 * C71 + 1e-16)),
                                                 2) /
                                        0.65124e2 +
                                    1.,
                                2) *
                       (C79 * C71 + 1e-16) * 2. * 2 * (C79 * C71 + 1e-16) * 0.130248e3);

    return result;
}

void PbeSolExchangeSecondDerivatives(double rhoA,
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
    matrix.upper[0] = -(
        (sigmaAa * C54 * 0. * C60 /
             (Pi *
              std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                               0.65124e2 +
                           1.,
                       2) *
              std::pow(2. * (C54 * C46 + 1e-16), 2) * (C54 * C46 + 1e-16) * 0.130248e3) +
         (C63 * 0. * C46 * 4. * sigmaAa /
              (0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
               std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                0.65124e2 +
                            1.,
                        2)) +
          ((1.804 -
            0.804 / (10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                         0.65124e2 +
                     1.)) *
               3. * rhoA +
           rhoA * 3. * C54 * -0.3216e2 * C46 * 4. * sigmaAa /
               (0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2)))) /
             (4. * Pi)) *
            C68 +
        (((0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
               std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                0.65124e2 +
                            1.,
                        2) *
               4. * Pi * C60 * 0. * C54 * 4. * sigmaAa -
           C60 * rhoA * 3. * C54 * -0.3216e2 * C54 * 4. * sigmaAa * 0.130248e3 *
               (sigmaAa * 4. * C54 * -80. *
                    (10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                         0.65124e2 +
                     1.) /
                    0.130248e3 +
                ((C54 * C46 + 1e-16) * 4. * C54 * 2. * (C54 * C46 + 1e-16) +
                 C54 * std::pow(2. * (C54 * C46 + 1e-16), 2)) *
                    std::pow(
                        10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                0.65124e2 +
                            1.,
                        2)) *
               4. * Pi) /
              std::pow(
                  0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                      std::pow(
                          10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                  0.65124e2 +
                              1.,
                          2) *
                      4. * Pi,
                  2) +
          (C63 *
               (3. * rhoA * -0.3216e2 * C54 * 4. * sigmaAa /
                    (0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                     std::pow(
                         10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2)) +
                (0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                     std::pow(
                         10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2) *
                     (rhoA * 3. * C54 * -0.12864e3 * sigmaAa + 0. * C46 * 4. * sigmaAa) -
                 rhoA * 3. * C54 * -0.3216e2 * C46 * 4. * sigmaAa * 0.130248e3 *
                     (sigmaAa * 4. * C54 * -80. *
                          (10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                               0.65124e2 +
                           1.) /
                          0.130248e3 +
                      ((C54 * C46 + 1e-16) * 4. * C54 * 2. * (C54 * C46 + 1e-16) +
                       C54 * std::pow(2. * (C54 * C46 + 1e-16), 2)) *
                          std::pow(10. *
                                           std::pow(std::sqrt(4. * sigmaAa) /
                                                        (2. * (C54 * C46 + 1e-16)),
                                                    2) /
                                           0.65124e2 +
                                       1.,
                                   2))) /
                    std::pow(0.130248e3 * (C54 * C46 + 1e-16) *
                                 std::pow(2. * (C54 * C46 + 1e-16), 2) *
                                 std::pow(10. *
                                                  std::pow(std::sqrt(4. * sigmaAa) /
                                                               (2. * (C54 * C46 + 1e-16)),
                                                           2) /
                                                  0.65124e2 +
                                              1.,
                                          2),
                             2)) +
           3. * C54 * -0.3216e2 * C54 * 4. * sigmaAa /
               (0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2))) /
              (4. * Pi)) *
             C60 +
         (((0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2) *
                4. * Pi * C60 *
                (rhoA * 3. * C54 * -0.12864e3 * sigmaAa +
                 3. * rhoA * -0.3216e2 * C54 * 4. * sigmaAa) -
            C60 * rhoA * 3. * C54 * -0.3216e2 * C54 * 4. * sigmaAa * 0.130248e3 *
                (sigmaAa * 4. * C46 * -80. *
                     (10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                          0.65124e2 +
                      1.) /
                     0.130248e3 +
                 ((C54 * C46 + 1e-16) * 4. * C46 * 2. * (C54 * C46 + 1e-16) +
                  C46 * std::pow(2. * (C54 * C46 + 1e-16), 2)) *
                     std::pow(
                         10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2)) *
                4. * Pi) /
               std::pow(
                   0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                       std::pow(
                           10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                   0.65124e2 +
                               1.,
                           2) *
                       4. * Pi,
                   2) +
           (C63 *
                (3. * rhoA * -0.3216e2 * C46 * 4. * sigmaAa /
                     (0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                      std::pow(
                          10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                  0.65124e2 +
                              1.,
                          2)) +
                 (0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                      std::pow(
                          10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                  0.65124e2 +
                              1.,
                          2) *
                      3. * rhoA * -0.3216e2 * C46 * 4. * sigmaAa -
                  rhoA * 3. * C54 * -0.3216e2 * C46 * 4. * sigmaAa * 0.130248e3 *
                      (sigmaAa * 4. * C46 * -80. *
                           (10. *
                                std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                0.65124e2 +
                            1.) /
                           0.130248e3 +
                       ((C54 * C46 + 1e-16) * 4. * C46 * 2. * (C54 * C46 + 1e-16) +
                        C46 * std::pow(2. * (C54 * C46 + 1e-16), 2)) *
                           std::pow(10. *
                                            std::pow(std::sqrt(4. * sigmaAa) /
                                                         (2. * (C54 * C46 + 1e-16)),
                                                     2) /
                                            0.65124e2 +
                                        1.,
                                    2))) /
                     std::pow(0.130248e3 * (C54 * C46 + 1e-16) *
                                  std::pow(2. * (C54 * C46 + 1e-16), 2) *
                                  std::pow(10. *
                                                   std::pow(std::sqrt(4. * sigmaAa) /
                                                                (2. * (C54 * C46 + 1e-16)),
                                                            2) /
                                                   0.65124e2 +
                                               1.,
                                           2),
                              2)) +
            (3. *
                 (1.804 -
                  0.804 / (10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                               0.65124e2 +
                           1.)) +
             3. * C54 * -0.3216e2 * C46 * 4. * sigmaAa /
                 (0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                  std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                   0.65124e2 +
                               1.,
                           2)))) /
               (4. * Pi)) *
              C63 +
          (sigmaAa * C54 * -0.3216e2 * C54 * 3. * C60 /
               (Pi *
                std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2) *
                std::pow(2. * (C54 * C46 + 1e-16), 2) * (C54 * C46 + 1e-16) * 0.130248e3) +
           C63 *
               (3. * (1.804 -
                      0.804 /
                          (10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                               0.65124e2 +
                           1.)) +
                3. * C54 * -0.3216e2 * C46 * 4. * sigmaAa /
                    (0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                     std::pow(
                         10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2))) /
               (4. * Pi)))));

    matrix.upper[1] =
        -(C60 * 0. * C54 * 4. * sigmaAa + C63 * 0. * C46 * 4. * sigmaAa) /
        (0.130248e3 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
         std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                          0.65124e2 +
                      1.,
                  2) *
         4. * Pi);

    matrix.upper[2] = -(
        C60 *
            (Pi * 4. *
                 std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                  0.65124e2 +
                              1.,
                          2) *
                 (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 0.130248e3 *
                 (0. * C54 + 0. * C54 * rhoA) -
             0.6432e2 * C54 * 3. * rhoA * 0.130248e3 *
                 (Pi * 4. *
                      std::pow(
                          10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                  0.65124e2 +
                              1.,
                          2) *
                      (C54 * C46 + 1e-16) * 2. * 2 * C54 +
                  4. *
                      (Pi * 4. *
                           std::pow(10. *
                                            std::pow(std::sqrt(4. * sigmaAa) /
                                                         (2. * (C54 * C46 + 1e-16)),
                                                     2) /
                                            0.65124e2 +
                                        1.,
                                    2) *
                           C54 +
                       sigmaAa * 4. * C54 * -80. *
                           (10. *
                                std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                0.65124e2 +
                            1.) *
                           4. * Pi / (std::pow(2. * (C54 * C46 + 1e-16), 2) * 0.130248e3)) *
                      (C54 * C46 + 1e-16))) /
            std::pow(
                Pi * 4. *
                    std::pow(
                        10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                0.65124e2 +
                            1.,
                        2) *
                    (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 0.130248e3,
                2) +
        (C63 *
             (Pi * 4. *
                  std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                   0.65124e2 +
                               1.,
                           2) *
                  (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 0.130248e3 *
                  (0. * C54 + (0. * C54 + 0.19296e3) * rhoA) -
              0.6432e2 * C54 * 3. * rhoA * 0.130248e3 *
                  (Pi * 4. *
                       std::pow(
                           10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                   0.65124e2 +
                               1.,
                           2) *
                       (C54 * C46 + 1e-16) * 2. * 2 * C46 +
                   4. *
                       (Pi * 4. *
                            std::pow(10. *
                                             std::pow(std::sqrt(4. * sigmaAa) /
                                                          (2. * (C54 * C46 + 1e-16)),
                                                      2) /
                                             0.65124e2 +
                                         1.,
                                     2) *
                            C46 +
                        sigmaAa * 4. * C46 * -80. *
                            (10. *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.) *
                            4. * Pi / (std::pow(2. * (C54 * C46 + 1e-16), 2) * 0.130248e3)) *
                       (C54 * C46 + 1e-16))) /
             std::pow(
                 Pi * 4. *
                     std::pow(
                         10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2) *
                     (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 0.130248e3,
                 2) +
         (0.19296e3 * C54 + 0. * C54 * rhoA) /
             (Pi * 4. *
              std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                               0.65124e2 +
                           1.,
                       2) *
              (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 0.130248e3)));

    matrix.upper[3] = 0;

    const double C47 = 2. * rhoB;
    const double C51 = C49 * C47;
    const double C53 = 3. * C51;
    const double C55 = std::pow(C53, C48);
    matrix.upper[4] =
        -(0. * C55 + 0. * C55 * rhoB) /
        (Pi * 4. *
         std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                          0.65124e2 +
                      1.,
                  2) *
         (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 0.130248e3);

    const double C64 = 2.;
    const double C65 = C49 * C64;
    const double C66 = 3. * C65;
    const double C67 = C48 * std::pow(C53, C48 - 1) * C66;
    const double C69 = C48 * (C48 - 1) * std::pow(C53, C48 - 2) * std::pow(C66, 2);
    matrix.upper[5] = -(
        (sigmaBb * C55 * 0. * C64 /
             (Pi *
              std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                               0.65124e2 +
                           1.,
                       2) *
              std::pow(2. * (C55 * C47 + 1e-16), 2) * (C55 * C47 + 1e-16) * 0.130248e3) +
         (C67 * 0. * C47 * 4. * sigmaBb /
              (0.130248e3 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
               std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                0.65124e2 +
                            1.,
                        2)) +
          ((1.804 -
            0.804 / (10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                         0.65124e2 +
                     1.)) *
               3. * rhoB +
           rhoB * 3. * C55 * -0.3216e2 * C47 * 4. * sigmaBb /
               (0.130248e3 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2)))) /
             (4. * Pi)) *
            C69 +
        (((0.130248e3 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
               std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                0.65124e2 +
                            1.,
                        2) *
               4. * Pi * C64 * 0. * C55 * 4. * sigmaBb -
           C64 * rhoB * 3. * C55 * -0.3216e2 * C55 * 4. * sigmaBb * 0.130248e3 *
               (sigmaBb * 4. * C55 * -80. *
                    (10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                         0.65124e2 +
                     1.) /
                    0.130248e3 +
                ((C55 * C47 + 1e-16) * 4. * C55 * 2. * (C55 * C47 + 1e-16) +
                 C55 * std::pow(2. * (C55 * C47 + 1e-16), 2)) *
                    std::pow(
                        10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                0.65124e2 +
                            1.,
                        2)) *
               4. * Pi) /
              std::pow(
                  0.130248e3 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                      std::pow(
                          10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                  0.65124e2 +
                              1.,
                          2) *
                      4. * Pi,
                  2) +
          (C67 *
               (3. * rhoB * -0.3216e2 * C55 * 4. * sigmaBb /
                    (0.130248e3 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                     std::pow(
                         10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2)) +
                (0.130248e3 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                     std::pow(
                         10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2) *
                     (rhoB * 3. * C55 * -0.12864e3 * sigmaBb + 0. * C47 * 4. * sigmaBb) -
                 rhoB * 3. * C55 * -0.3216e2 * C47 * 4. * sigmaBb * 0.130248e3 *
                     (sigmaBb * 4. * C55 * -80. *
                          (10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                               0.65124e2 +
                           1.) /
                          0.130248e3 +
                      ((C55 * C47 + 1e-16) * 4. * C55 * 2. * (C55 * C47 + 1e-16) +
                       C55 * std::pow(2. * (C55 * C47 + 1e-16), 2)) *
                          std::pow(10. *
                                           std::pow(std::sqrt(4. * sigmaBb) /
                                                        (2. * (C55 * C47 + 1e-16)),
                                                    2) /
                                           0.65124e2 +
                                       1.,
                                   2))) /
                    std::pow(0.130248e3 * (C55 * C47 + 1e-16) *
                                 std::pow(2. * (C55 * C47 + 1e-16), 2) *
                                 std::pow(10. *
                                                  std::pow(std::sqrt(4. * sigmaBb) /
                                                               (2. * (C55 * C47 + 1e-16)),
                                                           2) /
                                                  0.65124e2 +
                                              1.,
                                          2),
                             2)) +
           3. * C55 * -0.3216e2 * C55 * 4. * sigmaBb /
               (0.130248e3 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2))) /
              (4. * Pi)) *
             C64 +
         (((0.130248e3 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2) *
                4. * Pi * C64 *
                (rhoB * 3. * C55 * -0.12864e3 * sigmaBb +
                 3. * rhoB * -0.3216e2 * C55 * 4. * sigmaBb) -
            C64 * rhoB * 3. * C55 * -0.3216e2 * C55 * 4. * sigmaBb * 0.130248e3 *
                (sigmaBb * 4. * C47 * -80. *
                     (10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                          0.65124e2 +
                      1.) /
                     0.130248e3 +
                 ((C55 * C47 + 1e-16) * 4. * C47 * 2. * (C55 * C47 + 1e-16) +
                  C47 * std::pow(2. * (C55 * C47 + 1e-16), 2)) *
                     std::pow(
                         10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2)) *
                4. * Pi) /
               std::pow(
                   0.130248e3 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                       std::pow(
                           10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                   0.65124e2 +
                               1.,
                           2) *
                       4. * Pi,
                   2) +
           (C67 *
                (3. * rhoB * -0.3216e2 * C47 * 4. * sigmaBb /
                     (0.130248e3 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                      std::pow(
                          10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                  0.65124e2 +
                              1.,
                          2)) +
                 (0.130248e3 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                      std::pow(
                          10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                  0.65124e2 +
                              1.,
                          2) *
                      3. * rhoB * -0.3216e2 * C47 * 4. * sigmaBb -
                  rhoB * 3. * C55 * -0.3216e2 * C47 * 4. * sigmaBb * 0.130248e3 *
                      (sigmaBb * 4. * C47 * -80. *
                           (10. *
                                std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                0.65124e2 +
                            1.) /
                           0.130248e3 +
                       ((C55 * C47 + 1e-16) * 4. * C47 * 2. * (C55 * C47 + 1e-16) +
                        C47 * std::pow(2. * (C55 * C47 + 1e-16), 2)) *
                           std::pow(10. *
                                            std::pow(std::sqrt(4. * sigmaBb) /
                                                         (2. * (C55 * C47 + 1e-16)),
                                                     2) /
                                            0.65124e2 +
                                        1.,
                                    2))) /
                     std::pow(0.130248e3 * (C55 * C47 + 1e-16) *
                                  std::pow(2. * (C55 * C47 + 1e-16), 2) *
                                  std::pow(10. *
                                                   std::pow(std::sqrt(4. * sigmaBb) /
                                                                (2. * (C55 * C47 + 1e-16)),
                                                            2) /
                                                   0.65124e2 +
                                               1.,
                                           2),
                              2)) +
            (3. *
                 (1.804 -
                  0.804 / (10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                               0.65124e2 +
                           1.)) +
             3. * C55 * -0.3216e2 * C47 * 4. * sigmaBb /
                 (0.130248e3 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                  std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                   0.65124e2 +
                               1.,
                           2)))) /
               (4. * Pi)) *
              C67 +
          (sigmaBb * C55 * -0.3216e2 * C55 * 3. * C64 /
               (Pi *
                std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2) *
                std::pow(2. * (C55 * C47 + 1e-16), 2) * (C55 * C47 + 1e-16) * 0.130248e3) +
           C67 *
               (3. * (1.804 -
                      0.804 /
                          (10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                               0.65124e2 +
                           1.)) +
                3. * C55 * -0.3216e2 * C47 * 4. * sigmaBb /
                    (0.130248e3 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                     std::pow(
                         10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2))) /
               (4. * Pi)))));

    matrix.upper[6] =
        -(0. * C54 + 0. * C54 * rhoA) /
        (Pi * 4. *
         std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                          0.65124e2 +
                      1.,
                  2) *
         (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 0.130248e3);

    matrix.upper[7] = 0;

    matrix.upper[8] = -(
        C64 *
            (Pi * 4. *
                 std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                  0.65124e2 +
                              1.,
                          2) *
                 (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 0.130248e3 *
                 (0. * C55 + 0. * C55 * rhoB) -
             0.6432e2 * C55 * 3. * rhoB * 0.130248e3 *
                 (Pi * 4. *
                      std::pow(
                          10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                  0.65124e2 +
                              1.,
                          2) *
                      (C55 * C47 + 1e-16) * 2. * 2 * C55 +
                  4. *
                      (Pi * 4. *
                           std::pow(10. *
                                            std::pow(std::sqrt(4. * sigmaBb) /
                                                         (2. * (C55 * C47 + 1e-16)),
                                                     2) /
                                            0.65124e2 +
                                        1.,
                                    2) *
                           C55 +
                       sigmaBb * 4. * C55 * -80. *
                           (10. *
                                std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                0.65124e2 +
                            1.) *
                           4. * Pi / (std::pow(2. * (C55 * C47 + 1e-16), 2) * 0.130248e3)) *
                      (C55 * C47 + 1e-16))) /
            std::pow(
                Pi * 4. *
                    std::pow(
                        10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                0.65124e2 +
                            1.,
                        2) *
                    (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 0.130248e3,
                2) +
        (C67 *
             (Pi * 4. *
                  std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                   0.65124e2 +
                               1.,
                           2) *
                  (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 0.130248e3 *
                  (0. * C55 + (0. * C55 + 0.19296e3) * rhoB) -
              0.6432e2 * C55 * 3. * rhoB * 0.130248e3 *
                  (Pi * 4. *
                       std::pow(
                           10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                   0.65124e2 +
                               1.,
                           2) *
                       (C55 * C47 + 1e-16) * 2. * 2 * C47 +
                   4. *
                       (Pi * 4. *
                            std::pow(10. *
                                             std::pow(std::sqrt(4. * sigmaBb) /
                                                          (2. * (C55 * C47 + 1e-16)),
                                                      2) /
                                             0.65124e2 +
                                         1.,
                                     2) *
                            C47 +
                        sigmaBb * 4. * C47 * -80. *
                            (10. *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.) *
                            4. * Pi / (std::pow(2. * (C55 * C47 + 1e-16), 2) * 0.130248e3)) *
                       (C55 * C47 + 1e-16))) /
             std::pow(
                 Pi * 4. *
                     std::pow(
                         10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2) *
                     (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 0.130248e3,
                 2) +
         (0.19296e3 * C55 + 0. * C55 * rhoB) /
             (Pi * 4. *
              std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                               0.65124e2 +
                           1.,
                       2) *
              (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 0.130248e3)));

    matrix.upper[9] =
        -(Pi * 4. *
              std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                               0.65124e2 +
                           1.,
                       2) *
              (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 0.130248e3 *
              (0. * C54 + 0. * C54 * rhoA) -
          160. *
              (10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) / 0.65124e2 +
               1.) *
              4. * Pi * 4. * rhoA * 3. * C54 * 0.6432e2 / (2. * 2)) /
        std::pow(
            Pi * 4. *
                std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2) *
                (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 0.130248e3,
            2);

    matrix.upper[10] =
        -(0. * C54 + 0. * C54 * rhoA) /
        (Pi * 4. *
         std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                          0.65124e2 +
                      1.,
                  2) *
         (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 0.130248e3);

    matrix.upper[11] =
        -(0. * C54 + 0. * C54 * rhoA) /
        (Pi * 4. *
         std::pow(10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                          0.65124e2 +
                      1.,
                  2) *
         (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 0.130248e3);

    matrix.upper[12] = 0;

    matrix.upper[13] = 0;

    matrix.upper[14] =
        -(Pi * 4. *
              std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                               0.65124e2 +
                           1.,
                       2) *
              (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 0.130248e3 *
              (0. * C55 + 0. * C55 * rhoB) -
          160. *
              (10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) / 0.65124e2 +
               1.) *
              4. * Pi * 4. * rhoB * 3. * C55 * 0.6432e2 / (2. * 2)) /
        std::pow(
            Pi * 4. *
                std::pow(10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.65124e2 +
                             1.,
                         2) *
                (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 0.130248e3,
            2);
}

} // namespace excgrid
