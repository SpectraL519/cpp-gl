#include "runner.hpp"
#include "suite.hpp"

#include <gl/algorithms.hpp>
#include <gl/graph.hpp>
#include <gl/topologies.hpp>

#include <benchmark/benchmark.h>

namespace gl_bench::is_bipartite {

// CPP-GL Benchmark

template <gl::type_traits::c_undirected_graph Graph>
void bm_gl_is_bipartite(benchmark::State& state) {
    const auto n_vertices = static_cast<gl::types::size_type>(state.range(0));
    auto graph = gl::topology::biclique<Graph>(n_vertices, n_vertices);

    for (auto _ : state) {
        bool is_bip = gl::algorithm::is_bipartite(graph);
        benchmark::DoNotOptimize(is_bip);
    }

    state.counters["Vertices"] = static_cast<double>(graph.n_vertices());
    state.counters["Edges"] = static_cast<double>(graph.n_unique_edges());
}

void add_args(argon::argument_parser& parser) {
    auto& group = parser.add_group("Is-Bipartite Benchmark Options");
    parser.add_optional_argument<std::size_t>(group, "bip-v")
        .default_values(static_cast<std::size_t>(1000))
        .help("Number of vertices for a single set in bipartite generation");
}

void register_benchmarks(const argon::argument_parser& parser) {
    const auto n_vertices = static_cast<int64_t>(parser.value<std::size_t>("bip-v"));

    // CPP-GL Benchmarks
    using gl_list = gl::graph<gl::list_graph_traits<gl::undirected_t>>;
    using gl_matrix = gl::graph<gl::matrix_graph_traits<gl::undirected_t>>;

    benchmark::RegisterBenchmark("is_bipartite/CPP-GL/list", bm_gl_is_bipartite<gl_list>)
        ->Arg(n_vertices)
        ->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("is_bipartite/CPP-GL/matrix", bm_gl_is_bipartite<gl_matrix>)
        ->Arg(n_vertices)
        ->Unit(benchmark::kMillisecond);
}

suite get_suite() {
    return suite{add_args, register_benchmarks};
}

} // namespace gl_bench::is_bipartite
