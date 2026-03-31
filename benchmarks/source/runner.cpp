#include "runner.hpp"

#include <format>
#include <iostream>

namespace gl_bench {

runner::runner() : _parser("gl_benchmarks") {
    auto& glob_args = this->_parser.add_group("Global Benchmark Options");
    this->_parser.add_optional_argument<fs::path>(glob_args, "output", "o")
        .help("Path to the output JSON file")
        .action<argon::action_type::observe>([](const fs::path& path) {
            if (not fs::is_regular_file(path) or path.extension() != ".json")
                throw std::runtime_error(
                    std::format("Invlid output file path (must be a .json file, got: {})", path)
                );
        })
}

void runner::add_suite(suite suite) {
    this->_suites.push_back(suite);
    if (suite.add_args)
        suite.add_args(this->_parser);
}

int runner::run(int argc, char** argv) {
    std::vector<std::string> gbench_args = this->_parser.try_parse_known_args(argc, argv);

    // Register benchmark suites
    for (const auto& suite : this->_suites)
        if (suite.register_benchmarks)
            suite.register_benchmarks(this->_parser);

    // Handle JSON export if requested
    if (this->_parser.has_value("output")) {
        fs::path out_path = this->_parser.value<fs::path>("output");
        gbench_args.push_back("--benchmark_out=" + out_path.string());
        gbench_args.push_back("--benchmark_out_format=json");
    }

    // Reconstruct argv for Google Benchmark
    std::vector<char*> gbench_argv;
    gbench_argv.push_back(this->_argv[0]);
    for (auto& arg : gbench_args)
        gbench_argv.push_back(arg.data());

    int gbench_argc = static_cast<int>(gbench_argv.size());

    benchmark::Initialize(&gbench_argc, gbench_argv.data());
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    return 0;
}

} // namespace gl_bench
