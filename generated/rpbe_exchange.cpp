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

    const double C222 = 2. * rhoA;
    const double C223 = 2. * rhoB;
    const double C224 = 1. / 3.;
    const double C225 = std::pow(Pi, 2);
    const double C226 = C225 * C222;
    const double C227 = C225 * C223;
    const double C228 = 3. * C226;
    const double C229 = 3. * C227;
    const double C230 = std::pow(C228, C224);
    const double C231 = std::pow(C229, C224);

    result.exc =
        -((0.804 *
               (1. - std::exp(-0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaBb) / (2. * (C231 * C223 + 1e-16)), 2) /
                              0.804)) +
           1.) *
              rhoB * 3. * C231 +
          (0.804 *
               (1. - std::exp(-0.2195149727645171 *
                              std::pow(std::sqrt(4. * sigmaAa) / (2. * (C230 * C222 + 1e-16)), 2) /
                              0.804)) +
           1.) *
              rhoA * 3. * C230) /
        (4. * Pi);

    const double C236 = 3. * C230;
    const double C237 = 4. * sigmaAa;
    const double C238 = 6. * C225;
    const double C239 = C230 * C222;
    const double C240 = C239 + 1e-16;
    const double C241 = C230 / C238;
    const double C242 = std::sqrt(C237);
    const double C243 = 2. * C240;
    const double C244 = C242 / C243;
    const double C245 = std::pow(C244, 2);
    const double C246 = 0.2195149727645171 * C245;
    const double C247 = C246 / 0.804;
    const double C248 = -C247;
    const double C249 = std::exp(C248);

    result.vrhoA =
        -((0.804 * (1. - C249) + 1.) * (18. * C225 * C241 / 3. + C236) +
          rhoA * C236 * 0.804 * C249 * -0.8780598910580684 * (2. * C230 + 2. * C238 * C241 / 3.) *
              C237 / (1.608 * C240 * std::pow(C243, 2))) /
        (4. * Pi);

    const double C251 = 3. * C231;
    const double C252 = 4. * sigmaBb;
    const double C253 = 6. * C225;
    const double C254 = C231 * C223;
    const double C255 = C254 + 1e-16;
    const double C256 = C231 / C253;
    const double C257 = std::sqrt(C252);
    const double C258 = 2. * C255;
    const double C259 = C257 / C258;
    const double C260 = std::pow(C259, 2);
    const double C261 = 0.2195149727645171 * C260;
    const double C262 = C261 / 0.804;
    const double C263 = -C262;
    const double C264 = std::exp(C263);

    result.vrhoB =
        -((0.804 * (1. - C264) + 1.) * (18. * C225 * C256 / 3. + C251) +
          rhoB * C251 * 0.804 * C264 * -0.8780598910580684 * (2. * C231 + 2. * C253 * C256 / 3.) *
              C252 / (1.608 * C255 * std::pow(C258, 2))) /
        (4. * Pi);

    const double C266 = 2. * rhoA;
    const double C267 = 1. / 3.;
    const double C268 = std::pow(Pi, 2);
    const double C269 = C268 * C266;
    const double C270 = 3. * C269;
    const double C271 = std::pow(C270, C267);
    const double C272 = C271 * C266;
    const double C273 = C272 + 1e-16;

    result.vsigmaAa =
        -4.2357609144641219 *
        std::exp(-0.2195149727645171 *
                 std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) / (2. * C273), 2) / 0.804) *
        C271 * rhoA / (0.25728e2 * Pi * std::pow(C273, 2));

    result.vsigmaAb = 0;

    const double C276 = 2. * rhoB;
    const double C277 = 1. / 3.;
    const double C278 = std::pow(Pi, 2);
    const double C279 = C278 * C276;
    const double C280 = 3. * C279;
    const double C281 = std::pow(C280, C277);
    const double C282 = C281 * C276;
    const double C283 = C282 + 1e-16;

    result.vsigmaBb =
        -4.2357609144641219 *
        std::exp(-0.2195149727645171 *
                 std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) / (2. * C283), 2) / 0.804) *
        C281 * rhoB / (0.25728e2 * Pi * std::pow(C283, 2));

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

    const double C214 = 2. * rhoB;
    const double C215 = 1. / 3.;
    const double C216 = std::pow(Pi, 2);
    const double C217 = C216 * C214;
    const double C218 = 3. * C217;
    const double C219 = std::pow(C218, C215);
    const double C220 = C219 * C214;
    const double C221 = C220 + 1e-16;
    matrix.upper[14] =
        -1.7561197821161368 *
        std::exp(-0.2195149727645171 * std::pow(std::sqrt(4. * sigmaBb) / (2. * C221), 2) / 0.804) *
        -1.4119203048213739 * C219 * 3. * rhoB /
        (1.608 * C221 * 2 * 2. * C221 * 4. * Pi * C221 * 2. * 2 * C221 * 1.608);

    matrix.upper[13] = 0;

    matrix.upper[12] = 0;

    matrix.upper[11] = 0;

    matrix.upper[10] = 0;

    const double C206 = 2. * rhoA;
    const double C207 = 1. / 3.;
    const double C208 = std::pow(Pi, 2);
    const double C209 = C208 * C206;
    const double C210 = 3. * C209;
    const double C211 = std::pow(C210, C207);
    const double C212 = C211 * C206;
    const double C213 = C212 + 1e-16;
    matrix.upper[9] =
        -1.7561197821161368 *
        std::exp(-0.2195149727645171 * std::pow(std::sqrt(4. * sigmaAa) / (2. * C213), 2) / 0.804) *
        -1.4119203048213739 * C211 * 3. * rhoA /
        (1.608 * C213 * 2 * 2. * C213 * 4. * Pi * C213 * 2. * 2 * C213 * 1.608);

    const double C171 = 2. * rhoB;
    const double C172 = 4. * sigmaBb;
    const double C173 = Pi * 4.;
    const double C174 = -2. / 3.;
    const double C175 = 1. / 3.;
    const double C176 = std::pow(Pi, 2);
    const double C177 = 6. * C176;
    const double C178 = C176 * C171;
    const double C179 = std::sqrt(C172);
    const double C180 = 3. * C178;
    const double C181 = std::pow(C180, C174);
    const double C182 = std::pow(C180, C175);
    const double C183 = 2. * C182;
    const double C184 = C181 * C177;
    const double C185 = C182 * C171;
    const double C186 = rhoB * C184;
    const double C187 = C185 + 1e-16;
    const double C188 = 2. * C186;
    const double C189 = 2. * C187;
    const double C190 = C173 * C187;
    const double C191 = C190 * 2.;
    const double C192 = C179 / C189;
    const double C193 = C188 / 3.;
    const double C194 = C191 * 2;
    const double C195 = C183 + C193;
    const double C196 = std::pow(C192, 2);
    const double C197 = 0.2195149727645171 * C196;
    const double C198 = C194 * C187;
    const double C199 = C198 * 1.608;
    const double C200 = C197 / 0.804;
    const double C201 = -C200;
    const double C202 = std::exp(C201);
    const double C203 = 1.7561197821161368 * C202;
    const double C204 = C203 * 0.804;
    const double C205 = C204 * C182;
    matrix.upper[8] =
        -(C199 * (3. * C205 +
                  3. *
                      (C204 * C184 / 3. + C182 * -1.4119203048213739 * C202 * -0.8780598910580684 *
                                              C195 * C172 / (1.608 * C187 * std::pow(C189, 2))) *
                      rhoB) -
          C205 * 3. * rhoB * 1.608 * (C194 * C195 + 4. * C173 * C195 * C187)) /
        std::pow(C199, 2);

    matrix.upper[7] = 0;

    matrix.upper[6] = 0;

    const double C126 = 2. * rhoB;
    const double C127 = 4. * sigmaBb;
    const double C128 = -5. / 3.;
    const double C129 = -2. / 3.;
    const double C130 = 1. / 3.;
    const double C131 = std::pow(Pi, 2);
    const double C132 = -12. * C131;
    const double C133 = 6. * C131;
    const double C134 = C131 * C126;
    const double C135 = std::sqrt(C127);
    const double C136 = 3. * C134;
    const double C137 = std::pow(C136, C128);
    const double C138 = std::pow(C136, C129);
    const double C139 = std::pow(C136, C130);
    const double C140 = 2. * C139;
    const double C141 = 3. * C139;
    const double C142 = C137 * C132;
    const double C143 = C138 * C133;
    const double C144 = C139 * C126;
    const double C145 = 3. * C143;
    const double C146 = C131 * C142;
    const double C147 = rhoB * C141;
    const double C148 = rhoB * C143;
    const double C149 = C144 + 1e-16;
    const double C150 = 2. * C148;
    const double C151 = 2. * C149;
    const double C152 = rhoB * C145;
    const double C153 = C135 / C151;
    const double C154 = C150 / 3.;
    const double C155 = C152 / 3.;
    const double C156 = std::pow(C151, 2);
    const double C157 = C149 * C156;
    const double C158 = C140 + C154;
    const double C159 = C155 + C141;
    const double C160 = std::pow(C153, 2);
    const double C161 = 0.2195149727645171 * C160;
    const double C162 = 1.608 * C157;
    const double C163 = C158 * C127;
    const double C164 = -0.8780598910580684 * C163;
    const double C165 = C161 / 0.804;
    const double C166 = -C165;
    const double C167 = std::exp(C166);
    const double C168 = C167 * C164;
    const double C169 = 0.804 * C168;
    const double C170 = C159 * C169;
    matrix.upper[5] =
        -(((0.804 * (1. - C167) + 1.) * ((rhoB * 18. * C146 / 3. + C145) + C145) / 3. +
           C170 / C162) +
          (1.608 * C157 *
               (C147 * 0.804 *
                    (C167 * -3.5122395642322736 * sigmaBb *
                         (2. * C143 + 2. * (rhoB * 6. * C146 / 3. + C143)) / 3. -
                     -0.8780598910580684 * C163 * C168 / C162) +
                C170) -
           C147 * C169 * 1.608 * (C149 * 4. * C158 * C151 + C158 * C156)) /
              std::pow(C162, 2)) /
        (4. * Pi);

    matrix.upper[4] = 0;

    matrix.upper[3] = 0;

    const double C91 = 2. * rhoA;
    const double C92 = 4. * sigmaAa;
    const double C93 = Pi * 4.;
    const double C94 = -2. / 3.;
    const double C95 = 1. / 3.;
    const double C96 = std::pow(Pi, 2);
    const double C97 = 6. * C96;
    const double C98 = C96 * C91;
    const double C99 = std::sqrt(C92);
    const double C100 = 3. * C98;
    const double C101 = std::pow(C100, C94);
    const double C102 = std::pow(C100, C95);
    const double C103 = 2. * C102;
    const double C104 = C101 * C97;
    const double C105 = C102 * C91;
    const double C106 = rhoA * C104;
    const double C107 = C105 + 1e-16;
    const double C108 = 2. * C106;
    const double C109 = 2. * C107;
    const double C110 = C93 * C107;
    const double C111 = C110 * 2.;
    const double C112 = C108 / 3.;
    const double C113 = C99 / C109;
    const double C114 = C111 * 2;
    const double C115 = C103 + C112;
    const double C116 = std::pow(C113, 2);
    const double C117 = 0.2195149727645171 * C116;
    const double C118 = C114 * C107;
    const double C119 = C118 * 1.608;
    const double C120 = C117 / 0.804;
    const double C121 = -C120;
    const double C122 = std::exp(C121);
    const double C123 = 1.7561197821161368 * C122;
    const double C124 = C123 * 0.804;
    const double C125 = C124 * C102;
    matrix.upper[2] =
        -(C119 * (3. * C125 +
                  3. *
                      (C124 * C104 / 3. + C102 * -1.4119203048213739 * C122 * -0.8780598910580684 *
                                              C115 * C92 / (1.608 * C107 * std::pow(C109, 2))) *
                      rhoA) -
          C125 * 3. * rhoA * 1.608 * (C114 * C115 + 4. * C93 * C115 * C107)) /
        std::pow(C119, 2);

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
    const double C82 = 1.608 * C77;
    const double C83 = C78 * C47;
    const double C84 = -0.8780598910580684 * C83;
    const double C85 = C81 / 0.804;
    const double C86 = -C85;
    const double C87 = std::exp(C86);
    const double C88 = C87 * C84;
    const double C89 = 0.804 * C88;
    const double C90 = C79 * C89;
    matrix.upper[0] =
        -(((0.804 * (1. - C87) + 1.) * ((rhoA * 18. * C66 / 3. + C65) + C65) / 3. + C90 / C82) +
          (1.608 * C77 *
               (C67 * 0.804 *
                    (C87 * -3.5122395642322736 * sigmaAa *
                         (2. * C63 + 2. * (rhoA * 6. * C66 / 3. + C63)) / 3. -
                     -0.8780598910580684 * C83 * C88 / C82) +
                C90) -
           C67 * C89 * 1.608 * (C69 * 4. * C78 * C71 + C78 * C76)) /
              std::pow(C82, 2)) /
        (4. * Pi);
}

} // namespace excgrid
