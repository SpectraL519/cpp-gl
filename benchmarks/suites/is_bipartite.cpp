#include "runner.hpp"
#include "suite.hpp"

#include <gl/algorithm.hpp>
#include <gl/graph.hpp>
#include <gl/topologies.hpp>

#include <benchmark/benchmark.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/bipartite.hpp>

namespace gl_bench::is_bipartite {

// --- Graph Generation Utilities ---

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

// --- Benchmark Implementations ---

template <gl::traits::c_undirected_graph Graph>
void bm_gl_is_bipartite(benchmark::State& state) {
    const auto n_vertices = static_cast<gl::size_type>(state.range(0));
    auto graph = gl::topology::biclique<Graph>(n_vertices, n_vertices);

    for (auto _ : state) {
        bool is_bip = gl::algorithm::is_bipartite(graph);
        benchmark::DoNotOptimize(is_bip);
    }

    state.counters["Vertices"] = graph.order();
    state.counters["Edges"] = graph.size();
}

template <typename GraphType>
void bm_bgl_is_bipartite(benchmark::State& state) {
    const auto n_vertices = static_cast<std::size_t>(state.range(0));
    auto graph = gen_bgl_biclique<GraphType>(n_vertices, n_vertices);

    for (auto _ : state) {
        bool is_bip = boost::is_bipartite(graph);
        benchmark::DoNotOptimize(is_bip);
    }

    state.counters["Vertices"] = boost::num_vertices(graph);
    state.counters["Edges"] = boost::num_edges(graph);
}

// --- Suite Registration Interface ---

// Phase 1: Declare arguments specific to this benchmark
void add_args(argon::argument_parser& parser) {
    auto& group = parser.add_group("Is-Bipartite Benchmark Options");
    parser.add_optional_argument<std::size_t>(group, "bip-v")
        .default_values(1000uz)
        .help("Number of vertices for a single set in bipartite generation");
}

// Phase 2: Read arguments and register
void register_benchmarks(const argon::argument_parser& parser) {
    using gl_list_u32 = gl::graph<gl::list_graph_traits<
        gl::undirected_t,
        gl::empty_properties,
        gl::empty_properties,
        std::uint32_t>>;
    using gl_flat_list_u32 = gl::graph<gl::flat_list_graph_traits<
        gl::undirected_t,
        gl::empty_properties,
        gl::empty_properties,
        std::uint32_t>>;
    using bgl_list = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;

    const auto v = static_cast<int64_t>(parser.value<std::size_t>("bip-v"));

    // Register BGL
    benchmark::RegisterBenchmark("BGL/List/is_bipartite", bm_bgl_is_bipartite<bgl_list>)
        ->Arg(v)
        ->Unit(benchmark::kMillisecond);

    // Register CPP-GL U32
    benchmark::RegisterBenchmark("CPP-GL/List_U32/is_bipartite", bm_gl_is_bipartite<gl_list_u32>)
        ->Arg(v)
        ->Unit(benchmark::kMillisecond);

    benchmark::
        RegisterBenchmark("CPP-GL/FlatList_U32/is_bipartite", bm_gl_is_bipartite<gl_flat_list_u32>)
            ->Arg(v)
            ->Unit(benchmark::kMillisecond);
}

suite get_suite() {
    return suite{add_args, register_benchmarks};
}

} // namespace gl_bench::is_bipartite
