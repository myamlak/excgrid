#include "excgrid/kernel.hpp"
#include "excgrid/kernels.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <string_view>
#include <vector>

namespace excgrid {

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

    XcKernelValue Evaluate(
        double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) const override {
        if (_usesGradient)
        {
            return _gga(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb);
        }

        return _lda(rhoA, rhoB);
    }

private:
    std::string_view _name;
    bool _usesGradient;
    LdaKernel _lda;
    GgaKernel _gga;
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

    XcKernelValue Evaluate(
        double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) const override {
        XcKernelValue result;

        for (const Term& term : _terms)
        {
            const XcKernelValue part = term.gga != nullptr
                                           ? term.gga(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb)
                                           : term.lda(rhoA, rhoB);
            result += term.weight * part;
        }

        return result;
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

const std::array<const XcFunctional*, 23> kRegistry = {
    &kSlater, &kVwn5,   &kVwn3, &kPw92,   &kSvwn,     &kSpw92,     &kBecke88, &kPw91x,
    &kPbe,    &kRevPbe, &kRpbe, &kMPw91,  &kPbeSol,   &kLyp,       &kPbeC,    &kPw91c,
    &kP86,    &kB3Lyp,  &kPbe0, &kB3Pw91, &kMPw1Pw91, &kBHandHLyp, &kB3P86,
};

const std::array<std::string_view, kRegistry.size()> kNames = {
    "slater", "vwn5",   "vwn3", "pw92",   "svwn",     "spw92",     "becke88", "pw91",
    "pbe",    "revpbe", "rpbe", "mpw91",  "pbesol",   "lyp",       "pbe_c",   "pw91_c",
    "p86",    "b3lyp",  "pbe0", "b3pw91", "mpw1pw91", "bhandhlyp", "b3p86",
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
