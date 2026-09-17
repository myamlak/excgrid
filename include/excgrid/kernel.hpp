#pragma once

#include <span>
#include <string_view>

namespace excgrid {

/// \defgroup excgrid-kernel The per-point XC kernel API
///
/// The frozen seam between excgrid's functionals and their consumers
/// (docs/kernel-api.md is the governing contract; this header is its
/// implementation).  Units are Libxc's everywhere: densities in
/// electrons/Bohr^3, sigma in Bohr^-8, energy densities in
/// Hartree/Bohr^3, potentials in Hartree (vrho) and Hartree * Bohr^5
/// (vsigma).

/// One per-point XC kernel evaluation: the energy density and its first
/// derivatives.  A plain aggregate (the public fields are the API) with
/// the Lane-U-style algebra - value and derivatives in one struct,
/// functionals compose via `+` and scalar `*`.  The tau slots are
/// reserved for the meta-GGA extension: present in every result, zero
/// from every shipped LDA/GGA kernel, so the extension is additive.
/// \ingroup excgrid-kernel
struct XcKernelValue {
    double exc = 0.0; ///< Energy density e(r).
    double vrhoA = 0.0; ///< de/d rhoA.
    double vrhoB = 0.0; ///< de/d rhoB.
    double vsigmaAa = 0.0; ///< de/d sigmaAa.
    double vsigmaAb = 0.0; ///< de/d sigmaAb.
    double vsigmaBb = 0.0; ///< de/d sigmaBb.
    double vtauA = 0.0; ///< de/d tauA (reserved - zero from LDA/GGA).
    double vtauB = 0.0; ///< de/d tauB (reserved - zero from LDA/GGA).
};

/// Componentwise addition - functional composition by `+`.
/// \param a The first summand.
/// \param b The second summand.
/// \returns The componentwise sum.
/// \ingroup excgrid-kernel
constexpr XcKernelValue operator+(const XcKernelValue& a, const XcKernelValue& b) noexcept {
    return {a.exc + b.exc,
            a.vrhoA + b.vrhoA,
            a.vrhoB + b.vrhoB,
            a.vsigmaAa + b.vsigmaAa,
            a.vsigmaAb + b.vsigmaAb,
            a.vsigmaBb + b.vsigmaBb,
            a.vtauA + b.vtauA,
            a.vtauB + b.vtauB};
}

/// Componentwise accumulation.
/// \param a The accumulator (updated).
/// \param b The addend.
/// \returns The updated accumulator.
/// \ingroup excgrid-kernel
constexpr XcKernelValue& operator+=(XcKernelValue& a, const XcKernelValue& b) noexcept {
    a = a + b;

    return a;
}

/// Scalar weighting - functional composition by `*` (hybrid recipes).
/// \param weight The scalar weight.
/// \param v The weighted value.
/// \returns The componentwise product.
/// \ingroup excgrid-kernel
constexpr XcKernelValue operator*(double weight, const XcKernelValue& v) noexcept {
    return {weight * v.exc,
            weight * v.vrhoA,
            weight * v.vrhoB,
            weight * v.vsigmaAa,
            weight * v.vsigmaAb,
            weight * v.vsigmaBb,
            weight * v.vtauA,
            weight * v.vtauB};
}

/// Scalar weighting, mirrored order.
/// \param v The weighted value.
/// \param weight The scalar weight.
/// \returns The componentwise product.
/// \ingroup excgrid-kernel
constexpr XcKernelValue operator*(const XcKernelValue& v, double weight) noexcept {
    return weight * v;
}

/// LDA kernel: the energy density and its spin-density derivatives.
/// \ingroup excgrid-kernel
using LdaKernel = XcKernelValue (*)(double rhoA, double rhoB);

/// GGA kernel: additionally consumes the three sigma inputs and returns
/// the three vsigma derivatives.
/// \ingroup excgrid-kernel
using GgaKernel =
    XcKernelValue (*)(double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb);

/// A named, composed XC functional.  The consumer evaluates one kernel
/// call per grid point through this seam; hybrids are weighted sums of
/// the generated kernels (see docs/kernel-api.md section 4 for the
/// shipped recipes).
/// \ingroup excgrid-kernel
class XcFunctional {
public:
    virtual ~XcFunctional() = default;

    /// Whether the kernel consumes the sigma inputs (GGA or beyond).
    /// \returns True when sigma terms are present.
    [[nodiscard]] virtual bool UsesGradient() const = 0;

    /// The exact-exchange fraction the consumer routes through its HF
    /// Fock path (0 for pure functionals, e.g. 0.20 for B3LYP).
    /// \returns The HF exchange fraction in [0, 1].
    [[nodiscard]] virtual double ExchangeFraction() const = 0;

    /// One per-point evaluation.  LDA kernels ignore the sigma inputs.
    /// \param rhoA The alpha-spin density.
    /// \param rhoB The beta-spin density.
    /// \param sigmaAa grad(rhoA) . grad(rhoA).
    /// \param sigmaAb grad(rhoA) . grad(rhoB).
    /// \param sigmaBb grad(rhoB) . grad(rhoB).
    /// \returns The energy density and first derivatives.
    virtual XcKernelValue Evaluate(
        double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) const = 0;
};

/// Resolves a functional by its shipped name (docs/kernel-api.md section
/// 4); null when unknown.  Name-string resolution, never an enum - enums
/// drift with repo releases, the consumer's schema carries the string.
/// \param name The functional name.
/// \returns The functional, or nullptr.
/// \ingroup excgrid-kernel
const XcFunctional* FindFunctional(std::string_view name) noexcept;

/// All shipped functional names, registry order.
/// \returns The name span (owned by the registry).
/// \ingroup excgrid-kernel
std::span<const std::string_view> FunctionalNames() noexcept;

} // namespace excgrid
