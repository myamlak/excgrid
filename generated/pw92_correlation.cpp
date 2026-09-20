// excgrid codegen source: the PW92 LDA correlation functional
// (Perdew-Wang 1992).  Fresh authorship; formula from the PW92 paper
// (docs/mainpage.md references).  Regenerate with tools/regenerate.py.

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

// The excgrid LDA kernel skeleton (fresh authorship; BSD-3-Clause).
// Emits one excgrid::XcKernelValue function per LDA functional: the energy
// density and its spin-density derivatives, with the exact one-spin-zero
// limit branches (the generator's limit substitution supplies the limit
// expressions).
// The banner and the kernel.hpp include are emitted by the calling .ey, which
// always precedes this skeleton, so they are not repeated here.

#include <cmath>
#include <limits>

namespace excgrid {

XcKernelValue Pw92Correlation(double rhoA, double rhoB) {
    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        return result;
    }

    if (rhoA >= eps && rhoB >= eps)
    {
        const double C46 = rhoA + rhoB;
        const double C47 = rhoA - rhoB;
        const double C48 = 1. / 3.;
        const double C49 = 2. / 3.;
        const double C50 = 4. / 3.;
        const double C51 = 0.9999999999999999 * C47;
        const double C52 = Pi * C46;
        const double C53 = C47 / C46;
        const double C54 = std::pow(2., C48);
        const double C55 = 4. * C52;
        const double C56 = C54 - 1.;
        const double C57 = C51 / C46;
        const double C58 = std::pow(C53, 4);
        const double C59 = C57 + 1.;
        const double C60 = 1. - C57;
        const double C61 = 3. / C55;
        const double C62 = std::sqrt(C61);
        const double C63 = std::pow(C59, C50);
        const double C64 = std::pow(C60, C50);
        const double C65 = std::pow(C61, C48);
        const double C66 = std::pow(C61, C49);
        const double C67 = 0.2137 * C65;
        const double C68 = 0.49294 * C66;
        const double C69 = 1.6382 * C62;
        const double C70 = 3.5876 * C65;
        const double C71 = C63 + C64;
        const double C72 = std::sqrt(C65);
        const double C73 = 7.5957 * C72;
        const double C74 = C67 + 1.;
        const double C75 = C71 - 2.;
        const double C76 = C73 + C70;
        const double C77 = C76 + C69;
        const double C78 = C77 + C68;
        const double C79 = 0.062182 * C78;
        const double C80 = 1. / C79;
        const double C81 = C80 + 1.;
        const double C82 = std::log(C81);
        const double C83 = C74 * C82;
        const double C84 = 0.062182 * C83;

        result.exc =
            C46 * ((0.033774 * (0.11125 * C65 + 1.) *
                        std::log(1. / (0.033774 * (((10.357 * C72 + 3.6231 * C65) + 0.88026 * C62) +
                                                   0.49671 * C66)) +
                                 1.) *
                        C75 * (1. - C58) / (3.4198418683227306 * C56) -
                    C84) +
                   C58 *
                       (C84 - 0.03109 * (0.20548 * C65 + 1.) *
                                  std::log(1. / (0.03109 *
                                                 (((14.1189 * C72 + 6.1977 * C65) + 3.3662 * C62) +
                                                  0.62517 * C66)) +
                                           1.)) *
                       C75 / (2. * C56));
    } else if (rhoA < eps)
    {
        const double C89 = -0.9999999999999999 * rhoB;
        const double C90 = Pi * rhoB;
        const double C91 = -rhoB;
        const double C92 = 1. / 3.;
        const double C93 = 2. / 3.;
        const double C94 = 4. / 3.;
        const double C95 = 4. * C90;
        const double C96 = C89 / rhoB;
        const double C97 = C91 / rhoB;
        const double C98 = std::pow(2., C92);
        const double C99 = C96 + 1.;
        const double C100 = 1. - C96;
        const double C101 = C98 - 1.;
        const double C102 = 3. / C95;
        const double C103 = std::pow(C97, 4);
        const double C104 = std::sqrt(C102);
        const double C105 = std::pow(C100, C94);
        const double C106 = std::pow(C102, C92);
        const double C107 = std::pow(C102, C93);
        const double C108 = std::pow(C99, C94);
        const double C109 = 0.2137 * C106;
        const double C110 = 0.49294 * C107;
        const double C111 = 1.6382 * C104;
        const double C112 = 3.5876 * C106;
        const double C113 = C108 + C105;
        const double C114 = std::sqrt(C106);
        const double C115 = 7.5957 * C114;
        const double C116 = C109 + 1.;
        const double C117 = C113 - 2.;
        const double C118 = C115 + C112;
        const double C119 = C118 + C111;
        const double C120 = C119 + C110;
        const double C121 = 0.062182 * C120;
        const double C122 = 1. / C121;
        const double C123 = C122 + 1.;
        const double C124 = std::log(C123);
        const double C125 = C116 * C124;
        const double C126 = 0.062182 * C125;

        result.exc =
            rhoB *
            ((0.033774 * (0.11125 * C106 + 1.) *
                  std::log(1. / (0.033774 * (((10.357 * C114 + 3.6231 * C106) + 0.88026 * C104) +
                                             0.49671 * C107)) +
                           1.) *
                  C117 * (1. - C103) / (3.4198418683227306 * C101) -
              C126) +
             C103 *
                 (C126 -
                  0.03109 * (0.20548 * C106 + 1.) *
                      std::log(1. / (0.03109 * (((14.1189 * C114 + 6.1977 * C106) + 3.3662 * C104) +
                                                0.62517 * C107)) +
                               1.)) *
                 C117 / (2. * C101));
    } else
    {
        const double C128 = 0.9999999999999999 * rhoA;
        const double C129 = Pi * rhoA;
        const double C130 = 1. / 3.;
        const double C131 = 2. / 3.;
        const double C132 = 4. / 3.;
        const double C133 = 4. * C129;
        const double C134 = C128 / rhoA;
        const double C135 = 3. / C133;
        const double C136 = std::sqrt(C135);
        const double C137 = std::pow(C135, C130);
        const double C138 = std::pow(C135, C131);
        const double C139 = 0.2137 * C137;
        const double C140 = 0.49294 * C138;
        const double C141 = 1.6382 * C136;
        const double C142 = 3.5876 * C137;
        const double C143 = std::sqrt(C137);
        const double C144 = 7.5957 * C143;
        const double C145 = C139 + 1.;
        const double C146 = C144 + C142;
        const double C147 = C146 + C141;
        const double C148 = C147 + C140;
        const double C149 = 0.062182 * C148;
        const double C150 = 1. / C149;
        const double C151 = C150 + 1.;
        const double C152 = std::log(C151);
        const double C153 = C145 * C152;
        const double C154 = 0.062182 * C153;

        result.exc =
            rhoA * ((C154 - 0.03109 * (0.20548 * C137 + 1.) *
                                std::log(1. / (0.03109 *
                                               (((14.1189 * C143 + 6.1977 * C137) + 3.3662 * C136) +
                                                0.62517 * C138)) +
                                         1.)) *
                        ((std::pow(C134 + 1., C132) + std::pow(1. - C134, C132)) - 2.) /
                        (2. * (std::pow(2., C130) - 1.)) -
                    C154);
    }

    if (rhoA >= eps)
    {
        const double C130 = 1. / 3.;
        const double C131 = 2. / 3.;
        const double C132 = 4. / 3.;
        const double C156 = -24. * Pi;
        const double C157 = -0.196584e2 * Pi;
        const double C158 = 12. * Pi;
        const double C159 = rhoA + rhoB;
        const double C160 = rhoA - rhoB;
        const double C161 = -2. / 3.;
        const double C162 = -1. / 3.;
        const double C163 = std::pow(2., C130);
        const double C164 = 0.9999999999999999 * C159;
        const double C165 = 0.9999999999999999 * C160;
        const double C166 = Pi * C159;
        const double C167 = C159 - C160;
        const double C168 = C163 - 1.;
        const double C169 = C160 / C159;
        const double C170 = std::pow(C159, 2);
        const double C171 = 2. * C168;
        const double C172 = 3. * C170;
        const double C173 = 3.4198418683227306 * C168;
        const double C174 = 4. * C166;
        const double C175 = 4 * C167;
        const double C176 = C164 - C165;
        const double C177 = C165 / C159;
        const double C178 = std::pow(C169, 3);
        const double C179 = std::pow(C169, 4);
        const double C180 = -4. * C176;
        const double C181 = 4. * C176;
        const double C182 = C178 * C175;
        const double C183 = C177 + 1.;
        const double C184 = 1. - C177;
        const double C185 = 1. - C179;
        const double C186 = 3. / C174;
        const double C187 = std::pow(C174, 2);
        const double C188 = 3. * C187;
        const double C189 = std::sqrt(C186);
        const double C190 = std::pow(C183, C130);
        const double C191 = std::pow(C183, C132);
        const double C192 = std::pow(C184, C130);
        const double C193 = std::pow(C184, C132);
        const double C194 = std::pow(C186, C130);
        const double C195 = std::pow(C186, C131);
        const double C196 = std::pow(C186, C161);
        const double C197 = std::pow(C186, C162);
        const double C198 = 0.11125 * C194;
        const double C199 = 0.20548 * C194;
        const double C200 = 0.2137 * C194;
        const double C201 = 0.49294 * C195;
        const double C202 = 0.49671 * C195;
        const double C203 = 0.62517 * C195;
        const double C204 = 0.88026 * C189;
        const double C205 = 1.6382 * C189;
        const double C206 = 2 * C189;
        const double C207 = 3.3662 * C189;
        const double C208 = 3.5876 * C194;
        const double C209 = 3.6231 * C194;
        const double C210 = 6.1977 * C194;
        const double C211 = C190 * C181;
        const double C212 = C192 * C180;
        const double C213 = C196 * C158;
        const double C214 = C197 * C156;
        const double C215 = C191 + C193;
        const double C216 = std::sqrt(C194);
        const double C217 = -7.5957 * C213;
        const double C218 = -3.5876 * C213;
        const double C219 = -0.2137 * C213;
        const double C220 = 0.49294 * C214;
        const double C221 = 2 * C216;
        const double C222 = 7.5957 * C216;
        const double C223 = 10.357 * C216;
        const double C224 = 14.1189 * C216;
        const double C225 = C187 * C206;
        const double C226 = C198 + 1.;
        const double C227 = C199 + 1.;
        const double C228 = C200 + 1.;
        const double C229 = C211 + C212;
        const double C230 = C215 - 2.;
        const double C231 = C187 * C221;
        const double C232 = C222 + C208;
        const double C233 = C223 + C209;
        const double C234 = C224 + C210;
        const double C235 = C157 / C225;
        const double C236 = C218 / C188;
        const double C237 = C220 / C188;
        const double C238 = 3. * C231;
        const double C239 = C232 + C205;
        const double C240 = C233 + C204;
        const double C241 = C234 + C207;
        const double C242 = C239 + C201;
        const double C243 = C240 + C202;
        const double C244 = C241 + C203;
        const double C245 = C217 / C238;
        const double C246 = 0.03109 * C244;
        const double C247 = 0.033774 * C243;
        const double C248 = 0.062182 * C242;
        const double C249 = C245 + C236;
        const double C250 = C249 + C235;
        const double C251 = 1. / C246;
        const double C252 = 1. / C247;
        const double C253 = 1. / C248;
        const double C254 = std::pow(C248, 2);
        const double C255 = C250 + C237;
        const double C256 = C251 + 1.;
        const double C257 = C252 + 1.;
        const double C258 = C253 + 1.;
        const double C259 = 0.062182 * C255;
        const double C260 = C254 * C258;
        const double C261 = std::log(C256);
        const double C262 = std::log(C257);
        const double C263 = std::log(C258);
        const double C264 = C226 * C262;
        const double C265 = C227 * C261;
        const double C266 = C228 * C259;
        const double C267 = C228 * C263;
        const double C268 = C263 * C219;
        const double C269 = 0.03109 * C265;
        const double C270 = 0.062182 * C267;
        const double C271 = C264 * C230;
        const double C272 = C266 / C260;
        const double C273 = C268 / C188;
        const double C274 = C270 - C269;
        const double C275 = C273 - C272;
        const double C276 = 0.062182 * C275;
        const double C277 = C274 * C230;

        result.vrhoA =
            C159 * ((0.033774 *
                         ((C264 * C229 / C172 +
                           (C262 * -0.11125 * C213 / C188 -
                            C226 * 0.033774 *
                                (((-10.357 * C213 / C238 + -3.6231 * C213 / C188) +
                                  -10.56312 * Pi / C225) +
                                 0.49671 * C214 / C188) /
                                (std::pow(C247, 2) * C257)) *
                               C230) *
                              C185 -
                          C271 * C182 / C170) /
                         C173 -
                     C276) +
                    (C179 * (C274 * C229 / C172 +
                             (C276 -
                              0.03109 * (C261 * -0.20548 * C213 / C188 -
                                         C227 * 0.03109 *
                                             (((-0.141189e2 * C213 / C238 + -6.1977 * C213 / C188) +
                                               -0.403944e2 * Pi / C225) +
                                              0.62517 * C214 / C188) /
                                             (std::pow(C246, 2) * C256))) *
                                 C230) +
                     C277 * C182 / C170) /
                        C171) +
            ((0.033774 * C271 * C185 / C173 - C270) + C179 * C277 / C171);
    }

    if (rhoB >= eps)
    {
        const double C92 = 1. / 3.;
        const double C93 = 2. / 3.;
        const double C94 = 4. / 3.;
        const double C98 = std::pow(2., C92);
        const double C101 = C98 - 1.;
        const double C279 = -24. * Pi;
        const double C280 = -0.196584e2 * Pi;
        const double C281 = 2. * C101;
        const double C282 = 3.4198418683227306 * C101;
        const double C283 = 12. * Pi;
        const double C284 = rhoA + rhoB;
        const double C285 = rhoA - rhoB;
        const double C286 = -2. / 3.;
        const double C287 = -1. / 3.;
        const double C288 = -0.9999999999999999 * C284;
        const double C289 = 0.9999999999999999 * C285;
        const double C290 = Pi * C284;
        const double C291 = C284 + C285;
        const double C292 = C285 / C284;
        const double C293 = std::pow(C284, 2);
        const double C294 = -4 * C291;
        const double C295 = 3. * C293;
        const double C296 = 4. * C290;
        const double C297 = C288 - C289;
        const double C298 = C289 / C284;
        const double C299 = std::pow(C292, 3);
        const double C300 = std::pow(C292, 4);
        const double C301 = -4. * C297;
        const double C302 = 4. * C297;
        const double C303 = C299 * C294;
        const double C304 = C298 + 1.;
        const double C305 = 1. - C298;
        const double C306 = 1. - C300;
        const double C307 = 3. / C296;
        const double C308 = std::pow(C296, 2);
        const double C309 = 3. * C308;
        const double C310 = std::sqrt(C307);
        const double C311 = std::pow(C304, C92);
        const double C312 = std::pow(C304, C94);
        const double C313 = std::pow(C305, C92);
        const double C314 = std::pow(C305, C94);
        const double C315 = std::pow(C307, C286);
        const double C316 = std::pow(C307, C287);
        const double C317 = std::pow(C307, C92);
        const double C318 = std::pow(C307, C93);
        const double C319 = 0.11125 * C317;
        const double C320 = 0.20548 * C317;
        const double C321 = 0.2137 * C317;
        const double C322 = 0.49294 * C318;
        const double C323 = 0.49671 * C318;
        const double C324 = 0.62517 * C318;
        const double C325 = 0.88026 * C310;
        const double C326 = 1.6382 * C310;
        const double C327 = 2 * C310;
        const double C328 = 3.3662 * C310;
        const double C329 = 3.5876 * C317;
        const double C330 = 3.6231 * C317;
        const double C331 = 6.1977 * C317;
        const double C332 = C311 * C302;
        const double C333 = C313 * C301;
        const double C334 = C315 * C283;
        const double C335 = C316 * C279;
        const double C336 = C312 + C314;
        const double C337 = std::sqrt(C317);
        const double C338 = -7.5957 * C334;
        const double C339 = -3.5876 * C334;
        const double C340 = -0.2137 * C334;
        const double C341 = 0.49294 * C335;
        const double C342 = 2 * C337;
        const double C343 = 7.5957 * C337;
        const double C344 = 10.357 * C337;
        const double C345 = 14.1189 * C337;
        const double C346 = C308 * C327;
        const double C347 = C319 + 1.;
        const double C348 = C320 + 1.;
        const double C349 = C321 + 1.;
        const double C350 = C332 + C333;
        const double C351 = C336 - 2.;
        const double C352 = C308 * C342;
        const double C353 = C343 + C329;
        const double C354 = C344 + C330;
        const double C355 = C345 + C331;
        const double C356 = C280 / C346;
        const double C357 = C339 / C309;
        const double C358 = C341 / C309;
        const double C359 = 3. * C352;
        const double C360 = C353 + C326;
        const double C361 = C354 + C325;
        const double C362 = C355 + C328;
        const double C363 = C360 + C322;
        const double C364 = C361 + C323;
        const double C365 = C362 + C324;
        const double C366 = C338 / C359;
        const double C367 = 0.03109 * C365;
        const double C368 = 0.033774 * C364;
        const double C369 = 0.062182 * C363;
        const double C370 = C366 + C357;
        const double C371 = C370 + C356;
        const double C372 = 1. / C367;
        const double C373 = 1. / C368;
        const double C374 = 1. / C369;
        const double C375 = std::pow(C369, 2);
        const double C376 = C371 + C358;
        const double C377 = C372 + 1.;
        const double C378 = C373 + 1.;
        const double C379 = C374 + 1.;
        const double C380 = 0.062182 * C376;
        const double C381 = C375 * C379;
        const double C382 = std::log(C377);
        const double C383 = std::log(C378);
        const double C384 = std::log(C379);
        const double C385 = C347 * C383;
        const double C386 = C348 * C382;
        const double C387 = C349 * C380;
        const double C388 = C349 * C384;
        const double C389 = C384 * C340;
        const double C390 = 0.03109 * C386;
        const double C391 = 0.062182 * C388;
        const double C392 = C385 * C351;
        const double C393 = C387 / C381;
        const double C394 = C389 / C309;
        const double C395 = C391 - C390;
        const double C396 = C394 - C393;
        const double C397 = 0.062182 * C396;
        const double C398 = C395 * C351;

        result.vrhoB =
            C284 * ((0.033774 *
                         ((C385 * C350 / C295 +
                           (C383 * -0.11125 * C334 / C309 -
                            C347 * 0.033774 *
                                (((-10.357 * C334 / C359 + -3.6231 * C334 / C309) +
                                  -10.56312 * Pi / C346) +
                                 0.49671 * C335 / C309) /
                                (std::pow(C368, 2) * C378)) *
                               C351) *
                              C306 -
                          C392 * C303 / C293) /
                         C282 -
                     C397) +
                    (C300 * (C395 * C350 / C295 +
                             (C397 -
                              0.03109 * (C382 * -0.20548 * C334 / C309 -
                                         C348 * 0.03109 *
                                             (((-0.141189e2 * C334 / C359 + -6.1977 * C334 / C309) +
                                               -0.403944e2 * Pi / C346) +
                                              0.62517 * C335 / C309) /
                                             (std::pow(C367, 2) * C377))) *
                                 C351) +
                     C398 * C303 / C293) /
                        C281) +
            ((0.033774 * C392 * C306 / C282 - C391) + C300 * C398 / C281);
    }

    return result;
}

} // namespace excgrid
