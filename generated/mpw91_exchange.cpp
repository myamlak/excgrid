// excgrid codegen source: the mPW91 (modified PW91) GGA exchange
// functional (Adamson-Gill-Barthel).  Fresh authorship; the form is the
// published one (docs/mainpage.md references), and its damping exponent is
// 1.6455307846 - the FULL-PRECISION value, not the 1.6455 the published form
// prints.  Owner ruling 2026-09-14: the truncation was a transcription
// precision choice, not a distinct variant of mPW91, and the value was
// solved out of the oracle's own numbers (identically at every
// well-conditioned sample point; the fit is in the lane record and the
// comparison in tools/verify_pyscf.py).  Regenerate with tools/regenerate.py.

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

// The excgrid GGA kernel skeleton (fresh authorship; BSD-3-Clause).
// Emits one excgrid::XcKernelValue function per GGA functional: the energy
// density, the spin-density derivatives, and the three sigma derivatives.
// No spin-edge branches: the definitions' Tiny guards keep every expression
// finite at the spin and gamma edges (the LDA skeleton's exact-limit
// machinery is not needed for the gradient-corrected forms, whose edge
// contributions vanish with the spin density).

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

#include <cmath>
#include <limits>

namespace excgrid {

XcKernelValue MPw91Exchange(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
    (void)sigmaAb;

    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        return result;
    }

    const double C46 = 4. * Pi;
    const double C47 = 36. * Pi;
    const double C48 = rhoA + 1e-16;
    const double C49 = rhoB + 1e-16;
    const double C50 = -5. / 3.;
    const double C51 = 1. / 3.;
    const double C52 = 4. / 3.;
    const double C53 = std::sqrt(sigmaAa);
    const double C54 = std::sqrt(sigmaBb);
    const double C55 = std::pow(3., C52);
    const double C56 = std::pow(C46, C51);
    const double C57 = std::pow(C47, C50);
    const double C58 = std::pow(C48, C52);
    const double C59 = std::pow(C49, C52);
    const double C60 = std::pow(rhoA, C52);
    const double C61 = std::pow(rhoB, C52);
    const double C62 = 5. * C57;
    const double C63 = C53 / C58;
    const double C64 = C54 / C59;
    const double C65 = 0.00426 - C62;
    const double C66 = std::pow(C63, 2);
    const double C67 = std::pow(C63, 3.72);
    const double C68 = std::pow(C64, 2);
    const double C69 = std::pow(C64, 3.72);

    result.exc =
        -((0.9305257363491 * (C60 + C61) +
           C60 * ((0.00426 * C66 - C65 * C66 * std::exp(-1.6455307846 * C66)) - 0.000001 * C67) /
               ((std::log(C63 + std::sqrt(C66 + 1.)) * 0.02556 * C53 / C58 +
                 0.000002 * C67 * C56 / C55) +
                1.)) +
          C61 * ((0.00426 * C68 - C65 * C68 * std::exp(-1.6455307846 * C68)) - 0.000001 * C69) /
              ((std::log(C64 + std::sqrt(C68 + 1.)) * 0.02556 * C54 / C59 +
                0.000002 * C69 * C56 / C55) +
               1.));

    const double C74 = 0.000001 * C67;
    const double C75 = 0.00426 * C66;
    const double C76 = 0.02556 * C53;
    const double C77 = 1.6455307846 * C66;
    const double C78 = C65 * C66;
    const double C79 = C67 * C56;
    const double C80 = C66 + 1.;
    const double C81 = 8. / 3.;
    const double C82 = std::pow(C48, C51);
    const double C83 = std::pow(C63, 2.72);
    const double C84 = std::pow(rhoA, C51);
    const double C85 = 0.000002 * C79;
    const double C86 = 4. * C82;
    const double C87 = C82 * sigmaAa;
    const double C88 = -C77;
    const double C89 = std::sqrt(C80);
    const double C90 = std::pow(C48, C81);
    const double C91 = -8. * C87;
    const double C92 = 3. * C90;
    const double C93 = C53 * C86;
    const double C94 = C63 + C89;
    const double C95 = C85 / C55;
    const double C96 = std::exp(C88);
    const double C97 = -3.72 * C93;
    const double C98 = C58 * C92;
    const double C99 = C78 * C96;
    const double C100 = std::log(C94);
    const double C101 = C100 * C76;
    const double C102 = C83 * C97;
    const double C103 = C75 - C99;
    const double C104 = C103 - C74;
    const double C105 = C101 / C58;
    const double C106 = C105 + C95;
    const double C107 = C106 + 1.;

    result.vrhoA = -(
        3.7221029453964 * C84 / 3. +
        (C107 * (C60 * ((-0.03408 * C87 / C98 -
                         (C96 * C65 * C91 / C98 - C78 * C96 * -0.131642462768e2 * C87 / C98)) -
                        0.000001 * C102 / C92) +
                 C104 * 4. * C84 / 3.) -
         C60 * C104 *
             ((C58 * 0.02556 * C53 * (C91 / (C98 * 2 * C89) - C93 / C92) / C94 - C101 * C86 / 3.) /
                  C90 +
              0.000002 * C56 * C102 / (3. * C90 * C55))) /
            std::pow(C107, 2));

    const double C109 = 0.000001 * C69;
    const double C110 = 0.00426 * C68;
    const double C111 = 0.02556 * C54;
    const double C112 = 1.6455307846 * C68;
    const double C113 = C65 * C68;
    const double C114 = C69 * C56;
    const double C115 = C68 + 1.;
    const double C116 = 8. / 3.;
    const double C117 = std::pow(C49, C51);
    const double C118 = std::pow(C64, 2.72);
    const double C119 = std::pow(rhoB, C51);
    const double C120 = 0.000002 * C114;
    const double C121 = 4. * C117;
    const double C122 = C117 * sigmaBb;
    const double C123 = -C112;
    const double C124 = std::sqrt(C115);
    const double C125 = std::pow(C49, C116);
    const double C126 = -8. * C122;
    const double C127 = 3. * C125;
    const double C128 = C54 * C121;
    const double C129 = C64 + C124;
    const double C130 = C120 / C55;
    const double C131 = std::exp(C123);
    const double C132 = -3.72 * C128;
    const double C133 = C113 * C131;
    const double C134 = C59 * C127;
    const double C135 = std::log(C129);
    const double C136 = C118 * C132;
    const double C137 = C135 * C111;
    const double C138 = C110 - C133;
    const double C139 = C138 - C109;
    const double C140 = C137 / C59;
    const double C141 = C140 + C130;
    const double C142 = C141 + 1.;

    result.vrhoB = -(
        3.7221029453964 * C119 / 3. +
        (C142 * (C61 * ((-0.03408 * C122 / C134 - (C131 * C65 * C126 / C134 -
                                                   C113 * C131 * -0.131642462768e2 * C122 / C134)) -
                        0.000001 * C136 / C127) +
                 C139 * 4. * C119 / 3.) -
         C61 * C139 *
             ((C59 * 0.02556 * C54 * (C126 / (C134 * 2 * C124) - C128 / C127) / C129 -
               C137 * C121 / 3.) /
                  C125 +
              0.000002 * C56 * C136 / (3. * C125 * C55))) /
            std::pow(C142, 2));

    const double C144 = 4. * Pi;
    const double C145 = 36. * Pi;
    const double C146 = rhoA + 1e-16;
    const double C147 = -5. / 3.;
    const double C148 = 1. / 3.;
    const double C149 = 4. / 3.;
    const double C150 = 8. / 3.;
    const double C151 = std::sqrt(sigmaAa);
    const double C152 = 0.02556 * C151;
    const double C153 = std::pow(3., C149);
    const double C154 = std::pow(C144, C148);
    const double C155 = std::pow(C145, C147);
    const double C156 = std::pow(C146, C149);
    const double C157 = std::pow(C146, C150);
    const double C158 = std::pow(rhoA, C149);
    const double C159 = 5. * C155;
    const double C160 = C151 * C156;
    const double C161 = C151 * C157;
    const double C162 = C151 / C156;
    const double C163 = 2 * C160;
    const double C164 = 0.00426 - C159;
    const double C165 = std::pow(C162, 2);
    const double C166 = std::pow(C162, 2.72);
    const double C167 = std::pow(C162, 3.72);
    const double C168 = 1.6455307846 * C165;
    const double C169 = C164 * C165;
    const double C170 = C167 * C154;
    const double C171 = C165 + 1.;
    const double C172 = 0.000002 * C170;
    const double C173 = -C168;
    const double C174 = std::sqrt(C171);
    const double C175 = C162 + C174;
    const double C176 = C172 / C153;
    const double C177 = std::exp(C173);
    const double C178 = std::log(C175);
    const double C179 = C178 * C152;
    const double C180 = C179 / C156;
    const double C181 = C180 + C176;
    const double C182 = C181 + 1.;

    result.vsigmaAa = -((C182 * C158 *
                             ((0.00426 * C151 / C161 - (C177 * C164 * C151 / C161 -
                                                        C169 * C177 * 1.6455307846 * C151 / C161)) -
                              0.00000372 * C166 / C163) -
                         C158 * ((0.00426 * C165 - C169 * C177) - 0.000001 * C167) *
                             ((0.02556 * C178 / (2 * C151) +
                               0.02556 * C151 * (1 / C163 + C151 / (C161 * 2 * C174)) / C175) /
                                  C156 +
                              0.000002 * C154 * 3.72 * C166 / (2 * C160 * C153))) /
                            std::pow(C182, 2) +
                        0.);

    result.vsigmaAb = 0.;

    const double C185 = 4. * Pi;
    const double C186 = 36. * Pi;
    const double C187 = rhoB + 1e-16;
    const double C188 = -5. / 3.;
    const double C189 = 1. / 3.;
    const double C190 = 4. / 3.;
    const double C191 = 8. / 3.;
    const double C192 = std::sqrt(sigmaBb);
    const double C193 = 0.02556 * C192;
    const double C194 = std::pow(3., C190);
    const double C195 = std::pow(C185, C189);
    const double C196 = std::pow(C186, C188);
    const double C197 = std::pow(C187, C190);
    const double C198 = std::pow(C187, C191);
    const double C199 = std::pow(rhoB, C190);
    const double C200 = 5. * C196;
    const double C201 = C192 * C197;
    const double C202 = C192 * C198;
    const double C203 = C192 / C197;
    const double C204 = 2 * C201;
    const double C205 = 0.00426 - C200;
    const double C206 = std::pow(C203, 2);
    const double C207 = std::pow(C203, 2.72);
    const double C208 = std::pow(C203, 3.72);
    const double C209 = 1.6455307846 * C206;
    const double C210 = C205 * C206;
    const double C211 = C208 * C195;
    const double C212 = C206 + 1.;
    const double C213 = 0.000002 * C211;
    const double C214 = -C209;
    const double C215 = std::sqrt(C212);
    const double C216 = C203 + C215;
    const double C217 = C213 / C194;
    const double C218 = std::exp(C214);
    const double C219 = std::log(C216);
    const double C220 = C219 * C193;
    const double C221 = C220 / C197;
    const double C222 = C221 + C217;
    const double C223 = C222 + 1.;

    result.vsigmaBb = -((C223 * C199 *
                             ((0.00426 * C192 / C202 - (C218 * C205 * C192 / C202 -
                                                        C210 * C218 * 1.6455307846 * C192 / C202)) -
                              0.00000372 * C207 / C204) -
                         C199 * ((0.00426 * C206 - C210 * C218) - 0.000001 * C208) *
                             ((0.02556 * C219 / (2 * C192) +
                               0.02556 * C192 * (1 / C204 + C192 / (C202 * 2 * C215)) / C216) /
                                  C197 +
                              0.000002 * C195 * 3.72 * C207 / (2 * C201 * C194))) /
                            std::pow(C223, 2) +
                        0.);

    return result;
}

} // namespace excgrid
