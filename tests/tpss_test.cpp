// TPSS exchange and correlation (Tao, Perdew, Staroverov, Scuseria, PRL 91,
// 146401 (2003)): the Libxc 5.2.3 reference at three generic points, the
// 28-entry tier as the derivative of the kernel's own gradients, the
// uniform-gas, scaling and one-electron exact conditions each beside a
// deliberately broken case, the zero-gradient guard, and the TPSS energy of a
// real small system (He at its RHF/STO-3G density).

#include "excgrid/kernel.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include <utility>

namespace {

using excgrid::Component;
using excgrid::ComponentMask;
using excgrid::IndexOf;
using excgrid::PointInputs;
using excgrid::PointResult;
using excgrid::PointSecondDerivativeMatrix;
using excgrid::XcFunctional;

constexpr double kPi = 3.14159265358979323846;

/// (3/4)(6/pi)^(1/3): the spin-scaled Slater constant, e_x^LSDA = -this * rho^(4/3).
const double kSlaterSpinPrefactor = 0.75 * std::cbrt(6.0 / kPi);

/// (3/10)(6 pi^2)^(2/3): tau_sigma = this * rho_sigma^(5/3) for a uniform gas.
const double kFermiConstant = 0.3 * std::pow(6.0 * kPi * kPi, 2.0 / 3.0);

double TauUniform(double rho) {
    return kFermiConstant * std::pow(rho, 5.0 / 3.0);
}

/// The seven components the tau tier is emitted over, in kernel argument order.
const std::array<Component, 7> kComponents = {Component::RhoA,
                                              Component::RhoB,
                                              Component::SigmaAa,
                                              Component::SigmaAb,
                                              Component::SigmaBb,
                                              Component::TauA,
                                              Component::TauB};

struct Point {
    double rhoA = 0.0;
    double rhoB = 0.0;
    double sigmaAa = 0.0;
    double sigmaAb = 0.0;
    double sigmaBb = 0.0;
    double tauA = 0.0;
    double tauB = 0.0;
};

PointInputs Inputs(const Point& point) {
    PointInputs inputs;
    inputs.Set(Component::RhoA, point.rhoA);
    inputs.Set(Component::RhoB, point.rhoB);
    inputs.Set(Component::SigmaAa, point.sigmaAa);
    inputs.Set(Component::SigmaAb, point.sigmaAb);
    inputs.Set(Component::SigmaBb, point.sigmaBb);
    inputs.Set(Component::TauA, point.tauA);
    inputs.Set(Component::TauB, point.tauB);
    return inputs;
}

/// A GGA input: no kinetic-energy densities in the mask.
PointInputs GgaInputs(double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
    PointInputs inputs;
    inputs.Set(Component::RhoA, rhoA);
    inputs.Set(Component::RhoB, rhoB);
    inputs.Set(Component::SigmaAa, sigmaAa);
    inputs.Set(Component::SigmaAb, sigmaAb);
    inputs.Set(Component::SigmaBb, sigmaBb);
    return inputs;
}

const XcFunctional* Find(const std::string_view name) {
    const XcFunctional* functional = excgrid::FindFunctional(name);
    EXPECT_NE(functional, nullptr) << name;
    return functional;
}

PointResult Evaluate(const std::string_view name, const Point& point) {
    PointResult result;
    const XcFunctional* functional = Find(name);
    if (functional == nullptr)
    {
        return result;
    }
    EXPECT_EQ(functional->EvaluatePoint(Inputs(point), result), excgrid::KernelStatus::kOk) << name;
    return result;
}

PointResult EvaluateGga(const std::string_view name,
                        double rhoA,
                        double rhoB,
                        double sigmaAa,
                        double sigmaAb,
                        double sigmaBb) {
    PointResult result;
    const XcFunctional* functional = Find(name);
    if (functional == nullptr)
    {
        return result;
    }
    EXPECT_EQ(functional->EvaluatePoint(GgaInputs(rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb), result),
              excgrid::KernelStatus::kOk)
        << name;
    return result;
}

PointSecondDerivativeMatrix Tier(const std::string_view name, const Point& point) {
    PointSecondDerivativeMatrix matrix;
    const XcFunctional* functional = Find(name);
    if (functional == nullptr)
    {
        return matrix;
    }
    PointResult result;
    EXPECT_EQ(functional->EvaluatePointMaterialising(Inputs(point), result, matrix),
              excgrid::KernelStatus::kOk)
        << name;
    return matrix;
}

// ---------------------------------------------------------------------------
// The independent reference: Libxc 5.2.3, MGGA_X_TPSS (id 202) and
// MGGA_C_TPSS (id 231), first derivatives of the energy density and the
// 28-entry second-derivative tier, at three generic spin-polarized points.
// The one entry Libxc's undersized v2rhotau[3] buffer cannot print, (rB, tB),
// is the (rA, tA) entry of the spin-swapped point (the symmetry was checked:
// the printed cross entries agree to 7e-18).

/// edens, vrhoA, vrhoB, vsigmaAa, vsigmaAb, vsigmaBb, vtauA, vtauB.
struct KernelRow {
    std::array<double, 8> values;
};

/// The tier over (rhoA, rhoB, sigmaAa, sigmaAb, sigmaBb, tauA, tauB), in the
/// packed upper-triangle order of PointSecondDerivativeMatrix.
struct TierRow {
    std::array<double, 28> values;
};

const std::array<Point, 3> kReferencePoints = {{
    {0.7, 0.3, 0.25, 0.02, 0.11, 0.45, 0.16},
    {1.3, 0.9, 0.8, 0.1, 0.5, 0.99, 0.63},
    {0.4, 0.4, 0.12, 0.0, 0.12, 0.3, 0.3},
}};

const std::array<KernelRow, 3> kExchangeKernel = {{
    {-0.7798991578190562,
     -1.1177823767558661,
     -0.8354949751310807,
     -0.014100934227305954,
     0.0,
     -0.05832622463622496,
     0.011925700567808614,
     0.04048494673248404},
    {-2.16136085357357,
     -1.37433918521827,
     -1.2153017499049685,
     -0.006975368983995008,
     0.0,
     -0.01132284683587332,
     0.009251984874813882,
     0.012676367146387816},
    {-0.5567373123237526,
     -0.9263171761280683,
     -0.9263171761280683,
     -0.02157783630393699,
     0.0,
     -0.02157783630393699,
     0.012547139705304146,
     0.012547139705304146},
}};

const std::array<TierRow, 3> kExchangeTier = {{
    {-0.5358405910152951,
     -1.1102230246251565e-16,
     0.006432371052459859,
     0.0,
     1.3877787807814457e-17,
     -0.002391945657185061,
     0.0,
     -1.0095112850018173,
     1.734723475976807e-18,
     0.0,
     0.13089992076832524,
     0.0,
     -0.05265843613196412,
     -0.023114792804482795,
     0.0,
     0.0,
     0.03961126369245496,
     0.0,
     0.0,
     0.0,
     0.0,
     0.0,
     -0.3297842491545261,
     0.0,
     0.5071313863867376,
     -0.03827784092116886,
     0.0,
     -0.5492099677925568},
    {-0.3529625750920742,
     1.6653345369377348e-16,
     0.0009962066602889246,
     0.0,
     0.0,
     -0.0008405787848544547,
     8.673617379884035e-19,
     -0.4535929592445237,
     8.673617379884035e-19,
     0.0,
     0.00419964082173907,
     -1.734723475976807e-18,
     -0.0023489915021304908,
     -0.0029329230881859434,
     0.0,
     0.0,
     0.00864383388388753,
     0.0,
     0.0,
     0.0,
     0.0,
     0.0,
     -0.0099968844777349,
     0.0,
     0.023472982551027744,
     -0.012382679691062971,
     0.0,
     -0.03181778422055443},
    {-0.792455094725145,
     -2.220446049250313e-16,
     0.030826444748630364,
     0.0,
     0.0,
     -0.003309632944386319,
     3.469446951953614e-18,
     -0.792455094725145,
     0.0,
     0.0,
     0.030826444748630364,
     3.469446951953614e-18,
     -0.003309632944386319,
     -0.0891486378635207,
     0.0,
     0.0,
     0.08993486924424764,
     0.0,
     0.0,
     0.0,
     0.0,
     0.0,
     -0.0891486378635207,
     0.0,
     0.08993486924424764,
     -0.06327536976434552,
     0.0,
     -0.06327536976434552},
}};

const std::array<KernelRow, 3> kCorrelationKernel = {{
    {-0.0656627828162083,
     -0.06559795388216311,
     -0.10459104501976771,
     0.004278096039696388,
     0.008614901019492603,
     0.00423125687749049,
     -0.00024559652207882234,
     -0.0002455965220788225},
    {-0.16708247925741393,
     -0.07916348584169736,
     -0.09769649291263148,
     0.0015485963536532147,
     0.0031099391588628794,
     0.0015446103913943539,
     -0.0001173713315480048,
     -0.0001173713315480048},
    {-0.05412217314036736,
     -0.07967868927401112,
     -0.07967868927401112,
     0.005707660490870415,
     0.011481036328048044,
     0.005707660490870415,
     -0.0001348245114298059,
     -0.0001348245114298059},
}};

const std::array<TierRow, 3> kCorrelationTier = {{
    {0.019265355523124875,    -0.05231597149174702,   -0.005433667982537597,
     -0.01105258989002425,    -0.005408433923456898,  0.0003447357835577697,
     0.0003447357835577694,   0.09688789087836566,    -0.003474813388427722,
     -0.007035325942896182,   -0.0033051145691998986, -0.00040764358697845566,
     -0.00040764358697845614, 0.0002524498628356951,  0.0007499502882894819,
     -5.274638523777218e-06,  -0.0012105461317063271, -0.0012105461317063262,
     0.002094129199075397,    0.0002761224402100904,  -0.0026135221436984226,
     -0.0026135221436984217,  0.00012636078412411937, -0.0010570217389879364,
     -0.0010570217389879355,  0.0012610402885935724,  0.0012610402885935732,
     0.0012610402885935735},
    {0.013839833191403448,    -0.025606759287867695,  -0.0008927447821663352,
     -0.0018069467796771083,  -0.0008954711321186136, 5.8057161607043635e-05,
     5.80571616070437e-05,    0.02904941645861056,    -0.0007879295365067734,
     -0.0015862446405329684,  -0.0007721784000851896, 8.409576342473396e-06,
     8.409576342473396e-06,   4.6008603640516225e-05, 0.00010696487202727454,
     3.970625230157941e-05,   -0.0001553814329218991, -0.0001553814329218991,
     0.00024809010715110685,  9.628755944470957e-05,  -0.00032649776438181395,
     -0.00032649776438181395, 3.800089357819175e-05,  -0.00015046094886076434,
     -0.00015046094886076434, 0.000223243933055327,   0.00022324393305532693,
     0.000223243933055327},
    {0.051859809666407124,    -0.061840846339333726,   -0.008235673579479389,
     -0.016740891519318138,   -0.008304274911462168,   7.959282633810453e-05,
     7.959282633810453e-05,   0.051859809666407124,    -0.008304274911462168,
     -0.016740891519318138,   -0.008235673579479389,   7.959282633810453e-05,
     7.959282633810453e-05,   -0.00012877618118864345, 0.00011871614515435435,
     -0.00021321112296848104, -0.0010623302118255582,  -0.0010623302118255582,
     0.001341759071913901,    0.00011871614515435435,  -0.0023436811961706928,
     -0.0023436811961706928,  -0.00012877618118864307, -0.0010623302118255582,
     -0.0010623302118255582,  0.0006948695914542875,   0.0006948695914542883,
     0.0006948695914542875},
}};

/// The brief's 1e-8 agreement, absolute against a reference of order one.
double ReferenceTolerance(double reference) {
    return 1e-8 * std::max(1.0, std::abs(reference));
}

TEST(TpssTest, TheKernelsAndTheirTiersReproduceTheLibxcReference) {
    struct Half {
        std::string_view name;
        const std::array<KernelRow, 3>* kernel;
        const std::array<TierRow, 3>* tier;
    };

    const std::array<Half, 2> halves = {{
        {"tpss_x", &kExchangeKernel, &kExchangeTier},
        {"tpss_c", &kCorrelationKernel, &kCorrelationTier},
    }};

    for (const Half& half : halves)
    {
        for (std::size_t index = 0; index < kReferencePoints.size(); ++index)
        {
            const Point& point = kReferencePoints[index];
            const std::string where = std::string(half.name) + " point " + std::to_string(index);

            const PointResult result = Evaluate(half.name, point);
            const KernelRow& kernel = (*half.kernel)[index];

            EXPECT_NEAR(result.exc, kernel.values[0], ReferenceTolerance(kernel.values[0]))
                << where;
            for (std::size_t component = 0; component < kComponents.size(); ++component)
            {
                EXPECT_NEAR(result.first[IndexOf(kComponents[component])],
                            kernel.values[component + 1],
                            ReferenceTolerance(kernel.values[component + 1]))
                    << where << " first derivative " << component;
            }

            const PointSecondDerivativeMatrix matrix = Tier(half.name, point);
            ASSERT_EQ(matrix.mask.ActiveCount(), kComponents.size()) << where;
            const TierRow& tier = (*half.tier)[index];
            for (std::size_t entry = 0; entry < tier.values.size(); ++entry)
            {
                EXPECT_NEAR(
                    matrix.upper[entry], tier.values[entry], ReferenceTolerance(tier.values[entry]))
                    << where << " tier entry " << entry;
            }
        }
    }
}

// ---------------------------------------------------------------------------
// The tier as the derivative of the kernel's own first derivatives

// A point with both channels at a physical tau_P and a nonzero cross gradient:
// nothing here is at a symmetry that could hide a swapped slot.
Point kDifferencePoint = {0.61, 0.37, 0.19, 0.03, 0.08, 0.55, 0.22};

double ValueOf(const Point& point, std::size_t slot) {
    switch (slot)
    {
    case 0:
        return point.rhoA;
    case 1:
        return point.rhoB;
    case 2:
        return point.sigmaAa;
    case 3:
        return point.sigmaAb;
    case 4:
        return point.sigmaBb;
    case 5:
        return point.tauA;
    default:
        return point.tauB;
    }
}

TEST(TpssTest, TheTierIsTheDerivativeOfTheKernelsOwnFirstDerivatives) {
    for (const std::string_view name : {std::string_view("tpss_x"), std::string_view("tpss_c")})
    {
        const PointSecondDerivativeMatrix matrix = Tier(name, kDifferencePoint);
        ASSERT_EQ(matrix.mask.ActiveCount(), kComponents.size()) << name;

        const PointInputs inputs = Inputs(kDifferencePoint);

        const auto firstAt = [&](std::size_t slot, double value) {
            PointInputs shifted = inputs;
            shifted.Set(kComponents[slot], value);
            PointResult result;
            EXPECT_EQ(Find(name)->EvaluatePoint(shifted, result), excgrid::KernelStatus::kOk)
                << name;
            return result;
        };

        for (std::size_t i = 0; i < kComponents.size(); ++i)
        {
            for (std::size_t j = i; j < kComponents.size(); ++j)
            {
                const double centre = ValueOf(kDifferencePoint, j);
                const double step = 1e-5 * std::max(1.0, std::abs(centre));

                const double plus = firstAt(j, centre + step).first[IndexOf(kComponents[i])];
                const double minus = firstAt(j, centre - step).first[IndexOf(kComponents[i])];
                const double difference = (plus - minus) / (2.0 * step);
                const double emitted = matrix.upper[i * 7 - i * (i - 1) / 2 + (j - i)];

                EXPECT_NEAR(emitted, difference, 1e-5 * std::max(1.0, std::abs(difference)))
                    << name << " entry (" << i << "," << j << ")";
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Exact condition: the uniform-gas limit

TEST(TpssTest, TheUniformGasLimitIsTheLsdaAndPbeEnergyDensity) {
    const std::array<std::pair<double, double>, 3> gases = {
        {{0.26, 0.14}, {0.05, 0.11}, {0.4, 0.4}}};

    for (const auto& [rhoA, rhoB] : gases)
    {
        Point point;
        point.rhoA = rhoA;
        point.rhoB = rhoB;
        point.tauA = TauUniform(rhoA);
        point.tauB = TauUniform(rhoB);

        // Exchange: F_x = 1 exactly at the uniform gas, where tau_P = tau_unif.
        const double lsda =
            -kSlaterSpinPrefactor * (std::pow(rhoA, 4.0 / 3.0) + std::pow(rhoB, 4.0 / 3.0));
        const PointResult exchange = Evaluate("tpss_x", point);
        EXPECT_NEAR(exchange.exc, lsda, 1e-14 * std::abs(lsda))
            << "rhoA = " << rhoA << ", rhoB = " << rhoB;

        // Correlation: at a zero gradient TPSS reduces to PBE, whatever tau is.
        const PointResult correlation = Evaluate("tpss_c", point);
        const PointResult pbe = EvaluateGga("pbe_c", rhoA, rhoB, 0.0, 0.0, 0.0);
        ASSERT_NE(pbe.exc, 0.0);
        EXPECT_NEAR(correlation.exc, pbe.exc, 1e-14 * std::abs(pbe.exc))
            << "rhoA = " << rhoA << ", rhoB = " << rhoB;

        Point moved = point;
        moved.tauA = 2.0 * point.tauA;
        moved.tauB = 2.0 * point.tauB;
        EXPECT_NEAR(
            Evaluate("tpss_c", moved).exc, correlation.exc, 1e-14 * std::abs(correlation.exc))
            << "the correlation must not read tau at a zero gradient";
    }
}

TEST(TpssTest, TheUniformGasGateRejectsABrokenPoint) {
    Point point;
    point.rhoA = 0.26;
    point.rhoB = 0.14;
    point.tauA = TauUniform(point.rhoA);
    point.tauB = TauUniform(point.rhoB);

    const double lsda =
        -kSlaterSpinPrefactor * (std::pow(point.rhoA, 4.0 / 3.0) + std::pow(point.rhoB, 4.0 / 3.0));

    // A kinetic-energy density one tenth below the uniform gas: the exchange
    // enhancement moves to first order in tau_P/tau_unif and the LSDA identity
    // no longer holds.  The gate must see it.
    Point brokenExchange = point;
    brokenExchange.tauA = 0.9 * point.tauA;
    brokenExchange.tauB = 0.9 * point.tauB;
    const double exchangeMiss =
        std::abs(Evaluate("tpss_x", brokenExchange).exc - lsda) / std::abs(lsda);
    EXPECT_GT(exchangeMiss, 1e-5) << "the broken uniform-gas exchange passed the gate";

    // A nonzero gradient at the same tau: the correlation leaves PBE.
    Point brokenCorrelation = point;
    brokenCorrelation.sigmaAa = 0.04;
    brokenCorrelation.sigmaAb = 0.01;
    brokenCorrelation.sigmaBb = 0.03;
    const PointResult pbe = EvaluateGga("pbe_c", point.rhoA, point.rhoB, 0.04, 0.01, 0.03);
    const double correlationMiss =
        std::abs(Evaluate("tpss_c", brokenCorrelation).exc - pbe.exc) / std::abs(pbe.exc);
    EXPECT_GT(correlationMiss, 1e-5) << "the broken uniform-gas correlation passed the gate";
}

// ---------------------------------------------------------------------------
// Exact condition: exchange scaling, e(lam^3 rho, lam^8 sigma, lam^5 tau) = lam^4 e

TEST(TpssTest, TheExchangeObeysTheUniformScalingRelation) {
    const Point base = kReferencePoints[0];
    const PointResult reference = Evaluate("tpss_x", base);
    ASSERT_NE(reference.exc, 0.0);

    for (const double lambda : {0.5, 2.0, 3.0})
    {
        const double l3 = lambda * lambda * lambda;
        const double l5 = l3 * lambda * lambda;
        const double l8 = l5 * l3;

        Point scaled;
        scaled.rhoA = l3 * base.rhoA;
        scaled.rhoB = l3 * base.rhoB;
        scaled.sigmaAa = l8 * base.sigmaAa;
        scaled.sigmaAb = l8 * base.sigmaAb;
        scaled.sigmaBb = l8 * base.sigmaBb;
        scaled.tauA = l5 * base.tauA;
        scaled.tauB = l5 * base.tauB;

        const PointResult result = Evaluate("tpss_x", scaled);
        const double l4 = l3 * lambda;

        EXPECT_NEAR(result.exc, l4 * reference.exc, 1e-10 * std::abs(l4 * reference.exc))
            << "lambda = " << lambda;
        EXPECT_NEAR(result.first[IndexOf(Component::RhoA)],
                    lambda * reference.first[IndexOf(Component::RhoA)],
                    1e-10 * std::abs(lambda * reference.first[IndexOf(Component::RhoA)]))
            << "vrhoA, lambda = " << lambda;
        EXPECT_NEAR(result.first[IndexOf(Component::SigmaAa)],
                    reference.first[IndexOf(Component::SigmaAa)] / (l8 / l4),
                    1e-10 * std::abs(reference.first[IndexOf(Component::SigmaAa)] / (l8 / l4)))
            << "vsigmaAa, lambda = " << lambda;
        EXPECT_NEAR(result.first[IndexOf(Component::TauA)],
                    reference.first[IndexOf(Component::TauA)] / lambda,
                    1e-10 * std::abs(reference.first[IndexOf(Component::TauA)] / lambda))
            << "vtauA, lambda = " << lambda;
    }
}

TEST(TpssTest, TheScalingGateRejectsAWrongKineticEnergyExponent) {
    constexpr double kLambda = 2.0;
    const double l3 = kLambda * kLambda * kLambda;
    const double l5 = l3 * kLambda * kLambda;
    const double l8 = l5 * l3;
    const double l4 = l3 * kLambda;

    const Point base = kReferencePoints[0];
    const double reference = Evaluate("tpss_x", base).exc;

    // The sixth power of lambda is not a physical scaling of tau: an extra
    // factor lambda on tau_P and on tau_unif/tau moves the enhancement.
    Point wrong = base;
    wrong.rhoA = l3 * base.rhoA;
    wrong.rhoB = l3 * base.rhoB;
    wrong.sigmaAa = l8 * base.sigmaAa;
    wrong.sigmaAb = l8 * base.sigmaAb;
    wrong.sigmaBb = l8 * base.sigmaBb;
    wrong.tauA = l5 * kLambda * base.tauA;
    wrong.tauB = l5 * kLambda * base.tauB;

    const double miss =
        std::abs(Evaluate("tpss_x", wrong).exc / l4 - reference) / std::abs(reference);
    EXPECT_GT(miss, 1e-6) << "a wrong tau exponent passed the scaling gate";
}

// ---------------------------------------------------------------------------
// Exact condition: the one-electron self-interaction cancellation

struct OneElectronPoint {
    double rhoA = 0.0;
    double sigmaAa = 0.0;
    double tauWeizsaecker = 0.0;
    /// Libxc 5.2.3 MGGA_C_TPSS edens at tau = 1.1 tau_W (the broken control).
    double brokenEdens = 0.0;
};

const std::array<OneElectronPoint, 3> kOneElectronPoints = {{
    {0.7, 0.25, 0.044642857142857144, -0.004093265116277868},
    {1.0, 1.0, 0.125, -0.00589298503697941},
    {0.05, 0.004, 0.01, -0.00013564327347223237},
}};

TEST(TpssTest, TheCorrelationVanishesForAOneElectronDensity) {
    for (const OneElectronPoint& channel : kOneElectronPoints)
    {
        Point point;
        point.rhoA = channel.rhoA;
        point.sigmaAa = channel.sigmaAa;
        point.tauA = channel.tauWeizsaecker;

        const PointResult result = Evaluate("tpss_c", point);

        // Libxc 5.2.3 returns -5.8e-17 here; the exact condition is zero.
        EXPECT_LE(std::abs(result.exc), 1e-12)
            << "rhoA = " << channel.rhoA << ": correlation did not cancel";

        for (const Component component : kComponents)
        {
            EXPECT_TRUE(std::isfinite(result.first[IndexOf(component)]))
                << "rhoA = " << channel.rhoA << " first derivative " << IndexOf(component);
        }

        Point broken = point;
        broken.tauA = 1.1 * channel.tauWeizsaecker;
        const double brokenEdens = Evaluate("tpss_c", broken).exc;

        // The broken point is a real correlation energy, and it is Libxc's.
        EXPECT_GT(std::abs(brokenEdens), 1e-4) << "rhoA = " << channel.rhoA;
        EXPECT_NEAR(
            brokenEdens, channel.brokenEdens, 1e-8 * std::max(1.0, std::abs(channel.brokenEdens)))
            << "rhoA = " << channel.rhoA << ": the broken-tau control is not Libxc's value";
    }
}

// ---------------------------------------------------------------------------
// The zero-gradient guard: a functional written in s = sqrt(sigma) must not
// emit NaN in the sigma derivatives at an exactly zero gradient.

TEST(TpssTest, TheZeroGradientCornerIsFiniteAndContinuous) {
    constexpr double kNearlyZeroGradient = 1e-30;

    const auto corner = [](const std::string_view name, double sigma) {
        Point point;
        point.rhoA = 0.31;
        point.rhoB = 0.22;
        point.sigmaAa = sigma;
        point.sigmaAb = sigma;
        point.sigmaBb = sigma;
        point.tauA = TauUniform(point.rhoA);
        point.tauB = TauUniform(point.rhoB);
        return std::pair<PointResult, PointSecondDerivativeMatrix>{Evaluate(name, point),
                                                                   Tier(name, point)};
    };

    for (const std::string_view name : {std::string_view("tpss_x"), std::string_view("tpss_c")})
    {
        const auto [kernelAtZero, tierAtZero] = corner(name, 0.0);

        EXPECT_TRUE(std::isfinite(kernelAtZero.exc)) << name;
        for (const Component component : kComponents)
        {
            EXPECT_TRUE(std::isfinite(kernelAtZero.first[IndexOf(component)]))
                << name << " first derivative " << IndexOf(component);
        }
        for (std::size_t entry = 0; entry < tierAtZero.upper.size(); ++entry)
        {
            EXPECT_TRUE(std::isfinite(tierAtZero.upper[entry])) << name << " tier entry " << entry;
        }

        const auto [kernelAdjacent, tierAdjacent] = corner(name, kNearlyZeroGradient);
        for (std::size_t entry = 0; entry < tierAtZero.upper.size(); ++entry)
        {
            EXPECT_NEAR(tierAtZero.upper[entry],
                        tierAdjacent.upper[entry],
                        1e-9 * std::max(1.0, std::abs(tierAdjacent.upper[entry])))
                << name << " tier entry " << entry;
        }
        for (const Component component : kComponents)
        {
            const double atZero = kernelAtZero.first[IndexOf(component)];
            const double adjacent = kernelAdjacent.first[IndexOf(component)];
            EXPECT_NEAR(atZero, adjacent, 1e-9 * std::max(1.0, std::abs(adjacent)))
                << name << " first derivative " << IndexOf(component);
        }
    }
}

// The hazard the guard removes, measured on the sibling that lacks it: the
// tree's zero-gradient test pins p86's sigma-sigma curvature as divergent
// (gamma * exp(-a sqrt(gamma)) has a -3a/(4 sqrt(gamma)) curvature).  The same
// corner on TPSS must be finite.
TEST(TpssTest, TheUnguardedSiblingStillShowsTheZeroGradientHazard) {
    const XcFunctional* p86 = Find("p86");
    ASSERT_NE(p86, nullptr);

    PointResult result;
    PointSecondDerivativeMatrix matrix;
    ASSERT_EQ(p86->EvaluatePointMaterialising(GgaInputs(0.31, 0.22, 0.0, 0.0, 0.0), result, matrix),
              excgrid::KernelStatus::kOk);

    bool p86Hole = false;
    for (const double entry : matrix.upper)
    {
        p86Hole = p86Hole || !std::isfinite(entry);
    }
    EXPECT_TRUE(p86Hole) << "p86 no longer diverges at a zero gradient: the guard's broken case "
                            "has moved and this test's premise is gone";

    const Point tpss = {0.31, 0.22, 0.0, 0.0, 0.0, TauUniform(0.31), TauUniform(0.22)};
    const PointSecondDerivativeMatrix guarded = Tier("tpss_c", tpss);
    for (std::size_t entry = 0; entry < guarded.upper.size(); ++entry)
    {
        EXPECT_TRUE(std::isfinite(guarded.upper[entry])) << "tpss_c tier entry " << entry;
    }
}

// ---------------------------------------------------------------------------
// The small-system reference: the He atom at its RHF/STO-3G density

// STO-3G for He is a single contracted s function (pyscf: one basis function,
// MO coefficient 1.0, e_tot = -2.807783957539974 Ha), so the RHF density is
// analytic: rho = 2 |phi|^2 with phi the normalized contraction.  Each spin
// channel is one orbital, so tau_sigma = |grad phi|^2 / 2 exactly.
const std::array<double, 3> kHeExponents = {{6.36242139, 1.158923, 0.31364979}};
const std::array<double, 3> kHeCoefficients = {{0.15432897, 0.53532814, 0.44463454}};

double ContractionScale() {
    double norm = 0.0;
    for (std::size_t i = 0; i < kHeExponents.size(); ++i)
    {
        const double primitiveI = std::pow(2.0 * kHeExponents[i] / kPi, 0.75);
        for (std::size_t j = 0; j < kHeExponents.size(); ++j)
        {
            const double primitiveJ = std::pow(2.0 * kHeExponents[j] / kPi, 0.75);
            norm += kHeCoefficients[i] * kHeCoefficients[j] * primitiveI * primitiveJ *
                    std::pow(kPi / (kHeExponents[i] + kHeExponents[j]), 1.5);
        }
    }
    return 1.0 / std::sqrt(norm);
}

// Libxc 5.2.3 on the same nodes: the reference the kernels must reproduce.
// The quadrature is deliberately coarse and fixed; it is the same rule on both
// sides, so the comparison tests the kernels, not the quadrature.
constexpr std::size_t kRadialNodes = 24;
constexpr double kRadiusMax = 8.0;
constexpr std::size_t kThetaNodes = 10;
constexpr std::size_t kPhiNodes = 10;
constexpr double kHeExchangeReference = -1.06362722994104519e+00;
constexpr double kHeCorrelationReference = -4.56360351055058039e-02;

TEST(TpssTest, TheHeAtomReferenceEnergyIsReproduced) {
    const double scale = ContractionScale();
    const double dr = kRadiusMax / kRadialNodes;
    const double dtheta = kPi / kThetaNodes;
    const double dphi = 2.0 * kPi / kPhiNodes;

    double exchange = 0.0;
    double correlation = 0.0;

    for (std::size_t ir = 0; ir < kRadialNodes; ++ir)
    {
        const double radius = (ir + 0.5) * dr;
        for (std::size_t it = 0; it < kThetaNodes; ++it)
        {
            const double theta = (it + 0.5) * dtheta;
            const double sinTheta = std::sin(theta);
            const double cosTheta = std::cos(theta);
            for (std::size_t ip = 0; ip < kPhiNodes; ++ip)
            {
                const double azimuth = (ip + 0.5) * dphi;
                const double x = radius * sinTheta * std::cos(azimuth);
                const double y = radius * sinTheta * std::sin(azimuth);
                const double z = radius * cosTheta;

                double radial = 0.0;
                double gx = 0.0;
                double gy = 0.0;
                double gz = 0.0;
                for (std::size_t primitive = 0; primitive < kHeExponents.size(); ++primitive)
                {
                    const double alpha = kHeExponents[primitive];
                    const double gaussian = kHeCoefficients[primitive] *
                                            std::pow(2.0 * alpha / kPi, 0.75) *
                                            std::exp(-alpha * radius * radius);
                    radial += gaussian;
                    const double factor = -2.0 * alpha * gaussian;
                    gx += factor * x;
                    gy += factor * y;
                    gz += factor * z;
                }
                radial *= scale;
                gx *= scale;
                gy *= scale;
                gz *= scale;

                const double rho = radial * radial;
                const double rhoX = 2.0 * radial * gx;
                const double rhoY = 2.0 * radial * gy;
                const double rhoZ = 2.0 * radial * gz;
                const double sigmaRho = rhoX * rhoX + rhoY * rhoY + rhoZ * rhoZ;
                const double tau = 0.5 * (gx * gx + gy * gy + gz * gz);
                const double weight = radius * radius * sinTheta * dr * dtheta * dphi;

                Point point;
                point.rhoA = rho;
                point.rhoB = rho;
                point.sigmaAa = sigmaRho;
                point.sigmaAb = sigmaRho;
                point.sigmaBb = sigmaRho;
                point.tauA = tau;
                point.tauB = tau;

                exchange += weight * Evaluate("tpss_x", point).exc;
                correlation += weight * Evaluate("tpss_c", point).exc;
            }
        }
    }

    EXPECT_NEAR(exchange, kHeExchangeReference, 1e-8 * std::abs(kHeExchangeReference));
    EXPECT_NEAR(correlation, kHeCorrelationReference, 1e-8 * std::abs(kHeCorrelationReference));
    EXPECT_NEAR(exchange + correlation,
                kHeExchangeReference + kHeCorrelationReference,
                1e-8 * std::abs(kHeExchangeReference + kHeCorrelationReference));
}

} // namespace
