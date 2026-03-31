#include "runner.hpp"

namespace gl_bench::is_bipartite {
extern suite get_suite();
} // namespace gl_bench::is_bipartite

int main(int argc, char** argv) {
    gl_bench::runner runner(argc, argv);

    // Register suites
    runner.add_suite(gl_bench::is_bipartite::get_suite());

    // run benchmarks
    return runner.run();
}
