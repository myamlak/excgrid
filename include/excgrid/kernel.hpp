#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <string_view>

#include "excgrid/components.hpp"

namespace excgrid {

/// \defgroup excgrid-kernel The per-point XC kernel API
///
/// The seam between a functional and its consumer.  Inputs are supplied by
/// identifier through a mask; the second derivative crosses as a contraction.
/// Units are the component table's.

/// Which components a caller supplies, or a result publishes.
///
/// One bit per identifier.  Read it on every evaluation; the number of active
/// components is not the capacity.
/// \ingroup excgrid-kernel
struct ComponentMask {
    std::uint32_t bits = 0; ///< One bit per Component, by its value.

    /// Whether a component is in the mask.
    /// \param id The identifier.
    /// \returns True when its bit is set.
    /// \ingroup excgrid-kernel
    [[nodiscard]] constexpr bool Test(Component id) const noexcept {
        return (bits & (1U << static_cast<std::uint32_t>(id))) != 0;
    }

    /// Adds a component.
    /// \param id The identifier.
    /// \ingroup excgrid-kernel
    constexpr void Set(Component id) noexcept { bits |= 1U << static_cast<std::uint32_t>(id); }

    /// Removes a component.
    /// \param id The identifier.
    /// \ingroup excgrid-kernel
    constexpr void Clear(Component id) noexcept { bits &= ~(1U << static_cast<std::uint32_t>(id)); }

    /// How many components are active.
    /// \returns The population count.
    /// \ingroup excgrid-kernel
    [[nodiscard]] constexpr std::size_t ActiveCount() const noexcept {
        std::uint32_t rest = bits;
        std::size_t count = 0;
        while (rest != 0) {
            count += rest & 1U;
            rest >>= 1U;
        }
        return count;
    }

    /// The mask a collinear functional consumes.
    /// \returns Densities, gradient invariants and kinetic-energy densities.
    /// \ingroup excgrid-kernel
    [[nodiscard]] static constexpr ComponentMask Collinear() noexcept {
        ComponentMask mask;
        for (std::uint8_t i = 0; i <= static_cast<std::uint8_t>(Component::TauB); ++i) {
            mask.Set(static_cast<Component>(i));
        }
        return mask;
    }
};

/// One grid point's inputs.
/// \ingroup excgrid-kernel
struct PointInputs {
    std::array<double, kComponentCapacity> values{}; ///< Indexed by Component.
    ComponentMask mask{}; ///< Which of them are supplied.

    /// Reads one component.
    /// \param id The identifier.
    /// \returns Its value, or zero when it is not in the mask.
    /// \ingroup excgrid-kernel
    [[nodiscard]] constexpr double Get(Component id) const noexcept {
        return mask.Test(id) ? values[IndexOf(id)] : 0.0;
    }

    /// Writes one component and adds it to the mask.
    /// \param id The identifier.
    /// \param value Its value.
    /// \ingroup excgrid-kernel
    constexpr void Set(Component id, double value) noexcept {
        values[IndexOf(id)] = value;
        mask.Set(id);
    }
};

/// What an evaluation is asked to produce.
/// \ingroup excgrid-kernel
enum class Request : std::uint8_t {
    kFirstDerivatives = 0, ///< The energy density and the first derivatives.
    kSecondDerivativeContraction = 1, ///< Additionally the second derivatives, contracted.
    kSecondDerivativeMatrix = 2, ///< Additionally the second derivatives, materialised.
};

/// How an evaluation ended.
///
/// Every refusal is named.  None of them is answered with zeros.
/// \ingroup excgrid-kernel
enum class KernelStatus : std::uint8_t {
    kOk = 0, ///< The values were produced.
    kRefusedUnsupportedCapability = 1, ///< The functional or the tier is not implemented.
    kRefusedMissingProvider = 2, ///< A capability needs a provider that is absent.
    kRefusedExhaustedCapacity = 3, ///< The request needs more components than the capacity.
    kRefusedVersionMismatch = 4, ///< The version differs, or the mask carries an unknown component.
    kRefusedUnsupportedCombination = 5, ///< The combination is not one this library answers.
};

/// One status's name, for a refusal message.
/// \param status The status.
/// \returns A short human-readable description.
/// \ingroup excgrid-kernel
[[nodiscard]] std::string_view DescribeStatus(KernelStatus status) noexcept;

/// One functional term's per-point value, under named fields.
///
/// What the generated kernels return and what composition is written in.  This
/// is not the type a consumer receives.
/// \ingroup excgrid-kernel
struct XcKernelValue {
    double exc = 0.0; ///< Energy density e(r).
    double vrhoA = 0.0; ///< de/d rhoA.
    double vrhoB = 0.0; ///< de/d rhoB.
    double vsigmaAa = 0.0; ///< de/d sigmaAa.
    double vsigmaAb = 0.0; ///< de/d sigmaAb.
    double vsigmaBb = 0.0; ///< de/d sigmaBb.
    double vtauA = 0.0; ///< de/d tauA; zero from every shipped LDA/GGA kernel.
    double vtauB = 0.0; ///< de/d tauB; zero from every shipped LDA/GGA kernel.
};

/// Componentwise addition.
/// \param a The first summand.
/// \param b The second summand.
/// \returns The componentwise sum.
/// \ingroup excgrid-kernel
constexpr XcKernelValue operator+(const XcKernelValue& a, const XcKernelValue& b) noexcept {
    return {a.exc + b.exc,           a.vrhoA + b.vrhoA,       a.vrhoB + b.vrhoB,
            a.vsigmaAa + b.vsigmaAa, a.vsigmaAb + b.vsigmaAb, a.vsigmaBb + b.vsigmaBb,
            a.vtauA + b.vtauA,       a.vtauB + b.vtauB};
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

/// Scalar weighting.
/// \param weight The scalar weight.
/// \param v The weighted value.
/// \returns The componentwise product.
/// \ingroup excgrid-kernel
constexpr XcKernelValue operator*(double weight, const XcKernelValue& v) noexcept {
    return {weight * v.exc,     weight * v.vrhoA,     weight * v.vrhoB,
            weight * v.vsigmaAa, weight * v.vsigmaAb, weight * v.vsigmaBb,
            weight * v.vtauA,   weight * v.vtauB};
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

/// GGA kernel: additionally consumes the three sigma inputs.
/// \ingroup excgrid-kernel
using GgaKernel =
    XcKernelValue (*)(double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb);

/// One grid point's result on the first-derivative tier.
/// \ingroup excgrid-kernel
struct PointResult {
    SchemaVersion version = kSchemaVersion; ///< The version that produced it.
    ComponentMask mask{}; ///< Which components the first-derivative array carries.
    double exc = 0.0; ///< Energy density, Hartree / Bohr^3.
    std::array<double, kComponentCapacity> first{}; ///< d exc / d x_i, by Component.
};

/// Folds a named kernel value into a schema result.
///
/// Slots the mask does not name are left at zero.
/// \param value The named per-point value.
/// \param mask Which identifiers the result should carry.
/// \param result The result to fill.
/// \ingroup excgrid-kernel
void FoldIntoResult(const XcKernelValue& value, const ComponentMask& mask,
                    PointResult& result) noexcept;

/// The second-derivative work for one point, contracted.
///
/// The matrix multiplied by the caller's right-hand side; the matrix itself
/// does not cross here.
/// \ingroup excgrid-kernel
struct PointSecondDerivative {
    ComponentMask mask{}; ///< Which components the contraction is over.
    std::size_t rightHandSides = 0; ///< How many right-hand sides were supplied.
    /// One value per active component per right-hand side, in identifier order.
    std::array<double, kSecondDerivativeCapacity> contracted{};
};

/// The second derivatives of one point, materialised.  Debug only.
/// \ingroup excgrid-kernel
struct PointSecondDerivativeMatrix {
    ComponentMask mask{}; ///< Which components the matrix is over.
    /// The upper triangle, row-major over the active components in identifier
    /// order: entry (i,j), i <= j, at i*active - i*(i-1)/2 + (j-i).
    std::array<double, kSecondDerivativeCapacity * kSecondDerivativeCapacity> upper{};
};

/// A named, composed XC functional.
/// \ingroup excgrid-kernel
class XcFunctional {
public:
    virtual ~XcFunctional() = default;

    /// Whether the kernel consumes the gradient invariants.
    /// \returns True when sigma terms are present.
    [[nodiscard]] virtual bool UsesGradient() const = 0;

    /// The exact-exchange fraction the consumer routes through its own
    /// Hartree-Fock path.
    /// \returns The fraction in [0, 1].
    [[nodiscard]] virtual double ExchangeFraction() const = 0;

    /// The components this functional reads.
    /// \returns Its required mask.
    [[nodiscard]] virtual ComponentMask RequiredMask() const = 0;

    /// One per-point evaluation on the first-derivative tier.
    /// \param inputs The point's components and mask.
    /// \param result Filled on kOk; untouched otherwise.
    /// \returns kOk, or a named refusal.
    [[nodiscard]] virtual KernelStatus EvaluatePoint(const PointInputs& inputs,
                                                     PointResult& result) const = 0;

    /// One per-point evaluation including a second-derivative contraction.
    ///
    /// The default refuses with kRefusedUnsupportedCapability.
    /// \param inputs The point's components.
    /// \param rightHandSide The vector to contract with, by active component.
    /// \param result Filled on kOk; untouched otherwise.
    /// \param second Filled on kOk; untouched otherwise.
    /// \returns kOk, or a named refusal.
    [[nodiscard]] virtual KernelStatus EvaluatePointWithSecondDerivatives(
        const PointInputs& inputs, std::span<const double> rightHandSide, PointResult& result,
        PointSecondDerivative& second) const;

    /// One per-point evaluation materialising the second-derivative matrix.
    ///
    /// Debug path; the default refuses with kRefusedUnsupportedCapability.
    /// \param inputs The point's components.
    /// \param result Filled on kOk; untouched otherwise.
    /// \param matrix Filled on kOk; untouched otherwise.
    /// \returns kOk, or a named refusal.
    [[nodiscard]] virtual KernelStatus EvaluatePointMaterialising(
        const PointInputs& inputs, PointResult& result,
        PointSecondDerivativeMatrix& matrix) const;

    /// The positional entry point, kept as a temporary adapter for consumers
    /// that have not moved to EvaluatePoint.
    ///
    /// A refusal is reported as a zero-filled value with the status lost.
    /// \param rhoA The alpha-spin density.
    /// \param rhoB The beta-spin density.
    /// \param sigmaAa grad(rhoA) . grad(rhoA).
    /// \param sigmaAb grad(rhoA) . grad(rhoB).
    /// \param sigmaBb grad(rhoB) . grad(rhoB).
    /// \returns The energy density and first derivatives.
    /// \ingroup excgrid-kernel
    XcKernelValue Evaluate(double rhoA, double rhoB, double sigmaAa, double sigmaAb,
                           double sigmaBb) const noexcept;
};

/// Resolves a functional by its shipped name; null when unknown.
/// \param name The functional name.
/// \returns The functional, or nullptr.
/// \ingroup excgrid-kernel
const XcFunctional* FindFunctional(std::string_view name) noexcept;

/// All shipped functional names, registry order.
/// \returns The name span (owned by the registry).
/// \ingroup excgrid-kernel
std::span<const std::string_view> FunctionalNames() noexcept;

/// The schema version this build speaks.
/// \returns The version.
/// \ingroup excgrid-kernel
[[nodiscard]] SchemaVersion ContractVersion() noexcept;

} // namespace excgrid
