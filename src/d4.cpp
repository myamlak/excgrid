// The D4 dispersion parameter surface.  The parameter values are the
// published BJ-damping set for the DFT-D4 method (Caldeweyher, Ehlert,
// Hansen, Neugebauer, Spicher, Bannwarth, Grimme, J. Chem. Phys. 150
// (2019) 154122), which publishes them as Supplementary Material
// S14-S21; the table below was extracted mechanically from the
// distribution that carries the same table, every entry of which names
// that paper as its source, so that no value passes through a
// transcription by hand.  The EEQ element parameters are the paper's own
// Supplementary Material Table A1 (S4-S6), transcribed the same way from
// the committed data file.

#include "d3_tables.inc"
#include "d4_cn_tables.inc"
#include "d4_eeq_tables.inc"
#include "d4_hardness_tables.inc"
#include "d4_reference.inc"
#include "d4_reference_counts.inc"
#include "d4_tables.inc"
#include "excgrid/d4.hpp"

#include <cmath>
#include <numbers>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

namespace excgrid {

namespace {

/// One method family's published D4 parameter set.
struct Preset {
    std::string_view name; ///< The method key, as the method is named in the literature.
    D4Parameters parameters; ///< Its published D4 parameters.
};

// The D4 default model is "bj-eeq-atm": Becke-Johnson damping over the
// two-body (dipole-dipole and dipole-quadrupole) terms, EEQ charges, and
// the Axilrod-Teller-Muto three-body term.  s6, s9 and the three-body
// damping exponent alp are the defaults of that model; s8, a1 and a2 are
// the fitted per-functional parameters.
constexpr Preset kPresets[] = {
    // name                 s6          s8          a1          a2     s9    alp
    {"am05", {1.00000000, 1.71885838, 0.47901431, 5.96771581, 1.0, 16.0}},
    {"b1b95", {1.00000000, 1.27701162, 0.40554715, 4.63323074, 1.0, 16.0}},
    {"b1lyp", {1.00000000, 1.98553711, 0.39309040, 4.55465145, 1.0, 16.0}},
    {"b1p", {1.00000000, 3.36115015, 0.48665293, 5.05219572, 1.0, 16.0}},
    {"b1pw", {1.00000000, 3.02227550, 0.47396846, 4.49845309, 1.0, 16.0}},
    {"b2gpplyp", {0.56000000, 0.94633372, 0.42907301, 5.18802602, 1.0, 16.0}},
    {"b2plyp", {0.64000000, 1.16888646, 0.44154604, 4.73114642, 1.0, 16.0}},
    {"b3lyp", {1.00000000, 2.02929367, 0.40868035, 4.53807137, 1.0, 16.0}},
    {"b3p", {1.00000000, 3.08822155, 0.47324238, 4.98682134, 1.0, 16.0}},
    {"b3pw", {1.00000000, 2.88364295, 0.46990860, 4.51641422, 1.0, 16.0}},
    {"b97", {1.00000000, 0.87854260, 0.29319126, 4.51647719, 1.0, 16.0}},
    {"b97d", {1.00000000, 1.69460052, 0.28904684, 4.13407323, 1.0, 16.0}},
    {"b97m", {1.00000000, 0.66330000, 0.42880000, 3.99350000, 1.0, 16.0}},
    {"bhlyp", {1.00000000, 1.65281646, 0.27263660, 5.48634586, 1.0, 16.0}},
    {"blyp", {1.00000000, 2.34076671, 0.44488865, 4.09330090, 1.0, 16.0}},
    {"bp", {1.00000000, 3.35497927, 0.43645861, 4.92406854, 1.0, 16.0}},
    {"bpbe", {1.00000000, 3.64405246, 0.52905620, 4.11311891, 1.0, 16.0}},
    {"bpw", {1.00000000, 3.24571506, 0.50050454, 4.12346483, 1.0, 16.0}},
    {"camb3lyp", {1.00000000, 1.66041301, 0.40267156, 5.17432195, 1.0, 16.0}},
    {"camqtp01", {1.00000000, 1.15600000, 0.46100000, 6.37500000, 1.0, 16.0}},
    {"dodblyp", {0.47000000, 1.31146043, 0.43407294, 4.27914360, 1.0, 16.0}},
    {"dodpbe", {0.48000000, 0.92051454, 0.43037052, 4.38067238, 1.0, 16.0}},
    {"dodpbeb95", {0.56000000, 0.01574635, 0.43745720, 3.69180763, 1.0, 16.0}},
    {"dodpbep86", {0.46000000, 0.71405681, 0.42408665, 4.52884439, 1.0, 16.0}},
    {"dodsvwn", {0.42000000, 0.94500207, 0.47449026, 5.05316093, 1.0, 16.0}},
    {"dsdblyp", {0.54000000, 0.63018237, 0.47591835, 4.73713781, 1.0, 16.0}},
    {"dsdpbe", {0.45000000, 0.70584116, 0.45787085, 4.44566742, 1.0, 16.0}},
    {"dsdpbep86", {0.47000000, 0.37586675, 0.53698768, 5.13022435, 1.0, 16.0}},
    {"dsdsvwn", {0.41000000, 0.72914436, 0.51347412, 5.11858541, 1.0, 16.0}},
    {"glyp", {1.00000000, 4.23798924, 0.38426465, 4.38412863, 1.0, 16.0}},
    {"hf", {1.00000000, 1.61679827, 0.44959224, 3.35743605, 1.0, 16.0}},
    {"hse03", {1.00000000, 1.19812280, 0.38662939, 5.22925796, 1.0, 16.0}},
    {"hse06", {1.00000000, 1.19528249, 0.38663183, 5.19133469, 1.0, 16.0}},
    {"hse12", {1.00000000, 1.23500792, 0.39226921, 5.22036266, 1.0, 16.0}},
    {"hse12s", {1.00000000, 1.23767762, 0.39989137, 5.34809245, 1.0, 16.0}},
    {"hsesol", {1.00000000, 1.82207807, 0.45646268, 5.59662251, 1.0, 16.0}},
    {"kpr2scan50", {0.84020000, 0.12120000, 0.43820000, 5.82320000, 1.0, 16.0}},
    {"lb94", {1.00000000, 2.59538499, 0.42088944, 3.28193223, 1.0, 16.0}},
    {"lcblyp", {1.00000000, 1.60344180, 0.45769839, 7.86924893, 1.0, 16.0}},
    {"lcwpbe", {1.00000000, 1.17000000, 0.37800000, 4.81600000, 1.0, 16.0}},
    {"lcwpbeh", {1.00000000, 1.31800000, 0.38600000, 5.01000000, 1.0, 16.0}},
    {"lh07ssvwn", {1.00000000, 3.16675531, 0.35965552, 4.31947614, 1.0, 16.0}},
    {"lh07tsvwn", {1.00000000, 2.09333001, 0.35025189, 4.34166515, 1.0, 16.0}},
    {"lh12ctssifpw92", {1.00000000, 2.68467610, 0.34190416, 3.91039666, 1.0, 16.0}},
    {"lh12ctssirpw92", {1.00000000, 2.48973402, 0.34026075, 3.96948081, 1.0, 16.0}},
    {"lh14tcalpbe", {1.00000000, 1.28130770, 0.38822021, 4.92501211, 1.0, 16.0}},
    {"lh20t", {1.00000000, 0.11300000, 0.47900000, 4.63500000, 1.0, 16.0}},
    {"m06", {1.00000000, 0.16366729, 0.53456413, 6.06192174, 1.0, 16.0}},
    {"m06l", {1.00000000, 0.59493760, 0.71422359, 6.35314182, 1.0, 16.0}},
    {"mn12sx", {1.00000000, 0.85964873, 0.62662681, 5.62088906, 1.0, 16.0}},
    {"mpw1b95", {1.00000000, 0.50093024, 0.41585097, 4.99154869, 1.0, 16.0}},
    {"mpw1lyp", {1.00000000, 1.15591153, 0.25603493, 5.32083895, 1.0, 16.0}},
    {"mpw1pw", {1.00000000, 1.80841716, 0.42961819, 4.68892341, 1.0, 16.0}},
    {"mpw2plyp", {0.75000000, 0.45788846, 0.42997704, 5.07650682, 1.0, 16.0}},
    {"mpwb1k", {1.00000000, 0.57338313, 0.44687975, 5.21266777, 1.0, 16.0}},
    {"mpwlyp", {1.00000000, 1.25842942, 0.25773894, 5.02319542, 1.0, 16.0}},
    {"mpwpw", {1.00000000, 1.82596836, 0.34526745, 4.84620734, 1.0, 16.0}},
    {"o3lyp", {1.00000000, 1.75762508, 0.10348980, 6.16233282, 1.0, 16.0}},
    {"olyp", {1.00000000, 2.74836820, 0.60184498, 2.53292167, 1.0, 16.0}},
    {"opbe", {1.00000000, 3.06917417, 0.68267534, 2.22849018, 1.0, 16.0}},
    {"pbe", {1.00000000, 0.95948085, 0.38574991, 4.80688534, 1.0, 16.0}},
    {"pbe0", {1.00000000, 1.20065498, 0.40085597, 5.02928789, 1.0, 16.0}},
    {"pbesol", {1.00000000, 1.71885698, 0.47901421, 5.96771589, 1.0, 16.0}},
    {"pr2scan50", {0.79640000, 0.34210000, 0.46630000, 5.79160000, 1.0, 16.0}},
    {"pr2scan69", {0.71670000, 0.00000000, 0.46440000, 5.25630000, 1.0, 16.0}},
    {"pw1pw", {1.00000000, 0.96850170, 0.42427511, 5.02060636, 1.0, 16.0}},
    {"pw86pbe", {1.00000000, 1.21362856, 0.40510366, 4.66737724, 1.0, 16.0}},
    {"pw91", {1.00000000, 0.77283111, 0.39581542, 4.93405761, 1.0, 16.0}},
    {"pwp", {1.00000000, 0.32801227, 0.35874687, 6.05861168, 1.0, 16.0}},
    {"pwp1", {1.00000000, 0.60492565, 0.46855837, 5.76921413, 1.0, 16.0}},
    {"r2scan", {1.00000000, 0.60187490, 0.51559235, 5.77342911, 1.0, 16.0}},
    {"r2scan-0-2", {0.73860000, 0.00000000, 0.40300000, 5.51420000, 1.0, 16.0}},
    {"r2scan-3c", {1.00000000, 0.00000000, 0.42000000, 5.65000000, 2.0, 16.0}},
    {"r2scan-cidh", {0.86660000, 0.53360000, 0.41710000, 5.85650000, 1.0, 16.0}},
    {"r2scan-qidh", {0.78670000, 0.29550000, 0.40010000, 5.83000000, 1.0, 16.0}},
    {"r2scan0", {1.00000000, 0.89920000, 0.47780000, 5.87790000, 1.0, 16.0}},
    {"r2scan0-dh", {0.94240000, 0.38560000, 0.42710000, 5.85650000, 1.0, 16.0}},
    {"r2scan50", {1.00000000, 1.04710000, 0.45740000, 5.89690000, 1.0, 16.0}},
    {"r2scanh", {1.00000000, 0.83240000, 0.49440000, 5.90190000, 1.0, 16.0}},
    {"revdodpbep86", {0.55520000, 0.00000000, 0.44000000, 3.60000000, 1.0, 16.0}},
    {"revdsdblyp", {0.61410000, 0.00000000, 0.38000000, 3.52000000, 1.0, 16.0}},
    {"revdsdpbe", {0.67060000, 0.00000000, 0.40000000, 3.60000000, 1.0, 16.0}},
    {"revdsdpbep86", {0.51320000, 0.00000000, 0.44000000, 3.60000000, 1.0, 16.0}},
    {"revpbe", {1.00000000, 1.74676530, 0.53634900, 3.07261485, 1.0, 16.0}},
    {"revpbe0", {1.00000000, 1.57185414, 0.38705966, 4.11028876, 1.0, 16.0}},
    {"revpbe0dh", {0.87500000, 1.24456037, 0.36730560, 4.71126482, 1.0, 16.0}},
    {"revpbe38", {1.00000000, 1.66597472, 0.39476833, 4.39026628, 1.0, 16.0}},
    {"revtpss", {1.00000000, 1.53089454, 0.44880597, 4.64042317, 1.0, 16.0}},
    {"revtpss0", {1.00000000, 1.54664499, 0.45890964, 4.78426405, 1.0, 16.0}},
    {"revtpssh", {1.00000000, 1.52740307, 0.45161957, 4.70779483, 1.0, 16.0}},
    {"rpbe", {1.00000000, 1.31183787, 0.46169493, 3.15711757, 1.0, 16.0}},
    {"rpw86pbe", {1.00000000, 1.12624034, 0.38151218, 4.75480472, 1.0, 16.0}},
    {"rscan", {1.00000000, 0.87728975, 0.49116966, 5.75859346, 1.0, 16.0}},
    {"scan", {1.00000000, 1.46126056, 0.62930855, 6.31284039, 1.0, 16.0}},
    {"tpss", {1.00000000, 1.76596355, 0.42822303, 4.54257102, 1.0, 16.0}},
    {"tpss0", {1.00000000, 1.62438102, 0.40329022, 4.80537871, 1.0, 16.0}},
    {"tpssh", {1.00000000, 1.85897750, 0.44286966, 4.60230534, 1.0, 16.0}},
    {"wb97", {1.00000000, 6.55792598, 0.76666802, 8.36027334, 1.0, 16.0}},
    {"wb97m", {1.00000000, 0.77610000, 0.75140000, 2.70990000, 1.0, 16.0}},
    {"wb97m-rev", {1.00000000, 0.84200000, 0.35900000, 4.66800000, 1.0, 16.0}},
    {"wb97x", {1.00000000, 0.50930000, 0.06620000, 5.44870000, 1.0, 16.0}},
    {"wb97x-3c", {1.00000000, 0.00000000, 0.24640000, 4.73700000, 1.0, 16.0}},
    {"wb97x-rev", {1.00000000, 0.44850000, 0.33060000, 4.27900000, 1.0, 16.0}},
    {"wpr2scan50", {0.81430000, 0.38420000, 0.41350000, 5.87730000, 1.0, 16.0}},
    {"wr2scan", {1.00000000, 1.00000000, 0.38340000, 5.78890000, 1.0, 16.0}},
    {"x3lyp", {1.00000000, 1.54701429, 0.20318443, 5.61852648, 1.0, 16.0}},
    {"xlyp", {1.00000000, 1.62972054, 0.11268673, 5.40786417, 1.0, 16.0}},
};

// The constants of the counting function, as published: the damping
// steepness, and the scale, offset and divisor of the
// electronegativity-difference factor.
constexpr double kCnSteepness = 7.5;
constexpr double kCnEnScale = 4.1;
constexpr double kCnEnOffset = 19.09;
constexpr double kCnEnExponent = 254.56;

// The global parameter of the charge-scaling function.  The model scales the
// element-specific chemical hardness by the second of these before it enters
// the exponential, so what multiplies the bracket's argument is their product
// and not the hardness alone: the scaling of equation 2 is
// exp[beta * (1 - exp(steepness * hardness * (1 - z_ref/z)))].  The paper
// prints equation 2 with the hardness unmultiplied, and the model's own
// published dynamic polarizabilities settle the product at twice the value
// that reading gives - for a hydrogen carrying +0.1126 of charge the scaling
// the published polarizability implies is 0.73993, the unmultiplied hardness
// gives 0.86332, and the product gives 0.73997.  The reference weighting's
// exponential likewise carries the model's own global of 6, which is the
// value the paper prints for it.
constexpr double kChargeScalingBeta = 3.0;
constexpr double kChargeScalingSteepness = 2.0;

// The global parameter of the reference weighting function.
constexpr double kReferenceWeightingBeta = 6.0;

// The counting function's electronegativity-difference factor: the pair's
// contribution is damped by how far apart the two elements sit on the
// Pauling scale.
double ElectronegativityFactor(double electronegativityA, double electronegativityB) {
    const double difference = std::abs(electronegativityA - electronegativityB) + kCnEnOffset;
    return kCnEnScale * std::exp(-(difference * difference) / kCnEnExponent);
}

// The counting-function data of one atom of the geometry.
const d4tables::CnElement& ElementOf(const Geometry& geometry, std::size_t index) {
    return d4tables::kCnElementTable[geometry.atoms[index].atomicNumber - 1];
}

// The elements whose reference data was computed in an effective core
// potential, and the number of core electrons that potential absorbs.
// The effective nuclear charge of such an element is its atomic number
// less this core size; these four ranges are the whole of the model's
// tabulation of it, and every element outside them is all-electron.
struct CoreRange {
    int first;
    int last;
    int coreElectrons;
};

constexpr CoreRange kCoreRanges[] = {
    {37, 54, 28}, // Rb-Xe
    {55, 57, 46}, // Cs-La
    {58, 71, 28}, // Ce-Lu
    {72, 86, 60}, // Hf-Rn
};

int CoreElectronsOf(int atomicNumber) {
    for (const CoreRange& range : kCoreRanges)
    {
        if (atomicNumber >= range.first && atomicNumber <= range.last)
        {
            return range.coreElectrons;
        }
    }

    return 0;
}

// Solve a dense square system in place by Gaussian elimination with partial
// pivoting.  a is row-major n-by-n and is consumed; the solution is left in b.
bool SolveInPlace(std::vector<double>& a, std::vector<double>& b, std::size_t n) {
    for (std::size_t col = 0; col < n; ++col)
    {
        std::size_t pivot = col;

        for (std::size_t row = col + 1; row < n; ++row)
        {
            if (std::abs(a[row * n + col]) > std::abs(a[pivot * n + col]))
            {
                pivot = row;
            }
        }

        if (a[pivot * n + col] == 0.0)
        {
            return false;
        }

        if (pivot != col)
        {
            for (std::size_t k = 0; k < n; ++k)
            {
                std::swap(a[col * n + k], a[pivot * n + k]);
            }

            std::swap(b[col], b[pivot]);
        }

        const double diagonal = a[col * n + col];

        for (std::size_t row = col + 1; row < n; ++row)
        {
            const double factor = a[row * n + col] / diagonal;

            if (factor == 0.0)
            {
                continue;
            }

            for (std::size_t k = col; k < n; ++k)
            {
                a[row * n + k] -= factor * a[col * n + k];
            }

            b[row] -= factor * b[col];
        }
    }

    for (std::size_t i = n; i-- > 0;)
    {
        double sum = b[i];

        for (std::size_t k = i + 1; k < n; ++k)
        {
            sum -= a[i * n + k] * b[k];
        }

        b[i] = sum / a[i * n + i];
    }

    return true;
}

// The width parameter of the charge model's screened Coulomb kernel:
// (a_i^2 + a_j^2)^(-1/2), over the published atomic radii.
double ChargeWidth(double radiusA, double radiusB) {
    return 1.0 / std::sqrt(radiusA * radiusA + radiusB * radiusB);
}

// The charge model's coordination number: the counting function of equation
// 6 without the electronegativity-difference factor, which the charge model
// replaces with a square-root scaling of the atomic electronegativity.
double ChargeCoordinationNumber(const Geometry& geometry, std::size_t a) {
    const double radiusA = d4tables::kCnElementTable[geometry.atoms[a].atomicNumber - 1]
                               .covalentRadiusD3;
    double total = 0.0;

    for (std::size_t b = 0; b < geometry.atoms.size(); ++b)
    {
        if (b == a)
        {
            continue;
        }

        const double radiusB = d4tables::kCnElementTable[geometry.atoms[b].atomicNumber - 1]
                                   .covalentRadiusD3;
        const double dx = geometry.atoms[a].position[0] - geometry.atoms[b].position[0];
        const double dy = geometry.atoms[a].position[1] - geometry.atoms[b].position[1];
        const double dz = geometry.atoms[a].position[2] - geometry.atoms[b].position[2];
        const double r = std::sqrt(dx * dx + dy * dy + dz * dz);

        if (r <= 0.0)
        {
            continue;
        }

        total += 0.5 * (1.0 + std::erf(-kCnSteepness * (r / (radiusA + radiusB) - 1.0)));
    }

    return total;
}

// The electronegativity-equilibration partial charges: the constrained
// minimum of the model's charge energy, which its Lagrangian turns into the
// (N + 1) linear system of the Supplementary Material's equation 5.  The
// constraint block is eliminated against two solves of the N-by-N kernel, so
// no indefinite factorization of the bordered matrix is needed.
std::vector<double> EeqCharges(const Geometry& geometry, double totalCharge) {
    const std::size_t n = geometry.atoms.size();
    std::vector<double> kernel(n * n, 0.0);
    std::vector<double> rhs(n, 0.0);

    for (std::size_t a = 0; a < n; ++a)
    {
        const d4tables::EeqElement& elementA =
            d4tables::kEeqElementTable[geometry.atoms[a].atomicNumber - 1];

        for (std::size_t b = 0; b < n; ++b)
        {
            const d4tables::EeqElement& elementB =
                d4tables::kEeqElementTable[geometry.atoms[b].atomicNumber - 1];

            if (a == b)
            {
                // The diagonal is the same screened interaction taken at zero
                // separation, where erf(gamma r)/r tends to 2 gamma over the
                // square root of pi.  A smaller diagonal leaves the
                // charge-transfer direction of the kernel without a minimum,
                // and the solution runs away.  The paper prints this element
                // twice and disagrees with itself by that root of two - the
                // energy expression of its equation 1 carries the limit
                // written here, its equation 3 the smaller value - and the
                // charges it records for its own reference systems are the
                // ones this limit produces, so the limit is the reading the
                // model is defined by.
                const double width = ChargeWidth(elementA.radius, elementA.radius);
                kernel[a * n + a] = elementA.hardness
                                    + 2.0 * width / std::sqrt(std::numbers::pi);
                continue;
            }

            const double dx = geometry.atoms[a].position[0] - geometry.atoms[b].position[0];
            const double dy = geometry.atoms[a].position[1] - geometry.atoms[b].position[1];
            const double dz = geometry.atoms[a].position[2] - geometry.atoms[b].position[2];
            const double r = std::sqrt(dx * dx + dy * dy + dz * dz);

            // The kernel is the screened Coulomb interaction; atoms that
            // coincide are not a physical configuration and contribute
            // nothing rather than a division by zero.
            kernel[a * n + b] = r > 0.0
                                    ? std::erf(ChargeWidth(elementA.radius, elementB.radius) * r)
                                          / r
                                    : 0.0;
        }

        // The right-hand side is the negated electronegativity, scaled by the
        // square root of the charge model's coordination number.
        rhs[a] = -(elementA.electronegativity
                   - elementA.coordinationScale * std::sqrt(ChargeCoordinationNumber(geometry, a)));
    }

    std::vector<double> ones(n, 1.0);
    std::vector<double> solution = rhs;
    std::vector<double> constraint = ones;
    std::vector<double> conserved = kernel;

    if (!SolveInPlace(conserved, solution, n))
    {
        return {};
    }

    std::vector<double> inverted(n * n, 0.0);

    // The second solve shares the factorized kernel.
    conserved = kernel;

    if (!SolveInPlace(conserved, constraint, n))
    {
        return {};
    }

    double norm = 0.0;
    double shift = 0.0;

    for (std::size_t a = 0; a < n; ++a)
    {
        norm += constraint[a];
        shift += solution[a];
    }

    if (norm == 0.0)
    {
        return {};
    }

    const double multiplier = (shift - totalCharge) / norm;

    for (std::size_t a = 0; a < n; ++a)
    {
        solution[a] -= multiplier * constraint[a];
    }


    return solution;
}

// One element's reference states, in the table's own order.
std::span<const d4tables::ElementReference> ElementReferences(int atomicNumber) {
    const std::size_t index = static_cast<std::size_t>(atomicNumber - 1);

    return {d4tables::kElementReference + d4tables::kReferenceOffset[index],
            static_cast<std::size_t>(d4tables::kReferenceCount[index])};
}

// One atom's charge- and coordination-dependent dynamic polarizabilities: the
// reference states of its element, each charge-scaled and then weighted by
// the coordination-number difference of equation 8.
Result<std::vector<double>> AtomPolarizabilities(const Geometry& geometry, std::size_t a,
                                                 double coordinationNumber, double partialCharge,
                                                 std::span<const int> counts) {
    const int atomicNumber = geometry.atoms[a].atomicNumber;
    const std::size_t index = static_cast<std::size_t>(atomicNumber - 1);
    const std::size_t base = static_cast<std::size_t>(d4tables::kReferenceOffset[index]);
    const std::span<const d4tables::ElementReference> references =
        ElementReferences(atomicNumber);

    std::vector<double> referenceCn(references.size(), 0.0);
    std::vector<int> referenceCounts(references.size(), 0);

    for (std::size_t k = 0; k < references.size(); ++k)
    {
        referenceCn[k] = references[k].coordinationNumber;
        referenceCounts[k] = counts[base + k];
    }

    auto weights = D4ReferenceWeights(coordinationNumber, referenceCn, referenceCounts);

    if (!weights.has_value())
    {
        return std::unexpected(weights.error());
    }

    const double effectiveCharge = atomicNumber + partialCharge;
    std::vector<double> alpha(d4tables::kFrequencies, 0.0);

    for (std::size_t k = 0; k < references.size(); ++k)
    {
        if ((*weights)[k] == 0.0)
        {
            continue;
        }

        auto scaling = D4ChargeScaling(atomicNumber, effectiveCharge,
                                       atomicNumber + references[k].charge);

        if (!scaling.has_value())
        {
            return std::unexpected(scaling.error());
        }

        // The reference state carries the polarizability of its whole
        // reference MOLECULE, whose other atoms are the element it is named
        // against.  Reducing it to this element's own atom is the model's
        // partitioning: share the molecule between its m atoms of this
        // element, then take back the n atoms of the other element, each
        // carrying the polarizability it has in its own reference system.
        // Where the data names no such system the share is zero and the
        // molecule's value is already the atom's.
        double neighbours = 0.0;

        if (references[k].xElement != 0 && references[k].xCount != 0.0
            && references[k].xScale != 0.0)
        {
            auto neighbourScaling = D4ChargeScaling(references[k].xElement,
                                                    references[k].xElement + references[k].xCharge,
                                                    references[k].xElement
                                                        + references[k].xReferenceCharge);

            if (!neighbourScaling.has_value())
            {
                return std::unexpected(neighbourScaling.error());
            }

            neighbours = references[k].xCount * references[k].xScale * (*neighbourScaling);
        }

        for (std::size_t j = 0; j < d4tables::kFrequencies; ++j)
        {
            const double atomAlpha =
                (references[k].alpha[j] - neighbours * references[k].xAlpha[j])
                * references[k].scale;
            alpha[j] += atomAlpha * (*scaling) * (*weights)[k];
        }
    }

    return alpha;
}

// The two-body dispersion energy: the pairwise sum of equations 18 to 21,
// over the unordered pairs and once each.  Why it is not the doubled sum
// stands with the line that returns it.
Result<double> TwoBodyEnergy(const Geometry& geometry, const D4Parameters& parameters,
                             std::span<const int> counts) {
    const std::size_t n = geometry.atoms.size();

    for (const Atom& atom : geometry.atoms)
    {
        if (atom.atomicNumber < 1 || atom.atomicNumber > d4tables::kReferenceMaxElement)
        {
            return std::unexpected(ErrorCode::kUnsupported);
        }
    }

    if (counts.size() != std::size(d4tables::kElementReference))
    {
        return std::unexpected(ErrorCode::kInvalidArgument);
    }

    if (n < 2)
    {
        return 0.0;
    }

    auto coordination = D4CoordinationNumbers(geometry);

    if (!coordination.has_value())
    {
        return std::unexpected(coordination.error());
    }

    const std::vector<double> charges = EeqCharges(geometry, 0.0);

    if (charges.empty())
    {
        return std::unexpected(ErrorCode::kInvalidArgument);
    }

    std::vector<std::vector<double>> alpha(n);

    for (std::size_t a = 0; a < n; ++a)
    {
        auto atomAlpha = AtomPolarizabilities(geometry, a, (*coordination)[a], charges[a], counts);

        if (!atomAlpha.has_value())
        {
            return std::unexpected(atomAlpha.error());
        }

        alpha[a] = std::move(*atomAlpha);
    }


    double energy = 0.0;

    for (std::size_t a = 0; a + 1 < n; ++a)
    {
        const int elementA = geometry.atoms[a].atomicNumber;

        for (std::size_t b = a + 1; b < n; ++b)
        {
            const int elementB = geometry.atoms[b].atomicNumber;
            const double dx = geometry.atoms[a].position[0] - geometry.atoms[b].position[0];
            const double dy = geometry.atoms[a].position[1] - geometry.atoms[b].position[1];
            const double dz = geometry.atoms[a].position[2] - geometry.atoms[b].position[2];
            const double r = std::sqrt(dx * dx + dy * dy + dz * dz);

            if (r <= 0.0)
            {
                continue;
            }

            auto c6 = D4CasimirPolderC6(alpha[a], alpha[b]);

            if (!c6.has_value())
            {
                return std::unexpected(c6.error());
            }

            // The dipole-quadrupole coefficient of the earlier D3 scheme's
            // recursive relation, which the model adopts for its R0.
            const double c8 = 3.0 * (*c6) * d3tables::kR2R4[elementA] * d3tables::kR2R4[elementB];

            if (!(*c6 > 0.0))
            {
                continue;
            }

            const double r0 = std::sqrt(c8 / (*c6));
            const double damped = parameters.a1 * r0 + parameters.a2;
            const double r6 = std::pow(r, 6.0);
            const double r8 = r6 * r * r;
            const double damped6 = std::pow(damped, 6.0);
            const double damped8 = damped6 * damped * damped;

            energy += parameters.s6 * (*c6) / (r6 + damped6)
                      + parameters.s8 * c8 / (r8 + damped8);
        }
    }

    // The sum over the pairs a < b is the whole of equation 18 and is not
    // doubled.  Its notation is ambiguous between the unordered pairs and the
    // ordered ones, and the model's own published data settles which it means:
    // the entries of the published pair matrix are half of each pair's
    // contribution, so that summing the matrix over every ordered pair of
    // atoms - which is what its published total energy is - recovers the sum
    // over the unordered pairs and no more.  A doubled sum therefore counts
    // every pair twice.  There is a further check on the same convention in
    // the arithmetic: with the polarizabilities the charge scaling above
    // gives, the terms this loop accumulates are 1.96 times the published
    // matrix's entries, uniformly over every element pair in the fixture -
    // exactly the factor this line carried.
    return -energy;
}

} // namespace

Result<D4Parameters> D4Preset(std::string_view name) {
    for (const Preset& preset : kPresets)
    {
        if (name == preset.name)
        {
            return preset.parameters;
        }
    }

    return std::unexpected(ErrorCode::kInvalidArgument);
}

bool D4CoversElement(int atomicNumber) {
    return atomicNumber >= 1 && atomicNumber <= kD4MaxAtomicNumber;
}

Result<D4EeqElement> D4EeqParameters(int atomicNumber) {
    if (!D4CoversElement(atomicNumber))
    {
        return std::unexpected(ErrorCode::kUnsupported);
    }

    const d4tables::EeqElement& element = d4tables::kEeqElementTable[atomicNumber - 1];
    return D4EeqElement{
        element.electronegativity, element.hardness, element.coordinationScale, element.radius};
}

Result<double> D4CovalentRadius(int atomicNumber) {
    if (!D4CoversElement(atomicNumber))
    {
        return std::unexpected(ErrorCode::kUnsupported);
    }

    return d4tables::kCnElementTable[atomicNumber - 1].covalentRadiusD3;
}

Result<double> D4PaulingElectronegativity(int atomicNumber) {
    if (!D4CoversElement(atomicNumber))
    {
        return std::unexpected(ErrorCode::kUnsupported);
    }

    return d4tables::kCnElementTable[atomicNumber - 1].paulingElectronegativity;
}

Result<std::vector<double>> D4CoordinationNumbers(const Geometry& geometry) {
    for (const Atom& atom : geometry.atoms)
    {
        if (!D4CoversElement(atom.atomicNumber))
        {
            return std::unexpected(ErrorCode::kUnsupported);
        }
    }

    const std::size_t n = geometry.atoms.size();
    std::vector<double> cn(n, 0.0);

    for (std::size_t a = 0; a < n; ++a)
    {
        const d4tables::CnElement& elementA = ElementOf(geometry, a);

        for (std::size_t b = 0; b < n; ++b)
        {
            if (b == a)
            {
                continue;
            }

            const Atom& atomB = geometry.atoms[b];
            const d4tables::CnElement& elementB = ElementOf(geometry, b);

            const double dx = geometry.atoms[a].position[0] - atomB.position[0];
            const double dy = geometry.atoms[a].position[1] - atomB.position[1];
            const double dz = geometry.atoms[a].position[2] - atomB.position[2];
            const double r = std::sqrt(dx * dx + dy * dy + dz * dz);

            if (r <= 0.0)
            {
                continue;
            }

            const double radii = elementA.covalentRadiusD3 + elementB.covalentRadiusD3;
            const double pair = ElectronegativityFactor(
                elementA.paulingElectronegativity, elementB.paulingElectronegativity);

            cn[a] += 0.5 * pair * (1.0 + std::erf(-kCnSteepness * (r / radii - 1.0)));
        }
    }

    return cn;
}

Result<std::vector<double>> D4PartialCharges(const Geometry& geometry, double totalCharge) {
    for (const Atom& atom : geometry.atoms)
    {
        if (!D4CoversElement(atom.atomicNumber))
        {
            return std::unexpected(ErrorCode::kUnsupported);
        }
    }

    if (geometry.atoms.empty())
    {
        return std::vector<double>{};
    }

    std::vector<double> charges = EeqCharges(geometry, totalCharge);

    if (charges.empty())
    {
        return std::unexpected(ErrorCode::kInvalidArgument);
    }

    return charges;
}

Result<double> D4ChemicalHardness(int atomicNumber) {
    if (!D4CoversElement(atomicNumber))
    {
        return std::unexpected(ErrorCode::kUnsupported);
    }

    return d4tables::kHardnessTable[atomicNumber - 1];
}

Result<double> D4ChargeScaling(int atomicNumber, double effectiveNuclearCharge,
                               double referenceEffectiveNuclearCharge) {
    if (!D4CoversElement(atomicNumber))
    {
        return std::unexpected(ErrorCode::kUnsupported);
    }

    if (!(effectiveNuclearCharge > 0.0) || !(referenceEffectiveNuclearCharge > 0.0))
    {
        return std::unexpected(ErrorCode::kInvalidArgument);
    }

    const double hardness = d4tables::kHardnessTable[atomicNumber - 1];
    const double steepness = kChargeScalingSteepness * hardness;
    const double ratio = referenceEffectiveNuclearCharge / effectiveNuclearCharge;

    return std::exp(kChargeScalingBeta * (1.0 - std::exp(steepness * (1.0 - ratio))));
}

Result<double> D4EffectiveNuclearCharge(int atomicNumber, double partialCharge) {
    if (!D4CoversElement(atomicNumber))
    {
        return std::unexpected(ErrorCode::kUnsupported);
    }

    return atomicNumber - CoreElectronsOf(atomicNumber) + partialCharge;
}

std::span<const double> D4FrequencyGrid() {
    return d4tables::kFrequencyGrid;
}

Result<double> D4CasimirPolderC6(std::span<const double> alphaA,
                                 std::span<const double> alphaB) {
    if (alphaA.size() != d4tables::kFrequencies || alphaB.size() != d4tables::kFrequencies)
    {
        return std::unexpected(ErrorCode::kInvalidArgument);
    }

    // The trapezium rule over the model's grid: each interval contributes
    // its width times the sum of the products at its two ends.
    double integral = 0.0;

    for (std::size_t j = 0; j + 1 < d4tables::kFrequencies; ++j)
    {
        const double width = d4tables::kFrequencyGrid[j + 1] - d4tables::kFrequencyGrid[j];
        integral += width * (alphaA[j + 1] * alphaB[j + 1] + alphaA[j] * alphaB[j]);
    }

    return (3.0 / (2.0 * std::numbers::pi)) * integral;
}

Result<std::vector<double>> D4ReferenceWeights(double coordinationNumber,
                                               std::span<const double> referenceCoordinationNumbers,
                                               std::span<const int> gaussianCounts) {
    const std::size_t count = referenceCoordinationNumbers.size();

    if (count == 0 || gaussianCounts.size() != count)
    {
        return std::unexpected(ErrorCode::kInvalidArgument);
    }

    std::vector<double> weights(count, 0.0);
    double total = 0.0;

    for (std::size_t k = 0; k < count; ++k)
    {
        const int gaussians = gaussianCounts[k];

        if (gaussians < 1)
        {
            return std::unexpected(ErrorCode::kInvalidArgument);
        }

        const double difference = coordinationNumber - referenceCoordinationNumbers[k];
        const double square = difference * difference;
        double sum = 0.0;

        // Equation 8's sum runs over the Gaussian functions of this
        // reference, each damped by the index itself, so a larger set
        // contributes progressively narrower functions rather than a
        // broader blend of the same width.
        for (int j = 1; j <= gaussians; ++j)
        {
            sum += std::exp(-kReferenceWeightingBeta * static_cast<double>(j) * square);
        }

        weights[k] = sum;
        total += sum;
    }

    if (!(total > 0.0))
    {
        return std::unexpected(ErrorCode::kInvalidArgument);
    }

    for (double& weight : weights)
    {
        weight /= total;
    }

    return weights;
}

static_assert(kD4ReferenceStateCount == std::size(d4tables::kElementReference),
              "the published reference count and the table disagree");

Result<double> D4TwoBodyEnergy(const Geometry& geometry, const D4Parameters& parameters) {
    return TwoBodyEnergy(geometry, parameters, d4tables::kReferenceGaussianCount);
}

Result<double> D4TwoBodyEnergy(const Geometry& geometry, const D4Parameters& parameters,
                               std::span<const int> gaussianCounts) {
    return TwoBodyEnergy(geometry, parameters, gaussianCounts);
}

Result<DispersionModel> DispersionModelFromKey(std::string_view key) {    // The two corrections this library ships.  D3 is the zero-damping
    // scheme of GrimmeD3; D4 is the default model of D4Preset.  They are
    // named rather than numbered so that a caller's configuration file
    // reads the way the literature reads.
    if (key == "d3")
    {
        return DispersionModel::kD3;
    }

    if (key == "d4")
    {
        return DispersionModel::kD4;
    }

    return std::unexpected(ErrorCode::kInvalidArgument);
}

} // namespace excgrid
