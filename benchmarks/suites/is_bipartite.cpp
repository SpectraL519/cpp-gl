#include "runner.hpp"
#include "suite.hpp"

#include <gl/algorithms.hpp>
#include <gl/graph.hpp>
#include <gl/topologies.hpp>

#include <benchmark/benchmark.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/bipartite.hpp>

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

// BGL Benchmark

template <typename GraphType>
GraphType gen_bgl_biclique(const std::size_t n_vertices_a, const std::size_t n_vertices_b) {
    using directed_category = typename boost::graph_traits<GraphType>::directed_category;
    const auto n_vertices = n_vertices_a + n_vertices_b;
    GraphType graph{n_vertices};

    for (std::size_t source_id = 0ull; source_id < n_vertices_a; ++source_id) {
        for (std::size_t target_id = n_vertices_a; target_id < n_vertices; ++target_id) {
            boost::add_edge(source_id, target_id, graph);
            if constexpr (std::is_same_v<directed_category, boost::directed_tag>)
                boost::add_edge(target_id, source_id, graph);
        }
    }
    return graph;
}

template <typename GraphType>
void bm_bgl_is_bipartite(benchmark::State& state) {
    const auto n_vertices = static_cast<std::size_t>(state.range(0));
    auto graph = gen_bgl_biclique<GraphType>(n_vertices, n_vertices);

    for (auto _ : state) {
        bool is_bip = boost::is_bipartite(graph);
        benchmark::DoNotOptimize(is_bip);
    }

    state.counters["Vertices"] = static_cast<double>(boost::num_vertices(graph));
    state.counters["Edges"] = static_cast<double>(boost::num_edges(graph));
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

    // BGL Benchmarks
    using bgl_list = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
    using bgl_matrix =
        boost::adjacency_matrix<boost::directedS, boost::no_property, boost::no_property>;

    benchmark::RegisterBenchmark("is_bipartite/BGL/list", bm_bgl_is_bipartite<bgl_list>)
        ->Arg(n_vertices)
        ->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("is_bipartite/BGL/matrix", bm_bgl_is_bipartite<bgl_matrix>)
        ->Arg(n_vertices)
        ->Unit(benchmark::kMillisecond);
}

suite get_suite() {
    return suite{add_args, register_benchmarks};
}

} // namespace gl_bench::is_bipartite
