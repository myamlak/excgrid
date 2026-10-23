// excgrid codegen source: the VWN5 LDA correlation functional (the
// interpolation-V parametrization, most codes' bare "VWN").
// Formula from VWN 1980 Table V.

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

XcKernelValue Vwn5Correlation(double rhoA, double rhoB) {
    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        return result;
    }

    if (rhoA >= eps && rhoB >= eps)
    {
        const double C394 = rhoA + rhoB;
        const double C395 = rhoA - rhoB;
        const double C396 = 1. / 3.;
        const double C397 = 4. / 3.;
        const double C398 = std::sqrt(0.223816694236e2);
        const double C399 = std::sqrt(0.378469910464e2);
        const double C400 = std::sqrt(0.507386806551e2);
        const double C401 = std::pow(Pi, 2);
        const double C402 = 0.9999999999999999 * C395;
        const double C403 = 6. * C401;
        const double C404 = Pi * C394;
        const double C405 = 4. * C404;
        const double C406 = C402 / C394;
        const double C407 = 3. / C405;
        const double C408 = std::pow(C407, C396);
        const double C409 = std::sqrt(C408);
        const double C410 = 1.13107 * C409;
        const double C411 = 2. * C409;
        const double C412 = 3.72744 * C409;
        const double C413 = 7.06042 * C409;
        const double C414 = C409 + 0.0047584;
        const double C415 = C409 + 0.10498;
        const double C416 = C408 + C410;
        const double C417 = C408 + C412;
        const double C418 = C408 + C413;
        const double C419 = C411 + 1.13107;
        const double C420 = C411 + 3.72744;
        const double C421 = C411 + 7.06042;
        const double C422 = std::pow(C414, 2);
        const double C423 = std::pow(C415, 2);
        const double C424 = C416 + 13.0045;
        const double C425 = C417 + 12.9352;
        const double C426 = C418 + 18.0578;
        const double C427 = C398 / C421;
        const double C428 = C399 / C420;
        const double C429 = C400 / C419;
        const double C430 = C408 / C424;
        const double C431 = C408 / C425;
        const double C432 = C422 / C424;
        const double C433 = C423 / C425;
        const double C434 = std::atan(C427);
        const double C435 = std::atan(C428);
        const double C436 = std::atan(C429);
        const double C437 = 2.2431064 * C436;
        const double C438 = 2.26214 * C436;
        const double C439 = 7.03496 * C435;
        const double C440 = 7.45488 * C435;
        const double C441 = std::log(C430);
        const double C442 = std::log(C431);
        const double C443 = std::log(C432);
        const double C444 = std::log(C433);
        const double C445 = C437 / C400;
        const double C446 = C438 / C400;
        const double C447 = C439 / C399;
        const double C448 = C440 / C399;
        const double C449 = C441 + C446;
        const double C450 = C442 + C448;
        const double C451 = C443 + C445;
        const double C452 = C444 + C447;
        const double C453 = -0.3913066512 * C452;
        const double C454 = -0.005382083488 * C451;
        const double C455 = C453 / 12.5549141492;
        const double C456 = C454 / 12.99914055888256;
        const double C457 = C450 - C455;
        const double C458 = C456 - C449;
        const double C459 = 0.0310907 * C457;

        result.exc =
            C394 *
            (C459 + C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                        ((1.7099209341613653 *
                              (0.01554535 * ((std::log(C408 / C426) + 0.1412084e2 * C434 / C398) -
                                             -2.2946365 *
                                                 (std::log(std::pow(C409 + 0.325, 2) / C426) +
                                                  0.1282084e2 * C434 / C398) /
                                                 15.8687885) -
                               C459) *
                              C403 / C458 -
                          1.) *
                             std::pow(C395 / C394, 4) +
                         1.) /
                        (3.4198418683227306 * (std::pow(2., C396) - 1.) * C403));
    } else if (rhoA < eps)
    {
        const double C461 = -0.9999999999999999 * rhoB;
        const double C462 = Pi * rhoB;
        const double C463 = 1. / 3.;
        const double C464 = 4. / 3.;
        const double C465 = std::sqrt(0.223816694236e2);
        const double C466 = std::sqrt(0.378469910464e2);
        const double C467 = std::sqrt(0.507386806551e2);
        const double C468 = std::pow(Pi, 2);
        const double C469 = 4. * C462;
        const double C470 = 6. * C468;
        const double C471 = C461 / rhoB;
        const double C472 = 3. / C469;
        const double C473 = std::pow(C472, C463);
        const double C474 = std::sqrt(C473);
        const double C475 = 1.13107 * C474;
        const double C476 = 2. * C474;
        const double C477 = 3.72744 * C474;
        const double C478 = 7.06042 * C474;
        const double C479 = C474 + 0.0047584;
        const double C480 = C474 + 0.10498;
        const double C481 = C473 + C475;
        const double C482 = C473 + C477;
        const double C483 = C473 + C478;
        const double C484 = C476 + 1.13107;
        const double C485 = C476 + 3.72744;
        const double C486 = C476 + 7.06042;
        const double C487 = std::pow(C479, 2);
        const double C488 = std::pow(C480, 2);
        const double C489 = C481 + 13.0045;
        const double C490 = C482 + 12.9352;
        const double C491 = C483 + 18.0578;
        const double C492 = C465 / C486;
        const double C493 = C466 / C485;
        const double C494 = C467 / C484;
        const double C495 = C473 / C489;
        const double C496 = C473 / C490;
        const double C497 = C487 / C489;
        const double C498 = C488 / C490;
        const double C499 = std::atan(C492);
        const double C500 = std::atan(C493);
        const double C501 = std::atan(C494);
        const double C502 = 2.2431064 * C501;
        const double C503 = 2.26214 * C501;
        const double C504 = 7.03496 * C500;
        const double C505 = 7.45488 * C500;
        const double C506 = std::log(C495);
        const double C507 = std::log(C496);
        const double C508 = std::log(C497);
        const double C509 = std::log(C498);
        const double C510 = C502 / C467;
        const double C511 = C503 / C467;
        const double C512 = C504 / C466;
        const double C513 = C505 / C466;
        const double C514 = C506 + C511;
        const double C515 = C507 + C513;
        const double C516 = C508 + C510;
        const double C517 = C509 + C512;
        const double C518 = -0.3913066512 * C517;
        const double C519 = -0.005382083488 * C516;
        const double C520 = C518 / 12.5549141492;
        const double C521 = C519 / 12.99914055888256;
        const double C522 = C515 - C520;
        const double C523 = C521 - C514;
        const double C524 = 0.0310907 * C522;

        result.exc =
            rhoB *
            (C524 + C523 * ((std::pow(C471 + 1., C464) + std::pow(1. - C471, C464)) - 2.) *
                        ((1.7099209341613653 *
                              (0.01554535 * ((std::log(C473 / C491) + 0.1412084e2 * C499 / C465) -
                                             -2.2946365 *
                                                 (std::log(std::pow(C474 + 0.325, 2) / C491) +
                                                  0.1282084e2 * C499 / C465) /
                                                 15.8687885) -
                               C524) *
                              C470 / C523 -
                          1.) *
                             std::pow((-rhoB) / rhoB, 4) +
                         1.) /
                        (3.4198418683227306 * (std::pow(2., C463) - 1.) * C470));
    } else
    {
        const double C526 = 0.9999999999999999 * rhoA;
        const double C527 = Pi * rhoA;
        const double C528 = 1. / 3.;
        const double C529 = 4. / 3.;
        const double C530 = std::sqrt(0.223816694236e2);
        const double C531 = std::sqrt(0.378469910464e2);
        const double C532 = std::sqrt(0.507386806551e2);
        const double C533 = std::pow(Pi, 2);
        const double C534 = 4. * C527;
        const double C535 = 6. * C533;
        const double C536 = C526 / rhoA;
        const double C537 = 3. / C534;
        const double C538 = std::pow(C537, C528);
        const double C539 = std::sqrt(C538);
        const double C540 = 1.13107 * C539;
        const double C541 = 2. * C539;
        const double C542 = 3.72744 * C539;
        const double C543 = 7.06042 * C539;
        const double C544 = C539 + 0.0047584;
        const double C545 = C539 + 0.10498;
        const double C546 = C538 + C540;
        const double C547 = C538 + C542;
        const double C548 = C538 + C543;
        const double C549 = C541 + 1.13107;
        const double C550 = C541 + 3.72744;
        const double C551 = C541 + 7.06042;
        const double C552 = std::pow(C544, 2);
        const double C553 = std::pow(C545, 2);
        const double C554 = C546 + 13.0045;
        const double C555 = C547 + 12.9352;
        const double C556 = C548 + 18.0578;
        const double C557 = C530 / C551;
        const double C558 = C531 / C550;
        const double C559 = C532 / C549;
        const double C560 = C538 / C554;
        const double C561 = C538 / C555;
        const double C562 = C552 / C554;
        const double C563 = C553 / C555;
        const double C564 = std::atan(C557);
        const double C565 = std::atan(C558);
        const double C566 = std::atan(C559);
        const double C567 = 2.2431064 * C566;
        const double C568 = 2.26214 * C566;
        const double C569 = 7.03496 * C565;
        const double C570 = 7.45488 * C565;
        const double C571 = std::log(C560);
        const double C572 = std::log(C561);
        const double C573 = std::log(C562);
        const double C574 = std::log(C563);
        const double C575 = C567 / C532;
        const double C576 = C568 / C532;
        const double C577 = C569 / C531;
        const double C578 = C570 / C531;
        const double C579 = C571 + C576;
        const double C580 = C572 + C578;
        const double C581 = C573 + C575;
        const double C582 = C574 + C577;
        const double C583 = -0.3913066512 * C582;
        const double C584 = -0.005382083488 * C581;
        const double C585 = C583 / 12.5549141492;
        const double C586 = C584 / 12.99914055888256;
        const double C587 = C580 - C585;
        const double C588 = C586 - C579;
        const double C589 = 0.0310907 * C587;

        result.exc =
            rhoA *
            (C589 + C588 * ((std::pow(C536 + 1., C529) + std::pow(1. - C536, C529)) - 2.) *
                        1.7099209341613653 *
                        (0.01554535 * ((std::log(C538 / C556) + 0.1412084e2 * C564 / C530) -
                                       -2.2946365 *
                                           (std::log(std::pow(C539 + 0.325, 2) / C556) +
                                            0.1282084e2 * C564 / C530) /
                                           15.8687885) -
                         C589) *
                        C535 / (C588 * 3.4198418683227306 * (std::pow(2., C528) - 1.) * C535));
    }

    if (rhoA >= eps)
    {
        const double C530 = std::sqrt(0.223816694236e2);
        const double C531 = std::sqrt(0.378469910464e2);
        const double C532 = std::sqrt(0.507386806551e2);
        const double C394 = rhoA + rhoB;
        const double C395 = rhoA - rhoB;
        const double C396 = 1. / 3.;
        const double C397 = 4. / 3.;
        const double C401 = std::pow(Pi, 2);
        const double C402 = 0.9999999999999999 * C395;
        const double C403 = 6. * C401;
        const double C404 = Pi * C394;
        const double C405 = 4. * C404;
        const double C406 = C402 / C394;
        const double C407 = 3. / C405;
        const double C408 = std::pow(C407, C396);
        const double C410 = 1.13107 * std::sqrt(C408);
        const double C411 = 2. * std::sqrt(C408);
        const double C412 = 3.72744 * std::sqrt(C408);
        const double C413 = 7.06042 * std::sqrt(C408);
        const double C414 = std::sqrt(C408) + 0.0047584;
        const double C415 = std::sqrt(C408) + 0.10498;
        const double C416 = C408 + C410;
        const double C417 = C408 + C412;
        const double C418 = C408 + C413;
        const double C419 = C411 + 1.13107;
        const double C420 = C411 + 3.72744;
        const double C421 = C411 + 7.06042;
        const double C422 = std::pow(C414, 2);
        const double C423 = std::pow(C415, 2);
        const double C424 = C416 + 13.0045;
        const double C425 = C417 + 12.9352;
        const double C426 = C418 + 18.0578;
        const double C427 = std::sqrt(0.223816694236e2) / C421;
        const double C428 = std::sqrt(0.378469910464e2) / C420;
        const double C429 = std::sqrt(0.507386806551e2) / C419;
        const double C430 = C408 / C424;
        const double C431 = C408 / C425;
        const double C432 = C422 / C424;
        const double C433 = C423 / C425;
        const double C434 = std::atan(C427);
        const double C435 = std::atan(C428);
        const double C436 = std::atan(C429);
        const double C437 = 2.2431064 * C436;
        const double C438 = 2.26214 * C436;
        const double C439 = 7.03496 * C435;
        const double C440 = 7.45488 * C435;
        const double C441 = std::log(C430);
        const double C442 = std::log(C431);
        const double C443 = std::log(C432);
        const double C444 = std::log(C433);
        const double C445 = C437 / std::sqrt(0.507386806551e2);
        const double C446 = C438 / std::sqrt(0.507386806551e2);
        const double C447 = C439 / std::sqrt(0.378469910464e2);
        const double C448 = C440 / std::sqrt(0.378469910464e2);
        const double C449 = C441 + C446;
        const double C450 = C442 + C448;
        const double C451 = C443 + C445;
        const double C452 = C444 + C447;
        const double C453 = -0.3913066512 * C452;
        const double C454 = -0.005382083488 * C451;
        const double C455 = C453 / 12.5549141492;
        const double C456 = C454 / 12.99914055888256;
        const double C457 = C450 - C455;
        const double C458 = C456 - C449;
        const double C459 = 0.0310907 * C457;
        const double C591 = 1;
        const double C592 = 1;
        const double C593 = 0.9999999999999999 * C592 + 0.;
        const double C594 = Pi * C591;
        const double C595 = 4. * C594;
        const double C596 = C593 / C394 - C591 * C402 / std::pow(C394, 2);
        const double C597 = -3. * C595 / std::pow(C405, 2);
        const double C598 = C396 * std::pow(C407, C396 - 1) * C597;
        const double C599 = 1.13107 * C598 / (2 * std::sqrt(C408));
        const double C600 = 2. * C598 / (2 * std::sqrt(C408));
        const double C601 = 3.72744 * C598 / (2 * std::sqrt(C408));
        const double C602 = 7.06042 * C598 / (2 * std::sqrt(C408));
        const double C603 = C598 / (2 * std::sqrt(C408));
        const double C604 = C598 / (2 * std::sqrt(C408));
        const double C605 = C598 + C599;
        const double C606 = C598 + C601;
        const double C607 = C598 + C602;
        const double C608 = C600;
        const double C609 = C600;
        const double C610 = C600;
        const double C611 = 2 * C414 * C603;
        const double C612 = 2 * C415 * C604;
        const double C613 = C605;
        const double C614 = C606;
        const double C615 = C607;
        const double C616 = (-C610 * C530) / std::pow(C421, 2);
        const double C617 = (-C609 * C531) / std::pow(C420, 2);
        const double C618 = (-C608 * C532) / std::pow(C419, 2);
        const double C619 = C598 / C424 - C613 * C408 / std::pow(C424, 2);
        const double C620 = C598 / C425 - C614 * C408 / std::pow(C425, 2);
        const double C621 = C611 / C424 - C613 * C422 / std::pow(C424, 2);
        const double C622 = C612 / C425 - C614 * C423 / std::pow(C425, 2);
        const double C623 = C616 / (std::pow(C427, 2) + 1);
        const double C624 = C617 / (std::pow(C428, 2) + 1);
        const double C625 = C618 / (std::pow(C429, 2) + 1);
        const double C626 = 2.2431064 * C625;
        const double C627 = 2.26214 * C625;
        const double C628 = 7.03496 * C624;
        const double C629 = 7.45488 * C624;
        const double C630 = C619 / C430;
        const double C631 = C620 / C431;
        const double C632 = C621 / C432;
        const double C633 = C622 / C433;
        const double C634 = std::sqrt(1 / 0.507386806551e2) * C626;
        const double C635 = std::sqrt(1 / 0.507386806551e2) * C627;
        const double C636 = std::sqrt(1 / 0.378469910464e2) * C628;
        const double C637 = std::sqrt(1 / 0.378469910464e2) * C629;
        const double C638 = C630 + C635;
        const double C639 = C631 + C637;
        const double C640 = C632 + C634;
        const double C641 = C633 + C636;
        const double C642 = -0.3913066512 * C641;
        const double C643 = -0.005382083488 * C640;
        const double C644 = C642 / 12.5549141492;
        const double C645 = C643 / 12.99914055888256;
        const double C646 = C639 - C644;
        const double C647 = C645 - C638;
        const double C648 = 0.0310907 * C646;

        result.vrhoA =
            (((((((C394 * C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                       0. * std::pow(C395 / C394, 4) /
                       (C458 * 3.4198418683227306 * (std::pow(2., C396) - 1.) * C403) +
                   (C394 * C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                        ((1.7099209341613653 *
                              (0.01554535 *
                                   ((std::log(C408 / C426) + 0.1412084e2 * C434 / C530) -
                                    -2.2946365 *
                                        (std::log(std::pow(std::sqrt(C408) + 0.325, 2) / C426) +
                                         0.1282084e2 * C434 / C530) /
                                        15.8687885) -
                               C459) *
                              C403 / C458 -
                          1.) *
                             std::pow(C395 / C394, 3) * -4 * C395 / std::pow(C394, 2) +
                         0. * std::pow(C395 / C394, 4) / C458) /
                        (3.4198418683227306 * (std::pow(2., C396) - 1.) * C403) +
                    (C459 +
                     C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                         ((1.7099209341613653 *
                               (0.01554535 *
                                    ((std::log(C408 / C426) + 0.1412084e2 * C434 / C530) -
                                     -2.2946365 *
                                         (std::log(std::pow(std::sqrt(C408) + 0.325, 2) / C426) +
                                          0.1282084e2 * C434 / C530) /
                                         15.8687885) -
                                C459) *
                               C403 / C458 -
                           1.) *
                              std::pow(C395 / C394, 4) +
                          1.) /
                         (3.4198418683227306 * (std::pow(2., C396) - 1.) * C403))) *
                       C591) +
                  C394 *
                      (C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                           std::pow(C395 / C394, 4) * -1.7099209341613653 * C403 /
                           (C458 * 3.4198418683227306 * (std::pow(2., C396) - 1.) * C403) +
                       1) *
                      C648) +
                 C647 * C394 *
                     (C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                          std::pow(C395 / C394, 4) *
                          (0. * C458 -
                           1.7099209341613653 *
                               (0.01554535 *
                                    ((std::log(C408 / C426) + 0.1412084e2 * C434 / C530) -
                                     -2.2946365 *
                                         (std::log(std::pow(std::sqrt(C408) + 0.325, 2) / C426) +
                                          0.1282084e2 * C434 / C530) /
                                         15.8687885) -
                                C459) *
                               C403) /
                          std::pow(C458, 2) +
                      ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                          ((1.7099209341613653 *
                                (0.01554535 *
                                     ((std::log(C408 / C426) + 0.1412084e2 * C434 / C530) -
                                      -2.2946365 *
                                          (std::log(std::pow(std::sqrt(C408) + 0.325, 2) / C426) +
                                           0.1282084e2 * C434 / C530) /
                                          15.8687885) -
                                 C459) *
                                C403 / C458 -
                            1.) *
                               std::pow(C395 / C394, 4) +
                           1.)) /
                     (3.4198418683227306 * (std::pow(2., C396) - 1.) * C403)) +
                C596 * C394 *
                    (C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) * 0. *
                         std::pow(C395 / C394, 4) / C458 +
                     C458 *
                         (C397 * std::pow(C406 + 1., C397 - 1) -
                          C397 * std::pow(1. - C406, C397 - 1)) *
                         ((1.7099209341613653 *
                               (0.01554535 *
                                    ((std::log(C408 / C426) + 0.1412084e2 * C434 / C530) -
                                     -2.2946365 *
                                         (std::log(std::pow(std::sqrt(C408) + 0.325, 2) / C426) +
                                          0.1282084e2 * C434 / C530) /
                                         15.8687885) -
                                C459) *
                               C403 / C458 -
                           1.) *
                              std::pow(C395 / C394, 4) +
                          1.)) /
                    (3.4198418683227306 * (std::pow(2., C396) - 1.) * C403)) +
               C598 * C394 * C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                   std::pow(C395 / C394, 4) * 0.2658131939386538e-1 *
                   (C426 / (C426 * C408) - -2.2946365 * (std::sqrt(C408) + 0.325) * C426 /
                                               (15.8687885 * std::sqrt(C408) * C426 *
                                                std::pow(std::sqrt(C408) + 0.325, 2))) *
                   C403 / (C458 * 3.4198418683227306 * (std::pow(2., C396) - 1.) * C403)) +
              C615 * C394 * C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                  std::pow(C395 / C394, 4) * 0.2658131939386538e-1 *
                  ((-C408 * C426) / (std::pow(C426, 2) * C408) -
                   2.2946365 * std::pow(std::sqrt(C408) + 0.325, 2) * C426 /
                       (15.8687885 * std::pow(C426, 2) * std::pow(std::sqrt(C408) + 0.325, 2))) *
                  C403 / (C458 * 3.4198418683227306 * (std::pow(2., C396) - 1.) * C403)) +
             C623 * C394 * C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                 std::pow(C395 / C394, 4) * 0.2658131939386538e-1 *
                 (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                  (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                 C403 / (C458 * 3.4198418683227306 * (std::pow(2., C396) - 1.) * C403)) +
            C592 * C394 * C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                ((1.7099209341613653 *
                      (0.01554535 *
                           ((std::log(C408 / C426) + 0.1412084e2 * C434 / C530) -
                            -2.2946365 *
                                (std::log(std::pow(std::sqrt(C408) + 0.325, 2) / C426) +
                                 0.1282084e2 * C434 / C530) /
                                15.8687885) -
                       C459) *
                      C403 / C458 -
                  1.) *
                     4 * std::pow(C395 / C394, 3) / C394 +
                 0. * std::pow(C395 / C394, 4) / C458) /
                (3.4198418683227306 * (std::pow(2., C396) - 1.) * C403);
    }

    if (rhoB >= eps)
    {
        const double C465 = std::sqrt(0.223816694236e2);
        const double C466 = std::sqrt(0.378469910464e2);
        const double C467 = std::sqrt(0.507386806551e2);
        const double C394 = rhoA + rhoB;
        const double C395 = rhoA - rhoB;
        const double C396 = 1. / 3.;
        const double C397 = 4. / 3.;
        const double C401 = std::pow(Pi, 2);
        const double C402 = 0.9999999999999999 * C395;
        const double C403 = 6. * C401;
        const double C404 = Pi * C394;
        const double C405 = 4. * C404;
        const double C406 = C402 / C394;
        const double C407 = 3. / C405;
        const double C408 = std::pow(C407, C396);
        const double C410 = 1.13107 * std::sqrt(C408);
        const double C411 = 2. * std::sqrt(C408);
        const double C412 = 3.72744 * std::sqrt(C408);
        const double C413 = 7.06042 * std::sqrt(C408);
        const double C414 = std::sqrt(C408) + 0.0047584;
        const double C415 = std::sqrt(C408) + 0.10498;
        const double C416 = C408 + C410;
        const double C417 = C408 + C412;
        const double C418 = C408 + C413;
        const double C419 = C411 + 1.13107;
        const double C420 = C411 + 3.72744;
        const double C421 = C411 + 7.06042;
        const double C422 = std::pow(C414, 2);
        const double C423 = std::pow(C415, 2);
        const double C424 = C416 + 13.0045;
        const double C425 = C417 + 12.9352;
        const double C426 = C418 + 18.0578;
        const double C427 = std::sqrt(0.223816694236e2) / C421;
        const double C428 = std::sqrt(0.378469910464e2) / C420;
        const double C429 = std::sqrt(0.507386806551e2) / C419;
        const double C430 = C408 / C424;
        const double C431 = C408 / C425;
        const double C432 = C422 / C424;
        const double C433 = C423 / C425;
        const double C434 = std::atan(C427);
        const double C435 = std::atan(C428);
        const double C436 = std::atan(C429);
        const double C437 = 2.2431064 * C436;
        const double C438 = 2.26214 * C436;
        const double C439 = 7.03496 * C435;
        const double C440 = 7.45488 * C435;
        const double C441 = std::log(C430);
        const double C442 = std::log(C431);
        const double C443 = std::log(C432);
        const double C444 = std::log(C433);
        const double C445 = C437 / std::sqrt(0.507386806551e2);
        const double C446 = C438 / std::sqrt(0.507386806551e2);
        const double C447 = C439 / std::sqrt(0.378469910464e2);
        const double C448 = C440 / std::sqrt(0.378469910464e2);
        const double C449 = C441 + C446;
        const double C450 = C442 + C448;
        const double C451 = C443 + C445;
        const double C452 = C444 + C447;
        const double C453 = -0.3913066512 * C452;
        const double C454 = -0.005382083488 * C451;
        const double C455 = C453 / 12.5549141492;
        const double C456 = C454 / 12.99914055888256;
        const double C457 = C450 - C455;
        const double C458 = C456 - C449;
        const double C459 = 0.0310907 * C457;
        const double C649 = 1;
        const double C650 = -1;
        const double C651 = 0.9999999999999999 * C650 + 0.;
        const double C652 = Pi * C649;
        const double C653 = 4. * C652;
        const double C654 = C651 / C394 - C649 * C402 / std::pow(C394, 2);
        const double C655 = -3. * C653 / std::pow(C405, 2);
        const double C656 = C396 * std::pow(C407, C396 - 1) * C655;
        const double C657 = 1.13107 * C656 / (2 * std::sqrt(C408));
        const double C658 = 2. * C656 / (2 * std::sqrt(C408));
        const double C659 = 3.72744 * C656 / (2 * std::sqrt(C408));
        const double C660 = 7.06042 * C656 / (2 * std::sqrt(C408));
        const double C661 = C656 / (2 * std::sqrt(C408));
        const double C662 = C656 / (2 * std::sqrt(C408));
        const double C663 = C656 + C657;
        const double C664 = C656 + C659;
        const double C665 = C656 + C660;
        const double C666 = C658;
        const double C667 = C658;
        const double C668 = C658;
        const double C669 = 2 * C414 * C661;
        const double C670 = 2 * C415 * C662;
        const double C671 = C663;
        const double C672 = C664;
        const double C673 = C665;
        const double C674 = (-C668 * C465) / std::pow(C421, 2);
        const double C675 = (-C667 * C466) / std::pow(C420, 2);
        const double C676 = (-C666 * C467) / std::pow(C419, 2);
        const double C677 = C656 / C424 - C671 * C408 / std::pow(C424, 2);
        const double C678 = C656 / C425 - C672 * C408 / std::pow(C425, 2);
        const double C679 = C669 / C424 - C671 * C422 / std::pow(C424, 2);
        const double C680 = C670 / C425 - C672 * C423 / std::pow(C425, 2);
        const double C681 = C674 / (std::pow(C427, 2) + 1);
        const double C682 = C675 / (std::pow(C428, 2) + 1);
        const double C683 = C676 / (std::pow(C429, 2) + 1);
        const double C684 = 2.2431064 * C683;
        const double C685 = 2.26214 * C683;
        const double C686 = 7.03496 * C682;
        const double C687 = 7.45488 * C682;
        const double C688 = C677 / C430;
        const double C689 = C678 / C431;
        const double C690 = C679 / C432;
        const double C691 = C680 / C433;
        const double C692 = std::sqrt(1 / 0.507386806551e2) * C684;
        const double C693 = std::sqrt(1 / 0.507386806551e2) * C685;
        const double C694 = std::sqrt(1 / 0.378469910464e2) * C686;
        const double C695 = std::sqrt(1 / 0.378469910464e2) * C687;
        const double C696 = C688 + C693;
        const double C697 = C689 + C695;
        const double C698 = C690 + C692;
        const double C699 = C691 + C694;
        const double C700 = -0.3913066512 * C699;
        const double C701 = -0.005382083488 * C698;
        const double C702 = C700 / 12.5549141492;
        const double C703 = C701 / 12.99914055888256;
        const double C704 = C697 - C702;
        const double C705 = C703 - C696;
        const double C706 = 0.0310907 * C704;

        result.vrhoB =
            (((((((C394 * C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                       0. * std::pow(C395 / C394, 4) /
                       (C458 * 3.4198418683227306 * (std::pow(2., C396) - 1.) * C403) +
                   (C394 * C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                        ((1.7099209341613653 *
                              (0.01554535 *
                                   ((std::log(C408 / C426) + 0.1412084e2 * C434 / C465) -
                                    -2.2946365 *
                                        (std::log(std::pow(std::sqrt(C408) + 0.325, 2) / C426) +
                                         0.1282084e2 * C434 / C465) /
                                        15.8687885) -
                               C459) *
                              C403 / C458 -
                          1.) *
                             std::pow(C395 / C394, 3) * -4 * C395 / std::pow(C394, 2) +
                         0. * std::pow(C395 / C394, 4) / C458) /
                        (3.4198418683227306 * (std::pow(2., C396) - 1.) * C403) +
                    (C459 +
                     C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                         ((1.7099209341613653 *
                               (0.01554535 *
                                    ((std::log(C408 / C426) + 0.1412084e2 * C434 / C465) -
                                     -2.2946365 *
                                         (std::log(std::pow(std::sqrt(C408) + 0.325, 2) / C426) +
                                          0.1282084e2 * C434 / C465) /
                                         15.8687885) -
                                C459) *
                               C403 / C458 -
                           1.) *
                              std::pow(C395 / C394, 4) +
                          1.) /
                         (3.4198418683227306 * (std::pow(2., C396) - 1.) * C403))) *
                       C649) +
                  C394 *
                      (C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                           std::pow(C395 / C394, 4) * -1.7099209341613653 * C403 /
                           (C458 * 3.4198418683227306 * (std::pow(2., C396) - 1.) * C403) +
                       1) *
                      C706) +
                 C705 * C394 *
                     (C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                          std::pow(C395 / C394, 4) *
                          (0. * C458 -
                           1.7099209341613653 *
                               (0.01554535 *
                                    ((std::log(C408 / C426) + 0.1412084e2 * C434 / C465) -
                                     -2.2946365 *
                                         (std::log(std::pow(std::sqrt(C408) + 0.325, 2) / C426) +
                                          0.1282084e2 * C434 / C465) /
                                         15.8687885) -
                                C459) *
                               C403) /
                          std::pow(C458, 2) +
                      ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                          ((1.7099209341613653 *
                                (0.01554535 *
                                     ((std::log(C408 / C426) + 0.1412084e2 * C434 / C465) -
                                      -2.2946365 *
                                          (std::log(std::pow(std::sqrt(C408) + 0.325, 2) / C426) +
                                           0.1282084e2 * C434 / C465) /
                                          15.8687885) -
                                 C459) *
                                C403 / C458 -
                            1.) *
                               std::pow(C395 / C394, 4) +
                           1.)) /
                     (3.4198418683227306 * (std::pow(2., C396) - 1.) * C403)) +
                C654 * C394 *
                    (C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) * 0. *
                         std::pow(C395 / C394, 4) / C458 +
                     C458 *
                         (C397 * std::pow(C406 + 1., C397 - 1) -
                          C397 * std::pow(1. - C406, C397 - 1)) *
                         ((1.7099209341613653 *
                               (0.01554535 *
                                    ((std::log(C408 / C426) + 0.1412084e2 * C434 / C465) -
                                     -2.2946365 *
                                         (std::log(std::pow(std::sqrt(C408) + 0.325, 2) / C426) +
                                          0.1282084e2 * C434 / C465) /
                                         15.8687885) -
                                C459) *
                               C403 / C458 -
                           1.) *
                              std::pow(C395 / C394, 4) +
                          1.)) /
                    (3.4198418683227306 * (std::pow(2., C396) - 1.) * C403)) +
               C656 * C394 * C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                   std::pow(C395 / C394, 4) * 0.2658131939386538e-1 *
                   (C426 / (C426 * C408) - -2.2946365 * (std::sqrt(C408) + 0.325) * C426 /
                                               (15.8687885 * std::sqrt(C408) * C426 *
                                                std::pow(std::sqrt(C408) + 0.325, 2))) *
                   C403 / (C458 * 3.4198418683227306 * (std::pow(2., C396) - 1.) * C403)) +
              C673 * C394 * C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                  std::pow(C395 / C394, 4) * 0.2658131939386538e-1 *
                  ((-C408 * C426) / (std::pow(C426, 2) * C408) -
                   2.2946365 * std::pow(std::sqrt(C408) + 0.325, 2) * C426 /
                       (15.8687885 * std::pow(C426, 2) * std::pow(std::sqrt(C408) + 0.325, 2))) *
                  C403 / (C458 * 3.4198418683227306 * (std::pow(2., C396) - 1.) * C403)) +
             C681 * C394 * C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                 std::pow(C395 / C394, 4) * 0.2658131939386538e-1 *
                 (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                  (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                 C403 / (C458 * 3.4198418683227306 * (std::pow(2., C396) - 1.) * C403)) +
            C650 * C394 * C458 * ((std::pow(C406 + 1., C397) + std::pow(1. - C406, C397)) - 2.) *
                ((1.7099209341613653 *
                      (0.01554535 *
                           ((std::log(C408 / C426) + 0.1412084e2 * C434 / C465) -
                            -2.2946365 *
                                (std::log(std::pow(std::sqrt(C408) + 0.325, 2) / C426) +
                                 0.1282084e2 * C434 / C465) /
                                15.8687885) -
                       C459) *
                      C403 / C458 -
                  1.) *
                     4 * std::pow(C395 / C394, 3) / C394 +
                 0. * std::pow(C395 / C394, 4) / C458) /
                (3.4198418683227306 * (std::pow(2., C396) - 1.) * C403);
    }

    return result;
}

void Vwn5CorrelationSecondDerivatives(double rhoA,
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
    const double C50 = std::sqrt(0.223816694236e2);
    const double C51 = std::sqrt(0.378469910464e2);
    const double C52 = std::sqrt(0.507386806551e2);
    const double C53 = std::pow(Pi, 2);
    const double C54 = 0.9999999999999999 * C47;
    const double C55 = 6. * C53;
    const double C56 = Pi * C46;
    const double C57 = 4. * C56;
    const double C58 = C54 / C46;
    const double C59 = 3. / C57;
    const double C60 = std::pow(C59, C48);
    const double C61 = std::sqrt(C60);
    const double C62 = 1.13107 * C61;
    const double C63 = 2. * C61;
    const double C64 = 3.72744 * C61;
    const double C65 = 7.06042 * C61;
    const double C66 = C61 + 0.0047584;
    const double C67 = C61 + 0.10498;
    const double C68 = C60 + C62;
    const double C69 = C60 + C64;
    const double C70 = C60 + C65;
    const double C71 = C63 + 1.13107;
    const double C72 = C63 + 3.72744;
    const double C73 = C63 + 7.06042;
    const double C74 = std::pow(C66, 2);
    const double C75 = std::pow(C67, 2);
    const double C76 = C68 + 13.0045;
    const double C77 = C69 + 12.9352;
    const double C78 = C70 + 18.0578;
    const double C79 = C50 / C73;
    const double C80 = C51 / C72;
    const double C81 = C52 / C71;
    const double C82 = C60 / C76;
    const double C83 = C60 / C77;
    const double C84 = C74 / C76;
    const double C85 = C75 / C77;
    const double C86 = std::atan(C79);
    const double C87 = std::atan(C80);
    const double C88 = std::atan(C81);
    const double C89 = 2.2431064 * C88;
    const double C90 = 2.26214 * C88;
    const double C91 = 7.03496 * C87;
    const double C92 = 7.45488 * C87;
    const double C93 = std::log(C82);
    const double C94 = std::log(C83);
    const double C95 = std::log(C84);
    const double C96 = std::log(C85);
    const double C97 = C89 / C52;
    const double C98 = C90 / C52;
    const double C99 = C91 / C51;
    const double C100 = C92 / C51;
    const double C101 = C93 + C98;
    const double C102 = C94 + C100;
    const double C103 = C95 + C97;
    const double C104 = C96 + C99;
    const double C105 = -0.3913066512 * C104;
    const double C106 = -0.005382083488 * C103;
    const double C107 = C105 / 12.5549141492;
    const double C108 = C106 / 12.99914055888256;
    const double C109 = C102 - C107;
    const double C110 = C108 - C101;
    const double C111 = 0.0310907 * C109;
    const double C116 = 1;
    const double C117 = 1;
    const double C118 = 0.9999999999999999 * C117 + 0.;
    const double C119 = Pi * C116;
    const double C120 = 4. * C119;
    const double C121 = C118 / C46 - C116 * C54 / std::pow(C46, 2);
    const double C122 = -3. * C120 / std::pow(C57, 2);
    const double C123 = C48 * std::pow(C59, C48 - 1) * C122;
    const double C124 = 1.13107 * C123 / (2 * std::sqrt(C60));
    const double C125 = 2. * C123 / (2 * std::sqrt(C60));
    const double C126 = 3.72744 * C123 / (2 * std::sqrt(C60));
    const double C127 = 7.06042 * C123 / (2 * std::sqrt(C60));
    const double C128 = C123 / (2 * std::sqrt(C60));
    const double C129 = C123 / (2 * std::sqrt(C60));
    const double C130 = C123 + C124;
    const double C131 = C123 + C126;
    const double C132 = C123 + C127;
    const double C133 = C125;
    const double C134 = C125;
    const double C135 = C125;
    const double C136 = 2 * C66 * C128;
    const double C137 = 2 * C67 * C129;
    const double C138 = C130;
    const double C139 = C131;
    const double C140 = C132;
    const double C141 = (-C135 * std::sqrt(0.223816694236e2)) / std::pow(C73, 2);
    const double C142 = (-C134 * std::sqrt(0.378469910464e2)) / std::pow(C72, 2);
    const double C143 = (-C133 * std::sqrt(0.507386806551e2)) / std::pow(C71, 2);
    const double C144 = C123 / C76 - C138 * C60 / std::pow(C76, 2);
    const double C145 = C123 / C77 - C139 * C60 / std::pow(C77, 2);
    const double C146 = C136 / C76 - C138 * C74 / std::pow(C76, 2);
    const double C147 = C137 / C77 - C139 * C75 / std::pow(C77, 2);
    const double C148 = C141 / (std::pow(C79, 2) + 1);
    const double C149 = C142 / (std::pow(C80, 2) + 1);
    const double C150 = C143 / (std::pow(C81, 2) + 1);
    const double C151 = 2.2431064 * C150;
    const double C152 = 2.26214 * C150;
    const double C153 = 7.03496 * C149;
    const double C154 = 7.45488 * C149;
    const double C155 = C144 / C82;
    const double C156 = C145 / C83;
    const double C157 = C146 / C84;
    const double C158 = C147 / C85;
    const double C159 = std::sqrt(1 / 0.507386806551e2) * C151;
    const double C160 = std::sqrt(1 / 0.507386806551e2) * C152;
    const double C161 = std::sqrt(1 / 0.378469910464e2) * C153;
    const double C162 = std::sqrt(1 / 0.378469910464e2) * C154;
    const double C163 = C155 + C160;
    const double C164 = C156 + C162;
    const double C165 = C157 + C159;
    const double C166 = C158 + C161;
    const double C167 = -0.3913066512 * C166;
    const double C168 = -0.005382083488 * C165;
    const double C169 = C167 / 12.5549141492;
    const double C170 = C168 / 12.99914055888256;
    const double C171 = C164 - C169;
    const double C172 = C170 - C163;
    const double C173 = 0.0310907 * C171;
    const double C232 = 0.;
    const double C233 =
        (C232 / C46 +
         ((-C118) / std::pow(C46, 2) - (-C116 * C54 * 2 * C46) / std::pow(C46, 4)) * C116) -
        C118 * C116 / std::pow(C46, 2);
    const double C234 = (-C120 * -3. * C120 * 2 * C57) / std::pow(C57, 4);
    const double C235 = C48 * (C48 - 1) * std::pow(C59, C48 - 2) * std::pow(C122, 2) +
                        C48 * std::pow(C59, C48 - 1) * C234;
    const double C236 =
        1.13107 * C235 / (2 * std::sqrt(C60)) -
        1.13107 * std::pow(C123, 2) / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C237 = 2. * C235 / (2 * std::sqrt(C60)) -
                        2. * std::pow(C123, 2) / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C238 =
        3.72744 * C235 / (2 * std::sqrt(C60)) -
        3.72744 * std::pow(C123, 2) / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C239 =
        7.06042 * C235 / (2 * std::sqrt(C60)) -
        7.06042 * std::pow(C123, 2) / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C240 = C235 / (2 * std::sqrt(C60)) -
                        std::pow(C123, 2) / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C241 = C235 / (2 * std::sqrt(C60)) -
                        std::pow(C123, 2) / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C242 = C235 + C236;
    const double C243 = C235 + C238;
    const double C244 = C235 + C239;
    const double C245 = C237;
    const double C246 = C237;
    const double C247 = C237;
    const double C248 = 2 * std::pow(C128, 2) + 2 * C66 * C240;
    const double C249 = 2 * std::pow(C129, 2) + 2 * C67 * C241;
    const double C250 = C242;
    const double C251 = C243;
    const double C252 = C244;
    const double C253 = C135 * C135 * std::sqrt(0.223816694236e2) * 2 * C73 / std::pow(C73, 4) -
                        C247 * std::sqrt(0.223816694236e2) / std::pow(C73, 2);
    const double C254 = C134 * C134 * std::sqrt(0.378469910464e2) * 2 * C72 / std::pow(C72, 4) -
                        C246 * std::sqrt(0.378469910464e2) / std::pow(C72, 2);
    const double C255 = C133 * C133 * std::sqrt(0.507386806551e2) * 2 * C71 / std::pow(C71, 4) -
                        C245 * std::sqrt(0.507386806551e2) / std::pow(C71, 2);
    const double C256 =
        ((C235 / C76 +
          ((-C123) / std::pow(C76, 2) - (-C138 * C60 * 2 * C76) / std::pow(C76, 4)) * C138) -
         C250 * C60 / std::pow(C76, 2)) -
        C123 * C138 / std::pow(C76, 2);
    const double C257 =
        ((C235 / C77 +
          ((-C123) / std::pow(C77, 2) - (-C139 * C60 * 2 * C77) / std::pow(C77, 4)) * C139) -
         C251 * C60 / std::pow(C77, 2)) -
        C123 * C139 / std::pow(C77, 2);
    const double C258 =
        ((C248 / C76 +
          ((-C136) / std::pow(C76, 2) - (-C138 * C74 * 2 * C76) / std::pow(C76, 4)) * C138) -
         C250 * C74 / std::pow(C76, 2)) -
        C136 * C138 / std::pow(C76, 2);
    const double C259 =
        ((C249 / C77 +
          ((-C137) / std::pow(C77, 2) - (-C139 * C75 * 2 * C77) / std::pow(C77, 4)) * C139) -
         C251 * C75 / std::pow(C77, 2)) -
        C137 * C139 / std::pow(C77, 2);
    const double C260 = C253 / (std::pow(C79, 2) + 1) -
                        2 * C79 * std::pow(C141, 2) / std::pow(std::pow(C79, 2) + 1, 2);
    const double C261 = C254 / (std::pow(C80, 2) + 1) -
                        2 * C80 * std::pow(C142, 2) / std::pow(std::pow(C80, 2) + 1, 2);
    const double C262 = C255 / (std::pow(C81, 2) + 1) -
                        2 * C81 * std::pow(C143, 2) / std::pow(std::pow(C81, 2) + 1, 2);
    const double C263 = 2.2431064 * C262;
    const double C264 = 2.26214 * C262;
    const double C265 = 7.03496 * C261;
    const double C266 = 7.45488 * C261;
    const double C267 = C256 / C82 - std::pow(C144, 2) / std::pow(C82, 2);
    const double C268 = C257 / C83 - std::pow(C145, 2) / std::pow(C83, 2);
    const double C269 = C258 / C84 - std::pow(C146, 2) / std::pow(C84, 2);
    const double C270 = C259 / C85 - std::pow(C147, 2) / std::pow(C85, 2);
    const double C271 = std::sqrt(1 / 0.507386806551e2) * C263;
    const double C272 = std::sqrt(1 / 0.507386806551e2) * C264;
    const double C273 = std::sqrt(1 / 0.378469910464e2) * C265;
    const double C274 = std::sqrt(1 / 0.378469910464e2) * C266;
    const double C275 = C267 + C272;
    const double C276 = C268 + C274;
    const double C277 = C269 + C271;
    const double C278 = C270 + C273;
    const double C279 = -0.3913066512 * C278;
    const double C280 = -0.005382083488 * C277;
    const double C281 = C279 / 12.5549141492;
    const double C282 = C280 / 12.99914055888256;
    const double C283 = C276 - C281;
    const double C284 = C282 - C275;
    const double C285 = 0.0310907 * C283;
    matrix.upper[0] =
        (((((((((((((((((((((C46 *
                                 (0. * C110 *
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                  0. * std::pow(C47 / C46, 4)) /
                                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                             (C46 * C110 *
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  (0. * std::pow(C47 / C46, 3) * -4 * C47 /
                                       (C110 * std::pow(C46, 2)) +
                                   0. / C110) /
                                  (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                              C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  0. * std::pow(C47 / C46, 4) /
                                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                                 C116) +
                            C173 * C46 * C110 *
                                ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                C55 /
                                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                           C172 * C46 *
                               (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                    0. * std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                                ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                    std::pow(C47 / C46, 4) / C110) /
                               (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                          C121 * C46 *
                              ((0. * C110 *
                                    ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                0. * std::pow(C47 / C46, 4)) +
                               C110 *
                                   (C49 * std::pow(C58 + 1., C49 - 1) -
                                    C49 * std::pow(1. - C58, C49 - 1)) *
                                   0. * std::pow(C47 / C46, 4)) /
                              (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C123 * C46 * C110 *
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             (0. * std::pow(C47 / C46, 4) +
                              0. *
                                  (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                           (15.8687885 * std::sqrt(C60) * C78 *
                                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                  C55) /
                             (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        C140 * C46 * C110 *
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            (0. * std::pow(C47 / C46, 4) +
                             0. *
                                 ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                      (15.8687885 * std::pow(C78, 2) *
                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                                 C55) /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C148 * C46 * C110 *
                           ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                           (0. * std::pow(C47 / C46, 4) +
                            0. *
                                (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                 (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                     15.8687885) *
                                C55) /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C117 * C46 * C110 *
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                          (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
                          (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     (((((((((C46 *
                                  (C110 *
                                       ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                       0. * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                   0. * std::pow(C47 / C46, 4)) +
                              C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  0. * std::pow(C47 / C46, 4)) /
                                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                             C116 *
                                 ((C46 * C110 *
                                       ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                       ((std::pow(C46, 2) *
                                             ((1.7099209341613653 *
                                                   (0.01554535 *
                                                        ((std::log(C60 / C78) +
                                                          0.1412084e2 * C86 /
                                                              std::sqrt(0.223816694236e2)) -
                                                         -2.2946365 *
                                                             (std::log(
                                                                  std::pow(std::sqrt(C60) + 0.325,
                                                                           2) /
                                                                  C78) +
                                                              0.1282084e2 * C86 /
                                                                  std::sqrt(0.223816694236e2)) /
                                                             15.8687885) -
                                                    C111) *
                                                   C55 / C110 -
                                               1.) *
                                                  -4 * C47 * std::pow(C47 / C46, 2) * -3 * C47 /
                                                  std::pow(C46, 2) +
                                              0. * std::pow(C47 / C46, 3) * -4 * C47 / C110) -
                                         (1.7099209341613653 *
                                              (0.01554535 *
                                                   ((std::log(C60 / C78) +
                                                     0.1412084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) -
                                                    -2.2946365 *
                                                        (std::log(
                                                             std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                         0.1282084e2 * C86 /
                                                             std::sqrt(0.223816694236e2)) /
                                                        15.8687885) -
                                               C111) *
                                              C55 / C110 -
                                          1.) *
                                             std::pow(C47 / C46, 3) * -4 * C47 * 2 * C46) /
                                            std::pow(C46, 4) +
                                        0. * std::pow(C47 / C46, 3) * -4 * C47 /
                                            (std::pow(C46, 2) * C110)) +
                                   C110 *
                                       ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                       ((1.7099209341613653 *
                                             (0.01554535 *
                                                  ((std::log(C60 / C78) +
                                                    0.1412084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) -
                                                   -2.2946365 *
                                                       (std::log(
                                                            std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                        0.1282084e2 * C86 /
                                                            std::sqrt(0.223816694236e2)) /
                                                       15.8687885) -
                                              C111) *
                                             C55 / C110 -
                                         1.) *
                                            std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                        0. * std::pow(C47 / C46, 4) / C110)) +
                                  C110 *
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                      ((1.7099209341613653 *
                                            (0.01554535 *
                                                 ((std::log(C60 / C78) +
                                                   0.1412084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) -
                                                  -2.2946365 *
                                                      (std::log(
                                                           std::pow(std::sqrt(C60) + 0.325, 2) /
                                                           C78) +
                                                       0.1282084e2 * C86 /
                                                           std::sqrt(0.223816694236e2)) /
                                                      15.8687885) -
                                             C111) *
                                            C55 / C110 -
                                        1.) *
                                           std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                       0. * std::pow(C47 / C46, 4) / C110)) /
                                 (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                            (C46 * C110 *
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 -1.7099209341613653 * C55 * std::pow(C47 / C46, 3) * -4 * C47 /
                                 (std::pow(C46, 2) * C110 * 3.4198418683227306 *
                                  (std::pow(2., C48) - 1.) * C55) +
                             (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 /
                                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                              1)) *
                                C173) +
                           C172 *
                               (C46 *
                                    (C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         (0. * std::pow(C47 / C46, 4) +
                                          (0. * C110 -
                                           1.7099209341613653 *
                                               (0.01554535 *
                                                    ((std::log(C60 / C78) +
                                                      0.1412084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) -
                                                     -2.2946365 *
                                                         (std::log(
                                                              std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                          0.1282084e2 * C86 /
                                                              std::sqrt(0.223816694236e2)) /
                                                         15.8687885) -
                                                C111) *
                                               C55) *
                                              std::pow(C47 / C46, 3) * -4 * C47 /
                                              std::pow(C46, 2)) /
                                         std::pow(C110, 2) +
                                     ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                         ((1.7099209341613653 *
                                               (0.01554535 *
                                                    ((std::log(C60 / C78) +
                                                      0.1412084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) -
                                                     -2.2946365 *
                                                         (std::log(
                                                              std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                          0.1282084e2 * C86 /
                                                              std::sqrt(0.223816694236e2)) /
                                                         15.8687885) -
                                                C111) *
                                               C55 / C110 -
                                           1.) *
                                              std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                          0. * std::pow(C47 / C46, 4) / C110)) +
                                (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                     std::pow(C47 / C46, 4) *
                                     (0. * C110 -
                                      1.7099209341613653 *
                                          (0.01554535 *
                                               ((std::log(C60 / C78) +
                                                 0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                -2.2946365 *
                                                    (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                     0.1282084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) /
                                                    15.8687885) -
                                           C111) *
                                          C55) /
                                     std::pow(C110, 2) +
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                     ((1.7099209341613653 *
                                           (0.01554535 *
                                                ((std::log(C60 / C78) +
                                                  0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                 -2.2946365 *
                                                     (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                      0.1282084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) /
                                                     15.8687885) -
                                            C111) *
                                           C55 / C110 -
                                       1.) *
                                          std::pow(C47 / C46, 4) +
                                      1.))) /
                               (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                          C121 *
                              (C46 *
                                   ((C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         0. * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                     0. * std::pow(C47 / C46, 4)) /
                                        C110 +
                                    C110 *
                                        (C49 * std::pow(C58 + 1., C49 - 1) -
                                         C49 * std::pow(1. - C58, C49 - 1)) *
                                        ((1.7099209341613653 *
                                              (0.01554535 *
                                                   ((std::log(C60 / C78) +
                                                     0.1412084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) -
                                                    -2.2946365 *
                                                        (std::log(
                                                             std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                         0.1282084e2 * C86 /
                                                             std::sqrt(0.223816694236e2)) /
                                                        15.8687885) -
                                               C111) *
                                              C55 / C110 -
                                          1.) *
                                             std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                         0. * std::pow(C47 / C46, 4) / C110)) +
                               (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                    0. * std::pow(C47 / C46, 4) / C110 +
                                C110 *
                                    (C49 * std::pow(C58 + 1., C49 - 1) -
                                     C49 * std::pow(1. - C58, C49 - 1)) *
                                    ((1.7099209341613653 *
                                          (0.01554535 *
                                               ((std::log(C60 / C78) +
                                                 0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                -2.2946365 *
                                                    (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                     0.1282084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) /
                                                    15.8687885) -
                                           C111) *
                                          C55 / C110 -
                                      1.) *
                                         std::pow(C47 / C46, 4) +
                                     1.))) /
                              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C123 *
                             (C46 * C110 *
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  (0. * std::pow(C47 / C46, 4) +
                                   0.2658131939386538e-1 *
                                       (C78 / (C78 * C60) -
                                        -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                            (15.8687885 * std::sqrt(C60) * C78 *
                                             std::pow(std::sqrt(C60) + 0.325, 2))) *
                                       C55 * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2)) +
                              C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                  (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                           (15.8687885 * std::sqrt(C60) * C78 *
                                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                  C55) /
                             (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        C140 *
                            (C46 * C110 *
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 (0. * std::pow(C47 / C46, 4) +
                                  0.2658131939386538e-1 *
                                      ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                       2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                           (15.8687885 * std::pow(C78, 2) *
                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                      C55 * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2)) +
                             C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                 ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                      (15.8687885 * std::pow(C78, 2) *
                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                                 C55) /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C148 *
                           (C46 * C110 *
                                ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                (0. * std::pow(C47 / C46, 4) +
                                 0.2658131939386538e-1 *
                                     (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                      (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                          15.8687885) *
                                     C55 * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2)) +
                            C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                 (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                     15.8687885) *
                                C55) /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C117 *
                          (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               ((C46 *
                                     ((1.7099209341613653 *
                                           (0.01554535 *
                                                ((std::log(C60 / C78) +
                                                  0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                 -2.2946365 *
                                                     (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                      0.1282084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) /
                                                     15.8687885) -
                                            C111) *
                                           C55 / C110 -
                                       1.) *
                                          4 * std::pow(C47 / C46, 2) * -3 * C47 / std::pow(C46, 2) +
                                      0. * std::pow(C47 / C46, 3) / C110) -
                                 (1.7099209341613653 *
                                      (0.01554535 *
                                           ((std::log(C60 / C78) +
                                             0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                            -2.2946365 *
                                                (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                          C78) +
                                                 0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                                15.8687885) -
                                       C111) *
                                      C55 / C110 -
                                  1.) *
                                     4 * std::pow(C47 / C46, 3)) /
                                    std::pow(C46, 2) +
                                0. * std::pow(C47 / C46, 3) * -4 * C47 /
                                    (std::pow(C46, 2) * C110)) +
                           C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               ((1.7099209341613653 *
                                     (0.01554535 *
                                          ((std::log(C60 / C78) +
                                            0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                           -2.2946365 *
                                               (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                         C78) +
                                                0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                               15.8687885) -
                                      C111) *
                                     C55 / C110 -
                                 1.) *
                                    4 * std::pow(C47 / C46, 3) / C46 +
                                0. * std::pow(C47 / C46, 4) / C110)) /
                          (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                         C116) +
                    (((((((((C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 * C46 *
                                 (0. * C110 *
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                      std::pow(C47 / C46, 4)) -
                             C46 * C110 *
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                 std::pow(C47 / C46, 4) *
                                 (0. * C110 +
                                  3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                                std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55,
                                         2) +
                            C116 *
                                (C46 *
                                     (C110 *
                                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                           2.) *
                                          (std::pow(C47 / C46, 3) * -4 * C47 *
                                               (0. * C110 -
                                                1.7099209341613653 *
                                                    (0.01554535 *
                                                         ((std::log(C60 / C78) +
                                                           0.1412084e2 * C86 /
                                                               std::sqrt(0.223816694236e2)) -
                                                          -2.2946365 *
                                                              (std::log(
                                                                   std::pow(std::sqrt(C60) + 0.325,
                                                                            2) /
                                                                   C78) +
                                                               0.1282084e2 * C86 /
                                                                   std::sqrt(0.223816694236e2)) /
                                                              15.8687885) -
                                                     C111) *
                                                    C55) /
                                               (std::pow(C110, 2) * std::pow(C46, 2)) +
                                           (0. * C110 - 0. * std::pow(C47 / C46, 4)) /
                                               std::pow(C110, 2)) +
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                          ((1.7099209341613653 *
                                                (0.01554535 *
                                                     ((std::log(C60 / C78) +
                                                       0.1412084e2 * C86 /
                                                           std::sqrt(0.223816694236e2)) -
                                                      -2.2946365 *
                                                          (std::log(
                                                               std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                           0.1282084e2 * C86 /
                                                               std::sqrt(0.223816694236e2)) /
                                                          15.8687885) -
                                                 C111) *
                                                C55 / C110 -
                                            1.) *
                                               std::pow(C47 / C46, 3) * -4 * C47 /
                                               std::pow(C46, 2) +
                                           0. * std::pow(C47 / C46, 4) / C110)) +
                                 (C110 *
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                      std::pow(C47 / C46, 4) *
                                      (0. * C110 -
                                       1.7099209341613653 *
                                           (0.01554535 *
                                                ((std::log(C60 / C78) +
                                                  0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                 -2.2946365 *
                                                     (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                      0.1282084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) /
                                                     15.8687885) -
                                            C111) *
                                           C55) /
                                      std::pow(C110, 2) +
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                      ((1.7099209341613653 *
                                            (0.01554535 *
                                                 ((std::log(C60 / C78) +
                                                   0.1412084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) -
                                                  -2.2946365 *
                                                      (std::log(
                                                           std::pow(std::sqrt(C60) + 0.325, 2) /
                                                           C78) +
                                                       0.1282084e2 * C86 /
                                                           std::sqrt(0.223816694236e2)) /
                                                      15.8687885) -
                                             C111) *
                                            C55 / C110 -
                                        1.) *
                                           std::pow(C47 / C46, 4) +
                                       1.))) /
                                (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                           C173 * C46 *
                               (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 *
                                    (C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         0. * C55 +
                                     ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                         std::pow(C47 / C46, 4) * -1.7099209341613653 * C55) -
                                C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                    std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 *
                                    (0. * C110 +
                                     3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                               std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55,
                                        2)) +
                          C172 * C46 *
                              ((std::pow(C110, 2) *
                                    (C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         0. * std::pow(C47 / C46, 4) +
                                     ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                         std::pow(C47 / C46, 4) *
                                         (0. * C110 -
                                          1.7099209341613653 *
                                              (0.01554535 *
                                                   ((std::log(C60 / C78) +
                                                     0.1412084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) -
                                                    -2.2946365 *
                                                        (std::log(
                                                             std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                         0.1282084e2 * C86 /
                                                             std::sqrt(0.223816694236e2)) /
                                                        15.8687885) -
                                               C111) *
                                              C55)) -
                                C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                    std::pow(C47 / C46, 4) *
                                    (0. * C110 -
                                     1.7099209341613653 *
                                         (0.01554535 *
                                              ((std::log(C60 / C78) +
                                                0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                               -2.2946365 *
                                                   (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                    0.1282084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) /
                                                   15.8687885) -
                                          C111) *
                                         C55) *
                                    2 * C110) /
                                   std::pow(C110, 4) +
                               ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   std::pow(C47 / C46, 4) *
                                   (0. * C110 -
                                    1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55) /
                                   std::pow(C110, 2)) /
                              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C121 * C46 *
                             ((C110 *
                                   (0. * C110 *
                                        ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                    ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                        0. * std::pow(C47 / C46, 4)) -
                               C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   0. * std::pow(C47 / C46, 4)) /
                                  std::pow(C110, 2) +
                              (C110 *
                                   (C49 * std::pow(C58 + 1., C49 - 1) -
                                    C49 * std::pow(1. - C58, C49 - 1)) *
                                   std::pow(C47 / C46, 4) *
                                   (0. * C110 -
                                    1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55) /
                                   std::pow(C110, 2) +
                               (C49 * std::pow(C58 + 1., C49 - 1) -
                                C49 * std::pow(1. - C58, C49 - 1)) *
                                   ((1.7099209341613653 *
                                         (0.01554535 *
                                              ((std::log(C60 / C78) +
                                                0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                               -2.2946365 *
                                                   (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                    0.1282084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) /
                                                   15.8687885) -
                                          C111) *
                                         C55 / C110 -
                                     1.) *
                                        std::pow(C47 / C46, 4) +
                                    1.))) /
                             (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 * C123 * C46 *
                             (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  (0. * std::pow(C47 / C46, 4) +
                                   0. *
                                       (C78 / (C78 * C60) -
                                        -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                            (15.8687885 * std::sqrt(C60) * C78 *
                                             std::pow(std::sqrt(C60) + 0.325, 2))) *
                                       C55) +
                              ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                  (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                           (15.8687885 * std::sqrt(C60) * C78 *
                                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                  C55) -
                         C123 * C46 * C110 *
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                             (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                      (15.8687885 * std::sqrt(C60) * C78 *
                                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                             C55 *
                             (0. * C110 + 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                            std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55,
                                     2)) +
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 * C140 * C46 *
                            (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 (0. * std::pow(C47 / C46, 4) +
                                  0. *
                                      ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                       2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                           (15.8687885 * std::pow(C78, 2) *
                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                      C55) +
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                 ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                      (15.8687885 * std::pow(C78, 2) *
                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                                 C55) -
                        C140 * C46 * C110 *
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                            ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                             2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                 (15.8687885 * std::pow(C78, 2) *
                                  std::pow(std::sqrt(C60) + 0.325, 2))) *
                            C55 *
                            (0. * C110 + 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                           std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55,
                                    2)) +
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 * C148 * C46 *
                           (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                (0. * std::pow(C47 / C46, 4) +
                                 0. *
                                     (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                      (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                          15.8687885) *
                                     C55) +
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                 (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                     15.8687885) *
                                C55) -
                       C148 * C46 * C110 *
                           ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                           std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                           (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                            (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                           C55 *
                           (0. * C110 + 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                          std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55, 2)) +
                     C117 * C46 *
                         (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              (4 * std::pow(C47 / C46, 3) *
                                   (0. * C110 -
                                    1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55) /
                                   (std::pow(C110, 2) * C46) +
                               (0. * C110 - 0. * std::pow(C47 / C46, 4)) / std::pow(C110, 2)) +
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              ((1.7099209341613653 *
                                    (0.01554535 *
                                         ((std::log(C60 / C78) +
                                           0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                          -2.2946365 *
                                              (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                               0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                              15.8687885) -
                                     C111) *
                                    C55 / C110 -
                                1.) *
                                   4 * std::pow(C47 / C46, 3) / C46 +
                               0. * std::pow(C47 / C46, 4) / C110)) /
                         (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                        C172) +
                   ((((((((C46 *
                               (0. * C110 *
                                    ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                C110 *
                                    (C49 * std::pow(C58 + 1., C49 - 1) -
                                     C49 * std::pow(1. - C58, C49 - 1)) *
                                    0. * std::pow(C47 / C46, 4)) /
                               (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                           C116 *
                               (C46 *
                                    (C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         (0. * std::pow(C47 / C46, 3) * -4 * C47 /
                                              (C110 * std::pow(C46, 2)) +
                                          0. / C110) +
                                     C110 *
                                         (C49 * std::pow(C58 + 1., C49 - 1) -
                                          C49 * std::pow(1. - C58, C49 - 1)) *
                                         ((1.7099209341613653 *
                                               (0.01554535 *
                                                    ((std::log(C60 / C78) +
                                                      0.1412084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) -
                                                     -2.2946365 *
                                                         (std::log(
                                                              std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                          0.1282084e2 * C86 /
                                                              std::sqrt(0.223816694236e2)) /
                                                         15.8687885) -
                                                C111) *
                                               C55 / C110 -
                                           1.) *
                                              std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                          0. * std::pow(C47 / C46, 4) / C110)) +
                                (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                     0. * std::pow(C47 / C46, 4) / C110 +
                                 C110 *
                                     (C49 * std::pow(C58 + 1., C49 - 1) -
                                      C49 * std::pow(1. - C58, C49 - 1)) *
                                     ((1.7099209341613653 *
                                           (0.01554535 *
                                                ((std::log(C60 / C78) +
                                                  0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                 -2.2946365 *
                                                     (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                      0.1282084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) /
                                                     15.8687885) -
                                            C111) *
                                           C55 / C110 -
                                       1.) *
                                          std::pow(C47 / C46, 4) +
                                      1.))) /
                               (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                          C173 * C46 *
                              (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   0. * C55 +
                               C110 *
                                   (C49 * std::pow(C58 + 1., C49 - 1) -
                                    C49 * std::pow(1. - C58, C49 - 1)) *
                                   std::pow(C47 / C46, 4) * -1.7099209341613653 * C55) /
                              (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C172 * C46 *
                             ((C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   0. * std::pow(C47 / C46, 4) +
                               C110 *
                                   (C49 * std::pow(C58 + 1., C49 - 1) -
                                    C49 * std::pow(1. - C58, C49 - 1)) *
                                   std::pow(C47 / C46, 4) *
                                   (0. * C110 -
                                    1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55)) /
                                  std::pow(C110, 2) +
                              (((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                   std::pow(C47 / C46, 4) / C110 +
                               (C49 * std::pow(C58 + 1., C49 - 1) -
                                C49 * std::pow(1. - C58, C49 - 1)) *
                                   ((1.7099209341613653 *
                                         (0.01554535 *
                                              ((std::log(C60 / C78) +
                                                0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                               -2.2946365 *
                                                   (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                    0.1282084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) /
                                                   15.8687885) -
                                          C111) *
                                         C55 / C110 -
                                     1.) *
                                        std::pow(C47 / C46, 4) +
                                    1.))) /
                             (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        C121 * C46 *
                            ((0. * C110 *
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                              C110 *
                                  (C49 * std::pow(C58 + 1., C49 - 1) -
                                   C49 * std::pow(1. - C58, C49 - 1)) *
                                  0. * std::pow(C47 / C46, 4)) /
                                 C110 +
                             (C110 *
                                  (C49 * std::pow(C58 + 1., C49 - 1) -
                                   C49 * std::pow(1. - C58, C49 - 1)) *
                                  0. * std::pow(C47 / C46, 4) / C110 +
                              C110 *
                                  (C49 * (C49 - 1) * std::pow(C58 + 1., C49 - 2) -
                                   C49 * (1 - C49) * std::pow(1. - C58, C49 - 2)) *
                                  ((1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55 / C110 -
                                    1.) *
                                       std::pow(C47 / C46, 4) +
                                   1.))) /
                            (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C123 * C46 *
                           (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                (0. * std::pow(C47 / C46, 4) +
                                 0. *
                                     (C78 / (C78 * C60) -
                                      -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                          (15.8687885 * std::sqrt(C60) * C78 *
                                           std::pow(std::sqrt(C60) + 0.325, 2))) *
                                     C55) +
                            C110 *
                                (C49 * std::pow(C58 + 1., C49 - 1) -
                                 C49 * std::pow(1. - C58, C49 - 1)) *
                                std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                         (15.8687885 * std::sqrt(C60) * C78 *
                                                          std::pow(std::sqrt(C60) + 0.325, 2))) *
                                C55) /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C140 * C46 *
                          (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               (0. * std::pow(C47 / C46, 4) +
                                0. *
                                    ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                     2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                         (15.8687885 * std::pow(C78, 2) *
                                          std::pow(std::sqrt(C60) + 0.325, 2))) *
                                    C55) +
                           C110 *
                               (C49 * std::pow(C58 + 1., C49 - 1) -
                                C49 * std::pow(1. - C58, C49 - 1)) *
                               std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                               ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                    (15.8687885 * std::pow(C78, 2) *
                                     std::pow(std::sqrt(C60) + 0.325, 2))) *
                               C55) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C148 * C46 *
                         (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              (0. * std::pow(C47 / C46, 4) +
                               0. *
                                   (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                    (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                        15.8687885) *
                                   C55) +
                          C110 *
                              (C49 * std::pow(C58 + 1., C49 - 1) -
                               C49 * std::pow(1. - C58, C49 - 1)) *
                              std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                               (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                              C55) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C117 * C46 *
                        (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) +
                         C110 *
                             (C49 * std::pow(C58 + 1., C49 - 1) -
                              C49 * std::pow(1. - C58, C49 - 1)) *
                             ((1.7099209341613653 *
                                   (0.01554535 *
                                        ((std::log(C60 / C78) +
                                          0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                         -2.2946365 *
                                             (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                              0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                             15.8687885) -
                                    C111) *
                                   C55 / C110 -
                               1.) *
                                  4 * std::pow(C47 / C46, 3) / C46 +
                              0. * std::pow(C47 / C46, 4) / C110)) /
                        (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                       C121) +
                  ((((((((C46 *
                              (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   0. * std::pow(C47 / C46, 3) / C46 +
                               0. * std::pow(C47 / C46, 4)) /
                              (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                          C116 *
                              (C46 * C110 *
                                   ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   (((1.7099209341613653 *
                                          (0.01554535 *
                                               ((std::log(C60 / C78) +
                                                 0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                -2.2946365 *
                                                    (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                     0.1282084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) /
                                                    15.8687885) -
                                           C111) *
                                          C55 / C110 -
                                      1.) *
                                         (-4 * std::pow(C47 / C46, 3) +
                                          -4 * C47 * 3 * std::pow(C47 / C46, 2) / C46) +
                                     0. * std::pow(C47 / C46, 3) * -4 * C47 / C110) /
                                        std::pow(C46, 2) +
                                    0. * std::pow(C47 / C46, 3) / (C46 * C110)) +
                               C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   ((1.7099209341613653 *
                                         (0.01554535 *
                                              ((std::log(C60 / C78) +
                                                0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                               -2.2946365 *
                                                   (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                    0.1282084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) /
                                                   15.8687885) -
                                          C111) *
                                         C55 / C110 -
                                     1.) *
                                        4 * std::pow(C47 / C46, 3) / C46 +
                                    0. * std::pow(C47 / C46, 4) / C110)) /
                              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C173 * C46 * C110 *
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             -1.7099209341613653 * C55 * 4 * std::pow(C47 / C46, 3) /
                             (C46 * C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        C172 * C46 *
                            (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 (0. * std::pow(C47 / C46, 4) +
                                  (0. * C110 -
                                   1.7099209341613653 *
                                       (0.01554535 *
                                            ((std::log(C60 / C78) +
                                              0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                             -2.2946365 *
                                                 (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                           C78) +
                                                  0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                                 15.8687885) -
                                        C111) *
                                       C55) *
                                      4 * std::pow(C47 / C46, 3) / C46) /
                                 std::pow(C110, 2) +
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 ((1.7099209341613653 *
                                       (0.01554535 *
                                            ((std::log(C60 / C78) +
                                              0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                             -2.2946365 *
                                                 (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                           C78) +
                                                  0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                                 15.8687885) -
                                        C111) *
                                       C55 / C110 -
                                   1.) *
                                      4 * std::pow(C47 / C46, 3) / C46 +
                                  0. * std::pow(C47 / C46, 4) / C110)) /
                            (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C121 * C46 *
                           ((C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 0. * std::pow(C47 / C46, 3) / C46 +
                             0. * std::pow(C47 / C46, 4)) /
                                C110 +
                            C110 *
                                (C49 * std::pow(C58 + 1., C49 - 1) -
                                 C49 * std::pow(1. - C58, C49 - 1)) *
                                ((1.7099209341613653 *
                                      (0.01554535 *
                                           ((std::log(C60 / C78) +
                                             0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                            -2.2946365 *
                                                (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                          C78) +
                                                 0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                                15.8687885) -
                                       C111) *
                                      C55 / C110 -
                                  1.) *
                                     4 * std::pow(C47 / C46, 3) / C46 +
                                 0. * std::pow(C47 / C46, 4) / C110)) /
                           (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C123 * C46 * C110 *
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                          (0. * std::pow(C47 / C46, 4) +
                           0.2658131939386538e-1 *
                               (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                        (15.8687885 * std::sqrt(C60) * C78 *
                                                         std::pow(std::sqrt(C60) + 0.325, 2))) *
                               C55 * 4 * std::pow(C47 / C46, 3) / C46) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C140 * C46 * C110 *
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                         (0. * std::pow(C47 / C46, 4) +
                          0.2658131939386538e-1 *
                              ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                               2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                   (15.8687885 * std::pow(C78, 2) *
                                    std::pow(std::sqrt(C60) + 0.325, 2))) *
                              C55 * 4 * std::pow(C47 / C46, 3) / C46) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                        (0. * std::pow(C47 / C46, 4) +
                         0.2658131939386538e-1 *
                             (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                              (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                             C55 * 4 * std::pow(C47 / C46, 3) / C46) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (((1.7099209341613653 *
                              (0.01554535 *
                                   ((std::log(C60 / C78) +
                                     0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                    -2.2946365 *
                                        (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                         0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                        15.8687885) -
                               C111) *
                              C55 / C110 -
                          1.) *
                             12 * std::pow(C47 / C46, 2) / C46 +
                         0. * std::pow(C47 / C46, 3) / C110) /
                            C46 +
                        0. * std::pow(C47 / C46, 3) / (C46 * C110)) /
                       (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                      C117) +
                 ((((((((C46 *
                             (0. * C110 *
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                              0. * std::pow(C47 / C46, 4)) /
                             (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                         (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              (std::pow(C47 / C46, 3) * -4 * C47 * 0.2658131939386538e-1 *
                                   (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) *
                                                            C78 /
                                                            (15.8687885 * std::sqrt(C60) * C78 *
                                                             std::pow(std::sqrt(C60) + 0.325, 2))) *
                                   C55 / (C110 * std::pow(C46, 2)) +
                               0. / C110) /
                              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                          C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                       (15.8687885 * std::sqrt(C60) * C78 *
                                                        std::pow(std::sqrt(C60) + 0.325, 2))) *
                              C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                             C116) +
                        C173 * C46 * C110 *
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C172 * C46 *
                           (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) *
                                (0. - 0.2658131939386538e-1 *
                                          (C78 / (C78 * C60) -
                                           -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                               (15.8687885 * std::sqrt(C60) * C78 *
                                                std::pow(std::sqrt(C60) + 0.325, 2))) *
                                          C55) /
                                std::pow(C110, 2) +
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                         (15.8687885 * std::sqrt(C60) * C78 *
                                                          std::pow(std::sqrt(C60) + 0.325, 2))) *
                                C55 / C110) /
                           (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C121 * C46 *
                          ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                            0. * std::pow(C47 / C46, 4)) +
                           C110 *
                               (C49 * std::pow(C58 + 1., C49 - 1) -
                                C49 * std::pow(1. - C58, C49 - 1)) *
                               std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                               (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                        (15.8687885 * std::sqrt(C60) * C78 *
                                                         std::pow(std::sqrt(C60) + 0.325, 2))) *
                               C55) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C123 * C46 * C110 *
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                         (std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              ((-std::pow(C78, 2)) / std::pow(C78 * C60, 2) -
                               (15.8687885 * std::sqrt(C60) * C78 *
                                    std::pow(std::sqrt(C60) + 0.325, 2) * -2.2946365 * C78 /
                                    (2 * std::sqrt(C60)) -
                                -2.2946365 * (std::sqrt(C60) + 0.325) * C78 * 15.8687885 *
                                    (std::sqrt(C60) * C78 * (std::sqrt(C60) + 0.325) /
                                         std::sqrt(C60) +
                                     std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                         (2 * std::sqrt(C60)))) /
                                   std::pow(15.8687885 * std::sqrt(C60) * C78 *
                                                std::pow(std::sqrt(C60) + 0.325, 2),
                                            2)) *
                              C55 +
                          0. *
                              (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                       (15.8687885 * std::sqrt(C60) * C78 *
                                                        std::pow(std::sqrt(C60) + 0.325, 2))) *
                              C55) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                        (std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                             ((C60 * std::pow(C78, 3) - std::pow(C78, 2) * C60 * C78) /
                                  std::pow(std::pow(C78, 2) * C60, 2) -
                              (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2) *
                                   2.2946365 * C78 * (std::sqrt(C60) + 0.325) / std::sqrt(C60) -
                               2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 * 15.8687885 *
                                   std::pow(C78, 2) * (std::sqrt(C60) + 0.325) / std::sqrt(C60)) /
                                  std::pow(15.8687885 * std::pow(C78, 2) *
                                               std::pow(std::sqrt(C60) + 0.325, 2),
                                           2)) *
                             C55 +
                         0. *
                             ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                              2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                  (15.8687885 * std::pow(C78, 2) *
                                   std::pow(std::sqrt(C60) + 0.325, 2))) *
                             C55) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (0. * std::pow(C47 / C46, 4) +
                        0. *
                            (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                             (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                            C55) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (4 * std::pow(C47 / C46, 3) * 0.2658131939386538e-1 *
                           (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                    (15.8687885 * std::sqrt(C60) * C78 *
                                                     std::pow(std::sqrt(C60) + 0.325, 2))) *
                           C55 / (C110 * C46) +
                       0. / C110) /
                      (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                     C123) +
                ((((((((C46 *
                            (0. * C110 *
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                             0. * std::pow(C47 / C46, 4)) /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                        (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             (std::pow(C47 / C46, 3) * -4 * C47 * 0.2658131939386538e-1 *
                                  ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                   2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                       (15.8687885 * std::pow(C78, 2) *
                                        std::pow(std::sqrt(C60) + 0.325, 2))) *
                                  C55 / (C110 * std::pow(C46, 2)) +
                              0. / C110) /
                             (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                         C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                             ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                              2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                  (15.8687885 * std::pow(C78, 2) *
                                   std::pow(std::sqrt(C60) + 0.325, 2))) *
                             C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                            C116) +
                       C173 * C46 * C110 *
                           ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C172 * C46 *
                          (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               std::pow(C47 / C46, 4) *
                               (0. - 0.2658131939386538e-1 *
                                         ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                          2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                              (15.8687885 * std::pow(C78, 2) *
                                               std::pow(std::sqrt(C60) + 0.325, 2))) *
                                         C55) /
                               std::pow(C110, 2) +
                           ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                               ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                    (15.8687885 * std::pow(C78, 2) *
                                     std::pow(std::sqrt(C60) + 0.325, 2))) *
                               C55 / C110) /
                          (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C121 * C46 *
                         ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                           0. * std::pow(C47 / C46, 4)) +
                          C110 *
                              (C49 * std::pow(C58 + 1., C49 - 1) -
                               C49 * std::pow(1. - C58, C49 - 1)) *
                              std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                               2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                   (15.8687885 * std::pow(C78, 2) *
                                    std::pow(std::sqrt(C60) + 0.325, 2))) *
                              C55) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                        (std::pow(C47 / C46, 4) * -0.2658131939386538e-1 * C55 *
                             (15.8687885 * std::sqrt(C60) * C78 *
                                  std::pow(std::sqrt(C60) + 0.325, 2) * -2.2946365 *
                                  (std::sqrt(C60) + 0.325) -
                              -2.2946365 * (std::sqrt(C60) + 0.325) * C78 * 15.8687885 *
                                  std::sqrt(C60) * std::pow(std::sqrt(C60) + 0.325, 2)) /
                             std::pow(15.8687885 * std::sqrt(C60) * C78 *
                                          std::pow(std::sqrt(C60) + 0.325, 2),
                                      2) +
                         0. *
                             (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                      (15.8687885 * std::sqrt(C60) * C78 *
                                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                             C55) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                            ((C60 * C78 * 2 * C78 * C60 - std::pow(C78, 2) * std::pow(C60, 2)) /
                                 std::pow(std::pow(C78, 2) * C60, 2) -
                             (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2) *
                                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) -
                              2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 * 0.31737577e2 *
                                  C78 * std::pow(std::sqrt(C60) + 0.325, 2)) /
                                 std::pow(15.8687885 * std::pow(C78, 2) *
                                              std::pow(std::sqrt(C60) + 0.325, 2),
                                          2)) *
                            C55 +
                        0. *
                            ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                             2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                 (15.8687885 * std::pow(C78, 2) *
                                  std::pow(std::sqrt(C60) + 0.325, 2))) *
                            C55) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (0. * std::pow(C47 / C46, 4) +
                       0. *
                           (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                            (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                           C55) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (4 * std::pow(C47 / C46, 3) * 0.2658131939386538e-1 *
                          ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                           2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                               (15.8687885 * std::pow(C78, 2) *
                                std::pow(std::sqrt(C60) + 0.325, 2))) *
                          C55 / (C110 * C46) +
                      0. / C110) /
                     (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                    C140) +
               ((((((((C46 *
                           (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                            0. * std::pow(C47 / C46, 4)) /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                       (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            (std::pow(C47 / C46, 3) * -4 * C47 * 0.2658131939386538e-1 *
                                 (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                  (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                      15.8687885) *
                                 C55 / (C110 * std::pow(C46, 2)) +
                             0. / C110) /
                            (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                        C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                            (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                             (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                            C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                           C116) +
                      C173 * C46 * C110 *
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C172 * C46 *
                         (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              std::pow(C47 / C46, 4) *
                              (0. - 0.2658131939386538e-1 *
                                        (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                         (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                             15.8687885) *
                                        C55) /
                              std::pow(C110, 2) +
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                               (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                              C55 / C110) /
                         (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C121 * C46 *
                        ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                          0. * std::pow(C47 / C46, 4)) +
                         C110 *
                             (C49 * std::pow(C58 + 1., C49 - 1) -
                              C49 * std::pow(1. - C58, C49 - 1)) *
                             std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                             (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                              (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                             C55) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (0. * std::pow(C47 / C46, 4) +
                        0. *
                            (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                     (15.8687885 * std::sqrt(C60) * C78 *
                                                      std::pow(std::sqrt(C60) + 0.325, 2))) *
                            C55) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (0. * std::pow(C47 / C46, 4) +
                       0. *
                           ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                            2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                (15.8687885 * std::pow(C78, 2) *
                                 std::pow(std::sqrt(C60) + 0.325, 2))) *
                           C55) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (0. * std::pow(C47 / C46, 4) +
                      0. *
                          (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                           (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                          C55) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    (4 * std::pow(C47 / C46, 3) * 0.2658131939386538e-1 *
                         (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                          (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                         C55 / (C110 * C46) +
                     0. / C110) /
                    (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                   C148) +
              ((((((((C46 *
                          (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                           0. * std::pow(C47 / C46, 4)) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                      (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                           (std::pow(C47 / C46, 3) * -4 * C47 * -1.7099209341613653 * C55 /
                                (C110 * std::pow(C46, 2)) +
                            0. / C110) /
                           (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                       (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                        1)) *
                          C116) +
                     C173 * C46 * C110 *
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C172 * C46 *
                        (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             std::pow(C47 / C46, 4) * (0. - -1.7099209341613653 * C55) /
                             std::pow(C110, 2) +
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 / C110) /
                        (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C121 * C46 *
                       ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                         0. * std::pow(C47 / C46, 4)) +
                        C110 *
                            (C49 * std::pow(C58 + 1., C49 - 1) -
                             C49 * std::pow(1. - C58, C49 - 1)) *
                            std::pow(C47 / C46, 4) * -1.7099209341613653 * C55) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (0. * std::pow(C47 / C46, 4) +
                       0. *
                           (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                    (15.8687885 * std::sqrt(C60) * C78 *
                                                     std::pow(std::sqrt(C60) + 0.325, 2))) *
                           C55) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (0. * std::pow(C47 / C46, 4) +
                      0. *
                          ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                           2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                               (15.8687885 * std::pow(C78, 2) *
                                std::pow(std::sqrt(C60) + 0.325, 2))) *
                          C55) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    (0. * std::pow(C47 / C46, 4) +
                     0. *
                         (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                          (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                         C55) /
                    (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (4 * std::pow(C47 / C46, 3) * -1.7099209341613653 * C55 / (C110 * C46) +
                    0. / C110) /
                   (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                  C173) +
             ((((((((C46 *
                         (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                          0. * std::pow(C47 / C46, 4)) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                     (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                          (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                           0. / C110) /
                          (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                      C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                          std::pow(C47 / C46, 4) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                         C116) +
                    (C46 * (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 /
                                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                            1) +
                     C173 * C46 * C110 *
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55))) +
                   C172 * C46 *
                       (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                            std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                        ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                            std::pow(C47 / C46, 4) / C110) /
                       (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C121 * C46 *
                      ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                        0. * std::pow(C47 / C46, 4)) +
                       C110 *
                           (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                           0. * std::pow(C47 / C46, 4)) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (0. * std::pow(C47 / C46, 4) +
                      0. *
                          (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                   (15.8687885 * std::sqrt(C60) * C78 *
                                                    std::pow(std::sqrt(C60) + 0.325, 2))) *
                          C55) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    (0. * std::pow(C47 / C46, 4) +
                     0. *
                         ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                          2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                              (15.8687885 * std::pow(C78, 2) *
                               std::pow(std::sqrt(C60) + 0.325, 2))) *
                         C55) /
                    (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (0. * std::pow(C47 / C46, 4) +
                    0. *
                        (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                         (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                        C55) /
                   (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                  (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
                  (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                 C285) +
            ((((((((C46 *
                        (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                         0. * std::pow(C47 / C46, 4)) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                    (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                         (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                          0. / C110) /
                         (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                     C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                         std::pow(C47 / C46, 4) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                        C116) +
                   C173 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       0. * C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  (C172 * C46 *
                       (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                            std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                        ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                            std::pow(C47 / C46, 4) / C110) +
                   C46 * (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              std::pow(C47 / C46, 4) *
                              (0. * C110 -
                               1.7099209341613653 *
                                   (0.01554535 *
                                        ((std::log(C60 / C78) +
                                          0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                         -2.2946365 *
                                             (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                              0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                             15.8687885) -
                                    C111) *
                                   C55) /
                              std::pow(C110, 2) +
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              ((1.7099209341613653 *
                                    (0.01554535 *
                                         ((std::log(C60 / C78) +
                                           0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                          -2.2946365 *
                                              (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                               0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                              15.8687885) -
                                     C111) *
                                    C55 / C110 -
                                1.) *
                                   std::pow(C47 / C46, 4) +
                               1.))) /
                      (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C121 * C46 *
                     ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                       0. * std::pow(C47 / C46, 4)) +
                      C110 *
                          (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                          0. * std::pow(C47 / C46, 4)) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    (0. * std::pow(C47 / C46, 4) +
                     0. *
                         (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                  (15.8687885 * std::sqrt(C60) * C78 *
                                                   std::pow(std::sqrt(C60) + 0.325, 2))) *
                         C55) /
                    (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (0. * std::pow(C47 / C46, 4) +
                    0. *
                        ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                         2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                             (15.8687885 * std::pow(C78, 2) *
                              std::pow(std::sqrt(C60) + 0.325, 2))) *
                        C55) /
                   (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                  (0. * std::pow(C47 / C46, 4) +
                   0. *
                       (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                        (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                       C55) /
                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
                 (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                C284) +
           ((((((((C46 *
                       (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                        0. * std::pow(C47 / C46, 4)) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                   (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                        (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                         0. / C110) /
                        (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                    C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                        std::pow(C47 / C46, 4) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                       C116) +
                  C173 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      0. * C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C172 * C46 *
                     (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                          std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                          std::pow(C47 / C46, 4) / C110) /
                     (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                (C121 * C46 *
                     ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                       0. * std::pow(C47 / C46, 4)) +
                      C110 *
                          (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                          0. * std::pow(C47 / C46, 4)) /
                     C110 +
                 C46 *
                     (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                          std::pow(C47 / C46, 4) / C110 +
                      C110 *
                          (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                          ((1.7099209341613653 *
                                (0.01554535 *
                                     ((std::log(C60 / C78) +
                                       0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                      -2.2946365 *
                                          (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                           0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                          15.8687885) -
                                 C111) *
                                C55 / C110 -
                            1.) *
                               std::pow(C47 / C46, 4) +
                           1.))) /
                    (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (0. * std::pow(C47 / C46, 4) +
                    0. *
                        (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                 (15.8687885 * std::sqrt(C60) * C78 *
                                                  std::pow(std::sqrt(C60) + 0.325, 2))) *
                        C55) /
                   (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                  (0. * std::pow(C47 / C46, 4) +
                   0. *
                       ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                        2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                            (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                       C55) /
                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 4) +
                  0. *
                      (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                       (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                      C55) /
                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
            C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
                (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
               C233) +
          ((((((((C46 *
                      (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                       0. * std::pow(C47 / C46, 4)) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                  (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                        0. / C110) /
                       (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                   C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                       std::pow(C47 / C46, 4) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                      C116) +
                 C173 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     0. * C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C172 * C46 *
                    (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                         std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                     ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                         std::pow(C47 / C46, 4) / C110) /
                    (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C121 * C46 *
                   ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                     0. * std::pow(C47 / C46, 4)) +
                    C110 * (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                        0. * std::pow(C47 / C46, 4)) /
                   (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              (C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (0. * std::pow(C47 / C46, 4) +
                    0. *
                        (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                 (15.8687885 * std::sqrt(C60) * C78 *
                                                  std::pow(std::sqrt(C60) + 0.325, 2))) *
                        C55) +
               C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                   (C78 / (C78 * C60) -
                    -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                        (15.8687885 * std::sqrt(C60) * C78 * std::pow(std::sqrt(C60) + 0.325, 2))) *
                   C55) /
                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 4) +
                  0. *
                      ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                       2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                           (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                      C55) /
                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
            C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                (0. * std::pow(C47 / C46, 4) +
                 0. *
                     (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                      (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                     C55) /
                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
           C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
               (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
              C235) +
         ((((((((C46 *
                     (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                      0. * std::pow(C47 / C46, 4)) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                 (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                       0. / C110) /
                      (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                  C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                      std::pow(C47 / C46, 4) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                     C116) +
                C173 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    0. * C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C172 * C46 *
                   (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                        std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                    ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                        std::pow(C47 / C46, 4) / C110) /
                   (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C121 * C46 *
                  ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                    0. * std::pow(C47 / C46, 4)) +
                   C110 * (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                       0. * std::pow(C47 / C46, 4)) /
                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 4) +
                  0. *
                      (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                               (15.8687885 * std::sqrt(C60) * C78 *
                                                std::pow(std::sqrt(C60) + 0.325, 2))) *
                      C55) /
                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
            (C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 4) +
                  0. *
                      ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                       2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                           (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                      C55) +
             C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                 ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                      (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                 C55) /
                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
           C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               (0. * std::pow(C47 / C46, 4) +
                0. *
                    (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                     (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                    C55) /
               (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
          C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
              (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
             C252) +
        ((((((((C46 *
                    (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                     0. * std::pow(C47 / C46, 4)) /
                    (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                      0. / C110) /
                     (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                 C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                     std::pow(C47 / C46, 4) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                    C116) +
               C173 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                   C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C172 * C46 *
                  (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                       std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                   ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                       std::pow(C47 / C46, 4) / C110) /
                  (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C121 * C46 *
                 ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                   0. * std::pow(C47 / C46, 4)) +
                  C110 * (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                      0. * std::pow(C47 / C46, 4)) /
                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
            C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                (0. * std::pow(C47 / C46, 4) +
                 0. *
                     (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                              (15.8687885 * std::sqrt(C60) * C78 *
                                               std::pow(std::sqrt(C60) + 0.325, 2))) *
                     C55) /
                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
           C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               (0. * std::pow(C47 / C46, 4) +
                0. *
                    ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                     2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                         (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                    C55) /
               (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
          (C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               (0. * std::pow(C47 / C46, 4) +
                0. *
                    (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                     (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                    C55) +
           C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
               (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
               C55) /
              (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
         C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
             (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
             (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
            C260;

    const double C174 = 1;
    const double C175 = -1;
    const double C176 = 0.9999999999999999 * C175 + 0.;
    const double C177 = Pi * C174;
    const double C178 = 4. * C177;
    const double C179 = C176 / C46 - C174 * C54 / std::pow(C46, 2);
    const double C180 = -3. * C178 / std::pow(C57, 2);
    const double C181 = C48 * std::pow(C59, C48 - 1) * C180;
    const double C182 = 1.13107 * C181 / (2 * std::sqrt(C60));
    const double C183 = 2. * C181 / (2 * std::sqrt(C60));
    const double C184 = 3.72744 * C181 / (2 * std::sqrt(C60));
    const double C185 = 7.06042 * C181 / (2 * std::sqrt(C60));
    const double C186 = C181 / (2 * std::sqrt(C60));
    const double C187 = C181 / (2 * std::sqrt(C60));
    const double C188 = C181 + C182;
    const double C189 = C181 + C184;
    const double C190 = C181 + C185;
    const double C191 = C183;
    const double C192 = C183;
    const double C193 = C183;
    const double C194 = 2 * C66 * C186;
    const double C195 = 2 * C67 * C187;
    const double C196 = C188;
    const double C197 = C189;
    const double C198 = C190;
    const double C199 = (-C193 * std::sqrt(0.223816694236e2)) / std::pow(C73, 2);
    const double C200 = (-C192 * std::sqrt(0.378469910464e2)) / std::pow(C72, 2);
    const double C201 = (-C191 * std::sqrt(0.507386806551e2)) / std::pow(C71, 2);
    const double C202 = C181 / C76 - C196 * C60 / std::pow(C76, 2);
    const double C203 = C181 / C77 - C197 * C60 / std::pow(C77, 2);
    const double C204 = C194 / C76 - C196 * C74 / std::pow(C76, 2);
    const double C205 = C195 / C77 - C197 * C75 / std::pow(C77, 2);
    const double C206 = C199 / (std::pow(C79, 2) + 1);
    const double C207 = C200 / (std::pow(C80, 2) + 1);
    const double C208 = C201 / (std::pow(C81, 2) + 1);
    const double C209 = 2.2431064 * C208;
    const double C210 = 2.26214 * C208;
    const double C211 = 7.03496 * C207;
    const double C212 = 7.45488 * C207;
    const double C213 = C202 / C82;
    const double C214 = C203 / C83;
    const double C215 = C204 / C84;
    const double C216 = C205 / C85;
    const double C217 = std::sqrt(1 / 0.507386806551e2) * C209;
    const double C218 = std::sqrt(1 / 0.507386806551e2) * C210;
    const double C219 = std::sqrt(1 / 0.378469910464e2) * C211;
    const double C220 = std::sqrt(1 / 0.378469910464e2) * C212;
    const double C221 = C213 + C218;
    const double C222 = C214 + C220;
    const double C223 = C215 + C217;
    const double C224 = C216 + C219;
    const double C225 = -0.3913066512 * C224;
    const double C226 = -0.005382083488 * C223;
    const double C227 = C225 / 12.5549141492;
    const double C228 = C226 / 12.99914055888256;
    const double C229 = C222 - C227;
    const double C230 = C228 - C221;
    const double C231 = 0.0310907 * C229;
    const double C286 = 0.;
    const double C287 =
        (C286 / C46 +
         ((-C118) / std::pow(C46, 2) - (-C116 * C54 * 2 * C46) / std::pow(C46, 4)) * C174) -
        C176 * C116 / std::pow(C46, 2);
    const double C288 = (-C178 * -3. * C120 * 2 * C57) / std::pow(C57, 4);
    const double C289 = C48 * (C48 - 1) * std::pow(C59, C48 - 2) * C122 * C180 +
                        C48 * std::pow(C59, C48 - 1) * C288;
    const double C290 = 1.13107 * C289 / (2 * std::sqrt(C60)) -
                        C181 * 1.13107 * C123 / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C291 = 2. * C289 / (2 * std::sqrt(C60)) -
                        C181 * 2. * C123 / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C292 = 3.72744 * C289 / (2 * std::sqrt(C60)) -
                        C181 * 3.72744 * C123 / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C293 = 7.06042 * C289 / (2 * std::sqrt(C60)) -
                        C181 * 7.06042 * C123 / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C294 = C289 / (2 * std::sqrt(C60)) -
                        C181 * C123 / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C295 = C289 / (2 * std::sqrt(C60)) -
                        C181 * C123 / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C296 = C289 + C290;
    const double C297 = C289 + C292;
    const double C298 = C289 + C293;
    const double C299 = C291;
    const double C300 = C291;
    const double C301 = C291;
    const double C302 = 2 * C128 * C186 + 2 * C66 * C294;
    const double C303 = 2 * C129 * C187 + 2 * C67 * C295;
    const double C304 = C296;
    const double C305 = C297;
    const double C306 = C298;
    const double C307 = C193 * C135 * std::sqrt(0.223816694236e2) * 2 * C73 / std::pow(C73, 4) -
                        C301 * std::sqrt(0.223816694236e2) / std::pow(C73, 2);
    const double C308 = C192 * C134 * std::sqrt(0.378469910464e2) * 2 * C72 / std::pow(C72, 4) -
                        C300 * std::sqrt(0.378469910464e2) / std::pow(C72, 2);
    const double C309 = C191 * C133 * std::sqrt(0.507386806551e2) * 2 * C71 / std::pow(C71, 4) -
                        C299 * std::sqrt(0.507386806551e2) / std::pow(C71, 2);
    const double C310 =
        ((C289 / C76 +
          ((-C123) / std::pow(C76, 2) - (-C138 * C60 * 2 * C76) / std::pow(C76, 4)) * C196) -
         C304 * C60 / std::pow(C76, 2)) -
        C181 * C138 / std::pow(C76, 2);
    const double C311 =
        ((C289 / C77 +
          ((-C123) / std::pow(C77, 2) - (-C139 * C60 * 2 * C77) / std::pow(C77, 4)) * C197) -
         C305 * C60 / std::pow(C77, 2)) -
        C181 * C139 / std::pow(C77, 2);
    const double C312 =
        ((C302 / C76 +
          ((-C136) / std::pow(C76, 2) - (-C138 * C74 * 2 * C76) / std::pow(C76, 4)) * C196) -
         C304 * C74 / std::pow(C76, 2)) -
        C194 * C138 / std::pow(C76, 2);
    const double C313 =
        ((C303 / C77 +
          ((-C137) / std::pow(C77, 2) - (-C139 * C75 * 2 * C77) / std::pow(C77, 4)) * C197) -
         C305 * C75 / std::pow(C77, 2)) -
        C195 * C139 / std::pow(C77, 2);
    const double C314 =
        C307 / (std::pow(C79, 2) + 1) - C199 * C141 * 2 * C79 / std::pow(std::pow(C79, 2) + 1, 2);
    const double C315 =
        C308 / (std::pow(C80, 2) + 1) - C200 * C142 * 2 * C80 / std::pow(std::pow(C80, 2) + 1, 2);
    const double C316 =
        C309 / (std::pow(C81, 2) + 1) - C201 * C143 * 2 * C81 / std::pow(std::pow(C81, 2) + 1, 2);
    const double C317 = 2.2431064 * C316;
    const double C318 = 2.26214 * C316;
    const double C319 = 7.03496 * C315;
    const double C320 = 7.45488 * C315;
    const double C321 = C310 / C82 - C202 * C144 / std::pow(C82, 2);
    const double C322 = C311 / C83 - C203 * C145 / std::pow(C83, 2);
    const double C323 = C312 / C84 - C204 * C146 / std::pow(C84, 2);
    const double C324 = C313 / C85 - C205 * C147 / std::pow(C85, 2);
    const double C325 = std::sqrt(1 / 0.507386806551e2) * C317;
    const double C326 = std::sqrt(1 / 0.507386806551e2) * C318;
    const double C327 = std::sqrt(1 / 0.378469910464e2) * C319;
    const double C328 = std::sqrt(1 / 0.378469910464e2) * C320;
    const double C329 = C321 + C326;
    const double C330 = C322 + C328;
    const double C331 = C323 + C325;
    const double C332 = C324 + C327;
    const double C333 = -0.3913066512 * C332;
    const double C334 = -0.005382083488 * C331;
    const double C335 = C333 / 12.5549141492;
    const double C336 = C334 / 12.99914055888256;
    const double C337 = C330 - C335;
    const double C338 = C336 - C329;
    const double C339 = 0.0310907 * C337;
    matrix.upper[1] =
        (((((((((((((((((((((C46 *
                                 (0. * C110 *
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                  0. * std::pow(C47 / C46, 4)) /
                                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                             (C46 * C110 *
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  (0. * std::pow(C47 / C46, 3) * -4 * C47 /
                                       (C110 * std::pow(C46, 2)) +
                                   0. / C110) /
                                  (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                              C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  0. * std::pow(C47 / C46, 4) /
                                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                                 C116) +
                            C173 * C46 * C110 *
                                ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                C55 /
                                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                           C172 * C46 *
                               (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                    0. * std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                                ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                    std::pow(C47 / C46, 4) / C110) /
                               (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                          C121 * C46 *
                              ((0. * C110 *
                                    ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                0. * std::pow(C47 / C46, 4)) +
                               C110 *
                                   (C49 * std::pow(C58 + 1., C49 - 1) -
                                    C49 * std::pow(1. - C58, C49 - 1)) *
                                   0. * std::pow(C47 / C46, 4)) /
                              (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C123 * C46 * C110 *
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             (0. * std::pow(C47 / C46, 4) +
                              0. *
                                  (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                           (15.8687885 * std::sqrt(C60) * C78 *
                                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                  C55) /
                             (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        C140 * C46 * C110 *
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            (0. * std::pow(C47 / C46, 4) +
                             0. *
                                 ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                      (15.8687885 * std::pow(C78, 2) *
                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                                 C55) /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C148 * C46 * C110 *
                           ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                           (0. * std::pow(C47 / C46, 4) +
                            0. *
                                (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                 (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                     15.8687885) *
                                C55) /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C117 * C46 * C110 *
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                          (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
                          (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     (((((((((C46 *
                                  (C110 *
                                       ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                       0. * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                   0. * std::pow(C47 / C46, 4)) +
                              C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  0. * std::pow(C47 / C46, 4)) /
                                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                             C116 *
                                 ((C46 * C110 *
                                       ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                       ((std::pow(C46, 2) *
                                             ((1.7099209341613653 *
                                                   (0.01554535 *
                                                        ((std::log(C60 / C78) +
                                                          0.1412084e2 * C86 /
                                                              std::sqrt(0.223816694236e2)) -
                                                         -2.2946365 *
                                                             (std::log(
                                                                  std::pow(std::sqrt(C60) + 0.325,
                                                                           2) /
                                                                  C78) +
                                                              0.1282084e2 * C86 /
                                                                  std::sqrt(0.223816694236e2)) /
                                                             15.8687885) -
                                                    C111) *
                                                   C55 / C110 -
                                               1.) *
                                                  -4 * C47 * std::pow(C47 / C46, 2) * -3 * C47 /
                                                  std::pow(C46, 2) +
                                              0. * std::pow(C47 / C46, 3) * -4 * C47 / C110) -
                                         (1.7099209341613653 *
                                              (0.01554535 *
                                                   ((std::log(C60 / C78) +
                                                     0.1412084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) -
                                                    -2.2946365 *
                                                        (std::log(
                                                             std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                         0.1282084e2 * C86 /
                                                             std::sqrt(0.223816694236e2)) /
                                                        15.8687885) -
                                               C111) *
                                              C55 / C110 -
                                          1.) *
                                             std::pow(C47 / C46, 3) * -4 * C47 * 2 * C46) /
                                            std::pow(C46, 4) +
                                        0. * std::pow(C47 / C46, 3) * -4 * C47 /
                                            (std::pow(C46, 2) * C110)) +
                                   C110 *
                                       ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                       ((1.7099209341613653 *
                                             (0.01554535 *
                                                  ((std::log(C60 / C78) +
                                                    0.1412084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) -
                                                   -2.2946365 *
                                                       (std::log(
                                                            std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                        0.1282084e2 * C86 /
                                                            std::sqrt(0.223816694236e2)) /
                                                       15.8687885) -
                                              C111) *
                                             C55 / C110 -
                                         1.) *
                                            std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                        0. * std::pow(C47 / C46, 4) / C110)) +
                                  C110 *
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                      ((1.7099209341613653 *
                                            (0.01554535 *
                                                 ((std::log(C60 / C78) +
                                                   0.1412084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) -
                                                  -2.2946365 *
                                                      (std::log(
                                                           std::pow(std::sqrt(C60) + 0.325, 2) /
                                                           C78) +
                                                       0.1282084e2 * C86 /
                                                           std::sqrt(0.223816694236e2)) /
                                                      15.8687885) -
                                             C111) *
                                            C55 / C110 -
                                        1.) *
                                           std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                       0. * std::pow(C47 / C46, 4) / C110)) /
                                 (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                            (C46 * C110 *
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 -1.7099209341613653 * C55 * std::pow(C47 / C46, 3) * -4 * C47 /
                                 (std::pow(C46, 2) * C110 * 3.4198418683227306 *
                                  (std::pow(2., C48) - 1.) * C55) +
                             (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 /
                                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                              1)) *
                                C173) +
                           C172 *
                               (C46 *
                                    (C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         (0. * std::pow(C47 / C46, 4) +
                                          (0. * C110 -
                                           1.7099209341613653 *
                                               (0.01554535 *
                                                    ((std::log(C60 / C78) +
                                                      0.1412084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) -
                                                     -2.2946365 *
                                                         (std::log(
                                                              std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                          0.1282084e2 * C86 /
                                                              std::sqrt(0.223816694236e2)) /
                                                         15.8687885) -
                                                C111) *
                                               C55) *
                                              std::pow(C47 / C46, 3) * -4 * C47 /
                                              std::pow(C46, 2)) /
                                         std::pow(C110, 2) +
                                     ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                         ((1.7099209341613653 *
                                               (0.01554535 *
                                                    ((std::log(C60 / C78) +
                                                      0.1412084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) -
                                                     -2.2946365 *
                                                         (std::log(
                                                              std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                          0.1282084e2 * C86 /
                                                              std::sqrt(0.223816694236e2)) /
                                                         15.8687885) -
                                                C111) *
                                               C55 / C110 -
                                           1.) *
                                              std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                          0. * std::pow(C47 / C46, 4) / C110)) +
                                (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                     std::pow(C47 / C46, 4) *
                                     (0. * C110 -
                                      1.7099209341613653 *
                                          (0.01554535 *
                                               ((std::log(C60 / C78) +
                                                 0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                -2.2946365 *
                                                    (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                     0.1282084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) /
                                                    15.8687885) -
                                           C111) *
                                          C55) /
                                     std::pow(C110, 2) +
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                     ((1.7099209341613653 *
                                           (0.01554535 *
                                                ((std::log(C60 / C78) +
                                                  0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                 -2.2946365 *
                                                     (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                      0.1282084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) /
                                                     15.8687885) -
                                            C111) *
                                           C55 / C110 -
                                       1.) *
                                          std::pow(C47 / C46, 4) +
                                      1.))) /
                               (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                          C121 *
                              (C46 *
                                   ((C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         0. * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                     0. * std::pow(C47 / C46, 4)) /
                                        C110 +
                                    C110 *
                                        (C49 * std::pow(C58 + 1., C49 - 1) -
                                         C49 * std::pow(1. - C58, C49 - 1)) *
                                        ((1.7099209341613653 *
                                              (0.01554535 *
                                                   ((std::log(C60 / C78) +
                                                     0.1412084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) -
                                                    -2.2946365 *
                                                        (std::log(
                                                             std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                         0.1282084e2 * C86 /
                                                             std::sqrt(0.223816694236e2)) /
                                                        15.8687885) -
                                               C111) *
                                              C55 / C110 -
                                          1.) *
                                             std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                         0. * std::pow(C47 / C46, 4) / C110)) +
                               (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                    0. * std::pow(C47 / C46, 4) / C110 +
                                C110 *
                                    (C49 * std::pow(C58 + 1., C49 - 1) -
                                     C49 * std::pow(1. - C58, C49 - 1)) *
                                    ((1.7099209341613653 *
                                          (0.01554535 *
                                               ((std::log(C60 / C78) +
                                                 0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                -2.2946365 *
                                                    (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                     0.1282084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) /
                                                    15.8687885) -
                                           C111) *
                                          C55 / C110 -
                                      1.) *
                                         std::pow(C47 / C46, 4) +
                                     1.))) /
                              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C123 *
                             (C46 * C110 *
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  (0. * std::pow(C47 / C46, 4) +
                                   0.2658131939386538e-1 *
                                       (C78 / (C78 * C60) -
                                        -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                            (15.8687885 * std::sqrt(C60) * C78 *
                                             std::pow(std::sqrt(C60) + 0.325, 2))) *
                                       C55 * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2)) +
                              C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                  (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                           (15.8687885 * std::sqrt(C60) * C78 *
                                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                  C55) /
                             (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        C140 *
                            (C46 * C110 *
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 (0. * std::pow(C47 / C46, 4) +
                                  0.2658131939386538e-1 *
                                      ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                       2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                           (15.8687885 * std::pow(C78, 2) *
                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                      C55 * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2)) +
                             C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                 ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                      (15.8687885 * std::pow(C78, 2) *
                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                                 C55) /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C148 *
                           (C46 * C110 *
                                ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                (0. * std::pow(C47 / C46, 4) +
                                 0.2658131939386538e-1 *
                                     (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                      (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                          15.8687885) *
                                     C55 * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2)) +
                            C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                 (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                     15.8687885) *
                                C55) /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C117 *
                          (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               ((C46 *
                                     ((1.7099209341613653 *
                                           (0.01554535 *
                                                ((std::log(C60 / C78) +
                                                  0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                 -2.2946365 *
                                                     (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                      0.1282084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) /
                                                     15.8687885) -
                                            C111) *
                                           C55 / C110 -
                                       1.) *
                                          4 * std::pow(C47 / C46, 2) * -3 * C47 / std::pow(C46, 2) +
                                      0. * std::pow(C47 / C46, 3) / C110) -
                                 (1.7099209341613653 *
                                      (0.01554535 *
                                           ((std::log(C60 / C78) +
                                             0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                            -2.2946365 *
                                                (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                          C78) +
                                                 0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                                15.8687885) -
                                       C111) *
                                      C55 / C110 -
                                  1.) *
                                     4 * std::pow(C47 / C46, 3)) /
                                    std::pow(C46, 2) +
                                0. * std::pow(C47 / C46, 3) * -4 * C47 /
                                    (std::pow(C46, 2) * C110)) +
                           C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               ((1.7099209341613653 *
                                     (0.01554535 *
                                          ((std::log(C60 / C78) +
                                            0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                           -2.2946365 *
                                               (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                         C78) +
                                                0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                               15.8687885) -
                                      C111) *
                                     C55 / C110 -
                                 1.) *
                                    4 * std::pow(C47 / C46, 3) / C46 +
                                0. * std::pow(C47 / C46, 4) / C110)) /
                          (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                         C174) +
                    (((((((((C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 * C46 *
                                 (0. * C110 *
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                      std::pow(C47 / C46, 4)) -
                             C46 * C110 *
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                 std::pow(C47 / C46, 4) *
                                 (0. * C110 +
                                  3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                                std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55,
                                         2) +
                            C116 *
                                (C46 *
                                     (C110 *
                                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                           2.) *
                                          (std::pow(C47 / C46, 3) * -4 * C47 *
                                               (0. * C110 -
                                                1.7099209341613653 *
                                                    (0.01554535 *
                                                         ((std::log(C60 / C78) +
                                                           0.1412084e2 * C86 /
                                                               std::sqrt(0.223816694236e2)) -
                                                          -2.2946365 *
                                                              (std::log(
                                                                   std::pow(std::sqrt(C60) + 0.325,
                                                                            2) /
                                                                   C78) +
                                                               0.1282084e2 * C86 /
                                                                   std::sqrt(0.223816694236e2)) /
                                                              15.8687885) -
                                                     C111) *
                                                    C55) /
                                               (std::pow(C110, 2) * std::pow(C46, 2)) +
                                           (0. * C110 - 0. * std::pow(C47 / C46, 4)) /
                                               std::pow(C110, 2)) +
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                          ((1.7099209341613653 *
                                                (0.01554535 *
                                                     ((std::log(C60 / C78) +
                                                       0.1412084e2 * C86 /
                                                           std::sqrt(0.223816694236e2)) -
                                                      -2.2946365 *
                                                          (std::log(
                                                               std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                           0.1282084e2 * C86 /
                                                               std::sqrt(0.223816694236e2)) /
                                                          15.8687885) -
                                                 C111) *
                                                C55 / C110 -
                                            1.) *
                                               std::pow(C47 / C46, 3) * -4 * C47 /
                                               std::pow(C46, 2) +
                                           0. * std::pow(C47 / C46, 4) / C110)) +
                                 (C110 *
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                      std::pow(C47 / C46, 4) *
                                      (0. * C110 -
                                       1.7099209341613653 *
                                           (0.01554535 *
                                                ((std::log(C60 / C78) +
                                                  0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                 -2.2946365 *
                                                     (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                      0.1282084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) /
                                                     15.8687885) -
                                            C111) *
                                           C55) /
                                      std::pow(C110, 2) +
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                      ((1.7099209341613653 *
                                            (0.01554535 *
                                                 ((std::log(C60 / C78) +
                                                   0.1412084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) -
                                                  -2.2946365 *
                                                      (std::log(
                                                           std::pow(std::sqrt(C60) + 0.325, 2) /
                                                           C78) +
                                                       0.1282084e2 * C86 /
                                                           std::sqrt(0.223816694236e2)) /
                                                      15.8687885) -
                                             C111) *
                                            C55 / C110 -
                                        1.) *
                                           std::pow(C47 / C46, 4) +
                                       1.))) /
                                (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                           C173 * C46 *
                               (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 *
                                    (C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         0. * C55 +
                                     ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                         std::pow(C47 / C46, 4) * -1.7099209341613653 * C55) -
                                C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                    std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 *
                                    (0. * C110 +
                                     3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                               std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55,
                                        2)) +
                          C172 * C46 *
                              ((std::pow(C110, 2) *
                                    (C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         0. * std::pow(C47 / C46, 4) +
                                     ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                         std::pow(C47 / C46, 4) *
                                         (0. * C110 -
                                          1.7099209341613653 *
                                              (0.01554535 *
                                                   ((std::log(C60 / C78) +
                                                     0.1412084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) -
                                                    -2.2946365 *
                                                        (std::log(
                                                             std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                         0.1282084e2 * C86 /
                                                             std::sqrt(0.223816694236e2)) /
                                                        15.8687885) -
                                               C111) *
                                              C55)) -
                                C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                    std::pow(C47 / C46, 4) *
                                    (0. * C110 -
                                     1.7099209341613653 *
                                         (0.01554535 *
                                              ((std::log(C60 / C78) +
                                                0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                               -2.2946365 *
                                                   (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                    0.1282084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) /
                                                   15.8687885) -
                                          C111) *
                                         C55) *
                                    2 * C110) /
                                   std::pow(C110, 4) +
                               ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   std::pow(C47 / C46, 4) *
                                   (0. * C110 -
                                    1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55) /
                                   std::pow(C110, 2)) /
                              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C121 * C46 *
                             ((C110 *
                                   (0. * C110 *
                                        ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                    ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                        0. * std::pow(C47 / C46, 4)) -
                               C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   0. * std::pow(C47 / C46, 4)) /
                                  std::pow(C110, 2) +
                              (C110 *
                                   (C49 * std::pow(C58 + 1., C49 - 1) -
                                    C49 * std::pow(1. - C58, C49 - 1)) *
                                   std::pow(C47 / C46, 4) *
                                   (0. * C110 -
                                    1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55) /
                                   std::pow(C110, 2) +
                               (C49 * std::pow(C58 + 1., C49 - 1) -
                                C49 * std::pow(1. - C58, C49 - 1)) *
                                   ((1.7099209341613653 *
                                         (0.01554535 *
                                              ((std::log(C60 / C78) +
                                                0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                               -2.2946365 *
                                                   (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                    0.1282084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) /
                                                   15.8687885) -
                                          C111) *
                                         C55 / C110 -
                                     1.) *
                                        std::pow(C47 / C46, 4) +
                                    1.))) /
                             (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 * C123 * C46 *
                             (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  (0. * std::pow(C47 / C46, 4) +
                                   0. *
                                       (C78 / (C78 * C60) -
                                        -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                            (15.8687885 * std::sqrt(C60) * C78 *
                                             std::pow(std::sqrt(C60) + 0.325, 2))) *
                                       C55) +
                              ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                  (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                           (15.8687885 * std::sqrt(C60) * C78 *
                                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                  C55) -
                         C123 * C46 * C110 *
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                             (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                      (15.8687885 * std::sqrt(C60) * C78 *
                                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                             C55 *
                             (0. * C110 + 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                            std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55,
                                     2)) +
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 * C140 * C46 *
                            (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 (0. * std::pow(C47 / C46, 4) +
                                  0. *
                                      ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                       2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                           (15.8687885 * std::pow(C78, 2) *
                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                      C55) +
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                 ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                      (15.8687885 * std::pow(C78, 2) *
                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                                 C55) -
                        C140 * C46 * C110 *
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                            ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                             2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                 (15.8687885 * std::pow(C78, 2) *
                                  std::pow(std::sqrt(C60) + 0.325, 2))) *
                            C55 *
                            (0. * C110 + 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                           std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55,
                                    2)) +
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 * C148 * C46 *
                           (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                (0. * std::pow(C47 / C46, 4) +
                                 0. *
                                     (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                      (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                          15.8687885) *
                                     C55) +
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                 (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                     15.8687885) *
                                C55) -
                       C148 * C46 * C110 *
                           ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                           std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                           (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                            (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                           C55 *
                           (0. * C110 + 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                          std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55, 2)) +
                     C117 * C46 *
                         (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              (4 * std::pow(C47 / C46, 3) *
                                   (0. * C110 -
                                    1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55) /
                                   (std::pow(C110, 2) * C46) +
                               (0. * C110 - 0. * std::pow(C47 / C46, 4)) / std::pow(C110, 2)) +
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              ((1.7099209341613653 *
                                    (0.01554535 *
                                         ((std::log(C60 / C78) +
                                           0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                          -2.2946365 *
                                              (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                               0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                              15.8687885) -
                                     C111) *
                                    C55 / C110 -
                                1.) *
                                   4 * std::pow(C47 / C46, 3) / C46 +
                               0. * std::pow(C47 / C46, 4) / C110)) /
                         (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                        C230) +
                   ((((((((C46 *
                               (0. * C110 *
                                    ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                C110 *
                                    (C49 * std::pow(C58 + 1., C49 - 1) -
                                     C49 * std::pow(1. - C58, C49 - 1)) *
                                    0. * std::pow(C47 / C46, 4)) /
                               (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                           C116 *
                               (C46 *
                                    (C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         (0. * std::pow(C47 / C46, 3) * -4 * C47 /
                                              (C110 * std::pow(C46, 2)) +
                                          0. / C110) +
                                     C110 *
                                         (C49 * std::pow(C58 + 1., C49 - 1) -
                                          C49 * std::pow(1. - C58, C49 - 1)) *
                                         ((1.7099209341613653 *
                                               (0.01554535 *
                                                    ((std::log(C60 / C78) +
                                                      0.1412084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) -
                                                     -2.2946365 *
                                                         (std::log(
                                                              std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                          0.1282084e2 * C86 /
                                                              std::sqrt(0.223816694236e2)) /
                                                         15.8687885) -
                                                C111) *
                                               C55 / C110 -
                                           1.) *
                                              std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                          0. * std::pow(C47 / C46, 4) / C110)) +
                                (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                     0. * std::pow(C47 / C46, 4) / C110 +
                                 C110 *
                                     (C49 * std::pow(C58 + 1., C49 - 1) -
                                      C49 * std::pow(1. - C58, C49 - 1)) *
                                     ((1.7099209341613653 *
                                           (0.01554535 *
                                                ((std::log(C60 / C78) +
                                                  0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                 -2.2946365 *
                                                     (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                      0.1282084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) /
                                                     15.8687885) -
                                            C111) *
                                           C55 / C110 -
                                       1.) *
                                          std::pow(C47 / C46, 4) +
                                      1.))) /
                               (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                          C173 * C46 *
                              (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   0. * C55 +
                               C110 *
                                   (C49 * std::pow(C58 + 1., C49 - 1) -
                                    C49 * std::pow(1. - C58, C49 - 1)) *
                                   std::pow(C47 / C46, 4) * -1.7099209341613653 * C55) /
                              (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C172 * C46 *
                             ((C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   0. * std::pow(C47 / C46, 4) +
                               C110 *
                                   (C49 * std::pow(C58 + 1., C49 - 1) -
                                    C49 * std::pow(1. - C58, C49 - 1)) *
                                   std::pow(C47 / C46, 4) *
                                   (0. * C110 -
                                    1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55)) /
                                  std::pow(C110, 2) +
                              (((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                   std::pow(C47 / C46, 4) / C110 +
                               (C49 * std::pow(C58 + 1., C49 - 1) -
                                C49 * std::pow(1. - C58, C49 - 1)) *
                                   ((1.7099209341613653 *
                                         (0.01554535 *
                                              ((std::log(C60 / C78) +
                                                0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                               -2.2946365 *
                                                   (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                    0.1282084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) /
                                                   15.8687885) -
                                          C111) *
                                         C55 / C110 -
                                     1.) *
                                        std::pow(C47 / C46, 4) +
                                    1.))) /
                             (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        C121 * C46 *
                            ((0. * C110 *
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                              C110 *
                                  (C49 * std::pow(C58 + 1., C49 - 1) -
                                   C49 * std::pow(1. - C58, C49 - 1)) *
                                  0. * std::pow(C47 / C46, 4)) /
                                 C110 +
                             (C110 *
                                  (C49 * std::pow(C58 + 1., C49 - 1) -
                                   C49 * std::pow(1. - C58, C49 - 1)) *
                                  0. * std::pow(C47 / C46, 4) / C110 +
                              C110 *
                                  (C49 * (C49 - 1) * std::pow(C58 + 1., C49 - 2) -
                                   C49 * (1 - C49) * std::pow(1. - C58, C49 - 2)) *
                                  ((1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55 / C110 -
                                    1.) *
                                       std::pow(C47 / C46, 4) +
                                   1.))) /
                            (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C123 * C46 *
                           (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                (0. * std::pow(C47 / C46, 4) +
                                 0. *
                                     (C78 / (C78 * C60) -
                                      -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                          (15.8687885 * std::sqrt(C60) * C78 *
                                           std::pow(std::sqrt(C60) + 0.325, 2))) *
                                     C55) +
                            C110 *
                                (C49 * std::pow(C58 + 1., C49 - 1) -
                                 C49 * std::pow(1. - C58, C49 - 1)) *
                                std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                         (15.8687885 * std::sqrt(C60) * C78 *
                                                          std::pow(std::sqrt(C60) + 0.325, 2))) *
                                C55) /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C140 * C46 *
                          (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               (0. * std::pow(C47 / C46, 4) +
                                0. *
                                    ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                     2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                         (15.8687885 * std::pow(C78, 2) *
                                          std::pow(std::sqrt(C60) + 0.325, 2))) *
                                    C55) +
                           C110 *
                               (C49 * std::pow(C58 + 1., C49 - 1) -
                                C49 * std::pow(1. - C58, C49 - 1)) *
                               std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                               ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                    (15.8687885 * std::pow(C78, 2) *
                                     std::pow(std::sqrt(C60) + 0.325, 2))) *
                               C55) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C148 * C46 *
                         (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              (0. * std::pow(C47 / C46, 4) +
                               0. *
                                   (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                    (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                        15.8687885) *
                                   C55) +
                          C110 *
                              (C49 * std::pow(C58 + 1., C49 - 1) -
                               C49 * std::pow(1. - C58, C49 - 1)) *
                              std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                               (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                              C55) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C117 * C46 *
                        (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) +
                         C110 *
                             (C49 * std::pow(C58 + 1., C49 - 1) -
                              C49 * std::pow(1. - C58, C49 - 1)) *
                             ((1.7099209341613653 *
                                   (0.01554535 *
                                        ((std::log(C60 / C78) +
                                          0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                         -2.2946365 *
                                             (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                              0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                             15.8687885) -
                                    C111) *
                                   C55 / C110 -
                               1.) *
                                  4 * std::pow(C47 / C46, 3) / C46 +
                              0. * std::pow(C47 / C46, 4) / C110)) /
                        (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                       C179) +
                  ((((((((C46 *
                              (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   0. * std::pow(C47 / C46, 3) / C46 +
                               0. * std::pow(C47 / C46, 4)) /
                              (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                          C116 *
                              (C46 * C110 *
                                   ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   (((1.7099209341613653 *
                                          (0.01554535 *
                                               ((std::log(C60 / C78) +
                                                 0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                -2.2946365 *
                                                    (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                     0.1282084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) /
                                                    15.8687885) -
                                           C111) *
                                          C55 / C110 -
                                      1.) *
                                         (-4 * std::pow(C47 / C46, 3) +
                                          -4 * C47 * 3 * std::pow(C47 / C46, 2) / C46) +
                                     0. * std::pow(C47 / C46, 3) * -4 * C47 / C110) /
                                        std::pow(C46, 2) +
                                    0. * std::pow(C47 / C46, 3) / (C46 * C110)) +
                               C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   ((1.7099209341613653 *
                                         (0.01554535 *
                                              ((std::log(C60 / C78) +
                                                0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                               -2.2946365 *
                                                   (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                    0.1282084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) /
                                                   15.8687885) -
                                          C111) *
                                         C55 / C110 -
                                     1.) *
                                        4 * std::pow(C47 / C46, 3) / C46 +
                                    0. * std::pow(C47 / C46, 4) / C110)) /
                              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C173 * C46 * C110 *
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             -1.7099209341613653 * C55 * 4 * std::pow(C47 / C46, 3) /
                             (C46 * C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        C172 * C46 *
                            (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 (0. * std::pow(C47 / C46, 4) +
                                  (0. * C110 -
                                   1.7099209341613653 *
                                       (0.01554535 *
                                            ((std::log(C60 / C78) +
                                              0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                             -2.2946365 *
                                                 (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                           C78) +
                                                  0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                                 15.8687885) -
                                        C111) *
                                       C55) *
                                      4 * std::pow(C47 / C46, 3) / C46) /
                                 std::pow(C110, 2) +
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 ((1.7099209341613653 *
                                       (0.01554535 *
                                            ((std::log(C60 / C78) +
                                              0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                             -2.2946365 *
                                                 (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                           C78) +
                                                  0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                                 15.8687885) -
                                        C111) *
                                       C55 / C110 -
                                   1.) *
                                      4 * std::pow(C47 / C46, 3) / C46 +
                                  0. * std::pow(C47 / C46, 4) / C110)) /
                            (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C121 * C46 *
                           ((C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 0. * std::pow(C47 / C46, 3) / C46 +
                             0. * std::pow(C47 / C46, 4)) /
                                C110 +
                            C110 *
                                (C49 * std::pow(C58 + 1., C49 - 1) -
                                 C49 * std::pow(1. - C58, C49 - 1)) *
                                ((1.7099209341613653 *
                                      (0.01554535 *
                                           ((std::log(C60 / C78) +
                                             0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                            -2.2946365 *
                                                (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                          C78) +
                                                 0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                                15.8687885) -
                                       C111) *
                                      C55 / C110 -
                                  1.) *
                                     4 * std::pow(C47 / C46, 3) / C46 +
                                 0. * std::pow(C47 / C46, 4) / C110)) /
                           (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C123 * C46 * C110 *
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                          (0. * std::pow(C47 / C46, 4) +
                           0.2658131939386538e-1 *
                               (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                        (15.8687885 * std::sqrt(C60) * C78 *
                                                         std::pow(std::sqrt(C60) + 0.325, 2))) *
                               C55 * 4 * std::pow(C47 / C46, 3) / C46) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C140 * C46 * C110 *
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                         (0. * std::pow(C47 / C46, 4) +
                          0.2658131939386538e-1 *
                              ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                               2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                   (15.8687885 * std::pow(C78, 2) *
                                    std::pow(std::sqrt(C60) + 0.325, 2))) *
                              C55 * 4 * std::pow(C47 / C46, 3) / C46) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                        (0. * std::pow(C47 / C46, 4) +
                         0.2658131939386538e-1 *
                             (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                              (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                             C55 * 4 * std::pow(C47 / C46, 3) / C46) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (((1.7099209341613653 *
                              (0.01554535 *
                                   ((std::log(C60 / C78) +
                                     0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                    -2.2946365 *
                                        (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                         0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                        15.8687885) -
                               C111) *
                              C55 / C110 -
                          1.) *
                             12 * std::pow(C47 / C46, 2) / C46 +
                         0. * std::pow(C47 / C46, 3) / C110) /
                            C46 +
                        0. * std::pow(C47 / C46, 3) / (C46 * C110)) /
                       (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                      C175) +
                 ((((((((C46 *
                             (0. * C110 *
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                              0. * std::pow(C47 / C46, 4)) /
                             (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                         (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              (std::pow(C47 / C46, 3) * -4 * C47 * 0.2658131939386538e-1 *
                                   (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) *
                                                            C78 /
                                                            (15.8687885 * std::sqrt(C60) * C78 *
                                                             std::pow(std::sqrt(C60) + 0.325, 2))) *
                                   C55 / (C110 * std::pow(C46, 2)) +
                               0. / C110) /
                              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                          C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                       (15.8687885 * std::sqrt(C60) * C78 *
                                                        std::pow(std::sqrt(C60) + 0.325, 2))) *
                              C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                             C116) +
                        C173 * C46 * C110 *
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C172 * C46 *
                           (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) *
                                (0. - 0.2658131939386538e-1 *
                                          (C78 / (C78 * C60) -
                                           -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                               (15.8687885 * std::sqrt(C60) * C78 *
                                                std::pow(std::sqrt(C60) + 0.325, 2))) *
                                          C55) /
                                std::pow(C110, 2) +
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                         (15.8687885 * std::sqrt(C60) * C78 *
                                                          std::pow(std::sqrt(C60) + 0.325, 2))) *
                                C55 / C110) /
                           (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C121 * C46 *
                          ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                            0. * std::pow(C47 / C46, 4)) +
                           C110 *
                               (C49 * std::pow(C58 + 1., C49 - 1) -
                                C49 * std::pow(1. - C58, C49 - 1)) *
                               std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                               (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                        (15.8687885 * std::sqrt(C60) * C78 *
                                                         std::pow(std::sqrt(C60) + 0.325, 2))) *
                               C55) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C123 * C46 * C110 *
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                         (std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              ((-std::pow(C78, 2)) / std::pow(C78 * C60, 2) -
                               (15.8687885 * std::sqrt(C60) * C78 *
                                    std::pow(std::sqrt(C60) + 0.325, 2) * -2.2946365 * C78 /
                                    (2 * std::sqrt(C60)) -
                                -2.2946365 * (std::sqrt(C60) + 0.325) * C78 * 15.8687885 *
                                    (std::sqrt(C60) * C78 * (std::sqrt(C60) + 0.325) /
                                         std::sqrt(C60) +
                                     std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                         (2 * std::sqrt(C60)))) /
                                   std::pow(15.8687885 * std::sqrt(C60) * C78 *
                                                std::pow(std::sqrt(C60) + 0.325, 2),
                                            2)) *
                              C55 +
                          0. *
                              (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                       (15.8687885 * std::sqrt(C60) * C78 *
                                                        std::pow(std::sqrt(C60) + 0.325, 2))) *
                              C55) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                        (std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                             ((C60 * std::pow(C78, 3) - std::pow(C78, 2) * C60 * C78) /
                                  std::pow(std::pow(C78, 2) * C60, 2) -
                              (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2) *
                                   2.2946365 * C78 * (std::sqrt(C60) + 0.325) / std::sqrt(C60) -
                               2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 * 15.8687885 *
                                   std::pow(C78, 2) * (std::sqrt(C60) + 0.325) / std::sqrt(C60)) /
                                  std::pow(15.8687885 * std::pow(C78, 2) *
                                               std::pow(std::sqrt(C60) + 0.325, 2),
                                           2)) *
                             C55 +
                         0. *
                             ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                              2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                  (15.8687885 * std::pow(C78, 2) *
                                   std::pow(std::sqrt(C60) + 0.325, 2))) *
                             C55) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (0. * std::pow(C47 / C46, 4) +
                        0. *
                            (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                             (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                            C55) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (4 * std::pow(C47 / C46, 3) * 0.2658131939386538e-1 *
                           (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                    (15.8687885 * std::sqrt(C60) * C78 *
                                                     std::pow(std::sqrt(C60) + 0.325, 2))) *
                           C55 / (C110 * C46) +
                       0. / C110) /
                      (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                     C181) +
                ((((((((C46 *
                            (0. * C110 *
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                             0. * std::pow(C47 / C46, 4)) /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                        (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             (std::pow(C47 / C46, 3) * -4 * C47 * 0.2658131939386538e-1 *
                                  ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                   2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                       (15.8687885 * std::pow(C78, 2) *
                                        std::pow(std::sqrt(C60) + 0.325, 2))) *
                                  C55 / (C110 * std::pow(C46, 2)) +
                              0. / C110) /
                             (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                         C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                             ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                              2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                  (15.8687885 * std::pow(C78, 2) *
                                   std::pow(std::sqrt(C60) + 0.325, 2))) *
                             C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                            C116) +
                       C173 * C46 * C110 *
                           ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C172 * C46 *
                          (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               std::pow(C47 / C46, 4) *
                               (0. - 0.2658131939386538e-1 *
                                         ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                          2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                              (15.8687885 * std::pow(C78, 2) *
                                               std::pow(std::sqrt(C60) + 0.325, 2))) *
                                         C55) /
                               std::pow(C110, 2) +
                           ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                               ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                    (15.8687885 * std::pow(C78, 2) *
                                     std::pow(std::sqrt(C60) + 0.325, 2))) *
                               C55 / C110) /
                          (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C121 * C46 *
                         ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                           0. * std::pow(C47 / C46, 4)) +
                          C110 *
                              (C49 * std::pow(C58 + 1., C49 - 1) -
                               C49 * std::pow(1. - C58, C49 - 1)) *
                              std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                               2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                   (15.8687885 * std::pow(C78, 2) *
                                    std::pow(std::sqrt(C60) + 0.325, 2))) *
                              C55) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                        (std::pow(C47 / C46, 4) * -0.2658131939386538e-1 * C55 *
                             (15.8687885 * std::sqrt(C60) * C78 *
                                  std::pow(std::sqrt(C60) + 0.325, 2) * -2.2946365 *
                                  (std::sqrt(C60) + 0.325) -
                              -2.2946365 * (std::sqrt(C60) + 0.325) * C78 * 15.8687885 *
                                  std::sqrt(C60) * std::pow(std::sqrt(C60) + 0.325, 2)) /
                             std::pow(15.8687885 * std::sqrt(C60) * C78 *
                                          std::pow(std::sqrt(C60) + 0.325, 2),
                                      2) +
                         0. *
                             (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                      (15.8687885 * std::sqrt(C60) * C78 *
                                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                             C55) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                            ((C60 * C78 * 2 * C78 * C60 - std::pow(C78, 2) * std::pow(C60, 2)) /
                                 std::pow(std::pow(C78, 2) * C60, 2) -
                             (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2) *
                                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) -
                              2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 * 0.31737577e2 *
                                  C78 * std::pow(std::sqrt(C60) + 0.325, 2)) /
                                 std::pow(15.8687885 * std::pow(C78, 2) *
                                              std::pow(std::sqrt(C60) + 0.325, 2),
                                          2)) *
                            C55 +
                        0. *
                            ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                             2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                 (15.8687885 * std::pow(C78, 2) *
                                  std::pow(std::sqrt(C60) + 0.325, 2))) *
                            C55) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (0. * std::pow(C47 / C46, 4) +
                       0. *
                           (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                            (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                           C55) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (4 * std::pow(C47 / C46, 3) * 0.2658131939386538e-1 *
                          ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                           2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                               (15.8687885 * std::pow(C78, 2) *
                                std::pow(std::sqrt(C60) + 0.325, 2))) *
                          C55 / (C110 * C46) +
                      0. / C110) /
                     (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                    C198) +
               ((((((((C46 *
                           (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                            0. * std::pow(C47 / C46, 4)) /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                       (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            (std::pow(C47 / C46, 3) * -4 * C47 * 0.2658131939386538e-1 *
                                 (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                  (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                      15.8687885) *
                                 C55 / (C110 * std::pow(C46, 2)) +
                             0. / C110) /
                            (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                        C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                            (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                             (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                            C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                           C116) +
                      C173 * C46 * C110 *
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C172 * C46 *
                         (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              std::pow(C47 / C46, 4) *
                              (0. - 0.2658131939386538e-1 *
                                        (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                         (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                             15.8687885) *
                                        C55) /
                              std::pow(C110, 2) +
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                               (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                              C55 / C110) /
                         (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C121 * C46 *
                        ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                          0. * std::pow(C47 / C46, 4)) +
                         C110 *
                             (C49 * std::pow(C58 + 1., C49 - 1) -
                              C49 * std::pow(1. - C58, C49 - 1)) *
                             std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                             (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                              (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                             C55) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (0. * std::pow(C47 / C46, 4) +
                        0. *
                            (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                     (15.8687885 * std::sqrt(C60) * C78 *
                                                      std::pow(std::sqrt(C60) + 0.325, 2))) *
                            C55) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (0. * std::pow(C47 / C46, 4) +
                       0. *
                           ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                            2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                (15.8687885 * std::pow(C78, 2) *
                                 std::pow(std::sqrt(C60) + 0.325, 2))) *
                           C55) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (0. * std::pow(C47 / C46, 4) +
                      0. *
                          (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                           (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                          C55) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    (4 * std::pow(C47 / C46, 3) * 0.2658131939386538e-1 *
                         (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                          (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                         C55 / (C110 * C46) +
                     0. / C110) /
                    (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                   C206) +
              ((((((((C46 *
                          (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                           0. * std::pow(C47 / C46, 4)) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                      (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                           (std::pow(C47 / C46, 3) * -4 * C47 * -1.7099209341613653 * C55 /
                                (C110 * std::pow(C46, 2)) +
                            0. / C110) /
                           (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                       (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                        1)) *
                          C116) +
                     C173 * C46 * C110 *
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C172 * C46 *
                        (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             std::pow(C47 / C46, 4) * (0. - -1.7099209341613653 * C55) /
                             std::pow(C110, 2) +
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 / C110) /
                        (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C121 * C46 *
                       ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                         0. * std::pow(C47 / C46, 4)) +
                        C110 *
                            (C49 * std::pow(C58 + 1., C49 - 1) -
                             C49 * std::pow(1. - C58, C49 - 1)) *
                            std::pow(C47 / C46, 4) * -1.7099209341613653 * C55) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (0. * std::pow(C47 / C46, 4) +
                       0. *
                           (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                    (15.8687885 * std::sqrt(C60) * C78 *
                                                     std::pow(std::sqrt(C60) + 0.325, 2))) *
                           C55) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (0. * std::pow(C47 / C46, 4) +
                      0. *
                          ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                           2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                               (15.8687885 * std::pow(C78, 2) *
                                std::pow(std::sqrt(C60) + 0.325, 2))) *
                          C55) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    (0. * std::pow(C47 / C46, 4) +
                     0. *
                         (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                          (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                         C55) /
                    (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (4 * std::pow(C47 / C46, 3) * -1.7099209341613653 * C55 / (C110 * C46) +
                    0. / C110) /
                   (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                  C231) +
             ((((((((C46 *
                         (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                          0. * std::pow(C47 / C46, 4)) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                     (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                          (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                           0. / C110) /
                          (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                      C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                          std::pow(C47 / C46, 4) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                         C116) +
                    (C46 * (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 /
                                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                            1) +
                     C173 * C46 * C110 *
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55))) +
                   C172 * C46 *
                       (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                            std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                        ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                            std::pow(C47 / C46, 4) / C110) /
                       (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C121 * C46 *
                      ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                        0. * std::pow(C47 / C46, 4)) +
                       C110 *
                           (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                           0. * std::pow(C47 / C46, 4)) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (0. * std::pow(C47 / C46, 4) +
                      0. *
                          (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                   (15.8687885 * std::sqrt(C60) * C78 *
                                                    std::pow(std::sqrt(C60) + 0.325, 2))) *
                          C55) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    (0. * std::pow(C47 / C46, 4) +
                     0. *
                         ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                          2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                              (15.8687885 * std::pow(C78, 2) *
                               std::pow(std::sqrt(C60) + 0.325, 2))) *
                         C55) /
                    (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (0. * std::pow(C47 / C46, 4) +
                    0. *
                        (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                         (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                        C55) /
                   (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                  (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
                  (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                 C339) +
            ((((((((C46 *
                        (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                         0. * std::pow(C47 / C46, 4)) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                    (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                         (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                          0. / C110) /
                         (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                     C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                         std::pow(C47 / C46, 4) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                        C116) +
                   C173 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       0. * C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  (C172 * C46 *
                       (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                            std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                        ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                            std::pow(C47 / C46, 4) / C110) +
                   C46 * (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              std::pow(C47 / C46, 4) *
                              (0. * C110 -
                               1.7099209341613653 *
                                   (0.01554535 *
                                        ((std::log(C60 / C78) +
                                          0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                         -2.2946365 *
                                             (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                              0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                             15.8687885) -
                                    C111) *
                                   C55) /
                              std::pow(C110, 2) +
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              ((1.7099209341613653 *
                                    (0.01554535 *
                                         ((std::log(C60 / C78) +
                                           0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                          -2.2946365 *
                                              (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                               0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                              15.8687885) -
                                     C111) *
                                    C55 / C110 -
                                1.) *
                                   std::pow(C47 / C46, 4) +
                               1.))) /
                      (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C121 * C46 *
                     ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                       0. * std::pow(C47 / C46, 4)) +
                      C110 *
                          (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                          0. * std::pow(C47 / C46, 4)) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    (0. * std::pow(C47 / C46, 4) +
                     0. *
                         (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                  (15.8687885 * std::sqrt(C60) * C78 *
                                                   std::pow(std::sqrt(C60) + 0.325, 2))) *
                         C55) /
                    (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (0. * std::pow(C47 / C46, 4) +
                    0. *
                        ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                         2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                             (15.8687885 * std::pow(C78, 2) *
                              std::pow(std::sqrt(C60) + 0.325, 2))) *
                        C55) /
                   (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                  (0. * std::pow(C47 / C46, 4) +
                   0. *
                       (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                        (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                       C55) /
                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
                 (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                C338) +
           ((((((((C46 *
                       (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                        0. * std::pow(C47 / C46, 4)) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                   (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                        (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                         0. / C110) /
                        (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                    C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                        std::pow(C47 / C46, 4) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                       C116) +
                  C173 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      0. * C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C172 * C46 *
                     (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                          std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                          std::pow(C47 / C46, 4) / C110) /
                     (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                (C121 * C46 *
                     ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                       0. * std::pow(C47 / C46, 4)) +
                      C110 *
                          (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                          0. * std::pow(C47 / C46, 4)) /
                     C110 +
                 C46 *
                     (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                          std::pow(C47 / C46, 4) / C110 +
                      C110 *
                          (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                          ((1.7099209341613653 *
                                (0.01554535 *
                                     ((std::log(C60 / C78) +
                                       0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                      -2.2946365 *
                                          (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                           0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                          15.8687885) -
                                 C111) *
                                C55 / C110 -
                            1.) *
                               std::pow(C47 / C46, 4) +
                           1.))) /
                    (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (0. * std::pow(C47 / C46, 4) +
                    0. *
                        (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                 (15.8687885 * std::sqrt(C60) * C78 *
                                                  std::pow(std::sqrt(C60) + 0.325, 2))) *
                        C55) /
                   (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                  (0. * std::pow(C47 / C46, 4) +
                   0. *
                       ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                        2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                            (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                       C55) /
                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 4) +
                  0. *
                      (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                       (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                      C55) /
                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
            C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
                (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
               C287) +
          ((((((((C46 *
                      (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                       0. * std::pow(C47 / C46, 4)) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                  (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                        0. / C110) /
                       (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                   C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                       std::pow(C47 / C46, 4) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                      C116) +
                 C173 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     0. * C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C172 * C46 *
                    (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                         std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                     ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                         std::pow(C47 / C46, 4) / C110) /
                    (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C121 * C46 *
                   ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                     0. * std::pow(C47 / C46, 4)) +
                    C110 * (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                        0. * std::pow(C47 / C46, 4)) /
                   (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              (C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (0. * std::pow(C47 / C46, 4) +
                    0. *
                        (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                 (15.8687885 * std::sqrt(C60) * C78 *
                                                  std::pow(std::sqrt(C60) + 0.325, 2))) *
                        C55) +
               C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                   (C78 / (C78 * C60) -
                    -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                        (15.8687885 * std::sqrt(C60) * C78 * std::pow(std::sqrt(C60) + 0.325, 2))) *
                   C55) /
                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 4) +
                  0. *
                      ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                       2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                           (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                      C55) /
                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
            C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                (0. * std::pow(C47 / C46, 4) +
                 0. *
                     (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                      (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                     C55) /
                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
           C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
               (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
              C289) +
         ((((((((C46 *
                     (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                      0. * std::pow(C47 / C46, 4)) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                 (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                       0. / C110) /
                      (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                  C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                      std::pow(C47 / C46, 4) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                     C116) +
                C173 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    0. * C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C172 * C46 *
                   (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                        std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                    ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                        std::pow(C47 / C46, 4) / C110) /
                   (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C121 * C46 *
                  ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                    0. * std::pow(C47 / C46, 4)) +
                   C110 * (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                       0. * std::pow(C47 / C46, 4)) /
                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 4) +
                  0. *
                      (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                               (15.8687885 * std::sqrt(C60) * C78 *
                                                std::pow(std::sqrt(C60) + 0.325, 2))) *
                      C55) /
                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
            (C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 4) +
                  0. *
                      ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                       2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                           (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                      C55) +
             C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                 ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                      (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                 C55) /
                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
           C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               (0. * std::pow(C47 / C46, 4) +
                0. *
                    (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                     (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                    C55) /
               (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
          C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
              (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
             C306) +
        ((((((((C46 *
                    (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                     0. * std::pow(C47 / C46, 4)) /
                    (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                      0. / C110) /
                     (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                 C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                     std::pow(C47 / C46, 4) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                    C116) +
               C173 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                   C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C172 * C46 *
                  (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                       std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                   ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                       std::pow(C47 / C46, 4) / C110) /
                  (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C121 * C46 *
                 ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                   0. * std::pow(C47 / C46, 4)) +
                  C110 * (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                      0. * std::pow(C47 / C46, 4)) /
                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
            C123 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                (0. * std::pow(C47 / C46, 4) +
                 0. *
                     (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                              (15.8687885 * std::sqrt(C60) * C78 *
                                               std::pow(std::sqrt(C60) + 0.325, 2))) *
                     C55) /
                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
           C140 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               (0. * std::pow(C47 / C46, 4) +
                0. *
                    ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                     2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                         (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                    C55) /
               (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
          (C148 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               (0. * std::pow(C47 / C46, 4) +
                0. *
                    (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                     (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                    C55) +
           C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
               (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
               C55) /
              (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
         C117 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
             (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
             (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
            C314;

    const double C340 = 0.;
    const double C341 =
        (C340 / C46 +
         ((-C176) / std::pow(C46, 2) - (-C174 * C54 * 2 * C46) / std::pow(C46, 4)) * C174) -
        C176 * C174 / std::pow(C46, 2);
    const double C342 = (-C178 * -3. * C178 * 2 * C57) / std::pow(C57, 4);
    const double C343 = C48 * (C48 - 1) * std::pow(C59, C48 - 2) * std::pow(C180, 2) +
                        C48 * std::pow(C59, C48 - 1) * C342;
    const double C344 =
        1.13107 * C343 / (2 * std::sqrt(C60)) -
        1.13107 * std::pow(C181, 2) / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C345 = 2. * C343 / (2 * std::sqrt(C60)) -
                        2. * std::pow(C181, 2) / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C346 =
        3.72744 * C343 / (2 * std::sqrt(C60)) -
        3.72744 * std::pow(C181, 2) / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C347 =
        7.06042 * C343 / (2 * std::sqrt(C60)) -
        7.06042 * std::pow(C181, 2) / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C348 = C343 / (2 * std::sqrt(C60)) -
                        std::pow(C181, 2) / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C349 = C343 / (2 * std::sqrt(C60)) -
                        std::pow(C181, 2) / (std::sqrt(C60) * std::pow(2 * std::sqrt(C60), 2));
    const double C350 = C343 + C344;
    const double C351 = C343 + C346;
    const double C352 = C343 + C347;
    const double C353 = C345;
    const double C354 = C345;
    const double C355 = C345;
    const double C356 = 2 * std::pow(C186, 2) + 2 * C66 * C348;
    const double C357 = 2 * std::pow(C187, 2) + 2 * C67 * C349;
    const double C358 = C350;
    const double C359 = C351;
    const double C360 = C352;
    const double C361 = C193 * C193 * std::sqrt(0.223816694236e2) * 2 * C73 / std::pow(C73, 4) -
                        C355 * std::sqrt(0.223816694236e2) / std::pow(C73, 2);
    const double C362 = C192 * C192 * std::sqrt(0.378469910464e2) * 2 * C72 / std::pow(C72, 4) -
                        C354 * std::sqrt(0.378469910464e2) / std::pow(C72, 2);
    const double C363 = C191 * C191 * std::sqrt(0.507386806551e2) * 2 * C71 / std::pow(C71, 4) -
                        C353 * std::sqrt(0.507386806551e2) / std::pow(C71, 2);
    const double C364 =
        ((C343 / C76 +
          ((-C181) / std::pow(C76, 2) - (-C196 * C60 * 2 * C76) / std::pow(C76, 4)) * C196) -
         C358 * C60 / std::pow(C76, 2)) -
        C181 * C196 / std::pow(C76, 2);
    const double C365 =
        ((C343 / C77 +
          ((-C181) / std::pow(C77, 2) - (-C197 * C60 * 2 * C77) / std::pow(C77, 4)) * C197) -
         C359 * C60 / std::pow(C77, 2)) -
        C181 * C197 / std::pow(C77, 2);
    const double C366 =
        ((C356 / C76 +
          ((-C194) / std::pow(C76, 2) - (-C196 * C74 * 2 * C76) / std::pow(C76, 4)) * C196) -
         C358 * C74 / std::pow(C76, 2)) -
        C194 * C196 / std::pow(C76, 2);
    const double C367 =
        ((C357 / C77 +
          ((-C195) / std::pow(C77, 2) - (-C197 * C75 * 2 * C77) / std::pow(C77, 4)) * C197) -
         C359 * C75 / std::pow(C77, 2)) -
        C195 * C197 / std::pow(C77, 2);
    const double C368 = C361 / (std::pow(C79, 2) + 1) -
                        2 * C79 * std::pow(C199, 2) / std::pow(std::pow(C79, 2) + 1, 2);
    const double C369 = C362 / (std::pow(C80, 2) + 1) -
                        2 * C80 * std::pow(C200, 2) / std::pow(std::pow(C80, 2) + 1, 2);
    const double C370 = C363 / (std::pow(C81, 2) + 1) -
                        2 * C81 * std::pow(C201, 2) / std::pow(std::pow(C81, 2) + 1, 2);
    const double C371 = 2.2431064 * C370;
    const double C372 = 2.26214 * C370;
    const double C373 = 7.03496 * C369;
    const double C374 = 7.45488 * C369;
    const double C375 = C364 / C82 - std::pow(C202, 2) / std::pow(C82, 2);
    const double C376 = C365 / C83 - std::pow(C203, 2) / std::pow(C83, 2);
    const double C377 = C366 / C84 - std::pow(C204, 2) / std::pow(C84, 2);
    const double C378 = C367 / C85 - std::pow(C205, 2) / std::pow(C85, 2);
    const double C379 = std::sqrt(1 / 0.507386806551e2) * C371;
    const double C380 = std::sqrt(1 / 0.507386806551e2) * C372;
    const double C381 = std::sqrt(1 / 0.378469910464e2) * C373;
    const double C382 = std::sqrt(1 / 0.378469910464e2) * C374;
    const double C383 = C375 + C380;
    const double C384 = C376 + C382;
    const double C385 = C377 + C379;
    const double C386 = C378 + C381;
    const double C387 = -0.3913066512 * C386;
    const double C388 = -0.005382083488 * C385;
    const double C389 = C387 / 12.5549141492;
    const double C390 = C388 / 12.99914055888256;
    const double C391 = C384 - C389;
    const double C392 = C390 - C383;
    const double C393 = 0.0310907 * C391;
    matrix.upper[2] =
        (((((((((((((((((((((C46 *
                                 (0. * C110 *
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                  0. * std::pow(C47 / C46, 4)) /
                                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                             (C46 * C110 *
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  (0. * std::pow(C47 / C46, 3) * -4 * C47 /
                                       (C110 * std::pow(C46, 2)) +
                                   0. / C110) /
                                  (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                              C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  0. * std::pow(C47 / C46, 4) /
                                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                                 C174) +
                            C231 * C46 * C110 *
                                ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                C55 /
                                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                           C230 * C46 *
                               (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                    0. * std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                                ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                    std::pow(C47 / C46, 4) / C110) /
                               (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                          C179 * C46 *
                              ((0. * C110 *
                                    ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                0. * std::pow(C47 / C46, 4)) +
                               C110 *
                                   (C49 * std::pow(C58 + 1., C49 - 1) -
                                    C49 * std::pow(1. - C58, C49 - 1)) *
                                   0. * std::pow(C47 / C46, 4)) /
                              (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C181 * C46 * C110 *
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             (0. * std::pow(C47 / C46, 4) +
                              0. *
                                  (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                           (15.8687885 * std::sqrt(C60) * C78 *
                                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                  C55) /
                             (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        C198 * C46 * C110 *
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            (0. * std::pow(C47 / C46, 4) +
                             0. *
                                 ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                      (15.8687885 * std::pow(C78, 2) *
                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                                 C55) /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C206 * C46 * C110 *
                           ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                           (0. * std::pow(C47 / C46, 4) +
                            0. *
                                (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                 (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                     15.8687885) *
                                C55) /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C175 * C46 * C110 *
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                          (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
                          (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     (((((((((C46 *
                                  (C110 *
                                       ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                       0. * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                   0. * std::pow(C47 / C46, 4)) +
                              C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  0. * std::pow(C47 / C46, 4)) /
                                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                             C174 *
                                 ((C46 * C110 *
                                       ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                       ((std::pow(C46, 2) *
                                             ((1.7099209341613653 *
                                                   (0.01554535 *
                                                        ((std::log(C60 / C78) +
                                                          0.1412084e2 * C86 /
                                                              std::sqrt(0.223816694236e2)) -
                                                         -2.2946365 *
                                                             (std::log(
                                                                  std::pow(std::sqrt(C60) + 0.325,
                                                                           2) /
                                                                  C78) +
                                                              0.1282084e2 * C86 /
                                                                  std::sqrt(0.223816694236e2)) /
                                                             15.8687885) -
                                                    C111) *
                                                   C55 / C110 -
                                               1.) *
                                                  -4 * C47 * std::pow(C47 / C46, 2) * -3 * C47 /
                                                  std::pow(C46, 2) +
                                              0. * std::pow(C47 / C46, 3) * -4 * C47 / C110) -
                                         (1.7099209341613653 *
                                              (0.01554535 *
                                                   ((std::log(C60 / C78) +
                                                     0.1412084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) -
                                                    -2.2946365 *
                                                        (std::log(
                                                             std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                         0.1282084e2 * C86 /
                                                             std::sqrt(0.223816694236e2)) /
                                                        15.8687885) -
                                               C111) *
                                              C55 / C110 -
                                          1.) *
                                             std::pow(C47 / C46, 3) * -4 * C47 * 2 * C46) /
                                            std::pow(C46, 4) +
                                        0. * std::pow(C47 / C46, 3) * -4 * C47 /
                                            (std::pow(C46, 2) * C110)) +
                                   C110 *
                                       ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                       ((1.7099209341613653 *
                                             (0.01554535 *
                                                  ((std::log(C60 / C78) +
                                                    0.1412084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) -
                                                   -2.2946365 *
                                                       (std::log(
                                                            std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                        0.1282084e2 * C86 /
                                                            std::sqrt(0.223816694236e2)) /
                                                       15.8687885) -
                                              C111) *
                                             C55 / C110 -
                                         1.) *
                                            std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                        0. * std::pow(C47 / C46, 4) / C110)) +
                                  C110 *
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                      ((1.7099209341613653 *
                                            (0.01554535 *
                                                 ((std::log(C60 / C78) +
                                                   0.1412084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) -
                                                  -2.2946365 *
                                                      (std::log(
                                                           std::pow(std::sqrt(C60) + 0.325, 2) /
                                                           C78) +
                                                       0.1282084e2 * C86 /
                                                           std::sqrt(0.223816694236e2)) /
                                                      15.8687885) -
                                             C111) *
                                            C55 / C110 -
                                        1.) *
                                           std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                       0. * std::pow(C47 / C46, 4) / C110)) /
                                 (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                            (C46 * C110 *
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 -1.7099209341613653 * C55 * std::pow(C47 / C46, 3) * -4 * C47 /
                                 (std::pow(C46, 2) * C110 * 3.4198418683227306 *
                                  (std::pow(2., C48) - 1.) * C55) +
                             (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 /
                                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                              1)) *
                                C231) +
                           C230 *
                               (C46 *
                                    (C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         (0. * std::pow(C47 / C46, 4) +
                                          (0. * C110 -
                                           1.7099209341613653 *
                                               (0.01554535 *
                                                    ((std::log(C60 / C78) +
                                                      0.1412084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) -
                                                     -2.2946365 *
                                                         (std::log(
                                                              std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                          0.1282084e2 * C86 /
                                                              std::sqrt(0.223816694236e2)) /
                                                         15.8687885) -
                                                C111) *
                                               C55) *
                                              std::pow(C47 / C46, 3) * -4 * C47 /
                                              std::pow(C46, 2)) /
                                         std::pow(C110, 2) +
                                     ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                         ((1.7099209341613653 *
                                               (0.01554535 *
                                                    ((std::log(C60 / C78) +
                                                      0.1412084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) -
                                                     -2.2946365 *
                                                         (std::log(
                                                              std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                          0.1282084e2 * C86 /
                                                              std::sqrt(0.223816694236e2)) /
                                                         15.8687885) -
                                                C111) *
                                               C55 / C110 -
                                           1.) *
                                              std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                          0. * std::pow(C47 / C46, 4) / C110)) +
                                (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                     std::pow(C47 / C46, 4) *
                                     (0. * C110 -
                                      1.7099209341613653 *
                                          (0.01554535 *
                                               ((std::log(C60 / C78) +
                                                 0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                -2.2946365 *
                                                    (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                     0.1282084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) /
                                                    15.8687885) -
                                           C111) *
                                          C55) /
                                     std::pow(C110, 2) +
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                     ((1.7099209341613653 *
                                           (0.01554535 *
                                                ((std::log(C60 / C78) +
                                                  0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                 -2.2946365 *
                                                     (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                      0.1282084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) /
                                                     15.8687885) -
                                            C111) *
                                           C55 / C110 -
                                       1.) *
                                          std::pow(C47 / C46, 4) +
                                      1.))) /
                               (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                          C179 *
                              (C46 *
                                   ((C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         0. * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                     0. * std::pow(C47 / C46, 4)) /
                                        C110 +
                                    C110 *
                                        (C49 * std::pow(C58 + 1., C49 - 1) -
                                         C49 * std::pow(1. - C58, C49 - 1)) *
                                        ((1.7099209341613653 *
                                              (0.01554535 *
                                                   ((std::log(C60 / C78) +
                                                     0.1412084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) -
                                                    -2.2946365 *
                                                        (std::log(
                                                             std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                         0.1282084e2 * C86 /
                                                             std::sqrt(0.223816694236e2)) /
                                                        15.8687885) -
                                               C111) *
                                              C55 / C110 -
                                          1.) *
                                             std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                         0. * std::pow(C47 / C46, 4) / C110)) +
                               (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                    0. * std::pow(C47 / C46, 4) / C110 +
                                C110 *
                                    (C49 * std::pow(C58 + 1., C49 - 1) -
                                     C49 * std::pow(1. - C58, C49 - 1)) *
                                    ((1.7099209341613653 *
                                          (0.01554535 *
                                               ((std::log(C60 / C78) +
                                                 0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                -2.2946365 *
                                                    (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                     0.1282084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) /
                                                    15.8687885) -
                                           C111) *
                                          C55 / C110 -
                                      1.) *
                                         std::pow(C47 / C46, 4) +
                                     1.))) /
                              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C181 *
                             (C46 * C110 *
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  (0. * std::pow(C47 / C46, 4) +
                                   0.2658131939386538e-1 *
                                       (C78 / (C78 * C60) -
                                        -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                            (15.8687885 * std::sqrt(C60) * C78 *
                                             std::pow(std::sqrt(C60) + 0.325, 2))) *
                                       C55 * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2)) +
                              C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                  (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                           (15.8687885 * std::sqrt(C60) * C78 *
                                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                  C55) /
                             (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        C198 *
                            (C46 * C110 *
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 (0. * std::pow(C47 / C46, 4) +
                                  0.2658131939386538e-1 *
                                      ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                       2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                           (15.8687885 * std::pow(C78, 2) *
                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                      C55 * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2)) +
                             C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                 ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                      (15.8687885 * std::pow(C78, 2) *
                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                                 C55) /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C206 *
                           (C46 * C110 *
                                ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                (0. * std::pow(C47 / C46, 4) +
                                 0.2658131939386538e-1 *
                                     (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                      (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                          15.8687885) *
                                     C55 * std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2)) +
                            C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                 (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                     15.8687885) *
                                C55) /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C175 *
                          (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               ((C46 *
                                     ((1.7099209341613653 *
                                           (0.01554535 *
                                                ((std::log(C60 / C78) +
                                                  0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                 -2.2946365 *
                                                     (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                      0.1282084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) /
                                                     15.8687885) -
                                            C111) *
                                           C55 / C110 -
                                       1.) *
                                          4 * std::pow(C47 / C46, 2) * -3 * C47 / std::pow(C46, 2) +
                                      0. * std::pow(C47 / C46, 3) / C110) -
                                 (1.7099209341613653 *
                                      (0.01554535 *
                                           ((std::log(C60 / C78) +
                                             0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                            -2.2946365 *
                                                (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                          C78) +
                                                 0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                                15.8687885) -
                                       C111) *
                                      C55 / C110 -
                                  1.) *
                                     4 * std::pow(C47 / C46, 3)) /
                                    std::pow(C46, 2) +
                                0. * std::pow(C47 / C46, 3) * -4 * C47 /
                                    (std::pow(C46, 2) * C110)) +
                           C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               ((1.7099209341613653 *
                                     (0.01554535 *
                                          ((std::log(C60 / C78) +
                                            0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                           -2.2946365 *
                                               (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                         C78) +
                                                0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                               15.8687885) -
                                      C111) *
                                     C55 / C110 -
                                 1.) *
                                    4 * std::pow(C47 / C46, 3) / C46 +
                                0. * std::pow(C47 / C46, 4) / C110)) /
                          (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                         C174) +
                    (((((((((C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 * C46 *
                                 (0. * C110 *
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                      std::pow(C47 / C46, 4)) -
                             C46 * C110 *
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                 std::pow(C47 / C46, 4) *
                                 (0. * C110 +
                                  3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                                std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55,
                                         2) +
                            C174 *
                                (C46 *
                                     (C110 *
                                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                           2.) *
                                          (std::pow(C47 / C46, 3) * -4 * C47 *
                                               (0. * C110 -
                                                1.7099209341613653 *
                                                    (0.01554535 *
                                                         ((std::log(C60 / C78) +
                                                           0.1412084e2 * C86 /
                                                               std::sqrt(0.223816694236e2)) -
                                                          -2.2946365 *
                                                              (std::log(
                                                                   std::pow(std::sqrt(C60) + 0.325,
                                                                            2) /
                                                                   C78) +
                                                               0.1282084e2 * C86 /
                                                                   std::sqrt(0.223816694236e2)) /
                                                              15.8687885) -
                                                     C111) *
                                                    C55) /
                                               (std::pow(C110, 2) * std::pow(C46, 2)) +
                                           (0. * C110 - 0. * std::pow(C47 / C46, 4)) /
                                               std::pow(C110, 2)) +
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                          ((1.7099209341613653 *
                                                (0.01554535 *
                                                     ((std::log(C60 / C78) +
                                                       0.1412084e2 * C86 /
                                                           std::sqrt(0.223816694236e2)) -
                                                      -2.2946365 *
                                                          (std::log(
                                                               std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                           0.1282084e2 * C86 /
                                                               std::sqrt(0.223816694236e2)) /
                                                          15.8687885) -
                                                 C111) *
                                                C55 / C110 -
                                            1.) *
                                               std::pow(C47 / C46, 3) * -4 * C47 /
                                               std::pow(C46, 2) +
                                           0. * std::pow(C47 / C46, 4) / C110)) +
                                 (C110 *
                                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                      std::pow(C47 / C46, 4) *
                                      (0. * C110 -
                                       1.7099209341613653 *
                                           (0.01554535 *
                                                ((std::log(C60 / C78) +
                                                  0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                 -2.2946365 *
                                                     (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                      0.1282084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) /
                                                     15.8687885) -
                                            C111) *
                                           C55) /
                                      std::pow(C110, 2) +
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                      ((1.7099209341613653 *
                                            (0.01554535 *
                                                 ((std::log(C60 / C78) +
                                                   0.1412084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) -
                                                  -2.2946365 *
                                                      (std::log(
                                                           std::pow(std::sqrt(C60) + 0.325, 2) /
                                                           C78) +
                                                       0.1282084e2 * C86 /
                                                           std::sqrt(0.223816694236e2)) /
                                                      15.8687885) -
                                             C111) *
                                            C55 / C110 -
                                        1.) *
                                           std::pow(C47 / C46, 4) +
                                       1.))) /
                                (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                           C231 * C46 *
                               (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 *
                                    (C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         0. * C55 +
                                     ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                         std::pow(C47 / C46, 4) * -1.7099209341613653 * C55) -
                                C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                    std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 *
                                    (0. * C110 +
                                     3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                               std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55,
                                        2)) +
                          C230 * C46 *
                              ((std::pow(C110, 2) *
                                    (C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         0. * std::pow(C47 / C46, 4) +
                                     ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                         std::pow(C47 / C46, 4) *
                                         (0. * C110 -
                                          1.7099209341613653 *
                                              (0.01554535 *
                                                   ((std::log(C60 / C78) +
                                                     0.1412084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) -
                                                    -2.2946365 *
                                                        (std::log(
                                                             std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                         0.1282084e2 * C86 /
                                                             std::sqrt(0.223816694236e2)) /
                                                        15.8687885) -
                                               C111) *
                                              C55)) -
                                C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                    std::pow(C47 / C46, 4) *
                                    (0. * C110 -
                                     1.7099209341613653 *
                                         (0.01554535 *
                                              ((std::log(C60 / C78) +
                                                0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                               -2.2946365 *
                                                   (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                    0.1282084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) /
                                                   15.8687885) -
                                          C111) *
                                         C55) *
                                    2 * C110) /
                                   std::pow(C110, 4) +
                               ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   std::pow(C47 / C46, 4) *
                                   (0. * C110 -
                                    1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55) /
                                   std::pow(C110, 2)) /
                              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C179 * C46 *
                             ((C110 *
                                   (0. * C110 *
                                        ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                    ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                        0. * std::pow(C47 / C46, 4)) -
                               C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   0. * std::pow(C47 / C46, 4)) /
                                  std::pow(C110, 2) +
                              (C110 *
                                   (C49 * std::pow(C58 + 1., C49 - 1) -
                                    C49 * std::pow(1. - C58, C49 - 1)) *
                                   std::pow(C47 / C46, 4) *
                                   (0. * C110 -
                                    1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55) /
                                   std::pow(C110, 2) +
                               (C49 * std::pow(C58 + 1., C49 - 1) -
                                C49 * std::pow(1. - C58, C49 - 1)) *
                                   ((1.7099209341613653 *
                                         (0.01554535 *
                                              ((std::log(C60 / C78) +
                                                0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                               -2.2946365 *
                                                   (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                    0.1282084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) /
                                                   15.8687885) -
                                          C111) *
                                         C55 / C110 -
                                     1.) *
                                        std::pow(C47 / C46, 4) +
                                    1.))) /
                             (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 * C181 * C46 *
                             (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  (0. * std::pow(C47 / C46, 4) +
                                   0. *
                                       (C78 / (C78 * C60) -
                                        -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                            (15.8687885 * std::sqrt(C60) * C78 *
                                             std::pow(std::sqrt(C60) + 0.325, 2))) *
                                       C55) +
                              ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                  std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                  (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                           (15.8687885 * std::sqrt(C60) * C78 *
                                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                  C55) -
                         C181 * C46 * C110 *
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                             (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                      (15.8687885 * std::sqrt(C60) * C78 *
                                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                             C55 *
                             (0. * C110 + 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                            std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55,
                                     2)) +
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 * C198 * C46 *
                            (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 (0. * std::pow(C47 / C46, 4) +
                                  0. *
                                      ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                       2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                           (15.8687885 * std::pow(C78, 2) *
                                            std::pow(std::sqrt(C60) + 0.325, 2))) *
                                      C55) +
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                 ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                      (15.8687885 * std::pow(C78, 2) *
                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                                 C55) -
                        C198 * C46 * C110 *
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                            ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                             2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                 (15.8687885 * std::pow(C78, 2) *
                                  std::pow(std::sqrt(C60) + 0.325, 2))) *
                            C55 *
                            (0. * C110 + 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                           std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55,
                                    2)) +
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55 * C206 * C46 *
                           (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                (0. * std::pow(C47 / C46, 4) +
                                 0. *
                                     (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                      (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                          15.8687885) *
                                     C55) +
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                 (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                     15.8687885) *
                                C55) -
                       C206 * C46 * C110 *
                           ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                           std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                           (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                            (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                           C55 *
                           (0. * C110 + 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) /
                          std::pow(C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55, 2)) +
                     C175 * C46 *
                         (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              (4 * std::pow(C47 / C46, 3) *
                                   (0. * C110 -
                                    1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55) /
                                   (std::pow(C110, 2) * C46) +
                               (0. * C110 - 0. * std::pow(C47 / C46, 4)) / std::pow(C110, 2)) +
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              ((1.7099209341613653 *
                                    (0.01554535 *
                                         ((std::log(C60 / C78) +
                                           0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                          -2.2946365 *
                                              (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                               0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                              15.8687885) -
                                     C111) *
                                    C55 / C110 -
                                1.) *
                                   4 * std::pow(C47 / C46, 3) / C46 +
                               0. * std::pow(C47 / C46, 4) / C110)) /
                         (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                        C230) +
                   ((((((((C46 *
                               (0. * C110 *
                                    ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                                C110 *
                                    (C49 * std::pow(C58 + 1., C49 - 1) -
                                     C49 * std::pow(1. - C58, C49 - 1)) *
                                    0. * std::pow(C47 / C46, 4)) /
                               (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                           C174 *
                               (C46 *
                                    (C110 *
                                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) -
                                          2.) *
                                         (0. * std::pow(C47 / C46, 3) * -4 * C47 /
                                              (C110 * std::pow(C46, 2)) +
                                          0. / C110) +
                                     C110 *
                                         (C49 * std::pow(C58 + 1., C49 - 1) -
                                          C49 * std::pow(1. - C58, C49 - 1)) *
                                         ((1.7099209341613653 *
                                               (0.01554535 *
                                                    ((std::log(C60 / C78) +
                                                      0.1412084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) -
                                                     -2.2946365 *
                                                         (std::log(
                                                              std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                          0.1282084e2 * C86 /
                                                              std::sqrt(0.223816694236e2)) /
                                                         15.8687885) -
                                                C111) *
                                               C55 / C110 -
                                           1.) *
                                              std::pow(C47 / C46, 3) * -4 * C47 / std::pow(C46, 2) +
                                          0. * std::pow(C47 / C46, 4) / C110)) +
                                (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                     0. * std::pow(C47 / C46, 4) / C110 +
                                 C110 *
                                     (C49 * std::pow(C58 + 1., C49 - 1) -
                                      C49 * std::pow(1. - C58, C49 - 1)) *
                                     ((1.7099209341613653 *
                                           (0.01554535 *
                                                ((std::log(C60 / C78) +
                                                  0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                 -2.2946365 *
                                                     (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                               C78) +
                                                      0.1282084e2 * C86 /
                                                          std::sqrt(0.223816694236e2)) /
                                                     15.8687885) -
                                            C111) *
                                           C55 / C110 -
                                       1.) *
                                          std::pow(C47 / C46, 4) +
                                      1.))) /
                               (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                          C231 * C46 *
                              (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   0. * C55 +
                               C110 *
                                   (C49 * std::pow(C58 + 1., C49 - 1) -
                                    C49 * std::pow(1. - C58, C49 - 1)) *
                                   std::pow(C47 / C46, 4) * -1.7099209341613653 * C55) /
                              (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C230 * C46 *
                             ((C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   0. * std::pow(C47 / C46, 4) +
                               C110 *
                                   (C49 * std::pow(C58 + 1., C49 - 1) -
                                    C49 * std::pow(1. - C58, C49 - 1)) *
                                   std::pow(C47 / C46, 4) *
                                   (0. * C110 -
                                    1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55)) /
                                  std::pow(C110, 2) +
                              (((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                                   std::pow(C47 / C46, 4) / C110 +
                               (C49 * std::pow(C58 + 1., C49 - 1) -
                                C49 * std::pow(1. - C58, C49 - 1)) *
                                   ((1.7099209341613653 *
                                         (0.01554535 *
                                              ((std::log(C60 / C78) +
                                                0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                               -2.2946365 *
                                                   (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                    0.1282084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) /
                                                   15.8687885) -
                                          C111) *
                                         C55 / C110 -
                                     1.) *
                                        std::pow(C47 / C46, 4) +
                                    1.))) /
                             (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        C179 * C46 *
                            ((0. * C110 *
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                              C110 *
                                  (C49 * std::pow(C58 + 1., C49 - 1) -
                                   C49 * std::pow(1. - C58, C49 - 1)) *
                                  0. * std::pow(C47 / C46, 4)) /
                                 C110 +
                             (C110 *
                                  (C49 * std::pow(C58 + 1., C49 - 1) -
                                   C49 * std::pow(1. - C58, C49 - 1)) *
                                  0. * std::pow(C47 / C46, 4) / C110 +
                              C110 *
                                  (C49 * (C49 - 1) * std::pow(C58 + 1., C49 - 2) -
                                   C49 * (1 - C49) * std::pow(1. - C58, C49 - 2)) *
                                  ((1.7099209341613653 *
                                        (0.01554535 *
                                             ((std::log(C60 / C78) +
                                               0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                              -2.2946365 *
                                                  (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                            C78) +
                                                   0.1282084e2 * C86 /
                                                       std::sqrt(0.223816694236e2)) /
                                                  15.8687885) -
                                         C111) *
                                        C55 / C110 -
                                    1.) *
                                       std::pow(C47 / C46, 4) +
                                   1.))) /
                            (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C181 * C46 *
                           (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                (0. * std::pow(C47 / C46, 4) +
                                 0. *
                                     (C78 / (C78 * C60) -
                                      -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                          (15.8687885 * std::sqrt(C60) * C78 *
                                           std::pow(std::sqrt(C60) + 0.325, 2))) *
                                     C55) +
                            C110 *
                                (C49 * std::pow(C58 + 1., C49 - 1) -
                                 C49 * std::pow(1. - C58, C49 - 1)) *
                                std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                         (15.8687885 * std::sqrt(C60) * C78 *
                                                          std::pow(std::sqrt(C60) + 0.325, 2))) *
                                C55) /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C198 * C46 *
                          (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               (0. * std::pow(C47 / C46, 4) +
                                0. *
                                    ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                     2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                         (15.8687885 * std::pow(C78, 2) *
                                          std::pow(std::sqrt(C60) + 0.325, 2))) *
                                    C55) +
                           C110 *
                               (C49 * std::pow(C58 + 1., C49 - 1) -
                                C49 * std::pow(1. - C58, C49 - 1)) *
                               std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                               ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                    (15.8687885 * std::pow(C78, 2) *
                                     std::pow(std::sqrt(C60) + 0.325, 2))) *
                               C55) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C206 * C46 *
                         (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              (0. * std::pow(C47 / C46, 4) +
                               0. *
                                   (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                    (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                        15.8687885) *
                                   C55) +
                          C110 *
                              (C49 * std::pow(C58 + 1., C49 - 1) -
                               C49 * std::pow(1. - C58, C49 - 1)) *
                              std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                               (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                              C55) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C175 * C46 *
                        (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) +
                         C110 *
                             (C49 * std::pow(C58 + 1., C49 - 1) -
                              C49 * std::pow(1. - C58, C49 - 1)) *
                             ((1.7099209341613653 *
                                   (0.01554535 *
                                        ((std::log(C60 / C78) +
                                          0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                         -2.2946365 *
                                             (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                              0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                             15.8687885) -
                                    C111) *
                                   C55 / C110 -
                               1.) *
                                  4 * std::pow(C47 / C46, 3) / C46 +
                              0. * std::pow(C47 / C46, 4) / C110)) /
                        (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                       C179) +
                  ((((((((C46 *
                              (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   0. * std::pow(C47 / C46, 3) / C46 +
                               0. * std::pow(C47 / C46, 4)) /
                              (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                          C174 *
                              (C46 * C110 *
                                   ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   (((1.7099209341613653 *
                                          (0.01554535 *
                                               ((std::log(C60 / C78) +
                                                 0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                                -2.2946365 *
                                                    (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                              C78) +
                                                     0.1282084e2 * C86 /
                                                         std::sqrt(0.223816694236e2)) /
                                                    15.8687885) -
                                           C111) *
                                          C55 / C110 -
                                      1.) *
                                         (-4 * std::pow(C47 / C46, 3) +
                                          -4 * C47 * 3 * std::pow(C47 / C46, 2) / C46) +
                                     0. * std::pow(C47 / C46, 3) * -4 * C47 / C110) /
                                        std::pow(C46, 2) +
                                    0. * std::pow(C47 / C46, 3) / (C46 * C110)) +
                               C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                   ((1.7099209341613653 *
                                         (0.01554535 *
                                              ((std::log(C60 / C78) +
                                                0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                               -2.2946365 *
                                                   (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                             C78) +
                                                    0.1282084e2 * C86 /
                                                        std::sqrt(0.223816694236e2)) /
                                                   15.8687885) -
                                          C111) *
                                         C55 / C110 -
                                     1.) *
                                        4 * std::pow(C47 / C46, 3) / C46 +
                                    0. * std::pow(C47 / C46, 4) / C110)) /
                              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                         C231 * C46 * C110 *
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             -1.7099209341613653 * C55 * 4 * std::pow(C47 / C46, 3) /
                             (C46 * C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                        C230 * C46 *
                            (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 (0. * std::pow(C47 / C46, 4) +
                                  (0. * C110 -
                                   1.7099209341613653 *
                                       (0.01554535 *
                                            ((std::log(C60 / C78) +
                                              0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                             -2.2946365 *
                                                 (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                           C78) +
                                                  0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                                 15.8687885) -
                                        C111) *
                                       C55) *
                                      4 * std::pow(C47 / C46, 3) / C46) /
                                 std::pow(C110, 2) +
                             ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 ((1.7099209341613653 *
                                       (0.01554535 *
                                            ((std::log(C60 / C78) +
                                              0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                             -2.2946365 *
                                                 (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                           C78) +
                                                  0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                                 15.8687885) -
                                        C111) *
                                       C55 / C110 -
                                   1.) *
                                      4 * std::pow(C47 / C46, 3) / C46 +
                                  0. * std::pow(C47 / C46, 4) / C110)) /
                            (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C179 * C46 *
                           ((C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                 0. * std::pow(C47 / C46, 3) / C46 +
                             0. * std::pow(C47 / C46, 4)) /
                                C110 +
                            C110 *
                                (C49 * std::pow(C58 + 1., C49 - 1) -
                                 C49 * std::pow(1. - C58, C49 - 1)) *
                                ((1.7099209341613653 *
                                      (0.01554535 *
                                           ((std::log(C60 / C78) +
                                             0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                            -2.2946365 *
                                                (std::log(std::pow(std::sqrt(C60) + 0.325, 2) /
                                                          C78) +
                                                 0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                                15.8687885) -
                                       C111) *
                                      C55 / C110 -
                                  1.) *
                                     4 * std::pow(C47 / C46, 3) / C46 +
                                 0. * std::pow(C47 / C46, 4) / C110)) /
                           (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C181 * C46 * C110 *
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                          (0. * std::pow(C47 / C46, 4) +
                           0.2658131939386538e-1 *
                               (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                        (15.8687885 * std::sqrt(C60) * C78 *
                                                         std::pow(std::sqrt(C60) + 0.325, 2))) *
                               C55 * 4 * std::pow(C47 / C46, 3) / C46) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C198 * C46 * C110 *
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                         (0. * std::pow(C47 / C46, 4) +
                          0.2658131939386538e-1 *
                              ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                               2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                   (15.8687885 * std::pow(C78, 2) *
                                    std::pow(std::sqrt(C60) + 0.325, 2))) *
                              C55 * 4 * std::pow(C47 / C46, 3) / C46) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C206 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                        (0. * std::pow(C47 / C46, 4) +
                         0.2658131939386538e-1 *
                             (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                              (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                             C55 * 4 * std::pow(C47 / C46, 3) / C46) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C175 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (((1.7099209341613653 *
                              (0.01554535 *
                                   ((std::log(C60 / C78) +
                                     0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                    -2.2946365 *
                                        (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                         0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                        15.8687885) -
                               C111) *
                              C55 / C110 -
                          1.) *
                             12 * std::pow(C47 / C46, 2) / C46 +
                         0. * std::pow(C47 / C46, 3) / C110) /
                            C46 +
                        0. * std::pow(C47 / C46, 3) / (C46 * C110)) /
                       (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                      C175) +
                 ((((((((C46 *
                             (0. * C110 *
                                  ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                              0. * std::pow(C47 / C46, 4)) /
                             (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                         (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              (std::pow(C47 / C46, 3) * -4 * C47 * 0.2658131939386538e-1 *
                                   (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) *
                                                            C78 /
                                                            (15.8687885 * std::sqrt(C60) * C78 *
                                                             std::pow(std::sqrt(C60) + 0.325, 2))) *
                                   C55 / (C110 * std::pow(C46, 2)) +
                               0. / C110) /
                              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                          C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                       (15.8687885 * std::sqrt(C60) * C78 *
                                                        std::pow(std::sqrt(C60) + 0.325, 2))) *
                              C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                             C174) +
                        C231 * C46 * C110 *
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                       C230 * C46 *
                           (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) *
                                (0. - 0.2658131939386538e-1 *
                                          (C78 / (C78 * C60) -
                                           -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                               (15.8687885 * std::sqrt(C60) * C78 *
                                                std::pow(std::sqrt(C60) + 0.325, 2))) *
                                          C55) /
                                std::pow(C110, 2) +
                            ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                                (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                         (15.8687885 * std::sqrt(C60) * C78 *
                                                          std::pow(std::sqrt(C60) + 0.325, 2))) *
                                C55 / C110) /
                           (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C179 * C46 *
                          ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                            0. * std::pow(C47 / C46, 4)) +
                           C110 *
                               (C49 * std::pow(C58 + 1., C49 - 1) -
                                C49 * std::pow(1. - C58, C49 - 1)) *
                               std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                               (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                        (15.8687885 * std::sqrt(C60) * C78 *
                                                         std::pow(std::sqrt(C60) + 0.325, 2))) *
                               C55) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C181 * C46 * C110 *
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                         (std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              ((-std::pow(C78, 2)) / std::pow(C78 * C60, 2) -
                               (15.8687885 * std::sqrt(C60) * C78 *
                                    std::pow(std::sqrt(C60) + 0.325, 2) * -2.2946365 * C78 /
                                    (2 * std::sqrt(C60)) -
                                -2.2946365 * (std::sqrt(C60) + 0.325) * C78 * 15.8687885 *
                                    (std::sqrt(C60) * C78 * (std::sqrt(C60) + 0.325) /
                                         std::sqrt(C60) +
                                     std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                         (2 * std::sqrt(C60)))) /
                                   std::pow(15.8687885 * std::sqrt(C60) * C78 *
                                                std::pow(std::sqrt(C60) + 0.325, 2),
                                            2)) *
                              C55 +
                          0. *
                              (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                       (15.8687885 * std::sqrt(C60) * C78 *
                                                        std::pow(std::sqrt(C60) + 0.325, 2))) *
                              C55) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C198 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                        (std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                             ((C60 * std::pow(C78, 3) - std::pow(C78, 2) * C60 * C78) /
                                  std::pow(std::pow(C78, 2) * C60, 2) -
                              (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2) *
                                   2.2946365 * C78 * (std::sqrt(C60) + 0.325) / std::sqrt(C60) -
                               2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 * 15.8687885 *
                                   std::pow(C78, 2) * (std::sqrt(C60) + 0.325) / std::sqrt(C60)) /
                                  std::pow(15.8687885 * std::pow(C78, 2) *
                                               std::pow(std::sqrt(C60) + 0.325, 2),
                                           2)) *
                             C55 +
                         0. *
                             ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                              2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                  (15.8687885 * std::pow(C78, 2) *
                                   std::pow(std::sqrt(C60) + 0.325, 2))) *
                             C55) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C206 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (0. * std::pow(C47 / C46, 4) +
                        0. *
                            (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                             (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                            C55) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C175 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (4 * std::pow(C47 / C46, 3) * 0.2658131939386538e-1 *
                           (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                    (15.8687885 * std::sqrt(C60) * C78 *
                                                     std::pow(std::sqrt(C60) + 0.325, 2))) *
                           C55 / (C110 * C46) +
                       0. / C110) /
                      (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                     C181) +
                ((((((((C46 *
                            (0. * C110 *
                                 ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                             0. * std::pow(C47 / C46, 4)) /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                        (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             (std::pow(C47 / C46, 3) * -4 * C47 * 0.2658131939386538e-1 *
                                  ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                   2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                       (15.8687885 * std::pow(C78, 2) *
                                        std::pow(std::sqrt(C60) + 0.325, 2))) *
                                  C55 / (C110 * std::pow(C46, 2)) +
                              0. / C110) /
                             (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                         C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                             ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                              2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                  (15.8687885 * std::pow(C78, 2) *
                                   std::pow(std::sqrt(C60) + 0.325, 2))) *
                             C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                            C174) +
                       C231 * C46 * C110 *
                           ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                      C230 * C46 *
                          (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               std::pow(C47 / C46, 4) *
                               (0. - 0.2658131939386538e-1 *
                                         ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                          2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                              (15.8687885 * std::pow(C78, 2) *
                                               std::pow(std::sqrt(C60) + 0.325, 2))) *
                                         C55) /
                               std::pow(C110, 2) +
                           ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                               std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                               ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                                2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                    (15.8687885 * std::pow(C78, 2) *
                                     std::pow(std::sqrt(C60) + 0.325, 2))) *
                               C55 / C110) /
                          (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C179 * C46 *
                         ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                           0. * std::pow(C47 / C46, 4)) +
                          C110 *
                              (C49 * std::pow(C58 + 1., C49 - 1) -
                               C49 * std::pow(1. - C58, C49 - 1)) *
                              std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                               2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                   (15.8687885 * std::pow(C78, 2) *
                                    std::pow(std::sqrt(C60) + 0.325, 2))) *
                              C55) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C181 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                        (std::pow(C47 / C46, 4) * -0.2658131939386538e-1 * C55 *
                             (15.8687885 * std::sqrt(C60) * C78 *
                                  std::pow(std::sqrt(C60) + 0.325, 2) * -2.2946365 *
                                  (std::sqrt(C60) + 0.325) -
                              -2.2946365 * (std::sqrt(C60) + 0.325) * C78 * 15.8687885 *
                                  std::sqrt(C60) * std::pow(std::sqrt(C60) + 0.325, 2)) /
                             std::pow(15.8687885 * std::sqrt(C60) * C78 *
                                          std::pow(std::sqrt(C60) + 0.325, 2),
                                      2) +
                         0. *
                             (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                      (15.8687885 * std::sqrt(C60) * C78 *
                                                       std::pow(std::sqrt(C60) + 0.325, 2))) *
                             C55) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C198 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                            ((C60 * C78 * 2 * C78 * C60 - std::pow(C78, 2) * std::pow(C60, 2)) /
                                 std::pow(std::pow(C78, 2) * C60, 2) -
                             (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2) *
                                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) -
                              2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 * 0.31737577e2 *
                                  C78 * std::pow(std::sqrt(C60) + 0.325, 2)) /
                                 std::pow(15.8687885 * std::pow(C78, 2) *
                                              std::pow(std::sqrt(C60) + 0.325, 2),
                                          2)) *
                            C55 +
                        0. *
                            ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                             2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                 (15.8687885 * std::pow(C78, 2) *
                                  std::pow(std::sqrt(C60) + 0.325, 2))) *
                            C55) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C206 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (0. * std::pow(C47 / C46, 4) +
                       0. *
                           (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                            (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                           C55) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C175 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (4 * std::pow(C47 / C46, 3) * 0.2658131939386538e-1 *
                          ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                           2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                               (15.8687885 * std::pow(C78, 2) *
                                std::pow(std::sqrt(C60) + 0.325, 2))) *
                          C55 / (C110 * C46) +
                      0. / C110) /
                     (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                    C198) +
               ((((((((C46 *
                           (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                            0. * std::pow(C47 / C46, 4)) /
                           (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                       (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            (std::pow(C47 / C46, 3) * -4 * C47 * 0.2658131939386538e-1 *
                                 (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                  (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                      15.8687885) *
                                 C55 / (C110 * std::pow(C46, 2)) +
                             0. / C110) /
                            (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                        C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                            (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                             (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                            C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                           C174) +
                      C231 * C46 * C110 *
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                     C230 * C46 *
                         (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              std::pow(C47 / C46, 4) *
                              (0. - 0.2658131939386538e-1 *
                                        (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                                         (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) /
                                             15.8687885) *
                                        C55) /
                              std::pow(C110, 2) +
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                              (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                               (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                              C55 / C110) /
                         (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C179 * C46 *
                        ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                          0. * std::pow(C47 / C46, 4)) +
                         C110 *
                             (C49 * std::pow(C58 + 1., C49 - 1) -
                              C49 * std::pow(1. - C58, C49 - 1)) *
                             std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                             (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                              (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                             C55) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C181 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (0. * std::pow(C47 / C46, 4) +
                        0. *
                            (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                     (15.8687885 * std::sqrt(C60) * C78 *
                                                      std::pow(std::sqrt(C60) + 0.325, 2))) *
                            C55) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C198 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (0. * std::pow(C47 / C46, 4) +
                       0. *
                           ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                            2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                                (15.8687885 * std::pow(C78, 2) *
                                 std::pow(std::sqrt(C60) + 0.325, 2))) *
                           C55) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C206 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (0. * std::pow(C47 / C46, 4) +
                      0. *
                          (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                           (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                          C55) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C175 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    (4 * std::pow(C47 / C46, 3) * 0.2658131939386538e-1 *
                         (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                          (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                         C55 / (C110 * C46) +
                     0. / C110) /
                    (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                   C206) +
              ((((((((C46 *
                          (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                           0. * std::pow(C47 / C46, 4)) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                      (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                           (std::pow(C47 / C46, 3) * -4 * C47 * -1.7099209341613653 * C55 /
                                (C110 * std::pow(C46, 2)) +
                            0. / C110) /
                           (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                       (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                            std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 /
                            (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                        1)) *
                          C174) +
                     C231 * C46 * C110 *
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                    C230 * C46 *
                        (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             std::pow(C47 / C46, 4) * (0. - -1.7099209341613653 * C55) /
                             std::pow(C110, 2) +
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                             std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 / C110) /
                        (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                   C179 * C46 *
                       ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                         0. * std::pow(C47 / C46, 4)) +
                        C110 *
                            (C49 * std::pow(C58 + 1., C49 - 1) -
                             C49 * std::pow(1. - C58, C49 - 1)) *
                            std::pow(C47 / C46, 4) * -1.7099209341613653 * C55) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C181 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (0. * std::pow(C47 / C46, 4) +
                       0. *
                           (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                    (15.8687885 * std::sqrt(C60) * C78 *
                                                     std::pow(std::sqrt(C60) + 0.325, 2))) *
                           C55) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C198 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (0. * std::pow(C47 / C46, 4) +
                      0. *
                          ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                           2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                               (15.8687885 * std::pow(C78, 2) *
                                std::pow(std::sqrt(C60) + 0.325, 2))) *
                          C55) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C206 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    (0. * std::pow(C47 / C46, 4) +
                     0. *
                         (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                          (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                         C55) /
                    (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C175 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (4 * std::pow(C47 / C46, 3) * -1.7099209341613653 * C55 / (C110 * C46) +
                    0. / C110) /
                   (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                  C231) +
             ((((((((C46 *
                         (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                          0. * std::pow(C47 / C46, 4)) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                     (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                          (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                           0. / C110) /
                          (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                      C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                          std::pow(C47 / C46, 4) /
                          (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                         C174) +
                    (C46 * (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                                std::pow(C47 / C46, 4) * -1.7099209341613653 * C55 /
                                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                            1) +
                     C231 * C46 * C110 *
                         ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. * C55 /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55))) +
                   C230 * C46 *
                       (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                            std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                        ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                            std::pow(C47 / C46, 4) / C110) /
                       (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  C179 * C46 *
                      ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                        0. * std::pow(C47 / C46, 4)) +
                       C110 *
                           (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                           0. * std::pow(C47 / C46, 4)) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C181 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (0. * std::pow(C47 / C46, 4) +
                      0. *
                          (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                   (15.8687885 * std::sqrt(C60) * C78 *
                                                    std::pow(std::sqrt(C60) + 0.325, 2))) *
                          C55) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C198 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    (0. * std::pow(C47 / C46, 4) +
                     0. *
                         ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                          2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                              (15.8687885 * std::pow(C78, 2) *
                               std::pow(std::sqrt(C60) + 0.325, 2))) *
                         C55) /
                    (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C206 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (0. * std::pow(C47 / C46, 4) +
                    0. *
                        (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                         (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                        C55) /
                   (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C175 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                  (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
                  (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                 C393) +
            ((((((((C46 *
                        (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                         0. * std::pow(C47 / C46, 4)) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                    (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                         (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                          0. / C110) /
                         (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                     C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                         std::pow(C47 / C46, 4) /
                         (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                        C174) +
                   C231 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       0. * C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                  (C230 * C46 *
                       (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                            std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                        ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                            std::pow(C47 / C46, 4) / C110) +
                   C46 * (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              std::pow(C47 / C46, 4) *
                              (0. * C110 -
                               1.7099209341613653 *
                                   (0.01554535 *
                                        ((std::log(C60 / C78) +
                                          0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                         -2.2946365 *
                                             (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                              0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                             15.8687885) -
                                    C111) *
                                   C55) /
                              std::pow(C110, 2) +
                          ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                              ((1.7099209341613653 *
                                    (0.01554535 *
                                         ((std::log(C60 / C78) +
                                           0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                          -2.2946365 *
                                              (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                               0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                              15.8687885) -
                                     C111) *
                                    C55 / C110 -
                                1.) *
                                   std::pow(C47 / C46, 4) +
                               1.))) /
                      (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C179 * C46 *
                     ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                       0. * std::pow(C47 / C46, 4)) +
                      C110 *
                          (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                          0. * std::pow(C47 / C46, 4)) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C181 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    (0. * std::pow(C47 / C46, 4) +
                     0. *
                         (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                  (15.8687885 * std::sqrt(C60) * C78 *
                                                   std::pow(std::sqrt(C60) + 0.325, 2))) *
                         C55) /
                    (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C198 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (0. * std::pow(C47 / C46, 4) +
                    0. *
                        ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                         2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                             (15.8687885 * std::pow(C78, 2) *
                              std::pow(std::sqrt(C60) + 0.325, 2))) *
                        C55) /
                   (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C206 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                  (0. * std::pow(C47 / C46, 4) +
                   0. *
                       (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                        (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                       C55) /
                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C175 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
                 (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                C392) +
           ((((((((C46 *
                       (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                        0. * std::pow(C47 / C46, 4)) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                   (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                        (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                         0. / C110) /
                        (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                    C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                        std::pow(C47 / C46, 4) /
                        (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                       C174) +
                  C231 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      0. * C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                 C230 * C46 *
                     (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                          std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                      ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                          std::pow(C47 / C46, 4) / C110) /
                     (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                (C179 * C46 *
                     ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                       0. * std::pow(C47 / C46, 4)) +
                      C110 *
                          (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                          0. * std::pow(C47 / C46, 4)) /
                     C110 +
                 C46 *
                     (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                          std::pow(C47 / C46, 4) / C110 +
                      C110 *
                          (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                          ((1.7099209341613653 *
                                (0.01554535 *
                                     ((std::log(C60 / C78) +
                                       0.1412084e2 * C86 / std::sqrt(0.223816694236e2)) -
                                      -2.2946365 *
                                          (std::log(std::pow(std::sqrt(C60) + 0.325, 2) / C78) +
                                           0.1282084e2 * C86 / std::sqrt(0.223816694236e2)) /
                                          15.8687885) -
                                 C111) *
                                C55 / C110 -
                            1.) *
                               std::pow(C47 / C46, 4) +
                           1.))) /
                    (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C181 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (0. * std::pow(C47 / C46, 4) +
                    0. *
                        (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                 (15.8687885 * std::sqrt(C60) * C78 *
                                                  std::pow(std::sqrt(C60) + 0.325, 2))) *
                        C55) /
                   (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C198 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                  (0. * std::pow(C47 / C46, 4) +
                   0. *
                       ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                        2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                            (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                       C55) /
                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C206 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 4) +
                  0. *
                      (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                       (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                      C55) /
                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
            C175 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
                (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
               C341) +
          ((((((((C46 *
                      (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                       0. * std::pow(C47 / C46, 4)) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                  (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                       (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                        0. / C110) /
                       (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                   C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                       std::pow(C47 / C46, 4) /
                       (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                      C174) +
                 C231 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     0. * C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
                C230 * C46 *
                    (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                         std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                     ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                         std::pow(C47 / C46, 4) / C110) /
                    (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C179 * C46 *
                   ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                     0. * std::pow(C47 / C46, 4)) +
                    C110 * (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                        0. * std::pow(C47 / C46, 4)) /
                   (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              (C181 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   (0. * std::pow(C47 / C46, 4) +
                    0. *
                        (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                                 (15.8687885 * std::sqrt(C60) * C78 *
                                                  std::pow(std::sqrt(C60) + 0.325, 2))) *
                        C55) +
               C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                   std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                   (C78 / (C78 * C60) -
                    -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                        (15.8687885 * std::sqrt(C60) * C78 * std::pow(std::sqrt(C60) + 0.325, 2))) *
                   C55) /
                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C198 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 4) +
                  0. *
                      ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                       2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                           (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                      C55) /
                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
            C206 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                (0. * std::pow(C47 / C46, 4) +
                 0. *
                     (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                      (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                     C55) /
                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
           C175 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
               (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
              C343) +
         ((((((((C46 *
                     (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                      0. * std::pow(C47 / C46, 4)) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                 (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                      (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                       0. / C110) /
                      (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                  C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                      std::pow(C47 / C46, 4) /
                      (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                     C174) +
                C231 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                    0. * C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
               C230 * C46 *
                   (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                        std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                    ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                        std::pow(C47 / C46, 4) / C110) /
                   (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C179 * C46 *
                  ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                    0. * std::pow(C47 / C46, 4)) +
                   C110 * (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                       0. * std::pow(C47 / C46, 4)) /
                  (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C181 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 4) +
                  0. *
                      (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                               (15.8687885 * std::sqrt(C60) * C78 *
                                                std::pow(std::sqrt(C60) + 0.325, 2))) *
                      C55) /
                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
            (C198 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 (0. * std::pow(C47 / C46, 4) +
                  0. *
                      ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                       2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                           (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                      C55) +
             C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                 std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
                 ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                  2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                      (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                 C55) /
                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
           C206 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               (0. * std::pow(C47 / C46, 4) +
                0. *
                    (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                     (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                    C55) /
               (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
          C175 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
              (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
              (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
             C360) +
        ((((((((C46 *
                    (0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                     0. * std::pow(C47 / C46, 4)) /
                    (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                (C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                     (0. * std::pow(C47 / C46, 3) * -4 * C47 / (C110 * std::pow(C46, 2)) +
                      0. / C110) /
                     (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55) +
                 C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                     std::pow(C47 / C46, 4) /
                     (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
                    C174) +
               C231 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                   C55 / (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
              C230 * C46 *
                  (C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                       std::pow(C47 / C46, 4) / std::pow(C110, 2) +
                   ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) * 0. *
                       std::pow(C47 / C46, 4) / C110) /
                  (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
             C179 * C46 *
                 ((0. * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) +
                   0. * std::pow(C47 / C46, 4)) +
                  C110 * (C49 * std::pow(C58 + 1., C49 - 1) - C49 * std::pow(1. - C58, C49 - 1)) *
                      0. * std::pow(C47 / C46, 4)) /
                 (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
            C181 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
                (0. * std::pow(C47 / C46, 4) +
                 0. *
                     (C78 / (C78 * C60) - -2.2946365 * (std::sqrt(C60) + 0.325) * C78 /
                                              (15.8687885 * std::sqrt(C60) * C78 *
                                               std::pow(std::sqrt(C60) + 0.325, 2))) *
                     C55) /
                (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
           C198 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               (0. * std::pow(C47 / C46, 4) +
                0. *
                    ((-C60 * C78) / (std::pow(C78, 2) * C60) -
                     2.2946365 * std::pow(std::sqrt(C60) + 0.325, 2) * C78 /
                         (15.8687885 * std::pow(C78, 2) * std::pow(std::sqrt(C60) + 0.325, 2))) *
                    C55) /
               (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
          (C206 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               (0. * std::pow(C47 / C46, 4) +
                0. *
                    (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                     (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
                    C55) +
           C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
               std::pow(C47 / C46, 4) * 0.2658131939386538e-1 *
               (std::sqrt(0.1993981223056e3 / 0.223816694236e2) -
                (-std::sqrt(0.8654874119601761e3 / 0.223816694236e2)) / 15.8687885) *
               C55) /
              (C110 * 3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) +
         C175 * C46 * C110 * ((std::pow(C58 + 1., C49) + std::pow(1. - C58, C49)) - 2.) *
             (0. * std::pow(C47 / C46, 3) / (C110 * C46) + 0. / C110) /
             (3.4198418683227306 * (std::pow(2., C48) - 1.) * C55)) *
            C368;
}

} // namespace excgrid
