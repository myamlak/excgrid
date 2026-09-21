// The hand-written tau-dependent meta-GGA reference: the second-order gradient
// expansion of exchange, written in the iso-orbital variables.  It is the
// library's first functional to read the kinetic-energy densities, and it is
// hand-written on purpose - it comes from no generator, so it holds the schema's
// tau inputs to their exact conditions without depending on the codegen
// pipeline.  Its verification channels are its named exact conditions and its
// finite differences (tests/meta_gga_reference_test.cpp).
//
// What it is, in one line: per spin channel,
//
//     e_sigma = e_x^LSDA(rho_sigma) * (1 + (1 - tau_P/tau_unif)/12),
//     tau_P = tau_sigma - |grad rho_sigma|^2 / (8 rho_sigma),
//     tau_unif = (3/10)(6 pi^2)^(2/3) rho_sigma^(5/3).
//
// The gradient enters through the von Weizsacker kinetic-energy density
// |grad rho|^2/(8 rho) and nothing else, which is what the exact conditions in
// the test file are about.  Two limits follow from the form rather than from a
// fit: the uniform gas (tau_P = tau_unif, so the enhancement is exactly 1 and
// the kernel is the spin-scaled Slater kernel) and the slowly varying gas, where
// tau_P/tau_unif = 1 - (40/27) s^2 and the enhancement's leading coefficient is
// the exact 10/81 of the exchange gradient expansion.
//
// Deliberate limitations, so that no caller reads more into it than is there:
// the enhancement is unbounded above (the bare expansion in tau_P grows with
// tau, which is the known failure of the second-order expansion in high-tau
// regions), so it is a shape reference for the tau tier and not a production
// functional.  No external implementation supplied the formula or checked it.

#include "excgrid/kernel.hpp"

#include <cmath>
#include <limits>

namespace excgrid {
namespace {

/// (3/4)(6/pi)^(1/3): the spin-scaled Slater constant, so the LSDA exchange
/// energy density is -kSlaterSpinPrefactor rho^(4/3) per spin channel.  The
/// spin-scaled form is the one the registry's LDA kernels carry, so the uniform
/// limit below agrees with them by construction.
constexpr double kSlaterSpinPrefactor = 0.9305257363491;

/// (3/10)(6 pi^2)^(2/3): the spin-scaled Thomas-Fermi constant, so the uniform
/// electron gas's kinetic-energy density is kFermiConstant rho^(5/3) per spin
/// channel.  The 6 pi^2 (rather than 3 pi^2) is the spin scaling: each channel
/// carries its own Fermi sphere.
constexpr double kFermiConstant = 4.5577998723455959;

/// kSlaterSpinPrefactor / (12 kFermiConstant): the energy density per unit
/// tau_P once tau_unif's density factors have cancelled against the LSDA
/// prefactor.  The 12 is the enhancement's denominator.
constexpr double kTauPrefactor = kSlaterSpinPrefactor / (12.0 * kFermiConstant);

/// -(13/12) kSlaterSpinPrefactor: the rho^(4/3) coefficient the expansion
/// leaves once the uniform part of tau_P has been folded into it.
constexpr double kDensityPrefactor = -(13.0 / 12.0) * kSlaterSpinPrefactor;

/// The density below which a channel contributes nothing, matching the shipped
/// kernels' own guard: rho^(4/3) and rho^(1/3) both vanish there while the
/// inverse powers below would not.
constexpr double kMinimumDensity = std::numeric_limits<double>::epsilon();

/// One spin channel's energy density and its derivatives.
struct SpinChannel {
    double exc = 0.0; ///< Energy density, Hartree / Bohr^3.
    double vrho = 0.0; ///< de/d rho, Hartree.
    double vsigma = 0.0; ///< de/d sigma, Hartree Bohr^5.
    double vtau = 0.0; ///< de/d tau, Hartree Bohr^5.
};

/// One spin channel at (rho, sigma, tau).
///
/// The energy density is linear in tau_P = tau - sigma/(8 rho), so the three
/// derivatives below are exact and the tau derivative is a constant in tau.
/// \param rho The channel's density, above kMinimumDensity.
/// \param sigma grad(rho) . grad(rho) for this channel.
/// \param tau The channel's kinetic-energy density.
/// \returns The channel's energy density and first derivatives.
[[nodiscard]] SpinChannel EvaluateChannel(double rho, double sigma, double tau) noexcept {
    const double rhoCbrt = std::cbrt(rho);
    const double rhoInverseCbrt = 1.0 / rhoCbrt;
    const double tauPauli = tau - sigma / (8.0 * rho);

    SpinChannel channel;
    channel.exc = kDensityPrefactor * rho * rhoCbrt + kTauPrefactor * rhoInverseCbrt * tauPauli;
    channel.vtau = kTauPrefactor * rhoInverseCbrt;
    channel.vsigma = -kTauPrefactor * rhoInverseCbrt / (8.0 * rho);
    // e = kDensityPrefactor rho^(4/3) + kTauPrefactor rho^(-1/3) tau_P, so the
    // product rule gives three pieces: the density term's derivative, the
    // rho^(-1/3) factor's, and tau_P's own, sigma/(8 rho^2).  The last is written
    // as the 8 of tau_P above rather than as its folded coefficient, so the two
    // cannot drift apart.
    channel.vrho = (4.0 / 3.0) * kDensityPrefactor * rhoCbrt -
                   (kTauPrefactor / 3.0) * rhoInverseCbrt * tauPauli / rho +
                   kTauPrefactor * rhoInverseCbrt * sigma / (8.0 * rho * rho);

    return channel;
}

/// The tau tier's reference functional.
///
/// A pure exchange functional: the whole of its spin dependence is the exchange
/// spin scaling, and it carries no exact-exchange fraction.
class MetaGgaReference final : public XcFunctional {
public:
    [[nodiscard]] bool UsesGradient() const override {
        return true;
    }

    [[nodiscard]] double ExchangeFraction() const override {
        return 0.0;
    }

    /// The components a collinear functional consumes: the two densities, the
    /// three gradient invariants and the two kinetic-energy densities.  The
    /// gradient invariants are required whole, as every shipped gradient
    /// functional requires them, even where a channel does not read one of them.
    [[nodiscard]] ComponentMask RequiredMask() const override {
        return ComponentMask::Collinear();
    }

    [[nodiscard]] KernelStatus EvaluatePoint(const PointInputs& inputs,
                                             PointResult& result) const override {
        XcKernelValue value;

        const double rhoA = inputs.Get(Component::RhoA);

        if (rhoA >= kMinimumDensity)
        {
            const SpinChannel alpha =
                EvaluateChannel(rhoA, inputs.Get(Component::SigmaAa), inputs.Get(Component::TauA));
            value.exc += alpha.exc;
            value.vrhoA = alpha.vrho;
            value.vsigmaAa = alpha.vsigma;
            value.vtauA = alpha.vtau;
        }

        const double rhoB = inputs.Get(Component::RhoB);

        if (rhoB >= kMinimumDensity)
        {
            const SpinChannel beta =
                EvaluateChannel(rhoB, inputs.Get(Component::SigmaBb), inputs.Get(Component::TauB));
            value.exc += beta.exc;
            value.vrhoB = beta.vrho;
            value.vsigmaBb = beta.vsigma;
            value.vtauB = beta.vtau;
        }

        FoldIntoResult(value, inputs.mask, result);
        return KernelStatus::kOk;
    }
};

} // namespace

/// The tau tier's reference functional, for the registry to publish.
///
/// The tau tier's kernel is wider than the LDA/GGA kernel types (it reads the
/// two kinetic-energy densities as well as the three gradient invariants), so
/// the functional supplies itself rather than being assembled from a kernel
/// pointer.
/// \returns The functional, which outlives every caller.
const XcFunctional& MetaGgaReferenceFunctional() noexcept {
    static const MetaGgaReference functional;
    return functional;
}

} // namespace excgrid
