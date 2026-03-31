#pragma once

#include <argon/argument_parser.hpp>
#include <benchmark/benchmark.h>

#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace gl_bench {

struct benchmark_config {
    std::size_t n_vertices = 1000uz;
    std::optional<fs::path> output_file;
    bool export_json = false;
};

class benchmark_runner {
public:
    using registration_func = std::function<void(const benchmark_config&)>;

    benchmark_runner(int argc, char** argv);

    /// @brief Registers a new suite of benchmarks dynamically.
    void add_benchmarks(registration_func func);

    /// @brief Executes the benchmark suite.
    int run();

private:
    benchmark_config _config;
    std::vector<std::string> _unparsed_args;
    std::vector<registration_func> _registration_funcs;
    std::string _program_name;
};

} // namespace gl_bench
