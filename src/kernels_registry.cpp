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

// The tau tier's reference functional.  Defined in src/meta_gga_reference.cpp
// rather than built here from a kernel pointer, because its kernel is wider than
// the two kernel types below: it reads the two kinetic-energy densities as well
// as the three gradient invariants.  Declared out here, at namespace scope: a
// declaration inside the anonymous namespace below would give the definition in
// that other translation unit internal linkage and fail at link time.
//
// The registry publishes the GENERATED tau-tier kernel instead (kTauX below).
// This hand-written reference stays the tier's independent route to the same
// expansion, and tests/registry_test.cpp cross-checks the generated kernel
// against it.
const XcFunctional& MetaGgaReferenceFunctional() noexcept;

// The tau tier's generated kernel and its second-derivative tier, both emitted
// from xc_defs/tau_x.ey into generated/tau_x.cpp.  Declared out here, at
// namespace scope, for the same reason as the reference above: the tier's
// kernel is wider than the two kernel types below, so the registry names it
// directly rather than through a function pointer.
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

// A pure functional: one generated kernel, no exact-exchange fraction.
class PureFunctional final : public XcFunctional {
public:
    PureFunctional(std::string_view name, bool usesGradient, LdaKernel lda, GgaKernel gga) noexcept
        : _name(name), _usesGradient(usesGradient), _lda(lda), _gga(gga) {}

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

private:
    std::string_view _name;
    bool _usesGradient;
    LdaKernel _lda;
    GgaKernel _gga;
};

// The components the generated tau tier is emitted over: the argument count of
// its kernel, and the width of the matrix packing its generated source fills.
constexpr std::size_t kTauComponents = 7;

// The tier is emitted over the seven collinear components, and its argument
// position is the identifier's own value - which is what lets a caller's mask be
// read onto it by counting.  Both are dependencies on the generated source, so
// both are checked where they can still be fixed: a widened collinear set or a
// reordered identifier table has to regenerate it.
static_assert(ComponentMask::Collinear().ActiveCount() == kTauComponents,
              "the tau tier's kernel is emitted over the seven collinear components; the collinear "
              "set has changed, so xc_defs/tau_x.ey must be regenerated against it");
static_assert(static_cast<std::size_t>(Component::RhoA) == 0 &&
                  static_cast<std::size_t>(Component::TauB) == kTauComponents - 1,
              "the tau tier's kernel argument order is the identifier order RhoA..TauB");
static_assert(kTauComponents <= kSecondDerivativeCapacity,
              "the tau tier's packing is wider than the contract's second-derivative capacity");

// The contract's upper-triangle index over `active` components in identifier
// order: entry (i, j), i <= j, at i*active - i*(i-1)/2 + (j-i).
[[nodiscard]] constexpr std::size_t PackedIndex(std::size_t i,
                                                std::size_t j,
                                                std::size_t active) noexcept {
    return i * active - i * (i - 1) / 2 + (j - i);
}

// The caller's mask restricted to the components the generated tier reads, with
// the kernel argument slot of each of its active components.
struct TauSlots {
    ComponentMask mask{}; ///< The intersection: what a result or matrix is over.
    std::size_t active = 0; ///< Its active-component count.
    std::array<std::size_t, kTauComponents> slot{}; ///< Kernel slot, per active component.
};

// Reads a caller's mask onto the generated kernel's argument order.  The
// intersection is always a subsequence of the kernel's seven, in the same
// relative order, so the packed positions of one are the packed positions of the
// other once the dropped slots are counted out.
[[nodiscard]] constexpr TauSlots TauSlotsOf(const ComponentMask& caller) noexcept {
    TauSlots slots;

    for (std::size_t component = 0; component < kTauComponents; ++component)
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

// One entry of the generated tier's matrix, by kernel argument slots.  The
// generated packing carries the upper triangle only, and a Hessian is
// symmetric, so the pair is read at (min, max).
[[nodiscard]] constexpr double GeneratedEntry(const PointSecondDerivativeMatrix& matrix,
                                              std::size_t a,
                                              std::size_t b) noexcept {
    return matrix.upper[PackedIndex(a < b ? a : b, a < b ? b : a, kTauComponents)];
}

// The tau tier: the generated kernel behind the registry's tau_x, and the
// second-derivative tier the same generated source emits beside it.
//
// A meta-GGA reads two components a GGA does not, so it is neither of the two
// kernel types above and is not built from one - the kernel is named directly.
//
// The generated matrix packs over the kernel's own seven argument slots; the
// contract packs over the CALLER's mask.  The two are reconciled here rather
// than by asking the caller for a particular mask: an input the caller leaves
// out is not supplied, the contract reads it as zero, and its row and column
// are consequently dropped.  A result or matrix therefore carries the
// intersection of the caller's mask with the seven, which is what its own mask
// field then says.
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
        const TauSlots slots = TauSlotsOf(inputs.mask);

        // One value per active component per right-hand side has to fit the
        // contract's array, and an empty active set has no layout to divide by.
        if (slots.active == 0 || rightHandSide.size() % slots.active != 0)
        {
            return KernelStatus::kRefusedUnsupportedCombination;
        }

        if (rightHandSide.size() > kSecondDerivativeCapacity)
        {
            return KernelStatus::kRefusedExhaustedCapacity;
        }

        PointSecondDerivativeMatrix generated;
        FillGeneratedMatrix(inputs, generated);

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
                    contracted += GeneratedEntry(generated, slots.slot[i], slots.slot[j]) *
                                  rightHandSide[side * slots.active + j];
                }

                second.contracted[side * slots.active + i] = contracted;
            }
        }

        FoldIntoResult(Value(inputs), inputs.mask, result);
        return KernelStatus::kOk;
    }

    [[nodiscard]] KernelStatus EvaluatePointMaterialising(
        const PointInputs& inputs,
        PointResult& result,
        PointSecondDerivativeMatrix& matrix) const override {
        const TauSlots slots = TauSlotsOf(inputs.mask);

        PointSecondDerivativeMatrix generated;
        FillGeneratedMatrix(inputs, generated);

        // Nothing of the caller's buffer survives, so an entry the tier has no
        // value for is a zero rather than a leftover.
        matrix = {};
        matrix.mask = slots.mask;

        for (std::size_t i = 0; i < slots.active; ++i)
        {
            for (std::size_t j = i; j < slots.active; ++j)
            {
                matrix.upper[PackedIndex(i, j, slots.active)] =
                    GeneratedEntry(generated, slots.slot[i], slots.slot[j]);
            }
        }

        FoldIntoResult(Value(inputs), inputs.mask, result);
        return KernelStatus::kOk;
    }

private:
    /// The generated kernel's value at the point; the caller's mask decides
    /// which components arrive, and an unsupplied one arrives as zero.
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

// ---------------------------------------------------------------------------
// The folding rule
// ---------------------------------------------------------------------------
//
// THE RULE, checked below by the COMPILER for every hybrid of the registry and
// again by the constructor that builds it:
//
//     (LSDA-exchange weight) + (summed GGA-exchange weights) = 1 - exchangeFraction
//
// A published formula's coefficient can multiply one of two different things -
// the CORRECTION a GGA adds to the LSDA exchange, or the FULL kernel that
// already contains it - and the published number is different for each.  Every
// kernel in this file is the FULL functional, so the weight belonging on
// SlaterExchange is the published LSDA coefficient MINUS the GGA-exchange
// coefficient, and what the exact-exchange fraction leaves of the exchange is
// the sum the two must come to.  The HybridFunctional note below carries the
// arithmetic and the measured 5.953 Ha of the defect this catches.
//
// Why the check lives here and not only in the suite: the registry's own tests
// (tests/registry_test.cpp, RegistryFoldingGuard) check the same rule a second
// time, numerically, from each entry's behaviour - but this file is compiled
// into EVERY consumer that links excgrid, and the test suite defaults OFF for
// them, so a broken recipe is a build failure rather than a test somebody has
// to remember to run.

/// The role a recipe term's kernel plays.  It travels WITH the weight because
/// the rule reads the two together: a function pointer does not say whether
/// its kernel is an exchange kernel, and only an exchange kernel that already
/// carries its own LSDA part can hide a correction-form coefficient.
enum class TermRole {
    kLdaExchange, ///< The LSDA exchange kernel - the base the GGA kernels contain.
    kGgaExchange, ///< A FULL GGA exchange kernel (contains its own LSDA part).
    kCorrelation, ///< Any correlation kernel: outside the folding rule.
};

/// The rule's tolerance.  Measured over the eight shipped hybrids on
/// 2026-09-16: the weights are literal doubles, so the sum misses
/// 1 - exchangeFraction by at most 1.11e-16 (b3lyp, b3pw91 and b3p86, whose
/// 0.08 + 0.72 lands one ulp below 0.8), while the defect moves that same sum
/// by 0.72.  1e-12 sits four orders above the noise and eleven below the
/// defect.
constexpr double kFoldingRuleTolerance = 1e-12;

/// The DFT exchange a recipe's terms carry: the exchange roles, never a
/// correlation one.
/// \param terms The recipe's terms.
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

/// The folding rule, generic over the term list so that the terms the compiler
/// checks and the terms the functional is built from are ONE list: a rule
/// reading a list of its own would be a restatement of the recipe, not a check
/// on it.
/// \param terms The recipe's terms.
/// \param exchangeFraction The exact-exchange fraction the recipe declares.
/// \returns True when the exchange partition closes.
template <typename TermList>
constexpr bool FoldingRuleHolds(const TermList& terms, double exchangeFraction) noexcept {
    const double difference = DftExchangeWeight(terms) - (1.0 - exchangeFraction);

    return difference > -kFoldingRuleTolerance && difference < kFoldingRuleTolerance;
}

/// The construction-time half of the guard: an entry reaching a hybrid's
/// constructor without its own static_assert is REFUSED rather than computed.
/// A registry fault is not something a caller can recover from - a wrong
/// coefficient still converges and still looks physical, which is how the
/// defect behind this rule survived every self-consistency check the repo
/// could run - so the refusal names the entry, prints the arithmetic and
/// aborts.
/// \param name The recipe's shipped name.
/// \param exchangeFraction The exact-exchange fraction the recipe declares.
/// \param terms The recipe's terms.
template <typename TermList>
[[noreturn]] void RefuseFoldedRecipe(std::string_view name,
                                     double exchangeFraction,
                                     const TermList& terms) noexcept {
    // `stderr` is the C macro rather than std::stderr on purpose: MSVC defines
    // it as a function-like macro, so the qualified spelling does not compile.
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

// A hybrid: a weighted sum of component kernels plus the exact-exchange
// fraction the consumer routes through its HF Fock path.  The recipes
// are the published compositions (docs/kernel-api.md section 4); B3LYP
// uses VWN5, the modern majority convention, per the VWN3/VWN5
// landmine note.
//
// THE LDA-EXCHANGE WEIGHT IS NOT THE PUBLISHED ONE, and the arithmetic is
// the reason.  The B3LYP family is published as
//
//     0.80 E_x^LSDA + 0.72 dE_x^B88,   dE_x^B88 = E_x^B88 - E_x^LSDA,
//
// so the published 0.72 multiplies the B88 CORRECTION.  Every kernel in
// this file is the FULL functional instead: Becke88Exchange returns B88
// including its LDA part (whole-SCF, it agrees with libxc's gga_x_b88 to
// 2.8e-06).  That is not an accident of this file - it is the convention
// the single-component names ship under, which is what makes `becke88`
// on its own a usable functional.  Pairing a FULL kernel with the
// published 0.72 therefore requires the Slater term folded in as
// 0.80 - 0.72 = 0.08, which is how libxc writes the identical recipe
// (libxc's B3LYP resolves to 0.08 LDA_X + 0.72 GGA_X_B88).  Shipping
// 0.80 double-counted a whole extra 0.72 E_x^LSDA: measured at 5.953 Ha
// on H2O/STO-3G against pyscf's b3lyp5, and reproduced to 3.5e-06 by
// running pyscf with THIS file's weights (scf/tests/rks_convergence_
// probe_test.cpp, HybridFamilyIsolatesTheOffendingTerm).
class HybridFunctional final : public XcFunctional {
public:
    /// One weighted kernel of a recipe.  The role travels WITH the weight: the
    /// folding rule reads them together, and the constructor below refuses a
    /// recipe whose exchange partition does not close.
    struct Term {
        TermRole role;
        LdaKernel lda;
        GgaKernel gga;
        double weight;
    };

    /// The terms arrive as an array rather than an initializer list so that
    /// the very same object can be handed to the static_assert above each
    /// registry entry, which is what makes the compile-time check and the
    /// shipped recipe one fact instead of two.
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

private:
    std::string_view _name;
    double _exchangeFraction;
    bool _usesGradient;
    std::vector<Term> _terms;
};

// The shipped registry (name string -> functional).  The names are the
// consumer-facing schema strings (never an enum - enums drift with repo
// releases).
const PureFunctional kSlater("slater", false, &SlaterExchange, nullptr);
const PureFunctional kVwn5("vwn5", false, &Vwn5Correlation, nullptr);
const PureFunctional kVwn3("vwn3", false, &Vwn3Correlation, nullptr);
const PureFunctional kPw92("pw92", false, &Pw92Correlation, nullptr);

// Every hybrid below is written as this three-step shape - a named constexpr
// term list, the static_assert that the folding rule closes on it, and the
// object built from that same list - so a recipe's terms exist once and the
// compiler checks them.  The constructor refuses a list that reaches it
// without an assert, so this is a shape to copy, not a convention to remember.
constexpr std::array<HybridFunctional::Term, 2> kSvwnTerms = {{
    {TermRole::kLdaExchange, &SlaterExchange, nullptr, 1.0},
    {TermRole::kCorrelation, &Vwn5Correlation, nullptr, 1.0},
}};
static_assert(FoldingRuleHolds(kSvwnTerms, 0.0),
              "svwn: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kSvwn("svwn", 0.0, false, kSvwnTerms);

constexpr std::array<HybridFunctional::Term, 2> kSpw92Terms = {{
    {TermRole::kLdaExchange, &SlaterExchange, nullptr, 1.0},
    {TermRole::kCorrelation, &Pw92Correlation, nullptr, 1.0},
}};
static_assert(FoldingRuleHolds(kSpw92Terms, 0.0),
              "spw92: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kSpw92("spw92", 0.0, false, kSpw92Terms);

const PureFunctional kBecke88("becke88", true, nullptr, &Becke88Exchange);
const PureFunctional kPw91x("pw91", true, nullptr, &Pw91Exchange);
const PureFunctional kPbe("pbe", true, nullptr, &PbeExchange);
const PureFunctional kRevPbe("revpbe", true, nullptr, &RevPbeExchange);
const PureFunctional kRpbe("rpbe", true, nullptr, &RpbeExchange);
const PureFunctional kMPw91("mpw91", true, nullptr, &MPw91Exchange);
const PureFunctional kPbeSol("pbesol", true, nullptr, &PbeSolExchange);

const PureFunctional kLyp("lyp", true, nullptr, &LypCorrelation);
const PureFunctional kPbeC("pbe_c", true, nullptr, &PbeCorrelation);
const PureFunctional kPw91c("pw91_c", true, nullptr, &Pw91Correlation);
const PureFunctional kP86("p86", true, nullptr, &P86Correlation);

// 0.08, not the published 0.80: the Slater term is folded in against a
// FULL B88 kernel - see the HybridFunctional note above for the arithmetic.
constexpr std::array<HybridFunctional::Term, 4> kB3LypTerms = {{
    {TermRole::kLdaExchange, &SlaterExchange, nullptr, 0.08},
    {TermRole::kGgaExchange, nullptr, &Becke88Exchange, 0.72},
    {TermRole::kCorrelation, &Vwn5Correlation, nullptr, 0.19},
    {TermRole::kCorrelation, nullptr, &LypCorrelation, 0.81},
}};
static_assert(FoldingRuleHolds(kB3LypTerms, 0.20),
              "b3lyp: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kB3Lyp("b3lyp", 0.20, true, kB3LypTerms);

constexpr std::array<HybridFunctional::Term, 2> kPbe0Terms = {{
    {TermRole::kGgaExchange, nullptr, &PbeExchange, 0.75},
    {TermRole::kCorrelation, nullptr, &PbeCorrelation, 1.0},
}};
static_assert(FoldingRuleHolds(kPbe0Terms, 0.25),
              "pbe0: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kPbe0("pbe0", 0.25, true, kPbe0Terms);

constexpr std::array<HybridFunctional::Term, 4> kB3Pw91Terms = {{
    {TermRole::kLdaExchange, &SlaterExchange, nullptr, 0.08},
    {TermRole::kGgaExchange, nullptr, &Becke88Exchange, 0.72},
    {TermRole::kCorrelation, &Vwn5Correlation, nullptr, 0.19},
    {TermRole::kCorrelation, nullptr, &Pw91Correlation, 0.81},
}};
static_assert(FoldingRuleHolds(kB3Pw91Terms, 0.20),
              "b3pw91: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kB3Pw91("b3pw91", 0.20, true, kB3Pw91Terms);

constexpr std::array<HybridFunctional::Term, 2> kMPw1Pw91Terms = {{
    {TermRole::kGgaExchange, nullptr, &MPw91Exchange, 0.75},
    {TermRole::kCorrelation, nullptr, &Pw91Correlation, 1.0},
}};
static_assert(FoldingRuleHolds(kMPw1Pw91Terms, 0.25),
              "mpw1pw91: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kMPw1Pw91("mpw1pw91", 0.25, true, kMPw1Pw91Terms);

// The published BHandHLYP is 0.50 HF + 0.50 E_x^LSDA + 0.50 dE_x^B88 +
// E_c^LYP.  Against a FULL B88 the two LSDA terms cancel outright, so the
// exchange legs are 0.50 becke88 and nothing else - which is what libxc's
// BHandHLYP resolves to (measured identical to `0.5*HF + 0.5*B88 + LYP`).
constexpr std::array<HybridFunctional::Term, 2> kBHandHLypTerms = {{
    {TermRole::kGgaExchange, nullptr, &Becke88Exchange, 0.50},
    {TermRole::kCorrelation, nullptr, &LypCorrelation, 1.0},
}};
static_assert(FoldingRuleHolds(kBHandHLypTerms, 0.50),
              "bhandhlyp: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kBHandHLyp("bhandhlyp", 0.50, true, kBHandHLypTerms);

// b3p86 carries a NAMED OPEN DEFECT of its own - an unattributed 0.146 Ha
// residual against libxc 403, one the folding rule cannot see and does
// not speak to.
constexpr std::array<HybridFunctional::Term, 4> kB3P86Terms = {{
    {TermRole::kLdaExchange, &SlaterExchange, nullptr, 0.08},
    {TermRole::kGgaExchange, nullptr, &Becke88Exchange, 0.72},
    {TermRole::kCorrelation, &Vwn5Correlation, nullptr, 0.19},
    {TermRole::kCorrelation, nullptr, &P86Correlation, 0.81},
}};
static_assert(FoldingRuleHolds(kB3P86Terms, 0.20),
              "b3p86: the folding rule is violated - the DFT exchange weights must sum to "
              "1 - exchangeFraction, with the LSDA weight FOLDED against the full GGA kernels");
const HybridFunctional kB3P86("b3p86", 0.20, true, kB3P86Terms);

// The tau tier: the generator's first tau-reading kernel, and the only entry
// here that reads the kinetic-energy densities.  It is the second-order gradient
// expansion of exchange in the iso-orbital variables, emitted from
// xc_defs/tau_x.ey; its exact conditions and its finite-difference checks live
// with it (tests/meta_gga_reference_test.cpp), and its second-derivative tier
// with the registry (tests/registry_test.cpp).  The hand-written functional that
// proved the tau slot (src/meta_gga_reference.cpp) stays reachable beside it.
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
