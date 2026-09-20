// excgrid codegen source: the VWN5 LDA correlation functional (the VWN
// interpolation-V parametrization - the "full" VWN; most codes' bare
// "VWN").  Fresh authorship; formula from VWN 1980 Table V
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

XcKernelValue Vwn5Correlation(double rhoA, double rhoB) {
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
        const double C49 = 4. / 3.;
        const double C50 = std::sqrt(0.223816694236e2);
        const double C51 = std::sqrt(0.378469910464e2);
        const double C52 = std::sqrt(0.507386806551e2);
        const double C53 = std::pow(Pi, 2);
        const double C54 = 0.9999999999999999 * C47;
        const double C55 = 6. * C53;
        const double C56 = Pi * C46;
        const double C57 = 4. * C56;
        const double C58 = C54 / C46;
        const double C59 = 3. / C57;
        const double C60 = std::pow(C59, C48);
        const double C61 = std::sqrt(C60);
        const double C62 = 1.13107 * C61;
        const double C63 = 2. * C61;
        const double C64 = 3.72744 * C61;
        const double C65 = 7.06042 * C61;
        const double C66 = C61 + 0.0047584;
        const double C67 = C61 + 0.10498;
        const double C68 = C60 + C62;
        const double C69 = C60 + C64;
        const double C70 = C60 + C65;
        const double C71 = C63 + 1.13107;
        const double C72 = C63 + 3.72744;
        const double C73 = C63 + 7.06042;
        const double C74 = std::pow(C66, 2);
        const double C75 = std::pow(C67, 2);
        const double C76 = C68 + 13.0045;
        const double C77 = C69 + 12.9352;
        const double C78 = C70 + 18.0578;
        const double C79 = C50 / C73;
        const double C80 = C51 / C72;
        const double C81 = C52 / C71;
        const double C82 = C60 / C76;
        const double C83 = C60 / C77;
        const double C84 = C74 / C76;
        const double C85 = C75 / C77;
        const double C86 = std::atan(C79);
        const double C87 = std::atan(C80);
        const double C88 = std::atan(C81);
        const double C89 = 2.2431064 * C88;
        const double C90 = 2.26214 * C88;
        const double C91 = 7.03496 * C87;
        const double C92 = 7.45488 * C87;
        const double C93 = std::log(C82);
        const double C94 = std::log(C83);
        const double C95 = std::log(C84);
        const double C96 = std::log(C85);
        const double C97 = C89 / C52;
        const double C98 = C90 / C52;
        const double C99 = C91 / C51;
        const double C100 = C92 / C51;
        const double C101 = C93 + C98;
        const double C102 = C94 + C100;
        const double C103 = C95 + C97;
        const double C104 = C96 + C99;
        const double C105 = -0.3913066512 * C104;
        const double C106 = -0.005382083488 * C103;
        const double C107 = C105 / 12.5549141492;
        const double C108 = C106 / 12.99914055888256;
        const double C109 = C102 - C107;
        const double C110 = C108 - C101;
        const double C111 = 0.0310907 * C109;

        result.exc =
            C46 * (C111 + C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              ((1.7099209341613653 *
                                    (0.01554535 * ((std::log(C60 / C78) + 0.1412084e2 * C86 / C50) -
                                                   -2.2946365 *
                                                       (std::log(std::pow(C61 + 0.325, 2) / C78) +
                                                        0.1282084e2 * C86 / C50) /
                                                       15.8687885) -
                                     C111) *
                                    C55 / C110 -
                                1.) *
                                   std::pow(C47 / C46, 4) +
                               1.) /
                              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55));
    } else if (rhoA < eps)
    {
        const double C116 = -0.9999999999999999 * rhoB;
        const double C117 = Pi * rhoB;
        const double C118 = 1. / 3.;
        const double C119 = 4. / 3.;
        const double C120 = std::sqrt(0.223816694236e2);
        const double C121 = std::sqrt(0.378469910464e2);
        const double C122 = std::sqrt(0.507386806551e2);
        const double C123 = std::pow(Pi, 2);
        const double C124 = 4. * C117;
        const double C125 = 6. * C123;
        const double C126 = C116 / rhoB;
        const double C127 = 3. / C124;
        const double C128 = std::pow(C127, C118);
        const double C129 = std::sqrt(C128);
        const double C130 = 1.13107 * C129;
        const double C131 = 2. * C129;
        const double C132 = 3.72744 * C129;
        const double C133 = 7.06042 * C129;
        const double C134 = C129 + 0.0047584;
        const double C135 = C129 + 0.10498;
        const double C136 = C128 + C130;
        const double C137 = C128 + C132;
        const double C138 = C128 + C133;
        const double C139 = C131 + 1.13107;
        const double C140 = C131 + 3.72744;
        const double C141 = C131 + 7.06042;
        const double C142 = std::pow(C134, 2);
        const double C143 = std::pow(C135, 2);
        const double C144 = C136 + 13.0045;
        const double C145 = C137 + 12.9352;
        const double C146 = C138 + 18.0578;
        const double C147 = C120 / C141;
        const double C148 = C121 / C140;
        const double C149 = C122 / C139;
        const double C150 = C128 / C144;
        const double C151 = C128 / C145;
        const double C152 = C142 / C144;
        const double C153 = C143 / C145;
        const double C154 = std::atan(C147);
        const double C155 = std::atan(C148);
        const double C156 = std::atan(C149);
        const double C157 = 2.2431064 * C156;
        const double C158 = 2.26214 * C156;
        const double C159 = 7.03496 * C155;
        const double C160 = 7.45488 * C155;
        const double C161 = std::log(C150);
        const double C162 = std::log(C151);
        const double C163 = std::log(C152);
        const double C164 = std::log(C153);
        const double C165 = C157 / C122;
        const double C166 = C158 / C122;
        const double C167 = C159 / C121;
        const double C168 = C160 / C121;
        const double C169 = C161 + C166;
        const double C170 = C162 + C168;
        const double C171 = C163 + C165;
        const double C172 = C164 + C167;
        const double C173 = -0.3913066512 * C172;
        const double C174 = -0.005382083488 * C171;
        const double C175 = C173 / 12.5549141492;
        const double C176 = C174 / 12.99914055888256;
        const double C177 = C170 - C175;
        const double C178 = C176 - C169;
        const double C179 = 0.0310907 * C177;

        result.exc =
            rhoB *
            (C179 + C178 * ((std::pow(C126 + 1., C119) + std::pow(1. - C126, C119)) - 2.) *
                        ((1.7099209341613653 *
                              (0.01554535 * ((std::log(C128 / C146) + 0.1412084e2 * C154 / C120) -
                                             -2.2946365 *
                                                 (std::log(std::pow(C129 + 0.325, 2) / C146) +
                                                  0.1282084e2 * C154 / C120) /
                                                 15.8687885) -
                               C179) *
                              C125 / C178 -
                          1.) *
                             std::pow((-rhoB) / rhoB, 4) +
                         1.) /
                        (3.4198418683227306 * (std::pow(2., C118) - 1.) * C125));
    } else
    {
        const double C181 = 0.9999999999999999 * rhoA;
        const double C182 = Pi * rhoA;
        const double C183 = 1. / 3.;
        const double C184 = 4. / 3.;
        const double C185 = std::sqrt(0.223816694236e2);
        const double C186 = std::sqrt(0.378469910464e2);
        const double C187 = std::sqrt(0.507386806551e2);
        const double C188 = std::pow(Pi, 2);
        const double C189 = 4. * C182;
        const double C190 = 6. * C188;
        const double C191 = C181 / rhoA;
        const double C192 = 3. / C189;
        const double C193 = std::pow(C192, C183);
        const double C194 = std::sqrt(C193);
        const double C195 = 1.13107 * C194;
        const double C196 = 2. * C194;
        const double C197 = 3.72744 * C194;
        const double C198 = 7.06042 * C194;
        const double C199 = C194 + 0.0047584;
        const double C200 = C194 + 0.10498;
        const double C201 = C193 + C195;
        const double C202 = C193 + C197;
        const double C203 = C193 + C198;
        const double C204 = C196 + 1.13107;
        const double C205 = C196 + 3.72744;
        const double C206 = C196 + 7.06042;
        const double C207 = std::pow(C199, 2);
        const double C208 = std::pow(C200, 2);
        const double C209 = C201 + 13.0045;
        const double C210 = C202 + 12.9352;
        const double C211 = C203 + 18.0578;
        const double C212 = C185 / C206;
        const double C213 = C186 / C205;
        const double C214 = C187 / C204;
        const double C215 = C193 / C209;
        const double C216 = C193 / C210;
        const double C217 = C207 / C209;
        const double C218 = C208 / C210;
        const double C219 = std::atan(C212);
        const double C220 = std::atan(C213);
        const double C221 = std::atan(C214);
        const double C222 = 2.2431064 * C221;
        const double C223 = 2.26214 * C221;
        const double C224 = 7.03496 * C220;
        const double C225 = 7.45488 * C220;
        const double C226 = std::log(C215);
        const double C227 = std::log(C216);
        const double C228 = std::log(C217);
        const double C229 = std::log(C218);
        const double C230 = C222 / C187;
        const double C231 = C223 / C187;
        const double C232 = C224 / C186;
        const double C233 = C225 / C186;
        const double C234 = C226 + C231;
        const double C235 = C227 + C233;
        const double C236 = C228 + C230;
        const double C237 = C229 + C232;
        const double C238 = -0.3913066512 * C237;
        const double C239 = -0.005382083488 * C236;
        const double C240 = C238 / 12.5549141492;
        const double C241 = C239 / 12.99914055888256;
        const double C242 = C235 - C240;
        const double C243 = C241 - C234;
        const double C244 = 0.0310907 * C242;

        result.exc =
            rhoA *
            (C244 + C243 * ((std::pow(C191 + 1., C184) + std::pow(1. - C191, C184)) - 2.) *
                        1.7099209341613653 *
                        (0.01554535 * ((std::log(C193 / C211) + 0.1412084e2 * C219 / C185) -
                                       -2.2946365 *
                                           (std::log(std::pow(C194 + 0.325, 2) / C211) +
                                            0.1282084e2 * C219 / C185) /
                                           15.8687885) -
                         C244) *
                        C190 / (C243 * 3.4198418683227306 * (std::pow(2., C183) - 1.) * C190));
    }

    if (rhoA >= eps)
    {
        const double C183 = 1. / 3.;
        const double C184 = 4. / 3.;
        const double C185 = std::sqrt(0.223816694236e2);
        const double C186 = std::sqrt(0.378469910464e2);
        const double C187 = std::sqrt(0.507386806551e2);
        const double C188 = std::pow(Pi, 2);
        const double C190 = 6. * C188;
        const double C246 = 12. * Pi;
        const double C247 = rhoA + rhoB;
        const double C248 = rhoA - rhoB;
        const double C249 = -2. / 3.;
        const double C250 = std::pow(2., C183);
        const double C251 = 0.9999999999999999 * C247;
        const double C252 = 0.9999999999999999 * C248;
        const double C253 = Pi * C247;
        const double C254 = C250 - 1.;
        const double C255 = C248 / C247;
        const double C256 = std::pow(C247, 2);
        const double C257 = 4. * C253;
        const double C258 = C254 * C190;
        const double C259 = C251 - C252;
        const double C260 = C252 / C247;
        const double C261 = std::pow(C255, 4);
        const double C262 = 3.4198418683227306 * C258;
        const double C263 = C260 + 1.;
        const double C264 = 1. - C260;
        const double C265 = 3. / C257;
        const double C266 = std::pow(C257, 2);
        const double C267 = 3. * C266;
        const double C268 = std::pow(C263, C184);
        const double C269 = std::pow(C264, C184);
        const double C270 = std::pow(C265, C183);
        const double C271 = std::pow(C265, C249);
        const double C272 = C271 * C246;
        const double C273 = C268 + C269;
        const double C274 = std::sqrt(C270);
        const double C275 = -7.06042 * C272;
        const double C276 = -3.72744 * C272;
        const double C277 = -2. * C272;
        const double C278 = -1.13107 * C272;
        const double C279 = 1.13107 * C274;
        const double C280 = 2 * C274;
        const double C281 = 3.72744 * C274;
        const double C282 = 7.06042 * C274;
        const double C283 = C274 + 0.0047584;
        const double C284 = C274 + 0.10498;
        const double C285 = C274 + 0.325;
        const double C286 = C273 - 2.;
        const double C287 = C272 / C267;
        const double C288 = C185 * C277;
        const double C289 = C186 * C277;
        const double C290 = C187 * C277;
        const double C291 = C266 * C280;
        const double C292 = C283 * C277;
        const double C293 = C284 * C277;
        const double C294 = C270 + C279;
        const double C295 = C270 + C281;
        const double C296 = C270 + C282;
        const double C297 = C280 + 1.13107;
        const double C298 = C280 + 3.72744;
        const double C299 = C280 + 7.06042;
        const double C300 = std::pow(C283, 2);
        const double C301 = std::pow(C284, 2);
        const double C302 = std::pow(C285, 2);
        const double C303 = -7.45488 * C289;
        const double C304 = -7.03496 * C289;
        const double C305 = -2.26214 * C290;
        const double C306 = -2.2431064 * C290;
        const double C307 = 3. * C291;
        const double C308 = C294 + 13.0045;
        const double C309 = C295 + 12.9352;
        const double C310 = C296 + 18.0578;
        const double C311 = C185 / C299;
        const double C312 = C186 / C298;
        const double C313 = C187 / C297;
        const double C314 = std::pow(C297, 2);
        const double C315 = std::pow(C298, 2);
        const double C316 = std::pow(C299, 2);
        const double C317 = C308 * C272;
        const double C318 = C308 * C292;
        const double C319 = C309 * C272;
        const double C320 = C309 * C293;
        const double C321 = C270 / C308;
        const double C322 = C270 / C309;
        const double C323 = C270 / C310;
        const double C324 = C275 / C307;
        const double C325 = C276 / C307;
        const double C326 = C278 / C307;
        const double C327 = C300 / C308;
        const double C328 = C301 / C309;
        const double C329 = C302 / C310;
        const double C330 = std::atan(C311);
        const double C331 = std::atan(C312);
        const double C332 = std::atan(C313);
        const double C333 = std::pow(C308, 2);
        const double C334 = std::pow(C309, 2);
        const double C335 = std::pow(C310, 2);
        const double C336 = std::pow(C311, 2);
        const double C337 = std::pow(C312, 2);
        const double C338 = std::pow(C313, 2);
        const double C339 = 2.2431064 * C332;
        const double C340 = 2.26214 * C332;
        const double C341 = 7.03496 * C331;
        const double C342 = 7.45488 * C331;
        const double C343 = 0.1282084e2 * C330;
        const double C344 = 0.1412084e2 * C330;
        const double C345 = C333 * C270;
        const double C346 = C333 * C300;
        const double C347 = C334 * C270;
        const double C348 = C334 * C301;
        const double C349 = C336 + 1;
        const double C350 = C337 + 1;
        const double C351 = C338 + 1;
        const double C352 = -C317;
        const double C353 = -C319;
        const double C354 = C324 - C287;
        const double C355 = C325 - C287;
        const double C356 = C326 - C287;
        const double C357 = C318 / C307;
        const double C358 = C320 / C307;
        const double C359 = std::log(C321);
        const double C360 = std::log(C322);
        const double C361 = std::log(C323);
        const double C362 = std::log(C327);
        const double C363 = std::log(C328);
        const double C364 = std::log(C329);
        const double C365 = C270 * C355;
        const double C366 = C270 * C356;
        const double C367 = C300 * C356;
        const double C368 = C301 * C355;
        const double C369 = C314 * C351;
        const double C370 = C315 * C350;
        const double C371 = C316 * C349;
        const double C372 = C339 / C187;
        const double C373 = C340 / C187;
        const double C374 = C341 / C186;
        const double C375 = C342 / C186;
        const double C376 = C343 / C185;
        const double C377 = C344 / C185;
        const double C378 = C352 / C267;
        const double C379 = C353 / C267;
        const double C380 = C291 * C369;
        const double C381 = C291 * C370;
        const double C382 = C291 * C371;
        const double C383 = C359 + C373;
        const double C384 = C360 + C375;
        const double C385 = C361 + C377;
        const double C386 = C362 + C372;
        const double C387 = C363 + C374;
        const double C388 = C364 + C376;
        const double C389 = C357 - C367;
        const double C390 = C358 - C368;
        const double C391 = C378 - C366;
        const double C392 = C379 - C365;
        const double C393 = -2.2946365 * C388;
        const double C394 = -0.3913066512 * C387;
        const double C395 = -0.005382083488 * C386;
        const double C396 = C380 * C187;
        const double C397 = C381 * C186;
        const double C398 = C382 * C185;
        const double C399 = C389 * C308;
        const double C400 = C390 * C309;
        const double C401 = C391 * C308;
        const double C402 = C392 * C309;
        const double C403 = 3. * C396;
        const double C404 = 3. * C397;
        const double C405 = 3. * C398;
        const double C406 = C393 / 15.8687885;
        const double C407 = C394 / 12.5549141492;
        const double C408 = C395 / 12.99914055888256;
        const double C409 = C399 / C346;
        const double C410 = C400 / C348;
        const double C411 = C401 / C345;
        const double C412 = C402 / C347;
        const double C413 = C384 - C407;
        const double C414 = C385 - C406;
        const double C415 = C408 - C383;
        const double C416 = C303 / C404;
        const double C417 = C304 / C404;
        const double C418 = C305 / C403;
        const double C419 = C306 / C403;
        const double C420 = 0.01554535 * C414;
        const double C421 = 0.0310907 * C413;
        const double C422 = C415 * C286;
        const double C423 = C409 + C419;
        const double C424 = C410 + C417;
        const double C425 = C411 + C418;
        const double C426 = C412 + C416;
        const double C427 = -0.3913066512 * C424;
        const double C428 = -0.005382083488 * C423;
        const double C429 = C420 - C421;
        const double C430 = C429 * C190;
        const double C431 = C427 / 12.5549141492;
        const double C432 = C428 / 12.99914055888256;
        const double C433 = 1.7099209341613653 * C430;
        const double C434 = C426 - C431;
        const double C435 = C432 - C425;
        const double C436 = 0.0310907 * C434;
        const double C437 = C433 / C415;
        const double C438 = C437 - 1.;
        const double C439 = C438 * C261;
        const double C440 = C439 + 1.;

        result.vrhoA =
            C247 *
                (C436 +
                 (C422 * (C438 * std::pow(C255, 3) * 4 * (C247 - C248) / C256 +
                          C261 *
                              (C415 * 1.7099209341613653 *
                                   (0.01554535 * ((((-C310 * C272) / C267 - C270 * C354) * C310 /
                                                       (C335 * C270) +
                                                   -0.1412084e2 * C288 / C405) -
                                                  -2.2946365 *
                                                      ((C310 * C285 * C277 / C307 - C302 * C354) *
                                                           C310 / (C335 * C302) +
                                                       -0.1282084e2 * C288 / C405) /
                                                      15.8687885) -
                                    C436) *
                                   C190 -
                               1.7099209341613653 * C430 * C435) /
                              std::pow(C415, 2)) +
                  (C415 * (std::pow(C263, C183) * 4. * C259 + std::pow(C264, C183) * -4. * C259) /
                       (3. * C256) +
                   C435 * C286) *
                      C440) /
                     C262) +
            (C421 + C422 * C440 / C262);
    }

    if (rhoB >= eps)
    {
        const double C118 = 1. / 3.;
        const double C119 = 4. / 3.;
        const double C120 = std::sqrt(0.223816694236e2);
        const double C121 = std::sqrt(0.378469910464e2);
        const double C122 = std::sqrt(0.507386806551e2);
        const double C123 = std::pow(Pi, 2);
        const double C125 = 6. * C123;
        const double C442 = 12. * Pi;
        const double C443 = rhoA + rhoB;
        const double C444 = rhoA - rhoB;
        const double C445 = -2. / 3.;
        const double C446 = std::pow(2., C118);
        const double C447 = -0.9999999999999999 * C443;
        const double C448 = 0.9999999999999999 * C444;
        const double C449 = Pi * C443;
        const double C450 = C446 - 1.;
        const double C451 = C444 / C443;
        const double C452 = std::pow(C443, 2);
        const double C453 = 4. * C449;
        const double C454 = C450 * C125;
        const double C455 = C447 - C448;
        const double C456 = C448 / C443;
        const double C457 = std::pow(C451, 4);
        const double C458 = 3.4198418683227306 * C454;
        const double C459 = C456 + 1.;
        const double C460 = 1. - C456;
        const double C461 = 3. / C453;
        const double C462 = std::pow(C453, 2);
        const double C463 = 3. * C462;
        const double C464 = std::pow(C459, C119);
        const double C465 = std::pow(C460, C119);
        const double C466 = std::pow(C461, C118);
        const double C467 = std::pow(C461, C445);
        const double C468 = C467 * C442;
        const double C469 = C464 + C465;
        const double C470 = std::sqrt(C466);
        const double C471 = -7.06042 * C468;
        const double C472 = -3.72744 * C468;
        const double C473 = -2. * C468;
        const double C474 = -1.13107 * C468;
        const double C475 = 1.13107 * C470;
        const double C476 = 2 * C470;
        const double C477 = 3.72744 * C470;
        const double C478 = 7.06042 * C470;
        const double C479 = C470 + 0.0047584;
        const double C480 = C470 + 0.10498;
        const double C481 = C470 + 0.325;
        const double C482 = C469 - 2.;
        const double C483 = C468 / C463;
        const double C484 = C120 * C473;
        const double C485 = C121 * C473;
        const double C486 = C122 * C473;
        const double C487 = C462 * C476;
        const double C488 = C479 * C473;
        const double C489 = C480 * C473;
        const double C490 = C466 + C475;
        const double C491 = C466 + C477;
        const double C492 = C466 + C478;
        const double C493 = C476 + 1.13107;
        const double C494 = C476 + 3.72744;
        const double C495 = C476 + 7.06042;
        const double C496 = std::pow(C479, 2);
        const double C497 = std::pow(C480, 2);
        const double C498 = std::pow(C481, 2);
        const double C499 = -7.45488 * C485;
        const double C500 = -7.03496 * C485;
        const double C501 = -2.26214 * C486;
        const double C502 = -2.2431064 * C486;
        const double C503 = 3. * C487;
        const double C504 = C490 + 13.0045;
        const double C505 = C491 + 12.9352;
        const double C506 = C492 + 18.0578;
        const double C507 = C120 / C495;
        const double C508 = C121 / C494;
        const double C509 = C122 / C493;
        const double C510 = std::pow(C493, 2);
        const double C511 = std::pow(C494, 2);
        const double C512 = std::pow(C495, 2);
        const double C513 = C504 * C468;
        const double C514 = C504 * C488;
        const double C515 = C505 * C468;
        const double C516 = C505 * C489;
        const double C517 = C466 / C504;
        const double C518 = C466 / C505;
        const double C519 = C466 / C506;
        const double C520 = C471 / C503;
        const double C521 = C472 / C503;
        const double C522 = C474 / C503;
        const double C523 = C496 / C504;
        const double C524 = C497 / C505;
        const double C525 = C498 / C506;
        const double C526 = std::atan(C507);
        const double C527 = std::atan(C508);
        const double C528 = std::atan(C509);
        const double C529 = std::pow(C504, 2);
        const double C530 = std::pow(C505, 2);
        const double C531 = std::pow(C506, 2);
        const double C532 = std::pow(C507, 2);
        const double C533 = std::pow(C508, 2);
        const double C534 = std::pow(C509, 2);
        const double C535 = 2.2431064 * C528;
        const double C536 = 2.26214 * C528;
        const double C537 = 7.03496 * C527;
        const double C538 = 7.45488 * C527;
        const double C539 = 0.1282084e2 * C526;
        const double C540 = 0.1412084e2 * C526;
        const double C541 = C529 * C466;
        const double C542 = C529 * C496;
        const double C543 = C530 * C466;
        const double C544 = C530 * C497;
        const double C545 = C532 + 1;
        const double C546 = C533 + 1;
        const double C547 = C534 + 1;
        const double C548 = -C513;
        const double C549 = -C515;
        const double C550 = C520 - C483;
        const double C551 = C521 - C483;
        const double C552 = C522 - C483;
        const double C553 = C514 / C503;
        const double C554 = C516 / C503;
        const double C555 = std::log(C517);
        const double C556 = std::log(C518);
        const double C557 = std::log(C519);
        const double C558 = std::log(C523);
        const double C559 = std::log(C524);
        const double C560 = std::log(C525);
        const double C561 = C466 * C551;
        const double C562 = C466 * C552;
        const double C563 = C496 * C552;
        const double C564 = C497 * C551;
        const double C565 = C510 * C547;
        const double C566 = C511 * C546;
        const double C567 = C512 * C545;
        const double C568 = C535 / C122;
        const double C569 = C536 / C122;
        const double C570 = C537 / C121;
        const double C571 = C538 / C121;
        const double C572 = C539 / C120;
        const double C573 = C540 / C120;
        const double C574 = C548 / C463;
        const double C575 = C549 / C463;
        const double C576 = C487 * C565;
        const double C577 = C487 * C566;
        const double C578 = C487 * C567;
        const double C579 = C555 + C569;
        const double C580 = C556 + C571;
        const double C581 = C557 + C573;
        const double C582 = C558 + C568;
        const double C583 = C559 + C570;
        const double C584 = C560 + C572;
        const double C585 = C553 - C563;
        const double C586 = C554 - C564;
        const double C587 = C574 - C562;
        const double C588 = C575 - C561;
        const double C589 = -2.2946365 * C584;
        const double C590 = -0.3913066512 * C583;
        const double C591 = -0.005382083488 * C582;
        const double C592 = C576 * C122;
        const double C593 = C577 * C121;
        const double C594 = C578 * C120;
        const double C595 = C585 * C504;
        const double C596 = C586 * C505;
        const double C597 = C587 * C504;
        const double C598 = C588 * C505;
        const double C599 = 3. * C592;
        const double C600 = 3. * C593;
        const double C601 = 3. * C594;
        const double C602 = C589 / 15.8687885;
        const double C603 = C590 / 12.5549141492;
        const double C604 = C591 / 12.99914055888256;
        const double C605 = C595 / C542;
        const double C606 = C596 / C544;
        const double C607 = C597 / C541;
        const double C608 = C598 / C543;
        const double C609 = C580 - C603;
        const double C610 = C581 - C602;
        const double C611 = C604 - C579;
        const double C612 = C499 / C600;
        const double C613 = C500 / C600;
        const double C614 = C501 / C599;
        const double C615 = C502 / C599;
        const double C616 = 0.01554535 * C610;
        const double C617 = 0.0310907 * C609;
        const double C618 = C611 * C482;
        const double C619 = C605 + C615;
        const double C620 = C606 + C613;
        const double C621 = C607 + C614;
        const double C622 = C608 + C612;
        const double C623 = -0.3913066512 * C620;
        const double C624 = -0.005382083488 * C619;
        const double C625 = C616 - C617;
        const double C626 = C625 * C125;
        const double C627 = C623 / 12.5549141492;
        const double C628 = C624 / 12.99914055888256;
        const double C629 = 1.7099209341613653 * C626;
        const double C630 = C622 - C627;
        const double C631 = C628 - C621;
        const double C632 = 0.0310907 * C630;
        const double C633 = C629 / C611;
        const double C634 = C633 - 1.;
        const double C635 = C634 * C457;
        const double C636 = C635 + 1.;

        result.vrhoB =
            C443 *
                (C632 +
                 (C618 * (C634 * std::pow(C451, 3) * -4 * (C443 + C444) / C452 +
                          C457 *
                              (C611 * 1.7099209341613653 *
                                   (0.01554535 * ((((-C506 * C468) / C463 - C466 * C550) * C506 /
                                                       (C531 * C466) +
                                                   -0.1412084e2 * C484 / C601) -
                                                  -2.2946365 *
                                                      ((C506 * C481 * C473 / C503 - C498 * C550) *
                                                           C506 / (C531 * C498) +
                                                       -0.1282084e2 * C484 / C601) /
                                                      15.8687885) -
                                    C632) *
                                   C125 -
                               1.7099209341613653 * C626 * C631) /
                              std::pow(C611, 2)) +
                  (C611 * (std::pow(C459, C118) * 4. * C455 + std::pow(C460, C118) * -4. * C455) /
                       (3. * C452) +
                   C631 * C482) *
                      C636) /
                     C458) +
            (C617 + C618 * C636 / C458);
    }

    return result;
}

} // namespace excgrid
