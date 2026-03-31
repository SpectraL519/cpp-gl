#include "runner.hpp"

#include <iostream>

namespace gl_bench {

benchmark_runner::benchmark_runner(int argc, char** argv) : _program_name(argv[0]) {
    argon::argument_parser parser("gl_benchmarks");

    parser.add_optional_argument<std::size_t>("vertices", "v")
        .default_values(1000uz)
        .help("Number of vertices for a single set in bipartite generation");

    parser.add_optional_argument("output", "o")
        .help("Path to output JSON file (used for future plotting)");

    try {
        // 1. Parse our arguments, collect the rest
        this->_unparsed_args = parser.try_parse_known_args(argc, argv);
        this->_config.n_vertices = parser.value<std::size_t>("vertices");

        if (parser.has_value("output")) {
            this->_config.output_file = fs::path(parser.value("output"));
            this->_config.export_json = true;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[error] " << e.what() << '\n';
    }
}

void benchmark_runner::add_benchmarks(registration_func func) {
    this->_registration_funcs.push_back(std::move(func));
}

int benchmark_runner::run() {
    // 1. Run dynamic registrations for all added suites (e.g., is_bipartite)
    for (const auto& func : this->_registration_funcs)
        func(this->_config);

    // 2. Programmatically inject Google Benchmark JSON flags if -o was used
    if (this->_config.export_json && this->_config.output_file.has_value()) {
        this->_unparsed_args.push_back("--benchmark_out=" + this->_config.output_file->string());
        this->_unparsed_args.push_back("--benchmark_out_format=json");
    }

    // 3. Reconstruct argc/argv purely for Google Benchmark
    std::vector<char*> gbench_argv;
    gbench_argv.push_back(this->_program_name.data());
    for (auto& arg : this->_unparsed_args)
        gbench_argv.push_back(arg.data());

    int gbench_argc = static_cast<int>(gbench_argv.size());

    // 4. Initialize and Run
    benchmark::Initialize(&gbench_argc, gbench_argv.data());
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    return 0;
}

} // namespace gl_bench
