#include "excgrid/grid.hpp"

namespace excgrid {

Result<AngularGrid> AngularGrid::Create(std::size_t pointCount) {
    for (std::size_t i = 0; i < internal::kLebedevSizeCount; ++i)
    {
        if (internal::kLebedevSizes[i] == pointCount)
        {
            return AngularGrid(
                pointCount, internal::kLebedevDegrees[i], internal::kLebedevOffsets[i]);
        }
    }

    return std::unexpected(ErrorCode::kInvalidArgument);
}

AngularGrid::AngularGrid(std::size_t size, std::size_t degree, std::size_t offset) noexcept :
    _size(size), _degree(degree), _offset(offset) {}

std::array<double, 3> AngularGrid::Point(std::size_t index) const noexcept {
    // The tables are flat x,y,z triples, orbit-major.
    const std::size_t base = 3 * (_offset + index);

    return {internal::kLebedevPoints[base],
            internal::kLebedevPoints[base + 1],
            internal::kLebedevPoints[base + 2]};
}

double AngularGrid::Weight(std::size_t index) const noexcept {
    return internal::kLebedevWeights[_offset + index];
}

} // namespace excgrid
