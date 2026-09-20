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
    const double C99 = -2. / 3.;
    const double C100 = std::pow(C68, 2);
    const double C101 = std::pow(C70, 4);
    const double C102 = 0.004 * C101;
    const double C103 = C66 * C100;
    const double C104 = rhoA * C96;
    const double C105 = -C98;
    const double C106 = std::pow(C58, C99);
    const double C107 = 2. * C103;
    const double C108 = C103 * C95;
    const double C109 = C106 * C97;
    const double C110 = C88 + C102;
    const double C111 = std::exp(C105);
    const double C112 = 0.1508 * C111;
    const double C113 = 2. * C108;
    const double C114 = rhoA * C109;
    const double C115 = C110 + 1.;
    const double C116 = 2. * C114;
    const double C117 = Pi * C115;
    const double C118 = 0.2743 - C112;
    const double C119 = C118 * C74;
    const double C120 = C116 / 3.;
    const double C121 = C88 + C119;
    const double C122 = C94 + C120;
    const double C123 = 2. * C122;
    const double C124 = C122 * C48;
    const double C125 = C121 + 1.;
    const double C126 = C54 * C123;
    const double C127 = C86 * C123;
    const double C128 = 7.7956 * C126;
    const double C129 = C126 * C60;
    const double C130 = -0.155912e2 * C129;
    const double C131 = C128 / C100;
    const double C132 = C130 / C113;
    const double C133 = C132 - C131;
    const double C134 = C54 * C133;
    const double C135 = 0.19645 * C134;
    const double C136 = C66 * C135;
    const double C137 = 2. * C136;
    const double C138 = C137 / C82;
    const double C139 = C138 - C127;

    result.vrhoA = -(4. * C117 *
                         (C125 * (rhoA * 3. * C109 / 3. + C96) +
                          (C139 / C100 + (C118 * -4. * C124 / C107 -
                                          C74 * -0.1508 * C111 * -400. * C124 / C107)) *
                              C104) -
                     C125 * C104 * 4. * Pi * (C139 + 0.004 * std::pow(C70, 3) * -4 * C126) / C100) /
                   std::pow(4. * C117, 2);

    const double C141 = 2. * C63;
    const double C142 = 2 * C81;
    const double C143 = 3. * C63;
    const double C144 = 6. * C51;
    const double C145 = 100. * C75;
    const double C146 = -2. / 3.;
    const double C147 = std::pow(C69, 2);
    const double C148 = std::pow(C71, 4);
    const double C149 = 0.004 * C148;
    const double C150 = C67 * C147;
    const double C151 = rhoB * C143;
    const double C152 = -C145;
    const double C153 = std::pow(C59, C146);
    const double C154 = 2. * C150;
    const double C155 = C150 * C142;
    const double C156 = C153 * C144;
    const double C157 = C89 + C149;
    const double C158 = std::exp(C152);
    const double C159 = 0.1508 * C158;
    const double C160 = 2. * C155;
    const double C161 = rhoB * C156;
    const double C162 = C157 + 1.;
    const double C163 = 2. * C161;
    const double C164 = 0.2743 - C159;
    const double C165 = C164 * C75;
    const double C166 = C163 / 3.;
    const double C167 = C141 + C166;
    const double C168 = C89 + C165;
    const double C169 = 2. * C167;
    const double C170 = C167 * C49;
    const double C171 = C168 + 1.;
    const double C172 = C55 * C169;
    const double C173 = C87 * C169;
    const double C174 = 7.7956 * C172;
    const double C175 = C172 * C61;
    const double C176 = -0.155912e2 * C175;
    const double C177 = C174 / C147;
    const double C178 = C176 / C160;
    const double C179 = C178 - C177;
    const double C180 = C55 * C179;
    const double C181 = 0.19645 * C180;
    const double C182 = C67 * C181;
    const double C183 = 2. * C182;
    const double C184 = C183 / C83;
    const double C185 = C184 - C173;

    result.vrhoB = ((C185 + 0.004 * std::pow(C71, 3) * -4 * C172) * C171 * C151 / (4. * Pi * C147) -
                    C162 *
                        (C171 * (rhoB * 3. * C156 / 3. + C143) +
                         (C185 / C147 +
                          (C164 * -4. * C170 / C154 - C75 * -0.1508 * C158 * -400. * C170 / C154)) *
                             C151) /
                        (4. * Pi)) /
                   std::pow(C162, 2);

    const double C187 = 2. * rhoA;
    const double C188 = 4. * sigmaAa;
    const double C189 = 1. / 3.;
    const double C190 = std::pow(Pi, 2);
    const double C191 = C190 * C187;
    const double C192 = std::sqrt(C188);
    const double C193 = 0.19645 * C192;
    const double C194 = 2 * C192;
    const double C195 = 3. * C191;
    const double C196 = 7.7956 * C192;
    const double C197 = 0.48617103488e3 * C192;
    const double C198 = std::pow(C195, C189);
    const double C199 = 3. * C198;
    const double C200 = C198 * C187;
    const double C201 = rhoA * C199;
    const double C202 = C200 + 1e-16;
    const double C203 = 2. * C202;
    const double C204 = C192 * C203;
    const double C205 = C192 / C203;
    const double C206 = C196 / C203;
    const double C207 = 2 * C204;
    const double C208 = std::pow(C205, 2);
    const double C209 = std::pow(C205, 4);
    const double C210 = std::pow(C206, 2);
    const double C211 = 0.004 * C209;
    const double C212 = 100. * C208;
    const double C213 = C202 * C207;
    const double C214 = C210 + 1.;
    const double C215 = 0.311824e2 / C207;
    const double C216 = 2. * C213;
    const double C217 = -C212;
    const double C218 = std::sqrt(C214);
    const double C219 = 2 * C218;
    const double C220 = C206 + C218;
    const double C221 = std::exp(C217);
    const double C222 = 0.1508 * C221;
    const double C223 = C213 * C219;
    const double C224 = std::log(C220);
    const double C225 = 0.7858 * C224;
    const double C226 = 2. * C223;
    const double C227 = C224 * C193;
    const double C228 = 0.2743 - C222;
    const double C229 = C197 / C226;
    const double C230 = C225 / C194;
    const double C231 = C227 / C203;
    const double C232 = C215 + C229;
    const double C233 = C231 + C211;
    const double C234 = C192 * C232;
    const double C235 = C233 + 1.;
    const double C236 = 0.19645 * C234;
    const double C237 = Pi * C235;
    const double C238 = C236 / C220;
    const double C239 = C230 + C238;
    const double C240 = C239 / C203;

    result.vsigmaAa =
        -(4. * C237 *
              (C240 + (C228 * 8. * C192 / C216 - C208 * -0.1508 * C221 * 800. * C192 / C216)) *
              C201 -
          ((C231 + C228 * C208) + 1.) * C201 * 4. * Pi *
              (C240 + 0.064 * std::pow(C205, 3) / C207)) /
        std::pow(4. * C237, 2);

    result.vsigmaAb = 0;

    const double C243 = 2. * rhoB;
    const double C244 = 4. * Pi;
    const double C245 = 4. * sigmaBb;
    const double C246 = 1. / 3.;
    const double C247 = std::pow(Pi, 2);
    const double C248 = C247 * C243;
    const double C249 = std::sqrt(C245);
    const double C250 = 0.19645 * C249;
    const double C251 = 2 * C249;
    const double C252 = 3. * C248;
    const double C253 = 7.7956 * C249;
    const double C254 = 0.48617103488e3 * C249;
    const double C255 = std::pow(C252, C246);
    const double C256 = 3. * C255;
    const double C257 = C255 * C243;
    const double C258 = rhoB * C256;
    const double C259 = C257 + 1e-16;
    const double C260 = 2. * C259;
    const double C261 = C249 * C260;
    const double C262 = C249 / C260;
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
    const double C284 = C281 * C250;
    const double C285 = 0.2743 - C279;
    const double C286 = C254 / C283;
    const double C287 = C282 / C251;
    const double C288 = C284 / C260;
    const double C289 = C272 + C286;
    const double C290 = C288 + C268;
    const double C291 = C249 * C289;
    const double C292 = C290 + 1.;
    const double C293 = 0.19645 * C291;
    const double C294 = C293 / C277;
    const double C295 = C287 + C294;
    const double C296 = C295 / C260;

    result.vsigmaBb =
        ((C296 + 0.064 * std::pow(C262, 3) / C264) * ((C288 + C285 * C265) + 1.) * C258 / C244 -
         C292 * (C296 + (C285 * 8. * C249 / C273 - C265 * -0.1508 * C278 * 800. * C249 / C273)) *
             C258 / C244) /
        std::pow(C292, 2);

    return result;
}

} // namespace excgrid
