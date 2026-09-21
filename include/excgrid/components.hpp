#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

namespace excgrid {

/// \defgroup excgrid-components The contract's component table
///
/// The numbered schema through which a per-point quantity crosses the boundary
/// between this library and its consumers.  Every value carries the schema
/// version and an active mask, and every slot is named here rather than by
/// position.

/// The schema version.
///
/// A major change alters an identifier's meaning, reorders the table, exhausts
/// the capacity, or changes the composition semantics.  A minor change
/// activates a reserved identifier, adds an optional capability, or adds a
/// functional using only reserved identifiers.
/// \ingroup excgrid-components
struct SchemaVersion {
    std::uint16_t major = 0; ///< Changed only by a semantic break.
    std::uint16_t minor = 0; ///< Changed by an additive activation.

    /// Whether two versions can exchange values.
    /// \param other The version to compare against.
    /// \returns True when the major versions agree.
    /// \ingroup excgrid-components
    [[nodiscard]] constexpr bool CompatibleWith(const SchemaVersion& other) const noexcept {
        return major == other.major;
    }
};

/// The version this build speaks.
/// \ingroup excgrid-components
inline constexpr SchemaVersion kSchemaVersion{1, 0};

/// The number of input identifiers the schema reserves.
/// \ingroup excgrid-components
inline constexpr std::size_t kComponentCapacity = 32;

/// The number of components a second-derivative request may activate.
///
/// Smaller than the input capacity, whose cost is linear in the active count
/// where this one is quadratic.  Exceeding it is a refusal.
/// \ingroup excgrid-components
inline constexpr std::size_t kSecondDerivativeCapacity = 16;

/// A stable input identifier.
///
/// Identifiers 0 to 6 are active and keep the positions they arrived in;
/// 7 to 31 are reserved with fixed meanings.  Adding an identifier past 31 is a
/// major change.
/// \ingroup excgrid-components
enum class Component : std::uint8_t {
    RhoA = 0, ///< Alpha-spin density.
    RhoB = 1, ///< Beta-spin density.
    SigmaAa = 2, ///< grad(rhoA) . grad(rhoA).
    SigmaAb = 3, ///< grad(rhoA) . grad(rhoB).
    SigmaBb = 4, ///< grad(rhoB) . grad(rhoB).
    TauA = 5, ///< Alpha kinetic-energy density.
    TauB = 6, ///< Beta kinetic-energy density.

    LaplA = 7, ///< Reserved: laplacian of the alpha density.
    LaplB = 8, ///< Reserved: laplacian of the beta density.
    Jx = 9, ///< Reserved: paramagnetic current density, x.
    Jy = 10, ///< Reserved: paramagnetic current density, y.
    Jz = 11, ///< Reserved: paramagnetic current density, z.
    Rho = 12, ///< Reserved: total density, noncollinear.
    Mx = 13, ///< Reserved: magnetization, x.
    My = 14, ///< Reserved: magnetization, y.
    Mz = 15, ///< Reserved: magnetization, z.
    SigmaRhoRho = 16, ///< Reserved: grad(rho) . grad(rho).
    SigmaRhoMx = 17, ///< Reserved: grad(rho) . grad(mx).
    SigmaRhoMy = 18, ///< Reserved: grad(rho) . grad(my).
    SigmaRhoMz = 19, ///< Reserved: grad(rho) . grad(mz).
    SigmaMxMx = 20, ///< Reserved: grad(mx) . grad(mx).
    SigmaMxMy = 21, ///< Reserved: grad(mx) . grad(my).
    SigmaMxMz = 22, ///< Reserved: grad(mx) . grad(mz).
    SigmaMyMy = 23, ///< Reserved: grad(my) . grad(my).
    SigmaMyMz = 24, ///< Reserved: grad(my) . grad(mz).
    SigmaMzMz = 25, ///< Reserved: grad(mz) . grad(mz).
    Tau0 = 26, ///< Reserved: noncollinear kinetic-energy density, scalar part.
    TauX = 27, ///< Reserved: noncollinear kinetic-energy density, x.
    TauY = 28, ///< Reserved: noncollinear kinetic-energy density, y.
    TauZ = 29, ///< Reserved: noncollinear kinetic-energy density, z.
    Spare30 = 30, ///< Reserved, unassigned.
    Spare31 = 31, ///< Reserved, unassigned.
};

/// A flag on an identifier's table entry.
/// \ingroup excgrid-components
enum ComponentFlag : std::uint32_t {
    kComponentNone = 0, ///< No flag.
    kComponentActive = 1U << 0, ///< Active in this schema version.
    kComponentReserved = 1U << 1, ///< Meaning fixed, not yet active.
    kComponentNoncollinear = 1U << 2, ///< Requires a noncollinear treatment.
    kComponentProviderSupplied = 1U << 3, ///< Supplied by a provider.
};

/// One identifier's table entry.
///
/// The unit is a property of the table rather than data carried in each value.
/// \ingroup excgrid-components
struct ComponentInfo {
    Component id = Component::RhoA; ///< The stable identifier.
    std::string_view name = {}; ///< The canonical name.
    std::string_view unit = {}; ///< The unit its values carry.
    std::string_view description = {}; ///< One line.
    std::uint32_t flags = kComponentNone; ///< The ComponentFlag bits.
};

/// The whole table, in identifier order.
/// \returns The table; its size is kComponentCapacity.
/// \ingroup excgrid-components
[[nodiscard]] std::span<const ComponentInfo> ComponentTable() noexcept;

/// One identifier's entry.
/// \param id The identifier.
/// \returns Its table entry.
/// \ingroup excgrid-components
[[nodiscard]] const ComponentInfo& Describe(Component id) noexcept;

/// Whether an identifier is active in this build.
/// \param id The identifier.
/// \returns True when its flags carry kComponentActive.
/// \ingroup excgrid-components
[[nodiscard]] bool IsActive(Component id) noexcept;

/// The index an identifier occupies.
/// \param id The identifier.
/// \returns Its 0-based position.
/// \ingroup excgrid-components
[[nodiscard]] constexpr std::size_t IndexOf(Component id) noexcept {
    return static_cast<std::size_t>(id);
}

} // namespace excgrid
