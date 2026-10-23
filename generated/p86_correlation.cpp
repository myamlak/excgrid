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
// The functional's SECOND-DERIVATIVE tier is emitted beside the kernel from the
// same two expressions, and carries the same branch: the matrix is one
// functional's, so a request must be answered by the branch the point is in and
// not by either branch alone.  It is the contract's upper triangle, row-major
// over the five components the kernel takes, in identifier order.
// The banner and the kernel.hpp include are emitted by the calling .ey, which
// always precedes this skeleton, so they are not repeated here.

#include <cmath>
#include <limits>

namespace excgrid {

XcKernelValue P86Correlation(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;
    // Declared where the straight-line skeleton declares it, and for the same
    // reason: the generator's sigma-edge guard needs a constant too small for
    // the printer to write into an expression.  Both branches of the tree's
    // one piecewise functional now carry the guarded total-gradient radical
    // (the owner ruling of 2026-10-19), and every future branch that does will
    // find it here.
    [[maybe_unused]] constexpr double SigmaGuard = 1e-300;

    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        return result;
    }

    if (!(std::pow(3. / (4. * Pi * (rhoA + rhoB)), 1. / 3.) < 1.))
    {
        const double C522 = 2. * sigmaAb;
        const double C523 = rhoA + rhoB;
        const double C524 = rhoA - rhoB;
        const double C525 = 1. / 3.;
        const double C526 = 2. / 3.;
        const double C527 = 4. / 3.;
        const double C528 = 5. / 3.;
        const double C529 = 0.9999999999999999 * C524;
        const double C530 = Pi * C523;
        const double C531 = sigmaAa + C522;
        const double C532 = 4. * C530;
        const double C533 = C531 + sigmaBb;
        const double C534 = C529 / C523;
        const double C535 = C534 + 1.;
        const double C536 = 1. - C534;
        const double C537 = 0.22167 / C532;
        const double C538 = 3. / C532;
        const double C539 = std::pow(C538, C525);
        const double C540 = std::pow(C538, C526);
        const double C541 = 7.389e-6 * C540;
        const double C542 = 0.023266 * C539;
        const double C543 = 0.3334 * C539;
        const double C544 = 0.472 * C540;
        const double C545 = 8.723 * C539;
        const double C546 = std::sqrt(C539);
        const double C547 = 1.0529 * C546;
        const double C548 = C542 + C541;
        const double C549 = C545 + C544;
        const double C550 = C547 + C543;
        const double C551 = C548 + 0.002568;
        const double C552 = C549 + C537;
        const double C553 = C550 + 1.;
        const double C554 = C552 + 1.;
        const double C555 = 0.1423 / C553;
        const double C556 = C551 / C554;
        const double C557 = C556 + 0.001667;

        result.exc =
            C523 * (((C555 - 0.0843 / ((1.3981 * C546 + 0.2611 * C539) + 1.)) *
                         ((std::pow(C535, C527) + std::pow(C536, C527)) - 2.) /
                         (2. * (std::pow(2., C525) - 1.)) -
                     C555) +
                    std::exp(-0.00081290825 * std::sqrt(C533) / (std::pow(C523, 7. / 6.) * C557)) *
                        C557 * C533 /
                        (std::pow(C523, 7. / 3.) *
                         std::sqrt((std::pow(C535, C528) + std::pow(C536, C528)) / 2.)));

        const double C559 = -24. * Pi;
        const double C560 = 0.2611 * C539;
        const double C561 = 0.88668 * Pi;
        const double C562 = 0.9999999999999999 * C523;
        const double C563 = 1.3981 * C546;
        const double C564 = 2 * C546;
        const double C565 = 12. * Pi;
        const double C566 = -2. / 3.;
        const double C567 = -1. / 3.;
        const double C568 = 7. / 3.;
        const double C569 = 7. / 6.;
        const double C570 = std::sqrt(C533);
        const double C571 = std::pow(2., C525);
        const double C572 = std::pow(C523, 2);
        const double C573 = std::pow(C532, 2);
        const double C574 = std::pow(C535, C527);
        const double C575 = std::pow(C535, C528);
        const double C576 = std::pow(C536, C527);
        const double C577 = std::pow(C536, C528);
        const double C578 = std::pow(C553, 2);
        const double C579 = std::pow(C554, 2);
        const double C580 = 0.00081290825 * C570;
        const double C581 = 3. * C573;
        const double C582 = C573 * C564;
        const double C583 = C563 + C560;
        const double C584 = C574 + C576;
        const double C585 = C575 + C577;
        const double C586 = C562 - C529;
        const double C587 = C571 - 1.;
        const double C588 = C561 / C573;
        const double C589 = std::pow(C523, C568);
        const double C590 = std::pow(C523, C569);
        const double C591 = std::pow(C538, C566);
        const double C592 = std::pow(C538, C567);
        const double C593 = 2. * C587;
        const double C594 = 3. * C582;
        const double C595 = C590 * C557;
        const double C596 = C591 * C565;
        const double C597 = C592 * C559;
        const double C598 = C583 + 1.;
        const double C599 = C584 - 2.;
        const double C600 = C585 / 2.;
        const double C601 = -8.723 * C596;
        const double C602 = -1.0529 * C596;
        const double C603 = -0.3334 * C596;
        const double C604 = -0.023266 * C596;
        const double C605 = 7.389e-6 * C597;
        const double C606 = 0.472 * C597;
        const double C607 = 0.0843 / C598;
        const double C608 = C580 / C595;
        const double C609 = std::sqrt(C600);
        const double C610 = C589 * C609;
        const double C611 = C601 + C606;
        const double C612 = C604 + C605;
        const double C613 = C555 - C607;
        const double C614 = -C608;
        const double C615 = C602 / C594;
        const double C616 = C603 / C581;
        const double C617 = C554 * C612;
        const double C618 = C615 + C616;
        const double C619 = C611 / C581;
        const double C620 = std::exp(C614);
        const double C621 = 0.1423 * C618;
        const double C622 = C620 * C557;
        const double C623 = C619 - C588;
        const double C624 = C617 / C581;
        const double C625 = C551 * C623;
        const double C626 = C622 * C533;
        const double C627 = -C621;
        const double C628 = C624 - C625;
        const double C629 = C627 / C578;

        result.vrhoA =
            C523 *
                (((C613 * (std::pow(C535, C525) * 4. * C586 + std::pow(C536, C525) * -4. * C586) /
                       (3. * C572) +
                   (C629 - (-0.0843 * (-1.3981 * C596 / C594 + -0.2611 * C596 / C581)) /
                               std::pow(C598, 2)) *
                       C599) /
                      C593 -
                  C629) +
                 (C610 *
                      (C620 * C628 / C579 +
                       C557 * C620 * 0.00081290825 * C570 *
                           (C590 * C628 / C579 + C557 * 7. * std::pow(C523, 1. / 6.) / 6.) /
                           std::pow(C595, 2)) *
                      C533 -
                  C626 *
                      (C589 *
                           (std::pow(C535, C526) * 5. * C586 + std::pow(C536, C526) * -5. * C586) /
                           (6. * C572 * 2 * C609) +
                       C609 * 7. * std::pow(C523, C527) / 3.)) /
                     std::pow(C610, 2)) +
            ((C613 * C599 / C593 - C555) + C626 / C610);

        const double C631 = -24. * Pi;
        const double C632 = -0.9999999999999999 * C523;
        const double C633 = 0.2611 * C539;
        const double C634 = 0.88668 * Pi;
        const double C635 = 1.3981 * C546;
        const double C636 = 2 * C546;
        const double C637 = 12. * Pi;
        const double C638 = -2. / 3.;
        const double C639 = -1. / 3.;
        const double C640 = 7. / 3.;
        const double C641 = 7. / 6.;
        const double C642 = std::sqrt(C533);
        const double C643 = std::pow(2., C525);
        const double C644 = std::pow(C523, 2);
        const double C645 = std::pow(C532, 2);
        const double C646 = std::pow(C535, C527);
        const double C647 = std::pow(C535, C528);
        const double C648 = std::pow(C536, C527);
        const double C649 = std::pow(C536, C528);
        const double C650 = std::pow(C553, 2);
        const double C651 = std::pow(C554, 2);
        const double C652 = 0.00081290825 * C642;
        const double C653 = 3. * C645;
        const double C654 = C645 * C636;
        const double C655 = C635 + C633;
        const double C656 = C646 + C648;
        const double C657 = C647 + C649;
        const double C658 = C632 - C529;
        const double C659 = C643 - 1.;
        const double C660 = C634 / C645;
        const double C661 = std::pow(C523, C640);
        const double C662 = std::pow(C523, C641);
        const double C663 = std::pow(C538, C638);
        const double C664 = std::pow(C538, C639);
        const double C665 = 2. * C659;
        const double C666 = 3. * C654;
        const double C667 = C662 * C557;
        const double C668 = C663 * C637;
        const double C669 = C664 * C631;
        const double C670 = C655 + 1.;
        const double C671 = C656 - 2.;
        const double C672 = C657 / 2.;
        const double C673 = -8.723 * C668;
        const double C674 = -1.0529 * C668;
        const double C675 = -0.3334 * C668;
        const double C676 = -0.023266 * C668;
        const double C677 = 7.389e-6 * C669;
        const double C678 = 0.472 * C669;
        const double C679 = 0.0843 / C670;
        const double C680 = C652 / C667;
        const double C681 = std::sqrt(C672);
        const double C682 = C661 * C681;
        const double C683 = C673 + C678;
        const double C684 = C676 + C677;
        const double C685 = C555 - C679;
        const double C686 = -C680;
        const double C687 = C674 / C666;
        const double C688 = C675 / C653;
        const double C689 = C554 * C684;
        const double C690 = C687 + C688;
        const double C691 = C683 / C653;
        const double C692 = std::exp(C686);
        const double C693 = 0.1423 * C690;
        const double C694 = C692 * C557;
        const double C695 = C691 - C660;
        const double C696 = C689 / C653;
        const double C697 = C551 * C695;
        const double C698 = C694 * C533;
        const double C699 = -C693;
        const double C700 = C696 - C697;
        const double C701 = C699 / C650;

        result.vrhoB =
            C523 *
                (((C685 * (std::pow(C535, C525) * 4. * C658 + std::pow(C536, C525) * -4. * C658) /
                       (3. * C644) +
                   (C701 - (-0.0843 * (-1.3981 * C668 / C666 + -0.2611 * C668 / C653)) /
                               std::pow(C670, 2)) *
                       C671) /
                      C665 -
                  C701) +
                 (C682 *
                      (C692 * C700 / C651 +
                       C557 * C692 * 0.00081290825 * C642 *
                           (C662 * C700 / C651 + C557 * 7. * std::pow(C523, 1. / 6.) / 6.) /
                           std::pow(C667, 2)) *
                      C533 -
                  C698 *
                      (C661 *
                           (std::pow(C535, C526) * 5. * C658 + std::pow(C536, C526) * -5. * C658) /
                           (6. * C644 * 2 * C681) +
                       C681 * 7. * std::pow(C523, C527) / 3.)) /
                     std::pow(C682, 2)) +
            ((C685 * C671 / C665 - C555) + C698 / C682);

        const double C703 = 2. * sigmaAb;
        const double C704 = rhoA + rhoB;
        const double C705 = rhoA - rhoB;
        const double C706 = 1. / 3.;
        const double C707 = 2. / 3.;
        const double C708 = 5. / 3.;
        const double C709 = 7. / 6.;
        const double C710 = 0.9999999999999999 * C705;
        const double C711 = Pi * C704;
        const double C712 = sigmaAa + C703;
        const double C713 = std::pow(C704, C709);
        const double C714 = 4. * C711;
        const double C715 = C712 + sigmaBb;
        const double C716 = C710 / C704;
        const double C717 = C715 + SigmaGuard;
        const double C718 = C716 + 1.;
        const double C719 = 1. - C716;
        const double C720 = 0.22167 / C714;
        const double C721 = 3. / C714;
        const double C722 = std::sqrt(C717);
        const double C723 = std::pow(C721, C706);
        const double C724 = std::pow(C721, C707);
        const double C725 = 7.389e-6 * C724;
        const double C726 = 0.00081290825 * C722;
        const double C727 = 0.023266 * C723;
        const double C728 = 0.472 * C724;
        const double C729 = 8.723 * C723;
        const double C730 = std::sqrt(C723);
        const double C731 = C727 + C725;
        const double C732 = C729 + C728;
        const double C733 = C731 + 0.002568;
        const double C734 = C732 + C720;
        const double C735 = C734 + 1.;
        const double C736 = C733 / C735;
        const double C737 = C736 + 0.001667;
        const double C738 = C713 * C737;
        const double C739 = C726 / C738;
        const double C740 = -C739;
        const double C741 = std::exp(C740);

        result.vsigmaAa =
            C704 * ((0.1423 / ((1.0529 * C730 + 0.3334 * C723) + 1.) -
                     0.0843 / ((1.3981 * C730 + 0.2611 * C723) + 1.)) *
                        (0. * std::pow(C718, C706) + 0. * std::pow(C719, C706)) /
                        (3. * C704 * 2. * (std::pow(2., C706) - 1.)) +
                    (C741 * C737 +
                     (0. * C741 / C735 + C737 * -0.00081290825 * C741 / (2 * C722 * C738)) * C715) /
                        (std::pow(C704, 7. / 3.) *
                         std::sqrt((std::pow(C718, C708) + std::pow(C719, C708)) / 2.)));

        const double C743 = 2. * SigmaGuard;
        const double C744 = 2. * sigmaAb;
        const double C745 = rhoA + rhoB;
        const double C746 = rhoA - rhoB;
        const double C747 = 1. / 3.;
        const double C748 = 2. / 3.;
        const double C749 = 5. / 3.;
        const double C750 = 7. / 6.;
        const double C751 = 0.9999999999999999 * C746;
        const double C752 = Pi * C745;
        const double C753 = sigmaAa + C744;
        const double C754 = std::pow(C745, C750);
        const double C755 = 4. * C752;
        const double C756 = C753 + sigmaBb;
        const double C757 = C751 / C745;
        const double C758 = C756 + C743;
        const double C759 = C757 + 1.;
        const double C760 = 1. - C757;
        const double C761 = 0.22167 / C755;
        const double C762 = 3. / C755;
        const double C763 = std::sqrt(C758);
        const double C764 = std::pow(C762, C747);
        const double C765 = std::pow(C762, C748);
        const double C766 = 7.389e-6 * C765;
        const double C767 = 0.00081290825 * C763;
        const double C768 = 0.023266 * C764;
        const double C769 = 0.472 * C765;
        const double C770 = 8.723 * C764;
        const double C771 = std::sqrt(C764);
        const double C772 = C768 + C766;
        const double C773 = C770 + C769;
        const double C774 = C772 + 0.002568;
        const double C775 = C773 + C761;
        const double C776 = C775 + 1.;
        const double C777 = C774 / C776;
        const double C778 = C777 + 0.001667;
        const double C779 = C754 * C778;
        const double C780 = C767 / C779;
        const double C781 = -C780;
        const double C782 = std::exp(C781);

        result.vsigmaAb =
            C745 * ((0.1423 / ((1.0529 * C771 + 0.3334 * C764) + 1.) -
                     0.0843 / ((1.3981 * C771 + 0.2611 * C764) + 1.)) *
                        (0. * std::pow(C759, C747) + 0. * std::pow(C760, C747)) /
                        (3. * C745 * 2. * (std::pow(2., C747) - 1.)) +
                    (2. * C782 * C778 +
                     (0. * C782 / C776 + C778 * -0.0016258165 * C782 / (2 * C763 * C779)) * C756) /
                        (std::pow(C745, 7. / 3.) *
                         std::sqrt((std::pow(C759, C749) + std::pow(C760, C749)) / 2.)));

        const double C784 = 2. * sigmaAb;
        const double C785 = rhoA + rhoB;
        const double C786 = rhoA - rhoB;
        const double C787 = 1. / 3.;
        const double C788 = 2. / 3.;
        const double C789 = 5. / 3.;
        const double C790 = 7. / 6.;
        const double C791 = 0.9999999999999999 * C786;
        const double C792 = Pi * C785;
        const double C793 = sigmaAa + C784;
        const double C794 = std::pow(C785, C790);
        const double C795 = 4. * C792;
        const double C796 = C793 + sigmaBb;
        const double C797 = C791 / C785;
        const double C798 = C796 + SigmaGuard;
        const double C799 = C797 + 1.;
        const double C800 = 1. - C797;
        const double C801 = 0.22167 / C795;
        const double C802 = 3. / C795;
        const double C803 = std::sqrt(C798);
        const double C804 = std::pow(C802, C787);
        const double C805 = std::pow(C802, C788);
        const double C806 = 7.389e-6 * C805;
        const double C807 = 0.00081290825 * C803;
        const double C808 = 0.023266 * C804;
        const double C809 = 0.472 * C805;
        const double C810 = 8.723 * C804;
        const double C811 = std::sqrt(C804);
        const double C812 = C808 + C806;
        const double C813 = C810 + C809;
        const double C814 = C812 + 0.002568;
        const double C815 = C813 + C801;
        const double C816 = C815 + 1.;
        const double C817 = C814 / C816;
        const double C818 = C817 + 0.001667;
        const double C819 = C794 * C818;
        const double C820 = C807 / C819;
        const double C821 = -C820;
        const double C822 = std::exp(C821);

        result.vsigmaBb =
            C785 * ((0.1423 / ((1.0529 * C811 + 0.3334 * C804) + 1.) -
                     0.0843 / ((1.3981 * C811 + 0.2611 * C804) + 1.)) *
                        (0. * std::pow(C799, C787) + 0. * std::pow(C800, C787)) /
                        (3. * C785 * 2. * (std::pow(2., C787) - 1.)) +
                    (C822 * C818 +
                     (0. * C822 / C816 + C818 * -0.00081290825 * C822 / (2 * C803 * C819)) * C796) /
                        (std::pow(C785, 7. / 3.) *
                         std::sqrt((std::pow(C799, C789) + std::pow(C800, C789)) / 2.)));
    } else
    {
        const double C824 = 2. * sigmaAb;
        const double C825 = rhoA + rhoB;
        const double C826 = rhoA - rhoB;
        const double C827 = 1. / 3.;
        const double C828 = 2. / 3.;
        const double C829 = 4. / 3.;
        const double C830 = 5. / 3.;
        const double C831 = 0.9999999999999999 * C826;
        const double C832 = Pi * C825;
        const double C833 = sigmaAa + C824;
        const double C834 = 4. * C832;
        const double C835 = C833 + sigmaBb;
        const double C836 = C831 / C825;
        const double C837 = C836 + 1.;
        const double C838 = 1. - C836;
        const double C839 = 0.22167 / C834;
        const double C840 = 3. / C834;
        const double C841 = std::pow(C840, C827);
        const double C842 = std::pow(C840, C828);
        const double C843 = 7.389e-6 * C842;
        const double C844 = 0.0116 * C841;
        const double C845 = 0.023266 * C841;
        const double C846 = 0.472 * C842;
        const double C847 = 8.723 * C841;
        const double C848 = std::log(C841);
        const double C849 = 0.0311 * C848;
        const double C850 = C841 * C848;
        const double C851 = C845 + C843;
        const double C852 = C847 + C846;
        const double C853 = 0.0020 * C850;
        const double C854 = C851 + 0.002568;
        const double C855 = C852 + C839;
        const double C856 = C849 + C853;
        const double C857 = C855 + 1.;
        const double C858 = C856 - 0.048;
        const double C859 = C854 / C857;
        const double C860 = C859 + 0.001667;
        const double C861 = C858 - C844;

        result.exc =
            C825 * ((C861 + ((((0.01555 * C848 + 0.0007 * C850) - 0.0269) - 0.0048 * C841) - C861) *
                                ((std::pow(C837, C829) + std::pow(C838, C829)) - 2.) /
                                (2. * (std::pow(2., C827) - 1.))) +
                    std::exp(-0.00081290825 * std::sqrt(C835) / (std::pow(C825, 7. / 6.) * C860)) *
                        C860 * C835 /
                        (std::pow(C825, 7. / 3.) *
                         std::sqrt((std::pow(C837, C830) + std::pow(C838, C830)) / 2.)));

        const double C863 = -24. * Pi;
        const double C864 = 0.0007 * C850;
        const double C865 = 0.0048 * C841;
        const double C866 = 0.01555 * C848;
        const double C867 = 0.88668 * Pi;
        const double C868 = 0.9999999999999999 * C825;
        const double C869 = 12. * Pi;
        const double C870 = -2. / 3.;
        const double C871 = -1. / 3.;
        const double C872 = 7. / 3.;
        const double C873 = 7. / 6.;
        const double C874 = std::sqrt(C835);
        const double C875 = std::pow(2., C827);
        const double C876 = std::pow(C825, 2);
        const double C877 = std::pow(C834, 2);
        const double C878 = std::pow(C837, C829);
        const double C879 = std::pow(C837, C830);
        const double C880 = std::pow(C838, C829);
        const double C881 = std::pow(C838, C830);
        const double C882 = std::pow(C857, 2);
        const double C883 = 0.00081290825 * C874;
        const double C884 = 3. * C877;
        const double C885 = C877 * C841;
        const double C886 = C866 + C864;
        const double C887 = C878 + C880;
        const double C888 = C879 + C881;
        const double C889 = C868 - C831;
        const double C890 = C875 - 1.;
        const double C891 = C867 / C877;
        const double C892 = std::pow(C825, C872);
        const double C893 = std::pow(C825, C873);
        const double C894 = std::pow(C840, C870);
        const double C895 = std::pow(C840, C871);
        const double C896 = 2. * C890;
        const double C897 = 3. * C885;
        const double C898 = C893 * C860;
        const double C899 = C894 * C869;
        const double C900 = C895 * C863;
        const double C901 = C886 - 0.0269;
        const double C902 = C887 - 2.;
        const double C903 = C888 / 2.;
        const double C904 = -8.723 * C899;
        const double C905 = -0.0311 * C899;
        const double C906 = -0.023266 * C899;
        const double C907 = -0.0116 * C899;
        const double C908 = 7.389e-6 * C900;
        const double C909 = 0.472 * C900;
        const double C910 = C841 * C899;
        const double C911 = C848 * C899;
        const double C912 = C901 - C865;
        const double C913 = C883 / C898;
        const double C914 = std::sqrt(C903);
        const double C915 = C892 * C914;
        const double C916 = C904 + C909;
        const double C917 = C906 + C908;
        const double C918 = -C911;
        const double C919 = C912 - C861;
        const double C920 = -C913;
        const double C921 = C905 / C897;
        const double C922 = C907 / C884;
        const double C923 = C910 / C897;
        const double C924 = C857 * C917;
        const double C925 = C916 / C884;
        const double C926 = C918 / C884;
        const double C927 = std::exp(C920);
        const double C928 = C927 * C860;
        const double C929 = C925 - C891;
        const double C930 = C926 - C923;
        const double C931 = C924 / C884;
        const double C932 = 0.0020 * C930;
        const double C933 = C854 * C929;
        const double C934 = C928 * C835;
        const double C935 = C921 + C932;
        const double C936 = C931 - C933;
        const double C937 = C935 - C922;

        result.vrhoA =
            C825 *
                ((C937 +
                  (C919 * (std::pow(C837, C827) * 4. * C889 + std::pow(C838, C827) * -4. * C889) /
                       (3. * C876) +
                   (((-0.01555 * C899 / C897 + 0.0007 * C930) - -0.0048 * C899 / C884) - C937) *
                       C902) /
                      C896) +
                 (C915 *
                      (C927 * C936 / C882 +
                       C860 * C927 * 0.00081290825 * C874 *
                           (C893 * C936 / C882 + C860 * 7. * std::pow(C825, 1. / 6.) / 6.) /
                           std::pow(C898, 2)) *
                      C835 -
                  C934 *
                      (C892 *
                           (std::pow(C837, C828) * 5. * C889 + std::pow(C838, C828) * -5. * C889) /
                           (6. * C876 * 2 * C914) +
                       C914 * 7. * std::pow(C825, C829) / 3.)) /
                     std::pow(C915, 2)) +
            ((C861 + C919 * C902 / C896) + C934 / C915);

        const double C939 = -24. * Pi;
        const double C940 = -0.9999999999999999 * C825;
        const double C941 = 0.0007 * C850;
        const double C942 = 0.0048 * C841;
        const double C943 = 0.01555 * C848;
        const double C944 = 0.88668 * Pi;
        const double C945 = 12. * Pi;
        const double C946 = -2. / 3.;
        const double C947 = -1. / 3.;
        const double C948 = 7. / 3.;
        const double C949 = 7. / 6.;
        const double C950 = std::sqrt(C835);
        const double C951 = std::pow(2., C827);
        const double C952 = std::pow(C825, 2);
        const double C953 = std::pow(C834, 2);
        const double C954 = std::pow(C837, C829);
        const double C955 = std::pow(C837, C830);
        const double C956 = std::pow(C838, C829);
        const double C957 = std::pow(C838, C830);
        const double C958 = std::pow(C857, 2);
        const double C959 = 0.00081290825 * C950;
        const double C960 = 3. * C953;
        const double C961 = C953 * C841;
        const double C962 = C943 + C941;
        const double C963 = C954 + C956;
        const double C964 = C955 + C957;
        const double C965 = C940 - C831;
        const double C966 = C951 - 1.;
        const double C967 = C944 / C953;
        const double C968 = std::pow(C825, C948);
        const double C969 = std::pow(C825, C949);
        const double C970 = std::pow(C840, C946);
        const double C971 = std::pow(C840, C947);
        const double C972 = 2. * C966;
        const double C973 = 3. * C961;
        const double C974 = C969 * C860;
        const double C975 = C970 * C945;
        const double C976 = C971 * C939;
        const double C977 = C962 - 0.0269;
        const double C978 = C963 - 2.;
        const double C979 = C964 / 2.;
        const double C980 = -8.723 * C975;
        const double C981 = -0.0311 * C975;
        const double C982 = -0.023266 * C975;
        const double C983 = -0.0116 * C975;
        const double C984 = 7.389e-6 * C976;
        const double C985 = 0.472 * C976;
        const double C986 = C841 * C975;
        const double C987 = C848 * C975;
        const double C988 = C977 - C942;
        const double C989 = C959 / C974;
        const double C990 = std::sqrt(C979);
        const double C991 = C968 * C990;
        const double C992 = C980 + C985;
        const double C993 = C982 + C984;
        const double C994 = -C987;
        const double C995 = C988 - C861;
        const double C996 = -C989;
        const double C997 = C981 / C973;
        const double C998 = C983 / C960;
        const double C999 = C986 / C973;
        const double C1000 = C857 * C993;
        const double C1001 = C992 / C960;
        const double C1002 = C994 / C960;
        const double C1003 = std::exp(C996);
        const double C1004 = C1003 * C860;
        const double C1005 = C1001 - C967;
        const double C1006 = C1002 - C999;
        const double C1007 = C1000 / C960;
        const double C1008 = 0.0020 * C1006;
        const double C1009 = C1004 * C835;
        const double C1010 = C854 * C1005;
        const double C1011 = C997 + C1008;
        const double C1012 = C1007 - C1010;
        const double C1013 = C1011 - C998;

        result.vrhoB =
            C825 *
                ((C1013 +
                  (C995 * (std::pow(C837, C827) * 4. * C965 + std::pow(C838, C827) * -4. * C965) /
                       (3. * C952) +
                   (((-0.01555 * C975 / C973 + 0.0007 * C1006) - -0.0048 * C975 / C960) - C1013) *
                       C978) /
                      C972) +
                 (C991 *
                      (C1003 * C1012 / C958 +
                       C860 * C1003 * 0.00081290825 * C950 *
                           (C969 * C1012 / C958 + C860 * 7. * std::pow(C825, 1. / 6.) / 6.) /
                           std::pow(C974, 2)) *
                      C835 -
                  C1009 *
                      (C968 *
                           (std::pow(C837, C828) * 5. * C965 + std::pow(C838, C828) * -5. * C965) /
                           (6. * C952 * 2 * C990) +
                       C990 * 7. * std::pow(C825, C829) / 3.)) /
                     std::pow(C991, 2)) +
            ((C861 + C995 * C978 / C972) + C1009 / C991);

        const double C1015 = 2. * sigmaAb;
        const double C1016 = rhoA + rhoB;
        const double C1017 = rhoA - rhoB;
        const double C1018 = 1. / 3.;
        const double C1019 = 2. / 3.;
        const double C1020 = 5. / 3.;
        const double C1021 = 7. / 6.;
        const double C1022 = 0.9999999999999999 * C1017;
        const double C1023 = Pi * C1016;
        const double C1024 = sigmaAa + C1015;
        const double C1025 = std::pow(C1016, C1021);
        const double C1026 = 4. * C1023;
        const double C1027 = C1024 + sigmaBb;
        const double C1028 = C1022 / C1016;
        const double C1029 = C1027 + SigmaGuard;
        const double C1030 = C1028 + 1.;
        const double C1031 = 1. - C1028;
        const double C1032 = 0.22167 / C1026;
        const double C1033 = 3. / C1026;
        const double C1034 = std::sqrt(C1029);
        const double C1035 = std::pow(C1033, C1018);
        const double C1036 = std::pow(C1033, C1019);
        const double C1037 = 7.389e-6 * C1036;
        const double C1038 = 0.00081290825 * C1034;
        const double C1039 = 0.023266 * C1035;
        const double C1040 = 0.472 * C1036;
        const double C1041 = 8.723 * C1035;
        const double C1042 = std::log(C1035);
        const double C1043 = C1035 * C1042;
        const double C1044 = C1039 + C1037;
        const double C1045 = C1041 + C1040;
        const double C1046 = C1044 + 0.002568;
        const double C1047 = C1045 + C1032;
        const double C1048 = C1047 + 1.;
        const double C1049 = C1046 / C1048;
        const double C1050 = C1049 + 0.001667;
        const double C1051 = C1025 * C1050;
        const double C1052 = C1038 / C1051;
        const double C1053 = -C1052;
        const double C1054 = std::exp(C1053);

        result.vsigmaAa =
            C1016 *
            (((((0.01555 * C1042 + 0.0007 * C1043) - 0.0269) - 0.0048 * C1035) -
              (((0.0311 * C1042 + 0.0020 * C1043) - 0.048) - 0.0116 * C1035)) *
                 (0. * std::pow(C1030, C1018) + 0. * std::pow(C1031, C1018)) /
                 (3. * C1016 * 2. * (std::pow(2., C1018) - 1.)) +
             (C1054 * C1050 +
              (0. * C1054 / C1048 + C1050 * -0.00081290825 * C1054 / (2 * C1034 * C1051)) * C1027) /
                 (std::pow(C1016, 7. / 3.) *
                  std::sqrt((std::pow(C1030, C1020) + std::pow(C1031, C1020)) / 2.)));

        const double C1056 = 2. * SigmaGuard;
        const double C1057 = 2. * sigmaAb;
        const double C1058 = rhoA + rhoB;
        const double C1059 = rhoA - rhoB;
        const double C1060 = 1. / 3.;
        const double C1061 = 2. / 3.;
        const double C1062 = 5. / 3.;
        const double C1063 = 7. / 6.;
        const double C1064 = 0.9999999999999999 * C1059;
        const double C1065 = Pi * C1058;
        const double C1066 = sigmaAa + C1057;
        const double C1067 = std::pow(C1058, C1063);
        const double C1068 = 4. * C1065;
        const double C1069 = C1066 + sigmaBb;
        const double C1070 = C1064 / C1058;
        const double C1071 = C1069 + C1056;
        const double C1072 = C1070 + 1.;
        const double C1073 = 1. - C1070;
        const double C1074 = 0.22167 / C1068;
        const double C1075 = 3. / C1068;
        const double C1076 = std::sqrt(C1071);
        const double C1077 = std::pow(C1075, C1060);
        const double C1078 = std::pow(C1075, C1061);
        const double C1079 = 7.389e-6 * C1078;
        const double C1080 = 0.00081290825 * C1076;
        const double C1081 = 0.023266 * C1077;
        const double C1082 = 0.472 * C1078;
        const double C1083 = 8.723 * C1077;
        const double C1084 = std::log(C1077);
        const double C1085 = C1077 * C1084;
        const double C1086 = C1081 + C1079;
        const double C1087 = C1083 + C1082;
        const double C1088 = C1086 + 0.002568;
        const double C1089 = C1087 + C1074;
        const double C1090 = C1089 + 1.;
        const double C1091 = C1088 / C1090;
        const double C1092 = C1091 + 0.001667;
        const double C1093 = C1067 * C1092;
        const double C1094 = C1080 / C1093;
        const double C1095 = -C1094;
        const double C1096 = std::exp(C1095);

        result.vsigmaAb =
            C1058 *
            (((((0.01555 * C1084 + 0.0007 * C1085) - 0.0269) - 0.0048 * C1077) -
              (((0.0311 * C1084 + 0.0020 * C1085) - 0.048) - 0.0116 * C1077)) *
                 (0. * std::pow(C1072, C1060) + 0. * std::pow(C1073, C1060)) /
                 (3. * C1058 * 2. * (std::pow(2., C1060) - 1.)) +
             (2. * C1096 * C1092 +
              (0. * C1096 / C1090 + C1092 * -0.0016258165 * C1096 / (2 * C1076 * C1093)) * C1069) /
                 (std::pow(C1058, 7. / 3.) *
                  std::sqrt((std::pow(C1072, C1062) + std::pow(C1073, C1062)) / 2.)));

        const double C1098 = 2. * sigmaAb;
        const double C1099 = rhoA + rhoB;
        const double C1100 = rhoA - rhoB;
        const double C1101 = 1. / 3.;
        const double C1102 = 2. / 3.;
        const double C1103 = 5. / 3.;
        const double C1104 = 7. / 6.;
        const double C1105 = 0.9999999999999999 * C1100;
        const double C1106 = Pi * C1099;
        const double C1107 = sigmaAa + C1098;
        const double C1108 = std::pow(C1099, C1104);
        const double C1109 = 4. * C1106;
        const double C1110 = C1107 + sigmaBb;
        const double C1111 = C1105 / C1099;
        const double C1112 = C1110 + SigmaGuard;
        const double C1113 = C1111 + 1.;
        const double C1114 = 1. - C1111;
        const double C1115 = 0.22167 / C1109;
        const double C1116 = 3. / C1109;
        const double C1117 = std::sqrt(C1112);
        const double C1118 = std::pow(C1116, C1101);
        const double C1119 = std::pow(C1116, C1102);
        const double C1120 = 7.389e-6 * C1119;
        const double C1121 = 0.00081290825 * C1117;
        const double C1122 = 0.023266 * C1118;
        const double C1123 = 0.472 * C1119;
        const double C1124 = 8.723 * C1118;
        const double C1125 = std::log(C1118);
        const double C1126 = C1118 * C1125;
        const double C1127 = C1122 + C1120;
        const double C1128 = C1124 + C1123;
        const double C1129 = C1127 + 0.002568;
        const double C1130 = C1128 + C1115;
        const double C1131 = C1130 + 1.;
        const double C1132 = C1129 / C1131;
        const double C1133 = C1132 + 0.001667;
        const double C1134 = C1108 * C1133;
        const double C1135 = C1121 / C1134;
        const double C1136 = -C1135;
        const double C1137 = std::exp(C1136);

        result.vsigmaBb =
            C1099 *
            (((((0.01555 * C1125 + 0.0007 * C1126) - 0.0269) - 0.0048 * C1118) -
              (((0.0311 * C1125 + 0.0020 * C1126) - 0.048) - 0.0116 * C1118)) *
                 (0. * std::pow(C1113, C1101) + 0. * std::pow(C1114, C1101)) /
                 (3. * C1099 * 2. * (std::pow(2., C1101) - 1.)) +
             (C1137 * C1133 +
              (0. * C1137 / C1131 + C1133 * -0.00081290825 * C1137 / (2 * C1117 * C1134)) * C1110) /
                 (std::pow(C1099, 7. / 3.) *
                  std::sqrt((std::pow(C1113, C1103) + std::pow(C1114, C1103)) / 2.)));
    }

    return result;
}

void P86CorrelationSecondDerivatives(double rhoA,
                                     double rhoB,
                                     double sigmaAa,
                                     double sigmaAb,
                                     double sigmaBb,
                                     PointSecondDerivativeMatrix& matrix) {
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;
    [[maybe_unused]] constexpr double SigmaGuard = 1e-300;

    if (rhoA + rhoB < 2. * std::numeric_limits<double>::epsilon())
    {
        return;
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
        const double C86 = 1;
        const double C87 = 1;
        const double C88 = 0.9999999999999999 * C87 + 0.;
        const double C89 = Pi * C86;
        const double C90 = 4. * C89;
        const double C91 = C88 / C47 - C86 * C53 / std::pow(C47, 2);
        const double C92 = C91;
        const double C93 = -C91;
        const double C94 = -0.22167 * C90 / std::pow(C56, 2);
        const double C95 = -3. * C90 / std::pow(C56, 2);
        const double C96 = C49 * std::pow(C62, C49 - 1) * C95;
        const double C97 = C50 * std::pow(C62, C50 - 1) * C95;
        const double C98 = 0.7389e-5 * C97 + 0.;
        const double C99 = 0.023266 * C96;
        const double C100 = 0.3334 * C96;
        const double C101 = 0.472 * C97;
        const double C102 = 8.723 * C96;
        const double C103 = 1.0529 * C96 / (2 * std::sqrt(C63));
        const double C104 = C99 + C98;
        const double C105 = C102 + C101;
        const double C106 = C103 + C100;
        const double C107 = C104;
        const double C108 = C105 + C94;
        const double C109 = C106;
        const double C110 = C108;
        const double C111 = -0.1423 * C109 / std::pow(C77, 2);
        const double C112 = C107 / C78 - C110 * C75 / std::pow(C78, 2);
        const double C113 = C112;
        const double C175 = 0.;
        const double C176 =
            (C175 / C47 +
             ((-C88) / std::pow(C47, 2) - (-C86 * C53 * 2 * C47) / std::pow(C47, 4)) * C86) -
            C88 * C86 / std::pow(C47, 2);
        const double C177 = C176;
        const double C178 = -C176;
        const double C179 = (-C90 * -0.22167 * C90 * 2 * C56) / std::pow(C56, 4);
        const double C180 = (-C90 * -3. * C90 * 2 * C56) / std::pow(C56, 4);
        const double C181 = C49 * (C49 - 1) * std::pow(C62, C49 - 2) * std::pow(C95, 2) +
                            C49 * std::pow(C62, C49 - 1) * C180;
        const double C182 = C50 * (C50 - 1) * std::pow(C62, C50 - 2) * std::pow(C95, 2) +
                            C50 * std::pow(C62, C50 - 1) * C180;
        const double C183 = 0.7389e-5 * C182 + 0.;
        const double C184 = 0.023266 * C181;
        const double C185 = 0.3334 * C181;
        const double C186 = 0.472 * C182;
        const double C187 = 8.723 * C181;
        const double C188 =
            1.0529 * C181 / (2 * std::sqrt(C63)) -
            1.0529 * std::pow(C96, 2) / (std::sqrt(C63) * std::pow(2 * std::sqrt(C63), 2));
        const double C189 = C184 + C183;
        const double C190 = C187 + C186;
        const double C191 = C188 + C185;
        const double C192 = C189;
        const double C193 = C190 + C179;
        const double C194 = C191;
        const double C195 = C193;
        const double C196 =
            -0.1423 * C194 / std::pow(C77, 2) - C109 * -0.1423 * C109 * 2 * C77 / std::pow(C77, 4);
        const double C197 =
            ((C192 / C78 +
              ((-C107) / std::pow(C78, 2) - (-C110 * C75 * 2 * C78) / std::pow(C78, 4)) * C110) -
             C195 * C75 / std::pow(C78, 2)) -
            C107 * C110 / std::pow(C78, 2);
        const double C198 = C197;
        matrix.upper[0] =
            ((((((((((((((((std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                (C47 * ((6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                                             (std::pow(C47, 7. / 3.) *
                                                  std::sqrt(
                                                      (std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.) *
                                                  C57 * C81 *
                                                  (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                            (std::pow(C47, 7. / 6.) * C81)) *
                                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, -5. / 6.) / 6. +
                                                   0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, 1. / 6.) *
                                                       std::exp(-0.00081290825 * std::sqrt(C57) /
                                                                (std::pow(C47, 7. / 6.) * C81)) *
                                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, 1. / 6.) /
                                                       (6. * std::pow(std::pow(C47, 7. / 6.) * C81,
                                                                      2))) +
                                              C57 * C81 *
                                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                                           (std::pow(C47, 7. / 6.) * C81)) *
                                                  0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                  std::pow(C47, 1. / 6.) *
                                                  std::sqrt(
                                                      (std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.) *
                                                  7. * std::pow(C47, 4. / 3.) / 3.) -
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.) *
                                             C57 * C81 *
                                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81)) *
                                             0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                             std::pow(C47, 1. / 6.) * 6. * std::pow(C47, 7. / 6.) *
                                             C81 * 2 * C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                                            std::pow(6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2),
                                                     2) -
                                        (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                  (std::pow(C47, 7. / 6.) * C81)) *
                                             C81 * C57 *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.) *
                                             28. * std::pow(C47, 1. / 3.) / 3. +
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                             7. * std::pow(C47, 4. / 3.) * C57 * C81 *
                                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81)) *
                                             0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                             std::pow(C47, 1. / 6.) /
                                             (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) /
                                            3.) +
                                 (std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      C57 * C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.) /
                                      (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      C81 * C57 *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      7. * std::pow(C47, 4. / 3.) / 3.)) -
                            C47 *
                                (std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     7. * std::pow(C47, 4. / 3.) / 3.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   4) +
                           (std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                C81 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * C81 * 7. * std::pow(C47, 1. / 6.) /
                                (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2)) *
                              C86 +
                          (((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                               (2. * (std::pow(2., C49) - 1.)) -
                           1) *
                              C111) +
                         C96 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                             (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                             (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                              (std::pow(2., C49) - 1.))) +
                        (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                              std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                              (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) -
                         C47 *
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C57) /
                                            (std::pow(C47, 7. / 6.) * C81)) *
                                       C81 * C57 * C52 * std::pow(C59, C52 - 1) * 7. *
                                       std::pow(C47, 4. / 3.) / 3. +
                                   std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * C57 *
                                       C81 *
                                       std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                       std::pow(C47, 1. / 6.) /
                                       (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 *
                                  std::pow(C59, C52 - 1) * 4. *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                  std::pow(C47, 4. / 3.) / 3.) /
                             std::pow(
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.),
                                         2),
                                 2)) *
                            C92) +
                       (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                             std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2))) -
                        C47 *
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                 (std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      C81 * C57 * C52 * std::pow(C60, C52 - 1) * 7. *
                                      std::pow(C47, 4. / 3.) / 3. +
                                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * C57 *
                                      C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.) /
                                      (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                                 C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                 std::pow(C47, 4. / 3.) / 3.) /
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2)) *
                           C93) +
                      (std::pow(C47, 7. / 3.) *
                           std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C113 *
                           (C47 *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                 (std::pow(std::pow(C47, 7. / 6.) * C81, 2) * C81 *
                                      (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * 7. *
                                           std::pow(C47, 1. / 6.) / 6. +
                                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                                    (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                           std::pow(C47, 1. / 6.) /
                                           (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                                  C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                      std::pow(C47, 7. / 6.) * C81 * 2 * C81 * 7. *
                                      std::pow(C47, 1. / 6.) / 6.) /
                                     std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                                C57 +
                            (std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) +
                             C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                                C57) -
                       C113 * C47 *
                           (std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) +
                            C81 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                           C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                           std::pow(C47, 4. / 3.) / 3.) /
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2)) *
                         C86 +
                     (((((((std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C47 *
                                (C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) * std::pow(C47, 7. / 3.) * C52 *
                                     std::pow(C59, C52 - 1) /
                                     (4. *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * 7. * std::pow(C47, 4. / 3.) * C52 *
                                     std::pow(C59, C52 - 1) /
                                     (12. *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) -
                            C47 *
                                (std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     7. * std::pow(C47, 4. / 3.) / 3.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   4) +
                           ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                                std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2)))) *
                              C86 +
                          C111 * C47 * C51 * std::pow(C59, C51 - 1) /
                              (2. * (std::pow(2., C49) - 1.))) +
                         C96 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                             (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                             (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                              (std::pow(2., C49) - 1.))) +
                        C47 *
                            ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) *
                                 C51 * (C51 - 1) * std::pow(C59, C51 - 2) /
                                 (2. * (std::pow(2., C49) - 1.)) -
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                                  std::pow(C59, C52 - 2) -
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 *
                                  std::pow(C59, C52 - 1) * 4. *
                                  (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       2 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2) *
                                       C52 * std::pow(C59, C52 - 1)) /
                                  (4. *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                                 std::pow(
                                     4. *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                         std::pow(std::pow(C47, 7. / 3.) *
                                                      std::sqrt((std::pow(C59, C52) +
                                                                 std::pow(C60, C52)) /
                                                                2.),
                                                  2),
                                     2)) *
                            C92) +
                       C93 * C47 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2))) -
                      C113 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                          C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                          (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2))) *
                         C92) +
                    (((((((std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                               C47 *
                               (C57 * C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) * std::pow(C47, 7. / 3.) * C52 *
                                    std::pow(C60, C52 - 1) /
                                    (4. *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                    C81 * C57 * 7. * std::pow(C47, 4. / 3.) * C52 *
                                    std::pow(C60, C52 - 1) /
                                    (12. *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) -
                           C47 *
                               (std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    C57 * C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) /
                                    (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                    C81 * C57 *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                    std::pow(C47, 4. / 3.) / 3.) *
                               std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                               std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                               (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  4) +
                          ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                               std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                               C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                               (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2)))) *
                             C86 +
                         C111 * C47 * C51 * std::pow(C60, C51 - 1) /
                             (2. * (std::pow(2., C49) - 1.))) +
                        C96 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                            (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                            (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                             (std::pow(2., C49) - 1.))) +
                       C92 * C47 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C60, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2))) +
                      C47 *
                          ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                               (C51 - 1) * std::pow(C60, C51 - 2) /
                               (2. * (std::pow(2., C49) - 1.)) -
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                                std::pow(C60, C52 - 2) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                4. *
                                (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                     C52 * std::pow(C60, C52 - 1)) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                               std::pow(
                                   4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       std::pow(
                                           std::pow(C47, 7. / 3.) *
                                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                         2.),
                                           2),
                                   2)) *
                          C93) -
                     C113 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) *
                        C93) +
                   ((((C47 *
                           ((6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                 (C81 *
                                      (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * 7. *
                                           std::pow(C47, 1. / 6.) +
                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                           std::pow(C47, 1. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                                    (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                           std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.)) -
                             std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                 C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                 std::pow(C47, 1. / 6.) * 12. * C81 * std::pow(C47, 14. / 6.)) /
                                std::pow(6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2), 2) -
                            (std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) +
                             C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                                C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                7. * std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) +
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                           C57 /
                           (std::pow(C47, 7. / 3.) *
                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                          C86 -
                      C92 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                          C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                          (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2))) -
                     C93 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) +
                    C113 * C47 *
                        (std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                              (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                   std::exp(-0.00081290825 * std::sqrt(C57) /
                                            (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                   std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                              std::pow(C47, 14. / 6.)) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                        C57 /
                        (std::pow(C47, 7. / 3.) *
                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                       C113) +
                  (((((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (2. * (std::pow(2., C49) - 1.)) -
                     1) *
                        C86 +
                    C92 * C47 * C51 * std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.))) +
                   C93 * C47 * C51 * std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.))) *
                      C111) +
                 (((C86 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                        (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                        (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                         (std::pow(2., C49) - 1.)) +
                    (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                         (std::pow(2., C49) - 1.) * C96 * C47 * -0.11785983 *
                         ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (std::sqrt(C63) * std::pow(2 * std::sqrt(C63), 2)) -
                     C96 * C47 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                         (1.3981 / (2 * std::sqrt(C63)) + 0.2611) * 2 *
                         (1.3981 / (2 * std::sqrt(C63)) + 0.2611) *
                         ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.) * 2. *
                         (std::pow(2., C49) - 1.)) /
                        std::pow(std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                                     (std::pow(2., C49) - 1.),
                                 2)) +
                   C92 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                       (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                       (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                        (std::pow(2., C49) - 1.))) +
                  C93 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                      (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                      (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                       (std::pow(2., C49) - 1.))) *
                     C96) +
                C47 *
                    (((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (2. * (std::pow(2., C49) - 1.)) -
                     1) *
                    C196) +
               C181 * C47 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                   (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                   (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                    (std::pow(2., C49) - 1.))) +
              C47 *
                  ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                       std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) *
                  C177) +
             C47 *
                 ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                      std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) * C81 *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                 C178) +
            C198 * C47 *
                (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                 C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                     std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                C57 /
                (std::pow(C47, 7. / 3.) *
                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.));

        const double C114 = 1;
        const double C115 = -1;
        const double C116 = 0.9999999999999999 * C115 + 0.;
        const double C117 = Pi * C114;
        const double C118 = 4. * C117;
        const double C119 = C116 / C47 - C114 * C53 / std::pow(C47, 2);
        const double C120 = C119;
        const double C121 = -C119;
        const double C122 = -0.22167 * C118 / std::pow(C56, 2);
        const double C123 = -3. * C118 / std::pow(C56, 2);
        const double C124 = C49 * std::pow(C62, C49 - 1) * C123;
        const double C125 = C50 * std::pow(C62, C50 - 1) * C123;
        const double C126 = 0.7389e-5 * C125 + 0.;
        const double C127 = 0.023266 * C124;
        const double C128 = 0.3334 * C124;
        const double C129 = 0.472 * C125;
        const double C130 = 8.723 * C124;
        const double C131 = 1.0529 * C124 / (2 * std::sqrt(C63));
        const double C132 = C127 + C126;
        const double C133 = C130 + C129;
        const double C134 = C131 + C128;
        const double C135 = C132;
        const double C136 = C133 + C122;
        const double C137 = C134;
        const double C138 = C136;
        const double C139 = -0.1423 * C137 / std::pow(C77, 2);
        const double C140 = C135 / C78 - C138 * C75 / std::pow(C78, 2);
        const double C141 = C140;
        const double C199 = 0.;
        const double C200 =
            (C199 / C47 +
             ((-C88) / std::pow(C47, 2) - (-C86 * C53 * 2 * C47) / std::pow(C47, 4)) * C114) -
            C116 * C86 / std::pow(C47, 2);
        const double C201 = C200;
        const double C202 = -C200;
        const double C203 = (-C118 * -0.22167 * C90 * 2 * C56) / std::pow(C56, 4);
        const double C204 = (-C118 * -3. * C90 * 2 * C56) / std::pow(C56, 4);
        const double C205 = C49 * (C49 - 1) * std::pow(C62, C49 - 2) * C95 * C123 +
                            C49 * std::pow(C62, C49 - 1) * C204;
        const double C206 = C50 * (C50 - 1) * std::pow(C62, C50 - 2) * C95 * C123 +
                            C50 * std::pow(C62, C50 - 1) * C204;
        const double C207 = 0.7389e-5 * C206 + 0.;
        const double C208 = 0.023266 * C205;
        const double C209 = 0.3334 * C205;
        const double C210 = 0.472 * C206;
        const double C211 = 8.723 * C205;
        const double C212 =
            1.0529 * C205 / (2 * std::sqrt(C63)) -
            C124 * 1.0529 * C96 / (std::sqrt(C63) * std::pow(2 * std::sqrt(C63), 2));
        const double C213 = C208 + C207;
        const double C214 = C211 + C210;
        const double C215 = C212 + C209;
        const double C216 = C213;
        const double C217 = C214 + C203;
        const double C218 = C215;
        const double C219 = C217;
        const double C220 =
            -0.1423 * C218 / std::pow(C77, 2) - C137 * -0.1423 * C109 * 2 * C77 / std::pow(C77, 4);
        const double C221 =
            ((C216 / C78 +
              ((-C107) / std::pow(C78, 2) - (-C110 * C75 * 2 * C78) / std::pow(C78, 4)) * C138) -
             C219 * C75 / std::pow(C78, 2)) -
            C135 * C110 / std::pow(C78, 2);
        const double C222 = C221;
        matrix.upper[1] =
            ((((((((((((((((std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                (C47 * ((6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                                             (std::pow(C47, 7. / 3.) *
                                                  std::sqrt(
                                                      (std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.) *
                                                  C57 * C81 *
                                                  (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                            (std::pow(C47, 7. / 6.) * C81)) *
                                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, -5. / 6.) / 6. +
                                                   0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, 1. / 6.) *
                                                       std::exp(-0.00081290825 * std::sqrt(C57) /
                                                                (std::pow(C47, 7. / 6.) * C81)) *
                                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, 1. / 6.) /
                                                       (6. * std::pow(std::pow(C47, 7. / 6.) * C81,
                                                                      2))) +
                                              C57 * C81 *
                                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                                           (std::pow(C47, 7. / 6.) * C81)) *
                                                  0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                  std::pow(C47, 1. / 6.) *
                                                  std::sqrt(
                                                      (std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.) *
                                                  7. * std::pow(C47, 4. / 3.) / 3.) -
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.) *
                                             C57 * C81 *
                                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81)) *
                                             0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                             std::pow(C47, 1. / 6.) * 6. * std::pow(C47, 7. / 6.) *
                                             C81 * 2 * C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                                            std::pow(6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2),
                                                     2) -
                                        (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                  (std::pow(C47, 7. / 6.) * C81)) *
                                             C81 * C57 *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.) *
                                             28. * std::pow(C47, 1. / 3.) / 3. +
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                             7. * std::pow(C47, 4. / 3.) * C57 * C81 *
                                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81)) *
                                             0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                             std::pow(C47, 1. / 6.) /
                                             (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) /
                                            3.) +
                                 (std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      C57 * C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.) /
                                      (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      C81 * C57 *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      7. * std::pow(C47, 4. / 3.) / 3.)) -
                            C47 *
                                (std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     7. * std::pow(C47, 4. / 3.) / 3.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   4) +
                           (std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                C81 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * C81 * 7. * std::pow(C47, 1. / 6.) /
                                (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2)) *
                              C86 +
                          (((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                               (2. * (std::pow(2., C49) - 1.)) -
                           1) *
                              C111) +
                         C96 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                             (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                             (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                              (std::pow(2., C49) - 1.))) +
                        (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                              std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                              (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) -
                         C47 *
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C57) /
                                            (std::pow(C47, 7. / 6.) * C81)) *
                                       C81 * C57 * C52 * std::pow(C59, C52 - 1) * 7. *
                                       std::pow(C47, 4. / 3.) / 3. +
                                   std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * C57 *
                                       C81 *
                                       std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                       std::pow(C47, 1. / 6.) /
                                       (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 *
                                  std::pow(C59, C52 - 1) * 4. *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                  std::pow(C47, 4. / 3.) / 3.) /
                             std::pow(
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.),
                                         2),
                                 2)) *
                            C92) +
                       (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                             std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2))) -
                        C47 *
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                 (std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      C81 * C57 * C52 * std::pow(C60, C52 - 1) * 7. *
                                      std::pow(C47, 4. / 3.) / 3. +
                                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * C57 *
                                      C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.) /
                                      (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                                 C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                 std::pow(C47, 4. / 3.) / 3.) /
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2)) *
                           C93) +
                      (std::pow(C47, 7. / 3.) *
                           std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C113 *
                           (C47 *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                 (std::pow(std::pow(C47, 7. / 6.) * C81, 2) * C81 *
                                      (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * 7. *
                                           std::pow(C47, 1. / 6.) / 6. +
                                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                                    (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                           std::pow(C47, 1. / 6.) /
                                           (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                                  C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                      std::pow(C47, 7. / 6.) * C81 * 2 * C81 * 7. *
                                      std::pow(C47, 1. / 6.) / 6.) /
                                     std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                                C57 +
                            (std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) +
                             C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                                C57) -
                       C113 * C47 *
                           (std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) +
                            C81 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                           C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                           std::pow(C47, 4. / 3.) / 3.) /
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2)) *
                         C114 +
                     (((((((std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C47 *
                                (C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) * std::pow(C47, 7. / 3.) * C52 *
                                     std::pow(C59, C52 - 1) /
                                     (4. *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * 7. * std::pow(C47, 4. / 3.) * C52 *
                                     std::pow(C59, C52 - 1) /
                                     (12. *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) -
                            C47 *
                                (std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     7. * std::pow(C47, 4. / 3.) / 3.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   4) +
                           ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                                std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2)))) *
                              C86 +
                          C111 * C47 * C51 * std::pow(C59, C51 - 1) /
                              (2. * (std::pow(2., C49) - 1.))) +
                         C96 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                             (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                             (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                              (std::pow(2., C49) - 1.))) +
                        C47 *
                            ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) *
                                 C51 * (C51 - 1) * std::pow(C59, C51 - 2) /
                                 (2. * (std::pow(2., C49) - 1.)) -
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                                  std::pow(C59, C52 - 2) -
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 *
                                  std::pow(C59, C52 - 1) * 4. *
                                  (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       2 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2) *
                                       C52 * std::pow(C59, C52 - 1)) /
                                  (4. *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                                 std::pow(
                                     4. *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                         std::pow(std::pow(C47, 7. / 3.) *
                                                      std::sqrt((std::pow(C59, C52) +
                                                                 std::pow(C60, C52)) /
                                                                2.),
                                                  2),
                                     2)) *
                            C92) +
                       C93 * C47 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2))) -
                      C113 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                          C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                          (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2))) *
                         C120) +
                    (((((((std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                               C47 *
                               (C57 * C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) * std::pow(C47, 7. / 3.) * C52 *
                                    std::pow(C60, C52 - 1) /
                                    (4. *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                    C81 * C57 * 7. * std::pow(C47, 4. / 3.) * C52 *
                                    std::pow(C60, C52 - 1) /
                                    (12. *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) -
                           C47 *
                               (std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    C57 * C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) /
                                    (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                    C81 * C57 *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                    std::pow(C47, 4. / 3.) / 3.) *
                               std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                               std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                               (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  4) +
                          ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                               std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                               C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                               (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2)))) *
                             C86 +
                         C111 * C47 * C51 * std::pow(C60, C51 - 1) /
                             (2. * (std::pow(2., C49) - 1.))) +
                        C96 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                            (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                            (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                             (std::pow(2., C49) - 1.))) +
                       C92 * C47 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C60, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2))) +
                      C47 *
                          ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                               (C51 - 1) * std::pow(C60, C51 - 2) /
                               (2. * (std::pow(2., C49) - 1.)) -
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                                std::pow(C60, C52 - 2) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                4. *
                                (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                     C52 * std::pow(C60, C52 - 1)) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                               std::pow(
                                   4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       std::pow(
                                           std::pow(C47, 7. / 3.) *
                                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                         2.),
                                           2),
                                   2)) *
                          C93) -
                     C113 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) *
                        C121) +
                   ((((C47 *
                           ((6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                 (C81 *
                                      (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * 7. *
                                           std::pow(C47, 1. / 6.) +
                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                           std::pow(C47, 1. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                                    (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                           std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.)) -
                             std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                 C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                 std::pow(C47, 1. / 6.) * 12. * C81 * std::pow(C47, 14. / 6.)) /
                                std::pow(6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2), 2) -
                            (std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) +
                             C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                                C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                7. * std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) +
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                           C57 /
                           (std::pow(C47, 7. / 3.) *
                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                          C86 -
                      C92 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                          C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                          (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2))) -
                     C93 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) +
                    C113 * C47 *
                        (std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                              (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                   std::exp(-0.00081290825 * std::sqrt(C57) /
                                            (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                   std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                              std::pow(C47, 14. / 6.)) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                        C57 /
                        (std::pow(C47, 7. / 3.) *
                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                       C141) +
                  (((((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (2. * (std::pow(2., C49) - 1.)) -
                     1) *
                        C86 +
                    C92 * C47 * C51 * std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.))) +
                   C93 * C47 * C51 * std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.))) *
                      C139) +
                 (((C86 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                        (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                        (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                         (std::pow(2., C49) - 1.)) +
                    (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                         (std::pow(2., C49) - 1.) * C96 * C47 * -0.11785983 *
                         ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (std::sqrt(C63) * std::pow(2 * std::sqrt(C63), 2)) -
                     C96 * C47 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                         (1.3981 / (2 * std::sqrt(C63)) + 0.2611) * 2 *
                         (1.3981 / (2 * std::sqrt(C63)) + 0.2611) *
                         ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.) * 2. *
                         (std::pow(2., C49) - 1.)) /
                        std::pow(std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                                     (std::pow(2., C49) - 1.),
                                 2)) +
                   C92 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                       (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                       (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                        (std::pow(2., C49) - 1.))) +
                  C93 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                      (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                      (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                       (std::pow(2., C49) - 1.))) *
                     C124) +
                C47 *
                    (((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (2. * (std::pow(2., C49) - 1.)) -
                     1) *
                    C220) +
               C205 * C47 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                   (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                   (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                    (std::pow(2., C49) - 1.))) +
              C47 *
                  ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                       std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) *
                  C201) +
             C47 *
                 ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                      std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) * C81 *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                 C202) +
            C222 * C47 *
                (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                 C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                     std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                C57 /
                (std::pow(C47, 7. / 3.) *
                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.));

        const double C142 = 0.;
        const double C143 = 1;
        const double C144 = C143;
        const double C145 = C142 / C47;
        const double C146 = C145;
        const double C147 = -C145;
        const double C148 = 0.;
        const double C149 = C148;
        const double C150 = C149;
        const double C151 = C150 / C78;
        const double C152 = C151;
        const double C223 = (-C86 * C142) / std::pow(C47, 2);
        const double C224 = C223;
        const double C225 = -C223;
        const double C226 = (-C110 * C150) / std::pow(C78, 2);
        const double C227 = C226;
        matrix.upper[2] =
            (((((((((((((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                            std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2))) -
                       C47 *
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * C52 * std::pow(C59, C52 - 1) * 7. *
                                     std::pow(C47, 4. / 3.) / 3. +
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) *
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                          C146 +
                      (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                            std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2))) -
                       C47 *
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * C52 * std::pow(C60, C52 - 1) * 7. *
                                     std::pow(C47, 4. / 3.) / 3. +
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                          C147) +
                     (std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C144 *
                          (C47 *
                               (C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) /
                                    (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 * C81 *
                                     -0.00081290825 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 C57 * C81 * -0.00081290825 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     2 * std::sqrt(C57) * C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                                    std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81,
                                             2)) +
                           (std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 +
                            C57 * C81 * -0.00081290825 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) /
                                (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81))) -
                      C144 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                               C81 +
                           C57 * C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                          std::pow(C47, 4. / 3.) / 3.) /
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2)) +
                    (std::pow(C47, 7. / 3.) *
                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C152 *
                         (C47 *
                              (std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                   std::pow(C47, 1. / 6.) /
                                   (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                               (std::pow(std::pow(C47, 7. / 6.) * C81, 2) * C81 *
                                    (std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                         0.00081290825 * std::sqrt(C57) * 7. *
                                         std::pow(C47, 1. / 6.) / 6. +
                                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                                  (std::pow(C47, 7. / 6.) * C81)) *
                                         0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                         std::pow(C47, 1. / 6.) /
                                         (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                                C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                    std::pow(C47, 7. / 6.) * C81 * 2 * C81 * 7. *
                                    std::pow(C47, 1. / 6.) / 6.) /
                                   std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                              C57 +
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                              C57) -
                     C152 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                         std::pow(C47, 4. / 3.) / 3.) /
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2)) *
                       C86 +
                   C111 *
                       (C146 * C47 * C51 * std::pow(C59, C51 - 1) +
                        C147 * C47 * C51 * std::pow(C60, C51 - 1)) /
                       (2. * (std::pow(2., C49) - 1.))) +
                  C96 *
                      (C146 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                           (1.3981 / (2 * std::sqrt(C63)) + 0.2611) +
                       C147 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                           (1.3981 / (2 * std::sqrt(C63)) + 0.2611)) /
                      (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                       (std::pow(2., C49) - 1.))) +
                 (((C47 *
                        ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                             (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                              std::pow(C59, C52 - 2) -
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) *
                              4. *
                              (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                   std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                                   C52 * std::pow(C59, C52 - 1)) /
                              (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                             std::pow(
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.),
                                         2),
                                 2)) *
                        C146 +
                    C147 * C47 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                        (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(C47, 7. / 3.) *
                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                             std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                             C52 * std::pow(C59, C52 - 1)) /
                        (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(
                             4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                             2))) -
                   C144 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 +
                        C57 * C81 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) /
                            (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                       std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) -
                  C152 * C47 *
                      (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                       C81 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                           std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                     C92) +
                ((C144 * C47 *
                      ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                            (C81 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                             0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81))) -
                        C57 * C81 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                           std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                      (std::pow(C47, 7. / 3.) *
                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                  (C147 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                       C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                   C146 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                       C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) +
                 C152 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                          std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                      (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                           (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                       C81 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                           std::pow(C47, 14. / 6.)) /
                          std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                     C57 /
                     (std::pow(C47, 7. / 3.) *
                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                    C113) +
               (((C146 * C47 *
                      std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                      (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(C47, 7. / 3.) *
                           std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                           std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2) *
                           C52 * std::pow(C60, C52 - 1)) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(
                           4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2),
                           2)) +
                  C47 *
                      ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                           (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                            std::pow(C60, C52 - 2) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                            (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                 C52 * std::pow(C60, C52 - 1)) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                      C147) -
                 C144 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2))) -
                C152 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) *
                   C93) +
              C47 *
                  ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                       std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) *
                  C224) +
             C47 *
                 ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                      std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) * C81 *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                 C225) +
            C227 * C47 *
                (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                 C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                     std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                C57 /
                (std::pow(C47, 7. / 3.) *
                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.));

        const double C153 = 2.;
        const double C154 = 0.;
        const double C155 = C153;
        const double C156 = C155;
        const double C157 = C154 / C47;
        const double C158 = C157;
        const double C159 = -C157;
        const double C160 = 0.;
        const double C161 = C160;
        const double C162 = C161;
        const double C163 = C162 / C78;
        const double C164 = C163;
        const double C228 = (-C86 * C154) / std::pow(C47, 2);
        const double C229 = C228;
        const double C230 = -C228;
        const double C231 = (-C110 * C162) / std::pow(C78, 2);
        const double C232 = C231;
        matrix.upper[3] =
            (((((((((((((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                            std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2))) -
                       C47 *
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * C52 * std::pow(C59, C52 - 1) * 7. *
                                     std::pow(C47, 4. / 3.) / 3. +
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) *
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                          C158 +
                      (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                            std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2))) -
                       C47 *
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * C52 * std::pow(C60, C52 - 1) * 7. *
                                     std::pow(C47, 4. / 3.) / 3. +
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                          C159) +
                     (std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C156 *
                          (C47 *
                               (C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) /
                                    (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 * C81 *
                                     -0.00081290825 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 C57 * C81 * -0.00081290825 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     2 * std::sqrt(C57) * C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                                    std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81,
                                             2)) +
                           (std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 +
                            C57 * C81 * -0.00081290825 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) /
                                (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81))) -
                      C156 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                               C81 +
                           C57 * C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                          std::pow(C47, 4. / 3.) / 3.) /
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2)) +
                    (std::pow(C47, 7. / 3.) *
                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C164 *
                         (C47 *
                              (std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                   std::pow(C47, 1. / 6.) /
                                   (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                               (std::pow(std::pow(C47, 7. / 6.) * C81, 2) * C81 *
                                    (std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                         0.00081290825 * std::sqrt(C57) * 7. *
                                         std::pow(C47, 1. / 6.) / 6. +
                                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                                  (std::pow(C47, 7. / 6.) * C81)) *
                                         0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                         std::pow(C47, 1. / 6.) /
                                         (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                                C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                    std::pow(C47, 7. / 6.) * C81 * 2 * C81 * 7. *
                                    std::pow(C47, 1. / 6.) / 6.) /
                                   std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                              C57 +
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                              C57) -
                     C164 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                         std::pow(C47, 4. / 3.) / 3.) /
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2)) *
                       C86 +
                   C111 *
                       (C158 * C47 * C51 * std::pow(C59, C51 - 1) +
                        C159 * C47 * C51 * std::pow(C60, C51 - 1)) /
                       (2. * (std::pow(2., C49) - 1.))) +
                  C96 *
                      (C158 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                           (1.3981 / (2 * std::sqrt(C63)) + 0.2611) +
                       C159 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                           (1.3981 / (2 * std::sqrt(C63)) + 0.2611)) /
                      (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                       (std::pow(2., C49) - 1.))) +
                 (((C47 *
                        ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                             (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                              std::pow(C59, C52 - 2) -
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) *
                              4. *
                              (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                   std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                                   C52 * std::pow(C59, C52 - 1)) /
                              (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                             std::pow(
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.),
                                         2),
                                 2)) *
                        C158 +
                    C159 * C47 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                        (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(C47, 7. / 3.) *
                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                             std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                             C52 * std::pow(C59, C52 - 1)) /
                        (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(
                             4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                             2))) -
                   C156 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 +
                        C57 * C81 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) /
                            (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                       std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) -
                  C164 * C47 *
                      (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                       C81 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                           std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                     C92) +
                ((C156 * C47 *
                      ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                            (C81 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                             0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81))) -
                        C57 * C81 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                           std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                      (std::pow(C47, 7. / 3.) *
                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                  (C159 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                       C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                   C158 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                       C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) +
                 C164 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                          std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                      (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                           (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                       C81 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                           std::pow(C47, 14. / 6.)) /
                          std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                     C57 /
                     (std::pow(C47, 7. / 3.) *
                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                    C113) +
               (((C158 * C47 *
                      std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                      (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(C47, 7. / 3.) *
                           std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                           std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2) *
                           C52 * std::pow(C60, C52 - 1)) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(
                           4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2),
                           2)) +
                  C47 *
                      ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                           (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                            std::pow(C60, C52 - 2) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                            (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                 C52 * std::pow(C60, C52 - 1)) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                      C159) -
                 C156 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2))) -
                C164 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) *
                   C93) +
              C47 *
                  ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                       std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) *
                  C229) +
             C47 *
                 ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                      std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) * C81 *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                 C230) +
            C232 * C47 *
                (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                 C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                     std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                C57 /
                (std::pow(C47, 7. / 3.) *
                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.));

        const double C165 = 0.;
        const double C166 = 1;
        const double C167 = C165 / C47;
        const double C168 = C167;
        const double C169 = -C167;
        const double C170 = 0.;
        const double C171 = C170;
        const double C172 = C171;
        const double C173 = C172 / C78;
        const double C174 = C173;
        const double C233 = (-C86 * C165) / std::pow(C47, 2);
        const double C234 = C233;
        const double C235 = -C233;
        const double C236 = (-C110 * C172) / std::pow(C78, 2);
        const double C237 = C236;
        matrix.upper[4] =
            (((((((((((((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                            std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2))) -
                       C47 *
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * C52 * std::pow(C59, C52 - 1) * 7. *
                                     std::pow(C47, 4. / 3.) / 3. +
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) *
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                          C168 +
                      (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                            std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2))) -
                       C47 *
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * C52 * std::pow(C60, C52 - 1) * 7. *
                                     std::pow(C47, 4. / 3.) / 3. +
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                          C169) +
                     (std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C166 *
                          (C47 *
                               (C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) /
                                    (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 * C81 *
                                     -0.00081290825 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 C57 * C81 * -0.00081290825 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     2 * std::sqrt(C57) * C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                                    std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81,
                                             2)) +
                           (std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 +
                            C57 * C81 * -0.00081290825 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) /
                                (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81))) -
                      C166 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                               C81 +
                           C57 * C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                          std::pow(C47, 4. / 3.) / 3.) /
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2)) +
                    (std::pow(C47, 7. / 3.) *
                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C174 *
                         (C47 *
                              (std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                   std::pow(C47, 1. / 6.) /
                                   (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                               (std::pow(std::pow(C47, 7. / 6.) * C81, 2) * C81 *
                                    (std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                         0.00081290825 * std::sqrt(C57) * 7. *
                                         std::pow(C47, 1. / 6.) / 6. +
                                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                                  (std::pow(C47, 7. / 6.) * C81)) *
                                         0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                         std::pow(C47, 1. / 6.) /
                                         (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                                C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                    std::pow(C47, 7. / 6.) * C81 * 2 * C81 * 7. *
                                    std::pow(C47, 1. / 6.) / 6.) /
                                   std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                              C57 +
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                              C57) -
                     C174 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                         std::pow(C47, 4. / 3.) / 3.) /
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2)) *
                       C86 +
                   C111 *
                       (C168 * C47 * C51 * std::pow(C59, C51 - 1) +
                        C169 * C47 * C51 * std::pow(C60, C51 - 1)) /
                       (2. * (std::pow(2., C49) - 1.))) +
                  C96 *
                      (C168 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                           (1.3981 / (2 * std::sqrt(C63)) + 0.2611) +
                       C169 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                           (1.3981 / (2 * std::sqrt(C63)) + 0.2611)) /
                      (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                       (std::pow(2., C49) - 1.))) +
                 (((C47 *
                        ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                             (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                              std::pow(C59, C52 - 2) -
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) *
                              4. *
                              (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                   std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                                   C52 * std::pow(C59, C52 - 1)) /
                              (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                             std::pow(
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.),
                                         2),
                                 2)) *
                        C168 +
                    C169 * C47 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                        (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(C47, 7. / 3.) *
                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                             std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                             C52 * std::pow(C59, C52 - 1)) /
                        (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(
                             4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                             2))) -
                   C166 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 +
                        C57 * C81 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) /
                            (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                       std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) -
                  C174 * C47 *
                      (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                       C81 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                           std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                     C92) +
                ((C166 * C47 *
                      ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                            (C81 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                             0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81))) -
                        C57 * C81 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                           std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                      (std::pow(C47, 7. / 3.) *
                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                  (C169 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                       C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                   C168 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                       C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) +
                 C174 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                          std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                      (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                           (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                       C81 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                           std::pow(C47, 14. / 6.)) /
                          std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                     C57 /
                     (std::pow(C47, 7. / 3.) *
                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                    C113) +
               (((C168 * C47 *
                      std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                      (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(C47, 7. / 3.) *
                           std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                           std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2) *
                           C52 * std::pow(C60, C52 - 1)) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(
                           4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2),
                           2)) +
                  C47 *
                      ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                           (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                            std::pow(C60, C52 - 2) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                            (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                 C52 * std::pow(C60, C52 - 1)) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                      C169) -
                 C166 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2))) -
                C174 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) *
                   C93) +
              C47 *
                  ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                       std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) *
                  C234) +
             C47 *
                 ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                      std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) * C81 *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                 C235) +
            C237 * C47 *
                (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                 C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                     std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                C57 /
                (std::pow(C47, 7. / 3.) *
                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.));

        const double C238 = 0.;
        const double C239 =
            (C238 / C47 +
             ((-C116) / std::pow(C47, 2) - (-C114 * C53 * 2 * C47) / std::pow(C47, 4)) * C114) -
            C116 * C114 / std::pow(C47, 2);
        const double C240 = C239;
        const double C241 = -C239;
        const double C242 = (-C118 * -0.22167 * C118 * 2 * C56) / std::pow(C56, 4);
        const double C243 = (-C118 * -3. * C118 * 2 * C56) / std::pow(C56, 4);
        const double C244 = C49 * (C49 - 1) * std::pow(C62, C49 - 2) * std::pow(C123, 2) +
                            C49 * std::pow(C62, C49 - 1) * C243;
        const double C245 = C50 * (C50 - 1) * std::pow(C62, C50 - 2) * std::pow(C123, 2) +
                            C50 * std::pow(C62, C50 - 1) * C243;
        const double C246 = 0.7389e-5 * C245 + 0.;
        const double C247 = 0.023266 * C244;
        const double C248 = 0.3334 * C244;
        const double C249 = 0.472 * C245;
        const double C250 = 8.723 * C244;
        const double C251 =
            1.0529 * C244 / (2 * std::sqrt(C63)) -
            1.0529 * std::pow(C124, 2) / (std::sqrt(C63) * std::pow(2 * std::sqrt(C63), 2));
        const double C252 = C247 + C246;
        const double C253 = C250 + C249;
        const double C254 = C251 + C248;
        const double C255 = C252;
        const double C256 = C253 + C242;
        const double C257 = C254;
        const double C258 = C256;
        const double C259 =
            -0.1423 * C257 / std::pow(C77, 2) - C137 * -0.1423 * C137 * 2 * C77 / std::pow(C77, 4);
        const double C260 =
            ((C255 / C78 +
              ((-C135) / std::pow(C78, 2) - (-C138 * C75 * 2 * C78) / std::pow(C78, 4)) * C138) -
             C258 * C75 / std::pow(C78, 2)) -
            C135 * C138 / std::pow(C78, 2);
        const double C261 = C260;
        matrix.upper[5] =
            ((((((((((((((((std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                (C47 * ((6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                                             (std::pow(C47, 7. / 3.) *
                                                  std::sqrt(
                                                      (std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.) *
                                                  C57 * C81 *
                                                  (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                            (std::pow(C47, 7. / 6.) * C81)) *
                                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, -5. / 6.) / 6. +
                                                   0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, 1. / 6.) *
                                                       std::exp(-0.00081290825 * std::sqrt(C57) /
                                                                (std::pow(C47, 7. / 6.) * C81)) *
                                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, 1. / 6.) /
                                                       (6. * std::pow(std::pow(C47, 7. / 6.) * C81,
                                                                      2))) +
                                              C57 * C81 *
                                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                                           (std::pow(C47, 7. / 6.) * C81)) *
                                                  0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                  std::pow(C47, 1. / 6.) *
                                                  std::sqrt(
                                                      (std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.) *
                                                  7. * std::pow(C47, 4. / 3.) / 3.) -
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.) *
                                             C57 * C81 *
                                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81)) *
                                             0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                             std::pow(C47, 1. / 6.) * 6. * std::pow(C47, 7. / 6.) *
                                             C81 * 2 * C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                                            std::pow(6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2),
                                                     2) -
                                        (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                  (std::pow(C47, 7. / 6.) * C81)) *
                                             C81 * C57 *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.) *
                                             28. * std::pow(C47, 1. / 3.) / 3. +
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                             7. * std::pow(C47, 4. / 3.) * C57 * C81 *
                                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81)) *
                                             0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                             std::pow(C47, 1. / 6.) /
                                             (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) /
                                            3.) +
                                 (std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      C57 * C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.) /
                                      (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      C81 * C57 *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      7. * std::pow(C47, 4. / 3.) / 3.)) -
                            C47 *
                                (std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     7. * std::pow(C47, 4. / 3.) / 3.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   4) +
                           (std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                C81 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * C81 * 7. * std::pow(C47, 1. / 6.) /
                                (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2)) *
                              C114 +
                          (((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                               (2. * (std::pow(2., C49) - 1.)) -
                           1) *
                              C139) +
                         C124 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                             (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                             (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                              (std::pow(2., C49) - 1.))) +
                        (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                              std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                              (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) -
                         C47 *
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C57) /
                                            (std::pow(C47, 7. / 6.) * C81)) *
                                       C81 * C57 * C52 * std::pow(C59, C52 - 1) * 7. *
                                       std::pow(C47, 4. / 3.) / 3. +
                                   std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * C57 *
                                       C81 *
                                       std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                       std::pow(C47, 1. / 6.) /
                                       (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 *
                                  std::pow(C59, C52 - 1) * 4. *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                  std::pow(C47, 4. / 3.) / 3.) /
                             std::pow(
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.),
                                         2),
                                 2)) *
                            C120) +
                       (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                             std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2))) -
                        C47 *
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                 (std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      C81 * C57 * C52 * std::pow(C60, C52 - 1) * 7. *
                                      std::pow(C47, 4. / 3.) / 3. +
                                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * C57 *
                                      C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.) /
                                      (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                                 C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                 std::pow(C47, 4. / 3.) / 3.) /
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2)) *
                           C121) +
                      (std::pow(C47, 7. / 3.) *
                           std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C141 *
                           (C47 *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                 (std::pow(std::pow(C47, 7. / 6.) * C81, 2) * C81 *
                                      (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * 7. *
                                           std::pow(C47, 1. / 6.) / 6. +
                                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                                    (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                           std::pow(C47, 1. / 6.) /
                                           (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                                  C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                      std::pow(C47, 7. / 6.) * C81 * 2 * C81 * 7. *
                                      std::pow(C47, 1. / 6.) / 6.) /
                                     std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                                C57 +
                            (std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) +
                             C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                                C57) -
                       C141 * C47 *
                           (std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) +
                            C81 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                           C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                           std::pow(C47, 4. / 3.) / 3.) /
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2)) *
                         C114 +
                     (((((((std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C47 *
                                (C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) * std::pow(C47, 7. / 3.) * C52 *
                                     std::pow(C59, C52 - 1) /
                                     (4. *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * 7. * std::pow(C47, 4. / 3.) * C52 *
                                     std::pow(C59, C52 - 1) /
                                     (12. *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) -
                            C47 *
                                (std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     7. * std::pow(C47, 4. / 3.) / 3.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   4) +
                           ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                                std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2)))) *
                              C114 +
                          C139 * C47 * C51 * std::pow(C59, C51 - 1) /
                              (2. * (std::pow(2., C49) - 1.))) +
                         C124 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                             (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                             (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                              (std::pow(2., C49) - 1.))) +
                        C47 *
                            ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) *
                                 C51 * (C51 - 1) * std::pow(C59, C51 - 2) /
                                 (2. * (std::pow(2., C49) - 1.)) -
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                                  std::pow(C59, C52 - 2) -
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 *
                                  std::pow(C59, C52 - 1) * 4. *
                                  (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       2 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2) *
                                       C52 * std::pow(C59, C52 - 1)) /
                                  (4. *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                                 std::pow(
                                     4. *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                         std::pow(std::pow(C47, 7. / 3.) *
                                                      std::sqrt((std::pow(C59, C52) +
                                                                 std::pow(C60, C52)) /
                                                                2.),
                                                  2),
                                     2)) *
                            C120) +
                       C121 * C47 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2))) -
                      C141 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                          C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                          (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2))) *
                         C120) +
                    (((((((std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                               C47 *
                               (C57 * C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) * std::pow(C47, 7. / 3.) * C52 *
                                    std::pow(C60, C52 - 1) /
                                    (4. *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                    C81 * C57 * 7. * std::pow(C47, 4. / 3.) * C52 *
                                    std::pow(C60, C52 - 1) /
                                    (12. *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) -
                           C47 *
                               (std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    C57 * C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) /
                                    (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                    C81 * C57 *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                    std::pow(C47, 4. / 3.) / 3.) *
                               std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                               std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                               (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  4) +
                          ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                               std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                               C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                               (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2)))) *
                             C114 +
                         C139 * C47 * C51 * std::pow(C60, C51 - 1) /
                             (2. * (std::pow(2., C49) - 1.))) +
                        C124 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                            (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                            (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                             (std::pow(2., C49) - 1.))) +
                       C120 * C47 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C60, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2))) +
                      C47 *
                          ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                               (C51 - 1) * std::pow(C60, C51 - 2) /
                               (2. * (std::pow(2., C49) - 1.)) -
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                                std::pow(C60, C52 - 2) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                4. *
                                (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                     C52 * std::pow(C60, C52 - 1)) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                               std::pow(
                                   4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       std::pow(
                                           std::pow(C47, 7. / 3.) *
                                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                         2.),
                                           2),
                                   2)) *
                          C121) -
                     C141 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) *
                        C121) +
                   ((((C47 *
                           ((6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                 (C81 *
                                      (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * 7. *
                                           std::pow(C47, 1. / 6.) +
                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                           std::pow(C47, 1. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                                    (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                           std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.)) -
                             std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                 C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                 std::pow(C47, 1. / 6.) * 12. * C81 * std::pow(C47, 14. / 6.)) /
                                std::pow(6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2), 2) -
                            (std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) +
                             C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                                C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                7. * std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) +
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                           C57 /
                           (std::pow(C47, 7. / 3.) *
                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                          C114 -
                      C120 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                          C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                          (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2))) -
                     C121 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) +
                    C141 * C47 *
                        (std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                              (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                   std::exp(-0.00081290825 * std::sqrt(C57) /
                                            (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                   std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                              std::pow(C47, 14. / 6.)) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                        C57 /
                        (std::pow(C47, 7. / 3.) *
                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                       C141) +
                  (((((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (2. * (std::pow(2., C49) - 1.)) -
                     1) *
                        C114 +
                    C120 * C47 * C51 * std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.))) +
                   C121 * C47 * C51 * std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.))) *
                      C139) +
                 (((C114 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                        (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                        (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                         (std::pow(2., C49) - 1.)) +
                    (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                         (std::pow(2., C49) - 1.) * C124 * C47 * -0.11785983 *
                         ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (std::sqrt(C63) * std::pow(2 * std::sqrt(C63), 2)) -
                     C124 * C47 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                         (1.3981 / (2 * std::sqrt(C63)) + 0.2611) * 2 *
                         (1.3981 / (2 * std::sqrt(C63)) + 0.2611) *
                         ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.) * 2. *
                         (std::pow(2., C49) - 1.)) /
                        std::pow(std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                                     (std::pow(2., C49) - 1.),
                                 2)) +
                   C120 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                       (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                       (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                        (std::pow(2., C49) - 1.))) +
                  C121 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                      (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                      (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                       (std::pow(2., C49) - 1.))) *
                     C124) +
                C47 *
                    (((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (2. * (std::pow(2., C49) - 1.)) -
                     1) *
                    C259) +
               C244 * C47 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                   (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                   (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                    (std::pow(2., C49) - 1.))) +
              C47 *
                  ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                       std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) *
                  C240) +
             C47 *
                 ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                      std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) * C81 *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                 C241) +
            C261 * C47 *
                (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                 C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                     std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                C57 /
                (std::pow(C47, 7. / 3.) *
                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.));

        const double C262 = (-C114 * C142) / std::pow(C47, 2);
        const double C263 = C262;
        const double C264 = -C262;
        const double C265 = (-C138 * C150) / std::pow(C78, 2);
        const double C266 = C265;
        matrix.upper[6] =
            (((((((((((((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                            std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2))) -
                       C47 *
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * C52 * std::pow(C59, C52 - 1) * 7. *
                                     std::pow(C47, 4. / 3.) / 3. +
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) *
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                          C146 +
                      (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                            std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2))) -
                       C47 *
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * C52 * std::pow(C60, C52 - 1) * 7. *
                                     std::pow(C47, 4. / 3.) / 3. +
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                          C147) +
                     (std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C144 *
                          (C47 *
                               (C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) /
                                    (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 * C81 *
                                     -0.00081290825 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 C57 * C81 * -0.00081290825 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     2 * std::sqrt(C57) * C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                                    std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81,
                                             2)) +
                           (std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 +
                            C57 * C81 * -0.00081290825 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) /
                                (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81))) -
                      C144 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                               C81 +
                           C57 * C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                          std::pow(C47, 4. / 3.) / 3.) /
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2)) +
                    (std::pow(C47, 7. / 3.) *
                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C152 *
                         (C47 *
                              (std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                   std::pow(C47, 1. / 6.) /
                                   (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                               (std::pow(std::pow(C47, 7. / 6.) * C81, 2) * C81 *
                                    (std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                         0.00081290825 * std::sqrt(C57) * 7. *
                                         std::pow(C47, 1. / 6.) / 6. +
                                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                                  (std::pow(C47, 7. / 6.) * C81)) *
                                         0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                         std::pow(C47, 1. / 6.) /
                                         (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                                C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                    std::pow(C47, 7. / 6.) * C81 * 2 * C81 * 7. *
                                    std::pow(C47, 1. / 6.) / 6.) /
                                   std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                              C57 +
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                              C57) -
                     C152 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                         std::pow(C47, 4. / 3.) / 3.) /
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2)) *
                       C114 +
                   C139 *
                       (C146 * C47 * C51 * std::pow(C59, C51 - 1) +
                        C147 * C47 * C51 * std::pow(C60, C51 - 1)) /
                       (2. * (std::pow(2., C49) - 1.))) +
                  C124 *
                      (C146 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                           (1.3981 / (2 * std::sqrt(C63)) + 0.2611) +
                       C147 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                           (1.3981 / (2 * std::sqrt(C63)) + 0.2611)) /
                      (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                       (std::pow(2., C49) - 1.))) +
                 (((C47 *
                        ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                             (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                              std::pow(C59, C52 - 2) -
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) *
                              4. *
                              (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                   std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                                   C52 * std::pow(C59, C52 - 1)) /
                              (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                             std::pow(
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.),
                                         2),
                                 2)) *
                        C146 +
                    C147 * C47 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                        (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(C47, 7. / 3.) *
                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                             std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                             C52 * std::pow(C59, C52 - 1)) /
                        (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(
                             4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                             2))) -
                   C144 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 +
                        C57 * C81 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) /
                            (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                       std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) -
                  C152 * C47 *
                      (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                       C81 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                           std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                     C120) +
                ((C144 * C47 *
                      ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                            (C81 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                             0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81))) -
                        C57 * C81 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                           std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                      (std::pow(C47, 7. / 3.) *
                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                  (C147 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                       C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                   C146 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                       C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) +
                 C152 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                          std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                      (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                           (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                       C81 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                           std::pow(C47, 14. / 6.)) /
                          std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                     C57 /
                     (std::pow(C47, 7. / 3.) *
                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                    C141) +
               (((C146 * C47 *
                      std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                      (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(C47, 7. / 3.) *
                           std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                           std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2) *
                           C52 * std::pow(C60, C52 - 1)) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(
                           4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2),
                           2)) +
                  C47 *
                      ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                           (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                            std::pow(C60, C52 - 2) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                            (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                 C52 * std::pow(C60, C52 - 1)) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                      C147) -
                 C144 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2))) -
                C152 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) *
                   C121) +
              C47 *
                  ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                       std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) *
                  C263) +
             C47 *
                 ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                      std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) * C81 *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                 C264) +
            C266 * C47 *
                (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                 C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                     std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                C57 /
                (std::pow(C47, 7. / 3.) *
                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.));

        const double C267 = (-C114 * C154) / std::pow(C47, 2);
        const double C268 = C267;
        const double C269 = -C267;
        const double C270 = (-C138 * C162) / std::pow(C78, 2);
        const double C271 = C270;
        matrix.upper[7] =
            (((((((((((((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                            std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2))) -
                       C47 *
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * C52 * std::pow(C59, C52 - 1) * 7. *
                                     std::pow(C47, 4. / 3.) / 3. +
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) *
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                          C158 +
                      (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                            std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2))) -
                       C47 *
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * C52 * std::pow(C60, C52 - 1) * 7. *
                                     std::pow(C47, 4. / 3.) / 3. +
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                          C159) +
                     (std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C156 *
                          (C47 *
                               (C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) /
                                    (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 * C81 *
                                     -0.00081290825 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 C57 * C81 * -0.00081290825 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     2 * std::sqrt(C57) * C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                                    std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81,
                                             2)) +
                           (std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 +
                            C57 * C81 * -0.00081290825 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) /
                                (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81))) -
                      C156 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                               C81 +
                           C57 * C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                          std::pow(C47, 4. / 3.) / 3.) /
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2)) +
                    (std::pow(C47, 7. / 3.) *
                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C164 *
                         (C47 *
                              (std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                   std::pow(C47, 1. / 6.) /
                                   (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                               (std::pow(std::pow(C47, 7. / 6.) * C81, 2) * C81 *
                                    (std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                         0.00081290825 * std::sqrt(C57) * 7. *
                                         std::pow(C47, 1. / 6.) / 6. +
                                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                                  (std::pow(C47, 7. / 6.) * C81)) *
                                         0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                         std::pow(C47, 1. / 6.) /
                                         (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                                C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                    std::pow(C47, 7. / 6.) * C81 * 2 * C81 * 7. *
                                    std::pow(C47, 1. / 6.) / 6.) /
                                   std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                              C57 +
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                              C57) -
                     C164 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                         std::pow(C47, 4. / 3.) / 3.) /
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2)) *
                       C114 +
                   C139 *
                       (C158 * C47 * C51 * std::pow(C59, C51 - 1) +
                        C159 * C47 * C51 * std::pow(C60, C51 - 1)) /
                       (2. * (std::pow(2., C49) - 1.))) +
                  C124 *
                      (C158 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                           (1.3981 / (2 * std::sqrt(C63)) + 0.2611) +
                       C159 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                           (1.3981 / (2 * std::sqrt(C63)) + 0.2611)) /
                      (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                       (std::pow(2., C49) - 1.))) +
                 (((C47 *
                        ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                             (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                              std::pow(C59, C52 - 2) -
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) *
                              4. *
                              (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                   std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                                   C52 * std::pow(C59, C52 - 1)) /
                              (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                             std::pow(
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.),
                                         2),
                                 2)) *
                        C158 +
                    C159 * C47 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                        (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(C47, 7. / 3.) *
                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                             std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                             C52 * std::pow(C59, C52 - 1)) /
                        (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(
                             4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                             2))) -
                   C156 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 +
                        C57 * C81 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) /
                            (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                       std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) -
                  C164 * C47 *
                      (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                       C81 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                           std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                     C120) +
                ((C156 * C47 *
                      ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                            (C81 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                             0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81))) -
                        C57 * C81 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                           std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                      (std::pow(C47, 7. / 3.) *
                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                  (C159 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                       C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                   C158 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                       C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) +
                 C164 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                          std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                      (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                           (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                       C81 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                           std::pow(C47, 14. / 6.)) /
                          std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                     C57 /
                     (std::pow(C47, 7. / 3.) *
                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                    C141) +
               (((C158 * C47 *
                      std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                      (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(C47, 7. / 3.) *
                           std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                           std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2) *
                           C52 * std::pow(C60, C52 - 1)) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(
                           4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2),
                           2)) +
                  C47 *
                      ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                           (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                            std::pow(C60, C52 - 2) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                            (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                 C52 * std::pow(C60, C52 - 1)) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                      C159) -
                 C156 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2))) -
                C164 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) *
                   C121) +
              C47 *
                  ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                       std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) *
                  C268) +
             C47 *
                 ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                      std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) * C81 *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                 C269) +
            C271 * C47 *
                (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                 C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                     std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                C57 /
                (std::pow(C47, 7. / 3.) *
                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.));

        const double C272 = (-C114 * C165) / std::pow(C47, 2);
        const double C273 = C272;
        const double C274 = -C272;
        const double C275 = (-C138 * C172) / std::pow(C78, 2);
        const double C276 = C275;
        matrix.upper[8] =
            (((((((((((((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                            std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2))) -
                       C47 *
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * C52 * std::pow(C59, C52 - 1) * 7. *
                                     std::pow(C47, 4. / 3.) / 3. +
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) *
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                          C168 +
                      (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                            std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2))) -
                       C47 *
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * C52 * std::pow(C60, C52 - 1) * 7. *
                                     std::pow(C47, 4. / 3.) / 3. +
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                          C169) +
                     (std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C166 *
                          (C47 *
                               (C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) /
                                    (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 * C81 *
                                     -0.00081290825 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 C57 * C81 * -0.00081290825 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     2 * std::sqrt(C57) * C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                                    std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81,
                                             2)) +
                           (std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 +
                            C57 * C81 * -0.00081290825 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) /
                                (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81))) -
                      C166 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                               C81 +
                           C57 * C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                          std::pow(C47, 4. / 3.) / 3.) /
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2)) +
                    (std::pow(C47, 7. / 3.) *
                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C174 *
                         (C47 *
                              (std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                   std::pow(C47, 1. / 6.) /
                                   (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                               (std::pow(std::pow(C47, 7. / 6.) * C81, 2) * C81 *
                                    (std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                         0.00081290825 * std::sqrt(C57) * 7. *
                                         std::pow(C47, 1. / 6.) / 6. +
                                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                                  (std::pow(C47, 7. / 6.) * C81)) *
                                         0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                         std::pow(C47, 1. / 6.) /
                                         (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                                C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                    std::pow(C47, 7. / 6.) * C81 * 2 * C81 * 7. *
                                    std::pow(C47, 1. / 6.) / 6.) /
                                   std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                              C57 +
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                              C57) -
                     C174 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                         std::pow(C47, 4. / 3.) / 3.) /
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2)) *
                       C114 +
                   C139 *
                       (C168 * C47 * C51 * std::pow(C59, C51 - 1) +
                        C169 * C47 * C51 * std::pow(C60, C51 - 1)) /
                       (2. * (std::pow(2., C49) - 1.))) +
                  C124 *
                      (C168 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                           (1.3981 / (2 * std::sqrt(C63)) + 0.2611) +
                       C169 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                           (1.3981 / (2 * std::sqrt(C63)) + 0.2611)) /
                      (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                       (std::pow(2., C49) - 1.))) +
                 (((C47 *
                        ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                             (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                              std::pow(C59, C52 - 2) -
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) *
                              4. *
                              (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                   std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                                   C52 * std::pow(C59, C52 - 1)) /
                              (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                             std::pow(
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.),
                                         2),
                                 2)) *
                        C168 +
                    C169 * C47 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                        (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                             std::pow(C47, 7. / 3.) *
                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                             std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                             C52 * std::pow(C59, C52 - 1)) /
                        (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(
                             4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                             2))) -
                   C166 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 +
                        C57 * C81 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) /
                            (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                       std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) -
                  C174 * C47 *
                      (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                       C81 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                           std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                     C120) +
                ((C166 * C47 *
                      ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                            (C81 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                             0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81))) -
                        C57 * C81 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                           std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                      (std::pow(C47, 7. / 3.) *
                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                  (C169 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                       C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                   C168 * C47 *
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                       C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) +
                 C174 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                          std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                      (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                           (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                       C81 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                           std::pow(C47, 14. / 6.)) /
                          std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                     C57 /
                     (std::pow(C47, 7. / 3.) *
                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                    C141) +
               (((C168 * C47 *
                      std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                      (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(C47, 7. / 3.) *
                           std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                           std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2) *
                           C52 * std::pow(C60, C52 - 1)) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(
                           4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2),
                           2)) +
                  C47 *
                      ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                           (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                            std::pow(C60, C52 - 2) -
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                            C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                            (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                 std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                             std::pow(std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                 C52 * std::pow(C60, C52 - 1)) /
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                           std::pow(
                               4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2)) *
                      C169) -
                 C166 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2))) -
                C174 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) *
                   C121) +
              C47 *
                  ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                       std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) *
                  C273) +
             C47 *
                 ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                      std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) * C81 *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                 C274) +
            C276 * C47 *
                (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                 C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                     std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                C57 /
                (std::pow(C47, 7. / 3.) *
                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.));

        matrix.upper[9] =
            (((((C47 *
                     ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                          (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                           std::pow(C59, C52 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2),
                              2)) *
                     C146 +
                 C147 * C47 *
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                     (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                          std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2) *
                          C52 * std::pow(C59, C52 - 1)) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2))) -
                C144 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 +
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                    std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) -
               C152 * C47 *
                   (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                        std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                   C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) *
                  C146 +
              ((C144 * C47 *
                    (C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 *
                          (C57 * C81 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                           C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81))) -
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                          std::pow(C47, 7. / 6.) * C81 / std::sqrt(C57)) /
                         std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                    (std::pow(C47, 7. / 3.) *
                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                (C147 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                 C146 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) +
               C152 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     C81 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::pow(C47, 7. / 6.) / (2 * std::sqrt(C57)) +
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) /
                              (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                        C57) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                  C144) +
             ((C144 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                         (C81 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                   (std::pow(C47, 7. / 6.) * C81))) -
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
               (C147 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                C146 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) +
              C152 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                   (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                        (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                        std::pow(C47, 14. / 6.)) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                  C57 /
                  (std::pow(C47, 7. / 3.) *
                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                 C152) +
            (((C146 * C47 *
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                   C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                   (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(C47, 7. / 3.) *
                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                        std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2) *
                        C52 * std::pow(C60, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                        2)) +
               C47 *
                   ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                        (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                         std::pow(C60, C52 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                         (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(C47, 7. / 3.) *
                              std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                              std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                              C52 * std::pow(C60, C52 - 1)) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2),
                            2)) *
                   C147) -
              C144 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 +
                   C57 * C81 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) /
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                  (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                   std::pow(std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                            2))) -
             C152 * C47 *
                 (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                  C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                      std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                 C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                 (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                  std::pow(std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                           2))) *
                C147;

        matrix.upper[10] =
            (((((C47 *
                     ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                          (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                           std::pow(C59, C52 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2),
                              2)) *
                     C146 +
                 C147 * C47 *
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                     (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                          std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2) *
                          C52 * std::pow(C59, C52 - 1)) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2))) -
                C144 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 +
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                    std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) -
               C152 * C47 *
                   (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                        std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                   C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) *
                  C158 +
              ((C144 * C47 *
                    (C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 *
                          (C57 * C81 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                           C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81))) -
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                          std::pow(C47, 7. / 6.) * C81 / std::sqrt(C57)) /
                         std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                    (std::pow(C47, 7. / 3.) *
                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                (C147 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                 C146 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) +
               C152 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     C81 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::pow(C47, 7. / 6.) / (2 * std::sqrt(C57)) +
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) /
                              (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                        C57) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                  C156) +
             ((C144 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                         (C81 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                   (std::pow(C47, 7. / 6.) * C81))) -
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
               (C147 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                C146 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) +
              C152 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                   (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                        (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                        std::pow(C47, 14. / 6.)) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                  C57 /
                  (std::pow(C47, 7. / 3.) *
                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                 C164) +
            (((C146 * C47 *
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                   C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                   (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(C47, 7. / 3.) *
                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                        std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2) *
                        C52 * std::pow(C60, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                        2)) +
               C47 *
                   ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                        (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                         std::pow(C60, C52 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                         (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(C47, 7. / 3.) *
                              std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                              std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                              C52 * std::pow(C60, C52 - 1)) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2),
                            2)) *
                   C147) -
              C144 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 +
                   C57 * C81 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) /
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                  (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                   std::pow(std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                            2))) -
             C152 * C47 *
                 (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                  C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                      std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                 C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                 (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                  std::pow(std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                           2))) *
                C159;

        matrix.upper[11] =
            (((((C47 *
                     ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                          (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                           std::pow(C59, C52 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2),
                              2)) *
                     C146 +
                 C147 * C47 *
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                     (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                          std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2) *
                          C52 * std::pow(C59, C52 - 1)) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2))) -
                C144 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 +
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                    std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) -
               C152 * C47 *
                   (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                        std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                   C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) *
                  C168 +
              ((C144 * C47 *
                    (C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 *
                          (C57 * C81 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                           C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81))) -
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                          std::pow(C47, 7. / 6.) * C81 / std::sqrt(C57)) /
                         std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                    (std::pow(C47, 7. / 3.) *
                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                (C147 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                 C146 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) +
               C152 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     C81 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::pow(C47, 7. / 6.) / (2 * std::sqrt(C57)) +
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) /
                              (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                        C57) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                  C166) +
             ((C144 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                         (C81 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                   (std::pow(C47, 7. / 6.) * C81))) -
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
               (C147 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                C146 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) +
              C152 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                   (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                        (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                        std::pow(C47, 14. / 6.)) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                  C57 /
                  (std::pow(C47, 7. / 3.) *
                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                 C174) +
            (((C146 * C47 *
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                   C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                   (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(C47, 7. / 3.) *
                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                        std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2) *
                        C52 * std::pow(C60, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                        2)) +
               C47 *
                   ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                        (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                         std::pow(C60, C52 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                         (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(C47, 7. / 3.) *
                              std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                              std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                              C52 * std::pow(C60, C52 - 1)) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2),
                            2)) *
                   C147) -
              C144 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 +
                   C57 * C81 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) /
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                  (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                   std::pow(std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                            2))) -
             C152 * C47 *
                 (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                  C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                      std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                 C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                 (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                  std::pow(std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                           2))) *
                C169;

        matrix.upper[12] =
            (((((C47 *
                     ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                          (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                           std::pow(C59, C52 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2),
                              2)) *
                     C158 +
                 C159 * C47 *
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                     (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                          std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2) *
                          C52 * std::pow(C59, C52 - 1)) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2))) -
                C156 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 +
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                    std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) -
               C164 * C47 *
                   (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                        std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                   C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) *
                  C158 +
              ((C156 * C47 *
                    (C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 *
                          (C57 * C81 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                           C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81))) -
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                          std::pow(C47, 7. / 6.) * C81 / std::sqrt(C57)) /
                         std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                    (std::pow(C47, 7. / 3.) *
                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                (C159 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                 C158 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) +
               C164 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     C81 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::pow(C47, 7. / 6.) / (2 * std::sqrt(C57)) +
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) /
                              (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                        C57) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                  C156) +
             ((C156 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                         (C81 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                   (std::pow(C47, 7. / 6.) * C81))) -
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
               (C159 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                C158 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) +
              C164 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                   (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                        (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                        std::pow(C47, 14. / 6.)) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                  C57 /
                  (std::pow(C47, 7. / 3.) *
                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                 C164) +
            (((C158 * C47 *
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                   C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                   (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(C47, 7. / 3.) *
                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                        std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2) *
                        C52 * std::pow(C60, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                        2)) +
               C47 *
                   ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                        (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                         std::pow(C60, C52 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                         (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(C47, 7. / 3.) *
                              std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                              std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                              C52 * std::pow(C60, C52 - 1)) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2),
                            2)) *
                   C159) -
              C156 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 +
                   C57 * C81 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) /
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                  (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                   std::pow(std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                            2))) -
             C164 * C47 *
                 (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                  C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                      std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                 C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                 (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                  std::pow(std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                           2))) *
                C159;

        matrix.upper[13] =
            (((((C47 *
                     ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                          (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                           std::pow(C59, C52 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2),
                              2)) *
                     C158 +
                 C159 * C47 *
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                     (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                          std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2) *
                          C52 * std::pow(C59, C52 - 1)) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2))) -
                C156 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 +
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                    std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) -
               C164 * C47 *
                   (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                        std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                   C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) *
                  C168 +
              ((C156 * C47 *
                    (C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 *
                          (C57 * C81 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                           C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81))) -
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                          std::pow(C47, 7. / 6.) * C81 / std::sqrt(C57)) /
                         std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                    (std::pow(C47, 7. / 3.) *
                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                (C159 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                 C158 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) +
               C164 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     C81 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::pow(C47, 7. / 6.) / (2 * std::sqrt(C57)) +
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) /
                              (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                        C57) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                  C166) +
             ((C156 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                         (C81 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                   (std::pow(C47, 7. / 6.) * C81))) -
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
               (C159 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                C158 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) +
              C164 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                   (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                        (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                        std::pow(C47, 14. / 6.)) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                  C57 /
                  (std::pow(C47, 7. / 3.) *
                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                 C174) +
            (((C158 * C47 *
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                   C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                   (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(C47, 7. / 3.) *
                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                        std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2) *
                        C52 * std::pow(C60, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                        2)) +
               C47 *
                   ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                        (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                         std::pow(C60, C52 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                         (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(C47, 7. / 3.) *
                              std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                              std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                              C52 * std::pow(C60, C52 - 1)) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2),
                            2)) *
                   C159) -
              C156 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 +
                   C57 * C81 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) /
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                  (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                   std::pow(std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                            2))) -
             C164 * C47 *
                 (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                  C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                      std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                 C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                 (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                  std::pow(std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                           2))) *
                C169;

        matrix.upper[14] =
            (((((C47 *
                     ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                          (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                           std::pow(C59, C52 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2),
                              2)) *
                     C168 +
                 C169 * C47 *
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                     (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                          std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2) *
                          C52 * std::pow(C59, C52 - 1)) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2))) -
                C166 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 +
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                    std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) -
               C174 * C47 *
                   (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                        std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                   C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) *
                  C168 +
              ((C166 * C47 *
                    (C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 *
                          (C57 * C81 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                           C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81))) -
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                          std::pow(C47, 7. / 6.) * C81 / std::sqrt(C57)) /
                         std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                    (std::pow(C47, 7. / 3.) *
                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                (C169 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                 C168 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) +
               C174 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     C81 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::pow(C47, 7. / 6.) / (2 * std::sqrt(C57)) +
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) /
                              (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                        C57) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                  C166) +
             ((C166 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                         (C81 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                   (std::pow(C47, 7. / 6.) * C81))) -
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
               (C169 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                C168 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) +
              C174 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                   (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                        (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                        std::pow(C47, 14. / 6.)) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                  C57 /
                  (std::pow(C47, 7. / 3.) *
                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                 C174) +
            (((C168 * C47 *
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                   C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                   (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(C47, 7. / 3.) *
                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                        std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2) *
                        C52 * std::pow(C60, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                        2)) +
               C47 *
                   ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                        (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                         std::pow(C60, C52 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                         (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(C47, 7. / 3.) *
                              std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                              std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                              C52 * std::pow(C60, C52 - 1)) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2),
                            2)) *
                   C169) -
              C166 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 +
                   C57 * C81 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) /
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                  (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                   std::pow(std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                            2))) -
             C174 * C47 *
                 (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                  C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                      std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                 C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                 (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                  std::pow(std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                           2))) *
                C169;

    } else
    {
        const double C277 = 2. * sigmaAb;
        const double C278 = rhoA + rhoB;
        const double C279 = rhoA - rhoB;
        const double C280 = 1. / 3.;
        const double C281 = 2. / 3.;
        const double C282 = 4. / 3.;
        const double C283 = 5. / 3.;
        const double C284 = 0.9999999999999999 * C279;
        const double C285 = Pi * C278;
        const double C286 = sigmaAa + C277;
        const double C287 = 4. * C285;
        const double C288 = C286 + sigmaBb;
        const double C289 = C284 / C278;
        const double C290 = C289 + 1.;
        const double C291 = 1. - C289;
        const double C292 = 0.22167 / C287;
        const double C293 = 3. / C287;
        const double C294 = std::pow(C293, C280);
        const double C295 = std::pow(C293, C281);
        const double C296 = 7.389e-6 * C295;
        const double C297 = 0.0116 * C294;
        const double C298 = 0.023266 * C294;
        const double C299 = 0.472 * C295;
        const double C300 = 8.723 * C294;
        const double C301 = std::log(C294);
        const double C302 = 0.0311 * C301;
        const double C303 = C294 * C301;
        const double C304 = C298 + C296;
        const double C305 = C300 + C299;
        const double C306 = 0.0020 * C303;
        const double C307 = C304 + 0.002568;
        const double C308 = C305 + C292;
        const double C309 = C302 + C306;
        const double C310 = C308 + 1.;
        const double C311 = C309 - 0.048;
        const double C312 = C307 / C310;
        const double C313 = C312 + 0.001667;
        const double C314 = C311 - C297;
        const double C316 = 1;
        const double C317 = 1;
        const double C318 = 0.9999999999999999 * C317 + 0.;
        const double C319 = Pi * C316;
        const double C320 = 4. * C319;
        const double C321 = C318 / C278 - C316 * C284 / std::pow(C278, 2);
        const double C322 = C321;
        const double C323 = -C321;
        const double C324 = -0.22167 * C320 / std::pow(C287, 2);
        const double C325 = -3. * C320 / std::pow(C287, 2);
        const double C326 = C280 * std::pow(C293, C280 - 1) * C325;
        const double C327 = C281 * std::pow(C293, C281 - 1) * C325;
        const double C328 = 0.7389e-5 * C327 + 0.;
        const double C329 = 0.0116 * C326;
        const double C330 = 0.023266 * C326;
        const double C331 = 0.472 * C327;
        const double C332 = 8.723 * C326;
        const double C333 = C326 / C294;
        const double C334 = 0.0311 * C333;
        const double C335 = C301 * C326 + C294 * C333;
        const double C336 = C330 + C328;
        const double C337 = C332 + C331;
        const double C338 = 0.002 * C335;
        const double C339 = C336;
        const double C340 = C337 + C324;
        const double C341 = C334 + C338;
        const double C342 = C340;
        const double C343 = C341;
        const double C344 = C339 / C310 - C342 * C307 / std::pow(C310, 2);
        const double C345 = C344;
        const double C346 = C343 - C329;
        const double C411 = 0.;
        const double C412 =
            (C411 / C278 +
             ((-C318) / std::pow(C278, 2) - (-C316 * C284 * 2 * C278) / std::pow(C278, 4)) * C316) -
            C318 * C316 / std::pow(C278, 2);
        const double C413 = C412;
        const double C414 = -C412;
        const double C415 = (-C320 * -0.22167 * C320 * 2 * C287) / std::pow(C287, 4);
        const double C416 = (-C320 * -3. * C320 * 2 * C287) / std::pow(C287, 4);
        const double C417 = C280 * (C280 - 1) * std::pow(C293, C280 - 2) * std::pow(C325, 2) +
                            C280 * std::pow(C293, C280 - 1) * C416;
        const double C418 = C281 * (C281 - 1) * std::pow(C293, C281 - 2) * std::pow(C325, 2) +
                            C281 * std::pow(C293, C281 - 1) * C416;
        const double C419 = 0.7389e-5 * C418 + 0.;
        const double C420 = 0.0116 * C417;
        const double C421 = 0.023266 * C417;
        const double C422 = 0.472 * C418;
        const double C423 = 8.723 * C417;
        const double C424 = C417 / C294 - std::pow(C326, 2) / std::pow(C294, 2);
        const double C425 = 0.0311 * C424;
        const double C426 = ((C326 * C333 + C301 * C417) + C333 * C326) + C294 * C424;
        const double C427 = C421 + C419;
        const double C428 = C423 + C422;
        const double C429 = 0.002 * C426;
        const double C430 = C427;
        const double C431 = C428 + C415;
        const double C432 = C425 + C429;
        const double C433 = C431;
        const double C434 = C432;
        const double C435 = ((C430 / C310 + ((-C339) / std::pow(C310, 2) -
                                             (-C342 * C307 * 2 * C310) / std::pow(C310, 4)) *
                                                C342) -
                             C433 * C307 / std::pow(C310, 2)) -
                            C339 * C342 / std::pow(C310, 2);
        const double C436 = C435;
        const double C437 = C434 - C420;
        matrix.upper[0] =
            ((((((((((((((((((((((std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                      (C278 *
                                           ((6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                                                 (std::pow(C278, 7. / 3.) *
                                                      std::sqrt((std::pow(C290, C283) +
                                                                 std::pow(C291, C283)) /
                                                                2.) *
                                                      C288 * C313 *
                                                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                                (std::pow(C278, 7. / 6.) * C313)) *
                                                           0.00081290825 * std::sqrt(C288) * C313 *
                                                           7. * std::pow(C278, -5. / 6.) / 6. +
                                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                           std::pow(C278, 1. / 6.) *
                                                           std::exp(
                                                               -0.00081290825 * std::sqrt(C288) /
                                                               (std::pow(C278, 7. / 6.) * C313)) *
                                                           0.00081290825 * std::sqrt(C288) * C313 *
                                                           7. * std::pow(C278, 1. / 6.) /
                                                           (6. *
                                                            std::pow(std::pow(C278, 7. / 6.) * C313,
                                                                     2))) +
                                                  C288 * C313 *
                                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                                               (std::pow(C278, 7. / 6.) * C313)) *
                                                      0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                      std::pow(C278, 1. / 6.) *
                                                      std::sqrt((std::pow(C290, C283) +
                                                                 std::pow(C291, C283)) /
                                                                2.) *
                                                      7. * std::pow(C278, 4. / 3.) / 3.) -
                                             std::pow(C278, 7. / 3.) *
                                                 std::sqrt(
                                                     (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                                 C288 * C313 *
                                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                                          (std::pow(C278, 7. / 6.) * C313)) *
                                                 0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                 std::pow(C278, 1. / 6.) * 6. *
                                                 std::pow(C278, 7. / 6.) * C313 * 2 * C313 * 7. *
                                                 std::pow(C278, 1. / 6.) / 6.) /
                                                std::pow(
                                                    6. *
                                                        std::pow(std::pow(C278, 7. / 6.) * C313, 2),
                                                    2) -
                                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                      (std::pow(C278, 7. / 6.) * C313)) *
                                                 C313 * C288 *
                                                 std::sqrt(
                                                     (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                                 28. * std::pow(C278, 1. / 3.) / 3. +
                                             std::sqrt(
                                                 (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.) *
                                                 7. * std::pow(C278, 4. / 3.) * C288 * C313 *
                                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                                          (std::pow(C278, 7. / 6.) * C313)) *
                                                 0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                 std::pow(C278, 1. / 6.) /
                                                 (6. *
                                                  std::pow(std::pow(C278, 7. / 6.) * C313, 2))) /
                                                3.) +
                                       (std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                            C288 * C313 *
                                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                                     (std::pow(C278, 7. / 6.) * C313)) *
                                            0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                            std::pow(C278, 1. / 6.) /
                                            (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                                 (std::pow(C278, 7. / 6.) * C313)) *
                                            C313 * C288 *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                            7. * std::pow(C278, 4. / 3.) / 3.)) -
                                  C278 *
                                      (std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           C288 * C313 *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           C313 * C288 *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           7. * std::pow(C278, 4. / 3.) / 3.) *
                                      std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      2 *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      7. * std::pow(C278, 4. / 3.) / 3.) /
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              4) +
                                 (std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      C288 * C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                      std::pow(C278, 1. / 6.) /
                                      (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      7. * std::pow(C278, 4. / 3.) / 3.) /
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2)) *
                                    C316 +
                                ((2. - (std::pow(C290, C282) + std::pow(C291, C282))) /
                                     (2. * (std::pow(2., C280) - 1.)) +
                                 1) *
                                    C346) +
                               C333 * 0.01555 *
                                   ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                                   (2. * (std::pow(2., C280) - 1.))) +
                              C335 * 0.0007 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                                  (2. * (std::pow(2., C280) - 1.))) +
                             C326 * -0.0048 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                                 (2. * (std::pow(2., C280) - 1.))) +
                            ((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                               C314) *
                                  C282 * std::pow(C290, C282 - 1) /
                                  (2. * (std::pow(2., C280) - 1.)) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C290, C283 - 1) /
                                  (4. *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(
                                       std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.),
                                       2))) -
                             C278 *
                                 (4. *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      std::pow(std::pow(C278, 7. / 3.) *
                                                   std::sqrt((std::pow(C290, C283) +
                                                              std::pow(C291, C283)) /
                                                             2.),
                                               2) *
                                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           C313 * C288 * C283 * std::pow(C290, C283 - 1) * 7. *
                                           std::pow(C278, 4. / 3.) / 3. +
                                       std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                                           C288 * C313 *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                      std::pow(C290, C283 - 1) * 4. *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      2 *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      7. * std::pow(C278, 4. / 3.) / 3.) /
                                 std::pow(
                                     4. *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.) *
                                         std::pow(std::pow(C278, 7. / 3.) *
                                                      std::sqrt((std::pow(C290, C283) +
                                                                 std::pow(C291, C283)) /
                                                                2.),
                                                  2),
                                     2)) *
                                C322) +
                           ((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                              C314) *
                                 C282 * std::pow(C291, C282 - 1) /
                                 (2. * (std::pow(2., C280) - 1.)) -
                             std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                                 C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                 std::pow(C291, C283 - 1) /
                                 (4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2))) -
                            C278 *
                                (4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2) *
                                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                          C313 * C288 * C283 * std::pow(C291, C283 - 1) * 7. *
                                          std::pow(C278, 4. / 3.) / 3. +
                                      std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                                          C288 * C313 *
                                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                          std::pow(C278, 1. / 6.) /
                                          (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                     C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                     std::pow(C291, C283 - 1) * 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     2 *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     7. * std::pow(C278, 4. / 3.) / 3.) /
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.) *
                                        std::pow(std::pow(C278, 7. / 3.) *
                                                     std::sqrt((std::pow(C290, C283) +
                                                                std::pow(C291, C283)) /
                                                               2.),
                                                 2),
                                    2)) *
                               C323) +
                          (std::pow(C278, 7. / 3.) *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               C345 *
                               (C278 *
                                    (std::exp(-0.00081290825 * std::sqrt(C288) /
                                              (std::pow(C278, 7. / 6.) * C313)) *
                                         0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                         std::pow(C278, 1. / 6.) /
                                         (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                     (std::pow(std::pow(C278, 7. / 6.) * C313, 2) * C313 *
                                          (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                               0.00081290825 * std::sqrt(C288) * 7. *
                                               std::pow(C278, 1. / 6.) / 6. +
                                           0.00081290825 * std::sqrt(C288) *
                                               std::pow(C278, 7. / 6.) *
                                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                                        (std::pow(C278, 7. / 6.) * C313)) *
                                               0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                               std::pow(C278, 1. / 6.) /
                                               (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                      C313 *
                                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) *
                                          std::pow(C278, 7. / 6.) * std::pow(C278, 7. / 6.) * C313 *
                                          2 * C313 * 7. * std::pow(C278, 1. / 6.) / 6.) /
                                         std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                                    C288 +
                                (std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) +
                                 C313 *
                                     std::exp(-0.00081290825 * std::sqrt(C288) /
                                              (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                                    C288) -
                           C345 * C278 *
                               (std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) +
                                C313 *
                                    std::exp(-0.00081290825 * std::sqrt(C288) /
                                             (std::pow(C278, 7. / 6.) * C313)) *
                                    0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                    std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                               C288 *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 7. *
                               std::pow(C278, 4. / 3.) / 3.) /
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2)) *
                             C316 +
                         (((((((((std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                      C278 *
                                      (C288 * C313 *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) * std::pow(C278, 7. / 3.) *
                                           C283 * std::pow(C290, C283 - 1) /
                                           (4. *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                            6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           C313 * C288 * 7. * std::pow(C278, 4. / 3.) * C283 *
                                           std::pow(C290, C283 - 1) /
                                           (12. * std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.))) -
                                  C278 *
                                      (std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           C288 * C313 *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           C313 * C288 *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           7. * std::pow(C278, 4. / 3.) / 3.) *
                                      std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      2 * std::pow(C278, 7. / 3.) * C283 *
                                      std::pow(C290, C283 - 1) /
                                      (4. *
                                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.))) /
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              4) +
                                 (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                                   C314) *
                                      C282 * std::pow(C290, C282 - 1) /
                                      (2. * (std::pow(2., C280) - 1.)) -
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                      std::pow(C290, C283 - 1) /
                                      (4. *
                                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.) *
                                       std::pow(std::pow(C278, 7. / 3.) *
                                                    std::sqrt((std::pow(C290, C283) +
                                                               std::pow(C291, C283)) /
                                                              2.),
                                                2)))) *
                                    C316 -
                                C346 * C278 * C282 * std::pow(C290, C282 - 1) /
                                    (2. * (std::pow(2., C280) - 1.))) +
                               C333 * C278 * 0.01555 * C282 * std::pow(C290, C282 - 1) /
                                   (2. * (std::pow(2., C280) - 1.))) +
                              C335 * C278 * 0.0007 * C282 * std::pow(C290, C282 - 1) /
                                  (2. * (std::pow(2., C280) - 1.))) +
                             C326 * C278 * -0.0048 * C282 * std::pow(C290, C282 - 1) /
                                 (2. * (std::pow(2., C280) - 1.))) +
                            C278 *
                                (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                                  C314) *
                                     C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                                     (2. * (std::pow(2., C280) - 1.)) -
                                 (4. *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      std::pow(std::pow(C278, 7. / 3.) *
                                                   std::sqrt((std::pow(C290, C283) +
                                                              std::pow(C291, C283)) /
                                                             2.),
                                               2) *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                                      std::pow(C290, C283 - 2) -
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                      std::pow(C290, C283 - 1) * 4. *
                                      (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.) *
                                           std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           2 * std::pow(C278, 7. / 3.) * C283 *
                                           std::pow(C290, C283 - 1) +
                                       std::pow(std::pow(C278, 7. / 3.) *
                                                    std::sqrt((std::pow(C290, C283) +
                                                               std::pow(C291, C283)) /
                                                              2.),
                                                2) *
                                           C283 * std::pow(C290, C283 - 1)) /
                                      (4. *
                                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.))) /
                                     std::pow(4. *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.) *
                                                  std::pow(std::pow(C278, 7. / 3.) *
                                                               std::sqrt((std::pow(C290, C283) +
                                                                          std::pow(C291, C283)) /
                                                                         2.),
                                                           2),
                                              2)) *
                                C322) +
                           C323 * C278 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) *
                               C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                               std::pow(C291, C283 - 1) * 4. *
                               (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                                std::pow(
                                    std::pow(C278, 7. / 3.) *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.),
                                    2) *
                                    C283 * std::pow(C290, C283 - 1)) /
                               (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.) *
                                        std::pow(std::pow(C278, 7. / 3.) *
                                                     std::sqrt((std::pow(C290, C283) +
                                                                std::pow(C291, C283)) /
                                                               2.),
                                                 2),
                                    2))) -
                          C345 * C278 *
                              (std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) +
                               C313 *
                                   std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                   0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                   std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                              C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) *
                             C322) +
                        (((((((((std::pow(
                                     std::pow(C278, 7. / 3.) *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.),
                                     2) *
                                     C278 *
                                     (C288 * C313 *
                                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                          std::pow(C278, 1. / 6.) * std::pow(C278, 7. / 3.) * C283 *
                                          std::pow(C291, C283 - 1) /
                                          (4. *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                          C313 * C288 * 7. * std::pow(C278, 4. / 3.) * C283 *
                                          std::pow(C291, C283 - 1) /
                                          (12. *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.))) -
                                 C278 *
                                     (std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.) *
                                          C288 * C313 *
                                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                          std::pow(C278, 1. / 6.) /
                                          (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                          C313 * C288 *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.) *
                                          7. * std::pow(C278, 4. / 3.) / 3.) *
                                     std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.))) /
                                    std::pow(
                                        std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        4) +
                                (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                                  C314) *
                                     C282 * std::pow(C291, C282 - 1) /
                                     (2. * (std::pow(2., C280) - 1.)) -
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                     C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                     std::pow(C291, C283 - 1) /
                                     (4. *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      std::pow(std::pow(C278, 7. / 3.) *
                                                   std::sqrt((std::pow(C290, C283) +
                                                              std::pow(C291, C283)) /
                                                             2.),
                                               2)))) *
                                   C316 -
                               C346 * C278 * C282 * std::pow(C291, C282 - 1) /
                                   (2. * (std::pow(2., C280) - 1.))) +
                              C333 * C278 * 0.01555 * C282 * std::pow(C291, C282 - 1) /
                                  (2. * (std::pow(2., C280) - 1.))) +
                             C335 * C278 * 0.0007 * C282 * std::pow(C291, C282 - 1) /
                                 (2. * (std::pow(2., C280) - 1.))) +
                            C326 * C278 * -0.0048 * C282 * std::pow(C291, C282 - 1) /
                                (2. * (std::pow(2., C280) - 1.))) +
                           C322 * C278 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) *
                               C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                               std::pow(C290, C283 - 1) * 4. *
                               (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                                std::pow(
                                    std::pow(C278, 7. / 3.) *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.),
                                    2) *
                                    C283 * std::pow(C291, C283 - 1)) /
                               (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.) *
                                        std::pow(std::pow(C278, 7. / 3.) *
                                                     std::sqrt((std::pow(C290, C283) +
                                                                std::pow(C291, C283)) /
                                                               2.),
                                                 2),
                                    2))) +
                          C278 *
                              (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                                C314) *
                                   C282 * (C282 - 1) * std::pow(C291, C282 - 2) /
                                   (2. * (std::pow(2., C280) - 1.)) -
                               (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    std::pow(
                                        std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2) *
                                    std::exp(-0.00081290825 * std::sqrt(C288) /
                                             (std::pow(C278, 7. / 6.) * C313)) *
                                    C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                                    std::pow(C291, C283 - 2) -
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                    C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                    std::pow(C291, C283 - 1) * 4. *
                                    (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                         std::pow(C278, 7. / 3.) *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.) *
                                         2 * std::pow(C278, 7. / 3.) * C283 *
                                         std::pow(C291, C283 - 1) +
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2) *
                                         C283 * std::pow(C291, C283 - 1)) /
                                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.))) /
                                   std::pow(
                                       4. *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           std::pow(std::pow(C278, 7. / 3.) *
                                                        std::sqrt((std::pow(C290, C283) +
                                                                   std::pow(C291, C283)) /
                                                                  2.),
                                                    2),
                                       2)) *
                              C323) -
                         C345 * C278 *
                             (std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) +
                              C313 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                  0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                  std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                             C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2))) *
                            C323) +
                       ((((C278 *
                               ((6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                                     std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     C288 *
                                     (C313 * (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                       (std::pow(C278, 7. / 6.) * C313)) *
                                                  0.00081290825 * std::sqrt(C288) * 7. *
                                                  std::pow(C278, 1. / 6.) +
                                              0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                  std::pow(C278, 1. / 6.) *
                                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                                           (std::pow(C278, 7. / 6.) * C313)) *
                                                  0.00081290825 * std::sqrt(C288) *
                                                  std::pow(C278, 7. / 6.) /
                                                  std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                          std::pow(C278, 1. / 6.)) -
                                 std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     C288 * C313 *
                                     std::exp(-0.00081290825 * std::sqrt(C288) /
                                              (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                     std::pow(C278, 1. / 6.) * 12. * C313 *
                                     std::pow(C278, 14. / 6.)) /
                                    std::pow(6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2), 2) -
                                (std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) +
                                 C313 *
                                     std::exp(-0.00081290825 * std::sqrt(C288) /
                                              (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                                    C288 *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    7. * std::pow(C278, 4. / 3.) / 3.) /
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2) +
                           (std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) +
                            C313 *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                               C288 /
                               (std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                              C316 -
                          C322 * C278 *
                              (std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) +
                               C313 *
                                   std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                   0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                   std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                              C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C323 * C278 *
                             (std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) +
                              C313 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                  0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                  std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                             C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2))) +
                        C345 * C278 *
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                             (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                                  (C313 * 0.00081290825 * std::sqrt(C288) *
                                       std::pow(C278, 7. / 6.) *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                       std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                                   std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                              C313 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                  0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 *
                                  C313 * std::pow(C278, 14. / 6.)) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                            C288 /
                            (std::pow(C278, 7. / 3.) *
                             std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                           C345) +
                      ((((2. - (std::pow(C290, C282) + std::pow(C291, C282))) /
                             (2. * (std::pow(2., C280) - 1.)) +
                         1) *
                            C316 -
                        C322 * C278 * C282 * std::pow(C290, C282 - 1) /
                            (2. * (std::pow(2., C280) - 1.))) -
                       C323 * C278 * C282 * std::pow(C291, C282 - 1) /
                           (2. * (std::pow(2., C280) - 1.))) *
                          C346) +
                     C333 *
                         ((C316 * 0.01555 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) +
                           C322 * C278 * 0.01555 * C282 * std::pow(C290, C282 - 1)) +
                          C323 * C278 * 0.01555 * C282 * std::pow(C291, C282 - 1)) /
                         (2. * (std::pow(2., C280) - 1.))) +
                    C335 *
                        ((C316 * 0.0007 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) +
                          C322 * C278 * 0.0007 * C282 * std::pow(C290, C282 - 1)) +
                         C323 * C278 * 0.0007 * C282 * std::pow(C291, C282 - 1)) /
                        (2. * (std::pow(2., C280) - 1.))) +
                   C326 *
                       ((C316 * -0.0048 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) +
                         C322 * C278 * -0.0048 * C282 * std::pow(C290, C282 - 1)) +
                        C323 * C278 * -0.0048 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.))) +
                  C278 *
                      ((2. - (std::pow(C290, C282) + std::pow(C291, C282))) /
                           (2. * (std::pow(2., C280) - 1.)) +
                       1) *
                      C437) +
                 C424 * C278 * 0.01555 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                     (2. * (std::pow(2., C280) - 1.))) +
                C426 * C278 * 0.0007 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                    (2. * (std::pow(2., C280) - 1.))) +
               C417 * C278 * -0.0048 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                   (2. * (std::pow(2., C280) - 1.))) +
              C278 *
                  (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                       std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) *
                  C413) +
             C278 *
                 (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                      std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                 C414) +
            C436 * C278 *
                (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                 C313 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                C288 /
                (std::pow(C278, 7. / 3.) *
                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.));

        const double C347 = 1;
        const double C348 = -1;
        const double C349 = 0.9999999999999999 * C348 + 0.;
        const double C350 = Pi * C347;
        const double C351 = 4. * C350;
        const double C352 = C349 / C278 - C347 * C284 / std::pow(C278, 2);
        const double C353 = C352;
        const double C354 = -C352;
        const double C355 = -0.22167 * C351 / std::pow(C287, 2);
        const double C356 = -3. * C351 / std::pow(C287, 2);
        const double C357 = C280 * std::pow(C293, C280 - 1) * C356;
        const double C358 = C281 * std::pow(C293, C281 - 1) * C356;
        const double C359 = 0.7389e-5 * C358 + 0.;
        const double C360 = 0.0116 * C357;
        const double C361 = 0.023266 * C357;
        const double C362 = 0.472 * C358;
        const double C363 = 8.723 * C357;
        const double C364 = C357 / C294;
        const double C365 = 0.0311 * C364;
        const double C366 = C301 * C357 + C294 * C364;
        const double C367 = C361 + C359;
        const double C368 = C363 + C362;
        const double C369 = 0.002 * C366;
        const double C370 = C367;
        const double C371 = C368 + C355;
        const double C372 = C365 + C369;
        const double C373 = C371;
        const double C374 = C372;
        const double C375 = C370 / C310 - C373 * C307 / std::pow(C310, 2);
        const double C376 = C375;
        const double C377 = C374 - C360;
        const double C438 = 0.;
        const double C439 =
            (C438 / C278 +
             ((-C318) / std::pow(C278, 2) - (-C316 * C284 * 2 * C278) / std::pow(C278, 4)) * C347) -
            C349 * C316 / std::pow(C278, 2);
        const double C440 = C439;
        const double C441 = -C439;
        const double C442 = (-C351 * -0.22167 * C320 * 2 * C287) / std::pow(C287, 4);
        const double C443 = (-C351 * -3. * C320 * 2 * C287) / std::pow(C287, 4);
        const double C444 = C280 * (C280 - 1) * std::pow(C293, C280 - 2) * C325 * C356 +
                            C280 * std::pow(C293, C280 - 1) * C443;
        const double C445 = C281 * (C281 - 1) * std::pow(C293, C281 - 2) * C325 * C356 +
                            C281 * std::pow(C293, C281 - 1) * C443;
        const double C446 = 0.7389e-5 * C445 + 0.;
        const double C447 = 0.0116 * C444;
        const double C448 = 0.023266 * C444;
        const double C449 = 0.472 * C445;
        const double C450 = 8.723 * C444;
        const double C451 = C444 / C294 - C357 * C326 / std::pow(C294, 2);
        const double C452 = 0.0311 * C451;
        const double C453 = ((C326 * C364 + C301 * C444) + C333 * C357) + C294 * C451;
        const double C454 = C448 + C446;
        const double C455 = C450 + C449;
        const double C456 = 0.002 * C453;
        const double C457 = C454;
        const double C458 = C455 + C442;
        const double C459 = C452 + C456;
        const double C460 = C458;
        const double C461 = C459;
        const double C462 = ((C457 / C310 + ((-C339) / std::pow(C310, 2) -
                                             (-C342 * C307 * 2 * C310) / std::pow(C310, 4)) *
                                                C373) -
                             C460 * C307 / std::pow(C310, 2)) -
                            C370 * C342 / std::pow(C310, 2);
        const double C463 = C462;
        const double C464 = C461 - C447;
        matrix.upper[1] =
            ((((((((((((((((((((((std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                      (C278 *
                                           ((6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                                                 (std::pow(C278, 7. / 3.) *
                                                      std::sqrt((std::pow(C290, C283) +
                                                                 std::pow(C291, C283)) /
                                                                2.) *
                                                      C288 * C313 *
                                                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                                (std::pow(C278, 7. / 6.) * C313)) *
                                                           0.00081290825 * std::sqrt(C288) * C313 *
                                                           7. * std::pow(C278, -5. / 6.) / 6. +
                                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                           std::pow(C278, 1. / 6.) *
                                                           std::exp(
                                                               -0.00081290825 * std::sqrt(C288) /
                                                               (std::pow(C278, 7. / 6.) * C313)) *
                                                           0.00081290825 * std::sqrt(C288) * C313 *
                                                           7. * std::pow(C278, 1. / 6.) /
                                                           (6. *
                                                            std::pow(std::pow(C278, 7. / 6.) * C313,
                                                                     2))) +
                                                  C288 * C313 *
                                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                                               (std::pow(C278, 7. / 6.) * C313)) *
                                                      0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                      std::pow(C278, 1. / 6.) *
                                                      std::sqrt((std::pow(C290, C283) +
                                                                 std::pow(C291, C283)) /
                                                                2.) *
                                                      7. * std::pow(C278, 4. / 3.) / 3.) -
                                             std::pow(C278, 7. / 3.) *
                                                 std::sqrt(
                                                     (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                                 C288 * C313 *
                                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                                          (std::pow(C278, 7. / 6.) * C313)) *
                                                 0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                 std::pow(C278, 1. / 6.) * 6. *
                                                 std::pow(C278, 7. / 6.) * C313 * 2 * C313 * 7. *
                                                 std::pow(C278, 1. / 6.) / 6.) /
                                                std::pow(
                                                    6. *
                                                        std::pow(std::pow(C278, 7. / 6.) * C313, 2),
                                                    2) -
                                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                      (std::pow(C278, 7. / 6.) * C313)) *
                                                 C313 * C288 *
                                                 std::sqrt(
                                                     (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                                 28. * std::pow(C278, 1. / 3.) / 3. +
                                             std::sqrt(
                                                 (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.) *
                                                 7. * std::pow(C278, 4. / 3.) * C288 * C313 *
                                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                                          (std::pow(C278, 7. / 6.) * C313)) *
                                                 0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                 std::pow(C278, 1. / 6.) /
                                                 (6. *
                                                  std::pow(std::pow(C278, 7. / 6.) * C313, 2))) /
                                                3.) +
                                       (std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                            C288 * C313 *
                                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                                     (std::pow(C278, 7. / 6.) * C313)) *
                                            0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                            std::pow(C278, 1. / 6.) /
                                            (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                                 (std::pow(C278, 7. / 6.) * C313)) *
                                            C313 * C288 *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                            7. * std::pow(C278, 4. / 3.) / 3.)) -
                                  C278 *
                                      (std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           C288 * C313 *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           C313 * C288 *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           7. * std::pow(C278, 4. / 3.) / 3.) *
                                      std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      2 *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      7. * std::pow(C278, 4. / 3.) / 3.) /
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              4) +
                                 (std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      C288 * C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                      std::pow(C278, 1. / 6.) /
                                      (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      7. * std::pow(C278, 4. / 3.) / 3.) /
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2)) *
                                    C316 +
                                ((2. - (std::pow(C290, C282) + std::pow(C291, C282))) /
                                     (2. * (std::pow(2., C280) - 1.)) +
                                 1) *
                                    C346) +
                               C333 * 0.01555 *
                                   ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                                   (2. * (std::pow(2., C280) - 1.))) +
                              C335 * 0.0007 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                                  (2. * (std::pow(2., C280) - 1.))) +
                             C326 * -0.0048 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                                 (2. * (std::pow(2., C280) - 1.))) +
                            ((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                               C314) *
                                  C282 * std::pow(C290, C282 - 1) /
                                  (2. * (std::pow(2., C280) - 1.)) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C290, C283 - 1) /
                                  (4. *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(
                                       std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.),
                                       2))) -
                             C278 *
                                 (4. *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      std::pow(std::pow(C278, 7. / 3.) *
                                                   std::sqrt((std::pow(C290, C283) +
                                                              std::pow(C291, C283)) /
                                                             2.),
                                               2) *
                                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           C313 * C288 * C283 * std::pow(C290, C283 - 1) * 7. *
                                           std::pow(C278, 4. / 3.) / 3. +
                                       std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                                           C288 * C313 *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                      std::pow(C290, C283 - 1) * 4. *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      2 *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      7. * std::pow(C278, 4. / 3.) / 3.) /
                                 std::pow(
                                     4. *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.) *
                                         std::pow(std::pow(C278, 7. / 3.) *
                                                      std::sqrt((std::pow(C290, C283) +
                                                                 std::pow(C291, C283)) /
                                                                2.),
                                                  2),
                                     2)) *
                                C322) +
                           ((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                              C314) *
                                 C282 * std::pow(C291, C282 - 1) /
                                 (2. * (std::pow(2., C280) - 1.)) -
                             std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                                 C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                 std::pow(C291, C283 - 1) /
                                 (4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2))) -
                            C278 *
                                (4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2) *
                                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                          C313 * C288 * C283 * std::pow(C291, C283 - 1) * 7. *
                                          std::pow(C278, 4. / 3.) / 3. +
                                      std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                                          C288 * C313 *
                                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                          std::pow(C278, 1. / 6.) /
                                          (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                     C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                     std::pow(C291, C283 - 1) * 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     2 *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     7. * std::pow(C278, 4. / 3.) / 3.) /
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.) *
                                        std::pow(std::pow(C278, 7. / 3.) *
                                                     std::sqrt((std::pow(C290, C283) +
                                                                std::pow(C291, C283)) /
                                                               2.),
                                                 2),
                                    2)) *
                               C323) +
                          (std::pow(C278, 7. / 3.) *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               C345 *
                               (C278 *
                                    (std::exp(-0.00081290825 * std::sqrt(C288) /
                                              (std::pow(C278, 7. / 6.) * C313)) *
                                         0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                         std::pow(C278, 1. / 6.) /
                                         (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                     (std::pow(std::pow(C278, 7. / 6.) * C313, 2) * C313 *
                                          (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                               0.00081290825 * std::sqrt(C288) * 7. *
                                               std::pow(C278, 1. / 6.) / 6. +
                                           0.00081290825 * std::sqrt(C288) *
                                               std::pow(C278, 7. / 6.) *
                                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                                        (std::pow(C278, 7. / 6.) * C313)) *
                                               0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                               std::pow(C278, 1. / 6.) /
                                               (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                      C313 *
                                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) *
                                          std::pow(C278, 7. / 6.) * std::pow(C278, 7. / 6.) * C313 *
                                          2 * C313 * 7. * std::pow(C278, 1. / 6.) / 6.) /
                                         std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                                    C288 +
                                (std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) +
                                 C313 *
                                     std::exp(-0.00081290825 * std::sqrt(C288) /
                                              (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                                    C288) -
                           C345 * C278 *
                               (std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) +
                                C313 *
                                    std::exp(-0.00081290825 * std::sqrt(C288) /
                                             (std::pow(C278, 7. / 6.) * C313)) *
                                    0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                    std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                               C288 *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 7. *
                               std::pow(C278, 4. / 3.) / 3.) /
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2)) *
                             C347 +
                         (((((((((std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                      C278 *
                                      (C288 * C313 *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) * std::pow(C278, 7. / 3.) *
                                           C283 * std::pow(C290, C283 - 1) /
                                           (4. *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                            6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           C313 * C288 * 7. * std::pow(C278, 4. / 3.) * C283 *
                                           std::pow(C290, C283 - 1) /
                                           (12. * std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.))) -
                                  C278 *
                                      (std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           C288 * C313 *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           C313 * C288 *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           7. * std::pow(C278, 4. / 3.) / 3.) *
                                      std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      2 * std::pow(C278, 7. / 3.) * C283 *
                                      std::pow(C290, C283 - 1) /
                                      (4. *
                                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.))) /
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              4) +
                                 (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                                   C314) *
                                      C282 * std::pow(C290, C282 - 1) /
                                      (2. * (std::pow(2., C280) - 1.)) -
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                      std::pow(C290, C283 - 1) /
                                      (4. *
                                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.) *
                                       std::pow(std::pow(C278, 7. / 3.) *
                                                    std::sqrt((std::pow(C290, C283) +
                                                               std::pow(C291, C283)) /
                                                              2.),
                                                2)))) *
                                    C316 -
                                C346 * C278 * C282 * std::pow(C290, C282 - 1) /
                                    (2. * (std::pow(2., C280) - 1.))) +
                               C333 * C278 * 0.01555 * C282 * std::pow(C290, C282 - 1) /
                                   (2. * (std::pow(2., C280) - 1.))) +
                              C335 * C278 * 0.0007 * C282 * std::pow(C290, C282 - 1) /
                                  (2. * (std::pow(2., C280) - 1.))) +
                             C326 * C278 * -0.0048 * C282 * std::pow(C290, C282 - 1) /
                                 (2. * (std::pow(2., C280) - 1.))) +
                            C278 *
                                (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                                  C314) *
                                     C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                                     (2. * (std::pow(2., C280) - 1.)) -
                                 (4. *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      std::pow(std::pow(C278, 7. / 3.) *
                                                   std::sqrt((std::pow(C290, C283) +
                                                              std::pow(C291, C283)) /
                                                             2.),
                                               2) *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                                      std::pow(C290, C283 - 2) -
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                      std::pow(C290, C283 - 1) * 4. *
                                      (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.) *
                                           std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           2 * std::pow(C278, 7. / 3.) * C283 *
                                           std::pow(C290, C283 - 1) +
                                       std::pow(std::pow(C278, 7. / 3.) *
                                                    std::sqrt((std::pow(C290, C283) +
                                                               std::pow(C291, C283)) /
                                                              2.),
                                                2) *
                                           C283 * std::pow(C290, C283 - 1)) /
                                      (4. *
                                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.))) /
                                     std::pow(4. *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.) *
                                                  std::pow(std::pow(C278, 7. / 3.) *
                                                               std::sqrt((std::pow(C290, C283) +
                                                                          std::pow(C291, C283)) /
                                                                         2.),
                                                           2),
                                              2)) *
                                C322) +
                           C323 * C278 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) *
                               C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                               std::pow(C291, C283 - 1) * 4. *
                               (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                                std::pow(
                                    std::pow(C278, 7. / 3.) *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.),
                                    2) *
                                    C283 * std::pow(C290, C283 - 1)) /
                               (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.) *
                                        std::pow(std::pow(C278, 7. / 3.) *
                                                     std::sqrt((std::pow(C290, C283) +
                                                                std::pow(C291, C283)) /
                                                               2.),
                                                 2),
                                    2))) -
                          C345 * C278 *
                              (std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) +
                               C313 *
                                   std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                   0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                   std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                              C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) *
                             C353) +
                        (((((((((std::pow(
                                     std::pow(C278, 7. / 3.) *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.),
                                     2) *
                                     C278 *
                                     (C288 * C313 *
                                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                          std::pow(C278, 1. / 6.) * std::pow(C278, 7. / 3.) * C283 *
                                          std::pow(C291, C283 - 1) /
                                          (4. *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                          C313 * C288 * 7. * std::pow(C278, 4. / 3.) * C283 *
                                          std::pow(C291, C283 - 1) /
                                          (12. *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.))) -
                                 C278 *
                                     (std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.) *
                                          C288 * C313 *
                                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                          std::pow(C278, 1. / 6.) /
                                          (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                          C313 * C288 *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.) *
                                          7. * std::pow(C278, 4. / 3.) / 3.) *
                                     std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.))) /
                                    std::pow(
                                        std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        4) +
                                (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                                  C314) *
                                     C282 * std::pow(C291, C282 - 1) /
                                     (2. * (std::pow(2., C280) - 1.)) -
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                     C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                     std::pow(C291, C283 - 1) /
                                     (4. *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      std::pow(std::pow(C278, 7. / 3.) *
                                                   std::sqrt((std::pow(C290, C283) +
                                                              std::pow(C291, C283)) /
                                                             2.),
                                               2)))) *
                                   C316 -
                               C346 * C278 * C282 * std::pow(C291, C282 - 1) /
                                   (2. * (std::pow(2., C280) - 1.))) +
                              C333 * C278 * 0.01555 * C282 * std::pow(C291, C282 - 1) /
                                  (2. * (std::pow(2., C280) - 1.))) +
                             C335 * C278 * 0.0007 * C282 * std::pow(C291, C282 - 1) /
                                 (2. * (std::pow(2., C280) - 1.))) +
                            C326 * C278 * -0.0048 * C282 * std::pow(C291, C282 - 1) /
                                (2. * (std::pow(2., C280) - 1.))) +
                           C322 * C278 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) *
                               C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                               std::pow(C290, C283 - 1) * 4. *
                               (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                                std::pow(
                                    std::pow(C278, 7. / 3.) *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.),
                                    2) *
                                    C283 * std::pow(C291, C283 - 1)) /
                               (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.) *
                                        std::pow(std::pow(C278, 7. / 3.) *
                                                     std::sqrt((std::pow(C290, C283) +
                                                                std::pow(C291, C283)) /
                                                               2.),
                                                 2),
                                    2))) +
                          C278 *
                              (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                                C314) *
                                   C282 * (C282 - 1) * std::pow(C291, C282 - 2) /
                                   (2. * (std::pow(2., C280) - 1.)) -
                               (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    std::pow(
                                        std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2) *
                                    std::exp(-0.00081290825 * std::sqrt(C288) /
                                             (std::pow(C278, 7. / 6.) * C313)) *
                                    C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                                    std::pow(C291, C283 - 2) -
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                    C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                    std::pow(C291, C283 - 1) * 4. *
                                    (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                         std::pow(C278, 7. / 3.) *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.) *
                                         2 * std::pow(C278, 7. / 3.) * C283 *
                                         std::pow(C291, C283 - 1) +
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2) *
                                         C283 * std::pow(C291, C283 - 1)) /
                                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.))) /
                                   std::pow(
                                       4. *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           std::pow(std::pow(C278, 7. / 3.) *
                                                        std::sqrt((std::pow(C290, C283) +
                                                                   std::pow(C291, C283)) /
                                                                  2.),
                                                    2),
                                       2)) *
                              C323) -
                         C345 * C278 *
                             (std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) +
                              C313 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                  0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                  std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                             C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2))) *
                            C354) +
                       ((((C278 *
                               ((6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                                     std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     C288 *
                                     (C313 * (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                       (std::pow(C278, 7. / 6.) * C313)) *
                                                  0.00081290825 * std::sqrt(C288) * 7. *
                                                  std::pow(C278, 1. / 6.) +
                                              0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                  std::pow(C278, 1. / 6.) *
                                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                                           (std::pow(C278, 7. / 6.) * C313)) *
                                                  0.00081290825 * std::sqrt(C288) *
                                                  std::pow(C278, 7. / 6.) /
                                                  std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                          std::pow(C278, 1. / 6.)) -
                                 std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     C288 * C313 *
                                     std::exp(-0.00081290825 * std::sqrt(C288) /
                                              (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                     std::pow(C278, 1. / 6.) * 12. * C313 *
                                     std::pow(C278, 14. / 6.)) /
                                    std::pow(6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2), 2) -
                                (std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) +
                                 C313 *
                                     std::exp(-0.00081290825 * std::sqrt(C288) /
                                              (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                                    C288 *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    7. * std::pow(C278, 4. / 3.) / 3.) /
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2) +
                           (std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) +
                            C313 *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                               C288 /
                               (std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                              C316 -
                          C322 * C278 *
                              (std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) +
                               C313 *
                                   std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                   0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                   std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                              C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C323 * C278 *
                             (std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) +
                              C313 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                  0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                  std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                             C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2))) +
                        C345 * C278 *
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                             (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                                  (C313 * 0.00081290825 * std::sqrt(C288) *
                                       std::pow(C278, 7. / 6.) *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                       std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                                   std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                              C313 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                  0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 *
                                  C313 * std::pow(C278, 14. / 6.)) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                            C288 /
                            (std::pow(C278, 7. / 3.) *
                             std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                           C376) +
                      ((((2. - (std::pow(C290, C282) + std::pow(C291, C282))) /
                             (2. * (std::pow(2., C280) - 1.)) +
                         1) *
                            C316 -
                        C322 * C278 * C282 * std::pow(C290, C282 - 1) /
                            (2. * (std::pow(2., C280) - 1.))) -
                       C323 * C278 * C282 * std::pow(C291, C282 - 1) /
                           (2. * (std::pow(2., C280) - 1.))) *
                          C377) +
                     C364 *
                         ((C316 * 0.01555 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) +
                           C322 * C278 * 0.01555 * C282 * std::pow(C290, C282 - 1)) +
                          C323 * C278 * 0.01555 * C282 * std::pow(C291, C282 - 1)) /
                         (2. * (std::pow(2., C280) - 1.))) +
                    C366 *
                        ((C316 * 0.0007 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) +
                          C322 * C278 * 0.0007 * C282 * std::pow(C290, C282 - 1)) +
                         C323 * C278 * 0.0007 * C282 * std::pow(C291, C282 - 1)) /
                        (2. * (std::pow(2., C280) - 1.))) +
                   C357 *
                       ((C316 * -0.0048 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) +
                         C322 * C278 * -0.0048 * C282 * std::pow(C290, C282 - 1)) +
                        C323 * C278 * -0.0048 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.))) +
                  C278 *
                      ((2. - (std::pow(C290, C282) + std::pow(C291, C282))) /
                           (2. * (std::pow(2., C280) - 1.)) +
                       1) *
                      C464) +
                 C451 * C278 * 0.01555 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                     (2. * (std::pow(2., C280) - 1.))) +
                C453 * C278 * 0.0007 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                    (2. * (std::pow(2., C280) - 1.))) +
               C444 * C278 * -0.0048 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                   (2. * (std::pow(2., C280) - 1.))) +
              C278 *
                  (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                       std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) *
                  C440) +
             C278 *
                 (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                      std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                 C441) +
            C463 * C278 *
                (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                 C313 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                C288 /
                (std::pow(C278, 7. / 3.) *
                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.));

        const double C378 = 0.;
        const double C379 = 1;
        const double C380 = C379;
        const double C381 = C378 / C278;
        const double C382 = C381;
        const double C383 = -C381;
        const double C384 = 0.;
        const double C385 = C384;
        const double C386 = C385;
        const double C387 = C386 / C310;
        const double C388 = C387;
        const double C465 = (-C316 * C378) / std::pow(C278, 2);
        const double C466 = C465;
        const double C467 = -C465;
        const double C468 = (-C342 * C386) / std::pow(C310, 2);
        const double C469 = C468;
        matrix.upper[2] =
            ((((((((((((((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                              C282 * std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C290, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C278 *
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       C313 * C288 * C283 * std::pow(C290, C283 - 1) * 7. *
                                       std::pow(C278, 4. / 3.) / 3. +
                                   std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                                       C288 * C313 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C290, C283 - 1) * 4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  2 *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  7. * std::pow(C278, 4. / 3.) / 3.) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                            C382 +
                        ((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                              C282 * std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C291, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C278 *
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       C313 * C288 * C283 * std::pow(C291, C283 - 1) * 7. *
                                       std::pow(C278, 4. / 3.) / 3. +
                                   std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                                       C288 * C313 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C291, C283 - 1) * 4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  2 *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  7. * std::pow(C278, 4. / 3.) / 3.) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                            C383) +
                       (std::pow(C278, 7. / 3.) *
                            std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * C380 *
                            (C278 *
                                 (C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                      std::pow(C278, 1. / 6.) /
                                      (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                  (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                                       C313 * -0.00081290825 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                   C288 * C313 * -0.00081290825 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       2 * std::sqrt(C288) * C313 * 7. * std::pow(C278, 1. / 6.) /
                                       6.) /
                                      std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313,
                                               2)) +
                             (std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 +
                              C288 * C313 * -0.00081290825 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) /
                                  (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313))) -
                        C380 * C278 *
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                                 C313 +
                             C288 * C313 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) /
                                 (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                            std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 7. *
                            std::pow(C278, 4. / 3.) / 3.) /
                           std::pow(
                               std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2)) +
                      (std::pow(C278, 7. / 3.) *
                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * C388 *
                           (C278 *
                                (std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                     std::pow(C278, 1. / 6.) /
                                     (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                 (std::pow(std::pow(C278, 7. / 6.) * C313, 2) * C313 *
                                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * 7. *
                                           std::pow(C278, 1. / 6.) / 6. +
                                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                  C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                      std::pow(C278, 7. / 6.) * C313 * 2 * C313 * 7. *
                                      std::pow(C278, 1. / 6.) / 6.) /
                                     std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                                C288 +
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) +
                             C313 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                                C288) -
                       C388 * C278 *
                           (std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) +
                            C313 *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                           C288 * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           7. * std::pow(C278, 4. / 3.) / 3.) /
                          std::pow(
                              std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2)) *
                         C316 +
                     C333 *
                         (C382 * C278 * 0.01555 * C282 * std::pow(C290, C282 - 1) +
                          C383 * C278 * 0.01555 * C282 * std::pow(C291, C282 - 1)) /
                         (2. * (std::pow(2., C280) - 1.))) +
                    C335 *
                        (C382 * C278 * 0.0007 * C282 * std::pow(C290, C282 - 1) +
                         C383 * C278 * 0.0007 * C282 * std::pow(C291, C282 - 1)) /
                        (2. * (std::pow(2., C280) - 1.))) +
                   C326 *
                       (C382 * C278 * -0.0048 * C282 * std::pow(C290, C282 - 1) +
                        C383 * C278 * -0.0048 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.))) +
                  ((-C383 * C278 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.)) -
                   C382 * C278 * C282 * std::pow(C290, C282 - 1) /
                       (2. * (std::pow(2., C280) - 1.))) *
                      C346) +
                 (((C278 *
                        (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                             C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                             (2. * (std::pow(2., C280) - 1.)) -
                         (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                              std::pow(C290, C283 - 2) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C290, C283 - 1) * 4. *
                              (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2) *
                                   C283 * std::pow(C290, C283 - 1)) /
                              (4. *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                        C382 +
                    C383 * C278 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                        4. *
                        (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                             std::pow(C278, 7. / 3.) *
                             std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                             std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                         std::pow(std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                             C283 * std::pow(C290, C283 - 1)) /
                        (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                         std::pow(
                             4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                 std::pow(
                                     std::pow(C278, 7. / 3.) *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.),
                                     2),
                             2))) -
                   C380 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                            C313 +
                        C288 * C313 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) /
                            (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                       std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) -
                  C388 * C278 *
                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) +
                       C313 *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                           std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                      C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                     C322) +
                ((C380 * C278 *
                      ((std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                       (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                            (C313 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2) -
                             0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C288) /
                                                      (std::pow(C278, 7. / 6.) * C313))) -
                        C288 * C313 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            2 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) /
                           std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                      (std::pow(C278, 7. / 3.) *
                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
                  (C383 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                       C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                   C382 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                       C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) +
                 C388 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                          std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                      (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                           (C313 * 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                       C313 *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 * C313 *
                           std::pow(C278, 14. / 6.)) /
                          std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                     C288 /
                     (std::pow(C278, 7. / 3.) *
                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                    C345) +
               (((C382 * C278 *
                      std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) * 4. *
                      (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           std::pow(C278, 7. / 3.) *
                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                           std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                           C283 * std::pow(C291, C283 - 1)) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(
                           4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2),
                           2)) +
                  C278 *
                      (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                           C282 * (C282 - 1) * std::pow(C291, C282 - 2) /
                           (2. * (std::pow(2., C280) - 1.)) -
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                            std::pow(C291, C283 - 2) -
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                            C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                            std::pow(C291, C283 - 1) * 4. *
                            (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                 std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                                 std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                             std::pow(
                                 std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2) *
                                 C283 * std::pow(C291, C283 - 1)) /
                            (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                           std::pow(
                               4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(
                                       std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.),
                                       2),
                               2)) *
                      C383) -
                 C380 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                      std::pow(std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2))) -
                C388 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) *
                   C323) +
              C278 *
                  (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                       std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) *
                  C466) +
             C278 *
                 (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                      std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                 C467) +
            C469 * C278 *
                (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                 C313 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                C288 /
                (std::pow(C278, 7. / 3.) *
                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.));

        const double C389 = 2.;
        const double C390 = 0.;
        const double C391 = C389;
        const double C392 = C391;
        const double C393 = C390 / C278;
        const double C394 = C393;
        const double C395 = -C393;
        const double C396 = 0.;
        const double C397 = C396;
        const double C398 = C397;
        const double C399 = C398 / C310;
        const double C400 = C399;
        const double C470 = (-C316 * C390) / std::pow(C278, 2);
        const double C471 = C470;
        const double C472 = -C470;
        const double C473 = (-C342 * C398) / std::pow(C310, 2);
        const double C474 = C473;
        matrix.upper[3] =
            ((((((((((((((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                              C282 * std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C290, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C278 *
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       C313 * C288 * C283 * std::pow(C290, C283 - 1) * 7. *
                                       std::pow(C278, 4. / 3.) / 3. +
                                   std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                                       C288 * C313 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C290, C283 - 1) * 4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  2 *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  7. * std::pow(C278, 4. / 3.) / 3.) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                            C394 +
                        ((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                              C282 * std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C291, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C278 *
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       C313 * C288 * C283 * std::pow(C291, C283 - 1) * 7. *
                                       std::pow(C278, 4. / 3.) / 3. +
                                   std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                                       C288 * C313 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C291, C283 - 1) * 4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  2 *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  7. * std::pow(C278, 4. / 3.) / 3.) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                            C395) +
                       (std::pow(C278, 7. / 3.) *
                            std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * C392 *
                            (C278 *
                                 (C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                      std::pow(C278, 1. / 6.) /
                                      (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                  (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                                       C313 * -0.00081290825 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                   C288 * C313 * -0.00081290825 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       2 * std::sqrt(C288) * C313 * 7. * std::pow(C278, 1. / 6.) /
                                       6.) /
                                      std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313,
                                               2)) +
                             (std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 +
                              C288 * C313 * -0.00081290825 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) /
                                  (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313))) -
                        C392 * C278 *
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                                 C313 +
                             C288 * C313 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) /
                                 (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                            std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 7. *
                            std::pow(C278, 4. / 3.) / 3.) /
                           std::pow(
                               std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2)) +
                      (std::pow(C278, 7. / 3.) *
                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * C400 *
                           (C278 *
                                (std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                     std::pow(C278, 1. / 6.) /
                                     (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                 (std::pow(std::pow(C278, 7. / 6.) * C313, 2) * C313 *
                                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * 7. *
                                           std::pow(C278, 1. / 6.) / 6. +
                                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                  C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                      std::pow(C278, 7. / 6.) * C313 * 2 * C313 * 7. *
                                      std::pow(C278, 1. / 6.) / 6.) /
                                     std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                                C288 +
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) +
                             C313 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                                C288) -
                       C400 * C278 *
                           (std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) +
                            C313 *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                           C288 * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           7. * std::pow(C278, 4. / 3.) / 3.) /
                          std::pow(
                              std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2)) *
                         C316 +
                     C333 *
                         (C394 * C278 * 0.01555 * C282 * std::pow(C290, C282 - 1) +
                          C395 * C278 * 0.01555 * C282 * std::pow(C291, C282 - 1)) /
                         (2. * (std::pow(2., C280) - 1.))) +
                    C335 *
                        (C394 * C278 * 0.0007 * C282 * std::pow(C290, C282 - 1) +
                         C395 * C278 * 0.0007 * C282 * std::pow(C291, C282 - 1)) /
                        (2. * (std::pow(2., C280) - 1.))) +
                   C326 *
                       (C394 * C278 * -0.0048 * C282 * std::pow(C290, C282 - 1) +
                        C395 * C278 * -0.0048 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.))) +
                  ((-C395 * C278 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.)) -
                   C394 * C278 * C282 * std::pow(C290, C282 - 1) /
                       (2. * (std::pow(2., C280) - 1.))) *
                      C346) +
                 (((C278 *
                        (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                             C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                             (2. * (std::pow(2., C280) - 1.)) -
                         (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                              std::pow(C290, C283 - 2) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C290, C283 - 1) * 4. *
                              (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2) *
                                   C283 * std::pow(C290, C283 - 1)) /
                              (4. *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                        C394 +
                    C395 * C278 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                        4. *
                        (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                             std::pow(C278, 7. / 3.) *
                             std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                             std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                         std::pow(std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                             C283 * std::pow(C290, C283 - 1)) /
                        (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                         std::pow(
                             4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                 std::pow(
                                     std::pow(C278, 7. / 3.) *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.),
                                     2),
                             2))) -
                   C392 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                            C313 +
                        C288 * C313 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) /
                            (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                       std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) -
                  C400 * C278 *
                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) +
                       C313 *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                           std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                      C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                     C322) +
                ((C392 * C278 *
                      ((std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                       (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                            (C313 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2) -
                             0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C288) /
                                                      (std::pow(C278, 7. / 6.) * C313))) -
                        C288 * C313 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            2 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) /
                           std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                      (std::pow(C278, 7. / 3.) *
                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
                  (C395 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                       C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                   C394 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                       C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) +
                 C400 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                          std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                      (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                           (C313 * 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                       C313 *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 * C313 *
                           std::pow(C278, 14. / 6.)) /
                          std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                     C288 /
                     (std::pow(C278, 7. / 3.) *
                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                    C345) +
               (((C394 * C278 *
                      std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) * 4. *
                      (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           std::pow(C278, 7. / 3.) *
                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                           std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                           C283 * std::pow(C291, C283 - 1)) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(
                           4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2),
                           2)) +
                  C278 *
                      (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                           C282 * (C282 - 1) * std::pow(C291, C282 - 2) /
                           (2. * (std::pow(2., C280) - 1.)) -
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                            std::pow(C291, C283 - 2) -
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                            C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                            std::pow(C291, C283 - 1) * 4. *
                            (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                 std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                                 std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                             std::pow(
                                 std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2) *
                                 C283 * std::pow(C291, C283 - 1)) /
                            (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                           std::pow(
                               4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(
                                       std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.),
                                       2),
                               2)) *
                      C395) -
                 C392 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                      std::pow(std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2))) -
                C400 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) *
                   C323) +
              C278 *
                  (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                       std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) *
                  C471) +
             C278 *
                 (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                      std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                 C472) +
            C474 * C278 *
                (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                 C313 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                C288 /
                (std::pow(C278, 7. / 3.) *
                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.));

        const double C401 = 0.;
        const double C402 = 1;
        const double C403 = C401 / C278;
        const double C404 = C403;
        const double C405 = -C403;
        const double C406 = 0.;
        const double C407 = C406;
        const double C408 = C407;
        const double C409 = C408 / C310;
        const double C410 = C409;
        const double C475 = (-C316 * C401) / std::pow(C278, 2);
        const double C476 = C475;
        const double C477 = -C475;
        const double C478 = (-C342 * C408) / std::pow(C310, 2);
        const double C479 = C478;
        matrix.upper[4] =
            ((((((((((((((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                              C282 * std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C290, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C278 *
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       C313 * C288 * C283 * std::pow(C290, C283 - 1) * 7. *
                                       std::pow(C278, 4. / 3.) / 3. +
                                   std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                                       C288 * C313 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C290, C283 - 1) * 4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  2 *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  7. * std::pow(C278, 4. / 3.) / 3.) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                            C404 +
                        ((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                              C282 * std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C291, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C278 *
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       C313 * C288 * C283 * std::pow(C291, C283 - 1) * 7. *
                                       std::pow(C278, 4. / 3.) / 3. +
                                   std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                                       C288 * C313 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C291, C283 - 1) * 4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  2 *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  7. * std::pow(C278, 4. / 3.) / 3.) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                            C405) +
                       (std::pow(C278, 7. / 3.) *
                            std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * C402 *
                            (C278 *
                                 (C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                      std::pow(C278, 1. / 6.) /
                                      (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                  (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                                       C313 * -0.00081290825 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                   C288 * C313 * -0.00081290825 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       2 * std::sqrt(C288) * C313 * 7. * std::pow(C278, 1. / 6.) /
                                       6.) /
                                      std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313,
                                               2)) +
                             (std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 +
                              C288 * C313 * -0.00081290825 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) /
                                  (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313))) -
                        C402 * C278 *
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                                 C313 +
                             C288 * C313 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) /
                                 (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                            std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 7. *
                            std::pow(C278, 4. / 3.) / 3.) /
                           std::pow(
                               std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2)) +
                      (std::pow(C278, 7. / 3.) *
                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * C410 *
                           (C278 *
                                (std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                     std::pow(C278, 1. / 6.) /
                                     (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                 (std::pow(std::pow(C278, 7. / 6.) * C313, 2) * C313 *
                                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * 7. *
                                           std::pow(C278, 1. / 6.) / 6. +
                                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                  C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                      std::pow(C278, 7. / 6.) * C313 * 2 * C313 * 7. *
                                      std::pow(C278, 1. / 6.) / 6.) /
                                     std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                                C288 +
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) +
                             C313 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                                C288) -
                       C410 * C278 *
                           (std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) +
                            C313 *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                           C288 * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           7. * std::pow(C278, 4. / 3.) / 3.) /
                          std::pow(
                              std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2)) *
                         C316 +
                     C333 *
                         (C404 * C278 * 0.01555 * C282 * std::pow(C290, C282 - 1) +
                          C405 * C278 * 0.01555 * C282 * std::pow(C291, C282 - 1)) /
                         (2. * (std::pow(2., C280) - 1.))) +
                    C335 *
                        (C404 * C278 * 0.0007 * C282 * std::pow(C290, C282 - 1) +
                         C405 * C278 * 0.0007 * C282 * std::pow(C291, C282 - 1)) /
                        (2. * (std::pow(2., C280) - 1.))) +
                   C326 *
                       (C404 * C278 * -0.0048 * C282 * std::pow(C290, C282 - 1) +
                        C405 * C278 * -0.0048 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.))) +
                  ((-C405 * C278 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.)) -
                   C404 * C278 * C282 * std::pow(C290, C282 - 1) /
                       (2. * (std::pow(2., C280) - 1.))) *
                      C346) +
                 (((C278 *
                        (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                             C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                             (2. * (std::pow(2., C280) - 1.)) -
                         (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                              std::pow(C290, C283 - 2) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C290, C283 - 1) * 4. *
                              (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2) *
                                   C283 * std::pow(C290, C283 - 1)) /
                              (4. *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                        C404 +
                    C405 * C278 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                        4. *
                        (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                             std::pow(C278, 7. / 3.) *
                             std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                             std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                         std::pow(std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                             C283 * std::pow(C290, C283 - 1)) /
                        (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                         std::pow(
                             4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                 std::pow(
                                     std::pow(C278, 7. / 3.) *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.),
                                     2),
                             2))) -
                   C402 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                            C313 +
                        C288 * C313 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) /
                            (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                       std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) -
                  C410 * C278 *
                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) +
                       C313 *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                           std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                      C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                     C322) +
                ((C402 * C278 *
                      ((std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                       (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                            (C313 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2) -
                             0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C288) /
                                                      (std::pow(C278, 7. / 6.) * C313))) -
                        C288 * C313 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            2 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) /
                           std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                      (std::pow(C278, 7. / 3.) *
                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
                  (C405 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                       C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                   C404 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                       C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) +
                 C410 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                          std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                      (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                           (C313 * 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                       C313 *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 * C313 *
                           std::pow(C278, 14. / 6.)) /
                          std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                     C288 /
                     (std::pow(C278, 7. / 3.) *
                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                    C345) +
               (((C404 * C278 *
                      std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) * 4. *
                      (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           std::pow(C278, 7. / 3.) *
                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                           std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                           C283 * std::pow(C291, C283 - 1)) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(
                           4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2),
                           2)) +
                  C278 *
                      (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                           C282 * (C282 - 1) * std::pow(C291, C282 - 2) /
                           (2. * (std::pow(2., C280) - 1.)) -
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                            std::pow(C291, C283 - 2) -
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                            C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                            std::pow(C291, C283 - 1) * 4. *
                            (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                 std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                                 std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                             std::pow(
                                 std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2) *
                                 C283 * std::pow(C291, C283 - 1)) /
                            (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                           std::pow(
                               4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(
                                       std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.),
                                       2),
                               2)) *
                      C405) -
                 C402 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                      std::pow(std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2))) -
                C410 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) *
                   C323) +
              C278 *
                  (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                       std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) *
                  C476) +
             C278 *
                 (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                      std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                 C477) +
            C479 * C278 *
                (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                 C313 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                C288 /
                (std::pow(C278, 7. / 3.) *
                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.));

        const double C480 = 0.;
        const double C481 =
            (C480 / C278 +
             ((-C349) / std::pow(C278, 2) - (-C347 * C284 * 2 * C278) / std::pow(C278, 4)) * C347) -
            C349 * C347 / std::pow(C278, 2);
        const double C482 = C481;
        const double C483 = -C481;
        const double C484 = (-C351 * -0.22167 * C351 * 2 * C287) / std::pow(C287, 4);
        const double C485 = (-C351 * -3. * C351 * 2 * C287) / std::pow(C287, 4);
        const double C486 = C280 * (C280 - 1) * std::pow(C293, C280 - 2) * std::pow(C356, 2) +
                            C280 * std::pow(C293, C280 - 1) * C485;
        const double C487 = C281 * (C281 - 1) * std::pow(C293, C281 - 2) * std::pow(C356, 2) +
                            C281 * std::pow(C293, C281 - 1) * C485;
        const double C488 = 0.7389e-5 * C487 + 0.;
        const double C489 = 0.0116 * C486;
        const double C490 = 0.023266 * C486;
        const double C491 = 0.472 * C487;
        const double C492 = 8.723 * C486;
        const double C493 = C486 / C294 - std::pow(C357, 2) / std::pow(C294, 2);
        const double C494 = 0.0311 * C493;
        const double C495 = ((C357 * C364 + C301 * C486) + C364 * C357) + C294 * C493;
        const double C496 = C490 + C488;
        const double C497 = C492 + C491;
        const double C498 = 0.002 * C495;
        const double C499 = C496;
        const double C500 = C497 + C484;
        const double C501 = C494 + C498;
        const double C502 = C500;
        const double C503 = C501;
        const double C504 = ((C499 / C310 + ((-C370) / std::pow(C310, 2) -
                                             (-C373 * C307 * 2 * C310) / std::pow(C310, 4)) *
                                                C373) -
                             C502 * C307 / std::pow(C310, 2)) -
                            C370 * C373 / std::pow(C310, 2);
        const double C505 = C504;
        const double C506 = C503 - C489;
        matrix.upper[5] =
            ((((((((((((((((((((((std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                      (C278 *
                                           ((6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                                                 (std::pow(C278, 7. / 3.) *
                                                      std::sqrt((std::pow(C290, C283) +
                                                                 std::pow(C291, C283)) /
                                                                2.) *
                                                      C288 * C313 *
                                                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                                (std::pow(C278, 7. / 6.) * C313)) *
                                                           0.00081290825 * std::sqrt(C288) * C313 *
                                                           7. * std::pow(C278, -5. / 6.) / 6. +
                                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                           std::pow(C278, 1. / 6.) *
                                                           std::exp(
                                                               -0.00081290825 * std::sqrt(C288) /
                                                               (std::pow(C278, 7. / 6.) * C313)) *
                                                           0.00081290825 * std::sqrt(C288) * C313 *
                                                           7. * std::pow(C278, 1. / 6.) /
                                                           (6. *
                                                            std::pow(std::pow(C278, 7. / 6.) * C313,
                                                                     2))) +
                                                  C288 * C313 *
                                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                                               (std::pow(C278, 7. / 6.) * C313)) *
                                                      0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                      std::pow(C278, 1. / 6.) *
                                                      std::sqrt((std::pow(C290, C283) +
                                                                 std::pow(C291, C283)) /
                                                                2.) *
                                                      7. * std::pow(C278, 4. / 3.) / 3.) -
                                             std::pow(C278, 7. / 3.) *
                                                 std::sqrt(
                                                     (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                                 C288 * C313 *
                                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                                          (std::pow(C278, 7. / 6.) * C313)) *
                                                 0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                 std::pow(C278, 1. / 6.) * 6. *
                                                 std::pow(C278, 7. / 6.) * C313 * 2 * C313 * 7. *
                                                 std::pow(C278, 1. / 6.) / 6.) /
                                                std::pow(
                                                    6. *
                                                        std::pow(std::pow(C278, 7. / 6.) * C313, 2),
                                                    2) -
                                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                      (std::pow(C278, 7. / 6.) * C313)) *
                                                 C313 * C288 *
                                                 std::sqrt(
                                                     (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                                 28. * std::pow(C278, 1. / 3.) / 3. +
                                             std::sqrt(
                                                 (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.) *
                                                 7. * std::pow(C278, 4. / 3.) * C288 * C313 *
                                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                                          (std::pow(C278, 7. / 6.) * C313)) *
                                                 0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                 std::pow(C278, 1. / 6.) /
                                                 (6. *
                                                  std::pow(std::pow(C278, 7. / 6.) * C313, 2))) /
                                                3.) +
                                       (std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                            C288 * C313 *
                                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                                     (std::pow(C278, 7. / 6.) * C313)) *
                                            0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                            std::pow(C278, 1. / 6.) /
                                            (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                                 (std::pow(C278, 7. / 6.) * C313)) *
                                            C313 * C288 *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                            7. * std::pow(C278, 4. / 3.) / 3.)) -
                                  C278 *
                                      (std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           C288 * C313 *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           C313 * C288 *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           7. * std::pow(C278, 4. / 3.) / 3.) *
                                      std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      2 *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      7. * std::pow(C278, 4. / 3.) / 3.) /
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              4) +
                                 (std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      C288 * C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                      std::pow(C278, 1. / 6.) /
                                      (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      7. * std::pow(C278, 4. / 3.) / 3.) /
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2)) *
                                    C347 +
                                ((2. - (std::pow(C290, C282) + std::pow(C291, C282))) /
                                     (2. * (std::pow(2., C280) - 1.)) +
                                 1) *
                                    C377) +
                               C364 * 0.01555 *
                                   ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                                   (2. * (std::pow(2., C280) - 1.))) +
                              C366 * 0.0007 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                                  (2. * (std::pow(2., C280) - 1.))) +
                             C357 * -0.0048 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                                 (2. * (std::pow(2., C280) - 1.))) +
                            ((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                               C314) *
                                  C282 * std::pow(C290, C282 - 1) /
                                  (2. * (std::pow(2., C280) - 1.)) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C290, C283 - 1) /
                                  (4. *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(
                                       std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.),
                                       2))) -
                             C278 *
                                 (4. *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      std::pow(std::pow(C278, 7. / 3.) *
                                                   std::sqrt((std::pow(C290, C283) +
                                                              std::pow(C291, C283)) /
                                                             2.),
                                               2) *
                                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           C313 * C288 * C283 * std::pow(C290, C283 - 1) * 7. *
                                           std::pow(C278, 4. / 3.) / 3. +
                                       std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                                           C288 * C313 *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                      std::pow(C290, C283 - 1) * 4. *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      2 *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      7. * std::pow(C278, 4. / 3.) / 3.) /
                                 std::pow(
                                     4. *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.) *
                                         std::pow(std::pow(C278, 7. / 3.) *
                                                      std::sqrt((std::pow(C290, C283) +
                                                                 std::pow(C291, C283)) /
                                                                2.),
                                                  2),
                                     2)) *
                                C353) +
                           ((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                              C314) *
                                 C282 * std::pow(C291, C282 - 1) /
                                 (2. * (std::pow(2., C280) - 1.)) -
                             std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                                 C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                 std::pow(C291, C283 - 1) /
                                 (4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2))) -
                            C278 *
                                (4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2) *
                                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                          C313 * C288 * C283 * std::pow(C291, C283 - 1) * 7. *
                                          std::pow(C278, 4. / 3.) / 3. +
                                      std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                                          C288 * C313 *
                                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                          std::pow(C278, 1. / 6.) /
                                          (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                     C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                     std::pow(C291, C283 - 1) * 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     2 *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     7. * std::pow(C278, 4. / 3.) / 3.) /
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.) *
                                        std::pow(std::pow(C278, 7. / 3.) *
                                                     std::sqrt((std::pow(C290, C283) +
                                                                std::pow(C291, C283)) /
                                                               2.),
                                                 2),
                                    2)) *
                               C354) +
                          (std::pow(C278, 7. / 3.) *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               C376 *
                               (C278 *
                                    (std::exp(-0.00081290825 * std::sqrt(C288) /
                                              (std::pow(C278, 7. / 6.) * C313)) *
                                         0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                         std::pow(C278, 1. / 6.) /
                                         (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                     (std::pow(std::pow(C278, 7. / 6.) * C313, 2) * C313 *
                                          (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                               0.00081290825 * std::sqrt(C288) * 7. *
                                               std::pow(C278, 1. / 6.) / 6. +
                                           0.00081290825 * std::sqrt(C288) *
                                               std::pow(C278, 7. / 6.) *
                                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                                        (std::pow(C278, 7. / 6.) * C313)) *
                                               0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                               std::pow(C278, 1. / 6.) /
                                               (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                      C313 *
                                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) *
                                          std::pow(C278, 7. / 6.) * std::pow(C278, 7. / 6.) * C313 *
                                          2 * C313 * 7. * std::pow(C278, 1. / 6.) / 6.) /
                                         std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                                    C288 +
                                (std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) +
                                 C313 *
                                     std::exp(-0.00081290825 * std::sqrt(C288) /
                                              (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                                    C288) -
                           C376 * C278 *
                               (std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) +
                                C313 *
                                    std::exp(-0.00081290825 * std::sqrt(C288) /
                                             (std::pow(C278, 7. / 6.) * C313)) *
                                    0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                    std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                               C288 *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 7. *
                               std::pow(C278, 4. / 3.) / 3.) /
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2)) *
                             C347 +
                         (((((((((std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                      C278 *
                                      (C288 * C313 *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) * std::pow(C278, 7. / 3.) *
                                           C283 * std::pow(C290, C283 - 1) /
                                           (4. *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                            6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           C313 * C288 * 7. * std::pow(C278, 4. / 3.) * C283 *
                                           std::pow(C290, C283 - 1) /
                                           (12. * std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.))) -
                                  C278 *
                                      (std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           C288 * C313 *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           C313 * C288 *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           7. * std::pow(C278, 4. / 3.) / 3.) *
                                      std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      2 * std::pow(C278, 7. / 3.) * C283 *
                                      std::pow(C290, C283 - 1) /
                                      (4. *
                                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.))) /
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              4) +
                                 (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                                   C314) *
                                      C282 * std::pow(C290, C282 - 1) /
                                      (2. * (std::pow(2., C280) - 1.)) -
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                      std::pow(C290, C283 - 1) /
                                      (4. *
                                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.) *
                                       std::pow(std::pow(C278, 7. / 3.) *
                                                    std::sqrt((std::pow(C290, C283) +
                                                               std::pow(C291, C283)) /
                                                              2.),
                                                2)))) *
                                    C347 -
                                C377 * C278 * C282 * std::pow(C290, C282 - 1) /
                                    (2. * (std::pow(2., C280) - 1.))) +
                               C364 * C278 * 0.01555 * C282 * std::pow(C290, C282 - 1) /
                                   (2. * (std::pow(2., C280) - 1.))) +
                              C366 * C278 * 0.0007 * C282 * std::pow(C290, C282 - 1) /
                                  (2. * (std::pow(2., C280) - 1.))) +
                             C357 * C278 * -0.0048 * C282 * std::pow(C290, C282 - 1) /
                                 (2. * (std::pow(2., C280) - 1.))) +
                            C278 *
                                (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                                  C314) *
                                     C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                                     (2. * (std::pow(2., C280) - 1.)) -
                                 (4. *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      std::pow(std::pow(C278, 7. / 3.) *
                                                   std::sqrt((std::pow(C290, C283) +
                                                              std::pow(C291, C283)) /
                                                             2.),
                                               2) *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                                      std::pow(C290, C283 - 2) -
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                      std::pow(C290, C283 - 1) * 4. *
                                      (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.) *
                                           std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           2 * std::pow(C278, 7. / 3.) * C283 *
                                           std::pow(C290, C283 - 1) +
                                       std::pow(std::pow(C278, 7. / 3.) *
                                                    std::sqrt((std::pow(C290, C283) +
                                                               std::pow(C291, C283)) /
                                                              2.),
                                                2) *
                                           C283 * std::pow(C290, C283 - 1)) /
                                      (4. *
                                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                 2.))) /
                                     std::pow(4. *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.) *
                                                  std::pow(std::pow(C278, 7. / 3.) *
                                                               std::sqrt((std::pow(C290, C283) +
                                                                          std::pow(C291, C283)) /
                                                                         2.),
                                                           2),
                                              2)) *
                                C353) +
                           C354 * C278 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) *
                               C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                               std::pow(C291, C283 - 1) * 4. *
                               (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                                std::pow(
                                    std::pow(C278, 7. / 3.) *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.),
                                    2) *
                                    C283 * std::pow(C290, C283 - 1)) /
                               (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.) *
                                        std::pow(std::pow(C278, 7. / 3.) *
                                                     std::sqrt((std::pow(C290, C283) +
                                                                std::pow(C291, C283)) /
                                                               2.),
                                                 2),
                                    2))) -
                          C376 * C278 *
                              (std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) +
                               C313 *
                                   std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                   0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                   std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                              C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) *
                             C353) +
                        (((((((((std::pow(
                                     std::pow(C278, 7. / 3.) *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.),
                                     2) *
                                     C278 *
                                     (C288 * C313 *
                                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                          std::pow(C278, 1. / 6.) * std::pow(C278, 7. / 3.) * C283 *
                                          std::pow(C291, C283 - 1) /
                                          (4. *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                          C313 * C288 * 7. * std::pow(C278, 4. / 3.) * C283 *
                                          std::pow(C291, C283 - 1) /
                                          (12. *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.))) -
                                 C278 *
                                     (std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.) *
                                          C288 * C313 *
                                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                          std::pow(C278, 1. / 6.) /
                                          (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                          C313 * C288 *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.) *
                                          7. * std::pow(C278, 4. / 3.) / 3.) *
                                     std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.))) /
                                    std::pow(
                                        std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        4) +
                                (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                                  C314) *
                                     C282 * std::pow(C291, C282 - 1) /
                                     (2. * (std::pow(2., C280) - 1.)) -
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                     C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                     std::pow(C291, C283 - 1) /
                                     (4. *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                2.) *
                                      std::pow(std::pow(C278, 7. / 3.) *
                                                   std::sqrt((std::pow(C290, C283) +
                                                              std::pow(C291, C283)) /
                                                             2.),
                                               2)))) *
                                   C347 -
                               C377 * C278 * C282 * std::pow(C291, C282 - 1) /
                                   (2. * (std::pow(2., C280) - 1.))) +
                              C364 * C278 * 0.01555 * C282 * std::pow(C291, C282 - 1) /
                                  (2. * (std::pow(2., C280) - 1.))) +
                             C366 * C278 * 0.0007 * C282 * std::pow(C291, C282 - 1) /
                                 (2. * (std::pow(2., C280) - 1.))) +
                            C357 * C278 * -0.0048 * C282 * std::pow(C291, C282 - 1) /
                                (2. * (std::pow(2., C280) - 1.))) +
                           C353 * C278 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) *
                               C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                               std::pow(C290, C283 - 1) * 4. *
                               (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                                std::pow(
                                    std::pow(C278, 7. / 3.) *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.),
                                    2) *
                                    C283 * std::pow(C291, C283 - 1)) /
                               (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.) *
                                        std::pow(std::pow(C278, 7. / 3.) *
                                                     std::sqrt((std::pow(C290, C283) +
                                                                std::pow(C291, C283)) /
                                                               2.),
                                                 2),
                                    2))) +
                          C278 *
                              (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) -
                                C314) *
                                   C282 * (C282 - 1) * std::pow(C291, C282 - 2) /
                                   (2. * (std::pow(2., C280) - 1.)) -
                               (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    std::pow(
                                        std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2) *
                                    std::exp(-0.00081290825 * std::sqrt(C288) /
                                             (std::pow(C278, 7. / 6.) * C313)) *
                                    C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                                    std::pow(C291, C283 - 2) -
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                    C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                    std::pow(C291, C283 - 1) * 4. *
                                    (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                         std::pow(C278, 7. / 3.) *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.) *
                                         2 * std::pow(C278, 7. / 3.) * C283 *
                                         std::pow(C291, C283 - 1) +
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2) *
                                         C283 * std::pow(C291, C283 - 1)) /
                                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.))) /
                                   std::pow(
                                       4. *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.) *
                                           std::pow(std::pow(C278, 7. / 3.) *
                                                        std::sqrt((std::pow(C290, C283) +
                                                                   std::pow(C291, C283)) /
                                                                  2.),
                                                    2),
                                       2)) *
                              C354) -
                         C376 * C278 *
                             (std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) +
                              C313 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                  0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                  std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                             C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2))) *
                            C354) +
                       ((((C278 *
                               ((6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                                     std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     C288 *
                                     (C313 * (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                       (std::pow(C278, 7. / 6.) * C313)) *
                                                  0.00081290825 * std::sqrt(C288) * 7. *
                                                  std::pow(C278, 1. / 6.) +
                                              0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                                  std::pow(C278, 1. / 6.) *
                                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                                           (std::pow(C278, 7. / 6.) * C313)) *
                                                  0.00081290825 * std::sqrt(C288) *
                                                  std::pow(C278, 7. / 6.) /
                                                  std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                          0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                          std::pow(C278, 1. / 6.)) -
                                 std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     C288 * C313 *
                                     std::exp(-0.00081290825 * std::sqrt(C288) /
                                              (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                     std::pow(C278, 1. / 6.) * 12. * C313 *
                                     std::pow(C278, 14. / 6.)) /
                                    std::pow(6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2), 2) -
                                (std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) +
                                 C313 *
                                     std::exp(-0.00081290825 * std::sqrt(C288) /
                                              (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                                    C288 *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                    7. * std::pow(C278, 4. / 3.) / 3.) /
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2) +
                           (std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) +
                            C313 *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                               C288 /
                               (std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                              C347 -
                          C353 * C278 *
                              (std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) +
                               C313 *
                                   std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                   0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                   std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                              C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C354 * C278 *
                             (std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) +
                              C313 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                  0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                  std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                             C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2))) +
                        C376 * C278 *
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                             (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                                  (C313 * 0.00081290825 * std::sqrt(C288) *
                                       std::pow(C278, 7. / 6.) *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                       std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                                   std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                              C313 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) *
                                  0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 *
                                  C313 * std::pow(C278, 14. / 6.)) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                            C288 /
                            (std::pow(C278, 7. / 3.) *
                             std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                           C376) +
                      ((((2. - (std::pow(C290, C282) + std::pow(C291, C282))) /
                             (2. * (std::pow(2., C280) - 1.)) +
                         1) *
                            C347 -
                        C353 * C278 * C282 * std::pow(C290, C282 - 1) /
                            (2. * (std::pow(2., C280) - 1.))) -
                       C354 * C278 * C282 * std::pow(C291, C282 - 1) /
                           (2. * (std::pow(2., C280) - 1.))) *
                          C377) +
                     C364 *
                         ((C347 * 0.01555 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) +
                           C353 * C278 * 0.01555 * C282 * std::pow(C290, C282 - 1)) +
                          C354 * C278 * 0.01555 * C282 * std::pow(C291, C282 - 1)) /
                         (2. * (std::pow(2., C280) - 1.))) +
                    C366 *
                        ((C347 * 0.0007 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) +
                          C353 * C278 * 0.0007 * C282 * std::pow(C290, C282 - 1)) +
                         C354 * C278 * 0.0007 * C282 * std::pow(C291, C282 - 1)) /
                        (2. * (std::pow(2., C280) - 1.))) +
                   C357 *
                       ((C347 * -0.0048 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) +
                         C353 * C278 * -0.0048 * C282 * std::pow(C290, C282 - 1)) +
                        C354 * C278 * -0.0048 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.))) +
                  C278 *
                      ((2. - (std::pow(C290, C282) + std::pow(C291, C282))) /
                           (2. * (std::pow(2., C280) - 1.)) +
                       1) *
                      C506) +
                 C493 * C278 * 0.01555 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                     (2. * (std::pow(2., C280) - 1.))) +
                C495 * C278 * 0.0007 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                    (2. * (std::pow(2., C280) - 1.))) +
               C486 * C278 * -0.0048 * ((std::pow(C290, C282) + std::pow(C291, C282)) - 2.) /
                   (2. * (std::pow(2., C280) - 1.))) +
              C278 *
                  (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                       std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) *
                  C482) +
             C278 *
                 (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                      std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                 C483) +
            C505 * C278 *
                (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                 C313 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                C288 /
                (std::pow(C278, 7. / 3.) *
                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.));

        const double C507 = (-C347 * C378) / std::pow(C278, 2);
        const double C508 = C507;
        const double C509 = -C507;
        const double C510 = (-C373 * C386) / std::pow(C310, 2);
        const double C511 = C510;
        matrix.upper[6] =
            ((((((((((((((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                              C282 * std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C290, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C278 *
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       C313 * C288 * C283 * std::pow(C290, C283 - 1) * 7. *
                                       std::pow(C278, 4. / 3.) / 3. +
                                   std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                                       C288 * C313 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C290, C283 - 1) * 4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  2 *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  7. * std::pow(C278, 4. / 3.) / 3.) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                            C382 +
                        ((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                              C282 * std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C291, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C278 *
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       C313 * C288 * C283 * std::pow(C291, C283 - 1) * 7. *
                                       std::pow(C278, 4. / 3.) / 3. +
                                   std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                                       C288 * C313 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C291, C283 - 1) * 4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  2 *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  7. * std::pow(C278, 4. / 3.) / 3.) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                            C383) +
                       (std::pow(C278, 7. / 3.) *
                            std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * C380 *
                            (C278 *
                                 (C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                      std::pow(C278, 1. / 6.) /
                                      (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                  (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                                       C313 * -0.00081290825 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                   C288 * C313 * -0.00081290825 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       2 * std::sqrt(C288) * C313 * 7. * std::pow(C278, 1. / 6.) /
                                       6.) /
                                      std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313,
                                               2)) +
                             (std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 +
                              C288 * C313 * -0.00081290825 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) /
                                  (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313))) -
                        C380 * C278 *
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                                 C313 +
                             C288 * C313 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) /
                                 (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                            std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 7. *
                            std::pow(C278, 4. / 3.) / 3.) /
                           std::pow(
                               std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2)) +
                      (std::pow(C278, 7. / 3.) *
                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * C388 *
                           (C278 *
                                (std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                     std::pow(C278, 1. / 6.) /
                                     (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                 (std::pow(std::pow(C278, 7. / 6.) * C313, 2) * C313 *
                                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * 7. *
                                           std::pow(C278, 1. / 6.) / 6. +
                                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                  C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                      std::pow(C278, 7. / 6.) * C313 * 2 * C313 * 7. *
                                      std::pow(C278, 1. / 6.) / 6.) /
                                     std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                                C288 +
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) +
                             C313 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                                C288) -
                       C388 * C278 *
                           (std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) +
                            C313 *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                           C288 * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           7. * std::pow(C278, 4. / 3.) / 3.) /
                          std::pow(
                              std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2)) *
                         C347 +
                     C364 *
                         (C382 * C278 * 0.01555 * C282 * std::pow(C290, C282 - 1) +
                          C383 * C278 * 0.01555 * C282 * std::pow(C291, C282 - 1)) /
                         (2. * (std::pow(2., C280) - 1.))) +
                    C366 *
                        (C382 * C278 * 0.0007 * C282 * std::pow(C290, C282 - 1) +
                         C383 * C278 * 0.0007 * C282 * std::pow(C291, C282 - 1)) /
                        (2. * (std::pow(2., C280) - 1.))) +
                   C357 *
                       (C382 * C278 * -0.0048 * C282 * std::pow(C290, C282 - 1) +
                        C383 * C278 * -0.0048 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.))) +
                  ((-C383 * C278 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.)) -
                   C382 * C278 * C282 * std::pow(C290, C282 - 1) /
                       (2. * (std::pow(2., C280) - 1.))) *
                      C377) +
                 (((C278 *
                        (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                             C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                             (2. * (std::pow(2., C280) - 1.)) -
                         (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                              std::pow(C290, C283 - 2) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C290, C283 - 1) * 4. *
                              (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2) *
                                   C283 * std::pow(C290, C283 - 1)) /
                              (4. *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                        C382 +
                    C383 * C278 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                        4. *
                        (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                             std::pow(C278, 7. / 3.) *
                             std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                             std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                         std::pow(std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                             C283 * std::pow(C290, C283 - 1)) /
                        (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                         std::pow(
                             4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                 std::pow(
                                     std::pow(C278, 7. / 3.) *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.),
                                     2),
                             2))) -
                   C380 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                            C313 +
                        C288 * C313 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) /
                            (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                       std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) -
                  C388 * C278 *
                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) +
                       C313 *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                           std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                      C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                     C353) +
                ((C380 * C278 *
                      ((std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                       (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                            (C313 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2) -
                             0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C288) /
                                                      (std::pow(C278, 7. / 6.) * C313))) -
                        C288 * C313 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            2 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) /
                           std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                      (std::pow(C278, 7. / 3.) *
                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
                  (C383 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                       C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                   C382 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                       C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) +
                 C388 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                          std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                      (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                           (C313 * 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                       C313 *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 * C313 *
                           std::pow(C278, 14. / 6.)) /
                          std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                     C288 /
                     (std::pow(C278, 7. / 3.) *
                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                    C376) +
               (((C382 * C278 *
                      std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) * 4. *
                      (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           std::pow(C278, 7. / 3.) *
                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                           std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                           C283 * std::pow(C291, C283 - 1)) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(
                           4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2),
                           2)) +
                  C278 *
                      (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                           C282 * (C282 - 1) * std::pow(C291, C282 - 2) /
                           (2. * (std::pow(2., C280) - 1.)) -
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                            std::pow(C291, C283 - 2) -
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                            C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                            std::pow(C291, C283 - 1) * 4. *
                            (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                 std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                                 std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                             std::pow(
                                 std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2) *
                                 C283 * std::pow(C291, C283 - 1)) /
                            (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                           std::pow(
                               4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(
                                       std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.),
                                       2),
                               2)) *
                      C383) -
                 C380 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                      std::pow(std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2))) -
                C388 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) *
                   C354) +
              C278 *
                  (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                       std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) *
                  C508) +
             C278 *
                 (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                      std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                 C509) +
            C511 * C278 *
                (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                 C313 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                C288 /
                (std::pow(C278, 7. / 3.) *
                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.));

        const double C512 = (-C347 * C390) / std::pow(C278, 2);
        const double C513 = C512;
        const double C514 = -C512;
        const double C515 = (-C373 * C398) / std::pow(C310, 2);
        const double C516 = C515;
        matrix.upper[7] =
            ((((((((((((((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                              C282 * std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C290, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C278 *
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       C313 * C288 * C283 * std::pow(C290, C283 - 1) * 7. *
                                       std::pow(C278, 4. / 3.) / 3. +
                                   std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                                       C288 * C313 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C290, C283 - 1) * 4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  2 *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  7. * std::pow(C278, 4. / 3.) / 3.) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                            C394 +
                        ((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                              C282 * std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C291, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C278 *
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       C313 * C288 * C283 * std::pow(C291, C283 - 1) * 7. *
                                       std::pow(C278, 4. / 3.) / 3. +
                                   std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                                       C288 * C313 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C291, C283 - 1) * 4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  2 *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  7. * std::pow(C278, 4. / 3.) / 3.) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                            C395) +
                       (std::pow(C278, 7. / 3.) *
                            std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * C392 *
                            (C278 *
                                 (C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                      std::pow(C278, 1. / 6.) /
                                      (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                  (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                                       C313 * -0.00081290825 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                   C288 * C313 * -0.00081290825 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       2 * std::sqrt(C288) * C313 * 7. * std::pow(C278, 1. / 6.) /
                                       6.) /
                                      std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313,
                                               2)) +
                             (std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 +
                              C288 * C313 * -0.00081290825 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) /
                                  (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313))) -
                        C392 * C278 *
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                                 C313 +
                             C288 * C313 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) /
                                 (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                            std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 7. *
                            std::pow(C278, 4. / 3.) / 3.) /
                           std::pow(
                               std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2)) +
                      (std::pow(C278, 7. / 3.) *
                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * C400 *
                           (C278 *
                                (std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                     std::pow(C278, 1. / 6.) /
                                     (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                 (std::pow(std::pow(C278, 7. / 6.) * C313, 2) * C313 *
                                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * 7. *
                                           std::pow(C278, 1. / 6.) / 6. +
                                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                  C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                      std::pow(C278, 7. / 6.) * C313 * 2 * C313 * 7. *
                                      std::pow(C278, 1. / 6.) / 6.) /
                                     std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                                C288 +
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) +
                             C313 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                                C288) -
                       C400 * C278 *
                           (std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) +
                            C313 *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                           C288 * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           7. * std::pow(C278, 4. / 3.) / 3.) /
                          std::pow(
                              std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2)) *
                         C347 +
                     C364 *
                         (C394 * C278 * 0.01555 * C282 * std::pow(C290, C282 - 1) +
                          C395 * C278 * 0.01555 * C282 * std::pow(C291, C282 - 1)) /
                         (2. * (std::pow(2., C280) - 1.))) +
                    C366 *
                        (C394 * C278 * 0.0007 * C282 * std::pow(C290, C282 - 1) +
                         C395 * C278 * 0.0007 * C282 * std::pow(C291, C282 - 1)) /
                        (2. * (std::pow(2., C280) - 1.))) +
                   C357 *
                       (C394 * C278 * -0.0048 * C282 * std::pow(C290, C282 - 1) +
                        C395 * C278 * -0.0048 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.))) +
                  ((-C395 * C278 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.)) -
                   C394 * C278 * C282 * std::pow(C290, C282 - 1) /
                       (2. * (std::pow(2., C280) - 1.))) *
                      C377) +
                 (((C278 *
                        (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                             C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                             (2. * (std::pow(2., C280) - 1.)) -
                         (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                              std::pow(C290, C283 - 2) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C290, C283 - 1) * 4. *
                              (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2) *
                                   C283 * std::pow(C290, C283 - 1)) /
                              (4. *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                        C394 +
                    C395 * C278 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                        4. *
                        (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                             std::pow(C278, 7. / 3.) *
                             std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                             std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                         std::pow(std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                             C283 * std::pow(C290, C283 - 1)) /
                        (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                         std::pow(
                             4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                 std::pow(
                                     std::pow(C278, 7. / 3.) *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.),
                                     2),
                             2))) -
                   C392 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                            C313 +
                        C288 * C313 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) /
                            (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                       std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) -
                  C400 * C278 *
                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) +
                       C313 *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                           std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                      C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                     C353) +
                ((C392 * C278 *
                      ((std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                       (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                            (C313 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2) -
                             0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C288) /
                                                      (std::pow(C278, 7. / 6.) * C313))) -
                        C288 * C313 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            2 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) /
                           std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                      (std::pow(C278, 7. / 3.) *
                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
                  (C395 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                       C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                   C394 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                       C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) +
                 C400 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                          std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                      (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                           (C313 * 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                       C313 *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 * C313 *
                           std::pow(C278, 14. / 6.)) /
                          std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                     C288 /
                     (std::pow(C278, 7. / 3.) *
                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                    C376) +
               (((C394 * C278 *
                      std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) * 4. *
                      (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           std::pow(C278, 7. / 3.) *
                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                           std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                           C283 * std::pow(C291, C283 - 1)) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(
                           4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2),
                           2)) +
                  C278 *
                      (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                           C282 * (C282 - 1) * std::pow(C291, C282 - 2) /
                           (2. * (std::pow(2., C280) - 1.)) -
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                            std::pow(C291, C283 - 2) -
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                            C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                            std::pow(C291, C283 - 1) * 4. *
                            (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                 std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                                 std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                             std::pow(
                                 std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2) *
                                 C283 * std::pow(C291, C283 - 1)) /
                            (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                           std::pow(
                               4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(
                                       std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.),
                                       2),
                               2)) *
                      C395) -
                 C392 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                      std::pow(std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2))) -
                C400 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) *
                   C354) +
              C278 *
                  (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                       std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) *
                  C513) +
             C278 *
                 (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                      std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                 C514) +
            C516 * C278 *
                (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                 C313 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                C288 /
                (std::pow(C278, 7. / 3.) *
                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.));

        const double C517 = (-C347 * C401) / std::pow(C278, 2);
        const double C518 = C517;
        const double C519 = -C517;
        const double C520 = (-C373 * C408) / std::pow(C310, 2);
        const double C521 = C520;
        matrix.upper[8] =
            ((((((((((((((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                              C282 * std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C290, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C278 *
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       C313 * C288 * C283 * std::pow(C290, C283 - 1) * 7. *
                                       std::pow(C278, 4. / 3.) / 3. +
                                   std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                                       C288 * C313 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C290, C283 - 1) * 4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  2 *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  7. * std::pow(C278, 4. / 3.) / 3.) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                            C404 +
                        ((((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                              C282 * std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C291, C283 - 1) /
                              (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2))) -
                         C278 *
                             (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C288) /
                                            (std::pow(C278, 7. / 6.) * C313)) *
                                       C313 * C288 * C283 * std::pow(C291, C283 - 1) * 7. *
                                       std::pow(C278, 4. / 3.) / 3. +
                                   std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                                       C288 * C313 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                                  std::pow(C291, C283 - 1) * 4. *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  2 *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  7. * std::pow(C278, 4. / 3.) / 3.) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                            C405) +
                       (std::pow(C278, 7. / 3.) *
                            std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * C402 *
                            (C278 *
                                 (C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                      std::pow(C278, 1. / 6.) /
                                      (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                  (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                                       C313 * -0.00081290825 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                       std::pow(C278, 1. / 6.) /
                                       (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) -
                                   C288 * C313 * -0.00081290825 *
                                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                       2 * std::sqrt(C288) * C313 * 7. * std::pow(C278, 1. / 6.) /
                                       6.) /
                                      std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313,
                                               2)) +
                             (std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                                  C313 +
                              C288 * C313 * -0.00081290825 *
                                  std::exp(-0.00081290825 * std::sqrt(C288) /
                                           (std::pow(C278, 7. / 6.) * C313)) /
                                  (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313))) -
                        C402 * C278 *
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                                 C313 +
                             C288 * C313 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) /
                                 (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                            std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 7. *
                            std::pow(C278, 4. / 3.) / 3.) /
                           std::pow(
                               std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2)) +
                      (std::pow(C278, 7. / 3.) *
                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * C410 *
                           (C278 *
                                (std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                     0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                     std::pow(C278, 1. / 6.) /
                                     (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                                 (std::pow(std::pow(C278, 7. / 6.) * C313, 2) * C313 *
                                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                                (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * 7. *
                                           std::pow(C278, 1. / 6.) / 6. +
                                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                                    (std::pow(C278, 7. / 6.) * C313)) *
                                           0.00081290825 * std::sqrt(C288) * C313 * 7. *
                                           std::pow(C278, 1. / 6.) /
                                           (6. * std::pow(std::pow(C278, 7. / 6.) * C313, 2))) -
                                  C313 *
                                      std::exp(-0.00081290825 * std::sqrt(C288) /
                                               (std::pow(C278, 7. / 6.) * C313)) *
                                      0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                      std::pow(C278, 7. / 6.) * C313 * 2 * C313 * 7. *
                                      std::pow(C278, 1. / 6.) / 6.) /
                                     std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                                C288 +
                            (std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) +
                             C313 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                                C288) -
                       C410 * C278 *
                           (std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) +
                            C313 *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                           C288 * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           7. * std::pow(C278, 4. / 3.) / 3.) /
                          std::pow(
                              std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2)) *
                         C347 +
                     C364 *
                         (C404 * C278 * 0.01555 * C282 * std::pow(C290, C282 - 1) +
                          C405 * C278 * 0.01555 * C282 * std::pow(C291, C282 - 1)) /
                         (2. * (std::pow(2., C280) - 1.))) +
                    C366 *
                        (C404 * C278 * 0.0007 * C282 * std::pow(C290, C282 - 1) +
                         C405 * C278 * 0.0007 * C282 * std::pow(C291, C282 - 1)) /
                        (2. * (std::pow(2., C280) - 1.))) +
                   C357 *
                       (C404 * C278 * -0.0048 * C282 * std::pow(C290, C282 - 1) +
                        C405 * C278 * -0.0048 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.))) +
                  ((-C405 * C278 * C282 * std::pow(C291, C282 - 1)) /
                       (2. * (std::pow(2., C280) - 1.)) -
                   C404 * C278 * C282 * std::pow(C290, C282 - 1) /
                       (2. * (std::pow(2., C280) - 1.))) *
                      C377) +
                 (((C278 *
                        (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                             C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                             (2. * (std::pow(2., C280) - 1.)) -
                         (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                              std::pow(C290, C283 - 2) -
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                              std::pow(C290, C283 - 1) * 4. *
                              (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   2 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2) *
                                   C283 * std::pow(C290, C283 - 1)) /
                              (4. *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                             std::pow(
                                 4. *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                     std::pow(std::pow(C278, 7. / 3.) *
                                                  std::sqrt((std::pow(C290, C283) +
                                                             std::pow(C291, C283)) /
                                                            2.),
                                              2),
                                 2)) *
                        C404 +
                    C405 * C278 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                        4. *
                        (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                             std::pow(C278, 7. / 3.) *
                             std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                             std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                         std::pow(std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                             C283 * std::pow(C290, C283 - 1)) /
                        (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                         std::pow(
                             4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                 std::pow(
                                     std::pow(C278, 7. / 3.) *
                                         std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                   2.),
                                     2),
                             2))) -
                   C402 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                            C313 +
                        C288 * C313 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) /
                            (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                       std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) -
                  C410 * C278 *
                      (std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) +
                       C313 *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                           std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                      C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                     C353) +
                ((C402 * C278 *
                      ((std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                       (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                            (C313 * -0.00081290825 *
                                 std::exp(-0.00081290825 * std::sqrt(C288) /
                                          (std::pow(C278, 7. / 6.) * C313)) *
                                 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                 std::pow(std::pow(C278, 7. / 6.) * C313, 2) -
                             0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C288) /
                                                      (std::pow(C278, 7. / 6.) * C313))) -
                        C288 * C313 * -0.00081290825 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            2 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) /
                           std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                      (std::pow(C278, 7. / 3.) *
                       std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
                  (C405 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                       C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                   C404 * C278 *
                       (std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) +
                        C313 *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                            std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                       C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) +
                 C410 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                          std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                      (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                           (C313 * 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                                std::exp(-0.00081290825 * std::sqrt(C288) /
                                         (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                                std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                                0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                       C313 *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 * C313 *
                           std::pow(C278, 14. / 6.)) /
                          std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                     C288 /
                     (std::pow(C278, 7. / 3.) *
                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                    C376) +
               (((C404 * C278 *
                      std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) * 4. *
                      (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           std::pow(C278, 7. / 3.) *
                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                           std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                           C283 * std::pow(C291, C283 - 1)) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(
                           4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                               std::pow(std::pow(C278, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                        2),
                           2)) +
                  C278 *
                      (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                           C282 * (C282 - 1) * std::pow(C291, C282 - 2) /
                           (2. * (std::pow(2., C280) - 1.)) -
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                            std::exp(-0.00081290825 * std::sqrt(C288) /
                                     (std::pow(C278, 7. / 6.) * C313)) *
                            C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                            std::pow(C291, C283 - 2) -
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                            C313 * C288 * std::pow(C278, 7. / 3.) * C283 *
                            std::pow(C291, C283 - 1) * 4. *
                            (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                 std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                                 std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                             std::pow(
                                 std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2) *
                                 C283 * std::pow(C291, C283 - 1)) /
                            (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                           std::pow(
                               4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                   std::pow(
                                       std::pow(C278, 7. / 3.) *
                                           std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                     2.),
                                       2),
                               2)) *
                      C405) -
                 C402 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                      std::pow(std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2))) -
                C410 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) *
                   C354) +
              C278 *
                  (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                       std::pow(C290, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                       (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(std::pow(C278, 7. / 3.) *
                                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                 2))) *
                  C518) +
             C278 *
                 (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                      std::pow(C291, C282 - 1) / (2. * (std::pow(2., C280) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                      C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                       std::pow(std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2))) *
                 C519) +
            C521 * C278 *
                (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                 C313 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                     std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                C288 /
                (std::pow(C278, 7. / 3.) *
                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.));

        matrix.upper[9] =
            (((((C278 *
                     (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                          C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                          (2. * (std::pow(2., C280) - 1.)) -
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           std::pow(
                               std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2) *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                           std::pow(C290, C283 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) *
                           C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                           4. *
                           (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                                std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                                C283 * std::pow(C290, C283 - 1)) /
                           (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2),
                              2)) *
                     C382 +
                 C383 * C278 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) * 4. *
                     (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                          std::pow(C278, 7. / 3.) *
                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                          std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                      std::pow(std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2) *
                          C283 * std::pow(C290, C283 - 1)) /
                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                      std::pow(
                          4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2),
                          2))) -
                C380 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                         C313 +
                     C288 * C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                    std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) -
               C388 * C278 *
                   (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                    C313 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                        std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                   C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2))) *
                  C382 +
              ((C380 * C278 *
                    (C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                     (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 *
                          (C288 * C313 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) /
                               (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                           C313 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313))) -
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                          std::pow(C278, 7. / 6.) * C313 / std::sqrt(C288)) /
                         std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                    (std::pow(C278, 7. / 3.) *
                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
                (C383 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                 C382 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) +
               C388 * C278 *
                   ((std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                     C313 *
                         (std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              0.00081290825 * std::pow(C278, 7. / 6.) / (2 * std::sqrt(C288)) +
                          0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                              -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) /
                              (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                        C288) /
                   (std::pow(C278, 7. / 3.) *
                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                  C380) +
             ((C380 * C278 *
                   ((std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                    (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                         (C313 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                              0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                              std::pow(std::pow(C278, 7. / 6.) * C313, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313))) -
                     C288 * C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         2 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                   (std::pow(C278, 7. / 3.) *
                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
               (C383 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                C382 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2))) +
              C388 * C278 *
                  (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                       std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                   (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                        (C313 * 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                             0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                             std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                             0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                    C313 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 * C313 *
                        std::pow(C278, 14. / 6.)) /
                       std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                  C288 /
                  (std::pow(C278, 7. / 3.) *
                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                 C388) +
            (((C382 * C278 *
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                   C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) * 4. *
                   (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(C278, 7. / 3.) *
                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                        std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2) *
                        C283 * std::pow(C291, C283 - 1)) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2),
                        2)) +
               C278 *
                   (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                        (C282 - 1) * std::pow(C291, C282 - 2) / (2. * (std::pow(2., C280) - 1.)) -
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                         std::pow(std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                         std::pow(C291, C283 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                         C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                         4. *
                         (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(C278, 7. / 3.) *
                              std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                              std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                          std::pow(
                              std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2) *
                              C283 * std::pow(C291, C283 - 1)) /
                         (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(
                                    std::pow(C278, 7. / 3.) *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.),
                                    2),
                            2)) *
                   C383) -
              C380 * C278 *
                  (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 +
                   C288 * C313 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) /
                       (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                  std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                  (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                   std::pow(std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                            2))) -
             C388 * C278 *
                 (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                  C313 *
                      std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                      0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                      std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                 C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                 (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                  std::pow(std::pow(C278, 7. / 3.) *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                           2))) *
                C383;

        matrix.upper[10] =
            (((((C278 *
                     (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                          C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                          (2. * (std::pow(2., C280) - 1.)) -
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           std::pow(
                               std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2) *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                           std::pow(C290, C283 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) *
                           C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                           4. *
                           (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                                std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                                C283 * std::pow(C290, C283 - 1)) /
                           (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2),
                              2)) *
                     C382 +
                 C383 * C278 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) * 4. *
                     (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                          std::pow(C278, 7. / 3.) *
                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                          std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                      std::pow(std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2) *
                          C283 * std::pow(C290, C283 - 1)) /
                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                      std::pow(
                          4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2),
                          2))) -
                C380 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                         C313 +
                     C288 * C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                    std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) -
               C388 * C278 *
                   (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                    C313 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                        std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                   C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2))) *
                  C394 +
              ((C380 * C278 *
                    (C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                     (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 *
                          (C288 * C313 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) /
                               (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                           C313 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313))) -
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                          std::pow(C278, 7. / 6.) * C313 / std::sqrt(C288)) /
                         std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                    (std::pow(C278, 7. / 3.) *
                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
                (C383 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                 C382 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) +
               C388 * C278 *
                   ((std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                     C313 *
                         (std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              0.00081290825 * std::pow(C278, 7. / 6.) / (2 * std::sqrt(C288)) +
                          0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                              -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) /
                              (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                        C288) /
                   (std::pow(C278, 7. / 3.) *
                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                  C392) +
             ((C380 * C278 *
                   ((std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                    (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                         (C313 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                              0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                              std::pow(std::pow(C278, 7. / 6.) * C313, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313))) -
                     C288 * C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         2 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                   (std::pow(C278, 7. / 3.) *
                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
               (C383 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                C382 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2))) +
              C388 * C278 *
                  (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                       std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                   (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                        (C313 * 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                             0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                             std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                             0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                    C313 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 * C313 *
                        std::pow(C278, 14. / 6.)) /
                       std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                  C288 /
                  (std::pow(C278, 7. / 3.) *
                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                 C400) +
            (((C382 * C278 *
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                   C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) * 4. *
                   (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(C278, 7. / 3.) *
                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                        std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2) *
                        C283 * std::pow(C291, C283 - 1)) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2),
                        2)) +
               C278 *
                   (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                        (C282 - 1) * std::pow(C291, C282 - 2) / (2. * (std::pow(2., C280) - 1.)) -
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                         std::pow(std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                         std::pow(C291, C283 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                         C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                         4. *
                         (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(C278, 7. / 3.) *
                              std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                              std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                          std::pow(
                              std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2) *
                              C283 * std::pow(C291, C283 - 1)) /
                         (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(
                                    std::pow(C278, 7. / 3.) *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.),
                                    2),
                            2)) *
                   C383) -
              C380 * C278 *
                  (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 +
                   C288 * C313 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) /
                       (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                  std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                  (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                   std::pow(std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                            2))) -
             C388 * C278 *
                 (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                  C313 *
                      std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                      0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                      std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                 C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                 (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                  std::pow(std::pow(C278, 7. / 3.) *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                           2))) *
                C395;

        matrix.upper[11] =
            (((((C278 *
                     (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                          C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                          (2. * (std::pow(2., C280) - 1.)) -
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           std::pow(
                               std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2) *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                           std::pow(C290, C283 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) *
                           C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                           4. *
                           (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                                std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                                C283 * std::pow(C290, C283 - 1)) /
                           (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2),
                              2)) *
                     C382 +
                 C383 * C278 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) * 4. *
                     (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                          std::pow(C278, 7. / 3.) *
                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                          std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                      std::pow(std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2) *
                          C283 * std::pow(C290, C283 - 1)) /
                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                      std::pow(
                          4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2),
                          2))) -
                C380 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                         C313 +
                     C288 * C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                    std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) -
               C388 * C278 *
                   (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                    C313 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                        std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                   C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2))) *
                  C404 +
              ((C380 * C278 *
                    (C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                     (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 *
                          (C288 * C313 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) /
                               (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                           C313 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313))) -
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                          std::pow(C278, 7. / 6.) * C313 / std::sqrt(C288)) /
                         std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                    (std::pow(C278, 7. / 3.) *
                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
                (C383 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                 C382 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) +
               C388 * C278 *
                   ((std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                     C313 *
                         (std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              0.00081290825 * std::pow(C278, 7. / 6.) / (2 * std::sqrt(C288)) +
                          0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                              -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) /
                              (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                        C288) /
                   (std::pow(C278, 7. / 3.) *
                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                  C402) +
             ((C380 * C278 *
                   ((std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                    (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                         (C313 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                              0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                              std::pow(std::pow(C278, 7. / 6.) * C313, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313))) -
                     C288 * C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         2 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                   (std::pow(C278, 7. / 3.) *
                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
               (C383 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                C382 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2))) +
              C388 * C278 *
                  (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                       std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                   (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                        (C313 * 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                             0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                             std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                             0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                    C313 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 * C313 *
                        std::pow(C278, 14. / 6.)) /
                       std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                  C288 /
                  (std::pow(C278, 7. / 3.) *
                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                 C410) +
            (((C382 * C278 *
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                   C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) * 4. *
                   (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(C278, 7. / 3.) *
                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                        std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2) *
                        C283 * std::pow(C291, C283 - 1)) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2),
                        2)) +
               C278 *
                   (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                        (C282 - 1) * std::pow(C291, C282 - 2) / (2. * (std::pow(2., C280) - 1.)) -
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                         std::pow(std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                         std::pow(C291, C283 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                         C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                         4. *
                         (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(C278, 7. / 3.) *
                              std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                              std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                          std::pow(
                              std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2) *
                              C283 * std::pow(C291, C283 - 1)) /
                         (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(
                                    std::pow(C278, 7. / 3.) *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.),
                                    2),
                            2)) *
                   C383) -
              C380 * C278 *
                  (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 +
                   C288 * C313 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) /
                       (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                  std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                  (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                   std::pow(std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                            2))) -
             C388 * C278 *
                 (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                  C313 *
                      std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                      0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                      std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                 C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                 (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                  std::pow(std::pow(C278, 7. / 3.) *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                           2))) *
                C405;

        matrix.upper[12] =
            (((((C278 *
                     (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                          C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                          (2. * (std::pow(2., C280) - 1.)) -
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           std::pow(
                               std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2) *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                           std::pow(C290, C283 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) *
                           C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                           4. *
                           (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                                std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                                C283 * std::pow(C290, C283 - 1)) /
                           (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2),
                              2)) *
                     C394 +
                 C395 * C278 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) * 4. *
                     (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                          std::pow(C278, 7. / 3.) *
                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                          std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                      std::pow(std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2) *
                          C283 * std::pow(C290, C283 - 1)) /
                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                      std::pow(
                          4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2),
                          2))) -
                C392 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                         C313 +
                     C288 * C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                    std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) -
               C400 * C278 *
                   (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                    C313 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                        std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                   C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2))) *
                  C394 +
              ((C392 * C278 *
                    (C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                     (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 *
                          (C288 * C313 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) /
                               (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                           C313 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313))) -
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                          std::pow(C278, 7. / 6.) * C313 / std::sqrt(C288)) /
                         std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                    (std::pow(C278, 7. / 3.) *
                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
                (C395 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                 C394 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) +
               C400 * C278 *
                   ((std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                     C313 *
                         (std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              0.00081290825 * std::pow(C278, 7. / 6.) / (2 * std::sqrt(C288)) +
                          0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                              -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) /
                              (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                        C288) /
                   (std::pow(C278, 7. / 3.) *
                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                  C392) +
             ((C392 * C278 *
                   ((std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                    (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                         (C313 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                              0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                              std::pow(std::pow(C278, 7. / 6.) * C313, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313))) -
                     C288 * C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         2 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                   (std::pow(C278, 7. / 3.) *
                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
               (C395 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                C394 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2))) +
              C400 * C278 *
                  (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                       std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                   (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                        (C313 * 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                             0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                             std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                             0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                    C313 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 * C313 *
                        std::pow(C278, 14. / 6.)) /
                       std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                  C288 /
                  (std::pow(C278, 7. / 3.) *
                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                 C400) +
            (((C394 * C278 *
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                   C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) * 4. *
                   (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(C278, 7. / 3.) *
                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                        std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2) *
                        C283 * std::pow(C291, C283 - 1)) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2),
                        2)) +
               C278 *
                   (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                        (C282 - 1) * std::pow(C291, C282 - 2) / (2. * (std::pow(2., C280) - 1.)) -
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                         std::pow(std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                         std::pow(C291, C283 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                         C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                         4. *
                         (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(C278, 7. / 3.) *
                              std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                              std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                          std::pow(
                              std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2) *
                              C283 * std::pow(C291, C283 - 1)) /
                         (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(
                                    std::pow(C278, 7. / 3.) *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.),
                                    2),
                            2)) *
                   C395) -
              C392 * C278 *
                  (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 +
                   C288 * C313 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) /
                       (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                  std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                  (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                   std::pow(std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                            2))) -
             C400 * C278 *
                 (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                  C313 *
                      std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                      0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                      std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                 C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                 (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                  std::pow(std::pow(C278, 7. / 3.) *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                           2))) *
                C395;

        matrix.upper[13] =
            (((((C278 *
                     (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                          C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                          (2. * (std::pow(2., C280) - 1.)) -
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           std::pow(
                               std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2) *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                           std::pow(C290, C283 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) *
                           C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                           4. *
                           (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                                std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                                C283 * std::pow(C290, C283 - 1)) /
                           (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2),
                              2)) *
                     C394 +
                 C395 * C278 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) * 4. *
                     (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                          std::pow(C278, 7. / 3.) *
                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                          std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                      std::pow(std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2) *
                          C283 * std::pow(C290, C283 - 1)) /
                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                      std::pow(
                          4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2),
                          2))) -
                C392 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                         C313 +
                     C288 * C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                    std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) -
               C400 * C278 *
                   (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                    C313 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                        std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                   C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2))) *
                  C404 +
              ((C392 * C278 *
                    (C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                     (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 *
                          (C288 * C313 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) /
                               (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                           C313 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313))) -
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                          std::pow(C278, 7. / 6.) * C313 / std::sqrt(C288)) /
                         std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                    (std::pow(C278, 7. / 3.) *
                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
                (C395 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                 C394 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) +
               C400 * C278 *
                   ((std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                     C313 *
                         (std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              0.00081290825 * std::pow(C278, 7. / 6.) / (2 * std::sqrt(C288)) +
                          0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                              -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) /
                              (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                        C288) /
                   (std::pow(C278, 7. / 3.) *
                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                  C402) +
             ((C392 * C278 *
                   ((std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                    (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                         (C313 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                              0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                              std::pow(std::pow(C278, 7. / 6.) * C313, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313))) -
                     C288 * C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         2 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                   (std::pow(C278, 7. / 3.) *
                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
               (C395 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                C394 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2))) +
              C400 * C278 *
                  (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                       std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                   (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                        (C313 * 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                             0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                             std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                             0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                    C313 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 * C313 *
                        std::pow(C278, 14. / 6.)) /
                       std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                  C288 /
                  (std::pow(C278, 7. / 3.) *
                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                 C410) +
            (((C394 * C278 *
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                   C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) * 4. *
                   (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(C278, 7. / 3.) *
                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                        std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2) *
                        C283 * std::pow(C291, C283 - 1)) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2),
                        2)) +
               C278 *
                   (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                        (C282 - 1) * std::pow(C291, C282 - 2) / (2. * (std::pow(2., C280) - 1.)) -
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                         std::pow(std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                         std::pow(C291, C283 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                         C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                         4. *
                         (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(C278, 7. / 3.) *
                              std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                              std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                          std::pow(
                              std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2) *
                              C283 * std::pow(C291, C283 - 1)) /
                         (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(
                                    std::pow(C278, 7. / 3.) *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.),
                                    2),
                            2)) *
                   C395) -
              C392 * C278 *
                  (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 +
                   C288 * C313 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) /
                       (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                  std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                  (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                   std::pow(std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                            2))) -
             C400 * C278 *
                 (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                  C313 *
                      std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                      0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                      std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                 C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                 (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                  std::pow(std::pow(C278, 7. / 3.) *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                           2))) *
                C405;

        matrix.upper[14] =
            (((((C278 *
                     (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) *
                          C282 * (C282 - 1) * std::pow(C290, C282 - 2) /
                          (2. * (std::pow(2., C280) - 1.)) -
                      (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                           std::pow(
                               std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2) *
                           std::exp(-0.00081290825 * std::sqrt(C288) /
                                    (std::pow(C278, 7. / 6.) * C313)) *
                           C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                           std::pow(C290, C283 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) *
                           C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) *
                           4. *
                           (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                                std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2) *
                                C283 * std::pow(C290, C283 - 1)) /
                           (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                  std::pow(
                                      std::pow(C278, 7. / 3.) *
                                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                    2.),
                                      2),
                              2)) *
                     C404 +
                 C405 * C278 *
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                     C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) * 4. *
                     (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                          std::pow(C278, 7. / 3.) *
                          std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                          std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) +
                      std::pow(std::pow(C278, 7. / 3.) *
                                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                               2) *
                          C283 * std::pow(C290, C283 - 1)) /
                     (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                      std::pow(
                          4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(
                                  std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2),
                          2))) -
                C402 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                         C313 +
                     C288 * C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                    std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) -
               C410 * C278 *
                   (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                    C313 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                        std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                   C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2))) *
                  C404 +
              ((C402 * C278 *
                    (C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                     (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 *
                          (C288 * C313 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313)) /
                               (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                           C313 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C288) /
                                        (std::pow(C278, 7. / 6.) * C313))) -
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                          std::pow(C278, 7. / 6.) * C313 / std::sqrt(C288)) /
                         std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                    (std::pow(C278, 7. / 3.) *
                     std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
                (C405 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                 C404 * C278 *
                     (std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                          C313 +
                      C288 * C313 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) /
                          (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                     std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                     std::pow(std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2))) +
               C410 * C278 *
                   ((std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) /
                         (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313) +
                     C313 *
                         (std::exp(-0.00081290825 * std::sqrt(C288) /
                                   (std::pow(C278, 7. / 6.) * C313)) *
                              0.00081290825 * std::pow(C278, 7. / 6.) / (2 * std::sqrt(C288)) +
                          0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                              -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) /
                              (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                        C288) /
                   (std::pow(C278, 7. / 3.) *
                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                  C402) +
             ((C402 * C278 *
                   ((std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) +
                    (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313 * C288 *
                         (C313 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C288) /
                                       (std::pow(C278, 7. / 6.) * C313)) *
                              0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                              std::pow(std::pow(C278, 7. / 6.) * C313, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C288) /
                                                   (std::pow(C278, 7. / 6.) * C313))) -
                     C288 * C313 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         2 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313, 2)) /
                   (std::pow(C278, 7. / 3.) *
                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.)) -
               (C405 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                C404 * C278 *
                    (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                     C313 *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                         std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                    C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1)) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2))) +
              C410 * C278 *
                  (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                       std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                   (std::pow(std::pow(C278, 7. / 6.) * C313, 2) *
                        (C313 * 0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C288) /
                                      (std::pow(C278, 7. / 6.) * C313)) *
                             0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                             std::pow(std::pow(C278, 7. / 6.) * C313, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                             0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.)) -
                    C313 *
                        std::exp(-0.00081290825 * std::sqrt(C288) /
                                 (std::pow(C278, 7. / 6.) * C313)) *
                        0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * 2 * C313 *
                        std::pow(C278, 14. / 6.)) /
                       std::pow(std::pow(C278, 7. / 6.) * C313, 4)) *
                  C288 /
                  (std::pow(C278, 7. / 3.) *
                   std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) *
                 C410) +
            (((C404 * C278 *
                   std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                   C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C290, C283 - 1) * 4. *
                   (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                        std::pow(C278, 7. / 3.) *
                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                        std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                    std::pow(std::pow(C278, 7. / 3.) *
                                 std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                             2) *
                        C283 * std::pow(C291, C283 - 1)) /
                   (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                            std::pow(
                                std::pow(C278, 7. / 3.) *
                                    std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                2),
                        2)) +
               C278 *
                   (((((0.01555 * C301 + 0.0007 * C303) - 0.0269) - 0.0048 * C294) - C314) * C282 *
                        (C282 - 1) * std::pow(C291, C282 - 2) / (2. * (std::pow(2., C280) - 1.)) -
                    (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                         std::pow(std::pow(C278, 7. / 3.) *
                                      std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C288) /
                                  (std::pow(C278, 7. / 6.) * C313)) *
                         C313 * C288 * std::pow(C278, 7. / 3.) * C283 * (C283 - 1) *
                         std::pow(C291, C283 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                         C313 * C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) *
                         4. *
                         (std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                              std::pow(C278, 7. / 3.) *
                              std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) * 2 *
                              std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) +
                          std::pow(
                              std::pow(C278, 7. / 3.) *
                                  std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                              2) *
                              C283 * std::pow(C291, C283 - 1)) /
                         (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                                std::pow(
                                    std::pow(C278, 7. / 3.) *
                                        std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) /
                                                  2.),
                                    2),
                            2)) *
                   C405) -
              C402 * C278 *
                  (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) *
                       C313 +
                   C288 * C313 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C288) /
                                (std::pow(C278, 7. / 6.) * C313)) /
                       (2 * std::sqrt(C288) * std::pow(C278, 7. / 6.) * C313)) *
                  std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                  (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                   std::pow(std::pow(C278, 7. / 3.) *
                                std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                            2))) -
             C410 * C278 *
                 (std::exp(-0.00081290825 * std::sqrt(C288) / (std::pow(C278, 7. / 6.) * C313)) +
                  C313 *
                      std::exp(-0.00081290825 * std::sqrt(C288) /
                               (std::pow(C278, 7. / 6.) * C313)) *
                      0.00081290825 * std::sqrt(C288) * std::pow(C278, 7. / 6.) /
                      std::pow(std::pow(C278, 7. / 6.) * C313, 2)) *
                 C288 * std::pow(C278, 7. / 3.) * C283 * std::pow(C291, C283 - 1) /
                 (4. * std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.) *
                  std::pow(std::pow(C278, 7. / 3.) *
                               std::sqrt((std::pow(C290, C283) + std::pow(C291, C283)) / 2.),
                           2))) *
                C405;
    }
}

} // namespace excgrid
