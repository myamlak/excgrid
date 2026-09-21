cmake_minimum_required(VERSION 3.25)

# excgrid - the grid + XC-kernel + dispersion library (the standalone DFT
# foundation repo; its frozen public seam is docs/kernel-api.md).  Layout:
# public headers under include/excgrid/, hand-written sources under
# src/, the committed Yacas-generated kernels under generated/, the
# codegen sources under xc_defs/ (maintainer tooling, never built),
# the vendored Yacas and the regeneration driver under tools/.
#
# NO PCH in this tree: a precompiled header is a build-time optimization
# for a large consumer, and this library is small enough that one would
# only obscure which header pulls what.  No foreign targets, paths, or
# types - the standalone boundary (docs/kernel-api.md section 5).  Tests
# are gtest (FetchContent, pinned) and are ON only when this tree is the
# top-level project.  The verification jobs (pylibxc/pyscf point
# checks) are CI Python steps, not CMake targets.

project(excgrid VERSION 0.2.0 LANGUAGES CXX)

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(EXCGRID_TOP_LEVEL ON)
else()
    set(EXCGRID_TOP_LEVEL OFF)
endif()

option(EXCGRID_BUILD_TESTS "Build the excgrid test suite (gtest, FetchContent)"
    ${EXCGRID_TOP_LEVEL})

# Warnings in this library's own code are build failures.  ON by default:
# a gate that is off until someone opts in is not a gate, and a consumer
# embedding this tree inherits the same standard.  Set OFF only to build
# with a compiler whose warning noise this tree has not been made clean
# for - never to land a warning.
option(EXCGRID_WERROR "Treat warnings in excgrid's own code as errors" ON)

add_library(excgrid
    src/angular_grid.cpp
    src/block_grid.cpp
    src/components.cpp
    src/d3.cpp
    src/d4.cpp
    src/grid_derivatives.cpp
    src/grid_derivs.cpp
    src/kernel.cpp
    src/kernels_registry.cpp
    src/meta_gga_reference.cpp
    src/radial_grid.cpp
    generated/becke88_exchange.cpp
    generated/lyp_correlation.cpp
    generated/mpw91_exchange.cpp
    generated/p86_correlation.cpp
    generated/pbe_correlation.cpp
    generated/pbe_exchange.cpp
    generated/pbesol_exchange.cpp
    generated/pw91_correlation.cpp
    generated/pw91_exchange.cpp
    generated/pw92_correlation.cpp
    generated/revpbe_exchange.cpp
    generated/rpbe_exchange.cpp
    generated/slater_exchange.cpp
    generated/tau_x.cpp
    generated/vwn3_correlation.cpp
    generated/vwn5_correlation.cpp
)
add_library(excgrid::excgrid ALIAS excgrid)

target_include_directories(excgrid PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_compile_features(excgrid PUBLIC cxx_std_23)

# The warning flags, and they are TARGET-SCOPED on purpose.  A
# directory-scoped add_compile_options() or a CMAKE_COMPILE_WARNING_AS_ERROR
# variable would also reach the vendored third-party targets that are
# configured below (GoogleTest, via FetchContent), turning someone else's
# warning noise into a build failure in this tree - the exact inversion
# this gate exists to avoid.  Third-party code is not this project's to
# gate, and it is not gated: gtest/gtest_main keep the flags GoogleTest's
# own build gives them.
if (MSVC)
    target_compile_options(excgrid PRIVATE /W4)
    if (EXCGRID_WERROR)
        target_compile_options(excgrid PRIVATE /WX)
    endif()
else()
    target_compile_options(excgrid PRIVATE -Wall -Wextra)
    if (EXCGRID_WERROR)
        target_compile_options(excgrid PRIVATE -Werror)
    endif()
endif()

# The kernel probe CLI the verification scripts drive (top-level builds
# only - a consumer embedding this tree never needs the probe).
if (EXCGRID_TOP_LEVEL)
    add_executable(excgrid-kernel-probe tools/kernel_probe.cpp)
    target_link_libraries(excgrid-kernel-probe PRIVATE excgrid)
    target_compile_features(excgrid-kernel-probe PRIVATE cxx_std_23)
endif()

# The zero-warning Doxygen gate: WARN_AS_ERROR YES, EXTRACT_ALL NO.
find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
    add_custom_target(excgrid-docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "excgrid API reference (zero-warning gate)"
        VERBATIM)
endif()

if (EXCGRID_BUILD_TESTS)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        URL_HASH SHA256=40d4ec942217dcc84a9ebe2a68584ada7d4a33a8ee958755763278ea1c5e18ff)
    FetchContent_MakeAvailable(googletest)

    add_executable(excgrid-tests
        tests/angular_grid_test.cpp
        tests/block_grid_partition_test.cpp
        tests/block_grid_test.cpp
        tests/contract_test.cpp
        tests/d3_test.cpp
        tests/d4_test.cpp
        tests/grid_derivatives_test.cpp
        tests/grid_derivs_test.cpp
        tests/kernel_test.cpp
        tests/meta_gga_reference_test.cpp
        tests/radial_grid_test.cpp
        tests/registry_test.cpp)
    target_link_libraries(excgrid-tests PRIVATE excgrid gtest gtest_main)

    include(GoogleTest)
    gtest_discover_tests(excgrid-tests)
endif()
