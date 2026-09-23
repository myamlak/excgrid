// excgrid codegen source: the Perdew-Burke-Ernzerhof 1996 GGA exchange
// functional.  Fresh authorship; formula from the PBE paper
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

XcKernelValue PbeExchange(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
    (void)sigmaAb;

    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;
    // The sigma-edge guard the generator adds to a vanishing channel radical
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

    const double C232 = 2. * rhoA;
    const double C233 = 2. * rhoB;
    const double C234 = 1. / 3.;
    const double C235 = std::pow(Pi, 2);
    const double C236 = C235 * C232;
    const double C237 = C235 * C233;
    const double C238 = 3. * C236;
    const double C239 = 3. * C237;
    const double C240 = std::pow(C238, C234);
    const double C241 = std::pow(C239, C234);

    result.exc =
        -((1.804 -
           0.804 /
               (0.2195149727645171 *
                    std::pow(std::sqrt(4. * sigmaBb) / (2. * (C241 * C233 + 1e-16)), 2) / 0.804 +
                1.)) *
              rhoB * 3. * C241 +
          (1.804 -
           0.804 /
               (0.2195149727645171 *
                    std::pow(std::sqrt(4. * sigmaAa) / (2. * (C240 * C232 + 1e-16)), 2) / 0.804 +
                1.)) *
              rhoA * 3. * C240) /
        (4. * Pi);

    const double C246 = 3. * C240;
    const double C247 = 4. * sigmaAa;
    const double C248 = 6. * C235;
    const double C249 = C240 * C232;
    const double C250 = C249 + 1e-16;
    const double C251 = C240 / C248;
    const double C252 = std::sqrt(C247);
    const double C253 = 2. * C250;
    const double C254 = C252 / C253;
    const double C255 = std::pow(C254, 2);
    const double C256 = 0.2195149727645171 * C255;
    const double C257 = C256 / 0.804;
    const double C258 = C257 + 1.;

    result.vrhoA = -((1.804 - 0.804 / C258) * (18. * C235 * C251 / 3. + C246) +
                     rhoA * C246 * -0.705960152410687 * (2. * C240 + 2. * C248 * C251 / 3.) * C247 /
                         (1.608 * C250 * std::pow(C253, 2) * std::pow(C258, 2))) /
                   (4. * Pi);

    const double C260 = 3. * C241;
    const double C261 = 4. * sigmaBb;
    const double C262 = 6. * C235;
    const double C263 = C241 * C233;
    const double C264 = C263 + 1e-16;
    const double C265 = C241 / C262;
    const double C266 = std::sqrt(C261);
    const double C267 = 2. * C264;
    const double C268 = C266 / C267;
    const double C269 = std::pow(C268, 2);
    const double C270 = 0.2195149727645171 * C269;
    const double C271 = C270 / 0.804;
    const double C272 = C271 + 1.;

    result.vrhoB = -((1.804 - 0.804 / C272) * (18. * C235 * C265 / 3. + C260) +
                     rhoB * C260 * -0.705960152410687 * (2. * C241 + 2. * C262 * C265 / 3.) * C261 /
                         (1.608 * C264 * std::pow(C267, 2) * std::pow(C272, 2))) /
                   (4. * Pi);

    const double C274 = 2. * rhoA;
    const double C275 = 1. / 3.;
    const double C276 = std::pow(Pi, 2);
    const double C277 = C276 * C274;
    const double C278 = 3. * C277;
    const double C279 = std::pow(C278, C275);
    const double C280 = C279 * C274;
    const double C281 = C280 + 1e-16;

    result.vsigmaAa =
        -4.2357609144641219 * C279 * rhoA /
        (0.25728e2 * Pi *
         std::pow(0.2195149727645171 *
                          std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) / (2. * C281), 2) /
                          0.804 +
                      1.,
                  2) *
         std::pow(C281, 2));

    result.vsigmaAb = 0;

    const double C284 = 2. * rhoB;
    const double C285 = 1. / 3.;
    const double C286 = std::pow(Pi, 2);
    const double C287 = C286 * C284;
    const double C288 = 3. * C287;
    const double C289 = std::pow(C288, C285);
    const double C290 = C289 * C284;
    const double C291 = C290 + 1e-16;

    result.vsigmaBb =
        -4.2357609144641219 * C289 * rhoB /
        (0.25728e2 * Pi *
         std::pow(0.2195149727645171 *
                          std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) / (2. * C291), 2) /
                          0.804 +
                      1.,
                  2) *
         std::pow(C291, 2));

    return result;
}

void PbeExchangeSecondDerivatives(double rhoA,
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

    const double C216 = 2. * rhoB;
    const double C217 = 4. * sigmaBb;
    const double C218 = 1. / 3.;
    const double C219 = std::pow(Pi, 2);
    const double C220 = C219 * C216;
    const double C221 = std::sqrt(C217);
    const double C222 = 3. * C220;
    const double C223 = std::pow(C222, C218);
    const double C224 = C223 * C216;
    const double C225 = C224 + 1e-16;
    const double C226 = 2. * C225;
    const double C227 = C221 / C226;
    const double C228 = std::pow(C227, 2);
    const double C229 = 0.2195149727645171 * C228;
    const double C230 = C229 / 0.804;
    const double C231 = C230 + 1.;
    matrix.upper[14] =
        3.5122395642322736 * C231 * 4. * Pi * 4. * rhoB * 3. * C223 * 1.4119203048213739 /
        (std::pow(Pi * 4. * std::pow(C231, 2) * C225 * 2. * 2 * C225 * 1.608, 2) * 2. * 2);

    matrix.upper[13] = 0;

    matrix.upper[12] = 0;

    matrix.upper[11] = 0;

    matrix.upper[10] = 0;

    const double C200 = 2. * rhoA;
    const double C201 = 4. * sigmaAa;
    const double C202 = 1. / 3.;
    const double C203 = std::pow(Pi, 2);
    const double C204 = C203 * C200;
    const double C205 = std::sqrt(C201);
    const double C206 = 3. * C204;
    const double C207 = std::pow(C206, C202);
    const double C208 = C207 * C200;
    const double C209 = C208 + 1e-16;
    const double C210 = 2. * C209;
    const double C211 = C205 / C210;
    const double C212 = std::pow(C211, 2);
    const double C213 = 0.2195149727645171 * C212;
    const double C214 = C213 / 0.804;
    const double C215 = C214 + 1.;
    matrix.upper[9] =
        3.5122395642322736 * C215 * 4. * Pi * 4. * rhoA * 3. * C207 * 1.4119203048213739 /
        (std::pow(Pi * 4. * std::pow(C215, 2) * C209 * 2. * 2 * C209 * 1.608, 2) * 2. * 2);

    const double C167 = 2. * rhoB;
    const double C168 = 4. * sigmaBb;
    const double C169 = Pi * 4.;
    const double C170 = -2. / 3.;
    const double C171 = 1. / 3.;
    const double C172 = std::pow(Pi, 2);
    const double C173 = 6. * C172;
    const double C174 = C172 * C167;
    const double C175 = std::sqrt(C168);
    const double C176 = 3. * C174;
    const double C177 = std::pow(C176, C170);
    const double C178 = std::pow(C176, C171);
    const double C179 = 2. * C178;
    const double C180 = C177 * C173;
    const double C181 = C178 * C167;
    const double C182 = rhoB * C180;
    const double C183 = C181 + 1e-16;
    const double C184 = 2. * C182;
    const double C185 = 2. * C183;
    const double C186 = C175 / C185;
    const double C187 = C184 / 3.;
    const double C188 = C179 + C187;
    const double C189 = std::pow(C186, 2);
    const double C190 = 0.2195149727645171 * C189;
    const double C191 = C190 / 0.804;
    const double C192 = C191 + 1.;
    const double C193 = std::pow(C192, 2);
    const double C194 = C169 * C193;
    const double C195 = C194 * C183;
    const double C196 = C195 * 2.;
    const double C197 = C196 * 2;
    const double C198 = C197 * C183;
    const double C199 = C198 * 1.608;
    matrix.upper[8] =
        -(C199 * (4.2357609144641219 * C178 + rhoB * 4.2357609144641219 * C180 / 3.) -
          1.4119203048213739 * C178 * 3. * rhoB * 1.608 *
              (C197 * C188 + 4. *
                                 (C194 * C188 + C183 * C169 * C192 * -1.7561197821161368 * C188 *
                                                    C168 / (1.608 * C183 * std::pow(C185, 2))) *
                                 C183)) /
        std::pow(C199, 2);

    matrix.upper[7] = 0;

    matrix.upper[6] = 0;

    const double C123 = 2. * rhoB;
    const double C124 = 4. * sigmaBb;
    const double C125 = -5. / 3.;
    const double C126 = -2. / 3.;
    const double C127 = 1. / 3.;
    const double C128 = std::pow(Pi, 2);
    const double C129 = -12. * C128;
    const double C130 = 6. * C128;
    const double C131 = C128 * C123;
    const double C132 = std::sqrt(C124);
    const double C133 = 3. * C131;
    const double C134 = std::pow(C133, C125);
    const double C135 = std::pow(C133, C126);
    const double C136 = std::pow(C133, C127);
    const double C137 = 2. * C136;
    const double C138 = 3. * C136;
    const double C139 = C134 * C129;
    const double C140 = C135 * C130;
    const double C141 = C136 * C123;
    const double C142 = 3. * C140;
    const double C143 = C128 * C139;
    const double C144 = rhoB * C138;
    const double C145 = rhoB * C140;
    const double C146 = C141 + 1e-16;
    const double C147 = 2. * C145;
    const double C148 = 2. * C146;
    const double C149 = rhoB * C142;
    const double C150 = C132 / C148;
    const double C151 = C147 / 3.;
    const double C152 = C149 / 3.;
    const double C153 = std::pow(C148, 2);
    const double C154 = C146 * C153;
    const double C155 = C137 + C151;
    const double C156 = C152 + C138;
    const double C157 = std::pow(C150, 2);
    const double C158 = 0.2195149727645171 * C157;
    const double C159 = C155 * C124;
    const double C160 = -0.705960152410687 * C159;
    const double C161 = C158 / 0.804;
    const double C162 = C156 * C160;
    const double C163 = C161 + 1.;
    const double C164 = std::pow(C163, 2);
    const double C165 = C154 * C164;
    const double C166 = 1.608 * C165;
    matrix.upper[5] =
        -(((1.804 - 0.804 / C163) * ((rhoB * 18. * C143 / 3. + C142) + C142) / 3. + C162 / C166) +
          (1.608 * C165 *
               (C144 * -2.8238406096427479 * sigmaBb *
                    (2. * C140 + 2. * (rhoB * 6. * C143 / 3. + C140)) / 3. +
                C162) -
           C144 * C160 * 1.608 *
               (C154 * C163 * -1.7561197821161368 * C159 / (1.608 * C154) +
                (C146 * 4. * C155 * C148 + C155 * C153) * C164)) /
              std::pow(C166, 2)) /
        (4. * Pi);

    matrix.upper[4] = 0;

    matrix.upper[3] = 0;

    const double C90 = 2. * rhoA;
    const double C91 = 4. * sigmaAa;
    const double C92 = Pi * 4.;
    const double C93 = -2. / 3.;
    const double C94 = 1. / 3.;
    const double C95 = std::pow(Pi, 2);
    const double C96 = 6. * C95;
    const double C97 = C95 * C90;
    const double C98 = std::sqrt(C91);
    const double C99 = 3. * C97;
    const double C100 = std::pow(C99, C93);
    const double C101 = std::pow(C99, C94);
    const double C102 = 2. * C101;
    const double C103 = C100 * C96;
    const double C104 = C101 * C90;
    const double C105 = rhoA * C103;
    const double C106 = C104 + 1e-16;
    const double C107 = 2. * C105;
    const double C108 = 2. * C106;
    const double C109 = C107 / 3.;
    const double C110 = C98 / C108;
    const double C111 = C102 + C109;
    const double C112 = std::pow(C110, 2);
    const double C113 = 0.2195149727645171 * C112;
    const double C114 = C113 / 0.804;
    const double C115 = C114 + 1.;
    const double C116 = std::pow(C115, 2);
    const double C117 = C92 * C116;
    const double C118 = C117 * C106;
    const double C119 = C118 * 2.;
    const double C120 = C119 * 2;
    const double C121 = C120 * C106;
    const double C122 = C121 * 1.608;
    matrix.upper[2] =
        -(C122 * (4.2357609144641219 * C101 + rhoA * 4.2357609144641219 * C103 / 3.) -
          1.4119203048213739 * C101 * 3. * rhoA * 1.608 *
              (C120 * C111 + 4. *
                                 (C117 * C111 + C106 * C92 * C115 * -1.7561197821161368 * C111 *
                                                    C91 / (1.608 * C106 * std::pow(C108, 2))) *
                                 C106)) /
        std::pow(C122, 2);

    matrix.upper[1] = 0;

    const double C46 = 2. * rhoA;
    const double C47 = 4. * sigmaAa;
    const double C48 = -5. / 3.;
    const double C49 = -2. / 3.;
    const double C50 = 1. / 3.;
    const double C51 = std::pow(Pi, 2);
    const double C52 = -12. * C51;
    const double C53 = 6. * C51;
    const double C54 = C51 * C46;
    const double C55 = std::sqrt(C47);
    const double C56 = 3. * C54;
    const double C57 = std::pow(C56, C48);
    const double C58 = std::pow(C56, C49);
    const double C59 = std::pow(C56, C50);
    const double C60 = 2. * C59;
    const double C61 = 3. * C59;
    const double C62 = C57 * C52;
    const double C63 = C58 * C53;
    const double C64 = C59 * C46;
    const double C65 = 3. * C63;
    const double C66 = C51 * C62;
    const double C67 = rhoA * C61;
    const double C68 = rhoA * C63;
    const double C69 = C64 + 1e-16;
    const double C70 = 2. * C68;
    const double C71 = 2. * C69;
    const double C72 = rhoA * C65;
    const double C73 = C55 / C71;
    const double C74 = C70 / 3.;
    const double C75 = C72 / 3.;
    const double C76 = std::pow(C71, 2);
    const double C77 = C69 * C76;
    const double C78 = C60 + C74;
    const double C79 = C75 + C61;
    const double C80 = std::pow(C73, 2);
    const double C81 = 0.2195149727645171 * C80;
    const double C82 = C78 * C47;
    const double C83 = -0.705960152410687 * C82;
    const double C84 = C81 / 0.804;
    const double C85 = C79 * C83;
    const double C86 = C84 + 1.;
    const double C87 = std::pow(C86, 2);
    const double C88 = C77 * C87;
    const double C89 = 1.608 * C88;
    matrix.upper[0] =
        -(((1.804 - 0.804 / C86) * ((rhoA * 18. * C66 / 3. + C65) + C65) / 3. + C85 / C89) +
          (1.608 * C88 *
               (C67 * -2.8238406096427479 * sigmaAa *
                    (2. * C63 + 2. * (rhoA * 6. * C66 / 3. + C63)) / 3. +
                C85) -
           C67 * C83 * 1.608 *
               (C77 * C86 * -1.7561197821161368 * C82 / (1.608 * C77) +
                (C69 * 4. * C78 * C71 + C78 * C76) * C87)) /
              std::pow(C89, 2)) /
        (4. * Pi);
}

} // namespace excgrid
