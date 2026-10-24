#include "excgrid/kernel.hpp"
#include "excgrid/kernels.hpp"

#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <span>
#include <string_view>
#include <vector>

namespace excgrid {

// The tau tier's hand-written reference, defined in meta_gga_reference.cpp and
// declared at namespace scope so its definition keeps external linkage.
const XcFunctional& MetaGgaReferenceFunctional() noexcept;

// The tau tier's generated kernel and its second-derivative tier, from generated/tau_x.cpp.
XcKernelValue TauXExchange(double rhoA,
                           double rhoB,
                           double sigmaAa,
                           double sigmaAb,
                           double sigmaBb,
                           double tauA,
                           double tauB);
void TauXExchangeSecondDerivatives(double rhoA,
                                   double rhoB,
                                   double sigmaAa,
                                   double sigmaAb,
                                   double sigmaBb,
                                   double tauA,
                                   double tauB,
                                   PointSecondDerivativeMatrix& matrix);

namespace {
// The components each tier spans: a prefix of the identifier table, in identifier
// order, so a component's kernel slot is its own identifier value.
constexpr std::size_t kLdaTierComponents = 2;
constexpr std::size_t kGgaTierComponents = 5;
constexpr std::size_t kTauTierComponents = 7;

static_assert(static_cast<std::size_t>(Component::RhoB) == kLdaTierComponents - 1,
              "an LDA tier is emitted over (rhoA, rhoB) in identifier order");
static_assert(static_cast<std::size_t>(Component::SigmaBb) == kGgaTierComponents - 1,
              "a GGA tier is emitted over rhoA..sigmaBb in identifier order");
static_assert(ComponentMask::Collinear().ActiveCount() == kTauTierComponents,
              "the tau tier's kernel is emitted over the seven collinear components; the collinear "
              "set has changed, so xc_defs/tau_x.ey must be regenerated against it");
static_assert(static_cast<std::size_t>(Component::RhoA) == 0 &&
                  static_cast<std::size_t>(Component::TauB) == kTauTierComponents - 1,
              "the tau tier's kernel argument order is the identifier order RhoA..TauB");
static_assert(kTauTierComponents <= kSecondDerivativeCapacity,
              "the tau tier's packing is wider than the contract's second-derivative capacity");

[[nodiscard]] constexpr ComponentMask TierMask(std::size_t components) noexcept {
    ComponentMask mask;

    for (std::size_t component = 0; component < components; ++component)
    {
        mask.Set(static_cast<Component>(component));
    }

    return mask;
}

// The contract's upper-triangle index over `active` components: (i, j), i <= j,
// at i*active - i*(i-1)/2 + (j-i).
[[nodiscard]] constexpr std::size_t PackedIndex(std::size_t i,
                                                std::size_t j,
                                                std::size_t active) noexcept {
    return i * active - i * (i - 1) / 2 + (j - i);
}

// The caller's mask restricted to the tier's components, with each one's kernel argument slot.
template <std::size_t Components> struct TierSlots {
    ComponentMask mask{}; ///< The intersection: what a result or matrix is over.
    std::size_t active = 0; ///< Its active-component count.
    std::array<std::size_t, Components> slot{}; ///< Kernel slot, per active component.
};

// Reads a caller's mask onto the generated kernel's argument order.
template <std::size_t Components>
[[nodiscard]] constexpr TierSlots<Components> TierSlotsOf(const ComponentMask& caller) noexcept {
    TierSlots<Components> slots;

    for (std::size_t component = 0; component < Components; ++component)
    {
        const auto id = static_cast<Component>(component);

        if (caller.Test(id))
        {
            slots.mask.Set(id);
            slots.slot[slots.active] = component;
            ++slots.active;
        }
    }

    return slots;
}

// One entry of the generated tier's upper-triangle packing, read at (min, max).
template <std::size_t Components>
[[nodiscard]] constexpr double GeneratedEntry(const PointSecondDerivativeMatrix& matrix,
                                              std::size_t a,
                                              std::size_t b) noexcept {
    return matrix.upper[PackedIndex(a < b ? a : b, a < b ? b : a, Components)];
}

// The contracted entry point, taken from the materialised matrix so that the two
// entry points cannot disagree.
template <std::size_t Components>
[[nodiscard]] KernelStatus ContractFromMatrix(const TierSlots<Components>& slots,
                                              const PointSecondDerivativeMatrix& generated,
                                              std::span<const double> rightHandSide,
                                              PointSecondDerivative& second) noexcept {
    if (slots.active == 0 || rightHandSide.size() % slots.active != 0)
    {
        return KernelStatus::kRefusedUnsupportedCombination;
    }

    if (rightHandSide.size() > kSecondDerivativeCapacity)
    {
        return KernelStatus::kRefusedExhaustedCapacity;
    }

    const std::size_t rightHandSides = rightHandSide.size() / slots.active;
    second.mask = slots.mask;
    second.rightHandSides = rightHandSides;

    for (std::size_t side = 0; side < rightHandSides; ++side)
    {
        for (std::size_t i = 0; i < slots.active; ++i)
        {
            double contracted = 0.0;

            for (std::size_t j = 0; j < slots.active; ++j)
            {
                contracted += GeneratedEntry<Components>(generated, slots.slot[i], slots.slot[j]) *
                              rightHandSide[side * slots.active + j];
            }

            second.contracted[side * slots.active + i] = contracted;
        }
    }

    return KernelStatus::kOk;
}

// Re-packs the generated matrix onto the caller's mask; an entry with no value is a zero.
template <std::size_t Components>
[[nodiscard]] constexpr PointSecondDerivativeMatrix RepackMatrix(
    const TierSlots<Components>& slots,
    const PointSecondDerivativeMatrix& generated) noexcept {
    PointSecondDerivativeMatrix matrix;
    matrix.mask = slots.mask;

    for (std::size_t i = 0; i < slots.active; ++i)
    {
        for (std::size_t j = i; j < slots.active; ++j)
        {
            matrix.upper[PackedIndex(i, j, slots.active)] =
                GeneratedEntry<Components>(generated, slots.slot[i], slots.slot[j]);
        }
    }

    return matrix;
}

// A two-wide LDA tier's matrix placed in the five-wide GGA packing; the rest stays zero.
[[nodiscard]] constexpr PointSecondDerivativeMatrix WidenLdaMatrix(
    const PointSecondDerivativeMatrix& narrow) noexcept {
    PointSecondDerivativeMatrix wide;

    for (std::size_t i = 0; i < kLdaTierComponents; ++i)
    {
        for (std::size_t j = i; j < kLdaTierComponents; ++j)
        {
            wide.upper[PackedIndex(i, j, kGgaTierComponents)] =
                narrow.upper[PackedIndex(i, j, kLdaTierComponents)];
        }
    }

    return wide;
}

// A pure functional: one generated kernel, no exact-exchange fraction, and its
// optional second-derivative tier.
class PureFunctional final : public XcFunctional {
public:
    PureFunctional(std::string_view name,
                   bool usesGradient,
                   LdaKernel lda,
                   GgaKernel gga,
                   LdaSecondDerivative ldaSecondDerivative = nullptr,
                   GgaSecondDerivative ggaSecondDerivative = nullptr) noexcept :
        _name(name), _usesGradient(usesGradient), _lda(lda), _gga(gga),
        _ldaSecondDerivative(ldaSecondDerivative), _ggaSecondDerivative(ggaSecondDerivative) {}

    [[nodiscard]] bool UsesGradient() const override {
        return _usesGradient;
    }

    [[nodiscard]] double ExchangeFraction() const override {
        return 0.0;
    }

    [[nodiscard]] ComponentMask RequiredMask() const override {
        ComponentMask mask;
        mask.Set(Component::RhoA);
        mask.Set(Component::RhoB);
        if (_usesGradient)
        {
            mask.Set(Component::SigmaAa);
            mask.Set(Component::SigmaAb);
            mask.Set(Component::SigmaBb);
        }
        return mask;
    }

    [[nodiscard]] KernelStatus EvaluatePoint(const PointInputs& inputs,
                                             PointResult& result) const override {
        XcKernelValue value;
        if (_usesGradient)
        {
            value = _gga(inputs.Get(Component::RhoA),
                         inputs.Get(Component::RhoB),
                         inputs.Get(Component::SigmaAa),
                         inputs.Get(Component::SigmaAb),
                         inputs.Get(Component::SigmaBb));
        } else
        {
            value = _lda(inputs.Get(Component::RhoA), inputs.Get(Component::RhoB));
        }

        FoldIntoResult(value, inputs.mask, result);
        return KernelStatus::kOk;
    }

    /// The tier spans the kernel's own components: two for LDA, five for GGA.
    [[nodiscard]] ComponentMask SecondDerivativeMask() const noexcept override {
        if (_usesGradient ? _ggaSecondDerivative == nullptr : _ldaSecondDerivative == nullptr)
        {
            return {};
        }

        return TierMask(_usesGradient ? kGgaTierComponents : kLdaTierComponents);
    }

    [[nodiscard]] KernelStatus EvaluatePointWithSecondDerivatives(
        const PointInputs& inputs,
        std::span<const double> rightHandSide,
        PointResult& result,
        PointSecondDerivative& second) const override {
        const KernelStatus coverage =
            SecondDerivativeStatus(inputs.mask, RequiredMask(), SecondDerivativeMask());

        if (coverage != KernelStatus::kOk)
        {
            return coverage;
        }

        PointSecondDerivativeMatrix generated;

        if (_usesGradient)
        {
            const TierSlots<kGgaTierComponents> slots =
                TierSlotsOf<kGgaTierComponents>(inputs.mask);
            _ggaSecondDerivative(inputs.Get(Component::RhoA), inputs.Get(Component::RhoB),
                                 inputs.Get(Component::SigmaAa), inputs.Get(Component::SigmaAb),
                                 inputs.Get(Component::SigmaBb), generated);

            const KernelStatus contracted = ContractFromMatrix<kGgaTierComponents>(
                slots, generated, rightHandSide, second);

            if (contracted != KernelStatus::kOk)
            {
                return contracted;
            }
        } else
        {
            const TierSlots<kLdaTierComponents> slots =
                TierSlotsOf<kLdaTierComponents>(inputs.mask);
            _ldaSecondDerivative(inputs.Get(Component::RhoA), inputs.Get(Component::RhoB),
                                 generated);

            const KernelStatus contracted = ContractFromMatrix<kLdaTierComponents>(
                slots, generated, rightHandSide, second);

            if (contracted != KernelStatus::kOk)
            {
                return contracted;
            }
        }

        FoldIntoResult(Value(inputs), inputs.mask, result);
        return KernelStatus::kOk;
    }

    [[nodiscard]] KernelStatus EvaluatePointMaterialising(
        const PointInputs& inputs,
        PointResult& result,
        PointSecondDerivativeMatrix& matrix) const override {
        const KernelStatus coverage =
            SecondDerivativeStatus(inputs.mask, RequiredMask(), SecondDerivativeMask());

        if (coverage != KernelStatus::kOk)
        {
            return coverage;
        }

        PointSecondDerivativeMatrix generated;

        if (_usesGradient)
        {
            _ggaSecondDerivative(inputs.Get(Component::RhoA), inputs.Get(Component::RhoB),
                                 inputs.Get(Component::SigmaAa), inputs.Get(Component::SigmaAb),
                                 inputs.Get(Component::SigmaBb), generated);
            matrix = RepackMatrix<kGgaTierComponents>(TierSlotsOf<kGgaTierComponents>(inputs.mask),
                                                      generated);
        } else
        {
            _ldaSecondDerivative(inputs.Get(Component::RhoA), inputs.Get(Component::RhoB),
                                 generated);
            matrix = RepackMatrix<kLdaTierComponents>(TierSlotsOf<kLdaTierComponents>(inputs.mask),
                                                      generated);
        }

        FoldIntoResult(Value(inputs), inputs.mask, result);
        return KernelStatus::kOk;
    }

private:
    /// The generated kernel's value at the point.
    [[nodiscard]] XcKernelValue Value(const PointInputs& inputs) const noexcept {
        if (_usesGradient)
        {
            return _gga(inputs.Get(Component::RhoA), inputs.Get(Component::RhoB),
                        inputs.Get(Component::SigmaAa), inputs.Get(Component::SigmaAb),
                        inputs.Get(Component::SigmaBb));
        }

        return _lda(inputs.Get(Component::RhoA), inputs.Get(Component::RhoB));
    }

    std::string_view _name;
    bool _usesGradient;
    LdaKernel _lda;
    GgaKernel _gga;
    LdaSecondDerivative _ldaSecondDerivative;
    GgaSecondDerivative _ggaSecondDerivative;
};

// The tau tier: the generated tau_x kernel and its second-derivative tier, packed
// over the kernel's seven argument slots and intersected with the caller's mask.
class TauTierFunctional final : public XcFunctional {
public:
    [[nodiscard]] bool UsesGradient() const override {
        return true;
    }

    [[nodiscard]] double ExchangeFraction() const override {
        return 0.0;
    }

    [[nodiscard]] ComponentMask RequiredMask() const override {
        return ComponentMask::Collinear();
    }

    [[nodiscard]] ComponentMask SecondDerivativeMask() const noexcept override {
        return TierMask(kTauTierComponents);
    }

    [[nodiscard]] KernelStatus EvaluatePoint(const PointInputs& inputs,
                                             PointResult& result) const override {
        FoldIntoResult(Value(inputs), inputs.mask, result);
        return KernelStatus::kOk;
    }

    [[nodiscard]] KernelStatus EvaluatePointWithSecondDerivatives(
        const PointInputs& inputs,
        std::span<const double> rightHandSide,
        PointResult& result,
        PointSecondDerivative& second) const override {
        const KernelStatus coverage =
            SecondDerivativeStatus(inputs.mask, RequiredMask(), SecondDerivativeMask());

        if (coverage != KernelStatus::kOk)
        {
            return coverage;
        }

        const TierSlots<kTauTierComponents> slots = TierSlotsOf<kTauTierComponents>(inputs.mask);

        PointSecondDerivativeMatrix generated;
        FillGeneratedMatrix(inputs, generated);

        const KernelStatus contracted =
            ContractFromMatrix<kTauTierComponents>(slots, generated, rightHandSide, second);

        if (contracted != KernelStatus::kOk)
        {
            return contracted;
        }

        FoldIntoResult(Value(inputs), inputs.mask, result);
        return KernelStatus::kOk;
    }

    [[nodiscard]] KernelStatus EvaluatePointMaterialising(
        const PointInputs& inputs,
        PointResult& result,
        PointSecondDerivativeMatrix& matrix) const override {
        const KernelStatus coverage =
            SecondDerivativeStatus(inputs.mask, RequiredMask(), SecondDerivativeMask());

        if (coverage != KernelStatus::kOk)
        {
            return coverage;
        }

        PointSecondDerivativeMatrix generated;
        FillGeneratedMatrix(inputs, generated);

        matrix = RepackMatrix<kTauTierComponents>(
            TierSlotsOf<kTauTierComponents>(inputs.mask), generated);

        FoldIntoResult(Value(inputs), inputs.mask, result);
        return KernelStatus::kOk;
    }

private:
    /// The generated kernel's value at the point.
    [[nodiscard]] static XcKernelValue Value(const PointInputs& inputs) noexcept {
        return TauXExchange(inputs.Get(Component::RhoA),
                            inputs.Get(Component::RhoB),
                            inputs.Get(Component::SigmaAa),
                            inputs.Get(Component::SigmaAb),
                            inputs.Get(Component::SigmaBb),
                            inputs.Get(Component::TauA),
                            inputs.Get(Component::TauB));
    }

    /// The generated matrix, in the generated tier's own packing.
    static void FillGeneratedMatrix(const PointInputs& inputs,
                                    PointSecondDerivativeMatrix& matrix) noexcept {
        TauXExchangeSecondDerivatives(inputs.Get(Component::RhoA),
                                      inputs.Get(Component::RhoB),
                                      inputs.Get(Component::SigmaAa),
                                      inputs.Get(Component::SigmaAb),
                                      inputs.Get(Component::SigmaBb),
                                      inputs.Get(Component::TauA),
                                      inputs.Get(Component::TauB),
                                      matrix);
    }
};

// The folding rule, checked by the compiler for every recipe below:
//     (LSDA-exchange weight) + (summed GGA-exchange weights) = 1 - exchangeFraction
// on FULL kernels, so a published LSDA coefficient is folded down by the GGA one.

/// The role a recipe term's kernel plays.  It travels WITH the weight because
/// the rule reads the two together.
enum class TermRole {
    kLdaExchange, ///< The LSDA exchange kernel - the base the GGA kernels contain.
    kGgaExchange, ///< A FULL GGA exchange kernel (contains its own LSDA part).
    kCorrelation, ///< Any correlation kernel: outside the folding rule.
};

/// The rule's tolerance: four orders above the double noise, eleven below the defect.
constexpr double kFoldingRuleTolerance = 1e-12;

/// The DFT exchange a recipe's terms carry: the exchange roles, never a correlation one.
/// \returns The summed exchange weight.
template <typename TermList> constexpr double DftExchangeWeight(const TermList& terms) noexcept {
    double dftExchange = 0.0;

    for (const auto& term : terms)
    {
        if (term.role != TermRole::kCorrelation)
        {
            dftExchange += term.weight;
        }
    }

    return dftExchange;
}

/// The folding rule, over the one term list the compiler checks and the functional
/// is built from.
/// \returns True when the exchange partition closes.
template <typename TermList>
constexpr bool FoldingRuleHolds(const TermList& terms, double exchangeFraction) noexcept {
    const double difference = DftExchangeWeight(terms) - (1.0 - exchangeFraction);

    return difference > -kFoldingRuleTolerance && difference < kFoldingRuleTolerance;
}

/// The construction-time half of the guard: a recipe reaching a hybrid's constructor
/// without its own static_assert is refused by name and aborted.
template <typename TermList>
[[noreturn]] void RefuseFoldedRecipe(std::string_view name,
                                     double exchangeFraction,
                                     const TermList& terms) noexcept {
    // The C `stderr` macro, not std::stderr: MSVC defines it as a function-like macro.
    std::fprintf(stderr,
                 "excgrid: the folding rule is violated by the hybrid '%.*s' - its DFT exchange "
                 "weights sum to %.17g, but a recipe declaring an exact-exchange fraction of "
                 "%.17g must sum to %.17g.  A published correction-form LSDA coefficient "
                 "multiplied onto a FULL GGA kernel double counts the LSDA exchange; see the "
                 "HybridFunctional note in src/kernels_registry.cpp.\n",
                 static_cast<int>(name.size()),
                 name.data(),
                 DftExchangeWeight(terms),
                 exchangeFraction,
                 1.0 - exchangeFraction);
    std::abort();
}

// A hybrid: a weighted sum of kernels plus the exact-exchange fraction.  B3LYP is
// published as 0.80 E_x^LSDA + 0.72 dE_x^B88, dE_x^B88 = E_x^B88 - E_x^LSDA; against
// this file's FULL kernels that is 0.08 LDA_X + 0.72 GGA_X_B88.
class HybridFunctional final : public XcFunctional {
public:
    /// One weighted kernel of a recipe, with its role and its second-derivative tier.
    struct Term {
        TermRole role;
        LdaKernel lda;
        GgaKernel gga;
        double weight;
        LdaSecondDerivative ldaSecondDerivative = nullptr;
        GgaSecondDerivative ggaSecondDerivative = nullptr;
    };

    template <std::size_t N>
    HybridFunctional(std::string_view name,
                     double exchangeFraction,
                     bool usesGradient,
                     const std::array<Term, N>& terms) noexcept :
        _name(name), _exchangeFraction(exchangeFraction), _usesGradient(usesGradient),
        _terms(terms.begin(), terms.end()) {
        if (!FoldingRuleHolds(terms, exchangeFraction))
        {
            RefuseFoldedRecipe(name, exchangeFraction, terms);
        }
    }

    [[nodiscard]] bool UsesGradient() const override {
        return _usesGradient;
    }

    [[nodiscard]] double ExchangeFraction() const override {
        return _exchangeFraction;
    }

    [[nodiscard]] ComponentMask RequiredMask() const override {
        ComponentMask mask;
        mask.Set(Component::RhoA);
        mask.Set(Component::RhoB);
        if (_usesGradient)
        {
            mask.Set(Component::SigmaAa);
            mask.Set(Component::SigmaAb);
            mask.Set(Component::SigmaBb);
        }
        return mask;
    }

    [[nodiscard]] KernelStatus EvaluatePoint(const PointInputs& inputs,
                                             PointResult& result) const override {
        const double rhoA = inputs.Get(Component::RhoA);
        const double rhoB = inputs.Get(Component::RhoB);
        const double sigmaAa = inputs.Get(Component::SigmaAa);
        const double sigmaAb = inputs.Get(Component::SigmaAb);
        const double sigmaBb = inputs.Get(Component::SigmaBb);

        XcKernelValue value;
        for (const Term& term : _terms)
        {
            const XcKernelValue part = term.gga != nullptr
                                           ? term.gga(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb)
                                           : term.lda(rhoA, rhoB);
            value += term.weight * part;
        }

        FoldIntoResult(value, inputs.mask, result);
        return KernelStatus::kOk;
    }

    /// The weighted sum of the terms' tiers, spanning the recipe only when every term has one.
    [[nodiscard]] ComponentMask SecondDerivativeMask() const noexcept override {
        if (!TierIsComplete())
        {
            return {};
        }

        return TierMask(Width());
    }

    [[nodiscard]] KernelStatus EvaluatePointWithSecondDerivatives(
        const PointInputs& inputs,
        std::span<const double> rightHandSide,
        PointResult& result,
        PointSecondDerivative& second) const override {
        const KernelStatus coverage =
            SecondDerivativeStatus(inputs.mask, RequiredMask(), SecondDerivativeMask());

        if (coverage != KernelStatus::kOk)
        {
            return coverage;
        }

        PointSecondDerivativeMatrix generated;
        FillGeneratedMatrix(inputs, generated);

        const KernelStatus contracted =
            Width() == kGgaTierComponents
                ? ContractFromMatrix<kGgaTierComponents>(
                      TierSlotsOf<kGgaTierComponents>(inputs.mask), generated, rightHandSide, second)
                : ContractFromMatrix<kLdaTierComponents>(
                      TierSlotsOf<kLdaTierComponents>(inputs.mask), generated, rightHandSide, second);

        if (contracted != KernelStatus::kOk)
        {
            return contracted;
        }

        FoldIntoResult(Value(inputs), inputs.mask, result);
        return KernelStatus::kOk;
    }

    [[nodiscard]] KernelStatus EvaluatePointMaterialising(
        const PointInputs& inputs,
        PointResult& result,
        PointSecondDerivativeMatrix& matrix) const override {
        const KernelStatus coverage =
            SecondDerivativeStatus(inputs.mask, RequiredMask(), SecondDerivativeMask());

        if (coverage != KernelStatus::kOk)
        {
            return coverage;
        }

        PointSecondDerivativeMatrix generated;
        FillGeneratedMatrix(inputs, generated);

        matrix = Width() == kGgaTierComponents
                     ? RepackMatrix<kGgaTierComponents>(TierSlotsOf<kGgaTierComponents>(inputs.mask),
                                                        generated)
                     : RepackMatrix<kLdaTierComponents>(TierSlotsOf<kLdaTierComponents>(inputs.mask),
                                                        generated);

        FoldIntoResult(Value(inputs), inputs.mask, result);
        return KernelStatus::kOk;
    }

private:
    /// The packing width: five for a GGA recipe, two for an LDA one.
    [[nodiscard]] std::size_t Width() const noexcept {
        return _usesGradient ? kGgaTierComponents : kLdaTierComponents;
    }

    /// Whether every term carries the tier its width needs; an LDA term in a GGA
    /// recipe needs its two-wide tier to place.
    [[nodiscard]] bool TierIsComplete() const noexcept {
        for (const Term& term : _terms)
        {
            const bool absent = term.gga != nullptr ? term.ggaSecondDerivative == nullptr
                                                    : term.ldaSecondDerivative == nullptr;

            if (absent)
            {
                return false;
            }
        }

        return !_terms.empty();
    }

    /// The generated kernel's value at the point.
    [[nodiscard]] XcKernelValue Value(const PointInputs& inputs) const noexcept {
        const double rhoA = inputs.Get(Component::RhoA);
        const double rhoB = inputs.Get(Component::RhoB);
        const double sigmaAa = inputs.Get(Component::SigmaAa);
        const double sigmaAb = inputs.Get(Component::SigmaAb);
        const double sigmaBb = inputs.Get(Component::SigmaBb);

        XcKernelValue value;
        for (const Term& term : _terms)
        {
            const XcKernelValue part = term.gga != nullptr
                                           ? term.gga(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb)
                                           : term.lda(rhoA, rhoB);
            value += term.weight * part;
        }

        return value;
    }

    /// The recipe's matrix, in the width's own packing, an LDA term's widened.
    void FillGeneratedMatrix(const PointInputs& inputs,
                             PointSecondDerivativeMatrix& generated) const noexcept {
        const double rhoA = inputs.Get(Component::RhoA);
        const double rhoB = inputs.Get(Component::RhoB);
        const double sigmaAa = inputs.Get(Component::SigmaAa);
        const double sigmaAb = inputs.Get(Component::SigmaAb);
        const double sigmaBb = inputs.Get(Component::SigmaBb);

        generated = {};

        for (const Term& term : _terms)
        {
            PointSecondDerivativeMatrix part;

            if (term.gga != nullptr)
            {
                term.ggaSecondDerivative(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb, part);
            } else
            {
                PointSecondDerivativeMatrix narrow;
                term.ldaSecondDerivative(rhoA, rhoB, narrow);
                part = Width() == kGgaTierComponents ? WidenLdaMatrix(narrow) : narrow;
            }

            const std::size_t entries = PackedIndex(Width() - 1, Width() - 1, Width()) + 1;

            for (std::size_t k = 0; k < entries; ++k)
            {
                generated.upper[k] += term.weight * part.upper[k];
            }
        }
    }

    std::string_view _name;
    double _exchangeFraction;
    bool _usesGradient;
    std::vector<Term> _terms;
};

// The shipped registry: name string to functional.
const PureFunctional kSlater("slater", false, &SlaterExchange, nullptr, &SlaterExchangeSecondDerivatives);
const PureFunctional kVwn5("vwn5", false, &Vwn5Correlation, nullptr,
                           &Vwn5CorrelationSecondDerivatives);
const PureFunctional kVwn3("vwn3", false, &Vwn3Correlation, nullptr,
                           &Vwn3CorrelationSecondDerivatives);
const PureFunctional kPw92("pw92", false, &Pw92Correlation, nullptr,
                           &Pw92CorrelationSecondDerivatives);

// Each hybrid: a named term list, the folding static_assert on that list, and the
// object built from it.
constexpr std::array<HybridFunctional::Term, 2> kSvwnTerms = {{
    {TermRole::kLdaExchange, &SlaterExchange, nullptr, 1.0, &SlaterExchangeSecondDerivatives},
    {TermRole::kCorrelation, &Vwn5Correlation, nullptr, 1.0,
     &Vwn5CorrelationSecondDerivatives},
}};
static_assert(FoldingRuleHolds(kSvwnTerms, 0.0),
              "svwn: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kSvwn("svwn", 0.0, false, kSvwnTerms);

constexpr std::array<HybridFunctional::Term, 2> kSpw92Terms = {{
    {TermRole::kLdaExchange, &SlaterExchange, nullptr, 1.0, &SlaterExchangeSecondDerivatives},
    {TermRole::kCorrelation, &Pw92Correlation, nullptr, 1.0,
     &Pw92CorrelationSecondDerivatives},
}};
static_assert(FoldingRuleHolds(kSpw92Terms, 0.0),
              "spw92: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kSpw92("spw92", 0.0, false, kSpw92Terms);

const PureFunctional kBecke88("becke88", true, nullptr, &Becke88Exchange, nullptr,
                             &Becke88ExchangeSecondDerivatives);
const PureFunctional kPw91x("pw91", true, nullptr, &Pw91Exchange, nullptr,
                            &Pw91ExchangeSecondDerivatives);
const PureFunctional kPbe("pbe", true, nullptr, &PbeExchange, nullptr,
                         &PbeExchangeSecondDerivatives);
const PureFunctional kRevPbe("revpbe", true, nullptr, &RevPbeExchange, nullptr,
                            &RevPbeExchangeSecondDerivatives);
const PureFunctional kRpbe("rpbe", true, nullptr, &RpbeExchange, nullptr,
                          &RpbeExchangeSecondDerivatives);
const PureFunctional kMPw91("mpw91", true, nullptr, &MPw91Exchange, nullptr,
                           &MPw91ExchangeSecondDerivatives);
const PureFunctional kPbeSol("pbesol", true, nullptr, &PbeSolExchange, nullptr,
                            &PbeSolExchangeSecondDerivatives);

const PureFunctional kLyp("lyp", true, nullptr, &LypCorrelation, nullptr,
                         &LypCorrelationSecondDerivatives);
const PureFunctional kPbeC("pbe_c", true, nullptr, &PbeCorrelation, nullptr,
                          &PbeCorrelationSecondDerivatives);
const PureFunctional kPw91c("pw91_c", true, nullptr, &Pw91Correlation, nullptr,
                           &Pw91CorrelationSecondDerivatives);
const PureFunctional kP86("p86", true, nullptr, &P86Correlation, nullptr,
                         &P86CorrelationSecondDerivatives);

// 0.08, not the published 0.80: the Slater weight folded against a FULL B88 kernel.
constexpr std::array<HybridFunctional::Term, 4> kB3LypTerms = {{
    {TermRole::kLdaExchange, &SlaterExchange, nullptr, 0.08, &SlaterExchangeSecondDerivatives},
    {TermRole::kGgaExchange, nullptr, &Becke88Exchange, 0.72, nullptr,
     &Becke88ExchangeSecondDerivatives},
    {TermRole::kCorrelation, &Vwn5Correlation, nullptr, 0.19,
     &Vwn5CorrelationSecondDerivatives},
    {TermRole::kCorrelation, nullptr, &LypCorrelation, 0.81, nullptr,
     &LypCorrelationSecondDerivatives},
}};
static_assert(FoldingRuleHolds(kB3LypTerms, 0.20),
              "b3lyp: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kB3Lyp("b3lyp", 0.20, true, kB3LypTerms);

constexpr std::array<HybridFunctional::Term, 2> kPbe0Terms = {{
    {TermRole::kGgaExchange, nullptr, &PbeExchange, 0.75, nullptr, &PbeExchangeSecondDerivatives},
    {TermRole::kCorrelation, nullptr, &PbeCorrelation, 1.0, nullptr,
     &PbeCorrelationSecondDerivatives},
}};
static_assert(FoldingRuleHolds(kPbe0Terms, 0.25),
              "pbe0: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kPbe0("pbe0", 0.25, true, kPbe0Terms);

constexpr std::array<HybridFunctional::Term, 4> kB3Pw91Terms = {{
    {TermRole::kLdaExchange, &SlaterExchange, nullptr, 0.08, &SlaterExchangeSecondDerivatives},
    {TermRole::kGgaExchange, nullptr, &Becke88Exchange, 0.72, nullptr,
     &Becke88ExchangeSecondDerivatives},
    {TermRole::kCorrelation, &Vwn5Correlation, nullptr, 0.19,
     &Vwn5CorrelationSecondDerivatives},
    {TermRole::kCorrelation, nullptr, &Pw91Correlation, 0.81, nullptr,
     &Pw91CorrelationSecondDerivatives},
}};
static_assert(FoldingRuleHolds(kB3Pw91Terms, 0.20),
              "b3pw91: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kB3Pw91("b3pw91", 0.20, true, kB3Pw91Terms);

constexpr std::array<HybridFunctional::Term, 2> kMPw1Pw91Terms = {{
    {TermRole::kGgaExchange, nullptr, &MPw91Exchange, 0.75, nullptr,
     &MPw91ExchangeSecondDerivatives},
    {TermRole::kCorrelation, nullptr, &Pw91Correlation, 1.0, nullptr,
     &Pw91CorrelationSecondDerivatives},
}};
static_assert(FoldingRuleHolds(kMPw1Pw91Terms, 0.25),
              "mpw1pw91: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kMPw1Pw91("mpw1pw91", 0.25, true, kMPw1Pw91Terms);

// BHandHLYP: 0.50 HF + 0.50 E_x^LSDA + 0.50 dE_x^B88 + E_c^LYP; the LSDA terms
// cancel against a FULL B88, leaving 0.50 becke88.
constexpr std::array<HybridFunctional::Term, 2> kBHandHLypTerms = {{
    {TermRole::kGgaExchange, nullptr, &Becke88Exchange, 0.50, nullptr,
     &Becke88ExchangeSecondDerivatives},
    {TermRole::kCorrelation, nullptr, &LypCorrelation, 1.0, nullptr,
     &LypCorrelationSecondDerivatives},
}};
static_assert(FoldingRuleHolds(kBHandHLypTerms, 0.50),
              "bhandhlyp: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kBHandHLyp("bhandhlyp", 0.50, true, kBHandHLypTerms);

constexpr std::array<HybridFunctional::Term, 4> kB3P86Terms = {{
    {TermRole::kLdaExchange, &SlaterExchange, nullptr, 0.08, &SlaterExchangeSecondDerivatives},
    {TermRole::kGgaExchange, nullptr, &Becke88Exchange, 0.72, nullptr,
     &Becke88ExchangeSecondDerivatives},
    {TermRole::kCorrelation, &Vwn5Correlation, nullptr, 0.19,
     &Vwn5CorrelationSecondDerivatives},
    {TermRole::kCorrelation, nullptr, &P86Correlation, 0.81, nullptr,
     &P86CorrelationSecondDerivatives},
}};
static_assert(FoldingRuleHolds(kB3P86Terms, 0.20),
              "b3p86: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
// Known open defect: the b3p86 composition leaves a 0.146 Ha residual against libxc 403.
const HybridFunctional kB3P86("b3p86", 0.20, true, kB3P86Terms);

// The tau tier: the only entry here that reads the kinetic-energy densities.
const TauTierFunctional kTauX;

const std::array<const XcFunctional*, 24> kRegistry = {
    &kSlater, &kVwn5,   &kVwn3, &kPw92,   &kSvwn,     &kSpw92,     &kBecke88, &kPw91x,
    &kPbe,    &kRevPbe, &kRpbe, &kMPw91,  &kPbeSol,   &kLyp,       &kPbeC,    &kPw91c,
    &kP86,    &kB3Lyp,  &kPbe0, &kB3Pw91, &kMPw1Pw91, &kBHandHLyp, &kB3P86,   &kTauX,
};

const std::array<std::string_view, kRegistry.size()> kNames = {
    "slater", "vwn5",   "vwn3", "pw92",   "svwn",     "spw92",     "becke88", "pw91",
    "pbe",    "revpbe", "rpbe", "mpw91",  "pbesol",   "lyp",       "pbe_c",   "pw91_c",
    "p86",    "b3lyp",  "pbe0", "b3pw91", "mpw1pw91", "bhandhlyp", "b3p86",   "tau_x",
};

} // namespace

const XcFunctional* FindFunctional(std::string_view name) noexcept {
    for (std::size_t i = 0; i < kRegistry.size(); ++i)
    {
        if (name == kNames[i])
        {
            return kRegistry[i];
        }
    }

    return nullptr;
}

std::span<const std::string_view> FunctionalNames() noexcept {
    return kNames;
}

} // namespace excgrid
