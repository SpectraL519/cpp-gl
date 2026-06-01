#include "runner.hpp"
#include "suite.hpp"

#include <gl/algorithm.hpp>
#include <gl/graph.hpp>
#include <hgl/algorithm.hpp>
#include <hgl/conversion.hpp>
#include <hgl/hypergraph.hpp>

#include <benchmark/benchmark.h>

namespace gl_bench::hg_bfs {

// --- Hypergraph Topology Generator ---

template <hgl::traits::c_undirected_hypergraph HypergraphType>
HypergraphType gen_sliding_window_hypergraph(
    const std::size_t n_hyperedges, const std::size_t degree, const std::size_t stride
) {
    using id_type = typename HypergraphType::id_type;

    // V = (E - 1) * stride + degree
    const auto n_vertices = static_cast<id_type>((n_hyperedges - 1) * stride + degree);
    HypergraphType hgraph{n_vertices, static_cast<id_type>(n_hyperedges)};

    for (id_type e = 0; e < static_cast<id_type>(n_hyperedges); ++e) {
        const auto start_v = static_cast<id_type>(e * stride);
        const auto end_v = start_v + static_cast<id_type>(degree);
        hgraph.bind(std::views::iota(start_v, end_v), e);
    }

    return hgraph;
}

// --- HGL Benchmark ---

template <hgl::traits::c_undirected_hypergraph Hypergraph>
void bm_hgl_bfs(benchmark::State& state) {
    const auto n_hedges = static_cast<std::size_t>(state.range(0));
    const auto he_size = static_cast<std::size_t>(state.range(1));
    const auto stride = static_cast<std::size_t>(state.range(2));

    auto hg = gen_sliding_window_hypergraph<Hypergraph>(n_hedges, he_size, stride);

    for (auto _ : state) {
        auto search_tree = hgl::algorithm::breadth_first_search(hg, 0uz);
        benchmark::DoNotOptimize(search_tree);
    }

    state.counters["Vertices"] = static_cast<double>(hg.n_vertices());
    state.counters["Hyperedges"] = static_cast<double>(hg.n_hyperedges());
    state.counters["Incidences"] = static_cast<double>(n_hedges * he_size);
}

// --- GL Incidence Graph Benchmark ---

template <
    hgl::traits::c_undirected_hypergraph Hypergraph,
    gl::traits::c_undirected_graph IncidenceGraph>
void bm_gl_incidence_bfs(benchmark::State& state) {
    const auto n_hedges = static_cast<std::size_t>(state.range(0));
    const auto he_size = static_cast<std::size_t>(state.range(1));
    const auto stride = static_cast<std::size_t>(state.range(2));

    if constexpr (hgl::traits::c_incidence_matrix_hypergraph<Hypergraph>) {
        const auto n_vertices = (n_hedges - 1) * stride + he_size;
        const auto ig_vertices = n_vertices + n_hedges;

        // Hardcoded safety limit for ~16 GB of RAM (V_ig = 65,000)
        if (ig_vertices > 65000) {
            state.SkipWithError("Matrix requires > 16.0 GB of memory; skipping.");
            return;
        }
    }

    auto hg = gen_sliding_window_hypergraph<Hypergraph>(n_hedges, he_size, stride);
    auto ig = hgl::incidence_graph<IncidenceGraph>(hg);

    for (auto _ : state) {
        auto pred_map = gl::algorithm::breadth_first_search(ig, 0uz);
        benchmark::DoNotOptimize(pred_map);
    }

    state.counters["IG_Vertices"] = static_cast<double>(ig.n_vertices());
    state.counters["IG_Edges"] = static_cast<double>(ig.n_edges());
}

void add_args(argon::argument_parser& parser) {
    auto& group = parser.add_group("Hypergraph BFS Benchmark Suite Options (hg-bfs)");
    parser.add_optional_argument<std::size_t>(group, "hg-bfs-e")
        .default_values(1000uz)
        .help("Number of hyperedges");
    parser.add_optional_argument<std::size_t>(group, "hg-bfs-esize")
        .default_values(100uz)
        .help("Hyperedge size");
    parser.add_optional_argument<std::size_t>(group, "hg-bfs-stride")
        .default_values(2uz)
        .help("Vertex shift between consecutive hyperedges (smaller = denser)");
}

void register_benchmarks(const argon::argument_parser& parser) {
    const auto n_hedges = static_cast<int64_t>(parser.value<std::size_t>("hg-bfs-e"));
    const auto he_size = static_cast<int64_t>(parser.value<std::size_t>("hg-bfs-esize"));
    const auto stride = static_cast<int64_t>(parser.value<std::size_t>("hg-bfs-stride"));

    using gl_list = gl::list_graph<gl::undirected_t>;
    using gl_flat_list = gl::flat_list_graph<gl::undirected_t>;
    using gl_matrix = gl::matrix_graph<gl::undirected_t>;
    using gl_flat_matrix = gl::flat_matrix_graph<gl::undirected_t>;

    using hgl_list = hgl::list_hypergraph<hgl::repr::bidirectional_t, hgl::undirected_t>;
    using hgl_flat_list = hgl::flat_list_hypergraph<hgl::repr::bidirectional_t, hgl::undirected_t>;

    using hgl_v_matrix = hgl::matrix_hypergraph<hgl::repr::vertex_major_t, hgl::undirected_t>;
    using hgl_e_matrix = hgl::matrix_hypergraph<hgl::repr::hyperedge_major_t, hgl::undirected_t>;

    using hgl_v_flat_matrix =
        hgl::flat_matrix_hypergraph<hgl::repr::vertex_major_t, hgl::undirected_t>;
    using hgl_e_flat_matrix =
        hgl::flat_matrix_hypergraph<hgl::repr::hyperedge_major_t, hgl::undirected_t>;

    benchmark::RegisterBenchmark("bfs/HGL/list", bm_hgl_bfs<hgl_list>)
        ->Args({n_hedges, he_size, stride})
        ->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("bfs/HGL/flat_list", bm_hgl_bfs<hgl_flat_list>)
        ->Args({n_hedges, he_size, stride})
        ->Unit(benchmark::kMillisecond);

    benchmark::RegisterBenchmark("bfs/HGL/matrix/v_major", bm_hgl_bfs<hgl_v_matrix>)
        ->Args({n_hedges, he_size, stride})
        ->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("bfs/HGL/matrix/e_major", bm_hgl_bfs<hgl_e_matrix>)
        ->Args({n_hedges, he_size, stride})
        ->Unit(benchmark::kMillisecond);

    benchmark::RegisterBenchmark("bfs/HGL/flat_matrix/v_major", bm_hgl_bfs<hgl_v_flat_matrix>)
        ->Args({n_hedges, he_size, stride})
        ->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("bfs/HGL/flat_matrix/e_major", bm_hgl_bfs<hgl_e_flat_matrix>)
        ->Args({n_hedges, he_size, stride})
        ->Unit(benchmark::kMillisecond);

    benchmark::RegisterBenchmark("bfs/INCIDENCE/list", bm_gl_incidence_bfs<hgl_list, gl_list>)
        ->Args({n_hedges, he_size, stride})
        ->Unit(benchmark::kMillisecond);
    benchmark::
        RegisterBenchmark("bfs/INCIDENCE/flat_list", bm_gl_incidence_bfs<hgl_list, gl_flat_list>)
            ->Args({n_hedges, he_size, stride})
            ->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("bfs/INCIDENCE/matrix", bm_gl_incidence_bfs<hgl_list, gl_matrix>)
        ->Args({n_hedges, he_size, stride})
        ->Unit(benchmark::kMillisecond);
    benchmark::
        RegisterBenchmark("bfs/INCIDENCE/flat_matrix", bm_gl_incidence_bfs<hgl_list, gl_flat_matrix>)
            ->Args({n_hedges, he_size, stride})
            ->Unit(benchmark::kMillisecond);
}

namespace {
bool _registered = []() {
    gl_bench::runner::get().add_suite(
        "hg-bfs", suite{.add_args = add_args, .register_benchmarks = register_benchmarks}
    );
    return true;
}();
} // namespace

} // namespace gl_bench::hg_bfs
