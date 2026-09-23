// excgrid codegen source: the Perdew-Wang 1991 GGA correlation functional
// (the 1992 parametrization: the PW92 LDA piece + the H0/H1 gradient
// terms).  Fresh authorship; formulas from the PW91/PW92 papers
// (docs/mainpage.md references).  Regenerate with tools/regenerate.py.

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

XcKernelValue Pw91Correlation(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
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

    const double C46 = 2. * sigmaAb;
    const double C47 = rhoA + rhoB;
    const double C48 = rhoA - rhoB;
    const double C49 = 1. / 3.;
    const double C50 = 2. / 3.;
    const double C51 = 4. / 3.;
    const double C52 = std::pow(Pi, 2);
    const double C53 = 0.9999999999999999 * C48;
    const double C54 = C52 * C47;
    const double C55 = Pi * C47;
    const double C56 = sigmaAa + C46;
    const double C57 = C48 / C47;
    const double C58 = std::pow(2., C49);
    const double C59 = 3. * C54;
    const double C60 = 4. * C55;
    const double C61 = C56 + sigmaBb;
    const double C62 = C58 - 1.;
    const double C63 = C53 / C47;
    const double C64 = std::pow(C57, 4);
    const double C65 = 2. * C62;
    const double C66 = 3.4198418683227306 * C62;
    const double C67 = C63 + 1.;
    const double C68 = 1. - C63;
    const double C69 = 1. - C64;
    const double C70 = 3. / C60;
    const double C71 = std::sqrt(C61);
    const double C72 = std::pow(C59, C49);
    const double C73 = C72 / Pi;
    const double C74 = std::sqrt(C70);
    const double C75 = std::pow(C67, C50);
    const double C76 = std::pow(C67, C51);
    const double C77 = std::pow(C68, C50);
    const double C78 = std::pow(C68, C51);
    const double C79 = std::pow(C70, C49);
    const double C80 = std::pow(C70, C50);
    const double C81 = 0.11125 * C79;
    const double C82 = 0.20548 * C79;
    const double C83 = 0.2137 * C79;
    const double C84 = 0.49294 * C80;
    const double C85 = 0.49671 * C80;
    const double C86 = 0.62517 * C80;
    const double C87 = 0.88026 * C74;
    const double C88 = 1.6382 * C74;
    const double C89 = 3.3662 * C74;
    const double C90 = 3.5876 * C79;
    const double C91 = 3.6231 * C79;
    const double C92 = 6.1977 * C79;
    const double C93 = C75 + C77;
    const double C94 = C76 + C78;
    const double C95 = std::sqrt(C73);
    const double C96 = std::sqrt(C79);
    const double C97 = 7.5957 * C96;
    const double C98 = 10.357 * C96;
    const double C99 = 14.1189 * C96;
    const double C100 = C95 * C93;
    const double C101 = C81 + 1.;
    const double C102 = C82 + 1.;
    const double C103 = C83 + 1.;
    const double C104 = C94 - 2.;
    const double C105 = C93 / 2.;
    const double C106 = 2. * C100;
    const double C107 = C97 + C90;
    const double C108 = C98 + C91;
    const double C109 = C99 + C92;
    const double C110 = std::pow(C105, 3);
    const double C111 = 0.4452402138403639e-2 * C110;
    const double C112 = C47 * C106;
    const double C113 = C107 + C88;
    const double C114 = C108 + C87;
    const double C115 = C109 + C89;
    const double C116 = C113 + C84;
    const double C117 = C114 + C85;
    const double C118 = C115 + C86;
    const double C119 = C112 / 2.;
    const double C120 = 0.03109 * C118;
    const double C121 = 0.033774 * C117;
    const double C122 = 0.062182 * C116;
    const double C123 = C119 + 1e-16;
    const double C124 = 2. * C123;
    const double C125 = 1. / C120;
    const double C126 = 1. / C121;
    const double C127 = 1. / C122;
    const double C128 = C125 + 1.;
    const double C129 = C126 + 1.;
    const double C130 = C127 + 1.;
    const double C131 = C71 / C124;
    const double C132 = std::log(C128);
    const double C133 = std::log(C129);
    const double C134 = std::log(C130);
    const double C135 = std::pow(C131, 2);
    const double C136 = 0.18 * C135;
    const double C137 = C101 * C133;
    const double C138 = C102 * C132;
    const double C139 = C103 * C134;
    const double C140 = 0.03109 * C138;
    const double C141 = 0.062182 * C139;
    const double C142 = C137 * C104;
    const double C143 = C142 * C69;
    const double C144 = C141 - C140;
    const double C145 = 0.033774 * C143;
    const double C146 = C144 * C104;
    const double C147 = C64 * C146;
    const double C148 = C145 / C66;
    const double C149 = C148 - C141;
    const double C150 = C147 / C65;
    const double C151 = C149 + C150;
    const double C152 = 0.18 * C151;
    const double C153 = C152 / C111;
    const double C154 = -C153;
    const double C155 = std::exp(C154);
    const double C156 = C155 - 1.;
    const double C157 = 0.6672632268006112e-1 * C156;
    const double C158 = C136 / C157;

    result.exc =
        C47 *
        ((C151 + 0.4452402138403639e-2 * C110 *
                     std::log(0.18 * C135 * (C158 + 1.) /
                                  (0.6672632268006112e-1 * ((C158 + std::pow(C158, 2)) + 1.)) +
                              1.) /
                     0.18) +
         15.7559203494831455 *
             (((((23.266 * C79 + 0.007389 * C80) + 2.568) /
                    (1000. * (((8.723 * C79 + 0.472 * C80) + 0.22167 / C60) + 1.)) +
                0.001667) -
               0.004235) -
              -0.005001 / 7.) *
             C110 * C135 *
             std::exp(-100. * std::pow(C105, 4) * std::pow(2. * C95, 2) * C135 /
                      std::pow(C59, C50)));

    const double C163 = -0.403944e2 * Pi;
    const double C164 = -24. * Pi;
    const double C165 = -0.196584e2 * Pi;
    const double C166 = -10.56312 * Pi;
    const double C167 = 0.007389 * C80;
    const double C168 = 0.472 * C80;
    const double C169 = 0.9999999999999999 * C47;
    const double C170 = 2 * C74;
    const double C171 = 2 * C95;
    const double C172 = 2 * C96;
    const double C173 = 3. * C52;
    const double C174 = 8.723 * C79;
    const double C175 = 12. * Pi;
    const double C176 = 23.266 * C79;
    const double C177 = C158 + 1.;
    const double C178 = C47 - C48;
    const double C179 = -2. / 3.;
    const double C180 = -1. / 3.;
    const double C181 = -0.005001 / 7.;
    const double C182 = 0.22167 / C60;
    const double C183 = std::pow(C105, 2);
    const double C184 = std::pow(C105, 4);
    const double C185 = std::pow(C111, 2);
    const double C186 = std::pow(C120, 2);
    const double C187 = std::pow(C121, 2);
    const double C188 = std::pow(C122, 2);
    const double C189 = std::pow(C124, 2);
    const double C190 = std::pow(C157, 2);
    const double C191 = std::pow(C158, 2);
    const double C192 = std::pow(C47, 2);
    const double C193 = std::pow(C57, 3);
    const double C194 = std::pow(C59, C50);
    const double C195 = std::pow(C60, 2);
    const double C196 = std::pow(C67, C49);
    const double C197 = std::pow(C68, C49);
    const double C198 = 2. * C189;
    const double C199 = 3. * C192;
    const double C200 = 3. * C195;
    const double C201 = 4 * C178;
    const double C202 = 6. * C192;
    const double C203 = C135 * C177;
    const double C204 = C186 * C128;
    const double C205 = C187 * C129;
    const double C206 = C188 * C130;
    const double C207 = C195 * C170;
    const double C208 = C195 * C172;
    const double C209 = Pi * C171;
    const double C210 = C158 + C191;
    const double C211 = C174 + C168;
    const double C212 = C176 + C167;
    const double C213 = C169 - C53;
    const double C214 = std::pow(C171, 2);
    const double C215 = std::pow(C59, C179);
    const double C216 = std::pow(C67, C180);
    const double C217 = std::pow(C68, C180);
    const double C218 = std::pow(C70, C179);
    const double C219 = std::pow(C70, C180);
    const double C220 = -4. * C213;
    const double C221 = -2. * C213;
    const double C222 = 0.18 * C203;
    const double C223 = 2. * C213;
    const double C224 = 3. * C208;
    const double C225 = 3. * C209;
    const double C226 = 4. * C213;
    const double C227 = C123 * C198;
    const double C228 = C184 * C214;
    const double C229 = C193 * C201;
    const double C230 = C215 * C173;
    const double C231 = C218 * C175;
    const double C232 = C219 * C164;
    const double C233 = C210 + 1.;
    const double C234 = C211 + C182;
    const double C235 = C212 + 2.568;
    const double C236 = C163 / C207;
    const double C237 = C165 / C207;
    const double C238 = C166 / C207;
    const double C239 = -0.141189e2 * C231;
    const double C240 = -10.357 * C231;
    const double C241 = -7.5957 * C231;
    const double C242 = -6.1977 * C231;
    const double C243 = -3.6231 * C231;
    const double C244 = -3.5876 * C231;
    const double C245 = -0.2137 * C231;
    const double C246 = -0.20548 * C231;
    const double C247 = -0.11125 * C231;
    const double C248 = 0.6672632268006112e-1 * C233;
    const double C249 = 0.49294 * C232;
    const double C250 = 0.49671 * C232;
    const double C251 = 0.62517 * C232;
    const double C252 = 2. * C227;
    const double C253 = C142 * C229;
    const double C254 = C146 * C229;
    const double C255 = C196 * C226;
    const double C256 = C197 * C220;
    const double C257 = C216 * C223;
    const double C258 = C217 * C221;
    const double C259 = C228 * C135;
    const double C260 = C93 * C230;
    const double C261 = C234 + 1.;
    const double C262 = 100. * C259;
    const double C263 = 1000. * C261;
    const double C264 = C132 * C246;
    const double C265 = C133 * C247;
    const double C266 = C134 * C245;
    const double C267 = C255 + C256;
    const double C268 = C257 + C258;
    const double C269 = C222 / C248;
    const double C270 = C239 / C224;
    const double C271 = C240 / C224;
    const double C272 = C241 / C224;
    const double C273 = C242 / C200;
    const double C274 = C243 / C200;
    const double C275 = C244 / C200;
    const double C276 = C249 / C200;
    const double C277 = C250 / C200;
    const double C278 = C251 / C200;
    const double C279 = C253 / C192;
    const double C280 = C254 / C192;
    const double C281 = C260 / C225;
    const double C282 = 3 * C268;
    const double C283 = C137 * C267;
    const double C284 = C144 * C267;
    const double C285 = C95 * C268;
    const double C286 = C269 + 1.;
    const double C287 = C270 + C273;
    const double C288 = C271 + C274;
    const double C289 = C272 + C275;
    const double C290 = C235 / C263;
    const double C291 = C262 / C194;
    const double C292 = C264 / C200;
    const double C293 = C265 / C200;
    const double C294 = C266 / C200;
    const double C295 = C183 * C282;
    const double C296 = C287 + C236;
    const double C297 = C288 + C238;
    const double C298 = C289 + C237;
    const double C299 = C290 + 0.001667;
    const double C300 = -C291;
    const double C301 = C283 / C199;
    const double C302 = C284 / C199;
    const double C303 = C285 / C199;
    const double C304 = std::log(C286);
    const double C305 = 0.4452402138403639e-2 * C295;
    const double C306 = C296 + C278;
    const double C307 = C297 + C277;
    const double C308 = C298 + C276;
    const double C309 = C303 + C281;
    const double C310 = C299 - 0.004235;
    const double C311 = std::exp(C300);
    const double C312 = 0.03109 * C306;
    const double C313 = 0.033774 * C307;
    const double C314 = 0.062182 * C308;
    const double C315 = 2. * C309;
    const double C316 = C151 * C305;
    const double C317 = C310 - C181;
    const double C318 = 0.18 * C316;
    const double C319 = C101 * C313;
    const double C320 = C102 * C312;
    const double C321 = C103 * C314;
    const double C322 = C317 * C110;
    const double C323 = C47 * C315;
    const double C324 = C322 * C135;
    const double C325 = C323 + C106;
    const double C326 = C318 / C202;
    const double C327 = C319 / C205;
    const double C328 = C320 / C204;
    const double C329 = C321 / C206;
    const double C330 = C325 * C61;
    const double C331 = C292 - C328;
    const double C332 = C293 - C327;
    const double C333 = C294 - C329;
    const double C334 = -4. * C330;
    const double C335 = -0.72 * C330;
    const double C336 = 0.03109 * C331;
    const double C337 = 0.062182 * C333;
    const double C338 = C332 * C104;
    const double C339 = C156 * C335;
    const double C340 = C301 + C338;
    const double C341 = C337 - C336;
    const double C342 = 0.6672632268006112e-1 * C339;
    const double C343 = C340 * C69;
    const double C344 = C341 * C104;
    const double C345 = C302 + C344;
    const double C346 = C343 - C279;
    const double C347 = C342 / C252;
    const double C348 = 0.033774 * C346;
    const double C349 = C64 * C345;
    const double C350 = C349 + C280;
    const double C351 = C348 / C66;
    const double C352 = C351 - C337;
    const double C353 = C350 / C65;
    const double C354 = C352 + C353;
    const double C355 = 0.18 * C354;
    const double C356 = C110 * C355;
    const double C357 = 0.4452402138403639e-2 * C356;
    const double C358 = C357 - C326;
    const double C359 = C155 * C358;
    const double C360 = -0.6672632268006112e-1 * C359;
    const double C361 = C135 * C360;
    const double C362 = 0.18 * C361;
    const double C363 = C362 / C185;
    const double C364 = C347 - C363;

    result.vrhoA =
        C47 * ((C354 + 0.4452402138403639e-2 *
                           (C110 *
                                (0.6672632268006112e-1 * C233 * 0.18 *
                                     (C135 * C364 / C190 + C177 * C334 / C252) -
                                 0.18 * C203 * 0.6672632268006112e-1 *
                                     (C364 / C190 +
                                      2 * C364 * C136 / (0.6672632268006112e-1 * C156 * C190))) /
                                (std::pow(C248, 2) * C286) +
                            C304 * C295 / C202) /
                           0.18) +
               15.7559203494831455 *
                   ((C322 * C334 / C252 +
                     (C317 * C295 / C202 +
                      C110 *
                          (1000. * C261 * (-0.23266e2 * C231 + 0.007389 * C232) / C200 -
                           C235 * 1000. *
                               ((-8.723 * C231 + 0.472 * C232) / C200 - 0.88668 * Pi / C195)) /
                          std::pow(C263, 2)) *
                         C135) *
                        C311 -
                    C324 * C311 *
                        (C194 * 100. *
                             (C228 * C334 / C252 +
                              (C184 * 2. * C95 * 4. * C230 / C225 + C214 * C110 * 4 * C268 / C202) *
                                  C135) -
                         100. * C259 * std::pow(C59, C180) * 6. * C52 / 3.) /
                        std::pow(C59, C51))) +
        ((C151 + 0.4452402138403639e-2 * C110 * C304 / 0.18) + 15.7559203494831455 * C324 * C311);

    const double C366 = -0.403944e2 * Pi;
    const double C367 = -24. * Pi;
    const double C368 = -0.196584e2 * Pi;
    const double C369 = -10.56312 * Pi;
    const double C370 = -0.9999999999999999 * C47;
    const double C371 = 0.007389 * C80;
    const double C372 = 0.472 * C80;
    const double C373 = 2 * C74;
    const double C374 = 2 * C95;
    const double C375 = 2 * C96;
    const double C376 = 3. * C52;
    const double C377 = 8.723 * C79;
    const double C378 = 12. * Pi;
    const double C379 = 23.266 * C79;
    const double C380 = C158 + 1.;
    const double C381 = C47 + C48;
    const double C382 = -2. / 3.;
    const double C383 = -1. / 3.;
    const double C384 = -0.005001 / 7.;
    const double C385 = 0.22167 / C60;
    const double C386 = std::pow(C105, 2);
    const double C387 = std::pow(C105, 4);
    const double C388 = std::pow(C111, 2);
    const double C389 = std::pow(C120, 2);
    const double C390 = std::pow(C121, 2);
    const double C391 = std::pow(C122, 2);
    const double C392 = std::pow(C124, 2);
    const double C393 = std::pow(C157, 2);
    const double C394 = std::pow(C158, 2);
    const double C395 = std::pow(C47, 2);
    const double C396 = std::pow(C57, 3);
    const double C397 = std::pow(C59, C50);
    const double C398 = std::pow(C60, 2);
    const double C399 = std::pow(C67, C49);
    const double C400 = std::pow(C68, C49);
    const double C401 = -4 * C381;
    const double C402 = 2. * C392;
    const double C403 = 3. * C395;
    const double C404 = 3. * C398;
    const double C405 = 6. * C395;
    const double C406 = C135 * C380;
    const double C407 = C389 * C128;
    const double C408 = C390 * C129;
    const double C409 = C391 * C130;
    const double C410 = C398 * C373;
    const double C411 = C398 * C375;
    const double C412 = Pi * C374;
    const double C413 = C158 + C394;
    const double C414 = C377 + C372;
    const double C415 = C379 + C371;
    const double C416 = C370 - C53;
    const double C417 = std::pow(C374, 2);
    const double C418 = std::pow(C59, C382);
    const double C419 = std::pow(C67, C383);
    const double C420 = std::pow(C68, C383);
    const double C421 = std::pow(C70, C382);
    const double C422 = std::pow(C70, C383);
    const double C423 = -4. * C416;
    const double C424 = -2. * C416;
    const double C425 = 0.18 * C406;
    const double C426 = 2. * C416;
    const double C427 = 3. * C411;
    const double C428 = 3. * C412;
    const double C429 = 4. * C416;
    const double C430 = C123 * C402;
    const double C431 = C387 * C417;
    const double C432 = C396 * C401;
    const double C433 = C418 * C376;
    const double C434 = C421 * C378;
    const double C435 = C422 * C367;
    const double C436 = C413 + 1.;
    const double C437 = C414 + C385;
    const double C438 = C415 + 2.568;
    const double C439 = C366 / C410;
    const double C440 = C368 / C410;
    const double C441 = C369 / C410;
    const double C442 = -0.141189e2 * C434;
    const double C443 = -10.357 * C434;
    const double C444 = -7.5957 * C434;
    const double C445 = -6.1977 * C434;
    const double C446 = -3.6231 * C434;
    const double C447 = -3.5876 * C434;
    const double C448 = -0.2137 * C434;
    const double C449 = -0.20548 * C434;
    const double C450 = -0.11125 * C434;
    const double C451 = 0.6672632268006112e-1 * C436;
    const double C452 = 0.49294 * C435;
    const double C453 = 0.49671 * C435;
    const double C454 = 0.62517 * C435;
    const double C455 = 2. * C430;
    const double C456 = C142 * C432;
    const double C457 = C146 * C432;
    const double C458 = C399 * C429;
    const double C459 = C400 * C423;
    const double C460 = C419 * C426;
    const double C461 = C420 * C424;
    const double C462 = C431 * C135;
    const double C463 = C93 * C433;
    const double C464 = C437 + 1.;
    const double C465 = 100. * C462;
    const double C466 = 1000. * C464;
    const double C467 = C132 * C449;
    const double C468 = C133 * C450;
    const double C469 = C134 * C448;
    const double C470 = C458 + C459;
    const double C471 = C460 + C461;
    const double C472 = C425 / C451;
    const double C473 = C442 / C427;
    const double C474 = C443 / C427;
    const double C475 = C444 / C427;
    const double C476 = C445 / C404;
    const double C477 = C446 / C404;
    const double C478 = C447 / C404;
    const double C479 = C452 / C404;
    const double C480 = C453 / C404;
    const double C481 = C454 / C404;
    const double C482 = C456 / C395;
    const double C483 = C457 / C395;
    const double C484 = C463 / C428;
    const double C485 = 3 * C471;
    const double C486 = C137 * C470;
    const double C487 = C144 * C470;
    const double C488 = C95 * C471;
    const double C489 = C472 + 1.;
    const double C490 = C473 + C476;
    const double C491 = C474 + C477;
    const double C492 = C475 + C478;
    const double C493 = C438 / C466;
    const double C494 = C465 / C397;
    const double C495 = C467 / C404;
    const double C496 = C468 / C404;
    const double C497 = C469 / C404;
    const double C498 = C386 * C485;
    const double C499 = C490 + C439;
    const double C500 = C491 + C441;
    const double C501 = C492 + C440;
    const double C502 = C493 + 0.001667;
    const double C503 = -C494;
    const double C504 = C486 / C403;
    const double C505 = C487 / C403;
    const double C506 = C488 / C403;
    const double C507 = std::log(C489);
    const double C508 = 0.4452402138403639e-2 * C498;
    const double C509 = C499 + C481;
    const double C510 = C500 + C480;
    const double C511 = C501 + C479;
    const double C512 = C506 + C484;
    const double C513 = C502 - 0.004235;
    const double C514 = std::exp(C503);
    const double C515 = 0.03109 * C509;
    const double C516 = 0.033774 * C510;
    const double C517 = 0.062182 * C511;
    const double C518 = 2. * C512;
    const double C519 = C151 * C508;
    const double C520 = C513 - C384;
    const double C521 = 0.18 * C519;
    const double C522 = C101 * C516;
    const double C523 = C102 * C515;
    const double C524 = C103 * C517;
    const double C525 = C47 * C518;
    const double C526 = C520 * C110;
    const double C527 = C526 * C135;
    const double C528 = C525 + C106;
    const double C529 = C521 / C405;
    const double C530 = C522 / C408;
    const double C531 = C523 / C407;
    const double C532 = C524 / C409;
    const double C533 = C528 * C61;
    const double C534 = C495 - C531;
    const double C535 = C496 - C530;
    const double C536 = C497 - C532;
    const double C537 = -4. * C533;
    const double C538 = -0.72 * C533;
    const double C539 = 0.03109 * C534;
    const double C540 = 0.062182 * C536;
    const double C541 = C535 * C104;
    const double C542 = C156 * C538;
    const double C543 = C504 + C541;
    const double C544 = C540 - C539;
    const double C545 = 0.6672632268006112e-1 * C542;
    const double C546 = C543 * C69;
    const double C547 = C544 * C104;
    const double C548 = C505 + C547;
    const double C549 = C546 - C482;
    const double C550 = C545 / C455;
    const double C551 = 0.033774 * C549;
    const double C552 = C64 * C548;
    const double C553 = C552 + C483;
    const double C554 = C551 / C66;
    const double C555 = C554 - C540;
    const double C556 = C553 / C65;
    const double C557 = C555 + C556;
    const double C558 = 0.18 * C557;
    const double C559 = C110 * C558;
    const double C560 = 0.4452402138403639e-2 * C559;
    const double C561 = C560 - C529;
    const double C562 = C155 * C561;
    const double C563 = -0.6672632268006112e-1 * C562;
    const double C564 = C135 * C563;
    const double C565 = 0.18 * C564;
    const double C566 = C565 / C388;
    const double C567 = C550 - C566;

    result.vrhoB =
        C47 * ((C557 + 0.4452402138403639e-2 *
                           (C110 *
                                (0.6672632268006112e-1 * C436 * 0.18 *
                                     (C135 * C567 / C393 + C380 * C537 / C455) -
                                 0.18 * C406 * 0.6672632268006112e-1 *
                                     (C567 / C393 +
                                      2 * C567 * C136 / (0.6672632268006112e-1 * C156 * C393))) /
                                (std::pow(C451, 2) * C489) +
                            C507 * C498 / C405) /
                           0.18) +
               15.7559203494831455 *
                   ((C526 * C537 / C455 +
                     (C520 * C498 / C405 +
                      C110 *
                          (1000. * C464 * (-0.23266e2 * C434 + 0.007389 * C435) / C404 -
                           C438 * 1000. *
                               ((-8.723 * C434 + 0.472 * C435) / C404 - 0.88668 * Pi / C398)) /
                          std::pow(C466, 2)) *
                         C135) *
                        C514 -
                    C527 * C514 *
                        (C397 * 100. *
                             (C431 * C537 / C455 +
                              (C387 * 2. * C95 * 4. * C433 / C428 + C417 * C110 * 4 * C471 / C405) *
                                  C135) -
                         100. * C462 * std::pow(C59, C383) * 6. * C52 / 3.) /
                        std::pow(C59, C51))) +
        ((C151 + 0.4452402138403639e-2 * C110 * C507 / 0.18) + 15.7559203494831455 * C527 * C514);

    const double C569 = 2. * sigmaAb;
    const double C570 = rhoA + rhoB;
    const double C571 = rhoA - rhoB;
    const double C572 = -1. / 3.;
    const double C573 = -0.005001 / 7.;
    const double C574 = 1. / 3.;
    const double C575 = 2. / 3.;
    const double C576 = 4. / 3.;
    const double C577 = std::pow(Pi, 2);
    const double C578 = 0.9999999999999999 * C571;
    const double C579 = 6. * C570;
    const double C580 = C577 * C570;
    const double C581 = Pi * C570;
    const double C582 = sigmaAa + C569;
    const double C583 = C571 / C570;
    const double C584 = std::pow(2., C574);
    const double C585 = 3. * C580;
    const double C586 = 4. * C581;
    const double C587 = C582 + sigmaBb;
    const double C588 = C584 - 1.;
    const double C589 = C578 / C570;
    const double C590 = std::pow(C583, 4);
    const double C591 = 2. * C588;
    const double C592 = 3.4198418683227306 * C588;
    const double C593 = C589 + 1.;
    const double C594 = 1. - C589;
    const double C595 = 1. - C590;
    const double C596 = 0.22167 / C586;
    const double C597 = 3. / C586;
    const double C598 = std::sqrt(C587);
    const double C599 = std::pow(C585, C574);
    const double C600 = std::pow(C585, C575);
    const double C601 = 0.36 * C598;
    const double C602 = 2. * C598;
    const double C603 = C599 / Pi;
    const double C604 = std::sqrt(C597);
    const double C605 = std::pow(C593, C572);
    const double C606 = std::pow(C593, C574);
    const double C607 = std::pow(C593, C575);
    const double C608 = std::pow(C593, C576);
    const double C609 = std::pow(C594, C572);
    const double C610 = std::pow(C594, C574);
    const double C611 = std::pow(C594, C575);
    const double C612 = std::pow(C594, C576);
    const double C613 = std::pow(C597, C574);
    const double C614 = std::pow(C597, C575);
    const double C615 = 0. * C605;
    const double C616 = 0. * C606;
    const double C617 = 0. * C609;
    const double C618 = 0. * C610;
    const double C619 = 0.007389 * C614;
    const double C620 = 0.11125 * C613;
    const double C621 = 0.20548 * C613;
    const double C622 = 0.2137 * C613;
    const double C623 = 0.472 * C614;
    const double C624 = 0.49294 * C614;
    const double C625 = 0.49671 * C614;
    const double C626 = 0.62517 * C614;
    const double C627 = 0.88026 * C604;
    const double C628 = 1.6382 * C604;
    const double C629 = 3.3662 * C604;
    const double C630 = 3.5876 * C613;
    const double C631 = 3.6231 * C613;
    const double C632 = 6.1977 * C613;
    const double C633 = 8.723 * C613;
    const double C634 = 23.266 * C613;
    const double C635 = C607 + C611;
    const double C636 = C608 + C612;
    const double C637 = std::sqrt(C603);
    const double C638 = std::sqrt(C613);
    const double C639 = 2. * C637;
    const double C640 = 7.5957 * C638;
    const double C641 = 10.357 * C638;
    const double C642 = 14.1189 * C638;
    const double C643 = C637 * C635;
    const double C644 = C615 + C617;
    const double C645 = C616 + C618;
    const double C646 = C620 + 1.;
    const double C647 = C621 + 1.;
    const double C648 = C622 + 1.;
    const double C649 = C633 + C623;
    const double C650 = C634 + C619;
    const double C651 = C636 - 2.;
    const double C652 = C635 / 2.;
    const double C653 = 2. * C643;
    const double C654 = 3 * C644;
    const double C655 = C640 + C630;
    const double C656 = C641 + C631;
    const double C657 = C642 + C632;
    const double C658 = C649 + C596;
    const double C659 = C650 + 2.568;
    const double C660 = std::pow(C639, 2);
    const double C661 = std::pow(C652, 2);
    const double C662 = std::pow(C652, 3);
    const double C663 = std::pow(C652, 4);
    const double C664 = 0.4452402138403639e-2 * C662;
    const double C665 = C570 * C653;
    const double C666 = C661 * C654;
    const double C667 = C663 * C660;
    const double C668 = C655 + C628;
    const double C669 = C656 + C627;
    const double C670 = C657 + C629;
    const double C671 = C658 + 1.;
    const double C672 = 1000. * C671;
    const double C673 = C668 + C624;
    const double C674 = C669 + C625;
    const double C675 = C670 + C626;
    const double C676 = C665 / 2.;
    const double C677 = 0.03109 * C675;
    const double C678 = 0.033774 * C674;
    const double C679 = 0.062182 * C673;
    const double C680 = C676 + 1e-16;
    const double C681 = C659 / C672;
    const double C682 = 2. * C680;
    const double C683 = C681 + 0.001667;
    const double C684 = 1. / C677;
    const double C685 = 1. / C678;
    const double C686 = 1. / C679;
    const double C687 = C598 * C682;
    const double C688 = C684 + 1.;
    const double C689 = C685 + 1.;
    const double C690 = C686 + 1.;
    const double C691 = C683 - 0.004235;
    const double C692 = C598 / C682;
    const double C693 = 2 * C687;
    const double C694 = C691 - C573;
    const double C695 = std::log(C688);
    const double C696 = std::log(C689);
    const double C697 = std::log(C690);
    const double C698 = std::pow(C692, 2);
    const double C699 = 0.18 * C698;
    const double C700 = C646 * C696;
    const double C701 = C647 * C695;
    const double C702 = C648 * C697;
    const double C703 = C667 * C698;
    const double C704 = C680 * C693;
    const double C705 = C694 * C662;
    const double C706 = 0.03109 * C701;
    const double C707 = 0.062182 * C702;
    const double C708 = 2. * C704;
    const double C709 = 100. * C703;
    const double C710 = C700 * C651;
    const double C711 = C710 * C595;
    const double C712 = C707 - C706;
    const double C713 = C709 / C600;
    const double C714 = 0.033774 * C711;
    const double C715 = C712 * C651;
    const double C716 = -C713;
    const double C717 = C590 * C715;
    const double C718 = C714 / C592;
    const double C719 = std::exp(C716);
    const double C720 = C718 - C707;
    const double C721 = C717 / C591;
    const double C722 = C720 + C721;
    const double C723 = 0.18 * C722;
    const double C724 = C723 / C664;
    const double C725 = -C724;
    const double C726 = std::exp(C725);
    const double C727 = C726 - 1.;
    const double C728 = 0.6672632268006112e-1 * C727;
    const double C729 = C704 * C728;
    const double C730 = C699 / C728;
    const double C731 = 2. * C729;
    const double C732 = C730 + 1.;
    const double C733 = std::pow(C730, 2);
    const double C734 = C698 * C732;
    const double C735 = C730 + C733;
    const double C736 = 0.18 * C734;
    const double C737 = C735 + 1.;
    const double C738 = 0.6672632268006112e-1 * C737;
    const double C739 = C736 / C738;
    const double C740 = C739 + 1.;

    result.vsigmaAa =
        C570 * (((0.033774 * C595 * C700 * C645 / (3. * C570 * C592) +
                  C590 * C712 * C645 / (3. * C570 * C591)) +
                 0.4452402138403639e-2 *
                     (C662 *
                          (0.6672632268006112e-1 * C737 * 0.18 *
                               (C698 * C601 / C731 + C732 * C602 / C708) -
                           0.18 * C734 * 0.6672632268006112e-1 *
                               (C601 / C731 +
                                0.72 * C598 * C699 / (0.6672632268006112e-1 * C727 * C731))) /
                          (std::pow(C738, 2) * C740) +
                      std::log(C740) * C666 / C579) /
                     0.18) +
                15.7559203494831455 *
                    ((C705 * C602 / C708 + C698 * C694 * C666 / C579) * C719 -
                     C705 * C698 * C719 * 100. *
                         (C667 * C602 / C708 + C698 * C660 * C662 * 4 * C644 / C579) / C600));

    const double C742 = 2. * sigmaAb;
    const double C743 = rhoA + rhoB;
    const double C744 = rhoA - rhoB;
    const double C745 = -1. / 3.;
    const double C746 = -0.005001 / 7.;
    const double C747 = 1. / 3.;
    const double C748 = 2. / 3.;
    const double C749 = 4. / 3.;
    const double C750 = std::pow(Pi, 2);
    const double C751 = 0.9999999999999999 * C744;
    const double C752 = 6. * C743;
    const double C753 = C750 * C743;
    const double C754 = Pi * C743;
    const double C755 = sigmaAa + C742;
    const double C756 = C744 / C743;
    const double C757 = std::pow(2., C747);
    const double C758 = 3. * C753;
    const double C759 = 4. * C754;
    const double C760 = C755 + sigmaBb;
    const double C761 = C757 - 1.;
    const double C762 = C751 / C743;
    const double C763 = std::pow(C756, 4);
    const double C764 = 2. * C761;
    const double C765 = 3.4198418683227306 * C761;
    const double C766 = C762 + 1.;
    const double C767 = 1. - C762;
    const double C768 = 1. - C763;
    const double C769 = 0.22167 / C759;
    const double C770 = 3. / C759;
    const double C771 = std::sqrt(C760);
    const double C772 = std::pow(C758, C747);
    const double C773 = std::pow(C758, C748);
    const double C774 = 0.72 * C771;
    const double C775 = 4. * C771;
    const double C776 = C772 / Pi;
    const double C777 = std::sqrt(C770);
    const double C778 = std::pow(C766, C745);
    const double C779 = std::pow(C766, C747);
    const double C780 = std::pow(C766, C748);
    const double C781 = std::pow(C766, C749);
    const double C782 = std::pow(C767, C745);
    const double C783 = std::pow(C767, C747);
    const double C784 = std::pow(C767, C748);
    const double C785 = std::pow(C767, C749);
    const double C786 = std::pow(C770, C747);
    const double C787 = std::pow(C770, C748);
    const double C788 = 0. * C778;
    const double C789 = 0. * C779;
    const double C790 = 0. * C782;
    const double C791 = 0. * C783;
    const double C792 = 0.007389 * C787;
    const double C793 = 0.11125 * C786;
    const double C794 = 0.20548 * C786;
    const double C795 = 0.2137 * C786;
    const double C796 = 0.472 * C787;
    const double C797 = 0.49294 * C787;
    const double C798 = 0.49671 * C787;
    const double C799 = 0.62517 * C787;
    const double C800 = 0.88026 * C777;
    const double C801 = 1.6382 * C777;
    const double C802 = 3.3662 * C777;
    const double C803 = 3.5876 * C786;
    const double C804 = 3.6231 * C786;
    const double C805 = 6.1977 * C786;
    const double C806 = 8.723 * C786;
    const double C807 = 23.266 * C786;
    const double C808 = C780 + C784;
    const double C809 = C781 + C785;
    const double C810 = std::sqrt(C776);
    const double C811 = std::sqrt(C786);
    const double C812 = 2. * C810;
    const double C813 = 7.5957 * C811;
    const double C814 = 10.357 * C811;
    const double C815 = 14.1189 * C811;
    const double C816 = C810 * C808;
    const double C817 = C788 + C790;
    const double C818 = C789 + C791;
    const double C819 = C793 + 1.;
    const double C820 = C794 + 1.;
    const double C821 = C795 + 1.;
    const double C822 = C806 + C796;
    const double C823 = C807 + C792;
    const double C824 = C809 - 2.;
    const double C825 = C808 / 2.;
    const double C826 = 2. * C816;
    const double C827 = 3 * C817;
    const double C828 = C813 + C803;
    const double C829 = C814 + C804;
    const double C830 = C815 + C805;
    const double C831 = C822 + C769;
    const double C832 = C823 + 2.568;
    const double C833 = std::pow(C812, 2);
    const double C834 = std::pow(C825, 2);
    const double C835 = std::pow(C825, 3);
    const double C836 = std::pow(C825, 4);
    const double C837 = 0.4452402138403639e-2 * C835;
    const double C838 = C743 * C826;
    const double C839 = C834 * C827;
    const double C840 = C836 * C833;
    const double C841 = C828 + C801;
    const double C842 = C829 + C800;
    const double C843 = C830 + C802;
    const double C844 = C831 + 1.;
    const double C845 = 1000. * C844;
    const double C846 = C841 + C797;
    const double C847 = C842 + C798;
    const double C848 = C843 + C799;
    const double C849 = C838 / 2.;
    const double C850 = 0.03109 * C848;
    const double C851 = 0.033774 * C847;
    const double C852 = 0.062182 * C846;
    const double C853 = C849 + 1e-16;
    const double C854 = C832 / C845;
    const double C855 = 2. * C853;
    const double C856 = C854 + 0.001667;
    const double C857 = 1. / C850;
    const double C858 = 1. / C851;
    const double C859 = 1. / C852;
    const double C860 = C771 * C855;
    const double C861 = C857 + 1.;
    const double C862 = C858 + 1.;
    const double C863 = C859 + 1.;
    const double C864 = C856 - 0.004235;
    const double C865 = C771 / C855;
    const double C866 = 2 * C860;
    const double C867 = C864 - C746;
    const double C868 = std::log(C861);
    const double C869 = std::log(C862);
    const double C870 = std::log(C863);
    const double C871 = std::pow(C865, 2);
    const double C872 = 0.18 * C871;
    const double C873 = C819 * C869;
    const double C874 = C820 * C868;
    const double C875 = C821 * C870;
    const double C876 = C840 * C871;
    const double C877 = C853 * C866;
    const double C878 = C867 * C835;
    const double C879 = 0.03109 * C874;
    const double C880 = 0.062182 * C875;
    const double C881 = 2. * C877;
    const double C882 = 100. * C876;
    const double C883 = C873 * C824;
    const double C884 = C883 * C768;
    const double C885 = C880 - C879;
    const double C886 = C882 / C773;
    const double C887 = 0.033774 * C884;
    const double C888 = C885 * C824;
    const double C889 = -C886;
    const double C890 = C763 * C888;
    const double C891 = C887 / C765;
    const double C892 = std::exp(C889);
    const double C893 = C891 - C880;
    const double C894 = C890 / C764;
    const double C895 = C893 + C894;
    const double C896 = 0.18 * C895;
    const double C897 = C896 / C837;
    const double C898 = -C897;
    const double C899 = std::exp(C898);
    const double C900 = C899 - 1.;
    const double C901 = 0.6672632268006112e-1 * C900;
    const double C902 = C877 * C901;
    const double C903 = C872 / C901;
    const double C904 = 2. * C902;
    const double C905 = C903 + 1.;
    const double C906 = std::pow(C903, 2);
    const double C907 = C871 * C905;
    const double C908 = C903 + C906;
    const double C909 = 0.18 * C907;
    const double C910 = C908 + 1.;
    const double C911 = 0.6672632268006112e-1 * C910;
    const double C912 = C909 / C911;
    const double C913 = C912 + 1.;

    result.vsigmaAb =
        C743 * (((0.033774 * C768 * C873 * C818 / (3. * C743 * C765) +
                  C763 * C885 * C818 / (3. * C743 * C764)) +
                 0.4452402138403639e-2 *
                     (C835 *
                          (0.6672632268006112e-1 * C910 * 0.18 *
                               (C871 * C774 / C904 + C905 * C775 / C881) -
                           0.18 * C907 * 0.6672632268006112e-1 *
                               (C774 / C904 +
                                1.44 * C771 * C872 / (0.6672632268006112e-1 * C900 * C904))) /
                          (std::pow(C911, 2) * C913) +
                      std::log(C913) * C839 / C752) /
                     0.18) +
                15.7559203494831455 *
                    ((C878 * C775 / C881 + C871 * C867 * C839 / C752) * C892 -
                     C878 * C871 * C892 * 100. *
                         (C840 * C775 / C881 + C871 * C833 * C835 * 4 * C817 / C752) / C773));

    const double C915 = 2. * sigmaAb;
    const double C916 = rhoA + rhoB;
    const double C917 = rhoA - rhoB;
    const double C918 = -1. / 3.;
    const double C919 = -0.005001 / 7.;
    const double C920 = 1. / 3.;
    const double C921 = 2. / 3.;
    const double C922 = 4. / 3.;
    const double C923 = std::pow(Pi, 2);
    const double C924 = 0.9999999999999999 * C917;
    const double C925 = 6. * C916;
    const double C926 = C923 * C916;
    const double C927 = Pi * C916;
    const double C928 = sigmaAa + C915;
    const double C929 = C917 / C916;
    const double C930 = std::pow(2., C920);
    const double C931 = 3. * C926;
    const double C932 = 4. * C927;
    const double C933 = C928 + sigmaBb;
    const double C934 = C930 - 1.;
    const double C935 = C924 / C916;
    const double C936 = std::pow(C929, 4);
    const double C937 = 2. * C934;
    const double C938 = 3.4198418683227306 * C934;
    const double C939 = C935 + 1.;
    const double C940 = 1. - C935;
    const double C941 = 1. - C936;
    const double C942 = 0.22167 / C932;
    const double C943 = 3. / C932;
    const double C944 = std::sqrt(C933);
    const double C945 = std::pow(C931, C920);
    const double C946 = std::pow(C931, C921);
    const double C947 = 0.36 * C944;
    const double C948 = 2. * C944;
    const double C949 = C945 / Pi;
    const double C950 = std::sqrt(C943);
    const double C951 = std::pow(C939, C918);
    const double C952 = std::pow(C939, C920);
    const double C953 = std::pow(C939, C921);
    const double C954 = std::pow(C939, C922);
    const double C955 = std::pow(C940, C918);
    const double C956 = std::pow(C940, C920);
    const double C957 = std::pow(C940, C921);
    const double C958 = std::pow(C940, C922);
    const double C959 = std::pow(C943, C920);
    const double C960 = std::pow(C943, C921);
    const double C961 = 0. * C951;
    const double C962 = 0. * C952;
    const double C963 = 0. * C955;
    const double C964 = 0. * C956;
    const double C965 = 0.007389 * C960;
    const double C966 = 0.11125 * C959;
    const double C967 = 0.20548 * C959;
    const double C968 = 0.2137 * C959;
    const double C969 = 0.472 * C960;
    const double C970 = 0.49294 * C960;
    const double C971 = 0.49671 * C960;
    const double C972 = 0.62517 * C960;
    const double C973 = 0.88026 * C950;
    const double C974 = 1.6382 * C950;
    const double C975 = 3.3662 * C950;
    const double C976 = 3.5876 * C959;
    const double C977 = 3.6231 * C959;
    const double C978 = 6.1977 * C959;
    const double C979 = 8.723 * C959;
    const double C980 = 23.266 * C959;
    const double C981 = C953 + C957;
    const double C982 = C954 + C958;
    const double C983 = std::sqrt(C949);
    const double C984 = std::sqrt(C959);
    const double C985 = 2. * C983;
    const double C986 = 7.5957 * C984;
    const double C987 = 10.357 * C984;
    const double C988 = 14.1189 * C984;
    const double C989 = C983 * C981;
    const double C990 = C961 + C963;
    const double C991 = C962 + C964;
    const double C992 = C966 + 1.;
    const double C993 = C967 + 1.;
    const double C994 = C968 + 1.;
    const double C995 = C979 + C969;
    const double C996 = C980 + C965;
    const double C997 = C982 - 2.;
    const double C998 = C981 / 2.;
    const double C999 = 2. * C989;
    const double C1000 = 3 * C990;
    const double C1001 = C986 + C976;
    const double C1002 = C987 + C977;
    const double C1003 = C988 + C978;
    const double C1004 = C995 + C942;
    const double C1005 = C996 + 2.568;
    const double C1006 = std::pow(C985, 2);
    const double C1007 = std::pow(C998, 2);
    const double C1008 = std::pow(C998, 3);
    const double C1009 = std::pow(C998, 4);
    const double C1010 = 0.4452402138403639e-2 * C1008;
    const double C1011 = C1007 * C1000;
    const double C1012 = C1009 * C1006;
    const double C1013 = C916 * C999;
    const double C1014 = C1001 + C974;
    const double C1015 = C1002 + C973;
    const double C1016 = C1003 + C975;
    const double C1017 = C1004 + 1.;
    const double C1018 = 1000. * C1017;
    const double C1019 = C1014 + C970;
    const double C1020 = C1015 + C971;
    const double C1021 = C1016 + C972;
    const double C1022 = C1013 / 2.;
    const double C1023 = 0.03109 * C1021;
    const double C1024 = 0.033774 * C1020;
    const double C1025 = 0.062182 * C1019;
    const double C1026 = C1022 + 1e-16;
    const double C1027 = C1005 / C1018;
    const double C1028 = 2. * C1026;
    const double C1029 = C1027 + 0.001667;
    const double C1030 = 1. / C1023;
    const double C1031 = 1. / C1024;
    const double C1032 = 1. / C1025;
    const double C1033 = C944 * C1028;
    const double C1034 = C1030 + 1.;
    const double C1035 = C1031 + 1.;
    const double C1036 = C1032 + 1.;
    const double C1037 = C1029 - 0.004235;
    const double C1038 = C944 / C1028;
    const double C1039 = 2 * C1033;
    const double C1040 = C1037 - C919;
    const double C1041 = std::log(C1034);
    const double C1042 = std::log(C1035);
    const double C1043 = std::log(C1036);
    const double C1044 = std::pow(C1038, 2);
    const double C1045 = 0.18 * C1044;
    const double C1046 = C1012 * C1044;
    const double C1047 = C1026 * C1039;
    const double C1048 = C1040 * C1008;
    const double C1049 = C992 * C1042;
    const double C1050 = C993 * C1041;
    const double C1051 = C994 * C1043;
    const double C1052 = 0.03109 * C1050;
    const double C1053 = 0.062182 * C1051;
    const double C1054 = 2. * C1047;
    const double C1055 = 100. * C1046;
    const double C1056 = C1049 * C997;
    const double C1057 = C1056 * C941;
    const double C1058 = C1053 - C1052;
    const double C1059 = C1055 / C946;
    const double C1060 = 0.033774 * C1057;
    const double C1061 = C1058 * C997;
    const double C1062 = -C1059;
    const double C1063 = C936 * C1061;
    const double C1064 = C1060 / C938;
    const double C1065 = std::exp(C1062);
    const double C1066 = C1064 - C1053;
    const double C1067 = C1063 / C937;
    const double C1068 = C1066 + C1067;
    const double C1069 = 0.18 * C1068;
    const double C1070 = C1069 / C1010;
    const double C1071 = -C1070;
    const double C1072 = std::exp(C1071);
    const double C1073 = C1072 - 1.;
    const double C1074 = 0.6672632268006112e-1 * C1073;
    const double C1075 = C1047 * C1074;
    const double C1076 = C1045 / C1074;
    const double C1077 = 2. * C1075;
    const double C1078 = C1076 + 1.;
    const double C1079 = std::pow(C1076, 2);
    const double C1080 = C1044 * C1078;
    const double C1081 = C1076 + C1079;
    const double C1082 = 0.18 * C1080;
    const double C1083 = C1081 + 1.;
    const double C1084 = 0.6672632268006112e-1 * C1083;
    const double C1085 = C1082 / C1084;
    const double C1086 = C1085 + 1.;

    result.vsigmaBb =
        C916 * (((0.033774 * C941 * C1049 * C991 / (3. * C916 * C938) +
                  C936 * C1058 * C991 / (3. * C916 * C937)) +
                 0.4452402138403639e-2 *
                     (C1008 *
                          (0.6672632268006112e-1 * C1083 * 0.18 *
                               (C1044 * C947 / C1077 + C1078 * C948 / C1054) -
                           0.18 * C1080 * 0.6672632268006112e-1 *
                               (C947 / C1077 +
                                0.72 * C944 * C1045 / (0.6672632268006112e-1 * C1073 * C1077))) /
                          (std::pow(C1084, 2) * C1086) +
                      std::log(C1086) * C1011 / C925) /
                     0.18) +
                15.7559203494831455 *
                    ((C1048 * C948 / C1054 + C1044 * C1040 * C1011 / C925) * C1065 -
                     C1048 * C1044 * C1065 * 100. *
                         (C1012 * C948 / C1054 + C1044 * C1006 * C1008 * 4 * C990 / C925) / C946));

    return result;
}

} // namespace excgrid
