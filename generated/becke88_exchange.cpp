// excgrid codegen source: the Becke 1988 GGA exchange functional.
// Fresh authorship; formula from the B88 paper (docs/mainpage.md
// references).  Regenerate with tools/regenerate.py.

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

XcKernelValue Becke88Exchange(
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

    const double C352 = 4. * Pi;
    const double C353 = rhoA + 1e-16;
    const double C354 = rhoB + 1e-16;
    const double C355 = 1. / 3.;
    const double C356 = 4. / 3.;
    const double C357 = std::sqrt(sigmaAa);
    const double C358 = std::sqrt(sigmaBb);
    const double C359 = 3. / C352;
    const double C360 = std::pow(C353, C356);
    const double C361 = std::pow(C354, C356);
    const double C362 = C357 / C360;
    const double C363 = C358 / C361;
    const double C364 = std::pow(C359, C355);
    const double C365 = 3. * C364;
    const double C366 = std::pow(C362, 2);
    const double C367 = std::pow(C363, 2);
    const double C368 = C365 / 2.;

    result.exc =
        -(std::pow(rhoB, C356) *
              (C368 + 0.0042 * C367 /
                          (std::log(C363 + std::sqrt(C367 + 1.)) * 0.0252 * C358 / C361 + 1.)) +
          std::pow(rhoA, C356) *
              (C368 + 0.0042 * C366 /
                          (std::log(C362 + std::sqrt(C366 + 1.)) * 0.0252 * C357 / C360 + 1.)));

    const double C373 = 0.0252 * C357;
    const double C374 = C366 + 1.;
    const double C375 = 8. / 3.;
    const double C376 = std::pow(C353, C355);
    const double C377 = 4. * C376;
    const double C378 = C376 * sigmaAa;
    const double C379 = std::sqrt(C374);
    const double C380 = std::pow(C353, C375);
    const double C381 = 3. * C380;
    const double C382 = C362 + C379;
    const double C383 = C360 * C381;
    const double C384 = std::log(C382);
    const double C385 = C384 * C373;
    const double C386 = C385 / C360;
    const double C387 = C386 + 1.;

    result.vrhoA = -(std::pow(rhoA, C356) *
                         (C387 * -0.0336 * C378 / C383 -
                          0.0042 * C366 *
                              (C360 * 0.0252 * C357 *
                                   (-8. * C378 / (C383 * 2 * C379) - C357 * C377 / C381) / C382 -
                               C385 * C377 / 3.) /
                              C380) /
                         std::pow(C387, 2) +
                     (C368 + 0.0042 * C366 / C387) * 4. * std::pow(rhoA, C355) / 3.);

    const double C389 = 0.0252 * C358;
    const double C390 = C367 + 1.;
    const double C391 = 8. / 3.;
    const double C392 = std::pow(C354, C355);
    const double C393 = 4. * C392;
    const double C394 = C392 * sigmaBb;
    const double C395 = std::sqrt(C390);
    const double C396 = std::pow(C354, C391);
    const double C397 = 3. * C396;
    const double C398 = C363 + C395;
    const double C399 = C361 * C397;
    const double C400 = std::log(C398);
    const double C401 = C400 * C389;
    const double C402 = C401 / C361;
    const double C403 = C402 + 1.;

    result.vrhoB = -(std::pow(rhoB, C356) *
                         (C403 * -0.0336 * C394 / C399 -
                          0.0042 * C367 *
                              (C361 * 0.0252 * C358 *
                                   (-8. * C394 / (C399 * 2 * C395) - C358 * C393 / C397) / C398 -
                               C401 * C393 / 3.) /
                              C396) /
                         std::pow(C403, 2) +
                     (C368 + 0.0042 * C367 / C403) * 4. * std::pow(rhoB, C355) / 3.);

    const double C405 = rhoA + 1e-16;
    const double C406 = sigmaAa + SigmaGuard;
    const double C407 = 4. / 3.;
    const double C408 = 8. / 3.;
    const double C409 = std::sqrt(C406);
    const double C410 = std::pow(C405, C407);
    const double C411 = std::pow(C405, C408);
    const double C412 = 0.0252 * C409;
    const double C413 = C409 * C411;
    const double C414 = C409 / C410;
    const double C415 = std::pow(C414, 2);
    const double C416 = C415 + 1.;
    const double C417 = std::sqrt(C416);
    const double C418 = C414 + C417;
    const double C419 = std::log(C418);
    const double C420 = C419 * C412;
    const double C421 = C420 / C410;
    const double C422 = C421 + 1.;

    result.vsigmaAa =
        -std::pow(rhoA, C407) *
        (C422 * 0.0042 * C409 / C413 -
         0.0042 * C415 *
             (0.0252 * C419 / (2 * C409) +
              0.0252 * C409 * (1 / (2 * C409 * C410) + C409 / (C413 * 2 * C417)) / C418) /
             C410) /
        std::pow(C422, 2);

    result.vsigmaAb = 0;

    const double C425 = rhoB + 1e-16;
    const double C426 = sigmaBb + SigmaGuard;
    const double C427 = 4. / 3.;
    const double C428 = 8. / 3.;
    const double C429 = std::sqrt(C426);
    const double C430 = std::pow(C425, C427);
    const double C431 = std::pow(C425, C428);
    const double C432 = 0.0252 * C429;
    const double C433 = C429 * C431;
    const double C434 = C429 / C430;
    const double C435 = std::pow(C434, 2);
    const double C436 = C435 + 1.;
    const double C437 = std::sqrt(C436);
    const double C438 = C434 + C437;
    const double C439 = std::log(C438);
    const double C440 = C439 * C432;
    const double C441 = C440 / C430;
    const double C442 = C441 + 1.;

    result.vsigmaBb =
        -std::pow(rhoB, C427) *
        (C442 * 0.0042 * C429 / C433 -
         0.0042 * C435 *
             (0.0252 * C439 / (2 * C429) +
              0.0252 * C429 * (1 / (2 * C429 * C430) + C429 / (C433 * 2 * C437)) / C438) /
             C430) /
        std::pow(C442, 2);

    return result;
}

void Becke88ExchangeSecondDerivatives(double rhoA,
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

    const double C318 = rhoB + 1e-16;
    const double C319 = 4. / 3.;
    const double C320 = 8. / 3.;
    const double C321 = std::sqrt(sigmaBb);
    const double C322 = 0.0042 * C321;
    const double C323 = 0.0252 * C321;
    const double C324 = 2 * C321;
    const double C325 = std::pow(C318, C319);
    const double C326 = std::pow(C318, C320);
    const double C327 = std::pow(rhoB, C319);
    const double C328 = C321 * C325;
    const double C329 = C321 * C326;
    const double C330 = C321 / C325;
    const double C331 = 2 * C328;
    const double C332 = std::pow(C330, 2);
    const double C333 = C332 + 1.;
    const double C334 = 1 / C331;
    const double C335 = std::sqrt(C333);
    const double C336 = 2 * C335;
    const double C337 = C330 + C335;
    const double C338 = C329 * C336;
    const double C339 = std::log(C337);
    const double C340 = 0.0252 * C339;
    const double C341 = C339 * C323;
    const double C342 = C321 / C338;
    const double C343 = C334 + C342;
    const double C344 = C340 / C324;
    const double C345 = C341 / C325;
    const double C346 = C321 * C343;
    const double C347 = C345 + 1.;
    const double C348 = 0.0252 * C346;
    const double C349 = C347 * C322;
    const double C350 = C348 / C337;
    const double C351 = C344 + C350;
    matrix.upper[14] =
        -(std::pow(C347, 2) * C327 *
              ((C329 * (0.0042 * C347 / C324 + 0.0042 * C321 * C351 / C325) - C349 * C326 / C324) /
                   std::pow(C329, 2) -
               0.0042 *
                   (C332 * ((2 * C321 * 0.0252 * C343 / C337 - C340 / C321) / std::pow(C324, 2) +
                            (C337 * 0.0252 *
                                 (C321 * ((C338 / C324 -
                                           C321 * (C329 * C324 / C338 + C335 * C326 / C321)) /
                                              std::pow(C338, 2) -
                                          C325 / (C321 * std::pow(C331, 2))) +
                                  C343 / C324) -
                             0.0252 * C321 * std::pow(C343, 2)) /
                                std::pow(C337, 2)) +
                    C351 * C321 / C329) /
                   C325) -
          C327 * (C349 / C329 - 0.0042 * C332 * C351 / C325) * C347 * 2 * C351 / C325) /
        std::pow(C347, 4);

    matrix.upper[13] = 0;

    matrix.upper[12] = 0;

    matrix.upper[11] = 0;

    matrix.upper[10] = 0;

    const double C284 = rhoA + 1e-16;
    const double C285 = 4. / 3.;
    const double C286 = 8. / 3.;
    const double C287 = std::sqrt(sigmaAa);
    const double C288 = 0.0042 * C287;
    const double C289 = 0.0252 * C287;
    const double C290 = 2 * C287;
    const double C291 = std::pow(C284, C285);
    const double C292 = std::pow(C284, C286);
    const double C293 = std::pow(rhoA, C285);
    const double C294 = C287 * C291;
    const double C295 = C287 * C292;
    const double C296 = C287 / C291;
    const double C297 = 2 * C294;
    const double C298 = std::pow(C296, 2);
    const double C299 = C298 + 1.;
    const double C300 = 1 / C297;
    const double C301 = std::sqrt(C299);
    const double C302 = 2 * C301;
    const double C303 = C296 + C301;
    const double C304 = C295 * C302;
    const double C305 = std::log(C303);
    const double C306 = 0.0252 * C305;
    const double C307 = C305 * C289;
    const double C308 = C287 / C304;
    const double C309 = C300 + C308;
    const double C310 = C306 / C290;
    const double C311 = C307 / C291;
    const double C312 = C287 * C309;
    const double C313 = C311 + 1.;
    const double C314 = 0.0252 * C312;
    const double C315 = C313 * C288;
    const double C316 = C314 / C303;
    const double C317 = C310 + C316;
    matrix.upper[9] =
        -(std::pow(C313, 2) * C293 *
              ((C295 * (0.0042 * C313 / C290 + 0.0042 * C287 * C317 / C291) - C315 * C292 / C290) /
                   std::pow(C295, 2) -
               0.0042 *
                   (C298 * ((2 * C287 * 0.0252 * C309 / C303 - C306 / C287) / std::pow(C290, 2) +
                            (C303 * 0.0252 *
                                 (C287 * ((C304 / C290 -
                                           C287 * (C295 * C290 / C304 + C301 * C292 / C287)) /
                                              std::pow(C304, 2) -
                                          C291 / (C287 * std::pow(C297, 2))) +
                                  C309 / C290) -
                             0.0252 * C287 * std::pow(C309, 2)) /
                                std::pow(C303, 2)) +
                    C317 * C287 / C295) /
                   C291) -
          C293 * (C315 / C295 - 0.0042 * C298 * C317 / C291) * C313 * 2 * C317 / C291) /
        std::pow(C313, 4);

    const double C222 = rhoB + 1e-16;
    const double C223 = 1. / 3.;
    const double C224 = 4. / 3.;
    const double C225 = 5. / 3.;
    const double C226 = 8. / 3.;
    const double C227 = std::sqrt(sigmaBb);
    const double C228 = 0.0042 * C227;
    const double C229 = 0.0252 * C227;
    const double C230 = 2 * C227;
    const double C231 = std::pow(C222, C223);
    const double C232 = std::pow(C222, C224);
    const double C233 = std::pow(C222, C225);
    const double C234 = std::pow(C222, C226);
    const double C235 = std::pow(rhoB, C224);
    const double C236 = 3. * C234;
    const double C237 = 4. * C231;
    const double C238 = 8. * C233;
    const double C239 = C227 * C232;
    const double C240 = C227 * C234;
    const double C241 = C231 * sigmaBb;
    const double C242 = C227 / C232;
    const double C243 = -8. * C241;
    const double C244 = 2 * C239;
    const double C245 = C227 * C237;
    const double C246 = C227 * C238;
    const double C247 = C232 * C236;
    const double C248 = std::pow(C242, 2);
    const double C249 = C248 + 1.;
    const double C250 = 1 / C244;
    const double C251 = C245 / C236;
    const double C252 = std::sqrt(C249);
    const double C253 = 2 * C252;
    const double C254 = C242 + C252;
    const double C255 = C240 * C253;
    const double C256 = C247 * C253;
    const double C257 = std::log(C254);
    const double C258 = 0.0252 * C257;
    const double C259 = C257 * C229;
    const double C260 = C227 / C255;
    const double C261 = C243 / C256;
    const double C262 = C259 * C237;
    const double C263 = C250 + C260;
    const double C264 = C261 - C251;
    const double C265 = C258 / C230;
    const double C266 = C259 / C232;
    const double C267 = C227 * C263;
    const double C268 = C227 * C264;
    const double C269 = C266 + 1.;
    const double C270 = C262 / 3.;
    const double C271 = 0.0252 * C267;
    const double C272 = 0.0252 * C268;
    const double C273 = C269 * C228;
    const double C274 = C232 * C272;
    const double C275 = C271 / C254;
    const double C276 = C273 / C240;
    const double C277 = C265 + C275;
    const double C278 = C274 / C254;
    const double C279 = C248 * C277;
    const double C280 = C278 - C270;
    const double C281 = 0.0042 * C279;
    const double C282 = C281 / C232;
    const double C283 = C276 - C282;
    matrix.upper[8] =
        -(std::pow(C269, 2) *
              (C235 * ((C240 * 0.0042 * C227 * C280 / C234 - C273 * C246 / 3.) / std::pow(C240, 2) -
                       (C232 * 0.0042 *
                            (C248 * (0.0252 * C264 / (C254 * C230) +
                                     (C254 * 0.0252 * C227 *
                                          ((-C227 *
                                            (C240 * -16. * C241 / C256 + 2 * C252 * C246 / 3.)) /
                                               std::pow(C255, 2) -
                                           2 * C245 / (3. * std::pow(C244, 2))) -
                                      0.0252 * C267 * C264) /
                                         std::pow(C254, 2)) +
                             C277 * C243 / C247) -
                        0.0042 * C279 * C237 / 3.) /
                           C234) +
               C283 * 4. * std::pow(rhoB, C223) / 3.) -
          C235 * C283 * C269 * 2 * C280 / C234) /
        std::pow(C269, 4);

    matrix.upper[7] = 0;

    matrix.upper[6] = 0;

    const double C165 = rhoB + 1e-16;
    const double C166 = -2. / 3.;
    const double C167 = 1. / 3.;
    const double C168 = 4. / 3.;
    const double C169 = 5. / 3.;
    const double C170 = 8. / 3.;
    const double C171 = std::sqrt(sigmaBb);
    const double C172 = 0.0252 * C171;
    const double C173 = std::pow(C165, C166);
    const double C174 = std::pow(C165, C167);
    const double C175 = std::pow(C165, C168);
    const double C176 = std::pow(C165, C169);
    const double C177 = std::pow(C165, C170);
    const double C178 = std::pow(rhoB, C167);
    const double C179 = std::pow(rhoB, C168);
    const double C180 = 3. * C177;
    const double C181 = 4. * C173;
    const double C182 = 4. * C174;
    const double C183 = 24. * C176;
    const double C184 = C174 * sigmaBb;
    const double C185 = sigmaBb * C173;
    const double C186 = C171 / C175;
    const double C187 = -8. * C184;
    const double C188 = -0.0336 * C184;
    const double C189 = C171 * C182;
    const double C190 = C175 * C180;
    const double C191 = C175 * C183;
    const double C192 = C177 * C182;
    const double C193 = std::pow(C186, 2);
    const double C194 = 3. * C192;
    const double C195 = C193 + 1.;
    const double C196 = C189 / C180;
    const double C197 = C191 + C194;
    const double C198 = std::sqrt(C195);
    const double C199 = 2 * C198;
    const double C200 = C186 + C198;
    const double C201 = C190 * C199;
    const double C202 = std::log(C200);
    const double C203 = C202 * C172;
    const double C204 = C187 / C201;
    const double C205 = C203 * C182;
    const double C206 = C204 - C196;
    const double C207 = C203 / C175;
    const double C208 = C171 * C206;
    const double C209 = C207 + 1.;
    const double C210 = C205 / 3.;
    const double C211 = 0.0252 * C208;
    const double C212 = C209 * C188;
    const double C213 = std::pow(C209, 2);
    const double C214 = C175 * C211;
    const double C215 = C212 / C190;
    const double C216 = C214 / C200;
    const double C217 = C216 - C210;
    const double C218 = C193 * C217;
    const double C219 = 0.0042 * C218;
    const double C220 = C219 / C177;
    const double C221 = C215 - C220;
    matrix.upper[5] =
        -((C213 *
               (C179 *
                    ((C190 * (C209 * -0.0336 * C185 / 3. + -0.0336 * C184 * C217 / C177) -
                      C212 * C197 / 3.) /
                         std::pow(C190, 2) -
                     (C177 * 0.0042 *
                          (C193 * ((C200 * (C175 * 0.0252 * C171 *
                                                ((C201 * -8. * C185 / 3. -
                                                  -8. * C184 *
                                                      (C190 * -16. * C184 / C201 +
                                                       2 * C198 * C197 / 3.)) /
                                                     std::pow(C201, 2) -
                                                 (3. * C177 * C171 * C181 / 3. - C189 * C183 / 3.) /
                                                     std::pow(C180, 2)) +
                                            0.0252 * C208 * C182 / 3.) -
                                    C214 * C206) /
                                       std::pow(C200, 2) -
                                   (C203 * C181 / 3. + 4. * C174 * C211 / C200) / 3.) +
                           C217 * C187 / C190) -
                      0.0042 * C218 * 8. * C176 / 3.) /
                         std::pow(C165, 16. / 3.)) +
                C221 * 4. * C178 / 3.) -
           C179 * C221 * C209 * 2 * C217 / C177) /
              std::pow(C209, 4) +
          ((3. * std::pow(3. / (4. * Pi), C167) / 2. + 0.0042 * C193 / C209) * 4. *
               std::pow(rhoB, C166) / 3. +
           4. * C178 * C221 / C213) /
              3.);

    matrix.upper[4] = 0;

    matrix.upper[3] = 0;

    const double C103 = rhoA + 1e-16;
    const double C104 = 1. / 3.;
    const double C105 = 4. / 3.;
    const double C106 = 5. / 3.;
    const double C107 = 8. / 3.;
    const double C108 = std::sqrt(sigmaAa);
    const double C109 = 0.0042 * C108;
    const double C110 = 0.0252 * C108;
    const double C111 = 2 * C108;
    const double C112 = std::pow(C103, C104);
    const double C113 = std::pow(C103, C105);
    const double C114 = std::pow(C103, C106);
    const double C115 = std::pow(C103, C107);
    const double C116 = std::pow(rhoA, C105);
    const double C117 = 3. * C115;
    const double C118 = 4. * C112;
    const double C119 = 8. * C114;
    const double C120 = C108 * C113;
    const double C121 = C108 * C115;
    const double C122 = C112 * sigmaAa;
    const double C123 = C108 / C113;
    const double C124 = -8. * C122;
    const double C125 = 2 * C120;
    const double C126 = C108 * C118;
    const double C127 = C108 * C119;
    const double C128 = C113 * C117;
    const double C129 = std::pow(C123, 2);
    const double C130 = C129 + 1.;
    const double C131 = 1 / C125;
    const double C132 = C126 / C117;
    const double C133 = std::sqrt(C130);
    const double C134 = 2 * C133;
    const double C135 = C123 + C133;
    const double C136 = C121 * C134;
    const double C137 = C128 * C134;
    const double C138 = std::log(C135);
    const double C139 = 0.0252 * C138;
    const double C140 = C138 * C110;
    const double C141 = C108 / C136;
    const double C142 = C124 / C137;
    const double C143 = C140 * C118;
    const double C144 = C131 + C141;
    const double C145 = C142 - C132;
    const double C146 = C139 / C111;
    const double C147 = C140 / C113;
    const double C148 = C108 * C144;
    const double C149 = C108 * C145;
    const double C150 = C147 + 1.;
    const double C151 = C143 / 3.;
    const double C152 = 0.0252 * C148;
    const double C153 = 0.0252 * C149;
    const double C154 = C150 * C109;
    const double C155 = C113 * C153;
    const double C156 = C152 / C135;
    const double C157 = C154 / C121;
    const double C158 = C146 + C156;
    const double C159 = C155 / C135;
    const double C160 = C129 * C158;
    const double C161 = C159 - C151;
    const double C162 = 0.0042 * C160;
    const double C163 = C162 / C113;
    const double C164 = C157 - C163;
    matrix.upper[2] =
        -(std::pow(C150, 2) *
              (C116 * ((C121 * 0.0042 * C108 * C161 / C115 - C154 * C127 / 3.) / std::pow(C121, 2) -
                       (C113 * 0.0042 *
                            (C129 * (0.0252 * C145 / (C135 * C111) +
                                     (C135 * 0.0252 * C108 *
                                          ((-C108 *
                                            (C121 * -16. * C122 / C137 + 2 * C133 * C127 / 3.)) /
                                               std::pow(C136, 2) -
                                           2 * C126 / (3. * std::pow(C125, 2))) -
                                      0.0252 * C148 * C145) /
                                         std::pow(C135, 2)) +
                             C158 * C124 / C128) -
                        0.0042 * C160 * C118 / 3.) /
                           C115) +
               C164 * 4. * std::pow(rhoA, C104) / 3.) -
          C116 * C164 * C150 * 2 * C161 / C115) /
        std::pow(C150, 4);

    matrix.upper[1] = 0;

    const double C46 = rhoA + 1e-16;
    const double C47 = -2. / 3.;
    const double C48 = 1. / 3.;
    const double C49 = 4. / 3.;
    const double C50 = 5. / 3.;
    const double C51 = 8. / 3.;
    const double C52 = std::sqrt(sigmaAa);
    const double C53 = 0.0252 * C52;
    const double C54 = std::pow(C46, C47);
    const double C55 = std::pow(C46, C48);
    const double C56 = std::pow(C46, C49);
    const double C57 = std::pow(C46, C50);
    const double C58 = std::pow(C46, C51);
    const double C59 = std::pow(rhoA, C48);
    const double C60 = std::pow(rhoA, C49);
    const double C61 = 3. * C58;
    const double C62 = 4. * C54;
    const double C63 = 4. * C55;
    const double C64 = 24. * C57;
    const double C65 = C55 * sigmaAa;
    const double C66 = sigmaAa * C54;
    const double C67 = C52 / C56;
    const double C68 = -8. * C65;
    const double C69 = -0.0336 * C65;
    const double C70 = C52 * C63;
    const double C71 = C56 * C61;
    const double C72 = C56 * C64;
    const double C73 = C58 * C63;
    const double C74 = std::pow(C67, 2);
    const double C75 = 3. * C73;
    const double C76 = C74 + 1.;
    const double C77 = C70 / C61;
    const double C78 = C72 + C75;
    const double C79 = std::sqrt(C76);
    const double C80 = 2 * C79;
    const double C81 = C67 + C79;
    const double C82 = C71 * C80;
    const double C83 = std::log(C81);
    const double C84 = C83 * C53;
    const double C85 = C68 / C82;
    const double C86 = C84 * C63;
    const double C87 = C85 - C77;
    const double C88 = C84 / C56;
    const double C89 = C52 * C87;
    const double C90 = C88 + 1.;
    const double C91 = C86 / 3.;
    const double C92 = 0.0252 * C89;
    const double C93 = C90 * C69;
    const double C94 = std::pow(C90, 2);
    const double C95 = C56 * C92;
    const double C96 = C93 / C71;
    const double C97 = C95 / C81;
    const double C98 = C97 - C91;
    const double C99 = C74 * C98;
    const double C100 = 0.0042 * C99;
    const double C101 = C100 / C58;
    const double C102 = C96 - C101;
    matrix.upper[0] = -(
        (C94 *
             (C60 *
                  ((C71 * (C90 * -0.0336 * C66 / 3. + -0.0336 * C65 * C98 / C58) - C93 * C78 / 3.) /
                       std::pow(C71, 2) -
                   (C58 * 0.0042 *
                        (C74 * ((C81 * (C56 * 0.0252 * C52 *
                                            ((C82 * -8. * C66 / 3. -
                                              -8. * C65 *
                                                  (C71 * -16. * C65 / C82 + 2 * C79 * C78 / 3.)) /
                                                 std::pow(C82, 2) -
                                             (3. * C58 * C52 * C62 / 3. - C70 * C64 / 3.) /
                                                 std::pow(C61, 2)) +
                                        0.0252 * C89 * C63 / 3.) -
                                 C95 * C87) /
                                    std::pow(C81, 2) -
                                (C84 * C62 / 3. + 4. * C55 * C92 / C81) / 3.) +
                         C98 * C68 / C71) -
                    0.0042 * C99 * 8. * C57 / 3.) /
                       std::pow(C46, 16. / 3.)) +
              C102 * 4. * C59 / 3.) -
         C60 * C102 * C90 * 2 * C98 / C58) /
            std::pow(C90, 4) +
        ((3. * std::pow(3. / (4. * Pi), C48) / 2. + 0.0042 * C74 / C90) * 4. * std::pow(rhoA, C47) /
             3. +
         4. * C59 * C102 / C94) /
            3.);
}

} // namespace excgrid
