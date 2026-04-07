#include "runner.hpp"

int main(int argc, char** argv) {
    return gl_bench::runner::get().run(argc, argv);
}
