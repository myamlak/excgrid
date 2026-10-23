// excgrid codegen source: the Becke 1988 GGA exchange functional.
// Fresh authorship; formula from the B88 paper (docs/mainpage.md
// references).  Regenerate with tools/regenerate.py.

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

#include "excgrid/internal/asinh_over_x.hpp"

#include <cmath>
#include <limits>

namespace excgrid {

XcKernelValue Becke88Exchange(
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

    const double C127 = 4. * Pi;
    const double C128 = rhoA + 1e-16;
    const double C129 = rhoB + 1e-16;
    const double C130 = 1. / 3.;
    const double C131 = 4. / 3.;
    const double C132 = std::sqrt(sigmaAa + SigmaGuard);
    const double C133 = std::sqrt(sigmaBb + SigmaGuard);
    const double C134 = 3. / C127;
    const double C135 = std::pow(C128, C131);
    const double C136 = std::pow(C129, C131);
    const double C137 = C132 / C135;
    const double C138 = C133 / C136;
    const double C139 = std::pow(C134, C130);
    const double C140 = 3. * C139;
    const double C141 = std::pow(C137, 2);
    const double C142 = std::pow(C138, 2);
    const double C143 = C140 / 2.;

    result.exc =
        -(std::pow(rhoB, C131) *
              (C143 + 0.0042 * C142 /
                          (std::log(C138 + std::sqrt(C142 + 1.)) * 0.0252 * C133 / C136 + 1.)) +
          std::pow(rhoA, C131) *
              (C143 + 0.0042 * C141 /
                          (std::log(C137 + std::sqrt(C141 + 1.)) * 0.0252 * C132 / C135 + 1.)));

    const double C147 = 1;
    const double C148 = C131 * std::pow(C128, C131 - 1) * C147;
    const double C149 = (-C148 * std::sqrt(sigmaAa + SigmaGuard)) / std::pow(C135, 2);
    const double C150 = 2 * C137 * C149;

    result.vrhoA =
        (C149 * std::pow(rhoA, C131) * 0.0042 * C141 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
             ((C137 + std::sqrt(C141 + 1.)) * C135 *
              std::pow(std::log(C137 + std::sqrt(C141 + 1.)) * 0.0252 *
                               std::sqrt(sigmaAa + SigmaGuard) / C135 +
                           1.,
                       2)) -
         (C150 * std::pow(rhoA, C131) *
              (0.0042 * (std::log(C137 + std::sqrt(C141 + 1.)) * 0.0252 *
                             std::sqrt(sigmaAa + SigmaGuard) / C135 +
                         1.) -
               0.0042 * C141 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                   (2 * std::sqrt(C141 + 1.) * (C137 + std::sqrt(C141 + 1.)) * C135)) /
              std::pow(std::log(C137 + std::sqrt(C141 + 1.)) * 0.0252 *
                               std::sqrt(sigmaAa + SigmaGuard) / C135 +
                           1.,
                       2) +
          C131 * std::pow(rhoA, C131 - 1) *
              (C143 + 0.0042 * C141 /
                          (std::log(C137 + std::sqrt(C141 + 1.)) * 0.0252 *
                               std::sqrt(sigmaAa + SigmaGuard) / C135 +
                           1.)))) +
        C148 * std::pow(rhoA, C131) * -0.0042 * C141 * std::log(C137 + std::sqrt(C141 + 1.)) *
            0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
            (std::pow(C135, 2) * std::pow(std::log(C137 + std::sqrt(C141 + 1.)) * 0.0252 *
                                                  std::sqrt(sigmaAa + SigmaGuard) / C135 +
                                              1.,
                                          2));

    const double C156 = 1;
    const double C157 = C131 * std::pow(C129, C131 - 1) * C156;
    const double C158 = (-C157 * std::sqrt(sigmaBb + SigmaGuard)) / std::pow(C136, 2);
    const double C159 = 2 * C138 * C158;

    result.vrhoB =
        (C158 * std::pow(rhoB, C131) * 0.0042 * C142 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
             ((C138 + std::sqrt(C142 + 1.)) * C136 *
              std::pow(std::log(C138 + std::sqrt(C142 + 1.)) * 0.0252 *
                               std::sqrt(sigmaBb + SigmaGuard) / C136 +
                           1.,
                       2)) -
         (C159 * std::pow(rhoB, C131) *
              (0.0042 * (std::log(C138 + std::sqrt(C142 + 1.)) * 0.0252 *
                             std::sqrt(sigmaBb + SigmaGuard) / C136 +
                         1.) -
               0.0042 * C142 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                   (2 * std::sqrt(C142 + 1.) * (C138 + std::sqrt(C142 + 1.)) * C136)) /
              std::pow(std::log(C138 + std::sqrt(C142 + 1.)) * 0.0252 *
                               std::sqrt(sigmaBb + SigmaGuard) / C136 +
                           1.,
                       2) +
          C131 * std::pow(rhoB, C131 - 1) *
              (C143 + 0.0042 * C142 /
                          (std::log(C138 + std::sqrt(C142 + 1.)) * 0.0252 *
                               std::sqrt(sigmaBb + SigmaGuard) / C136 +
                           1.)))) +
        C157 * std::pow(rhoB, C131) * -0.0042 * C142 * std::log(C138 + std::sqrt(C142 + 1.)) *
            0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
            (std::pow(C136, 2) * std::pow(std::log(C138 + std::sqrt(C142 + 1.)) * 0.0252 *
                                                  std::sqrt(sigmaBb + SigmaGuard) / C136 +
                                              1.,
                                          2));

    const double C165 = 1 / (2 * std::sqrt(sigmaAa + SigmaGuard) * C135);
    const double C166 = 2 * C137 * C165;

    result.vsigmaAa =
        (std::pow(rhoA, C131) * 0.0042 * C141 * 0.0252 * std::log(C137 + std::sqrt(C141 + 1.)) /
             (2 * std::sqrt(sigmaAa + SigmaGuard) * C135 *
              std::pow(std::log(C137 + std::sqrt(C141 + 1.)) * 0.0252 *
                               std::sqrt(sigmaAa + SigmaGuard) / C135 +
                           1.,
                       2)) -
         C166 * std::pow(rhoA, C131) *
             (0.0042 * (std::log(C137 + std::sqrt(C141 + 1.)) * 0.0252 *
                            std::sqrt(sigmaAa + SigmaGuard) / C135 +
                        1.) -
              0.0042 * C141 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                  (2 * std::sqrt(C141 + 1.) * (C137 + std::sqrt(C141 + 1.)) * C135)) /
             std::pow(std::log(C137 + std::sqrt(C141 + 1.)) * 0.0252 *
                              std::sqrt(sigmaAa + SigmaGuard) / C135 +
                          1.,
                      2)) +
        C165 * std::pow(rhoA, C131) * 0.0042 * C141 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
            ((C137 + std::sqrt(C141 + 1.)) * C135 *
             std::pow(std::log(C137 + std::sqrt(C141 + 1.)) * 0.0252 *
                              std::sqrt(sigmaAa + SigmaGuard) / C135 +
                          1.,
                      2));

    result.vsigmaAb = 0;

    const double C167 = 1 / (2 * std::sqrt(sigmaBb + SigmaGuard) * C136);
    const double C168 = 2 * C138 * C167;

    result.vsigmaBb =
        (std::pow(rhoB, C131) * 0.0042 * C142 * 0.0252 * std::log(C138 + std::sqrt(C142 + 1.)) /
             (2 * std::sqrt(sigmaBb + SigmaGuard) * C136 *
              std::pow(std::log(C138 + std::sqrt(C142 + 1.)) * 0.0252 *
                               std::sqrt(sigmaBb + SigmaGuard) / C136 +
                           1.,
                       2)) -
         C168 * std::pow(rhoB, C131) *
             (0.0042 * (std::log(C138 + std::sqrt(C142 + 1.)) * 0.0252 *
                            std::sqrt(sigmaBb + SigmaGuard) / C136 +
                        1.) -
              0.0042 * C142 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                  (2 * std::sqrt(C142 + 1.) * (C138 + std::sqrt(C142 + 1.)) * C136)) /
             std::pow(std::log(C138 + std::sqrt(C142 + 1.)) * 0.0252 *
                              std::sqrt(sigmaBb + SigmaGuard) / C136 +
                          1.,
                      2)) +
        C167 * std::pow(rhoB, C131) * 0.0042 * C142 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
            ((C138 + std::sqrt(C142 + 1.)) * C136 *
             std::pow(std::log(C138 + std::sqrt(C142 + 1.)) * 0.0252 *
                              std::sqrt(sigmaBb + SigmaGuard) / C136 +
                          1.,
                      2));

    return result;
}

void Becke88ExchangeSecondDerivatives(double rhoA,
                                      double rhoB,
                                      double sigmaAa,
                                      double sigmaAb,
                                      double sigmaBb,
                                      PointSecondDerivativeMatrix& matrix) {
    (void)sigmaAb;

    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;
    [[maybe_unused]] constexpr double SigmaGuard = 1e-300;

    if (rhoA + rhoB < 2. * std::numeric_limits<double>::epsilon())
    {
        return;
    }

    const double C46 = 4. * Pi;
    const double C47 = rhoA + 1e-16;
    const double C49 = sigmaAa + SigmaGuard;
    const double C51 = 1. / 3.;
    const double C52 = 4. / 3.;
    const double C53 = 3. / C46;
    const double C54 = std::sqrt(C49);
    const double C56 = std::pow(C47, C52);
    const double C58 = C54 / C56;
    const double C60 = std::pow(C53, C51);
    const double C61 = 3. * C60;
    const double C62 = std::pow(C58, 2);
    const double C64 = C61 / 2.;
    const double C69 = 1;
    const double C70 = C52 * std::pow(C47, C52 - 1) * C69;
    const double C71 = (-C70 * std::sqrt(C49)) / std::pow(C56, 2);
    const double C72 = 2 * C58 * C71;
    const double C83 = C52 * (C52 - 1) * std::pow(C47, C52 - 2) * std::pow(C69, 2);
    const double C84 = C70 * C70 * std::sqrt(C49) * 2 * C56 / std::pow(C56, 4) -
                       C83 * std::sqrt(C49) / std::pow(C56, 2);
    const double C85 = 2 * std::pow(C71, 2) + 2 * C58 * C84;
    matrix.upper[0] =
        (((((((C71 * C52 * std::pow(rhoA, C52 - 1) * 0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                   ((C58 + std::sqrt(C62 + 1.)) * C56 *
                    std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                 1.,
                             2)) -
               (C72 * C52 * std::pow(rhoA, C52 - 1) *
                    (0.0042 * (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                               1.) -
                     0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                         (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) /
                    std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                 1.,
                             2) +
                C52 * (C52 - 1) * std::pow(rhoA, C52 - 2) *
                    (C64 +
                     0.0042 * C62 /
                         (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                          1.)))) +
              C70 * C52 * std::pow(rhoA, C52 - 1) * -0.0042 * C62 *
                  std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) /
                  (std::pow(C56, 2) *
                   std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                1.,
                            2))) +
             C84 * std::pow(rhoA, C52) * 0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                 ((C58 + std::sqrt(C62 + 1.)) * C56 *
                  std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                           2))) +
            ((((C58 + std::sqrt(C62 + 1.)) * C56 *
                   std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                1.,
                            2) *
                   C71 * std::pow(rhoA, C52) * 0.00010584 * std::sqrt(C49) -
               C71 * std::pow(rhoA, C52) * 0.0042 * C62 * 0.0252 * std::sqrt(C49) *
                   ((C58 + std::sqrt(C62 + 1.)) * C56 *
                        (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) *
                        0.0504 * std::sqrt(C49) /
                        (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56) +
                    std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                 1.,
                             2) *
                        C56 / (2 * std::sqrt(C62 + 1.)))) /
                  std::pow((C58 + std::sqrt(C62 + 1.)) * C56 *
                               std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 *
                                                std::sqrt(C49) / C56 +
                                            1.,
                                        2),
                           2) -
              ((std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                         2) *
                    C72 * std::pow(rhoA, C52) *
                    (0.00010584 * std::sqrt(C49) /
                         (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56) -
                     (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56 * 0.00010584 *
                          std::sqrt(C49) -
                      0.0042 * C62 * 0.0252 * std::sqrt(C49) * C56 *
                          (std::sqrt(C62 + 1.) + (C58 + std::sqrt(C62 + 1.))) /
                          std::sqrt(C62 + 1.)) /
                         std::pow(2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56, 2)) -
                C72 * std::pow(rhoA, C52) *
                    (0.0042 * (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                               1.) -
                     0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                         (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) *
                    (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) *
                    0.0504 * std::sqrt(C49) /
                    (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) /
                   std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                1.,
                            4) +
               C52 * std::pow(rhoA, C52 - 1) *
                   (0.0042 *
                        (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) -
                    0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                        (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) /
                   std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                1.,
                            2))) +
             (std::pow(C56, 2) *
                  std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                           2) *
                  C70 * std::pow(rhoA, C52) * -0.0042 *
                  (C62 * 0.0252 * std::sqrt(C49) /
                       (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.))) +
                   std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49)) -
              C70 * std::pow(rhoA, C52) * -0.0042 * C62 * std::log(C58 + std::sqrt(C62 + 1.)) *
                  0.0252 * std::sqrt(C49) * std::pow(C56, 2) *
                  (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) *
                  0.0504 * std::sqrt(C49) /
                  (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) /
                 std::pow(std::pow(C56, 2) * std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 *
                                                              std::sqrt(C49) / C56 +
                                                          1.,
                                                      2),
                          2)) *
                C72) +
           (((-C71 * std::pow(rhoA, C52) * 0.0042 * C62 * 0.0252 * std::sqrt(C49) *
              ((C58 + std::sqrt(C62 + 1.)) * C56 *
                   (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) *
                   0.0504 * std::sqrt(C49) / ((C58 + std::sqrt(C62 + 1.)) * C56) +
               C56 * std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                  1.,
                              2))) /
                 std::pow((C58 + std::sqrt(C62 + 1.)) * C56 *
                              std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 *
                                               std::sqrt(C49) / C56 +
                                           1.,
                                       2),
                          2) -
             ((std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                        2) *
                   C72 * std::pow(rhoA, C52) *
                   (0.00010584 * std::sqrt(C49) / ((C58 + std::sqrt(C62 + 1.)) * C56) -
                    (-0.0042 * C62 * 0.0252 * std::sqrt(C49) * 2 * std::sqrt(C62 + 1.) * C56) /
                        std::pow(2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56, 2)) -
               C72 * std::pow(rhoA, C52) *
                   (0.0042 *
                        (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) -
                    0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                        (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) *
                   (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) *
                   0.0504 * std::sqrt(C49) / ((C58 + std::sqrt(C62 + 1.)) * C56)) /
                  std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                           4) -
              C52 * std::pow(rhoA, C52 - 1) * 0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                  ((C58 + std::sqrt(C62 + 1.)) * C56 *
                   std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                1.,
                            2)))) +
            (std::pow(C56, 2) *
                 std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                          2) *
                 C70 * std::pow(rhoA, C52) * -0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                 (C58 + std::sqrt(C62 + 1.)) -
             C70 * std::pow(rhoA, C52) * -0.0042 * C62 * std::log(C58 + std::sqrt(C62 + 1.)) *
                 0.0252 * std::sqrt(C49) * std::pow(C56, 2) *
                 (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) *
                 0.0504 * std::sqrt(C49) / ((C58 + std::sqrt(C62 + 1.)) * C56)) /
                std::pow(std::pow(C56, 2) * std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 *
                                                             std::sqrt(C49) / C56 +
                                                         1.,
                                                     2),
                         2)) *
               C71) +
          (((-C71 * std::pow(rhoA, C52) * 0.0042 * C62 * 0.0252 * std::sqrt(C49) *
             ((C58 + std::sqrt(C62 + 1.)) * C56 *
                  (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) * -2 *
                  std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / std::pow(C56, 2) +
              (C58 + std::sqrt(C62 + 1.)) *
                  std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                           2))) /
                std::pow((C58 + std::sqrt(C62 + 1.)) * C56 *
                             std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 *
                                              std::sqrt(C49) / C56 +
                                          1.,
                                      2),
                         2) -
            ((std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                       2) *
                  C72 * std::pow(rhoA, C52) *
                  (-0.0042 * std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) /
                       std::pow(C56, 2) -
                   (-0.0042 * C62 * 0.0252 * std::sqrt(C49) * 2 * std::sqrt(C62 + 1.) *
                    (C58 + std::sqrt(C62 + 1.))) /
                       std::pow(2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56, 2)) -
              C72 * std::pow(rhoA, C52) *
                  (0.0042 *
                       (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) -
                   0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                       (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) *
                  (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) * -2 *
                  std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) /
                  std::pow(C56, 2)) /
                 std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                          4) -
             C52 * std::pow(rhoA, C52 - 1) * -0.0042 * C62 * std::log(C58 + std::sqrt(C62 + 1.)) *
                 0.0252 * std::sqrt(C49) /
                 (std::pow(C56, 2) *
                  std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                           2)))) -
           C70 * std::pow(rhoA, C52) * -0.0042 * C62 * std::log(C58 + std::sqrt(C62 + 1.)) *
               0.0252 * std::sqrt(C49) *
               (std::pow(C56, 2) *
                    (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) *
                    -2 * std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) /
                    std::pow(C56, 2) +
                2 * C56 *
                    std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                 1.,
                             2)) /
               std::pow(std::pow(C56, 2) * std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 *
                                                            std::sqrt(C49) / C56 +
                                                        1.,
                                                    2),
                        2)) *
              C70) -
         C85 * std::pow(rhoA, C52) *
             (0.0042 * (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) -
              0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                  (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) /
             std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                      2)) +
        C83 * std::pow(rhoA, C52) * -0.0042 * C62 * std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 *
            std::sqrt(C49) /
            (std::pow(C56, 2) *
             std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1., 2));

    matrix.upper[1] = 0;

    const double C77 = 1;
    const double C78 = C77 / (2 * std::sqrt(C49) * C56);
    const double C79 = 2 * C58 * C78;
    const double C86 = (-C70 * C77 * 2 * std::sqrt(C49)) / std::pow(2 * std::sqrt(C49) * C56, 2);
    const double C87 = 2 * C78 * C71 + 2 * C58 * C86;
    matrix.upper[2] =
        ((((((C78 * C52 * std::pow(rhoA, C52 - 1) * 0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                  ((C58 + std::sqrt(C62 + 1.)) * C56 *
                   std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                1.,
                            2)) -
              C79 * C52 * std::pow(rhoA, C52 - 1) *
                  (0.0042 *
                       (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) -
                   0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                       (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) /
                  std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                           2)) +
             C77 * C52 * std::pow(rhoA, C52 - 1) * 0.0042 * C62 * 0.0252 *
                 std::log(C58 + std::sqrt(C62 + 1.)) /
                 (2 * std::sqrt(C49) * C56 *
                  std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                           2))) +
            C86 * std::pow(rhoA, C52) * 0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                ((C58 + std::sqrt(C62 + 1.)) * C56 *
                 std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                          2))) +
           ((((C58 + std::sqrt(C62 + 1.)) * C56 *
                  std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                           2) *
                  C78 * std::pow(rhoA, C52) * 0.00010584 * std::sqrt(C49) -
              C78 * std::pow(rhoA, C52) * 0.0042 * C62 * 0.0252 * std::sqrt(C49) *
                  ((C58 + std::sqrt(C62 + 1.)) * C56 *
                       (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) *
                       0.0504 * std::sqrt(C49) /
                       (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56) +
                   std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                1.,
                            2) *
                       C56 / (2 * std::sqrt(C62 + 1.)))) /
                 std::pow((C58 + std::sqrt(C62 + 1.)) * C56 *
                              std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 *
                                               std::sqrt(C49) / C56 +
                                           1.,
                                       2),
                          2) -
             (std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                       2) *
                  C79 * std::pow(rhoA, C52) *
                  (0.00010584 * std::sqrt(C49) /
                       (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56) -
                   (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56 * 0.00010584 *
                        std::sqrt(C49) -
                    0.0042 * C62 * 0.0252 * std::sqrt(C49) * C56 *
                        (std::sqrt(C62 + 1.) + (C58 + std::sqrt(C62 + 1.))) / std::sqrt(C62 + 1.)) /
                       std::pow(2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56, 2)) -
              C79 * std::pow(rhoA, C52) *
                  (0.0042 *
                       (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) -
                   0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                       (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) *
                  (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) *
                  0.0504 * std::sqrt(C49) /
                  (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) /
                 std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                          4)) +
            (2 * std::sqrt(C49) * C56 *
                 std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                          2) *
                 C77 * std::pow(rhoA, C52) * 0.0042 *
                 (0.0252 * C62 / (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.))) +
                  0.0252 * std::log(C58 + std::sqrt(C62 + 1.))) -
             C77 * std::pow(rhoA, C52) * 0.0042 * C62 * 0.0252 *
                 std::log(C58 + std::sqrt(C62 + 1.)) * std::sqrt(C49) * C56 *
                 (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) *
                 0.0504 * std::sqrt(C49) /
                 (std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) /
                std::pow(2 * std::sqrt(C49) * C56 *
                             std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 *
                                              std::sqrt(C49) / C56 +
                                          1.,
                                      2),
                         2)) *
               C72) +
          (((-C78 * std::pow(rhoA, C52) * 0.0042 * C62 * 0.0252 * std::sqrt(C49) *
             ((C58 + std::sqrt(C62 + 1.)) * C56 *
                  (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) *
                  0.0504 * std::sqrt(C49) / ((C58 + std::sqrt(C62 + 1.)) * C56) +
              C56 *
                  std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                           2))) /
                std::pow((C58 + std::sqrt(C62 + 1.)) * C56 *
                             std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 *
                                              std::sqrt(C49) / C56 +
                                          1.,
                                      2),
                         2) -
            (std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1., 2) *
                 C79 * std::pow(rhoA, C52) *
                 (0.00010584 * std::sqrt(C49) / ((C58 + std::sqrt(C62 + 1.)) * C56) -
                  (-0.0042 * C62 * 0.0252 * std::sqrt(C49) * 2 * std::sqrt(C62 + 1.) * C56) /
                      std::pow(2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56, 2)) -
             C79 * std::pow(rhoA, C52) *
                 (0.0042 *
                      (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) -
                  0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                      (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) *
                 (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) *
                 0.0504 * std::sqrt(C49) / ((C58 + std::sqrt(C62 + 1.)) * C56)) /
                std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                         4)) +
           (2 * std::sqrt(C49) * C56 *
                std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                         2) *
                C77 * std::pow(rhoA, C52) * 0.00010584 * C62 / (C58 + std::sqrt(C62 + 1.)) -
            C77 * std::pow(rhoA, C52) * 0.0042 * C62 * 0.0252 *
                std::log(C58 + std::sqrt(C62 + 1.)) * 2 * std::sqrt(C49) * C56 *
                (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) *
                0.0504 * std::sqrt(C49) / ((C58 + std::sqrt(C62 + 1.)) * C56)) /
               std::pow(
                   2 * std::sqrt(C49) * C56 *
                       std::pow(
                           std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                           2),
                   2)) *
              C71) +
         (((-C78 * std::pow(rhoA, C52) * 0.0042 * C62 * 0.0252 * std::sqrt(C49) *
            ((C58 + std::sqrt(C62 + 1.)) * C56 *
                 (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) * -2 *
                 std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / std::pow(C56, 2) +
             (C58 + std::sqrt(C62 + 1.)) *
                 std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                          2))) /
               std::pow(
                   (C58 + std::sqrt(C62 + 1.)) * C56 *
                       std::pow(
                           std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                           2),
                   2) -
           (std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1., 2) *
                C79 * std::pow(rhoA, C52) *
                (-0.0042 * std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) /
                     std::pow(C56, 2) -
                 (-0.0042 * C62 * 0.0252 * std::sqrt(C49) * 2 * std::sqrt(C62 + 1.) *
                  (C58 + std::sqrt(C62 + 1.))) /
                     std::pow(2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56, 2)) -
            C79 * std::pow(rhoA, C52) *
                (0.0042 *
                     (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) -
                 0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                     (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) *
                (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) * -2 *
                std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / std::pow(C56, 2)) /
               std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.,
                        4)) -
          C77 * std::pow(rhoA, C52) * 0.0042 * C62 * 0.0252 * std::log(C58 + std::sqrt(C62 + 1.)) *
              2 *
              (std::sqrt(C49) * C56 *
                   (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) * -2 *
                   std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) /
                   std::pow(C56, 2) +
               std::sqrt(C49) *
                   std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                1.,
                            2)) /
              std::pow(
                  2 * std::sqrt(C49) * C56 *
                      std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 +
                                   1.,
                               2),
                  2)) *
             C70) -
        C87 * std::pow(rhoA, C52) *
            (0.0042 * (std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1.) -
             0.0042 * C62 * 0.0252 * std::sqrt(C49) /
                 (2 * std::sqrt(C62 + 1.) * (C58 + std::sqrt(C62 + 1.)) * C56)) /
            std::pow(std::log(C58 + std::sqrt(C62 + 1.)) * 0.0252 * std::sqrt(C49) / C56 + 1., 2);

    matrix.upper[3] = 0;

    matrix.upper[4] = 0;

    const double C48 = rhoB + 1e-16;
    const double C50 = sigmaBb + SigmaGuard;
    const double C55 = std::sqrt(C50);
    const double C57 = std::pow(C48, C52);
    const double C59 = C55 / C57;
    const double C63 = std::pow(C59, 2);
    const double C73 = 1;
    const double C74 = C52 * std::pow(C48, C52 - 1) * C73;
    const double C75 = (-C74 * std::sqrt(C50)) / std::pow(C57, 2);
    const double C76 = 2 * C59 * C75;
    const double C88 = C52 * (C52 - 1) * std::pow(C48, C52 - 2) * std::pow(C73, 2);
    const double C89 = C74 * C74 * std::sqrt(C50) * 2 * C57 / std::pow(C57, 4) -
                       C88 * std::sqrt(C50) / std::pow(C57, 2);
    const double C90 = 2 * std::pow(C75, 2) + 2 * C59 * C89;
    matrix.upper[5] =
        (((((((C75 * C52 * std::pow(rhoB, C52 - 1) * 0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                   ((C59 + std::sqrt(C63 + 1.)) * C57 *
                    std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                 1.,
                             2)) -
               (C76 * C52 * std::pow(rhoB, C52 - 1) *
                    (0.0042 * (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                               1.) -
                     0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                         (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) /
                    std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                 1.,
                             2) +
                C52 * (C52 - 1) * std::pow(rhoB, C52 - 2) *
                    (C64 +
                     0.0042 * C63 /
                         (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                          1.)))) +
              C74 * C52 * std::pow(rhoB, C52 - 1) * -0.0042 * C63 *
                  std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) /
                  (std::pow(C57, 2) *
                   std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                1.,
                            2))) +
             C89 * std::pow(rhoB, C52) * 0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                 ((C59 + std::sqrt(C63 + 1.)) * C57 *
                  std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                           2))) +
            ((((C59 + std::sqrt(C63 + 1.)) * C57 *
                   std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                1.,
                            2) *
                   C75 * std::pow(rhoB, C52) * 0.00010584 * std::sqrt(C50) -
               C75 * std::pow(rhoB, C52) * 0.0042 * C63 * 0.0252 * std::sqrt(C50) *
                   ((C59 + std::sqrt(C63 + 1.)) * C57 *
                        (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) *
                        0.0504 * std::sqrt(C50) /
                        (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57) +
                    std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                 1.,
                             2) *
                        C57 / (2 * std::sqrt(C63 + 1.)))) /
                  std::pow((C59 + std::sqrt(C63 + 1.)) * C57 *
                               std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 *
                                                std::sqrt(C50) / C57 +
                                            1.,
                                        2),
                           2) -
              ((std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                         2) *
                    C76 * std::pow(rhoB, C52) *
                    (0.00010584 * std::sqrt(C50) /
                         (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57) -
                     (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57 * 0.00010584 *
                          std::sqrt(C50) -
                      0.0042 * C63 * 0.0252 * std::sqrt(C50) * C57 *
                          (std::sqrt(C63 + 1.) + (C59 + std::sqrt(C63 + 1.))) /
                          std::sqrt(C63 + 1.)) /
                         std::pow(2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57, 2)) -
                C76 * std::pow(rhoB, C52) *
                    (0.0042 * (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                               1.) -
                     0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                         (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) *
                    (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) *
                    0.0504 * std::sqrt(C50) /
                    (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) /
                   std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                1.,
                            4) +
               C52 * std::pow(rhoB, C52 - 1) *
                   (0.0042 *
                        (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) -
                    0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                        (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) /
                   std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                1.,
                            2))) +
             (std::pow(C57, 2) *
                  std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                           2) *
                  C74 * std::pow(rhoB, C52) * -0.0042 *
                  (C63 * 0.0252 * std::sqrt(C50) /
                       (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.))) +
                   std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50)) -
              C74 * std::pow(rhoB, C52) * -0.0042 * C63 * std::log(C59 + std::sqrt(C63 + 1.)) *
                  0.0252 * std::sqrt(C50) * std::pow(C57, 2) *
                  (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) *
                  0.0504 * std::sqrt(C50) /
                  (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) /
                 std::pow(std::pow(C57, 2) * std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 *
                                                              std::sqrt(C50) / C57 +
                                                          1.,
                                                      2),
                          2)) *
                C76) +
           (((-C75 * std::pow(rhoB, C52) * 0.0042 * C63 * 0.0252 * std::sqrt(C50) *
              ((C59 + std::sqrt(C63 + 1.)) * C57 *
                   (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) *
                   0.0504 * std::sqrt(C50) / ((C59 + std::sqrt(C63 + 1.)) * C57) +
               C57 * std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                  1.,
                              2))) /
                 std::pow((C59 + std::sqrt(C63 + 1.)) * C57 *
                              std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 *
                                               std::sqrt(C50) / C57 +
                                           1.,
                                       2),
                          2) -
             ((std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                        2) *
                   C76 * std::pow(rhoB, C52) *
                   (0.00010584 * std::sqrt(C50) / ((C59 + std::sqrt(C63 + 1.)) * C57) -
                    (-0.0042 * C63 * 0.0252 * std::sqrt(C50) * 2 * std::sqrt(C63 + 1.) * C57) /
                        std::pow(2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57, 2)) -
               C76 * std::pow(rhoB, C52) *
                   (0.0042 *
                        (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) -
                    0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                        (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) *
                   (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) *
                   0.0504 * std::sqrt(C50) / ((C59 + std::sqrt(C63 + 1.)) * C57)) /
                  std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                           4) -
              C52 * std::pow(rhoB, C52 - 1) * 0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                  ((C59 + std::sqrt(C63 + 1.)) * C57 *
                   std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                1.,
                            2)))) +
            (std::pow(C57, 2) *
                 std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                          2) *
                 C74 * std::pow(rhoB, C52) * -0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                 (C59 + std::sqrt(C63 + 1.)) -
             C74 * std::pow(rhoB, C52) * -0.0042 * C63 * std::log(C59 + std::sqrt(C63 + 1.)) *
                 0.0252 * std::sqrt(C50) * std::pow(C57, 2) *
                 (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) *
                 0.0504 * std::sqrt(C50) / ((C59 + std::sqrt(C63 + 1.)) * C57)) /
                std::pow(std::pow(C57, 2) * std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 *
                                                             std::sqrt(C50) / C57 +
                                                         1.,
                                                     2),
                         2)) *
               C75) +
          (((-C75 * std::pow(rhoB, C52) * 0.0042 * C63 * 0.0252 * std::sqrt(C50) *
             ((C59 + std::sqrt(C63 + 1.)) * C57 *
                  (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) * -2 *
                  std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / std::pow(C57, 2) +
              (C59 + std::sqrt(C63 + 1.)) *
                  std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                           2))) /
                std::pow((C59 + std::sqrt(C63 + 1.)) * C57 *
                             std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 *
                                              std::sqrt(C50) / C57 +
                                          1.,
                                      2),
                         2) -
            ((std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                       2) *
                  C76 * std::pow(rhoB, C52) *
                  (-0.0042 * std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) /
                       std::pow(C57, 2) -
                   (-0.0042 * C63 * 0.0252 * std::sqrt(C50) * 2 * std::sqrt(C63 + 1.) *
                    (C59 + std::sqrt(C63 + 1.))) /
                       std::pow(2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57, 2)) -
              C76 * std::pow(rhoB, C52) *
                  (0.0042 *
                       (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) -
                   0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                       (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) *
                  (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) * -2 *
                  std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) /
                  std::pow(C57, 2)) /
                 std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                          4) -
             C52 * std::pow(rhoB, C52 - 1) * -0.0042 * C63 * std::log(C59 + std::sqrt(C63 + 1.)) *
                 0.0252 * std::sqrt(C50) /
                 (std::pow(C57, 2) *
                  std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                           2)))) -
           C74 * std::pow(rhoB, C52) * -0.0042 * C63 * std::log(C59 + std::sqrt(C63 + 1.)) *
               0.0252 * std::sqrt(C50) *
               (std::pow(C57, 2) *
                    (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) *
                    -2 * std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) /
                    std::pow(C57, 2) +
                2 * C57 *
                    std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                 1.,
                             2)) /
               std::pow(std::pow(C57, 2) * std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 *
                                                            std::sqrt(C50) / C57 +
                                                        1.,
                                                    2),
                        2)) *
              C74) -
         C90 * std::pow(rhoB, C52) *
             (0.0042 * (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) -
              0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                  (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) /
             std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                      2)) +
        C88 * std::pow(rhoB, C52) * -0.0042 * C63 * std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 *
            std::sqrt(C50) /
            (std::pow(C57, 2) *
             std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1., 2));

    matrix.upper[6] = 0;

    matrix.upper[7] = 0;

    const double C80 = 1;
    const double C81 = C80 / (2 * std::sqrt(C50) * C57);
    const double C82 = 2 * C59 * C81;
    const double C91 = (-C74 * C80 * 2 * std::sqrt(C50)) / std::pow(2 * std::sqrt(C50) * C57, 2);
    const double C92 = 2 * C81 * C75 + 2 * C59 * C91;
    matrix.upper[8] =
        ((((((C81 * C52 * std::pow(rhoB, C52 - 1) * 0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                  ((C59 + std::sqrt(C63 + 1.)) * C57 *
                   std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                1.,
                            2)) -
              C82 * C52 * std::pow(rhoB, C52 - 1) *
                  (0.0042 *
                       (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) -
                   0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                       (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) /
                  std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                           2)) +
             C80 * C52 * std::pow(rhoB, C52 - 1) * 0.0042 * C63 * 0.0252 *
                 std::log(C59 + std::sqrt(C63 + 1.)) /
                 (2 * std::sqrt(C50) * C57 *
                  std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                           2))) +
            C91 * std::pow(rhoB, C52) * 0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                ((C59 + std::sqrt(C63 + 1.)) * C57 *
                 std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                          2))) +
           ((((C59 + std::sqrt(C63 + 1.)) * C57 *
                  std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                           2) *
                  C81 * std::pow(rhoB, C52) * 0.00010584 * std::sqrt(C50) -
              C81 * std::pow(rhoB, C52) * 0.0042 * C63 * 0.0252 * std::sqrt(C50) *
                  ((C59 + std::sqrt(C63 + 1.)) * C57 *
                       (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) *
                       0.0504 * std::sqrt(C50) /
                       (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57) +
                   std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                1.,
                            2) *
                       C57 / (2 * std::sqrt(C63 + 1.)))) /
                 std::pow((C59 + std::sqrt(C63 + 1.)) * C57 *
                              std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 *
                                               std::sqrt(C50) / C57 +
                                           1.,
                                       2),
                          2) -
             (std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                       2) *
                  C82 * std::pow(rhoB, C52) *
                  (0.00010584 * std::sqrt(C50) /
                       (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57) -
                   (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57 * 0.00010584 *
                        std::sqrt(C50) -
                    0.0042 * C63 * 0.0252 * std::sqrt(C50) * C57 *
                        (std::sqrt(C63 + 1.) + (C59 + std::sqrt(C63 + 1.))) / std::sqrt(C63 + 1.)) /
                       std::pow(2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57, 2)) -
              C82 * std::pow(rhoB, C52) *
                  (0.0042 *
                       (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) -
                   0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                       (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) *
                  (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) *
                  0.0504 * std::sqrt(C50) /
                  (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) /
                 std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                          4)) +
            (2 * std::sqrt(C50) * C57 *
                 std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                          2) *
                 C80 * std::pow(rhoB, C52) * 0.0042 *
                 (0.0252 * C63 / (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.))) +
                  0.0252 * std::log(C59 + std::sqrt(C63 + 1.))) -
             C80 * std::pow(rhoB, C52) * 0.0042 * C63 * 0.0252 *
                 std::log(C59 + std::sqrt(C63 + 1.)) * std::sqrt(C50) * C57 *
                 (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) *
                 0.0504 * std::sqrt(C50) /
                 (std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) /
                std::pow(2 * std::sqrt(C50) * C57 *
                             std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 *
                                              std::sqrt(C50) / C57 +
                                          1.,
                                      2),
                         2)) *
               C76) +
          (((-C81 * std::pow(rhoB, C52) * 0.0042 * C63 * 0.0252 * std::sqrt(C50) *
             ((C59 + std::sqrt(C63 + 1.)) * C57 *
                  (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) *
                  0.0504 * std::sqrt(C50) / ((C59 + std::sqrt(C63 + 1.)) * C57) +
              C57 *
                  std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                           2))) /
                std::pow((C59 + std::sqrt(C63 + 1.)) * C57 *
                             std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 *
                                              std::sqrt(C50) / C57 +
                                          1.,
                                      2),
                         2) -
            (std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1., 2) *
                 C82 * std::pow(rhoB, C52) *
                 (0.00010584 * std::sqrt(C50) / ((C59 + std::sqrt(C63 + 1.)) * C57) -
                  (-0.0042 * C63 * 0.0252 * std::sqrt(C50) * 2 * std::sqrt(C63 + 1.) * C57) /
                      std::pow(2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57, 2)) -
             C82 * std::pow(rhoB, C52) *
                 (0.0042 *
                      (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) -
                  0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                      (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) *
                 (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) *
                 0.0504 * std::sqrt(C50) / ((C59 + std::sqrt(C63 + 1.)) * C57)) /
                std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                         4)) +
           (2 * std::sqrt(C50) * C57 *
                std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                         2) *
                C80 * std::pow(rhoB, C52) * 0.00010584 * C63 / (C59 + std::sqrt(C63 + 1.)) -
            C80 * std::pow(rhoB, C52) * 0.0042 * C63 * 0.0252 *
                std::log(C59 + std::sqrt(C63 + 1.)) * 2 * std::sqrt(C50) * C57 *
                (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) *
                0.0504 * std::sqrt(C50) / ((C59 + std::sqrt(C63 + 1.)) * C57)) /
               std::pow(
                   2 * std::sqrt(C50) * C57 *
                       std::pow(
                           std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                           2),
                   2)) *
              C75) +
         (((-C81 * std::pow(rhoB, C52) * 0.0042 * C63 * 0.0252 * std::sqrt(C50) *
            ((C59 + std::sqrt(C63 + 1.)) * C57 *
                 (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) * -2 *
                 std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / std::pow(C57, 2) +
             (C59 + std::sqrt(C63 + 1.)) *
                 std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                          2))) /
               std::pow(
                   (C59 + std::sqrt(C63 + 1.)) * C57 *
                       std::pow(
                           std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                           2),
                   2) -
           (std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1., 2) *
                C82 * std::pow(rhoB, C52) *
                (-0.0042 * std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) /
                     std::pow(C57, 2) -
                 (-0.0042 * C63 * 0.0252 * std::sqrt(C50) * 2 * std::sqrt(C63 + 1.) *
                  (C59 + std::sqrt(C63 + 1.))) /
                     std::pow(2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57, 2)) -
            C82 * std::pow(rhoB, C52) *
                (0.0042 *
                     (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) -
                 0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                     (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) *
                (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) * -2 *
                std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / std::pow(C57, 2)) /
               std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.,
                        4)) -
          C80 * std::pow(rhoB, C52) * 0.0042 * C63 * 0.0252 * std::log(C59 + std::sqrt(C63 + 1.)) *
              2 *
              (std::sqrt(C50) * C57 *
                   (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) * -2 *
                   std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) /
                   std::pow(C57, 2) +
               std::sqrt(C50) *
                   std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                1.,
                            2)) /
              std::pow(
                  2 * std::sqrt(C50) * C57 *
                      std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 +
                                   1.,
                               2),
                  2)) *
             C74) -
        C92 * std::pow(rhoB, C52) *
            (0.0042 * (std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1.) -
             0.0042 * C63 * 0.0252 * std::sqrt(C50) /
                 (2 * std::sqrt(C63 + 1.) * (C59 + std::sqrt(C63 + 1.)) * C57)) /
            std::pow(std::log(C59 + std::sqrt(C63 + 1.)) * 0.0252 * std::sqrt(C50) / C57 + 1., 2);

    const double C93 = rhoA + 1e-16;
    const double C94 = 4. / 3.;
    const double C95 = std::sqrt(sigmaAa);
    const double C96 = 0.0252 * C95;
    const double C97 = std::pow(C93, C94);
    const double C98 = C95 / C97;
    const double C99 = excgrid::internal::AsinhOverX(C98);
    const double C100 = std::pow(C98, 2);
    const double C101 = C100 + 1.;
    const double C102 = std::sqrt(C101);
    const double C103 = C98 + C102;
    const double C104 = 1 / C102;
    const double C105 = C99 + C104;
    const double C106 = std::log(C103);
    const double C107 = C106 * C96;
    const double C108 = C107 / C97;
    const double C109 = C108 + 1;
    matrix.upper[9] = std::pow(rhoA, C94) * 0.00010584 *
                      ((C109 * (C100 + 2) * std::pow(C104, 3) + 3 * C109 * C105) -
                       0.0504 * C100 * std::pow(C105, 2)) /
                      (std::pow(C109, 3) * 4 * std::pow(C93, 16. / 3.));

    matrix.upper[10] = 0;

    matrix.upper[11] = 0;

    matrix.upper[12] = 0;

    matrix.upper[13] = 0;

    const double C110 = rhoB + 1e-16;
    const double C111 = 4. / 3.;
    const double C112 = std::sqrt(sigmaBb);
    const double C113 = 0.0252 * C112;
    const double C114 = std::pow(C110, C111);
    const double C115 = C112 / C114;
    const double C116 = excgrid::internal::AsinhOverX(C115);
    const double C117 = std::pow(C115, 2);
    const double C118 = C117 + 1.;
    const double C119 = std::sqrt(C118);
    const double C120 = C115 + C119;
    const double C121 = 1 / C119;
    const double C122 = C116 + C121;
    const double C123 = std::log(C120);
    const double C124 = C123 * C113;
    const double C125 = C124 / C114;
    const double C126 = C125 + 1;
    matrix.upper[14] = std::pow(rhoB, C111) * 0.00010584 *
                       ((C126 * (C117 + 2) * std::pow(C121, 3) + 3 * C126 * C122) -
                        0.0504 * C117 * std::pow(C122, 2)) /
                       (std::pow(C126, 3) * 4 * std::pow(C110, 16. / 3.));
}

} // namespace excgrid
