// excgrid codegen source: the mPW91 (modified PW91) GGA exchange
// functional (Adamo-Barone).  Fresh authorship; the form is the published
// PW91 exchange with this functional's modified constants.
//
// The damping exponent, re-derived here rather than quoted at one digit
// count: PW91 damps with Exp(-alpha s^2), alpha = 100, where
// s = |grad rho| / (2 kF rho) and kF = (6 pi^2 rho)^(1/3) is the
// spin-density Fermi wavevector.  This kernel works in the variable
// x = |grad rho| / rho^(4/3), so s = x / (2 (6 pi^2)^(1/3)) and the damping
// is Exp(-100 / (4 (6 pi^2)^(2/3)) * x^2).  The exponent below is that
// constant - 1.6455307846020557... - carried at double precision, so no
// digit of it is a free parameter.  Regenerate with tools/regenerate.py.

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

XcKernelValue MPw91Exchange(
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

    const double C114 = 4. * Pi;
    const double C115 = 36. * Pi;
    const double C116 = rhoA + 1e-16;
    const double C117 = rhoB + 1e-16;
    const double C118 = -5. / 3.;
    const double C119 = 1. / 3.;
    const double C120 = 4. / 3.;
    const double C121 = std::sqrt(sigmaAa);
    const double C122 = std::sqrt(sigmaBb);
    const double C123 = std::pow(3., C120);
    const double C124 = std::pow(C114, C119);
    const double C125 = std::pow(C115, C118);
    const double C126 = std::pow(C116, C120);
    const double C127 = std::pow(C117, C120);
    const double C128 = std::pow(rhoA, C120);
    const double C129 = std::pow(rhoB, C120);
    const double C130 = 5. * C125;
    const double C131 = C121 / C126;
    const double C132 = C122 / C127;
    const double C133 = 0.00426 - C130;
    const double C134 = std::pow(C131, 2);
    const double C135 = std::pow(C131, 3.72);
    const double C136 = std::pow(C132, 2);
    const double C137 = std::pow(C132, 3.72);

    result.exc = -(
        (0.9305257363491 * (C128 + C129) +
         C128 *
             ((0.00426 * C134 - C133 * C134 * std::exp(-1.6455307846 * C134)) - 0.000001 * C135) /
             ((std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 * C121 / C126 +
               0.000002 * C135 * C124 / C123) +
              1.)) +
        C129 * ((0.00426 * C136 - C133 * C136 * std::exp(-1.6455307846 * C136)) - 0.000001 * C137) /
            ((std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 * C122 / C127 +
              0.000002 * C137 * C124 / C123) +
             1.));

    const double C139 = 1;
    const double C140 = C120 * std::pow(C116, C120 - 1) * C139;
    const double C141 = C120 * std::pow(rhoA, C120 - 1);
    const double C142 = (-C140 * std::sqrt(sigmaAa)) / std::pow(C126, 2);
    const double C143 = 2 * C131 * C142;
    const double C144 = 3.72 * std::pow(C131, 2.72) * C142;

    result.vrhoA =
        ((((0. -
            (((0.00426 * C134 - C133 * C134 * std::exp(-1.6455307846 * C134)) - 0.000001 * C135) /
                 ((std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C126 +
                   0.000002 * C135 * C124 / C123) +
                  1.) +
             0.9305257363491) *
                C141) -
           ((((std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C126 +
               0.000002 * C135 * C124 / C123) +
              1.) *
                 C128 *
                 (0.00426 - (C133 * C134 * -1.6455307846 * std::exp(-1.6455307846 * C134) +
                             C133 * std::exp(-1.6455307846 * C134))) -
             C128 *
                 ((0.00426 * C134 - C133 * C134 * std::exp(-1.6455307846 * C134)) -
                  0.000001 * C135) *
                 0.02556 * std::sqrt(sigmaAa) /
                 (2 * std::sqrt(C134 + 1.) * (C131 + std::sqrt(C134 + 1.)) * C126)) /
                std::pow(
                    (std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C126 +
                     0.000002 * C135 * C124 / C123) +
                        1.,
                    2) +
            0.) *
               C143) -
          ((((std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C126 +
              0.000002 * C135 * C124 / C123) +
             1.) *
                -0.000001 * C128 -
            C128 *
                ((0.00426 * C134 - C133 * C134 * std::exp(-1.6455307846 * C134)) -
                 0.000001 * C135) *
                0.000002 * C124 / C123) /
               std::pow(
                   (std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C126 +
                    0.000002 * C135 * C124 / C123) +
                       1.,
                   2) +
           0.) *
              C144) +
         (C128 *
              ((0.00426 * C134 - C133 * C134 * std::exp(-1.6455307846 * C134)) - 0.000001 * C135) *
              0.02556 * std::sqrt(sigmaAa) /
              ((C131 + std::sqrt(C134 + 1.)) * C126 *
               std::pow(
                   (std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C126 +
                    0.000002 * C135 * C124 / C123) +
                       1.,
                   2)) -
          0.) *
             C142) -
        (C128 *
             ((0.00426 * C134 - C133 * C134 * std::exp(-1.6455307846 * C134)) - 0.000001 * C135) *
             std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
             (std::pow(C126, 2) * std::pow((std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 *
                                                std::sqrt(sigmaAa) / C126 +
                                            0.000002 * C135 * C124 / C123) +
                                               1.,
                                           2)) +
         0.) *
            C140;

    const double C145 = 1;
    const double C146 = C120 * std::pow(C117, C120 - 1) * C145;
    const double C147 = C120 * std::pow(rhoB, C120 - 1);
    const double C148 = (-C146 * std::sqrt(sigmaBb)) / std::pow(C127, 2);
    const double C149 = 2 * C132 * C148;
    const double C150 = 3.72 * std::pow(C132, 2.72) * C148;

    result.vrhoB =
        ((((0. -
            (((0.00426 * C136 - C133 * C136 * std::exp(-1.6455307846 * C136)) - 0.000001 * C137) /
                 ((std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C127 +
                   0.000002 * C137 * C124 / C123) +
                  1.) +
             0.9305257363491) *
                C147) -
           ((((std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C127 +
               0.000002 * C137 * C124 / C123) +
              1.) *
                 C129 *
                 (0.00426 - (C133 * C136 * -1.6455307846 * std::exp(-1.6455307846 * C136) +
                             C133 * std::exp(-1.6455307846 * C136))) -
             C129 *
                 ((0.00426 * C136 - C133 * C136 * std::exp(-1.6455307846 * C136)) -
                  0.000001 * C137) *
                 0.02556 * std::sqrt(sigmaBb) /
                 (2 * std::sqrt(C136 + 1.) * (C132 + std::sqrt(C136 + 1.)) * C127)) /
                std::pow(
                    (std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C127 +
                     0.000002 * C137 * C124 / C123) +
                        1.,
                    2) +
            0.) *
               C149) -
          ((((std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C127 +
              0.000002 * C137 * C124 / C123) +
             1.) *
                -0.000001 * C129 -
            C129 *
                ((0.00426 * C136 - C133 * C136 * std::exp(-1.6455307846 * C136)) -
                 0.000001 * C137) *
                0.000002 * C124 / C123) /
               std::pow(
                   (std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C127 +
                    0.000002 * C137 * C124 / C123) +
                       1.,
                   2) +
           0.) *
              C150) +
         (C129 *
              ((0.00426 * C136 - C133 * C136 * std::exp(-1.6455307846 * C136)) - 0.000001 * C137) *
              0.02556 * std::sqrt(sigmaBb) /
              ((C132 + std::sqrt(C136 + 1.)) * C127 *
               std::pow(
                   (std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C127 +
                    0.000002 * C137 * C124 / C123) +
                       1.,
                   2)) -
          0.) *
             C148) -
        (C129 *
             ((0.00426 * C136 - C133 * C136 * std::exp(-1.6455307846 * C136)) - 0.000001 * C137) *
             std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
             (std::pow(C127, 2) * std::pow((std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 *
                                                std::sqrt(sigmaBb) / C127 +
                                            0.000002 * C137 * C124 / C123) +
                                               1.,
                                           2)) +
         0.) *
            C146;

    const double C151 = 1 / (2 * std::sqrt(sigmaAa + SigmaGuard) * C126);
    const double C152 = 2 * C131 * C151;
    const double C153 = 3.72 * std::pow(C131, 2.72) * C151;

    result.vsigmaAa =
        (((C128 *
               ((0.00426 * C134 - C133 * C134 * std::exp(-1.6455307846 * C134)) - 0.000001 * C135) *
               0.02556 * std::log(C131 + std::sqrt(C134 + 1.)) /
               (2 * std::sqrt(sigmaAa + SigmaGuard) * C126 *
                std::pow((std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 *
                              std::sqrt(sigmaAa + SigmaGuard) / C126 +
                          0.000002 * C135 * C124 / C123) +
                             1.,
                         2)) -
           0.) -
          ((((std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 * std::sqrt(sigmaAa + SigmaGuard) /
                  C126 +
              0.000002 * C135 * C124 / C123) +
             1.) *
                C128 *
                (0.00426 - (C133 * C134 * -1.6455307846 * std::exp(-1.6455307846 * C134) +
                            C133 * std::exp(-1.6455307846 * C134))) -
            C128 *
                ((0.00426 * C134 - C133 * C134 * std::exp(-1.6455307846 * C134)) -
                 0.000001 * C135) *
                0.02556 * std::sqrt(sigmaAa + SigmaGuard) /
                (2 * std::sqrt(C134 + 1.) * (C131 + std::sqrt(C134 + 1.)) * C126)) /
               std::pow((std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 *
                             std::sqrt(sigmaAa + SigmaGuard) / C126 +
                         0.000002 * C135 * C124 / C123) +
                            1.,
                        2) +
           0.) *
              C152) -
         ((((std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 * std::sqrt(sigmaAa + SigmaGuard) /
                 C126 +
             0.000002 * C135 * C124 / C123) +
            1.) *
               -0.000001 * C128 -
           C128 *
               ((0.00426 * C134 - C133 * C134 * std::exp(-1.6455307846 * C134)) - 0.000001 * C135) *
               0.000002 * C124 / C123) /
              std::pow((std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 *
                            std::sqrt(sigmaAa + SigmaGuard) / C126 +
                        0.000002 * C135 * C124 / C123) +
                           1.,
                       2) +
          0.) *
             C153) +
        (C128 *
             ((0.00426 * C134 - C133 * C134 * std::exp(-1.6455307846 * C134)) - 0.000001 * C135) *
             0.02556 * std::sqrt(sigmaAa + SigmaGuard) /
             ((C131 + std::sqrt(C134 + 1.)) * C126 *
              std::pow((std::log(C131 + std::sqrt(C134 + 1.)) * 0.02556 *
                            std::sqrt(sigmaAa + SigmaGuard) / C126 +
                        0.000002 * C135 * C124 / C123) +
                           1.,
                       2)) -
         0.) *
            C151;

    result.vsigmaAb = 0.;

    const double C154 = 1 / (2 * std::sqrt(sigmaBb + SigmaGuard) * C127);
    const double C155 = 2 * C132 * C154;
    const double C156 = 3.72 * std::pow(C132, 2.72) * C154;

    result.vsigmaBb =
        (((C129 *
               ((0.00426 * C136 - C133 * C136 * std::exp(-1.6455307846 * C136)) - 0.000001 * C137) *
               0.02556 * std::log(C132 + std::sqrt(C136 + 1.)) /
               (2 * std::sqrt(sigmaBb + SigmaGuard) * C127 *
                std::pow((std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 *
                              std::sqrt(sigmaBb + SigmaGuard) / C127 +
                          0.000002 * C137 * C124 / C123) +
                             1.,
                         2)) -
           0.) -
          ((((std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 * std::sqrt(sigmaBb + SigmaGuard) /
                  C127 +
              0.000002 * C137 * C124 / C123) +
             1.) *
                C129 *
                (0.00426 - (C133 * C136 * -1.6455307846 * std::exp(-1.6455307846 * C136) +
                            C133 * std::exp(-1.6455307846 * C136))) -
            C129 *
                ((0.00426 * C136 - C133 * C136 * std::exp(-1.6455307846 * C136)) -
                 0.000001 * C137) *
                0.02556 * std::sqrt(sigmaBb + SigmaGuard) /
                (2 * std::sqrt(C136 + 1.) * (C132 + std::sqrt(C136 + 1.)) * C127)) /
               std::pow((std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 *
                             std::sqrt(sigmaBb + SigmaGuard) / C127 +
                         0.000002 * C137 * C124 / C123) +
                            1.,
                        2) +
           0.) *
              C155) -
         ((((std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 * std::sqrt(sigmaBb + SigmaGuard) /
                 C127 +
             0.000002 * C137 * C124 / C123) +
            1.) *
               -0.000001 * C129 -
           C129 *
               ((0.00426 * C136 - C133 * C136 * std::exp(-1.6455307846 * C136)) - 0.000001 * C137) *
               0.000002 * C124 / C123) /
              std::pow((std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 *
                            std::sqrt(sigmaBb + SigmaGuard) / C127 +
                        0.000002 * C137 * C124 / C123) +
                           1.,
                       2) +
          0.) *
             C156) +
        (C129 *
             ((0.00426 * C136 - C133 * C136 * std::exp(-1.6455307846 * C136)) - 0.000001 * C137) *
             0.02556 * std::sqrt(sigmaBb + SigmaGuard) /
             ((C132 + std::sqrt(C136 + 1.)) * C127 *
              std::pow((std::log(C132 + std::sqrt(C136 + 1.)) * 0.02556 *
                            std::sqrt(sigmaBb + SigmaGuard) / C127 +
                        0.000002 * C137 * C124 / C123) +
                           1.,
                       2)) -
         0.) *
            C154;

    return result;
}

void MPw91ExchangeSecondDerivatives(double rhoA,
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

    const double C46 = 4. * Pi;
    const double C47 = 36. * Pi;
    const double C48 = rhoA + 1e-16;
    const double C50 = -5. / 3.;
    const double C51 = 1. / 3.;
    const double C52 = 4. / 3.;
    const double C53 = std::sqrt(sigmaAa);
    const double C55 = std::pow(3., C52);
    const double C56 = std::pow(C46, C51);
    const double C57 = std::pow(C47, C50);
    const double C58 = std::pow(C48, C52);
    const double C60 = std::pow(rhoA, C52);
    const double C62 = 5. * C57;
    const double C63 = C53 / C58;
    const double C65 = 0.00426 - C62;
    const double C66 = std::pow(C63, 2);
    const double C67 = std::pow(C63, 3.72);
    const double C74 = 1;
    const double C75 = C52 * std::pow(C48, C52 - 1) * C74;
    const double C76 = C52 * std::pow(rhoA, C52 - 1);
    const double C77 = (-C75 * std::sqrt(sigmaAa)) / std::pow(C58, 2);
    const double C78 = 2 * C63 * C77;
    const double C79 = 3.72 * std::pow(C63, 2.72) * C77;
    const double C92 = C52 * (C52 - 1) * std::pow(C48, C52 - 2) * std::pow(C74, 2);
    const double C93 = C52 * (C52 - 1) * std::pow(rhoA, C52 - 2);
    const double C94 = C75 * C75 * std::sqrt(sigmaAa) * 2 * C58 / std::pow(C58, 4) -
                       C92 * std::sqrt(sigmaAa) / std::pow(C58, 2);
    const double C95 = 2 * std::pow(C77, 2) + 2 * C63 * C94;
    const double C96 =
        10.1184 * std::pow(C63, 1.72) * std::pow(C77, 2) + 3.72 * std::pow(C63, 2.72) * C94;
    matrix.upper[0] =
        ((((((((((C77 *
                      ((C63 + std::sqrt(C66 + 1.)) * C58 *
                           std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                         std::sqrt(sigmaAa) / C58 +
                                     0.000002 * C67 * C56 / C55) +
                                        1.,
                                    2) *
                           C60 *
                           (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                       C65 * std::exp(-1.6455307846 * C66))) *
                           0.02556 * std::sqrt(sigmaAa) -
                       C60 *
                           ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                            0.000001 * C67) *
                           0.02556 * std::sqrt(sigmaAa) *
                           ((C63 + std::sqrt(C66 + 1.)) * C58 *
                                ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaAa) / C58 +
                                  0.000002 * C67 * C56 / C55) +
                                 1.) *
                                0.05112 * std::sqrt(sigmaAa) /
                                (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58) +
                            std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                          std::sqrt(sigmaAa) / C58 +
                                      0.000002 * C67 * C56 / C55) +
                                         1.,
                                     2) *
                                C58 / (2 * std::sqrt(C66 + 1.)))) /
                      std::pow((C63 + std::sqrt(C66 + 1.)) * C58 *
                                   std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                                 std::sqrt(sigmaAa) / C58 +
                                             0.000002 * C67 * C56 / C55) +
                                                1.,
                                            2),
                               2) -
                  ((C76 *
                        (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                               C58 +
                           0.000002 * C67 * C56 / C55) +
                          1.) *
                             (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                         C65 * std::exp(-1.6455307846 * C66))) -
                         ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                          0.000001 * C67) *
                             0.02556 * std::sqrt(sigmaAa) /
                             (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
                        std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaAa) / C58 +
                                  0.000002 * C67 * C56 / C55) +
                                     1.,
                                 2) +
                    C78 *
                        (std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                       std::sqrt(sigmaAa) / C58 +
                                   0.000002 * C67 * C56 / C55) +
                                      1.,
                                  2) *
                             ((C60 *
                                   (0.00426 -
                                    (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                     C65 * std::exp(-1.6455307846 * C66))) *
                                   0.02556 * std::sqrt(sigmaAa) /
                                   (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58) -
                               ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaAa) / C58 +
                                 0.000002 * C67 * C56 / C55) +
                                1.) *
                                   C60 *
                                   ((C65 * C66 * 2.7077715630662916 *
                                         std::exp(-1.6455307846 * C66) +
                                     C65 * -1.6455307846 * std::exp(-1.6455307846 * C66)) +
                                    C65 * -1.6455307846 * std::exp(-1.6455307846 * C66))) -
                              (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58 * C60 *
                                   (0.00426 -
                                    (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                     C65 * std::exp(-1.6455307846 * C66))) *
                                   0.02556 * std::sqrt(sigmaAa) -
                               C60 *
                                   ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                                    0.000001 * C67) *
                                   0.02556 * std::sqrt(sigmaAa) * C58 *
                                   (std::sqrt(C66 + 1.) + (C63 + std::sqrt(C66 + 1.))) /
                                   std::sqrt(C66 + 1.)) /
                                  std::pow(2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) *
                                               C58,
                                           2)) -
                         (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                C58 +
                            0.000002 * C67 * C56 / C55) +
                           1.) *
                              C60 *
                              (0.00426 -
                               (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                C65 * std::exp(-1.6455307846 * C66))) -
                          C60 *
                              ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                               0.000001 * C67) *
                              0.02556 * std::sqrt(sigmaAa) /
                              (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) *
                             ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                   C58 +
                               0.000002 * C67 * C56 / C55) +
                              1.) *
                             0.05112 * std::sqrt(sigmaAa) /
                             (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
                        std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaAa) / C58 +
                                  0.000002 * C67 * C56 / C55) +
                                     1.,
                                 4)) +
                   C79 *
                       (std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaAa) / C58 +
                                  0.000002 * C67 * C56 / C55) +
                                     1.,
                                 2) *
                            (-0.000001 * C60 * 0.02556 * std::sqrt(sigmaAa) /
                                 (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58) -
                             C60 *
                                 (0.00426 -
                                  (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                   C65 * std::exp(-1.6455307846 * C66))) *
                                 0.000002 * C56 / C55) -
                        (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                               C58 +
                           0.000002 * C67 * C56 / C55) +
                          1.) *
                             -0.000001 * C60 -
                         C60 *
                             ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                              0.000001 * C67) *
                             0.000002 * C56 / C55) *
                            ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                  C58 +
                              0.000002 * C67 * C56 / C55) +
                             1.) *
                            0.05112 * std::sqrt(sigmaAa) /
                            (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
                       std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaAa) / C58 +
                                 0.000002 * C67 * C56 / C55) +
                                    1.,
                                4))) -
                 C75 *
                     (std::pow(C58, 2) *
                          std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                        std::sqrt(sigmaAa) / C58 +
                                    0.000002 * C67 * C56 / C55) +
                                       1.,
                                   2) *
                          (C60 *
                               ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                                0.000001 * C67) *
                               0.02556 * std::sqrt(sigmaAa) /
                               (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.))) +
                           C60 *
                               (0.00426 -
                                (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                 C65 * std::exp(-1.6455307846 * C66))) *
                               std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa)) -
                      C60 *
                          ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                           0.000001 * C67) *
                          std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) *
                          std::pow(C58, 2) *
                          ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                C58 +
                            0.000002 * C67 * C56 / C55) +
                           1.) *
                          0.05112 * std::sqrt(sigmaAa) /
                          (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
                     std::pow(std::pow(C58, 2) * std::pow((std::log(C63 + std::sqrt(C66 + 1.)) *
                                                               0.02556 * std::sqrt(sigmaAa) / C58 +
                                                           0.000002 * C67 * C56 / C55) +
                                                              1.,
                                                          2),
                              2)) *
                    C78 +
                (((C79 *
                       (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                         1.) *
                            -0.000001 * C60 -
                        C60 *
                            ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                             0.000001 * C67) *
                            0.000002 * C56 / C55) *
                       ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                        1.) *
                       0.000004 * C56 /
                       (C55 * std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                            std::sqrt(sigmaAa) / C58 +
                                        0.000002 * C67 * C56 / C55) +
                                           1.,
                                       4)) -
                   (C76 *
                        (-0.000001 * ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                           std::sqrt(sigmaAa) / C58 +
                                       0.000002 * C67 * C56 / C55) +
                                      1.) -
                         ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                          0.000001 * C67) *
                             0.000002 * C56 / C55) /
                        std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaAa) / C58 +
                                  0.000002 * C67 * C56 / C55) +
                                     1.,
                                 2) +
                    C78 *
                        (std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                       std::sqrt(sigmaAa) / C58 +
                                   0.000002 * C67 * C56 / C55) +
                                      1.,
                                  2) *
                             (C60 *
                                  (0.00426 -
                                   (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                    C65 * std::exp(-1.6455307846 * C66))) *
                                  0.000002 * C56 / C55 -
                              -0.000001 * C60 * 0.02556 * std::sqrt(sigmaAa) /
                                  (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) -
                         (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                C58 +
                            0.000002 * C67 * C56 / C55) +
                           1.) *
                              C60 *
                              (0.00426 -
                               (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                C65 * std::exp(-1.6455307846 * C66))) -
                          C60 *
                              ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                               0.000001 * C67) *
                              0.02556 * std::sqrt(sigmaAa) /
                              (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) *
                             ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                   C58 +
                               0.000002 * C67 * C56 / C55) +
                              1.) *
                             0.000004 * C56 / C55) /
                        std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaAa) / C58 +
                                  0.000002 * C67 * C56 / C55) +
                                     1.,
                                 4))) +
                  C77 *
                      ((C63 + std::sqrt(C66 + 1.)) * C58 *
                           std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                         std::sqrt(sigmaAa) / C58 +
                                     0.000002 * C67 * C56 / C55) +
                                        1.,
                                    2) *
                           -0.000001 * C60 * 0.02556 * std::sqrt(sigmaAa) -
                       C60 *
                           ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                            0.000001 * C67) *
                           0.02556 * std::sqrt(sigmaAa) * (C63 + std::sqrt(C66 + 1.)) * C58 *
                           ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                 C58 +
                             0.000002 * C67 * C56 / C55) +
                            1.) *
                           0.000004 * C56 / C55) /
                      std::pow((C63 + std::sqrt(C66 + 1.)) * C58 *
                                   std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                                 std::sqrt(sigmaAa) / C58 +
                                             0.000002 * C67 * C56 / C55) +
                                                1.,
                                            2),
                               2)) -
                 C75 *
                     (std::pow(C58, 2) *
                          std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                        std::sqrt(sigmaAa) / C58 +
                                    0.000002 * C67 * C56 / C55) +
                                       1.,
                                   2) *
                          -0.000001 * C60 * std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                          std::sqrt(sigmaAa) -
                      C60 *
                          ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                           0.000001 * C67) *
                          std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) *
                          std::pow(C58, 2) *
                          ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                C58 +
                            0.000002 * C67 * C56 / C55) +
                           1.) *
                          0.000004 * C56 / C55) /
                     std::pow(std::pow(C58, 2) * std::pow((std::log(C63 + std::sqrt(C66 + 1.)) *
                                                               0.02556 * std::sqrt(sigmaAa) / C58 +
                                                           0.000002 * C67 * C56 / C55) +
                                                              1.,
                                                          2),
                              2)) *
                    C79) +
               ((((C76 *
                       ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                        0.000001 * C67) *
                       0.02556 * std::sqrt(sigmaAa) /
                       ((C63 + std::sqrt(C66 + 1.)) * C58 *
                        std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaAa) / C58 +
                                  0.000002 * C67 * C56 / C55) +
                                     1.,
                                 2)) -
                   C78 *
                       (std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaAa) / C58 +
                                  0.000002 * C67 * C56 / C55) +
                                     1.,
                                 2) *
                            (C60 *
                                 (0.00426 -
                                  (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                   C65 * std::exp(-1.6455307846 * C66))) *
                                 0.02556 * std::sqrt(sigmaAa) /
                                 ((C63 + std::sqrt(C66 + 1.)) * C58) -
                             (-C60 *
                              ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                               0.000001 * C67) *
                              0.02556 * std::sqrt(sigmaAa) * 2 * std::sqrt(C66 + 1.) * C58) /
                                 std::pow(2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) *
                                              C58,
                                          2)) -
                        (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                               C58 +
                           0.000002 * C67 * C56 / C55) +
                          1.) *
                             C60 *
                             (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                         C65 * std::exp(-1.6455307846 * C66))) -
                         C60 *
                             ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                              0.000001 * C67) *
                             0.02556 * std::sqrt(sigmaAa) /
                             (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) *
                            ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                  C58 +
                              0.000002 * C67 * C56 / C55) +
                             1.) *
                            0.05112 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58)) /
                       std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaAa) / C58 +
                                 0.000002 * C67 * C56 / C55) +
                                    1.,
                                4)) -
                  C79 *
                      (std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaAa) / C58 +
                                 0.000002 * C67 * C56 / C55) +
                                    1.,
                                2) *
                           -0.000001 * C60 * 0.02556 * std::sqrt(sigmaAa) /
                           ((C63 + std::sqrt(C66 + 1.)) * C58) -
                       (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                         1.) *
                            -0.000001 * C60 -
                        C60 *
                            ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                             0.000001 * C67) *
                            0.000002 * C56 / C55) *
                           ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                 C58 +
                             0.000002 * C67 * C56 / C55) +
                            1.) *
                           0.05112 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58)) /
                      std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                    C58 +
                                0.000002 * C67 * C56 / C55) +
                                   1.,
                               4)) -
                 C77 * C60 *
                     ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                      0.000001 * C67) *
                     0.02556 * std::sqrt(sigmaAa) *
                     ((C63 + std::sqrt(C66 + 1.)) * C58 *
                          ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                C58 +
                            0.000002 * C67 * C56 / C55) +
                           1.) *
                          0.05112 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58) +
                      C58 * std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                          std::sqrt(sigmaAa) / C58 +
                                      0.000002 * C67 * C56 / C55) +
                                         1.,
                                     2)) /
                     std::pow((C63 + std::sqrt(C66 + 1.)) * C58 *
                                  std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                                std::sqrt(sigmaAa) / C58 +
                                            0.000002 * C67 * C56 / C55) +
                                               1.,
                                           2),
                              2)) -
                C75 *
                    (std::pow(C58, 2) *
                         std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                       std::sqrt(sigmaAa) / C58 +
                                   0.000002 * C67 * C56 / C55) +
                                      1.,
                                  2) *
                         C60 *
                         ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                          0.000001 * C67) *
                         0.02556 * std::sqrt(sigmaAa) / (C63 + std::sqrt(C66 + 1.)) -
                     C60 *
                         ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                          0.000001 * C67) *
                         std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) *
                         std::pow(C58, 2) *
                         ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                               C58 +
                           0.000002 * C67 * C56 / C55) +
                          1.) *
                         0.05112 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58)) /
                    std::pow(std::pow(C58, 2) * std::pow((std::log(C63 + std::sqrt(C66 + 1.)) *
                                                              0.02556 * std::sqrt(sigmaAa) / C58 +
                                                          0.000002 * C67 * C56 / C55) +
                                                             1.,
                                                         2),
                             2)) *
                   C77) +
              (((-C77 * C60 *
                 ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                 0.02556 * std::sqrt(sigmaAa) *
                 ((C63 + std::sqrt(C66 + 1.)) * C58 *
                      ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                       1.) *
                      -2 * std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                      std::pow(C58, 2) +
                  (C63 + std::sqrt(C66 + 1.)) * std::pow((std::log(C63 + std::sqrt(C66 + 1.)) *
                                                              0.02556 * std::sqrt(sigmaAa) / C58 +
                                                          0.000002 * C67 * C56 / C55) +
                                                             1.,
                                                         2))) /
                    std::pow((C63 + std::sqrt(C66 + 1.)) * C58 *
                                 std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                               std::sqrt(sigmaAa) / C58 +
                                           0.000002 * C67 * C56 / C55) +
                                              1.,
                                          2),
                             2) -
                ((C76 *
                      ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                       0.000001 * C67) *
                      std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                      (std::pow(C58, 2) * std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                                        std::sqrt(sigmaAa) / C58 +
                                                    0.000002 * C67 * C56 / C55) +
                                                       1.,
                                                   2)) +
                  C78 *
                      (std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaAa) / C58 +
                                 0.000002 * C67 * C56 / C55) +
                                    1.,
                                2) *
                           ((-C60 *
                             (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                         C65 * std::exp(-1.6455307846 * C66))) *
                             std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa)) /
                                std::pow(C58, 2) -
                            (-C60 *
                             ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                              0.000001 * C67) *
                             0.02556 * std::sqrt(sigmaAa) * 2 * std::sqrt(C66 + 1.) *
                             (C63 + std::sqrt(C66 + 1.))) /
                                std::pow(2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) *
                                             C58,
                                         2)) -
                       (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                         1.) *
                            C60 *
                            (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                        C65 * std::exp(-1.6455307846 * C66))) -
                        C60 *
                            ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                             0.000001 * C67) *
                            0.02556 * std::sqrt(sigmaAa) /
                            (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) *
                           ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                 C58 +
                             0.000002 * C67 * C56 / C55) +
                            1.) *
                           -2 * std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                           std::pow(C58, 2)) /
                      std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                    C58 +
                                0.000002 * C67 * C56 / C55) +
                                   1.,
                               4)) +
                 C79 *
                     (std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                    C58 +
                                0.000002 * C67 * C56 / C55) +
                                   1.,
                               2) *
                          0.000001 * C60 * std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                          std::sqrt(sigmaAa) / std::pow(C58, 2) -
                      (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                        1.) *
                           -0.000001 * C60 -
                       C60 *
                           ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                            0.000001 * C67) *
                           0.000002 * C56 / C55) *
                          ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                C58 +
                            0.000002 * C67 * C56 / C55) +
                           1.) *
                          -2 * std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                          std::pow(C58, 2)) /
                     std::pow(
                         (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                             1.,
                         4))) -
               (-C75 * C60 *
                ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) *
                (std::pow(C58, 2) *
                     ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                      1.) *
                     -2 * std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                     std::pow(C58, 2) +
                 2 * C58 *
                     std::pow(
                         (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                             1.,
                         2))) /
                   std::pow(std::pow(C58, 2) * std::pow((std::log(C63 + std::sqrt(C66 + 1.)) *
                                                             0.02556 * std::sqrt(sigmaAa) / C58 +
                                                         0.000002 * C67 * C56 / C55) +
                                                            1.,
                                                        2),
                            2)) *
                  C75) -
             (((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) /
                  ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                    0.000002 * C67 * C56 / C55) +
                   1.) +
              0.9305257363491) *
                 C93) +
            ((C77 * ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                  0.02556 * std::sqrt(sigmaAa) /
                  ((C63 + std::sqrt(C66 + 1.)) * C58 *
                   std::pow(
                       (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       2)) -
              (C78 *
                   (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                      0.000002 * C67 * C56 / C55) +
                     1.) *
                        (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                    C65 * std::exp(-1.6455307846 * C66))) -
                    ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                        0.02556 * std::sqrt(sigmaAa) /
                        (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) +
               C79 *
                   (-0.000001 *
                        ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                         1.) -
                    ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                        0.000002 * C56 / C55)) /
                  std::pow(
                      (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                          1.,
                      2)) -
             C75 * ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                 std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                 (std::pow(C58, 2) * std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                                   std::sqrt(sigmaAa) / C58 +
                                               0.000002 * C67 * C56 / C55) +
                                                  1.,
                                              2))) *
                C76) -
           ((((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
               0.000002 * C67 * C56 / C55) +
              1.) *
                 C60 *
                 (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                             C65 * std::exp(-1.6455307846 * C66))) -
             C60 * ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                 0.02556 * std::sqrt(sigmaAa) /
                 (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
                std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                             1.,
                         2) +
            0.) *
               C95) -
          ((((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
              0.000002 * C67 * C56 / C55) +
             1.) *
                -0.000001 * C60 -
            C60 * ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                0.000002 * C56 / C55) /
               std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        2) +
           0.) *
              C96) +
         (C60 * ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
              0.02556 * std::sqrt(sigmaAa) /
              ((C63 + std::sqrt(C66 + 1.)) * C58 *
               std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        2)) -
          0.) *
             C94) -
        (C60 * ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
             std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
             (std::pow(C58, 2) *
              std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       2)) +
         0.) *
            C92;

    matrix.upper[1] = 0;

    const double C86 = 1 / (2 * std::sqrt(sigmaAa) * C58);
    const double C87 = 2 * C63 * C86;
    const double C88 = 3.72 * std::pow(C63, 2.72) * C86;
    const double C97 = (-C75 * 2 * std::sqrt(sigmaAa)) / std::pow(2 * std::sqrt(sigmaAa) * C58, 2);
    const double C98 = 2 * C86 * C77 + 2 * C63 * C97;
    const double C99 = 10.1184 * std::pow(C63, 1.72) * C86 * C77 + 3.72 * std::pow(C63, 2.72) * C97;
    matrix.upper[2] =
        (((((((((((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                     0.02556 * std::log(C63 + std::sqrt(C66 + 1.)) /
                     (2 * std::sqrt(sigmaAa) * C58 *
                      std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                    C58 +
                                0.000002 * C67 * C56 / C55) +
                                   1.,
                               2)) -
                 C87 *
                     (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                       1.) *
                          (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                      C65 * std::exp(-1.6455307846 * C66))) -
                      ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                       0.000001 * C67) *
                          0.02556 * std::sqrt(sigmaAa) /
                          (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
                     std::pow(
                         (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                             1.,
                         2)) -
                C88 *
                    (-0.000001 * ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                       std::sqrt(sigmaAa) / C58 +
                                   0.000002 * C67 * C56 / C55) +
                                  1.) -
                     ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                      0.000001 * C67) *
                         0.000002 * C56 / C55) /
                    std::pow(
                        (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        2)) +
               C86 *
                   ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                   0.02556 * std::sqrt(sigmaAa) /
                   ((C63 + std::sqrt(C66 + 1.)) * C58 *
                    std::pow(
                        (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        2))) *
                  C76 +
              ((((2 * std::sqrt(sigmaAa) * C58 *
                      std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                    C58 +
                                0.000002 * C67 * C56 / C55) +
                                   1.,
                               2) *
                      (0.02556 * C60 *
                           ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                            0.000001 * C67) /
                           (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.))) +
                       C60 *
                           (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                       C65 * std::exp(-1.6455307846 * C66))) *
                           0.02556 * std::log(C63 + std::sqrt(C66 + 1.))) -
                  C60 *
                      ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                       0.000001 * C67) *
                      0.02556 * std::log(C63 + std::sqrt(C66 + 1.)) * std::sqrt(sigmaAa) * C58 *
                      ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                       1.) *
                      0.05112 * std::sqrt(sigmaAa) /
                      (std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
                     std::pow(2 * std::sqrt(sigmaAa) * C58 *
                                  std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                                std::sqrt(sigmaAa) / C58 +
                                            0.000002 * C67 * C56 / C55) +
                                               1.,
                                           2),
                              2) -
                 C87 *
                     (std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                    C58 +
                                0.000002 * C67 * C56 / C55) +
                                   1.,
                               2) *
                          ((C60 *
                                (0.00426 -
                                 (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                  C65 * std::exp(-1.6455307846 * C66))) *
                                0.02556 * std::sqrt(sigmaAa) /
                                (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58) -
                            ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                  C58 +
                              0.000002 * C67 * C56 / C55) +
                             1.) *
                                C60 *
                                ((C65 * C66 * 2.7077715630662916 * std::exp(-1.6455307846 * C66) +
                                  C65 * -1.6455307846 * std::exp(-1.6455307846 * C66)) +
                                 C65 * -1.6455307846 * std::exp(-1.6455307846 * C66))) -
                           (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58 * C60 *
                                (0.00426 -
                                 (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                  C65 * std::exp(-1.6455307846 * C66))) *
                                0.02556 * std::sqrt(sigmaAa) -
                            C60 *
                                ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                                 0.000001 * C67) *
                                0.02556 * std::sqrt(sigmaAa) * C58 *
                                (std::sqrt(C66 + 1.) + (C63 + std::sqrt(C66 + 1.))) /
                                std::sqrt(C66 + 1.)) /
                               std::pow(2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58,
                                        2)) -
                      (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                        1.) *
                           C60 *
                           (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                       C65 * std::exp(-1.6455307846 * C66))) -
                       C60 *
                           ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                            0.000001 * C67) *
                           0.02556 * std::sqrt(sigmaAa) /
                           (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) *
                          ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                C58 +
                            0.000002 * C67 * C56 / C55) +
                           1.) *
                          0.05112 * std::sqrt(sigmaAa) /
                          (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
                     std::pow(
                         (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                             1.,
                         4)) -
                C88 *
                    (std::pow(
                         (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                             1.,
                         2) *
                         (-0.000001 * C60 * 0.02556 * std::sqrt(sigmaAa) /
                              (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58) -
                          C60 *
                              (0.00426 -
                               (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                C65 * std::exp(-1.6455307846 * C66))) *
                              0.000002 * C56 / C55) -
                     (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                       1.) *
                          -0.000001 * C60 -
                      C60 *
                          ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                           0.000001 * C67) *
                          0.000002 * C56 / C55) *
                         ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                               C58 +
                           0.000002 * C67 * C56 / C55) +
                          1.) *
                         0.05112 * std::sqrt(sigmaAa) /
                         (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
                    std::pow(
                        (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        4)) +
               C86 *
                   ((C63 + std::sqrt(C66 + 1.)) * C58 *
                        std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaAa) / C58 +
                                  0.000002 * C67 * C56 / C55) +
                                     1.,
                                 2) *
                        C60 *
                        (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                    C65 * std::exp(-1.6455307846 * C66))) *
                        0.02556 * std::sqrt(sigmaAa) -
                    C60 *
                        ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                         0.000001 * C67) *
                        0.02556 * std::sqrt(sigmaAa) *
                        ((C63 + std::sqrt(C66 + 1.)) * C58 *
                             ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                   C58 +
                               0.000002 * C67 * C56 / C55) +
                              1.) *
                             0.05112 * std::sqrt(sigmaAa) /
                             (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58) +
                         std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                       std::sqrt(sigmaAa) / C58 +
                                   0.000002 * C67 * C56 / C55) +
                                      1.,
                                  2) *
                             C58 / (2 * std::sqrt(C66 + 1.)))) /
                   std::pow((C63 + std::sqrt(C66 + 1.)) * C58 *
                                std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                              std::sqrt(sigmaAa) / C58 +
                                          0.000002 * C67 * C56 / C55) +
                                             1.,
                                         2),
                            2)) *
                  C78) +
             ((((2 * std::sqrt(sigmaAa) * C58 *
                     std::pow(
                         (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                             1.,
                         2) *
                     -0.000001 * C60 * 0.02556 * std::log(C63 + std::sqrt(C66 + 1.)) -
                 C60 *
                     ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                      0.000001 * C67) *
                     0.02556 * std::log(C63 + std::sqrt(C66 + 1.)) * 2 * std::sqrt(sigmaAa) * C58 *
                     ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                      1.) *
                     0.000004 * C56 / C55) /
                    std::pow(2 * std::sqrt(sigmaAa) * C58 *
                                 std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                               std::sqrt(sigmaAa) / C58 +
                                           0.000002 * C67 * C56 / C55) +
                                              1.,
                                          2),
                             2) -
                C87 *
                    (std::pow(
                         (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                             1.,
                         2) *
                         (C60 *
                              (0.00426 -
                               (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                C65 * std::exp(-1.6455307846 * C66))) *
                              0.000002 * C56 / C55 -
                          -0.000001 * C60 * 0.02556 * std::sqrt(sigmaAa) /
                              (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) -
                     (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                       1.) *
                          C60 *
                          (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                      C65 * std::exp(-1.6455307846 * C66))) -
                      C60 *
                          ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                           0.000001 * C67) *
                          0.02556 * std::sqrt(sigmaAa) /
                          (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) *
                         ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                               C58 +
                           0.000002 * C67 * C56 / C55) +
                          1.) *
                         0.000004 * C56 / C55) /
                    std::pow(
                        (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        4)) -
               (-C88 *
                (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                   0.000002 * C67 * C56 / C55) +
                  1.) *
                     -0.000001 * C60 -
                 C60 *
                     ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                      0.000001 * C67) *
                     0.000002 * C56 / C55) *
                ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                  0.000002 * C67 * C56 / C55) +
                 1.) *
                0.000004 * C56 / C55) /
                   std::pow(
                       (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       4)) +
              C86 *
                  ((C63 + std::sqrt(C66 + 1.)) * C58 *
                       std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaAa) / C58 +
                                 0.000002 * C67 * C56 / C55) +
                                    1.,
                                2) *
                       -0.000001 * C60 * 0.02556 * std::sqrt(sigmaAa) -
                   C60 *
                       ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                        0.000001 * C67) *
                       0.02556 * std::sqrt(sigmaAa) * (C63 + std::sqrt(C66 + 1.)) * C58 *
                       ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                        1.) *
                       0.000004 * C56 / C55) /
                  std::pow((C63 + std::sqrt(C66 + 1.)) * C58 *
                               std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                             std::sqrt(sigmaAa) / C58 +
                                         0.000002 * C67 * C56 / C55) +
                                            1.,
                                        2),
                           2)) *
                 C79) +
            ((((2 * std::sqrt(sigmaAa) * C58 *
                    std::pow(
                        (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        2) *
                    0.02556 * C60 *
                    ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) /
                    (C63 + std::sqrt(C66 + 1.)) -
                C60 *
                    ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                    0.02556 * std::log(C63 + std::sqrt(C66 + 1.)) * 2 * std::sqrt(sigmaAa) * C58 *
                    ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                      0.000002 * C67 * C56 / C55) +
                     1.) *
                    0.05112 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58)) /
                   std::pow(2 * std::sqrt(sigmaAa) * C58 *
                                std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                              std::sqrt(sigmaAa) / C58 +
                                          0.000002 * C67 * C56 / C55) +
                                             1.,
                                         2),
                            2) -
               C87 *
                   (std::pow(
                        (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        2) *
                        (C60 *
                             (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                         C65 * std::exp(-1.6455307846 * C66))) *
                             0.02556 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58) -
                         (-C60 *
                          ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                           0.000001 * C67) *
                          0.02556 * std::sqrt(sigmaAa) * 2 * std::sqrt(C66 + 1.) * C58) /
                             std::pow(2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58,
                                      2)) -
                    (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                      1.) *
                         C60 *
                         (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                     C65 * std::exp(-1.6455307846 * C66))) -
                     C60 *
                         ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                          0.000001 * C67) *
                         0.02556 * std::sqrt(sigmaAa) /
                         (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) *
                        ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                         1.) *
                        0.05112 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58)) /
                   std::pow(
                       (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       4)) -
              C88 *
                  (std::pow(
                       (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       2) *
                       -0.000001 * C60 * 0.02556 * std::sqrt(sigmaAa) /
                       ((C63 + std::sqrt(C66 + 1.)) * C58) -
                   (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                      0.000002 * C67 * C56 / C55) +
                     1.) *
                        -0.000001 * C60 -
                    C60 *
                        ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                         0.000001 * C67) *
                        0.000002 * C56 / C55) *
                       ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                        1.) *
                       0.05112 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58)) /
                  std::pow(
                      (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                          1.,
                      4)) -
             C86 * C60 *
                 ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                 0.02556 * std::sqrt(sigmaAa) *
                 ((C63 + std::sqrt(C66 + 1.)) * C58 *
                      ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                       1.) *
                      0.05112 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58) +
                  C58 * std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaAa) / C58 +
                                  0.000002 * C67 * C56 / C55) +
                                     1.,
                                 2)) /
                 std::pow((C63 + std::sqrt(C66 + 1.)) * C58 *
                              std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                            std::sqrt(sigmaAa) / C58 +
                                        0.000002 * C67 * C56 / C55) +
                                           1.,
                                       2),
                          2)) *
                C77) +
           ((((-C60 *
               ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
               0.02556 * std::log(C63 + std::sqrt(C66 + 1.)) * 2 *
               (std::sqrt(sigmaAa) * C58 *
                    ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                      0.000002 * C67 * C56 / C55) +
                     1.) *
                    -2 * std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                    std::pow(C58, 2) +
                std::sqrt(sigmaAa) * std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                                   std::sqrt(sigmaAa) / C58 +
                                               0.000002 * C67 * C56 / C55) +
                                                  1.,
                                              2))) /
                  std::pow(2 * std::sqrt(sigmaAa) * C58 *
                               std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                             std::sqrt(sigmaAa) / C58 +
                                         0.000002 * C67 * C56 / C55) +
                                            1.,
                                        2),
                           2) -
              C87 *
                  (std::pow(
                       (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       2) *
                       ((-C60 *
                         (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                     C65 * std::exp(-1.6455307846 * C66))) *
                         std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa)) /
                            std::pow(C58, 2) -
                        (-C60 *
                         ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                          0.000001 * C67) *
                         0.02556 * std::sqrt(sigmaAa) * 2 * std::sqrt(C66 + 1.) *
                         (C63 + std::sqrt(C66 + 1.))) /
                            std::pow(2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58,
                                     2)) -
                   (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                      0.000002 * C67 * C56 / C55) +
                     1.) *
                        C60 *
                        (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                    C65 * std::exp(-1.6455307846 * C66))) -
                    C60 *
                        ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                         0.000001 * C67) *
                        0.02556 * std::sqrt(sigmaAa) /
                        (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) *
                       ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                        1.) *
                       -2 * std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                       std::pow(C58, 2)) /
                  std::pow(
                      (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                          1.,
                      4)) -
             C88 *
                 (std::pow(
                      (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                          1.,
                      2) *
                      0.000001 * C60 * std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                      std::sqrt(sigmaAa) / std::pow(C58, 2) -
                  (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                     0.000002 * C67 * C56 / C55) +
                    1.) *
                       -0.000001 * C60 -
                   C60 *
                       ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                        0.000001 * C67) *
                       0.000002 * C56 / C55) *
                      ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                       1.) *
                      -2 * std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                      std::pow(C58, 2)) /
                 std::pow(
                     (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                      0.000002 * C67 * C56 / C55) +
                         1.,
                     4)) -
            C86 * C60 *
                ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                0.02556 * std::sqrt(sigmaAa) *
                ((C63 + std::sqrt(C66 + 1.)) * C58 *
                     ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                      1.) *
                     -2 * std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                     std::pow(C58, 2) +
                 (C63 + std::sqrt(C66 + 1.)) * std::pow((std::log(C63 + std::sqrt(C66 + 1.)) *
                                                             0.02556 * std::sqrt(sigmaAa) / C58 +
                                                         0.000002 * C67 * C56 / C55) +
                                                            1.,
                                                        2)) /
                std::pow((C63 + std::sqrt(C66 + 1.)) * C58 *
                             std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                           std::sqrt(sigmaAa) / C58 +
                                       0.000002 * C67 * C56 / C55) +
                                          1.,
                                      2),
                         2)) *
               C75) -
          ((((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
              0.000002 * C67 * C56 / C55) +
             1.) *
                C60 *
                (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                            C65 * std::exp(-1.6455307846 * C66))) -
            C60 * ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                0.02556 * std::sqrt(sigmaAa) /
                (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
               std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        2) +
           0.) *
              C98) -
         ((((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
             0.000002 * C67 * C56 / C55) +
            1.) *
               -0.000001 * C60 -
           C60 * ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
               0.000002 * C56 / C55) /
              std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       2) +
          0.) *
             C99) +
        (C60 * ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
             0.02556 * std::sqrt(sigmaAa) /
             ((C63 + std::sqrt(C66 + 1.)) * C58 *
              std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       2)) -
         0.) *
            C97;

    matrix.upper[3] = 0;

    matrix.upper[4] = 0;

    const double C49 = rhoB + 1e-16;
    const double C54 = std::sqrt(sigmaBb);
    const double C59 = std::pow(C49, C52);
    const double C61 = std::pow(rhoB, C52);
    const double C64 = C54 / C59;
    const double C68 = std::pow(C64, 2);
    const double C69 = std::pow(C64, 3.72);
    const double C80 = 1;
    const double C81 = C52 * std::pow(C49, C52 - 1) * C80;
    const double C82 = C52 * std::pow(rhoB, C52 - 1);
    const double C83 = (-C81 * std::sqrt(sigmaBb)) / std::pow(C59, 2);
    const double C84 = 2 * C64 * C83;
    const double C85 = 3.72 * std::pow(C64, 2.72) * C83;
    const double C100 = C52 * (C52 - 1) * std::pow(C49, C52 - 2) * std::pow(C80, 2);
    const double C101 = C52 * (C52 - 1) * std::pow(rhoB, C52 - 2);
    const double C102 = C81 * C81 * std::sqrt(sigmaBb) * 2 * C59 / std::pow(C59, 4) -
                        C100 * std::sqrt(sigmaBb) / std::pow(C59, 2);
    const double C103 = 2 * std::pow(C83, 2) + 2 * C64 * C102;
    const double C104 =
        10.1184 * std::pow(C64, 1.72) * std::pow(C83, 2) + 3.72 * std::pow(C64, 2.72) * C102;
    matrix.upper[5] =
        ((((((((((C83 *
                      ((C64 + std::sqrt(C68 + 1.)) * C59 *
                           std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                         std::sqrt(sigmaBb) / C59 +
                                     0.000002 * C69 * C56 / C55) +
                                        1.,
                                    2) *
                           C61 *
                           (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                       C65 * std::exp(-1.6455307846 * C68))) *
                           0.02556 * std::sqrt(sigmaBb) -
                       C61 *
                           ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                            0.000001 * C69) *
                           0.02556 * std::sqrt(sigmaBb) *
                           ((C64 + std::sqrt(C68 + 1.)) * C59 *
                                ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaBb) / C59 +
                                  0.000002 * C69 * C56 / C55) +
                                 1.) *
                                0.05112 * std::sqrt(sigmaBb) /
                                (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59) +
                            std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                          std::sqrt(sigmaBb) / C59 +
                                      0.000002 * C69 * C56 / C55) +
                                         1.,
                                     2) *
                                C59 / (2 * std::sqrt(C68 + 1.)))) /
                      std::pow((C64 + std::sqrt(C68 + 1.)) * C59 *
                                   std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                                 std::sqrt(sigmaBb) / C59 +
                                             0.000002 * C69 * C56 / C55) +
                                                1.,
                                            2),
                               2) -
                  ((C82 *
                        (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                               C59 +
                           0.000002 * C69 * C56 / C55) +
                          1.) *
                             (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                         C65 * std::exp(-1.6455307846 * C68))) -
                         ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                          0.000001 * C69) *
                             0.02556 * std::sqrt(sigmaBb) /
                             (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
                        std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaBb) / C59 +
                                  0.000002 * C69 * C56 / C55) +
                                     1.,
                                 2) +
                    C84 *
                        (std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                       std::sqrt(sigmaBb) / C59 +
                                   0.000002 * C69 * C56 / C55) +
                                      1.,
                                  2) *
                             ((C61 *
                                   (0.00426 -
                                    (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                     C65 * std::exp(-1.6455307846 * C68))) *
                                   0.02556 * std::sqrt(sigmaBb) /
                                   (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59) -
                               ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaBb) / C59 +
                                 0.000002 * C69 * C56 / C55) +
                                1.) *
                                   C61 *
                                   ((C65 * C68 * 2.7077715630662916 *
                                         std::exp(-1.6455307846 * C68) +
                                     C65 * -1.6455307846 * std::exp(-1.6455307846 * C68)) +
                                    C65 * -1.6455307846 * std::exp(-1.6455307846 * C68))) -
                              (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59 * C61 *
                                   (0.00426 -
                                    (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                     C65 * std::exp(-1.6455307846 * C68))) *
                                   0.02556 * std::sqrt(sigmaBb) -
                               C61 *
                                   ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                                    0.000001 * C69) *
                                   0.02556 * std::sqrt(sigmaBb) * C59 *
                                   (std::sqrt(C68 + 1.) + (C64 + std::sqrt(C68 + 1.))) /
                                   std::sqrt(C68 + 1.)) /
                                  std::pow(2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) *
                                               C59,
                                           2)) -
                         (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                C59 +
                            0.000002 * C69 * C56 / C55) +
                           1.) *
                              C61 *
                              (0.00426 -
                               (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                C65 * std::exp(-1.6455307846 * C68))) -
                          C61 *
                              ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                               0.000001 * C69) *
                              0.02556 * std::sqrt(sigmaBb) /
                              (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) *
                             ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                   C59 +
                               0.000002 * C69 * C56 / C55) +
                              1.) *
                             0.05112 * std::sqrt(sigmaBb) /
                             (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
                        std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaBb) / C59 +
                                  0.000002 * C69 * C56 / C55) +
                                     1.,
                                 4)) +
                   C85 *
                       (std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaBb) / C59 +
                                  0.000002 * C69 * C56 / C55) +
                                     1.,
                                 2) *
                            (-0.000001 * C61 * 0.02556 * std::sqrt(sigmaBb) /
                                 (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59) -
                             C61 *
                                 (0.00426 -
                                  (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                   C65 * std::exp(-1.6455307846 * C68))) *
                                 0.000002 * C56 / C55) -
                        (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                               C59 +
                           0.000002 * C69 * C56 / C55) +
                          1.) *
                             -0.000001 * C61 -
                         C61 *
                             ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                              0.000001 * C69) *
                             0.000002 * C56 / C55) *
                            ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                  C59 +
                              0.000002 * C69 * C56 / C55) +
                             1.) *
                            0.05112 * std::sqrt(sigmaBb) /
                            (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
                       std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaBb) / C59 +
                                 0.000002 * C69 * C56 / C55) +
                                    1.,
                                4))) -
                 C81 *
                     (std::pow(C59, 2) *
                          std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                        std::sqrt(sigmaBb) / C59 +
                                    0.000002 * C69 * C56 / C55) +
                                       1.,
                                   2) *
                          (C61 *
                               ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                                0.000001 * C69) *
                               0.02556 * std::sqrt(sigmaBb) /
                               (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.))) +
                           C61 *
                               (0.00426 -
                                (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                 C65 * std::exp(-1.6455307846 * C68))) *
                               std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb)) -
                      C61 *
                          ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                           0.000001 * C69) *
                          std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) *
                          std::pow(C59, 2) *
                          ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                C59 +
                            0.000002 * C69 * C56 / C55) +
                           1.) *
                          0.05112 * std::sqrt(sigmaBb) /
                          (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
                     std::pow(std::pow(C59, 2) * std::pow((std::log(C64 + std::sqrt(C68 + 1.)) *
                                                               0.02556 * std::sqrt(sigmaBb) / C59 +
                                                           0.000002 * C69 * C56 / C55) +
                                                              1.,
                                                          2),
                              2)) *
                    C84 +
                (((C85 *
                       (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                         1.) *
                            -0.000001 * C61 -
                        C61 *
                            ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                             0.000001 * C69) *
                            0.000002 * C56 / C55) *
                       ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                        1.) *
                       0.000004 * C56 /
                       (C55 * std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                            std::sqrt(sigmaBb) / C59 +
                                        0.000002 * C69 * C56 / C55) +
                                           1.,
                                       4)) -
                   (C82 *
                        (-0.000001 * ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                           std::sqrt(sigmaBb) / C59 +
                                       0.000002 * C69 * C56 / C55) +
                                      1.) -
                         ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                          0.000001 * C69) *
                             0.000002 * C56 / C55) /
                        std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaBb) / C59 +
                                  0.000002 * C69 * C56 / C55) +
                                     1.,
                                 2) +
                    C84 *
                        (std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                       std::sqrt(sigmaBb) / C59 +
                                   0.000002 * C69 * C56 / C55) +
                                      1.,
                                  2) *
                             (C61 *
                                  (0.00426 -
                                   (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                    C65 * std::exp(-1.6455307846 * C68))) *
                                  0.000002 * C56 / C55 -
                              -0.000001 * C61 * 0.02556 * std::sqrt(sigmaBb) /
                                  (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) -
                         (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                C59 +
                            0.000002 * C69 * C56 / C55) +
                           1.) *
                              C61 *
                              (0.00426 -
                               (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                C65 * std::exp(-1.6455307846 * C68))) -
                          C61 *
                              ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                               0.000001 * C69) *
                              0.02556 * std::sqrt(sigmaBb) /
                              (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) *
                             ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                   C59 +
                               0.000002 * C69 * C56 / C55) +
                              1.) *
                             0.000004 * C56 / C55) /
                        std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaBb) / C59 +
                                  0.000002 * C69 * C56 / C55) +
                                     1.,
                                 4))) +
                  C83 *
                      ((C64 + std::sqrt(C68 + 1.)) * C59 *
                           std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                         std::sqrt(sigmaBb) / C59 +
                                     0.000002 * C69 * C56 / C55) +
                                        1.,
                                    2) *
                           -0.000001 * C61 * 0.02556 * std::sqrt(sigmaBb) -
                       C61 *
                           ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                            0.000001 * C69) *
                           0.02556 * std::sqrt(sigmaBb) * (C64 + std::sqrt(C68 + 1.)) * C59 *
                           ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                 C59 +
                             0.000002 * C69 * C56 / C55) +
                            1.) *
                           0.000004 * C56 / C55) /
                      std::pow((C64 + std::sqrt(C68 + 1.)) * C59 *
                                   std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                                 std::sqrt(sigmaBb) / C59 +
                                             0.000002 * C69 * C56 / C55) +
                                                1.,
                                            2),
                               2)) -
                 C81 *
                     (std::pow(C59, 2) *
                          std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                        std::sqrt(sigmaBb) / C59 +
                                    0.000002 * C69 * C56 / C55) +
                                       1.,
                                   2) *
                          -0.000001 * C61 * std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                          std::sqrt(sigmaBb) -
                      C61 *
                          ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                           0.000001 * C69) *
                          std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) *
                          std::pow(C59, 2) *
                          ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                C59 +
                            0.000002 * C69 * C56 / C55) +
                           1.) *
                          0.000004 * C56 / C55) /
                     std::pow(std::pow(C59, 2) * std::pow((std::log(C64 + std::sqrt(C68 + 1.)) *
                                                               0.02556 * std::sqrt(sigmaBb) / C59 +
                                                           0.000002 * C69 * C56 / C55) +
                                                              1.,
                                                          2),
                              2)) *
                    C85) +
               ((((C82 *
                       ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                        0.000001 * C69) *
                       0.02556 * std::sqrt(sigmaBb) /
                       ((C64 + std::sqrt(C68 + 1.)) * C59 *
                        std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaBb) / C59 +
                                  0.000002 * C69 * C56 / C55) +
                                     1.,
                                 2)) -
                   C84 *
                       (std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaBb) / C59 +
                                  0.000002 * C69 * C56 / C55) +
                                     1.,
                                 2) *
                            (C61 *
                                 (0.00426 -
                                  (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                   C65 * std::exp(-1.6455307846 * C68))) *
                                 0.02556 * std::sqrt(sigmaBb) /
                                 ((C64 + std::sqrt(C68 + 1.)) * C59) -
                             (-C61 *
                              ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                               0.000001 * C69) *
                              0.02556 * std::sqrt(sigmaBb) * 2 * std::sqrt(C68 + 1.) * C59) /
                                 std::pow(2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) *
                                              C59,
                                          2)) -
                        (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                               C59 +
                           0.000002 * C69 * C56 / C55) +
                          1.) *
                             C61 *
                             (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                         C65 * std::exp(-1.6455307846 * C68))) -
                         C61 *
                             ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                              0.000001 * C69) *
                             0.02556 * std::sqrt(sigmaBb) /
                             (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) *
                            ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                  C59 +
                              0.000002 * C69 * C56 / C55) +
                             1.) *
                            0.05112 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59)) /
                       std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaBb) / C59 +
                                 0.000002 * C69 * C56 / C55) +
                                    1.,
                                4)) -
                  C85 *
                      (std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaBb) / C59 +
                                 0.000002 * C69 * C56 / C55) +
                                    1.,
                                2) *
                           -0.000001 * C61 * 0.02556 * std::sqrt(sigmaBb) /
                           ((C64 + std::sqrt(C68 + 1.)) * C59) -
                       (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                         1.) *
                            -0.000001 * C61 -
                        C61 *
                            ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                             0.000001 * C69) *
                            0.000002 * C56 / C55) *
                           ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                 C59 +
                             0.000002 * C69 * C56 / C55) +
                            1.) *
                           0.05112 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59)) /
                      std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                    C59 +
                                0.000002 * C69 * C56 / C55) +
                                   1.,
                               4)) -
                 C83 * C61 *
                     ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                      0.000001 * C69) *
                     0.02556 * std::sqrt(sigmaBb) *
                     ((C64 + std::sqrt(C68 + 1.)) * C59 *
                          ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                C59 +
                            0.000002 * C69 * C56 / C55) +
                           1.) *
                          0.05112 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59) +
                      C59 * std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                          std::sqrt(sigmaBb) / C59 +
                                      0.000002 * C69 * C56 / C55) +
                                         1.,
                                     2)) /
                     std::pow((C64 + std::sqrt(C68 + 1.)) * C59 *
                                  std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                                std::sqrt(sigmaBb) / C59 +
                                            0.000002 * C69 * C56 / C55) +
                                               1.,
                                           2),
                              2)) -
                C81 *
                    (std::pow(C59, 2) *
                         std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                       std::sqrt(sigmaBb) / C59 +
                                   0.000002 * C69 * C56 / C55) +
                                      1.,
                                  2) *
                         C61 *
                         ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                          0.000001 * C69) *
                         0.02556 * std::sqrt(sigmaBb) / (C64 + std::sqrt(C68 + 1.)) -
                     C61 *
                         ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                          0.000001 * C69) *
                         std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) *
                         std::pow(C59, 2) *
                         ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                               C59 +
                           0.000002 * C69 * C56 / C55) +
                          1.) *
                         0.05112 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59)) /
                    std::pow(std::pow(C59, 2) * std::pow((std::log(C64 + std::sqrt(C68 + 1.)) *
                                                              0.02556 * std::sqrt(sigmaBb) / C59 +
                                                          0.000002 * C69 * C56 / C55) +
                                                             1.,
                                                         2),
                             2)) *
                   C83) +
              (((-C83 * C61 *
                 ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                 0.02556 * std::sqrt(sigmaBb) *
                 ((C64 + std::sqrt(C68 + 1.)) * C59 *
                      ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                       1.) *
                      -2 * std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                      std::pow(C59, 2) +
                  (C64 + std::sqrt(C68 + 1.)) * std::pow((std::log(C64 + std::sqrt(C68 + 1.)) *
                                                              0.02556 * std::sqrt(sigmaBb) / C59 +
                                                          0.000002 * C69 * C56 / C55) +
                                                             1.,
                                                         2))) /
                    std::pow((C64 + std::sqrt(C68 + 1.)) * C59 *
                                 std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                               std::sqrt(sigmaBb) / C59 +
                                           0.000002 * C69 * C56 / C55) +
                                              1.,
                                          2),
                             2) -
                ((C82 *
                      ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                       0.000001 * C69) *
                      std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                      (std::pow(C59, 2) * std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                                        std::sqrt(sigmaBb) / C59 +
                                                    0.000002 * C69 * C56 / C55) +
                                                       1.,
                                                   2)) +
                  C84 *
                      (std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaBb) / C59 +
                                 0.000002 * C69 * C56 / C55) +
                                    1.,
                                2) *
                           ((-C61 *
                             (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                         C65 * std::exp(-1.6455307846 * C68))) *
                             std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb)) /
                                std::pow(C59, 2) -
                            (-C61 *
                             ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                              0.000001 * C69) *
                             0.02556 * std::sqrt(sigmaBb) * 2 * std::sqrt(C68 + 1.) *
                             (C64 + std::sqrt(C68 + 1.))) /
                                std::pow(2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) *
                                             C59,
                                         2)) -
                       (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                         1.) *
                            C61 *
                            (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                        C65 * std::exp(-1.6455307846 * C68))) -
                        C61 *
                            ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                             0.000001 * C69) *
                            0.02556 * std::sqrt(sigmaBb) /
                            (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) *
                           ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                 C59 +
                             0.000002 * C69 * C56 / C55) +
                            1.) *
                           -2 * std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                           std::pow(C59, 2)) /
                      std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                    C59 +
                                0.000002 * C69 * C56 / C55) +
                                   1.,
                               4)) +
                 C85 *
                     (std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                    C59 +
                                0.000002 * C69 * C56 / C55) +
                                   1.,
                               2) *
                          0.000001 * C61 * std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                          std::sqrt(sigmaBb) / std::pow(C59, 2) -
                      (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                        1.) *
                           -0.000001 * C61 -
                       C61 *
                           ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                            0.000001 * C69) *
                           0.000002 * C56 / C55) *
                          ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                C59 +
                            0.000002 * C69 * C56 / C55) +
                           1.) *
                          -2 * std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                          std::pow(C59, 2)) /
                     std::pow(
                         (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                             1.,
                         4))) -
               (-C81 * C61 *
                ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) *
                (std::pow(C59, 2) *
                     ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                      1.) *
                     -2 * std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                     std::pow(C59, 2) +
                 2 * C59 *
                     std::pow(
                         (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                             1.,
                         2))) /
                   std::pow(std::pow(C59, 2) * std::pow((std::log(C64 + std::sqrt(C68 + 1.)) *
                                                             0.02556 * std::sqrt(sigmaBb) / C59 +
                                                         0.000002 * C69 * C56 / C55) +
                                                            1.,
                                                        2),
                            2)) *
                  C81) -
             (((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) /
                  ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                    0.000002 * C69 * C56 / C55) +
                   1.) +
              0.9305257363491) *
                 C101) +
            ((C83 * ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                  0.02556 * std::sqrt(sigmaBb) /
                  ((C64 + std::sqrt(C68 + 1.)) * C59 *
                   std::pow(
                       (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       2)) -
              (C84 *
                   (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                      0.000002 * C69 * C56 / C55) +
                     1.) *
                        (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                    C65 * std::exp(-1.6455307846 * C68))) -
                    ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                        0.02556 * std::sqrt(sigmaBb) /
                        (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) +
               C85 *
                   (-0.000001 *
                        ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                         1.) -
                    ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                        0.000002 * C56 / C55)) /
                  std::pow(
                      (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                          1.,
                      2)) -
             C81 * ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                 std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                 (std::pow(C59, 2) * std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                                   std::sqrt(sigmaBb) / C59 +
                                               0.000002 * C69 * C56 / C55) +
                                                  1.,
                                              2))) *
                C82) -
           ((((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
               0.000002 * C69 * C56 / C55) +
              1.) *
                 C61 *
                 (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                             C65 * std::exp(-1.6455307846 * C68))) -
             C61 * ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                 0.02556 * std::sqrt(sigmaBb) /
                 (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
                std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                             1.,
                         2) +
            0.) *
               C103) -
          ((((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
              0.000002 * C69 * C56 / C55) +
             1.) *
                -0.000001 * C61 -
            C61 * ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                0.000002 * C56 / C55) /
               std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        2) +
           0.) *
              C104) +
         (C61 * ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
              0.02556 * std::sqrt(sigmaBb) /
              ((C64 + std::sqrt(C68 + 1.)) * C59 *
               std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        2)) -
          0.) *
             C102) -
        (C61 * ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
             std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
             (std::pow(C59, 2) *
              std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       2)) +
         0.) *
            C100;

    matrix.upper[6] = 0;

    matrix.upper[7] = 0;

    const double C89 = 1 / (2 * std::sqrt(sigmaBb) * C59);
    const double C90 = 2 * C64 * C89;
    const double C91 = 3.72 * std::pow(C64, 2.72) * C89;
    const double C105 = (-C81 * 2 * std::sqrt(sigmaBb)) / std::pow(2 * std::sqrt(sigmaBb) * C59, 2);
    const double C106 = 2 * C89 * C83 + 2 * C64 * C105;
    const double C107 =
        10.1184 * std::pow(C64, 1.72) * C89 * C83 + 3.72 * std::pow(C64, 2.72) * C105;
    matrix.upper[8] =
        (((((((((((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                     0.02556 * std::log(C64 + std::sqrt(C68 + 1.)) /
                     (2 * std::sqrt(sigmaBb) * C59 *
                      std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                    C59 +
                                0.000002 * C69 * C56 / C55) +
                                   1.,
                               2)) -
                 C90 *
                     (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                       1.) *
                          (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                      C65 * std::exp(-1.6455307846 * C68))) -
                      ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                       0.000001 * C69) *
                          0.02556 * std::sqrt(sigmaBb) /
                          (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
                     std::pow(
                         (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                             1.,
                         2)) -
                C91 *
                    (-0.000001 * ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                       std::sqrt(sigmaBb) / C59 +
                                   0.000002 * C69 * C56 / C55) +
                                  1.) -
                     ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                      0.000001 * C69) *
                         0.000002 * C56 / C55) /
                    std::pow(
                        (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        2)) +
               C89 *
                   ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                   0.02556 * std::sqrt(sigmaBb) /
                   ((C64 + std::sqrt(C68 + 1.)) * C59 *
                    std::pow(
                        (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        2))) *
                  C82 +
              ((((2 * std::sqrt(sigmaBb) * C59 *
                      std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                    C59 +
                                0.000002 * C69 * C56 / C55) +
                                   1.,
                               2) *
                      (0.02556 * C61 *
                           ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                            0.000001 * C69) /
                           (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.))) +
                       C61 *
                           (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                       C65 * std::exp(-1.6455307846 * C68))) *
                           0.02556 * std::log(C64 + std::sqrt(C68 + 1.))) -
                  C61 *
                      ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                       0.000001 * C69) *
                      0.02556 * std::log(C64 + std::sqrt(C68 + 1.)) * std::sqrt(sigmaBb) * C59 *
                      ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                       1.) *
                      0.05112 * std::sqrt(sigmaBb) /
                      (std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
                     std::pow(2 * std::sqrt(sigmaBb) * C59 *
                                  std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                                std::sqrt(sigmaBb) / C59 +
                                            0.000002 * C69 * C56 / C55) +
                                               1.,
                                           2),
                              2) -
                 C90 *
                     (std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                    C59 +
                                0.000002 * C69 * C56 / C55) +
                                   1.,
                               2) *
                          ((C61 *
                                (0.00426 -
                                 (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                  C65 * std::exp(-1.6455307846 * C68))) *
                                0.02556 * std::sqrt(sigmaBb) /
                                (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59) -
                            ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                  C59 +
                              0.000002 * C69 * C56 / C55) +
                             1.) *
                                C61 *
                                ((C65 * C68 * 2.7077715630662916 * std::exp(-1.6455307846 * C68) +
                                  C65 * -1.6455307846 * std::exp(-1.6455307846 * C68)) +
                                 C65 * -1.6455307846 * std::exp(-1.6455307846 * C68))) -
                           (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59 * C61 *
                                (0.00426 -
                                 (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                  C65 * std::exp(-1.6455307846 * C68))) *
                                0.02556 * std::sqrt(sigmaBb) -
                            C61 *
                                ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                                 0.000001 * C69) *
                                0.02556 * std::sqrt(sigmaBb) * C59 *
                                (std::sqrt(C68 + 1.) + (C64 + std::sqrt(C68 + 1.))) /
                                std::sqrt(C68 + 1.)) /
                               std::pow(2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59,
                                        2)) -
                      (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                        1.) *
                           C61 *
                           (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                       C65 * std::exp(-1.6455307846 * C68))) -
                       C61 *
                           ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                            0.000001 * C69) *
                           0.02556 * std::sqrt(sigmaBb) /
                           (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) *
                          ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                C59 +
                            0.000002 * C69 * C56 / C55) +
                           1.) *
                          0.05112 * std::sqrt(sigmaBb) /
                          (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
                     std::pow(
                         (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                             1.,
                         4)) -
                C91 *
                    (std::pow(
                         (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                             1.,
                         2) *
                         (-0.000001 * C61 * 0.02556 * std::sqrt(sigmaBb) /
                              (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59) -
                          C61 *
                              (0.00426 -
                               (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                C65 * std::exp(-1.6455307846 * C68))) *
                              0.000002 * C56 / C55) -
                     (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                       1.) *
                          -0.000001 * C61 -
                      C61 *
                          ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                           0.000001 * C69) *
                          0.000002 * C56 / C55) *
                         ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                               C59 +
                           0.000002 * C69 * C56 / C55) +
                          1.) *
                         0.05112 * std::sqrt(sigmaBb) /
                         (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
                    std::pow(
                        (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        4)) +
               C89 *
                   ((C64 + std::sqrt(C68 + 1.)) * C59 *
                        std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaBb) / C59 +
                                  0.000002 * C69 * C56 / C55) +
                                     1.,
                                 2) *
                        C61 *
                        (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                    C65 * std::exp(-1.6455307846 * C68))) *
                        0.02556 * std::sqrt(sigmaBb) -
                    C61 *
                        ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                         0.000001 * C69) *
                        0.02556 * std::sqrt(sigmaBb) *
                        ((C64 + std::sqrt(C68 + 1.)) * C59 *
                             ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                   C59 +
                               0.000002 * C69 * C56 / C55) +
                              1.) *
                             0.05112 * std::sqrt(sigmaBb) /
                             (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59) +
                         std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                       std::sqrt(sigmaBb) / C59 +
                                   0.000002 * C69 * C56 / C55) +
                                      1.,
                                  2) *
                             C59 / (2 * std::sqrt(C68 + 1.)))) /
                   std::pow((C64 + std::sqrt(C68 + 1.)) * C59 *
                                std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                              std::sqrt(sigmaBb) / C59 +
                                          0.000002 * C69 * C56 / C55) +
                                             1.,
                                         2),
                            2)) *
                  C84) +
             ((((2 * std::sqrt(sigmaBb) * C59 *
                     std::pow(
                         (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                             1.,
                         2) *
                     -0.000001 * C61 * 0.02556 * std::log(C64 + std::sqrt(C68 + 1.)) -
                 C61 *
                     ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                      0.000001 * C69) *
                     0.02556 * std::log(C64 + std::sqrt(C68 + 1.)) * 2 * std::sqrt(sigmaBb) * C59 *
                     ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                      1.) *
                     0.000004 * C56 / C55) /
                    std::pow(2 * std::sqrt(sigmaBb) * C59 *
                                 std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                               std::sqrt(sigmaBb) / C59 +
                                           0.000002 * C69 * C56 / C55) +
                                              1.,
                                          2),
                             2) -
                C90 *
                    (std::pow(
                         (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                             1.,
                         2) *
                         (C61 *
                              (0.00426 -
                               (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                C65 * std::exp(-1.6455307846 * C68))) *
                              0.000002 * C56 / C55 -
                          -0.000001 * C61 * 0.02556 * std::sqrt(sigmaBb) /
                              (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) -
                     (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                       1.) *
                          C61 *
                          (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                      C65 * std::exp(-1.6455307846 * C68))) -
                      C61 *
                          ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                           0.000001 * C69) *
                          0.02556 * std::sqrt(sigmaBb) /
                          (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) *
                         ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                               C59 +
                           0.000002 * C69 * C56 / C55) +
                          1.) *
                         0.000004 * C56 / C55) /
                    std::pow(
                        (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        4)) -
               (-C91 *
                (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                   0.000002 * C69 * C56 / C55) +
                  1.) *
                     -0.000001 * C61 -
                 C61 *
                     ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                      0.000001 * C69) *
                     0.000002 * C56 / C55) *
                ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                  0.000002 * C69 * C56 / C55) +
                 1.) *
                0.000004 * C56 / C55) /
                   std::pow(
                       (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       4)) +
              C89 *
                  ((C64 + std::sqrt(C68 + 1.)) * C59 *
                       std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaBb) / C59 +
                                 0.000002 * C69 * C56 / C55) +
                                    1.,
                                2) *
                       -0.000001 * C61 * 0.02556 * std::sqrt(sigmaBb) -
                   C61 *
                       ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                        0.000001 * C69) *
                       0.02556 * std::sqrt(sigmaBb) * (C64 + std::sqrt(C68 + 1.)) * C59 *
                       ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                        1.) *
                       0.000004 * C56 / C55) /
                  std::pow((C64 + std::sqrt(C68 + 1.)) * C59 *
                               std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                             std::sqrt(sigmaBb) / C59 +
                                         0.000002 * C69 * C56 / C55) +
                                            1.,
                                        2),
                           2)) *
                 C85) +
            ((((2 * std::sqrt(sigmaBb) * C59 *
                    std::pow(
                        (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        2) *
                    0.02556 * C61 *
                    ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) /
                    (C64 + std::sqrt(C68 + 1.)) -
                C61 *
                    ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                    0.02556 * std::log(C64 + std::sqrt(C68 + 1.)) * 2 * std::sqrt(sigmaBb) * C59 *
                    ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                      0.000002 * C69 * C56 / C55) +
                     1.) *
                    0.05112 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59)) /
                   std::pow(2 * std::sqrt(sigmaBb) * C59 *
                                std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                              std::sqrt(sigmaBb) / C59 +
                                          0.000002 * C69 * C56 / C55) +
                                             1.,
                                         2),
                            2) -
               C90 *
                   (std::pow(
                        (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        2) *
                        (C61 *
                             (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                         C65 * std::exp(-1.6455307846 * C68))) *
                             0.02556 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59) -
                         (-C61 *
                          ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                           0.000001 * C69) *
                          0.02556 * std::sqrt(sigmaBb) * 2 * std::sqrt(C68 + 1.) * C59) /
                             std::pow(2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59,
                                      2)) -
                    (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                      1.) *
                         C61 *
                         (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                     C65 * std::exp(-1.6455307846 * C68))) -
                     C61 *
                         ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                          0.000001 * C69) *
                         0.02556 * std::sqrt(sigmaBb) /
                         (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) *
                        ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                         1.) *
                        0.05112 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59)) /
                   std::pow(
                       (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       4)) -
              C91 *
                  (std::pow(
                       (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       2) *
                       -0.000001 * C61 * 0.02556 * std::sqrt(sigmaBb) /
                       ((C64 + std::sqrt(C68 + 1.)) * C59) -
                   (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                      0.000002 * C69 * C56 / C55) +
                     1.) *
                        -0.000001 * C61 -
                    C61 *
                        ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                         0.000001 * C69) *
                        0.000002 * C56 / C55) *
                       ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                        1.) *
                       0.05112 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59)) /
                  std::pow(
                      (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                          1.,
                      4)) -
             C89 * C61 *
                 ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                 0.02556 * std::sqrt(sigmaBb) *
                 ((C64 + std::sqrt(C68 + 1.)) * C59 *
                      ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                       1.) *
                      0.05112 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59) +
                  C59 * std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaBb) / C59 +
                                  0.000002 * C69 * C56 / C55) +
                                     1.,
                                 2)) /
                 std::pow((C64 + std::sqrt(C68 + 1.)) * C59 *
                              std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                            std::sqrt(sigmaBb) / C59 +
                                        0.000002 * C69 * C56 / C55) +
                                           1.,
                                       2),
                          2)) *
                C83) +
           ((((-C61 *
               ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
               0.02556 * std::log(C64 + std::sqrt(C68 + 1.)) * 2 *
               (std::sqrt(sigmaBb) * C59 *
                    ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                      0.000002 * C69 * C56 / C55) +
                     1.) *
                    -2 * std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                    std::pow(C59, 2) +
                std::sqrt(sigmaBb) * std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                                   std::sqrt(sigmaBb) / C59 +
                                               0.000002 * C69 * C56 / C55) +
                                                  1.,
                                              2))) /
                  std::pow(2 * std::sqrt(sigmaBb) * C59 *
                               std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                             std::sqrt(sigmaBb) / C59 +
                                         0.000002 * C69 * C56 / C55) +
                                            1.,
                                        2),
                           2) -
              C90 *
                  (std::pow(
                       (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       2) *
                       ((-C61 *
                         (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                     C65 * std::exp(-1.6455307846 * C68))) *
                         std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb)) /
                            std::pow(C59, 2) -
                        (-C61 *
                         ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                          0.000001 * C69) *
                         0.02556 * std::sqrt(sigmaBb) * 2 * std::sqrt(C68 + 1.) *
                         (C64 + std::sqrt(C68 + 1.))) /
                            std::pow(2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59,
                                     2)) -
                   (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                      0.000002 * C69 * C56 / C55) +
                     1.) *
                        C61 *
                        (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                    C65 * std::exp(-1.6455307846 * C68))) -
                    C61 *
                        ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                         0.000001 * C69) *
                        0.02556 * std::sqrt(sigmaBb) /
                        (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) *
                       ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                        1.) *
                       -2 * std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                       std::pow(C59, 2)) /
                  std::pow(
                      (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                          1.,
                      4)) -
             C91 *
                 (std::pow(
                      (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                          1.,
                      2) *
                      0.000001 * C61 * std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                      std::sqrt(sigmaBb) / std::pow(C59, 2) -
                  (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                     0.000002 * C69 * C56 / C55) +
                    1.) *
                       -0.000001 * C61 -
                   C61 *
                       ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                        0.000001 * C69) *
                       0.000002 * C56 / C55) *
                      ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                       1.) *
                      -2 * std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                      std::pow(C59, 2)) /
                 std::pow(
                     (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                      0.000002 * C69 * C56 / C55) +
                         1.,
                     4)) -
            C89 * C61 *
                ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                0.02556 * std::sqrt(sigmaBb) *
                ((C64 + std::sqrt(C68 + 1.)) * C59 *
                     ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                      1.) *
                     -2 * std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                     std::pow(C59, 2) +
                 (C64 + std::sqrt(C68 + 1.)) * std::pow((std::log(C64 + std::sqrt(C68 + 1.)) *
                                                             0.02556 * std::sqrt(sigmaBb) / C59 +
                                                         0.000002 * C69 * C56 / C55) +
                                                            1.,
                                                        2)) /
                std::pow((C64 + std::sqrt(C68 + 1.)) * C59 *
                             std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                           std::sqrt(sigmaBb) / C59 +
                                       0.000002 * C69 * C56 / C55) +
                                          1.,
                                      2),
                         2)) *
               C81) -
          ((((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
              0.000002 * C69 * C56 / C55) +
             1.) *
                C61 *
                (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                            C65 * std::exp(-1.6455307846 * C68))) -
            C61 * ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                0.02556 * std::sqrt(sigmaBb) /
                (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
               std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        2) +
           0.) *
              C106) -
         ((((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
             0.000002 * C69 * C56 / C55) +
            1.) *
               -0.000001 * C61 -
           C61 * ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
               0.000002 * C56 / C55) /
              std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       2) +
          0.) *
             C107) +
        (C61 * ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
             0.02556 * std::sqrt(sigmaBb) /
             ((C64 + std::sqrt(C68 + 1.)) * C59 *
              std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       2)) -
         0.) *
            C105;

    const double C108 = (-C58 / std::sqrt(sigmaAa)) / std::pow(2 * std::sqrt(sigmaAa) * C58, 2);
    const double C109 = 2 * std::pow(C86, 2) + 2 * C63 * C108;
    const double C110 =
        10.1184 * std::pow(C63, 1.72) * std::pow(C86, 2) + 3.72 * std::pow(C63, 2.72) * C108;
    matrix.upper[9] =
        (((((((((-C60 *
                 ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                 0.02556 * std::log(C63 + std::sqrt(C66 + 1.)) * 2 *
                 (std::sqrt(sigmaAa) * C58 *
                      ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                       1.) *
                      0.05112 * std::log(C63 + std::sqrt(C66 + 1.)) /
                      (2 * std::sqrt(sigmaAa) * C58) +
                  std::pow(
                      (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                          1.,
                      2) *
                      C58 / (2 * std::sqrt(sigmaAa)))) /
                    std::pow(2 * std::sqrt(sigmaAa) * C58 *
                                 std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                               std::sqrt(sigmaAa) / C58 +
                                           0.000002 * C67 * C56 / C55) +
                                              1.,
                                          2),
                             2) -
                C87 *
                    (std::pow(
                         (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                             1.,
                         2) *
                         (C60 *
                              (0.00426 -
                               (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                C65 * std::exp(-1.6455307846 * C66))) *
                              0.02556 * std::log(C63 + std::sqrt(C66 + 1.)) /
                              (2 * std::sqrt(sigmaAa) * C58) -
                          0.02556 * C60 *
                              ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                               0.000001 * C67) /
                              (2 * std::sqrt(sigmaAa) * 2 * std::sqrt(C66 + 1.) *
                               (C63 + std::sqrt(C66 + 1.)) * C58)) -
                     (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                       1.) *
                          C60 *
                          (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                      C65 * std::exp(-1.6455307846 * C66))) -
                      C60 *
                          ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                           0.000001 * C67) *
                          0.02556 * std::sqrt(sigmaAa) /
                          (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) *
                         ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                               C58 +
                           0.000002 * C67 * C56 / C55) +
                          1.) *
                         0.05112 * std::log(C63 + std::sqrt(C66 + 1.)) /
                         (2 * std::sqrt(sigmaAa) * C58)) /
                    std::pow(
                        (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        4)) -
               C88 *
                   (std::pow(
                        (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        2) *
                        -0.000001 * C60 * 0.02556 * std::log(C63 + std::sqrt(C66 + 1.)) /
                        (2 * std::sqrt(sigmaAa) * C58) -
                    (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                      1.) *
                         -0.000001 * C60 -
                     C60 *
                         ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                          0.000001 * C67) *
                         0.000002 * C56 / C55) *
                        ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                         1.) *
                        0.05112 * std::log(C63 + std::sqrt(C66 + 1.)) /
                        (2 * std::sqrt(sigmaAa) * C58)) /
                   std::pow(
                       (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       4)) +
              C86 *
                  ((C63 + std::sqrt(C66 + 1.)) * C58 *
                       std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaAa) / C58 +
                                 0.000002 * C67 * C56 / C55) +
                                    1.,
                                2) *
                       0.02556 * C60 *
                       ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                        0.000001 * C67) /
                       (2 * std::sqrt(sigmaAa)) -
                   C60 *
                       ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                        0.000001 * C67) *
                       0.02556 * std::sqrt(sigmaAa) * (C63 + std::sqrt(C66 + 1.)) * C58 *
                       ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                        1.) *
                       0.05112 * std::log(C63 + std::sqrt(C66 + 1.)) /
                       (2 * std::sqrt(sigmaAa) * C58)) /
                  std::pow((C63 + std::sqrt(C66 + 1.)) * C58 *
                               std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                             std::sqrt(sigmaAa) / C58 +
                                         0.000002 * C67 * C56 / C55) +
                                            1.,
                                        2),
                           2)) +
             ((((2 * std::sqrt(sigmaAa) * C58 *
                     std::pow(
                         (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                             1.,
                         2) *
                     (0.02556 * C60 *
                          ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                           0.000001 * C67) /
                          (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.))) +
                      C60 *
                          (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                      C65 * std::exp(-1.6455307846 * C66))) *
                          0.02556 * std::log(C63 + std::sqrt(C66 + 1.))) -
                 C60 *
                     ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                      0.000001 * C67) *
                     0.02556 * std::log(C63 + std::sqrt(C66 + 1.)) * std::sqrt(sigmaAa) * C58 *
                     ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                      1.) *
                     0.05112 * std::sqrt(sigmaAa) /
                     (std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
                    std::pow(2 * std::sqrt(sigmaAa) * C58 *
                                 std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                               std::sqrt(sigmaAa) / C58 +
                                           0.000002 * C67 * C56 / C55) +
                                              1.,
                                          2),
                             2) -
                C87 *
                    (std::pow(
                         (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                             1.,
                         2) *
                         ((C60 *
                               (0.00426 -
                                (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                 C65 * std::exp(-1.6455307846 * C66))) *
                               0.02556 * std::sqrt(sigmaAa) /
                               (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58) -
                           ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                 C58 +
                             0.000002 * C67 * C56 / C55) +
                            1.) *
                               C60 *
                               ((C65 * C66 * 2.7077715630662916 * std::exp(-1.6455307846 * C66) +
                                 C65 * -1.6455307846 * std::exp(-1.6455307846 * C66)) +
                                C65 * -1.6455307846 * std::exp(-1.6455307846 * C66))) -
                          (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58 * C60 *
                               (0.00426 -
                                (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                 C65 * std::exp(-1.6455307846 * C66))) *
                               0.02556 * std::sqrt(sigmaAa) -
                           C60 *
                               ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                                0.000001 * C67) *
                               0.02556 * std::sqrt(sigmaAa) * C58 *
                               (std::sqrt(C66 + 1.) + (C63 + std::sqrt(C66 + 1.))) /
                               std::sqrt(C66 + 1.)) /
                              std::pow(2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58,
                                       2)) -
                     (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                       1.) *
                          C60 *
                          (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                      C65 * std::exp(-1.6455307846 * C66))) -
                      C60 *
                          ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                           0.000001 * C67) *
                          0.02556 * std::sqrt(sigmaAa) /
                          (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) *
                         ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                               C58 +
                           0.000002 * C67 * C56 / C55) +
                          1.) *
                         0.05112 * std::sqrt(sigmaAa) /
                         (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
                    std::pow(
                        (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        4)) -
               C88 *
                   (std::pow(
                        (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        2) *
                        (-0.000001 * C60 * 0.02556 * std::sqrt(sigmaAa) /
                             (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58) -
                         C60 *
                             (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                         C65 * std::exp(-1.6455307846 * C66))) *
                             0.000002 * C56 / C55) -
                    (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                      1.) *
                         -0.000001 * C60 -
                     C60 *
                         ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                          0.000001 * C67) *
                         0.000002 * C56 / C55) *
                        ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                         1.) *
                        0.05112 * std::sqrt(sigmaAa) /
                        (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
                   std::pow(
                       (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       4)) +
              C86 *
                  ((C63 + std::sqrt(C66 + 1.)) * C58 *
                       std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaAa) / C58 +
                                 0.000002 * C67 * C56 / C55) +
                                    1.,
                                2) *
                       C60 *
                       (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                   C65 * std::exp(-1.6455307846 * C66))) *
                       0.02556 * std::sqrt(sigmaAa) -
                   C60 *
                       ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                        0.000001 * C67) *
                       0.02556 * std::sqrt(sigmaAa) *
                       ((C63 + std::sqrt(C66 + 1.)) * C58 *
                            ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                  C58 +
                              0.000002 * C67 * C56 / C55) +
                             1.) *
                            0.05112 * std::sqrt(sigmaAa) /
                            (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58) +
                        std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaAa) / C58 +
                                  0.000002 * C67 * C56 / C55) +
                                     1.,
                                 2) *
                            C58 / (2 * std::sqrt(C66 + 1.)))) /
                  std::pow((C63 + std::sqrt(C66 + 1.)) * C58 *
                               std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                             std::sqrt(sigmaAa) / C58 +
                                         0.000002 * C67 * C56 / C55) +
                                            1.,
                                        2),
                           2)) *
                 C87) +
            ((((2 * std::sqrt(sigmaAa) * C58 *
                    std::pow(
                        (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        2) *
                    -0.000001 * C60 * 0.02556 * std::log(C63 + std::sqrt(C66 + 1.)) -
                C60 *
                    ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                    0.02556 * std::log(C63 + std::sqrt(C66 + 1.)) * 2 * std::sqrt(sigmaAa) * C58 *
                    ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                      0.000002 * C67 * C56 / C55) +
                     1.) *
                    0.000004 * C56 / C55) /
                   std::pow(2 * std::sqrt(sigmaAa) * C58 *
                                std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                              std::sqrt(sigmaAa) / C58 +
                                          0.000002 * C67 * C56 / C55) +
                                             1.,
                                         2),
                            2) -
               C87 *
                   (std::pow(
                        (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        2) *
                        (C60 *
                             (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                         C65 * std::exp(-1.6455307846 * C66))) *
                             0.000002 * C56 / C55 -
                         -0.000001 * C60 * 0.02556 * std::sqrt(sigmaAa) /
                             (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) -
                    (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                      1.) *
                         C60 *
                         (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                     C65 * std::exp(-1.6455307846 * C66))) -
                     C60 *
                         ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                          0.000001 * C67) *
                         0.02556 * std::sqrt(sigmaAa) /
                         (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) *
                        ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                          0.000002 * C67 * C56 / C55) +
                         1.) *
                        0.000004 * C56 / C55) /
                   std::pow(
                       (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       4)) -
              (-C88 *
               (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                  0.000002 * C67 * C56 / C55) +
                 1.) *
                    -0.000001 * C60 -
                C60 *
                    ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                    0.000002 * C56 / C55) *
               ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                 0.000002 * C67 * C56 / C55) +
                1.) *
               0.000004 * C56 / C55) /
                  std::pow(
                      (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                          1.,
                      4)) +
             C86 *
                 ((C63 + std::sqrt(C66 + 1.)) * C58 *
                      std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) /
                                    C58 +
                                0.000002 * C67 * C56 / C55) +
                                   1.,
                               2) *
                      -0.000001 * C60 * 0.02556 * std::sqrt(sigmaAa) -
                  C60 *
                      ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                       0.000001 * C67) *
                      0.02556 * std::sqrt(sigmaAa) * (C63 + std::sqrt(C66 + 1.)) * C58 *
                      ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                       1.) *
                      0.000004 * C56 / C55) /
                 std::pow((C63 + std::sqrt(C66 + 1.)) * C58 *
                              std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                            std::sqrt(sigmaAa) / C58 +
                                        0.000002 * C67 * C56 / C55) +
                                           1.,
                                       2),
                          2)) *
                C88) +
           ((((2 * std::sqrt(sigmaAa) * C58 *
                   std::pow(
                       (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       2) *
                   0.02556 * C60 *
                   ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) /
                   (C63 + std::sqrt(C66 + 1.)) -
               C60 *
                   ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                   0.02556 * std::log(C63 + std::sqrt(C66 + 1.)) * 2 * std::sqrt(sigmaAa) * C58 *
                   ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                     0.000002 * C67 * C56 / C55) +
                    1.) *
                   0.05112 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58)) /
                  std::pow(2 * std::sqrt(sigmaAa) * C58 *
                               std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                             std::sqrt(sigmaAa) / C58 +
                                         0.000002 * C67 * C56 / C55) +
                                            1.,
                                        2),
                           2) -
              C87 *
                  (std::pow(
                       (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       2) *
                       (C60 *
                            (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                        C65 * std::exp(-1.6455307846 * C66))) *
                            0.02556 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58) -
                        (-C60 *
                         ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                          0.000001 * C67) *
                         0.02556 * std::sqrt(sigmaAa) * 2 * std::sqrt(C66 + 1.) * C58) /
                            std::pow(2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58,
                                     2)) -
                   (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                      0.000002 * C67 * C56 / C55) +
                     1.) *
                        C60 *
                        (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                                    C65 * std::exp(-1.6455307846 * C66))) -
                    C60 *
                        ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                         0.000001 * C67) *
                        0.02556 * std::sqrt(sigmaAa) /
                        (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) *
                       ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                        1.) *
                       0.05112 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58)) /
                  std::pow(
                      (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                          1.,
                      4)) -
             C88 *
                 (std::pow(
                      (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                          1.,
                      2) *
                      -0.000001 * C60 * 0.02556 * std::sqrt(sigmaAa) /
                      ((C63 + std::sqrt(C66 + 1.)) * C58) -
                  (((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                     0.000002 * C67 * C56 / C55) +
                    1.) *
                       -0.000001 * C60 -
                   C60 *
                       ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) -
                        0.000001 * C67) *
                       0.000002 * C56 / C55) *
                      ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                       1.) *
                      0.05112 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58)) /
                 std::pow(
                     (std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                      0.000002 * C67 * C56 / C55) +
                         1.,
                     4)) -
            C86 * C60 *
                ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                0.02556 * std::sqrt(sigmaAa) *
                ((C63 + std::sqrt(C66 + 1.)) * C58 *
                     ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                       0.000002 * C67 * C56 / C55) +
                      1.) *
                     0.05112 * std::sqrt(sigmaAa) / ((C63 + std::sqrt(C66 + 1.)) * C58) +
                 C58 * std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaAa) / C58 +
                                 0.000002 * C67 * C56 / C55) +
                                    1.,
                                2)) /
                std::pow((C63 + std::sqrt(C66 + 1.)) * C58 *
                             std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 *
                                           std::sqrt(sigmaAa) / C58 +
                                       0.000002 * C67 * C56 / C55) +
                                          1.,
                                      2),
                         2)) *
               C86) -
          ((((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
              0.000002 * C67 * C56 / C55) +
             1.) *
                C60 *
                (0.00426 - (C65 * C66 * -1.6455307846 * std::exp(-1.6455307846 * C66) +
                            C65 * std::exp(-1.6455307846 * C66))) -
            C60 * ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
                0.02556 * std::sqrt(sigmaAa) /
                (2 * std::sqrt(C66 + 1.) * (C63 + std::sqrt(C66 + 1.)) * C58)) /
               std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                         0.000002 * C67 * C56 / C55) +
                            1.,
                        2) +
           0.) *
              C109) -
         ((((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
             0.000002 * C67 * C56 / C55) +
            1.) *
               -0.000001 * C60 -
           C60 * ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
               0.000002 * C56 / C55) /
              std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       2) +
          0.) *
             C110) +
        (C60 * ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) *
             0.02556 * std::sqrt(sigmaAa) /
             ((C63 + std::sqrt(C66 + 1.)) * C58 *
              std::pow((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * std::sqrt(sigmaAa) / C58 +
                        0.000002 * C67 * C56 / C55) +
                           1.,
                       2)) -
         0.) *
            C108;

    matrix.upper[10] = 0;

    matrix.upper[11] = 0;

    matrix.upper[12] = 0;

    matrix.upper[13] = 0;

    const double C111 = (-C59 / std::sqrt(sigmaBb)) / std::pow(2 * std::sqrt(sigmaBb) * C59, 2);
    const double C112 = 2 * std::pow(C89, 2) + 2 * C64 * C111;
    const double C113 =
        10.1184 * std::pow(C64, 1.72) * std::pow(C89, 2) + 3.72 * std::pow(C64, 2.72) * C111;
    matrix.upper[14] =
        (((((((((-C61 *
                 ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                 0.02556 * std::log(C64 + std::sqrt(C68 + 1.)) * 2 *
                 (std::sqrt(sigmaBb) * C59 *
                      ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                       1.) *
                      0.05112 * std::log(C64 + std::sqrt(C68 + 1.)) /
                      (2 * std::sqrt(sigmaBb) * C59) +
                  std::pow(
                      (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                          1.,
                      2) *
                      C59 / (2 * std::sqrt(sigmaBb)))) /
                    std::pow(2 * std::sqrt(sigmaBb) * C59 *
                                 std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                               std::sqrt(sigmaBb) / C59 +
                                           0.000002 * C69 * C56 / C55) +
                                              1.,
                                          2),
                             2) -
                C90 *
                    (std::pow(
                         (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                             1.,
                         2) *
                         (C61 *
                              (0.00426 -
                               (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                C65 * std::exp(-1.6455307846 * C68))) *
                              0.02556 * std::log(C64 + std::sqrt(C68 + 1.)) /
                              (2 * std::sqrt(sigmaBb) * C59) -
                          0.02556 * C61 *
                              ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                               0.000001 * C69) /
                              (2 * std::sqrt(sigmaBb) * 2 * std::sqrt(C68 + 1.) *
                               (C64 + std::sqrt(C68 + 1.)) * C59)) -
                     (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                       1.) *
                          C61 *
                          (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                      C65 * std::exp(-1.6455307846 * C68))) -
                      C61 *
                          ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                           0.000001 * C69) *
                          0.02556 * std::sqrt(sigmaBb) /
                          (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) *
                         ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                               C59 +
                           0.000002 * C69 * C56 / C55) +
                          1.) *
                         0.05112 * std::log(C64 + std::sqrt(C68 + 1.)) /
                         (2 * std::sqrt(sigmaBb) * C59)) /
                    std::pow(
                        (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        4)) -
               C91 *
                   (std::pow(
                        (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        2) *
                        -0.000001 * C61 * 0.02556 * std::log(C64 + std::sqrt(C68 + 1.)) /
                        (2 * std::sqrt(sigmaBb) * C59) -
                    (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                      1.) *
                         -0.000001 * C61 -
                     C61 *
                         ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                          0.000001 * C69) *
                         0.000002 * C56 / C55) *
                        ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                         1.) *
                        0.05112 * std::log(C64 + std::sqrt(C68 + 1.)) /
                        (2 * std::sqrt(sigmaBb) * C59)) /
                   std::pow(
                       (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       4)) +
              C89 *
                  ((C64 + std::sqrt(C68 + 1.)) * C59 *
                       std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaBb) / C59 +
                                 0.000002 * C69 * C56 / C55) +
                                    1.,
                                2) *
                       0.02556 * C61 *
                       ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                        0.000001 * C69) /
                       (2 * std::sqrt(sigmaBb)) -
                   C61 *
                       ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                        0.000001 * C69) *
                       0.02556 * std::sqrt(sigmaBb) * (C64 + std::sqrt(C68 + 1.)) * C59 *
                       ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                        1.) *
                       0.05112 * std::log(C64 + std::sqrt(C68 + 1.)) /
                       (2 * std::sqrt(sigmaBb) * C59)) /
                  std::pow((C64 + std::sqrt(C68 + 1.)) * C59 *
                               std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                             std::sqrt(sigmaBb) / C59 +
                                         0.000002 * C69 * C56 / C55) +
                                            1.,
                                        2),
                           2)) +
             ((((2 * std::sqrt(sigmaBb) * C59 *
                     std::pow(
                         (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                             1.,
                         2) *
                     (0.02556 * C61 *
                          ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                           0.000001 * C69) /
                          (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.))) +
                      C61 *
                          (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                      C65 * std::exp(-1.6455307846 * C68))) *
                          0.02556 * std::log(C64 + std::sqrt(C68 + 1.))) -
                 C61 *
                     ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                      0.000001 * C69) *
                     0.02556 * std::log(C64 + std::sqrt(C68 + 1.)) * std::sqrt(sigmaBb) * C59 *
                     ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                      1.) *
                     0.05112 * std::sqrt(sigmaBb) /
                     (std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
                    std::pow(2 * std::sqrt(sigmaBb) * C59 *
                                 std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                               std::sqrt(sigmaBb) / C59 +
                                           0.000002 * C69 * C56 / C55) +
                                              1.,
                                          2),
                             2) -
                C90 *
                    (std::pow(
                         (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                             1.,
                         2) *
                         ((C61 *
                               (0.00426 -
                                (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                 C65 * std::exp(-1.6455307846 * C68))) *
                               0.02556 * std::sqrt(sigmaBb) /
                               (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59) -
                           ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                 C59 +
                             0.000002 * C69 * C56 / C55) +
                            1.) *
                               C61 *
                               ((C65 * C68 * 2.7077715630662916 * std::exp(-1.6455307846 * C68) +
                                 C65 * -1.6455307846 * std::exp(-1.6455307846 * C68)) +
                                C65 * -1.6455307846 * std::exp(-1.6455307846 * C68))) -
                          (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59 * C61 *
                               (0.00426 -
                                (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                 C65 * std::exp(-1.6455307846 * C68))) *
                               0.02556 * std::sqrt(sigmaBb) -
                           C61 *
                               ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                                0.000001 * C69) *
                               0.02556 * std::sqrt(sigmaBb) * C59 *
                               (std::sqrt(C68 + 1.) + (C64 + std::sqrt(C68 + 1.))) /
                               std::sqrt(C68 + 1.)) /
                              std::pow(2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59,
                                       2)) -
                     (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                       1.) *
                          C61 *
                          (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                      C65 * std::exp(-1.6455307846 * C68))) -
                      C61 *
                          ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                           0.000001 * C69) *
                          0.02556 * std::sqrt(sigmaBb) /
                          (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) *
                         ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                               C59 +
                           0.000002 * C69 * C56 / C55) +
                          1.) *
                         0.05112 * std::sqrt(sigmaBb) /
                         (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
                    std::pow(
                        (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        4)) -
               C91 *
                   (std::pow(
                        (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        2) *
                        (-0.000001 * C61 * 0.02556 * std::sqrt(sigmaBb) /
                             (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59) -
                         C61 *
                             (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                         C65 * std::exp(-1.6455307846 * C68))) *
                             0.000002 * C56 / C55) -
                    (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                      1.) *
                         -0.000001 * C61 -
                     C61 *
                         ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                          0.000001 * C69) *
                         0.000002 * C56 / C55) *
                        ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                         1.) *
                        0.05112 * std::sqrt(sigmaBb) /
                        (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
                   std::pow(
                       (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       4)) +
              C89 *
                  ((C64 + std::sqrt(C68 + 1.)) * C59 *
                       std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaBb) / C59 +
                                 0.000002 * C69 * C56 / C55) +
                                    1.,
                                2) *
                       C61 *
                       (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                   C65 * std::exp(-1.6455307846 * C68))) *
                       0.02556 * std::sqrt(sigmaBb) -
                   C61 *
                       ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                        0.000001 * C69) *
                       0.02556 * std::sqrt(sigmaBb) *
                       ((C64 + std::sqrt(C68 + 1.)) * C59 *
                            ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                  C59 +
                              0.000002 * C69 * C56 / C55) +
                             1.) *
                            0.05112 * std::sqrt(sigmaBb) /
                            (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59) +
                        std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                      std::sqrt(sigmaBb) / C59 +
                                  0.000002 * C69 * C56 / C55) +
                                     1.,
                                 2) *
                            C59 / (2 * std::sqrt(C68 + 1.)))) /
                  std::pow((C64 + std::sqrt(C68 + 1.)) * C59 *
                               std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                             std::sqrt(sigmaBb) / C59 +
                                         0.000002 * C69 * C56 / C55) +
                                            1.,
                                        2),
                           2)) *
                 C90) +
            ((((2 * std::sqrt(sigmaBb) * C59 *
                    std::pow(
                        (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        2) *
                    -0.000001 * C61 * 0.02556 * std::log(C64 + std::sqrt(C68 + 1.)) -
                C61 *
                    ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                    0.02556 * std::log(C64 + std::sqrt(C68 + 1.)) * 2 * std::sqrt(sigmaBb) * C59 *
                    ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                      0.000002 * C69 * C56 / C55) +
                     1.) *
                    0.000004 * C56 / C55) /
                   std::pow(2 * std::sqrt(sigmaBb) * C59 *
                                std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                              std::sqrt(sigmaBb) / C59 +
                                          0.000002 * C69 * C56 / C55) +
                                             1.,
                                         2),
                            2) -
               C90 *
                   (std::pow(
                        (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        2) *
                        (C61 *
                             (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                         C65 * std::exp(-1.6455307846 * C68))) *
                             0.000002 * C56 / C55 -
                         -0.000001 * C61 * 0.02556 * std::sqrt(sigmaBb) /
                             (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) -
                    (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                      1.) *
                         C61 *
                         (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                     C65 * std::exp(-1.6455307846 * C68))) -
                     C61 *
                         ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                          0.000001 * C69) *
                         0.02556 * std::sqrt(sigmaBb) /
                         (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) *
                        ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                          0.000002 * C69 * C56 / C55) +
                         1.) *
                        0.000004 * C56 / C55) /
                   std::pow(
                       (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       4)) -
              (-C91 *
               (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                  0.000002 * C69 * C56 / C55) +
                 1.) *
                    -0.000001 * C61 -
                C61 *
                    ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                    0.000002 * C56 / C55) *
               ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                 0.000002 * C69 * C56 / C55) +
                1.) *
               0.000004 * C56 / C55) /
                  std::pow(
                      (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                          1.,
                      4)) +
             C89 *
                 ((C64 + std::sqrt(C68 + 1.)) * C59 *
                      std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) /
                                    C59 +
                                0.000002 * C69 * C56 / C55) +
                                   1.,
                               2) *
                      -0.000001 * C61 * 0.02556 * std::sqrt(sigmaBb) -
                  C61 *
                      ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                       0.000001 * C69) *
                      0.02556 * std::sqrt(sigmaBb) * (C64 + std::sqrt(C68 + 1.)) * C59 *
                      ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                       1.) *
                      0.000004 * C56 / C55) /
                 std::pow((C64 + std::sqrt(C68 + 1.)) * C59 *
                              std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                            std::sqrt(sigmaBb) / C59 +
                                        0.000002 * C69 * C56 / C55) +
                                           1.,
                                       2),
                          2)) *
                C91) +
           ((((2 * std::sqrt(sigmaBb) * C59 *
                   std::pow(
                       (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       2) *
                   0.02556 * C61 *
                   ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) /
                   (C64 + std::sqrt(C68 + 1.)) -
               C61 *
                   ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                   0.02556 * std::log(C64 + std::sqrt(C68 + 1.)) * 2 * std::sqrt(sigmaBb) * C59 *
                   ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                     0.000002 * C69 * C56 / C55) +
                    1.) *
                   0.05112 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59)) /
                  std::pow(2 * std::sqrt(sigmaBb) * C59 *
                               std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                             std::sqrt(sigmaBb) / C59 +
                                         0.000002 * C69 * C56 / C55) +
                                            1.,
                                        2),
                           2) -
              C90 *
                  (std::pow(
                       (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       2) *
                       (C61 *
                            (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                        C65 * std::exp(-1.6455307846 * C68))) *
                            0.02556 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59) -
                        (-C61 *
                         ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                          0.000001 * C69) *
                         0.02556 * std::sqrt(sigmaBb) * 2 * std::sqrt(C68 + 1.) * C59) /
                            std::pow(2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59,
                                     2)) -
                   (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                      0.000002 * C69 * C56 / C55) +
                     1.) *
                        C61 *
                        (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                                    C65 * std::exp(-1.6455307846 * C68))) -
                    C61 *
                        ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                         0.000001 * C69) *
                        0.02556 * std::sqrt(sigmaBb) /
                        (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) *
                       ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                        1.) *
                       0.05112 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59)) /
                  std::pow(
                      (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                          1.,
                      4)) -
             C91 *
                 (std::pow(
                      (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                          1.,
                      2) *
                      -0.000001 * C61 * 0.02556 * std::sqrt(sigmaBb) /
                      ((C64 + std::sqrt(C68 + 1.)) * C59) -
                  (((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                     0.000002 * C69 * C56 / C55) +
                    1.) *
                       -0.000001 * C61 -
                   C61 *
                       ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) -
                        0.000001 * C69) *
                       0.000002 * C56 / C55) *
                      ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                       1.) *
                      0.05112 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59)) /
                 std::pow(
                     (std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                      0.000002 * C69 * C56 / C55) +
                         1.,
                     4)) -
            C89 * C61 *
                ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                0.02556 * std::sqrt(sigmaBb) *
                ((C64 + std::sqrt(C68 + 1.)) * C59 *
                     ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                       0.000002 * C69 * C56 / C55) +
                      1.) *
                     0.05112 * std::sqrt(sigmaBb) / ((C64 + std::sqrt(C68 + 1.)) * C59) +
                 C59 * std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                     std::sqrt(sigmaBb) / C59 +
                                 0.000002 * C69 * C56 / C55) +
                                    1.,
                                2)) /
                std::pow((C64 + std::sqrt(C68 + 1.)) * C59 *
                             std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 *
                                           std::sqrt(sigmaBb) / C59 +
                                       0.000002 * C69 * C56 / C55) +
                                          1.,
                                      2),
                         2)) *
               C89) -
          ((((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
              0.000002 * C69 * C56 / C55) +
             1.) *
                C61 *
                (0.00426 - (C65 * C68 * -1.6455307846 * std::exp(-1.6455307846 * C68) +
                            C65 * std::exp(-1.6455307846 * C68))) -
            C61 * ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
                0.02556 * std::sqrt(sigmaBb) /
                (2 * std::sqrt(C68 + 1.) * (C64 + std::sqrt(C68 + 1.)) * C59)) /
               std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                         0.000002 * C69 * C56 / C55) +
                            1.,
                        2) +
           0.) *
              C112) -
         ((((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
             0.000002 * C69 * C56 / C55) +
            1.) *
               -0.000001 * C61 -
           C61 * ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
               0.000002 * C56 / C55) /
              std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       2) +
          0.) *
             C113) +
        (C61 * ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) *
             0.02556 * std::sqrt(sigmaBb) /
             ((C64 + std::sqrt(C68 + 1.)) * C59 *
              std::pow((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * std::sqrt(sigmaBb) / C59 +
                        0.000002 * C69 * C56 / C55) +
                           1.,
                       2)) -
         0.) *
            C111;
}

} // namespace excgrid
