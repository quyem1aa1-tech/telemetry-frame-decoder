cmake_minimum_required(VERSION 3.20)

project(telemetry-frame-decoder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ==============================================================================
# Global Compiler Options (Interface Target)
# ==============================================================================

add_library(project_options INTERFACE)

# Enforce C++17 standard for any target linking to project_options
target_compile_features(project_options INTERFACE cxx_std_17)

# Apply strict warning flags based on the compiler being used
target_compile_options(
    project_options
    INTERFACE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
              -Wall
              -Wextra
              -Wpedantic
              -Wconversion
              -Wshadow
              -fdiagnostics-color=always
              >
              $<$<CXX_COMPILER_ID:MSVC>:
              /W4
              /permissive-
              >
)

# ==============================================================================
# External Dependencies
# ==============================================================================

include(FetchContent)

# fmt: Fast and safe string formatting library
fetchcontent_declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 12.2.0
)

set(EXPECTED_BUILD_TESTS
    OFF
    CACHE BOOL "" FORCE
)
set(FMT_TEST
    OFF
    CACHE BOOL "" FORCE
)
set(FMT_DOC
    OFF
    CACHE BOOL "" FORCE
)

fetchcontent_makeavailable(fmt)

# ==============================================================================
# Add Subdirectory
# ==============================================================================

enable_testing()
add_subdirectory(src)