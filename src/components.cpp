#include "excgrid/components.hpp"

#include <array>

namespace excgrid {
namespace {

// The order of this array is the schema: an entry's position is its identifier.
constexpr std::array<ComponentInfo, kComponentCapacity> kTable{{
    {Component::RhoA, "rhoA", "electrons/Bohr^3", "Alpha-spin density.", kComponentActive},
    {Component::RhoB, "rhoB", "electrons/Bohr^3", "Beta-spin density.", kComponentActive},
    {Component::SigmaAa, "sigmaAa", "Bohr^-8", "grad(rhoA) . grad(rhoA).", kComponentActive},
    {Component::SigmaAb, "sigmaAb", "Bohr^-8", "grad(rhoA) . grad(rhoB).", kComponentActive},
    {Component::SigmaBb, "sigmaBb", "Bohr^-8", "grad(rhoB) . grad(rhoB).", kComponentActive},
    {Component::TauA, "tauA", "Bohr^-5", "Alpha kinetic-energy density.", kComponentActive},
    {Component::TauB, "tauB", "Bohr^-5", "Beta kinetic-energy density.", kComponentActive},

    {Component::LaplA, "laplA", "Bohr^-5", "Laplacian of the alpha density.",
     kComponentReserved},
    {Component::LaplB, "laplB", "Bohr^-5", "Laplacian of the beta density.",
     kComponentReserved},
    {Component::Jx, "jx", "Bohr^-4", "Paramagnetic current density, x.",
     kComponentReserved},
    {Component::Jy, "jy", "Bohr^-4", "Paramagnetic current density, y.",
     kComponentReserved},
    {Component::Jz, "jz", "Bohr^-4", "Paramagnetic current density, z.",
     kComponentReserved},

    {Component::Rho, "rho", "electrons/Bohr^3", "Total density, noncollinear treatment.",
     kComponentReserved | kComponentNoncollinear},
    {Component::Mx, "mx", "electrons/Bohr^3", "Magnetization, x.",
     kComponentReserved | kComponentNoncollinear},
    {Component::My, "my", "electrons/Bohr^3", "Magnetization, y.",
     kComponentReserved | kComponentNoncollinear},
    {Component::Mz, "mz", "electrons/Bohr^3", "Magnetization, z.",
     kComponentReserved | kComponentNoncollinear},
    {Component::SigmaRhoRho, "sigmaRhoRho", "Bohr^-8", "grad(rho) . grad(rho).",
     kComponentReserved | kComponentNoncollinear},
    {Component::SigmaRhoMx, "sigmaRhoMx", "Bohr^-8", "grad(rho) . grad(mx).",
     kComponentReserved | kComponentNoncollinear},
    {Component::SigmaRhoMy, "sigmaRhoMy", "Bohr^-8", "grad(rho) . grad(my).",
     kComponentReserved | kComponentNoncollinear},
    {Component::SigmaRhoMz, "sigmaRhoMz", "Bohr^-8", "grad(rho) . grad(mz).",
     kComponentReserved | kComponentNoncollinear},
    {Component::SigmaMxMx, "sigmaMxMx", "Bohr^-8", "grad(mx) . grad(mx).",
     kComponentReserved | kComponentNoncollinear},
    {Component::SigmaMxMy, "sigmaMxMy", "Bohr^-8", "grad(mx) . grad(my).",
     kComponentReserved | kComponentNoncollinear},
    {Component::SigmaMxMz, "sigmaMxMz", "Bohr^-8", "grad(mx) . grad(mz).",
     kComponentReserved | kComponentNoncollinear},
    {Component::SigmaMyMy, "sigmaMyMy", "Bohr^-8", "grad(my) . grad(my).",
     kComponentReserved | kComponentNoncollinear},
    {Component::SigmaMyMz, "sigmaMyMz", "Bohr^-8", "grad(my) . grad(mz).",
     kComponentReserved | kComponentNoncollinear},
    {Component::SigmaMzMz, "sigmaMzMz", "Bohr^-8", "grad(mz) . grad(mz).",
     kComponentReserved | kComponentNoncollinear},
    {Component::Tau0, "tau0", "Bohr^-5", "Noncollinear kinetic-energy density, scalar part.",
     kComponentReserved | kComponentNoncollinear},
    {Component::TauX, "tauX", "Bohr^-5", "Noncollinear kinetic-energy density, x.",
     kComponentReserved | kComponentNoncollinear},
    {Component::TauY, "tauY", "Bohr^-5", "Noncollinear kinetic-energy density, y.",
     kComponentReserved | kComponentNoncollinear},
    {Component::TauZ, "tauZ", "Bohr^-5", "Noncollinear kinetic-energy density, z.",
     kComponentReserved | kComponentNoncollinear},

    {Component::Spare30, "spare30", "", "Reserved, unassigned.", kComponentReserved},
    {Component::Spare31, "spare31", "", "Reserved, unassigned.", kComponentReserved},
}};

static_assert(kTable.size() == kComponentCapacity,
              "the table must describe exactly the reserved capacity");

} // namespace

std::span<const ComponentInfo> ComponentTable() noexcept { return kTable; }

const ComponentInfo& Describe(Component id) noexcept { return kTable[IndexOf(id)]; }

bool IsActive(Component id) noexcept {
    return (Describe(id).flags & kComponentActive) != 0;
}

} // namespace excgrid
