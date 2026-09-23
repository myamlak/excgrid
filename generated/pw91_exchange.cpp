// excgrid codegen source: the Perdew-Wang 1991 GGA exchange functional.
// Fresh authorship; formula from the PW91 paper (docs/mainpage.md
// references).  Regenerate with tools/regenerate.py.

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

// The excgrid GGA kernel skeleton (fresh authorship; BSD-3-Clause).
// Emits one excgrid::XcKernelValue function per GGA functional: the energy
// density, the spin-density derivatives, and the three sigma derivatives.
// No spin-edge branches: the definitions' Tiny guards keep every expression
// finite at the spin and gamma edges (the LDA skeleton's exact-limit
// machinery is not needed for the gradient-corrected forms, whose edge
// contributions vanish with the spin density).
// The banner and the kernel.hpp include are emitted by the calling .ey, which
// always precedes this skeleton, so they are not repeated here.

#include <cmath>
#include <limits>

namespace excgrid {

XcKernelValue Pw91Exchange(
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

    const double C46 = 2. * rhoA;
    const double C47 = 2. * rhoB;
    const double C48 = 4. * sigmaAa;
    const double C49 = 4. * sigmaBb;
    const double C50 = 1. / 3.;
    const double C51 = std::pow(Pi, 2);
    const double C52 = C51 * C46;
    const double C53 = C51 * C47;
    const double C54 = std::sqrt(C48);
    const double C55 = std::sqrt(C49);
    const double C56 = 0.19645 * C54;
    const double C57 = 0.19645 * C55;
    const double C58 = 3. * C52;
    const double C59 = 3. * C53;
    const double C60 = 7.7956 * C54;
    const double C61 = 7.7956 * C55;
    const double C62 = std::pow(C58, C50);
    const double C63 = std::pow(C59, C50);
    const double C64 = C62 * C46;
    const double C65 = C63 * C47;
    const double C66 = C64 + 1e-16;
    const double C67 = C65 + 1e-16;
    const double C68 = 2. * C66;
    const double C69 = 2. * C67;
    const double C70 = C54 / C68;
    const double C71 = C55 / C69;
    const double C72 = C60 / C68;
    const double C73 = C61 / C69;
    const double C74 = std::pow(C70, 2);
    const double C75 = std::pow(C71, 2);
    const double C76 = std::pow(C72, 2);
    const double C77 = std::pow(C73, 2);
    const double C78 = C76 + 1.;
    const double C79 = C77 + 1.;
    const double C80 = std::sqrt(C78);
    const double C81 = std::sqrt(C79);
    const double C82 = C72 + C80;
    const double C83 = C73 + C81;
    const double C84 = std::log(C82);
    const double C85 = std::log(C83);
    const double C86 = C84 * C56;
    const double C87 = C85 * C57;
    const double C88 = C86 / C68;
    const double C89 = C87 / C69;

    result.exc = (-((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * rhoB * 3. *
                  C63 / (4. * Pi)) /
                     ((C89 + 0.004 * std::pow(C71, 4)) + 1.) -
                 ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. * C62 /
                     (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.));

    const double C94 = 2. * C62;
    const double C95 = 2 * C80;
    const double C96 = 3. * C62;
    const double C97 = 6. * C51;
    const double C98 = 100. * C74;
    const double C99 = std::pow(C68, 2);
    const double C100 = std::pow(C70, 4);
    const double C101 = 0.004 * C100;
    const double C102 = C66 * C99;
    const double C103 = rhoA * C96;
    const double C104 = -C98;
    const double C105 = C62 / C97;
    const double C106 = 2. * C102;
    const double C107 = C102 * C95;
    const double C108 = C97 * C105;
    const double C109 = C88 + C101;
    const double C110 = std::exp(C104);
    const double C111 = 0.1508 * C110;
    const double C112 = 2. * C107;
    const double C113 = 2. * C108;
    const double C114 = C109 + 1.;
    const double C115 = Pi * C114;
    const double C116 = 0.2743 - C111;
    const double C117 = C113 / 3.;
    const double C118 = C116 * C74;
    const double C119 = C94 + C117;
    const double C120 = 2. * C119;
    const double C121 = C119 * C48;
    const double C122 = C88 + C118;
    const double C123 = C54 * C120;
    const double C124 = C86 * C120;
    const double C125 = C122 + 1.;
    const double C126 = 7.7956 * C123;
    const double C127 = C123 * C60;
    const double C128 = -0.155912e2 * C127;
    const double C129 = C126 / C99;
    const double C130 = C128 / C112;
    const double C131 = C130 - C129;
    const double C132 = C54 * C131;
    const double C133 = 0.19645 * C132;
    const double C134 = C66 * C133;
    const double C135 = 2. * C134;
    const double C136 = C135 / C82;
    const double C137 = C136 - C124;

    result.vrhoA = -(4. * C115 *
                         (C125 * (18. * C51 * C105 / 3. + C96) +
                          (C137 / C99 + (C116 * -4. * C121 / C106 -
                                         C74 * -0.1508 * C110 * -400. * C121 / C106)) *
                              C103) -
                     C125 * C103 * 4. * Pi * (C137 + 0.004 * std::pow(C70, 3) * -4 * C123) / C99) /
                   std::pow(4. * C115, 2);

    const double C139 = 2. * C63;
    const double C140 = 2 * C81;
    const double C141 = 3. * C63;
    const double C142 = 6. * C51;
    const double C143 = 100. * C75;
    const double C144 = std::pow(C69, 2);
    const double C145 = std::pow(C71, 4);
    const double C146 = 0.004 * C145;
    const double C147 = C67 * C144;
    const double C148 = rhoB * C141;
    const double C149 = -C143;
    const double C150 = C63 / C142;
    const double C151 = 2. * C147;
    const double C152 = C142 * C150;
    const double C153 = C147 * C140;
    const double C154 = C89 + C146;
    const double C155 = std::exp(C149);
    const double C156 = 0.1508 * C155;
    const double C157 = 2. * C152;
    const double C158 = 2. * C153;
    const double C159 = C154 + 1.;
    const double C160 = 0.2743 - C156;
    const double C161 = C157 / 3.;
    const double C162 = C160 * C75;
    const double C163 = C139 + C161;
    const double C164 = 2. * C163;
    const double C165 = C163 * C49;
    const double C166 = C89 + C162;
    const double C167 = C55 * C164;
    const double C168 = C87 * C164;
    const double C169 = C166 + 1.;
    const double C170 = 7.7956 * C167;
    const double C171 = C167 * C61;
    const double C172 = -0.155912e2 * C171;
    const double C173 = C170 / C144;
    const double C174 = C172 / C158;
    const double C175 = C174 - C173;
    const double C176 = C55 * C175;
    const double C177 = 0.19645 * C176;
    const double C178 = C67 * C177;
    const double C179 = 2. * C178;
    const double C180 = C179 / C83;
    const double C181 = C180 - C168;

    result.vrhoB = ((C181 + 0.004 * std::pow(C71, 3) * -4 * C167) * C169 * C148 / (4. * Pi * C144) -
                    C159 *
                        (C169 * (18. * C51 * C150 / 3. + C141) +
                         (C181 / C144 +
                          (C160 * -4. * C165 / C151 - C75 * -0.1508 * C155 * -400. * C165 / C151)) *
                             C148) /
                        (4. * Pi)) /
                   std::pow(C159, 2);

    const double C183 = 2. * rhoA;
    const double C184 = 4. * SigmaGuard;
    const double C185 = 4. * sigmaAa;
    const double C186 = 1. / 3.;
    const double C187 = std::pow(Pi, 2);
    const double C188 = C187 * C183;
    const double C189 = C185 + C184;
    const double C190 = 3. * C188;
    const double C191 = std::sqrt(C189);
    const double C192 = 0.19645 * C191;
    const double C193 = 2 * C191;
    const double C194 = 7.7956 * C191;
    const double C195 = 0.48617103488e3 * C191;
    const double C196 = std::pow(C190, C186);
    const double C197 = 3. * C196;
    const double C198 = C196 * C183;
    const double C199 = rhoA * C197;
    const double C200 = C198 + 1e-16;
    const double C201 = 2. * C200;
    const double C202 = C191 * C201;
    const double C203 = C191 / C201;
    const double C204 = C194 / C201;
    const double C205 = 2 * C202;
    const double C206 = std::pow(C203, 2);
    const double C207 = std::pow(C203, 4);
    const double C208 = std::pow(C204, 2);
    const double C209 = 0.004 * C207;
    const double C210 = 100. * C206;
    const double C211 = C200 * C205;
    const double C212 = C208 + 1.;
    const double C213 = 0.311824e2 / C205;
    const double C214 = 2. * C211;
    const double C215 = -C210;
    const double C216 = std::sqrt(C212);
    const double C217 = 2 * C216;
    const double C218 = C204 + C216;
    const double C219 = std::exp(C215);
    const double C220 = 0.1508 * C219;
    const double C221 = C211 * C217;
    const double C222 = std::log(C218);
    const double C223 = 0.7858 * C222;
    const double C224 = 2. * C221;
    const double C225 = C222 * C192;
    const double C226 = 0.2743 - C220;
    const double C227 = C195 / C224;
    const double C228 = C223 / C193;
    const double C229 = C225 / C201;
    const double C230 = C213 + C227;
    const double C231 = C229 + C209;
    const double C232 = C191 * C230;
    const double C233 = C231 + 1.;
    const double C234 = 0.19645 * C232;
    const double C235 = Pi * C233;
    const double C236 = C234 / C218;
    const double C237 = C228 + C236;
    const double C238 = C237 / C201;

    result.vsigmaAa =
        -(4. * C235 *
              (C238 + (C226 * 8. * C191 / C214 - C206 * -0.1508 * C219 * 800. * C191 / C214)) *
              C199 -
          ((C229 + C226 * C206) + 1.) * C199 * 4. * Pi *
              (C238 + 0.064 * std::pow(C203, 3) / C205)) /
        std::pow(4. * C235, 2);

    result.vsigmaAb = 0;

    const double C241 = 2. * rhoB;
    const double C242 = 4. * Pi;
    const double C243 = 4. * SigmaGuard;
    const double C244 = 4. * sigmaBb;
    const double C245 = 1. / 3.;
    const double C246 = std::pow(Pi, 2);
    const double C247 = C246 * C241;
    const double C248 = C244 + C243;
    const double C249 = 3. * C247;
    const double C250 = std::sqrt(C248);
    const double C251 = 0.19645 * C250;
    const double C252 = 2 * C250;
    const double C253 = 7.7956 * C250;
    const double C254 = 0.48617103488e3 * C250;
    const double C255 = std::pow(C249, C245);
    const double C256 = 3. * C255;
    const double C257 = C255 * C241;
    const double C258 = rhoB * C256;
    const double C259 = C257 + 1e-16;
    const double C260 = 2. * C259;
    const double C261 = C250 * C260;
    const double C262 = C250 / C260;
    const double C263 = C253 / C260;
    const double C264 = 2 * C261;
    const double C265 = std::pow(C262, 2);
    const double C266 = std::pow(C262, 4);
    const double C267 = std::pow(C263, 2);
    const double C268 = 0.004 * C266;
    const double C269 = 100. * C265;
    const double C270 = C259 * C264;
    const double C271 = C267 + 1.;
    const double C272 = 0.311824e2 / C264;
    const double C273 = 2. * C270;
    const double C274 = -C269;
    const double C275 = std::sqrt(C271);
    const double C276 = 2 * C275;
    const double C277 = C263 + C275;
    const double C278 = std::exp(C274);
    const double C279 = 0.1508 * C278;
    const double C280 = C270 * C276;
    const double C281 = std::log(C277);
    const double C282 = 0.7858 * C281;
    const double C283 = 2. * C280;
    const double C284 = C281 * C251;
    const double C285 = 0.2743 - C279;
    const double C286 = C254 / C283;
    const double C287 = C282 / C252;
    const double C288 = C284 / C260;
    const double C289 = C272 + C286;
    const double C290 = C288 + C268;
    const double C291 = C250 * C289;
    const double C292 = C290 + 1.;
    const double C293 = 0.19645 * C291;
    const double C294 = C293 / C277;
    const double C295 = C287 + C294;
    const double C296 = C295 / C260;

    result.vsigmaBb =
        ((C296 + 0.064 * std::pow(C262, 3) / C264) * ((C288 + C285 * C265) + 1.) * C258 / C242 -
         C292 * (C296 + (C285 * 8. * C250 / C273 - C265 * -0.1508 * C278 * 800. * C250 / C273)) *
             C258 / C242) /
        std::pow(C292, 2);

    return result;
}

} // namespace excgrid
