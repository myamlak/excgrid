// excgrid codegen source: the mPW91 (modified PW91) GGA exchange
// functional (Adamo-Barone).  Fresh authorship; the form is the published
// PW91 exchange with this functional's modified constants.
//
// The damping exponent, re-derived here rather than quoted at one digit
// count: PW91 damps with Exp(-alpha s^2), alpha = 100, where
// s = |grad rho| / (2 kF rho) and kF = (6 pi^2 rho)^(1/3) is the
// spin-density Fermi wavevector.  This kernel works in the variable
// x = |grad rho| / rho^(4/3), so s = x / (2 (6 pi^2)^(1/3)) and the damping
// is Exp(-100 / (4 (6 pi^2)^(2/3)) * x^2).  The exponent below is that
// constant - 1.6455307846020557... - carried at double precision, so no
// digit of it is a free parameter.  Regenerate with tools/regenerate.py.

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

    const double C715 = 4. * Pi;
    const double C716 = 36. * Pi;
    const double C717 = rhoA + 1e-16;
    const double C718 = rhoB + 1e-16;
    const double C719 = -5. / 3.;
    const double C720 = 1. / 3.;
    const double C721 = 4. / 3.;
    const double C722 = std::sqrt(sigmaAa);
    const double C723 = std::sqrt(sigmaBb);
    const double C724 = std::pow(3., C721);
    const double C725 = std::pow(C715, C720);
    const double C726 = std::pow(C716, C719);
    const double C727 = std::pow(C717, C721);
    const double C728 = std::pow(C718, C721);
    const double C729 = std::pow(rhoA, C721);
    const double C730 = std::pow(rhoB, C721);
    const double C731 = 5. * C726;
    const double C732 = C722 / C727;
    const double C733 = C723 / C728;
    const double C734 = 0.00426 - C731;
    const double C735 = std::pow(C732, 2);
    const double C736 = std::pow(C732, 3.72);
    const double C737 = std::pow(C733, 2);
    const double C738 = std::pow(C733, 3.72);

    result.exc = -(
        (0.9305257363491 * (C729 + C730) +
         C729 *
             ((0.00426 * C735 - C734 * C735 * std::exp(-1.6455307846 * C735)) - 0.000001 * C736) /
             ((std::log(C732 + std::sqrt(C735 + 1.)) * 0.02556 * C722 / C727 +
               0.000002 * C736 * C725 / C724) +
              1.)) +
        C730 * ((0.00426 * C737 - C734 * C737 * std::exp(-1.6455307846 * C737)) - 0.000001 * C738) /
            ((std::log(C733 + std::sqrt(C737 + 1.)) * 0.02556 * C723 / C728 +
              0.000002 * C738 * C725 / C724) +
             1.));

    const double C743 = 0.000001 * C736;
    const double C744 = 0.00426 * C735;
    const double C745 = 0.02556 * C722;
    const double C746 = 1.6455307846 * C735;
    const double C747 = C734 * C735;
    const double C748 = C736 * C725;
    const double C749 = C735 + 1.;
    const double C750 = 8. / 3.;
    const double C751 = std::pow(C717, C720);
    const double C752 = std::pow(C732, 2.72);
    const double C753 = std::pow(rhoA, C720);
    const double C754 = 0.000002 * C748;
    const double C755 = 4. * C751;
    const double C756 = C751 * sigmaAa;
    const double C757 = -C746;
    const double C758 = std::sqrt(C749);
    const double C759 = std::pow(C717, C750);
    const double C760 = -8. * C756;
    const double C761 = 3. * C759;
    const double C762 = C722 * C755;
    const double C763 = C732 + C758;
    const double C764 = C754 / C724;
    const double C765 = std::exp(C757);
    const double C766 = -3.72 * C762;
    const double C767 = C727 * C761;
    const double C768 = C747 * C765;
    const double C769 = std::log(C763);
    const double C770 = C752 * C766;
    const double C771 = C769 * C745;
    const double C772 = C744 - C768;
    const double C773 = C772 - C743;
    const double C774 = C771 / C727;
    const double C775 = C774 + C764;
    const double C776 = C775 + 1.;

    result.vrhoA =
        -(3.7221029453964 * C753 / 3. +
          (C776 *
               (C729 * ((-0.03408 * C756 / C767 - (C765 * C734 * C760 / C767 -
                                                   C747 * C765 * -0.131642462768e2 * C756 / C767)) -
                        0.000001 * C770 / C761) +
                C773 * 4. * C753 / 3.) -
           C729 * C773 *
               ((C727 * 0.02556 * C722 * (C760 / (C767 * 2 * C758) - C762 / C761) / C763 -
                 C771 * C755 / 3.) /
                    C759 +
                0.000002 * C725 * C770 / (3. * C759 * C724))) /
              std::pow(C776, 2));

    const double C778 = 0.000001 * C738;
    const double C779 = 0.00426 * C737;
    const double C780 = 0.02556 * C723;
    const double C781 = 1.6455307846 * C737;
    const double C782 = C734 * C737;
    const double C783 = C738 * C725;
    const double C784 = C737 + 1.;
    const double C785 = 8. / 3.;
    const double C786 = std::pow(C718, C720);
    const double C787 = std::pow(C733, 2.72);
    const double C788 = std::pow(rhoB, C720);
    const double C789 = 0.000002 * C783;
    const double C790 = 4. * C786;
    const double C791 = C786 * sigmaBb;
    const double C792 = -C781;
    const double C793 = std::sqrt(C784);
    const double C794 = std::pow(C718, C785);
    const double C795 = -8. * C791;
    const double C796 = 3. * C794;
    const double C797 = C723 * C790;
    const double C798 = C733 + C793;
    const double C799 = C789 / C724;
    const double C800 = std::exp(C792);
    const double C801 = -3.72 * C797;
    const double C802 = C728 * C796;
    const double C803 = C782 * C800;
    const double C804 = std::log(C798);
    const double C805 = C787 * C801;
    const double C806 = C804 * C780;
    const double C807 = C779 - C803;
    const double C808 = C807 - C778;
    const double C809 = C806 / C728;
    const double C810 = C809 + C799;
    const double C811 = C810 + 1.;

    result.vrhoB =
        -(3.7221029453964 * C788 / 3. +
          (C811 *
               (C730 * ((-0.03408 * C791 / C802 - (C800 * C734 * C795 / C802 -
                                                   C782 * C800 * -0.131642462768e2 * C791 / C802)) -
                        0.000001 * C805 / C796) +
                C808 * 4. * C788 / 3.) -
           C730 * C808 *
               ((C728 * 0.02556 * C723 * (C795 / (C802 * 2 * C793) - C797 / C796) / C798 -
                 C806 * C790 / 3.) /
                    C794 +
                0.000002 * C725 * C805 / (3. * C794 * C724))) /
              std::pow(C811, 2));

    const double C813 = 4. * Pi;
    const double C814 = 36. * Pi;
    const double C815 = rhoA + 1e-16;
    const double C816 = -5. / 3.;
    const double C817 = 1. / 3.;
    const double C818 = 4. / 3.;
    const double C819 = 8. / 3.;
    const double C820 = std::sqrt(sigmaAa);
    const double C821 = 0.02556 * C820;
    const double C822 = std::pow(3., C818);
    const double C823 = std::pow(C813, C817);
    const double C824 = std::pow(C814, C816);
    const double C825 = std::pow(C815, C818);
    const double C826 = std::pow(C815, C819);
    const double C827 = std::pow(rhoA, C818);
    const double C828 = 5. * C824;
    const double C829 = C820 * C825;
    const double C830 = C820 * C826;
    const double C831 = C820 / C825;
    const double C832 = 2 * C829;
    const double C833 = 0.00426 - C828;
    const double C834 = std::pow(C831, 2);
    const double C835 = std::pow(C831, 2.72);
    const double C836 = std::pow(C831, 3.72);
    const double C837 = 1.6455307846 * C834;
    const double C838 = C833 * C834;
    const double C839 = C836 * C823;
    const double C840 = C834 + 1.;
    const double C841 = 0.000002 * C839;
    const double C842 = -C837;
    const double C843 = std::sqrt(C840);
    const double C844 = C831 + C843;
    const double C845 = C841 / C822;
    const double C846 = std::exp(C842);
    const double C847 = std::log(C844);
    const double C848 = C847 * C821;
    const double C849 = C848 / C825;
    const double C850 = C849 + C845;
    const double C851 = C850 + 1.;

    result.vsigmaAa = -((C851 * C827 *
                             ((0.00426 * C820 / C830 - (C846 * C833 * C820 / C830 -
                                                        C838 * C846 * 1.6455307846 * C820 / C830)) -
                              0.00000372 * C835 / C832) -
                         C827 * ((0.00426 * C834 - C838 * C846) - 0.000001 * C836) *
                             ((0.02556 * C847 / (2 * C820) +
                               0.02556 * C820 * (1 / C832 + C820 / (C830 * 2 * C843)) / C844) /
                                  C825 +
                              0.000002 * C823 * 3.72 * C835 / (2 * C829 * C822))) /
                            std::pow(C851, 2) +
                        0.);

    result.vsigmaAb = 0.;

    const double C854 = 4. * Pi;
    const double C855 = 36. * Pi;
    const double C856 = rhoB + 1e-16;
    const double C857 = -5. / 3.;
    const double C858 = 1. / 3.;
    const double C859 = 4. / 3.;
    const double C860 = 8. / 3.;
    const double C861 = std::sqrt(sigmaBb);
    const double C862 = 0.02556 * C861;
    const double C863 = std::pow(3., C859);
    const double C864 = std::pow(C854, C858);
    const double C865 = std::pow(C855, C857);
    const double C866 = std::pow(C856, C859);
    const double C867 = std::pow(C856, C860);
    const double C868 = std::pow(rhoB, C859);
    const double C869 = 5. * C865;
    const double C870 = C861 * C866;
    const double C871 = C861 * C867;
    const double C872 = C861 / C866;
    const double C873 = 2 * C870;
    const double C874 = 0.00426 - C869;
    const double C875 = std::pow(C872, 2);
    const double C876 = std::pow(C872, 2.72);
    const double C877 = std::pow(C872, 3.72);
    const double C878 = 1.6455307846 * C875;
    const double C879 = C874 * C875;
    const double C880 = C877 * C864;
    const double C881 = C875 + 1.;
    const double C882 = 0.000002 * C880;
    const double C883 = -C878;
    const double C884 = std::sqrt(C881);
    const double C885 = C872 + C884;
    const double C886 = C882 / C863;
    const double C887 = std::exp(C883);
    const double C888 = std::log(C885);
    const double C889 = C888 * C862;
    const double C890 = C889 / C866;
    const double C891 = C890 + C886;
    const double C892 = C891 + 1.;

    result.vsigmaBb = -((C892 * C868 *
                             ((0.00426 * C861 / C871 - (C887 * C874 * C861 / C871 -
                                                        C879 * C887 * 1.6455307846 * C861 / C871)) -
                              0.00000372 * C876 / C873) -
                         C868 * ((0.00426 * C875 - C879 * C887) - 0.000001 * C877) *
                             ((0.02556 * C888 / (2 * C861) +
                               0.02556 * C861 * (1 / C873 + C861 / (C871 * 2 * C884)) / C885) /
                                  C866 +
                              0.000002 * C864 * 3.72 * C876 / (2 * C870 * C863))) /
                            std::pow(C892, 2) +
                        0.);

    return result;
}

void MPw91ExchangeSecondDerivatives(double rhoA,
                                    double rhoB,
                                    double sigmaAa,
                                    double sigmaAb,
                                    double sigmaBb,
                                    PointSecondDerivativeMatrix& matrix) {
    (void)sigmaAb;

    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;

    if (rhoA + rhoB < 2. * std::numeric_limits<double>::epsilon())
    {
        return;
    }

    const double C632 = 4. * Pi;
    const double C633 = 36. * Pi;
    const double C634 = rhoB + 1e-16;
    const double C635 = -5. / 3.;
    const double C636 = 1. / 3.;
    const double C637 = 4. / 3.;
    const double C638 = 8. / 3.;
    const double C639 = std::sqrt(sigmaBb);
    const double C640 = 0.00426 * C639;
    const double C641 = 0.02556 * C639;
    const double C642 = 1.6455307846 * C639;
    const double C643 = 2 * C639;
    const double C644 = std::pow(3., C637);
    const double C645 = std::pow(C632, C636);
    const double C646 = std::pow(C633, C635);
    const double C647 = std::pow(C634, C637);
    const double C648 = std::pow(C634, C638);
    const double C649 = std::pow(rhoB, C637);
    const double C650 = 5. * C646;
    const double C651 = C639 * C647;
    const double C652 = C639 * C648;
    const double C653 = C639 / C647;
    const double C654 = 2 * C651;
    const double C655 = C651 * C644;
    const double C656 = 0.00426 - C650;
    const double C657 = C640 / C652;
    const double C658 = std::pow(C652, 2);
    const double C659 = std::pow(C653, 1.72);
    const double C660 = std::pow(C653, 2);
    const double C661 = std::pow(C653, 2.72);
    const double C662 = std::pow(C653, 3.72);
    const double C663 = 0.000001 * C662;
    const double C664 = 0.00000372 * C661;
    const double C665 = 0.00426 * C660;
    const double C666 = 1.6455307846 * C660;
    const double C667 = 2 * C655;
    const double C668 = 3.72 * C661;
    const double C669 = C656 * C639;
    const double C670 = C656 * C660;
    const double C671 = C662 * C645;
    const double C672 = C660 + 1.;
    const double C673 = 1 / C654;
    const double C674 = std::pow(C654, 2);
    const double C675 = 0.000002 * C671;
    const double C676 = C645 * C668;
    const double C677 = -C666;
    const double C678 = C664 / C654;
    const double C679 = std::sqrt(C672);
    const double C680 = 0.000002 * C676;
    const double C681 = 2 * C679;
    const double C682 = C653 + C679;
    const double C683 = C675 / C644;
    const double C684 = std::exp(C677);
    const double C685 = C652 * C681;
    const double C686 = C670 * C684;
    const double C687 = C684 * C642;
    const double C688 = C684 * C669;
    const double C689 = C680 / C667;
    const double C690 = std::log(C682);
    const double C691 = 0.02556 * C690;
    const double C692 = C670 * C687;
    const double C693 = C690 * C641;
    const double C694 = C665 - C686;
    const double C695 = C639 / C685;
    const double C696 = C688 / C652;
    const double C697 = C673 + C695;
    const double C698 = C694 - C663;
    const double C699 = C691 / C643;
    const double C700 = C692 / C652;
    const double C701 = C693 / C647;
    const double C702 = C639 * C697;
    const double C703 = C649 * C698;
    const double C704 = C701 + C683;
    const double C705 = C696 - C700;
    const double C706 = 0.02556 * C702;
    const double C707 = C704 + 1.;
    const double C708 = C657 - C705;
    const double C709 = C708 - C678;
    const double C710 = C706 / C682;
    const double C711 = C649 * C709;
    const double C712 = C699 + C710;
    const double C713 = C712 / C647;
    const double C714 = C713 + C689;
    matrix.upper[14] =
        -(std::pow(C707, 2) *
              ((C707 * C649 *
                    (((C652 * (C670 * (1.6455307846 * C684 / C643 -
                                       1.6455307846 * C639 * C687 / C652) +
                               C687 * C669 / C652) -
                       C692 * C648 / C643) /
                          C658 -
                      (C652 * (C684 * C656 / C643 - C669 * C687 / C652) - C688 * C648 / C643) /
                          C658) -
                     (C651 * 0.0000101184 * C659 / C651 - 0.00000372 * C661 * C647 / C639) / C674) +
                C714 * C711) -
               (C703 * (((2 * C639 * 0.02556 * C697 / C682 - C691 / C639) / std::pow(C643, 2) +
                         (C682 * 0.02556 *
                              (C639 * ((C685 / C643 -
                                        C639 * (C652 * C643 / C685 + C679 * C648 / C639)) /
                                           std::pow(C685, 2) -
                                       C647 / (C639 * C674)) +
                               C697 / C643) -
                          0.02556 * C639 * std::pow(C697, 2)) /
                             std::pow(C682, 2)) /
                            C647 +
                        (C655 * 0.000002 * C645 * 10.1184 * C659 / C651 -
                         0.000002 * C676 * C644 * C647 / C639) /
                            std::pow(C667, 2)) +
                C711 * C714)) -
          (C707 * C711 - C703 * C714) * 2 * C714 * C707) /
        std::pow(C707, 4);

    matrix.upper[13] = 0;

    matrix.upper[12] = 0;

    matrix.upper[11] = 0;

    matrix.upper[10] = 0;

    const double C549 = 4. * Pi;
    const double C550 = 36. * Pi;
    const double C551 = rhoA + 1e-16;
    const double C552 = -5. / 3.;
    const double C553 = 1. / 3.;
    const double C554 = 4. / 3.;
    const double C555 = 8. / 3.;
    const double C556 = std::sqrt(sigmaAa);
    const double C557 = 0.00426 * C556;
    const double C558 = 0.02556 * C556;
    const double C559 = 1.6455307846 * C556;
    const double C560 = 2 * C556;
    const double C561 = std::pow(3., C554);
    const double C562 = std::pow(C549, C553);
    const double C563 = std::pow(C550, C552);
    const double C564 = std::pow(C551, C554);
    const double C565 = std::pow(C551, C555);
    const double C566 = std::pow(rhoA, C554);
    const double C567 = 5. * C563;
    const double C568 = C556 * C564;
    const double C569 = C556 * C565;
    const double C570 = C556 / C564;
    const double C571 = 2 * C568;
    const double C572 = C568 * C561;
    const double C573 = 0.00426 - C567;
    const double C574 = C557 / C569;
    const double C575 = std::pow(C569, 2);
    const double C576 = std::pow(C570, 1.72);
    const double C577 = std::pow(C570, 2);
    const double C578 = std::pow(C570, 2.72);
    const double C579 = std::pow(C570, 3.72);
    const double C580 = 0.000001 * C579;
    const double C581 = 0.00000372 * C578;
    const double C582 = 0.00426 * C577;
    const double C583 = 1.6455307846 * C577;
    const double C584 = 2 * C572;
    const double C585 = 3.72 * C578;
    const double C586 = C573 * C556;
    const double C587 = C573 * C577;
    const double C588 = C579 * C562;
    const double C589 = C577 + 1.;
    const double C590 = 1 / C571;
    const double C591 = std::pow(C571, 2);
    const double C592 = 0.000002 * C588;
    const double C593 = C562 * C585;
    const double C594 = -C583;
    const double C595 = C581 / C571;
    const double C596 = std::sqrt(C589);
    const double C597 = 0.000002 * C593;
    const double C598 = 2 * C596;
    const double C599 = C570 + C596;
    const double C600 = C592 / C561;
    const double C601 = std::exp(C594);
    const double C602 = C569 * C598;
    const double C603 = C587 * C601;
    const double C604 = C601 * C559;
    const double C605 = C601 * C586;
    const double C606 = C597 / C584;
    const double C607 = std::log(C599);
    const double C608 = 0.02556 * C607;
    const double C609 = C587 * C604;
    const double C610 = C607 * C558;
    const double C611 = C582 - C603;
    const double C612 = C556 / C602;
    const double C613 = C605 / C569;
    const double C614 = C590 + C612;
    const double C615 = C611 - C580;
    const double C616 = C608 / C560;
    const double C617 = C609 / C569;
    const double C618 = C610 / C564;
    const double C619 = C556 * C614;
    const double C620 = C566 * C615;
    const double C621 = C618 + C600;
    const double C622 = C613 - C617;
    const double C623 = 0.02556 * C619;
    const double C624 = C621 + 1.;
    const double C625 = C574 - C622;
    const double C626 = C625 - C595;
    const double C627 = C623 / C599;
    const double C628 = C566 * C626;
    const double C629 = C616 + C627;
    const double C630 = C629 / C564;
    const double C631 = C630 + C606;
    matrix.upper[9] =
        -(std::pow(C624, 2) *
              ((C624 * C566 *
                    (((C569 * (C587 * (1.6455307846 * C601 / C560 -
                                       1.6455307846 * C556 * C604 / C569) +
                               C604 * C586 / C569) -
                       C609 * C565 / C560) /
                          C575 -
                      (C569 * (C601 * C573 / C560 - C586 * C604 / C569) - C605 * C565 / C560) /
                          C575) -
                     (C568 * 0.0000101184 * C576 / C568 - 0.00000372 * C578 * C564 / C556) / C591) +
                C631 * C628) -
               (C620 * (((2 * C556 * 0.02556 * C614 / C599 - C608 / C556) / std::pow(C560, 2) +
                         (C599 * 0.02556 *
                              (C556 * ((C602 / C560 -
                                        C556 * (C569 * C560 / C602 + C596 * C565 / C556)) /
                                           std::pow(C602, 2) -
                                       C564 / (C556 * C591)) +
                               C614 / C560) -
                          0.02556 * C556 * std::pow(C614, 2)) /
                             std::pow(C599, 2)) /
                            C564 +
                        (C572 * 0.000002 * C562 * 10.1184 * C576 / C568 -
                         0.000002 * C593 * C561 * C564 / C556) /
                            std::pow(C584, 2)) +
                C628 * C631)) -
          (C624 * C628 - C620 * C631) * 2 * C631 * C624) /
        std::pow(C624, 4);

    const double C427 = 4. * Pi;
    const double C428 = 36. * Pi;
    const double C429 = rhoB + 1e-16;
    const double C430 = -5. / 3.;
    const double C431 = 1. / 3.;
    const double C432 = 4. / 3.;
    const double C433 = 5. / 3.;
    const double C434 = 8. / 3.;
    const double C435 = std::sqrt(sigmaBb);
    const double C436 = 0.00426 * C435;
    const double C437 = 0.02556 * C435;
    const double C438 = 1.6455307846 * C435;
    const double C439 = 2 * C435;
    const double C440 = std::pow(3., C432);
    const double C441 = std::pow(C427, C431);
    const double C442 = std::pow(C428, C430);
    const double C443 = std::pow(C429, C431);
    const double C444 = std::pow(C429, C432);
    const double C445 = std::pow(C429, C433);
    const double C446 = std::pow(C429, C434);
    const double C447 = std::pow(rhoB, C431);
    const double C448 = std::pow(rhoB, C432);
    const double C449 = 3. * C446;
    const double C450 = 4. * C443;
    const double C451 = 4. * C447;
    const double C452 = 5. * C442;
    const double C453 = 8. * C445;
    const double C454 = C435 * C444;
    const double C455 = C435 * C446;
    const double C456 = C443 * sigmaBb;
    const double C457 = C446 * C440;
    const double C458 = C435 / C444;
    const double C459 = -0.131642462768e2 * C456;
    const double C460 = -8. * C456;
    const double C461 = 2 * C454;
    const double C462 = 3. * C457;
    const double C463 = C435 * C450;
    const double C464 = C435 * C453;
    const double C465 = C444 * C449;
    const double C466 = C454 * C440;
    const double C467 = 0.00426 - C452;
    const double C468 = C436 / C455;
    const double C469 = std::pow(C455, 2);
    const double C470 = std::pow(C458, 1.72);
    const double C471 = std::pow(C458, 2);
    const double C472 = std::pow(C458, 2.72);
    const double C473 = std::pow(C458, 3.72);
    const double C474 = -3.72 * C463;
    const double C475 = -2.72 * C463;
    const double C476 = 0.000001 * C473;
    const double C477 = 0.00000372 * C472;
    const double C478 = 0.00426 * C471;
    const double C479 = 1.6455307846 * C471;
    const double C480 = 2 * C463;
    const double C481 = 2 * C466;
    const double C482 = 3.72 * C472;
    const double C483 = C467 * C435;
    const double C484 = C467 * C460;
    const double C485 = C467 * C471;
    const double C486 = C473 * C441;
    const double C487 = C471 + 1.;
    const double C488 = 1 / C461;
    const double C489 = C463 / C449;
    const double C490 = std::pow(C461, 2);
    const double C491 = 0.000002 * C486;
    const double C492 = C441 * C482;
    const double C493 = C470 * C475;
    const double C494 = C472 * C474;
    const double C495 = -C479;
    const double C496 = C477 / C461;
    const double C497 = std::sqrt(C487);
    const double C498 = 0.000002 * C492;
    const double C499 = 2 * C497;
    const double C500 = C441 * C494;
    const double C501 = C458 + C497;
    const double C502 = C491 / C440;
    const double C503 = std::exp(C495);
    const double C504 = 0.000002 * C500;
    const double C505 = C455 * C499;
    const double C506 = C465 * C499;
    const double C507 = C485 * C503;
    const double C508 = C503 * C438;
    const double C509 = C503 * C459;
    const double C510 = C503 * C483;
    const double C511 = C498 / C481;
    const double C512 = std::log(C501);
    const double C513 = 0.02556 * C512;
    const double C514 = C485 * C508;
    const double C515 = C512 * C437;
    const double C516 = C478 - C507;
    const double C517 = C435 / C505;
    const double C518 = C460 / C506;
    const double C519 = C504 / C462;
    const double C520 = C510 / C455;
    const double C521 = C515 * C450;
    const double C522 = C488 + C517;
    const double C523 = C516 - C476;
    const double C524 = C518 - C489;
    const double C525 = C513 / C439;
    const double C526 = C514 / C455;
    const double C527 = C515 / C444;
    const double C528 = C435 * C522;
    const double C529 = C435 * C524;
    const double C530 = C448 * C523;
    const double C531 = C527 + C502;
    const double C532 = C520 - C526;
    const double C533 = C521 / 3.;
    const double C534 = 0.02556 * C528;
    const double C535 = 0.02556 * C529;
    const double C536 = C531 + 1.;
    const double C537 = C468 - C532;
    const double C538 = C444 * C535;
    const double C539 = C537 - C496;
    const double C540 = C534 / C501;
    const double C541 = C448 * C539;
    const double C542 = C525 + C540;
    const double C543 = C538 / C501;
    const double C544 = C543 - C533;
    const double C545 = C542 / C444;
    const double C546 = C545 + C511;
    const double C547 = C544 / C446;
    const double C548 = C547 + C519;
    matrix.upper[8] =
        -(std::pow(C536, 2) *
              ((C536 *
                    (C448 * (((-0.03408 * C445 * sigmaBb / 3.) / C469 -
                              ((-(C455 * C483 * C509 / C465 + C510 * C464 / 3.)) / C469 -
                               (C455 * (C508 * C484 / C465 -
                                        C485 * 1.6455307846 * C435 * C509 / C465) -
                                C514 * C464 / 3.) /
                                   C469)) -
                             (2 * C454 * 0.00000372 * C493 / C449 - 0.00000372 * C472 * C480 / 3.) /
                                 C490) +
                     C539 * C451 / 3.) +
                C548 * C541) -
               (C530 *
                    ((C444 * (0.02556 * C524 / (C501 * C439) +
                              (C501 * 0.02556 * C435 *
                                   ((-C435 * (C455 * -16. * C456 / C506 + 2 * C497 * C464 / 3.)) /
                                        std::pow(C505, 2) -
                                    C480 / (3. * C490)) -
                               0.02556 * C528 * C524) /
                                  std::pow(C501, 2)) -
                      C542 * C450 / 3.) /
                         C446 +
                     (2 * C466 * 0.000002 * C441 * 3.72 * C493 / C449 -
                      0.000002 * C492 * 2 * C440 * C463 / 3.) /
                         std::pow(C481, 2)) +
                (C448 * ((-0.03408 * C456 / C465 - (C503 * C484 / C465 - C485 * C509 / C465)) -
                         0.000001 * C494 / C449) +
                 C523 * C451 / 3.) *
                    C546)) -
          (C536 * C541 - C530 * C546) * 2 * C548 * C536) /
        std::pow(C536, 4);

    matrix.upper[7] = 0;

    matrix.upper[6] = 0;

    const double C309 = 4. * Pi;
    const double C310 = 36. * Pi;
    const double C311 = rhoB + 1e-16;
    const double C312 = -5. / 3.;
    const double C313 = -2. / 3.;
    const double C314 = 1. / 3.;
    const double C315 = 4. / 3.;
    const double C316 = 5. / 3.;
    const double C317 = 8. / 3.;
    const double C318 = std::sqrt(sigmaBb);
    const double C319 = 0.02556 * C318;
    const double C320 = std::pow(3., C315);
    const double C321 = std::pow(C309, C314);
    const double C322 = std::pow(C310, C312);
    const double C323 = std::pow(C311, C313);
    const double C324 = std::pow(C311, C314);
    const double C325 = std::pow(C311, C315);
    const double C326 = std::pow(C311, C316);
    const double C327 = std::pow(C311, C317);
    const double C328 = std::pow(rhoB, C313);
    const double C329 = std::pow(rhoB, C314);
    const double C330 = std::pow(rhoB, C315);
    const double C331 = 3. * C327;
    const double C332 = 4. * C323;
    const double C333 = 4. * C324;
    const double C334 = 4. * C329;
    const double C335 = 5. * C322;
    const double C336 = 8. * C326;
    const double C337 = 24. * C326;
    const double C338 = C324 * sigmaBb;
    const double C339 = C327 * C320;
    const double C340 = sigmaBb * C323;
    const double C341 = C318 / C325;
    const double C342 = -0.131642462768e2 * C338;
    const double C343 = -8. * C338;
    const double C344 = -8. * C340;
    const double C345 = -0.03408 * C338;
    const double C346 = 3. * C339;
    const double C347 = C318 * C332;
    const double C348 = C318 * C333;
    const double C349 = C325 * C331;
    const double C350 = C325 * C337;
    const double C351 = C327 * C333;
    const double C352 = 0.00426 - C335;
    const double C353 = std::pow(C331, 2);
    const double C354 = std::pow(C341, 1.72);
    const double C355 = std::pow(C341, 2);
    const double C356 = std::pow(C341, 2.72);
    const double C357 = std::pow(C341, 3.72);
    const double C358 = -3.72 * C347;
    const double C359 = -3.72 * C348;
    const double C360 = -2.72 * C348;
    const double C361 = 0.000001 * C357;
    const double C362 = 0.00426 * C355;
    const double C363 = 1.6455307846 * C355;
    const double C364 = 3. * C351;
    const double C365 = C352 * C343;
    const double C366 = C352 * C355;
    const double C367 = C357 * C321;
    const double C368 = C355 + 1.;
    const double C369 = C345 / C349;
    const double C370 = C348 / C331;
    const double C371 = std::pow(C349, 2);
    const double C372 = 0.000002 * C367;
    const double C373 = C354 * C360;
    const double C374 = C356 * C358;
    const double C375 = C356 * C359;
    const double C376 = C350 + C364;
    const double C377 = -C363;
    const double C378 = std::sqrt(C368);
    const double C379 = 0.000001 * C375;
    const double C380 = 2 * C378;
    const double C381 = C321 * C375;
    const double C382 = C348 * C373;
    const double C383 = C341 + C378;
    const double C384 = C372 / C320;
    const double C385 = C374 / 3.;
    const double C386 = std::exp(C377);
    const double C387 = -3.72 * C382;
    const double C388 = 0.000002 * C381;
    const double C389 = C349 * C380;
    const double C390 = C366 * C386;
    const double C391 = C386 * C342;
    const double C392 = C386 * C365;
    const double C393 = C379 / C331;
    const double C394 = std::log(C383);
    const double C395 = C366 * C391;
    const double C396 = C394 * C319;
    const double C397 = C362 - C390;
    const double C398 = C343 / C389;
    const double C399 = C387 / C331;
    const double C400 = C388 / C346;
    const double C401 = C392 / C349;
    const double C402 = C396 * C333;
    const double C403 = C385 + C399;
    const double C404 = C397 - C361;
    const double C405 = C398 - C370;
    const double C406 = C395 / C349;
    const double C407 = C396 / C325;
    const double C408 = C318 * C405;
    const double C409 = C330 * C404;
    const double C410 = C404 * C334;
    const double C411 = C407 + C384;
    const double C412 = C401 - C406;
    const double C413 = C402 / 3.;
    const double C414 = 0.02556 * C408;
    const double C415 = C411 + 1.;
    const double C416 = C369 - C412;
    const double C417 = C410 / 3.;
    const double C418 = C325 * C414;
    const double C419 = C416 - C393;
    const double C420 = C330 * C419;
    const double C421 = C419 * C334;
    const double C422 = C418 / C383;
    const double C423 = C420 + C417;
    const double C424 = C422 - C413;
    const double C425 = C424 / C327;
    const double C426 = C425 + C400;
    matrix.upper[5] = -(
        3.7221029453964 * C328 / 9. +
        (std::pow(C415, 2) *
             ((C415 *
                   ((C330 * (((C349 * -0.03408 * C340 / 3. - -0.03408 * C338 * C376 / 3.) / C371 -
                              ((C349 * (C386 * C352 * C344 / 3. - C365 * C391 / C349) -
                                C392 * C376 / 3.) /
                                   C371 -
                               (C349 * (C366 * (C386 * -0.131642462768e2 * C340 / 3. -
                                                -0.131642462768e2 * C338 * C391 / C349) +
                                        C391 * C365 / C349) -
                                C395 * C376 / 3.) /
                                   C371)) -
                             (3. * C327 * 0.000001 * C403 - 0.000001 * C375 * C337 / 3.) / C353) +
                     C421 / 3.) +
                    (C404 * 4. * C328 / 3. + C421) / 3.) +
               C426 * C423) -
              (C409 *
                   ((C327 * ((C383 * (C325 * 0.02556 * C318 *
                                          ((C389 * C344 / 3. - -8. * C338 *
                                                                   (C349 * -16. * C338 / C389 +
                                                                    2 * C378 * C376 / 3.)) /
                                               std::pow(C389, 2) -
                                           (3. * C327 * C347 / 3. - C348 * C337 / 3.) / C353) +
                                      0.02556 * C408 * C333 / 3.) -
                              C418 * C405) /
                                 std::pow(C383, 2) -
                             (C396 * C332 / 3. + 4. * C324 * C414 / C383) / 3.) -
                     C424 * C336 / 3.) /
                        std::pow(C311, 16. / 3.) +
                    (3. * C339 * 0.000002 * C321 * C403 - 0.000002 * C381 * 3. * C320 * C336 / 3.) /
                        std::pow(C346, 2)) +
               C423 * C426)) -
         (C415 * C423 - C409 * C426) * 2 * C426 * C415) /
            std::pow(C415, 4));

    matrix.upper[4] = 0;

    matrix.upper[3] = 0;

    const double C187 = 4. * Pi;
    const double C188 = 36. * Pi;
    const double C189 = rhoA + 1e-16;
    const double C190 = -5. / 3.;
    const double C191 = 1. / 3.;
    const double C192 = 4. / 3.;
    const double C193 = 5. / 3.;
    const double C194 = 8. / 3.;
    const double C195 = std::sqrt(sigmaAa);
    const double C196 = 0.00426 * C195;
    const double C197 = 0.02556 * C195;
    const double C198 = 1.6455307846 * C195;
    const double C199 = 2 * C195;
    const double C200 = std::pow(3., C192);
    const double C201 = std::pow(C187, C191);
    const double C202 = std::pow(C188, C190);
    const double C203 = std::pow(C189, C191);
    const double C204 = std::pow(C189, C192);
    const double C205 = std::pow(C189, C193);
    const double C206 = std::pow(C189, C194);
    const double C207 = std::pow(rhoA, C191);
    const double C208 = std::pow(rhoA, C192);
    const double C209 = 3. * C206;
    const double C210 = 4. * C203;
    const double C211 = 4. * C207;
    const double C212 = 5. * C202;
    const double C213 = 8. * C205;
    const double C214 = C195 * C204;
    const double C215 = C195 * C206;
    const double C216 = C203 * sigmaAa;
    const double C217 = C206 * C200;
    const double C218 = C195 / C204;
    const double C219 = -0.131642462768e2 * C216;
    const double C220 = -8. * C216;
    const double C221 = 2 * C214;
    const double C222 = 3. * C217;
    const double C223 = C195 * C210;
    const double C224 = C195 * C213;
    const double C225 = C204 * C209;
    const double C226 = C214 * C200;
    const double C227 = 0.00426 - C212;
    const double C228 = C196 / C215;
    const double C229 = std::pow(C215, 2);
    const double C230 = std::pow(C218, 1.72);
    const double C231 = std::pow(C218, 2);
    const double C232 = std::pow(C218, 2.72);
    const double C233 = std::pow(C218, 3.72);
    const double C234 = -3.72 * C223;
    const double C235 = -2.72 * C223;
    const double C236 = 0.000001 * C233;
    const double C237 = 0.00000372 * C232;
    const double C238 = 0.00426 * C231;
    const double C239 = 1.6455307846 * C231;
    const double C240 = 2 * C223;
    const double C241 = 2 * C226;
    const double C242 = 3.72 * C232;
    const double C243 = C227 * C195;
    const double C244 = C227 * C220;
    const double C245 = C227 * C231;
    const double C246 = C233 * C201;
    const double C247 = C231 + 1.;
    const double C248 = 1 / C221;
    const double C249 = C223 / C209;
    const double C250 = std::pow(C221, 2);
    const double C251 = 0.000002 * C246;
    const double C252 = C201 * C242;
    const double C253 = C230 * C235;
    const double C254 = C232 * C234;
    const double C255 = -C239;
    const double C256 = C237 / C221;
    const double C257 = std::sqrt(C247);
    const double C258 = 0.000002 * C252;
    const double C259 = 2 * C257;
    const double C260 = C201 * C254;
    const double C261 = C218 + C257;
    const double C262 = C251 / C200;
    const double C263 = std::exp(C255);
    const double C264 = 0.000002 * C260;
    const double C265 = C215 * C259;
    const double C266 = C225 * C259;
    const double C267 = C245 * C263;
    const double C268 = C263 * C198;
    const double C269 = C263 * C219;
    const double C270 = C263 * C243;
    const double C271 = C258 / C241;
    const double C272 = std::log(C261);
    const double C273 = 0.02556 * C272;
    const double C274 = C245 * C268;
    const double C275 = C272 * C197;
    const double C276 = C238 - C267;
    const double C277 = C195 / C265;
    const double C278 = C220 / C266;
    const double C279 = C264 / C222;
    const double C280 = C270 / C215;
    const double C281 = C275 * C210;
    const double C282 = C248 + C277;
    const double C283 = C276 - C236;
    const double C284 = C278 - C249;
    const double C285 = C273 / C199;
    const double C286 = C274 / C215;
    const double C287 = C275 / C204;
    const double C288 = C195 * C282;
    const double C289 = C195 * C284;
    const double C290 = C208 * C283;
    const double C291 = C287 + C262;
    const double C292 = C280 - C286;
    const double C293 = C281 / 3.;
    const double C294 = 0.02556 * C288;
    const double C295 = 0.02556 * C289;
    const double C296 = C291 + 1.;
    const double C297 = C228 - C292;
    const double C298 = C204 * C295;
    const double C299 = C297 - C256;
    const double C300 = C294 / C261;
    const double C301 = C208 * C299;
    const double C302 = C285 + C300;
    const double C303 = C298 / C261;
    const double C304 = C303 - C293;
    const double C305 = C302 / C204;
    const double C306 = C305 + C271;
    const double C307 = C304 / C206;
    const double C308 = C307 + C279;
    matrix.upper[2] =
        -(std::pow(C296, 2) *
              ((C296 *
                    (C208 * (((-0.03408 * C205 * sigmaAa / 3.) / C229 -
                              ((-(C215 * C243 * C269 / C225 + C270 * C224 / 3.)) / C229 -
                               (C215 * (C268 * C244 / C225 -
                                        C245 * 1.6455307846 * C195 * C269 / C225) -
                                C274 * C224 / 3.) /
                                   C229)) -
                             (2 * C214 * 0.00000372 * C253 / C209 - 0.00000372 * C232 * C240 / 3.) /
                                 C250) +
                     C299 * C211 / 3.) +
                C308 * C301) -
               (C290 *
                    ((C204 * (0.02556 * C284 / (C261 * C199) +
                              (C261 * 0.02556 * C195 *
                                   ((-C195 * (C215 * -16. * C216 / C266 + 2 * C257 * C224 / 3.)) /
                                        std::pow(C265, 2) -
                                    C240 / (3. * C250)) -
                               0.02556 * C288 * C284) /
                                  std::pow(C261, 2)) -
                      C302 * C210 / 3.) /
                         C206 +
                     (2 * C226 * 0.000002 * C201 * 3.72 * C253 / C209 -
                      0.000002 * C252 * 2 * C200 * C223 / 3.) /
                         std::pow(C241, 2)) +
                (C208 * ((-0.03408 * C216 / C225 - (C263 * C244 / C225 - C245 * C269 / C225)) -
                         0.000001 * C254 / C209) +
                 C283 * C211 / 3.) *
                    C306)) -
          (C296 * C301 - C290 * C306) * 2 * C308 * C296) /
        std::pow(C296, 4);

    const double C164 = 4. * Pi;
    const double C165 = rhoA + 1e-16;
    const double C166 = 1. / 3.;
    const double C167 = 4. / 3.;
    const double C168 = std::sqrt(sigmaAa);
    const double C169 = 0.02556 * C168;
    const double C170 = std::pow(3., C167);
    const double C171 = std::pow(C164, C166);
    const double C172 = std::pow(C165, C167);
    const double C173 = C168 / C172;
    const double C174 = std::pow(C173, 2);
    const double C175 = std::pow(C173, 3.72);
    const double C176 = C175 * C171;
    const double C177 = C174 + 1.;
    const double C178 = 0.000002 * C176;
    const double C179 = std::sqrt(C177);
    const double C180 = C173 + C179;
    const double C181 = C178 / C170;
    const double C182 = std::log(C180);
    const double C183 = C182 * C169;
    const double C184 = C183 / C172;
    const double C185 = C184 + C181;
    const double C186 = C185 + 1.;
    matrix.upper[1] =
        -(0. / 3. + C186 * std::pow(rhoA, C167) * (0.00426 - 5. * std::pow(36. * Pi, -5. / 3.)) *
                        C174 * 0. * std::pow(C165, C166) * sigmaAa /
                        (C172 * 3. * std::pow(C165, 8. / 3.) * std::pow(C186, 2)));

    const double C46 = 4. * Pi;
    const double C47 = 36. * Pi;
    const double C48 = rhoA + 1e-16;
    const double C49 = -5. / 3.;
    const double C50 = -2. / 3.;
    const double C51 = 1. / 3.;
    const double C52 = 4. / 3.;
    const double C53 = 5. / 3.;
    const double C54 = 8. / 3.;
    const double C55 = std::sqrt(sigmaAa);
    const double C56 = 0.02556 * C55;
    const double C57 = std::pow(3., C52);
    const double C58 = std::pow(C46, C51);
    const double C59 = std::pow(C47, C49);
    const double C60 = std::pow(C48, C50);
    const double C61 = std::pow(C48, C51);
    const double C62 = std::pow(C48, C52);
    const double C63 = std::pow(C48, C53);
    const double C64 = std::pow(C48, C54);
    const double C65 = std::pow(rhoA, C50);
    const double C66 = std::pow(rhoA, C51);
    const double C67 = std::pow(rhoA, C52);
    const double C68 = 3. * C64;
    const double C69 = 4. * C60;
    const double C70 = 4. * C61;
    const double C71 = 4. * C66;
    const double C72 = 5. * C59;
    const double C73 = 8. * C63;
    const double C74 = 24. * C63;
    const double C75 = C61 * sigmaAa;
    const double C76 = C64 * C57;
    const double C77 = sigmaAa * C60;
    const double C78 = C55 / C62;
    const double C79 = -0.131642462768e2 * C75;
    const double C80 = -8. * C75;
    const double C81 = -8. * C77;
    const double C82 = -0.03408 * C75;
    const double C83 = 3. * C76;
    const double C84 = C55 * C69;
    const double C85 = C55 * C70;
    const double C86 = C62 * C68;
    const double C87 = C62 * C74;
    const double C88 = C64 * C70;
    const double C89 = 0.00426 - C72;
    const double C90 = std::pow(C68, 2);
    const double C91 = std::pow(C78, 1.72);
    const double C92 = std::pow(C78, 2);
    const double C93 = std::pow(C78, 2.72);
    const double C94 = std::pow(C78, 3.72);
    const double C95 = -3.72 * C84;
    const double C96 = -3.72 * C85;
    const double C97 = -2.72 * C85;
    const double C98 = 0.000001 * C94;
    const double C99 = 0.00426 * C92;
    const double C100 = 1.6455307846 * C92;
    const double C101 = 3. * C88;
    const double C102 = C89 * C80;
    const double C103 = C89 * C92;
    const double C104 = C94 * C58;
    const double C105 = C92 + 1.;
    const double C106 = C82 / C86;
    const double C107 = C85 / C68;
    const double C108 = std::pow(C86, 2);
    const double C109 = 0.000002 * C104;
    const double C110 = C91 * C97;
    const double C111 = C93 * C95;
    const double C112 = C93 * C96;
    const double C113 = C87 + C101;
    const double C114 = -C100;
    const double C115 = std::sqrt(C105);
    const double C116 = 0.000001 * C112;
    const double C117 = 2 * C115;
    const double C118 = C58 * C112;
    const double C119 = C85 * C110;
    const double C120 = C78 + C115;
    const double C121 = C109 / C57;
    const double C122 = C111 / 3.;
    const double C123 = std::exp(C114);
    const double C124 = -3.72 * C119;
    const double C125 = 0.000002 * C118;
    const double C126 = C103 * C123;
    const double C127 = C123 * C102;
    const double C128 = C123 * C79;
    const double C129 = C86 * C117;
    const double C130 = C116 / C68;
    const double C131 = std::log(C120);
    const double C132 = C103 * C128;
    const double C133 = C131 * C56;
    const double C134 = C99 - C126;
    const double C135 = C124 / C68;
    const double C136 = C125 / C83;
    const double C137 = C127 / C86;
    const double C138 = C80 / C129;
    const double C139 = C133 * C70;
    const double C140 = C122 + C135;
    const double C141 = C134 - C98;
    const double C142 = C138 - C107;
    const double C143 = C132 / C86;
    const double C144 = C133 / C62;
    const double C145 = C141 * C71;
    const double C146 = C55 * C142;
    const double C147 = C67 * C141;
    const double C148 = C144 + C121;
    const double C149 = C137 - C143;
    const double C150 = C139 / 3.;
    const double C151 = 0.02556 * C146;
    const double C152 = C148 + 1.;
    const double C153 = C106 - C149;
    const double C154 = C145 / 3.;
    const double C155 = C62 * C151;
    const double C156 = C153 - C130;
    const double C157 = C156 * C71;
    const double C158 = C67 * C156;
    const double C159 = C155 / C120;
    const double C160 = C158 + C154;
    const double C161 = C159 - C150;
    const double C162 = C161 / C64;
    const double C163 = C162 + C136;
    matrix.upper[0] = -(
        3.7221029453964 * C65 / 9. +
        (std::pow(C152, 2) *
             ((C152 * ((C67 * (((C86 * -0.03408 * C77 / 3. - -0.03408 * C75 * C113 / 3.) / C108 -
                                ((C86 * (C123 * C89 * C81 / 3. - C102 * C128 / C86) -
                                  C127 * C113 / 3.) /
                                     C108 -
                                 (C86 * (C103 * (C123 * -0.131642462768e2 * C77 / 3. -
                                                 -0.131642462768e2 * C75 * C128 / C86) +
                                         C128 * C102 / C86) -
                                  C132 * C113 / 3.) /
                                     C108)) -
                               (3. * C64 * 0.000001 * C140 - 0.000001 * C112 * C74 / 3.) / C90) +
                        C157 / 3.) +
                       (C141 * 4. * C65 / 3. + C157) / 3.) +
               C163 * C160) -
              (C147 * ((C64 * ((C120 * (C62 * 0.02556 * C55 *
                                            ((C129 * C81 / 3. - -8. * C75 *
                                                                    (C86 * -16. * C75 / C129 +
                                                                     2 * C115 * C113 / 3.)) /
                                                 std::pow(C129, 2) -
                                             (3. * C64 * C84 / 3. - C85 * C74 / 3.) / C90) +
                                        0.02556 * C146 * C70 / 3.) -
                                C155 * C142) /
                                   std::pow(C120, 2) -
                               (C133 * C69 / 3. + 4. * C61 * C151 / C120) / 3.) -
                        C161 * C73 / 3.) /
                           std::pow(C48, 16. / 3.) +
                       (3. * C76 * 0.000002 * C58 * C140 - 0.000002 * C118 * 3. * C57 * C73 / 3.) /
                           std::pow(C83, 2)) +
               C160 * C163)) -
         (C152 * C160 - C147 * C163) * 2 * C163 * C152) /
            std::pow(C152, 4));
}

} // namespace excgrid
