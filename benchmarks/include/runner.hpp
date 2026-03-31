#pragma once

#include "suite.hpp"

#include <argon/argument_parser.hpp>
#include <benchmark/benchmark.h>

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace gl_bench {

class runner {
public:
    runner();

    void add_suite(suite suite);
    int run(int argc, char** argv);

private:
    argon::argument_parser _parser;
    std::vector<suite> _suites;
};

} // namespace gl_bench
