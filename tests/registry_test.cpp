// The functional registry tests: name resolution, the shipped-name
// surface, hybrid recipes and exchange fractions, the VWN3/VWN5
// separation, the exchange-folding guard - the class guard against
// the B3LYP Slater double count fixed in 4bddaceb coming back through a
// future entry - and the tau tier's generated kernel with its generated
// second-derivative matrix.

#include "excgrid/kernel.hpp"
#include "excgrid/kernels.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>
#include <initializer_list>
#include <span>
#include <string_view>
#include <vector>

// The tau tier's hand-written reference, defined in src/meta_gga_reference.cpp.
// Declared here at namespace scope, OUTSIDE the anonymous namespace below: a
// namespace nested inside an unnamed namespace has internal linkage, so a
// declaration there would name a different function than the library defines and
// fail at link time.
namespace excgrid {
const XcFunctional& MetaGgaReferenceFunctional() noexcept;
} // namespace excgrid

namespace {

TEST(RegistryTest, EveryNameResolves) {
    for (std::string_view name : excgrid::FunctionalNames())
    {
        EXPECT_NE(excgrid::FindFunctional(name), nullptr) << name;
    }

    EXPECT_EQ(excgrid::FindFunctional("nosuch"), nullptr);
}

TEST(RegistryTest, ShippedSurface) {
    const std::vector<std::string_view> names(excgrid::FunctionalNames().begin(),
                                              excgrid::FunctionalNames().end());

    EXPECT_EQ(names.size(), 24);

    for (const char* expected :
         {"slater", "vwn5",   "vwn3", "pw92",   "svwn",     "spw92",     "becke88", "pw91",
          "pbe",    "revpbe", "rpbe", "mpw91",  "pbesol",   "lyp",       "pbe_c",   "pw91_c",
          "p86",    "b3lyp",  "pbe0", "b3pw91", "mpw1pw91", "bhandhlyp", "b3p86",   "tau_x"})
    {
        EXPECT_NE(std::find(names.begin(), names.end(), expected), names.end()) << expected;
    }
}

TEST(RegistryTest, HybridRecipes) {
    // B3LYP's Slater weight is 0.08, NOT the published 0.80: the published
    // form multiplies the B88 CORRECTION, while Becke88Exchange is the full
    // B88, so the Slater term folds in as 0.80 - 0.72 = 0.08. This assertion
    // moved 0.80 -> 0.08 on 2026-09-13 with the fix (5.953 Ha on H2O/STO-3G
    // against pyscf's b3lyp5); see kernels_registry.cpp's HybridFunctional
    // note and docs/kernel-api.md section 4. Everything else is unchanged:
    // B3LYP = 0.08 slater + 0.72 becke88 + 0.19 VWN5 + 0.81 lyp with an
    // HF fraction of 0.20; the VWN5 choice is the documented convention.
    const excgrid::XcFunctional* b3lyp = excgrid::FindFunctional("b3lyp");

    ASSERT_NE(b3lyp, nullptr);
    EXPECT_TRUE(b3lyp->UsesGradient());
    EXPECT_DOUBLE_EQ(b3lyp->ExchangeFraction(), 0.20);

    const double rhoA = 0.3;
    const double rhoB = 0.22;
    const double sigmaAa = 0.4;
    const double sigmaAb = 0.15;
    const double sigmaBb = 0.25;

    const excgrid::XcKernelValue value = b3lyp->Evaluate(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb);
    const excgrid::XcKernelValue expect =
        0.08 * excgrid::SlaterExchange(rhoA, rhoB) +
        0.72 * excgrid::Becke88Exchange(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb) +
        0.19 * excgrid::Vwn5Correlation(rhoA, rhoB) +
        0.81 * excgrid::LypCorrelation(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb);

    EXPECT_NEAR(value.exc, expect.exc, 1e-14);
    EXPECT_NEAR(value.vrhoA, expect.vrhoA, 1e-14);
    EXPECT_NEAR(value.vsigmaBb, expect.vsigmaBb, 1e-14);
}

TEST(RegistryTest, HybridExchangeFractions) {
    EXPECT_DOUBLE_EQ(excgrid::FindFunctional("pbe0")->ExchangeFraction(), 0.25);
    EXPECT_DOUBLE_EQ(excgrid::FindFunctional("b3pw91")->ExchangeFraction(), 0.20);
    EXPECT_DOUBLE_EQ(excgrid::FindFunctional("mpw1pw91")->ExchangeFraction(), 0.25);
    EXPECT_DOUBLE_EQ(excgrid::FindFunctional("bhandhlyp")->ExchangeFraction(), 0.50);
    EXPECT_DOUBLE_EQ(excgrid::FindFunctional("b3p86")->ExchangeFraction(), 0.20);
    EXPECT_DOUBLE_EQ(excgrid::FindFunctional("slater")->ExchangeFraction(), 0.0);
}

TEST(RegistryTest, Vwn3AndVwn5AreDistinct) {
    // The landmine note: VWN3 and VWN5 are different parametrizations
    // and must never be conflated - their energies genuinely differ.
    const excgrid::XcKernelValue vwn3 = excgrid::Vwn3Correlation(0.4, 0.4);
    const excgrid::XcKernelValue vwn5 = excgrid::Vwn5Correlation(0.4, 0.4);

    EXPECT_GT(std::abs(vwn3.exc - vwn5.exc), 1e-4);
}

TEST(RegistryTest, LdaComposites) {
    const excgrid::XcFunctional* svwn = excgrid::FindFunctional("svwn");
    const excgrid::XcFunctional* spw92 = excgrid::FindFunctional("spw92");

    ASSERT_NE(svwn, nullptr);
    ASSERT_NE(spw92, nullptr);
    EXPECT_FALSE(svwn->UsesGradient());

    const excgrid::XcKernelValue value = svwn->Evaluate(0.3, 0.2, 0.0, 0.0, 0.0);
    const excgrid::XcKernelValue expect =
        excgrid::SlaterExchange(0.3, 0.2) + excgrid::Vwn5Correlation(0.3, 0.2);

    EXPECT_NEAR(value.exc, expect.exc, 1e-14);
}

// ---------------------------------------------------------------------------
// The exchange-folding guard
// ---------------------------------------------------------------------------
//
// excgrid/src/kernels_registry.cpp stores FULL kernels while the hybrid
// weights come from formulas published against CORRECTION functionals.  The
// B3LYP family is published as
//
//     0.80 E_x^LSDA + 0.72 dE_x^B88,   dE_x^B88 = E_x^B88 - E_x^LSDA,
//
// and Becke88Exchange is the FULL B88 - it already contains the LSDA
// exchange - so the published 0.72 only translates to the registry's kernels
// once the Slater term is folded to 0.80 - 0.72 = 0.08.  Shipping 0.80
// counted the LSDA exchange twice: 5.953 Ha on H2O/STO-3G against pyscf's
// b3lyp5 (fixed 4bddaceb; the arithmetic is the HybridFunctional note in
// kernels_registry.cpp, and docs/kernel-api.md section 4 is the recipe
// documentation).
//
// This guard fixtures that RULE rather than the four entries that broke it -
// b3lyp, b3pw91, b3p86 and bhandhlyp, the four the sweep found:
//
//     a recipe's DFT exchange is split exactly once, so
//
//         (LSDA exchange weight) + (summed GGA exchange weights)
//             = 1 - ExchangeFraction()
//
//     which is the folding rule: the LSDA weight must be the published LSDA
//     coefficient minus the GGA-exchange coefficient, the published
//     coefficient being what the exact-exchange fraction leaves of the LSDA
//     exchange.
//
// It is structural rather than enumerated.  It walks FunctionalNames() and
// reads no recipe: the weights are RECOVERED numerically from Evaluate()
// alone, by least squares against the shipped kernel universe below.  A
// hybrid added later is checked by the same arithmetic - there is no table of
// names or of coefficient values to extend - and the check fails CLOSED: an
// entry whose terms are not in the span of that universe (to ~1e-9 relative,
// the measured fit residual) fails instead of being skipped, so a kernel this
// guard has never seen cannot pass through it unnoticed.
//
// What it deliberately does NOT do: pin the value of any single coefficient
// (only the partition sum, which the folding rule fixes), restate
// RegistryTest.HybridRecipes, or require a particular implementation shape.
//
// The same rule is checked a second time where the recipes live:
// excgrid/src/kernels_registry.cpp carries every hybrid's terms in a constexpr
// list with a static_assert beside it and a constructor that refuses a list
// reaching it without one.  That check is the stronger of the two in REACH -
// the registry compiles into every build that links excgrid, while this suite
// runs only where the excgrid tests are built - and this one is the stronger
// in EVIDENCE: it reads the behaviour of the shipped object rather than the
// list its author wrote.
//
// Blind spot, stated plainly: this API does not say whether a GGA kernel is
// the full functional or a correction form, so a future correction-form
// kernel paired with an unfolded LSDA term would still pass.  The registry's
// documented convention - every kernel is full - is the premise here, and
// that provenance would have to come from a registry surface change.

/// The role a shipped kernel plays in a recipe.  A property of the kernel,
/// never of a functional.
enum class KernelRole { LdaExchange, GgaExchange, LdaCorrelation, GgaCorrelation };

constexpr std::size_t kKernelCount = 15;
constexpr std::size_t kProbeComponentCount = 6;
constexpr std::size_t kProbeDensityCount = 6;
constexpr std::size_t kProbeGradientCount = 6;
constexpr std::size_t kProbePointCount = kProbeDensityCount * kProbeGradientCount;

/// The recovery's accuracy was measured over all 22 shipped entries on
/// 2026-09-13: fit residuals 5e-15 to 1.7e-12 (relative), and the recovered
/// exchange partition summing to within 1.2e-12 of the required value, while
/// the folding defect moves that sum by 0.72.  Both thresholds therefore sit
/// three orders above the measured noise and six below the defect.  Neither
/// is a claim about any coefficient's value: they bound the arithmetic, not
/// the recipe.
constexpr double kWeightTolerance = 1e-6;
constexpr double kFitTolerance = 1e-9;

/// One kernel of the shipped universe (excgrid/kernels.hpp), with its role.
struct UniverseKernel {
    std::string_view name;
    KernelRole role;
    excgrid::LdaKernel lda;
    excgrid::GgaKernel gga;

    [[nodiscard]] excgrid::XcKernelValue Evaluate(
        double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) const {
        if (gga != nullptr)
        {
            return gga(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb);
        }

        return lda(rhoA, rhoB);
    }
};

/// Every function declared in excgrid/kernels.hpp.  This is the guard's only
/// enumeration, and it is an enumeration of KERNELS - a fact of the header -
/// not of recipes or of coefficient values.  A functional whose terms do not
/// lie in the span of this set fails the fit residual, which is the intended
/// reading: the guard cannot see that entry, so it must not pass it silently.
const std::array<UniverseKernel, kKernelCount> kKernelUniverse = {{
    {"slater", KernelRole::LdaExchange, &excgrid::SlaterExchange, nullptr},
    {"vwn5", KernelRole::LdaCorrelation, &excgrid::Vwn5Correlation, nullptr},
    {"vwn3", KernelRole::LdaCorrelation, &excgrid::Vwn3Correlation, nullptr},
    {"pw92", KernelRole::LdaCorrelation, &excgrid::Pw92Correlation, nullptr},
    {"becke88", KernelRole::GgaExchange, nullptr, &excgrid::Becke88Exchange},
    {"pw91", KernelRole::GgaExchange, nullptr, &excgrid::Pw91Exchange},
    {"pbe", KernelRole::GgaExchange, nullptr, &excgrid::PbeExchange},
    {"revpbe", KernelRole::GgaExchange, nullptr, &excgrid::RevPbeExchange},
    {"rpbe", KernelRole::GgaExchange, nullptr, &excgrid::RpbeExchange},
    {"mpw91", KernelRole::GgaExchange, nullptr, &excgrid::MPw91Exchange},
    {"pbesol", KernelRole::GgaExchange, nullptr, &excgrid::PbeSolExchange},
    {"lyp", KernelRole::GgaCorrelation, nullptr, &excgrid::LypCorrelation},
    {"pbe_c", KernelRole::GgaCorrelation, nullptr, &excgrid::PbeCorrelation},
    {"pw91_c", KernelRole::GgaCorrelation, nullptr, &excgrid::Pw91Correlation},
    {"p86", KernelRole::GgaCorrelation, nullptr, &excgrid::P86Correlation},
}};

/// Probe points: spin-density and gradient patterns chosen so that no two
/// kernels of the universe are collinear on the set - the six gradient
/// patterns separate the gradient kernels, the six density patterns the
/// spin-density ones.  The recovery measures its own residual, so a
/// degenerate probe set fails the guard loudly rather than weakening it
/// silently.
///
/// The near-LDA-limit pattern is 1e-12 rather than a clean 0 on purpose: at
/// EXACTLY zero gradient the shipped GGA kernels return NaN in their vsigma
/// components (measured 2026-09-13: 9 of the 10 GGA kernels, LYP excepted;
/// at sigma = 1e-300 they already return the finite limit).  That is a
/// property of the kernels, not of this guard, and the finiteness check in
/// RecoverKernelWeights would otherwise report it as an unusable probe set.
const std::array<std::array<double, 2>, kProbeDensityCount> kProbeDensities = {{
    {0.4, 0.4},
    {0.65, 0.15},
    {0.09, 0.09},
    {1.7, 0.25},
    {0.02, 0.018},
    {3.2, 0.7},
}};

const std::array<std::array<double, 3>, kProbeGradientCount> kProbeGradients = {{
    {1e-12, 1e-13, 1e-12},
    {0.004, 0.001, 0.003},
    {0.09, 0.02, 0.06},
    {0.8, 0.15, 0.55},
    {7.0, 1.2, 4.5},
    {50.0, 7.0, 22.0},
}};

std::array<std::array<double, 5>, kProbePointCount> MakeProbePoints() {
    std::array<std::array<double, 5>, kProbePointCount> points{};
    std::size_t index = 0;

    for (const std::array<double, 2>& density : kProbeDensities)
    {
        for (const std::array<double, 3>& gradient : kProbeGradients)
        {
            points[index] = {density[0], density[1], gradient[0], gradient[1], gradient[2]};
            ++index;
        }
    }

    return points;
}

const std::array<std::array<double, 5>, kProbePointCount> kProbePoints = MakeProbePoints();

std::array<double, kProbeComponentCount> ComponentsOf(const excgrid::XcKernelValue& value) {
    return {value.exc, value.vrhoA, value.vrhoB, value.vsigmaAa, value.vsigmaAb, value.vsigmaBb};
}

/// Gaussian elimination with partial pivoting.  False when singular.
template <std::size_t N>
bool SolveLinearSystem(std::array<std::array<double, N>, N>& matrix, std::array<double, N>& rhs) {
    for (std::size_t column = 0; column < N; ++column)
    {
        std::size_t pivot = column;

        for (std::size_t row = column + 1; row < N; ++row)
        {
            if (std::abs(matrix[row][column]) > std::abs(matrix[pivot][column]))
            {
                pivot = row;
            }
        }

        if (matrix[pivot][column] == 0.0)
        {
            return false;
        }

        std::swap(matrix[pivot], matrix[column]);
        std::swap(rhs[pivot], rhs[column]);

        for (std::size_t row = column + 1; row < N; ++row)
        {
            const double factor = matrix[row][column] / matrix[column][column];

            for (std::size_t inner = column; inner < N; ++inner)
            {
                matrix[row][inner] -= factor * matrix[column][inner];
            }

            rhs[row] -= factor * rhs[column];
        }
    }

    for (std::size_t index = N; index-- > 0;)
    {
        double sum = rhs[index];

        for (std::size_t inner = index + 1; inner < N; ++inner)
        {
            sum -= matrix[index][inner] * rhs[inner];
        }

        rhs[index] = sum / matrix[index][index];
    }

    return true;
}

struct WeightRecovery {
    std::array<double, kKernelCount> weights{};
    double fitResidual = 0.0;
    bool solved = false;
};

/// Least-squares recovery of the kernel-universe weights from behaviour
/// alone: the design matrix holds the six XcKernelValue components of every
/// universe kernel at every probe point, the right-hand side the same
/// components of the functional under test.  Columns are equilibrated (each
/// divided by its largest magnitude), which is what keeps the normal
/// equations conditioned across the spread of kernel magnitudes.  The energy
/// density AND all three sigma derivatives are fitted at once, so a term
/// cannot hide behind a matching value with wrong derivatives.
///
/// `fitResidual` is the largest deviation of the fitted functional from the
/// measured one, relative to the largest measured component: nonzero means
/// the entry is not a combination of the universe.  `solved` is false when
/// the probe set degenerates (a non-finite kernel value, an all-zero column,
/// or a singular normal matrix), which fails the guard rather than passing an
/// unexamined entry.
WeightRecovery RecoverKernelWeights(const excgrid::XcFunctional& functional) {
    constexpr std::size_t rowCount = kProbePointCount * kProbeComponentCount;

    WeightRecovery recovery;
    std::vector<double> design(rowCount * kKernelCount, 0.0);
    std::vector<double> measured(rowCount, 0.0);
    std::size_t row = 0;

    for (const std::array<double, 5>& point : kProbePoints)
    {
        const std::array<double, kProbeComponentCount> measuredComponents =
            ComponentsOf(functional.Evaluate(point[0], point[1], point[2], point[3], point[4]));
        std::array<std::array<double, kProbeComponentCount>, kKernelCount> kernelComponents;

        for (std::size_t kernel = 0; kernel < kKernelCount; ++kernel)
        {
            kernelComponents[kernel] = ComponentsOf(
                kKernelUniverse[kernel].Evaluate(point[0], point[1], point[2], point[3], point[4]));
        }

        for (std::size_t component = 0; component < kProbeComponentCount; ++component)
        {
            for (std::size_t kernel = 0; kernel < kKernelCount; ++kernel)
            {
                design[row * kKernelCount + kernel] = kernelComponents[kernel][component];
            }

            measured[row] = measuredComponents[component];
            ++row;
        }
    }

    for (const double value : design)
    {
        if (!std::isfinite(value))
        {
            return recovery;
        }
    }

    for (const double value : measured)
    {
        if (!std::isfinite(value))
        {
            return recovery;
        }
    }

    std::array<double, kKernelCount> scale{};

    for (std::size_t kernel = 0; kernel < kKernelCount; ++kernel)
    {
        double magnitude = 0.0;

        for (std::size_t probeRow = 0; probeRow < rowCount; ++probeRow)
        {
            magnitude = std::max(magnitude, std::abs(design[probeRow * kKernelCount + kernel]));
        }

        if (!(magnitude > 0.0))
        {
            return recovery;
        }

        scale[kernel] = magnitude;
    }

    std::array<std::array<double, kKernelCount>, kKernelCount> system{};
    std::array<double, kKernelCount> rhs{};

    for (std::size_t probeRow = 0; probeRow < rowCount; ++probeRow)
    {
        for (std::size_t i = 0; i < kKernelCount; ++i)
        {
            const double equilibrated = design[probeRow * kKernelCount + i] / scale[i];
            rhs[i] += equilibrated * measured[probeRow];

            for (std::size_t j = 0; j < kKernelCount; ++j)
            {
                system[i][j] += equilibrated * (design[probeRow * kKernelCount + j] / scale[j]);
            }
        }
    }

    std::array<double, kKernelCount> solution = rhs;

    if (!SolveLinearSystem<kKernelCount>(system, solution))
    {
        return recovery;
    }

    recovery.solved = true;

    for (std::size_t kernel = 0; kernel < kKernelCount; ++kernel)
    {
        recovery.weights[kernel] = solution[kernel] / scale[kernel];
    }

    double largestMeasured = 0.0;
    double largestDeviation = 0.0;

    for (std::size_t probeRow = 0; probeRow < rowCount; ++probeRow)
    {
        double fitted = 0.0;

        for (std::size_t kernel = 0; kernel < kKernelCount; ++kernel)
        {
            fitted += recovery.weights[kernel] * design[probeRow * kKernelCount + kernel];
        }

        largestMeasured = std::max(largestMeasured, std::abs(measured[probeRow]));
        largestDeviation = std::max(largestDeviation, std::abs(fitted - measured[probeRow]));
    }

    recovery.fitResidual =
        largestMeasured > 0.0 ? largestDeviation / largestMeasured : largestDeviation;

    return recovery;
}

/// The exchange side of a recipe, measured rather than read.
struct ExchangePartition {
    double ldaExchange = 0.0; ///< weight on the LSDA exchange kernel
    double ggaExchange = 0.0; ///< summed weight on the GGA exchange kernels
    double exactExchangeFraction = 0.0; ///< the HF fraction the recipe declares
    double fitResidual = 0.0;
    bool solved = false;

    /// The DFT exchange the folding rule requires: the published LSDA
    /// coefficient, which is what the exact-exchange fraction leaves of the
    /// exchange.
    [[nodiscard]] double ClosingExchange() const {
        return 1.0 - exactExchangeFraction;
    }

    /// The LSDA weight the folding rule requires, spelled as the rule spells
    /// it: the published LSDA coefficient minus the GGA-exchange coefficient.
    [[nodiscard]] double ClosingLdaExchange() const {
        return ClosingExchange() - ggaExchange;
    }

    [[nodiscard]] double DftExchange() const {
        return ldaExchange + ggaExchange;
    }

    /// Whether the recipe has any DFT exchange at all - the folding rule's
    /// precondition.  A pure correlation component (vwn5, lyp, ...) has no
    /// exchange partition to close.
    [[nodiscard]] bool CarriesDftExchange() const {
        return DftExchange() > kWeightTolerance;
    }
};

ExchangePartition MeasureExchangePartition(const excgrid::XcFunctional& functional) {
    const WeightRecovery recovery = RecoverKernelWeights(functional);
    ExchangePartition partition;
    partition.fitResidual = recovery.fitResidual;
    partition.solved = recovery.solved;
    partition.exactExchangeFraction = functional.ExchangeFraction();

    for (std::size_t kernel = 0; kernel < kKernelCount; ++kernel)
    {
        switch (kKernelUniverse[kernel].role)
        {
        case KernelRole::LdaExchange:
            partition.ldaExchange += recovery.weights[kernel];
            break;
        case KernelRole::GgaExchange:
            partition.ggaExchange += recovery.weights[kernel];
            break;
        case KernelRole::LdaCorrelation:
        case KernelRole::GgaCorrelation:
            break;
        }
    }

    return partition;
}

/// The guard's verdict on one functional: does its exchange partition close?
[[nodiscard]] bool PartitionCloses(const ExchangePartition& partition) {
    return std::abs(partition.DftExchange() - partition.ClosingExchange()) < kWeightTolerance;
}

/// A test-local recipe composition, the same weighted-sum shape the
/// registry's own hybrids use.  It exists so the guard can be shown to reject
/// the pre-fix recipe through the same code path that clears the shipped one;
/// it is never part of the registry.
struct ReplicaTerm {
    excgrid::LdaKernel lda;
    excgrid::GgaKernel gga;
    double weight;
};

class ReplicaHybrid final : public excgrid::XcFunctional {
public:
    ReplicaHybrid(double exchangeFraction, std::initializer_list<ReplicaTerm> terms) :
        _exchangeFraction(exchangeFraction), _terms(terms.begin(), terms.end()) {}

    [[nodiscard]] bool UsesGradient() const override {
        return true;
    }

    [[nodiscard]] double ExchangeFraction() const override {
        return _exchangeFraction;
    }

    [[nodiscard]] excgrid::ComponentMask RequiredMask() const override {
        return excgrid::ComponentMask::Collinear();
    }

    [[nodiscard]] excgrid::KernelStatus EvaluatePoint(const excgrid::PointInputs& inputs,
                                                      excgrid::PointResult& result) const override {
        const double rhoA = inputs.Get(excgrid::Component::RhoA);
        const double rhoB = inputs.Get(excgrid::Component::RhoB);
        const double sigmaAa = inputs.Get(excgrid::Component::SigmaAa);
        const double sigmaAb = inputs.Get(excgrid::Component::SigmaAb);
        const double sigmaBb = inputs.Get(excgrid::Component::SigmaBb);

        excgrid::XcKernelValue value;

        for (const ReplicaTerm& term : _terms)
        {
            const excgrid::XcKernelValue part =
                term.gga != nullptr ? term.gga(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb)
                                    : term.lda(rhoA, rhoB);
            value += term.weight * part;
        }

        excgrid::FoldIntoResult(value, inputs.mask, result);
        return excgrid::KernelStatus::kOk;
    }

private:
    double _exchangeFraction;
    std::vector<ReplicaTerm> _terms;
};

// ---------------------------------------------------------------------------
// The tau tier's exemption from the folding guard
// ---------------------------------------------------------------------------
//
// The recovery above fits each entry as a weighted sum over the LDA/GGA kernel
// universe, so an entry that reads the kinetic-energy densities is outside its
// span by construction: there is no combination of those kernels that reads tau,
// and the fit would report a spurious ill-conditioned one rather than the tier
// the entry belongs to.
//
// Such an entry is therefore EXEMPT, and the exemption is keyed on what the
// entry's RequiredMask says it reads - never on its name, so it cannot be
// extended by somebody choosing a string, and so the tier moves with the schema
// rather than with a list kept here.
//
// The exemption is not a pass and not free.  What the guard asks in exchange for
// it is that the mask is not a lie: tau arrives as a pair, and supplying it must
// move the entry's energy density.  An entry claiming tau without reading it -
// the one way the exemption could hide something - fails on the response check
// instead of being waved through, and the exempted entries are named in the
// messages so the record shows which entries left the guard's reach.

/// Whether an entry belongs to the tau tier, with the pairing invariant checked
/// as the mask is read.
/// \param functional The entry.
/// \returns True when its mask requires the kinetic-energy densities.
[[nodiscard]] bool RequiresTau(const excgrid::XcFunctional& functional) {
    const excgrid::ComponentMask mask = functional.RequiredMask();

    EXPECT_EQ(mask.Test(excgrid::Component::TauA), mask.Test(excgrid::Component::TauB))
        << "a mask that claims one spin channel's kinetic-energy density and not the other's is "
           "not a tau tier";

    return mask.Test(excgrid::Component::TauA);
}

/// How far an entry's energy density moves when the kinetic-energy densities are
/// supplied, which is the evidence the exemption is earned on.
/// \param functional The entry.
/// \returns The absolute energy-density difference between the two evaluations.
[[nodiscard]] double TauResponse(const excgrid::XcFunctional& functional) {
    excgrid::PointInputs inputs;
    inputs.Set(excgrid::Component::RhoA, 0.31);
    inputs.Set(excgrid::Component::RhoB, 0.29);
    inputs.Set(excgrid::Component::SigmaAa, 0.07);
    inputs.Set(excgrid::Component::SigmaAb, 0.05);
    inputs.Set(excgrid::Component::SigmaBb, 0.06);
    inputs.Set(excgrid::Component::TauA, 0.11);
    inputs.Set(excgrid::Component::TauB, 0.10);

    excgrid::PointInputs densitiesOnly = inputs;
    densitiesOnly.mask.Clear(excgrid::Component::TauA);
    densitiesOnly.mask.Clear(excgrid::Component::TauB);

    excgrid::PointResult withTau;
    excgrid::PointResult withoutTau;

    EXPECT_EQ(functional.EvaluatePoint(inputs, withTau), excgrid::KernelStatus::kOk);
    EXPECT_EQ(functional.EvaluatePoint(densitiesOnly, withoutTau), excgrid::KernelStatus::kOk);

    return std::abs(withTau.exc - withoutTau.exc);
}

TEST(RegistryFoldingGuard, EveryExchangePartitionCloses) {
    // The whole registry, name by name, through the one rule.  Nothing here
    // knows which entries are hybrids or what any weight is: the weights come
    // out of the recovery, so a functional added later is checked without
    // touching this test - and an entry the recovery cannot account for fails
    // rather than skipping.
    int checked = 0;
    int checkedWithExactExchange = 0;
    int exemptedTauTier = 0;

    for (std::string_view name : excgrid::FunctionalNames())
    {
        const excgrid::XcFunctional* functional = excgrid::FindFunctional(name);

        ASSERT_NE(functional, nullptr) << name;

        if (RequiresTau(*functional))
        {
            ++exemptedTauTier;
            EXPECT_GT(TauResponse(*functional), 0.0)
                << name
                << ": its RequiredMask claims the kinetic-energy densities, but its energy density "
                   "does not move when they are supplied - the exemption from the folding rule is "
                   "earned by reading tau, not by listing it";
            continue;
        }

        const ExchangePartition partition = MeasureExchangePartition(*functional);

        EXPECT_TRUE(partition.solved)
            << name << ": the kernel-universe solve is singular on this probe set";

        EXPECT_LT(partition.fitResidual, kFitTolerance)
            << name
            << ": this entry is not a combination of the shipped kernel universe (relative "
               "residual "
            << partition.fitResidual
            << "). Either it uses a kernel the guard does not know - extend kKernelUniverse "
               "with its role and re-run - or it is not a weighted sum of kernels.";

        if (!partition.CarriesDftExchange())
        {
            continue;
        }

        ++checked;

        if (partition.exactExchangeFraction > 0.0)
        {
            ++checkedWithExactExchange;
        }

        EXPECT_NEAR(partition.ldaExchange, partition.ClosingLdaExchange(), kWeightTolerance)
            << name << ": the recipe carries " << partition.ldaExchange
            << " of the LSDA exchange kernel, but with " << partition.ggaExchange
            << " of full GGA exchange kernels and an exact-exchange fraction of "
            << partition.exactExchangeFraction << " the folding rule requires "
            << partition.ClosingLdaExchange()
            << " (= 1 - exchangeFraction - ggaExchange). A published LSDA coefficient "
               "multiplied onto a FULL GGA kernel double counts the LSDA exchange: see the "
               "HybridFunctional note in excgrid/src/kernels_registry.cpp.";
    }

    EXPECT_GT(checked, 0) << "the guard examined no exchange at all";

    EXPECT_GT(checkedWithExactExchange, 0)
        << "the guard examined no exchange-bearing entry with an exact-exchange fraction";

    // The exemption branch has to be exercised, or it is dead code that would
    // silently stop exempting the tier it exists for - and with it, the pairing
    // and response checks that keep the exemption honest.
    EXPECT_GT(exemptedTauTier, 0)
        << "the guard exempted no entry from the tau tier: none is registered, or the exemption "
           "is no longer keyed on RequiredMask";
}

TEST(RegistryFoldingGuard, TheGuardRejectsTheUnfoldedRecipeShape) {
    // Failing inputs this guard exists for, each with its folded control.
    //
    // 1 and 2 are the pre-fix b3lyp entry as it stood before 4bddaceb - the
    // published 0.80 left on the FULL Slater kernel next to the full B88 at
    // 0.72 - and the same entry as the registry now ships it (0.08).
    //
    // 3 and 4 are a hybrid that has never existed: the same mistake in a NEW
    // entry, with PBE exchange in place of B88.  A guard that fixtureed four
    // names and their weights would not look at it at all, which is exactly
    // the failure mode this guard is for - the arithmetic has to be the
    // check, not a list.
    const ReplicaHybrid unfoldedB3Lyp(0.20,
                                      {{&excgrid::SlaterExchange, nullptr, 0.80},
                                       {nullptr, &excgrid::Becke88Exchange, 0.72},
                                       {&excgrid::Vwn5Correlation, nullptr, 0.19},
                                       {nullptr, &excgrid::LypCorrelation, 0.81}});
    const ReplicaHybrid foldedB3Lyp(0.20,
                                    {{&excgrid::SlaterExchange, nullptr, 0.08},
                                     {nullptr, &excgrid::Becke88Exchange, 0.72},
                                     {&excgrid::Vwn5Correlation, nullptr, 0.19},
                                     {nullptr, &excgrid::LypCorrelation, 0.81}});
    const ReplicaHybrid unfoldedPbeHybrid(0.20,
                                          {{&excgrid::SlaterExchange, nullptr, 0.80},
                                           {nullptr, &excgrid::PbeExchange, 0.75},
                                           {&excgrid::Pw92Correlation, nullptr, 0.19},
                                           {nullptr, &excgrid::PbeCorrelation, 0.81}});
    const ReplicaHybrid foldedPbeHybrid(0.20,
                                        {{&excgrid::SlaterExchange, nullptr, 0.05},
                                         {nullptr, &excgrid::PbeExchange, 0.75},
                                         {&excgrid::Pw92Correlation, nullptr, 0.19},
                                         {nullptr, &excgrid::PbeCorrelation, 0.81}});

    const std::array<const ReplicaHybrid*, 4> replicas = {
        &unfoldedB3Lyp, &foldedB3Lyp, &unfoldedPbeHybrid, &foldedPbeHybrid};
    const std::array<double, 4> shippedLdaWeights = {0.80, 0.08, 0.80, 0.05};
    const std::array<bool, 4> expectedToClose = {false, true, false, true};
    const std::array<const char*, 4> labels = {"b3lyp as shipped before 4bddaceb (0.80)",
                                               "b3lyp as shipped now (0.08)",
                                               "a hypothetical new PBE hybrid, unfolded (0.80)",
                                               "the same hybrid, folded (0.05)"};

    for (std::size_t index = 0; index < replicas.size(); ++index)
    {
        const ExchangePartition partition = MeasureExchangePartition(*replicas[index]);

        EXPECT_TRUE(partition.solved) << labels[index];
        EXPECT_LT(partition.fitResidual, kFitTolerance) << labels[index];

        // The recovery reads the shipped LSDA weight back before the rule
        // judges it, so the verdict below is the folding arithmetic firing on
        // the actual recipe rather than on a fit artefact.
        EXPECT_NEAR(partition.ldaExchange, shippedLdaWeights[index], kWeightTolerance)
            << labels[index];

        EXPECT_EQ(PartitionCloses(partition), expectedToClose[index])
            << labels[index] << ": recovered LSDA exchange " << partition.ldaExchange
            << ", folding rule requires " << partition.ClosingLdaExchange() << " (DFT exchange "
            << partition.DftExchange() << ", required " << partition.ClosingExchange() << ")";
    }
}

// ---------------------------------------------------------------------------
// The tau tier's generated kernel and its generated second-derivative tier
// ---------------------------------------------------------------------------
//
// The registry's `tau_x` is expanded from one symbolic definition
// (xc_defs/tau_x.ey) by the codegen pipeline, which emits the order-1 kernel and
// the second-derivative matrix from the same expression.  The tier's exact
// conditions - the uniform-gas limit through the energy density and through the
// density derivative along the gas's own manifold, the von Weizsacker tie
// between the sigma and the tau slot, the exact 10/81 coefficient, the schema's
// mask semantics on the new inputs - are asserted where they were first stated,
// in tests/meta_gga_reference_test.cpp, which resolves its functional by NAME
// and therefore gates the generated kernel as it gated the hand-written one.
//
// What is added here is the tier the generator emits beside that kernel: the
// materialised matrix and the contracted form, which no condition file reaches.
// They are checked against finite differences of THIS functional's own first
// derivatives, never against another object's numbers: a Hessian is the
// derivative of the gradient the same object returns, so the comparison is
// internally exact, and it catches what the emission can actually get wrong - a
// wrong row or column, a dropped or duplicated entry, a sign, and a packing that
// follows the kernel's argument order instead of the caller's mask.

/// The seven components the tau tier is emitted over, in kernel argument order.
const std::array<excgrid::Component, 7> kTauComponents = {excgrid::Component::RhoA,
                                                          excgrid::Component::RhoB,
                                                          excgrid::Component::SigmaAa,
                                                          excgrid::Component::SigmaAb,
                                                          excgrid::Component::SigmaBb,
                                                          excgrid::Component::TauA,
                                                          excgrid::Component::TauB};

/// The slots a request of the tier's own width fills: the upper triangle over
/// the seven active components.
constexpr std::size_t kTauPackedCount = kTauComponents.size() * (kTauComponents.size() + 1) / 2;

/// The contract's upper-triangle index over `active` components in identifier
/// order: entry (i, j), i <= j, at i*active - i*(i-1)/2 + (j-i).
[[nodiscard]] std::size_t Packed(std::size_t i, std::size_t j, std::size_t active) {
    return i * active - i * (i - 1) / 2 + (j - i);
}

/// One point's seven tau-tier components, in the same order.
using TauPoint = std::array<double, 7>;

[[nodiscard]] excgrid::PointInputs TauInputs(const TauPoint& values) {
    excgrid::PointInputs inputs;

    for (std::size_t i = 0; i < kTauComponents.size(); ++i)
    {
        inputs.Set(kTauComponents[i], values[i]);
    }

    return inputs;
}

/// One functional's first derivatives at one point, by tau-tier component.
[[nodiscard]] std::array<double, 7> TauFirstDerivatives(const excgrid::XcFunctional& functional,
                                                        const TauPoint& values) {
    excgrid::PointResult result;
    EXPECT_EQ(functional.EvaluatePoint(TauInputs(values), result), excgrid::KernelStatus::kOk);

    std::array<double, 7> derivatives{};

    for (std::size_t i = 0; i < kTauComponents.size(); ++i)
    {
        derivatives[i] = result.first[excgrid::IndexOf(kTauComponents[i])];
    }

    return derivatives;
}

/// The generated materialised tier at one point, which must also carry the
/// order-1 tier's own result.
[[nodiscard]] excgrid::PointSecondDerivativeMatrix TauMatrix(const excgrid::XcFunctional& tau,
                                                             const TauPoint& values) {
    excgrid::PointResult result;
    excgrid::PointResult order1;
    excgrid::PointSecondDerivativeMatrix matrix;

    EXPECT_EQ(tau.EvaluatePointMaterialising(TauInputs(values), result, matrix),
              excgrid::KernelStatus::kOk);
    EXPECT_EQ(tau.EvaluatePoint(TauInputs(values), order1), excgrid::KernelStatus::kOk);
    EXPECT_EQ(result.exc, order1.exc);
    EXPECT_EQ(result.first[excgrid::IndexOf(excgrid::Component::TauA)],
              order1.first[excgrid::IndexOf(excgrid::Component::TauA)]);

    return matrix;
}

/// The points every finite-difference check below is taken at: a moderate one, a
/// high-density strong-gradient one, and a low-density one, where the entries'
/// own magnitudes spread over four orders.
const std::array<TauPoint, 3> kTauPoints = {{
    {0.42, 0.31, 0.021, 0.004, 0.017, 0.09, 0.07},
    {1.7, 0.25, 0.8, 0.15, 0.55, 2.4, 0.06},
    {0.02, 0.018, 0.05, 0.002, 0.03, 0.02, 0.9},
}};

// Every entry of the materialised matrix against the finite difference of the
// first derivative the same functional returns, with the difference and its
// accuracy measured on this kernel rather than assumed.
//
// A plain central difference at h = 1e-4 leaves an O(h^2) term that is visible
// where the third derivative is steep: at the rhoA = 0.02 / tauB = 0.9 corner
// it is 1.15e-4 relative, and 1.15e-6 at h = 1e-5 and 1.15e-8 at h = 1e-6 -
// falling by four per halving, exactly as a truncation term does, which is what
// separates it from an error in the entry.  Cancelling that term with one
// Richardson step at h = 1e-4 leaves the round-off floor: 2.1e-9 relative over
// the three points and all twenty-eight entries, measured.  The gate is 1e-7 -
// fifty times that floor, and seven orders below the O(1) relative move a wrong
// coefficient in any single entry makes.
//
// The entries the generator folded to zero are held exactly instead of to a
// tolerance, because the derivative each differences is textually free of that
// component: its two sides are bit-identical, so the difference is exactly zero
// at any step (measured at four steps across the three points).
TEST(RegistryTauTier, TheMatrixIsTheDerivativeOfTheGradientsTheKernelReturns) {
    // Both numbers come from the measurement in the comment above this test.
    constexpr double kStep = 1e-4;
    constexpr double kRelativeTolerance = 1e-7;

    const excgrid::XcFunctional* tau = excgrid::FindFunctional("tau_x");
    ASSERT_NE(tau, nullptr);

    for (const TauPoint& point : kTauPoints)
    {
        const excgrid::PointSecondDerivativeMatrix matrix = TauMatrix(*tau, point);

        EXPECT_EQ(matrix.mask.ActiveCount(), kTauComponents.size());
        EXPECT_TRUE(matrix.mask.Test(excgrid::Component::TauB));

        for (std::size_t i = 0; i < kTauComponents.size(); ++i)
        {
            for (std::size_t j = i; j < kTauComponents.size(); ++j)
            {
                const auto differenceAt = [&](double step) {
                    TauPoint ahead = point;
                    TauPoint behind = point;
                    ahead[j] += step;
                    behind[j] -= step;

                    const std::array<double, 7> forward = TauFirstDerivatives(*tau, ahead);
                    const std::array<double, 7> backward = TauFirstDerivatives(*tau, behind);
                    return (forward[i] - backward[i]) / (2.0 * step);
                };

                const double coarse = differenceAt(kStep);
                const double difference = (4.0 * differenceAt(kStep / 2.0) - coarse) / 3.0;

                const double entry = matrix.upper[Packed(i, j, kTauComponents.size())];
                const double scale = std::max({std::abs(entry), std::abs(difference), 1.0});

                if (entry == 0.0)
                {
                    // Exact, not to a tolerance; see this test's own comment.
                    EXPECT_DOUBLE_EQ(difference, 0.0)
                        << "entry (" << i << ", " << j << ") at rhoA = " << point[0]
                        << ": the matrix carries an exact zero, so the difference must be one";
                    continue;
                }

                EXPECT_NEAR(entry, difference, kRelativeTolerance * scale)
                    << "entry (" << i << ", " << j << ") at rhoA = " << point[0]
                    << ": the generated matrix carries " << entry
                    << ", the extrapolated difference of the generated first derivatives is "
                    << difference;
            }
        }

        // The packing stops where the contract's triangle does: the seven active
        // components use the first 28 of the capacity's slots and no others.
        for (std::size_t slot = kTauPackedCount; slot < matrix.upper.size(); ++slot)
        {
            EXPECT_DOUBLE_EQ(matrix.upper[slot], 0.0) << "slot " << slot;
        }
    }
}

// The other half of the schema's mask rule, on the tier that packs: the matrix a
// caller gets is over the caller's mask, so clearing the tau bits must drop the
// tau rows and columns rather than leave them inside a seven-wide packing.
//
// The two matrices below come from the SAME buffer values - the tau components
// are supplied and zero in one case, absent in the other - so the block the two
// packings share is bit-identical, and the entries that involve tau are gone
// from the second rather than present as zeros.  The check is exact on purpose:
// anything else means the packing counts something the caller did not supply.
TEST(RegistryTauTier, TheMatrixPacksOverTheCallersMask) {
    TauPoint point = kTauPoints[0];
    point[5] = 0.0; // tauA
    point[6] = 0.0; // tauB

    const excgrid::XcFunctional* tau = excgrid::FindFunctional("tau_x");
    ASSERT_NE(tau, nullptr);

    excgrid::PointInputs withoutTau = TauInputs(point);
    withoutTau.mask.Clear(excgrid::Component::TauA);
    withoutTau.mask.Clear(excgrid::Component::TauB);

    excgrid::PointResult result;
    excgrid::PointSecondDerivativeMatrix restricted;
    ASSERT_EQ(tau->EvaluatePointMaterialising(withoutTau, result, restricted),
              excgrid::KernelStatus::kOk);

    const excgrid::PointSecondDerivativeMatrix full = TauMatrix(*tau, point);

    EXPECT_EQ(restricted.mask.ActiveCount(), 5U);
    EXPECT_FALSE(restricted.mask.Test(excgrid::Component::TauA));

    for (std::size_t i = 0; i < 5; ++i)
    {
        for (std::size_t j = i; j < 5; ++j)
        {
            EXPECT_DOUBLE_EQ(restricted.upper[Packed(i, j, 5)], full.upper[Packed(i, j, 7)])
                << "entry (" << i << ", " << j << ")";
        }
    }

    for (std::size_t slot = 5 * 6 / 2; slot < restricted.upper.size(); ++slot)
    {
        EXPECT_DOUBLE_EQ(restricted.upper[slot], 0.0) << "slot " << slot;
    }

    // The density-tau block is not among the shared ones: at this point the
    // functional is linear in tau, so those entries are nonzero in the full
    // packing - which is what makes their absence above a drop rather than a
    // value that happened to be zero.
    EXPECT_NE(full.upper[Packed(0, 5, 7)], 0.0);
}

// The contracted tier against the materialised one.  The schema's two entry
// points have to agree on the matrix and on the layout of a right-hand side,
// which neither publishes on its own: the contraction is checked here as exactly
// the materialised matrix acting on the vectors it was handed, in the order the
// contract names them.
TEST(RegistryTauTier, TheContractionIsTheMaterialisedMatrixOnTheRightHandSides) {
    const std::array<double, 7> firstSide = {0.5, -1.25, 2.0, 0.75, -0.5, 1.5, -2.0};
    const std::array<double, 7> secondSide = {1.0, 1.0, -1.0, 0.0, 3.0, -1.0, 0.25};
    std::vector<double> rightHandSides(firstSide.begin(), firstSide.end());
    rightHandSides.insert(rightHandSides.end(), secondSide.begin(), secondSide.end());

    const excgrid::XcFunctional* tau = excgrid::FindFunctional("tau_x");
    ASSERT_NE(tau, nullptr);

    const std::size_t active = kTauComponents.size();

    for (const TauPoint& point : kTauPoints)
    {
        const excgrid::PointSecondDerivativeMatrix matrix = TauMatrix(*tau, point);

        excgrid::PointResult result;
        excgrid::PointResult order1;
        excgrid::PointSecondDerivative second;
        ASSERT_EQ(tau->EvaluatePointWithSecondDerivatives(
                      TauInputs(point), rightHandSides, result, second),
                  excgrid::KernelStatus::kOk);
        ASSERT_EQ(tau->EvaluatePoint(TauInputs(point), order1), excgrid::KernelStatus::kOk);

        EXPECT_EQ(result.exc, order1.exc);
        EXPECT_EQ(second.rightHandSides, 2U);
        EXPECT_EQ(second.mask.ActiveCount(), active);

        for (std::size_t side = 0; side < 2; ++side)
        {
            for (std::size_t i = 0; i < active; ++i)
            {
                double expected = 0.0;

                for (std::size_t j = 0; j < active; ++j)
                {
                    expected += matrix.upper[Packed(std::min(i, j), std::max(i, j), active)] *
                                rightHandSides[side * active + j];
                }

                const double contracted = second.contracted[side * active + i];

                // The two sums are the same product in the same order, so this is
                // round-off and not an approximation.
                EXPECT_NEAR(contracted, expected, 1e-14 * std::max(std::abs(expected), 1.0))
                    << "right-hand side " << side << ", component " << i
                    << ", at rhoA = " << point[0] << ": the contraction carries " << contracted
                    << " against " << expected << " from the materialised matrix";
            }
        }
    }
}

// The refusals of the new tier, by name, and the property that a refusal is not
// answered with zeros: the capacity and the layout are checked before anything
// is written, so a caller who receives a number can trust it.
TEST(RegistryTauTier, TheContractedTierRefusesByShape) {
    const excgrid::XcFunctional* tau = excgrid::FindFunctional("tau_x");
    ASSERT_NE(tau, nullptr);

    const excgrid::PointInputs inputs = TauInputs(kTauPoints[0]);
    const std::vector<double> ragged(10, 1.0); // not a multiple of the 7 active
    const std::vector<double> threeSides(21, 1.0); // 3 x 7 against a capacity of 16

    excgrid::PointResult result;
    excgrid::PointSecondDerivative second;

    second.rightHandSides = 42; // a sentinel: a refusal must not touch it
    EXPECT_EQ(tau->EvaluatePointWithSecondDerivatives(inputs, ragged, result, second),
              excgrid::KernelStatus::kRefusedUnsupportedCombination);
    EXPECT_EQ(second.rightHandSides, 42U);
    EXPECT_EQ(second.mask.ActiveCount(), 0U);

    EXPECT_EQ(tau->EvaluatePointWithSecondDerivatives(inputs, threeSides, result, second),
              excgrid::KernelStatus::kRefusedExhaustedCapacity);
    EXPECT_EQ(second.rightHandSides, 42U);

    // No active component at all: there is no layout to read a right-hand side
    // in, so the request has no answer rather than an empty one.
    excgrid::PointInputs empty;
    EXPECT_EQ(
        tau->EvaluatePointWithSecondDerivatives(empty, std::span<const double>{}, result, second),
        excgrid::KernelStatus::kRefusedUnsupportedCombination);

    // The order-1 tier answers the same call: a mask with no collinear component
    // in it is a point with zero densities, not a malformed request.
    excgrid::PointResult densitiesOnly;
    EXPECT_EQ(tau->EvaluatePoint(empty, densitiesOnly), excgrid::KernelStatus::kOk);
    EXPECT_DOUBLE_EQ(densitiesOnly.exc, 0.0);
}

// The generated kernel against the hand-written one that proved the tau slot, on
// the fields the two share by construction.
//
// Both are the same published expansion, so the energy density, the two sigma
// derivatives and the two tau derivatives have to agree to the precision the
// hand-written channel's constants carry: its Slater constant is written to 13
// digits (src/meta_gga_reference.cpp), which is a 1e-13 relative floor and
// nothing more, and the tolerance is set there.
//
// The density derivative is checked the same way.  Both channels are gated
// against the difference of the energy the generated one computes, so an error
// in either is a mismatch here rather than a tolerance to widen.  This check
// once carried a hundredfold looser bound because the hand-written channel's
// vrho was wrong on its last term; the term is fixed and the bound with it.
TEST(RegistryTauTier, TheGeneratedKernelAgreesWithTheHandWrittenReference) {
    constexpr double kSharedTolerance = 1e-12;
    constexpr double kStep = 1e-5;

    const excgrid::XcFunctional* tau = excgrid::FindFunctional("tau_x");
    ASSERT_NE(tau, nullptr);

    const excgrid::XcFunctional& reference = excgrid::MetaGgaReferenceFunctional();
    const TauPoint point = {1.3, 0.31, 2.7, 0.11, 0.77, 0.66, 0.21};

    excgrid::PointResult generated;
    excgrid::PointResult handWritten;
    ASSERT_EQ(tau->EvaluatePoint(TauInputs(point), generated), excgrid::KernelStatus::kOk);
    ASSERT_EQ(reference.EvaluatePoint(TauInputs(point), handWritten), excgrid::KernelStatus::kOk);

    EXPECT_NEAR(generated.exc, handWritten.exc, kSharedTolerance * std::abs(handWritten.exc));

    for (const excgrid::Component component : {excgrid::Component::SigmaAa,
                                               excgrid::Component::SigmaBb,
                                               excgrid::Component::TauA,
                                               excgrid::Component::TauB})
    {
        const double mine = generated.first[excgrid::IndexOf(component)];
        const double theirs = handWritten.first[excgrid::IndexOf(component)];

        EXPECT_NEAR(mine, theirs, kSharedTolerance * std::max(std::abs(theirs), 1.0))
            << "component " << excgrid::IndexOf(component);
    }

    // The cross invariant carries no channel's own von Weizsacker density, so
    // neither channel has a derivative with respect to it.
    EXPECT_DOUBLE_EQ(generated.first[excgrid::IndexOf(excgrid::Component::SigmaAb)], 0.0);
    EXPECT_DOUBLE_EQ(handWritten.first[excgrid::IndexOf(excgrid::Component::SigmaAb)], 0.0);

    // The generated density derivative against the difference of the energy.
    TauPoint ahead = point;
    TauPoint behind = point;
    ahead[0] += kStep;
    behind[0] -= kStep;

    excgrid::PointResult aheadResult;
    excgrid::PointResult behindResult;
    ASSERT_EQ(tau->EvaluatePoint(TauInputs(ahead), aheadResult), excgrid::KernelStatus::kOk);
    ASSERT_EQ(tau->EvaluatePoint(TauInputs(behind), behindResult), excgrid::KernelStatus::kOk);

    const double difference = (aheadResult.exc - behindResult.exc) / (2.0 * kStep);
    const double analytic = generated.first[excgrid::IndexOf(excgrid::Component::RhoA)];

    EXPECT_NEAR(analytic, difference, 1e-8 * std::max(std::abs(difference), 1.0));
    EXPECT_NEAR(handWritten.first[excgrid::IndexOf(excgrid::Component::RhoA)],
                difference,
                1e-8 * std::max(std::abs(difference), 1.0))
        << "the two channels' density derivatives have to agree: they are the same functional, so "
           "a mismatch is one of them being wrong.  The bound is the difference step's own, the "
           "same one the generated channel is held to above";
}

// Which implementation a name reaches is a fact, and it is pinned here rather
// than assumed.  Two implementations of this functional exist - the generated
// one and the hand-written reference - and a test that looks its subject up by
// name follows the name wherever it points.  When the name was repointed from
// one to the other, every such test silently changed what it was exercising,
// and the implementation it left behind had no coverage at all.  This states
// the binding so that a future repointing fails loudly instead.
TEST(RegistryTauTier, TheNameReachesTheGeneratedKernelAndNotTheReference) {
    const excgrid::XcFunctional* named = excgrid::FindFunctional("tau_x");
    ASSERT_NE(named, nullptr);

    const excgrid::XcFunctional& reference = excgrid::MetaGgaReferenceFunctional();
    EXPECT_NE(named, &reference) << "the name and the direct reference must be different objects; "
                                    "if they are the same, one implementation has been retired";

    const TauPoint point = {1.3, 0.31, 2.7, 0.11, 0.77, 0.66, 0.21};
    excgrid::PointResult throughName;
    excgrid::PointResult throughReference;
    ASSERT_EQ(named->EvaluatePoint(TauInputs(point), throughName), excgrid::KernelStatus::kOk);
    ASSERT_EQ(reference.EvaluatePoint(TauInputs(point), throughReference),
              excgrid::KernelStatus::kOk);

    // Two implementations of one functional have to give one answer.
    EXPECT_NEAR(throughName.exc, throughReference.exc,
                1e-12 * std::max(std::abs(throughReference.exc), 1.0));
}

} // namespace
