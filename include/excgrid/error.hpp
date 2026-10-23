#pragma once

#include <expected>

namespace excgrid {

/// \defgroup excgrid-error Error reporting
///
/// Fallible construction returns `Result<T>`; consumers translate the codes.
/// \{

/// The error codes the library reports.
/// \ingroup excgrid-error
enum class ErrorCode {
    kInvalidArgument, ///< A parameter or input value is out of contract.
    kUnsupported, ///< A requested size/feature is not shipped.
};

/// The library's result type.
/// \ingroup excgrid-error
template <typename T> using Result = std::expected<T, ErrorCode>;

} // namespace excgrid
