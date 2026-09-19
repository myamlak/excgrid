// excgrid codegen source: the VWN3 LDA correlation functional (the VWN
// RPA-parameter interpolation).  Distinct from VWN5 - the two are
// separately named, never conflated (docs/kernel-api.md section 4).
// Fresh authorship; formula from VWN 1980 Table I.  Regenerate with
// tools/regenerate.py.

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

XcKernelValue Vwn3Correlation(double rhoA, double rhoB) {
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
        const double C50 = std::sqrt(0.00002016e2);
        const double C51 = std::sqrt(0.0137284639e2);
        const double C52 = 0.9999999999999999 * C47;
        const double C53 = Pi * C46;
        const double C54 = 4. * C53;
        const double C55 = C52 / C46;
        const double C56 = 3. / C54;
        const double C57 = std::pow(C56, C48);
        const double C58 = std::sqrt(C57);
        const double C59 = 2. * C58;
        const double C60 = 13.072 * C58;
        const double C61 = 20.1231 * C58;
        const double C62 = C58 + 0.409286;
        const double C63 = C57 + C60;
        const double C64 = C57 + C61;
        const double C65 = C59 + 13.072;
        const double C66 = C59 + 20.1231;
        const double C67 = std::pow(C62, 2);
        const double C68 = C63 + 42.7198;
        const double C69 = C64 + 101.578;
        const double C70 = C50 / C65;
        const double C71 = C51 / C66;
        const double C72 = C57 / C68;
        const double C73 = C67 / C68;
        const double C74 = std::atan(C70);
        const double C75 = std::atan(C71);
        const double C76 = 0.24506856e2 * C74;
        const double C77 = 0.26144e2 * C74;
        const double C78 = std::log(C72);
        const double C79 = std::log(C73);
        const double C80 = C76 / C50;
        const double C81 = C77 / C50;
        const double C82 = C78 + C81;
        const double C83 = C79 + C80;
        const double C84 = -0.5350186592e1 * C83;
        const double C85 = C84 / 37.537128437796;
        const double C86 = C82 - C85;
        const double C87 = 0.0310907 * C86;

        result.exc = C46 * (C87 + (0.01554535 * ((std::log(C57 / C69) + 0.402462e2 * C75 / C51) -
                                                 -0.149573794914e2 *
                                                     (std::log(std::pow(C58 + 0.743294, 2) / C69) +
                                                      0.37273024e2 * C75 / C51) /
                                                     0.87173106479036e2) -
                                   C87) *
                                      ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                                      (2. * (std::pow(2., C48) - 1.)));
    } else if (rhoA < eps)
    {
        const double C92 = -0.9999999999999999 * rhoB;
        const double C93 = Pi * rhoB;
        const double C94 = 1. / 3.;
        const double C95 = 4. / 3.;
        const double C96 = std::sqrt(0.00002016e2);
        const double C97 = std::sqrt(0.0137284639e2);
        const double C98 = 4. * C93;
        const double C99 = C92 / rhoB;
        const double C100 = 3. / C98;
        const double C101 = std::pow(C100, C94);
        const double C102 = std::sqrt(C101);
        const double C103 = 2. * C102;
        const double C104 = 13.072 * C102;
        const double C105 = 20.1231 * C102;
        const double C106 = C102 + 0.409286;
        const double C107 = C101 + C104;
        const double C108 = C101 + C105;
        const double C109 = C103 + 13.072;
        const double C110 = C103 + 20.1231;
        const double C111 = std::pow(C106, 2);
        const double C112 = C107 + 42.7198;
        const double C113 = C108 + 101.578;
        const double C114 = C96 / C109;
        const double C115 = C97 / C110;
        const double C116 = C101 / C112;
        const double C117 = C111 / C112;
        const double C118 = std::atan(C114);
        const double C119 = std::atan(C115);
        const double C120 = 0.24506856e2 * C118;
        const double C121 = 0.26144e2 * C118;
        const double C122 = std::log(C116);
        const double C123 = std::log(C117);
        const double C124 = C120 / C96;
        const double C125 = C121 / C96;
        const double C126 = C122 + C125;
        const double C127 = C123 + C124;
        const double C128 = -0.5350186592e1 * C127;
        const double C129 = C128 / 37.537128437796;
        const double C130 = C126 - C129;
        const double C131 = 0.0310907 * C130;

        result.exc =
            rhoB * (C131 + (0.01554535 * ((std::log(C101 / C113) + 0.402462e2 * C119 / C97) -
                                          -0.149573794914e2 *
                                              (std::log(std::pow(C102 + 0.743294, 2) / C113) +
                                               0.37273024e2 * C119 / C97) /
                                              0.87173106479036e2) -
                            C131) *
                               ((std::pow(C99 + 1., C95) + std::pow(1. - C99, C95)) - 2.) /
                               (2. * (std::pow(2., C94) - 1.)));
    } else
    {
        const double C133 = 0.9999999999999999 * rhoA;
        const double C134 = Pi * rhoA;
        const double C135 = 1. / 3.;
        const double C136 = 4. / 3.;
        const double C137 = std::sqrt(0.00002016e2);
        const double C138 = std::sqrt(0.0137284639e2);
        const double C139 = 4. * C134;
        const double C140 = C133 / rhoA;
        const double C141 = 3. / C139;
        const double C142 = std::pow(C141, C135);
        const double C143 = std::sqrt(C142);
        const double C144 = 2. * C143;
        const double C145 = 13.072 * C143;
        const double C146 = 20.1231 * C143;
        const double C147 = C143 + 0.409286;
        const double C148 = C142 + C145;
        const double C149 = C142 + C146;
        const double C150 = C144 + 13.072;
        const double C151 = C144 + 20.1231;
        const double C152 = std::pow(C147, 2);
        const double C153 = C148 + 42.7198;
        const double C154 = C149 + 101.578;
        const double C155 = C137 / C150;
        const double C156 = C138 / C151;
        const double C157 = C142 / C153;
        const double C158 = C152 / C153;
        const double C159 = std::atan(C155);
        const double C160 = std::atan(C156);
        const double C161 = 0.24506856e2 * C159;
        const double C162 = 0.26144e2 * C159;
        const double C163 = std::log(C157);
        const double C164 = std::log(C158);
        const double C165 = C161 / C137;
        const double C166 = C162 / C137;
        const double C167 = C163 + C166;
        const double C168 = C164 + C165;
        const double C169 = -0.5350186592e1 * C168;
        const double C170 = C169 / 37.537128437796;
        const double C171 = C167 - C170;
        const double C172 = 0.0310907 * C171;

        result.exc =
            rhoA * (C172 + (0.01554535 * ((std::log(C142 / C154) + 0.402462e2 * C160 / C138) -
                                          -0.149573794914e2 *
                                              (std::log(std::pow(C143 + 0.743294, 2) / C154) +
                                               0.37273024e2 * C160 / C138) /
                                              0.87173106479036e2) -
                            C172) *
                               ((std::pow(C140 + 1., C136) + std::pow(1. - C140, C136)) - 2.) /
                               (2. * (std::pow(2., C135) - 1.)));
    }

    if (rhoA >= eps)
    {
        const double C135 = 1. / 3.;
        const double C136 = 4. / 3.;
        const double C137 = std::sqrt(0.00002016e2);
        const double C138 = std::sqrt(0.0137284639e2);
        const double C174 = 12. * Pi;
        const double C175 = rhoA + rhoB;
        const double C176 = rhoA - rhoB;
        const double C177 = -2. / 3.;
        const double C178 = std::pow(2., C135);
        const double C179 = 0.9999999999999999 * C175;
        const double C180 = 0.9999999999999999 * C176;
        const double C181 = Pi * C175;
        const double C182 = C178 - 1.;
        const double C183 = 2. * C182;
        const double C184 = 4. * C181;
        const double C185 = C179 - C180;
        const double C186 = C180 / C175;
        const double C187 = C186 + 1.;
        const double C188 = 1. - C186;
        const double C189 = 3. / C184;
        const double C190 = std::pow(C184, 2);
        const double C191 = 3. * C190;
        const double C192 = std::pow(C187, C136);
        const double C193 = std::pow(C188, C136);
        const double C194 = std::pow(C189, C135);
        const double C195 = std::pow(C189, C177);
        const double C196 = C195 * C174;
        const double C197 = C192 + C193;
        const double C198 = std::sqrt(C194);
        const double C199 = -0.201231e2 * C196;
        const double C200 = -0.13072e2 * C196;
        const double C201 = -2. * C196;
        const double C202 = 2 * C198;
        const double C203 = 13.072 * C198;
        const double C204 = 20.1231 * C198;
        const double C205 = C198 + 0.409286;
        const double C206 = C198 + 0.743294;
        const double C207 = C197 - 2.;
        const double C208 = C196 / C191;
        const double C209 = C137 * C201;
        const double C210 = C138 * C201;
        const double C211 = C190 * C202;
        const double C212 = C205 * C201;
        const double C213 = C194 + C203;
        const double C214 = C194 + C204;
        const double C215 = C202 + 13.072;
        const double C216 = C202 + 20.1231;
        const double C217 = std::pow(C205, 2);
        const double C218 = std::pow(C206, 2);
        const double C219 = -0.26144e2 * C209;
        const double C220 = -0.24506856e2 * C209;
        const double C221 = 3. * C211;
        const double C222 = C213 + 42.7198;
        const double C223 = C214 + 101.578;
        const double C224 = C137 / C215;
        const double C225 = C138 / C216;
        const double C226 = std::pow(C215, 2);
        const double C227 = std::pow(C216, 2);
        const double C228 = C222 * C196;
        const double C229 = C222 * C212;
        const double C230 = C194 / C222;
        const double C231 = C194 / C223;
        const double C232 = C199 / C221;
        const double C233 = C200 / C221;
        const double C234 = C217 / C222;
        const double C235 = C218 / C223;
        const double C236 = std::atan(C224);
        const double C237 = std::atan(C225);
        const double C238 = std::pow(C222, 2);
        const double C239 = std::pow(C223, 2);
        const double C240 = std::pow(C224, 2);
        const double C241 = std::pow(C225, 2);
        const double C242 = 0.24506856e2 * C236;
        const double C243 = 0.26144e2 * C236;
        const double C244 = 0.37273024e2 * C237;
        const double C245 = 0.402462e2 * C237;
        const double C246 = C238 * C194;
        const double C247 = C238 * C217;
        const double C248 = C240 + 1;
        const double C249 = C241 + 1;
        const double C250 = -C228;
        const double C251 = C232 - C208;
        const double C252 = C233 - C208;
        const double C253 = C229 / C221;
        const double C254 = std::log(C230);
        const double C255 = std::log(C231);
        const double C256 = std::log(C234);
        const double C257 = std::log(C235);
        const double C258 = C194 * C252;
        const double C259 = C217 * C252;
        const double C260 = C226 * C248;
        const double C261 = C227 * C249;
        const double C262 = C242 / C137;
        const double C263 = C243 / C137;
        const double C264 = C244 / C138;
        const double C265 = C245 / C138;
        const double C266 = C250 / C191;
        const double C267 = C211 * C260;
        const double C268 = C211 * C261;
        const double C269 = C254 + C263;
        const double C270 = C255 + C265;
        const double C271 = C256 + C262;
        const double C272 = C257 + C264;
        const double C273 = C253 - C259;
        const double C274 = C266 - C258;
        const double C275 = -0.149573794914e2 * C272;
        const double C276 = -0.5350186592e1 * C271;
        const double C277 = C267 * C137;
        const double C278 = C268 * C138;
        const double C279 = C273 * C222;
        const double C280 = C274 * C222;
        const double C281 = 3. * C277;
        const double C282 = 3. * C278;
        const double C283 = C275 / 0.87173106479036e2;
        const double C284 = C276 / 37.537128437796;
        const double C285 = C279 / C247;
        const double C286 = C280 / C246;
        const double C287 = C269 - C284;
        const double C288 = C270 - C283;
        const double C289 = C219 / C281;
        const double C290 = C220 / C281;
        const double C291 = 0.01554535 * C288;
        const double C292 = 0.0310907 * C287;
        const double C293 = C285 + C290;
        const double C294 = C286 + C289;
        const double C295 = -0.5350186592e1 * C293;
        const double C296 = C291 - C292;
        const double C297 = C295 / 37.537128437796;
        const double C298 = C294 - C297;
        const double C299 = 0.0310907 * C298;

        result.vrhoA =
            C175 * (C299 +
                    (C296 * (std::pow(C187, C135) * 4. * C185 + std::pow(C188, C135) * -4. * C185) /
                         (3. * std::pow(C175, 2)) +
                     (0.01554535 *
                          ((((-C223 * C196) / C191 - C194 * C251) * C223 / (C239 * C194) +
                            -0.402462e2 * C210 / C282) -
                           -0.149573794914e2 *
                               ((C223 * C206 * C201 / C221 - C218 * C251) * C223 / (C239 * C218) +
                                -0.37273024e2 * C210 / C282) /
                               0.87173106479036e2) -
                      C299) *
                         C207) /
                        C183) +
            (C292 + C296 * C207 / C183);
    }

    if (rhoB >= eps)
    {
        const double C94 = 1. / 3.;
        const double C95 = 4. / 3.;
        const double C96 = std::sqrt(0.00002016e2);
        const double C97 = std::sqrt(0.0137284639e2);
        const double C301 = 12. * Pi;
        const double C302 = rhoA + rhoB;
        const double C303 = rhoA - rhoB;
        const double C304 = -2. / 3.;
        const double C305 = std::pow(2., C94);
        const double C306 = -0.9999999999999999 * C302;
        const double C307 = 0.9999999999999999 * C303;
        const double C308 = Pi * C302;
        const double C309 = C305 - 1.;
        const double C310 = 2. * C309;
        const double C311 = 4. * C308;
        const double C312 = C306 - C307;
        const double C313 = C307 / C302;
        const double C314 = C313 + 1.;
        const double C315 = 1. - C313;
        const double C316 = 3. / C311;
        const double C317 = std::pow(C311, 2);
        const double C318 = 3. * C317;
        const double C319 = std::pow(C314, C95);
        const double C320 = std::pow(C315, C95);
        const double C321 = std::pow(C316, C304);
        const double C322 = std::pow(C316, C94);
        const double C323 = C321 * C301;
        const double C324 = C319 + C320;
        const double C325 = std::sqrt(C322);
        const double C326 = -0.201231e2 * C323;
        const double C327 = -0.13072e2 * C323;
        const double C328 = -2. * C323;
        const double C329 = 2 * C325;
        const double C330 = 13.072 * C325;
        const double C331 = 20.1231 * C325;
        const double C332 = C325 + 0.409286;
        const double C333 = C325 + 0.743294;
        const double C334 = C324 - 2.;
        const double C335 = C323 / C318;
        const double C336 = C317 * C329;
        const double C337 = C332 * C328;
        const double C338 = C96 * C328;
        const double C339 = C97 * C328;
        const double C340 = C322 + C330;
        const double C341 = C322 + C331;
        const double C342 = C329 + 13.072;
        const double C343 = C329 + 20.1231;
        const double C344 = std::pow(C332, 2);
        const double C345 = std::pow(C333, 2);
        const double C346 = -0.26144e2 * C338;
        const double C347 = -0.24506856e2 * C338;
        const double C348 = 3. * C336;
        const double C349 = C340 + 42.7198;
        const double C350 = C341 + 101.578;
        const double C351 = C96 / C342;
        const double C352 = C97 / C343;
        const double C353 = std::pow(C342, 2);
        const double C354 = std::pow(C343, 2);
        const double C355 = C349 * C323;
        const double C356 = C349 * C337;
        const double C357 = C322 / C349;
        const double C358 = C322 / C350;
        const double C359 = C326 / C348;
        const double C360 = C327 / C348;
        const double C361 = C344 / C349;
        const double C362 = C345 / C350;
        const double C363 = std::atan(C351);
        const double C364 = std::atan(C352);
        const double C365 = std::pow(C349, 2);
        const double C366 = std::pow(C350, 2);
        const double C367 = std::pow(C351, 2);
        const double C368 = std::pow(C352, 2);
        const double C369 = 0.24506856e2 * C363;
        const double C370 = 0.26144e2 * C363;
        const double C371 = 0.37273024e2 * C364;
        const double C372 = 0.402462e2 * C364;
        const double C373 = C365 * C322;
        const double C374 = C365 * C344;
        const double C375 = C367 + 1;
        const double C376 = C368 + 1;
        const double C377 = -C355;
        const double C378 = C359 - C335;
        const double C379 = C360 - C335;
        const double C380 = C356 / C348;
        const double C381 = std::log(C357);
        const double C382 = std::log(C358);
        const double C383 = std::log(C361);
        const double C384 = std::log(C362);
        const double C385 = C322 * C379;
        const double C386 = C344 * C379;
        const double C387 = C353 * C375;
        const double C388 = C354 * C376;
        const double C389 = C369 / C96;
        const double C390 = C370 / C96;
        const double C391 = C371 / C97;
        const double C392 = C372 / C97;
        const double C393 = C377 / C318;
        const double C394 = C336 * C387;
        const double C395 = C336 * C388;
        const double C396 = C381 + C390;
        const double C397 = C382 + C392;
        const double C398 = C383 + C389;
        const double C399 = C384 + C391;
        const double C400 = C380 - C386;
        const double C401 = C393 - C385;
        const double C402 = -0.149573794914e2 * C399;
        const double C403 = -0.5350186592e1 * C398;
        const double C404 = C394 * C96;
        const double C405 = C395 * C97;
        const double C406 = C400 * C349;
        const double C407 = C401 * C349;
        const double C408 = 3. * C404;
        const double C409 = 3. * C405;
        const double C410 = C402 / 0.87173106479036e2;
        const double C411 = C403 / 37.537128437796;
        const double C412 = C406 / C374;
        const double C413 = C407 / C373;
        const double C414 = C396 - C411;
        const double C415 = C397 - C410;
        const double C416 = C346 / C408;
        const double C417 = C347 / C408;
        const double C418 = 0.01554535 * C415;
        const double C419 = 0.0310907 * C414;
        const double C420 = C412 + C417;
        const double C421 = C413 + C416;
        const double C422 = -0.5350186592e1 * C420;
        const double C423 = C418 - C419;
        const double C424 = C422 / 37.537128437796;
        const double C425 = C421 - C424;
        const double C426 = 0.0310907 * C425;

        result.vrhoB =
            C302 * (C426 +
                    (C423 * (std::pow(C314, C94) * 4. * C312 + std::pow(C315, C94) * -4. * C312) /
                         (3. * std::pow(C302, 2)) +
                     (0.01554535 *
                          ((((-C350 * C323) / C318 - C322 * C378) * C350 / (C366 * C322) +
                            -0.402462e2 * C339 / C409) -
                           -0.149573794914e2 *
                               ((C350 * C333 * C328 / C348 - C345 * C378) * C350 / (C366 * C345) +
                                -0.37273024e2 * C339 / C409) /
                               0.87173106479036e2) -
                      C426) *
                         C334) /
                        C310) +
            (C419 + C423 * C334 / C310);
    }

    return result;
}

} // namespace excgrid
