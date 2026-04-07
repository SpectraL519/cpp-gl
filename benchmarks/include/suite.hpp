#pragma once

#include <argon/argument_parser.hpp>

#include <functional>

namespace gl_bench {

struct suite {
    std::function<void(argon::argument_parser&)> add_args = nullptr;
    std::function<void(const argon::argument_parser&)> register_benchmarks =
        [](const argon::argument_parser&) {};
};

} // namespace gl_bench
