// The Bragg-Slater covalent radii, in one home.
//
// The Becke partition is a ratio of the two atoms' radii, so the build's
// weight and the derivative pass over that weight must read the same numbers:
// a second copy of the table is a second fact, and the two copies would part
// company silently.  Slater, J. Chem. Phys. 41 (1964) 3199.

#pragma once

#include <array>

namespace excgrid::internal {

/// One element's tabulated covalent radius.
struct BraggSlaterEntry {
    int atomicNumber; ///< The element's Z.
    double radiusAngstrom; ///< Its covalent radius, in Angstrom.
};

/// The table.
inline constexpr std::array<BraggSlaterEntry, 46> kBraggSlaterRadii = {{
    {1, 0.35},  {2, 1.0},   {3, 1.45},  {4, 1.05},  {5, 0.85},  {6, 0.70},  {7, 0.65},  {8, 0.60},
    {9, 0.50},  {10, 1.0},  {11, 1.80}, {12, 1.50}, {13, 1.25}, {14, 1.10}, {15, 1.00}, {16, 1.00},
    {17, 1.00}, {18, 1.0},  {19, 2.20}, {20, 1.80}, {21, 1.60}, {22, 1.40}, {23, 1.35}, {24, 1.40},
    {25, 1.40}, {26, 1.40}, {27, 1.35}, {28, 1.35}, {29, 1.35}, {30, 1.35}, {31, 1.30}, {32, 1.25},
    {33, 1.15}, {34, 1.15}, {35, 1.15}, {36, 1.0},  {37, 2.35}, {38, 2.00}, {39, 1.80}, {40, 1.55},
    {41, 1.45}, {42, 1.45}, {43, 1.35}, {44, 1.30}, {45, 1.35}, {46, 1.40},
}};

/// The radius of an element the table does not carry, in Angstrom.
inline constexpr double kBraggSlaterFallbackAngstrom = 1.0;

/// Angstrom to Bohr.
inline constexpr double kAngstromToBohr = 1.8897261246257702;

/// The Bragg-Slater covalent radius of an element, in Bohr.
/// \param atomicNumber The element's Z.
/// \returns The tabulated radius in Bohr, or the fallback for an element the
/// table does not carry.
[[nodiscard]] inline double BraggSlaterRadius(int atomicNumber) noexcept {
    for (const BraggSlaterEntry& entry : kBraggSlaterRadii)
    {
        if (entry.atomicNumber == atomicNumber)
        {
            return entry.radiusAngstrom * kAngstromToBohr;
        }
    }

    return kBraggSlaterFallbackAngstrom * kAngstromToBohr;
}

} // namespace excgrid::internal
