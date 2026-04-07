#pragma once

#include "suite.hpp"

#include <argon/argument_parser.hpp>
#include <benchmark/benchmark.h>

#include <map>

namespace gl_bench {

class runner {
public:
    static runner& get() {
        static runner instance;
        return instance;
    }

    runner(const runner&) = delete;
    runner& operator=(const runner&) = delete;

    void add_suite(const std::string& name, suite suite);
    int run(int argc, char** argv);

private:
    runner();

    argon::argument_parser _parser;
    argon::argument_group* _glob_args = nullptr;
    std::map<std::string, suite> _suites;
};

} // namespace gl_bench
