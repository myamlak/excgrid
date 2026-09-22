#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "excgrid/components.hpp"
#include "excgrid/kernel.hpp"

namespace {

using excgrid::Component;
using excgrid::ComponentInfo;
using excgrid::ComponentMask;
using excgrid::Describe;
using excgrid::IndexOf;
using excgrid::IsActive;
using excgrid::KernelStatus;
using excgrid::kComponentCapacity;
using excgrid::kSecondDerivativeCapacity;
using excgrid::PointInputs;
using excgrid::PointResult;
using excgrid::PointSecondDerivative;
using excgrid::PointSecondDerivativeMatrix;
using excgrid::SecondDerivativeStatus;
using excgrid::XcFunctional;

constexpr std::uint8_t kComponentCount = 32;

// A functional that publishes a known value, so the shape of the seam can be
// tested without depending on any generated kernel.
class StubFunctional final : public XcFunctional {
public:
    [[nodiscard]] bool UsesGradient() const override { return false; }

    [[nodiscard]] double ExchangeFraction() const override { return 0.0; }

    [[nodiscard]] ComponentMask RequiredMask() const override {
        ComponentMask mask;
        mask.Set(Component::RhoA);
        mask.Set(Component::RhoB);
        return mask;
    }

    [[nodiscard]] KernelStatus EvaluatePoint(const PointInputs& inputs,
                                             PointResult& result) const override {
        excgrid::XcKernelValue value;
        value.exc = inputs.Get(Component::RhoA) + inputs.Get(Component::RhoB);
        value.vrhoA = 1.0;
        value.vrhoB = 1.0;
        excgrid::FoldIntoResult(value, inputs.mask, result);
        return KernelStatus::kOk;
    }
};

std::vector<Component> AllComponents() {
    std::vector<Component> ids;
    for (std::uint8_t i = 0; i < kComponentCount; ++i) {
        ids.push_back(static_cast<Component>(i));
    }
    return ids;
}

// A functional whose second-derivative tier spans less than it reads: it reads
// both densities, and answers on the alpha one alone.  This is the shape the
// finer refusal names, and no shipped functional has it yet.
class PartialTierFunctional final : public XcFunctional {
public:
    [[nodiscard]] bool UsesGradient() const override { return false; }

    [[nodiscard]] double ExchangeFraction() const override { return 0.0; }

    [[nodiscard]] ComponentMask RequiredMask() const override {
        ComponentMask mask;
        mask.Set(Component::RhoA);
        mask.Set(Component::RhoB);
        return mask;
    }

    [[nodiscard]] ComponentMask SecondDerivativeMask() const noexcept override {
        ComponentMask mask;
        mask.Set(Component::RhoA);
        return mask;
    }

    [[nodiscard]] KernelStatus EvaluatePoint(const PointInputs& inputs,
                                             PointResult& result) const override {
        excgrid::XcKernelValue value;
        value.exc = inputs.Get(Component::RhoA) + inputs.Get(Component::RhoB);
        excgrid::FoldIntoResult(value, inputs.mask, result);
        return KernelStatus::kOk;
    }

    [[nodiscard]] KernelStatus EvaluatePointMaterialising(
        const PointInputs& inputs, PointResult& result,
        PointSecondDerivativeMatrix& matrix) const override {
        const ComponentMask spanned = SecondDerivativeMask();
        const KernelStatus status = SecondDerivativeStatus(inputs.mask, RequiredMask(), spanned);
        if (status != KernelStatus::kOk) {
            return status;
        }

        const ComponentMask published{inputs.mask.bits & spanned.bits};
        result = PointResult{};
        result.mask = published;
        matrix = PointSecondDerivativeMatrix{};
        matrix.mask = published;
        return KernelStatus::kOk;
    }
};

} // namespace

// The table is the schema: an entry's position must be its identifier, and every
// slot the capacity promises must have one.
TEST(ContractTest, TheTableCoversTheWholeCapacity) {
    const auto table = excgrid::ComponentTable();
    ASSERT_EQ(table.size(), kComponentCapacity);

    for (const Component id : AllComponents()) {
        const ComponentInfo& info = Describe(id);
        EXPECT_EQ(info.id, id);
        EXPECT_EQ(&info, &table[IndexOf(id)]);
    }
}

// Every identifier carries a name and a unit, and the two unassigned spares are
// the only ones without a unit.  A unit that is missing for an assigned slot is
// a value that cannot be interpreted.
TEST(ContractTest, EveryAssignedIdentifierStatesItsUnit) {
    for (const Component id : AllComponents()) {
        const ComponentInfo& info = Describe(id);
        EXPECT_FALSE(info.name.empty()) << "identifier " << IndexOf(id);
        EXPECT_FALSE(info.description.empty()) << "identifier " << IndexOf(id);

        const bool spare = info.name == "spare30" || info.name == "spare31";
        EXPECT_EQ(info.unit.empty(), spare) << "identifier " << info.name;
    }
}

// The seven that arrived first keep the positions they arrived in.  A reorder
// would be a major change, and this is what would catch one.
TEST(ContractTest, TheActiveIdentifiersKeepTheirPositions) {
    EXPECT_EQ(IndexOf(Component::RhoA), 0U);
    EXPECT_EQ(IndexOf(Component::RhoB), 1U);
    EXPECT_EQ(IndexOf(Component::SigmaAa), 2U);
    EXPECT_EQ(IndexOf(Component::SigmaAb), 3U);
    EXPECT_EQ(IndexOf(Component::SigmaBb), 4U);
    EXPECT_EQ(IndexOf(Component::TauA), 5U);
    EXPECT_EQ(IndexOf(Component::TauB), 6U);

    for (const Component id : AllComponents()) {
        EXPECT_EQ(IsActive(id), IndexOf(id) <= 6U) << "identifier " << IndexOf(id);
    }
}

// The active count is what the mask says, not the capacity.  Everything that
// sizes storage reads this, so it is asserted directly.
TEST(ContractTest, TheActiveCountFollowsTheMask) {
    EXPECT_EQ(ComponentMask{}.ActiveCount(), 0U);
    EXPECT_EQ(ComponentMask::Collinear().ActiveCount(), 7U);

    ComponentMask one;
    one.Set(Component::Mz);
    EXPECT_EQ(one.ActiveCount(), 1U);

    one.Clear(Component::Mz);
    EXPECT_EQ(one.ActiveCount(), 0U);
}

// A component outside the mask reads zero, and that zero is the contract's whole
// meaning: it is the only place a zero is the right answer.
TEST(ContractTest, AnAbsentComponentReadsZeroAndAMaskedOneIsKept) {
    PointInputs inputs;
    inputs.Set(Component::RhoA, 0.5);
    inputs.Set(Component::TauA, 0.25);

    EXPECT_DOUBLE_EQ(inputs.Get(Component::RhoA), 0.5);
    EXPECT_DOUBLE_EQ(inputs.Get(Component::TauA), 0.25);
    EXPECT_DOUBLE_EQ(inputs.Get(Component::RhoB), 0.0);
    EXPECT_FALSE(inputs.mask.Test(Component::RhoB));
}

// The result carries the mask it answered for, and a slot outside it is left at
// zero rather than at whatever a kernel happened to compute.
TEST(ContractTest, TheResultCarriesTheVersionAndTheMask) {
    const StubFunctional functional;

    PointInputs inputs;
    inputs.Set(Component::RhoA, 0.3);
    inputs.Set(Component::RhoB, 0.4);

    PointResult result;
    ASSERT_EQ(functional.EvaluatePoint(inputs, result), KernelStatus::kOk);

    EXPECT_TRUE(result.version.CompatibleWith(excgrid::ContractVersion()));
    EXPECT_EQ(result.mask.bits, inputs.mask.bits);
    EXPECT_DOUBLE_EQ(result.exc, 0.7);
    EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::RhoA)], 1.0);
    EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::RhoB)], 1.0);
    EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::TauA)], 0.0);
}

// Composition is componentwise, so a weighted sum of two functionals is the
// functional of the sum.
TEST(ContractTest, CompositionIsComponentwise) {
    excgrid::XcKernelValue a;
    a.exc = 1.0;
    a.vrhoA = 2.0;
    a.vtauB = 4.0;

    excgrid::XcKernelValue b;
    b.exc = 3.0;
    b.vrhoA = 5.0;
    b.vsigmaBb = 6.0;

    const excgrid::XcKernelValue sum = a + b;
    EXPECT_DOUBLE_EQ(sum.exc, 4.0);
    EXPECT_DOUBLE_EQ(sum.vrhoA, 7.0);
    EXPECT_DOUBLE_EQ(sum.vtauB, 4.0);
    EXPECT_DOUBLE_EQ(sum.vsigmaBb, 6.0);

    const excgrid::XcKernelValue scaled = 0.5 * sum;
    EXPECT_DOUBLE_EQ(scaled.exc, 2.0);
    EXPECT_DOUBLE_EQ(scaled.vrhoA, 3.5);
}

// A functional with no second-derivative tier refuses by name.  It does not
// return zeros, because a zero here would be carried into a Hessian as if it
// were an answer.
TEST(ContractTest, AnAbsentSecondDerivativeTierIsRefusedByName) {
    const StubFunctional functional;

    PointInputs inputs;
    inputs.Set(Component::RhoA, 0.3);
    inputs.Set(Component::RhoB, 0.4);

    PointResult result;
    PointSecondDerivative second;
    const std::vector<double> rhs(2, 1.0);

    EXPECT_EQ(functional.EvaluatePointWithSecondDerivatives(inputs, rhs, result, second),
              KernelStatus::kRefusedUnsupportedCapability);

    excgrid::PointSecondDerivativeMatrix matrix;
    EXPECT_EQ(functional.EvaluatePointMaterialising(inputs, result, matrix),
              KernelStatus::kRefusedUnsupportedCapability);
}

// Every refusal has its own name, and the names differ, so a caller can say
// which one it hit.
TEST(ContractTest, EachRefusalHasItsOwnName) {
    const std::vector<KernelStatus> refusals{
        KernelStatus::kRefusedUnsupportedCapability,
        KernelStatus::kRefusedMissingProvider,
        KernelStatus::kRefusedExhaustedCapacity,
        KernelStatus::kRefusedVersionMismatch,
        KernelStatus::kRefusedUnsupportedCombination,
        KernelStatus::kRefusedSecondDerivativeCoverage,
    };

    std::vector<std::string> seen;
    for (const KernelStatus status : refusals) {
        const std::string name(excgrid::DescribeStatus(status));
        ASSERT_FALSE(name.empty());
        EXPECT_EQ(name.rfind("refused", 0), 0U) << name;
        EXPECT_EQ(std::find(seen.begin(), seen.end(), name), seen.end()) << name;
        seen.push_back(name);
    }

    EXPECT_EQ(std::string(excgrid::DescribeStatus(KernelStatus::kOk)), "ok");
}

// The finer grain: a caller that has a tier on a functional still has to be able
// to tell "there is no tier" from "the tier does not reach the component you
// asked about", and the two are separate statuses with separate names.
TEST(ContractTest, ATierThatDoesNotSpanTheRequestSaysSo) {
    const PartialTierFunctional partial;

    PointInputs both;
    both.Set(Component::RhoA, 0.4);
    both.Set(Component::RhoB, 0.3);

    PointResult result;
    PointSecondDerivativeMatrix matrix;

    EXPECT_EQ(partial.EvaluatePointMaterialising(both, result, matrix),
              KernelStatus::kRefusedSecondDerivativeCoverage);
    EXPECT_NE(KernelStatus::kRefusedSecondDerivativeCoverage,
              KernelStatus::kRefusedUnsupportedCapability);

    // The same functional answers when the request stays inside what it spans.
    PointInputs alphaOnly;
    alphaOnly.Set(Component::RhoA, 0.4);

    EXPECT_EQ(partial.EvaluatePointMaterialising(alphaOnly, result, matrix), KernelStatus::kOk);
    EXPECT_EQ(matrix.mask.bits, ComponentMask{}.bits | (1U << 0U));
}

// The second-derivative capacity is smaller than the input capacity, and the
// difference is the contract rather than an implementation detail.
TEST(ContractTest, TheSecondDerivativeCapacityIsTheSmallerOne) {
    EXPECT_LT(kSecondDerivativeCapacity, kComponentCapacity);
    EXPECT_EQ(kSecondDerivativeCapacity, 16U);
}

// The shipped functionals still answer, through the schema, with the values the
// positional adapter reports.  This is the adapter pin: it is what fails if the
// reseating changed a semantic rather than adding a field.
TEST(ContractTest, TheShippedFunctionalsAgreeWithThePositionalAdapter) {
    const std::span<const std::string_view> names = excgrid::FunctionalNames();
    ASSERT_FALSE(names.empty());

    std::size_t compared = 0;
    for (const std::string_view name : names) {
        const XcFunctional* functional = excgrid::FindFunctional(name);
        ASSERT_NE(functional, nullptr) << name;

        // The positional adapter cannot supply the kinetic-energy density, so
        // the two surfaces are comparable only for a functional that does not
        // read it.
        if (functional->RequiredMask().Test(Component::TauA)) {
            continue;
        }

        ++compared;

        PointInputs inputs;
        inputs.Set(Component::RhoA, 0.31);
        inputs.Set(Component::RhoB, 0.29);
        inputs.Set(Component::SigmaAa, 0.07);
        inputs.Set(Component::SigmaAb, 0.05);
        inputs.Set(Component::SigmaBb, 0.06);
        inputs.Set(Component::TauA, 0.11);
        inputs.Set(Component::TauB, 0.10);

        PointResult result;
        ASSERT_EQ(functional->EvaluatePoint(inputs, result), KernelStatus::kOk) << name;

        const excgrid::XcKernelValue legacy =
            functional->Evaluate(0.31, 0.29, 0.07, 0.05, 0.06);

        EXPECT_DOUBLE_EQ(result.exc, legacy.exc) << name;
        EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::RhoA)], legacy.vrhoA) << name;
        EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::RhoB)], legacy.vrhoB) << name;
        EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::SigmaAa)], legacy.vsigmaAa) << name;
        EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::SigmaAb)], legacy.vsigmaAb) << name;
        EXPECT_DOUBLE_EQ(result.first[IndexOf(Component::SigmaBb)], legacy.vsigmaBb) << name;
    }

    EXPECT_GT(compared, 0U) << "no shipped functional was comparable through the adapter";
}

// A functional that reads the gradient says so, and the shipped registry agrees
// with its own kernels.
TEST(ContractTest, TheRequiredMaskMatchesWhatTheFunctionalReads) {
    for (const std::string_view name : excgrid::FunctionalNames()) {
        const XcFunctional* functional = excgrid::FindFunctional(name);
        ASSERT_NE(functional, nullptr) << name;

        const ComponentMask mask = functional->RequiredMask();
        EXPECT_TRUE(mask.Test(Component::RhoA)) << name;
        EXPECT_TRUE(mask.Test(Component::RhoB)) << name;
        EXPECT_EQ(mask.Test(Component::SigmaAa), functional->UsesGradient()) << name;

        // The spin channels of a quantity arrive together, so a functional
        // cannot require one and not the other.
        EXPECT_EQ(mask.Test(Component::TauA), mask.Test(Component::TauB)) << name;
    }
}
