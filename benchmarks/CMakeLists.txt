cmake_minimum_required(VERSION 3.14)
project(gl_benchmarks LANGUAGES CXX)

include(FetchContent)

# Build options
set(BENCH_GL_DEFS "" CACHE STRING "Additional compile definitions for the CPP-GL target")
set(BENCH_OPT_MODE "3" CACHE STRING "The optimisation mode used to build the project benchmarks (default: 3)")

message(STATUS "Build options:")
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

add_executable(gl_benchmarks
    source/main.cpp
    source/runner.cpp
    suites/is_bipartite.cpp
)

target_compile_options(gl_benchmarks PRIVATE
    -Wall
    -Wextra
    -Wcast-align
    -Wconversion
    -Wunreachable-code
    -Wuninitialized
    -Wunused
    -pedantic
    -O${BENCH_OPT_MODE}
)

target_include_directories(gl_benchmarks PRIVATE include)
target_link_libraries(gl_benchmarks PRIVATE
    benchmark::benchmark
    cpp-argon
    cpp-gl
)

if(BENCH_GL_DEFS)
    separate_arguments(BENCH_GL_DEFS_LIST NATIVE_COMMAND "${BENCH_GL_DEFS}")
    target_compile_definitions(gl_benchmarks PRIVATE ${BENCH_GL_DEFS_LIST})
endif()
