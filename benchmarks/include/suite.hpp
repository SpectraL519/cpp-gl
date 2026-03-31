#pragma once

#include <argon/argument_parser.hpp>

namespace gl_bench {

struct suite {
    // Phase 1: Add custom CLI arguments to the parser
    void (*add_args)(argon::argument_parser& parser);

    // Phase 2: Read the parsed arguments and register with Google Benchmark
    void (*register_benchmarks)(const argon::argument_parser& parser);
};

} // namespace gl_bench
