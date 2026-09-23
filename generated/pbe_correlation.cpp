// excgrid codegen source: the Perdew-Burke-Ernzerhof 1996 GGA correlation
// functional.  Fresh authorship; formula from the PBE paper
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

XcKernelValue PbeCorrelation(
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
    const double C111 = 0.3109069086965489e-1 * C110;
    const double C112 = C47 * C106;
    const double C113 = C107 + C88;
    const double C114 = C108 + C87;
    const double C115 = C109 + C89;
    const double C116 = C113 + C84;
    const double C117 = C114 + C85;
    const double C118 = C115 + C86;
    const double C119 = C112 / 2.;
    const double C120 = 0.0310907 * C118;
    const double C121 = 0.0337738 * C117;
    const double C122 = 0.0621814 * C116;
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
    const double C136 = 0.06672455060314922 * C135;
    const double C137 = C101 * C133;
    const double C138 = C102 * C132;
    const double C139 = C103 * C134;
    const double C140 = 0.0310907 * C138;
    const double C141 = 0.0621814 * C139;
    const double C142 = C137 * C104;
    const double C143 = C142 * C69;
    const double C144 = C141 - C140;
    const double C145 = 0.0337738 * C143;
    const double C146 = C144 * C104;
    const double C147 = C64 * C146;
    const double C148 = C145 / C66;
    const double C149 = C148 - C141;
    const double C150 = C147 / C65;
    const double C151 = C149 + C150;
    const double C152 = C151 / C111;
    const double C153 = -C152;
    const double C154 = std::exp(C153);
    const double C155 = C154 - 1.;
    const double C156 = 0.3109069086965489e-1 * C155;
    const double C157 = C136 / C156;

    result.exc =
        C47 * (C151 + 0.3109069086965489e-1 * C110 *
                          std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.));

    const double C162 = -0.403944e2 * Pi;
    const double C163 = -24. * Pi;
    const double C164 = -0.196584e2 * Pi;
    const double C165 = -10.56312 * Pi;
    const double C166 = 0.9999999999999999 * C47;
    const double C167 = 2 * C74;
    const double C168 = 2 * C95;
    const double C169 = 2 * C96;
    const double C170 = 3. * C52;
    const double C171 = 12. * Pi;
    const double C172 = C157 + 1.;
    const double C173 = C47 - C48;
    const double C174 = -2. / 3.;
    const double C175 = -1. / 3.;
    const double C176 = std::pow(C105, 2);
    const double C177 = std::pow(C111, 2);
    const double C178 = std::pow(C120, 2);
    const double C179 = std::pow(C121, 2);
    const double C180 = std::pow(C122, 2);
    const double C181 = std::pow(C124, 2);
    const double C182 = std::pow(C156, 2);
    const double C183 = std::pow(C157, 2);
    const double C184 = std::pow(C47, 2);
    const double C185 = std::pow(C57, 3);
    const double C186 = std::pow(C60, 2);
    const double C187 = std::pow(C67, C49);
    const double C188 = std::pow(C68, C49);
    const double C189 = 2. * C181;
    const double C190 = 3. * C184;
    const double C191 = 3. * C186;
    const double C192 = 4 * C173;
    const double C193 = 6. * C184;
    const double C194 = C135 * C172;
    const double C195 = C178 * C128;
    const double C196 = C179 * C129;
    const double C197 = C180 * C130;
    const double C198 = C186 * C167;
    const double C199 = C186 * C169;
    const double C200 = Pi * C168;
    const double C201 = C157 + C183;
    const double C202 = C166 - C53;
    const double C203 = std::pow(C59, C174);
    const double C204 = std::pow(C67, C175);
    const double C205 = std::pow(C68, C175);
    const double C206 = std::pow(C70, C174);
    const double C207 = std::pow(C70, C175);
    const double C208 = -4. * C202;
    const double C209 = -2. * C202;
    const double C210 = 0.06672455060314922 * C194;
    const double C211 = 2. * C202;
    const double C212 = 3. * C199;
    const double C213 = 3. * C200;
    const double C214 = 4. * C202;
    const double C215 = C123 * C189;
    const double C216 = C185 * C192;
    const double C217 = C203 * C170;
    const double C218 = C206 * C171;
    const double C219 = C207 * C163;
    const double C220 = C201 + 1.;
    const double C221 = C162 / C198;
    const double C222 = C164 / C198;
    const double C223 = C165 / C198;
    const double C224 = -0.141189e2 * C218;
    const double C225 = -10.357 * C218;
    const double C226 = -7.5957 * C218;
    const double C227 = -6.1977 * C218;
    const double C228 = -3.6231 * C218;
    const double C229 = -3.5876 * C218;
    const double C230 = -0.2137 * C218;
    const double C231 = -0.20548 * C218;
    const double C232 = -0.11125 * C218;
    const double C233 = 0.3109069086965489e-1 * C220;
    const double C234 = 0.49294 * C219;
    const double C235 = 0.49671 * C219;
    const double C236 = 0.62517 * C219;
    const double C237 = 2. * C215;
    const double C238 = C142 * C216;
    const double C239 = C146 * C216;
    const double C240 = C187 * C214;
    const double C241 = C188 * C208;
    const double C242 = C204 * C211;
    const double C243 = C205 * C209;
    const double C244 = C93 * C217;
    const double C245 = C132 * C231;
    const double C246 = C133 * C232;
    const double C247 = C134 * C230;
    const double C248 = C240 + C241;
    const double C249 = C242 + C243;
    const double C250 = C210 / C233;
    const double C251 = C224 / C212;
    const double C252 = C225 / C212;
    const double C253 = C226 / C212;
    const double C254 = C227 / C191;
    const double C255 = C228 / C191;
    const double C256 = C229 / C191;
    const double C257 = C234 / C191;
    const double C258 = C235 / C191;
    const double C259 = C236 / C191;
    const double C260 = C238 / C184;
    const double C261 = C239 / C184;
    const double C262 = C244 / C213;
    const double C263 = 3 * C249;
    const double C264 = C137 * C248;
    const double C265 = C144 * C248;
    const double C266 = C95 * C249;
    const double C267 = C250 + 1.;
    const double C268 = C251 + C254;
    const double C269 = C252 + C255;
    const double C270 = C253 + C256;
    const double C271 = C245 / C191;
    const double C272 = C246 / C191;
    const double C273 = C247 / C191;
    const double C274 = C176 * C263;
    const double C275 = C268 + C221;
    const double C276 = C269 + C223;
    const double C277 = C270 + C222;
    const double C278 = C264 / C190;
    const double C279 = C265 / C190;
    const double C280 = C266 / C190;
    const double C281 = std::log(C267);
    const double C282 = 0.3109069086965489e-1 * C274;
    const double C283 = C275 + C259;
    const double C284 = C276 + C258;
    const double C285 = C277 + C257;
    const double C286 = C280 + C262;
    const double C287 = 0.0310907 * C283;
    const double C288 = 0.0337738 * C284;
    const double C289 = 0.0621814 * C285;
    const double C290 = 2. * C286;
    const double C291 = C151 * C282;
    const double C292 = C101 * C288;
    const double C293 = C102 * C287;
    const double C294 = C103 * C289;
    const double C295 = C47 * C290;
    const double C296 = C291 / C193;
    const double C297 = C295 + C106;
    const double C298 = C292 / C196;
    const double C299 = C293 / C195;
    const double C300 = C294 / C197;
    const double C301 = C297 * C61;
    const double C302 = C271 - C299;
    const double C303 = C272 - C298;
    const double C304 = C273 - C300;
    const double C305 = -0.2668982024125968 * C301;
    const double C306 = 0.0310907 * C302;
    const double C307 = 0.0621814 * C304;
    const double C308 = C303 * C104;
    const double C309 = C155 * C305;
    const double C310 = C278 + C308;
    const double C311 = C307 - C306;
    const double C312 = 0.3109069086965489e-1 * C309;
    const double C313 = C310 * C69;
    const double C314 = C311 * C104;
    const double C315 = C279 + C314;
    const double C316 = C313 - C260;
    const double C317 = C312 / C237;
    const double C318 = 0.0337738 * C316;
    const double C319 = C64 * C315;
    const double C320 = C319 + C261;
    const double C321 = C318 / C66;
    const double C322 = C321 - C307;
    const double C323 = C320 / C65;
    const double C324 = C322 + C323;
    const double C325 = C110 * C324;
    const double C326 = 0.3109069086965489e-1 * C325;
    const double C327 = C326 - C296;
    const double C328 = C154 * C327;
    const double C329 = -0.3109069086965489e-1 * C328;
    const double C330 = C135 * C329;
    const double C331 = 0.06672455060314922 * C330;
    const double C332 = C331 / C177;
    const double C333 = C317 - C332;

    result.vrhoA =
        C47 * (C324 + 0.3109069086965489e-1 *
                          (C110 *
                               (0.3109069086965489e-1 * C220 * 0.06672455060314922 *
                                    (C135 * C333 / C182 + C172 * -4. * C301 / C237) -
                                0.06672455060314922 * C194 * 0.3109069086965489e-1 *
                                    (C333 / C182 +
                                     2 * C333 * C136 / (0.3109069086965489e-1 * C155 * C182))) /
                               (std::pow(C233, 2) * C267) +
                           C281 * C274 / C193)) +
        (C151 + 0.3109069086965489e-1 * C110 * C281);

    const double C335 = -0.403944e2 * Pi;
    const double C336 = -24. * Pi;
    const double C337 = -0.196584e2 * Pi;
    const double C338 = -10.56312 * Pi;
    const double C339 = -0.9999999999999999 * C47;
    const double C340 = 2 * C74;
    const double C341 = 2 * C95;
    const double C342 = 2 * C96;
    const double C343 = 3. * C52;
    const double C344 = 12. * Pi;
    const double C345 = C157 + 1.;
    const double C346 = C47 + C48;
    const double C347 = -2. / 3.;
    const double C348 = -1. / 3.;
    const double C349 = std::pow(C105, 2);
    const double C350 = std::pow(C111, 2);
    const double C351 = std::pow(C120, 2);
    const double C352 = std::pow(C121, 2);
    const double C353 = std::pow(C122, 2);
    const double C354 = std::pow(C124, 2);
    const double C355 = std::pow(C156, 2);
    const double C356 = std::pow(C157, 2);
    const double C357 = std::pow(C47, 2);
    const double C358 = std::pow(C57, 3);
    const double C359 = std::pow(C60, 2);
    const double C360 = std::pow(C67, C49);
    const double C361 = std::pow(C68, C49);
    const double C362 = -4 * C346;
    const double C363 = 2. * C354;
    const double C364 = 3. * C357;
    const double C365 = 3. * C359;
    const double C366 = 6. * C357;
    const double C367 = C135 * C345;
    const double C368 = C351 * C128;
    const double C369 = C352 * C129;
    const double C370 = C353 * C130;
    const double C371 = C359 * C340;
    const double C372 = C359 * C342;
    const double C373 = Pi * C341;
    const double C374 = C157 + C356;
    const double C375 = C339 - C53;
    const double C376 = std::pow(C59, C347);
    const double C377 = std::pow(C67, C348);
    const double C378 = std::pow(C68, C348);
    const double C379 = std::pow(C70, C347);
    const double C380 = std::pow(C70, C348);
    const double C381 = -4. * C375;
    const double C382 = -2. * C375;
    const double C383 = 0.06672455060314922 * C367;
    const double C384 = 2. * C375;
    const double C385 = 3. * C372;
    const double C386 = 3. * C373;
    const double C387 = 4. * C375;
    const double C388 = C123 * C363;
    const double C389 = C358 * C362;
    const double C390 = C376 * C343;
    const double C391 = C379 * C344;
    const double C392 = C380 * C336;
    const double C393 = C374 + 1.;
    const double C394 = C335 / C371;
    const double C395 = C337 / C371;
    const double C396 = C338 / C371;
    const double C397 = -0.141189e2 * C391;
    const double C398 = -10.357 * C391;
    const double C399 = -7.5957 * C391;
    const double C400 = -6.1977 * C391;
    const double C401 = -3.6231 * C391;
    const double C402 = -3.5876 * C391;
    const double C403 = -0.2137 * C391;
    const double C404 = -0.20548 * C391;
    const double C405 = -0.11125 * C391;
    const double C406 = 0.3109069086965489e-1 * C393;
    const double C407 = 0.49294 * C392;
    const double C408 = 0.49671 * C392;
    const double C409 = 0.62517 * C392;
    const double C410 = 2. * C388;
    const double C411 = C142 * C389;
    const double C412 = C146 * C389;
    const double C413 = C360 * C387;
    const double C414 = C361 * C381;
    const double C415 = C377 * C384;
    const double C416 = C378 * C382;
    const double C417 = C93 * C390;
    const double C418 = C132 * C404;
    const double C419 = C133 * C405;
    const double C420 = C134 * C403;
    const double C421 = C413 + C414;
    const double C422 = C415 + C416;
    const double C423 = C383 / C406;
    const double C424 = C397 / C385;
    const double C425 = C398 / C385;
    const double C426 = C399 / C385;
    const double C427 = C400 / C365;
    const double C428 = C401 / C365;
    const double C429 = C402 / C365;
    const double C430 = C407 / C365;
    const double C431 = C408 / C365;
    const double C432 = C409 / C365;
    const double C433 = C411 / C357;
    const double C434 = C412 / C357;
    const double C435 = C417 / C386;
    const double C436 = 3 * C422;
    const double C437 = C137 * C421;
    const double C438 = C144 * C421;
    const double C439 = C95 * C422;
    const double C440 = C423 + 1.;
    const double C441 = C424 + C427;
    const double C442 = C425 + C428;
    const double C443 = C426 + C429;
    const double C444 = C418 / C365;
    const double C445 = C419 / C365;
    const double C446 = C420 / C365;
    const double C447 = C349 * C436;
    const double C448 = C441 + C394;
    const double C449 = C442 + C396;
    const double C450 = C443 + C395;
    const double C451 = C437 / C364;
    const double C452 = C438 / C364;
    const double C453 = C439 / C364;
    const double C454 = std::log(C440);
    const double C455 = 0.3109069086965489e-1 * C447;
    const double C456 = C448 + C432;
    const double C457 = C449 + C431;
    const double C458 = C450 + C430;
    const double C459 = C453 + C435;
    const double C460 = 0.0310907 * C456;
    const double C461 = 0.0337738 * C457;
    const double C462 = 0.0621814 * C458;
    const double C463 = 2. * C459;
    const double C464 = C151 * C455;
    const double C465 = C101 * C461;
    const double C466 = C102 * C460;
    const double C467 = C103 * C462;
    const double C468 = C47 * C463;
    const double C469 = C464 / C366;
    const double C470 = C468 + C106;
    const double C471 = C465 / C369;
    const double C472 = C466 / C368;
    const double C473 = C467 / C370;
    const double C474 = C470 * C61;
    const double C475 = C444 - C472;
    const double C476 = C445 - C471;
    const double C477 = C446 - C473;
    const double C478 = -0.2668982024125968 * C474;
    const double C479 = 0.0310907 * C475;
    const double C480 = 0.0621814 * C477;
    const double C481 = C476 * C104;
    const double C482 = C155 * C478;
    const double C483 = C451 + C481;
    const double C484 = C480 - C479;
    const double C485 = 0.3109069086965489e-1 * C482;
    const double C486 = C483 * C69;
    const double C487 = C484 * C104;
    const double C488 = C452 + C487;
    const double C489 = C486 - C433;
    const double C490 = C485 / C410;
    const double C491 = 0.0337738 * C489;
    const double C492 = C64 * C488;
    const double C493 = C492 + C434;
    const double C494 = C491 / C66;
    const double C495 = C494 - C480;
    const double C496 = C493 / C65;
    const double C497 = C495 + C496;
    const double C498 = C110 * C497;
    const double C499 = 0.3109069086965489e-1 * C498;
    const double C500 = C499 - C469;
    const double C501 = C154 * C500;
    const double C502 = -0.3109069086965489e-1 * C501;
    const double C503 = C135 * C502;
    const double C504 = 0.06672455060314922 * C503;
    const double C505 = C504 / C350;
    const double C506 = C490 - C505;

    result.vrhoB =
        C47 * (C497 + 0.3109069086965489e-1 *
                          (C110 *
                               (0.3109069086965489e-1 * C393 * 0.06672455060314922 *
                                    (C135 * C506 / C355 + C345 * -4. * C474 / C410) -
                                0.06672455060314922 * C367 * 0.3109069086965489e-1 *
                                    (C506 / C355 +
                                     2 * C506 * C136 / (0.3109069086965489e-1 * C155 * C355))) /
                               (std::pow(C406, 2) * C440) +
                           C454 * C447 / C366)) +
        (C151 + 0.3109069086965489e-1 * C110 * C454);

    const double C508 = 2. * sigmaAb;
    const double C509 = rhoA + rhoB;
    const double C510 = rhoA - rhoB;
    const double C511 = -1. / 3.;
    const double C512 = 1. / 3.;
    const double C513 = 2. / 3.;
    const double C514 = 4. / 3.;
    const double C515 = std::pow(Pi, 2);
    const double C516 = 0.9999999999999999 * C510;
    const double C517 = C515 * C509;
    const double C518 = Pi * C509;
    const double C519 = sigmaAa + C508;
    const double C520 = C510 / C509;
    const double C521 = std::pow(2., C512);
    const double C522 = 3. * C517;
    const double C523 = 4. * C518;
    const double C524 = C519 + sigmaBb;
    const double C525 = C521 - 1.;
    const double C526 = C516 / C509;
    const double C527 = std::pow(C520, 4);
    const double C528 = 2. * C525;
    const double C529 = 3.4198418683227306 * C525;
    const double C530 = C526 + 1.;
    const double C531 = 1. - C526;
    const double C532 = 1. - C527;
    const double C533 = 3. / C523;
    const double C534 = std::sqrt(C524);
    const double C535 = std::pow(C522, C512);
    const double C536 = 0.1334491012062984 * C534;
    const double C537 = C535 / Pi;
    const double C538 = std::sqrt(C533);
    const double C539 = std::pow(C530, C512);
    const double C540 = std::pow(C530, C513);
    const double C541 = std::pow(C530, C514);
    const double C542 = std::pow(C531, C512);
    const double C543 = std::pow(C531, C513);
    const double C544 = std::pow(C531, C514);
    const double C545 = std::pow(C533, C512);
    const double C546 = std::pow(C533, C513);
    const double C547 = 0. * C539;
    const double C548 = 0. * C542;
    const double C549 = 0.11125 * C545;
    const double C550 = 0.20548 * C545;
    const double C551 = 0.2137 * C545;
    const double C552 = 0.49294 * C546;
    const double C553 = 0.49671 * C546;
    const double C554 = 0.62517 * C546;
    const double C555 = 0.88026 * C538;
    const double C556 = 1.6382 * C538;
    const double C557 = 3.3662 * C538;
    const double C558 = 3.5876 * C545;
    const double C559 = 3.6231 * C545;
    const double C560 = 6.1977 * C545;
    const double C561 = C540 + C543;
    const double C562 = C541 + C544;
    const double C563 = std::sqrt(C537);
    const double C564 = std::sqrt(C545);
    const double C565 = 7.5957 * C564;
    const double C566 = 10.357 * C564;
    const double C567 = 14.1189 * C564;
    const double C568 = C563 * C561;
    const double C569 = C547 + C548;
    const double C570 = C549 + 1.;
    const double C571 = C550 + 1.;
    const double C572 = C551 + 1.;
    const double C573 = C562 - 2.;
    const double C574 = C561 / 2.;
    const double C575 = 2. * C568;
    const double C576 = C565 + C558;
    const double C577 = C566 + C559;
    const double C578 = C567 + C560;
    const double C579 = std::pow(C574, 3);
    const double C580 = 0.3109069086965489e-1 * C579;
    const double C581 = C509 * C575;
    const double C582 = C576 + C556;
    const double C583 = C577 + C555;
    const double C584 = C578 + C557;
    const double C585 = C582 + C552;
    const double C586 = C583 + C553;
    const double C587 = C584 + C554;
    const double C588 = C581 / 2.;
    const double C589 = 0.0310907 * C587;
    const double C590 = 0.0337738 * C586;
    const double C591 = 0.0621814 * C585;
    const double C592 = C588 + 1e-16;
    const double C593 = 2. * C592;
    const double C594 = 1. / C589;
    const double C595 = 1. / C590;
    const double C596 = 1. / C591;
    const double C597 = C534 * C593;
    const double C598 = C594 + 1.;
    const double C599 = C595 + 1.;
    const double C600 = C596 + 1.;
    const double C601 = C534 / C593;
    const double C602 = 2 * C597;
    const double C603 = std::log(C598);
    const double C604 = std::log(C599);
    const double C605 = std::log(C600);
    const double C606 = std::pow(C601, 2);
    const double C607 = 0.06672455060314922 * C606;
    const double C608 = C570 * C604;
    const double C609 = C571 * C603;
    const double C610 = C572 * C605;
    const double C611 = C592 * C602;
    const double C612 = 0.0310907 * C609;
    const double C613 = 0.0621814 * C610;
    const double C614 = C608 * C573;
    const double C615 = C614 * C532;
    const double C616 = C613 - C612;
    const double C617 = 0.0337738 * C615;
    const double C618 = C616 * C573;
    const double C619 = C527 * C618;
    const double C620 = C617 / C529;
    const double C621 = C620 - C613;
    const double C622 = C619 / C528;
    const double C623 = C621 + C622;
    const double C624 = C623 / C580;
    const double C625 = -C624;
    const double C626 = std::exp(C625);
    const double C627 = C626 - 1.;
    const double C628 = 0.3109069086965489e-1 * C627;
    const double C629 = C611 * C628;
    const double C630 = C607 / C628;
    const double C631 = 2. * C629;
    const double C632 = C630 + 1.;
    const double C633 = std::pow(C630, 2);
    const double C634 = C606 * C632;
    const double C635 = C630 + C633;
    const double C636 = 0.06672455060314922 * C634;
    const double C637 = C635 + 1.;
    const double C638 = 0.3109069086965489e-1 * C637;
    const double C639 = C636 / C638;
    const double C640 = C639 + 1.;

    result.vsigmaAa =
        C509 * ((0.0337738 * C532 * C608 * C569 / (3. * C509 * C529) +
                 C527 * C616 * C569 / (3. * C509 * C528)) +
                0.3109069086965489e-1 *
                    (C579 *
                         (0.3109069086965489e-1 * C637 * 0.06672455060314922 *
                              (C606 * C536 / C631 + C632 * 2. * C534 / (2. * C611)) -
                          0.06672455060314922 * C634 * 0.3109069086965489e-1 *
                              (C536 / C631 + 0.2668982024125968 * C534 * C607 /
                                                 (0.3109069086965489e-1 * C627 * C631))) /
                         (std::pow(C638, 2) * C640) +
                     std::log(C640) * std::pow(C574, 2) * 3 *
                         (0. * std::pow(C530, C511) + 0. * std::pow(C531, C511)) / (6. * C509)));

    const double C642 = 2. * sigmaAb;
    const double C643 = rhoA + rhoB;
    const double C644 = rhoA - rhoB;
    const double C645 = -1. / 3.;
    const double C646 = 1. / 3.;
    const double C647 = 2. / 3.;
    const double C648 = 4. / 3.;
    const double C649 = std::pow(Pi, 2);
    const double C650 = 0.9999999999999999 * C644;
    const double C651 = C649 * C643;
    const double C652 = Pi * C643;
    const double C653 = sigmaAa + C642;
    const double C654 = C644 / C643;
    const double C655 = std::pow(2., C646);
    const double C656 = 3. * C651;
    const double C657 = 4. * C652;
    const double C658 = C653 + sigmaBb;
    const double C659 = C655 - 1.;
    const double C660 = C650 / C643;
    const double C661 = std::pow(C654, 4);
    const double C662 = 2. * C659;
    const double C663 = 3.4198418683227306 * C659;
    const double C664 = C660 + 1.;
    const double C665 = 1. - C660;
    const double C666 = 1. - C661;
    const double C667 = 3. / C657;
    const double C668 = std::sqrt(C658);
    const double C669 = std::pow(C656, C646);
    const double C670 = 0.2668982024125968 * C668;
    const double C671 = C669 / Pi;
    const double C672 = std::sqrt(C667);
    const double C673 = std::pow(C664, C646);
    const double C674 = std::pow(C664, C647);
    const double C675 = std::pow(C664, C648);
    const double C676 = std::pow(C665, C646);
    const double C677 = std::pow(C665, C647);
    const double C678 = std::pow(C665, C648);
    const double C679 = std::pow(C667, C646);
    const double C680 = std::pow(C667, C647);
    const double C681 = 0. * C673;
    const double C682 = 0. * C676;
    const double C683 = 0.11125 * C679;
    const double C684 = 0.20548 * C679;
    const double C685 = 0.2137 * C679;
    const double C686 = 0.49294 * C680;
    const double C687 = 0.49671 * C680;
    const double C688 = 0.62517 * C680;
    const double C689 = 0.88026 * C672;
    const double C690 = 1.6382 * C672;
    const double C691 = 3.3662 * C672;
    const double C692 = 3.5876 * C679;
    const double C693 = 3.6231 * C679;
    const double C694 = 6.1977 * C679;
    const double C695 = C674 + C677;
    const double C696 = C675 + C678;
    const double C697 = std::sqrt(C671);
    const double C698 = std::sqrt(C679);
    const double C699 = 7.5957 * C698;
    const double C700 = 10.357 * C698;
    const double C701 = 14.1189 * C698;
    const double C702 = C697 * C695;
    const double C703 = C681 + C682;
    const double C704 = C683 + 1.;
    const double C705 = C684 + 1.;
    const double C706 = C685 + 1.;
    const double C707 = C696 - 2.;
    const double C708 = C695 / 2.;
    const double C709 = 2. * C702;
    const double C710 = C699 + C692;
    const double C711 = C700 + C693;
    const double C712 = C701 + C694;
    const double C713 = std::pow(C708, 3);
    const double C714 = 0.3109069086965489e-1 * C713;
    const double C715 = C643 * C709;
    const double C716 = C710 + C690;
    const double C717 = C711 + C689;
    const double C718 = C712 + C691;
    const double C719 = C716 + C686;
    const double C720 = C717 + C687;
    const double C721 = C718 + C688;
    const double C722 = C715 / 2.;
    const double C723 = 0.0310907 * C721;
    const double C724 = 0.0337738 * C720;
    const double C725 = 0.0621814 * C719;
    const double C726 = C722 + 1e-16;
    const double C727 = 2. * C726;
    const double C728 = 1. / C723;
    const double C729 = 1. / C724;
    const double C730 = 1. / C725;
    const double C731 = C668 * C727;
    const double C732 = C728 + 1.;
    const double C733 = C729 + 1.;
    const double C734 = C730 + 1.;
    const double C735 = C668 / C727;
    const double C736 = 2 * C731;
    const double C737 = std::log(C732);
    const double C738 = std::log(C733);
    const double C739 = std::log(C734);
    const double C740 = std::pow(C735, 2);
    const double C741 = 0.06672455060314922 * C740;
    const double C742 = C704 * C738;
    const double C743 = C705 * C737;
    const double C744 = C706 * C739;
    const double C745 = C726 * C736;
    const double C746 = 0.0310907 * C743;
    const double C747 = 0.0621814 * C744;
    const double C748 = C742 * C707;
    const double C749 = C748 * C666;
    const double C750 = C747 - C746;
    const double C751 = 0.0337738 * C749;
    const double C752 = C750 * C707;
    const double C753 = C661 * C752;
    const double C754 = C751 / C663;
    const double C755 = C754 - C747;
    const double C756 = C753 / C662;
    const double C757 = C755 + C756;
    const double C758 = C757 / C714;
    const double C759 = -C758;
    const double C760 = std::exp(C759);
    const double C761 = C760 - 1.;
    const double C762 = 0.3109069086965489e-1 * C761;
    const double C763 = C745 * C762;
    const double C764 = C741 / C762;
    const double C765 = 2. * C763;
    const double C766 = C764 + 1.;
    const double C767 = std::pow(C764, 2);
    const double C768 = C740 * C766;
    const double C769 = C764 + C767;
    const double C770 = 0.06672455060314922 * C768;
    const double C771 = C769 + 1.;
    const double C772 = 0.3109069086965489e-1 * C771;
    const double C773 = C770 / C772;
    const double C774 = C773 + 1.;

    result.vsigmaAb =
        C643 * ((0.0337738 * C666 * C742 * C703 / (3. * C643 * C663) +
                 C661 * C750 * C703 / (3. * C643 * C662)) +
                0.3109069086965489e-1 *
                    (C713 *
                         (0.3109069086965489e-1 * C771 * 0.06672455060314922 *
                              (C740 * C670 / C765 + C766 * 4. * C668 / (2. * C745)) -
                          0.06672455060314922 * C768 * 0.3109069086965489e-1 *
                              (C670 / C765 + 0.5337964048251937 * C668 * C741 /
                                                 (0.3109069086965489e-1 * C761 * C765))) /
                         (std::pow(C772, 2) * C774) +
                     std::log(C774) * std::pow(C708, 2) * 3 *
                         (0. * std::pow(C664, C645) + 0. * std::pow(C665, C645)) / (6. * C643)));

    const double C776 = 2. * sigmaAb;
    const double C777 = rhoA + rhoB;
    const double C778 = rhoA - rhoB;
    const double C779 = -1. / 3.;
    const double C780 = 1. / 3.;
    const double C781 = 2. / 3.;
    const double C782 = 4. / 3.;
    const double C783 = std::pow(Pi, 2);
    const double C784 = 0.9999999999999999 * C778;
    const double C785 = C783 * C777;
    const double C786 = Pi * C777;
    const double C787 = sigmaAa + C776;
    const double C788 = C778 / C777;
    const double C789 = std::pow(2., C780);
    const double C790 = 3. * C785;
    const double C791 = 4. * C786;
    const double C792 = C787 + sigmaBb;
    const double C793 = C789 - 1.;
    const double C794 = C784 / C777;
    const double C795 = std::pow(C788, 4);
    const double C796 = 2. * C793;
    const double C797 = 3.4198418683227306 * C793;
    const double C798 = C794 + 1.;
    const double C799 = 1. - C794;
    const double C800 = 1. - C795;
    const double C801 = 3. / C791;
    const double C802 = std::sqrt(C792);
    const double C803 = std::pow(C790, C780);
    const double C804 = 0.1334491012062984 * C802;
    const double C805 = C803 / Pi;
    const double C806 = std::sqrt(C801);
    const double C807 = std::pow(C798, C780);
    const double C808 = std::pow(C798, C781);
    const double C809 = std::pow(C798, C782);
    const double C810 = std::pow(C799, C780);
    const double C811 = std::pow(C799, C781);
    const double C812 = std::pow(C799, C782);
    const double C813 = std::pow(C801, C780);
    const double C814 = std::pow(C801, C781);
    const double C815 = 0. * C807;
    const double C816 = 0. * C810;
    const double C817 = 0.11125 * C813;
    const double C818 = 0.20548 * C813;
    const double C819 = 0.2137 * C813;
    const double C820 = 0.49294 * C814;
    const double C821 = 0.49671 * C814;
    const double C822 = 0.62517 * C814;
    const double C823 = 0.88026 * C806;
    const double C824 = 1.6382 * C806;
    const double C825 = 3.3662 * C806;
    const double C826 = 3.5876 * C813;
    const double C827 = 3.6231 * C813;
    const double C828 = 6.1977 * C813;
    const double C829 = C808 + C811;
    const double C830 = C809 + C812;
    const double C831 = std::sqrt(C805);
    const double C832 = std::sqrt(C813);
    const double C833 = 7.5957 * C832;
    const double C834 = 10.357 * C832;
    const double C835 = 14.1189 * C832;
    const double C836 = C831 * C829;
    const double C837 = C815 + C816;
    const double C838 = C817 + 1.;
    const double C839 = C818 + 1.;
    const double C840 = C819 + 1.;
    const double C841 = C830 - 2.;
    const double C842 = C829 / 2.;
    const double C843 = 2. * C836;
    const double C844 = C833 + C826;
    const double C845 = C834 + C827;
    const double C846 = C835 + C828;
    const double C847 = std::pow(C842, 3);
    const double C848 = 0.3109069086965489e-1 * C847;
    const double C849 = C777 * C843;
    const double C850 = C844 + C824;
    const double C851 = C845 + C823;
    const double C852 = C846 + C825;
    const double C853 = C850 + C820;
    const double C854 = C851 + C821;
    const double C855 = C852 + C822;
    const double C856 = C849 / 2.;
    const double C857 = 0.0310907 * C855;
    const double C858 = 0.0337738 * C854;
    const double C859 = 0.0621814 * C853;
    const double C860 = C856 + 1e-16;
    const double C861 = 2. * C860;
    const double C862 = 1. / C857;
    const double C863 = 1. / C858;
    const double C864 = 1. / C859;
    const double C865 = C802 * C861;
    const double C866 = C862 + 1.;
    const double C867 = C863 + 1.;
    const double C868 = C864 + 1.;
    const double C869 = C802 / C861;
    const double C870 = 2 * C865;
    const double C871 = std::log(C866);
    const double C872 = std::log(C867);
    const double C873 = std::log(C868);
    const double C874 = std::pow(C869, 2);
    const double C875 = 0.06672455060314922 * C874;
    const double C876 = C838 * C872;
    const double C877 = C839 * C871;
    const double C878 = C840 * C873;
    const double C879 = C860 * C870;
    const double C880 = 0.0310907 * C877;
    const double C881 = 0.0621814 * C878;
    const double C882 = C876 * C841;
    const double C883 = C882 * C800;
    const double C884 = C881 - C880;
    const double C885 = 0.0337738 * C883;
    const double C886 = C884 * C841;
    const double C887 = C795 * C886;
    const double C888 = C885 / C797;
    const double C889 = C888 - C881;
    const double C890 = C887 / C796;
    const double C891 = C889 + C890;
    const double C892 = C891 / C848;
    const double C893 = -C892;
    const double C894 = std::exp(C893);
    const double C895 = C894 - 1.;
    const double C896 = 0.3109069086965489e-1 * C895;
    const double C897 = C879 * C896;
    const double C898 = C875 / C896;
    const double C899 = 2. * C897;
    const double C900 = C898 + 1.;
    const double C901 = std::pow(C898, 2);
    const double C902 = C874 * C900;
    const double C903 = C898 + C901;
    const double C904 = 0.06672455060314922 * C902;
    const double C905 = C903 + 1.;
    const double C906 = 0.3109069086965489e-1 * C905;
    const double C907 = C904 / C906;
    const double C908 = C907 + 1.;

    result.vsigmaBb =
        C777 * ((0.0337738 * C800 * C876 * C837 / (3. * C777 * C797) +
                 C795 * C884 * C837 / (3. * C777 * C796)) +
                0.3109069086965489e-1 *
                    (C847 *
                         (0.3109069086965489e-1 * C905 * 0.06672455060314922 *
                              (C874 * C804 / C899 + C900 * 2. * C802 / (2. * C879)) -
                          0.06672455060314922 * C902 * 0.3109069086965489e-1 *
                              (C804 / C899 + 0.2668982024125968 * C802 * C875 /
                                                 (0.3109069086965489e-1 * C895 * C899))) /
                         (std::pow(C906, 2) * C908) +
                     std::log(C908) * std::pow(C842, 2) * 3 *
                         (0. * std::pow(C798, C779) + 0. * std::pow(C799, C779)) / (6. * C777)));

    return result;
}

} // namespace excgrid
