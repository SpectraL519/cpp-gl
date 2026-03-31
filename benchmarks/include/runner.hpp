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
    runner(int argc, char** argv);

    void add_suite(suite suite);
    int run();

private:
    int _argc;
    char** _argv;
    argon::argument_parser _parser;
    std::vector<suite> _suites;
};

} // namespace gl_bench
