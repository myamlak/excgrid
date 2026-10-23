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

    const double C284 = 2. * rhoA;
    const double C285 = 2. * rhoB;
    const double C286 = 4. * sigmaAa;
    const double C287 = 4. * sigmaBb;
    const double C288 = 1. / 3.;
    const double C289 = std::pow(Pi, 2);
    const double C290 = C289 * C284;
    const double C291 = C289 * C285;
    const double C292 = std::sqrt(C286);
    const double C293 = std::sqrt(C287);
    const double C294 = 0.19645 * C292;
    const double C295 = 0.19645 * C293;
    const double C296 = 3. * C290;
    const double C297 = 3. * C291;
    const double C298 = 7.7956 * C292;
    const double C299 = 7.7956 * C293;
    const double C300 = std::pow(C296, C288);
    const double C301 = std::pow(C297, C288);
    const double C302 = C300 * C284;
    const double C303 = C301 * C285;
    const double C304 = C302 + 1e-16;
    const double C305 = C303 + 1e-16;
    const double C306 = 2. * C304;
    const double C307 = 2. * C305;
    const double C308 = C292 / C306;
    const double C309 = C293 / C307;
    const double C310 = C298 / C306;
    const double C311 = C299 / C307;
    const double C312 = std::pow(C308, 2);
    const double C313 = std::pow(C309, 2);
    const double C314 = std::pow(C310, 2);
    const double C315 = std::pow(C311, 2);
    const double C316 = C314 + 1.;
    const double C317 = C315 + 1.;
    const double C318 = std::sqrt(C316);
    const double C319 = std::sqrt(C317);
    const double C320 = C310 + C318;
    const double C321 = C311 + C319;
    const double C322 = std::log(C320);
    const double C323 = std::log(C321);
    const double C324 = C322 * C294;
    const double C325 = C323 * C295;
    const double C326 = C324 / C306;
    const double C327 = C325 / C307;

    result.exc = (-((C327 + (0.2743 - 0.1508 * std::exp(-100. * C313)) * C313) + 1.) * rhoB * 3. *
                  C301 / (4. * Pi)) /
                     ((C327 + 0.004 * std::pow(C309, 4)) + 1.) -
                 ((C326 + (0.2743 - 0.1508 * std::exp(-100. * C312)) * C312) + 1.) * rhoA * 3. *
                     C300 / (4. * Pi * ((C326 + 0.004 * std::pow(C308, 4)) + 1.));

    const double C329 = 2.;
    const double C330 = C289 * C329;
    const double C331 = 3. * C330;
    const double C332 = C288 * std::pow(C296, C288 - 1) * C331;
    const double C333 = C284 * C332 + C300 * C329;
    const double C334 = C333;
    const double C335 = 2. * C334;
    const double C336 = (-C335 * std::sqrt(C286)) / std::pow(C306, 2);
    const double C337 = (-C335 * C298) / std::pow(C306, 2);
    const double C338 = 2 * C308 * C336;
    const double C339 = 2 * C310 * C337;
    const double C340 = C339;
    const double C341 = C337 + C340 / (2 * std::sqrt(C316));
    const double C342 = C341 / C320;
    const double C343 = C294 * C342;
    const double C344 = C343 / C306 - C335 * C324 / std::pow(C306, 2);

    result.vrhoA = C336 * ((C326 + (0.2743 - 0.1508 * std::exp(-100. * C312)) * C312) + 1.) * rhoA *
                       3. * C300 * 4. * Pi * 0.016 * std::pow(C308, 3) /
                       std::pow(4. * Pi * ((C326 + 0.004 * std::pow(C308, 4)) + 1.), 2) -
                   (C332 * ((C326 + (0.2743 - 0.1508 * std::exp(-100. * C312)) * C312) + 1.) * 3. *
                        rhoA / (4. * Pi * ((C326 + 0.004 * std::pow(C308, 4)) + 1.)) +
                    (C338 *
                         ((0.2743 - 0.1508 * std::exp(-100. * C312)) -
                          -0.1508e2 * std::exp(-100. * C312) * C312) *
                         rhoA * 3. * C300 / (4. * Pi * ((C326 + 0.004 * std::pow(C308, 4)) + 1.)) +
                     (C344 *
                          (4. * Pi * ((C326 + 0.004 * std::pow(C308, 4)) + 1.) * rhoA * 3. * C300 -
                           ((C326 + (0.2743 - 0.1508 * std::exp(-100. * C312)) * C312) + 1.) *
                               rhoA * 3. * C300 * 4. * Pi) /
                          std::pow(4. * Pi * ((C326 + 0.004 * std::pow(C308, 4)) + 1.), 2) +
                      ((C326 + (0.2743 - 0.1508 * std::exp(-100. * C312)) * C312) + 1.) * 3. *
                          C300 / (4. * Pi * ((C326 + 0.004 * std::pow(C308, 4)) + 1.)))));

    const double C345 = 2.;
    const double C346 = C289 * C345;
    const double C347 = 3. * C346;
    const double C348 = C288 * std::pow(C297, C288 - 1) * C347;
    const double C349 = C285 * C348 + C301 * C345;
    const double C350 = C349;
    const double C351 = 2. * C350;
    const double C352 = (-C351 * std::sqrt(C287)) / std::pow(C307, 2);
    const double C353 = (-C351 * C299) / std::pow(C307, 2);
    const double C354 = 2 * C309 * C352;
    const double C355 = 2 * C311 * C353;
    const double C356 = C355;
    const double C357 = C353 + C356 / (2 * std::sqrt(C317));
    const double C358 = C357 / C321;
    const double C359 = C295 * C358;
    const double C360 = C359 / C307 - C351 * C325 / std::pow(C307, 2);

    result.vrhoB =
        (((C360 *
               (((C327 + (0.2743 - 0.1508 * std::exp(-100. * C313)) * C313) + 1.) * rhoB * 3. *
                    C301 / (4. * Pi) -
                ((C327 + 0.004 * std::pow(C309, 4)) + 1.) * rhoB * 3. * C301 / (4. * Pi)) /
               std::pow((C327 + 0.004 * std::pow(C309, 4)) + 1., 2) -
           ((C327 + (0.2743 - 0.1508 * std::exp(-100. * C313)) * C313) + 1.) * 3. * C301 /
               (4. * Pi * ((C327 + 0.004 * std::pow(C309, 4)) + 1.))) -
          C354 *
              ((0.2743 - 0.1508 * std::exp(-100. * C313)) -
               -0.1508e2 * std::exp(-100. * C313) * C313) *
              rhoB * 3. * C301 / (4. * Pi * ((C327 + 0.004 * std::pow(C309, 4)) + 1.))) -
         C348 * ((C327 + (0.2743 - 0.1508 * std::exp(-100. * C313)) * C313) + 1.) * 3. * rhoB /
             (4. * Pi * ((C327 + 0.004 * std::pow(C309, 4)) + 1.))) +
        C352 * 0.016 * std::pow(C309, 3) *
            ((C327 + (0.2743 - 0.1508 * std::exp(-100. * C313)) * C313) + 1.) * rhoB * 3. * C301 /
            (4. * Pi * std::pow((C327 + 0.004 * std::pow(C309, 4)) + 1., 2));

    const double C361 = 4.;
    const double C362 = 0.19645 * C361 / (2 * std::sqrt(C286));
    const double C363 = 7.7956 * C361 / (2 * std::sqrt(C286));
    const double C364 = C361 / (2 * std::sqrt(C286) * C306);
    const double C365 = C363 / C306;
    const double C366 = 2 * C308 * C364;
    const double C367 = 2 * C310 * C365;
    const double C368 = C367;
    const double C369 = C365 + C368 / (2 * std::sqrt(C316));
    const double C370 = C369 / C320;
    const double C371 = C294 * C370 + C322 * C362;
    const double C372 = C371 / C306;

    result.vsigmaAa =
        C364 * ((C326 + (0.2743 - 0.1508 * std::exp(-100. * C312)) * C312) + 1.) * rhoA * 3. *
            C300 * 4. * Pi * 0.016 * std::pow(C308, 3) /
            std::pow(4. * Pi * ((C326 + 0.004 * std::pow(C308, 4)) + 1.), 2) -
        (C366 *
             ((0.2743 - 0.1508 * std::exp(-100. * C312)) -
              -0.1508e2 * std::exp(-100. * C312) * C312) *
             rhoA * 3. * C300 / (4. * Pi * ((C326 + 0.004 * std::pow(C308, 4)) + 1.)) +
         C372 *
             (4. * Pi * ((C326 + 0.004 * std::pow(C308, 4)) + 1.) * rhoA * 3. * C300 -
              ((C326 + (0.2743 - 0.1508 * std::exp(-100. * C312)) * C312) + 1.) * rhoA * 3. * C300 *
                  4. * Pi) /
             std::pow(4. * Pi * ((C326 + 0.004 * std::pow(C308, 4)) + 1.), 2));

    result.vsigmaAb = 0;

    const double C373 = 4.;
    const double C374 = 0.19645 * C373 / (2 * std::sqrt(C287));
    const double C375 = 7.7956 * C373 / (2 * std::sqrt(C287));
    const double C376 = C373 / (2 * std::sqrt(C287) * C307);
    const double C377 = C375 / C307;
    const double C378 = 2 * C309 * C376;
    const double C379 = 2 * C311 * C377;
    const double C380 = C379;
    const double C381 = C377 + C380 / (2 * std::sqrt(C317));
    const double C382 = C381 / C321;
    const double C383 = C295 * C382 + C323 * C374;
    const double C384 = C383 / C307;

    result.vsigmaBb =
        (C384 *
             (((C327 + (0.2743 - 0.1508 * std::exp(-100. * C313)) * C313) + 1.) * rhoB * 3. * C301 /
                  (4. * Pi) -
              ((C327 + 0.004 * std::pow(C309, 4)) + 1.) * rhoB * 3. * C301 / (4. * Pi)) /
             std::pow((C327 + 0.004 * std::pow(C309, 4)) + 1., 2) -
         C378 *
             ((0.2743 - 0.1508 * std::exp(-100. * C313)) -
              -0.1508e2 * std::exp(-100. * C313) * C313) *
             rhoB * 3. * C301 / (4. * Pi * ((C327 + 0.004 * std::pow(C309, 4)) + 1.))) +
        C376 * 0.016 * std::pow(C309, 3) *
            ((C327 + (0.2743 - 0.1508 * std::exp(-100. * C313)) * C313) + 1.) * rhoB * 3. * C301 /
            (4. * Pi * std::pow((C327 + 0.004 * std::pow(C309, 4)) + 1., 2));

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
    const double C48 = 4. * sigmaAa;
    const double C50 = 1. / 3.;
    const double C51 = std::pow(Pi, 2);
    const double C52 = C51 * C46;
    const double C54 = std::sqrt(C48);
    const double C56 = 0.19645 * C54;
    const double C58 = 3. * C52;
    const double C60 = 7.7956 * C54;
    const double C62 = std::pow(C58, C50);
    const double C64 = C62 * C46;
    const double C66 = C64 + 1e-16;
    const double C68 = 2. * C66;
    const double C70 = C54 / C68;
    const double C72 = C60 / C68;
    const double C74 = std::pow(C70, 2);
    const double C76 = std::pow(C72, 2);
    const double C78 = C76 + 1.;
    const double C80 = std::sqrt(C78);
    const double C82 = C72 + C80;
    const double C84 = std::log(C82);
    const double C86 = C84 * C56;
    const double C88 = C86 / C68;
    const double C94 = 2.;
    const double C95 = C51 * C94;
    const double C96 = 3. * C95;
    const double C97 = C50 * std::pow(C58, C50 - 1) * C96;
    const double C98 = C46 * C97 + C62 * C94;
    const double C99 = C98;
    const double C100 = 2. * C99;
    const double C101 = (-C100 * std::sqrt(C48)) / std::pow(C68, 2);
    const double C102 = (-C100 * C60) / std::pow(C68, 2);
    const double C103 = 2 * C70 * C101;
    const double C104 = 2 * C72 * C102;
    const double C105 = C104;
    const double C106 = C102 + C105 / (2 * std::sqrt(C78));
    const double C107 = C106 / C82;
    const double C108 = C56 * C107;
    const double C109 = C108 / C68 - C100 * C86 / std::pow(C68, 2);
    const double C150 = C50 * (C50 - 1) * std::pow(C58, C50 - 2) * std::pow(C96, 2);
    const double C151 = (C97 * C94 + C46 * C150) + C94 * C97;
    const double C152 = C151;
    const double C153 = 2. * C152;
    const double C154 = C100 * C100 * std::sqrt(C48) * 2 * C68 / std::pow(C68, 4) -
                        C153 * std::sqrt(C48) / std::pow(C68, 2);
    const double C155 =
        C100 * C100 * C60 * 2 * C68 / std::pow(C68, 4) - C153 * C60 / std::pow(C68, 2);
    const double C156 = 2 * std::pow(C101, 2) + 2 * C70 * C154;
    const double C157 = 2 * std::pow(C102, 2) + 2 * C72 * C155;
    const double C158 = C157;
    const double C159 = (C155 + C158 / (2 * std::sqrt(C78))) -
                        std::pow(C105, 2) / (std::sqrt(C78) * std::pow(2 * std::sqrt(C78), 2));
    const double C160 = C159 / C82 - std::pow(C106, 2) / std::pow(C82, 2);
    const double C161 = C56 * C160;
    const double C162 =
        ((C161 / C68 +
          ((-C108) / std::pow(C68, 2) - (-C100 * C86 * 2 * C68) / std::pow(C68, 4)) * C100) -
         C153 * C86 / std::pow(C68, 2)) -
        C108 * C100 / std::pow(C68, 2);
    matrix.upper[0] =
        ((((((((C101 * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * 3. * C62 *
                    4. * Pi * 0.016 * std::pow(C70, 3) /
                    std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) -
                (C97 * 3. * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) /
                     (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) +
                 (C103 *
                      ((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                       -0.1508e2 * std::exp(-100. * C74) * C74) *
                      3. * C62 / (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) +
                  C109 *
                      (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.) * 3. * C62 -
                       ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * 3. * C62 *
                           4. * Pi) /
                      std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2)))) +
               C154 * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. *
                   C62 * 4. * Pi * 0.016 * std::pow(C70, 3) /
                   std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2)) +
              ((std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) * C101 * rhoA * 3. *
                    C62 * 4. * Pi * 0.016 * std::pow(C70, 3) -
                C101 * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. *
                    C62 * 4. * Pi * 0.016 * std::pow(C70, 3) * 8. * Pi * 4. * Pi *
                    ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) /
                   std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 4) -
               ((4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.) * C97 * 3. * rhoA -
                 C97 * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * 3. * rhoA *
                     4. * Pi) /
                    std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) +
                (((std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) * C109 *
                       (4. * Pi * rhoA * 3. * C62 - rhoA * 3. * C62 * 4. * Pi) -
                   C109 *
                       (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.) * rhoA * 3. * C62 -
                        ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. *
                            C62 * 4. * Pi) *
                       8. * Pi * 4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) /
                      std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 4) +
                  (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.) * 3. * C62 -
                   ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * 3. * C62 * 4. *
                       Pi) /
                      std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2)) -
                 C103 *
                     ((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                      -0.1508e2 * std::exp(-100. * C74) * C74) *
                     rhoA * 3. * C62 * 4. * Pi /
                     std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2)))) *
                  C109) +
             (C101 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                   -0.1508e2 * std::exp(-100. * C74) * C74) *
                  rhoA * 3. * C62 * 4. * Pi * 0.016 * std::pow(C70, 3) /
                  std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) -
              (C97 *
                   ((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                    -0.1508e2 * std::exp(-100. * C74) * C74) *
                   3. * rhoA / (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) +
               (C103 *
                    (0.1508e2 * (std::exp(-100. * C74) - 100. * std::exp(-100. * C74) * C74) -
                     -0.1508e2 * std::exp(-100. * C74)) *
                    rhoA * 3. * C62 / (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) +
                (((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                  -0.1508e2 * std::exp(-100. * C74) * C74) *
                     3. * C62 / (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) -
                 C109 *
                     ((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                      -0.1508e2 * std::exp(-100. * C74) * C74) *
                     rhoA * 3. * C62 * 4. * Pi /
                     std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2))))) *
                 C103) +
            (C101 * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * 3. * rhoA *
                 4. * Pi * 0.016 * std::pow(C70, 3) /
                 std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) -
             (C103 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                   -0.1508e2 * std::exp(-100. * C74) * C74) *
                  3. * rhoA / (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) +
              (C109 *
                   (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.) * 3. * rhoA -
                    ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * 3. * rhoA *
                        4. * Pi) /
                   std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) +
               3. * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) /
                   (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.))))) *
                C97) +
           ((std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) * C101 *
                 ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. * C62 *
                 4. * Pi * 0.048 * std::pow(C70, 2) -
             C101 * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. *
                 C62 * 4. * Pi * 0.016 * std::pow(C70, 3) * 8. * Pi * 0.016 * std::pow(C70, 3) *
                 4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) /
                std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 4) -
            ((((std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) * C109 * 4. * Pi *
                    0.016 * std::pow(C70, 3) * rhoA * 3. * C62 -
                C109 *
                    (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.) * rhoA * 3. * C62 -
                     ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. *
                         C62 * 4. * Pi) *
                    8. * Pi * 0.016 * std::pow(C70, 3) * 4. * Pi *
                    ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) /
                   std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 4) -
               ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * 3. * C62 * 4. * Pi *
                   0.016 * std::pow(C70, 3) /
                   std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2)) -
              C103 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                   -0.1508e2 * std::exp(-100. * C74) * C74) *
                  rhoA * 3. * C62 * 4. * Pi * 0.016 * std::pow(C70, 3) /
                  std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2)) -
             C97 * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * 3. * rhoA * 4. *
                 Pi * 0.016 * std::pow(C70, 3) /
                 std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2))) *
               C101) -
          C150 * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * 3. * rhoA /
              (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.))) -
         C156 *
             ((0.2743 - 0.1508 * std::exp(-100. * C74)) - -0.1508e2 * std::exp(-100. * C74) * C74) *
             rhoA * 3. * C62 / (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.))) -
        C162 *
            (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.) * rhoA * 3. * C62 -
             ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. * C62 * 4. *
                 Pi) /
            std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2);

    matrix.upper[1] = 0;

    const double C126 = 4.;
    const double C127 = 0.19645 * C126 / (2 * std::sqrt(C48));
    const double C128 = 7.7956 * C126 / (2 * std::sqrt(C48));
    const double C129 = C126 / (2 * std::sqrt(C48) * C68);
    const double C130 = C128 / C68;
    const double C131 = 2 * C70 * C129;
    const double C132 = 2 * C72 * C130;
    const double C133 = C132;
    const double C134 = C130 + C133 / (2 * std::sqrt(C78));
    const double C135 = C134 / C82;
    const double C136 = C56 * C135 + C84 * C127;
    const double C137 = C136 / C68;
    const double C163 = (-C100 * C126 * 2 * std::sqrt(C48)) / std::pow(2 * std::sqrt(C48) * C68, 2);
    const double C164 = (-C100 * C128) / std::pow(C68, 2);
    const double C165 = 2 * C129 * C101 + 2 * C70 * C163;
    const double C166 = 2 * C130 * C102 + 2 * C72 * C164;
    const double C167 = C166;
    const double C168 = (C164 + C167 / (2 * std::sqrt(C78))) -
                        C105 * C133 / (std::sqrt(C78) * std::pow(2 * std::sqrt(C78), 2));
    const double C169 = C168 / C82 - C106 * C134 / std::pow(C82, 2);
    const double C170 = C56 * C169 + C127 * C107;
    const double C171 = C170 / C68 - C100 * C136 / std::pow(C68, 2);
    matrix.upper[2] =
        (((((((C129 * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * 3. * C62 *
                   4. * Pi * 0.016 * std::pow(C70, 3) /
                   std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) -
               (C131 *
                    ((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                     -0.1508e2 * std::exp(-100. * C74) * C74) *
                    3. * C62 / (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) +
                C137 *
                    (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.) * 3. * C62 -
                     ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * 3. * C62 *
                         4. * Pi) /
                    std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2))) +
              C163 * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. *
                  C62 * 4. * Pi * 0.016 * std::pow(C70, 3) /
                  std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2)) +
             ((std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) * C129 * rhoA * 3. *
                   C62 * 4. * Pi * 0.016 * std::pow(C70, 3) -
               C129 * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. *
                   C62 * 4. * Pi * 0.016 * std::pow(C70, 3) * 8. * Pi * 4. * Pi *
                   ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) /
                  std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 4) -
              ((std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) * C137 *
                    (4. * Pi * rhoA * 3. * C62 - rhoA * 3. * C62 * 4. * Pi) -
                C137 *
                    (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.) * rhoA * 3. * C62 -
                     ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. *
                         C62 * 4. * Pi) *
                    8. * Pi * 4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) /
                   std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 4) -
               C131 *
                   ((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                    -0.1508e2 * std::exp(-100. * C74) * C74) *
                   rhoA * 3. * C62 * 4. * Pi /
                   std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2))) *
                 C109) +
            (C129 *
                 ((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                  -0.1508e2 * std::exp(-100. * C74) * C74) *
                 rhoA * 3. * C62 * 4. * Pi * 0.016 * std::pow(C70, 3) /
                 std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) -
             (C131 *
                  (0.1508e2 * (std::exp(-100. * C74) - 100. * std::exp(-100. * C74) * C74) -
                   -0.1508e2 * std::exp(-100. * C74)) *
                  rhoA * 3. * C62 / (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) -
              C137 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                   -0.1508e2 * std::exp(-100. * C74) * C74) *
                  rhoA * 3. * C62 * 4. * Pi /
                  std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2))) *
                C103) +
           (C129 * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * 3. * rhoA * 4. *
                Pi * 0.016 * std::pow(C70, 3) /
                std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) -
            (C131 *
                 ((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                  -0.1508e2 * std::exp(-100. * C74) * C74) *
                 3. * rhoA / (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) +
             C137 *
                 (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.) * 3. * rhoA -
                  ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * 3. * rhoA * 4. *
                      Pi) /
                 std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2))) *
               C97) +
          ((std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) * C129 *
                ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. * C62 *
                4. * Pi * 0.048 * std::pow(C70, 2) -
            C129 * ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. *
                C62 * 4. * Pi * 0.016 * std::pow(C70, 3) * 8. * Pi * 0.016 * std::pow(C70, 3) * 4. *
                Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) /
               std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 4) -
           ((std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2) * C137 * 4. * Pi *
                 0.016 * std::pow(C70, 3) * rhoA * 3. * C62 -
             C137 *
                 (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.) * rhoA * 3. * C62 -
                  ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. * C62 *
                      4. * Pi) *
                 8. * Pi * 0.016 * std::pow(C70, 3) * 4. * Pi *
                 ((C88 + 0.004 * std::pow(C70, 4)) + 1.)) /
                std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 4) -
            C131 *
                ((0.2743 - 0.1508 * std::exp(-100. * C74)) -
                 -0.1508e2 * std::exp(-100. * C74) * C74) *
                rhoA * 3. * C62 * 4. * Pi * 0.016 * std::pow(C70, 3) /
                std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2))) *
              C101) -
         C165 *
             ((0.2743 - 0.1508 * std::exp(-100. * C74)) - -0.1508e2 * std::exp(-100. * C74) * C74) *
             rhoA * 3. * C62 / (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.))) -
        C171 *
            (4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.) * rhoA * 3. * C62 -
             ((C88 + (0.2743 - 0.1508 * std::exp(-100. * C74)) * C74) + 1.) * rhoA * 3. * C62 * 4. *
                 Pi) /
            std::pow(4. * Pi * ((C88 + 0.004 * std::pow(C70, 4)) + 1.), 2);

    matrix.upper[3] = 0;

    matrix.upper[4] = 0;

    const double C47 = 2. * rhoB;
    const double C49 = 4. * sigmaBb;
    const double C53 = C51 * C47;
    const double C55 = std::sqrt(C49);
    const double C57 = 0.19645 * C55;
    const double C59 = 3. * C53;
    const double C61 = 7.7956 * C55;
    const double C63 = std::pow(C59, C50);
    const double C65 = C63 * C47;
    const double C67 = C65 + 1e-16;
    const double C69 = 2. * C67;
    const double C71 = C55 / C69;
    const double C73 = C61 / C69;
    const double C75 = std::pow(C71, 2);
    const double C77 = std::pow(C73, 2);
    const double C79 = C77 + 1.;
    const double C81 = std::sqrt(C79);
    const double C83 = C73 + C81;
    const double C85 = std::log(C83);
    const double C87 = C85 * C57;
    const double C89 = C87 / C69;
    const double C110 = 2.;
    const double C111 = C51 * C110;
    const double C112 = 3. * C111;
    const double C113 = C50 * std::pow(C59, C50 - 1) * C112;
    const double C114 = C47 * C113 + C63 * C110;
    const double C115 = C114;
    const double C116 = 2. * C115;
    const double C117 = (-C116 * std::sqrt(C49)) / std::pow(C69, 2);
    const double C118 = (-C116 * C61) / std::pow(C69, 2);
    const double C119 = 2 * C71 * C117;
    const double C120 = 2 * C73 * C118;
    const double C121 = C120;
    const double C122 = C118 + C121 / (2 * std::sqrt(C79));
    const double C123 = C122 / C83;
    const double C124 = C57 * C123;
    const double C125 = C124 / C69 - C116 * C87 / std::pow(C69, 2);
    const double C172 = C50 * (C50 - 1) * std::pow(C59, C50 - 2) * std::pow(C112, 2);
    const double C173 = (C113 * C110 + C47 * C172) + C110 * C113;
    const double C174 = C173;
    const double C175 = 2. * C174;
    const double C176 = C116 * C116 * std::sqrt(C49) * 2 * C69 / std::pow(C69, 4) -
                        C175 * std::sqrt(C49) / std::pow(C69, 2);
    const double C177 =
        C116 * C116 * C61 * 2 * C69 / std::pow(C69, 4) - C175 * C61 / std::pow(C69, 2);
    const double C178 = 2 * std::pow(C117, 2) + 2 * C71 * C176;
    const double C179 = 2 * std::pow(C118, 2) + 2 * C73 * C177;
    const double C180 = C179;
    const double C181 = (C177 + C180 / (2 * std::sqrt(C79))) -
                        std::pow(C121, 2) / (std::sqrt(C79) * std::pow(2 * std::sqrt(C79), 2));
    const double C182 = C181 / C83 - std::pow(C122, 2) / std::pow(C83, 2);
    const double C183 = C57 * C182;
    const double C184 =
        ((C183 / C69 +
          ((-C124) / std::pow(C69, 2) - (-C116 * C87 * 2 * C69) / std::pow(C69, 4)) * C116) -
         C175 * C87 / std::pow(C69, 2)) -
        C124 * C116 / std::pow(C69, 2);
    matrix.upper[5] =
        ((((((((((C125 *
                      (((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * 3. * C63 /
                           (4. * Pi) -
                       ((C89 + 0.004 * std::pow(C71, 4)) + 1.) * 3. * C63 / (4. * Pi)) /
                      std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2) -
                  C119 *
                      ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
                       -0.1508e2 * std::exp(-100. * C75) * C75) *
                      3. * C63 / (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) -
                 C113 * 3. * ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) /
                     (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) +
                C117 * 0.016 * std::pow(C71, 3) *
                    ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * 3. * C63 /
                    (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2))) +
               C184 *
                   (((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * rhoB * 3. *
                        C63 / (4. * Pi) -
                    ((C89 + 0.004 * std::pow(C71, 4)) + 1.) * rhoB * 3. * C63 / (4. * Pi)) /
                   std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2)) +
              (((((-C125 *
                   (((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * rhoB * 3. *
                        C63 / (4. * Pi) -
                    ((C89 + 0.004 * std::pow(C71, 4)) + 1.) * rhoB * 3. * C63 / (4. * Pi)) *
                   2. * ((C89 + 0.004 * std::pow(C71, 4)) + 1.)) /
                      std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 4) -
                  (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.) * 3. * C63 -
                   ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * 3. * C63 * 4. *
                       Pi) /
                      std::pow(4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.), 2)) -
                 (-C119 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
                   -0.1508e2 * std::exp(-100. * C75) * C75) *
                  rhoB * 3. * C63 * 4. * Pi) /
                     std::pow(4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.), 2)) -
                (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.) * C113 * 3. * rhoB -
                 C113 * ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * 3. * rhoB *
                     4. * Pi) /
                    std::pow(4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.), 2)) +
               (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2) * C117 * 0.016 *
                    std::pow(C71, 3) * rhoB * 3. * C63 -
                C117 * 0.016 * std::pow(C71, 3) *
                    ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * rhoB * 3. *
                    C63 * 4. * Pi * 2. * ((C89 + 0.004 * std::pow(C71, 4)) + 1.)) /
                   std::pow(4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2), 2)) *
                  C125) +
             ((((C125 *
                     ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
                      -0.1508e2 * std::exp(-100. * C75) * C75) *
                     rhoB * 3. * C63 /
                     (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2)) -
                 ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
                  -0.1508e2 * std::exp(-100. * C75) * C75) *
                     3. * C63 / (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) -
                C119 *
                    (0.1508e2 * (std::exp(-100. * C75) - 100. * std::exp(-100. * C75) * C75) -
                     -0.1508e2 * std::exp(-100. * C75)) *
                    rhoB * 3. * C63 / (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) -
               C113 *
                   ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
                    -0.1508e2 * std::exp(-100. * C75) * C75) *
                   3. * rhoB / (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) +
              C117 * 0.016 * std::pow(C71, 3) *
                  ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
                   -0.1508e2 * std::exp(-100. * C75) * C75) *
                  rhoB * 3. * C63 /
                  (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2))) *
                 C119) +
            (((C125 *
                   (((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * 3. * rhoB /
                        (4. * Pi) -
                    ((C89 + 0.004 * std::pow(C71, 4)) + 1.) * 3. * rhoB / (4. * Pi)) /
                   std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2) -
               3. * ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) /
                   (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) -
              C119 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
                   -0.1508e2 * std::exp(-100. * C75) * C75) *
                  3. * rhoB / (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) +
             C117 * 0.016 * std::pow(C71, 3) *
                 ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * 3. * rhoB /
                 (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2))) *
                C113) +
           (((((-(std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2) * C125 * 0.016 *
                      std::pow(C71, 3) * rhoB * 3. * C63 / (4. * Pi) +
                  C125 *
                      (((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * rhoB * 3. *
                           C63 / (4. * Pi) -
                       ((C89 + 0.004 * std::pow(C71, 4)) + 1.) * rhoB * 3. * C63 / (4. * Pi)) *
                      0.032 * std::pow(C71, 3) * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) /
                   std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 4) -
               (-((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * 3. * C63 * 4. *
                Pi * 0.016 * std::pow(C71, 3)) /
                   std::pow(4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.), 2)) -
              (-C119 *
               ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
                -0.1508e2 * std::exp(-100. * C75) * C75) *
               rhoB * 3. * C63 * 4. * Pi * 0.016 * std::pow(C71, 3)) /
                  std::pow(4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.), 2)) -
             (-C113 * ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * 3. * rhoB *
              4. * Pi * 0.016 * std::pow(C71, 3)) /
                 std::pow(4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.), 2)) +
            (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2) * C117 * 0.048 *
                 std::pow(C71, 2) * ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) *
                 rhoB * 3. * C63 -
             C117 * 0.016 * std::pow(C71, 3) *
                 ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * rhoB * 3. * C63 *
                 4. * Pi * 0.032 * std::pow(C71, 3) * ((C89 + 0.004 * std::pow(C71, 4)) + 1.)) /
                std::pow(4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2), 2)) *
               C117) -
          C178 *
              ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
               -0.1508e2 * std::exp(-100. * C75) * C75) *
              rhoB * 3. * C63 / (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) -
         C172 * ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * 3. * rhoB /
             (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) +
        C176 * 0.016 * std::pow(C71, 3) *
            ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * rhoB * 3. * C63 /
            (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2));

    matrix.upper[6] = 0;

    matrix.upper[7] = 0;

    const double C138 = 4.;
    const double C139 = 0.19645 * C138 / (2 * std::sqrt(C49));
    const double C140 = 7.7956 * C138 / (2 * std::sqrt(C49));
    const double C141 = C138 / (2 * std::sqrt(C49) * C69);
    const double C142 = C140 / C69;
    const double C143 = 2 * C71 * C141;
    const double C144 = 2 * C73 * C142;
    const double C145 = C144;
    const double C146 = C142 + C145 / (2 * std::sqrt(C79));
    const double C147 = C146 / C83;
    const double C148 = C57 * C147 + C85 * C139;
    const double C149 = C148 / C69;
    const double C185 = (-C116 * C138 * 2 * std::sqrt(C49)) / std::pow(2 * std::sqrt(C49) * C69, 2);
    const double C186 = (-C116 * C140) / std::pow(C69, 2);
    const double C187 = 2 * C141 * C117 + 2 * C71 * C185;
    const double C188 = 2 * C142 * C118 + 2 * C73 * C186;
    const double C189 = C188;
    const double C190 = (C186 + C189 / (2 * std::sqrt(C79))) -
                        C121 * C145 / (std::sqrt(C79) * std::pow(2 * std::sqrt(C79), 2));
    const double C191 = C190 / C83 - C122 * C146 / std::pow(C83, 2);
    const double C192 = C57 * C191 + C139 * C123;
    const double C193 = C192 / C69 - C116 * C148 / std::pow(C69, 2);
    matrix.upper[8] =
        ((((((((C149 *
                    (((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * 3. * C63 /
                         (4. * Pi) -
                     ((C89 + 0.004 * std::pow(C71, 4)) + 1.) * 3. * C63 / (4. * Pi)) /
                    std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2) -
                C143 *
                    ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
                     -0.1508e2 * std::exp(-100. * C75) * C75) *
                    3. * C63 / (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) +
               C141 * 0.016 * std::pow(C71, 3) *
                   ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * 3. * C63 /
                   (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2))) +
              C193 *
                  (((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * rhoB * 3. *
                       C63 / (4. * Pi) -
                   ((C89 + 0.004 * std::pow(C71, 4)) + 1.) * rhoB * 3. * C63 / (4. * Pi)) /
                  std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2)) +
             (((-C149 *
                (((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * rhoB * 3. * C63 /
                     (4. * Pi) -
                 ((C89 + 0.004 * std::pow(C71, 4)) + 1.) * rhoB * 3. * C63 / (4. * Pi)) *
                2. * ((C89 + 0.004 * std::pow(C71, 4)) + 1.)) /
                   std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 4) -
               (-C143 *
                ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
                 -0.1508e2 * std::exp(-100. * C75) * C75) *
                rhoB * 3. * C63 * 4. * Pi) /
                   std::pow(4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.), 2)) +
              (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2) * C141 * 0.016 *
                   std::pow(C71, 3) * rhoB * 3. * C63 -
               C141 * 0.016 * std::pow(C71, 3) *
                   ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * rhoB * 3. *
                   C63 * 4. * Pi * 2. * ((C89 + 0.004 * std::pow(C71, 4)) + 1.)) /
                  std::pow(4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2), 2)) *
                 C125) +
            ((C149 *
                  ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
                   -0.1508e2 * std::exp(-100. * C75) * C75) *
                  rhoB * 3. * C63 / (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2)) -
              C143 *
                  (0.1508e2 * (std::exp(-100. * C75) - 100. * std::exp(-100. * C75) * C75) -
                   -0.1508e2 * std::exp(-100. * C75)) *
                  rhoB * 3. * C63 / (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) +
             C141 * 0.016 * std::pow(C71, 3) *
                 ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
                  -0.1508e2 * std::exp(-100. * C75) * C75) *
                 rhoB * 3. * C63 / (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2))) *
                C119) +
           ((C149 *
                 (((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * 3. * rhoB /
                      (4. * Pi) -
                  ((C89 + 0.004 * std::pow(C71, 4)) + 1.) * 3. * rhoB / (4. * Pi)) /
                 std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2) -
             C143 *
                 ((0.2743 - 0.1508 * std::exp(-100. * C75)) -
                  -0.1508e2 * std::exp(-100. * C75) * C75) *
                 3. * rhoB / (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) +
            C141 * 0.016 * std::pow(C71, 3) *
                ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * 3. * rhoB /
                (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2))) *
               C113) +
          (((-(std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2) * C149 * 0.016 *
                   std::pow(C71, 3) * rhoB * 3. * C63 / (4. * Pi) +
               C149 *
                   (((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * rhoB * 3. *
                        C63 / (4. * Pi) -
                    ((C89 + 0.004 * std::pow(C71, 4)) + 1.) * rhoB * 3. * C63 / (4. * Pi)) *
                   0.032 * std::pow(C71, 3) * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) /
                std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 4) -
            (-C143 *
             ((0.2743 - 0.1508 * std::exp(-100. * C75)) - -0.1508e2 * std::exp(-100. * C75) * C75) *
             rhoB * 3. * C63 * 4. * Pi * 0.016 * std::pow(C71, 3)) /
                std::pow(4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.), 2)) +
           (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2) * C141 * 0.048 *
                std::pow(C71, 2) * ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) *
                rhoB * 3. * C63 -
            C141 * 0.016 * std::pow(C71, 3) *
                ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * rhoB * 3. * C63 *
                4. * Pi * 0.032 * std::pow(C71, 3) * ((C89 + 0.004 * std::pow(C71, 4)) + 1.)) /
               std::pow(4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2), 2)) *
              C117) -
         C187 *
             ((0.2743 - 0.1508 * std::exp(-100. * C75)) - -0.1508e2 * std::exp(-100. * C75) * C75) *
             rhoB * 3. * C63 / (4. * Pi * ((C89 + 0.004 * std::pow(C71, 4)) + 1.))) +
        C185 * 0.016 * std::pow(C71, 3) *
            ((C89 + (0.2743 - 0.1508 * std::exp(-100. * C75)) * C75) + 1.) * rhoB * 3. * C63 /
            (4. * Pi * std::pow((C89 + 0.004 * std::pow(C71, 4)) + 1., 2));

    const double C194 = 2 * rhoA;
    const double C195 = 4 * sigmaAa;
    const double C196 = 1. / 3.;
    const double C197 = std::pow(Pi, 2);
    const double C198 = C197 * C194;
    const double C199 = std::sqrt(C195);
    const double C200 = 3. * C198;
    const double C201 = 7.7956 * C199;
    const double C202 = std::pow(C200, C196);
    const double C203 = C202 * C194;
    const double C204 = C203 + 1e-16;
    const double C205 = 2. * C204;
    const double C206 = C199 / C205;
    const double C207 = C201 / C205;
    const double C208 = excgrid::internal::AsinhOverX(C207);
    const double C209 = excgrid::internal::AsinhOverXDerivX2(C207);
    const double C210 = std::pow(C206, 2);
    const double C211 = std::pow(C206, 4);
    const double C212 = std::pow(C207, 2);
    const double C213 = 0.004 * C211;
    const double C214 = 0.008 * C210;
    const double C215 = 100 * C210;
    const double C216 = C210 * C208;
    const double C217 = C212 + 1;
    const double C218 = 1.53144562 * C216;
    const double C219 = -C215;
    const double C220 = std::sqrt(C217);
    const double C221 = C218 + C213;
    const double C222 = 1 / C220;
    const double C223 = std::exp(C219);
    const double C224 = 0.1508 * C223;
    const double C225 = C208 + C222;
    const double C226 = C221 + 1;
    const double C227 = std::pow(C222, 3);
    const double C228 = 1.53144562 * C225;
    const double C229 = 0.2743 - C224;
    const double C230 = C227 / 2;
    const double C231 = C229 * C210;
    const double C232 = C209 - C230;
    const double C233 = C228 / 2;
    const double C234 = 0.930680627422304e2 * C232;
    const double C235 = C218 + C231;
    const double C236 = C233 + C214;
    const double C237 = C235 + 1;
    const double C238 = C234 / 2;
    matrix.upper[9] = (-(((C238 + 0.1508e2 * C223 * (2 - C215)) * C226 - C237 * (C238 + 0.008)) /
                             std::pow(C226, 2) -
                         2 * (((C233 + C229) + 0.1508e2 * C223 * C210) * C226 - C237 * C236) *
                             C236 / std::pow(C226, 3)) *
                       rhoA * 3. * C202 / (4. * Pi)) /
                      std::pow(2 * C202 * rhoA + 1e-16, 4);

    matrix.upper[10] = 0;

    matrix.upper[11] = 0;

    matrix.upper[12] = 0;

    matrix.upper[13] = 0;

    const double C239 = 2 * rhoB;
    const double C240 = 4 * sigmaBb;
    const double C241 = 1. / 3.;
    const double C242 = std::pow(Pi, 2);
    const double C243 = C242 * C239;
    const double C244 = std::sqrt(C240);
    const double C245 = 3. * C243;
    const double C246 = 7.7956 * C244;
    const double C247 = std::pow(C245, C241);
    const double C248 = C247 * C239;
    const double C249 = C248 + 1e-16;
    const double C250 = 2. * C249;
    const double C251 = C244 / C250;
    const double C252 = C246 / C250;
    const double C253 = excgrid::internal::AsinhOverX(C252);
    const double C254 = excgrid::internal::AsinhOverXDerivX2(C252);
    const double C255 = std::pow(C251, 2);
    const double C256 = std::pow(C251, 4);
    const double C257 = std::pow(C252, 2);
    const double C258 = 0.004 * C256;
    const double C259 = 0.008 * C255;
    const double C260 = 100 * C255;
    const double C261 = C255 * C253;
    const double C262 = C257 + 1;
    const double C263 = 1.53144562 * C261;
    const double C264 = -C260;
    const double C265 = std::sqrt(C262);
    const double C266 = C263 + C258;
    const double C267 = 1 / C265;
    const double C268 = std::exp(C264);
    const double C269 = 0.1508 * C268;
    const double C270 = C253 + C267;
    const double C271 = C266 + 1;
    const double C272 = std::pow(C267, 3);
    const double C273 = 1.53144562 * C270;
    const double C274 = 0.2743 - C269;
    const double C275 = C272 / 2;
    const double C276 = C274 * C255;
    const double C277 = C254 - C275;
    const double C278 = C273 / 2;
    const double C279 = 0.930680627422304e2 * C277;
    const double C280 = C263 + C276;
    const double C281 = C278 + C259;
    const double C282 = C280 + 1;
    const double C283 = C279 / 2;
    matrix.upper[14] = (-(((C283 + 0.1508e2 * C268 * (2 - C260)) * C271 - C282 * (C283 + 0.008)) /
                              std::pow(C271, 2) -
                          2 * (((C278 + C274) + 0.1508e2 * C268 * C255) * C271 - C282 * C281) *
                              C281 / std::pow(C271, 3)) *
                        rhoB * 3. * C247 / (4. * Pi)) /
                       std::pow(2 * C247 * rhoB + 1e-16, 4);
}

} // namespace excgrid
