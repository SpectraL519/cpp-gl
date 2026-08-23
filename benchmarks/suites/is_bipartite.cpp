#include "runner.hpp"
#include "suite.hpp"

#include <gl/algorithm.hpp>
#include <gl/graph.hpp>
#include <gl/topology.hpp>

#include <benchmark/benchmark.h>

#ifdef GL_BENCH_INCLUDE_BGL
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/bipartite.hpp>
#endif

namespace gl_bench::is_bipartite {

// CPP-GL Benchmark

template <gl::traits::c_undirected_graph Graph>
void bm_gl_is_bipartite(benchmark::State& state) {
    const auto n_vertices = static_cast<gl::size_type>(state.range(0));
    auto graph = gl::topology::biclique<Graph>(n_vertices, n_vertices);

    for (auto _ : state) {
        bool is_bip = gl::algorithm::is_bipartite(graph);
        benchmark::DoNotOptimize(is_bip);
    }

    state.counters["Vertices"] = static_cast<double>(graph.n_vertices());
    state.counters["Edges"] = static_cast<double>(graph.n_edges());
}

#ifdef GL_BENCH_INCLUDE_BGL

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

#endif

void add_args(argon::argument_parser& parser) {
    auto& group = parser.add_group("Is-Bipartite Benchmark Suite Options (is-bipartite)");
    parser.add_optional_argument<std::size_t>(group, "bip-v")
        .default_values(1000uz)
        .help("Number of vertices for a single set in bipartite generation");
}

template <typename StrictGraph>
void register_is_bipartite_benchmark(const std::string& base_name, int64_t n_vertices) {
    using RelaxedGraph = gl::traits::make_relaxed_t<StrictGraph>;

    benchmark::RegisterBenchmark(base_name + "/strict", bm_gl_is_bipartite<StrictGraph>)
        ->Arg(n_vertices)
        ->Unit(benchmark::kMillisecond);

    benchmark::RegisterBenchmark(base_name + "/relaxed", bm_gl_is_bipartite<RelaxedGraph>)
        ->Arg(n_vertices)
        ->Unit(benchmark::kMillisecond);
}

void register_benchmarks(const argon::argument_parser& parser) {
    const auto n_vertices = static_cast<int64_t>(parser.value<std::size_t>("bip-v"));

    // CPP-GL Adjacency List Benchmarks
    using gl_list_u32 = gl::list_graph<
        gl::undirected_t,
        gl::empty_properties,
        gl::empty_properties,
        gl::api::strict_t,
        std::uint32_t>;
    using gl_list_u64 = gl::list_graph<
        gl::undirected_t,
        gl::empty_properties,
        gl::empty_properties,
        gl::api::strict_t,
        std::uint64_t>;

    benchmark::RegisterBenchmark("is_bipartite/CPP-GL/list/u32", bm_gl_is_bipartite<gl_list_u32>)
        ->Arg(n_vertices)
        ->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("is_bipartite/CPP-GL/list/u64", bm_gl_is_bipartite<gl_list_u64>)
        ->Arg(n_vertices)
        ->Unit(benchmark::kMillisecond);

    // CPP-GL Flat Adjacency List Benchmarks
    using gl_flat_list_u32 = gl::flat_list_graph<
        gl::undirected_t,
        gl::empty_properties,
        gl::empty_properties,
        gl::api::strict_t,
        std::uint32_t>;
    using gl_flat_list_u64 = gl::flat_list_graph<
        gl::undirected_t,
        gl::empty_properties,
        gl::empty_properties,
        gl::api::strict_t,
        std::uint64_t>;

    benchmark::
        RegisterBenchmark("is_bipartite/CPP-GL/flat_list/u32", bm_gl_is_bipartite<gl_flat_list_u32>)
            ->Arg(n_vertices)
            ->Unit(benchmark::kMillisecond);
    benchmark::
        RegisterBenchmark("is_bipartite/CPP-GL/flat_list/u64", bm_gl_is_bipartite<gl_flat_list_u64>)
            ->Arg(n_vertices)
            ->Unit(benchmark::kMillisecond);

    // CPP-GL Adjacency Matrix Benchmarks
    using gl_matrix_u32 = gl::matrix_graph<
        gl::undirected_t,
        gl::empty_properties,
        gl::empty_properties,
        gl::api::strict_t,
        std::uint32_t>;
    using gl_matrix_u64 = gl::matrix_graph<
        gl::undirected_t,
        gl::empty_properties,
        gl::empty_properties,
        gl::api::strict_t,
        std::uint64_t>;

    benchmark::RegisterBenchmark("is_bipartite/CPP-GL/matrix/u32", bm_gl_is_bipartite<gl_matrix_u32>)
        ->Arg(n_vertices)
        ->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("is_bipartite/CPP-GL/matrix/u64", bm_gl_is_bipartite<gl_matrix_u64>)
        ->Arg(n_vertices)
        ->Unit(benchmark::kMillisecond);

    // CPP-GL Flat Adjacency Matrix Benchmarks
    using gl_flat_matrix_u32 = gl::flat_matrix_graph<
        gl::undirected_t,
        gl::empty_properties,
        gl::empty_properties,
        gl::api::strict_t,
        std::uint32_t>;
    using gl_flat_matrix_u64 = gl::flat_matrix_graph<
        gl::undirected_t,
        gl::empty_properties,
        gl::empty_properties,
        gl::api::strict_t,
        std::uint64_t>;

    benchmark::
        RegisterBenchmark("is_bipartite/CPP-GL/flat_matrix/u32", bm_gl_is_bipartite<gl_flat_matrix_u32>)
            ->Arg(n_vertices)
            ->Unit(benchmark::kMillisecond);
    benchmark::
        RegisterBenchmark("is_bipartite/CPP-GL/flat_matrix/u64", bm_gl_is_bipartite<gl_flat_matrix_u64>)
            ->Arg(n_vertices)
            ->Unit(benchmark::kMillisecond);


#ifdef GL_BENCH_INCLUDE_BGL
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
#endif
}

namespace {

// Register the suite
bool _registered = []() {
    gl_bench::runner::get().add_suite(
        "is-bipartite", suite{.add_args = add_args, .register_benchmarks = register_benchmarks}
    );
    return true;
}();

} // namespace

} // namespace gl_bench::is_bipartite
