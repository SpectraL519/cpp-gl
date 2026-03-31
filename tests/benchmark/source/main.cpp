#include "runner.hpp"

// Forward declaration of our registration function
namespace gl_bench {
extern void register_is_bipartite(const benchmark_config& config);
}

int main(int argc, char** argv) {
    gl_bench::benchmark_runner runner(argc, argv);

    // Register all benchmark suites
    runner.add_benchmarks(gl_bench::register_is_bipartite);
    // runner.add_benchmarks(gl_bench::register_shortest_path); // Easy extension later!

    return runner.run();
}
