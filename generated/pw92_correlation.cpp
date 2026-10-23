// excgrid codegen source: the PW92 LDA correlation functional (Perdew-Wang 1992).
// Formula from the PW92 paper.

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
        const double C237 = rhoA + rhoB;
        const double C238 = rhoA - rhoB;
        const double C239 = 1. / 3.;
        const double C240 = 2. / 3.;
        const double C241 = 4. / 3.;
        const double C242 = 0.9999999999999999 * C238;
        const double C243 = Pi * C237;
        const double C244 = C238 / C237;
        const double C245 = std::pow(2., C239);
        const double C246 = 4. * C243;
        const double C247 = C245 - 1.;
        const double C248 = C242 / C237;
        const double C249 = std::pow(C244, 4);
        const double C250 = C248 + 1.;
        const double C251 = 1. - C248;
        const double C252 = 3. / C246;
        const double C253 = std::sqrt(C252);
        const double C254 = std::pow(C250, C241);
        const double C255 = std::pow(C251, C241);
        const double C256 = std::pow(C252, C239);
        const double C257 = std::pow(C252, C240);
        const double C258 = 0.2137 * C256;
        const double C259 = 0.49294 * C257;
        const double C260 = 1.6382 * C253;
        const double C261 = 3.5876 * C256;
        const double C262 = C254 + C255;
        const double C263 = std::sqrt(C256);
        const double C264 = 7.5957 * C263;
        const double C265 = C258 + 1.;
        const double C266 = C262 - 2.;
        const double C267 = C264 + C261;
        const double C268 = C267 + C260;
        const double C269 = C268 + C259;
        const double C270 = 0.062182 * C269;
        const double C271 = 1. / C270;
        const double C272 = C271 + 1.;
        const double C273 = std::log(C272);
        const double C274 = C265 * C273;
        const double C275 = 0.062182 * C274;

        result.exc =
            C237 *
            ((0.033774 * (0.11125 * C256 + 1.) *
                  std::log(1. / (0.033774 * (((10.357 * C263 + 3.6231 * C256) + 0.88026 * C253) +
                                             0.49671 * C257)) +
                           1.) *
                  C266 * (1. - C249) / (3.4198418683227306 * C247) -
              C275) +
             C249 *
                 (C275 -
                  0.03109 * (0.20548 * C256 + 1.) *
                      std::log(1. / (0.03109 * (((14.1189 * C263 + 6.1977 * C256) + 3.3662 * C253) +
                                                0.62517 * C257)) +
                               1.)) *
                 C266 / (2. * C247));
    } else if (rhoA < eps)
    {
        const double C277 = -0.9999999999999999 * rhoB;
        const double C278 = Pi * rhoB;
        const double C279 = -rhoB;
        const double C280 = 1. / 3.;
        const double C281 = 2. / 3.;
        const double C282 = 4. / 3.;
        const double C283 = 4. * C278;
        const double C284 = C277 / rhoB;
        const double C285 = C279 / rhoB;
        const double C286 = std::pow(2., C280);
        const double C287 = C284 + 1.;
        const double C288 = 1. - C284;
        const double C289 = C286 - 1.;
        const double C290 = 3. / C283;
        const double C291 = std::pow(C285, 4);
        const double C292 = std::sqrt(C290);
        const double C293 = std::pow(C287, C282);
        const double C294 = std::pow(C288, C282);
        const double C295 = std::pow(C290, C280);
        const double C296 = std::pow(C290, C281);
        const double C297 = 0.2137 * C295;
        const double C298 = 0.49294 * C296;
        const double C299 = 1.6382 * C292;
        const double C300 = 3.5876 * C295;
        const double C301 = C293 + C294;
        const double C302 = std::sqrt(C295);
        const double C303 = 7.5957 * C302;
        const double C304 = C297 + 1.;
        const double C305 = C301 - 2.;
        const double C306 = C303 + C300;
        const double C307 = C306 + C299;
        const double C308 = C307 + C298;
        const double C309 = 0.062182 * C308;
        const double C310 = 1. / C309;
        const double C311 = C310 + 1.;
        const double C312 = std::log(C311);
        const double C313 = C304 * C312;
        const double C314 = 0.062182 * C313;

        result.exc =
            rhoB *
            ((0.033774 * (0.11125 * C295 + 1.) *
                  std::log(1. / (0.033774 * (((10.357 * C302 + 3.6231 * C295) + 0.88026 * C292) +
                                             0.49671 * C296)) +
                           1.) *
                  C305 * (1. - C291) / (3.4198418683227306 * C289) -
              C314) +
             C291 *
                 (C314 -
                  0.03109 * (0.20548 * C295 + 1.) *
                      std::log(1. / (0.03109 * (((14.1189 * C302 + 6.1977 * C295) + 3.3662 * C292) +
                                                0.62517 * C296)) +
                               1.)) *
                 C305 / (2. * C289));
    } else
    {
        const double C316 = 0.9999999999999999 * rhoA;
        const double C317 = Pi * rhoA;
        const double C318 = 1. / 3.;
        const double C319 = 2. / 3.;
        const double C320 = 4. / 3.;
        const double C321 = 4. * C317;
        const double C322 = C316 / rhoA;
        const double C323 = 3. / C321;
        const double C324 = std::sqrt(C323);
        const double C325 = std::pow(C323, C318);
        const double C326 = std::pow(C323, C319);
        const double C327 = 0.2137 * C325;
        const double C328 = 0.49294 * C326;
        const double C329 = 1.6382 * C324;
        const double C330 = 3.5876 * C325;
        const double C331 = std::sqrt(C325);
        const double C332 = 7.5957 * C331;
        const double C333 = C327 + 1.;
        const double C334 = C332 + C330;
        const double C335 = C334 + C329;
        const double C336 = C335 + C328;
        const double C337 = 0.062182 * C336;
        const double C338 = 1. / C337;
        const double C339 = C338 + 1.;
        const double C340 = std::log(C339);
        const double C341 = C333 * C340;
        const double C342 = 0.062182 * C341;

        result.exc =
            rhoA * ((C342 - 0.03109 * (0.20548 * C325 + 1.) *
                                std::log(1. / (0.03109 *
                                               (((14.1189 * C331 + 6.1977 * C325) + 3.3662 * C324) +
                                                0.62517 * C326)) +
                                         1.)) *
                        ((std::pow(C322 + 1., C320) + std::pow(1. - C322, C320)) - 2.) /
                        (2. * (std::pow(2., C318) - 1.)) -
                    C342);
    }

    if (rhoA >= eps)
    {
        const double C237 = rhoA + rhoB;
        const double C238 = rhoA - rhoB;
        const double C239 = 1. / 3.;
        const double C240 = 2. / 3.;
        const double C241 = 4. / 3.;
        const double C242 = 0.9999999999999999 * C238;
        const double C243 = Pi * C237;
        const double C244 = C238 / C237;
        const double C245 = std::pow(2., C239);
        const double C246 = 4. * C243;
        const double C247 = C245 - 1.;
        const double C248 = C242 / C237;
        const double C249 = std::pow(C244, 4);
        const double C250 = C248 + 1.;
        const double C251 = 1. - C248;
        const double C252 = 3. / C246;
        const double C254 = std::pow(C250, C241);
        const double C255 = std::pow(C251, C241);
        const double C256 = std::pow(C252, C239);
        const double C257 = std::pow(C252, C240);
        const double C258 = 0.2137 * C256;
        const double C259 = 0.49294 * C257;
        const double C260 = 1.6382 * std::sqrt(C252);
        const double C261 = 3.5876 * C256;
        const double C262 = C254 + C255;
        const double C264 = 7.5957 * std::sqrt(C256);
        const double C265 = C258 + 1.;
        const double C266 = C262 - 2.;
        const double C267 = C264 + C261;
        const double C268 = C267 + C260;
        const double C269 = C268 + C259;
        const double C270 = 0.062182 * C269;
        const double C271 = 1. / C270;
        const double C272 = C271 + 1.;
        const double C273 = std::log(C272);
        const double C274 = C265 * C273;
        const double C275 = 0.062182 * C274;
        const double C344 = 1;
        const double C345 = 1;
        const double C346 = 0.9999999999999999 * C345 + 0.;
        const double C347 = Pi * C344;
        const double C348 = C345 / C237 - C344 * C238 / std::pow(C237, 2);
        const double C349 = 4. * C347;
        const double C350 = C346 / C237 - C344 * C242 / std::pow(C237, 2);
        const double C351 = 4 * std::pow(C244, 3) * C348;
        const double C352 = C350;
        const double C353 = -C350;
        const double C354 = -3. * C349 / std::pow(C246, 2);
        const double C355 = C241 * std::pow(C250, C241 - 1) * C352;
        const double C356 = C241 * std::pow(C251, C241 - 1) * C353;
        const double C357 = C239 * std::pow(C252, C239 - 1) * C354;
        const double C358 = C240 * std::pow(C252, C240 - 1) * C354;
        const double C359 = 0.2137 * C357;
        const double C360 = 0.49294 * C358;
        const double C361 = 1.6382 * C354 / (2 * std::sqrt(C252));
        const double C362 = 3.5876 * C357;
        const double C363 = C355 + C356;
        const double C364 = 7.5957 * C357 / (2 * std::sqrt(C256));
        const double C365 = C359;
        const double C366 = C363;
        const double C367 = C364 + C362;
        const double C368 = C367 + C361;
        const double C369 = C368 + C360;
        const double C370 = 0.062182 * C369;
        const double C371 = (-C370) / std::pow(C270, 2);
        const double C372 = C371;
        const double C373 = C372 / C272;
        const double C374 = C273 * C365 + C265 * C373;
        const double C375 = 0.062182 * C374;

        result.vrhoA =
            (((((((0.033774 * (0.11125 * C256 + 1.) *
                       std::log(1. / (0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                   0.88026 * std::sqrt(C252)) +
                                                  0.49671 * C257)) +
                                1.) *
                       C266 * (1. - C249) / (3.4198418683227306 * C247) -
                   C275) +
                  C249 *
                      (C275 -
                       0.03109 * (0.20548 * C256 + 1.) *
                           std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                                      3.3662 * std::sqrt(C252)) +
                                                     0.62517 * C257)) +
                                    1.)) *
                      C266 / (2. * C247)) *
                     C344 +
                 C237 *
                     (0.033774 *
                          (0.11125 * std::log(1. / (0.033774 *
                                                    (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                      0.88026 * std::sqrt(C252)) +
                                                     0.49671 * C257)) +
                                              1.) -
                           (0.11125 * C256 + 1.) * 0.033774 *
                               (10.357 / (2 * std::sqrt(C256)) + 3.6231) /
                               (std::pow(0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                      0.88026 * std::sqrt(C252)) +
                                                     0.49671 * C257),
                                         2) *
                                (1. / (0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                    0.88026 * std::sqrt(C252)) +
                                                   0.49671 * C257)) +
                                 1.))) *
                          C266 * (1. - C249) / (3.4198418683227306 * C247) -
                      C249 * 0.03109 *
                          (0.20548 * std::log(1. / (0.03109 *
                                                    (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                                      3.3662 * std::sqrt(C252)) +
                                                     0.62517 * C257)) +
                                              1.) -
                           (0.20548 * C256 + 1.) * 0.03109 *
                               (0.141189e2 / (2 * std::sqrt(C256)) + 6.1977) /
                               (std::pow(0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                                     3.3662 * std::sqrt(C252)) +
                                                    0.62517 * C257),
                                         2) *
                                (1. / (0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                                   3.3662 * std::sqrt(C252)) +
                                                  0.62517 * C257)) +
                                 1.))) *
                          C266 / (2. * C247)) *
                     C357) +
                C237 *
                    (0.033774 * (1. - C249) * C266 * -0.02972990124 * (0.11125 * C256 + 1.) /
                         (2 * std::sqrt(C252) *
                          std::pow(0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                0.88026 * std::sqrt(C252)) +
                                               0.49671 * C257),
                                   2) *
                          (1. / (0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                              0.88026 * std::sqrt(C252)) +
                                             0.49671 * C257)) +
                           1.) *
                          3.4198418683227306 * C247) -
                     C249 * C266 * -0.00325372886222 * (0.20548 * C256 + 1.) /
                         (2 * std::sqrt(C252) *
                          std::pow(0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                               3.3662 * std::sqrt(C252)) +
                                              0.62517 * C257),
                                   2) *
                          (1. / (0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                             3.3662 * std::sqrt(C252)) +
                                            0.62517 * C257)) +
                           1.) *
                          2. * C247)) *
                    C354) +
               C237 *
                   (0.033774 * (1. - C249) * C266 * -0.01677588354 * (0.11125 * C256 + 1.) /
                        (std::pow(0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                               0.88026 * std::sqrt(C252)) +
                                              0.49671 * C257),
                                  2) *
                         (1. / (0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                             0.88026 * std::sqrt(C252)) +
                                            0.49671 * C257)) +
                          1.) *
                         3.4198418683227306 * C247) -
                    C249 * C266 * -0.000604281882477 * (0.20548 * C256 + 1.) /
                        (std::pow(0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                              3.3662 * std::sqrt(C252)) +
                                             0.62517 * C257),
                                  2) *
                         (1. / (0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                            3.3662 * std::sqrt(C252)) +
                                           0.62517 * C257)) +
                          1.) *
                         2. * C247)) *
                   C358) +
              C237 *
                  (0.033774 * (0.11125 * C256 + 1.) *
                       std::log(1. / (0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                   0.88026 * std::sqrt(C252)) +
                                                  0.49671 * C257)) +
                                1.) *
                       (1. - C249) / (3.4198418683227306 * C247) +
                   C249 *
                       (C275 -
                        0.03109 * (0.20548 * C256 + 1.) *
                            std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                                       3.3662 * std::sqrt(C252)) +
                                                      0.62517 * C257)) +
                                     1.)) /
                       (2. * C247)) *
                  C366) +
             C237 *
                 (-0.033774 * (0.11125 * C256 + 1.) *
                      std::log(1. / (0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                  0.88026 * std::sqrt(C252)) +
                                                 0.49671 * C257)) +
                               1.) *
                      C266 / (3.4198418683227306 * C247) +
                  (C275 -
                   0.03109 * (0.20548 * C256 + 1.) *
                       std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                                  3.3662 * std::sqrt(C252)) +
                                                 0.62517 * C257)) +
                                1.)) *
                      C266 / (2. * C247)) *
                 C351) +
            C237 * (C249 * C266 / (2. * C247) - 1) * C375;
    }

    if (rhoB >= eps)
    {
        const double C237 = rhoA + rhoB;
        const double C238 = rhoA - rhoB;
        const double C239 = 1. / 3.;
        const double C240 = 2. / 3.;
        const double C241 = 4. / 3.;
        const double C242 = 0.9999999999999999 * C238;
        const double C243 = Pi * C237;
        const double C244 = C238 / C237;
        const double C245 = std::pow(2., C239);
        const double C246 = 4. * C243;
        const double C247 = C245 - 1.;
        const double C248 = C242 / C237;
        const double C249 = std::pow(C244, 4);
        const double C250 = C248 + 1.;
        const double C251 = 1. - C248;
        const double C252 = 3. / C246;
        const double C254 = std::pow(C250, C241);
        const double C255 = std::pow(C251, C241);
        const double C256 = std::pow(C252, C239);
        const double C257 = std::pow(C252, C240);
        const double C258 = 0.2137 * C256;
        const double C259 = 0.49294 * C257;
        const double C260 = 1.6382 * std::sqrt(C252);
        const double C261 = 3.5876 * C256;
        const double C262 = C254 + C255;
        const double C264 = 7.5957 * std::sqrt(C256);
        const double C265 = C258 + 1.;
        const double C266 = C262 - 2.;
        const double C267 = C264 + C261;
        const double C268 = C267 + C260;
        const double C269 = C268 + C259;
        const double C270 = 0.062182 * C269;
        const double C271 = 1. / C270;
        const double C272 = C271 + 1.;
        const double C273 = std::log(C272);
        const double C274 = C265 * C273;
        const double C275 = 0.062182 * C274;
        const double C376 = 1;
        const double C377 = -1;
        const double C378 = 0.9999999999999999 * C377 + 0.;
        const double C379 = Pi * C376;
        const double C380 = C377 / C237 - C376 * C238 / std::pow(C237, 2);
        const double C381 = 4. * C379;
        const double C382 = C378 / C237 - C376 * C242 / std::pow(C237, 2);
        const double C383 = 4 * std::pow(C244, 3) * C380;
        const double C384 = C382;
        const double C385 = -C382;
        const double C386 = -3. * C381 / std::pow(C246, 2);
        const double C387 = C241 * std::pow(C250, C241 - 1) * C384;
        const double C388 = C241 * std::pow(C251, C241 - 1) * C385;
        const double C389 = C239 * std::pow(C252, C239 - 1) * C386;
        const double C390 = C240 * std::pow(C252, C240 - 1) * C386;
        const double C391 = 0.2137 * C389;
        const double C392 = 0.49294 * C390;
        const double C393 = 1.6382 * C386 / (2 * std::sqrt(C252));
        const double C394 = 3.5876 * C389;
        const double C395 = C387 + C388;
        const double C396 = 7.5957 * C389 / (2 * std::sqrt(C256));
        const double C397 = C391;
        const double C398 = C395;
        const double C399 = C396 + C394;
        const double C400 = C399 + C393;
        const double C401 = C400 + C392;
        const double C402 = 0.062182 * C401;
        const double C403 = (-C402) / std::pow(C270, 2);
        const double C404 = C403;
        const double C405 = C404 / C272;
        const double C406 = C273 * C397 + C265 * C405;
        const double C407 = 0.062182 * C406;

        result.vrhoB =
            (((((((0.033774 * (0.11125 * C256 + 1.) *
                       std::log(1. / (0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                   0.88026 * std::sqrt(C252)) +
                                                  0.49671 * C257)) +
                                1.) *
                       C266 * (1. - C249) / (3.4198418683227306 * C247) -
                   C275) +
                  C249 *
                      (C275 -
                       0.03109 * (0.20548 * C256 + 1.) *
                           std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                                      3.3662 * std::sqrt(C252)) +
                                                     0.62517 * C257)) +
                                    1.)) *
                      C266 / (2. * C247)) *
                     C376 +
                 C237 *
                     (0.033774 *
                          (0.11125 * std::log(1. / (0.033774 *
                                                    (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                      0.88026 * std::sqrt(C252)) +
                                                     0.49671 * C257)) +
                                              1.) -
                           (0.11125 * C256 + 1.) * 0.033774 *
                               (10.357 / (2 * std::sqrt(C256)) + 3.6231) /
                               (std::pow(0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                      0.88026 * std::sqrt(C252)) +
                                                     0.49671 * C257),
                                         2) *
                                (1. / (0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                    0.88026 * std::sqrt(C252)) +
                                                   0.49671 * C257)) +
                                 1.))) *
                          C266 * (1. - C249) / (3.4198418683227306 * C247) -
                      C249 * 0.03109 *
                          (0.20548 * std::log(1. / (0.03109 *
                                                    (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                                      3.3662 * std::sqrt(C252)) +
                                                     0.62517 * C257)) +
                                              1.) -
                           (0.20548 * C256 + 1.) * 0.03109 *
                               (0.141189e2 / (2 * std::sqrt(C256)) + 6.1977) /
                               (std::pow(0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                                     3.3662 * std::sqrt(C252)) +
                                                    0.62517 * C257),
                                         2) *
                                (1. / (0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                                   3.3662 * std::sqrt(C252)) +
                                                  0.62517 * C257)) +
                                 1.))) *
                          C266 / (2. * C247)) *
                     C389) +
                C237 *
                    (0.033774 * (1. - C249) * C266 * -0.02972990124 * (0.11125 * C256 + 1.) /
                         (2 * std::sqrt(C252) *
                          std::pow(0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                0.88026 * std::sqrt(C252)) +
                                               0.49671 * C257),
                                   2) *
                          (1. / (0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                              0.88026 * std::sqrt(C252)) +
                                             0.49671 * C257)) +
                           1.) *
                          3.4198418683227306 * C247) -
                     C249 * C266 * -0.00325372886222 * (0.20548 * C256 + 1.) /
                         (2 * std::sqrt(C252) *
                          std::pow(0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                               3.3662 * std::sqrt(C252)) +
                                              0.62517 * C257),
                                   2) *
                          (1. / (0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                             3.3662 * std::sqrt(C252)) +
                                            0.62517 * C257)) +
                           1.) *
                          2. * C247)) *
                    C386) +
               C237 *
                   (0.033774 * (1. - C249) * C266 * -0.01677588354 * (0.11125 * C256 + 1.) /
                        (std::pow(0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                               0.88026 * std::sqrt(C252)) +
                                              0.49671 * C257),
                                  2) *
                         (1. / (0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                             0.88026 * std::sqrt(C252)) +
                                            0.49671 * C257)) +
                          1.) *
                         3.4198418683227306 * C247) -
                    C249 * C266 * -0.000604281882477 * (0.20548 * C256 + 1.) /
                        (std::pow(0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                              3.3662 * std::sqrt(C252)) +
                                             0.62517 * C257),
                                  2) *
                         (1. / (0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                            3.3662 * std::sqrt(C252)) +
                                           0.62517 * C257)) +
                          1.) *
                         2. * C247)) *
                   C390) +
              C237 *
                  (0.033774 * (0.11125 * C256 + 1.) *
                       std::log(1. / (0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                   0.88026 * std::sqrt(C252)) +
                                                  0.49671 * C257)) +
                                1.) *
                       (1. - C249) / (3.4198418683227306 * C247) +
                   C249 *
                       (C275 -
                        0.03109 * (0.20548 * C256 + 1.) *
                            std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                                       3.3662 * std::sqrt(C252)) +
                                                      0.62517 * C257)) +
                                     1.)) /
                       (2. * C247)) *
                  C398) +
             C237 *
                 (-0.033774 * (0.11125 * C256 + 1.) *
                      std::log(1. / (0.033774 * (((10.357 * std::sqrt(C256) + 3.6231 * C256) +
                                                  0.88026 * std::sqrt(C252)) +
                                                 0.49671 * C257)) +
                               1.) *
                      C266 / (3.4198418683227306 * C247) +
                  (C275 -
                   0.03109 * (0.20548 * C256 + 1.) *
                       std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C256) + 6.1977 * C256) +
                                                  3.3662 * std::sqrt(C252)) +
                                                 0.62517 * C257)) +
                                1.)) *
                      C266 / (2. * C247)) *
                 C383) +
            C237 * (C249 * C266 / (2. * C247) - 1) * C407;
    }

    return result;
}

void Pw92CorrelationSecondDerivatives(double rhoA,
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
    const double C89 = 1;
    const double C90 = 1;
    const double C91 = 0.9999999999999999 * C90 + 0.;
    const double C92 = Pi * C89;
    const double C93 = C90 / C46 - C89 * C47 / std::pow(C46, 2);
    const double C94 = 4. * C92;
    const double C95 = C91 / C46 - C89 * C51 / std::pow(C46, 2);
    const double C96 = 4 * std::pow(C53, 3) * C93;
    const double C97 = C95;
    const double C98 = -C95;
    const double C99 = -3. * C94 / std::pow(C55, 2);
    const double C100 = C50 * std::pow(C59, C50 - 1) * C97;
    const double C101 = C50 * std::pow(C60, C50 - 1) * C98;
    const double C102 = C48 * std::pow(C61, C48 - 1) * C99;
    const double C103 = C49 * std::pow(C61, C49 - 1) * C99;
    const double C104 = 0.2137 * C102;
    const double C105 = 0.49294 * C103;
    const double C106 = 1.6382 * C99 / (2 * std::sqrt(C61));
    const double C107 = 3.5876 * C102;
    const double C108 = C100 + C101;
    const double C109 = 7.5957 * C102 / (2 * std::sqrt(C65));
    const double C110 = C104;
    const double C111 = C108;
    const double C112 = C109 + C107;
    const double C113 = C112 + C106;
    const double C114 = C113 + C105;
    const double C115 = 0.062182 * C114;
    const double C116 = (-C115) / std::pow(C79, 2);
    const double C117 = C116;
    const double C118 = C117 / C81;
    const double C119 = C82 * C110 + C74 * C118;
    const double C120 = 0.062182 * C119;
    const double C153 = 0.;
    const double C154 =
        ((-C90) / std::pow(C46, 2) - (-C89 * C47 * 2 * C46) / std::pow(C46, 4)) * C89 -
        C90 * C89 / std::pow(C46, 2);
    const double C155 =
        (C153 / C46 +
         ((-C91) / std::pow(C46, 2) - (-C89 * C51 * 2 * C46) / std::pow(C46, 4)) * C89) -
        C91 * C89 / std::pow(C46, 2);
    const double C156 = 12 * std::pow(C53, 2) * std::pow(C93, 2) + 4 * std::pow(C53, 3) * C154;
    const double C157 = C155;
    const double C158 = -C155;
    const double C159 = (-C94 * -3. * C94 * 2 * C55) / std::pow(C55, 4);
    const double C160 = C50 * (C50 - 1) * std::pow(C59, C50 - 2) * std::pow(C97, 2) +
                        C50 * std::pow(C59, C50 - 1) * C157;
    const double C161 = C50 * (C50 - 1) * std::pow(C60, C50 - 2) * std::pow(C98, 2) +
                        C50 * std::pow(C60, C50 - 1) * C158;
    const double C162 = C48 * (C48 - 1) * std::pow(C61, C48 - 2) * std::pow(C99, 2) +
                        C48 * std::pow(C61, C48 - 1) * C159;
    const double C163 = C49 * (C49 - 1) * std::pow(C61, C49 - 2) * std::pow(C99, 2) +
                        C49 * std::pow(C61, C49 - 1) * C159;
    const double C164 = 0.2137 * C162;
    const double C165 = 0.49294 * C163;
    const double C166 =
        1.6382 * C159 / (2 * std::sqrt(C61)) -
        1.6382 * std::pow(C99, 2) / (std::sqrt(C61) * std::pow(2 * std::sqrt(C61), 2));
    const double C167 = 3.5876 * C162;
    const double C168 = C160 + C161;
    const double C169 =
        7.5957 * C162 / (2 * std::sqrt(C65)) -
        7.5957 * std::pow(C102, 2) / (std::sqrt(C65) * std::pow(2 * std::sqrt(C65), 2));
    const double C170 = C164;
    const double C171 = C168;
    const double C172 = C169 + C167;
    const double C173 = C172 + C166;
    const double C174 = C173 + C165;
    const double C175 = 0.062182 * C174;
    const double C176 = 2 * C79 * std::pow(C115, 2) / std::pow(C79, 4) - C175 / std::pow(C79, 2);
    const double C177 = C176;
    const double C178 = C177 / C81 - std::pow(C117, 2) / std::pow(C81, 2);
    const double C179 = ((C110 * C118 + C82 * C170) + C118 * C110) + C74 * C178;
    const double C180 = 0.062182 * C179;
    matrix.upper[0] =
        (((((((((((((((((0.033774 *
                             (0.11125 * std::log(1. / (0.033774 *
                                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                                 1.) -
                              (0.11125 * C65 + 1.) * 0.033774 *
                                  (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                  (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                   (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.))) *
                             C75 * (1. - C58) / (3.4198418683227306 * C56) -
                         C58 * 0.03109 *
                             (0.20548 * std::log(1. / (0.03109 *
                                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                                 1.) -
                              (0.20548 * C65 + 1.) * 0.03109 *
                                  (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                  (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                   (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.))) *
                             C75 / (2. * C56)) *
                            C89 +
                        C46 *
                            (0.033774 *
                                 (-0.0037573575 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                      (std::pow(0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                                2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.)) -
                                  (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) *
                                       (-0.349797318 * (0.11125 * C65 + 1.) /
                                            (std::sqrt(C65) * std::pow(2 * std::sqrt(C65), 2)) +
                                        0.0037573575 * (10.357 / (2 * std::sqrt(C65)) + 3.6231)) -
                                   (0.11125 * C65 + 1.) * 0.033774 *
                                       (10.357 / (2 * std::sqrt(C65)) + 3.6231) *
                                       (0.067548 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) *
                                            0.033774 *
                                            (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                              0.88026 * std::sqrt(C61)) +
                                             0.49671 * C66) *
                                            (1. / (0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                             1.) -
                                        std::pow(0.033774 *
                                                     (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                                 2) *
                                            0.033774 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                            std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                   3.6231 * C65) +
                                                                  0.88026 * std::sqrt(C61)) +
                                                                 0.49671 * C66),
                                                     2))) /
                                      std::pow(std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                      3.6231 * C65) +
                                                                     0.88026 * std::sqrt(C61)) +
                                                                    0.49671 * C66),
                                                        2) *
                                                   (1. / (0.033774 * (((10.357 * std::sqrt(C65) +
                                                                        3.6231 * C65) +
                                                                       0.88026 * std::sqrt(C61)) +
                                                                      0.49671 * C66)) +
                                                    1.),
                                               2)) *
                                 C75 * (1. - C58) / (3.4198418683227306 * C56) -
                             C58 * 0.03109 *
                                 (-0.0063883732 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                      (std::pow(0.03109 *
                                                    (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                                2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.)) -
                                  (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.) *
                                       (-0.0438956601e1 * (0.20548 * C65 + 1.) /
                                            (std::sqrt(C65) * std::pow(2 * std::sqrt(C65), 2)) +
                                        0.0063883732 *
                                            (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977)) -
                                   (0.20548 * C65 + 1.) * 0.03109 *
                                       (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) *
                                       (0.06218 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) *
                                            0.03109 *
                                            (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                              3.3662 * std::sqrt(C61)) +
                                             0.62517 * C66) *
                                            (1. / (0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66)) +
                                             1.) -
                                        std::pow(0.03109 *
                                                     (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66),
                                                 2) *
                                            0.03109 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                            std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                  6.1977 * C65) +
                                                                 3.3662 * std::sqrt(C61)) +
                                                                0.62517 * C66),
                                                     2))) /
                                      std::pow(std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                     6.1977 * C65) +
                                                                    3.3662 * std::sqrt(C61)) +
                                                                   0.62517 * C66),
                                                        2) *
                                                   (1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                       6.1977 * C65) +
                                                                      3.3662 * std::sqrt(C61)) +
                                                                     0.62517 * C66)) +
                                                    1.),
                                               2)) *
                                 C75 / (2. * C56)) *
                            C102) +
                       C46 *
                           ((2 * std::sqrt(C61) *
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) *
                                 (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                  1.) *
                                 3.4198418683227306 * C56 * 0.033774 * (1. - C58) *
                                 -0.00330745151295 * C75 -
                             0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) *
                                 2 *
                                 (0. * std::sqrt(C61) *
                                      std::pow(0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                               2) *
                                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                       1.) +
                                  (std::sqrt(C61) * 0.067548 *
                                       (10.357 / (2 * std::sqrt(C65)) + 3.6231) * 0.033774 *
                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                         0.88026 * std::sqrt(C61)) +
                                        0.49671 * C66) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) -
                                   std::sqrt(C61) *
                                       std::pow(0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                                2) *
                                       0.033774 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                       std::pow(0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                                2)) *
                                      3.4198418683227306 * C56)) /
                                std::pow(2 * std::sqrt(C61) *
                                             std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                    3.6231 * C65) +
                                                                   0.88026 * std::sqrt(C61)) +
                                                                  0.49671 * C66),
                                                      2) *
                                             (1. / (0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                              1.) *
                                             3.4198418683227306 * C56,
                                         2) -
                            (2 * std::sqrt(C61) *
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) *
                                 (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66)) +
                                  1.) *
                                 2. * C56 * C58 * -0.0006685762066089 * C75 -
                             C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) * 2 *
                                 (std::sqrt(C61) * 0.06218 *
                                      (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) * 0.03109 *
                                      (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                        3.3662 * std::sqrt(C61)) +
                                       0.62517 * C66) *
                                      (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                       1.) -
                                  std::sqrt(C61) *
                                      std::pow(0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                               2) *
                                      0.03109 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                      std::pow(0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                               2)) *
                                 2. * C56) /
                                std::pow(2 * std::sqrt(C61) *
                                             std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                   6.1977 * C65) +
                                                                  3.3662 * std::sqrt(C61)) +
                                                                 0.62517 * C66),
                                                      2) *
                                             (1. / (0.03109 *
                                                    (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                              1.) *
                                             2. * C56,
                                         2)) *
                           C99) +
                      C46 *
                          ((std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66),
                                     2) *
                                (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66)) +
                                 1.) *
                                3.4198418683227306 * C56 * 0.033774 * (1. - C58) *
                                -0.001866317043825 * C75 -
                            0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) *
                                (0. *
                                     std::pow(0.033774 *
                                                  (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                              2) *
                                     (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                      1.) +
                                 (0.067548 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) * 0.033774 *
                                      (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                        0.88026 * std::sqrt(C61)) +
                                       0.49671 * C66) *
                                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                       1.) -
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) *
                                      0.033774 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                      std::pow(0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                               2)) *
                                     3.4198418683227306 * C56)) /
                               std::pow(
                                   std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) *
                                       3.4198418683227306 * C56,
                                   2) -
                           (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66),
                                     2) *
                                (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66)) +
                                 1.) *
                                2. * C56 * C58 * -0.0001241678412113 * C75 -
                            C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) *
                                (0.06218 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) * 0.03109 *
                                     (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                       3.3662 * std::sqrt(C61)) +
                                      0.62517 * C66) *
                                     (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66)) +
                                      1.) -
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) *
                                     0.03109 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                     std::pow(0.03109 *
                                                  (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66),
                                              2)) *
                                2. * C56) /
                               std::pow(
                                   std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.) *
                                       2. * C56,
                                   2)) *
                          C103) +
                     C46 *
                         (0.033774 *
                              (0.11125 * std::log(1. / (0.033774 *
                                                        (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                                  1.) -
                               (0.11125 * C65 + 1.) * 0.033774 *
                                   (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                   (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                             2) *
                                    (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66)) +
                                     1.))) *
                              (1. - C58) / (3.4198418683227306 * C56) -
                          C58 * 0.03109 *
                              (0.20548 * std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                     6.1977 * C65) +
                                                                    3.3662 * std::sqrt(C61)) +
                                                                   0.62517 * C66)) +
                                                  1.) -
                               (0.20548 * C65 + 1.) * 0.03109 *
                                   (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                   (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66),
                                             2) *
                                    (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                     1.))) /
                              (2. * C56)) *
                         C111) +
                    C46 *
                        (-0.033774 *
                             (0.11125 * std::log(1. / (0.033774 *
                                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                                 1.) -
                              (0.11125 * C65 + 1.) * 0.033774 *
                                  (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                  (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                   (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.))) *
                             C75 / (3.4198418683227306 * C56) -
                         0.03109 *
                             (0.20548 * std::log(1. / (0.03109 *
                                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                                 1.) -
                              (0.20548 * C65 + 1.) * 0.03109 *
                                  (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                  (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                   (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.))) *
                             C75 / (2. * C56)) *
                        C96) *
                       C102 +
                   ((((((0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                            C89 +
                        C46 *
                            (0.033774 *
                                 ((0.11125 * C65 + 1.) * 0.033774 *
                                      (10.357 / (2 * std::sqrt(C65)) + 3.6231) *
                                      (-0.02972990124 *
                                           std::pow(0.033774 *
                                                        (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                                    2) /
                                           (2 * std::sqrt(C61) *
                                            std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                   3.6231 * C65) +
                                                                  0.88026 * std::sqrt(C61)) +
                                                                 0.49671 * C66),
                                                     2)) +
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) *
                                           0.0020081953689595 *
                                           (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                             0.88026 * std::sqrt(C61)) +
                                            0.49671 * C66) /
                                           (2 * std::sqrt(C61))) /
                                      std::pow(std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                      3.6231 * C65) +
                                                                     0.88026 * std::sqrt(C61)) +
                                                                    0.49671 * C66),
                                                        2) *
                                                   (1. / (0.033774 * (((10.357 * std::sqrt(C65) +
                                                                        3.6231 * C65) +
                                                                       0.88026 * std::sqrt(C61)) +
                                                                      0.49671 * C66)) +
                                                    1.),
                                               2) -
                                  0.00330745151295 /
                                      (2 * std::sqrt(C61) *
                                       std::pow(0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                                2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.))) *
                                 C75 * (1. - C58) / (3.4198418683227306 * C56) -
                             C58 * 0.03109 *
                                 ((0.20548 * C65 + 1.) * 0.03109 *
                                      (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) *
                                      (-0.104655158 *
                                           std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                 6.1977 * C65) +
                                                                3.3662 * std::sqrt(C61)) +
                                                               0.62517 * C66),
                                                    2) /
                                           (2 * std::sqrt(C61) *
                                            std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                  6.1977 * C65) +
                                                                 3.3662 * std::sqrt(C61)) +
                                                                0.62517 * C66),
                                                     2)) +
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.) *
                                           0.00650745772444 *
                                           (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                             3.3662 * std::sqrt(C61)) +
                                            0.62517 * C66) /
                                           (2 * std::sqrt(C61))) /
                                      std::pow(std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                     6.1977 * C65) +
                                                                    3.3662 * std::sqrt(C61)) +
                                                                   0.62517 * C66),
                                                        2) *
                                                   (1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                       6.1977 * C65) +
                                                                      3.3662 * std::sqrt(C61)) +
                                                                     0.62517 * C66)) +
                                                    1.),
                                               2) -
                                  0.02150454186584 /
                                      (2 * std::sqrt(C61) *
                                       std::pow(0.03109 *
                                                    (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                                2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.))) *
                                 C75 / (2. * C56)) *
                            C102) +
                       C46 *
                           ((-0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) *
                             2 *
                             (0. * std::sqrt(C61) *
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) *
                                  (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                   1.) +
                              (-0.02972990124 * std::sqrt(C61) *
                                   std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) /
                                   (2 * std::sqrt(C61) *
                                    std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                             2)) +
                               (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66)) +
                                1.) *
                                   (std::sqrt(C61) * 0.0020081953689595 *
                                        (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                          0.88026 * std::sqrt(C61)) +
                                         0.49671 * C66) +
                                    std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                             2)) /
                                   (2 * std::sqrt(C61))) *
                                  3.4198418683227306 * C56)) /
                                std::pow(2 * std::sqrt(C61) *
                                             std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                    3.6231 * C65) +
                                                                   0.88026 * std::sqrt(C61)) +
                                                                  0.49671 * C66),
                                                      2) *
                                             (1. / (0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                              1.) *
                                             3.4198418683227306 * C56,
                                         2) -
                            (-C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) * 2 *
                             (-0.104655158 * std::sqrt(C61) *
                                  std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66),
                                           2) /
                                  (2 * std::sqrt(C61) *
                                   std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2)) +
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                                  (std::sqrt(C61) * 0.00650745772444 *
                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                         3.3662 * std::sqrt(C61)) +
                                        0.62517 * C66) +
                                   std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2)) /
                                  (2 * std::sqrt(C61))) *
                             2. * C56) /
                                std::pow(2 * std::sqrt(C61) *
                                             std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                   6.1977 * C65) +
                                                                  3.3662 * std::sqrt(C61)) +
                                                                 0.62517 * C66),
                                                      2) *
                                             (1. / (0.03109 *
                                                    (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                              1.) *
                                             2. * C56,
                                         2)) *
                           C99) +
                      C46 *
                          ((-0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) *
                            (0. *
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) *
                                 (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                  1.) +
                             (-0.02972990124 *
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) /
                                  (2 * std::sqrt(C61) *
                                   std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2)) +
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                                  0.0020081953689595 *
                                  (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                    0.88026 * std::sqrt(C61)) +
                                   0.49671 * C66) /
                                  (2 * std::sqrt(C61))) *
                                 3.4198418683227306 * C56)) /
                               std::pow(
                                   std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) *
                                       3.4198418683227306 * C56,
                                   2) -
                           (-C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) *
                            (-0.104655158 *
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) /
                                 (2 * std::sqrt(C61) *
                                  std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66),
                                           2)) +
                             (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                3.3662 * std::sqrt(C61)) +
                                               0.62517 * C66)) +
                              1.) *
                                 0.00650745772444 *
                                 (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                   3.3662 * std::sqrt(C61)) +
                                  0.62517 * C66) /
                                 (2 * std::sqrt(C61))) *
                            2. * C56) /
                               std::pow(
                                   std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.) *
                                       2. * C56,
                                   2)) *
                          C103) +
                     C46 *
                         (0.033774 * (1. - C58) * -0.02972990124 * (0.11125 * C65 + 1.) /
                              (2 * std::sqrt(C61) *
                               std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                        2) *
                               (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66)) +
                                1.) *
                               3.4198418683227306 * C56) -
                          C58 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                              (2 * std::sqrt(C61) *
                               std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66),
                                        2) *
                               (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66)) +
                                1.) *
                               2. * C56)) *
                         C111) +
                    C46 *
                        (-0.033774 * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                        C96) *
                       C99) +
                  ((((((0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                            (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66),
                                      2) *
                             (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                 0.88026 * std::sqrt(C61)) +
                                                0.49671 * C66)) +
                              1.) *
                             3.4198418683227306 * C56) -
                        C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                            (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66),
                                      2) *
                             (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                3.3662 * std::sqrt(C61)) +
                                               0.62517 * C66)) +
                              1.) *
                             2. * C56)) *
                           C89 +
                       C46 *
                           (0.033774 *
                                ((0.11125 * C65 + 1.) * 0.033774 *
                                     (10.357 / (2 * std::sqrt(C65)) + 3.6231) *
                                     (-0.01677588354 *
                                          std::pow(0.033774 *
                                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                                   2) /
                                          std::pow(0.033774 *
                                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                                   2) +
                                      0.0011331773813599 *
                                          (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                            0.88026 * std::sqrt(C61)) +
                                           0.49671 * C66) *
                                          (1. / (0.033774 *
                                                 (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66)) +
                                           1.)) /
                                     std::pow(std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                     3.6231 * C65) +
                                                                    0.88026 * std::sqrt(C61)) +
                                                                   0.49671 * C66),
                                                       2) *
                                                  (1. / (0.033774 * (((10.357 * std::sqrt(C65) +
                                                                       3.6231 * C65) +
                                                                      0.88026 * std::sqrt(C61)) +
                                                                     0.49671 * C66)) +
                                                   1.),
                                              2) -
                                 0.001866317043825 /
                                     (std::pow(0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                               2) *
                                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                       1.))) *
                                C75 * (1. - C58) / (3.4198418683227306 * C56) -
                            C58 * 0.03109 *
                                ((0.20548 * C65 + 1.) * 0.03109 *
                                     (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) *
                                     (-0.0194365353 *
                                          std::pow(0.03109 *
                                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66),
                                                   2) /
                                          std::pow(0.03109 *
                                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66),
                                                   2) +
                                      0.001208563764954 *
                                          (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                            3.3662 * std::sqrt(C61)) +
                                           0.62517 * C66) *
                                          (1. / (0.03109 *
                                                 (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66)) +
                                           1.)) /
                                     std::pow(std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                    6.1977 * C65) +
                                                                   3.3662 * std::sqrt(C61)) +
                                                                  0.62517 * C66),
                                                       2) *
                                                  (1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                      6.1977 * C65) +
                                                                     3.3662 * std::sqrt(C61)) +
                                                                    0.62517 * C66)) +
                                                   1.),
                                              2) -
                                 0.003993819273444 /
                                     (std::pow(0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                               2) *
                                      (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                       1.))) *
                                C75 / (2. * C56)) *
                           C102) +
                      C46 *
                          ((-0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) *
                            2 *
                            (0. * std::sqrt(C61) *
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) *
                                 (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                  1.) +
                             (-0.01677588354 * std::sqrt(C61) *
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) /
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) +
                              std::sqrt(C61) * 0.0011331773813599 *
                                  (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                    0.88026 * std::sqrt(C61)) +
                                   0.49671 * C66) *
                                  (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                   1.)) *
                                 3.4198418683227306 * C56)) /
                               std::pow(2 * std::sqrt(C61) *
                                            std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                   3.6231 * C65) +
                                                                  0.88026 * std::sqrt(C61)) +
                                                                 0.49671 * C66),
                                                     2) *
                                            (1. / (0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                             1.) *
                                            3.4198418683227306 * C56,
                                        2) -
                           (-C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) * 2 *
                            (-0.0194365353 * std::sqrt(C61) *
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) /
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) +
                             std::sqrt(C61) * 0.001208563764954 *
                                 (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                   3.3662 * std::sqrt(C61)) +
                                  0.62517 * C66) *
                                 (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66)) +
                                  1.)) *
                            2. * C56) /
                               std::pow(2 * std::sqrt(C61) *
                                            std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                  6.1977 * C65) +
                                                                 3.3662 * std::sqrt(C61)) +
                                                                0.62517 * C66),
                                                     2) *
                                            (1. / (0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66)) +
                                             1.) *
                                            2. * C56,
                                        2)) *
                          C99) +
                     C46 *
                         ((-0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) *
                           (0. *
                                std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                         2) *
                                (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66)) +
                                 1.) +
                            (-0.01677588354 *
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) /
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) +
                             0.0011331773813599 *
                                 (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                   0.88026 * std::sqrt(C61)) +
                                  0.49671 * C66) *
                                 (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                  1.)) *
                                3.4198418683227306 * C56)) /
                              std::pow(
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) *
                                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                       1.) *
                                      3.4198418683227306 * C56,
                                  2) -
                          (-C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) *
                           (-0.0194365353 *
                                std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                         2) /
                                std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                         2) +
                            0.001208563764954 *
                                (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                  3.3662 * std::sqrt(C61)) +
                                 0.62517 * C66) *
                                (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66)) +
                                 1.)) *
                           2. * C56) /
                              std::pow(
                                  std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66),
                                           2) *
                                      (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                       1.) *
                                      2. * C56,
                                  2)) *
                         C103) +
                    C46 *
                        (0.033774 * (1. - C58) * -0.01677588354 * (0.11125 * C65 + 1.) /
                             (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C58 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                             (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                        C111) +
                   C46 *
                       (-0.033774 * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                            (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66),
                                      2) *
                             (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                 0.88026 * std::sqrt(C61)) +
                                                0.49671 * C66)) +
                              1.) *
                             3.4198418683227306 * C56) -
                        C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                            (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66),
                                      2) *
                             (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                3.3662 * std::sqrt(C61)) +
                                               0.62517 * C66)) +
                              1.) *
                             2. * C56)) *
                       C96) *
                      C103) +
                 ((((((0.033774 * (0.11125 * C65 + 1.) *
                           std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.) *
                           (1. - C58) / (3.4198418683227306 * C56) +
                       C58 *
                           (C84 - 0.03109 * (0.20548 * C65 + 1.) *
                                      std::log(1. / (0.03109 *
                                                     (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                               1.)) /
                           (2. * C56)) *
                          C89 +
                      C46 *
                          (0.033774 *
                               (0.11125 * std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) +
                                                                       3.6231 * C65) +
                                                                      0.88026 * std::sqrt(C61)) +
                                                                     0.49671 * C66)) +
                                                   1.) -
                                (0.11125 * C65 + 1.) * 0.033774 *
                                    (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                    (std::pow(0.033774 *
                                                  (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                              2) *
                                     (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                      1.))) *
                               (1. - C58) / (3.4198418683227306 * C56) -
                           C58 * 0.03109 *
                               (0.20548 * std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                      6.1977 * C65) +
                                                                     3.3662 * std::sqrt(C61)) +
                                                                    0.62517 * C66)) +
                                                   1.) -
                                (0.20548 * C65 + 1.) * 0.03109 *
                                    (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                    (std::pow(0.03109 *
                                                  (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66),
                                              2) *
                                     (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66)) +
                                      1.))) /
                               (2. * C56)) *
                          C102) +
                     C46 *
                         (0.033774 * (1. - C58) * -0.02972990124 * (0.11125 * C65 + 1.) /
                              (2 * std::sqrt(C61) *
                               std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                        2) *
                               (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66)) +
                                1.) *
                               3.4198418683227306 * C56) -
                          C58 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                              (2 * std::sqrt(C61) *
                               std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66),
                                        2) *
                               (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66)) +
                                1.) *
                               2. * C56)) *
                         C99) +
                    C46 *
                        (0.033774 * (1. - C58) * -0.01677588354 * (0.11125 * C65 + 1.) /
                             (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C58 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                             (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                        C103) +
                   C46 *
                       (-0.033774 * (0.11125 * C65 + 1.) *
                            std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66)) +
                                     1.) /
                            (3.4198418683227306 * C56) +
                        (C84 -
                         0.03109 * (0.20548 * C65 + 1.) *
                             std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66)) +
                                      1.)) /
                            (2. * C56)) *
                       C96) +
                  C120 * C46 * C58 / (2. * C56)) *
                     C111) +
                ((((((-0.033774 * (0.11125 * C65 + 1.) *
                          std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                   1.) *
                          C75 / (3.4198418683227306 * C56) +
                      (C84 -
                       0.03109 * (0.20548 * C65 + 1.) *
                           std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.)) *
                          C75 / (2. * C56)) *
                         C89 +
                     C46 *
                         (-0.033774 *
                              (0.11125 * std::log(1. / (0.033774 *
                                                        (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                                  1.) -
                               (0.11125 * C65 + 1.) * 0.033774 *
                                   (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                   (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                             2) *
                                    (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66)) +
                                     1.))) *
                              C75 / (3.4198418683227306 * C56) -
                          0.03109 *
                              (0.20548 * std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                     6.1977 * C65) +
                                                                    3.3662 * std::sqrt(C61)) +
                                                                   0.62517 * C66)) +
                                                  1.) -
                               (0.20548 * C65 + 1.) * 0.03109 *
                                   (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                   (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66),
                                             2) *
                                    (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                     1.))) *
                              C75 / (2. * C56)) *
                         C102) +
                    C46 *
                        (-0.033774 * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                        C99) +
                   C46 *
                       (-0.033774 * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                            (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66),
                                      2) *
                             (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                 0.88026 * std::sqrt(C61)) +
                                                0.49671 * C66)) +
                              1.) *
                             3.4198418683227306 * C56) -
                        C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                            (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66),
                                      2) *
                             (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                3.3662 * std::sqrt(C61)) +
                                               0.62517 * C66)) +
                              1.) *
                             2. * C56)) *
                       C103) +
                  C46 *
                      (-0.033774 * (0.11125 * C65 + 1.) *
                           std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.) /
                           (3.4198418683227306 * C56) +
                       (C84 -
                        0.03109 * (0.20548 * C65 + 1.) *
                            std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                     1.)) /
                           (2. * C56)) *
                      C111) +
                 C120 * C46 * C75 / (2. * C56)) *
                    C96) +
               (((C58 * C75 / (2. * C56) - 1) * C89 + C111 * C46 * C58 / (2. * C56)) +
                C96 * C46 * C75 / (2. * C56)) *
                   C120) +
              ((((((0.033774 *
                        (0.11125 *
                             std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                      1.) -
                         (0.11125 * C65 + 1.) * 0.033774 *
                             (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                             (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.))) *
                        C75 * (1. - C58) / (3.4198418683227306 * C56) -
                    C58 * 0.03109 *
                        (0.20548 *
                             std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66)) +
                                      1.) -
                         (0.20548 * C65 + 1.) * 0.03109 *
                             (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                             (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.))) *
                        C75 / (2. * C56)) *
                       C102 +
                   (0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                        (2 * std::sqrt(C61) *
                         std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                               0.88026 * std::sqrt(C61)) +
                                              0.49671 * C66),
                                  2) *
                         (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                             0.88026 * std::sqrt(C61)) +
                                            0.49671 * C66)) +
                          1.) *
                         3.4198418683227306 * C56) -
                    C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                        (2 * std::sqrt(C61) *
                         std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                              3.3662 * std::sqrt(C61)) +
                                             0.62517 * C66),
                                  2) *
                         (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                            3.3662 * std::sqrt(C61)) +
                                           0.62517 * C66)) +
                          1.) *
                         2. * C56)) *
                       C99) +
                  (0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                       (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                              0.88026 * std::sqrt(C61)) +
                                             0.49671 * C66),
                                 2) *
                        (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                            0.88026 * std::sqrt(C61)) +
                                           0.49671 * C66)) +
                         1.) *
                        3.4198418683227306 * C56) -
                   C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                       (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                             3.3662 * std::sqrt(C61)) +
                                            0.62517 * C66),
                                 2) *
                        (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                           3.3662 * std::sqrt(C61)) +
                                          0.62517 * C66)) +
                         1.) *
                        2. * C56)) *
                      C103) +
                 (0.033774 * (0.11125 * C65 + 1.) *
                      std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                      (1. - C58) / (3.4198418683227306 * C56) +
                  C58 *
                      (C84 -
                       0.03109 * (0.20548 * C65 + 1.) *
                           std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.)) /
                      (2. * C56)) *
                     C111) +
                (-0.033774 * (0.11125 * C65 + 1.) *
                     std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                 0.88026 * std::sqrt(C61)) +
                                                0.49671 * C66)) +
                              1.) *
                     C75 / (3.4198418683227306 * C56) +
                 (C84 - 0.03109 * (0.20548 * C65 + 1.) *
                            std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                     1.)) *
                     C75 / (2. * C56)) *
                    C96) +
               (C58 * C75 / (2. * C56) - 1) * C120) *
                  C89) +
             C46 *
                 (0.033774 *
                      (0.11125 *
                           std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.) -
                       (0.11125 * C65 + 1.) * 0.033774 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                           (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66),
                                     2) *
                            (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                0.88026 * std::sqrt(C61)) +
                                               0.49671 * C66)) +
                             1.))) *
                      C75 * (1. - C58) / (3.4198418683227306 * C56) -
                  C58 * 0.03109 *
                      (0.20548 *
                           std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.) -
                       (0.20548 * C65 + 1.) * 0.03109 *
                           (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                           (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66),
                                     2) *
                            (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                               3.3662 * std::sqrt(C61)) +
                                              0.62517 * C66)) +
                             1.))) *
                      C75 / (2. * C56)) *
                 C162) +
            C46 *
                (0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                     (2 * std::sqrt(C61) *
                      std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                            0.88026 * std::sqrt(C61)) +
                                           0.49671 * C66),
                               2) *
                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                          0.88026 * std::sqrt(C61)) +
                                         0.49671 * C66)) +
                       1.) *
                      3.4198418683227306 * C56) -
                 C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                     (2 * std::sqrt(C61) *
                      std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                           3.3662 * std::sqrt(C61)) +
                                          0.62517 * C66),
                               2) *
                      (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                         3.3662 * std::sqrt(C61)) +
                                        0.62517 * C66)) +
                       1.) *
                      2. * C56)) *
                C159) +
           C46 *
               (0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                    (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                           0.88026 * std::sqrt(C61)) +
                                          0.49671 * C66),
                              2) *
                     (1. / (0.033774 *
                            (((10.357 * std::sqrt(C65) + 3.6231 * C65) + 0.88026 * std::sqrt(C61)) +
                             0.49671 * C66)) +
                      1.) *
                     3.4198418683227306 * C56) -
                C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                    (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                          3.3662 * std::sqrt(C61)) +
                                         0.62517 * C66),
                              2) *
                     (1. / (0.03109 *
                            (((14.1189 * std::sqrt(C65) + 6.1977 * C65) + 3.3662 * std::sqrt(C61)) +
                             0.62517 * C66)) +
                      1.) *
                     2. * C56)) *
               C163) +
          C46 *
              (0.033774 * (0.11125 * C65 + 1.) *
                   std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                               0.88026 * std::sqrt(C61)) +
                                              0.49671 * C66)) +
                            1.) *
                   (1. - C58) / (3.4198418683227306 * C56) +
               C58 *
                   (C84 - 0.03109 * (0.20548 * C65 + 1.) *
                              std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                       1.)) /
                   (2. * C56)) *
              C171) +
         C46 *
             (-0.033774 * (0.11125 * C65 + 1.) *
                  std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                              0.88026 * std::sqrt(C61)) +
                                             0.49671 * C66)) +
                           1.) *
                  C75 / (3.4198418683227306 * C56) +
              (C84 - 0.03109 * (0.20548 * C65 + 1.) *
                         std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66)) +
                                  1.)) *
                  C75 / (2. * C56)) *
             C156) +
        C46 * (C58 * C75 / (2. * C56) - 1) * C180;

    const double C121 = 1;
    const double C122 = -1;
    const double C123 = 0.9999999999999999 * C122 + 0.;
    const double C124 = Pi * C121;
    const double C125 = C122 / C46 - C121 * C47 / std::pow(C46, 2);
    const double C126 = 4. * C124;
    const double C127 = C123 / C46 - C121 * C51 / std::pow(C46, 2);
    const double C128 = 4 * std::pow(C53, 3) * C125;
    const double C129 = C127;
    const double C130 = -C127;
    const double C131 = -3. * C126 / std::pow(C55, 2);
    const double C132 = C50 * std::pow(C59, C50 - 1) * C129;
    const double C133 = C50 * std::pow(C60, C50 - 1) * C130;
    const double C134 = C48 * std::pow(C61, C48 - 1) * C131;
    const double C135 = C49 * std::pow(C61, C49 - 1) * C131;
    const double C136 = 0.2137 * C134;
    const double C137 = 0.49294 * C135;
    const double C138 = 1.6382 * C131 / (2 * std::sqrt(C61));
    const double C139 = 3.5876 * C134;
    const double C140 = C132 + C133;
    const double C141 = 7.5957 * C134 / (2 * std::sqrt(C65));
    const double C142 = C136;
    const double C143 = C140;
    const double C144 = C141 + C139;
    const double C145 = C144 + C138;
    const double C146 = C145 + C137;
    const double C147 = 0.062182 * C146;
    const double C148 = (-C147) / std::pow(C79, 2);
    const double C149 = C148;
    const double C150 = C149 / C81;
    const double C151 = C82 * C142 + C74 * C150;
    const double C152 = 0.062182 * C151;
    const double C181 = 0.;
    const double C182 =
        ((-C90) / std::pow(C46, 2) - (-C89 * C47 * 2 * C46) / std::pow(C46, 4)) * C121 -
        C122 * C89 / std::pow(C46, 2);
    const double C183 =
        (C181 / C46 +
         ((-C91) / std::pow(C46, 2) - (-C89 * C51 * 2 * C46) / std::pow(C46, 4)) * C121) -
        C123 * C89 / std::pow(C46, 2);
    const double C184 = 12 * std::pow(C53, 2) * C93 * C125 + 4 * std::pow(C53, 3) * C182;
    const double C185 = C183;
    const double C186 = -C183;
    const double C187 = (-C126 * -3. * C94 * 2 * C55) / std::pow(C55, 4);
    const double C188 =
        C50 * (C50 - 1) * std::pow(C59, C50 - 2) * C97 * C129 + C50 * std::pow(C59, C50 - 1) * C185;
    const double C189 =
        C50 * (C50 - 1) * std::pow(C60, C50 - 2) * C98 * C130 + C50 * std::pow(C60, C50 - 1) * C186;
    const double C190 =
        C48 * (C48 - 1) * std::pow(C61, C48 - 2) * C99 * C131 + C48 * std::pow(C61, C48 - 1) * C187;
    const double C191 =
        C49 * (C49 - 1) * std::pow(C61, C49 - 2) * C99 * C131 + C49 * std::pow(C61, C49 - 1) * C187;
    const double C192 = 0.2137 * C190;
    const double C193 = 0.49294 * C191;
    const double C194 = 1.6382 * C187 / (2 * std::sqrt(C61)) -
                        C131 * 1.6382 * C99 / (std::sqrt(C61) * std::pow(2 * std::sqrt(C61), 2));
    const double C195 = 3.5876 * C190;
    const double C196 = C188 + C189;
    const double C197 = 7.5957 * C190 / (2 * std::sqrt(C65)) -
                        C134 * 7.5957 * C102 / (std::sqrt(C65) * std::pow(2 * std::sqrt(C65), 2));
    const double C198 = C192;
    const double C199 = C196;
    const double C200 = C197 + C195;
    const double C201 = C200 + C194;
    const double C202 = C201 + C193;
    const double C203 = 0.062182 * C202;
    const double C204 = C147 * C115 * 2 * C79 / std::pow(C79, 4) - C203 / std::pow(C79, 2);
    const double C205 = C204;
    const double C206 = C205 / C81 - C149 * C117 / std::pow(C81, 2);
    const double C207 = ((C110 * C150 + C82 * C198) + C118 * C142) + C74 * C206;
    const double C208 = 0.062182 * C207;
    matrix.upper[1] =
        (((((((((((((((((0.033774 *
                             (0.11125 * std::log(1. / (0.033774 *
                                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                                 1.) -
                              (0.11125 * C65 + 1.) * 0.033774 *
                                  (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                  (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                   (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.))) *
                             C75 * (1. - C58) / (3.4198418683227306 * C56) -
                         C58 * 0.03109 *
                             (0.20548 * std::log(1. / (0.03109 *
                                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                                 1.) -
                              (0.20548 * C65 + 1.) * 0.03109 *
                                  (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                  (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                   (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.))) *
                             C75 / (2. * C56)) *
                            C89 +
                        C46 *
                            (0.033774 *
                                 (-0.0037573575 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                      (std::pow(0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                                2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.)) -
                                  (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) *
                                       (-0.349797318 * (0.11125 * C65 + 1.) /
                                            (std::sqrt(C65) * std::pow(2 * std::sqrt(C65), 2)) +
                                        0.0037573575 * (10.357 / (2 * std::sqrt(C65)) + 3.6231)) -
                                   (0.11125 * C65 + 1.) * 0.033774 *
                                       (10.357 / (2 * std::sqrt(C65)) + 3.6231) *
                                       (0.067548 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) *
                                            0.033774 *
                                            (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                              0.88026 * std::sqrt(C61)) +
                                             0.49671 * C66) *
                                            (1. / (0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                             1.) -
                                        std::pow(0.033774 *
                                                     (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                                 2) *
                                            0.033774 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                            std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                   3.6231 * C65) +
                                                                  0.88026 * std::sqrt(C61)) +
                                                                 0.49671 * C66),
                                                     2))) /
                                      std::pow(std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                      3.6231 * C65) +
                                                                     0.88026 * std::sqrt(C61)) +
                                                                    0.49671 * C66),
                                                        2) *
                                                   (1. / (0.033774 * (((10.357 * std::sqrt(C65) +
                                                                        3.6231 * C65) +
                                                                       0.88026 * std::sqrt(C61)) +
                                                                      0.49671 * C66)) +
                                                    1.),
                                               2)) *
                                 C75 * (1. - C58) / (3.4198418683227306 * C56) -
                             C58 * 0.03109 *
                                 (-0.0063883732 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                      (std::pow(0.03109 *
                                                    (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                                2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.)) -
                                  (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.) *
                                       (-0.0438956601e1 * (0.20548 * C65 + 1.) /
                                            (std::sqrt(C65) * std::pow(2 * std::sqrt(C65), 2)) +
                                        0.0063883732 *
                                            (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977)) -
                                   (0.20548 * C65 + 1.) * 0.03109 *
                                       (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) *
                                       (0.06218 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) *
                                            0.03109 *
                                            (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                              3.3662 * std::sqrt(C61)) +
                                             0.62517 * C66) *
                                            (1. / (0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66)) +
                                             1.) -
                                        std::pow(0.03109 *
                                                     (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66),
                                                 2) *
                                            0.03109 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                            std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                  6.1977 * C65) +
                                                                 3.3662 * std::sqrt(C61)) +
                                                                0.62517 * C66),
                                                     2))) /
                                      std::pow(std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                     6.1977 * C65) +
                                                                    3.3662 * std::sqrt(C61)) +
                                                                   0.62517 * C66),
                                                        2) *
                                                   (1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                       6.1977 * C65) +
                                                                      3.3662 * std::sqrt(C61)) +
                                                                     0.62517 * C66)) +
                                                    1.),
                                               2)) *
                                 C75 / (2. * C56)) *
                            C102) +
                       C46 *
                           ((2 * std::sqrt(C61) *
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) *
                                 (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                  1.) *
                                 3.4198418683227306 * C56 * 0.033774 * (1. - C58) *
                                 -0.00330745151295 * C75 -
                             0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) *
                                 2 *
                                 (0. * std::sqrt(C61) *
                                      std::pow(0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                               2) *
                                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                       1.) +
                                  (std::sqrt(C61) * 0.067548 *
                                       (10.357 / (2 * std::sqrt(C65)) + 3.6231) * 0.033774 *
                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                         0.88026 * std::sqrt(C61)) +
                                        0.49671 * C66) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) -
                                   std::sqrt(C61) *
                                       std::pow(0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                                2) *
                                       0.033774 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                       std::pow(0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                                2)) *
                                      3.4198418683227306 * C56)) /
                                std::pow(2 * std::sqrt(C61) *
                                             std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                    3.6231 * C65) +
                                                                   0.88026 * std::sqrt(C61)) +
                                                                  0.49671 * C66),
                                                      2) *
                                             (1. / (0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                              1.) *
                                             3.4198418683227306 * C56,
                                         2) -
                            (2 * std::sqrt(C61) *
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) *
                                 (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66)) +
                                  1.) *
                                 2. * C56 * C58 * -0.0006685762066089 * C75 -
                             C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) * 2 *
                                 (std::sqrt(C61) * 0.06218 *
                                      (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) * 0.03109 *
                                      (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                        3.3662 * std::sqrt(C61)) +
                                       0.62517 * C66) *
                                      (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                       1.) -
                                  std::sqrt(C61) *
                                      std::pow(0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                               2) *
                                      0.03109 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                      std::pow(0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                               2)) *
                                 2. * C56) /
                                std::pow(2 * std::sqrt(C61) *
                                             std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                   6.1977 * C65) +
                                                                  3.3662 * std::sqrt(C61)) +
                                                                 0.62517 * C66),
                                                      2) *
                                             (1. / (0.03109 *
                                                    (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                              1.) *
                                             2. * C56,
                                         2)) *
                           C99) +
                      C46 *
                          ((std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66),
                                     2) *
                                (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66)) +
                                 1.) *
                                3.4198418683227306 * C56 * 0.033774 * (1. - C58) *
                                -0.001866317043825 * C75 -
                            0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) *
                                (0. *
                                     std::pow(0.033774 *
                                                  (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                              2) *
                                     (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                      1.) +
                                 (0.067548 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) * 0.033774 *
                                      (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                        0.88026 * std::sqrt(C61)) +
                                       0.49671 * C66) *
                                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                       1.) -
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) *
                                      0.033774 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                      std::pow(0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                               2)) *
                                     3.4198418683227306 * C56)) /
                               std::pow(
                                   std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) *
                                       3.4198418683227306 * C56,
                                   2) -
                           (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66),
                                     2) *
                                (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66)) +
                                 1.) *
                                2. * C56 * C58 * -0.0001241678412113 * C75 -
                            C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) *
                                (0.06218 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) * 0.03109 *
                                     (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                       3.3662 * std::sqrt(C61)) +
                                      0.62517 * C66) *
                                     (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66)) +
                                      1.) -
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) *
                                     0.03109 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                     std::pow(0.03109 *
                                                  (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66),
                                              2)) *
                                2. * C56) /
                               std::pow(
                                   std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.) *
                                       2. * C56,
                                   2)) *
                          C103) +
                     C46 *
                         (0.033774 *
                              (0.11125 * std::log(1. / (0.033774 *
                                                        (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                                  1.) -
                               (0.11125 * C65 + 1.) * 0.033774 *
                                   (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                   (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                             2) *
                                    (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66)) +
                                     1.))) *
                              (1. - C58) / (3.4198418683227306 * C56) -
                          C58 * 0.03109 *
                              (0.20548 * std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                     6.1977 * C65) +
                                                                    3.3662 * std::sqrt(C61)) +
                                                                   0.62517 * C66)) +
                                                  1.) -
                               (0.20548 * C65 + 1.) * 0.03109 *
                                   (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                   (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66),
                                             2) *
                                    (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                     1.))) /
                              (2. * C56)) *
                         C111) +
                    C46 *
                        (-0.033774 *
                             (0.11125 * std::log(1. / (0.033774 *
                                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                                 1.) -
                              (0.11125 * C65 + 1.) * 0.033774 *
                                  (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                  (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                   (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.))) *
                             C75 / (3.4198418683227306 * C56) -
                         0.03109 *
                             (0.20548 * std::log(1. / (0.03109 *
                                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                                 1.) -
                              (0.20548 * C65 + 1.) * 0.03109 *
                                  (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                  (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                   (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.))) *
                             C75 / (2. * C56)) *
                        C96) *
                       C134 +
                   ((((((0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                            C89 +
                        C46 *
                            (0.033774 *
                                 ((0.11125 * C65 + 1.) * 0.033774 *
                                      (10.357 / (2 * std::sqrt(C65)) + 3.6231) *
                                      (-0.02972990124 *
                                           std::pow(0.033774 *
                                                        (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                                    2) /
                                           (2 * std::sqrt(C61) *
                                            std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                   3.6231 * C65) +
                                                                  0.88026 * std::sqrt(C61)) +
                                                                 0.49671 * C66),
                                                     2)) +
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) *
                                           0.0020081953689595 *
                                           (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                             0.88026 * std::sqrt(C61)) +
                                            0.49671 * C66) /
                                           (2 * std::sqrt(C61))) /
                                      std::pow(std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                      3.6231 * C65) +
                                                                     0.88026 * std::sqrt(C61)) +
                                                                    0.49671 * C66),
                                                        2) *
                                                   (1. / (0.033774 * (((10.357 * std::sqrt(C65) +
                                                                        3.6231 * C65) +
                                                                       0.88026 * std::sqrt(C61)) +
                                                                      0.49671 * C66)) +
                                                    1.),
                                               2) -
                                  0.00330745151295 /
                                      (2 * std::sqrt(C61) *
                                       std::pow(0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                                2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.))) *
                                 C75 * (1. - C58) / (3.4198418683227306 * C56) -
                             C58 * 0.03109 *
                                 ((0.20548 * C65 + 1.) * 0.03109 *
                                      (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) *
                                      (-0.104655158 *
                                           std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                 6.1977 * C65) +
                                                                3.3662 * std::sqrt(C61)) +
                                                               0.62517 * C66),
                                                    2) /
                                           (2 * std::sqrt(C61) *
                                            std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                  6.1977 * C65) +
                                                                 3.3662 * std::sqrt(C61)) +
                                                                0.62517 * C66),
                                                     2)) +
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.) *
                                           0.00650745772444 *
                                           (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                             3.3662 * std::sqrt(C61)) +
                                            0.62517 * C66) /
                                           (2 * std::sqrt(C61))) /
                                      std::pow(std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                     6.1977 * C65) +
                                                                    3.3662 * std::sqrt(C61)) +
                                                                   0.62517 * C66),
                                                        2) *
                                                   (1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                       6.1977 * C65) +
                                                                      3.3662 * std::sqrt(C61)) +
                                                                     0.62517 * C66)) +
                                                    1.),
                                               2) -
                                  0.02150454186584 /
                                      (2 * std::sqrt(C61) *
                                       std::pow(0.03109 *
                                                    (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                                2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.))) *
                                 C75 / (2. * C56)) *
                            C102) +
                       C46 *
                           ((-0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) *
                             2 *
                             (0. * std::sqrt(C61) *
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) *
                                  (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                   1.) +
                              (-0.02972990124 * std::sqrt(C61) *
                                   std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) /
                                   (2 * std::sqrt(C61) *
                                    std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                             2)) +
                               (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66)) +
                                1.) *
                                   (std::sqrt(C61) * 0.0020081953689595 *
                                        (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                          0.88026 * std::sqrt(C61)) +
                                         0.49671 * C66) +
                                    std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                             2)) /
                                   (2 * std::sqrt(C61))) *
                                  3.4198418683227306 * C56)) /
                                std::pow(2 * std::sqrt(C61) *
                                             std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                    3.6231 * C65) +
                                                                   0.88026 * std::sqrt(C61)) +
                                                                  0.49671 * C66),
                                                      2) *
                                             (1. / (0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                              1.) *
                                             3.4198418683227306 * C56,
                                         2) -
                            (-C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) * 2 *
                             (-0.104655158 * std::sqrt(C61) *
                                  std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66),
                                           2) /
                                  (2 * std::sqrt(C61) *
                                   std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2)) +
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                                  (std::sqrt(C61) * 0.00650745772444 *
                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                         3.3662 * std::sqrt(C61)) +
                                        0.62517 * C66) +
                                   std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2)) /
                                  (2 * std::sqrt(C61))) *
                             2. * C56) /
                                std::pow(2 * std::sqrt(C61) *
                                             std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                   6.1977 * C65) +
                                                                  3.3662 * std::sqrt(C61)) +
                                                                 0.62517 * C66),
                                                      2) *
                                             (1. / (0.03109 *
                                                    (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                              1.) *
                                             2. * C56,
                                         2)) *
                           C99) +
                      C46 *
                          ((-0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) *
                            (0. *
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) *
                                 (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                  1.) +
                             (-0.02972990124 *
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) /
                                  (2 * std::sqrt(C61) *
                                   std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2)) +
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                                  0.0020081953689595 *
                                  (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                    0.88026 * std::sqrt(C61)) +
                                   0.49671 * C66) /
                                  (2 * std::sqrt(C61))) *
                                 3.4198418683227306 * C56)) /
                               std::pow(
                                   std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) *
                                       3.4198418683227306 * C56,
                                   2) -
                           (-C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) *
                            (-0.104655158 *
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) /
                                 (2 * std::sqrt(C61) *
                                  std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66),
                                           2)) +
                             (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                3.3662 * std::sqrt(C61)) +
                                               0.62517 * C66)) +
                              1.) *
                                 0.00650745772444 *
                                 (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                   3.3662 * std::sqrt(C61)) +
                                  0.62517 * C66) /
                                 (2 * std::sqrt(C61))) *
                            2. * C56) /
                               std::pow(
                                   std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.) *
                                       2. * C56,
                                   2)) *
                          C103) +
                     C46 *
                         (0.033774 * (1. - C58) * -0.02972990124 * (0.11125 * C65 + 1.) /
                              (2 * std::sqrt(C61) *
                               std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                        2) *
                               (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66)) +
                                1.) *
                               3.4198418683227306 * C56) -
                          C58 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                              (2 * std::sqrt(C61) *
                               std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66),
                                        2) *
                               (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66)) +
                                1.) *
                               2. * C56)) *
                         C111) +
                    C46 *
                        (-0.033774 * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                        C96) *
                       C131) +
                  ((((((0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                            (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66),
                                      2) *
                             (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                 0.88026 * std::sqrt(C61)) +
                                                0.49671 * C66)) +
                              1.) *
                             3.4198418683227306 * C56) -
                        C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                            (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66),
                                      2) *
                             (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                3.3662 * std::sqrt(C61)) +
                                               0.62517 * C66)) +
                              1.) *
                             2. * C56)) *
                           C89 +
                       C46 *
                           (0.033774 *
                                ((0.11125 * C65 + 1.) * 0.033774 *
                                     (10.357 / (2 * std::sqrt(C65)) + 3.6231) *
                                     (-0.01677588354 *
                                          std::pow(0.033774 *
                                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                                   2) /
                                          std::pow(0.033774 *
                                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                                   2) +
                                      0.0011331773813599 *
                                          (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                            0.88026 * std::sqrt(C61)) +
                                           0.49671 * C66) *
                                          (1. / (0.033774 *
                                                 (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66)) +
                                           1.)) /
                                     std::pow(std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                     3.6231 * C65) +
                                                                    0.88026 * std::sqrt(C61)) +
                                                                   0.49671 * C66),
                                                       2) *
                                                  (1. / (0.033774 * (((10.357 * std::sqrt(C65) +
                                                                       3.6231 * C65) +
                                                                      0.88026 * std::sqrt(C61)) +
                                                                     0.49671 * C66)) +
                                                   1.),
                                              2) -
                                 0.001866317043825 /
                                     (std::pow(0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                               2) *
                                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                       1.))) *
                                C75 * (1. - C58) / (3.4198418683227306 * C56) -
                            C58 * 0.03109 *
                                ((0.20548 * C65 + 1.) * 0.03109 *
                                     (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) *
                                     (-0.0194365353 *
                                          std::pow(0.03109 *
                                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66),
                                                   2) /
                                          std::pow(0.03109 *
                                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66),
                                                   2) +
                                      0.001208563764954 *
                                          (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                            3.3662 * std::sqrt(C61)) +
                                           0.62517 * C66) *
                                          (1. / (0.03109 *
                                                 (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66)) +
                                           1.)) /
                                     std::pow(std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                    6.1977 * C65) +
                                                                   3.3662 * std::sqrt(C61)) +
                                                                  0.62517 * C66),
                                                       2) *
                                                  (1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                      6.1977 * C65) +
                                                                     3.3662 * std::sqrt(C61)) +
                                                                    0.62517 * C66)) +
                                                   1.),
                                              2) -
                                 0.003993819273444 /
                                     (std::pow(0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                               2) *
                                      (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                       1.))) *
                                C75 / (2. * C56)) *
                           C102) +
                      C46 *
                          ((-0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) *
                            2 *
                            (0. * std::sqrt(C61) *
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) *
                                 (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                  1.) +
                             (-0.01677588354 * std::sqrt(C61) *
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) /
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) +
                              std::sqrt(C61) * 0.0011331773813599 *
                                  (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                    0.88026 * std::sqrt(C61)) +
                                   0.49671 * C66) *
                                  (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                   1.)) *
                                 3.4198418683227306 * C56)) /
                               std::pow(2 * std::sqrt(C61) *
                                            std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                   3.6231 * C65) +
                                                                  0.88026 * std::sqrt(C61)) +
                                                                 0.49671 * C66),
                                                     2) *
                                            (1. / (0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                             1.) *
                                            3.4198418683227306 * C56,
                                        2) -
                           (-C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) * 2 *
                            (-0.0194365353 * std::sqrt(C61) *
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) /
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) +
                             std::sqrt(C61) * 0.001208563764954 *
                                 (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                   3.3662 * std::sqrt(C61)) +
                                  0.62517 * C66) *
                                 (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66)) +
                                  1.)) *
                            2. * C56) /
                               std::pow(2 * std::sqrt(C61) *
                                            std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                  6.1977 * C65) +
                                                                 3.3662 * std::sqrt(C61)) +
                                                                0.62517 * C66),
                                                     2) *
                                            (1. / (0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66)) +
                                             1.) *
                                            2. * C56,
                                        2)) *
                          C99) +
                     C46 *
                         ((-0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) *
                           (0. *
                                std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                         2) *
                                (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66)) +
                                 1.) +
                            (-0.01677588354 *
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) /
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) +
                             0.0011331773813599 *
                                 (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                   0.88026 * std::sqrt(C61)) +
                                  0.49671 * C66) *
                                 (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                  1.)) *
                                3.4198418683227306 * C56)) /
                              std::pow(
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) *
                                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                       1.) *
                                      3.4198418683227306 * C56,
                                  2) -
                          (-C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) *
                           (-0.0194365353 *
                                std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                         2) /
                                std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                         2) +
                            0.001208563764954 *
                                (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                  3.3662 * std::sqrt(C61)) +
                                 0.62517 * C66) *
                                (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66)) +
                                 1.)) *
                           2. * C56) /
                              std::pow(
                                  std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66),
                                           2) *
                                      (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                       1.) *
                                      2. * C56,
                                  2)) *
                         C103) +
                    C46 *
                        (0.033774 * (1. - C58) * -0.01677588354 * (0.11125 * C65 + 1.) /
                             (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C58 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                             (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                        C111) +
                   C46 *
                       (-0.033774 * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                            (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66),
                                      2) *
                             (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                 0.88026 * std::sqrt(C61)) +
                                                0.49671 * C66)) +
                              1.) *
                             3.4198418683227306 * C56) -
                        C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                            (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66),
                                      2) *
                             (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                3.3662 * std::sqrt(C61)) +
                                               0.62517 * C66)) +
                              1.) *
                             2. * C56)) *
                       C96) *
                      C135) +
                 ((((((0.033774 * (0.11125 * C65 + 1.) *
                           std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.) *
                           (1. - C58) / (3.4198418683227306 * C56) +
                       C58 *
                           (C84 - 0.03109 * (0.20548 * C65 + 1.) *
                                      std::log(1. / (0.03109 *
                                                     (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                               1.)) /
                           (2. * C56)) *
                          C89 +
                      C46 *
                          (0.033774 *
                               (0.11125 * std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) +
                                                                       3.6231 * C65) +
                                                                      0.88026 * std::sqrt(C61)) +
                                                                     0.49671 * C66)) +
                                                   1.) -
                                (0.11125 * C65 + 1.) * 0.033774 *
                                    (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                    (std::pow(0.033774 *
                                                  (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                              2) *
                                     (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                      1.))) *
                               (1. - C58) / (3.4198418683227306 * C56) -
                           C58 * 0.03109 *
                               (0.20548 * std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                      6.1977 * C65) +
                                                                     3.3662 * std::sqrt(C61)) +
                                                                    0.62517 * C66)) +
                                                   1.) -
                                (0.20548 * C65 + 1.) * 0.03109 *
                                    (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                    (std::pow(0.03109 *
                                                  (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66),
                                              2) *
                                     (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66)) +
                                      1.))) /
                               (2. * C56)) *
                          C102) +
                     C46 *
                         (0.033774 * (1. - C58) * -0.02972990124 * (0.11125 * C65 + 1.) /
                              (2 * std::sqrt(C61) *
                               std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                        2) *
                               (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66)) +
                                1.) *
                               3.4198418683227306 * C56) -
                          C58 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                              (2 * std::sqrt(C61) *
                               std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66),
                                        2) *
                               (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66)) +
                                1.) *
                               2. * C56)) *
                         C99) +
                    C46 *
                        (0.033774 * (1. - C58) * -0.01677588354 * (0.11125 * C65 + 1.) /
                             (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C58 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                             (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                        C103) +
                   C46 *
                       (-0.033774 * (0.11125 * C65 + 1.) *
                            std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66)) +
                                     1.) /
                            (3.4198418683227306 * C56) +
                        (C84 -
                         0.03109 * (0.20548 * C65 + 1.) *
                             std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66)) +
                                      1.)) /
                            (2. * C56)) *
                       C96) +
                  C120 * C46 * C58 / (2. * C56)) *
                     C143) +
                ((((((-0.033774 * (0.11125 * C65 + 1.) *
                          std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                   1.) *
                          C75 / (3.4198418683227306 * C56) +
                      (C84 -
                       0.03109 * (0.20548 * C65 + 1.) *
                           std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.)) *
                          C75 / (2. * C56)) *
                         C89 +
                     C46 *
                         (-0.033774 *
                              (0.11125 * std::log(1. / (0.033774 *
                                                        (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                                  1.) -
                               (0.11125 * C65 + 1.) * 0.033774 *
                                   (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                   (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                             2) *
                                    (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66)) +
                                     1.))) *
                              C75 / (3.4198418683227306 * C56) -
                          0.03109 *
                              (0.20548 * std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                     6.1977 * C65) +
                                                                    3.3662 * std::sqrt(C61)) +
                                                                   0.62517 * C66)) +
                                                  1.) -
                               (0.20548 * C65 + 1.) * 0.03109 *
                                   (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                   (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66),
                                             2) *
                                    (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                     1.))) *
                              C75 / (2. * C56)) *
                         C102) +
                    C46 *
                        (-0.033774 * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                        C99) +
                   C46 *
                       (-0.033774 * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                            (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66),
                                      2) *
                             (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                 0.88026 * std::sqrt(C61)) +
                                                0.49671 * C66)) +
                              1.) *
                             3.4198418683227306 * C56) -
                        C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                            (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66),
                                      2) *
                             (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                3.3662 * std::sqrt(C61)) +
                                               0.62517 * C66)) +
                              1.) *
                             2. * C56)) *
                       C103) +
                  C46 *
                      (-0.033774 * (0.11125 * C65 + 1.) *
                           std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.) /
                           (3.4198418683227306 * C56) +
                       (C84 -
                        0.03109 * (0.20548 * C65 + 1.) *
                            std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                     1.)) /
                           (2. * C56)) *
                      C111) +
                 C120 * C46 * C75 / (2. * C56)) *
                    C128) +
               (((C58 * C75 / (2. * C56) - 1) * C89 + C111 * C46 * C58 / (2. * C56)) +
                C96 * C46 * C75 / (2. * C56)) *
                   C152) +
              ((((((0.033774 *
                        (0.11125 *
                             std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                      1.) -
                         (0.11125 * C65 + 1.) * 0.033774 *
                             (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                             (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.))) *
                        C75 * (1. - C58) / (3.4198418683227306 * C56) -
                    C58 * 0.03109 *
                        (0.20548 *
                             std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66)) +
                                      1.) -
                         (0.20548 * C65 + 1.) * 0.03109 *
                             (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                             (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.))) *
                        C75 / (2. * C56)) *
                       C102 +
                   (0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                        (2 * std::sqrt(C61) *
                         std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                               0.88026 * std::sqrt(C61)) +
                                              0.49671 * C66),
                                  2) *
                         (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                             0.88026 * std::sqrt(C61)) +
                                            0.49671 * C66)) +
                          1.) *
                         3.4198418683227306 * C56) -
                    C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                        (2 * std::sqrt(C61) *
                         std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                              3.3662 * std::sqrt(C61)) +
                                             0.62517 * C66),
                                  2) *
                         (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                            3.3662 * std::sqrt(C61)) +
                                           0.62517 * C66)) +
                          1.) *
                         2. * C56)) *
                       C99) +
                  (0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                       (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                              0.88026 * std::sqrt(C61)) +
                                             0.49671 * C66),
                                 2) *
                        (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                            0.88026 * std::sqrt(C61)) +
                                           0.49671 * C66)) +
                         1.) *
                        3.4198418683227306 * C56) -
                   C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                       (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                             3.3662 * std::sqrt(C61)) +
                                            0.62517 * C66),
                                 2) *
                        (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                           3.3662 * std::sqrt(C61)) +
                                          0.62517 * C66)) +
                         1.) *
                        2. * C56)) *
                      C103) +
                 (0.033774 * (0.11125 * C65 + 1.) *
                      std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                      (1. - C58) / (3.4198418683227306 * C56) +
                  C58 *
                      (C84 -
                       0.03109 * (0.20548 * C65 + 1.) *
                           std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.)) /
                      (2. * C56)) *
                     C111) +
                (-0.033774 * (0.11125 * C65 + 1.) *
                     std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                 0.88026 * std::sqrt(C61)) +
                                                0.49671 * C66)) +
                              1.) *
                     C75 / (3.4198418683227306 * C56) +
                 (C84 - 0.03109 * (0.20548 * C65 + 1.) *
                            std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                     1.)) *
                     C75 / (2. * C56)) *
                    C96) +
               (C58 * C75 / (2. * C56) - 1) * C120) *
                  C121) +
             C46 *
                 (0.033774 *
                      (0.11125 *
                           std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.) -
                       (0.11125 * C65 + 1.) * 0.033774 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                           (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66),
                                     2) *
                            (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                0.88026 * std::sqrt(C61)) +
                                               0.49671 * C66)) +
                             1.))) *
                      C75 * (1. - C58) / (3.4198418683227306 * C56) -
                  C58 * 0.03109 *
                      (0.20548 *
                           std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.) -
                       (0.20548 * C65 + 1.) * 0.03109 *
                           (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                           (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66),
                                     2) *
                            (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                               3.3662 * std::sqrt(C61)) +
                                              0.62517 * C66)) +
                             1.))) *
                      C75 / (2. * C56)) *
                 C190) +
            C46 *
                (0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                     (2 * std::sqrt(C61) *
                      std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                            0.88026 * std::sqrt(C61)) +
                                           0.49671 * C66),
                               2) *
                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                          0.88026 * std::sqrt(C61)) +
                                         0.49671 * C66)) +
                       1.) *
                      3.4198418683227306 * C56) -
                 C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                     (2 * std::sqrt(C61) *
                      std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                           3.3662 * std::sqrt(C61)) +
                                          0.62517 * C66),
                               2) *
                      (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                         3.3662 * std::sqrt(C61)) +
                                        0.62517 * C66)) +
                       1.) *
                      2. * C56)) *
                C187) +
           C46 *
               (0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                    (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                           0.88026 * std::sqrt(C61)) +
                                          0.49671 * C66),
                              2) *
                     (1. / (0.033774 *
                            (((10.357 * std::sqrt(C65) + 3.6231 * C65) + 0.88026 * std::sqrt(C61)) +
                             0.49671 * C66)) +
                      1.) *
                     3.4198418683227306 * C56) -
                C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                    (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                          3.3662 * std::sqrt(C61)) +
                                         0.62517 * C66),
                              2) *
                     (1. / (0.03109 *
                            (((14.1189 * std::sqrt(C65) + 6.1977 * C65) + 3.3662 * std::sqrt(C61)) +
                             0.62517 * C66)) +
                      1.) *
                     2. * C56)) *
               C191) +
          C46 *
              (0.033774 * (0.11125 * C65 + 1.) *
                   std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                               0.88026 * std::sqrt(C61)) +
                                              0.49671 * C66)) +
                            1.) *
                   (1. - C58) / (3.4198418683227306 * C56) +
               C58 *
                   (C84 - 0.03109 * (0.20548 * C65 + 1.) *
                              std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                       1.)) /
                   (2. * C56)) *
              C199) +
         C46 *
             (-0.033774 * (0.11125 * C65 + 1.) *
                  std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                              0.88026 * std::sqrt(C61)) +
                                             0.49671 * C66)) +
                           1.) *
                  C75 / (3.4198418683227306 * C56) +
              (C84 - 0.03109 * (0.20548 * C65 + 1.) *
                         std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66)) +
                                  1.)) *
                  C75 / (2. * C56)) *
             C184) +
        C46 * (C58 * C75 / (2. * C56) - 1) * C208;

    const double C209 = 0.;
    const double C210 =
        ((-C122) / std::pow(C46, 2) - (-C121 * C47 * 2 * C46) / std::pow(C46, 4)) * C121 -
        C122 * C121 / std::pow(C46, 2);
    const double C211 =
        (C209 / C46 +
         ((-C123) / std::pow(C46, 2) - (-C121 * C51 * 2 * C46) / std::pow(C46, 4)) * C121) -
        C123 * C121 / std::pow(C46, 2);
    const double C212 = 12 * std::pow(C53, 2) * std::pow(C125, 2) + 4 * std::pow(C53, 3) * C210;
    const double C213 = C211;
    const double C214 = -C211;
    const double C215 = (-C126 * -3. * C126 * 2 * C55) / std::pow(C55, 4);
    const double C216 = C50 * (C50 - 1) * std::pow(C59, C50 - 2) * std::pow(C129, 2) +
                        C50 * std::pow(C59, C50 - 1) * C213;
    const double C217 = C50 * (C50 - 1) * std::pow(C60, C50 - 2) * std::pow(C130, 2) +
                        C50 * std::pow(C60, C50 - 1) * C214;
    const double C218 = C48 * (C48 - 1) * std::pow(C61, C48 - 2) * std::pow(C131, 2) +
                        C48 * std::pow(C61, C48 - 1) * C215;
    const double C219 = C49 * (C49 - 1) * std::pow(C61, C49 - 2) * std::pow(C131, 2) +
                        C49 * std::pow(C61, C49 - 1) * C215;
    const double C220 = 0.2137 * C218;
    const double C221 = 0.49294 * C219;
    const double C222 =
        1.6382 * C215 / (2 * std::sqrt(C61)) -
        1.6382 * std::pow(C131, 2) / (std::sqrt(C61) * std::pow(2 * std::sqrt(C61), 2));
    const double C223 = 3.5876 * C218;
    const double C224 = C216 + C217;
    const double C225 =
        7.5957 * C218 / (2 * std::sqrt(C65)) -
        7.5957 * std::pow(C134, 2) / (std::sqrt(C65) * std::pow(2 * std::sqrt(C65), 2));
    const double C226 = C220;
    const double C227 = C224;
    const double C228 = C225 + C223;
    const double C229 = C228 + C222;
    const double C230 = C229 + C221;
    const double C231 = 0.062182 * C230;
    const double C232 = 2 * C79 * std::pow(C147, 2) / std::pow(C79, 4) - C231 / std::pow(C79, 2);
    const double C233 = C232;
    const double C234 = C233 / C81 - std::pow(C149, 2) / std::pow(C81, 2);
    const double C235 = ((C142 * C150 + C82 * C226) + C150 * C142) + C74 * C234;
    const double C236 = 0.062182 * C235;
    matrix.upper[2] =
        (((((((((((((((((0.033774 *
                             (0.11125 * std::log(1. / (0.033774 *
                                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                                 1.) -
                              (0.11125 * C65 + 1.) * 0.033774 *
                                  (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                  (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                   (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.))) *
                             C75 * (1. - C58) / (3.4198418683227306 * C56) -
                         C58 * 0.03109 *
                             (0.20548 * std::log(1. / (0.03109 *
                                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                                 1.) -
                              (0.20548 * C65 + 1.) * 0.03109 *
                                  (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                  (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                   (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.))) *
                             C75 / (2. * C56)) *
                            C121 +
                        C46 *
                            (0.033774 *
                                 (-0.0037573575 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                      (std::pow(0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                                2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.)) -
                                  (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) *
                                       (-0.349797318 * (0.11125 * C65 + 1.) /
                                            (std::sqrt(C65) * std::pow(2 * std::sqrt(C65), 2)) +
                                        0.0037573575 * (10.357 / (2 * std::sqrt(C65)) + 3.6231)) -
                                   (0.11125 * C65 + 1.) * 0.033774 *
                                       (10.357 / (2 * std::sqrt(C65)) + 3.6231) *
                                       (0.067548 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) *
                                            0.033774 *
                                            (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                              0.88026 * std::sqrt(C61)) +
                                             0.49671 * C66) *
                                            (1. / (0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                             1.) -
                                        std::pow(0.033774 *
                                                     (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                                 2) *
                                            0.033774 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                            std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                   3.6231 * C65) +
                                                                  0.88026 * std::sqrt(C61)) +
                                                                 0.49671 * C66),
                                                     2))) /
                                      std::pow(std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                      3.6231 * C65) +
                                                                     0.88026 * std::sqrt(C61)) +
                                                                    0.49671 * C66),
                                                        2) *
                                                   (1. / (0.033774 * (((10.357 * std::sqrt(C65) +
                                                                        3.6231 * C65) +
                                                                       0.88026 * std::sqrt(C61)) +
                                                                      0.49671 * C66)) +
                                                    1.),
                                               2)) *
                                 C75 * (1. - C58) / (3.4198418683227306 * C56) -
                             C58 * 0.03109 *
                                 (-0.0063883732 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                      (std::pow(0.03109 *
                                                    (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                                2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.)) -
                                  (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.) *
                                       (-0.0438956601e1 * (0.20548 * C65 + 1.) /
                                            (std::sqrt(C65) * std::pow(2 * std::sqrt(C65), 2)) +
                                        0.0063883732 *
                                            (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977)) -
                                   (0.20548 * C65 + 1.) * 0.03109 *
                                       (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) *
                                       (0.06218 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) *
                                            0.03109 *
                                            (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                              3.3662 * std::sqrt(C61)) +
                                             0.62517 * C66) *
                                            (1. / (0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66)) +
                                             1.) -
                                        std::pow(0.03109 *
                                                     (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66),
                                                 2) *
                                            0.03109 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                            std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                  6.1977 * C65) +
                                                                 3.3662 * std::sqrt(C61)) +
                                                                0.62517 * C66),
                                                     2))) /
                                      std::pow(std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                     6.1977 * C65) +
                                                                    3.3662 * std::sqrt(C61)) +
                                                                   0.62517 * C66),
                                                        2) *
                                                   (1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                       6.1977 * C65) +
                                                                      3.3662 * std::sqrt(C61)) +
                                                                     0.62517 * C66)) +
                                                    1.),
                                               2)) *
                                 C75 / (2. * C56)) *
                            C134) +
                       C46 *
                           ((2 * std::sqrt(C61) *
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) *
                                 (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                  1.) *
                                 3.4198418683227306 * C56 * 0.033774 * (1. - C58) *
                                 -0.00330745151295 * C75 -
                             0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) *
                                 2 *
                                 (0. * std::sqrt(C61) *
                                      std::pow(0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                               2) *
                                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                       1.) +
                                  (std::sqrt(C61) * 0.067548 *
                                       (10.357 / (2 * std::sqrt(C65)) + 3.6231) * 0.033774 *
                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                         0.88026 * std::sqrt(C61)) +
                                        0.49671 * C66) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) -
                                   std::sqrt(C61) *
                                       std::pow(0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                                2) *
                                       0.033774 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                       std::pow(0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                                2)) *
                                      3.4198418683227306 * C56)) /
                                std::pow(2 * std::sqrt(C61) *
                                             std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                    3.6231 * C65) +
                                                                   0.88026 * std::sqrt(C61)) +
                                                                  0.49671 * C66),
                                                      2) *
                                             (1. / (0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                              1.) *
                                             3.4198418683227306 * C56,
                                         2) -
                            (2 * std::sqrt(C61) *
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) *
                                 (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66)) +
                                  1.) *
                                 2. * C56 * C58 * -0.0006685762066089 * C75 -
                             C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) * 2 *
                                 (std::sqrt(C61) * 0.06218 *
                                      (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) * 0.03109 *
                                      (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                        3.3662 * std::sqrt(C61)) +
                                       0.62517 * C66) *
                                      (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                       1.) -
                                  std::sqrt(C61) *
                                      std::pow(0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                               2) *
                                      0.03109 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                      std::pow(0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                               2)) *
                                 2. * C56) /
                                std::pow(2 * std::sqrt(C61) *
                                             std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                   6.1977 * C65) +
                                                                  3.3662 * std::sqrt(C61)) +
                                                                 0.62517 * C66),
                                                      2) *
                                             (1. / (0.03109 *
                                                    (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                              1.) *
                                             2. * C56,
                                         2)) *
                           C131) +
                      C46 *
                          ((std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66),
                                     2) *
                                (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66)) +
                                 1.) *
                                3.4198418683227306 * C56 * 0.033774 * (1. - C58) *
                                -0.001866317043825 * C75 -
                            0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) *
                                (0. *
                                     std::pow(0.033774 *
                                                  (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                              2) *
                                     (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                      1.) +
                                 (0.067548 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) * 0.033774 *
                                      (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                        0.88026 * std::sqrt(C61)) +
                                       0.49671 * C66) *
                                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                       1.) -
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) *
                                      0.033774 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                      std::pow(0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                               2)) *
                                     3.4198418683227306 * C56)) /
                               std::pow(
                                   std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) *
                                       3.4198418683227306 * C56,
                                   2) -
                           (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66),
                                     2) *
                                (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66)) +
                                 1.) *
                                2. * C56 * C58 * -0.0001241678412113 * C75 -
                            C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) *
                                (0.06218 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) * 0.03109 *
                                     (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                       3.3662 * std::sqrt(C61)) +
                                      0.62517 * C66) *
                                     (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66)) +
                                      1.) -
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) *
                                     0.03109 * (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                     std::pow(0.03109 *
                                                  (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66),
                                              2)) *
                                2. * C56) /
                               std::pow(
                                   std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.) *
                                       2. * C56,
                                   2)) *
                          C135) +
                     C46 *
                         (0.033774 *
                              (0.11125 * std::log(1. / (0.033774 *
                                                        (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                                  1.) -
                               (0.11125 * C65 + 1.) * 0.033774 *
                                   (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                   (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                             2) *
                                    (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66)) +
                                     1.))) *
                              (1. - C58) / (3.4198418683227306 * C56) -
                          C58 * 0.03109 *
                              (0.20548 * std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                     6.1977 * C65) +
                                                                    3.3662 * std::sqrt(C61)) +
                                                                   0.62517 * C66)) +
                                                  1.) -
                               (0.20548 * C65 + 1.) * 0.03109 *
                                   (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                   (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66),
                                             2) *
                                    (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                     1.))) /
                              (2. * C56)) *
                         C143) +
                    C46 *
                        (-0.033774 *
                             (0.11125 * std::log(1. / (0.033774 *
                                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                                 1.) -
                              (0.11125 * C65 + 1.) * 0.033774 *
                                  (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                  (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                   (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.))) *
                             C75 / (3.4198418683227306 * C56) -
                         0.03109 *
                             (0.20548 * std::log(1. / (0.03109 *
                                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                                 1.) -
                              (0.20548 * C65 + 1.) * 0.03109 *
                                  (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                  (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                   (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.))) *
                             C75 / (2. * C56)) *
                        C128) *
                       C134 +
                   ((((((0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                            C121 +
                        C46 *
                            (0.033774 *
                                 ((0.11125 * C65 + 1.) * 0.033774 *
                                      (10.357 / (2 * std::sqrt(C65)) + 3.6231) *
                                      (-0.02972990124 *
                                           std::pow(0.033774 *
                                                        (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                                    2) /
                                           (2 * std::sqrt(C61) *
                                            std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                   3.6231 * C65) +
                                                                  0.88026 * std::sqrt(C61)) +
                                                                 0.49671 * C66),
                                                     2)) +
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) *
                                           0.0020081953689595 *
                                           (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                             0.88026 * std::sqrt(C61)) +
                                            0.49671 * C66) /
                                           (2 * std::sqrt(C61))) /
                                      std::pow(std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                      3.6231 * C65) +
                                                                     0.88026 * std::sqrt(C61)) +
                                                                    0.49671 * C66),
                                                        2) *
                                                   (1. / (0.033774 * (((10.357 * std::sqrt(C65) +
                                                                        3.6231 * C65) +
                                                                       0.88026 * std::sqrt(C61)) +
                                                                      0.49671 * C66)) +
                                                    1.),
                                               2) -
                                  0.00330745151295 /
                                      (2 * std::sqrt(C61) *
                                       std::pow(0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                                2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.))) *
                                 C75 * (1. - C58) / (3.4198418683227306 * C56) -
                             C58 * 0.03109 *
                                 ((0.20548 * C65 + 1.) * 0.03109 *
                                      (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) *
                                      (-0.104655158 *
                                           std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                 6.1977 * C65) +
                                                                3.3662 * std::sqrt(C61)) +
                                                               0.62517 * C66),
                                                    2) /
                                           (2 * std::sqrt(C61) *
                                            std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                  6.1977 * C65) +
                                                                 3.3662 * std::sqrt(C61)) +
                                                                0.62517 * C66),
                                                     2)) +
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.) *
                                           0.00650745772444 *
                                           (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                             3.3662 * std::sqrt(C61)) +
                                            0.62517 * C66) /
                                           (2 * std::sqrt(C61))) /
                                      std::pow(std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                     6.1977 * C65) +
                                                                    3.3662 * std::sqrt(C61)) +
                                                                   0.62517 * C66),
                                                        2) *
                                                   (1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                       6.1977 * C65) +
                                                                      3.3662 * std::sqrt(C61)) +
                                                                     0.62517 * C66)) +
                                                    1.),
                                               2) -
                                  0.02150454186584 /
                                      (2 * std::sqrt(C61) *
                                       std::pow(0.03109 *
                                                    (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                                2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.))) *
                                 C75 / (2. * C56)) *
                            C134) +
                       C46 *
                           ((-0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) *
                             2 *
                             (0. * std::sqrt(C61) *
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) *
                                  (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                   1.) +
                              (-0.02972990124 * std::sqrt(C61) *
                                   std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) /
                                   (2 * std::sqrt(C61) *
                                    std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                             2)) +
                               (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66)) +
                                1.) *
                                   (std::sqrt(C61) * 0.0020081953689595 *
                                        (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                          0.88026 * std::sqrt(C61)) +
                                         0.49671 * C66) +
                                    std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                             2)) /
                                   (2 * std::sqrt(C61))) *
                                  3.4198418683227306 * C56)) /
                                std::pow(2 * std::sqrt(C61) *
                                             std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                    3.6231 * C65) +
                                                                   0.88026 * std::sqrt(C61)) +
                                                                  0.49671 * C66),
                                                      2) *
                                             (1. / (0.033774 *
                                                    (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                              1.) *
                                             3.4198418683227306 * C56,
                                         2) -
                            (-C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) * 2 *
                             (-0.104655158 * std::sqrt(C61) *
                                  std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66),
                                           2) /
                                  (2 * std::sqrt(C61) *
                                   std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2)) +
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                                  (std::sqrt(C61) * 0.00650745772444 *
                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                         3.3662 * std::sqrt(C61)) +
                                        0.62517 * C66) +
                                   std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2)) /
                                  (2 * std::sqrt(C61))) *
                             2. * C56) /
                                std::pow(2 * std::sqrt(C61) *
                                             std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                   6.1977 * C65) +
                                                                  3.3662 * std::sqrt(C61)) +
                                                                 0.62517 * C66),
                                                      2) *
                                             (1. / (0.03109 *
                                                    (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                              1.) *
                                             2. * C56,
                                         2)) *
                           C131) +
                      C46 *
                          ((-0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) *
                            (0. *
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) *
                                 (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                  1.) +
                             (-0.02972990124 *
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) /
                                  (2 * std::sqrt(C61) *
                                   std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2)) +
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                                  0.0020081953689595 *
                                  (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                    0.88026 * std::sqrt(C61)) +
                                   0.49671 * C66) /
                                  (2 * std::sqrt(C61))) *
                                 3.4198418683227306 * C56)) /
                               std::pow(
                                   std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                            2) *
                                       (1. /
                                            (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                        1.) *
                                       3.4198418683227306 * C56,
                                   2) -
                           (-C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) *
                            (-0.104655158 *
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) /
                                 (2 * std::sqrt(C61) *
                                  std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66),
                                           2)) +
                             (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                3.3662 * std::sqrt(C61)) +
                                               0.62517 * C66)) +
                              1.) *
                                 0.00650745772444 *
                                 (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                   3.3662 * std::sqrt(C61)) +
                                  0.62517 * C66) /
                                 (2 * std::sqrt(C61))) *
                            2. * C56) /
                               std::pow(
                                   std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66),
                                            2) *
                                       (1. /
                                            (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                        1.) *
                                       2. * C56,
                                   2)) *
                          C135) +
                     C46 *
                         (0.033774 * (1. - C58) * -0.02972990124 * (0.11125 * C65 + 1.) /
                              (2 * std::sqrt(C61) *
                               std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                        2) *
                               (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66)) +
                                1.) *
                               3.4198418683227306 * C56) -
                          C58 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                              (2 * std::sqrt(C61) *
                               std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66),
                                        2) *
                               (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66)) +
                                1.) *
                               2. * C56)) *
                         C143) +
                    C46 *
                        (-0.033774 * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                        C128) *
                       C131) +
                  ((((((0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                            (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66),
                                      2) *
                             (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                 0.88026 * std::sqrt(C61)) +
                                                0.49671 * C66)) +
                              1.) *
                             3.4198418683227306 * C56) -
                        C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                            (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66),
                                      2) *
                             (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                3.3662 * std::sqrt(C61)) +
                                               0.62517 * C66)) +
                              1.) *
                             2. * C56)) *
                           C121 +
                       C46 *
                           (0.033774 *
                                ((0.11125 * C65 + 1.) * 0.033774 *
                                     (10.357 / (2 * std::sqrt(C65)) + 3.6231) *
                                     (-0.01677588354 *
                                          std::pow(0.033774 *
                                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                                   2) /
                                          std::pow(0.033774 *
                                                       (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66),
                                                   2) +
                                      0.0011331773813599 *
                                          (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                            0.88026 * std::sqrt(C61)) +
                                           0.49671 * C66) *
                                          (1. / (0.033774 *
                                                 (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66)) +
                                           1.)) /
                                     std::pow(std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                     3.6231 * C65) +
                                                                    0.88026 * std::sqrt(C61)) +
                                                                   0.49671 * C66),
                                                       2) *
                                                  (1. / (0.033774 * (((10.357 * std::sqrt(C65) +
                                                                       3.6231 * C65) +
                                                                      0.88026 * std::sqrt(C61)) +
                                                                     0.49671 * C66)) +
                                                   1.),
                                              2) -
                                 0.001866317043825 /
                                     (std::pow(0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                               2) *
                                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                       1.))) *
                                C75 * (1. - C58) / (3.4198418683227306 * C56) -
                            C58 * 0.03109 *
                                ((0.20548 * C65 + 1.) * 0.03109 *
                                     (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) *
                                     (-0.0194365353 *
                                          std::pow(0.03109 *
                                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66),
                                                   2) /
                                          std::pow(0.03109 *
                                                       (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66),
                                                   2) +
                                      0.001208563764954 *
                                          (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                            3.3662 * std::sqrt(C61)) +
                                           0.62517 * C66) *
                                          (1. / (0.03109 *
                                                 (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66)) +
                                           1.)) /
                                     std::pow(std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                    6.1977 * C65) +
                                                                   3.3662 * std::sqrt(C61)) +
                                                                  0.62517 * C66),
                                                       2) *
                                                  (1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                      6.1977 * C65) +
                                                                     3.3662 * std::sqrt(C61)) +
                                                                    0.62517 * C66)) +
                                                   1.),
                                              2) -
                                 0.003993819273444 /
                                     (std::pow(0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                               2) *
                                      (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                       1.))) *
                                C75 / (2. * C56)) *
                           C134) +
                      C46 *
                          ((-0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) *
                            2 *
                            (0. * std::sqrt(C61) *
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) *
                                 (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                  1.) +
                             (-0.01677588354 * std::sqrt(C61) *
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) /
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) +
                              std::sqrt(C61) * 0.0011331773813599 *
                                  (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                    0.88026 * std::sqrt(C61)) +
                                   0.49671 * C66) *
                                  (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                   1.)) *
                                 3.4198418683227306 * C56)) /
                               std::pow(2 * std::sqrt(C61) *
                                            std::pow(0.033774 * (((10.357 * std::sqrt(C65) +
                                                                   3.6231 * C65) +
                                                                  0.88026 * std::sqrt(C61)) +
                                                                 0.49671 * C66),
                                                     2) *
                                            (1. / (0.033774 *
                                                   (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                             1.) *
                                            3.4198418683227306 * C56,
                                        2) -
                           (-C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) * 2 *
                            (-0.0194365353 * std::sqrt(C61) *
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) /
                                 std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66),
                                          2) +
                             std::sqrt(C61) * 0.001208563764954 *
                                 (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                   3.3662 * std::sqrt(C61)) +
                                  0.62517 * C66) *
                                 (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66)) +
                                  1.)) *
                            2. * C56) /
                               std::pow(2 * std::sqrt(C61) *
                                            std::pow(0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                  6.1977 * C65) +
                                                                 3.3662 * std::sqrt(C61)) +
                                                                0.62517 * C66),
                                                     2) *
                                            (1. / (0.03109 *
                                                   (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66)) +
                                             1.) *
                                            2. * C56,
                                        2)) *
                          C131) +
                     C46 *
                         ((-0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) *
                           (0. *
                                std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66),
                                         2) *
                                (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66)) +
                                 1.) +
                            (-0.01677588354 *
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) /
                                 std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66),
                                          2) +
                             0.0011331773813599 *
                                 (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                   0.88026 * std::sqrt(C61)) +
                                  0.49671 * C66) *
                                 (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66)) +
                                  1.)) *
                                3.4198418683227306 * C56)) /
                              std::pow(
                                  std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66),
                                           2) *
                                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                       1.) *
                                      3.4198418683227306 * C56,
                                  2) -
                          (-C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) *
                           (-0.0194365353 *
                                std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                         2) /
                                std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                     3.3662 * std::sqrt(C61)) +
                                                    0.62517 * C66),
                                         2) +
                            0.001208563764954 *
                                (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                  3.3662 * std::sqrt(C61)) +
                                 0.62517 * C66) *
                                (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66)) +
                                 1.)) *
                           2. * C56) /
                              std::pow(
                                  std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66),
                                           2) *
                                      (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                       1.) *
                                      2. * C56,
                                  2)) *
                         C135) +
                    C46 *
                        (0.033774 * (1. - C58) * -0.01677588354 * (0.11125 * C65 + 1.) /
                             (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C58 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                             (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                        C143) +
                   C46 *
                       (-0.033774 * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                            (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66),
                                      2) *
                             (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                 0.88026 * std::sqrt(C61)) +
                                                0.49671 * C66)) +
                              1.) *
                             3.4198418683227306 * C56) -
                        C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                            (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66),
                                      2) *
                             (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                3.3662 * std::sqrt(C61)) +
                                               0.62517 * C66)) +
                              1.) *
                             2. * C56)) *
                       C128) *
                      C135) +
                 ((((((0.033774 * (0.11125 * C65 + 1.) *
                           std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.) *
                           (1. - C58) / (3.4198418683227306 * C56) +
                       C58 *
                           (C84 - 0.03109 * (0.20548 * C65 + 1.) *
                                      std::log(1. / (0.03109 *
                                                     (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                               1.)) /
                           (2. * C56)) *
                          C121 +
                      C46 *
                          (0.033774 *
                               (0.11125 * std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) +
                                                                       3.6231 * C65) +
                                                                      0.88026 * std::sqrt(C61)) +
                                                                     0.49671 * C66)) +
                                                   1.) -
                                (0.11125 * C65 + 1.) * 0.033774 *
                                    (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                    (std::pow(0.033774 *
                                                  (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                              2) *
                                     (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                      1.))) *
                               (1. - C58) / (3.4198418683227306 * C56) -
                           C58 * 0.03109 *
                               (0.20548 * std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                      6.1977 * C65) +
                                                                     3.3662 * std::sqrt(C61)) +
                                                                    0.62517 * C66)) +
                                                   1.) -
                                (0.20548 * C65 + 1.) * 0.03109 *
                                    (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                    (std::pow(0.03109 *
                                                  (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66),
                                              2) *
                                     (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66)) +
                                      1.))) /
                               (2. * C56)) *
                          C134) +
                     C46 *
                         (0.033774 * (1. - C58) * -0.02972990124 * (0.11125 * C65 + 1.) /
                              (2 * std::sqrt(C61) *
                               std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                     0.88026 * std::sqrt(C61)) +
                                                    0.49671 * C66),
                                        2) *
                               (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66)) +
                                1.) *
                               3.4198418683227306 * C56) -
                          C58 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                              (2 * std::sqrt(C61) *
                               std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66),
                                        2) *
                               (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66)) +
                                1.) *
                               2. * C56)) *
                         C131) +
                    C46 *
                        (0.033774 * (1. - C58) * -0.01677588354 * (0.11125 * C65 + 1.) /
                             (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C58 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                             (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                        C135) +
                   C46 *
                       (-0.033774 * (0.11125 * C65 + 1.) *
                            std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66)) +
                                     1.) /
                            (3.4198418683227306 * C56) +
                        (C84 -
                         0.03109 * (0.20548 * C65 + 1.) *
                             std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66)) +
                                      1.)) /
                            (2. * C56)) *
                       C128) +
                  C152 * C46 * C58 / (2. * C56)) *
                     C143) +
                ((((((-0.033774 * (0.11125 * C65 + 1.) *
                          std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                      0.88026 * std::sqrt(C61)) +
                                                     0.49671 * C66)) +
                                   1.) *
                          C75 / (3.4198418683227306 * C56) +
                      (C84 -
                       0.03109 * (0.20548 * C65 + 1.) *
                           std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.)) *
                          C75 / (2. * C56)) *
                         C121 +
                     C46 *
                         (-0.033774 *
                              (0.11125 * std::log(1. / (0.033774 *
                                                        (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66)) +
                                                  1.) -
                               (0.11125 * C65 + 1.) * 0.033774 *
                                   (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                                   (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                          0.88026 * std::sqrt(C61)) +
                                                         0.49671 * C66),
                                             2) *
                                    (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                        0.88026 * std::sqrt(C61)) +
                                                       0.49671 * C66)) +
                                     1.))) *
                              C75 / (3.4198418683227306 * C56) -
                          0.03109 *
                              (0.20548 * std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) +
                                                                     6.1977 * C65) +
                                                                    3.3662 * std::sqrt(C61)) +
                                                                   0.62517 * C66)) +
                                                  1.) -
                               (0.20548 * C65 + 1.) * 0.03109 *
                                   (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                                   (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66),
                                             2) *
                                    (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                     1.))) *
                              C75 / (2. * C56)) *
                         C134) +
                    C46 *
                        (-0.033774 * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                              3.4198418683227306 * C56) -
                         C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                             (2 * std::sqrt(C61) *
                              std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.) *
                              2. * C56)) *
                        C131) +
                   C46 *
                       (-0.033774 * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                            (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                   0.88026 * std::sqrt(C61)) +
                                                  0.49671 * C66),
                                      2) *
                             (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                 0.88026 * std::sqrt(C61)) +
                                                0.49671 * C66)) +
                              1.) *
                             3.4198418683227306 * C56) -
                        C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                            (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                  3.3662 * std::sqrt(C61)) +
                                                 0.62517 * C66),
                                      2) *
                             (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                3.3662 * std::sqrt(C61)) +
                                               0.62517 * C66)) +
                              1.) *
                             2. * C56)) *
                       C135) +
                  C46 *
                      (-0.033774 * (0.11125 * C65 + 1.) *
                           std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.) /
                           (3.4198418683227306 * C56) +
                       (C84 -
                        0.03109 * (0.20548 * C65 + 1.) *
                            std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                     1.)) /
                           (2. * C56)) *
                      C143) +
                 C152 * C46 * C75 / (2. * C56)) *
                    C128) +
               (((C58 * C75 / (2. * C56) - 1) * C121 + C143 * C46 * C58 / (2. * C56)) +
                C128 * C46 * C75 / (2. * C56)) *
                   C152) +
              ((((((0.033774 *
                        (0.11125 *
                             std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                         0.88026 * std::sqrt(C61)) +
                                                        0.49671 * C66)) +
                                      1.) -
                         (0.11125 * C65 + 1.) * 0.033774 *
                             (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                             (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                    0.88026 * std::sqrt(C61)) +
                                                   0.49671 * C66),
                                       2) *
                              (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.))) *
                        C75 * (1. - C58) / (3.4198418683227306 * C56) -
                    C58 * 0.03109 *
                        (0.20548 *
                             std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                        3.3662 * std::sqrt(C61)) +
                                                       0.62517 * C66)) +
                                      1.) -
                         (0.20548 * C65 + 1.) * 0.03109 *
                             (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                             (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                   3.3662 * std::sqrt(C61)) +
                                                  0.62517 * C66),
                                       2) *
                              (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66)) +
                               1.))) *
                        C75 / (2. * C56)) *
                       C134 +
                   (0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                        (2 * std::sqrt(C61) *
                         std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                               0.88026 * std::sqrt(C61)) +
                                              0.49671 * C66),
                                  2) *
                         (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                             0.88026 * std::sqrt(C61)) +
                                            0.49671 * C66)) +
                          1.) *
                         3.4198418683227306 * C56) -
                    C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                        (2 * std::sqrt(C61) *
                         std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                              3.3662 * std::sqrt(C61)) +
                                             0.62517 * C66),
                                  2) *
                         (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                            3.3662 * std::sqrt(C61)) +
                                           0.62517 * C66)) +
                          1.) *
                         2. * C56)) *
                       C131) +
                  (0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                       (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                              0.88026 * std::sqrt(C61)) +
                                             0.49671 * C66),
                                 2) *
                        (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                            0.88026 * std::sqrt(C61)) +
                                           0.49671 * C66)) +
                         1.) *
                        3.4198418683227306 * C56) -
                   C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                       (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                             3.3662 * std::sqrt(C61)) +
                                            0.62517 * C66),
                                 2) *
                        (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                           3.3662 * std::sqrt(C61)) +
                                          0.62517 * C66)) +
                         1.) *
                        2. * C56)) *
                      C135) +
                 (0.033774 * (0.11125 * C65 + 1.) *
                      std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66)) +
                               1.) *
                      (1. - C58) / (3.4198418683227306 * C56) +
                  C58 *
                      (C84 -
                       0.03109 * (0.20548 * C65 + 1.) *
                           std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.)) /
                      (2. * C56)) *
                     C143) +
                (-0.033774 * (0.11125 * C65 + 1.) *
                     std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                 0.88026 * std::sqrt(C61)) +
                                                0.49671 * C66)) +
                              1.) *
                     C75 / (3.4198418683227306 * C56) +
                 (C84 - 0.03109 * (0.20548 * C65 + 1.) *
                            std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                       3.3662 * std::sqrt(C61)) +
                                                      0.62517 * C66)) +
                                     1.)) *
                     C75 / (2. * C56)) *
                    C128) +
               (C58 * C75 / (2. * C56) - 1) * C152) *
                  C121) +
             C46 *
                 (0.033774 *
                      (0.11125 *
                           std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                       0.88026 * std::sqrt(C61)) +
                                                      0.49671 * C66)) +
                                    1.) -
                       (0.11125 * C65 + 1.) * 0.033774 * (10.357 / (2 * std::sqrt(C65)) + 3.6231) /
                           (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                  0.88026 * std::sqrt(C61)) +
                                                 0.49671 * C66),
                                     2) *
                            (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                                0.88026 * std::sqrt(C61)) +
                                               0.49671 * C66)) +
                             1.))) *
                      C75 * (1. - C58) / (3.4198418683227306 * C56) -
                  C58 * 0.03109 *
                      (0.20548 *
                           std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                      3.3662 * std::sqrt(C61)) +
                                                     0.62517 * C66)) +
                                    1.) -
                       (0.20548 * C65 + 1.) * 0.03109 *
                           (0.141189e2 / (2 * std::sqrt(C65)) + 6.1977) /
                           (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                 3.3662 * std::sqrt(C61)) +
                                                0.62517 * C66),
                                     2) *
                            (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                               3.3662 * std::sqrt(C61)) +
                                              0.62517 * C66)) +
                             1.))) *
                      C75 / (2. * C56)) *
                 C218) +
            C46 *
                (0.033774 * (1. - C58) * C75 * -0.02972990124 * (0.11125 * C65 + 1.) /
                     (2 * std::sqrt(C61) *
                      std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                            0.88026 * std::sqrt(C61)) +
                                           0.49671 * C66),
                               2) *
                      (1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                          0.88026 * std::sqrt(C61)) +
                                         0.49671 * C66)) +
                       1.) *
                      3.4198418683227306 * C56) -
                 C58 * C75 * -0.00325372886222 * (0.20548 * C65 + 1.) /
                     (2 * std::sqrt(C61) *
                      std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                           3.3662 * std::sqrt(C61)) +
                                          0.62517 * C66),
                               2) *
                      (1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                         3.3662 * std::sqrt(C61)) +
                                        0.62517 * C66)) +
                       1.) *
                      2. * C56)) *
                C215) +
           C46 *
               (0.033774 * (1. - C58) * C75 * -0.01677588354 * (0.11125 * C65 + 1.) /
                    (std::pow(0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                           0.88026 * std::sqrt(C61)) +
                                          0.49671 * C66),
                              2) *
                     (1. / (0.033774 *
                            (((10.357 * std::sqrt(C65) + 3.6231 * C65) + 0.88026 * std::sqrt(C61)) +
                             0.49671 * C66)) +
                      1.) *
                     3.4198418683227306 * C56) -
                C58 * C75 * -0.000604281882477 * (0.20548 * C65 + 1.) /
                    (std::pow(0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                          3.3662 * std::sqrt(C61)) +
                                         0.62517 * C66),
                              2) *
                     (1. / (0.03109 *
                            (((14.1189 * std::sqrt(C65) + 6.1977 * C65) + 3.3662 * std::sqrt(C61)) +
                             0.62517 * C66)) +
                      1.) *
                     2. * C56)) *
               C219) +
          C46 *
              (0.033774 * (0.11125 * C65 + 1.) *
                   std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                               0.88026 * std::sqrt(C61)) +
                                              0.49671 * C66)) +
                            1.) *
                   (1. - C58) / (3.4198418683227306 * C56) +
               C58 *
                   (C84 - 0.03109 * (0.20548 * C65 + 1.) *
                              std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                         3.3662 * std::sqrt(C61)) +
                                                        0.62517 * C66)) +
                                       1.)) /
                   (2. * C56)) *
              C227) +
         C46 *
             (-0.033774 * (0.11125 * C65 + 1.) *
                  std::log(1. / (0.033774 * (((10.357 * std::sqrt(C65) + 3.6231 * C65) +
                                              0.88026 * std::sqrt(C61)) +
                                             0.49671 * C66)) +
                           1.) *
                  C75 / (3.4198418683227306 * C56) +
              (C84 - 0.03109 * (0.20548 * C65 + 1.) *
                         std::log(1. / (0.03109 * (((14.1189 * std::sqrt(C65) + 6.1977 * C65) +
                                                    3.3662 * std::sqrt(C61)) +
                                                   0.62517 * C66)) +
                                  1.)) *
                  C75 / (2. * C56)) *
             C212) +
        C46 * (C58 * C75 / (2. * C56) - 1) * C236;
}

} // namespace excgrid
