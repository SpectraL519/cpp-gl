#include "runner.hpp"

namespace gl_bench::is_bipartite {
extern suite get_suite();
} // namespace gl_bench::is_bipartite

int main(int argc, char** argv) {
    return gl_bench::runner::get().run(argc, argv);
}
