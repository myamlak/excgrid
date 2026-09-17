// excgrid codegen source: the Perdew 1986 GGA correlation functional (the
// gradient correction on the Perdew-Zunger LDA piece).  Transcribed from the
// printed paper - J. P. Perdew, Phys. Rev. B 33, 8822 (1986) (docs/mainpage.md
// references) - not from another tree.
// Regenerate with tools/regenerate.py.

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

// The excgrid GGA PIECEWISE-kernel skeleton (fresh authorship; BSD-3-Clause).
//
// WHY THIS SKELETON EXISTS.  Every other kernel in this tree is one
// straight-line expression, and the GGA skeleton's own note says so: the
// formulas' Tiny guards keep every expression finite at the spin and gamma
// edges, so no branches are needed.  That is true of every functional here
// EXCEPT the Perdew-Zunger 1981 LDA piece (`lda_c_pz`, Perdew & Zunger,
// Phys. Rev. B 23, 5048 (1981)), which the P86 correlation is defined on:
// PZ81 is genuinely piecewise at rs = 1 -
//
//   rs >= 1 :  ec = gamma / (1 + beta1 sqrt(rs) + beta2 rs)
//   rs <  1 :  ec = a ln rs + b + c rs ln rs + d rs
//
// - two different analytic forms, not one expression with a removable
// singularity, so NO choice of guard makes it straight-line.  The two
// branches meet at rs = 1 to about 3e-5 (they are continuous but not
// smooth there), which is why the split is a real branch and not a
// cosmetic one.
//
// THE CONTRACT.  A piecewise kernel is emitted as two complete branch bodies
// inside `if (cond) { ... } else { ... }`, each branch carrying its own CSE
// declarations in its OWN braces, exactly as the LDA skeleton already scopes
// its spin-edge branches.  `tools/regenerate.py`'s temporary pruner computes
// reachability per C++ scope, so the two branches' tag namespaces may collide
// (both branches declare `C1`) and each branch prunes independently.
// `cond` is evaluated once, from the kernel arguments, and must select the
// branch that the definition's first expression binds - the generator's
// `ExPiecewiseGgaGenerate` takes them in that order and does not reorder.
// A kernel author adding a piecewise form must therefore state the boundary
// and its source in the `.ey` beside the condition, as `p86_correlation.ey`
// does; nothing here infers it.

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

#include <cmath>
#include <limits>

namespace excgrid {

XcKernelValue P86Correlation(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        return result;
    }

    if (!(std::pow(3. / (4. * Pi * (rhoA + rhoB)), 1. / 3.) < 1.))
    {
        const double C46 = 2. * sigmaAb;
        const double C47 = rhoA + rhoB;
        const double C48 = rhoA - rhoB;
        const double C49 = 1. / 3.;
        const double C50 = 2. / 3.;
        const double C51 = 4. / 3.;
        const double C52 = 5. / 3.;
        const double C53 = 0.9999999999999999 * C48;
        const double C54 = Pi * C47;
        const double C55 = sigmaAa + C46;
        const double C56 = 4. * C54;
        const double C57 = C55 + sigmaBb;
        const double C58 = C53 / C47;
        const double C59 = C58 + 1.;
        const double C60 = 1. - C58;
        const double C61 = 0.22167 / C56;
        const double C62 = 3. / C56;
        const double C63 = std::pow(C62, C49);
        const double C64 = std::pow(C62, C50);
        const double C65 = 7.389e-6 * C64;
        const double C66 = 0.023266 * C63;
        const double C67 = 0.3334 * C63;
        const double C68 = 0.472 * C64;
        const double C69 = 8.723 * C63;
        const double C70 = std::sqrt(C63);
        const double C71 = 1.0529 * C70;
        const double C72 = C66 + C65;
        const double C73 = C69 + C68;
        const double C74 = C71 + C67;
        const double C75 = C72 + 0.002568;
        const double C76 = C73 + C61;
        const double C77 = C74 + 1.;
        const double C78 = C76 + 1.;
        const double C79 = 0.1423 / C77;
        const double C80 = C75 / C78;
        const double C81 = C80 + 0.001667;

        result.exc =
            C47 * (((C79 - 0.0843 / ((1.3981 * C70 + 0.2611 * C63) + 1.)) *
                        ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                        (2. * (std::pow(2., C49) - 1.)) -
                    C79) +
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 * C57 /
                       (std::pow(C47, 7. / 3.) *
                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)));

        const double C86 = -24. * Pi;
        const double C87 = 0.2611 * C63;
        const double C88 = 0.88668 * Pi;
        const double C89 = 0.9999999999999999 * C47;
        const double C90 = 1.3981 * C70;
        const double C91 = 2 * C70;
        const double C92 = 12. * Pi;
        const double C93 = -2. / 3.;
        const double C94 = -1. / 3.;
        const double C95 = 7. / 3.;
        const double C96 = 7. / 6.;
        const double C97 = std::sqrt(C57);
        const double C98 = std::pow(2., C49);
        const double C99 = std::pow(C47, 2);
        const double C100 = std::pow(C56, 2);
        const double C101 = std::pow(C59, C51);
        const double C102 = std::pow(C59, C52);
        const double C103 = std::pow(C60, C51);
        const double C104 = std::pow(C60, C52);
        const double C105 = std::pow(C77, 2);
        const double C106 = std::pow(C78, 2);
        const double C107 = 0.00081290825 * C97;
        const double C108 = 3. * C100;
        const double C109 = C100 * C91;
        const double C110 = C101 + C103;
        const double C111 = C102 + C104;
        const double C112 = C90 + C87;
        const double C113 = C89 - C53;
        const double C114 = C98 - 1.;
        const double C115 = C88 / C100;
        const double C116 = std::pow(C47, C95);
        const double C117 = std::pow(C47, C96);
        const double C118 = std::pow(C62, C93);
        const double C119 = std::pow(C62, C94);
        const double C120 = 2. * C114;
        const double C121 = 3. * C109;
        const double C122 = C117 * C81;
        const double C123 = C118 * C92;
        const double C124 = C119 * C86;
        const double C125 = C112 + 1.;
        const double C126 = C110 - 2.;
        const double C127 = C111 / 2.;
        const double C128 = -8.723 * C123;
        const double C129 = -1.0529 * C123;
        const double C130 = -0.3334 * C123;
        const double C131 = -0.023266 * C123;
        const double C132 = 7.389e-6 * C124;
        const double C133 = 0.472 * C124;
        const double C134 = 0.0843 / C125;
        const double C135 = C107 / C122;
        const double C136 = std::sqrt(C127);
        const double C137 = C116 * C136;
        const double C138 = C128 + C133;
        const double C139 = C131 + C132;
        const double C140 = -C135;
        const double C141 = C79 - C134;
        const double C142 = C129 / C121;
        const double C143 = C130 / C108;
        const double C144 = C78 * C139;
        const double C145 = C142 + C143;
        const double C146 = C138 / C108;
        const double C147 = std::exp(C140);
        const double C148 = 0.1423 * C145;
        const double C149 = C147 * C81;
        const double C150 = C146 - C115;
        const double C151 = C144 / C108;
        const double C152 = C149 * C57;
        const double C153 = C75 * C150;
        const double C154 = -C148;
        const double C155 = C151 - C153;
        const double C156 = C154 / C105;

        result.vrhoA =
            C47 * (((C141 * (std::pow(C59, C49) * 4. * C113 + std::pow(C60, C49) * -4. * C113) /
                         (3. * C99) +
                     (C156 - (-0.0843 * (-1.3981 * C123 / C121 + -0.2611 * C123 / C108)) /
                                 std::pow(C125, 2)) *
                         C126) /
                        C120 -
                    C156) +
                   (C137 *
                        (C147 * C155 / C106 +
                         C81 * C147 * 0.00081290825 * C97 *
                             (C117 * C155 / C106 + C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                             std::pow(C122, 2)) *
                        C57 -
                    C152 *
                        (C116 * (std::pow(C59, C50) * 5. * C113 + std::pow(C60, C50) * -5. * C113) /
                             (6. * C99 * 2 * C136) +
                         C136 * 7. * std::pow(C47, C51) / 3.)) /
                       std::pow(C137, 2)) +
            ((C141 * C126 / C120 - C79) + C152 / C137);

        const double C158 = -24. * Pi;
        const double C159 = -0.9999999999999999 * C47;
        const double C160 = 0.2611 * C63;
        const double C161 = 0.88668 * Pi;
        const double C162 = 1.3981 * C70;
        const double C163 = 2 * C70;
        const double C164 = 12. * Pi;
        const double C165 = -2. / 3.;
        const double C166 = -1. / 3.;
        const double C167 = 7. / 3.;
        const double C168 = 7. / 6.;
        const double C169 = std::sqrt(C57);
        const double C170 = std::pow(2., C49);
        const double C171 = std::pow(C47, 2);
        const double C172 = std::pow(C56, 2);
        const double C173 = std::pow(C59, C51);
        const double C174 = std::pow(C59, C52);
        const double C175 = std::pow(C60, C51);
        const double C176 = std::pow(C60, C52);
        const double C177 = std::pow(C77, 2);
        const double C178 = std::pow(C78, 2);
        const double C179 = 0.00081290825 * C169;
        const double C180 = 3. * C172;
        const double C181 = C172 * C163;
        const double C182 = C162 + C160;
        const double C183 = C173 + C175;
        const double C184 = C174 + C176;
        const double C185 = C159 - C53;
        const double C186 = C170 - 1.;
        const double C187 = C161 / C172;
        const double C188 = std::pow(C47, C167);
        const double C189 = std::pow(C47, C168);
        const double C190 = std::pow(C62, C165);
        const double C191 = std::pow(C62, C166);
        const double C192 = 2. * C186;
        const double C193 = 3. * C181;
        const double C194 = C189 * C81;
        const double C195 = C190 * C164;
        const double C196 = C191 * C158;
        const double C197 = C182 + 1.;
        const double C198 = C183 - 2.;
        const double C199 = C184 / 2.;
        const double C200 = -8.723 * C195;
        const double C201 = -1.0529 * C195;
        const double C202 = -0.3334 * C195;
        const double C203 = -0.023266 * C195;
        const double C204 = 7.389e-6 * C196;
        const double C205 = 0.472 * C196;
        const double C206 = 0.0843 / C197;
        const double C207 = C179 / C194;
        const double C208 = std::sqrt(C199);
        const double C209 = C188 * C208;
        const double C210 = C200 + C205;
        const double C211 = C203 + C204;
        const double C212 = -C207;
        const double C213 = C79 - C206;
        const double C214 = C201 / C193;
        const double C215 = C202 / C180;
        const double C216 = C78 * C211;
        const double C217 = C214 + C215;
        const double C218 = C210 / C180;
        const double C219 = std::exp(C212);
        const double C220 = 0.1423 * C217;
        const double C221 = C219 * C81;
        const double C222 = C218 - C187;
        const double C223 = C216 / C180;
        const double C224 = C221 * C57;
        const double C225 = C75 * C222;
        const double C226 = -C220;
        const double C227 = C223 - C225;
        const double C228 = C226 / C177;

        result.vrhoB =
            C47 * (((C213 * (std::pow(C59, C49) * 4. * C185 + std::pow(C60, C49) * -4. * C185) /
                         (3. * C171) +
                     (C228 - (-0.0843 * (-1.3981 * C195 / C193 + -0.2611 * C195 / C180)) /
                                 std::pow(C197, 2)) *
                         C198) /
                        C192 -
                    C228) +
                   (C209 *
                        (C219 * C227 / C178 +
                         C81 * C219 * 0.00081290825 * C169 *
                             (C189 * C227 / C178 + C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                             std::pow(C194, 2)) *
                        C57 -
                    C224 *
                        (C188 * (std::pow(C59, C50) * 5. * C185 + std::pow(C60, C50) * -5. * C185) /
                             (6. * C171 * 2 * C208) +
                         C208 * 7. * std::pow(C47, C51) / 3.)) /
                       std::pow(C209, 2)) +
            ((C213 * C198 / C192 - C79) + C224 / C209);

        const double C230 = 2. * sigmaAb;
        const double C231 = rhoA + rhoB;
        const double C232 = rhoA - rhoB;
        const double C233 = 1. / 3.;
        const double C234 = 2. / 3.;
        const double C235 = 5. / 3.;
        const double C236 = 7. / 6.;
        const double C237 = 0.9999999999999999 * C232;
        const double C238 = Pi * C231;
        const double C239 = sigmaAa + C230;
        const double C240 = std::pow(C231, C236);
        const double C241 = 4. * C238;
        const double C242 = C239 + sigmaBb;
        const double C243 = C237 / C231;
        const double C244 = C243 + 1.;
        const double C245 = 1. - C243;
        const double C246 = 0.22167 / C241;
        const double C247 = 3. / C241;
        const double C248 = std::sqrt(C242);
        const double C249 = 0.00081290825 * C248;
        const double C250 = std::pow(C247, C233);
        const double C251 = std::pow(C247, C234);
        const double C252 = 7.389e-6 * C251;
        const double C253 = 0.023266 * C250;
        const double C254 = 0.472 * C251;
        const double C255 = 8.723 * C250;
        const double C256 = std::sqrt(C250);
        const double C257 = C253 + C252;
        const double C258 = C255 + C254;
        const double C259 = C257 + 0.002568;
        const double C260 = C258 + C246;
        const double C261 = C260 + 1.;
        const double C262 = C259 / C261;
        const double C263 = C262 + 0.001667;
        const double C264 = C240 * C263;
        const double C265 = C249 / C264;
        const double C266 = -C265;
        const double C267 = std::exp(C266);

        result.vsigmaAa =
            C231 * ((0.1423 / ((1.0529 * C256 + 0.3334 * C250) + 1.) -
                     0.0843 / ((1.3981 * C256 + 0.2611 * C250) + 1.)) *
                        (0. * std::pow(C244, C233) + 0. * std::pow(C245, C233)) /
                        (3. * C231 * 2. * (std::pow(2., C233) - 1.)) +
                    (C267 * C263 +
                     (0. * C267 / C261 + C263 * -0.00081290825 * C267 / (2 * C248 * C264)) * C242) /
                        (std::pow(C231, 7. / 3.) *
                         std::sqrt((std::pow(C244, C235) + std::pow(C245, C235)) / 2.)));

        const double C269 = 2. * sigmaAb;
        const double C270 = rhoA + rhoB;
        const double C271 = rhoA - rhoB;
        const double C272 = 1. / 3.;
        const double C273 = 2. / 3.;
        const double C274 = 5. / 3.;
        const double C275 = 7. / 6.;
        const double C276 = 0.9999999999999999 * C271;
        const double C277 = Pi * C270;
        const double C278 = sigmaAa + C269;
        const double C279 = std::pow(C270, C275);
        const double C280 = 4. * C277;
        const double C281 = C278 + sigmaBb;
        const double C282 = C276 / C270;
        const double C283 = C282 + 1.;
        const double C284 = 1. - C282;
        const double C285 = 0.22167 / C280;
        const double C286 = 3. / C280;
        const double C287 = std::sqrt(C281);
        const double C288 = 0.00081290825 * C287;
        const double C289 = std::pow(C286, C272);
        const double C290 = std::pow(C286, C273);
        const double C291 = 7.389e-6 * C290;
        const double C292 = 0.023266 * C289;
        const double C293 = 0.472 * C290;
        const double C294 = 8.723 * C289;
        const double C295 = std::sqrt(C289);
        const double C296 = C292 + C291;
        const double C297 = C294 + C293;
        const double C298 = C296 + 0.002568;
        const double C299 = C297 + C285;
        const double C300 = C299 + 1.;
        const double C301 = C298 / C300;
        const double C302 = C301 + 0.001667;
        const double C303 = C279 * C302;
        const double C304 = C288 / C303;
        const double C305 = -C304;
        const double C306 = std::exp(C305);

        result.vsigmaAb =
            C270 * ((0.1423 / ((1.0529 * C295 + 0.3334 * C289) + 1.) -
                     0.0843 / ((1.3981 * C295 + 0.2611 * C289) + 1.)) *
                        (0. * std::pow(C283, C272) + 0. * std::pow(C284, C272)) /
                        (3. * C270 * 2. * (std::pow(2., C272) - 1.)) +
                    (2. * C306 * C302 +
                     (0. * C306 / C300 + C302 * -0.0016258165 * C306 / (2 * C287 * C303)) * C281) /
                        (std::pow(C270, 7. / 3.) *
                         std::sqrt((std::pow(C283, C274) + std::pow(C284, C274)) / 2.)));

        const double C308 = 2. * sigmaAb;
        const double C309 = rhoA + rhoB;
        const double C310 = rhoA - rhoB;
        const double C311 = 1. / 3.;
        const double C312 = 2. / 3.;
        const double C313 = 5. / 3.;
        const double C314 = 7. / 6.;
        const double C315 = 0.9999999999999999 * C310;
        const double C316 = Pi * C309;
        const double C317 = sigmaAa + C308;
        const double C318 = std::pow(C309, C314);
        const double C319 = 4. * C316;
        const double C320 = C317 + sigmaBb;
        const double C321 = C315 / C309;
        const double C322 = C321 + 1.;
        const double C323 = 1. - C321;
        const double C324 = 0.22167 / C319;
        const double C325 = 3. / C319;
        const double C326 = std::sqrt(C320);
        const double C327 = 0.00081290825 * C326;
        const double C328 = std::pow(C325, C311);
        const double C329 = std::pow(C325, C312);
        const double C330 = 7.389e-6 * C329;
        const double C331 = 0.023266 * C328;
        const double C332 = 0.472 * C329;
        const double C333 = 8.723 * C328;
        const double C334 = std::sqrt(C328);
        const double C335 = C331 + C330;
        const double C336 = C333 + C332;
        const double C337 = C335 + 0.002568;
        const double C338 = C336 + C324;
        const double C339 = C338 + 1.;
        const double C340 = C337 / C339;
        const double C341 = C340 + 0.001667;
        const double C342 = C318 * C341;
        const double C343 = C327 / C342;
        const double C344 = -C343;
        const double C345 = std::exp(C344);

        result.vsigmaBb =
            C309 * ((0.1423 / ((1.0529 * C334 + 0.3334 * C328) + 1.) -
                     0.0843 / ((1.3981 * C334 + 0.2611 * C328) + 1.)) *
                        (0. * std::pow(C322, C311) + 0. * std::pow(C323, C311)) /
                        (3. * C309 * 2. * (std::pow(2., C311) - 1.)) +
                    (C345 * C341 +
                     (0. * C345 / C339 + C341 * -0.00081290825 * C345 / (2 * C326 * C342)) * C320) /
                        (std::pow(C309, 7. / 3.) *
                         std::sqrt((std::pow(C322, C313) + std::pow(C323, C313)) / 2.)));
    } else
    {
        const double C347 = 2. * sigmaAb;
        const double C348 = rhoA + rhoB;
        const double C349 = rhoA - rhoB;
        const double C350 = 1. / 3.;
        const double C351 = 2. / 3.;
        const double C352 = 4. / 3.;
        const double C353 = 5. / 3.;
        const double C354 = 0.9999999999999999 * C349;
        const double C355 = Pi * C348;
        const double C356 = sigmaAa + C347;
        const double C357 = 4. * C355;
        const double C358 = C356 + sigmaBb;
        const double C359 = C354 / C348;
        const double C360 = C359 + 1.;
        const double C361 = 1. - C359;
        const double C362 = 0.22167 / C357;
        const double C363 = 3. / C357;
        const double C364 = std::pow(C363, C350);
        const double C365 = std::pow(C363, C351);
        const double C366 = 7.389e-6 * C365;
        const double C367 = 0.0116 * C364;
        const double C368 = 0.023266 * C364;
        const double C369 = 0.472 * C365;
        const double C370 = 8.723 * C364;
        const double C371 = std::log(C364);
        const double C372 = 0.0311 * C371;
        const double C373 = C364 * C371;
        const double C374 = C368 + C366;
        const double C375 = C370 + C369;
        const double C376 = 0.0020 * C373;
        const double C377 = C374 + 0.002568;
        const double C378 = C375 + C362;
        const double C379 = C372 + C376;
        const double C380 = C378 + 1.;
        const double C381 = C379 - 0.048;
        const double C382 = C377 / C380;
        const double C383 = C382 + 0.001667;
        const double C384 = C381 - C367;

        result.exc =
            C348 * ((C384 + ((((0.01555 * C371 + 0.0007 * C373) - 0.0269) - 0.0048 * C364) - C384) *
                                ((std::pow(C360, C352) + std::pow(C361, C352)) - 2.) /
                                (2. * (std::pow(2., C350) - 1.))) +
                    std::exp(-0.00081290825 * std::sqrt(C358) / (std::pow(C348, 7. / 6.) * C383)) *
                        C383 * C358 /
                        (std::pow(C348, 7. / 3.) *
                         std::sqrt((std::pow(C360, C353) + std::pow(C361, C353)) / 2.)));

        const double C386 = -24. * Pi;
        const double C387 = 0.0007 * C373;
        const double C388 = 0.0048 * C364;
        const double C389 = 0.01555 * C371;
        const double C390 = 0.88668 * Pi;
        const double C391 = 0.9999999999999999 * C348;
        const double C392 = 12. * Pi;
        const double C393 = -2. / 3.;
        const double C394 = -1. / 3.;
        const double C395 = 7. / 3.;
        const double C396 = 7. / 6.;
        const double C397 = std::sqrt(C358);
        const double C398 = std::pow(2., C350);
        const double C399 = std::pow(C348, 2);
        const double C400 = std::pow(C357, 2);
        const double C401 = std::pow(C360, C352);
        const double C402 = std::pow(C360, C353);
        const double C403 = std::pow(C361, C352);
        const double C404 = std::pow(C361, C353);
        const double C405 = std::pow(C380, 2);
        const double C406 = 0.00081290825 * C397;
        const double C407 = 3. * C400;
        const double C408 = C400 * C364;
        const double C409 = C389 + C387;
        const double C410 = C401 + C403;
        const double C411 = C402 + C404;
        const double C412 = C391 - C354;
        const double C413 = C398 - 1.;
        const double C414 = C390 / C400;
        const double C415 = std::pow(C348, C395);
        const double C416 = std::pow(C348, C396);
        const double C417 = std::pow(C363, C393);
        const double C418 = std::pow(C363, C394);
        const double C419 = 2. * C413;
        const double C420 = 3. * C408;
        const double C421 = C416 * C383;
        const double C422 = C417 * C392;
        const double C423 = C418 * C386;
        const double C424 = C409 - 0.0269;
        const double C425 = C410 - 2.;
        const double C426 = C411 / 2.;
        const double C427 = -8.723 * C422;
        const double C428 = -0.0311 * C422;
        const double C429 = -0.023266 * C422;
        const double C430 = -0.0116 * C422;
        const double C431 = 7.389e-6 * C423;
        const double C432 = 0.472 * C423;
        const double C433 = C364 * C422;
        const double C434 = C371 * C422;
        const double C435 = C424 - C388;
        const double C436 = C406 / C421;
        const double C437 = std::sqrt(C426);
        const double C438 = C415 * C437;
        const double C439 = C427 + C432;
        const double C440 = C429 + C431;
        const double C441 = -C434;
        const double C442 = C435 - C384;
        const double C443 = -C436;
        const double C444 = C428 / C420;
        const double C445 = C430 / C407;
        const double C446 = C433 / C420;
        const double C447 = C380 * C440;
        const double C448 = C439 / C407;
        const double C449 = C441 / C407;
        const double C450 = std::exp(C443);
        const double C451 = C450 * C383;
        const double C452 = C448 - C414;
        const double C453 = C449 - C446;
        const double C454 = C447 / C407;
        const double C455 = 0.0020 * C453;
        const double C456 = C377 * C452;
        const double C457 = C451 * C358;
        const double C458 = C444 + C455;
        const double C459 = C454 - C456;
        const double C460 = C458 - C445;

        result.vrhoA =
            C348 *
                ((C460 +
                  (C442 * (std::pow(C360, C350) * 4. * C412 + std::pow(C361, C350) * -4. * C412) /
                       (3. * C399) +
                   (((-0.01555 * C422 / C420 + 0.0007 * C453) - -0.0048 * C422 / C407) - C460) *
                       C425) /
                      C419) +
                 (C438 *
                      (C450 * C459 / C405 +
                       C383 * C450 * 0.00081290825 * C397 *
                           (C416 * C459 / C405 + C383 * 7. * std::pow(C348, 1. / 6.) / 6.) /
                           std::pow(C421, 2)) *
                      C358 -
                  C457 *
                      (C415 *
                           (std::pow(C360, C351) * 5. * C412 + std::pow(C361, C351) * -5. * C412) /
                           (6. * C399 * 2 * C437) +
                       C437 * 7. * std::pow(C348, C352) / 3.)) /
                     std::pow(C438, 2)) +
            ((C384 + C442 * C425 / C419) + C457 / C438);

        const double C462 = -24. * Pi;
        const double C463 = -0.9999999999999999 * C348;
        const double C464 = 0.0007 * C373;
        const double C465 = 0.0048 * C364;
        const double C466 = 0.01555 * C371;
        const double C467 = 0.88668 * Pi;
        const double C468 = 12. * Pi;
        const double C469 = -2. / 3.;
        const double C470 = -1. / 3.;
        const double C471 = 7. / 3.;
        const double C472 = 7. / 6.;
        const double C473 = std::sqrt(C358);
        const double C474 = std::pow(2., C350);
        const double C475 = std::pow(C348, 2);
        const double C476 = std::pow(C357, 2);
        const double C477 = std::pow(C360, C352);
        const double C478 = std::pow(C360, C353);
        const double C479 = std::pow(C361, C352);
        const double C480 = std::pow(C361, C353);
        const double C481 = std::pow(C380, 2);
        const double C482 = 0.00081290825 * C473;
        const double C483 = 3. * C476;
        const double C484 = C476 * C364;
        const double C485 = C466 + C464;
        const double C486 = C477 + C479;
        const double C487 = C478 + C480;
        const double C488 = C463 - C354;
        const double C489 = C474 - 1.;
        const double C490 = C467 / C476;
        const double C491 = std::pow(C348, C471);
        const double C492 = std::pow(C348, C472);
        const double C493 = std::pow(C363, C469);
        const double C494 = std::pow(C363, C470);
        const double C495 = 2. * C489;
        const double C496 = 3. * C484;
        const double C497 = C492 * C383;
        const double C498 = C493 * C468;
        const double C499 = C494 * C462;
        const double C500 = C485 - 0.0269;
        const double C501 = C486 - 2.;
        const double C502 = C487 / 2.;
        const double C503 = -8.723 * C498;
        const double C504 = -0.0311 * C498;
        const double C505 = -0.023266 * C498;
        const double C506 = -0.0116 * C498;
        const double C507 = 7.389e-6 * C499;
        const double C508 = 0.472 * C499;
        const double C509 = C364 * C498;
        const double C510 = C371 * C498;
        const double C511 = C500 - C465;
        const double C512 = C482 / C497;
        const double C513 = std::sqrt(C502);
        const double C514 = C491 * C513;
        const double C515 = C503 + C508;
        const double C516 = C505 + C507;
        const double C517 = -C510;
        const double C518 = C511 - C384;
        const double C519 = -C512;
        const double C520 = C504 / C496;
        const double C521 = C506 / C483;
        const double C522 = C509 / C496;
        const double C523 = C380 * C516;
        const double C524 = C515 / C483;
        const double C525 = C517 / C483;
        const double C526 = std::exp(C519);
        const double C527 = C526 * C383;
        const double C528 = C524 - C490;
        const double C529 = C525 - C522;
        const double C530 = C523 / C483;
        const double C531 = 0.0020 * C529;
        const double C532 = C377 * C528;
        const double C533 = C527 * C358;
        const double C534 = C520 + C531;
        const double C535 = C530 - C532;
        const double C536 = C534 - C521;

        result.vrhoB =
            C348 *
                ((C536 +
                  (C518 * (std::pow(C360, C350) * 4. * C488 + std::pow(C361, C350) * -4. * C488) /
                       (3. * C475) +
                   (((-0.01555 * C498 / C496 + 0.0007 * C529) - -0.0048 * C498 / C483) - C536) *
                       C501) /
                      C495) +
                 (C514 *
                      (C526 * C535 / C481 +
                       C383 * C526 * 0.00081290825 * C473 *
                           (C492 * C535 / C481 + C383 * 7. * std::pow(C348, 1. / 6.) / 6.) /
                           std::pow(C497, 2)) *
                      C358 -
                  C533 *
                      (C491 *
                           (std::pow(C360, C351) * 5. * C488 + std::pow(C361, C351) * -5. * C488) /
                           (6. * C475 * 2 * C513) +
                       C513 * 7. * std::pow(C348, C352) / 3.)) /
                     std::pow(C514, 2)) +
            ((C384 + C518 * C501 / C495) + C533 / C514);

        const double C538 = 2. * sigmaAb;
        const double C539 = rhoA + rhoB;
        const double C540 = rhoA - rhoB;
        const double C541 = 1. / 3.;
        const double C542 = 2. / 3.;
        const double C543 = 5. / 3.;
        const double C544 = 7. / 6.;
        const double C545 = 0.9999999999999999 * C540;
        const double C546 = Pi * C539;
        const double C547 = sigmaAa + C538;
        const double C548 = std::pow(C539, C544);
        const double C549 = 4. * C546;
        const double C550 = C547 + sigmaBb;
        const double C551 = C545 / C539;
        const double C552 = C551 + 1.;
        const double C553 = 1. - C551;
        const double C554 = 0.22167 / C549;
        const double C555 = 3. / C549;
        const double C556 = std::sqrt(C550);
        const double C557 = 0.00081290825 * C556;
        const double C558 = std::pow(C555, C541);
        const double C559 = std::pow(C555, C542);
        const double C560 = 7.389e-6 * C559;
        const double C561 = 0.023266 * C558;
        const double C562 = 0.472 * C559;
        const double C563 = 8.723 * C558;
        const double C564 = std::log(C558);
        const double C565 = C558 * C564;
        const double C566 = C561 + C560;
        const double C567 = C563 + C562;
        const double C568 = C566 + 0.002568;
        const double C569 = C567 + C554;
        const double C570 = C569 + 1.;
        const double C571 = C568 / C570;
        const double C572 = C571 + 0.001667;
        const double C573 = C548 * C572;
        const double C574 = C557 / C573;
        const double C575 = -C574;
        const double C576 = std::exp(C575);

        result.vsigmaAa =
            C539 * (((((0.01555 * C564 + 0.0007 * C565) - 0.0269) - 0.0048 * C558) -
                     (((0.0311 * C564 + 0.0020 * C565) - 0.048) - 0.0116 * C558)) *
                        (0. * std::pow(C552, C541) + 0. * std::pow(C553, C541)) /
                        (3. * C539 * 2. * (std::pow(2., C541) - 1.)) +
                    (C576 * C572 +
                     (0. * C576 / C570 + C572 * -0.00081290825 * C576 / (2 * C556 * C573)) * C550) /
                        (std::pow(C539, 7. / 3.) *
                         std::sqrt((std::pow(C552, C543) + std::pow(C553, C543)) / 2.)));

        const double C578 = 2. * sigmaAb;
        const double C579 = rhoA + rhoB;
        const double C580 = rhoA - rhoB;
        const double C581 = 1. / 3.;
        const double C582 = 2. / 3.;
        const double C583 = 5. / 3.;
        const double C584 = 7. / 6.;
        const double C585 = 0.9999999999999999 * C580;
        const double C586 = Pi * C579;
        const double C587 = sigmaAa + C578;
        const double C588 = std::pow(C579, C584);
        const double C589 = 4. * C586;
        const double C590 = C587 + sigmaBb;
        const double C591 = C585 / C579;
        const double C592 = C591 + 1.;
        const double C593 = 1. - C591;
        const double C594 = 0.22167 / C589;
        const double C595 = 3. / C589;
        const double C596 = std::sqrt(C590);
        const double C597 = 0.00081290825 * C596;
        const double C598 = std::pow(C595, C581);
        const double C599 = std::pow(C595, C582);
        const double C600 = 7.389e-6 * C599;
        const double C601 = 0.023266 * C598;
        const double C602 = 0.472 * C599;
        const double C603 = 8.723 * C598;
        const double C604 = std::log(C598);
        const double C605 = C598 * C604;
        const double C606 = C601 + C600;
        const double C607 = C603 + C602;
        const double C608 = C606 + 0.002568;
        const double C609 = C607 + C594;
        const double C610 = C609 + 1.;
        const double C611 = C608 / C610;
        const double C612 = C611 + 0.001667;
        const double C613 = C588 * C612;
        const double C614 = C597 / C613;
        const double C615 = -C614;
        const double C616 = std::exp(C615);

        result.vsigmaAb =
            C579 * (((((0.01555 * C604 + 0.0007 * C605) - 0.0269) - 0.0048 * C598) -
                     (((0.0311 * C604 + 0.0020 * C605) - 0.048) - 0.0116 * C598)) *
                        (0. * std::pow(C592, C581) + 0. * std::pow(C593, C581)) /
                        (3. * C579 * 2. * (std::pow(2., C581) - 1.)) +
                    (2. * C616 * C612 +
                     (0. * C616 / C610 + C612 * -0.0016258165 * C616 / (2 * C596 * C613)) * C590) /
                        (std::pow(C579, 7. / 3.) *
                         std::sqrt((std::pow(C592, C583) + std::pow(C593, C583)) / 2.)));

        const double C618 = 2. * sigmaAb;
        const double C619 = rhoA + rhoB;
        const double C620 = rhoA - rhoB;
        const double C621 = 1. / 3.;
        const double C622 = 2. / 3.;
        const double C623 = 5. / 3.;
        const double C624 = 7. / 6.;
        const double C625 = 0.9999999999999999 * C620;
        const double C626 = Pi * C619;
        const double C627 = sigmaAa + C618;
        const double C628 = std::pow(C619, C624);
        const double C629 = 4. * C626;
        const double C630 = C627 + sigmaBb;
        const double C631 = C625 / C619;
        const double C632 = C631 + 1.;
        const double C633 = 1. - C631;
        const double C634 = 0.22167 / C629;
        const double C635 = 3. / C629;
        const double C636 = std::sqrt(C630);
        const double C637 = 0.00081290825 * C636;
        const double C638 = std::pow(C635, C621);
        const double C639 = std::pow(C635, C622);
        const double C640 = 7.389e-6 * C639;
        const double C641 = 0.023266 * C638;
        const double C642 = 0.472 * C639;
        const double C643 = 8.723 * C638;
        const double C644 = std::log(C638);
        const double C645 = C638 * C644;
        const double C646 = C641 + C640;
        const double C647 = C643 + C642;
        const double C648 = C646 + 0.002568;
        const double C649 = C647 + C634;
        const double C650 = C649 + 1.;
        const double C651 = C648 / C650;
        const double C652 = C651 + 0.001667;
        const double C653 = C628 * C652;
        const double C654 = C637 / C653;
        const double C655 = -C654;
        const double C656 = std::exp(C655);

        result.vsigmaBb =
            C619 * (((((0.01555 * C644 + 0.0007 * C645) - 0.0269) - 0.0048 * C638) -
                     (((0.0311 * C644 + 0.0020 * C645) - 0.048) - 0.0116 * C638)) *
                        (0. * std::pow(C632, C621) + 0. * std::pow(C633, C621)) /
                        (3. * C619 * 2. * (std::pow(2., C621) - 1.)) +
                    (C656 * C652 +
                     (0. * C656 / C650 + C652 * -0.00081290825 * C656 / (2 * C636 * C653)) * C630) /
                        (std::pow(C619, 7. / 3.) *
                         std::sqrt((std::pow(C632, C623) + std::pow(C633, C623)) / 2.)));
    }

    return result;
}

} // namespace excgrid
