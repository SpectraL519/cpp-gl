#pragma once

#include <argon/argument_parser.hpp>

namespace gl_bench {

struct suite {
    void (*add_args)(argon::argument_parser& parser);
    void (*register_benchmarks)(const argon::argument_parser& parser);
};

} // namespace gl_bench
