#include "d3_tables.inc"
#include "excgrid/dispersion.hpp"

#include <cmath>
#include <string_view>

namespace excgrid {

namespace {

// The scheme constants (the published DFT-D3 choices).
constexpr double k1 = 16.0; // The counting-function steepness.
constexpr double k3 = -4.0; // The C6-interpolation exponent (k3 = 4, with
                            // the paper's negative sign folded in).
constexpr double kDistanceThreshold = 2000.0; // Pair R^2 cutoff, Bohr^2.

// The pair cut-off radius r0(ab) in Bohr: the committed upper-triangle
// table (elements 1..kMaxElement), converted from Angstrom.
double R0Pair(std::size_t a, std::size_t b) {
    const std::size_t i = a <= b ? a : b;
    const std::size_t j = a <= b ? b : a;
    const std::size_t index = j + (i - 1) * (2 * d3tables::kMaxElement - i) / 2;

    return d3tables::kR0Angstrom[index] * d3tables::kBohrPerAngstrom;
}

// The CN-interpolated C6 of one element pair and its CN derivatives:
// L_ij = exp(k3 [(cn_i - CN_a)^2 + (cn_j - CN_b)^2]) over the element
// pair's reference states (the classic getc6 scheme, with the
// nearest-reference fallback).
struct C6Pair {
    double c6 = 0.0;
    double dC6dCnA = 0.0;
    double dC6dCnB = 0.0;
};

C6Pair C6Of(std::size_t a, std::size_t b, double cnA, double cnB) {
    const int countA = d3tables::kRefCount[a];
    const int countB = d3tables::kRefCount[b];
    const int base = d3tables::kC6OffsetFlat[a * (d3tables::kMaxElement + 1) + b];

    C6Pair result;

    if (countA == 1 && countB == 1)
    {
        result.c6 = d3tables::kC6[base];
        return result;
    }

    double sum = 0.0;
    double cSum = 0.0;
    double dSumA = 0.0;
    double cDSumA = 0.0;
    double dSumB = 0.0;
    double cDSumB = 0.0;
    double nearest = -1e99;
    double nearestDistance = 1e99;

    for (int i = 0; i < countA; ++i)
    {
        for (int j = 0; j < countB; ++j)
        {
            const double c6 = d3tables::kC6[base + i * countB + j];
            const double cnI = d3tables::kRefCnFlat[a * d3tables::kMaxRef + i];
            const double cnJ = d3tables::kRefCnFlat[b * d3tables::kMaxRef + j];
            const double da = cnI - cnA;
            const double db = cnJ - cnB;
            const double dist = da * da + db * db;

            if (dist < nearestDistance)
            {
                nearestDistance = dist;
                nearest = c6;
            }

            const double l = std::exp(k3 * dist);
            // dL/dCN_a = L * (-2 k3 (cn_i - CN_a)); likewise for b.
            const double dlDa = l * (-2.0 * k3 * da);
            const double dlDb = l * (-2.0 * k3 * db);
            sum += l;
            cSum += l * c6;
            dSumA += dlDa;
            cDSumA += dlDa * c6;
            dSumB += dlDb;
            cDSumB += dlDb * c6;
        }
    }

    if (sum > 1e-99)
    {
        result.c6 = cSum / sum;
        result.dC6dCnA = (cDSumA * sum - cSum * dSumA) / (sum * sum);
        result.dC6dCnB = (cDSumB * sum - cSum * dSumB) / (sum * sum);
    } else
    {
        result.c6 = nearest;
    }

    return result;
}

// The counting-function coordination numbers and their pair derivatives:
// CN_a = sum_b 1 / (1 + exp(-k1 (R_cov(ab) / r_ab - 1))).
struct Coordination {
    std::vector<double> cn; // Per atom.
    std::vector<std::vector<double>> dCnDr; // [a][b] = dCN_a / d r_ab.
};

Coordination CoordinationNumbers(const Geometry& geometry) {
    const std::size_t n = geometry.atoms.size();

    Coordination result;
    result.cn.assign(n, 0.0);
    result.dCnDr.assign(n, std::vector<double>(n, 0.0));

    for (std::size_t a = 0; a < n; ++a)
    {
        for (std::size_t b = 0; b < n; ++b)
        {
            if (b == a)
            {
                continue;
            }

            const double dx = geometry.atoms[a].position[0] - geometry.atoms[b].position[0];
            const double dy = geometry.atoms[a].position[1] - geometry.atoms[b].position[1];
            const double dz = geometry.atoms[a].position[2] - geometry.atoms[b].position[2];
            const double r2 = dx * dx + dy * dy + dz * dz;

            if (r2 > kDistanceThreshold)
            {
                continue;
            }

            const double r = std::sqrt(r2);
            const double cov = 0.5 * (d3tables::kRcov[geometry.atoms[a].atomicNumber] +
                                      d3tables::kRcov[geometry.atoms[b].atomicNumber]);
            const double h = std::exp(-k1 * (cov / r - 1.0));
            const double g = 1.0 / (1.0 + h);

            result.cn[a] += g;
            // dg/dr = -k1 cov g^2 h / r^2.
            result.dCnDr[a][b] = -k1 * cov * g * g * h / r2;
        }
    }

    return result;
}

// The two-body D3 evaluation over the whole geometry.
Result<D3Result> Evaluate(const Geometry& geometry, const D3Parameters& p) {
    const std::size_t n = geometry.atoms.size();

    for (const Atom& atom : geometry.atoms)
    {
        if (atom.atomicNumber < 1 ||
            static_cast<std::size_t>(atom.atomicNumber) > d3tables::kMaxElement)
        {
            return std::unexpected(ErrorCode::kUnsupported);
        }
    }

    const Coordination coord = CoordinationNumbers(geometry);

    D3Result result;
    result.gradient.assign(n, {0.0, 0.0, 0.0});

    // The energy accumulates the CN-chain derivatives per atom for the
    // second pass (dE/dCN_a), because the CNs couple every pair to every
    // other pair through the counting functions.
    std::vector<double> dEdCn(n, 0.0);

    double energy = 0.0;

    for (std::size_t a = 0; a + 1 < n; ++a)
    {
        for (std::size_t b = a + 1; b < n; ++b)
        {
            const double dx = geometry.atoms[a].position[0] - geometry.atoms[b].position[0];
            const double dy = geometry.atoms[a].position[1] - geometry.atoms[b].position[1];
            const double dz = geometry.atoms[a].position[2] - geometry.atoms[b].position[2];
            const double r2 = dx * dx + dy * dy + dz * dz;

            if (r2 > kDistanceThreshold)
            {
                continue;
            }

            const std::size_t za = geometry.atoms[a].atomicNumber;
            const std::size_t zb = geometry.atoms[b].atomicNumber;
            const double r = std::sqrt(r2);
            const double r0 = R0Pair(za, zb);

            // Zero damping: damp6 = 1 / (1 + 6 (rs6 r0 / r)^alpha6).
            const double u6 = p.rs6 * r0 / r;
            const double damp6 = 1.0 / (1.0 + 6.0 * std::pow(u6, p.alpha6));
            const double u8 = p.rs8 * r0 / r;
            const double damp8 = 1.0 / (1.0 + 6.0 * std::pow(u8, p.alpha8));

            const C6Pair c6 = C6Of(za, zb, coord.cn[a], coord.cn[b]);
            const double c8 = 3.0 * c6.c6 * d3tables::kR2R4[za] * d3tables::kR2R4[zb];

            const double r6 = r2 * r2 * r2;
            const double r8 = r6 * r2;

            // The positive pair terms; the energy is their negative sum.
            const double energy6 = p.s6 * c6.c6 * damp6 / r6;
            const double energy8 = p.s8 * c8 * damp8 / r8;

            energy -= energy6 + energy8;

            // d damp / dr = 6 alpha u^(alpha-1) rs r0 damp^2 / r^2.
            const double dDamp6dr =
                6.0 * p.alpha6 * std::pow(u6, p.alpha6 - 1.0) * p.rs6 * r0 * damp6 * damp6 / r2;
            const double dDamp8dr =
                6.0 * p.alpha8 * std::pow(u8, p.alpha8 - 1.0) * p.rs8 * r0 * damp8 * damp8 / r2;

            // d energyN / dr = sN cN (dampN' r^-N - N dampN r^-(N+1)).
            const double dE6dr = p.s6 * c6.c6 * (dDamp6dr / r6 - 6.0 * damp6 / (r6 * r));
            const double dE8dr = p.s8 * c8 * (dDamp8dr / r8 - 8.0 * damp8 / (r8 * r));

            // The pair's gradient contribution: dE/dr along the a-b axis.
            // dE/dR_a -= dE/dr * u_ab (energy decreases with r, dE/dr > 0).
            const double gScalar = dE6dr + dE8dr;

            for (int k = 0; k < 3; ++k)
            {
                const double unit =
                    (geometry.atoms[a].position[k] - geometry.atoms[b].position[k]) / r;
                result.gradient[a][k] -= gScalar * unit;
                result.gradient[b][k] += gScalar * unit;
            }

            // The CN chain: d energyN / d CN = sN dampN r^-N * (d cN / d CN).
            const double r2r4 = d3tables::kR2R4[za] * d3tables::kR2R4[zb];
            const double dE6dCnA = p.s6 * c6.dC6dCnA * damp6 / r6;
            const double dE6dCnB = p.s6 * c6.dC6dCnB * damp6 / r6;
            const double dE8dCnA = p.s8 * 3.0 * r2r4 * c6.dC6dCnA * damp8 / r8;
            const double dE8dCnB = p.s8 * 3.0 * r2r4 * c6.dC6dCnB * damp8 / r8;

            // dE/dCN = -(d energy6/dCN + d energy8/dCN).
            dEdCn[a] -= dE6dCnA + dE8dCnA;
            dEdCn[b] -= dE6dCnB + dE8dCnB;
        }
    }

    // The CN-chain gradient pass: dCN_a / d r_ab couples atom a to every
    // other atom b, so each atom's gradient picks up dE/dCN_a * dCN_a/dr
    // along every a-b direction.
    for (std::size_t a = 0; a < n; ++a)
    {
        if (dEdCn[a] == 0.0)
        {
            continue;
        }

        for (std::size_t b = 0; b < n; ++b)
        {
            if (b == a)
            {
                continue;
            }

            const double dx = geometry.atoms[a].position[0] - geometry.atoms[b].position[0];
            const double dy = geometry.atoms[a].position[1] - geometry.atoms[b].position[1];
            const double dz = geometry.atoms[a].position[2] - geometry.atoms[b].position[2];
            const double r = std::sqrt(dx * dx + dy * dy + dz * dz);
            const double slope = coord.dCnDr[a][b]; // dCN_a / d r_ab.

            if (slope == 0.0)
            {
                continue;
            }

            for (int k = 0; k < 3; ++k)
            {
                const double unit =
                    (geometry.atoms[a].position[k] - geometry.atoms[b].position[k]) / r;
                result.gradient[a][k] += dEdCn[a] * slope * unit;
                result.gradient[b][k] -= dEdCn[a] * slope * unit;
            }
        }
    }

    result.energy = energy;
    return result;
}

} // namespace

Result<double> GrimmeD3Energy(const Geometry& geometry, const D3Parameters& parameters) {
    Result<D3Result> full = Evaluate(geometry, parameters);

    if (!full)
    {
        return std::unexpected(full.error());
    }

    return full->energy;
}

Result<D3Result> GrimmeD3(const Geometry& geometry, const D3Parameters& parameters) {
    return Evaluate(geometry, parameters);
}

Result<D3Parameters> D3Preset(std::string_view name) {
    struct Preset {
        std::string_view name;
        D3Parameters parameters;
    };

    // The published DFT-D3 zero-damping parameter sets (s6 = 1, sr8 = 1,
    // alpha6 = 14, alpha8 = 16 throughout).
    constexpr Preset presets[] = {
        {"b3lyp", {1.0, 1.703, 1.261, 14.0, 1.0, 16.0}},
        {"pbe", {1.0, 0.722, 1.217, 14.0, 1.0, 16.0}},
        {"pbe0", {1.0, 0.928, 1.287, 14.0, 1.0, 16.0}},
        {"bp86", {1.0, 1.683, 1.139, 14.0, 1.0, 16.0}},
        {"blyp", {1.0, 1.682, 1.094, 14.0, 1.0, 16.0}},
        {"revpbe", {1.0, 1.010, 0.923, 14.0, 1.0, 16.0}},
        {"pbesol", {1.0, 0.612, 1.345, 14.0, 1.0, 16.0}},
        {"b97d", {1.0, 0.909, 0.892, 14.0, 1.0, 16.0}},
        {"tpss", {1.0, 1.105, 1.166, 14.0, 1.0, 16.0}},
        {"bpbe", {1.0, 2.033, 1.087, 14.0, 1.0, 16.0}},
        {"bhandhlyp", {1.0, 1.442, 1.370, 14.0, 1.0, 16.0}},
        {"b3pw91", {1.0, 1.775, 1.176, 14.0, 1.0, 16.0}},
        {"hf", {1.0, 1.746, 1.158, 14.0, 1.0, 16.0}},
    };

    for (const Preset& preset : presets)
    {
        if (name == preset.name)
        {
            return preset.parameters;
        }
    }

    return std::unexpected(ErrorCode::kInvalidArgument);
}

} // namespace excgrid
