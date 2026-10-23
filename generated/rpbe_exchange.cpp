// excgrid codegen source: the Hammer-Hansen-Norskov 1999 revised PBE GGA
// exchange functional (RPBE).  Fresh authorship.
//
// The enhancement factor Fx(s) = 1 + kappa (1 - exp(-mu s^2 / kappa)) has
// kappa = 0.804 and mu = beta pi^2 / 3, the PBE relation, evaluated with
// this tree's own full-precision PBE beta (X'PbeCorrBeta).  That product is
// 0.2195149727645171 - the standard PBE mu, and the same value the tree's
// pbe and revpbe rules carry - and it is what RPBE uses: RPBE keeps PBE's
// kappa and mu and changes only the enhancement's FORM, from PBE's rational
// expression to this exponential one.
// Verified against the oracle by excgrid/tools/verify_pyscf.py.
// Regenerate with tools/regenerate.py.

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

XcKernelValue RpbeExchange(
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

    result.exc = -((0.804 * (1. - std::exp(-0.2195149727645171 *
                                           std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) /
                                                        (2. * (C79 * C71 + 1e-16)),
                                                    2) /
                                           0.804)) +
                    1.) *
                       rhoB * 3. * C79 +
                   (0.804 * (1. - std::exp(-0.2195149727645171 *
                                           std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) /
                                                        (2. * (C78 * C70 + 1e-16)),
                                                    2) /
                                           0.804)) +
                    1.) *
                       rhoA * 3. * C78) /
                 (4. * Pi);

    const double C81 = 2.;
    const double C82 = C73 * C81;
    const double C83 = 3. * C82;
    const double C84 = C72 * std::pow(C76 + 6. * std::pow(Pi, 2) * SigmaGuard, C72 - 1) * C83;

    result.vrhoA =
        -(C81 * rhoA * 3. * C78 * 0.804 *
              std::exp(
                  -0.2195149727645171 *
                  std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) / (2. * (C78 * C70 + 1e-16)),
                           2) /
                  0.804) *
              -0.8780598910580684 * C78 * (4. * sigmaAa + 4. * SigmaGuard) /
              (1.608 * (C78 * C70 + 1e-16) * std::pow(2. * (C78 * C70 + 1e-16), 2) * 4. * Pi) +
          (C84 * ((0.804 * (1. - std::exp(-0.2195149727645171 *
                                          std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) /
                                                       (2. * (C78 * C70 + 1e-16)),
                                                   2) /
                                          0.804)) +
                   1.) *
                      3. * rhoA +
                  rhoA * 3. * C78 * 0.804 *
                      std::exp(-0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) /
                                            (2. * (C78 * C70 + 1e-16)),
                                        2) /
                               0.804) *
                      -0.8780598910580684 * C70 * (4. * sigmaAa + 4. * SigmaGuard) /
                      (1.608 * (C78 * C70 + 1e-16) * std::pow(2. * (C78 * C70 + 1e-16), 2))) +
           (0.804 * (1. - std::exp(-0.2195149727645171 *
                                   std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) /
                                                (2. * (C78 * C70 + 1e-16)),
                                            2) /
                                   0.804)) +
            1.) *
               3. * C78) /
              (4. * Pi));

    const double C87 = 2.;
    const double C88 = C73 * C87;
    const double C89 = 3. * C88;
    const double C90 = C72 * std::pow(C77 + 6. * std::pow(Pi, 2) * SigmaGuard, C72 - 1) * C89;

    result.vrhoB =
        -(C87 * rhoB * 3. * C79 * 0.804 *
              std::exp(
                  -0.2195149727645171 *
                  std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) / (2. * (C79 * C71 + 1e-16)),
                           2) /
                  0.804) *
              -0.8780598910580684 * C79 * (4. * sigmaBb + 4. * SigmaGuard) /
              (1.608 * (C79 * C71 + 1e-16) * std::pow(2. * (C79 * C71 + 1e-16), 2) * 4. * Pi) +
          (C90 * ((0.804 * (1. - std::exp(-0.2195149727645171 *
                                          std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) /
                                                       (2. * (C79 * C71 + 1e-16)),
                                                   2) /
                                          0.804)) +
                   1.) *
                      3. * rhoB +
                  rhoB * 3. * C79 * 0.804 *
                      std::exp(-0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) /
                                            (2. * (C79 * C71 + 1e-16)),
                                        2) /
                               0.804) *
                      -0.8780598910580684 * C71 * (4. * sigmaBb + 4. * SigmaGuard) /
                      (1.608 * (C79 * C71 + 1e-16) * std::pow(2. * (C79 * C71 + 1e-16), 2))) +
           (0.804 * (1. - std::exp(-0.2195149727645171 *
                                   std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) /
                                                (2. * (C79 * C71 + 1e-16)),
                                            2) /
                                   0.804)) +
            1.) *
               3. * C79) /
              (4. * Pi));

    result.vsigmaAa =
        -1.7561197821161368 *
        std::exp(
            -0.2195149727645171 *
            std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) / (2. * (C78 * C70 + 1e-16)), 2) /
            0.804) *
        0.804 * C78 * 3. * rhoA /
        (Pi * 4. * (C78 * C70 + 1e-16) * 2. * 2 * (C78 * C70 + 1e-16) * 1.608);

    result.vsigmaAb = 0;

    result.vsigmaBb =
        -1.7561197821161368 *
        std::exp(
            -0.2195149727645171 *
            std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) / (2. * (C79 * C71 + 1e-16)), 2) /
            0.804) *
        0.804 * C79 * 3. * rhoB /
        (Pi * 4. * (C79 * C71 + 1e-16) * 2. * 2 * (C79 * C71 + 1e-16) * 1.608);

    return result;
}

void RpbeExchangeSecondDerivatives(double rhoA,
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
        C68 *
            ((0.804 *
                  (1. - std::exp(-0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.804)) +
              1.) *
                 3. * rhoA +
             rhoA * 3. * C54 * 0.804 *
                 std::exp(-0.2195149727645171 *
                          std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                          0.804) *
                 -0.8780598910580684 * C46 * 4. * sigmaAa /
                 (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2))) /
            (4. * Pi) +
        (((1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) * 4. * Pi * C60 *
               rhoA * 3. * C54 * 0.705960152410687 * C54 * 4. * sigmaAa *
               std::exp(-0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) / 0.804) *
               -0.8780598910580684 * C54 * 4. * sigmaAa /
               (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2)) -
           C60 * rhoA * 3. * C54 * 0.804 *
               std::exp(-0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) / 0.804) *
               -0.8780598910580684 * C54 * 4. * sigmaAa * 1.608 *
               ((C54 * C46 + 1e-16) * 4. * C54 * 2. * (C54 * C46 + 1e-16) +
                C54 * std::pow(2. * (C54 * C46 + 1e-16), 2)) *
               4. * Pi) /
              std::pow(1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) * 4. *
                           Pi,
                       2) +
          (C63 * (3. * rhoA * 0.804 *
                      std::exp(-0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                               0.804) *
                      -0.8780598910580684 * C54 * 4. * sigmaAa /
                      (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2)) +
                  (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) * rhoA * 3. *
                       C54 * 0.804 *
                       (std::exp(-0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.804) *
                            -3.5122395642322736 * sigmaAa -
                        -0.8780598910580684 * C46 * 4. * sigmaAa *
                            std::exp(
                                -0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                0.804) *
                            -0.8780598910580684 * C54 * 4. * sigmaAa /
                            (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2))) -
                   rhoA * 3. * C54 * 0.804 *
                       std::exp(-0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                0.804) *
                       -0.8780598910580684 * C46 * 4. * sigmaAa * 1.608 *
                       ((C54 * C46 + 1e-16) * 4. * C54 * 2. * (C54 * C46 + 1e-16) +
                        C54 * std::pow(2. * (C54 * C46 + 1e-16), 2))) /
                      std::pow(1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2),
                               2)) +
           3. * C54 * 0.804 *
               std::exp(-0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) / 0.804) *
               -0.8780598910580684 * C54 * 4. * sigmaAa /
               (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2))) /
              (4. * Pi)) *
             C60 +
         (((1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) * 4. * Pi * C60 *
                (rhoA * 3. * C54 * 0.804 *
                     (std::exp(-0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                               0.804) *
                          -3.5122395642322736 * sigmaAa -
                      -0.8780598910580684 * C54 * 4. * sigmaAa *
                          std::exp(
                              -0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                              0.804) *
                          -0.8780598910580684 * C46 * 4. * sigmaAa /
                          (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2))) +
                 3. * rhoA * 0.804 *
                     std::exp(-0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                              0.804) *
                     -0.8780598910580684 * C54 * 4. * sigmaAa) -
            C60 * rhoA * 3. * C54 * 0.804 *
                std::exp(-0.2195149727645171 *
                         std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                         0.804) *
                -0.8780598910580684 * C54 * 4. * sigmaAa * 1.608 *
                ((C54 * C46 + 1e-16) * 4. * C46 * 2. * (C54 * C46 + 1e-16) +
                 C46 * std::pow(2. * (C54 * C46 + 1e-16), 2)) *
                4. * Pi) /
               std::pow(1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) * 4. *
                            Pi,
                        2) +
           (C63 * (3. * rhoA * 0.804 *
                       std::exp(-0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                0.804) *
                       -0.8780598910580684 * C46 * 4. * sigmaAa /
                       (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2)) +
                   (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) *
                        (rhoA * 3. * C54 * 0.705960152410687 * C46 * 4. * sigmaAa *
                             std::exp(
                                 -0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.804) *
                             -0.8780598910580684 * C46 * 4. * sigmaAa /
                             (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2)) +
                         3. * rhoA * 0.804 *
                             std::exp(
                                 -0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.804) *
                             -0.8780598910580684 * C46 * 4. * sigmaAa) -
                    rhoA * 3. * C54 * 0.804 *
                        std::exp(-0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.804) *
                        -0.8780598910580684 * C46 * 4. * sigmaAa * 1.608 *
                        ((C54 * C46 + 1e-16) * 4. * C46 * 2. * (C54 * C46 + 1e-16) +
                         C46 * std::pow(2. * (C54 * C46 + 1e-16), 2))) /
                       std::pow(1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2),
                                2)) +
            (3. * (0.804 *
                       (1. -
                        std::exp(-0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                 0.804)) +
                   1.) +
             3. * C54 * 0.804 *
                 std::exp(-0.2195149727645171 *
                          std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                          0.804) *
                 -0.8780598910580684 * C46 * 4. * sigmaAa /
                 (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2)))) /
               (4. * Pi)) *
              C63 +
          (C60 * 3. * C54 * 0.804 *
               std::exp(-0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) / 0.804) *
               -0.8780598910580684 * C54 * 4. * sigmaAa /
               (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2) * 4. * Pi) +
           C63 *
               (3. * (0.804 * (1. - std::exp(-0.2195149727645171 *
                                             std::pow(std::sqrt(4. * sigmaAa) /
                                                          (2. * (C54 * C46 + 1e-16)),
                                                      2) /
                                             0.804)) +
                      1.) +
                3. * C54 * 0.804 *
                    std::exp(-0.2195149727645171 *
                             std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                             0.804) *
                    -0.8780598910580684 * C46 * 4. * sigmaAa /
                    (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2))) /
               (4. * Pi)))));

    matrix.upper[1] = 0;

    matrix.upper[2] = -(
        C60 *
            (Pi * 4. * (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 1.608 * rhoA * 3. *
                 C54 * -1.4119203048213739 *
                 std::exp(-0.2195149727645171 *
                          std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                          0.804) *
                 -0.8780598910580684 * C54 * 4. * sigmaAa /
                 (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2)) -
             1.7561197821161368 *
                 std::exp(-0.2195149727645171 *
                          std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                          0.804) *
                 0.804 * C54 * 3. * rhoA * 1.608 *
                 (Pi * 4. * (C54 * C46 + 1e-16) * 2. * 2 * C54 +
                  4. * Pi * 4. * C54 * (C54 * C46 + 1e-16))) /
            std::pow(Pi * 4. * (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 1.608, 2) +
        (C63 *
             (Pi * 4. * (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 1.608 * 3. *
                  (1.4119203048213739 *
                       std::exp(-0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                0.804) +
                   C54 * -1.4119203048213739 *
                       std::exp(-0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                                0.804) *
                       -0.8780598910580684 * C46 * 4. * sigmaAa /
                       (1.608 * (C54 * C46 + 1e-16) * std::pow(2. * (C54 * C46 + 1e-16), 2))) *
                  rhoA -
              1.7561197821161368 *
                  std::exp(-0.2195149727645171 *
                           std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                           0.804) *
                  0.804 * C54 * 3. * rhoA * 1.608 *
                  (Pi * 4. * (C54 * C46 + 1e-16) * 2. * 2 * C46 +
                   4. * Pi * 4. * C46 * (C54 * C46 + 1e-16))) /
             std::pow(Pi * 4. * (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 1.608, 2) +
         3. * 1.7561197821161368 *
             std::exp(-0.2195149727645171 *
                      std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) / 0.804) *
             0.804 * C54 / (Pi * 4. * (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 1.608)));

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
    matrix.upper[5] = -(
        C69 *
            ((0.804 *
                  (1. - std::exp(-0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.804)) +
              1.) *
                 3. * rhoB +
             rhoB * 3. * C55 * 0.804 *
                 std::exp(-0.2195149727645171 *
                          std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                          0.804) *
                 -0.8780598910580684 * C47 * 4. * sigmaBb /
                 (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2))) /
            (4. * Pi) +
        (((1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) * 4. * Pi * C64 *
               rhoB * 3. * C55 * 0.705960152410687 * C55 * 4. * sigmaBb *
               std::exp(-0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) / 0.804) *
               -0.8780598910580684 * C55 * 4. * sigmaBb /
               (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2)) -
           C64 * rhoB * 3. * C55 * 0.804 *
               std::exp(-0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) / 0.804) *
               -0.8780598910580684 * C55 * 4. * sigmaBb * 1.608 *
               ((C55 * C47 + 1e-16) * 4. * C55 * 2. * (C55 * C47 + 1e-16) +
                C55 * std::pow(2. * (C55 * C47 + 1e-16), 2)) *
               4. * Pi) /
              std::pow(1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) * 4. *
                           Pi,
                       2) +
          (C67 * (3. * rhoB * 0.804 *
                      std::exp(-0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                               0.804) *
                      -0.8780598910580684 * C55 * 4. * sigmaBb /
                      (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2)) +
                  (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) * rhoB * 3. *
                       C55 * 0.804 *
                       (std::exp(-0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.804) *
                            -3.5122395642322736 * sigmaBb -
                        -0.8780598910580684 * C47 * 4. * sigmaBb *
                            std::exp(
                                -0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                0.804) *
                            -0.8780598910580684 * C55 * 4. * sigmaBb /
                            (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2))) -
                   rhoB * 3. * C55 * 0.804 *
                       std::exp(-0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                0.804) *
                       -0.8780598910580684 * C47 * 4. * sigmaBb * 1.608 *
                       ((C55 * C47 + 1e-16) * 4. * C55 * 2. * (C55 * C47 + 1e-16) +
                        C55 * std::pow(2. * (C55 * C47 + 1e-16), 2))) /
                      std::pow(1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2),
                               2)) +
           3. * C55 * 0.804 *
               std::exp(-0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) / 0.804) *
               -0.8780598910580684 * C55 * 4. * sigmaBb /
               (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2))) /
              (4. * Pi)) *
             C64 +
         (((1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) * 4. * Pi * C64 *
                (rhoB * 3. * C55 * 0.804 *
                     (std::exp(-0.2195149727645171 *
                               std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                               0.804) *
                          -3.5122395642322736 * sigmaBb -
                      -0.8780598910580684 * C55 * 4. * sigmaBb *
                          std::exp(
                              -0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                              0.804) *
                          -0.8780598910580684 * C47 * 4. * sigmaBb /
                          (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2))) +
                 3. * rhoB * 0.804 *
                     std::exp(-0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                              0.804) *
                     -0.8780598910580684 * C55 * 4. * sigmaBb) -
            C64 * rhoB * 3. * C55 * 0.804 *
                std::exp(-0.2195149727645171 *
                         std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                         0.804) *
                -0.8780598910580684 * C55 * 4. * sigmaBb * 1.608 *
                ((C55 * C47 + 1e-16) * 4. * C47 * 2. * (C55 * C47 + 1e-16) +
                 C47 * std::pow(2. * (C55 * C47 + 1e-16), 2)) *
                4. * Pi) /
               std::pow(1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) * 4. *
                            Pi,
                        2) +
           (C67 * (3. * rhoB * 0.804 *
                       std::exp(-0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                0.804) *
                       -0.8780598910580684 * C47 * 4. * sigmaBb /
                       (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2)) +
                   (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) *
                        (rhoB * 3. * C55 * 0.705960152410687 * C47 * 4. * sigmaBb *
                             std::exp(
                                 -0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.804) *
                             -0.8780598910580684 * C47 * 4. * sigmaBb /
                             (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2)) +
                         3. * rhoB * 0.804 *
                             std::exp(
                                 -0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.804) *
                             -0.8780598910580684 * C47 * 4. * sigmaBb) -
                    rhoB * 3. * C55 * 0.804 *
                        std::exp(-0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.804) *
                        -0.8780598910580684 * C47 * 4. * sigmaBb * 1.608 *
                        ((C55 * C47 + 1e-16) * 4. * C47 * 2. * (C55 * C47 + 1e-16) +
                         C47 * std::pow(2. * (C55 * C47 + 1e-16), 2))) /
                       std::pow(1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2),
                                2)) +
            (3. * (0.804 *
                       (1. -
                        std::exp(-0.2195149727645171 *
                                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                 0.804)) +
                   1.) +
             3. * C55 * 0.804 *
                 std::exp(-0.2195149727645171 *
                          std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                          0.804) *
                 -0.8780598910580684 * C47 * 4. * sigmaBb /
                 (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2)))) /
               (4. * Pi)) *
              C67 +
          (C64 * 3. * C55 * 0.804 *
               std::exp(-0.2195149727645171 *
                        std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) / 0.804) *
               -0.8780598910580684 * C55 * 4. * sigmaBb /
               (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2) * 4. * Pi) +
           C67 *
               (3. * (0.804 * (1. - std::exp(-0.2195149727645171 *
                                             std::pow(std::sqrt(4. * sigmaBb) /
                                                          (2. * (C55 * C47 + 1e-16)),
                                                      2) /
                                             0.804)) +
                      1.) +
                3. * C55 * 0.804 *
                    std::exp(-0.2195149727645171 *
                             std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                             0.804) *
                    -0.8780598910580684 * C47 * 4. * sigmaBb /
                    (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2))) /
               (4. * Pi)))));

    matrix.upper[6] = 0;

    matrix.upper[7] = 0;

    matrix.upper[8] = -(
        C64 *
            (Pi * 4. * (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 1.608 * rhoB * 3. *
                 C55 * -1.4119203048213739 *
                 std::exp(-0.2195149727645171 *
                          std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                          0.804) *
                 -0.8780598910580684 * C55 * 4. * sigmaBb /
                 (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2)) -
             1.7561197821161368 *
                 std::exp(-0.2195149727645171 *
                          std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                          0.804) *
                 0.804 * C55 * 3. * rhoB * 1.608 *
                 (Pi * 4. * (C55 * C47 + 1e-16) * 2. * 2 * C55 +
                  4. * Pi * 4. * C55 * (C55 * C47 + 1e-16))) /
            std::pow(Pi * 4. * (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 1.608, 2) +
        (C67 *
             (Pi * 4. * (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 1.608 * 3. *
                  (1.4119203048213739 *
                       std::exp(-0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                0.804) +
                   C55 * -1.4119203048213739 *
                       std::exp(-0.2195149727645171 *
                                std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                                0.804) *
                       -0.8780598910580684 * C47 * 4. * sigmaBb /
                       (1.608 * (C55 * C47 + 1e-16) * std::pow(2. * (C55 * C47 + 1e-16), 2))) *
                  rhoB -
              1.7561197821161368 *
                  std::exp(-0.2195149727645171 *
                           std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                           0.804) *
                  0.804 * C55 * 3. * rhoB * 1.608 *
                  (Pi * 4. * (C55 * C47 + 1e-16) * 2. * 2 * C47 +
                   4. * Pi * 4. * C47 * (C55 * C47 + 1e-16))) /
             std::pow(Pi * 4. * (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 1.608, 2) +
         3. * 1.7561197821161368 *
             std::exp(-0.2195149727645171 *
                      std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) / 0.804) *
             0.804 * C55 / (Pi * 4. * (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 1.608)));

    matrix.upper[9] =
        -1.7561197821161368 *
        std::exp(-0.2195149727645171 *
                 std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) / 0.804) *
        -1.4119203048213739 * C54 * 3. * rhoA /
        (1.608 * (C54 * C46 + 1e-16) * 2 * 2. * (C54 * C46 + 1e-16) * 4. * Pi *
         (C54 * C46 + 1e-16) * 2. * 2 * (C54 * C46 + 1e-16) * 1.608);

    matrix.upper[10] = 0;

    matrix.upper[11] = 0;

    matrix.upper[12] = 0;

    matrix.upper[13] = 0;

    matrix.upper[14] =
        -1.7561197821161368 *
        std::exp(-0.2195149727645171 *
                 std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) / 0.804) *
        -1.4119203048213739 * C55 * 3. * rhoB /
        (1.608 * (C55 * C47 + 1e-16) * 2 * 2. * (C55 * C47 + 1e-16) * 4. * Pi *
         (C55 * C47 + 1e-16) * 2. * 2 * (C55 * C47 + 1e-16) * 1.608);
}

} // namespace excgrid
