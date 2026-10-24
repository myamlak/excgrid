#pragma once

#include "excgrid/kernel.hpp"

namespace excgrid {

/// \defgroup excgrid-kernels The shipped kernel functions
///
/// The LDA and GGA kernels generated from one expression per functional: each
/// name's first-derivative kernel and its second-derivative tier, declared in
/// registry order, the tier of `Foo` being `FooSecondDerivatives`.
/// \{

/// Slater (Dirac) LDA exchange.
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \returns The energy density and its spin-density derivatives.
/// \ingroup excgrid-kernels
XcKernelValue SlaterExchange(double rhoA, double rhoB);

/// VWN5 LDA correlation (the interpolation-V parametrization).
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \returns The energy density and its spin-density derivatives.
/// \ingroup excgrid-kernels
XcKernelValue Vwn5Correlation(double rhoA, double rhoB);

/// VWN3 LDA correlation (the RPA-parameter parametrization).
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \returns The energy density and its spin-density derivatives.
/// \ingroup excgrid-kernels
XcKernelValue Vwn3Correlation(double rhoA, double rhoB);

/// PW92 LDA correlation.
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \returns The energy density and its spin-density derivatives.
/// \ingroup excgrid-kernels
XcKernelValue Pw92Correlation(double rhoA, double rhoB);

/// Becke 1988 GGA exchange.
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \returns The energy density and its first derivatives.
/// \ingroup excgrid-kernels
XcKernelValue Becke88Exchange(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb);

/// Perdew-Wang 1991 GGA exchange.
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \returns The energy density and its first derivatives.
/// \ingroup excgrid-kernels
XcKernelValue Pw91Exchange(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb);

/// Perdew-Burke-Ernzerhof 1996 GGA exchange.
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \returns The energy density and its first derivatives.
/// \ingroup excgrid-kernels
XcKernelValue PbeExchange(double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb);

/// revPBE GGA exchange.
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \returns The energy density and its first derivatives.
/// \ingroup excgrid-kernels
XcKernelValue RevPbeExchange(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb);

/// RPBE (Hammer-Hansen-Norskov 1999) GGA exchange.
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \returns The energy density and its first derivatives.
/// \ingroup excgrid-kernels
XcKernelValue RpbeExchange(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb);

/// mPW91 GGA exchange.
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \returns The energy density and its first derivatives.
/// \ingroup excgrid-kernels
XcKernelValue MPw91Exchange(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb);

/// PBEsol GGA exchange.
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \returns The energy density and its first derivatives.
/// \ingroup excgrid-kernels
XcKernelValue PbeSolExchange(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb);

/// Lee-Yang-Parr 1988 GGA correlation.
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \returns The energy density and its first derivatives.
/// \ingroup excgrid-kernels
XcKernelValue LypCorrelation(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb);

/// Perdew-Burke-Ernzerhof 1996 GGA correlation.
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \returns The energy density and its first derivatives.
/// \ingroup excgrid-kernels
XcKernelValue PbeCorrelation(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb);

/// Perdew-Wang 1991 GGA correlation.
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \returns The energy density and its first derivatives.
/// \ingroup excgrid-kernels
XcKernelValue Pw91Correlation(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb);

/// Perdew 1986 GGA correlation.
/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \returns The energy density and its first derivatives.
/// \ingroup excgrid-kernels
XcKernelValue P86Correlation(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb);

/// \name The second-derivative tiers
///
/// One per functional, filling the contract's upper triangle over the
/// components that kernel reads, in identifier order; the matrix is left
/// untouched where the kernel's own guards do not answer.  A functional without
/// one has no declaration here.  These are the generated entry points under
/// XcFunctional::EvaluatePointMaterialising, which re-packs their matrix.  The
/// tau tier is not declared here: its kernel is wider than these pointer types.
/// \{
/// \ingroup excgrid-kernels

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param matrix Filled with the upper triangle over (rhoA, rhoB).
/// \ingroup excgrid-kernels
void SlaterExchangeSecondDerivatives(double rhoA, double rhoB, PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param matrix Filled with the upper triangle over (rhoA, rhoB).
/// \ingroup excgrid-kernels
void Vwn5CorrelationSecondDerivatives(double rhoA, double rhoB,
                                      PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param matrix Filled with the upper triangle over (rhoA, rhoB).
/// \ingroup excgrid-kernels
void Vwn3CorrelationSecondDerivatives(double rhoA, double rhoB,
                                      PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param matrix Filled with the upper triangle over (rhoA, rhoB).
/// \ingroup excgrid-kernels
void Pw92CorrelationSecondDerivatives(double rhoA, double rhoB,
                                      PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \param matrix Filled with the upper triangle over rhoA..sigmaBb.
/// \ingroup excgrid-kernels
void Becke88ExchangeSecondDerivatives(double rhoA, double rhoB, double sigmaAa, double sigmaAb,
                                      double sigmaBb, PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \param matrix Filled with the upper triangle over rhoA..sigmaBb.
/// \ingroup excgrid-kernels
void Pw91ExchangeSecondDerivatives(double rhoA, double rhoB, double sigmaAa, double sigmaAb,
                                   double sigmaBb, PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \param matrix Filled with the upper triangle over rhoA..sigmaBb.
/// \ingroup excgrid-kernels
void PbeExchangeSecondDerivatives(double rhoA, double rhoB, double sigmaAa, double sigmaAb,
                                  double sigmaBb, PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \param matrix Filled with the upper triangle over rhoA..sigmaBb.
/// \ingroup excgrid-kernels
void RevPbeExchangeSecondDerivatives(double rhoA, double rhoB, double sigmaAa, double sigmaAb,
                                     double sigmaBb, PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \param matrix Filled with the upper triangle over rhoA..sigmaBb.
/// \ingroup excgrid-kernels
void RpbeExchangeSecondDerivatives(double rhoA, double rhoB, double sigmaAa, double sigmaAb,
                                   double sigmaBb, PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \param matrix Filled with the upper triangle over rhoA..sigmaBb.
/// \ingroup excgrid-kernels
void MPw91ExchangeSecondDerivatives(double rhoA, double rhoB, double sigmaAa, double sigmaAb,
                                    double sigmaBb, PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \param matrix Filled with the upper triangle over rhoA..sigmaBb.
/// \ingroup excgrid-kernels
void PbeSolExchangeSecondDerivatives(double rhoA, double rhoB, double sigmaAa, double sigmaAb,
                                     double sigmaBb, PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \param matrix Filled with the upper triangle over rhoA..sigmaBb.
/// \ingroup excgrid-kernels
void LypCorrelationSecondDerivatives(double rhoA, double rhoB, double sigmaAa, double sigmaAb,
                                     double sigmaBb, PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \param matrix Filled with the upper triangle over rhoA..sigmaBb.
/// \ingroup excgrid-kernels
void PbeCorrelationSecondDerivatives(double rhoA, double rhoB, double sigmaAa, double sigmaAb,
                                     double sigmaBb, PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \param matrix Filled with the upper triangle over rhoA..sigmaBb.
/// \ingroup excgrid-kernels
void Pw91CorrelationSecondDerivatives(double rhoA, double rhoB, double sigmaAa, double sigmaAb,
                                      double sigmaBb, PointSecondDerivativeMatrix& matrix);

/// \param rhoA The alpha-spin density.
/// \param rhoB The beta-spin density.
/// \param sigmaAa grad(rhoA) . grad(rhoA).
/// \param sigmaAb grad(rhoA) . grad(rhoB).
/// \param sigmaBb grad(rhoB) . grad(rhoB).
/// \param matrix Filled with the upper triangle over rhoA..sigmaBb.
/// \ingroup excgrid-kernels
void P86CorrelationSecondDerivatives(double rhoA, double rhoB, double sigmaAa, double sigmaAb,
                                     double sigmaBb, PointSecondDerivativeMatrix& matrix);

/// \}
/// \ingroup excgrid-kernels

} // namespace excgrid
