// excgrid codegen source: the PBEsol GGA exchange functional (the PBE
// exchange form with mu = 10/81).  The formula is sourced from the PBEsol
// paper - J. P. Perdew et al., Phys. Rev. Lett. 100 (2008) 136406, listed
// with the other functional citations in docs/mainpage.md.  Regenerate
// with tools/regenerate.py.

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

XcKernelValue PbeSolExchange(
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

    const double C46 = 2. * rhoA;
    const double C47 = 2. * rhoB;
    const double C48 = 1. / 3.;
    const double C49 = std::pow(Pi, 2);
    const double C50 = C49 * C46;
    const double C51 = C49 * C47;
    const double C52 = 3. * C50;
    const double C53 = 3. * C51;
    const double C54 = std::pow(C52, C48);
    const double C55 = std::pow(C53, C48);

    result.exc =
        -((1.804 -
           0.804 / (10. * std::pow(std::sqrt(4. * sigmaBb) / (2. * (C55 * C47 + 1e-16)), 2) /
                        0.65124e2 +
                    1.)) *
              rhoB * 3. * C55 +
          (1.804 -
           0.804 / (10. * std::pow(std::sqrt(4. * sigmaAa) / (2. * (C54 * C46 + 1e-16)), 2) /
                        0.65124e2 +
                    1.)) *
              rhoA * 3. * C54) /
        (4. * Pi);

    const double C60 = 3. * C54;
    const double C61 = 4. * sigmaAa;
    const double C62 = 6. * C49;
    const double C63 = C54 * C46;
    const double C64 = C63 + 1e-16;
    const double C65 = C54 / C62;
    const double C66 = std::sqrt(C61);
    const double C67 = 2. * C64;
    const double C68 = C66 / C67;
    const double C69 = std::pow(C68, 2);
    const double C70 = 10. * C69;
    const double C71 = C70 / 0.65124e2;
    const double C72 = C71 + 1.;

    result.vrhoA = -((1.804 - 0.804 / C72) * (18. * C49 * C65 / 3. + C60) +
                     rhoA * C60 * -0.3216e2 * (2. * C54 + 2. * C62 * C65 / 3.) * C61 /
                         (0.130248e3 * C64 * std::pow(C67, 2) * std::pow(C72, 2))) /
                   (4. * Pi);

    const double C74 = 3. * C55;
    const double C75 = 4. * sigmaBb;
    const double C76 = 6. * C49;
    const double C77 = C55 * C47;
    const double C78 = C77 + 1e-16;
    const double C79 = C55 / C76;
    const double C80 = std::sqrt(C75);
    const double C81 = 2. * C78;
    const double C82 = C80 / C81;
    const double C83 = std::pow(C82, 2);
    const double C84 = 10. * C83;
    const double C85 = C84 / 0.65124e2;
    const double C86 = C85 + 1.;

    result.vrhoB = -((1.804 - 0.804 / C86) * (18. * C49 * C79 / 3. + C74) +
                     rhoB * C74 * -0.3216e2 * (2. * C55 + 2. * C76 * C79 / 3.) * C75 /
                         (0.130248e3 * C78 * std::pow(C81, 2) * std::pow(C86, 2))) /
                   (4. * Pi);

    const double C88 = 2. * rhoA;
    const double C89 = 1. / 3.;
    const double C90 = std::pow(Pi, 2);
    const double C91 = C90 * C88;
    const double C92 = 3. * C91;
    const double C93 = std::pow(C92, C89);
    const double C94 = C93 * C88;
    const double C95 = C94 + 1e-16;

    result.vsigmaAa =
        -0.19296e3 * C93 * rhoA /
        (0.2083968e4 * Pi *
         std::pow(10. * std::pow(std::sqrt(4. * sigmaAa + 4. * SigmaGuard) / (2. * C95), 2) /
                          0.65124e2 +
                      1.,
                  2) *
         std::pow(C95, 2));

    result.vsigmaAb = 0;

    const double C98 = 2. * rhoB;
    const double C99 = 1. / 3.;
    const double C100 = std::pow(Pi, 2);
    const double C101 = C100 * C98;
    const double C102 = 3. * C101;
    const double C103 = std::pow(C102, C99);
    const double C104 = C103 * C98;
    const double C105 = C104 + 1e-16;

    result.vsigmaBb =
        -0.19296e3 * C103 * rhoB /
        (0.2083968e4 * Pi *
         std::pow(10. * std::pow(std::sqrt(4. * sigmaBb + 4. * SigmaGuard) / (2. * C105), 2) /
                          0.65124e2 +
                      1.,
                  2) *
         std::pow(C105, 2));

    return result;
}

} // namespace excgrid
