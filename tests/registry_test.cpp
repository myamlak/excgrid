// The functional registry tests: name resolution, the shipped-name
// surface, hybrid recipes and exchange fractions, the VWN3/VWN5
// separation, and the exchange-folding guard - the class guard against
// the B3LYP Slater double count fixed in 4bddaceb coming back through a
// future entry.

#include "excgrid/kernel.hpp"
#include "excgrid/kernels.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>
#include <initializer_list>
#include <string_view>
#include <vector>

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

    EXPECT_EQ(names.size(), 23);

    for (const char* expected :
         {"slater", "vwn5",   "vwn3", "pw92",   "svwn",     "spw92",     "becke88", "pw91",
          "pbe",    "revpbe", "rpbe", "mpw91",  "pbesol",   "lyp",       "pbe_c",   "pw91_c",
          "p86",    "b3lyp",  "pbe0", "b3pw91", "mpw1pw91", "bhandhlyp", "b3p86"})
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
// b3lyp, b3pw91, b3p86 and bhandhlyp, whose 22-entry sweep lives in
// .claude/lane-status/ks-hybrid-exchange-hunt-lane.md section 6:
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

    excgrid::XcKernelValue Evaluate(
        double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) const override {
        excgrid::XcKernelValue result;

        for (const ReplicaTerm& term : _terms)
        {
            const excgrid::XcKernelValue part =
                term.gga != nullptr ? term.gga(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb)
                                    : term.lda(rhoA, rhoB);
            result += term.weight * part;
        }

        return result;
    }

private:
    double _exchangeFraction;
    std::vector<ReplicaTerm> _terms;
};

TEST(RegistryFoldingGuard, EveryExchangePartitionCloses) {
    // The whole registry, name by name, through the one rule.  Nothing here
    // knows which entries are hybrids or what any weight is: the weights come
    // out of the recovery, so a functional added later is checked without
    // touching this test - and an entry the recovery cannot account for fails
    // rather than skipping.
    int checked = 0;
    int checkedWithExactExchange = 0;

    for (std::string_view name : excgrid::FunctionalNames())
    {
        const excgrid::XcFunctional* functional = excgrid::FindFunctional(name);

        ASSERT_NE(functional, nullptr) << name;
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

} // namespace
