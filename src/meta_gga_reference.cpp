// The hand-written tau-dependent meta-GGA reference: the second-order gradient
// expansion of exchange in the iso-orbital variables.  Per spin channel:
//     e_sigma = e_x^LSDA(rho_sigma) * (1 + (1 - tau_P/tau_unif)/12),
//     tau_P = tau_sigma - |grad rho_sigma|^2 / (8 rho_sigma),
//     tau_unif = (3/10)(6 pi^2)^(2/3) rho_sigma^(5/3).
// A shape reference, not a production functional: unbounded above in high-tau regions.

#include "excgrid/kernel.hpp"

#include <cmath>
#include <limits>

namespace excgrid {
namespace {

/// (3/4)(6/pi)^(1/3), the spin-scaled Slater constant: e_x = -c rho^(4/3) per channel.
constexpr double kSlaterSpinPrefactor = 0.9305257363491;

/// (3/10)(6 pi^2)^(2/3), the spin-scaled Thomas-Fermi constant: tau = c rho^(5/3) per channel.
constexpr double kFermiConstant = 4.5577998723455959;

/// kSlaterSpinPrefactor / (12 kFermiConstant): the energy density per unit tau_P.
constexpr double kTauPrefactor = kSlaterSpinPrefactor / (12.0 * kFermiConstant);

/// -(13/12) kSlaterSpinPrefactor: the rho^(4/3) coefficient of the expansion.
constexpr double kDensityPrefactor = -(13.0 / 12.0) * kSlaterSpinPrefactor;

/// The density below which a channel contributes nothing, as the shipped kernels' guard.
constexpr double kMinimumDensity = std::numeric_limits<double>::epsilon();

/// One spin channel's energy density and its derivatives.
struct SpinChannel {
    double exc = 0.0; ///< Energy density, Hartree / Bohr^3.
    double vrho = 0.0; ///< de/d rho, Hartree.
    double vsigma = 0.0; ///< de/d sigma, Hartree Bohr^5.
    double vtau = 0.0; ///< de/d tau, Hartree Bohr^5.
};

/// One spin channel at (rho, sigma, tau); the energy density is linear in
/// tau_P = tau - sigma/(8 rho), so the derivatives are exact.
/// \param rho The channel's density, above kMinimumDensity.
/// \param sigma grad(rho) . grad(rho) for this channel.
[[nodiscard]] SpinChannel EvaluateChannel(double rho, double sigma, double tau) noexcept {
    const double rhoCbrt = std::cbrt(rho);
    const double rhoInverseCbrt = 1.0 / rhoCbrt;
    const double tauPauli = tau - sigma / (8.0 * rho);

    SpinChannel channel;
    channel.exc = kDensityPrefactor * rho * rhoCbrt + kTauPrefactor * rhoInverseCbrt * tauPauli;
    channel.vtau = kTauPrefactor * rhoInverseCbrt;
    channel.vsigma = -kTauPrefactor * rhoInverseCbrt / (8.0 * rho);
    // vrho = d/drho [kDensityPrefactor rho^(4/3) + kTauPrefactor rho^(-1/3) tau_P],
    // with tau_P's own derivative sigma/(8 rho^2) written through the 8 above.
    channel.vrho = (4.0 / 3.0) * kDensityPrefactor * rhoCbrt -
                   (kTauPrefactor / 3.0) * rhoInverseCbrt * tauPauli / rho +
                   kTauPrefactor * rhoInverseCbrt * sigma / (8.0 * rho * rho);

    return channel;
}

/// The tau tier's reference functional: pure exchange, no exact-exchange fraction.
class MetaGgaReference final : public XcFunctional {
public:
    [[nodiscard]] bool UsesGradient() const override {
        return true;
    }

    [[nodiscard]] double ExchangeFraction() const override {
        return 0.0;
    }

    /// The collinear components: two densities, three gradient invariants, two tau.
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
/// \returns The functional, which outlives every caller.
const XcFunctional& MetaGgaReferenceFunctional() noexcept {
    static const MetaGgaReference functional;
    return functional;
}

} // namespace excgrid
