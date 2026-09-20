#include "excgrid/grid.hpp"

#include <cmath>
#include <utility>

namespace excgrid {

// The Murray-Handy-Laming substitution (Mol. Phys. 78 (1993) 997):
//     r(q) = alpha (q / (1 - q))^m,  q in [0, 1)
// with Euler-Maclaurin trapezoidal points q_i = i / N, N = pointCount + 1.
// The weight carries dr/dq * dq; the r^2 of the volume element belongs to
// the atomic product grid, not here.
Result<RadialGrid> RadialGrid::Create(std::size_t pointCount, double alpha, std::size_t exponent) {
    if (pointCount == 0 || exponent == 0)
    {
        return std::unexpected(ErrorCode::kInvalidArgument);
    }

    std::vector<double> points;
    std::vector<double> weights;
    points.reserve(pointCount);
    weights.reserve(pointCount);

    const double step = 1.0 / static_cast<double>(pointCount + 1);
    const double m = static_cast<double>(exponent);

    for (std::size_t i = 1; i <= pointCount; ++i)
    {
        const double q = static_cast<double>(i) * step;
        const double ratio = q / (1.0 - q);
        const double r = alpha * std::pow(ratio, m);
        // dr/dq = alpha m q^(m-1) / (1 - q)^(m+1); the trapezoidal weight
        // is dr/dq * step.
        const double jacobian = alpha * m * std::pow(q, m - 1.0) * std::pow(1.0 - q, -(m + 1.0));

        points.push_back(r);
        weights.push_back(jacobian * step);
    }

    return RadialGrid(std::move(points), std::move(weights));
}

RadialGrid::RadialGrid(std::vector<double> points, std::vector<double> weights) noexcept :
    _points(std::move(points)), _weights(std::move(weights)) {}

} // namespace excgrid
