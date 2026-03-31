cmake_minimum_required(VERSION 3.14)
project(gl_benchmarks LANGUAGES CXX)

include(FetchContent)

# Build options
set(BENCH_GL_TAG "master" CACHE STRING "The branch or tag name used to fetch the CPP-GL library for benchmarks")
set(BENCH_GL_DEFS "" CACHE STRING "Additional compile definitions for the CPP-GL target")
set(BENCH_OPT_MODE "0" CACHE STRING "The optimisation mode used to build the project benchmarks (-O${BENCH_OPT_MODE})") # TODO: handle

message(STATUS "Build options:")
message(STATUS "[BENCH_GL_TAG = ${BENCH_GL_TAG}]")
message(STATUS "[BENCH_GL_DEFS = ${BENCH_GL_DEFS}]")
message(STATUS "[BENCH_OPT_MODE = ${BENCH_OPT_MODE}]")

# Fetch Google Benchmark
FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.3
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark tests" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

# Fetch CPP-ARGON
FetchContent_Declare(
    cpp-argon
    GIT_REPOSITORY https://github.com/SpectraL519/cpp-argon.git
    GIT_TAG v4.0.0
)
FetchContent_MakeAvailable(cpp-argon)

# Fetch CPP-GL
FetchContent_Declare(
    cpp-gl
    GIT_REPOSITORY https://github.com/SpectraL519/cpp-gl.git
    GIT_TAG ${BENCH_GL_TAG}
)
FetchContent_MakeAvailable(cpp-gl)

# Find Boost for BGL comparisons
find_package(Boost REQUIRED)

add_executable(gl_benchmarks
    source/main.cpp
    source/runner.cpp
    suites/is_bipartite.cpp
)

target_include_directories(gl_benchmarks PRIVATE include)
target_link_libraries(gl_benchmarks PRIVATE
    benchmark::benchmark
    cpp-argon
    cpp-gl
    Boost::headers
)
