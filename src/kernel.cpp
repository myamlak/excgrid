#include "excgrid/kernel.hpp"

#include <array>

namespace excgrid {

std::string_view DescribeStatus(KernelStatus status) noexcept {
    switch (status) {
        case KernelStatus::kOk:
            return "ok";
        case KernelStatus::kRefusedUnsupportedCapability:
            return "refused: this build does not implement the functional or the requested tier";
        case KernelStatus::kRefusedMissingProvider:
            return "refused: this capability needs a provider that is not present";
        case KernelStatus::kRefusedExhaustedCapacity:
            return "refused: the request needs more components than the capacity allows";
        case KernelStatus::kRefusedVersionMismatch:
            return "refused: the caller's schema version does not match this build's";
        case KernelStatus::kRefusedUnsupportedCombination:
            return "refused: this combination is not one this library answers";
        case KernelStatus::kRefusedSecondDerivativeCoverage:
            return "refused: the second-derivative tier does not span the components requested";
    }
    return "refused: unrecognised status";
}

SchemaVersion ContractVersion() noexcept { return kSchemaVersion; }

void FoldIntoResult(const XcKernelValue& value, const ComponentMask& mask,
                    PointResult& result) noexcept {
    result.version = kSchemaVersion;
    result.mask = mask;
    result.exc = value.exc;

    // A slot the mask does not name keeps its zero.
    const std::array<std::pair<Component, double>, 8> named{{
        {Component::RhoA, value.vrhoA},
        {Component::RhoB, value.vrhoB},
        {Component::SigmaAa, value.vsigmaAa},
        {Component::SigmaAb, value.vsigmaAb},
        {Component::SigmaBb, value.vsigmaBb},
        {Component::TauA, value.vtauA},
        {Component::TauB, value.vtauB},
        {Component::Rho, 0.0},
    }};

    for (const auto& [id, number] : named) {
        const std::size_t index = IndexOf(id);
        result.first[index] = mask.Test(id) ? number : 0.0;
    }
}

ComponentMask XcFunctional::SecondDerivativeMask() const noexcept { return {}; }

// The two defaults refuse outright rather than reporting what
// SecondDerivativeStatus would say about this class's own mask: a derived class
// that publishes a mask without overriding these writes no numbers, and kOk
// over an untouched matrix is the one answer a caller cannot detect.
KernelStatus XcFunctional::EvaluatePointWithSecondDerivatives(const PointInputs&,
                                                              std::span<const double>,
                                                              PointResult&,
                                                              PointSecondDerivative&) const {
    return KernelStatus::kRefusedUnsupportedCapability;
}

KernelStatus XcFunctional::EvaluatePointMaterialising(const PointInputs&, PointResult&,
                                                      PointSecondDerivativeMatrix&) const {
    return KernelStatus::kRefusedUnsupportedCapability;
}

XcKernelValue XcFunctional::Evaluate(double rhoA, double rhoB, double sigmaAa, double sigmaAb,
                                     double sigmaBb) const noexcept {
    PointInputs inputs;
    inputs.Set(Component::RhoA, rhoA);
    inputs.Set(Component::RhoB, rhoB);
    inputs.Set(Component::SigmaAa, sigmaAa);
    inputs.Set(Component::SigmaAb, sigmaAb);
    inputs.Set(Component::SigmaBb, sigmaBb);

    PointResult result;
    if (EvaluatePoint(inputs, result) != KernelStatus::kOk) {
        return {};
    }

    XcKernelValue value;
    value.exc = result.exc;
    value.vrhoA = result.first[IndexOf(Component::RhoA)];
    value.vrhoB = result.first[IndexOf(Component::RhoB)];
    value.vsigmaAa = result.first[IndexOf(Component::SigmaAa)];
    value.vsigmaAb = result.first[IndexOf(Component::SigmaAb)];
    value.vsigmaBb = result.first[IndexOf(Component::SigmaBb)];
    value.vtauA = result.first[IndexOf(Component::TauA)];
    value.vtauB = result.first[IndexOf(Component::TauB)];

    return value;
}

} // namespace excgrid
