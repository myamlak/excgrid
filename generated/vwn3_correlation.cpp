// excgrid codegen source: the VWN3 LDA correlation functional (the VWN
// RPA-parameter interpolation).  VWN3 and VWN5 are distinct functionals, never
// conflated.  Formula from VWN 1980 Table I.

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

// The excgrid LDA kernel skeleton (fresh authorship; BSD-3-Clause).  Emits one
// XcKernelValue function per LDA functional, with its spin-zero limit branches.
// It also emits the functional's SECOND-DERIVATIVE tier: one function filling
// the contract's materialised matrix, from the same expression as the kernel.
// The matrix is the contract's upper triangle, row-major over the two
// densities in identifier order, which is the LDA tier's whole span - an LDA
// energy density reads no other component, so the gradient and
// kinetic-energy-density rows and columns are not emitted at all rather than
// emitted as zeros.  The tier is taken from the guarded expression, not from
// the limit branches above: the Tiny guards already keep it finite at the spin
// edges, which is what makes it a straight-line form where the order-1 kernel
// is not.
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
        const double C265 = rhoA + rhoB;
        const double C266 = rhoA - rhoB;
        const double C267 = 1. / 3.;
        const double C268 = 4. / 3.;
        const double C269 = std::sqrt(0.00002016e2);
        const double C270 = std::sqrt(0.0137284639e2);
        const double C271 = 0.9999999999999999 * C266;
        const double C272 = Pi * C265;
        const double C273 = 4. * C272;
        const double C274 = C271 / C265;
        const double C275 = 3. / C273;
        const double C276 = std::pow(C275, C267);
        const double C277 = std::sqrt(C276);
        const double C278 = 2. * C277;
        const double C279 = 13.072 * C277;
        const double C280 = 20.1231 * C277;
        const double C281 = C277 + 0.409286;
        const double C282 = C276 + C279;
        const double C283 = C276 + C280;
        const double C284 = C278 + 13.072;
        const double C285 = C278 + 20.1231;
        const double C286 = std::pow(C281, 2);
        const double C287 = C282 + 42.7198;
        const double C288 = C283 + 101.578;
        const double C289 = C269 / C284;
        const double C290 = C270 / C285;
        const double C291 = C276 / C287;
        const double C292 = C286 / C287;
        const double C293 = std::atan(C289);
        const double C294 = std::atan(C290);
        const double C295 = 0.24506856e2 * C293;
        const double C296 = 0.26144e2 * C293;
        const double C297 = std::log(C291);
        const double C298 = std::log(C292);
        const double C299 = C295 / C269;
        const double C300 = C296 / C269;
        const double C301 = C297 + C300;
        const double C302 = C298 + C299;
        const double C303 = -0.5350186592e1 * C302;
        const double C304 = C303 / 37.537128437796;
        const double C305 = C301 - C304;
        const double C306 = 0.0310907 * C305;

        result.exc =
            C265 * (C306 + (0.01554535 * ((std::log(C276 / C288) + 0.402462e2 * C294 / C270) -
                                          -0.149573794914e2 *
                                              (std::log(std::pow(C277 + 0.743294, 2) / C288) +
                                               0.37273024e2 * C294 / C270) /
                                              0.87173106479036e2) -
                            C306) *
                               ((std::pow(C274 + 1., C268) + std::pow(1. - C274, C268)) - 2.) /
                               (2. * (std::pow(2., C267) - 1.)));
    } else if (rhoA < eps)
    {
        const double C308 = -0.9999999999999999 * rhoB;
        const double C309 = Pi * rhoB;
        const double C310 = 1. / 3.;
        const double C311 = 4. / 3.;
        const double C312 = std::sqrt(0.00002016e2);
        const double C313 = std::sqrt(0.0137284639e2);
        const double C314 = 4. * C309;
        const double C315 = C308 / rhoB;
        const double C316 = 3. / C314;
        const double C317 = std::pow(C316, C310);
        const double C318 = std::sqrt(C317);
        const double C319 = 2. * C318;
        const double C320 = 13.072 * C318;
        const double C321 = 20.1231 * C318;
        const double C322 = C318 + 0.409286;
        const double C323 = C317 + C320;
        const double C324 = C317 + C321;
        const double C325 = C319 + 13.072;
        const double C326 = C319 + 20.1231;
        const double C327 = std::pow(C322, 2);
        const double C328 = C323 + 42.7198;
        const double C329 = C324 + 101.578;
        const double C330 = C312 / C325;
        const double C331 = C313 / C326;
        const double C332 = C317 / C328;
        const double C333 = C327 / C328;
        const double C334 = std::atan(C330);
        const double C335 = std::atan(C331);
        const double C336 = 0.24506856e2 * C334;
        const double C337 = 0.26144e2 * C334;
        const double C338 = std::log(C332);
        const double C339 = std::log(C333);
        const double C340 = C336 / C312;
        const double C341 = C337 / C312;
        const double C342 = C338 + C341;
        const double C343 = C339 + C340;
        const double C344 = -0.5350186592e1 * C343;
        const double C345 = C344 / 37.537128437796;
        const double C346 = C342 - C345;
        const double C347 = 0.0310907 * C346;

        result.exc =
            rhoB * (C347 + (0.01554535 * ((std::log(C317 / C329) + 0.402462e2 * C335 / C313) -
                                          -0.149573794914e2 *
                                              (std::log(std::pow(C318 + 0.743294, 2) / C329) +
                                               0.37273024e2 * C335 / C313) /
                                              0.87173106479036e2) -
                            C347) *
                               ((std::pow(C315 + 1., C311) + std::pow(1. - C315, C311)) - 2.) /
                               (2. * (std::pow(2., C310) - 1.)));
    } else
    {
        const double C349 = 0.9999999999999999 * rhoA;
        const double C350 = Pi * rhoA;
        const double C351 = 1. / 3.;
        const double C352 = 4. / 3.;
        const double C353 = std::sqrt(0.00002016e2);
        const double C354 = std::sqrt(0.0137284639e2);
        const double C355 = 4. * C350;
        const double C356 = C349 / rhoA;
        const double C357 = 3. / C355;
        const double C358 = std::pow(C357, C351);
        const double C359 = std::sqrt(C358);
        const double C360 = 2. * C359;
        const double C361 = 13.072 * C359;
        const double C362 = 20.1231 * C359;
        const double C363 = C359 + 0.409286;
        const double C364 = C358 + C361;
        const double C365 = C358 + C362;
        const double C366 = C360 + 13.072;
        const double C367 = C360 + 20.1231;
        const double C368 = std::pow(C363, 2);
        const double C369 = C364 + 42.7198;
        const double C370 = C365 + 101.578;
        const double C371 = C353 / C366;
        const double C372 = C354 / C367;
        const double C373 = C358 / C369;
        const double C374 = C368 / C369;
        const double C375 = std::atan(C371);
        const double C376 = std::atan(C372);
        const double C377 = 0.24506856e2 * C375;
        const double C378 = 0.26144e2 * C375;
        const double C379 = std::log(C373);
        const double C380 = std::log(C374);
        const double C381 = C377 / C353;
        const double C382 = C378 / C353;
        const double C383 = C379 + C382;
        const double C384 = C380 + C381;
        const double C385 = -0.5350186592e1 * C384;
        const double C386 = C385 / 37.537128437796;
        const double C387 = C383 - C386;
        const double C388 = 0.0310907 * C387;

        result.exc =
            rhoA * (C388 + (0.01554535 * ((std::log(C358 / C370) + 0.402462e2 * C376 / C354) -
                                          -0.149573794914e2 *
                                              (std::log(std::pow(C359 + 0.743294, 2) / C370) +
                                               0.37273024e2 * C376 / C354) /
                                              0.87173106479036e2) -
                            C388) *
                               ((std::pow(C356 + 1., C352) + std::pow(1. - C356, C352)) - 2.) /
                               (2. * (std::pow(2., C351) - 1.)));
    }

    if (rhoA >= eps)
    {
        const double C353 = std::sqrt(0.00002016e2);
        const double C354 = std::sqrt(0.0137284639e2);
        const double C265 = rhoA + rhoB;
        const double C266 = rhoA - rhoB;
        const double C267 = 1. / 3.;
        const double C268 = 4. / 3.;
        const double C271 = 0.9999999999999999 * C266;
        const double C272 = Pi * C265;
        const double C273 = 4. * C272;
        const double C274 = C271 / C265;
        const double C275 = 3. / C273;
        const double C276 = std::pow(C275, C267);
        const double C278 = 2. * std::sqrt(C276);
        const double C279 = 13.072 * std::sqrt(C276);
        const double C280 = 20.1231 * std::sqrt(C276);
        const double C281 = std::sqrt(C276) + 0.409286;
        const double C282 = C276 + C279;
        const double C283 = C276 + C280;
        const double C284 = C278 + 13.072;
        const double C285 = C278 + 20.1231;
        const double C286 = std::pow(C281, 2);
        const double C287 = C282 + 42.7198;
        const double C288 = C283 + 101.578;
        const double C289 = std::sqrt(0.00002016e2) / C284;
        const double C290 = std::sqrt(0.0137284639e2) / C285;
        const double C291 = C276 / C287;
        const double C292 = C286 / C287;
        const double C293 = std::atan(C289);
        const double C294 = std::atan(C290);
        const double C295 = 0.24506856e2 * C293;
        const double C296 = 0.26144e2 * C293;
        const double C297 = std::log(C291);
        const double C298 = std::log(C292);
        const double C299 = C295 / std::sqrt(0.00002016e2);
        const double C300 = C296 / std::sqrt(0.00002016e2);
        const double C301 = C297 + C300;
        const double C302 = C298 + C299;
        const double C303 = -0.5350186592e1 * C302;
        const double C304 = C303 / 37.537128437796;
        const double C305 = C301 - C304;
        const double C306 = 0.0310907 * C305;
        const double C390 = 1;
        const double C391 = 1;
        const double C392 = 0.9999999999999999 * C391 + 0.;
        const double C393 = Pi * C390;
        const double C394 = 4. * C393;
        const double C395 = C392 / C265 - C390 * C271 / std::pow(C265, 2);
        const double C396 = -3. * C394 / std::pow(C273, 2);
        const double C397 = C267 * std::pow(C275, C267 - 1) * C396;
        const double C398 = 2. * C397 / (2 * std::sqrt(C276));
        const double C399 = 0.13072e2 * C397 / (2 * std::sqrt(C276));
        const double C400 = 0.201231e2 * C397 / (2 * std::sqrt(C276));
        const double C401 = C397 / (2 * std::sqrt(C276));
        const double C402 = C397 + C399;
        const double C403 = C397 + C400;
        const double C404 = C398;
        const double C405 = C398;
        const double C406 = 2 * C281 * C401;
        const double C407 = C402;
        const double C408 = C403;
        const double C409 = (-C404 * C353) / std::pow(C284, 2);
        const double C410 = (-C405 * C354) / std::pow(C285, 2);
        const double C411 = C397 / C287 - C407 * C276 / std::pow(C287, 2);
        const double C412 = C406 / C287 - C407 * C286 / std::pow(C287, 2);
        const double C413 = C409 / (std::pow(C289, 2) + 1);
        const double C414 = C410 / (std::pow(C290, 2) + 1);
        const double C415 = 0.24506856e2 * C413;
        const double C416 = 0.26144e2 * C413;
        const double C417 = C411 / C291;
        const double C418 = C412 / C292;
        const double C419 = std::sqrt(1 / 0.00002016e2) * C415;
        const double C420 = std::sqrt(1 / 0.00002016e2) * C416;
        const double C421 = C417 + C420;
        const double C422 = C418 + C419;
        const double C423 = -5.350186592 * C422;
        const double C424 = C423 / 37.537128437796;
        const double C425 = C421 - C424;
        const double C426 = 0.0310907 * C425;

        result.vrhoA =
            (((((C306 +
                 (0.01554535 * ((std::log(C276 / C288) + 0.402462e2 * C294 / C354) -
                                -0.149573794914e2 *
                                    (std::log(std::pow(std::sqrt(C276) + 0.743294, 2) / C288) +
                                     0.37273024e2 * C294 / C354) /
                                    0.87173106479036e2) -
                  C306) *
                     ((std::pow(C274 + 1., C268) + std::pow(1. - C274, C268)) - 2.) /
                     (2. * (std::pow(2., C267) - 1.))) *
                    C390 +
                C265 *
                    ((2. - (std::pow(C274 + 1., C268) + std::pow(1. - C274, C268))) /
                         (2. * (std::pow(2., C267) - 1.)) +
                     1) *
                    C426) +
               C397 * C265 * 0.01554535 *
                   (C288 / (C288 * C276) - -0.149573794914e2 * (std::sqrt(C276) + 0.743294) * C288 /
                                               (0.87173106479036e2 * std::sqrt(C276) * C288 *
                                                std::pow(std::sqrt(C276) + 0.743294, 2))) *
                   ((std::pow(C274 + 1., C268) + std::pow(1. - C274, C268)) - 2.) /
                   (2. * (std::pow(2., C267) - 1.))) +
              C408 * C265 * 0.01554535 *
                  ((-C276 * C288) / (std::pow(C288, 2) * C276) -
                   0.149573794914e2 * std::pow(std::sqrt(C276) + 0.743294, 2) * C288 /
                       (0.87173106479036e2 * std::pow(C288, 2) *
                        std::pow(std::sqrt(C276) + 0.743294, 2))) *
                  ((std::pow(C274 + 1., C268) + std::pow(1. - C274, C268)) - 2.) /
                  (2. * (std::pow(2., C267) - 1.))) +
             C414 * C265 * 0.01554535 *
                 (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                  (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                 ((std::pow(C274 + 1., C268) + std::pow(1. - C274, C268)) - 2.) /
                 (2. * (std::pow(2., C267) - 1.))) +
            C395 * C265 *
                (0.01554535 * ((std::log(C276 / C288) + 0.402462e2 * C294 / C354) -
                               -0.149573794914e2 *
                                   (std::log(std::pow(std::sqrt(C276) + 0.743294, 2) / C288) +
                                    0.37273024e2 * C294 / C354) /
                                   0.87173106479036e2) -
                 C306) *
                (C268 * std::pow(C274 + 1., C268 - 1) - C268 * std::pow(1. - C274, C268 - 1)) /
                (2. * (std::pow(2., C267) - 1.));
    }

    if (rhoB >= eps)
    {
        const double C312 = std::sqrt(0.00002016e2);
        const double C313 = std::sqrt(0.0137284639e2);
        const double C265 = rhoA + rhoB;
        const double C266 = rhoA - rhoB;
        const double C267 = 1. / 3.;
        const double C268 = 4. / 3.;
        const double C271 = 0.9999999999999999 * C266;
        const double C272 = Pi * C265;
        const double C273 = 4. * C272;
        const double C274 = C271 / C265;
        const double C275 = 3. / C273;
        const double C276 = std::pow(C275, C267);
        const double C278 = 2. * std::sqrt(C276);
        const double C279 = 13.072 * std::sqrt(C276);
        const double C280 = 20.1231 * std::sqrt(C276);
        const double C281 = std::sqrt(C276) + 0.409286;
        const double C282 = C276 + C279;
        const double C283 = C276 + C280;
        const double C284 = C278 + 13.072;
        const double C285 = C278 + 20.1231;
        const double C286 = std::pow(C281, 2);
        const double C287 = C282 + 42.7198;
        const double C288 = C283 + 101.578;
        const double C289 = std::sqrt(0.00002016e2) / C284;
        const double C290 = std::sqrt(0.0137284639e2) / C285;
        const double C291 = C276 / C287;
        const double C292 = C286 / C287;
        const double C293 = std::atan(C289);
        const double C294 = std::atan(C290);
        const double C295 = 0.24506856e2 * C293;
        const double C296 = 0.26144e2 * C293;
        const double C297 = std::log(C291);
        const double C298 = std::log(C292);
        const double C299 = C295 / std::sqrt(0.00002016e2);
        const double C300 = C296 / std::sqrt(0.00002016e2);
        const double C301 = C297 + C300;
        const double C302 = C298 + C299;
        const double C303 = -0.5350186592e1 * C302;
        const double C304 = C303 / 37.537128437796;
        const double C305 = C301 - C304;
        const double C306 = 0.0310907 * C305;
        const double C427 = 1;
        const double C428 = -1;
        const double C429 = 0.9999999999999999 * C428 + 0.;
        const double C430 = Pi * C427;
        const double C431 = 4. * C430;
        const double C432 = C429 / C265 - C427 * C271 / std::pow(C265, 2);
        const double C433 = -3. * C431 / std::pow(C273, 2);
        const double C434 = C267 * std::pow(C275, C267 - 1) * C433;
        const double C435 = 2. * C434 / (2 * std::sqrt(C276));
        const double C436 = 0.13072e2 * C434 / (2 * std::sqrt(C276));
        const double C437 = 0.201231e2 * C434 / (2 * std::sqrt(C276));
        const double C438 = C434 / (2 * std::sqrt(C276));
        const double C439 = C434 + C436;
        const double C440 = C434 + C437;
        const double C441 = C435;
        const double C442 = C435;
        const double C443 = 2 * C281 * C438;
        const double C444 = C439;
        const double C445 = C440;
        const double C446 = (-C441 * C312) / std::pow(C284, 2);
        const double C447 = (-C442 * C313) / std::pow(C285, 2);
        const double C448 = C434 / C287 - C444 * C276 / std::pow(C287, 2);
        const double C449 = C443 / C287 - C444 * C286 / std::pow(C287, 2);
        const double C450 = C446 / (std::pow(C289, 2) + 1);
        const double C451 = C447 / (std::pow(C290, 2) + 1);
        const double C452 = 0.24506856e2 * C450;
        const double C453 = 0.26144e2 * C450;
        const double C454 = C448 / C291;
        const double C455 = C449 / C292;
        const double C456 = std::sqrt(1 / 0.00002016e2) * C452;
        const double C457 = std::sqrt(1 / 0.00002016e2) * C453;
        const double C458 = C454 + C457;
        const double C459 = C455 + C456;
        const double C460 = -5.350186592 * C459;
        const double C461 = C460 / 37.537128437796;
        const double C462 = C458 - C461;
        const double C463 = 0.0310907 * C462;

        result.vrhoB =
            (((((C306 +
                 (0.01554535 * ((std::log(C276 / C288) + 0.402462e2 * C294 / C313) -
                                -0.149573794914e2 *
                                    (std::log(std::pow(std::sqrt(C276) + 0.743294, 2) / C288) +
                                     0.37273024e2 * C294 / C313) /
                                    0.87173106479036e2) -
                  C306) *
                     ((std::pow(C274 + 1., C268) + std::pow(1. - C274, C268)) - 2.) /
                     (2. * (std::pow(2., C267) - 1.))) *
                    C427 +
                C265 *
                    ((2. - (std::pow(C274 + 1., C268) + std::pow(1. - C274, C268))) /
                         (2. * (std::pow(2., C267) - 1.)) +
                     1) *
                    C463) +
               C434 * C265 * 0.01554535 *
                   (C288 / (C288 * C276) - -0.149573794914e2 * (std::sqrt(C276) + 0.743294) * C288 /
                                               (0.87173106479036e2 * std::sqrt(C276) * C288 *
                                                std::pow(std::sqrt(C276) + 0.743294, 2))) *
                   ((std::pow(C274 + 1., C268) + std::pow(1. - C274, C268)) - 2.) /
                   (2. * (std::pow(2., C267) - 1.))) +
              C445 * C265 * 0.01554535 *
                  ((-C276 * C288) / (std::pow(C288, 2) * C276) -
                   0.149573794914e2 * std::pow(std::sqrt(C276) + 0.743294, 2) * C288 /
                       (0.87173106479036e2 * std::pow(C288, 2) *
                        std::pow(std::sqrt(C276) + 0.743294, 2))) *
                  ((std::pow(C274 + 1., C268) + std::pow(1. - C274, C268)) - 2.) /
                  (2. * (std::pow(2., C267) - 1.))) +
             C451 * C265 * 0.01554535 *
                 (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                  (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                 ((std::pow(C274 + 1., C268) + std::pow(1. - C274, C268)) - 2.) /
                 (2. * (std::pow(2., C267) - 1.))) +
            C432 * C265 *
                (0.01554535 * ((std::log(C276 / C288) + 0.402462e2 * C294 / C313) -
                               -0.149573794914e2 *
                                   (std::log(std::pow(std::sqrt(C276) + 0.743294, 2) / C288) +
                                    0.37273024e2 * C294 / C313) /
                                   0.87173106479036e2) -
                 C306) *
                (C268 * std::pow(C274 + 1., C268 - 1) - C268 * std::pow(1. - C274, C268 - 1)) /
                (2. * (std::pow(2., C267) - 1.));
    }

    return result;
}

void Vwn3CorrelationSecondDerivatives(double rhoA,
                                      double rhoB,
                                      PointSecondDerivativeMatrix& matrix) {
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    if (rhoA + rhoB < 2. * std::numeric_limits<double>::epsilon())
    {
        return;
    }

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
    const double C92 = 1;
    const double C93 = 1;
    const double C94 = 0.9999999999999999 * C93 + 0.;
    const double C95 = Pi * C92;
    const double C96 = 4. * C95;
    const double C97 = C94 / C46 - C92 * C52 / std::pow(C46, 2);
    const double C98 = -3. * C96 / std::pow(C54, 2);
    const double C99 = C48 * std::pow(C56, C48 - 1) * C98;
    const double C100 = 2. * C99 / (2 * std::sqrt(C57));
    const double C101 = 0.13072e2 * C99 / (2 * std::sqrt(C57));
    const double C102 = 0.201231e2 * C99 / (2 * std::sqrt(C57));
    const double C103 = C99 / (2 * std::sqrt(C57));
    const double C104 = C99 + C101;
    const double C105 = C99 + C102;
    const double C106 = C100;
    const double C107 = C100;
    const double C108 = 2 * C62 * C103;
    const double C109 = C104;
    const double C110 = C105;
    const double C111 = (-C106 * std::sqrt(0.00002016e2)) / std::pow(C65, 2);
    const double C112 = (-C107 * std::sqrt(0.0137284639e2)) / std::pow(C66, 2);
    const double C113 = C99 / C68 - C109 * C57 / std::pow(C68, 2);
    const double C114 = C108 / C68 - C109 * C67 / std::pow(C68, 2);
    const double C115 = C111 / (std::pow(C70, 2) + 1);
    const double C116 = C112 / (std::pow(C71, 2) + 1);
    const double C117 = 0.24506856e2 * C115;
    const double C118 = 0.26144e2 * C115;
    const double C119 = C113 / C72;
    const double C120 = C114 / C73;
    const double C121 = std::sqrt(1 / 0.00002016e2) * C117;
    const double C122 = std::sqrt(1 / 0.00002016e2) * C118;
    const double C123 = C119 + C122;
    const double C124 = C120 + C121;
    const double C125 = -5.350186592 * C124;
    const double C126 = C125 / 37.537128437796;
    const double C127 = C123 - C126;
    const double C128 = 0.0310907 * C127;
    const double C166 = 0.;
    const double C167 =
        (C166 / C46 +
         ((-C94) / std::pow(C46, 2) - (-C92 * C52 * 2 * C46) / std::pow(C46, 4)) * C92) -
        C94 * C92 / std::pow(C46, 2);
    const double C168 = (-C96 * -3. * C96 * 2 * C54) / std::pow(C54, 4);
    const double C169 = C48 * (C48 - 1) * std::pow(C56, C48 - 2) * std::pow(C98, 2) +
                        C48 * std::pow(C56, C48 - 1) * C168;
    const double C170 = 2. * C169 / (2 * std::sqrt(C57)) -
                        2. * std::pow(C99, 2) / (std::sqrt(C57) * std::pow(2 * std::sqrt(C57), 2));
    const double C171 =
        0.13072e2 * C169 / (2 * std::sqrt(C57)) -
        0.13072e2 * std::pow(C99, 2) / (std::sqrt(C57) * std::pow(2 * std::sqrt(C57), 2));
    const double C172 =
        0.201231e2 * C169 / (2 * std::sqrt(C57)) -
        0.201231e2 * std::pow(C99, 2) / (std::sqrt(C57) * std::pow(2 * std::sqrt(C57), 2));
    const double C173 = C169 / (2 * std::sqrt(C57)) -
                        std::pow(C99, 2) / (std::sqrt(C57) * std::pow(2 * std::sqrt(C57), 2));
    const double C174 = C169 + C171;
    const double C175 = C169 + C172;
    const double C176 = C170;
    const double C177 = C170;
    const double C178 = 2 * std::pow(C103, 2) + 2 * C62 * C173;
    const double C179 = C174;
    const double C180 = C175;
    const double C181 = C106 * C106 * std::sqrt(0.00002016e2) * 2 * C65 / std::pow(C65, 4) -
                        C176 * std::sqrt(0.00002016e2) / std::pow(C65, 2);
    const double C182 = C107 * C107 * std::sqrt(0.0137284639e2) * 2 * C66 / std::pow(C66, 4) -
                        C177 * std::sqrt(0.0137284639e2) / std::pow(C66, 2);
    const double C183 =
        ((C169 / C68 +
          ((-C99) / std::pow(C68, 2) - (-C109 * C57 * 2 * C68) / std::pow(C68, 4)) * C109) -
         C179 * C57 / std::pow(C68, 2)) -
        C99 * C109 / std::pow(C68, 2);
    const double C184 =
        ((C178 / C68 +
          ((-C108) / std::pow(C68, 2) - (-C109 * C67 * 2 * C68) / std::pow(C68, 4)) * C109) -
         C179 * C67 / std::pow(C68, 2)) -
        C108 * C109 / std::pow(C68, 2);
    const double C185 = C181 / (std::pow(C70, 2) + 1) -
                        2 * C70 * std::pow(C111, 2) / std::pow(std::pow(C70, 2) + 1, 2);
    const double C186 = C182 / (std::pow(C71, 2) + 1) -
                        2 * C71 * std::pow(C112, 2) / std::pow(std::pow(C71, 2) + 1, 2);
    const double C187 = 0.24506856e2 * C185;
    const double C188 = 0.26144e2 * C185;
    const double C189 = C183 / C72 - std::pow(C113, 2) / std::pow(C72, 2);
    const double C190 = C184 / C73 - std::pow(C114, 2) / std::pow(C73, 2);
    const double C191 = std::sqrt(1 / 0.00002016e2) * C187;
    const double C192 = std::sqrt(1 / 0.00002016e2) * C188;
    const double C193 = C189 + C192;
    const double C194 = C190 + C191;
    const double C195 = -5.350186592 * C194;
    const double C196 = C195 / 37.537128437796;
    const double C197 = C193 - C196;
    const double C198 = 0.0310907 * C197;
    matrix.upper[0] =
        ((((((((((((2. - (std::pow(C55 + 1., C49) + std::pow(1. - C55, C49))) /
                       (2. * (std::pow(2., C48) - 1.)) +
                   1) *
                      C92 +
                  C97 * C46 *
                      (C49 * std::pow(1. - C55, C49 - 1) - C49 * std::pow(C55 + 1., C49 - 1)) /
                      (2. * (std::pow(2., C48) - 1.))) *
                     C128 +
                 C99 *
                     (((C92 * 0.01554535 *
                            (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) *
                                                     C69 /
                                                     (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                                      std::pow(std::sqrt(C57) + 0.743294, 2))) *
                            ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) +
                        C99 * C46 * 0.01554535 *
                            ((-std::pow(C69, 2)) / std::pow(C69 * C57, 2) -
                             (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                  std::pow(std::sqrt(C57) + 0.743294, 2) * -0.149573794914e2 * C69 /
                                  (2 * std::sqrt(C57)) -
                              -0.149573794914e2 * (std::sqrt(C57) + 0.743294) * C69 *
                                  0.87173106479036e2 *
                                  (std::sqrt(C57) * C69 * (std::sqrt(C57) + 0.743294) /
                                       std::sqrt(C57) +
                                   std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                                       (2 * std::sqrt(C57)))) /
                                 std::pow(0.87173106479036e2 * std::sqrt(C57) * C69 *
                                              std::pow(std::sqrt(C57) + 0.743294, 2),
                                          2)) *
                            ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.)) +
                       C110 * C46 * 0.01554535 *
                           ((C57 * std::pow(C69, 3) - std::pow(C69, 2) * C57 * C69) /
                                std::pow(std::pow(C69, 2) * C57, 2) -
                            (0.87173106479036e2 * std::pow(C69, 2) *
                                 std::pow(std::sqrt(C57) + 0.743294, 2) * 0.149573794914e2 * C69 *
                                 (std::sqrt(C57) + 0.743294) / std::sqrt(C57) -
                             0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 *
                                 0.87173106479036e2 * std::pow(C69, 2) *
                                 (std::sqrt(C57) + 0.743294) / std::sqrt(C57)) /
                                std::pow(0.87173106479036e2 * std::pow(C69, 2) *
                                             std::pow(std::sqrt(C57) + 0.743294, 2),
                                         2)) *
                           ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.)) +
                      C97 * C46 * 0.01554535 *
                          (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) *
                                                   C69 /
                                                   (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                                    std::pow(std::sqrt(C57) + 0.743294, 2))) *
                          (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) /
                     (2. * (std::pow(2., C48) - 1.))) +
                (((C92 * 0.01554535 *
                       ((-C57 * C69) / (std::pow(C69, 2) * C57) -
                        0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                            (0.87173106479036e2 * std::pow(C69, 2) *
                             std::pow(std::sqrt(C57) + 0.743294, 2))) *
                       ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                       (2. * (std::pow(2., C48) - 1.)) +
                   C99 * C46 * -0.01554535 *
                       ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) *
                       (0.87173106479036e2 * std::sqrt(C57) * C69 *
                            std::pow(std::sqrt(C57) + 0.743294, 2) * -0.149573794914e2 *
                            (std::sqrt(C57) + 0.743294) -
                        -0.149573794914e2 * (std::sqrt(C57) + 0.743294) * C69 * 0.87173106479036e2 *
                            std::sqrt(C57) * std::pow(std::sqrt(C57) + 0.743294, 2)) /
                       (std::pow(0.87173106479036e2 * std::sqrt(C57) * C69 *
                                     std::pow(std::sqrt(C57) + 0.743294, 2),
                                 2) *
                        2. * (std::pow(2., C48) - 1.))) +
                  C110 * C46 * 0.01554535 *
                      ((C57 * C69 * 2 * C69 * C57 - std::pow(C69, 2) * std::pow(C57, 2)) /
                           std::pow(std::pow(C69, 2) * C57, 2) -
                       (0.87173106479036e2 * std::pow(C69, 2) *
                            std::pow(std::sqrt(C57) + 0.743294, 2) * 0.149573794914e2 *
                            std::pow(std::sqrt(C57) + 0.743294, 2) -
                        0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 *
                            0.174346212958072e3 * C69 * std::pow(std::sqrt(C57) + 0.743294, 2)) /
                           std::pow(0.87173106479036e2 * std::pow(C69, 2) *
                                        std::pow(std::sqrt(C57) + 0.743294, 2),
                                    2)) *
                      ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                      (2. * (std::pow(2., C48) - 1.))) +
                 C97 * C46 * 0.01554535 *
                     ((-C57 * C69) / (std::pow(C69, 2) * C57) -
                      0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                          (0.87173106479036e2 * std::pow(C69, 2) *
                           std::pow(std::sqrt(C57) + 0.743294, 2))) *
                     (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1)) /
                     (2. * (std::pow(2., C48) - 1.))) *
                    C110) +
               C116 *
                   (C92 * 0.01554535 *
                        (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                         (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                        ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) +
                    C97 * C46 * 0.01554535 *
                        (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                         (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                        (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) /
                   (2. * (std::pow(2., C48) - 1.))) +
              C97 *
                  (((((C92 *
                           (0.01554535 *
                                ((std::log(C57 / C69) +
                                  0.402462e2 * C75 / std::sqrt(0.0137284639e2)) -
                                 -0.149573794914e2 *
                                     (std::log(std::pow(std::sqrt(C57) + 0.743294, 2) / C69) +
                                      0.37273024e2 * C75 / std::sqrt(0.0137284639e2)) /
                                     0.87173106479036e2) -
                            C87) *
                           (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1)) +
                       C128 * C46 *
                           (C49 * std::pow(1. - C55, C49 - 1) -
                            C49 * std::pow(C55 + 1., C49 - 1))) +
                      C99 * C46 * 0.01554535 *
                          (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) *
                                                   C69 /
                                                   (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                                    std::pow(std::sqrt(C57) + 0.743294, 2))) *
                          (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) +
                     C110 * C46 * 0.01554535 *
                         ((-C57 * C69) / (std::pow(C69, 2) * C57) -
                          0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                              (0.87173106479036e2 * std::pow(C69, 2) *
                               std::pow(std::sqrt(C57) + 0.743294, 2))) *
                         (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) +
                    C116 * C46 * 0.01554535 *
                        (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                         (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                        (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) +
                   C97 * C46 *
                       (0.01554535 *
                            ((std::log(C57 / C69) + 0.402462e2 * C75 / std::sqrt(0.0137284639e2)) -
                             -0.149573794914e2 *
                                 (std::log(std::pow(std::sqrt(C57) + 0.743294, 2) / C69) +
                                  0.37273024e2 * C75 / std::sqrt(0.0137284639e2)) /
                                 0.87173106479036e2) -
                        C87) *
                       (C49 * (C49 - 1) * std::pow(C55 + 1., C49 - 2) -
                        C49 * (1 - C49) * std::pow(1. - C55, C49 - 2))) /
                  (2. * (std::pow(2., C48) - 1.))) +
             ((((((2. - (std::pow(C55 + 1., C49) + std::pow(1. - C55, C49))) /
                      (2. * (std::pow(2., C48) - 1.)) +
                  1) *
                     C128 +
                 C99 * 0.01554535 *
                     (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) * C69 /
                                              (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                               std::pow(std::sqrt(C57) + 0.743294, 2))) *
                     ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                     (2. * (std::pow(2., C48) - 1.))) +
                C110 * 0.01554535 *
                    ((-C57 * C69) / (std::pow(C69, 2) * C57) -
                     0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                         (0.87173106479036e2 * std::pow(C69, 2) *
                          std::pow(std::sqrt(C57) + 0.743294, 2))) *
                    ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                    (2. * (std::pow(2., C48) - 1.))) +
               C116 * 0.01554535 *
                   (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                    (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                   ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                   (2. * (std::pow(2., C48) - 1.))) +
              C97 *
                  (0.01554535 *
                       ((std::log(C57 / C69) + 0.402462e2 * C75 / std::sqrt(0.0137284639e2)) -
                        -0.149573794914e2 *
                            (std::log(std::pow(std::sqrt(C57) + 0.743294, 2) / C69) +
                             0.37273024e2 * C75 / std::sqrt(0.0137284639e2)) /
                            0.87173106479036e2) -
                   C87) *
                  (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1)) /
                  (2. * (std::pow(2., C48) - 1.))) *
                 C92) +
            C46 *
                ((2. - (std::pow(C55 + 1., C49) + std::pow(1. - C55, C49))) /
                     (2. * (std::pow(2., C48) - 1.)) +
                 1) *
                C198) +
           C169 * C46 * 0.01554535 *
               (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) * C69 /
                                        (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                         std::pow(std::sqrt(C57) + 0.743294, 2))) *
               ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
               (2. * (std::pow(2., C48) - 1.))) +
          C180 * C46 * 0.01554535 *
              ((-C57 * C69) / (std::pow(C69, 2) * C57) -
               0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                   (0.87173106479036e2 * std::pow(C69, 2) *
                    std::pow(std::sqrt(C57) + 0.743294, 2))) *
              ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
              (2. * (std::pow(2., C48) - 1.))) +
         C186 * C46 * 0.01554535 *
             (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
              (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
             ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
             (2. * (std::pow(2., C48) - 1.))) +
        C167 * C46 *
            (0.01554535 * ((std::log(C57 / C69) + 0.402462e2 * C75 / std::sqrt(0.0137284639e2)) -
                           -0.149573794914e2 *
                               (std::log(std::pow(std::sqrt(C57) + 0.743294, 2) / C69) +
                                0.37273024e2 * C75 / std::sqrt(0.0137284639e2)) /
                               0.87173106479036e2) -
             C87) *
            (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1)) /
            (2. * (std::pow(2., C48) - 1.));

    const double C129 = 1;
    const double C130 = -1;
    const double C131 = 0.9999999999999999 * C130 + 0.;
    const double C132 = Pi * C129;
    const double C133 = 4. * C132;
    const double C134 = C131 / C46 - C129 * C52 / std::pow(C46, 2);
    const double C135 = -3. * C133 / std::pow(C54, 2);
    const double C136 = C48 * std::pow(C56, C48 - 1) * C135;
    const double C137 = 2. * C136 / (2 * std::sqrt(C57));
    const double C138 = 0.13072e2 * C136 / (2 * std::sqrt(C57));
    const double C139 = 0.201231e2 * C136 / (2 * std::sqrt(C57));
    const double C140 = C136 / (2 * std::sqrt(C57));
    const double C141 = C136 + C138;
    const double C142 = C136 + C139;
    const double C143 = C137;
    const double C144 = C137;
    const double C145 = 2 * C62 * C140;
    const double C146 = C141;
    const double C147 = C142;
    const double C148 = (-C143 * std::sqrt(0.00002016e2)) / std::pow(C65, 2);
    const double C149 = (-C144 * std::sqrt(0.0137284639e2)) / std::pow(C66, 2);
    const double C150 = C136 / C68 - C146 * C57 / std::pow(C68, 2);
    const double C151 = C145 / C68 - C146 * C67 / std::pow(C68, 2);
    const double C152 = C148 / (std::pow(C70, 2) + 1);
    const double C153 = C149 / (std::pow(C71, 2) + 1);
    const double C154 = 0.24506856e2 * C152;
    const double C155 = 0.26144e2 * C152;
    const double C156 = C150 / C72;
    const double C157 = C151 / C73;
    const double C158 = std::sqrt(1 / 0.00002016e2) * C154;
    const double C159 = std::sqrt(1 / 0.00002016e2) * C155;
    const double C160 = C156 + C159;
    const double C161 = C157 + C158;
    const double C162 = -5.350186592 * C161;
    const double C163 = C162 / 37.537128437796;
    const double C164 = C160 - C163;
    const double C165 = 0.0310907 * C164;
    const double C199 = 0.;
    const double C200 =
        (C199 / C46 +
         ((-C94) / std::pow(C46, 2) - (-C92 * C52 * 2 * C46) / std::pow(C46, 4)) * C129) -
        C131 * C92 / std::pow(C46, 2);
    const double C201 = (-C133 * -3. * C96 * 2 * C54) / std::pow(C54, 4);
    const double C202 =
        C48 * (C48 - 1) * std::pow(C56, C48 - 2) * C98 * C135 + C48 * std::pow(C56, C48 - 1) * C201;
    const double C203 = 2. * C202 / (2 * std::sqrt(C57)) -
                        C136 * 2. * C99 / (std::sqrt(C57) * std::pow(2 * std::sqrt(C57), 2));
    const double C204 = 0.13072e2 * C202 / (2 * std::sqrt(C57)) -
                        C136 * 0.13072e2 * C99 / (std::sqrt(C57) * std::pow(2 * std::sqrt(C57), 2));
    const double C205 =
        0.201231e2 * C202 / (2 * std::sqrt(C57)) -
        C136 * 0.201231e2 * C99 / (std::sqrt(C57) * std::pow(2 * std::sqrt(C57), 2));
    const double C206 = C202 / (2 * std::sqrt(C57)) -
                        C136 * C99 / (std::sqrt(C57) * std::pow(2 * std::sqrt(C57), 2));
    const double C207 = C202 + C204;
    const double C208 = C202 + C205;
    const double C209 = C203;
    const double C210 = C203;
    const double C211 = 2 * C103 * C140 + 2 * C62 * C206;
    const double C212 = C207;
    const double C213 = C208;
    const double C214 = C143 * C106 * std::sqrt(0.00002016e2) * 2 * C65 / std::pow(C65, 4) -
                        C209 * std::sqrt(0.00002016e2) / std::pow(C65, 2);
    const double C215 = C144 * C107 * std::sqrt(0.0137284639e2) * 2 * C66 / std::pow(C66, 4) -
                        C210 * std::sqrt(0.0137284639e2) / std::pow(C66, 2);
    const double C216 =
        ((C202 / C68 +
          ((-C99) / std::pow(C68, 2) - (-C109 * C57 * 2 * C68) / std::pow(C68, 4)) * C146) -
         C212 * C57 / std::pow(C68, 2)) -
        C136 * C109 / std::pow(C68, 2);
    const double C217 =
        ((C211 / C68 +
          ((-C108) / std::pow(C68, 2) - (-C109 * C67 * 2 * C68) / std::pow(C68, 4)) * C146) -
         C212 * C67 / std::pow(C68, 2)) -
        C145 * C109 / std::pow(C68, 2);
    const double C218 =
        C214 / (std::pow(C70, 2) + 1) - C148 * C111 * 2 * C70 / std::pow(std::pow(C70, 2) + 1, 2);
    const double C219 =
        C215 / (std::pow(C71, 2) + 1) - C149 * C112 * 2 * C71 / std::pow(std::pow(C71, 2) + 1, 2);
    const double C220 = 0.24506856e2 * C218;
    const double C221 = 0.26144e2 * C218;
    const double C222 = C216 / C72 - C150 * C113 / std::pow(C72, 2);
    const double C223 = C217 / C73 - C151 * C114 / std::pow(C73, 2);
    const double C224 = std::sqrt(1 / 0.00002016e2) * C220;
    const double C225 = std::sqrt(1 / 0.00002016e2) * C221;
    const double C226 = C222 + C225;
    const double C227 = C223 + C224;
    const double C228 = -5.350186592 * C227;
    const double C229 = C228 / 37.537128437796;
    const double C230 = C226 - C229;
    const double C231 = 0.0310907 * C230;
    matrix.upper[1] =
        ((((((((((((2. - (std::pow(C55 + 1., C49) + std::pow(1. - C55, C49))) /
                       (2. * (std::pow(2., C48) - 1.)) +
                   1) *
                      C92 +
                  C97 * C46 *
                      (C49 * std::pow(1. - C55, C49 - 1) - C49 * std::pow(C55 + 1., C49 - 1)) /
                      (2. * (std::pow(2., C48) - 1.))) *
                     C165 +
                 C136 *
                     (((C92 * 0.01554535 *
                            (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) *
                                                     C69 /
                                                     (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                                      std::pow(std::sqrt(C57) + 0.743294, 2))) *
                            ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) +
                        C99 * C46 * 0.01554535 *
                            ((-std::pow(C69, 2)) / std::pow(C69 * C57, 2) -
                             (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                  std::pow(std::sqrt(C57) + 0.743294, 2) * -0.149573794914e2 * C69 /
                                  (2 * std::sqrt(C57)) -
                              -0.149573794914e2 * (std::sqrt(C57) + 0.743294) * C69 *
                                  0.87173106479036e2 *
                                  (std::sqrt(C57) * C69 * (std::sqrt(C57) + 0.743294) /
                                       std::sqrt(C57) +
                                   std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                                       (2 * std::sqrt(C57)))) /
                                 std::pow(0.87173106479036e2 * std::sqrt(C57) * C69 *
                                              std::pow(std::sqrt(C57) + 0.743294, 2),
                                          2)) *
                            ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.)) +
                       C110 * C46 * 0.01554535 *
                           ((C57 * std::pow(C69, 3) - std::pow(C69, 2) * C57 * C69) /
                                std::pow(std::pow(C69, 2) * C57, 2) -
                            (0.87173106479036e2 * std::pow(C69, 2) *
                                 std::pow(std::sqrt(C57) + 0.743294, 2) * 0.149573794914e2 * C69 *
                                 (std::sqrt(C57) + 0.743294) / std::sqrt(C57) -
                             0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 *
                                 0.87173106479036e2 * std::pow(C69, 2) *
                                 (std::sqrt(C57) + 0.743294) / std::sqrt(C57)) /
                                std::pow(0.87173106479036e2 * std::pow(C69, 2) *
                                             std::pow(std::sqrt(C57) + 0.743294, 2),
                                         2)) *
                           ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.)) +
                      C97 * C46 * 0.01554535 *
                          (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) *
                                                   C69 /
                                                   (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                                    std::pow(std::sqrt(C57) + 0.743294, 2))) *
                          (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) /
                     (2. * (std::pow(2., C48) - 1.))) +
                (((C92 * 0.01554535 *
                       ((-C57 * C69) / (std::pow(C69, 2) * C57) -
                        0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                            (0.87173106479036e2 * std::pow(C69, 2) *
                             std::pow(std::sqrt(C57) + 0.743294, 2))) *
                       ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                       (2. * (std::pow(2., C48) - 1.)) +
                   C99 * C46 * -0.01554535 *
                       ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) *
                       (0.87173106479036e2 * std::sqrt(C57) * C69 *
                            std::pow(std::sqrt(C57) + 0.743294, 2) * -0.149573794914e2 *
                            (std::sqrt(C57) + 0.743294) -
                        -0.149573794914e2 * (std::sqrt(C57) + 0.743294) * C69 * 0.87173106479036e2 *
                            std::sqrt(C57) * std::pow(std::sqrt(C57) + 0.743294, 2)) /
                       (std::pow(0.87173106479036e2 * std::sqrt(C57) * C69 *
                                     std::pow(std::sqrt(C57) + 0.743294, 2),
                                 2) *
                        2. * (std::pow(2., C48) - 1.))) +
                  C110 * C46 * 0.01554535 *
                      ((C57 * C69 * 2 * C69 * C57 - std::pow(C69, 2) * std::pow(C57, 2)) /
                           std::pow(std::pow(C69, 2) * C57, 2) -
                       (0.87173106479036e2 * std::pow(C69, 2) *
                            std::pow(std::sqrt(C57) + 0.743294, 2) * 0.149573794914e2 *
                            std::pow(std::sqrt(C57) + 0.743294, 2) -
                        0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 *
                            0.174346212958072e3 * C69 * std::pow(std::sqrt(C57) + 0.743294, 2)) /
                           std::pow(0.87173106479036e2 * std::pow(C69, 2) *
                                        std::pow(std::sqrt(C57) + 0.743294, 2),
                                    2)) *
                      ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                      (2. * (std::pow(2., C48) - 1.))) +
                 C97 * C46 * 0.01554535 *
                     ((-C57 * C69) / (std::pow(C69, 2) * C57) -
                      0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                          (0.87173106479036e2 * std::pow(C69, 2) *
                           std::pow(std::sqrt(C57) + 0.743294, 2))) *
                     (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1)) /
                     (2. * (std::pow(2., C48) - 1.))) *
                    C147) +
               C153 *
                   (C92 * 0.01554535 *
                        (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                         (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                        ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) +
                    C97 * C46 * 0.01554535 *
                        (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                         (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                        (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) /
                   (2. * (std::pow(2., C48) - 1.))) +
              C134 *
                  (((((C92 *
                           (0.01554535 *
                                ((std::log(C57 / C69) +
                                  0.402462e2 * C75 / std::sqrt(0.0137284639e2)) -
                                 -0.149573794914e2 *
                                     (std::log(std::pow(std::sqrt(C57) + 0.743294, 2) / C69) +
                                      0.37273024e2 * C75 / std::sqrt(0.0137284639e2)) /
                                     0.87173106479036e2) -
                            C87) *
                           (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1)) +
                       C128 * C46 *
                           (C49 * std::pow(1. - C55, C49 - 1) -
                            C49 * std::pow(C55 + 1., C49 - 1))) +
                      C99 * C46 * 0.01554535 *
                          (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) *
                                                   C69 /
                                                   (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                                    std::pow(std::sqrt(C57) + 0.743294, 2))) *
                          (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) +
                     C110 * C46 * 0.01554535 *
                         ((-C57 * C69) / (std::pow(C69, 2) * C57) -
                          0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                              (0.87173106479036e2 * std::pow(C69, 2) *
                               std::pow(std::sqrt(C57) + 0.743294, 2))) *
                         (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) +
                    C116 * C46 * 0.01554535 *
                        (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                         (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                        (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) +
                   C97 * C46 *
                       (0.01554535 *
                            ((std::log(C57 / C69) + 0.402462e2 * C75 / std::sqrt(0.0137284639e2)) -
                             -0.149573794914e2 *
                                 (std::log(std::pow(std::sqrt(C57) + 0.743294, 2) / C69) +
                                  0.37273024e2 * C75 / std::sqrt(0.0137284639e2)) /
                                 0.87173106479036e2) -
                        C87) *
                       (C49 * (C49 - 1) * std::pow(C55 + 1., C49 - 2) -
                        C49 * (1 - C49) * std::pow(1. - C55, C49 - 2))) /
                  (2. * (std::pow(2., C48) - 1.))) +
             ((((((2. - (std::pow(C55 + 1., C49) + std::pow(1. - C55, C49))) /
                      (2. * (std::pow(2., C48) - 1.)) +
                  1) *
                     C128 +
                 C99 * 0.01554535 *
                     (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) * C69 /
                                              (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                               std::pow(std::sqrt(C57) + 0.743294, 2))) *
                     ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                     (2. * (std::pow(2., C48) - 1.))) +
                C110 * 0.01554535 *
                    ((-C57 * C69) / (std::pow(C69, 2) * C57) -
                     0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                         (0.87173106479036e2 * std::pow(C69, 2) *
                          std::pow(std::sqrt(C57) + 0.743294, 2))) *
                    ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                    (2. * (std::pow(2., C48) - 1.))) +
               C116 * 0.01554535 *
                   (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                    (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                   ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                   (2. * (std::pow(2., C48) - 1.))) +
              C97 *
                  (0.01554535 *
                       ((std::log(C57 / C69) + 0.402462e2 * C75 / std::sqrt(0.0137284639e2)) -
                        -0.149573794914e2 *
                            (std::log(std::pow(std::sqrt(C57) + 0.743294, 2) / C69) +
                             0.37273024e2 * C75 / std::sqrt(0.0137284639e2)) /
                            0.87173106479036e2) -
                   C87) *
                  (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1)) /
                  (2. * (std::pow(2., C48) - 1.))) *
                 C129) +
            C46 *
                ((2. - (std::pow(C55 + 1., C49) + std::pow(1. - C55, C49))) /
                     (2. * (std::pow(2., C48) - 1.)) +
                 1) *
                C231) +
           C202 * C46 * 0.01554535 *
               (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) * C69 /
                                        (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                         std::pow(std::sqrt(C57) + 0.743294, 2))) *
               ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
               (2. * (std::pow(2., C48) - 1.))) +
          C213 * C46 * 0.01554535 *
              ((-C57 * C69) / (std::pow(C69, 2) * C57) -
               0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                   (0.87173106479036e2 * std::pow(C69, 2) *
                    std::pow(std::sqrt(C57) + 0.743294, 2))) *
              ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
              (2. * (std::pow(2., C48) - 1.))) +
         C219 * C46 * 0.01554535 *
             (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
              (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
             ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
             (2. * (std::pow(2., C48) - 1.))) +
        C200 * C46 *
            (0.01554535 * ((std::log(C57 / C69) + 0.402462e2 * C75 / std::sqrt(0.0137284639e2)) -
                           -0.149573794914e2 *
                               (std::log(std::pow(std::sqrt(C57) + 0.743294, 2) / C69) +
                                0.37273024e2 * C75 / std::sqrt(0.0137284639e2)) /
                               0.87173106479036e2) -
             C87) *
            (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1)) /
            (2. * (std::pow(2., C48) - 1.));

    const double C232 = 0.;
    const double C233 =
        (C232 / C46 +
         ((-C131) / std::pow(C46, 2) - (-C129 * C52 * 2 * C46) / std::pow(C46, 4)) * C129) -
        C131 * C129 / std::pow(C46, 2);
    const double C234 = (-C133 * -3. * C133 * 2 * C54) / std::pow(C54, 4);
    const double C235 = C48 * (C48 - 1) * std::pow(C56, C48 - 2) * std::pow(C135, 2) +
                        C48 * std::pow(C56, C48 - 1) * C234;
    const double C236 = 2. * C235 / (2 * std::sqrt(C57)) -
                        2. * std::pow(C136, 2) / (std::sqrt(C57) * std::pow(2 * std::sqrt(C57), 2));
    const double C237 =
        0.13072e2 * C235 / (2 * std::sqrt(C57)) -
        0.13072e2 * std::pow(C136, 2) / (std::sqrt(C57) * std::pow(2 * std::sqrt(C57), 2));
    const double C238 =
        0.201231e2 * C235 / (2 * std::sqrt(C57)) -
        0.201231e2 * std::pow(C136, 2) / (std::sqrt(C57) * std::pow(2 * std::sqrt(C57), 2));
    const double C239 = C235 / (2 * std::sqrt(C57)) -
                        std::pow(C136, 2) / (std::sqrt(C57) * std::pow(2 * std::sqrt(C57), 2));
    const double C240 = C235 + C237;
    const double C241 = C235 + C238;
    const double C242 = C236;
    const double C243 = C236;
    const double C244 = 2 * std::pow(C140, 2) + 2 * C62 * C239;
    const double C245 = C240;
    const double C246 = C241;
    const double C247 = C143 * C143 * std::sqrt(0.00002016e2) * 2 * C65 / std::pow(C65, 4) -
                        C242 * std::sqrt(0.00002016e2) / std::pow(C65, 2);
    const double C248 = C144 * C144 * std::sqrt(0.0137284639e2) * 2 * C66 / std::pow(C66, 4) -
                        C243 * std::sqrt(0.0137284639e2) / std::pow(C66, 2);
    const double C249 =
        ((C235 / C68 +
          ((-C136) / std::pow(C68, 2) - (-C146 * C57 * 2 * C68) / std::pow(C68, 4)) * C146) -
         C245 * C57 / std::pow(C68, 2)) -
        C136 * C146 / std::pow(C68, 2);
    const double C250 =
        ((C244 / C68 +
          ((-C145) / std::pow(C68, 2) - (-C146 * C67 * 2 * C68) / std::pow(C68, 4)) * C146) -
         C245 * C67 / std::pow(C68, 2)) -
        C145 * C146 / std::pow(C68, 2);
    const double C251 = C247 / (std::pow(C70, 2) + 1) -
                        2 * C70 * std::pow(C148, 2) / std::pow(std::pow(C70, 2) + 1, 2);
    const double C252 = C248 / (std::pow(C71, 2) + 1) -
                        2 * C71 * std::pow(C149, 2) / std::pow(std::pow(C71, 2) + 1, 2);
    const double C253 = 0.24506856e2 * C251;
    const double C254 = 0.26144e2 * C251;
    const double C255 = C249 / C72 - std::pow(C150, 2) / std::pow(C72, 2);
    const double C256 = C250 / C73 - std::pow(C151, 2) / std::pow(C73, 2);
    const double C257 = std::sqrt(1 / 0.00002016e2) * C253;
    const double C258 = std::sqrt(1 / 0.00002016e2) * C254;
    const double C259 = C255 + C258;
    const double C260 = C256 + C257;
    const double C261 = -5.350186592 * C260;
    const double C262 = C261 / 37.537128437796;
    const double C263 = C259 - C262;
    const double C264 = 0.0310907 * C263;
    matrix.upper[2] =
        ((((((((((((2. - (std::pow(C55 + 1., C49) + std::pow(1. - C55, C49))) /
                       (2. * (std::pow(2., C48) - 1.)) +
                   1) *
                      C129 +
                  C134 * C46 *
                      (C49 * std::pow(1. - C55, C49 - 1) - C49 * std::pow(C55 + 1., C49 - 1)) /
                      (2. * (std::pow(2., C48) - 1.))) *
                     C165 +
                 C136 *
                     (((C129 * 0.01554535 *
                            (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) *
                                                     C69 /
                                                     (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                                      std::pow(std::sqrt(C57) + 0.743294, 2))) *
                            ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) +
                        C136 * C46 * 0.01554535 *
                            ((-std::pow(C69, 2)) / std::pow(C69 * C57, 2) -
                             (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                  std::pow(std::sqrt(C57) + 0.743294, 2) * -0.149573794914e2 * C69 /
                                  (2 * std::sqrt(C57)) -
                              -0.149573794914e2 * (std::sqrt(C57) + 0.743294) * C69 *
                                  0.87173106479036e2 *
                                  (std::sqrt(C57) * C69 * (std::sqrt(C57) + 0.743294) /
                                       std::sqrt(C57) +
                                   std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                                       (2 * std::sqrt(C57)))) /
                                 std::pow(0.87173106479036e2 * std::sqrt(C57) * C69 *
                                              std::pow(std::sqrt(C57) + 0.743294, 2),
                                          2)) *
                            ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.)) +
                       C147 * C46 * 0.01554535 *
                           ((C57 * std::pow(C69, 3) - std::pow(C69, 2) * C57 * C69) /
                                std::pow(std::pow(C69, 2) * C57, 2) -
                            (0.87173106479036e2 * std::pow(C69, 2) *
                                 std::pow(std::sqrt(C57) + 0.743294, 2) * 0.149573794914e2 * C69 *
                                 (std::sqrt(C57) + 0.743294) / std::sqrt(C57) -
                             0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 *
                                 0.87173106479036e2 * std::pow(C69, 2) *
                                 (std::sqrt(C57) + 0.743294) / std::sqrt(C57)) /
                                std::pow(0.87173106479036e2 * std::pow(C69, 2) *
                                             std::pow(std::sqrt(C57) + 0.743294, 2),
                                         2)) *
                           ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.)) +
                      C134 * C46 * 0.01554535 *
                          (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) *
                                                   C69 /
                                                   (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                                    std::pow(std::sqrt(C57) + 0.743294, 2))) *
                          (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) /
                     (2. * (std::pow(2., C48) - 1.))) +
                (((C129 * 0.01554535 *
                       ((-C57 * C69) / (std::pow(C69, 2) * C57) -
                        0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                            (0.87173106479036e2 * std::pow(C69, 2) *
                             std::pow(std::sqrt(C57) + 0.743294, 2))) *
                       ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                       (2. * (std::pow(2., C48) - 1.)) +
                   C136 * C46 * -0.01554535 *
                       ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) *
                       (0.87173106479036e2 * std::sqrt(C57) * C69 *
                            std::pow(std::sqrt(C57) + 0.743294, 2) * -0.149573794914e2 *
                            (std::sqrt(C57) + 0.743294) -
                        -0.149573794914e2 * (std::sqrt(C57) + 0.743294) * C69 * 0.87173106479036e2 *
                            std::sqrt(C57) * std::pow(std::sqrt(C57) + 0.743294, 2)) /
                       (std::pow(0.87173106479036e2 * std::sqrt(C57) * C69 *
                                     std::pow(std::sqrt(C57) + 0.743294, 2),
                                 2) *
                        2. * (std::pow(2., C48) - 1.))) +
                  C147 * C46 * 0.01554535 *
                      ((C57 * C69 * 2 * C69 * C57 - std::pow(C69, 2) * std::pow(C57, 2)) /
                           std::pow(std::pow(C69, 2) * C57, 2) -
                       (0.87173106479036e2 * std::pow(C69, 2) *
                            std::pow(std::sqrt(C57) + 0.743294, 2) * 0.149573794914e2 *
                            std::pow(std::sqrt(C57) + 0.743294, 2) -
                        0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 *
                            0.174346212958072e3 * C69 * std::pow(std::sqrt(C57) + 0.743294, 2)) /
                           std::pow(0.87173106479036e2 * std::pow(C69, 2) *
                                        std::pow(std::sqrt(C57) + 0.743294, 2),
                                    2)) *
                      ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                      (2. * (std::pow(2., C48) - 1.))) +
                 C134 * C46 * 0.01554535 *
                     ((-C57 * C69) / (std::pow(C69, 2) * C57) -
                      0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                          (0.87173106479036e2 * std::pow(C69, 2) *
                           std::pow(std::sqrt(C57) + 0.743294, 2))) *
                     (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1)) /
                     (2. * (std::pow(2., C48) - 1.))) *
                    C147) +
               C153 *
                   (C129 * 0.01554535 *
                        (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                         (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                        ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) +
                    C134 * C46 * 0.01554535 *
                        (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                         (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                        (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) /
                   (2. * (std::pow(2., C48) - 1.))) +
              C134 *
                  (((((C129 *
                           (0.01554535 *
                                ((std::log(C57 / C69) +
                                  0.402462e2 * C75 / std::sqrt(0.0137284639e2)) -
                                 -0.149573794914e2 *
                                     (std::log(std::pow(std::sqrt(C57) + 0.743294, 2) / C69) +
                                      0.37273024e2 * C75 / std::sqrt(0.0137284639e2)) /
                                     0.87173106479036e2) -
                            C87) *
                           (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1)) +
                       C165 * C46 *
                           (C49 * std::pow(1. - C55, C49 - 1) -
                            C49 * std::pow(C55 + 1., C49 - 1))) +
                      C136 * C46 * 0.01554535 *
                          (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) *
                                                   C69 /
                                                   (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                                    std::pow(std::sqrt(C57) + 0.743294, 2))) *
                          (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) +
                     C147 * C46 * 0.01554535 *
                         ((-C57 * C69) / (std::pow(C69, 2) * C57) -
                          0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                              (0.87173106479036e2 * std::pow(C69, 2) *
                               std::pow(std::sqrt(C57) + 0.743294, 2))) *
                         (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) +
                    C153 * C46 * 0.01554535 *
                        (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                         (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                        (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1))) +
                   C134 * C46 *
                       (0.01554535 *
                            ((std::log(C57 / C69) + 0.402462e2 * C75 / std::sqrt(0.0137284639e2)) -
                             -0.149573794914e2 *
                                 (std::log(std::pow(std::sqrt(C57) + 0.743294, 2) / C69) +
                                  0.37273024e2 * C75 / std::sqrt(0.0137284639e2)) /
                                 0.87173106479036e2) -
                        C87) *
                       (C49 * (C49 - 1) * std::pow(C55 + 1., C49 - 2) -
                        C49 * (1 - C49) * std::pow(1. - C55, C49 - 2))) /
                  (2. * (std::pow(2., C48) - 1.))) +
             ((((((2. - (std::pow(C55 + 1., C49) + std::pow(1. - C55, C49))) /
                      (2. * (std::pow(2., C48) - 1.)) +
                  1) *
                     C165 +
                 C136 * 0.01554535 *
                     (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) * C69 /
                                              (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                               std::pow(std::sqrt(C57) + 0.743294, 2))) *
                     ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                     (2. * (std::pow(2., C48) - 1.))) +
                C147 * 0.01554535 *
                    ((-C57 * C69) / (std::pow(C69, 2) * C57) -
                     0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                         (0.87173106479036e2 * std::pow(C69, 2) *
                          std::pow(std::sqrt(C57) + 0.743294, 2))) *
                    ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                    (2. * (std::pow(2., C48) - 1.))) +
               C153 * 0.01554535 *
                   (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
                    (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
                   ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
                   (2. * (std::pow(2., C48) - 1.))) +
              C134 *
                  (0.01554535 *
                       ((std::log(C57 / C69) + 0.402462e2 * C75 / std::sqrt(0.0137284639e2)) -
                        -0.149573794914e2 *
                            (std::log(std::pow(std::sqrt(C57) + 0.743294, 2) / C69) +
                             0.37273024e2 * C75 / std::sqrt(0.0137284639e2)) /
                            0.87173106479036e2) -
                   C87) *
                  (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1)) /
                  (2. * (std::pow(2., C48) - 1.))) *
                 C129) +
            C46 *
                ((2. - (std::pow(C55 + 1., C49) + std::pow(1. - C55, C49))) /
                     (2. * (std::pow(2., C48) - 1.)) +
                 1) *
                C264) +
           C235 * C46 * 0.01554535 *
               (C69 / (C69 * C57) - -0.149573794914e2 * (std::sqrt(C57) + 0.743294) * C69 /
                                        (0.87173106479036e2 * std::sqrt(C57) * C69 *
                                         std::pow(std::sqrt(C57) + 0.743294, 2))) *
               ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
               (2. * (std::pow(2., C48) - 1.))) +
          C246 * C46 * 0.01554535 *
              ((-C57 * C69) / (std::pow(C69, 2) * C57) -
               0.149573794914e2 * std::pow(std::sqrt(C57) + 0.743294, 2) * C69 /
                   (0.87173106479036e2 * std::pow(C69, 2) *
                    std::pow(std::sqrt(C57) + 0.743294, 2))) *
              ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
              (2. * (std::pow(2., C48) - 1.))) +
         C252 * C46 * 0.01554535 *
             (std::sqrt(0.161975661444e4 / 0.0137284639e2) -
              (-std::sqrt(0.3108137927532288e6 / 0.0137284639e2)) / 0.87173106479036e2) *
             ((std::pow(C55 + 1., C49) + std::pow(1. - C55, C49)) - 2.) /
             (2. * (std::pow(2., C48) - 1.))) +
        C233 * C46 *
            (0.01554535 * ((std::log(C57 / C69) + 0.402462e2 * C75 / std::sqrt(0.0137284639e2)) -
                           -0.149573794914e2 *
                               (std::log(std::pow(std::sqrt(C57) + 0.743294, 2) / C69) +
                                0.37273024e2 * C75 / std::sqrt(0.0137284639e2)) /
                               0.87173106479036e2) -
             C87) *
            (C49 * std::pow(C55 + 1., C49 - 1) - C49 * std::pow(1. - C55, C49 - 1)) /
            (2. * (std::pow(2., C48) - 1.));
}

} // namespace excgrid
