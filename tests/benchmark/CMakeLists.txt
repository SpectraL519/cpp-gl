cmake_minimum_required(VERSION 3.14)
project(gl_benchmarks LANGUAGES CXX)

include(FetchContent)

# Fetch Google Benchmark
FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.3
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark tests" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

# Add your existing cpp-gl, cpp-argon, and Boost fetches here...
# ...

add_executable(gl_benchmarks
    src/main.cpp
    src/benchmark_runner.cpp
    benchmarks/is_bipartite.cpp
)

target_include_directories(gl_benchmarks PRIVATE include)
target_link_libraries(gl_benchmarks PRIVATE
    benchmark::benchmark
    cpp-argon
    cpp-gl
    Boost::graph
)
