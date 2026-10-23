// excgrid codegen source: the Lee-Yang-Parr 1988 GGA correlation
// functional.  Fresh authorship; formula from the LYP paper
// (docs/mainpage.md references).  Regenerate with tools/regenerate.py.

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

XcKernelValue LypCorrelation(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
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

    const double C148 = rhoB * rhoA;
    const double C149 = rhoA + rhoB;
    const double C150 = -11. / 3.;
    const double C151 = 1. / 3.;
    const double C152 = 8. / 3.;
    const double C153 = std::pow(C149, C150);
    const double C154 = std::pow(C149, C151);
    const double C155 = -0.2533 / C154;
    const double C156 = 0.2533 / C154;
    const double C157 = 0.349 / C154;
    const double C158 = C157 + 1.;
    const double C159 = std::exp(C155);
    const double C160 = C153 * C159;
    const double C161 = C154 * C158;
    const double C162 = 0.00649176 * C160;
    const double C163 = 0.349 / C161;
    const double C164 = C156 + C163;
    const double C165 = 3. * C164;
    const double C166 = C164 - 11.;
    const double C167 = 1. - C165;

    result.exc =
        -((((0.19672 * rhoA * rhoB / (C149 * C158) +
             (std::pow(rhoA, C152) + std::pow(rhoB, C152)) * rhoB * rhoA * 0.00649176 *
                 std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) * C160 /
                 (10. * C158)) +
            sigmaAa * ((C167 - C166 * rhoA / C149) * C148 / 9. - std::pow(rhoB, 2)) * C162 / C158) +
           sigmaAb * ((47. - 7. * C164) * C148 / 9. - 4. * std::pow(C149, 2) / 3.) * C162 / C158) +
          sigmaBb * ((C167 - C166 * rhoB / C149) * C148 / 9. - std::pow(rhoA, 2)) * C162 / C158);

    const double C169 = rhoB;
    const double C170 = 1;
    const double C171 = C150 * std::pow(C149, C150 - 1) * C170;
    const double C172 = C151 * std::pow(C149, C151 - 1) * C170;
    const double C173 = 0.2533 * C172 / std::pow(C154, 2);
    const double C174 = -0.2533 * C172 / std::pow(C154, 2);
    const double C175 = -0.349 * C172 / std::pow(C154, 2);
    const double C176 = C175;
    const double C177 = std::exp(C155) * C173;
    const double C178 = C159 * C171 + C153 * C177;
    const double C179 = C158 * C172 + C154 * C176;
    const double C180 = 0.00649176 * C178;
    const double C181 = -0.349 * C179 / std::pow(C161, 2);
    const double C182 = C174 + C181;
    const double C183 = 3. * C182;
    const double C184 = C182;
    const double C185 = -C183;

    result.vrhoA =
        ((((((((sigmaBb * 2 * rhoA * C162 / C158 -
                ((0.19672 * rhoB / (C149 * C158) +
                  ((std::pow(rhoA, C152) + std::pow(rhoB, C152)) * rhoB * 0.00649176 *
                       std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) *
                       C160 +
                   C152 * std::pow(rhoA, C152 - 1) * rhoB * rhoA * 0.00649176 *
                       std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) *
                       C160) /
                      (10. * C158)) -
                 sigmaAa * C162 * C148 * C166 / (C149 * 9. * C158))) -
               (((sigmaAa * C162 * C148 * C166 * rhoA / (9. * std::pow(C149, 2) * C158) -
                  0.19672 * rhoA * rhoB * C158 / std::pow(C149 * C158, 2)) -
                 sigmaAb * C162 * 8. * C149 / (3. * C158)) +
                sigmaBb * C162 * C148 * C166 * rhoB / (9. * std::pow(C149, 2) * C158)) *
                   C170) +
              (sigmaBb * ((C167 - C166 * rhoB / C149) * C148 / 9. - std::pow(rhoA, 2)) * C162 /
                   std::pow(C158, 2) -
               ((((-10. * (std::pow(rhoA, C152) + std::pow(rhoB, C152)) * rhoB * rhoA * 0.00649176 *
                   std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) * C160) /
                      std::pow(10. * C158, 2) -
                  0.19672 * rhoA * rhoB * C149 / std::pow(C149 * C158, 2)) -
                 sigmaAa * ((C167 - C166 * rhoA / C149) * C148 / 9. - std::pow(rhoB, 2)) * C162 /
                     std::pow(C158, 2)) -
                sigmaAb * ((47. - 7. * C164) * C148 / 9. - 4. * std::pow(C149, 2) / 3.) * C162 /
                    std::pow(C158, 2))) *
                  C176) -
             C178 * (std::pow(rhoA, C152) + std::pow(rhoB, C152)) * rhoB * rhoA * 0.00649176 *
                 std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) /
                 (10. * C158)) -
            C185 * (sigmaAa * C162 * C148 + sigmaBb * C162 * C148) / (9. * C158)) +
           (sigmaAa * C162 * C148 * rhoA / (C149 * 9. * C158) -
            (-sigmaBb * C162 * C148 * rhoB / C149) / (9. * C158)) *
               C184) -
          C169 *
              ((sigmaAa * C162 * (C167 - C166 * rhoA / C149) + sigmaAb * C162 * (47. - 7. * C164)) +
               sigmaBb * C162 * (C167 - C166 * rhoB / C149)) /
              (9. * C158)) -
         C180 *
             ((sigmaAa * ((C167 - C166 * rhoA / C149) * C148 / 9. - std::pow(rhoB, 2)) +
               sigmaAb * ((47. - 7. * C164) * C148 / 9. - 4. * std::pow(C149, 2) / 3.)) +
              sigmaBb * ((C167 - C166 * rhoB / C149) * C148 / 9. - std::pow(rhoA, 2))) /
             C158) -
        C182 * sigmaAb * C162 * -7. * C148 / (9. * C158);

    const double C196 = rhoA;
    const double C197 = 1;
    const double C198 = C150 * std::pow(C149, C150 - 1) * C197;
    const double C199 = C151 * std::pow(C149, C151 - 1) * C197;
    const double C200 = 0.2533 * C199 / std::pow(C154, 2);
    const double C201 = -0.2533 * C199 / std::pow(C154, 2);
    const double C202 = -0.349 * C199 / std::pow(C154, 2);
    const double C203 = C202;
    const double C204 = std::exp(C155) * C200;
    const double C205 = C159 * C198 + C153 * C204;
    const double C206 = C158 * C199 + C154 * C203;
    const double C207 = 0.00649176 * C205;
    const double C208 = -0.349 * C206 / std::pow(C161, 2);
    const double C209 = C201 + C208;
    const double C210 = 3. * C209;
    const double C211 = C209;
    const double C212 = -C210;

    result.vrhoB =
        ((((((((sigmaBb * C162 * C148 * C166 / (C149 * 9. * C158) -
                ((0.19672 * rhoA / (C149 * C158) +
                  ((std::pow(rhoA, C152) + std::pow(rhoB, C152)) * rhoA * 0.00649176 *
                       std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) *
                       C160 +
                   C152 * std::pow(rhoB, C152 - 1) * rhoB * rhoA * 0.00649176 *
                       std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) *
                       C160) /
                      (10. * C158)) -
                 sigmaAa * 2 * rhoB * C162 / C158)) -
               (((sigmaAa * C162 * C148 * C166 * rhoA / (9. * std::pow(C149, 2) * C158) -
                  0.19672 * rhoA * rhoB * C158 / std::pow(C149 * C158, 2)) -
                 sigmaAb * C162 * 8. * C149 / (3. * C158)) +
                sigmaBb * C162 * C148 * C166 * rhoB / (9. * std::pow(C149, 2) * C158)) *
                   C197) +
              (sigmaBb * ((C167 - C166 * rhoB / C149) * C148 / 9. - std::pow(rhoA, 2)) * C162 /
                   std::pow(C158, 2) -
               ((((-10. * (std::pow(rhoA, C152) + std::pow(rhoB, C152)) * rhoB * rhoA * 0.00649176 *
                   std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) * C160) /
                      std::pow(10. * C158, 2) -
                  0.19672 * rhoA * rhoB * C149 / std::pow(C149 * C158, 2)) -
                 sigmaAa * ((C167 - C166 * rhoA / C149) * C148 / 9. - std::pow(rhoB, 2)) * C162 /
                     std::pow(C158, 2)) -
                sigmaAb * ((47. - 7. * C164) * C148 / 9. - 4. * std::pow(C149, 2) / 3.) * C162 /
                    std::pow(C158, 2))) *
                  C203) -
             C205 * (std::pow(rhoA, C152) + std::pow(rhoB, C152)) * rhoB * rhoA * 0.00649176 *
                 std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) /
                 (10. * C158)) -
            C212 * (sigmaAa * C162 * C148 + sigmaBb * C162 * C148) / (9. * C158)) +
           (sigmaAa * C162 * C148 * rhoA / (C149 * 9. * C158) -
            (-sigmaBb * C162 * C148 * rhoB / C149) / (9. * C158)) *
               C211) -
          C196 *
              ((sigmaAa * C162 * (C167 - C166 * rhoA / C149) + sigmaAb * C162 * (47. - 7. * C164)) +
               sigmaBb * C162 * (C167 - C166 * rhoB / C149)) /
              (9. * C158)) -
         C207 *
             ((sigmaAa * ((C167 - C166 * rhoA / C149) * C148 / 9. - std::pow(rhoB, 2)) +
               sigmaAb * ((47. - 7. * C164) * C148 / 9. - 4. * std::pow(C149, 2) / 3.)) +
              sigmaBb * ((C167 - C166 * rhoB / C149) * C148 / 9. - std::pow(rhoA, 2))) /
             C158) -
        C209 * sigmaAb * C162 * -7. * C148 / (9. * C158);

    result.vsigmaAa = -((C167 - C166 * rhoA / C149) * C148 / 9. - std::pow(rhoB, 2)) * C162 / C158;

    result.vsigmaAb = -((47. - 7. * C164) * C148 / 9. - 4. * std::pow(C149, 2) / 3.) * C162 / C158;

    result.vsigmaBb = -((C167 - C166 * rhoB / C149) * C148 / 9. - std::pow(rhoA, 2)) * C162 / C158;

    return result;
}

void LypCorrelationSecondDerivatives(double rhoA,
                                     double rhoB,
                                     double sigmaAa,
                                     double sigmaAb,
                                     double sigmaBb,
                                     PointSecondDerivativeMatrix& matrix) {
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    if (rhoA + rhoB < 2. * std::numeric_limits<double>::epsilon())
    {
        return;
    }

    const double C44 = rhoB * rhoA;
    const double C45 = rhoA + rhoB;
    const double C46 = -11. / 3.;
    const double C47 = 1. / 3.;
    const double C48 = 8. / 3.;
    const double C49 = std::pow(C45, C46);
    const double C50 = std::pow(C45, C47);
    const double C51 = -0.2533 / C50;
    const double C52 = 0.2533 / C50;
    const double C53 = 0.349 / C50;
    const double C54 = C53 + 1.;
    const double C55 = std::exp(C51);
    const double C56 = C49 * C55;
    const double C57 = C50 * C54;
    const double C58 = 0.00649176 * C56;
    const double C59 = 0.349 / C57;
    const double C60 = C52 + C59;
    const double C61 = 3. * C60;
    const double C62 = C60 - 11.;
    const double C63 = 1. - C61;
    const double C68 = rhoB;
    const double C69 = 1;
    const double C70 = C46 * std::pow(C45, C46 - 1) * C69;
    const double C71 = C47 * std::pow(C45, C47 - 1) * C69;
    const double C72 = 0.2533 * C71 / std::pow(C50, 2);
    const double C73 = -0.2533 * C71 / std::pow(C50, 2);
    const double C74 = -0.349 * C71 / std::pow(C50, 2);
    const double C75 = C74;
    const double C76 = std::exp(C51) * C72;
    const double C77 = C55 * C70 + C49 * C76;
    const double C78 = C54 * C71 + C50 * C75;
    const double C79 = 0.00649176 * C77;
    const double C80 = -0.349 * C78 / std::pow(C57, 2);
    const double C81 = C73 + C80;
    const double C82 = 3. * C81;
    const double C83 = C81;
    const double C84 = -C82;
    const double C102 = C46 * (C46 - 1) * std::pow(C45, C46 - 2) * std::pow(C69, 2);
    const double C103 = C47 * (C47 - 1) * std::pow(C45, C47 - 2) * std::pow(C69, 2);
    const double C104 =
        0.2533 * C103 / std::pow(C50, 2) - C71 * 0.2533 * C71 * 2 * C50 / std::pow(C50, 4);
    const double C105 =
        -0.2533 * C103 / std::pow(C50, 2) - C71 * -0.2533 * C71 * 2 * C50 / std::pow(C50, 4);
    const double C106 =
        -0.349 * C103 / std::pow(C50, 2) - C71 * -0.349 * C71 * 2 * C50 / std::pow(C50, 4);
    const double C107 = C106;
    const double C108 = std::exp(C51) * std::pow(C72, 2) + std::exp(C51) * C104;
    const double C109 = ((C70 * C76 + C55 * C102) + C76 * C70) + C49 * C108;
    const double C110 = ((C71 * C75 + C54 * C103) + C75 * C71) + C50 * C107;
    const double C111 = 0.00649176 * C109;
    const double C112 =
        -0.349 * C110 / std::pow(C57, 2) - C78 * -0.349 * C78 * 2 * C57 / std::pow(C57, 4);
    const double C113 = C105 + C112;
    const double C114 = 3. * C113;
    const double C115 = C113;
    const double C116 = -C114;
    matrix.upper[0] =
        ((((((((((((((((((((sigmaBb * 2 * C58 / C54 -
                            (C48 * std::pow(rhoA, C48 - 1) * rhoB * 0.00649176 *
                                 std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                 std::pow(2., 11. / 3.) * C56 +
                             (C48 * std::pow(rhoA, C48 - 1) * rhoB * 0.00649176 *
                                  std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                  std::pow(2., 11. / 3.) * C56 +
                              C48 * (C48 - 1) * std::pow(rhoA, C48 - 2) * rhoB * rhoA * 0.00649176 *
                                  std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                  std::pow(2., 11. / 3.) * C56)) /
                                (10. * C54)) -
                           (sigmaAa * C58 * C44 * C62 / (9. * std::pow(C45, 2) * C54) -
                            0.19672 * rhoB * C54 / std::pow(C45 * C54, 2)) *
                               C69) +
                          ((-sigmaBb * 2 * rhoA * C58) / std::pow(C54, 2) -
                           (((-10. * ((std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB *
                                          0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) *
                                          3. * std::pow(2., 11. / 3.) * C56 +
                                      C48 * std::pow(rhoA, C48 - 1) * rhoB * rhoA * 0.00649176 *
                                          std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                          std::pow(2., 11. / 3.) * C56)) /
                                 std::pow(10. * C54, 2) -
                             0.19672 * rhoB * C45 / std::pow(C45 * C54, 2)) -
                            (-sigmaAa * C58 * C44 * C62 / C45) / (9. * std::pow(C54, 2)))) *
                              C75) -
                         C77 *
                             ((std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * 0.00649176 *
                                  std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                  std::pow(2., 11. / 3.) +
                              C48 * std::pow(rhoA, C48 - 1) * rhoB * rhoA * 0.00649176 *
                                  std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                  std::pow(2., 11. / 3.)) /
                             (10. * C54)) +
                        C83 * sigmaAa * C58 * C44 / (C45 * 9. * C54)) -
                       (-C68 * sigmaAa * C58 * C62 / C45) / (9. * C54)) -
                      (-C79 * (sigmaBb * 2 * rhoA + sigmaAa * C44 * C62 / (9. * C45))) / C54) +
                     (((((((sigmaBb * 2 * rhoA / C54 + sigmaAa * C44 * C62 / (C45 * 9. * C54)) -
                           ((sigmaAa * C44 * C62 * rhoA / (9. * std::pow(C45, 2) * C54) -
                             sigmaAb * 8. * C45 / (3. * C54)) +
                            sigmaBb * C44 * C62 * rhoB / (9. * std::pow(C45, 2) * C54)) *
                               C69) +
                          C75 *
                              (sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) +
                               (sigmaAa *
                                    ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) +
                                sigmaAb *
                                    ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.))) /
                              std::pow(C54, 2)) -
                         C84 * (sigmaAa * C44 + sigmaBb * C44) / (9. * C54)) +
                        (sigmaAa * C44 * rhoA / (C45 * 9. * C54) -
                         (-sigmaBb * C44 * rhoB / C45) / (9. * C54)) *
                            C83) -
                       C68 *
                           ((sigmaAa * (C63 - C62 * rhoA / C45) + sigmaAb * (47. - 7. * C60)) +
                            sigmaBb * (C63 - C62 * rhoB / C45)) /
                           (9. * C54)) -
                      C81 * sigmaAb * -7. * C44 / (9. * C54)) *
                         C79) +
                    ((((((((((-sigmaBb * 2 * rhoA * C58) / std::pow(C54, 2) -
                             (((-10. * ((std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB *
                                            0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) *
                                            3. * std::pow(2., 11. / 3.) * C56 +
                                        C48 * std::pow(rhoA, C48 - 1) * rhoB * rhoA * 0.00649176 *
                                            std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                            std::pow(2., 11. / 3.) * C56)) /
                                   std::pow(10. * C54, 2) -
                               0.19672 * rhoB * C45 / std::pow(C45 * C54, 2)) -
                              (-sigmaAa * C58 * C44 * C62 * 9. * C45) /
                                  std::pow(C45 * 9. * C54, 2))) -
                            ((((-sigmaAa * C58 * C44 * C62 * rhoA * 9. * std::pow(C45, 2)) /
                                   std::pow(9. * std::pow(C45, 2) * C54, 2) -
                               (std::pow(C45 * C54, 2) * 0.19672 * rhoA * rhoB -
                                0.19672 * rhoA * rhoB * C54 * 2 * C54 * std::pow(C45, 2)) /
                                   std::pow(C45 * C54, 4)) -
                              (-3. * sigmaAb * C58 * 8. * C45) / std::pow(3. * C54, 2)) -
                             sigmaBb * C58 * C44 * C62 * rhoB * 9. * std::pow(C45, 2) /
                                 std::pow(9. * std::pow(C45, 2) * C54, 2)) *
                                C69) +
                           ((-sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) *
                             C58 * 2 * C54) /
                                std::pow(C54, 4) -
                            (((10. * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA *
                                   0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                   std::pow(2., 11. / 3.) * C56 * 200. * C54 /
                                   std::pow(10. * C54, 4) -
                               (-0.19672 * rhoA * rhoB * C45 * 2 * C54 * std::pow(C45, 2)) /
                                   std::pow(C45 * C54, 4)) -
                              (-sigmaAa *
                               ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) * C58 * 2 *
                               C54) /
                                  std::pow(C54, 4)) -
                             (-sigmaAb *
                              ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.) * C58 * 2 *
                              C54) /
                                 std::pow(C54, 4))) *
                               C75) -
                          (-10. * C77 * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA *
                           0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                           std::pow(2., 11. / 3.)) /
                              std::pow(10. * C54, 2)) -
                         (-9. * C84 * (sigmaAa * C58 * C44 + sigmaBb * C58 * C44)) /
                             std::pow(9. * C54, 2)) +
                        ((-sigmaAa * C58 * C44 * rhoA * 9. * C45) / std::pow(C45 * 9. * C54, 2) -
                         (- -9. * sigmaBb * C58 * C44 * rhoB / C45) / std::pow(9. * C54, 2)) *
                            C83) -
                       (-9. * C68 *
                        ((sigmaAa * C58 * (C63 - C62 * rhoA / C45) +
                          sigmaAb * C58 * (47. - 7. * C60)) +
                         sigmaBb * C58 * (C63 - C62 * rhoB / C45))) /
                           std::pow(9. * C54, 2)) -
                      (-C79 *
                       ((sigmaAa * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) +
                         sigmaAb * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.)) +
                        sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)))) /
                          std::pow(C54, 2)) -
                     (-9. * C81 * sigmaAb * C58 * -7. * C44) / std::pow(9. * C54, 2)) *
                        C75) +
                   (((((((-sigmaAa * C58 * C44 * C62 * 9. * C54) / std::pow(C45 * 9. * C54, 2) -
                         (-0.19672 * rhoB * C54) / std::pow(C45 * C54, 2)) -
                        ((((-sigmaAa * C58 * C44 * C62 * rhoA * 18. * C45 * C54) /
                               std::pow(9. * std::pow(C45, 2) * C54, 2) -
                           (-0.19672 * rhoA * rhoB * C54 * 2 * C45 * std::pow(C54, 2)) /
                               std::pow(C45 * C54, 4)) -
                          sigmaAb * 8. * C58 / (3. * C54)) -
                         sigmaBb * C58 * C44 * C62 * rhoB * 18. * C45 * C54 /
                             std::pow(9. * std::pow(C45, 2) * C54, 2)) *
                            C69) +
                       (sigmaBb * C58 * C44 * C62 * rhoB /
                            (9. * std::pow(C45, 2) * std::pow(C54, 2)) -
                        (sigmaAb * C58 * 8. * C45 / (3. * std::pow(C54, 2)) -
                         ((std::pow(C45 * C54, 2) * 0.19672 * rhoA * rhoB -
                           0.19672 * rhoA * rhoB * C45 * 2 * C45 * std::pow(C54, 2)) /
                              std::pow(C45 * C54, 4) +
                          sigmaAa * C58 * C44 * C62 * rhoA /
                              (9. * std::pow(C45, 2) * std::pow(C54, 2))))) *
                           C75) +
                      ((-sigmaAa * C58 * C44 * rhoA * 9. * C54) / std::pow(C45 * 9. * C54, 2) -
                       sigmaBb * C58 * C44 * rhoB / (std::pow(C45, 2) * 9. * C54)) *
                          C83) -
                     C68 * (sigmaAa * C58 * C62 * rhoA + sigmaBb * C58 * C62 * rhoB) /
                         (std::pow(C45, 2) * 9. * C54)) -
                    C79 *
                        ((sigmaAa * C44 * C62 * rhoA / (9. * std::pow(C45, 2)) -
                          sigmaAb * 8. * C45 / 3.) +
                         sigmaBb * C44 * C62 * rhoB / (9. * std::pow(C45, 2))) /
                        C54) *
                       C69) +
                  (C75 * 10. * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA *
                       0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                       std::pow(2., 11. / 3.) / std::pow(10. * C54, 2) -
                   ((std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * 0.00649176 *
                        std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) +
                    C48 * std::pow(rhoA, C48 - 1) * rhoB * rhoA * 0.00649176 *
                        std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.)) /
                       (10. * C54)) *
                      C77) +
                 ((((((sigmaAa * C58 * C62 / (C45 * 9. * C54) -
                       C69 * (sigmaAa * C58 * C62 * rhoA + sigmaBb * C58 * C62 * rhoB) /
                           (9. * std::pow(C45, 2) * C54)) +
                      C75 *
                          (sigmaBb * C58 * (C63 - C62 * rhoB / C45) +
                           (sigmaAa * C58 * (C63 - C62 * rhoA / C45) +
                            sigmaAb * C58 * (47. - 7. * C60))) /
                          (9. * std::pow(C54, 2))) -
                     C84 * (sigmaAa * C58 + sigmaBb * C58) / (9. * C54)) +
                    (sigmaAa * C58 * rhoA / (C45 * 9. * C54) -
                     (-sigmaBb * C58 * rhoB / C45) / (9. * C54)) *
                        C83) -
                   C79 *
                       ((sigmaAa * (C63 - C62 * rhoA / C45) + sigmaAb * (47. - 7. * C60)) +
                        sigmaBb * (C63 - C62 * rhoB / C45)) /
                       (9. * C54)) -
                  C81 * sigmaAb * -7. * C58 / (9. * C54)) *
                     C68) +
                ((((sigmaAa * C58 * C44 / (C45 * 9. * C54) -
                    C69 * (sigmaAa * C58 * C44 * rhoA + sigmaBb * C58 * C44 * rhoB) /
                        (9. * std::pow(C45, 2) * C54)) +
                   ((-sigmaBb * C58 * C44 * rhoB / C45) / (9. * std::pow(C54, 2)) -
                    sigmaAa * C58 * C44 * rhoA / (C45 * 9. * std::pow(C54, 2))) *
                       C75) -
                  (-C68 * (sigmaBb * C58 * rhoB + sigmaAa * C58 * rhoA) / C45) / (9. * C54)) -
                 C79 * ((-sigmaBb * C44 * rhoB / C45) / 9. - sigmaAa * C44 * rhoA / (9. * C45)) /
                     C54) *
                    C83) +
               ((C75 * (sigmaBb * C58 * C44 + sigmaAa * C58 * C44) / (9. * std::pow(C54, 2)) -
                 C68 * (sigmaAa * C58 + sigmaBb * C58) / (9. * C54)) -
                C79 * (sigmaAa * C44 + sigmaBb * C44) / (9. * C54)) *
                   C84) +
              ((C75 * sigmaAb * C58 * -7. * C44 / (9. * std::pow(C54, 2)) -
                C68 * sigmaAb * -7. * C58 / (9. * C54)) -
               C79 * sigmaAb * -7. * C44 / (9. * C54)) *
                  C81) +
             (sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) * C58 /
                  std::pow(C54, 2) -
              ((((-10. * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA * 0.00649176 *
                  std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) * C56) /
                     std::pow(10. * C54, 2) -
                 0.19672 * rhoA * rhoB * C45 / std::pow(C45 * C54, 2)) -
                sigmaAa * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) * C58 /
                    std::pow(C54, 2)) -
               sigmaAb * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.) * C58 /
                   std::pow(C54, 2))) *
                 C107) -
            C109 * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA * 0.00649176 *
                std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) /
                (10. * C54)) -
           C116 * (sigmaAa * C58 * C44 + sigmaBb * C58 * C44) / (9. * C54)) +
          (sigmaAa * C58 * C44 * rhoA / (C45 * 9. * C54) -
           (-sigmaBb * C58 * C44 * rhoB / C45) / (9. * C54)) *
              C115) -
         C111 *
             ((sigmaAa * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) +
               sigmaAb * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.)) +
              sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2))) /
             C54) -
        C113 * sigmaAb * C58 * -7. * C44 / (9. * C54);

    const double C85 = rhoA;
    const double C86 = 1;
    const double C87 = C46 * std::pow(C45, C46 - 1) * C86;
    const double C88 = C47 * std::pow(C45, C47 - 1) * C86;
    const double C89 = 0.2533 * C88 / std::pow(C50, 2);
    const double C90 = -0.2533 * C88 / std::pow(C50, 2);
    const double C91 = -0.349 * C88 / std::pow(C50, 2);
    const double C92 = C91;
    const double C93 = std::exp(C51) * C89;
    const double C94 = C55 * C87 + C49 * C93;
    const double C95 = C54 * C88 + C50 * C92;
    const double C96 = 0.00649176 * C94;
    const double C97 = -0.349 * C95 / std::pow(C57, 2);
    const double C98 = C90 + C97;
    const double C99 = 3. * C98;
    const double C100 = C98;
    const double C101 = -C99;
    const double C117 = 1;
    const double C118 = C46 * (C46 - 1) * std::pow(C45, C46 - 2) * C69 * C86;
    const double C119 = C47 * (C47 - 1) * std::pow(C45, C47 - 2) * C69 * C86;
    const double C120 =
        0.2533 * C119 / std::pow(C50, 2) - C88 * 0.2533 * C71 * 2 * C50 / std::pow(C50, 4);
    const double C121 =
        -0.2533 * C119 / std::pow(C50, 2) - C88 * -0.2533 * C71 * 2 * C50 / std::pow(C50, 4);
    const double C122 =
        -0.349 * C119 / std::pow(C50, 2) - C88 * -0.349 * C71 * 2 * C50 / std::pow(C50, 4);
    const double C123 = C122;
    const double C124 = std::exp(C51) * C72 * C89 + std::exp(C51) * C120;
    const double C125 = ((C70 * C93 + C55 * C118) + C76 * C87) + C49 * C124;
    const double C126 = ((C71 * C92 + C54 * C119) + C75 * C88) + C50 * C123;
    const double C127 = 0.00649176 * C125;
    const double C128 =
        -0.349 * C126 / std::pow(C57, 2) - C95 * -0.349 * C78 * 2 * C57 / std::pow(C57, 4);
    const double C129 = C121 + C128;
    const double C130 = 3. * C129;
    const double C131 = C129;
    const double C132 = -C130;
    matrix.upper[1] =
        (((((((((((((((((((((-sigmaBb * C58 * C44 * C62 / C45) / (9. * std::pow(C54, 2)) -
                            (((-10. * ((std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoA *
                                           0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) *
                                           3. * std::pow(2., 11. / 3.) * C56 +
                                       C48 * std::pow(rhoB, C48 - 1) * rhoB * rhoA * 0.00649176 *
                                           std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                           std::pow(2., 11. / 3.) * C56)) /
                                  std::pow(10. * C54, 2) -
                              0.19672 * rhoA * C45 / std::pow(C45 * C54, 2)) -
                             (-sigmaAa * 2 * rhoB * C58) / std::pow(C54, 2))) *
                               C75 -
                           ((0.19672 / (C45 * C54) +
                             (((std::pow(rhoA, C48) + std::pow(rhoB, C48)) * 0.00649176 *
                                   std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                   std::pow(2., 11. / 3.) * C56 +
                               C48 * std::pow(rhoB, C48 - 1) * rhoB * 0.00649176 *
                                   std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                   std::pow(2., 11. / 3.) * C56) +
                              C48 * std::pow(rhoA, C48 - 1) * rhoA * 0.00649176 *
                                  std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                  std::pow(2., 11. / 3.) * C56) /
                                 (10. * C54)) +
                            (sigmaBb * C58 * C44 * C62 / (9. * std::pow(C45, 2) * C54) -
                             0.19672 * rhoA * C54 / std::pow(C45 * C54, 2)) *
                                C69)) -
                          C77 *
                              ((std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoA * 0.00649176 *
                                   std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                   std::pow(2., 11. / 3.) +
                               C48 * std::pow(rhoB, C48 - 1) * rhoB * rhoA * 0.00649176 *
                                   std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                   std::pow(2., 11. / 3.)) /
                              (10. * C54)) +
                         C83 * sigmaBb * C58 * C44 / (C45 * 9. * C54)) -
                        (-C68 * sigmaBb * C58 * C62 / C45) / (9. * C54)) -
                       C79 * ((-sigmaBb * C44 * C62 / C45) / 9. - sigmaAa * 2 * rhoB) / C54) +
                      (((((((sigmaBb * 2 * rhoA / C54 + sigmaAa * C44 * C62 / (C45 * 9. * C54)) -
                            ((sigmaAa * C44 * C62 * rhoA / (9. * std::pow(C45, 2) * C54) -
                              sigmaAb * 8. * C45 / (3. * C54)) +
                             sigmaBb * C44 * C62 * rhoB / (9. * std::pow(C45, 2) * C54)) *
                                C69) +
                           C75 *
                               (sigmaBb *
                                    ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) +
                                (sigmaAa *
                                     ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) +
                                 sigmaAb *
                                     ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.))) /
                               std::pow(C54, 2)) -
                          C84 * (sigmaAa * C44 + sigmaBb * C44) / (9. * C54)) +
                         (sigmaAa * C44 * rhoA / (C45 * 9. * C54) -
                          (-sigmaBb * C44 * rhoB / C45) / (9. * C54)) *
                             C83) -
                        C68 *
                            ((sigmaAa * (C63 - C62 * rhoA / C45) + sigmaAb * (47. - 7. * C60)) +
                             sigmaBb * (C63 - C62 * rhoB / C45)) /
                            (9. * C54)) -
                       C81 * sigmaAb * -7. * C44 / (9. * C54)) *
                          C96) +
                     ((((((((((-sigmaBb * 2 * rhoA * C58) / std::pow(C54, 2) -
                              (((-10. * ((std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB *
                                             0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) *
                                             3. * std::pow(2., 11. / 3.) * C56 +
                                         C48 * std::pow(rhoA, C48 - 1) * rhoB * rhoA * 0.00649176 *
                                             std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                             std::pow(2., 11. / 3.) * C56)) /
                                    std::pow(10. * C54, 2) -
                                0.19672 * rhoB * C45 / std::pow(C45 * C54, 2)) -
                               (-sigmaAa * C58 * C44 * C62 * 9. * C45) /
                                   std::pow(C45 * 9. * C54, 2))) -
                             ((((-sigmaAa * C58 * C44 * C62 * rhoA * 9. * std::pow(C45, 2)) /
                                    std::pow(9. * std::pow(C45, 2) * C54, 2) -
                                (std::pow(C45 * C54, 2) * 0.19672 * rhoA * rhoB -
                                 0.19672 * rhoA * rhoB * C54 * 2 * C54 * std::pow(C45, 2)) /
                                    std::pow(C45 * C54, 4)) -
                               (-3. * sigmaAb * C58 * 8. * C45) / std::pow(3. * C54, 2)) -
                              sigmaBb * C58 * C44 * C62 * rhoB * 9. * std::pow(C45, 2) /
                                  std::pow(9. * std::pow(C45, 2) * C54, 2)) *
                                 C69) +
                            ((-sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) *
                              C58 * 2 * C54) /
                                 std::pow(C54, 4) -
                             (((10. * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA *
                                    0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                    std::pow(2., 11. / 3.) * C56 * 200. * C54 /
                                    std::pow(10. * C54, 4) -
                                (-0.19672 * rhoA * rhoB * C45 * 2 * C54 * std::pow(C45, 2)) /
                                    std::pow(C45 * C54, 4)) -
                               (-sigmaAa *
                                ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) * C58 *
                                2 * C54) /
                                   std::pow(C54, 4)) -
                              (-sigmaAb *
                               ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.) * C58 *
                               2 * C54) /
                                  std::pow(C54, 4))) *
                                C75) -
                           (-10. * C77 * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA *
                            0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                            std::pow(2., 11. / 3.)) /
                               std::pow(10. * C54, 2)) -
                          (-9. * C84 * (sigmaAa * C58 * C44 + sigmaBb * C58 * C44)) /
                              std::pow(9. * C54, 2)) +
                         ((-sigmaAa * C58 * C44 * rhoA * 9. * C45) / std::pow(C45 * 9. * C54, 2) -
                          (- -9. * sigmaBb * C58 * C44 * rhoB / C45) / std::pow(9. * C54, 2)) *
                             C83) -
                        (-9. * C68 *
                         ((sigmaAa * C58 * (C63 - C62 * rhoA / C45) +
                           sigmaAb * C58 * (47. - 7. * C60)) +
                          sigmaBb * C58 * (C63 - C62 * rhoB / C45))) /
                            std::pow(9. * C54, 2)) -
                       (-C79 *
                        ((sigmaAa * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) +
                          sigmaAb * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.)) +
                         sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)))) /
                           std::pow(C54, 2)) -
                      (-9. * C81 * sigmaAb * C58 * -7. * C44) / std::pow(9. * C54, 2)) *
                         C92) +
                    (((((((-sigmaAa * C58 * C44 * C62 * 9. * C54) / std::pow(C45 * 9. * C54, 2) -
                          (-0.19672 * rhoB * C54) / std::pow(C45 * C54, 2)) -
                         ((((-sigmaAa * C58 * C44 * C62 * rhoA * 18. * C45 * C54) /
                                std::pow(9. * std::pow(C45, 2) * C54, 2) -
                            (-0.19672 * rhoA * rhoB * C54 * 2 * C45 * std::pow(C54, 2)) /
                                std::pow(C45 * C54, 4)) -
                           sigmaAb * 8. * C58 / (3. * C54)) -
                          sigmaBb * C58 * C44 * C62 * rhoB * 18. * C45 * C54 /
                              std::pow(9. * std::pow(C45, 2) * C54, 2)) *
                             C69) +
                        (sigmaBb * C58 * C44 * C62 * rhoB /
                             (9. * std::pow(C45, 2) * std::pow(C54, 2)) -
                         (sigmaAb * C58 * 8. * C45 / (3. * std::pow(C54, 2)) -
                          ((std::pow(C45 * C54, 2) * 0.19672 * rhoA * rhoB -
                            0.19672 * rhoA * rhoB * C45 * 2 * C45 * std::pow(C54, 2)) /
                               std::pow(C45 * C54, 4) +
                           sigmaAa * C58 * C44 * C62 * rhoA /
                               (9. * std::pow(C45, 2) * std::pow(C54, 2))))) *
                            C75) +
                       ((-sigmaAa * C58 * C44 * rhoA * 9. * C54) / std::pow(C45 * 9. * C54, 2) -
                        sigmaBb * C58 * C44 * rhoB / (std::pow(C45, 2) * 9. * C54)) *
                           C83) -
                      C68 * (sigmaAa * C58 * C62 * rhoA + sigmaBb * C58 * C62 * rhoB) /
                          (std::pow(C45, 2) * 9. * C54)) -
                     C79 *
                         ((sigmaAa * C44 * C62 * rhoA / (9. * std::pow(C45, 2)) -
                           sigmaAb * 8. * C45 / 3.) +
                          sigmaBb * C44 * C62 * rhoB / (9. * std::pow(C45, 2))) /
                         C54) *
                        C86) +
                   (C75 * 10. * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA *
                        0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                        std::pow(2., 11. / 3.) / std::pow(10. * C54, 2) -
                    ((std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * 0.00649176 *
                         std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) +
                     C48 * std::pow(rhoA, C48 - 1) * rhoB * rhoA * 0.00649176 *
                         std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.)) /
                        (10. * C54)) *
                       C94) +
                  ((((((sigmaAa * C58 * C62 / (C45 * 9. * C54) -
                        C69 * (sigmaAa * C58 * C62 * rhoA + sigmaBb * C58 * C62 * rhoB) /
                            (9. * std::pow(C45, 2) * C54)) +
                       C75 *
                           (sigmaBb * C58 * (C63 - C62 * rhoB / C45) +
                            (sigmaAa * C58 * (C63 - C62 * rhoA / C45) +
                             sigmaAb * C58 * (47. - 7. * C60))) /
                           (9. * std::pow(C54, 2))) -
                      C84 * (sigmaAa * C58 + sigmaBb * C58) / (9. * C54)) +
                     (sigmaAa * C58 * rhoA / (C45 * 9. * C54) -
                      (-sigmaBb * C58 * rhoB / C45) / (9. * C54)) *
                         C83) -
                    C79 *
                        ((sigmaAa * (C63 - C62 * rhoA / C45) + sigmaAb * (47. - 7. * C60)) +
                         sigmaBb * (C63 - C62 * rhoB / C45)) /
                        (9. * C54)) -
                   C81 * sigmaAb * -7. * C58 / (9. * C54)) *
                      C85) +
                 ((((sigmaAa * C58 * C44 / (C45 * 9. * C54) -
                     C69 * (sigmaAa * C58 * C44 * rhoA + sigmaBb * C58 * C44 * rhoB) /
                         (9. * std::pow(C45, 2) * C54)) +
                    ((-sigmaBb * C58 * C44 * rhoB / C45) / (9. * std::pow(C54, 2)) -
                     sigmaAa * C58 * C44 * rhoA / (C45 * 9. * std::pow(C54, 2))) *
                        C75) -
                   (-C68 * (sigmaBb * C58 * rhoB + sigmaAa * C58 * rhoA) / C45) / (9. * C54)) -
                  C79 * ((-sigmaBb * C44 * rhoB / C45) / 9. - sigmaAa * C44 * rhoA / (9. * C45)) /
                      C54) *
                     C100) +
                ((C75 * (sigmaBb * C58 * C44 + sigmaAa * C58 * C44) / (9. * std::pow(C54, 2)) -
                  C68 * (sigmaAa * C58 + sigmaBb * C58) / (9. * C54)) -
                 C79 * (sigmaAa * C44 + sigmaBb * C44) / (9. * C54)) *
                    C101) +
               ((C75 * sigmaAb * C58 * -7. * C44 / (9. * std::pow(C54, 2)) -
                 C68 * sigmaAb * -7. * C58 / (9. * C54)) -
                C79 * sigmaAb * -7. * C44 / (9. * C54)) *
                   C98) +
              (sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) * C58 /
                   std::pow(C54, 2) -
               ((((-10. * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA * 0.00649176 *
                   std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) * C56) /
                      std::pow(10. * C54, 2) -
                  0.19672 * rhoA * rhoB * C45 / std::pow(C45 * C54, 2)) -
                 sigmaAa * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) * C58 /
                     std::pow(C54, 2)) -
                sigmaAb * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.) * C58 /
                    std::pow(C54, 2))) *
                  C123) -
             C125 * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA * 0.00649176 *
                 std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) /
                 (10. * C54)) -
            C132 * (sigmaAa * C58 * C44 + sigmaBb * C58 * C44) / (9. * C54)) +
           (sigmaAa * C58 * C44 * rhoA / (C45 * 9. * C54) -
            (-sigmaBb * C58 * C44 * rhoB / C45) / (9. * C54)) *
               C131) -
          C117 *
              ((sigmaAa * C58 * (C63 - C62 * rhoA / C45) + sigmaAb * C58 * (47. - 7. * C60)) +
               sigmaBb * C58 * (C63 - C62 * rhoB / C45)) /
              (9. * C54)) -
         C127 *
             ((sigmaAa * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) +
               sigmaAb * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.)) +
              sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2))) /
             C54) -
        C129 * sigmaAb * C58 * -7. * C44 / (9. * C54);

    matrix.upper[2] =
        (((((C58 * C44 * C62 / (C45 * 9. * C54) - C84 * C58 * C44 / (9. * C54)) +
            C83 * C58 * C44 * rhoA / (C45 * 9. * C54)) -
           C69 * C58 * C44 * C62 * rhoA / (9. * std::pow(C45, 2) * C54)) -
          C68 * C58 * (C63 - C62 * rhoA / C45) / (9. * C54)) -
         C79 * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) / C54) +
        C75 * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) * C58 / std::pow(C54, 2);

    matrix.upper[3] =
        ((C69 * C58 * 8. * C45 / (3. * C54) -
          (C68 * C58 * (47. - 7. * C60) + C81 * C58 * -7. * C44) / (9. * C54)) -
         C79 * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.) / C54) +
        C75 * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.) * C58 / std::pow(C54, 2);

    matrix.upper[4] =
        (((((2 * rhoA * C58 / C54 - C84 * C58 * C44 / (9. * C54)) +
            C83 * C58 * C44 * rhoB / (C45 * 9. * C54)) -
           C69 * C58 * C44 * C62 * rhoB / (9. * std::pow(C45, 2) * C54)) -
          C68 * C58 * (C63 - C62 * rhoB / C45) / (9. * C54)) -
         C79 * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) / C54) +
        C75 * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) * C58 / std::pow(C54, 2);

    const double C133 = C46 * (C46 - 1) * std::pow(C45, C46 - 2) * std::pow(C86, 2);
    const double C134 = C47 * (C47 - 1) * std::pow(C45, C47 - 2) * std::pow(C86, 2);
    const double C135 =
        0.2533 * C134 / std::pow(C50, 2) - C88 * 0.2533 * C88 * 2 * C50 / std::pow(C50, 4);
    const double C136 =
        -0.2533 * C134 / std::pow(C50, 2) - C88 * -0.2533 * C88 * 2 * C50 / std::pow(C50, 4);
    const double C137 =
        -0.349 * C134 / std::pow(C50, 2) - C88 * -0.349 * C88 * 2 * C50 / std::pow(C50, 4);
    const double C138 = C137;
    const double C139 = std::exp(C51) * std::pow(C89, 2) + std::exp(C51) * C135;
    const double C140 = ((C87 * C93 + C55 * C133) + C93 * C87) + C49 * C139;
    const double C141 = ((C88 * C92 + C54 * C134) + C92 * C88) + C50 * C138;
    const double C142 = 0.00649176 * C140;
    const double C143 =
        -0.349 * C141 / std::pow(C57, 2) - C95 * -0.349 * C95 * 2 * C57 / std::pow(C57, 4);
    const double C144 = C136 + C143;
    const double C145 = 3. * C144;
    const double C146 = C144;
    const double C147 = -C145;
    matrix.upper[5] =
        ((((((((((((((((((((sigmaAa * 2 * C58 / C54 -
                            (C48 * std::pow(rhoB, C48 - 1) * rhoA * 0.00649176 *
                                 std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                 std::pow(2., 11. / 3.) * C56 +
                             (C48 * std::pow(rhoB, C48 - 1) * rhoA * 0.00649176 *
                                  std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                  std::pow(2., 11. / 3.) * C56 +
                              C48 * (C48 - 1) * std::pow(rhoB, C48 - 2) * rhoB * rhoA * 0.00649176 *
                                  std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                  std::pow(2., 11. / 3.) * C56)) /
                                (10. * C54)) -
                           (sigmaBb * C58 * C44 * C62 / (9. * std::pow(C45, 2) * C54) -
                            0.19672 * rhoA * C54 / std::pow(C45 * C54, 2)) *
                               C86) +
                          ((-sigmaBb * C58 * C44 * C62 / C45) / (9. * std::pow(C54, 2)) -
                           (((-10. * ((std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoA *
                                          0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) *
                                          3. * std::pow(2., 11. / 3.) * C56 +
                                      C48 * std::pow(rhoB, C48 - 1) * rhoB * rhoA * 0.00649176 *
                                          std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                          std::pow(2., 11. / 3.) * C56)) /
                                 std::pow(10. * C54, 2) -
                             0.19672 * rhoA * C45 / std::pow(C45 * C54, 2)) -
                            (-sigmaAa * 2 * rhoB * C58) / std::pow(C54, 2))) *
                              C92) -
                         C94 *
                             ((std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoA * 0.00649176 *
                                  std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                  std::pow(2., 11. / 3.) +
                              C48 * std::pow(rhoB, C48 - 1) * rhoB * rhoA * 0.00649176 *
                                  std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                  std::pow(2., 11. / 3.)) /
                             (10. * C54)) +
                        C100 * sigmaBb * C58 * C44 / (C45 * 9. * C54)) -
                       (-C85 * sigmaBb * C58 * C62 / C45) / (9. * C54)) -
                      C96 * ((-sigmaBb * C44 * C62 / C45) / 9. - sigmaAa * 2 * rhoB) / C54) +
                     (((((((sigmaBb * C44 * C62 / (C45 * 9. * C54) + sigmaAa * 2 * rhoB / C54) -
                           ((sigmaAa * C44 * C62 * rhoA / (9. * std::pow(C45, 2) * C54) -
                             sigmaAb * 8. * C45 / (3. * C54)) +
                            sigmaBb * C44 * C62 * rhoB / (9. * std::pow(C45, 2) * C54)) *
                               C86) +
                          C92 *
                              (sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) +
                               (sigmaAa *
                                    ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) +
                                sigmaAb *
                                    ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.))) /
                              std::pow(C54, 2)) -
                         C101 * (sigmaAa * C44 + sigmaBb * C44) / (9. * C54)) +
                        (sigmaAa * C44 * rhoA / (C45 * 9. * C54) -
                         (-sigmaBb * C44 * rhoB / C45) / (9. * C54)) *
                            C100) -
                       C85 *
                           ((sigmaAa * (C63 - C62 * rhoA / C45) + sigmaAb * (47. - 7. * C60)) +
                            sigmaBb * (C63 - C62 * rhoB / C45)) /
                           (9. * C54)) -
                      C98 * sigmaAb * -7. * C44 / (9. * C54)) *
                         C96) +
                    ((((((sigmaBb * C58 * C62 / (C45 * 9. * C54) -
                          C86 * (sigmaAa * C58 * C62 * rhoA + sigmaBb * C58 * C62 * rhoB) /
                              (9. * std::pow(C45, 2) * C54)) +
                         C92 *
                             (sigmaBb * C58 * (C63 - C62 * rhoB / C45) +
                              (sigmaAa * C58 * (C63 - C62 * rhoA / C45) +
                               sigmaAb * C58 * (47. - 7. * C60))) /
                             (9. * std::pow(C54, 2))) -
                        C101 * (sigmaAa * C58 + sigmaBb * C58) / (9. * C54)) +
                       (sigmaAa * C58 * rhoA / (C45 * 9. * C54) -
                        (-sigmaBb * C58 * rhoB / C45) / (9. * C54)) *
                           C100) -
                      C96 *
                          ((sigmaAa * (C63 - C62 * rhoA / C45) + sigmaAb * (47. - 7. * C60)) +
                           sigmaBb * (C63 - C62 * rhoB / C45)) /
                          (9. * C54)) -
                     C98 * sigmaAb * -7. * C58 / (9. * C54)) *
                        C85) +
                   ((((sigmaBb * C58 * C44 / (C45 * 9. * C54) -
                       C86 * (sigmaAa * C58 * C44 * rhoA + sigmaBb * C58 * C44 * rhoB) /
                           (9. * std::pow(C45, 2) * C54)) +
                      ((-sigmaBb * C58 * C44 * rhoB / C45) / (9. * std::pow(C54, 2)) -
                       sigmaAa * C58 * C44 * rhoA / (C45 * 9. * std::pow(C54, 2))) *
                          C92) -
                     (-C85 * (sigmaBb * C58 * rhoB + sigmaAa * C58 * rhoA) / C45) / (9. * C54)) -
                    C96 * ((-sigmaBb * C44 * rhoB / C45) / 9. - sigmaAa * C44 * rhoA / (9. * C45)) /
                        C54) *
                       C100) +
                  (((((((-sigmaBb * C58 * C44 * C62 * 9. * C54) / std::pow(C45 * 9. * C54, 2) -
                        (-0.19672 * rhoA * C54) / std::pow(C45 * C54, 2)) -
                       ((((-sigmaAa * C58 * C44 * C62 * rhoA * 18. * C45 * C54) /
                              std::pow(9. * std::pow(C45, 2) * C54, 2) -
                          (-0.19672 * rhoA * rhoB * C54 * 2 * C45 * std::pow(C54, 2)) /
                              std::pow(C45 * C54, 4)) -
                         sigmaAb * 8. * C58 / (3. * C54)) -
                        sigmaBb * C58 * C44 * C62 * rhoB * 18. * C45 * C54 /
                            std::pow(9. * std::pow(C45, 2) * C54, 2)) *
                           C86) +
                      (sigmaBb * C58 * C44 * C62 * rhoB /
                           (9. * std::pow(C45, 2) * std::pow(C54, 2)) -
                       (sigmaAb * C58 * 8. * C45 / (3. * std::pow(C54, 2)) -
                        ((std::pow(C45 * C54, 2) * 0.19672 * rhoA * rhoB -
                          0.19672 * rhoA * rhoB * C45 * 2 * C45 * std::pow(C54, 2)) /
                             std::pow(C45 * C54, 4) +
                         sigmaAa * C58 * C44 * C62 * rhoA /
                             (9. * std::pow(C45, 2) * std::pow(C54, 2))))) *
                          C92) +
                     ((-sigmaAa * C58 * C44 * rhoA * 9. * C54) / std::pow(C45 * 9. * C54, 2) -
                      sigmaBb * C58 * C44 * rhoB / (std::pow(C45, 2) * 9. * C54)) *
                         C100) -
                    C85 * (sigmaAa * C58 * C62 * rhoA + sigmaBb * C58 * C62 * rhoB) /
                        (std::pow(C45, 2) * 9. * C54)) -
                   C96 *
                       ((sigmaAa * C44 * C62 * rhoA / (9. * std::pow(C45, 2)) -
                         sigmaAb * 8. * C45 / 3.) +
                        sigmaBb * C44 * C62 * rhoB / (9. * std::pow(C45, 2))) /
                       C54) *
                      C86) +
                 ((((((((((-sigmaBb * C58 * C44 * C62 * 9. * C45) / std::pow(C45 * 9. * C54, 2) -
                          (((-10. * ((std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoA *
                                         0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                         std::pow(2., 11. / 3.) * C56 +
                                     C48 * std::pow(rhoB, C48 - 1) * rhoB * rhoA * 0.00649176 *
                                         std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                         std::pow(2., 11. / 3.) * C56)) /
                                std::pow(10. * C54, 2) -
                            0.19672 * rhoA * C45 / std::pow(C45 * C54, 2)) -
                           (-sigmaAa * 2 * rhoB * C58) / std::pow(C54, 2))) -
                         ((((-sigmaAa * C58 * C44 * C62 * rhoA * 9. * std::pow(C45, 2)) /
                                std::pow(9. * std::pow(C45, 2) * C54, 2) -
                            (std::pow(C45 * C54, 2) * 0.19672 * rhoA * rhoB -
                             0.19672 * rhoA * rhoB * C54 * 2 * C54 * std::pow(C45, 2)) /
                                std::pow(C45 * C54, 4)) -
                           (-3. * sigmaAb * C58 * 8. * C45) / std::pow(3. * C54, 2)) -
                          sigmaBb * C58 * C44 * C62 * rhoB * 9. * std::pow(C45, 2) /
                              std::pow(9. * std::pow(C45, 2) * C54, 2)) *
                             C86) +
                        ((-sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) *
                          C58 * 2 * C54) /
                             std::pow(C54, 4) -
                         (((10. * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA *
                                0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                                std::pow(2., 11. / 3.) * C56 * 200. * C54 / std::pow(10. * C54, 4) -
                            (-0.19672 * rhoA * rhoB * C45 * 2 * C54 * std::pow(C45, 2)) /
                                std::pow(C45 * C54, 4)) -
                           (-sigmaAa * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) *
                            C58 * 2 * C54) /
                               std::pow(C54, 4)) -
                          (-sigmaAb * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.) *
                           C58 * 2 * C54) /
                              std::pow(C54, 4))) *
                            C92) -
                       (-10. * C94 * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA *
                        0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                        std::pow(2., 11. / 3.)) /
                           std::pow(10. * C54, 2)) -
                      (-9. * C101 * (sigmaAa * C58 * C44 + sigmaBb * C58 * C44)) /
                          std::pow(9. * C54, 2)) +
                     ((-sigmaAa * C58 * C44 * rhoA * 9. * C45) / std::pow(C45 * 9. * C54, 2) -
                      (- -9. * sigmaBb * C58 * C44 * rhoB / C45) / std::pow(9. * C54, 2)) *
                         C100) -
                    (-9. * C85 *
                     ((sigmaAa * C58 * (C63 - C62 * rhoA / C45) +
                       sigmaAb * C58 * (47. - 7. * C60)) +
                      sigmaBb * C58 * (C63 - C62 * rhoB / C45))) /
                        std::pow(9. * C54, 2)) -
                   (-C96 * ((sigmaAa * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) +
                             sigmaAb * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.)) +
                            sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)))) /
                       std::pow(C54, 2)) -
                  (-9. * C98 * sigmaAb * C58 * -7. * C44) / std::pow(9. * C54, 2)) *
                     C92) +
                (C92 * 10. * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA *
                     0.00649176 * std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. *
                     std::pow(2., 11. / 3.) / std::pow(10. * C54, 2) -
                 ((std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoA * 0.00649176 *
                      std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) +
                  C48 * std::pow(rhoB, C48 - 1) * rhoB * rhoA * 0.00649176 *
                      std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.)) /
                     (10. * C54)) *
                    C94) +
               ((C92 * (sigmaBb * C58 * C44 + sigmaAa * C58 * C44) / (9. * std::pow(C54, 2)) -
                 C85 * (sigmaAa * C58 + sigmaBb * C58) / (9. * C54)) -
                C96 * (sigmaAa * C44 + sigmaBb * C44) / (9. * C54)) *
                   C101) +
              ((C92 * sigmaAb * C58 * -7. * C44 / (9. * std::pow(C54, 2)) -
                C85 * sigmaAb * -7. * C58 / (9. * C54)) -
               C96 * sigmaAb * -7. * C44 / (9. * C54)) *
                  C98) +
             (sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) * C58 /
                  std::pow(C54, 2) -
              ((((-10. * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA * 0.00649176 *
                  std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) * C56) /
                     std::pow(10. * C54, 2) -
                 0.19672 * rhoA * rhoB * C45 / std::pow(C45 * C54, 2)) -
                sigmaAa * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) * C58 /
                    std::pow(C54, 2)) -
               sigmaAb * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.) * C58 /
                   std::pow(C54, 2))) *
                 C138) -
            C140 * (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA * 0.00649176 *
                std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) /
                (10. * C54)) -
           C147 * (sigmaAa * C58 * C44 + sigmaBb * C58 * C44) / (9. * C54)) +
          (sigmaAa * C58 * C44 * rhoA / (C45 * 9. * C54) -
           (-sigmaBb * C58 * C44 * rhoB / C45) / (9. * C54)) *
              C146) -
         C142 *
             ((sigmaAa * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) +
               sigmaAb * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.)) +
              sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2))) /
             C54) -
        C144 * sigmaAb * C58 * -7. * C44 / (9. * C54);

    matrix.upper[6] =
        (((((2 * rhoB * C58 / C54 - C101 * C58 * C44 / (9. * C54)) +
            C100 * C58 * C44 * rhoA / (C45 * 9. * C54)) -
           C86 * C58 * C44 * C62 * rhoA / (9. * std::pow(C45, 2) * C54)) -
          C85 * C58 * (C63 - C62 * rhoA / C45) / (9. * C54)) -
         C96 * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) / C54) +
        C92 * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) * C58 / std::pow(C54, 2);

    matrix.upper[7] =
        ((C86 * C58 * 8. * C45 / (3. * C54) -
          (C85 * C58 * (47. - 7. * C60) + C98 * C58 * -7. * C44) / (9. * C54)) -
         C96 * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.) / C54) +
        C92 * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.) * C58 / std::pow(C54, 2);

    matrix.upper[8] =
        (((((C58 * C44 * C62 / (C45 * 9. * C54) - C101 * C58 * C44 / (9. * C54)) +
            C100 * C58 * C44 * rhoB / (C45 * 9. * C54)) -
           C86 * C58 * C44 * C62 * rhoB / (9. * std::pow(C45, 2) * C54)) -
          C85 * C58 * (C63 - C62 * rhoB / C45) / (9. * C54)) -
         C96 * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) / C54) +
        C92 * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) * C58 / std::pow(C54, 2);

    matrix.upper[9] = 0;

    matrix.upper[10] = 0;

    matrix.upper[11] = 0;

    matrix.upper[12] = 0;

    matrix.upper[13] = 0;

    matrix.upper[14] = 0;
}

} // namespace excgrid
