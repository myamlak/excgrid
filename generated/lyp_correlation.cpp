// excgrid codegen source: the Lee-Yang-Parr 1988 GGA correlation
// functional.  Fresh authorship; formula from the LYP paper
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

XcKernelValue LypCorrelation(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        return result;
    }

    const double C44 = rhoB * rhoA;
    const double C45 = rhoA + rhoB;
    const double C46 = -11. / 3.;
    const double C47 = 1. / 3.;
    const double C48 = 8. / 3.;
    const double C49 = std::pow(C45, C46);
    const double C50 = std::pow(C45, C47);
    const double C51 = -0.2533 / C50;
    const double C52 = 0.2533 / C50;
    const double C53 = 0.349 / C50;
    const double C54 = C53 + 1.;
    const double C55 = std::exp(C51);
    const double C56 = C49 * C55;
    const double C57 = C50 * C54;
    const double C58 = 0.00649176 * C56;
    const double C59 = 0.349 / C57;
    const double C60 = C52 + C59;
    const double C61 = 3. * C60;
    const double C62 = C60 - 11.;
    const double C63 = 1. - C61;

    result.exc =
        -((((0.19672 * rhoA * rhoB / (C45 * C54) +
             (std::pow(rhoA, C48) + std::pow(rhoB, C48)) * rhoB * rhoA * 0.00649176 *
                 std::pow(3. * std::pow(Pi, 2), 2. / 3.) * 3. * std::pow(2., 11. / 3.) * C56 /
                 (10. * C54)) +
            sigmaAa * ((C63 - C62 * rhoA / C45) * C44 / 9. - std::pow(rhoB, 2)) * C58 / C54) +
           sigmaAb * ((47. - 7. * C60) * C44 / 9. - 4. * std::pow(C45, 2) / 3.) * C58 / C54) +
          sigmaBb * ((C63 - C62 * rhoB / C45) * C44 / 9. - std::pow(rhoA, 2)) * C58 / C54);

    const double C68 = 7. * C60;
    const double C69 = C45 * C54;
    const double C70 = C62 * rhoA;
    const double C71 = C62 * rhoB;
    const double C72 = -14. / 3.;
    const double C73 = -2. / 3.;
    const double C74 = 2. / 3.;
    const double C75 = 11. / 3.;
    const double C76 = std::pow(C45, 2);
    const double C77 = std::pow(C54, 2);
    const double C78 = std::pow(C57, 2);
    const double C79 = std::pow(Pi, 2);
    const double C80 = std::pow(rhoA, 2);
    const double C81 = std::pow(rhoA, C48);
    const double C82 = std::pow(rhoB, 2);
    const double C83 = std::pow(rhoB, C48);
    const double C84 = 3. * C79;
    const double C85 = 4. * C76;
    const double C86 = C81 + C83;
    const double C87 = 47. - C68;
    const double C88 = C70 / C45;
    const double C89 = C71 / C45;
    const double C90 = std::pow(2., C75);
    const double C91 = std::pow(C45, C72);
    const double C92 = std::pow(C45, C73);
    const double C93 = std::pow(C45, C74);
    const double C94 = -11. * C91;
    const double C95 = -0.2533 * C92;
    const double C96 = 0.2533 * C92;
    const double C97 = 0.349 * C92;
    const double C98 = 3. * C90;
    const double C99 = 3. * C93;
    const double C100 = C54 * C92;
    const double C101 = C87 * C44;
    const double C102 = C63 - C88;
    const double C103 = C63 - C89;
    const double C104 = C85 / 3.;
    const double C105 = std::pow(C84, C74);
    const double C106 = C102 * C44;
    const double C107 = C103 * C44;
    const double C108 = C105 * C98;
    const double C109 = C50 * C97;
    const double C110 = C55 * C94;
    const double C111 = C55 * C95;
    const double C112 = C100 / 3.;
    const double C113 = C101 / 9.;
    const double C114 = C96 / C99;
    const double C115 = C108 * C56;
    const double C116 = C49 * C111;
    const double C117 = C113 - C104;
    const double C118 = C106 / 9.;
    const double C119 = C107 / 9.;
    const double C120 = C109 / C99;
    const double C121 = C110 / 3.;
    const double C122 = 0.00649176 * C115;
    const double C123 = C112 - C120;
    const double C124 = C118 - C82;
    const double C125 = C119 - C80;
    const double C126 = C116 / C99;
    const double C127 = 0.349 * C123;
    const double C128 = rhoA * C122;
    const double C129 = C121 - C126;
    const double C130 = 0.00649176 * C129;
    const double C131 = rhoB * C128;
    const double C132 = -C127;
    const double C133 = C132 / C78;
    const double C134 = C133 - C114;
    const double C135 = 3. * C134;

    result.vrhoA = -(
        ((((C69 * 0.19672 * rhoB - 0.19672 * rhoA * rhoB * ((C53 - C45 * C97 / C99) + 1.)) /
               std::pow(C69, 2) +
           (10. * C54 *
                (C86 * rhoB * (rhoA * 0.00649176 * C108 * C129 + C122) +
                 C131 * 8. * std::pow(rhoA, 5. / 3.) / 3.) -
            C86 * C131 * -3.49 * C92 / C99) /
               std::pow(10. * C54, 2)) +
          (C54 * sigmaAa *
               (C124 * C130 +
                0.00649176 * C56 *
                    (C102 * rhoB - (C135 + (C45 * ((C60 + C134 * rhoA) - 11.) - C70) / C76) * C44) /
                    9.) -
           (-sigmaAa * C124 * C58 * C97 / 3.) / C93) /
              C77) +
         (C54 * sigmaAb *
              (C117 * C130 + ((C87 * rhoB - 7. * C134 * C44) / 9. - 8. * C45 / 3.) * C58) -
          (-sigmaAb * C117 * C58 * C97 / 3.) / C93) /
             C77) +
        (C54 * sigmaBb *
             (C125 * C130 +
              ((C103 * rhoB - (C135 + (C45 * C134 * rhoB - C71) / C76) * C44) / 9. - 2 * rhoA) *
                  C58) -
         (-sigmaBb * C125 * C58 * C97 / 3.) / C93) /
            C77);

    const double C137 = 7. * C60;
    const double C138 = C45 * C54;
    const double C139 = C62 * rhoA;
    const double C140 = C62 * rhoB;
    const double C141 = -14. / 3.;
    const double C142 = -2. / 3.;
    const double C143 = 2. / 3.;
    const double C144 = 11. / 3.;
    const double C145 = std::pow(C45, 2);
    const double C146 = std::pow(C54, 2);
    const double C147 = std::pow(C57, 2);
    const double C148 = std::pow(Pi, 2);
    const double C149 = std::pow(rhoA, 2);
    const double C150 = std::pow(rhoA, C48);
    const double C151 = std::pow(rhoB, 2);
    const double C152 = std::pow(rhoB, C48);
    const double C153 = 3. * C148;
    const double C154 = 4. * C145;
    const double C155 = C150 + C152;
    const double C156 = 47. - C137;
    const double C157 = C139 / C45;
    const double C158 = C140 / C45;
    const double C159 = std::pow(2., C144);
    const double C160 = std::pow(C45, C141);
    const double C161 = std::pow(C45, C142);
    const double C162 = std::pow(C45, C143);
    const double C163 = -11. * C160;
    const double C164 = -0.2533 * C161;
    const double C165 = 0.2533 * C161;
    const double C166 = 0.349 * C161;
    const double C167 = 3. * C159;
    const double C168 = 3. * C162;
    const double C169 = C156 * C44;
    const double C170 = C54 * C161;
    const double C171 = C63 - C157;
    const double C172 = C63 - C158;
    const double C173 = C154 / 3.;
    const double C174 = std::pow(C153, C143);
    const double C175 = C171 * C44;
    const double C176 = C172 * C44;
    const double C177 = C174 * C167;
    const double C178 = C50 * C166;
    const double C179 = C55 * C163;
    const double C180 = C55 * C164;
    const double C181 = C165 / C168;
    const double C182 = C169 / 9.;
    const double C183 = C170 / 3.;
    const double C184 = C177 * C56;
    const double C185 = C49 * C180;
    const double C186 = C182 - C173;
    const double C187 = C175 / 9.;
    const double C188 = C176 / 9.;
    const double C189 = C178 / C168;
    const double C190 = C179 / 3.;
    const double C191 = 0.00649176 * C184;
    const double C192 = C183 - C189;
    const double C193 = C187 - C151;
    const double C194 = C188 - C149;
    const double C195 = C185 / C168;
    const double C196 = 0.349 * C192;
    const double C197 = rhoA * C191;
    const double C198 = C190 - C195;
    const double C199 = 0.00649176 * C198;
    const double C200 = rhoB * C197;
    const double C201 = -C196;
    const double C202 = C201 / C147;
    const double C203 = C202 - C181;
    const double C204 = 3. * C203;

    result.vrhoB = -(
        ((((C138 * 0.19672 * rhoA - 0.19672 * rhoA * rhoB * ((C53 - C45 * C166 / C168) + 1.)) /
               std::pow(C138, 2) +
           (10. * C54 *
                (C155 * (rhoB * rhoA * 0.00649176 * C177 * C198 + C197) +
                 C200 * 8. * std::pow(rhoB, 5. / 3.) / 3.) -
            C155 * C200 * -3.49 * C161 / C168) /
               std::pow(10. * C54, 2)) +
          (C54 * sigmaAa *
               (C193 * C199 +
                ((C171 * rhoA - (C204 + (C45 * C203 * rhoA - C139) / C145) * C44) / 9. - 2 * rhoB) *
                    C58) -
           (-sigmaAa * C193 * C58 * C166 / 3.) / C162) /
              C146) +
         (C54 * sigmaAb *
              (C186 * C199 + ((C156 * rhoA - 7. * C203 * C44) / 9. - 8. * C45 / 3.) * C58) -
          (-sigmaAb * C186 * C58 * C166 / 3.) / C162) /
             C146) +
        (C54 * sigmaBb *
             (C194 * C199 +
              0.00649176 * C56 *
                  (C172 * rhoA - (C204 + (C45 * ((C60 + C203 * rhoB) - 11.) - C140) / C145) * C44) /
                  9.) -
         (-sigmaBb * C194 * C58 * C166 / 3.) / C162) /
            C146);

    const double C206 = rhoA + rhoB;
    const double C207 = 1. / 3.;
    const double C208 = std::pow(C206, C207);
    const double C209 = 0.2533 / C208;
    const double C210 = 0.349 / C208;
    const double C211 = C210 + 1.;
    const double C212 = C208 * C211;
    const double C213 = 0.349 / C212;
    const double C214 = C209 + C213;

    result.vsigmaAa =
        -(((1. - 3. * C214) - (C214 - 11.) * rhoA / C206) * rhoB * rhoA / 9. - std::pow(rhoB, 2)) *
        0.00649176 * std::pow(C206, -11. / 3.) * std::exp(-0.2533 / C208) / C211;

    const double C216 = rhoA + rhoB;
    const double C217 = 1. / 3.;
    const double C218 = std::pow(C216, C217);
    const double C219 = 0.349 / C218;
    const double C220 = C219 + 1.;

    result.vsigmaAb = -((47. - 7. * (0.2533 / C218 + 0.349 / (C218 * C220))) * rhoB * rhoA / 9. -
                        4. * std::pow(C216, 2) / 3.) *
                      0.00649176 * std::pow(C216, -11. / 3.) * std::exp(-0.2533 / C218) / C220;

    const double C222 = rhoA + rhoB;
    const double C223 = 1. / 3.;
    const double C224 = std::pow(C222, C223);
    const double C225 = 0.2533 / C224;
    const double C226 = 0.349 / C224;
    const double C227 = C226 + 1.;
    const double C228 = C224 * C227;
    const double C229 = 0.349 / C228;
    const double C230 = C225 + C229;

    result.vsigmaBb =
        -(((1. - 3. * C230) - (C230 - 11.) * rhoB / C222) * rhoB * rhoA / 9. - std::pow(rhoA, 2)) *
        0.00649176 * std::pow(C222, -11. / 3.) * std::exp(-0.2533 / C224) / C227;

    return result;
}

} // namespace excgrid
