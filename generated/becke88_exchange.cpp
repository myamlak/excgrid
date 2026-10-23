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

    const double C123 = 4. * Pi;
    const double C124 = rhoA + 1e-16;
    const double C125 = rhoB + 1e-16;
    const double C126 = 1. / 3.;
    const double C127 = 4. / 3.;
    const double C128 = std::sqrt(sigmaAa);
    const double C129 = std::sqrt(sigmaBb);
    const double C130 = 3. / C123;
    const double C131 = std::pow(C124, C127);
    const double C132 = std::pow(C125, C127);
    const double C133 = C128 / C131;
    const double C134 = C129 / C132;
    const double C135 = std::pow(C130, C126);
    const double C136 = 3. * C135;
    const double C137 = std::pow(C133, 2);
    const double C138 = std::pow(C134, 2);
    const double C139 = C136 / 2.;

    result.exc =
        -(std::pow(rhoB, C127) *
              (C139 + 0.0042 * C138 /
                          (std::log(C134 + std::sqrt(C138 + 1.)) * 0.0252 * C129 / C132 + 1.)) +
          std::pow(rhoA, C127) *
              (C139 + 0.0042 * C137 /
                          (std::log(C133 + std::sqrt(C137 + 1.)) * 0.0252 * C128 / C131 + 1.)));

    const double C141 = 1;
    const double C142 = C127 * std::pow(C124, C127 - 1) * C141;
    const double C143 = (-C142 * std::sqrt(sigmaAa)) / std::pow(C131, 2);
    const double C144 = 2 * C133 * C143;

    result.vrhoA =
        (C143 * std::pow(rhoA, C127) * 0.0042 * C137 * 0.0252 * std::sqrt(sigmaAa) /
             ((C133 + std::sqrt(C137 + 1.)) * C131 *
              std::pow(std::log(C133 + std::sqrt(C137 + 1.)) * 0.0252 * std::sqrt(sigmaAa) / C131 +
                           1.,
                       2)) -
         (C144 * std::pow(rhoA, C127) *
              (0.0042 *
                   (std::log(C133 + std::sqrt(C137 + 1.)) * 0.0252 * std::sqrt(sigmaAa) / C131 +
                    1.) -
               0.0042 * C137 * 0.0252 * std::sqrt(sigmaAa) /
                   (2 * std::sqrt(C137 + 1.) * (C133 + std::sqrt(C137 + 1.)) * C131)) /
              std::pow(std::log(C133 + std::sqrt(C137 + 1.)) * 0.0252 * std::sqrt(sigmaAa) / C131 +
                           1.,
                       2) +
          C127 * std::pow(rhoA, C127 - 1) *
              (C139 +
               0.0042 * C137 /
                   (std::log(C133 + std::sqrt(C137 + 1.)) * 0.0252 * std::sqrt(sigmaAa) / C131 +
                    1.)))) +
        C142 * std::pow(rhoA, C127) * -0.0042 * C137 * std::log(C133 + std::sqrt(C137 + 1.)) *
            0.0252 * std::sqrt(sigmaAa) /
            (std::pow(C131, 2) *
             std::pow(std::log(C133 + std::sqrt(C137 + 1.)) * 0.0252 * std::sqrt(sigmaAa) / C131 +
                          1.,
                      2));

    const double C145 = 1;
    const double C146 = C127 * std::pow(C125, C127 - 1) * C145;
    const double C147 = (-C146 * std::sqrt(sigmaBb)) / std::pow(C132, 2);
    const double C148 = 2 * C134 * C147;

    result.vrhoB =
        (C147 * std::pow(rhoB, C127) * 0.0042 * C138 * 0.0252 * std::sqrt(sigmaBb) /
             ((C134 + std::sqrt(C138 + 1.)) * C132 *
              std::pow(std::log(C134 + std::sqrt(C138 + 1.)) * 0.0252 * std::sqrt(sigmaBb) / C132 +
                           1.,
                       2)) -
         (C148 * std::pow(rhoB, C127) *
              (0.0042 *
                   (std::log(C134 + std::sqrt(C138 + 1.)) * 0.0252 * std::sqrt(sigmaBb) / C132 +
                    1.) -
               0.0042 * C138 * 0.0252 * std::sqrt(sigmaBb) /
                   (2 * std::sqrt(C138 + 1.) * (C134 + std::sqrt(C138 + 1.)) * C132)) /
              std::pow(std::log(C134 + std::sqrt(C138 + 1.)) * 0.0252 * std::sqrt(sigmaBb) / C132 +
                           1.,
                       2) +
          C127 * std::pow(rhoB, C127 - 1) *
              (C139 +
               0.0042 * C138 /
                   (std::log(C134 + std::sqrt(C138 + 1.)) * 0.0252 * std::sqrt(sigmaBb) / C132 +
                    1.)))) +
        C146 * std::pow(rhoB, C127) * -0.0042 * C138 * std::log(C134 + std::sqrt(C138 + 1.)) *
            0.0252 * std::sqrt(sigmaBb) /
            (std::pow(C132, 2) *
             std::pow(std::log(C134 + std::sqrt(C138 + 1.)) * 0.0252 * std::sqrt(sigmaBb) / C132 +
                          1.,
                      2));

    const double C149 = 1 / (2 * std::sqrt(sigmaAa + SigmaGuard) * C131);
    const double C150 = 2 * C133 * C149;

    result.vsigmaAa =
        (std::pow(rhoA, C127) * 0.0042 * C137 * 0.0252 * std::log(C133 + std::sqrt(C137 + 1.)) /
             (2 * std::sqrt(sigmaAa + SigmaGuard) * C131 *
              std::pow(std::log(C133 + std::sqrt(C137 + 1.)) * 0.0252 *
                               std::sqrt(sigmaAa + SigmaGuard) / C131 +
                           1.,
                       2)) -
         C150 * std::pow(rhoA, C127) *
             (0.0042 * (std::log(C133 + std::sqrt(C137 + 1.)) * 0.0252 *
                            std::sqrt(sigmaAa + SigmaGuard) / C131 +
                        1.) -
              0.0042 * C137 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                  (2 * std::sqrt(C137 + 1.) * (C133 + std::sqrt(C137 + 1.)) * C131)) /
             std::pow(std::log(C133 + std::sqrt(C137 + 1.)) * 0.0252 *
                              std::sqrt(sigmaAa + SigmaGuard) / C131 +
                          1.,
                      2)) +
        C149 * std::pow(rhoA, C127) * 0.0042 * C137 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
            ((C133 + std::sqrt(C137 + 1.)) * C131 *
             std::pow(std::log(C133 + std::sqrt(C137 + 1.)) * 0.0252 *
                              std::sqrt(sigmaAa + SigmaGuard) / C131 +
                          1.,
                      2));

    result.vsigmaAb = 0;

    const double C151 = 1 / (2 * std::sqrt(sigmaBb + SigmaGuard) * C132);
    const double C152 = 2 * C134 * C151;

    result.vsigmaBb =
        (std::pow(rhoB, C127) * 0.0042 * C138 * 0.0252 * std::log(C134 + std::sqrt(C138 + 1.)) /
             (2 * std::sqrt(sigmaBb + SigmaGuard) * C132 *
              std::pow(std::log(C134 + std::sqrt(C138 + 1.)) * 0.0252 *
                               std::sqrt(sigmaBb + SigmaGuard) / C132 +
                           1.,
                       2)) -
         C152 * std::pow(rhoB, C127) *
             (0.0042 * (std::log(C134 + std::sqrt(C138 + 1.)) * 0.0252 *
                            std::sqrt(sigmaBb + SigmaGuard) / C132 +
                        1.) -
              0.0042 * C138 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                  (2 * std::sqrt(C138 + 1.) * (C134 + std::sqrt(C138 + 1.)) * C132)) /
             std::pow(std::log(C134 + std::sqrt(C138 + 1.)) * 0.0252 *
                              std::sqrt(sigmaBb + SigmaGuard) / C132 +
                          1.,
                      2)) +
        C151 * std::pow(rhoB, C127) * 0.0042 * C138 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
            ((C134 + std::sqrt(C138 + 1.)) * C132 *
             std::pow(std::log(C134 + std::sqrt(C138 + 1.)) * 0.0252 *
                              std::sqrt(sigmaBb + SigmaGuard) / C132 +
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
    const double C49 = 1. / 3.;
    const double C50 = 4. / 3.;
    const double C51 = std::sqrt(sigmaAa);
    const double C53 = 3. / C46;
    const double C54 = std::pow(C47, C50);
    const double C56 = C51 / C54;
    const double C58 = std::pow(C53, C49);
    const double C59 = 3. * C58;
    const double C60 = std::pow(C56, 2);
    const double C62 = C59 / 2.;
    const double C67 = 1;
    const double C68 = C50 * std::pow(C47, C50 - 1) * C67;
    const double C69 = (-C68 * std::sqrt(sigmaAa + SigmaGuard)) / std::pow(C54, 2);
    const double C70 = 2 * C56 * C69;
    const double C79 = C50 * (C50 - 1) * std::pow(C47, C50 - 2) * std::pow(C67, 2);
    const double C80 = C68 * C68 * std::sqrt(sigmaAa + SigmaGuard) * 2 * C54 / std::pow(C54, 4) -
                       C79 * std::sqrt(sigmaAa + SigmaGuard) / std::pow(C54, 2);
    const double C81 = 2 * std::pow(C69, 2) + 2 * C56 * C80;
    matrix.upper[0] =
        (((((((C69 * C50 * std::pow(rhoA, C50 - 1) * 0.0042 * C60 * 0.0252 *
                   std::sqrt(sigmaAa + SigmaGuard) /
                   ((C56 + std::sqrt(C60 + 1.)) * C54 *
                    std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                     std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                 1.,
                             2)) -
               (C70 * C50 * std::pow(rhoA, C50 - 1) *
                    (0.0042 * (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaAa + SigmaGuard) / C54 +
                               1.) -
                     0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                         (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) /
                    std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                     std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                 1.,
                             2) +
                C50 * (C50 - 1) * std::pow(rhoA, C50 - 2) *
                    (C62 + 0.0042 * C60 /
                               (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                1.)))) +
              C68 * C50 * std::pow(rhoA, C50 - 1) * -0.0042 * C60 *
                  std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                  (std::pow(C54, 2) * std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                                       std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                                   1.,
                                               2))) +
             C80 * std::pow(rhoA, C50) * 0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                 ((C56 + std::sqrt(C60 + 1.)) * C54 *
                  std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaAa + SigmaGuard) / C54 +
                               1.,
                           2))) +
            ((((C56 + std::sqrt(C60 + 1.)) * C54 *
                   std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                1.,
                            2) *
                   C69 * std::pow(rhoA, C50) * 0.00010584 * std::sqrt(sigmaAa + SigmaGuard) -
               C69 * std::pow(rhoA, C50) * 0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) *
                   ((C56 + std::sqrt(C60 + 1.)) * C54 *
                        (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                             std::sqrt(sigmaAa + SigmaGuard) / C54 +
                         1.) *
                        0.0504 * std::sqrt(sigmaAa + SigmaGuard) /
                        (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54) +
                    std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                     std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                 1.,
                             2) *
                        C54 / (2 * std::sqrt(C60 + 1.)))) /
                  std::pow((C56 + std::sqrt(C60 + 1.)) * C54 *
                               std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                                std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                            1.,
                                        2),
                           2) -
              ((std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                 std::sqrt(sigmaAa + SigmaGuard) / C54 +
                             1.,
                         2) *
                    C70 * std::pow(rhoA, C50) *
                    (0.00010584 * std::sqrt(sigmaAa + SigmaGuard) /
                         (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54) -
                     (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54 * 0.00010584 *
                          std::sqrt(sigmaAa + SigmaGuard) -
                      0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) * C54 *
                          (std::sqrt(C60 + 1.) + (C56 + std::sqrt(C60 + 1.))) /
                          std::sqrt(C60 + 1.)) /
                         std::pow(2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54, 2)) -
                C70 * std::pow(rhoA, C50) *
                    (0.0042 * (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaAa + SigmaGuard) / C54 +
                               1.) -
                     0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                         (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) *
                    (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                         std::sqrt(sigmaAa + SigmaGuard) / C54 +
                     1.) *
                    0.0504 * std::sqrt(sigmaAa + SigmaGuard) /
                    (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) /
                   std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                1.,
                            4) +
               C50 * std::pow(rhoA, C50 - 1) *
                   (0.0042 * (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                  std::sqrt(sigmaAa + SigmaGuard) / C54 +
                              1.) -
                    0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                        (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) /
                   std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                1.,
                            2))) +
             (std::pow(C54, 2) *
                  std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaAa + SigmaGuard) / C54 +
                               1.,
                           2) *
                  C68 * std::pow(rhoA, C50) * -0.0042 *
                  (C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                       (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.))) +
                   std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard)) -
              C68 * std::pow(rhoA, C50) * -0.0042 * C60 * std::log(C56 + std::sqrt(C60 + 1.)) *
                  0.0252 * std::sqrt(sigmaAa + SigmaGuard) * std::pow(C54, 2) *
                  (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                       C54 +
                   1.) *
                  0.0504 * std::sqrt(sigmaAa + SigmaGuard) /
                  (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) /
                 std::pow(std::pow(C54, 2) *
                              std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                               std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                           1.,
                                       2),
                          2)) *
                C70) +
           (((-C69 * std::pow(rhoA, C50) * 0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) *
              ((C56 + std::sqrt(C60 + 1.)) * C54 *
                   (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                        C54 +
                    1.) *
                   0.0504 * std::sqrt(sigmaAa + SigmaGuard) / ((C56 + std::sqrt(C60 + 1.)) * C54) +
               C54 * std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                      std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                  1.,
                              2))) /
                 std::pow((C56 + std::sqrt(C60 + 1.)) * C54 *
                              std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                               std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                           1.,
                                       2),
                          2) -
             ((std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                std::sqrt(sigmaAa + SigmaGuard) / C54 +
                            1.,
                        2) *
                   C70 * std::pow(rhoA, C50) *
                   (0.00010584 * std::sqrt(sigmaAa + SigmaGuard) /
                        ((C56 + std::sqrt(C60 + 1.)) * C54) -
                    (-0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) * 2 *
                     std::sqrt(C60 + 1.) * C54) /
                        std::pow(2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54, 2)) -
               C70 * std::pow(rhoA, C50) *
                   (0.0042 * (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                  std::sqrt(sigmaAa + SigmaGuard) / C54 +
                              1.) -
                    0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                        (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) *
                   (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                        C54 +
                    1.) *
                   0.0504 * std::sqrt(sigmaAa + SigmaGuard) / ((C56 + std::sqrt(C60 + 1.)) * C54)) /
                  std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaAa + SigmaGuard) / C54 +
                               1.,
                           4) -
              C50 * std::pow(rhoA, C50 - 1) * 0.0042 * C60 * 0.0252 *
                  std::sqrt(sigmaAa + SigmaGuard) /
                  ((C56 + std::sqrt(C60 + 1.)) * C54 *
                   std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                1.,
                            2)))) +
            (std::pow(C54, 2) *
                 std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                  std::sqrt(sigmaAa + SigmaGuard) / C54 +
                              1.,
                          2) *
                 C68 * std::pow(rhoA, C50) * -0.0042 * C60 * 0.0252 *
                 std::sqrt(sigmaAa + SigmaGuard) / (C56 + std::sqrt(C60 + 1.)) -
             C68 * std::pow(rhoA, C50) * -0.0042 * C60 * std::log(C56 + std::sqrt(C60 + 1.)) *
                 0.0252 * std::sqrt(sigmaAa + SigmaGuard) * std::pow(C54, 2) *
                 (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                      C54 +
                  1.) *
                 0.0504 * std::sqrt(sigmaAa + SigmaGuard) / ((C56 + std::sqrt(C60 + 1.)) * C54)) /
                std::pow(std::pow(C54, 2) * std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                                             std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                                         1.,
                                                     2),
                         2)) *
               C69) +
          (((-C69 * std::pow(rhoA, C50) * 0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) *
             ((C56 + std::sqrt(C60 + 1.)) * C54 *
                  (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                       C54 +
                   1.) *
                  -2 * std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                  std::sqrt(sigmaAa + SigmaGuard) / std::pow(C54, 2) +
              (C56 + std::sqrt(C60 + 1.)) * std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                                             std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                                         1.,
                                                     2))) /
                std::pow((C56 + std::sqrt(C60 + 1.)) * C54 *
                             std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                              std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                          1.,
                                      2),
                         2) -
            ((std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                               std::sqrt(sigmaAa + SigmaGuard) / C54 +
                           1.,
                       2) *
                  C70 * std::pow(rhoA, C50) *
                  (-0.0042 * std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                       std::sqrt(sigmaAa + SigmaGuard) / std::pow(C54, 2) -
                   (-0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) * 2 *
                    std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.))) /
                       std::pow(2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54, 2)) -
              C70 * std::pow(rhoA, C50) *
                  (0.0042 * (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                 std::sqrt(sigmaAa + SigmaGuard) / C54 +
                             1.) -
                   0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                       (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) *
                  (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                       C54 +
                   1.) *
                  -2 * std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                  std::sqrt(sigmaAa + SigmaGuard) / std::pow(C54, 2)) /
                 std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                  std::sqrt(sigmaAa + SigmaGuard) / C54 +
                              1.,
                          4) -
             C50 * std::pow(rhoA, C50 - 1) * -0.0042 * C60 * std::log(C56 + std::sqrt(C60 + 1.)) *
                 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                 (std::pow(C54, 2) * std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                                      std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                                  1.,
                                              2)))) -
           C68 * std::pow(rhoA, C50) * -0.0042 * C60 * std::log(C56 + std::sqrt(C60 + 1.)) *
               0.0252 * std::sqrt(sigmaAa + SigmaGuard) *
               (std::pow(C54, 2) *
                    (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                         std::sqrt(sigmaAa + SigmaGuard) / C54 +
                     1.) *
                    -2 * std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                    std::sqrt(sigmaAa + SigmaGuard) / std::pow(C54, 2) +
                2 * C54 *
                    std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                     std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                 1.,
                             2)) /
               std::pow(std::pow(C54, 2) * std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                                            std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                                        1.,
                                                    2),
                        2)) *
              C68) -
         C81 * std::pow(rhoA, C50) *
             (0.0042 * (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                            std::sqrt(sigmaAa + SigmaGuard) / C54 +
                        1.) -
              0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                  (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) /
             std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                              std::sqrt(sigmaAa + SigmaGuard) / C54 +
                          1.,
                      2)) +
        C79 * std::pow(rhoA, C50) * -0.0042 * C60 * std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
            std::sqrt(sigmaAa + SigmaGuard) /
            (std::pow(C54, 2) * std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                                 std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                             1.,
                                         2));

    matrix.upper[1] = 0;

    const double C75 = 1 / (2 * std::sqrt(sigmaAa + SigmaGuard) * C54);
    const double C76 = 2 * C56 * C75;
    const double C82 = (-C68 * 2 * std::sqrt(sigmaAa + SigmaGuard)) /
                       std::pow(2 * std::sqrt(sigmaAa + SigmaGuard) * C54, 2);
    const double C83 = 2 * C75 * C69 + 2 * C56 * C82;
    matrix.upper[2] =
        ((((((C50 * std::pow(rhoA, C50 - 1) * 0.0042 * C60 * 0.0252 *
                  std::log(C56 + std::sqrt(C60 + 1.)) /
                  (2 * std::sqrt(sigmaAa + SigmaGuard) * C54 *
                   std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                1.,
                            2)) -
              C76 * C50 * std::pow(rhoA, C50 - 1) *
                  (0.0042 * (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                 std::sqrt(sigmaAa + SigmaGuard) / C54 +
                             1.) -
                   0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                       (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) /
                  std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaAa + SigmaGuard) / C54 +
                               1.,
                           2)) +
             C75 * C50 * std::pow(rhoA, C50 - 1) * 0.0042 * C60 * 0.0252 *
                 std::sqrt(sigmaAa + SigmaGuard) /
                 ((C56 + std::sqrt(C60 + 1.)) * C54 *
                  std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaAa + SigmaGuard) / C54 +
                               1.,
                           2))) +
            (((2 * std::sqrt(sigmaAa + SigmaGuard) * C54 *
                   std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                1.,
                            2) *
                   std::pow(rhoA, C50) * 0.0042 *
                   (0.0252 * C60 / (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.))) +
                    0.0252 * std::log(C56 + std::sqrt(C60 + 1.))) -
               std::pow(rhoA, C50) * 0.0042 * C60 * 0.0252 * std::log(C56 + std::sqrt(C60 + 1.)) *
                   std::sqrt(sigmaAa + SigmaGuard) * C54 *
                   (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                        C54 +
                    1.) *
                   0.0504 * std::sqrt(sigmaAa + SigmaGuard) /
                   (std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) /
                  std::pow(2 * std::sqrt(sigmaAa + SigmaGuard) * C54 *
                               std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                                std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                            1.,
                                        2),
                           2) -
              (std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                std::sqrt(sigmaAa + SigmaGuard) / C54 +
                            1.,
                        2) *
                   C76 * std::pow(rhoA, C50) *
                   (0.00010584 * std::sqrt(sigmaAa + SigmaGuard) /
                        (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54) -
                    (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54 * 0.00010584 *
                         std::sqrt(sigmaAa + SigmaGuard) -
                     0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) * C54 *
                         (std::sqrt(C60 + 1.) + (C56 + std::sqrt(C60 + 1.))) /
                         std::sqrt(C60 + 1.)) /
                        std::pow(2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54, 2)) -
               C76 * std::pow(rhoA, C50) *
                   (0.0042 * (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                  std::sqrt(sigmaAa + SigmaGuard) / C54 +
                              1.) -
                    0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                        (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) *
                   (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                        C54 +
                    1.) *
                   0.0504 * std::sqrt(sigmaAa + SigmaGuard) /
                   (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) /
                  std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaAa + SigmaGuard) / C54 +
                               1.,
                           4)) +
             ((C56 + std::sqrt(C60 + 1.)) * C54 *
                  std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaAa + SigmaGuard) / C54 +
                               1.,
                           2) *
                  C75 * std::pow(rhoA, C50) * 0.00010584 * std::sqrt(sigmaAa + SigmaGuard) -
              C75 * std::pow(rhoA, C50) * 0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) *
                  ((C56 + std::sqrt(C60 + 1.)) * C54 *
                       (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                            std::sqrt(sigmaAa + SigmaGuard) / C54 +
                        1.) *
                       0.0504 * std::sqrt(sigmaAa + SigmaGuard) /
                       (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54) +
                   std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                1.,
                            2) *
                       C54 / (2 * std::sqrt(C60 + 1.)))) /
                 std::pow((C56 + std::sqrt(C60 + 1.)) * C54 *
                              std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                               std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                           1.,
                                       2),
                          2)) *
                C70) +
           (((2 * std::sqrt(sigmaAa + SigmaGuard) * C54 *
                  std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaAa + SigmaGuard) / C54 +
                               1.,
                           2) *
                  std::pow(rhoA, C50) * 0.00010584 * C60 / (C56 + std::sqrt(C60 + 1.)) -
              std::pow(rhoA, C50) * 0.0042 * C60 * 0.0252 * std::log(C56 + std::sqrt(C60 + 1.)) *
                  2 * std::sqrt(sigmaAa + SigmaGuard) * C54 *
                  (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                       C54 +
                   1.) *
                  0.0504 * std::sqrt(sigmaAa + SigmaGuard) / ((C56 + std::sqrt(C60 + 1.)) * C54)) /
                 std::pow(2 * std::sqrt(sigmaAa + SigmaGuard) * C54 *
                              std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                               std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                           1.,
                                       2),
                          2) -
             (std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                               std::sqrt(sigmaAa + SigmaGuard) / C54 +
                           1.,
                       2) *
                  C76 * std::pow(rhoA, C50) *
                  (0.00010584 * std::sqrt(sigmaAa + SigmaGuard) /
                       ((C56 + std::sqrt(C60 + 1.)) * C54) -
                   (-0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) * 2 *
                    std::sqrt(C60 + 1.) * C54) /
                       std::pow(2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54, 2)) -
              C76 * std::pow(rhoA, C50) *
                  (0.0042 * (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                 std::sqrt(sigmaAa + SigmaGuard) / C54 +
                             1.) -
                   0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                       (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) *
                  (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                       C54 +
                   1.) *
                  0.0504 * std::sqrt(sigmaAa + SigmaGuard) / ((C56 + std::sqrt(C60 + 1.)) * C54)) /
                 std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                  std::sqrt(sigmaAa + SigmaGuard) / C54 +
                              1.,
                          4)) -
            C75 * std::pow(rhoA, C50) * 0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) *
                ((C56 + std::sqrt(C60 + 1.)) * C54 *
                     (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                          std::sqrt(sigmaAa + SigmaGuard) / C54 +
                      1.) *
                     0.0504 * std::sqrt(sigmaAa + SigmaGuard) /
                     ((C56 + std::sqrt(C60 + 1.)) * C54) +
                 C54 * std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                        std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                    1.,
                                2)) /
                std::pow((C56 + std::sqrt(C60 + 1.)) * C54 *
                             std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                              std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                          1.,
                                      2),
                         2)) *
               C69) +
          (((-std::pow(rhoA, C50) * 0.0042 * C60 * 0.0252 * std::log(C56 + std::sqrt(C60 + 1.)) *
             2 *
             (std::sqrt(sigmaAa + SigmaGuard) * C54 *
                  (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                       C54 +
                   1.) *
                  -2 * std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                  std::sqrt(sigmaAa + SigmaGuard) / std::pow(C54, 2) +
              std::sqrt(sigmaAa + SigmaGuard) *
                  std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaAa + SigmaGuard) / C54 +
                               1.,
                           2))) /
                std::pow(2 * std::sqrt(sigmaAa + SigmaGuard) * C54 *
                             std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                              std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                          1.,
                                      2),
                         2) -
            (std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                              std::sqrt(sigmaAa + SigmaGuard) / C54 +
                          1.,
                      2) *
                 C76 * std::pow(rhoA, C50) *
                 (-0.0042 * std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                      std::sqrt(sigmaAa + SigmaGuard) / std::pow(C54, 2) -
                  (-0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) * 2 *
                   std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.))) /
                      std::pow(2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54, 2)) -
             C76 * std::pow(rhoA, C50) *
                 (0.0042 * (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                std::sqrt(sigmaAa + SigmaGuard) / C54 +
                            1.) -
                  0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                      (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) *
                 (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                      C54 +
                  1.) *
                 -2 * std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                 std::sqrt(sigmaAa + SigmaGuard) / std::pow(C54, 2)) /
                std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                 std::sqrt(sigmaAa + SigmaGuard) / C54 +
                             1.,
                         4)) -
           C75 * std::pow(rhoA, C50) * 0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) *
               ((C56 + std::sqrt(C60 + 1.)) * C54 *
                    (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                         std::sqrt(sigmaAa + SigmaGuard) / C54 +
                     1.) *
                    -2 * std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                    std::sqrt(sigmaAa + SigmaGuard) / std::pow(C54, 2) +
                (C56 + std::sqrt(C60 + 1.)) *
                    std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                     std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                 1.,
                             2)) /
               std::pow((C56 + std::sqrt(C60 + 1.)) * C54 *
                            std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                                             std::sqrt(sigmaAa + SigmaGuard) / C54 +
                                         1.,
                                     2),
                        2)) *
              C68) -
         C83 * std::pow(rhoA, C50) *
             (0.0042 * (std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                            std::sqrt(sigmaAa + SigmaGuard) / C54 +
                        1.) -
              0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
                  (2 * std::sqrt(C60 + 1.) * (C56 + std::sqrt(C60 + 1.)) * C54)) /
             std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                              std::sqrt(sigmaAa + SigmaGuard) / C54 +
                          1.,
                      2)) +
        C82 * std::pow(rhoA, C50) * 0.0042 * C60 * 0.0252 * std::sqrt(sigmaAa + SigmaGuard) /
            ((C56 + std::sqrt(C60 + 1.)) * C54 *
             std::pow(std::log(C56 + std::sqrt(C60 + 1.)) * 0.0252 *
                              std::sqrt(sigmaAa + SigmaGuard) / C54 +
                          1.,
                      2));

    matrix.upper[3] = 0;

    matrix.upper[4] = 0;

    const double C48 = rhoB + 1e-16;
    const double C52 = std::sqrt(sigmaBb);
    const double C55 = std::pow(C48, C50);
    const double C57 = C52 / C55;
    const double C61 = std::pow(C57, 2);
    const double C71 = 1;
    const double C72 = C50 * std::pow(C48, C50 - 1) * C71;
    const double C73 = (-C72 * std::sqrt(sigmaBb + SigmaGuard)) / std::pow(C55, 2);
    const double C74 = 2 * C57 * C73;
    const double C84 = C50 * (C50 - 1) * std::pow(C48, C50 - 2) * std::pow(C71, 2);
    const double C85 = C72 * C72 * std::sqrt(sigmaBb + SigmaGuard) * 2 * C55 / std::pow(C55, 4) -
                       C84 * std::sqrt(sigmaBb + SigmaGuard) / std::pow(C55, 2);
    const double C86 = 2 * std::pow(C73, 2) + 2 * C57 * C85;
    matrix.upper[5] =
        (((((((C73 * C50 * std::pow(rhoB, C50 - 1) * 0.0042 * C61 * 0.0252 *
                   std::sqrt(sigmaBb + SigmaGuard) /
                   ((C57 + std::sqrt(C61 + 1.)) * C55 *
                    std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                     std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                 1.,
                             2)) -
               (C74 * C50 * std::pow(rhoB, C50 - 1) *
                    (0.0042 * (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaBb + SigmaGuard) / C55 +
                               1.) -
                     0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                         (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) /
                    std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                     std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                 1.,
                             2) +
                C50 * (C50 - 1) * std::pow(rhoB, C50 - 2) *
                    (C62 + 0.0042 * C61 /
                               (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                1.)))) +
              C72 * C50 * std::pow(rhoB, C50 - 1) * -0.0042 * C61 *
                  std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                  (std::pow(C55, 2) * std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                                       std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                                   1.,
                                               2))) +
             C85 * std::pow(rhoB, C50) * 0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                 ((C57 + std::sqrt(C61 + 1.)) * C55 *
                  std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaBb + SigmaGuard) / C55 +
                               1.,
                           2))) +
            ((((C57 + std::sqrt(C61 + 1.)) * C55 *
                   std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                1.,
                            2) *
                   C73 * std::pow(rhoB, C50) * 0.00010584 * std::sqrt(sigmaBb + SigmaGuard) -
               C73 * std::pow(rhoB, C50) * 0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) *
                   ((C57 + std::sqrt(C61 + 1.)) * C55 *
                        (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                             std::sqrt(sigmaBb + SigmaGuard) / C55 +
                         1.) *
                        0.0504 * std::sqrt(sigmaBb + SigmaGuard) /
                        (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55) +
                    std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                     std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                 1.,
                             2) *
                        C55 / (2 * std::sqrt(C61 + 1.)))) /
                  std::pow((C57 + std::sqrt(C61 + 1.)) * C55 *
                               std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                                std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                            1.,
                                        2),
                           2) -
              ((std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                 std::sqrt(sigmaBb + SigmaGuard) / C55 +
                             1.,
                         2) *
                    C74 * std::pow(rhoB, C50) *
                    (0.00010584 * std::sqrt(sigmaBb + SigmaGuard) /
                         (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55) -
                     (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55 * 0.00010584 *
                          std::sqrt(sigmaBb + SigmaGuard) -
                      0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) * C55 *
                          (std::sqrt(C61 + 1.) + (C57 + std::sqrt(C61 + 1.))) /
                          std::sqrt(C61 + 1.)) /
                         std::pow(2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55, 2)) -
                C74 * std::pow(rhoB, C50) *
                    (0.0042 * (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaBb + SigmaGuard) / C55 +
                               1.) -
                     0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                         (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) *
                    (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                         std::sqrt(sigmaBb + SigmaGuard) / C55 +
                     1.) *
                    0.0504 * std::sqrt(sigmaBb + SigmaGuard) /
                    (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) /
                   std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                1.,
                            4) +
               C50 * std::pow(rhoB, C50 - 1) *
                   (0.0042 * (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                  std::sqrt(sigmaBb + SigmaGuard) / C55 +
                              1.) -
                    0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                        (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) /
                   std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                1.,
                            2))) +
             (std::pow(C55, 2) *
                  std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaBb + SigmaGuard) / C55 +
                               1.,
                           2) *
                  C72 * std::pow(rhoB, C50) * -0.0042 *
                  (C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                       (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.))) +
                   std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard)) -
              C72 * std::pow(rhoB, C50) * -0.0042 * C61 * std::log(C57 + std::sqrt(C61 + 1.)) *
                  0.0252 * std::sqrt(sigmaBb + SigmaGuard) * std::pow(C55, 2) *
                  (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                       C55 +
                   1.) *
                  0.0504 * std::sqrt(sigmaBb + SigmaGuard) /
                  (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) /
                 std::pow(std::pow(C55, 2) *
                              std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                               std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                           1.,
                                       2),
                          2)) *
                C74) +
           (((-C73 * std::pow(rhoB, C50) * 0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) *
              ((C57 + std::sqrt(C61 + 1.)) * C55 *
                   (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                        C55 +
                    1.) *
                   0.0504 * std::sqrt(sigmaBb + SigmaGuard) / ((C57 + std::sqrt(C61 + 1.)) * C55) +
               C55 * std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                      std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                  1.,
                              2))) /
                 std::pow((C57 + std::sqrt(C61 + 1.)) * C55 *
                              std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                               std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                           1.,
                                       2),
                          2) -
             ((std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                std::sqrt(sigmaBb + SigmaGuard) / C55 +
                            1.,
                        2) *
                   C74 * std::pow(rhoB, C50) *
                   (0.00010584 * std::sqrt(sigmaBb + SigmaGuard) /
                        ((C57 + std::sqrt(C61 + 1.)) * C55) -
                    (-0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) * 2 *
                     std::sqrt(C61 + 1.) * C55) /
                        std::pow(2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55, 2)) -
               C74 * std::pow(rhoB, C50) *
                   (0.0042 * (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                  std::sqrt(sigmaBb + SigmaGuard) / C55 +
                              1.) -
                    0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                        (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) *
                   (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                        C55 +
                    1.) *
                   0.0504 * std::sqrt(sigmaBb + SigmaGuard) / ((C57 + std::sqrt(C61 + 1.)) * C55)) /
                  std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaBb + SigmaGuard) / C55 +
                               1.,
                           4) -
              C50 * std::pow(rhoB, C50 - 1) * 0.0042 * C61 * 0.0252 *
                  std::sqrt(sigmaBb + SigmaGuard) /
                  ((C57 + std::sqrt(C61 + 1.)) * C55 *
                   std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                1.,
                            2)))) +
            (std::pow(C55, 2) *
                 std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                  std::sqrt(sigmaBb + SigmaGuard) / C55 +
                              1.,
                          2) *
                 C72 * std::pow(rhoB, C50) * -0.0042 * C61 * 0.0252 *
                 std::sqrt(sigmaBb + SigmaGuard) / (C57 + std::sqrt(C61 + 1.)) -
             C72 * std::pow(rhoB, C50) * -0.0042 * C61 * std::log(C57 + std::sqrt(C61 + 1.)) *
                 0.0252 * std::sqrt(sigmaBb + SigmaGuard) * std::pow(C55, 2) *
                 (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                      C55 +
                  1.) *
                 0.0504 * std::sqrt(sigmaBb + SigmaGuard) / ((C57 + std::sqrt(C61 + 1.)) * C55)) /
                std::pow(std::pow(C55, 2) * std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                                             std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                                         1.,
                                                     2),
                         2)) *
               C73) +
          (((-C73 * std::pow(rhoB, C50) * 0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) *
             ((C57 + std::sqrt(C61 + 1.)) * C55 *
                  (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                       C55 +
                   1.) *
                  -2 * std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                  std::sqrt(sigmaBb + SigmaGuard) / std::pow(C55, 2) +
              (C57 + std::sqrt(C61 + 1.)) * std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                                             std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                                         1.,
                                                     2))) /
                std::pow((C57 + std::sqrt(C61 + 1.)) * C55 *
                             std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                              std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                          1.,
                                      2),
                         2) -
            ((std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                               std::sqrt(sigmaBb + SigmaGuard) / C55 +
                           1.,
                       2) *
                  C74 * std::pow(rhoB, C50) *
                  (-0.0042 * std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                       std::sqrt(sigmaBb + SigmaGuard) / std::pow(C55, 2) -
                   (-0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) * 2 *
                    std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.))) /
                       std::pow(2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55, 2)) -
              C74 * std::pow(rhoB, C50) *
                  (0.0042 * (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                 std::sqrt(sigmaBb + SigmaGuard) / C55 +
                             1.) -
                   0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                       (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) *
                  (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                       C55 +
                   1.) *
                  -2 * std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                  std::sqrt(sigmaBb + SigmaGuard) / std::pow(C55, 2)) /
                 std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                  std::sqrt(sigmaBb + SigmaGuard) / C55 +
                              1.,
                          4) -
             C50 * std::pow(rhoB, C50 - 1) * -0.0042 * C61 * std::log(C57 + std::sqrt(C61 + 1.)) *
                 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                 (std::pow(C55, 2) * std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                                      std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                                  1.,
                                              2)))) -
           C72 * std::pow(rhoB, C50) * -0.0042 * C61 * std::log(C57 + std::sqrt(C61 + 1.)) *
               0.0252 * std::sqrt(sigmaBb + SigmaGuard) *
               (std::pow(C55, 2) *
                    (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                         std::sqrt(sigmaBb + SigmaGuard) / C55 +
                     1.) *
                    -2 * std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                    std::sqrt(sigmaBb + SigmaGuard) / std::pow(C55, 2) +
                2 * C55 *
                    std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                     std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                 1.,
                             2)) /
               std::pow(std::pow(C55, 2) * std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                                            std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                                        1.,
                                                    2),
                        2)) *
              C72) -
         C86 * std::pow(rhoB, C50) *
             (0.0042 * (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                            std::sqrt(sigmaBb + SigmaGuard) / C55 +
                        1.) -
              0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                  (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) /
             std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                              std::sqrt(sigmaBb + SigmaGuard) / C55 +
                          1.,
                      2)) +
        C84 * std::pow(rhoB, C50) * -0.0042 * C61 * std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
            std::sqrt(sigmaBb + SigmaGuard) /
            (std::pow(C55, 2) * std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                                 std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                             1.,
                                         2));

    matrix.upper[6] = 0;

    matrix.upper[7] = 0;

    const double C77 = 1 / (2 * std::sqrt(sigmaBb + SigmaGuard) * C55);
    const double C78 = 2 * C57 * C77;
    const double C87 = (-C72 * 2 * std::sqrt(sigmaBb + SigmaGuard)) /
                       std::pow(2 * std::sqrt(sigmaBb + SigmaGuard) * C55, 2);
    const double C88 = 2 * C77 * C73 + 2 * C57 * C87;
    matrix.upper[8] =
        ((((((C50 * std::pow(rhoB, C50 - 1) * 0.0042 * C61 * 0.0252 *
                  std::log(C57 + std::sqrt(C61 + 1.)) /
                  (2 * std::sqrt(sigmaBb + SigmaGuard) * C55 *
                   std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                1.,
                            2)) -
              C78 * C50 * std::pow(rhoB, C50 - 1) *
                  (0.0042 * (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                 std::sqrt(sigmaBb + SigmaGuard) / C55 +
                             1.) -
                   0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                       (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) /
                  std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaBb + SigmaGuard) / C55 +
                               1.,
                           2)) +
             C77 * C50 * std::pow(rhoB, C50 - 1) * 0.0042 * C61 * 0.0252 *
                 std::sqrt(sigmaBb + SigmaGuard) /
                 ((C57 + std::sqrt(C61 + 1.)) * C55 *
                  std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaBb + SigmaGuard) / C55 +
                               1.,
                           2))) +
            (((2 * std::sqrt(sigmaBb + SigmaGuard) * C55 *
                   std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                1.,
                            2) *
                   std::pow(rhoB, C50) * 0.0042 *
                   (0.0252 * C61 / (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.))) +
                    0.0252 * std::log(C57 + std::sqrt(C61 + 1.))) -
               std::pow(rhoB, C50) * 0.0042 * C61 * 0.0252 * std::log(C57 + std::sqrt(C61 + 1.)) *
                   std::sqrt(sigmaBb + SigmaGuard) * C55 *
                   (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                        C55 +
                    1.) *
                   0.0504 * std::sqrt(sigmaBb + SigmaGuard) /
                   (std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) /
                  std::pow(2 * std::sqrt(sigmaBb + SigmaGuard) * C55 *
                               std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                                std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                            1.,
                                        2),
                           2) -
              (std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                std::sqrt(sigmaBb + SigmaGuard) / C55 +
                            1.,
                        2) *
                   C78 * std::pow(rhoB, C50) *
                   (0.00010584 * std::sqrt(sigmaBb + SigmaGuard) /
                        (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55) -
                    (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55 * 0.00010584 *
                         std::sqrt(sigmaBb + SigmaGuard) -
                     0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) * C55 *
                         (std::sqrt(C61 + 1.) + (C57 + std::sqrt(C61 + 1.))) /
                         std::sqrt(C61 + 1.)) /
                        std::pow(2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55, 2)) -
               C78 * std::pow(rhoB, C50) *
                   (0.0042 * (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                  std::sqrt(sigmaBb + SigmaGuard) / C55 +
                              1.) -
                    0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                        (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) *
                   (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                        C55 +
                    1.) *
                   0.0504 * std::sqrt(sigmaBb + SigmaGuard) /
                   (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) /
                  std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaBb + SigmaGuard) / C55 +
                               1.,
                           4)) +
             ((C57 + std::sqrt(C61 + 1.)) * C55 *
                  std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaBb + SigmaGuard) / C55 +
                               1.,
                           2) *
                  C77 * std::pow(rhoB, C50) * 0.00010584 * std::sqrt(sigmaBb + SigmaGuard) -
              C77 * std::pow(rhoB, C50) * 0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) *
                  ((C57 + std::sqrt(C61 + 1.)) * C55 *
                       (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                            std::sqrt(sigmaBb + SigmaGuard) / C55 +
                        1.) *
                       0.0504 * std::sqrt(sigmaBb + SigmaGuard) /
                       (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55) +
                   std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                    std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                1.,
                            2) *
                       C55 / (2 * std::sqrt(C61 + 1.)))) /
                 std::pow((C57 + std::sqrt(C61 + 1.)) * C55 *
                              std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                               std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                           1.,
                                       2),
                          2)) *
                C74) +
           (((2 * std::sqrt(sigmaBb + SigmaGuard) * C55 *
                  std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaBb + SigmaGuard) / C55 +
                               1.,
                           2) *
                  std::pow(rhoB, C50) * 0.00010584 * C61 / (C57 + std::sqrt(C61 + 1.)) -
              std::pow(rhoB, C50) * 0.0042 * C61 * 0.0252 * std::log(C57 + std::sqrt(C61 + 1.)) *
                  2 * std::sqrt(sigmaBb + SigmaGuard) * C55 *
                  (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                       C55 +
                   1.) *
                  0.0504 * std::sqrt(sigmaBb + SigmaGuard) / ((C57 + std::sqrt(C61 + 1.)) * C55)) /
                 std::pow(2 * std::sqrt(sigmaBb + SigmaGuard) * C55 *
                              std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                               std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                           1.,
                                       2),
                          2) -
             (std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                               std::sqrt(sigmaBb + SigmaGuard) / C55 +
                           1.,
                       2) *
                  C78 * std::pow(rhoB, C50) *
                  (0.00010584 * std::sqrt(sigmaBb + SigmaGuard) /
                       ((C57 + std::sqrt(C61 + 1.)) * C55) -
                   (-0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) * 2 *
                    std::sqrt(C61 + 1.) * C55) /
                       std::pow(2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55, 2)) -
              C78 * std::pow(rhoB, C50) *
                  (0.0042 * (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                 std::sqrt(sigmaBb + SigmaGuard) / C55 +
                             1.) -
                   0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                       (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) *
                  (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                       C55 +
                   1.) *
                  0.0504 * std::sqrt(sigmaBb + SigmaGuard) / ((C57 + std::sqrt(C61 + 1.)) * C55)) /
                 std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                  std::sqrt(sigmaBb + SigmaGuard) / C55 +
                              1.,
                          4)) -
            C77 * std::pow(rhoB, C50) * 0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) *
                ((C57 + std::sqrt(C61 + 1.)) * C55 *
                     (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                          std::sqrt(sigmaBb + SigmaGuard) / C55 +
                      1.) *
                     0.0504 * std::sqrt(sigmaBb + SigmaGuard) /
                     ((C57 + std::sqrt(C61 + 1.)) * C55) +
                 C55 * std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                        std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                    1.,
                                2)) /
                std::pow((C57 + std::sqrt(C61 + 1.)) * C55 *
                             std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                              std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                          1.,
                                      2),
                         2)) *
               C73) +
          (((-std::pow(rhoB, C50) * 0.0042 * C61 * 0.0252 * std::log(C57 + std::sqrt(C61 + 1.)) *
             2 *
             (std::sqrt(sigmaBb + SigmaGuard) * C55 *
                  (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                       C55 +
                   1.) *
                  -2 * std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                  std::sqrt(sigmaBb + SigmaGuard) / std::pow(C55, 2) +
              std::sqrt(sigmaBb + SigmaGuard) *
                  std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                   std::sqrt(sigmaBb + SigmaGuard) / C55 +
                               1.,
                           2))) /
                std::pow(2 * std::sqrt(sigmaBb + SigmaGuard) * C55 *
                             std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                              std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                          1.,
                                      2),
                         2) -
            (std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                              std::sqrt(sigmaBb + SigmaGuard) / C55 +
                          1.,
                      2) *
                 C78 * std::pow(rhoB, C50) *
                 (-0.0042 * std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                      std::sqrt(sigmaBb + SigmaGuard) / std::pow(C55, 2) -
                  (-0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) * 2 *
                   std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.))) /
                      std::pow(2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55, 2)) -
             C78 * std::pow(rhoB, C50) *
                 (0.0042 * (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                std::sqrt(sigmaBb + SigmaGuard) / C55 +
                            1.) -
                  0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                      (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) *
                 (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                      C55 +
                  1.) *
                 -2 * std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                 std::sqrt(sigmaBb + SigmaGuard) / std::pow(C55, 2)) /
                std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                 std::sqrt(sigmaBb + SigmaGuard) / C55 +
                             1.,
                         4)) -
           C77 * std::pow(rhoB, C50) * 0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) *
               ((C57 + std::sqrt(C61 + 1.)) * C55 *
                    (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                         std::sqrt(sigmaBb + SigmaGuard) / C55 +
                     1.) *
                    -2 * std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                    std::sqrt(sigmaBb + SigmaGuard) / std::pow(C55, 2) +
                (C57 + std::sqrt(C61 + 1.)) *
                    std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                     std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                 1.,
                             2)) /
               std::pow((C57 + std::sqrt(C61 + 1.)) * C55 *
                            std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                                             std::sqrt(sigmaBb + SigmaGuard) / C55 +
                                         1.,
                                     2),
                        2)) *
              C72) -
         C88 * std::pow(rhoB, C50) *
             (0.0042 * (std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                            std::sqrt(sigmaBb + SigmaGuard) / C55 +
                        1.) -
              0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
                  (2 * std::sqrt(C61 + 1.) * (C57 + std::sqrt(C61 + 1.)) * C55)) /
             std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                              std::sqrt(sigmaBb + SigmaGuard) / C55 +
                          1.,
                      2)) +
        C87 * std::pow(rhoB, C50) * 0.0042 * C61 * 0.0252 * std::sqrt(sigmaBb + SigmaGuard) /
            ((C57 + std::sqrt(C61 + 1.)) * C55 *
             std::pow(std::log(C57 + std::sqrt(C61 + 1.)) * 0.0252 *
                              std::sqrt(sigmaBb + SigmaGuard) / C55 +
                          1.,
                      2));

    const double C89 = rhoA + 1e-16;
    const double C90 = 4. / 3.;
    const double C91 = std::sqrt(sigmaAa);
    const double C92 = 0.0252 * C91;
    const double C93 = std::pow(C89, C90);
    const double C94 = C91 / C93;
    const double C95 = excgrid::internal::AsinhOverX(C94);
    const double C96 = std::pow(C94, 2);
    const double C97 = C96 + 1.;
    const double C98 = std::sqrt(C97);
    const double C99 = C94 + C98;
    const double C100 = 1 / C98;
    const double C101 = C95 + C100;
    const double C102 = std::log(C99);
    const double C103 = C102 * C92;
    const double C104 = C103 / C93;
    const double C105 = C104 + 1;
    matrix.upper[9] = std::pow(rhoA, C90) * 0.00010584 *
                      ((C105 * (C96 + 2) * std::pow(C100, 3) + 3 * C105 * C101) -
                       0.0504 * C96 * std::pow(C101, 2)) /
                      (std::pow(C105, 3) * 4 * std::pow(C89, 16. / 3.));

    matrix.upper[10] = 0;

    matrix.upper[11] = 0;

    matrix.upper[12] = 0;

    matrix.upper[13] = 0;

    const double C106 = rhoB + 1e-16;
    const double C107 = 4. / 3.;
    const double C108 = std::sqrt(sigmaBb);
    const double C109 = 0.0252 * C108;
    const double C110 = std::pow(C106, C107);
    const double C111 = C108 / C110;
    const double C112 = excgrid::internal::AsinhOverX(C111);
    const double C113 = std::pow(C111, 2);
    const double C114 = C113 + 1.;
    const double C115 = std::sqrt(C114);
    const double C116 = C111 + C115;
    const double C117 = 1 / C115;
    const double C118 = C112 + C117;
    const double C119 = std::log(C116);
    const double C120 = C119 * C109;
    const double C121 = C120 / C110;
    const double C122 = C121 + 1;
    matrix.upper[14] = std::pow(rhoB, C107) * 0.00010584 *
                       ((C122 * (C113 + 2) * std::pow(C117, 3) + 3 * C122 * C118) -
                        0.0504 * C113 * std::pow(C118, 2)) /
                       (std::pow(C122, 3) * 4 * std::pow(C106, 16. / 3.));
}

} // namespace excgrid
