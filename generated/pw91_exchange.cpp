// excgrid codegen source: the Perdew-Wang 1991 GGA exchange functional.
// Fresh authorship; formula from the PW91 paper (docs/mainpage.md
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

#include "excgrid/internal/asinh_over_x.hpp"

#include <cmath>
#include <limits>

namespace excgrid {

XcKernelValue Pw91Exchange(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
    (void)sigmaAb;

    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;
    // The sigma-edge guard the generator adds to a vanishing gradient radical
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

    const double C289 = 2. * rhoA;
    const double C290 = 2. * rhoB;
    const double C291 = 4. * sigmaAa;
    const double C292 = 4. * sigmaBb;
    const double C293 = 1. / 3.;
    const double C294 = std::pow(Pi, 2);
    const double C295 = C294 * C289;
    const double C296 = C294 * C290;
    const double C297 = std::sqrt(C291 + 4. * SigmaGuard);
    const double C298 = std::sqrt(C292 + 4. * SigmaGuard);
    const double C299 = 0.19645 * C297;
    const double C300 = 0.19645 * C298;
    const double C301 = 3. * C295;
    const double C302 = 3. * C296;
    const double C303 = 7.7956 * C297;
    const double C304 = 7.7956 * C298;
    const double C305 = std::pow(C301, C293);
    const double C306 = std::pow(C302, C293);
    const double C307 = C305 * C289;
    const double C308 = C306 * C290;
    const double C309 = C307 + 1e-16;
    const double C310 = C308 + 1e-16;
    const double C311 = 2. * C309;
    const double C312 = 2. * C310;
    const double C313 = C297 / C311;
    const double C314 = C298 / C312;
    const double C315 = C303 / C311;
    const double C316 = C304 / C312;
    const double C317 = std::pow(C313, 2);
    const double C318 = std::pow(C314, 2);
    const double C319 = std::pow(C315, 2);
    const double C320 = std::pow(C316, 2);
    const double C321 = C319 + 1.;
    const double C322 = C320 + 1.;
    const double C323 = std::sqrt(C321);
    const double C324 = std::sqrt(C322);
    const double C325 = C315 + C323;
    const double C326 = C316 + C324;
    const double C327 = std::log(C325);
    const double C328 = std::log(C326);
    const double C329 = C327 * C299;
    const double C330 = C328 * C300;
    const double C331 = C329 / C311;
    const double C332 = C330 / C312;

    result.exc = (-((C332 + (0.2743 - 0.1508 * std::exp(-100. * C318)) * C318) + 1.) * rhoB * 3. *
                  C306 / (4. * Pi)) /
                     ((C332 + 0.004 * std::pow(C314, 4)) + 1.) -
                 ((C331 + (0.2743 - 0.1508 * std::exp(-100. * C317)) * C317) + 1.) * rhoA * 3. *
                     C305 / (4. * Pi * ((C331 + 0.004 * std::pow(C313, 4)) + 1.));

    const double C338 = 2.;
    const double C339 = C294 * C338;
    const double C340 = 3. * C339;
    const double C341 = C293 * std::pow(C301 + 6. * std::pow(Pi, 2) * SigmaGuard, C293 - 1) * C340;
    const double C342 = C289 * C341 + C305 * C338;
    const double C343 = C342;
    const double C344 = 2. * C343;
    const double C345 = (-C344 * std::sqrt(C291 + 4. * SigmaGuard)) / std::pow(C311, 2);
    const double C346 = (-C344 * C303) / std::pow(C311, 2);
    const double C347 = 2 * C313 * C345;
    const double C348 = 2 * C315 * C346;
    const double C349 = C348;
    const double C350 = C346 + C349 / (2 * std::sqrt(C321));
    const double C351 = C350 / C325;
    const double C352 = C299 * C351;
    const double C353 = C352 / C311 - C344 * C329 / std::pow(C311, 2);

    result.vrhoA = C345 * ((C331 + (0.2743 - 0.1508 * std::exp(-100. * C317)) * C317) + 1.) * rhoA *
                       3. * C305 * 4. * Pi * 0.016 * std::pow(C313, 3) /
                       std::pow(4. * Pi * ((C331 + 0.004 * std::pow(C313, 4)) + 1.), 2) -
                   (C341 * ((C331 + (0.2743 - 0.1508 * std::exp(-100. * C317)) * C317) + 1.) * 3. *
                        rhoA / (4. * Pi * ((C331 + 0.004 * std::pow(C313, 4)) + 1.)) +
                    (C347 *
                         ((0.2743 - 0.1508 * std::exp(-100. * C317)) -
                          -0.1508e2 * std::exp(-100. * C317) * C317) *
                         rhoA * 3. * C305 / (4. * Pi * ((C331 + 0.004 * std::pow(C313, 4)) + 1.)) +
                     (C353 *
                          (4. * Pi * ((C331 + 0.004 * std::pow(C313, 4)) + 1.) * rhoA * 3. * C305 -
                           ((C331 + (0.2743 - 0.1508 * std::exp(-100. * C317)) * C317) + 1.) *
                               rhoA * 3. * C305 * 4. * Pi) /
                          std::pow(4. * Pi * ((C331 + 0.004 * std::pow(C313, 4)) + 1.), 2) +
                      ((C331 + (0.2743 - 0.1508 * std::exp(-100. * C317)) * C317) + 1.) * 3. *
                          C305 / (4. * Pi * ((C331 + 0.004 * std::pow(C313, 4)) + 1.)))));

    const double C356 = 2.;
    const double C357 = C294 * C356;
    const double C358 = 3. * C357;
    const double C359 = C293 * std::pow(C302 + 6. * std::pow(Pi, 2) * SigmaGuard, C293 - 1) * C358;
    const double C360 = C290 * C359 + C306 * C356;
    const double C361 = C360;
    const double C362 = 2. * C361;
    const double C363 = (-C362 * std::sqrt(C292 + 4. * SigmaGuard)) / std::pow(C312, 2);
    const double C364 = (-C362 * C304) / std::pow(C312, 2);
    const double C365 = 2 * C314 * C363;
    const double C366 = 2 * C316 * C364;
    const double C367 = C366;
    const double C368 = C364 + C367 / (2 * std::sqrt(C322));
    const double C369 = C368 / C326;
    const double C370 = C300 * C369;
    const double C371 = C370 / C312 - C362 * C330 / std::pow(C312, 2);

    result.vrhoB =
        (((C371 *
               (((C332 + (0.2743 - 0.1508 * std::exp(-100. * C318)) * C318) + 1.) * rhoB * 3. *
                    C306 / (4. * Pi) -
                ((C332 + 0.004 * std::pow(C314, 4)) + 1.) * rhoB * 3. * C306 / (4. * Pi)) /
               std::pow((C332 + 0.004 * std::pow(C314, 4)) + 1., 2) -
           ((C332 + (0.2743 - 0.1508 * std::exp(-100. * C318)) * C318) + 1.) * 3. * C306 /
               (4. * Pi * ((C332 + 0.004 * std::pow(C314, 4)) + 1.))) -
          C365 *
              ((0.2743 - 0.1508 * std::exp(-100. * C318)) -
               -0.1508e2 * std::exp(-100. * C318) * C318) *
              rhoB * 3. * C306 / (4. * Pi * ((C332 + 0.004 * std::pow(C314, 4)) + 1.))) -
         C359 * ((C332 + (0.2743 - 0.1508 * std::exp(-100. * C318)) * C318) + 1.) * 3. * rhoB /
             (4. * Pi * ((C332 + 0.004 * std::pow(C314, 4)) + 1.))) +
        C363 * 0.016 * std::pow(C314, 3) *
            ((C332 + (0.2743 - 0.1508 * std::exp(-100. * C318)) * C318) + 1.) * rhoB * 3. * C306 /
            (4. * Pi * std::pow((C332 + 0.004 * std::pow(C314, 4)) + 1., 2));

    const double C374 = 4.;
    const double C375 = 0.19645 * C374 / (2 * std::sqrt(C291 + 4. * SigmaGuard));
    const double C376 = 7.7956 * C374 / (2 * std::sqrt(C291 + 4. * SigmaGuard));
    const double C377 = C374 / (2 * std::sqrt(C291 + 4. * SigmaGuard) * C311);
    const double C378 = C376 / C311;
    const double C379 = 2 * C313 * C377;
    const double C380 = 2 * C315 * C378;
    const double C381 = C380;
    const double C382 = C378 + C381 / (2 * std::sqrt(C321));
    const double C383 = C382 / C325;
    const double C384 = C299 * C383 + C327 * C375;
    const double C385 = C384 / C311;

    result.vsigmaAa =
        C377 * ((C331 + (0.2743 - 0.1508 * std::exp(-100. * C317)) * C317) + 1.) * rhoA * 3. *
            C305 * 4. * Pi * 0.016 * std::pow(C313, 3) /
            std::pow(4. * Pi * ((C331 + 0.004 * std::pow(C313, 4)) + 1.), 2) -
        (C379 *
             ((0.2743 - 0.1508 * std::exp(-100. * C317)) -
              -0.1508e2 * std::exp(-100. * C317) * C317) *
             rhoA * 3. * C305 / (4. * Pi * ((C331 + 0.004 * std::pow(C313, 4)) + 1.)) +
         C385 *
             (4. * Pi * ((C331 + 0.004 * std::pow(C313, 4)) + 1.) * rhoA * 3. * C305 -
              ((C331 + (0.2743 - 0.1508 * std::exp(-100. * C317)) * C317) + 1.) * rhoA * 3. * C305 *
                  4. * Pi) /
             std::pow(4. * Pi * ((C331 + 0.004 * std::pow(C313, 4)) + 1.), 2));

    result.vsigmaAb = 0;

    const double C386 = 4.;
    const double C387 = 0.19645 * C386 / (2 * std::sqrt(C292 + 4. * SigmaGuard));
    const double C388 = 7.7956 * C386 / (2 * std::sqrt(C292 + 4. * SigmaGuard));
    const double C389 = C386 / (2 * std::sqrt(C292 + 4. * SigmaGuard) * C312);
    const double C390 = C388 / C312;
    const double C391 = 2 * C314 * C389;
    const double C392 = 2 * C316 * C390;
    const double C393 = C392;
    const double C394 = C390 + C393 / (2 * std::sqrt(C322));
    const double C395 = C394 / C326;
    const double C396 = C300 * C395 + C328 * C387;
    const double C397 = C396 / C312;

    result.vsigmaBb =
        (C397 *
             (((C332 + (0.2743 - 0.1508 * std::exp(-100. * C318)) * C318) + 1.) * rhoB * 3. * C306 /
                  (4. * Pi) -
              ((C332 + 0.004 * std::pow(C314, 4)) + 1.) * rhoB * 3. * C306 / (4. * Pi)) /
             std::pow((C332 + 0.004 * std::pow(C314, 4)) + 1., 2) -
         C391 *
             ((0.2743 - 0.1508 * std::exp(-100. * C318)) -
              -0.1508e2 * std::exp(-100. * C318) * C318) *
             rhoB * 3. * C306 / (4. * Pi * ((C332 + 0.004 * std::pow(C314, 4)) + 1.))) +
        C389 * 0.016 * std::pow(C314, 3) *
            ((C332 + (0.2743 - 0.1508 * std::exp(-100. * C318)) * C318) + 1.) * rhoB * 3. * C306 /
            (4. * Pi * std::pow((C332 + 0.004 * std::pow(C314, 4)) + 1., 2));

    return result;
}

void Pw91ExchangeSecondDerivatives(double rhoA,
                                   double rhoB,
                                   double sigmaAa,
                                   double sigmaAb,
                                   double sigmaBb,
                                   PointSecondDerivativeMatrix& matrix) {
    (void)sigmaAb;

    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;
    [[maybe_unused]] constexpr double SigmaGuard = 1e-300;

    if (rhoA + rhoB < 2. * std::numeric_limits<double>::epsilon())
    {
        return;
    }

    const double C46 = 2. * rhoA;
    const double C48 = 4. * SigmaGuard;
    const double C49 = 4. * sigmaAa;
    const double C51 = 1. / 3.;
    const double C52 = std::pow(Pi, 2);
    const double C53 = C52 * C46;
    const double C55 = C49 + C48;
    const double C57 = 3. * C53;
    const double C59 = std::sqrt(C55);
    const double C61 = 0.19645 * C59;
    const double C63 = 7.7956 * C59;
    const double C65 = std::pow(C57, C51);
    const double C67 = C65 * C46;
    const double C69 = C67 + 1e-16;
    const double C71 = 2. * C69;
    const double C73 = C59 / C71;
    const double C75 = C63 / C71;
    const double C77 = std::pow(C73, 2);
    const double C79 = std::pow(C75, 2);
    const double C81 = C79 + 1.;
    const double C83 = std::sqrt(C81);
    const double C85 = C75 + C83;
    const double C87 = std::log(C85);
    const double C89 = C87 * C61;
    const double C91 = C89 / C71;
    const double C97 = 2.;
    const double C98 = C52 * C97;
    const double C99 = 3. * C98;
    const double C100 = C51 * std::pow(C57, C51 - 1) * C99;
    const double C101 = C46 * C100 + C65 * C97;
    const double C102 = C101;
    const double C103 = 2. * C102;
    const double C104 = (-C103 * std::sqrt(C55)) / std::pow(C71, 2);
    const double C105 = (-C103 * C63) / std::pow(C71, 2);
    const double C106 = 2 * C73 * C104;
    const double C107 = 2 * C75 * C105;
    const double C108 = C107;
    const double C109 = C105 + C108 / (2 * std::sqrt(C81));
    const double C110 = C109 / C85;
    const double C111 = C61 * C110;
    const double C112 = C111 / C71 - C103 * C89 / std::pow(C71, 2);
    const double C155 = C51 * (C51 - 1) * std::pow(C57, C51 - 2) * std::pow(C99, 2);
    const double C156 = (C100 * C97 + C46 * C155) + C97 * C100;
    const double C157 = C156;
    const double C158 = 2. * C157;
    const double C159 = C103 * C103 * std::sqrt(C55) * 2 * C71 / std::pow(C71, 4) -
                        C158 * std::sqrt(C55) / std::pow(C71, 2);
    const double C160 =
        C103 * C103 * C63 * 2 * C71 / std::pow(C71, 4) - C158 * C63 / std::pow(C71, 2);
    const double C161 = 2 * std::pow(C104, 2) + 2 * C73 * C159;
    const double C162 = 2 * std::pow(C105, 2) + 2 * C75 * C160;
    const double C163 = C162;
    const double C164 = (C160 + C163 / (2 * std::sqrt(C81))) -
                        std::pow(C108, 2) / (std::sqrt(C81) * std::pow(2 * std::sqrt(C81), 2));
    const double C165 = C164 / C85 - std::pow(C109, 2) / std::pow(C85, 2);
    const double C166 = C61 * C165;
    const double C167 =
        ((C166 / C71 +
          ((-C111) / std::pow(C71, 2) - (-C103 * C89 * 2 * C71) / std::pow(C71, 4)) * C103) -
         C158 * C89 / std::pow(C71, 2)) -
        C111 * C103 / std::pow(C71, 2);
    matrix.upper[0] =
        ((((((((C104 * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * 3. * C65 *
                    4. * Pi * 0.016 * std::pow(C73, 3) /
                    std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) -
                (C100 * 3. * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) /
                     (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) +
                 (C106 *
                      ((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                       -0.1508e2 * std::exp(-100. * C77) * C77) *
                      3. * C65 / (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) +
                  C112 *
                      (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.) * 3. * C65 -
                       ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * 3. * C65 *
                           4. * Pi) /
                      std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2)))) +
               C159 * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. *
                   C65 * 4. * Pi * 0.016 * std::pow(C73, 3) /
                   std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2)) +
              ((std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) * C104 * rhoA * 3. *
                    C65 * 4. * Pi * 0.016 * std::pow(C73, 3) -
                C104 * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. *
                    C65 * 4. * Pi * 0.016 * std::pow(C73, 3) * 8. * Pi * 4. * Pi *
                    ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) /
                   std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 4) -
               ((4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.) * C100 * 3. * rhoA -
                 C100 * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * 3. * rhoA *
                     4. * Pi) /
                    std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) +
                (((std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) * C112 *
                       (4. * Pi * rhoA * 3. * C65 - rhoA * 3. * C65 * 4. * Pi) -
                   C112 *
                       (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.) * rhoA * 3. * C65 -
                        ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. *
                            C65 * 4. * Pi) *
                       8. * Pi * 4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) /
                      std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 4) +
                  (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.) * 3. * C65 -
                   ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * 3. * C65 * 4. *
                       Pi) /
                      std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2)) -
                 C106 *
                     ((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                      -0.1508e2 * std::exp(-100. * C77) * C77) *
                     rhoA * 3. * C65 * 4. * Pi /
                     std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2)))) *
                  C112) +
             (C104 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                   -0.1508e2 * std::exp(-100. * C77) * C77) *
                  rhoA * 3. * C65 * 4. * Pi * 0.016 * std::pow(C73, 3) /
                  std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) -
              (C100 *
                   ((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                    -0.1508e2 * std::exp(-100. * C77) * C77) *
                   3. * rhoA / (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) +
               (C106 *
                    (0.1508e2 * (std::exp(-100. * C77) - 100. * std::exp(-100. * C77) * C77) -
                     -0.1508e2 * std::exp(-100. * C77)) *
                    rhoA * 3. * C65 / (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) +
                (((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                  -0.1508e2 * std::exp(-100. * C77) * C77) *
                     3. * C65 / (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) -
                 C112 *
                     ((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                      -0.1508e2 * std::exp(-100. * C77) * C77) *
                     rhoA * 3. * C65 * 4. * Pi /
                     std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2))))) *
                 C106) +
            (C104 * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * 3. * rhoA *
                 4. * Pi * 0.016 * std::pow(C73, 3) /
                 std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) -
             (C106 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                   -0.1508e2 * std::exp(-100. * C77) * C77) *
                  3. * rhoA / (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) +
              (C112 *
                   (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.) * 3. * rhoA -
                    ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * 3. * rhoA *
                        4. * Pi) /
                   std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) +
               3. * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) /
                   (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.))))) *
                C100) +
           ((std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) * C104 *
                 ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. * C65 *
                 4. * Pi * 0.048 * std::pow(C73, 2) -
             C104 * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. *
                 C65 * 4. * Pi * 0.016 * std::pow(C73, 3) * 8. * Pi * 0.016 * std::pow(C73, 3) *
                 4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) /
                std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 4) -
            ((((std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) * C112 * 4. * Pi *
                    0.016 * std::pow(C73, 3) * rhoA * 3. * C65 -
                C112 *
                    (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.) * rhoA * 3. * C65 -
                     ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. *
                         C65 * 4. * Pi) *
                    8. * Pi * 0.016 * std::pow(C73, 3) * 4. * Pi *
                    ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) /
                   std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 4) -
               ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * 3. * C65 * 4. * Pi *
                   0.016 * std::pow(C73, 3) /
                   std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2)) -
              C106 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                   -0.1508e2 * std::exp(-100. * C77) * C77) *
                  rhoA * 3. * C65 * 4. * Pi * 0.016 * std::pow(C73, 3) /
                  std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2)) -
             C100 * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * 3. * rhoA *
                 4. * Pi * 0.016 * std::pow(C73, 3) /
                 std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2))) *
               C104) -
          C155 * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * 3. * rhoA /
              (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.))) -
         C161 *
             ((0.2743 - 0.1508 * std::exp(-100. * C77)) - -0.1508e2 * std::exp(-100. * C77) * C77) *
             rhoA * 3. * C65 / (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.))) -
        C167 *
            (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.) * rhoA * 3. * C65 -
             ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. * C65 * 4. *
                 Pi) /
            std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2);

    matrix.upper[1] = 0;

    const double C129 = 4.;
    const double C130 = C129;
    const double C131 = 0.19645 * C130 / (2 * std::sqrt(C55));
    const double C132 = 7.7956 * C130 / (2 * std::sqrt(C55));
    const double C133 = C130 / (2 * std::sqrt(C55) * C71);
    const double C134 = C132 / C71;
    const double C135 = 2 * C73 * C133;
    const double C136 = 2 * C75 * C134;
    const double C137 = C136;
    const double C138 = C134 + C137 / (2 * std::sqrt(C81));
    const double C139 = C138 / C85;
    const double C140 = C61 * C139 + C87 * C131;
    const double C141 = C140 / C71;
    const double C168 = (-C103 * C130 * 2 * std::sqrt(C55)) / std::pow(2 * std::sqrt(C55) * C71, 2);
    const double C169 = (-C103 * C132) / std::pow(C71, 2);
    const double C170 = 2 * C133 * C104 + 2 * C73 * C168;
    const double C171 = 2 * C134 * C105 + 2 * C75 * C169;
    const double C172 = C171;
    const double C173 = (C169 + C172 / (2 * std::sqrt(C81))) -
                        C108 * C137 / (std::sqrt(C81) * std::pow(2 * std::sqrt(C81), 2));
    const double C174 = C173 / C85 - C109 * C138 / std::pow(C85, 2);
    const double C175 = C61 * C174 + C131 * C110;
    const double C176 = C175 / C71 - C103 * C140 / std::pow(C71, 2);
    matrix.upper[2] =
        (((((((C133 * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * 3. * C65 *
                   4. * Pi * 0.016 * std::pow(C73, 3) /
                   std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) -
               (C135 *
                    ((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                     -0.1508e2 * std::exp(-100. * C77) * C77) *
                    3. * C65 / (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) +
                C141 *
                    (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.) * 3. * C65 -
                     ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * 3. * C65 *
                         4. * Pi) /
                    std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2))) +
              C168 * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. *
                  C65 * 4. * Pi * 0.016 * std::pow(C73, 3) /
                  std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2)) +
             ((std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) * C133 * rhoA * 3. *
                   C65 * 4. * Pi * 0.016 * std::pow(C73, 3) -
               C133 * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. *
                   C65 * 4. * Pi * 0.016 * std::pow(C73, 3) * 8. * Pi * 4. * Pi *
                   ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) /
                  std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 4) -
              ((std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) * C141 *
                    (4. * Pi * rhoA * 3. * C65 - rhoA * 3. * C65 * 4. * Pi) -
                C141 *
                    (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.) * rhoA * 3. * C65 -
                     ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. *
                         C65 * 4. * Pi) *
                    8. * Pi * 4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) /
                   std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 4) -
               C135 *
                   ((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                    -0.1508e2 * std::exp(-100. * C77) * C77) *
                   rhoA * 3. * C65 * 4. * Pi /
                   std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2))) *
                 C112) +
            (C133 *
                 ((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                  -0.1508e2 * std::exp(-100. * C77) * C77) *
                 rhoA * 3. * C65 * 4. * Pi * 0.016 * std::pow(C73, 3) /
                 std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) -
             (C135 *
                  (0.1508e2 * (std::exp(-100. * C77) - 100. * std::exp(-100. * C77) * C77) -
                   -0.1508e2 * std::exp(-100. * C77)) *
                  rhoA * 3. * C65 / (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) -
              C141 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                   -0.1508e2 * std::exp(-100. * C77) * C77) *
                  rhoA * 3. * C65 * 4. * Pi /
                  std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2))) *
                C106) +
           (C133 * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * 3. * rhoA * 4. *
                Pi * 0.016 * std::pow(C73, 3) /
                std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) -
            (C135 *
                 ((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                  -0.1508e2 * std::exp(-100. * C77) * C77) *
                 3. * rhoA / (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) +
             C141 *
                 (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.) * 3. * rhoA -
                  ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * 3. * rhoA * 4. *
                      Pi) /
                 std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2))) *
               C100) +
          ((std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) * C133 *
                ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. * C65 *
                4. * Pi * 0.048 * std::pow(C73, 2) -
            C133 * ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. *
                C65 * 4. * Pi * 0.016 * std::pow(C73, 3) * 8. * Pi * 0.016 * std::pow(C73, 3) * 4. *
                Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) /
               std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 4) -
           ((std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2) * C141 * 4. * Pi *
                 0.016 * std::pow(C73, 3) * rhoA * 3. * C65 -
             C141 *
                 (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.) * rhoA * 3. * C65 -
                  ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. * C65 *
                      4. * Pi) *
                 8. * Pi * 0.016 * std::pow(C73, 3) * 4. * Pi *
                 ((C91 + 0.004 * std::pow(C73, 4)) + 1.)) /
                std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 4) -
            C135 *
                ((0.2743 - 0.1508 * std::exp(-100. * C77)) -
                 -0.1508e2 * std::exp(-100. * C77) * C77) *
                rhoA * 3. * C65 * 4. * Pi * 0.016 * std::pow(C73, 3) /
                std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2))) *
              C104) -
         C170 *
             ((0.2743 - 0.1508 * std::exp(-100. * C77)) - -0.1508e2 * std::exp(-100. * C77) * C77) *
             rhoA * 3. * C65 / (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.))) -
        C176 *
            (4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.) * rhoA * 3. * C65 -
             ((C91 + (0.2743 - 0.1508 * std::exp(-100. * C77)) * C77) + 1.) * rhoA * 3. * C65 * 4. *
                 Pi) /
            std::pow(4. * Pi * ((C91 + 0.004 * std::pow(C73, 4)) + 1.), 2);

    matrix.upper[3] = 0;

    matrix.upper[4] = 0;

    const double C47 = 2. * rhoB;
    const double C50 = 4. * sigmaBb;
    const double C54 = C52 * C47;
    const double C56 = C50 + C48;
    const double C58 = 3. * C54;
    const double C60 = std::sqrt(C56);
    const double C62 = 0.19645 * C60;
    const double C64 = 7.7956 * C60;
    const double C66 = std::pow(C58, C51);
    const double C68 = C66 * C47;
    const double C70 = C68 + 1e-16;
    const double C72 = 2. * C70;
    const double C74 = C60 / C72;
    const double C76 = C64 / C72;
    const double C78 = std::pow(C74, 2);
    const double C80 = std::pow(C76, 2);
    const double C82 = C80 + 1.;
    const double C84 = std::sqrt(C82);
    const double C86 = C76 + C84;
    const double C88 = std::log(C86);
    const double C90 = C88 * C62;
    const double C92 = C90 / C72;
    const double C113 = 2.;
    const double C114 = C52 * C113;
    const double C115 = 3. * C114;
    const double C116 = C51 * std::pow(C58, C51 - 1) * C115;
    const double C117 = C47 * C116 + C66 * C113;
    const double C118 = C117;
    const double C119 = 2. * C118;
    const double C120 = (-C119 * std::sqrt(C56)) / std::pow(C72, 2);
    const double C121 = (-C119 * C64) / std::pow(C72, 2);
    const double C122 = 2 * C74 * C120;
    const double C123 = 2 * C76 * C121;
    const double C124 = C123;
    const double C125 = C121 + C124 / (2 * std::sqrt(C82));
    const double C126 = C125 / C86;
    const double C127 = C62 * C126;
    const double C128 = C127 / C72 - C119 * C90 / std::pow(C72, 2);
    const double C177 = C51 * (C51 - 1) * std::pow(C58, C51 - 2) * std::pow(C115, 2);
    const double C178 = (C116 * C113 + C47 * C177) + C113 * C116;
    const double C179 = C178;
    const double C180 = 2. * C179;
    const double C181 = C119 * C119 * std::sqrt(C56) * 2 * C72 / std::pow(C72, 4) -
                        C180 * std::sqrt(C56) / std::pow(C72, 2);
    const double C182 =
        C119 * C119 * C64 * 2 * C72 / std::pow(C72, 4) - C180 * C64 / std::pow(C72, 2);
    const double C183 = 2 * std::pow(C120, 2) + 2 * C74 * C181;
    const double C184 = 2 * std::pow(C121, 2) + 2 * C76 * C182;
    const double C185 = C184;
    const double C186 = (C182 + C185 / (2 * std::sqrt(C82))) -
                        std::pow(C124, 2) / (std::sqrt(C82) * std::pow(2 * std::sqrt(C82), 2));
    const double C187 = C186 / C86 - std::pow(C125, 2) / std::pow(C86, 2);
    const double C188 = C62 * C187;
    const double C189 =
        ((C188 / C72 +
          ((-C127) / std::pow(C72, 2) - (-C119 * C90 * 2 * C72) / std::pow(C72, 4)) * C119) -
         C180 * C90 / std::pow(C72, 2)) -
        C127 * C119 / std::pow(C72, 2);
    matrix.upper[5] =
        ((((((((((C128 *
                      (((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * 3. * C66 /
                           (4. * Pi) -
                       ((C92 + 0.004 * std::pow(C74, 4)) + 1.) * 3. * C66 / (4. * Pi)) /
                      std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2) -
                  C122 *
                      ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
                       -0.1508e2 * std::exp(-100. * C78) * C78) *
                      3. * C66 / (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) -
                 C116 * 3. * ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) /
                     (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) +
                C120 * 0.016 * std::pow(C74, 3) *
                    ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * 3. * C66 /
                    (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2))) +
               C189 *
                   (((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * rhoB * 3. *
                        C66 / (4. * Pi) -
                    ((C92 + 0.004 * std::pow(C74, 4)) + 1.) * rhoB * 3. * C66 / (4. * Pi)) /
                   std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2)) +
              (((((-C128 *
                   (((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * rhoB * 3. *
                        C66 / (4. * Pi) -
                    ((C92 + 0.004 * std::pow(C74, 4)) + 1.) * rhoB * 3. * C66 / (4. * Pi)) *
                   2. * ((C92 + 0.004 * std::pow(C74, 4)) + 1.)) /
                      std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 4) -
                  (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.) * 3. * C66 -
                   ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * 3. * C66 * 4. *
                       Pi) /
                      std::pow(4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.), 2)) -
                 (-C122 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
                   -0.1508e2 * std::exp(-100. * C78) * C78) *
                  rhoB * 3. * C66 * 4. * Pi) /
                     std::pow(4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.), 2)) -
                (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.) * C116 * 3. * rhoB -
                 C116 * ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * 3. * rhoB *
                     4. * Pi) /
                    std::pow(4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.), 2)) +
               (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2) * C120 * 0.016 *
                    std::pow(C74, 3) * rhoB * 3. * C66 -
                C120 * 0.016 * std::pow(C74, 3) *
                    ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * rhoB * 3. *
                    C66 * 4. * Pi * 2. * ((C92 + 0.004 * std::pow(C74, 4)) + 1.)) /
                   std::pow(4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2), 2)) *
                  C128) +
             ((((C128 *
                     ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
                      -0.1508e2 * std::exp(-100. * C78) * C78) *
                     rhoB * 3. * C66 /
                     (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2)) -
                 ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
                  -0.1508e2 * std::exp(-100. * C78) * C78) *
                     3. * C66 / (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) -
                C122 *
                    (0.1508e2 * (std::exp(-100. * C78) - 100. * std::exp(-100. * C78) * C78) -
                     -0.1508e2 * std::exp(-100. * C78)) *
                    rhoB * 3. * C66 / (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) -
               C116 *
                   ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
                    -0.1508e2 * std::exp(-100. * C78) * C78) *
                   3. * rhoB / (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) +
              C120 * 0.016 * std::pow(C74, 3) *
                  ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
                   -0.1508e2 * std::exp(-100. * C78) * C78) *
                  rhoB * 3. * C66 /
                  (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2))) *
                 C122) +
            (((C128 *
                   (((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * 3. * rhoB /
                        (4. * Pi) -
                    ((C92 + 0.004 * std::pow(C74, 4)) + 1.) * 3. * rhoB / (4. * Pi)) /
                   std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2) -
               3. * ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) /
                   (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) -
              C122 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
                   -0.1508e2 * std::exp(-100. * C78) * C78) *
                  3. * rhoB / (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) +
             C120 * 0.016 * std::pow(C74, 3) *
                 ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * 3. * rhoB /
                 (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2))) *
                C116) +
           (((((-(std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2) * C128 * 0.016 *
                      std::pow(C74, 3) * rhoB * 3. * C66 / (4. * Pi) +
                  C128 *
                      (((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * rhoB * 3. *
                           C66 / (4. * Pi) -
                       ((C92 + 0.004 * std::pow(C74, 4)) + 1.) * rhoB * 3. * C66 / (4. * Pi)) *
                      0.032 * std::pow(C74, 3) * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) /
                   std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 4) -
               (-((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * 3. * C66 * 4. *
                Pi * 0.016 * std::pow(C74, 3)) /
                   std::pow(4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.), 2)) -
              (-C122 *
               ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
                -0.1508e2 * std::exp(-100. * C78) * C78) *
               rhoB * 3. * C66 * 4. * Pi * 0.016 * std::pow(C74, 3)) /
                  std::pow(4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.), 2)) -
             (-C116 * ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * 3. * rhoB *
              4. * Pi * 0.016 * std::pow(C74, 3)) /
                 std::pow(4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.), 2)) +
            (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2) * C120 * 0.048 *
                 std::pow(C74, 2) * ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) *
                 rhoB * 3. * C66 -
             C120 * 0.016 * std::pow(C74, 3) *
                 ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * rhoB * 3. * C66 *
                 4. * Pi * 0.032 * std::pow(C74, 3) * ((C92 + 0.004 * std::pow(C74, 4)) + 1.)) /
                std::pow(4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2), 2)) *
               C120) -
          C183 *
              ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
               -0.1508e2 * std::exp(-100. * C78) * C78) *
              rhoB * 3. * C66 / (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) -
         C177 * ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * 3. * rhoB /
             (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) +
        C181 * 0.016 * std::pow(C74, 3) *
            ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * rhoB * 3. * C66 /
            (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2));

    matrix.upper[6] = 0;

    matrix.upper[7] = 0;

    const double C142 = 4.;
    const double C143 = C142;
    const double C144 = 0.19645 * C143 / (2 * std::sqrt(C56));
    const double C145 = 7.7956 * C143 / (2 * std::sqrt(C56));
    const double C146 = C143 / (2 * std::sqrt(C56) * C72);
    const double C147 = C145 / C72;
    const double C148 = 2 * C74 * C146;
    const double C149 = 2 * C76 * C147;
    const double C150 = C149;
    const double C151 = C147 + C150 / (2 * std::sqrt(C82));
    const double C152 = C151 / C86;
    const double C153 = C62 * C152 + C88 * C144;
    const double C154 = C153 / C72;
    const double C190 = (-C119 * C143 * 2 * std::sqrt(C56)) / std::pow(2 * std::sqrt(C56) * C72, 2);
    const double C191 = (-C119 * C145) / std::pow(C72, 2);
    const double C192 = 2 * C146 * C120 + 2 * C74 * C190;
    const double C193 = 2 * C147 * C121 + 2 * C76 * C191;
    const double C194 = C193;
    const double C195 = (C191 + C194 / (2 * std::sqrt(C82))) -
                        C124 * C150 / (std::sqrt(C82) * std::pow(2 * std::sqrt(C82), 2));
    const double C196 = C195 / C86 - C125 * C151 / std::pow(C86, 2);
    const double C197 = C62 * C196 + C144 * C126;
    const double C198 = C197 / C72 - C119 * C153 / std::pow(C72, 2);
    matrix.upper[8] =
        ((((((((C154 *
                    (((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * 3. * C66 /
                         (4. * Pi) -
                     ((C92 + 0.004 * std::pow(C74, 4)) + 1.) * 3. * C66 / (4. * Pi)) /
                    std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2) -
                C148 *
                    ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
                     -0.1508e2 * std::exp(-100. * C78) * C78) *
                    3. * C66 / (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) +
               C146 * 0.016 * std::pow(C74, 3) *
                   ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * 3. * C66 /
                   (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2))) +
              C198 *
                  (((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * rhoB * 3. *
                       C66 / (4. * Pi) -
                   ((C92 + 0.004 * std::pow(C74, 4)) + 1.) * rhoB * 3. * C66 / (4. * Pi)) /
                  std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2)) +
             (((-C154 *
                (((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * rhoB * 3. * C66 /
                     (4. * Pi) -
                 ((C92 + 0.004 * std::pow(C74, 4)) + 1.) * rhoB * 3. * C66 / (4. * Pi)) *
                2. * ((C92 + 0.004 * std::pow(C74, 4)) + 1.)) /
                   std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 4) -
               (-C148 *
                ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
                 -0.1508e2 * std::exp(-100. * C78) * C78) *
                rhoB * 3. * C66 * 4. * Pi) /
                   std::pow(4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.), 2)) +
              (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2) * C146 * 0.016 *
                   std::pow(C74, 3) * rhoB * 3. * C66 -
               C146 * 0.016 * std::pow(C74, 3) *
                   ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * rhoB * 3. *
                   C66 * 4. * Pi * 2. * ((C92 + 0.004 * std::pow(C74, 4)) + 1.)) /
                  std::pow(4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2), 2)) *
                 C128) +
            ((C154 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
                   -0.1508e2 * std::exp(-100. * C78) * C78) *
                  rhoB * 3. * C66 / (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2)) -
              C148 *
                  (0.1508e2 * (std::exp(-100. * C78) - 100. * std::exp(-100. * C78) * C78) -
                   -0.1508e2 * std::exp(-100. * C78)) *
                  rhoB * 3. * C66 / (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) +
             C146 * 0.016 * std::pow(C74, 3) *
                 ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
                  -0.1508e2 * std::exp(-100. * C78) * C78) *
                 rhoB * 3. * C66 / (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2))) *
                C122) +
           ((C154 *
                 (((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * 3. * rhoB /
                      (4. * Pi) -
                  ((C92 + 0.004 * std::pow(C74, 4)) + 1.) * 3. * rhoB / (4. * Pi)) /
                 std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2) -
             C148 *
                 ((0.2743 - 0.1508 * std::exp(-100. * C78)) -
                  -0.1508e2 * std::exp(-100. * C78) * C78) *
                 3. * rhoB / (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) +
            C146 * 0.016 * std::pow(C74, 3) *
                ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * 3. * rhoB /
                (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2))) *
               C116) +
          (((-(std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2) * C154 * 0.016 *
                   std::pow(C74, 3) * rhoB * 3. * C66 / (4. * Pi) +
               C154 *
                   (((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * rhoB * 3. *
                        C66 / (4. * Pi) -
                    ((C92 + 0.004 * std::pow(C74, 4)) + 1.) * rhoB * 3. * C66 / (4. * Pi)) *
                   0.032 * std::pow(C74, 3) * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) /
                std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 4) -
            (-C148 *
             ((0.2743 - 0.1508 * std::exp(-100. * C78)) - -0.1508e2 * std::exp(-100. * C78) * C78) *
             rhoB * 3. * C66 * 4. * Pi * 0.016 * std::pow(C74, 3)) /
                std::pow(4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.), 2)) +
           (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2) * C146 * 0.048 *
                std::pow(C74, 2) * ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) *
                rhoB * 3. * C66 -
            C146 * 0.016 * std::pow(C74, 3) *
                ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * rhoB * 3. * C66 *
                4. * Pi * 0.032 * std::pow(C74, 3) * ((C92 + 0.004 * std::pow(C74, 4)) + 1.)) /
               std::pow(4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2), 2)) *
              C120) -
         C192 *
             ((0.2743 - 0.1508 * std::exp(-100. * C78)) - -0.1508e2 * std::exp(-100. * C78) * C78) *
             rhoB * 3. * C66 / (4. * Pi * ((C92 + 0.004 * std::pow(C74, 4)) + 1.))) +
        C190 * 0.016 * std::pow(C74, 3) *
            ((C92 + (0.2743 - 0.1508 * std::exp(-100. * C78)) * C78) + 1.) * rhoB * 3. * C66 /
            (4. * Pi * std::pow((C92 + 0.004 * std::pow(C74, 4)) + 1., 2));

    const double C199 = 2 * rhoA;
    const double C200 = 4 * sigmaAa;
    const double C201 = 1. / 3.;
    const double C202 = std::pow(Pi, 2);
    const double C203 = C202 * C199;
    const double C204 = std::sqrt(C200);
    const double C205 = 3. * C203;
    const double C206 = 7.7956 * C204;
    const double C207 = std::pow(C205, C201);
    const double C208 = C207 * C199;
    const double C209 = C208 + 1e-16;
    const double C210 = 2. * C209;
    const double C211 = C204 / C210;
    const double C212 = C206 / C210;
    const double C213 = excgrid::internal::AsinhOverX(C212);
    const double C214 = excgrid::internal::AsinhOverXDerivX2(C212);
    const double C215 = std::pow(C211, 2);
    const double C216 = std::pow(C211, 4);
    const double C217 = std::pow(C212, 2);
    const double C218 = 0.004 * C216;
    const double C219 = 0.008 * C215;
    const double C220 = 100 * C215;
    const double C221 = C215 * C213;
    const double C222 = C217 + 1;
    const double C223 = 1.53144562 * C221;
    const double C224 = -C220;
    const double C225 = std::sqrt(C222);
    const double C226 = C223 + C218;
    const double C227 = 1 / C225;
    const double C228 = std::exp(C224);
    const double C229 = 0.1508 * C228;
    const double C230 = C213 + C227;
    const double C231 = C226 + 1;
    const double C232 = std::pow(C227, 3);
    const double C233 = 1.53144562 * C230;
    const double C234 = 0.2743 - C229;
    const double C235 = C232 / 2;
    const double C236 = C234 * C215;
    const double C237 = C214 - C235;
    const double C238 = C233 / 2;
    const double C239 = 0.930680627422304e2 * C237;
    const double C240 = C223 + C236;
    const double C241 = C238 + C219;
    const double C242 = C240 + 1;
    const double C243 = C239 / 2;
    matrix.upper[9] = (-(((C243 + 0.1508e2 * C228 * (2 - C220)) * C231 - C242 * (C243 + 0.008)) /
                             std::pow(C231, 2) -
                         2 * (((C238 + C234) + 0.1508e2 * C228 * C215) * C231 - C242 * C241) *
                             C241 / std::pow(C231, 3)) *
                       rhoA * 3. * C207 / (4. * Pi)) /
                      std::pow(2 * C207 * rhoA + 1e-16, 4);

    matrix.upper[10] = 0;

    matrix.upper[11] = 0;

    matrix.upper[12] = 0;

    matrix.upper[13] = 0;

    const double C244 = 2 * rhoB;
    const double C245 = 4 * sigmaBb;
    const double C246 = 1. / 3.;
    const double C247 = std::pow(Pi, 2);
    const double C248 = C247 * C244;
    const double C249 = std::sqrt(C245);
    const double C250 = 3. * C248;
    const double C251 = 7.7956 * C249;
    const double C252 = std::pow(C250, C246);
    const double C253 = C252 * C244;
    const double C254 = C253 + 1e-16;
    const double C255 = 2. * C254;
    const double C256 = C249 / C255;
    const double C257 = C251 / C255;
    const double C258 = excgrid::internal::AsinhOverX(C257);
    const double C259 = excgrid::internal::AsinhOverXDerivX2(C257);
    const double C260 = std::pow(C256, 2);
    const double C261 = std::pow(C256, 4);
    const double C262 = std::pow(C257, 2);
    const double C263 = 0.004 * C261;
    const double C264 = 0.008 * C260;
    const double C265 = 100 * C260;
    const double C266 = C260 * C258;
    const double C267 = C262 + 1;
    const double C268 = 1.53144562 * C266;
    const double C269 = -C265;
    const double C270 = std::sqrt(C267);
    const double C271 = C268 + C263;
    const double C272 = 1 / C270;
    const double C273 = std::exp(C269);
    const double C274 = 0.1508 * C273;
    const double C275 = C258 + C272;
    const double C276 = C271 + 1;
    const double C277 = std::pow(C272, 3);
    const double C278 = 1.53144562 * C275;
    const double C279 = 0.2743 - C274;
    const double C280 = C277 / 2;
    const double C281 = C279 * C260;
    const double C282 = C259 - C280;
    const double C283 = C278 / 2;
    const double C284 = 0.930680627422304e2 * C282;
    const double C285 = C268 + C281;
    const double C286 = C283 + C264;
    const double C287 = C285 + 1;
    const double C288 = C284 / 2;
    matrix.upper[14] = (-(((C288 + 0.1508e2 * C273 * (2 - C265)) * C276 - C287 * (C288 + 0.008)) /
                              std::pow(C276, 2) -
                          2 * (((C283 + C279) + 0.1508e2 * C273 * C260) * C276 - C287 * C286) *
                              C286 / std::pow(C276, 3)) *
                        rhoB * 3. * C252 / (4. * Pi)) /
                       std::pow(2 * C252 * rhoB + 1e-16, 4);
}

} // namespace excgrid
