#include "gl/algorithm/core.hpp"
#include "hgl/directional_tags.hpp"
#include "runner.hpp"
#include "suite.hpp"

#include <gl/algorithm.hpp>
#include <gl/graph.hpp>
#include <hgl/algorithm.hpp>
#include <hgl/conversion.hpp>
#include <hgl/hypergraph.hpp>

#include <benchmark/benchmark.h>
#include <sys/types.h>

namespace gl_bench::hg_b_bfs {

// --- Hypergraph Topology Generator ---

template <hgl::traits::c_bf_directed_hypergraph HypergraphType>
HypergraphType gen_bf_overlapping_chain_hypergraph(
    const std::size_t n_hyperedges, const std::size_t layer_width, const std::size_t stride
) {
    using id_type = typename HypergraphType::id_type;

    // V = maximum index reached by the last hyperedge's head.
    const auto n_vertices = static_cast<id_type>((n_hyperedges - 1) * stride + 2 * layer_width);

    HypergraphType hgraph{n_vertices, static_cast<id_type>(n_hyperedges)};

    for (id_type e = 0; e < static_cast<id_type>(n_hyperedges); ++e) {
        const auto tail_start = static_cast<id_type>(e * stride);
        const auto tail_end = tail_start + static_cast<id_type>(layer_width);

        const auto head_start = tail_end;
        const auto head_end = head_start + static_cast<id_type>(layer_width);

        hgraph.bind_tail(std::views::iota(tail_start, tail_end), e);
        hgraph.bind_head(std::views::iota(head_start, head_end), e);
    }

    return hgraph;
}

template <hgl::traits::c_bf_directed_hypergraph HypergraphType>
requires hgl::traits::c_flat_list_hypergraph<HypergraphType>
HypergraphType gen_bf_overlapping_chain_hypergraph(
    const std::size_t n_hyperedges, const std::size_t layer_width, const std::size_t stride
) {
    using traits_type = typename HypergraphType::traits_type;
    using layout_tag = typename traits_type::layout_tag;

    using list_repr_tag = hgl::repr::list_t<layout_tag>;
    using list_hypergraph = hgl::traits::swap_repr_tag_t<HypergraphType, list_repr_tag>;

    return hgl::to<typename traits_type::representation_tag>(
        gen_bf_overlapping_chain_hypergraph<list_hypergraph>(n_hyperedges, layer_width, stride)
    );
}

// --- Native HGL Backward BFS Benchmark ---

template <hgl::traits::c_bf_directed_hypergraph Hypergraph>
void bm_hgl_backward_bfs(benchmark::State& state) {
    using id_type = typename Hypergraph::id_type;

    const auto n_hedges = static_cast<std::size_t>(state.range(0));
    const auto layer_width = static_cast<std::size_t>(state.range(1));
    const auto stride = static_cast<std::size_t>(state.range(2));

    auto hg = gen_bf_overlapping_chain_hypergraph<Hypergraph>(n_hedges, layer_width, stride);

    auto roots = std::views::iota(id_type{0}, static_cast<id_type>(layer_width))
               | std::ranges::to<std::vector<id_type>>();

    for (auto _ : state) {
        auto search_tree = hgl::algorithm::backward_bfs(hg, roots);
        benchmark::DoNotOptimize(search_tree);
    }

    state.counters["Vertices"] = static_cast<double>(hg.n_vertices());
    state.counters["Hyperedges"] = static_cast<double>(hg.n_hyperedges());
    state.counters["Incidences"] = static_cast<double>(n_hedges * layer_width * 2);
}

// --- Incidence Graph Utilities ---

/// @brief Executes a Backward BFS equivalent on an Incidence Graph.
template <gl::traits::c_directed_graph IncidenceGraph>
bool incidence_backward_bfs(
    IncidenceGraph&& ig,
    const std::vector<gl::id_t<IncidenceGraph>>& roots,
    const gl::size_type original_n_vertices
) {
    using id_type = gl::id_t<IncidenceGraph>;

    std::vector<bool> visited_v(original_n_vertices, false);
    auto tail_unvisited =
        ig.in_degree_map() | std::views::drop(original_n_vertices)
        | std::ranges::to<std::vector<gl::size_type>>();

    auto root_nodes =
        roots | std::views::transform([](const id_type root_id) {
            return gl::algorithm::search_node<gl::val_t<IncidenceGraph>>{root_id};
        })
        | std::ranges::to<std::vector>();

    auto visit_vertex_pred = [&](id_type v) {
        if (v < original_n_vertices)
            return not visited_v[gl::to_idx(v)];
        return true;
    };

    auto visit = [&](id_type v, id_type /*p*/) {
        if (v < original_n_vertices)
            visited_v[gl::to_idx(v)] = true;
        return true;
    };

    auto enqueue_node_pred =
        [&](id_type target_id, const auto& /*edge*/) -> gl::algorithm::decision {
        if (target_id >= original_n_vertices) {
            const auto he_idx = target_id - original_n_vertices;
            return --tail_unvisited[gl::to_idx(he_idx)] == 0uz;
        }
        else {
            return not visited_v[gl::to_idx(target_id)];
        }
    };

    return gl::algorithm::bfs(ig, root_nodes, visit_vertex_pred, visit, enqueue_node_pred);
}

// --- GL Incidence Graph Backward BFS Benchmark ---

template <
    hgl::traits::c_bf_directed_hypergraph Hypergraph,
    gl::traits::c_directed_graph IncidenceGraph>
void bm_gl_incidence_backward_bfs(benchmark::State& state) {
    using id_type = typename IncidenceGraph::id_type;

    const auto n_hedges = static_cast<std::size_t>(state.range(0));
    const auto layer_width = static_cast<std::size_t>(state.range(1));
    const auto stride = static_cast<std::size_t>(state.range(2));

    if constexpr (gl::traits::c_adjacency_matrix_graph<IncidenceGraph>) {
        const auto n_vertices = (n_hedges * stride) + layer_width;
        const auto ig_vertices = n_vertices + n_hedges;

        // Hardcoded safety limit for ~16 GB of RAM (V_ig = 65,000)
        if (ig_vertices > 65000) {
            state.SkipWithError("Matrix requires > 16.0 GB of memory; skipping.");
            return;
        }
    }

    auto hg = gen_bf_overlapping_chain_hypergraph<Hypergraph>(n_hedges, layer_width, stride);
    auto ig = hgl::incidence_graph<IncidenceGraph>(hg);

    auto roots = std::views::iota(id_type{0}, static_cast<id_type>(layer_width))
               | std::ranges::to<std::vector<id_type>>();

    for (auto _ : state) {
        bool completed = incidence_backward_bfs(ig, roots, static_cast<id_type>(hg.n_vertices()));
        benchmark::DoNotOptimize(completed);
    }

    state.counters["IG_Vertices"] = static_cast<double>(ig.n_vertices());
    state.counters["IG_Edges"] = static_cast<double>(ig.n_edges());
}

// --- Suite Setup & Registration ---

void add_args(argon::argument_parser& parser) {
    auto& group = parser.add_group("Backward BFS Benchmark Suite Options (hg-b-bfs)");
    parser.add_optional_argument<std::size_t>(group, "hg-b-bfs-e")
        .default_values(1000uz)
        .help("Number of BF-directed hyperedges");
    parser.add_optional_argument<std::size_t>(group, "hg-b-bfs-layer-width")
        .default_values(10uz)
        .help("Number of vertices per layer (tail and head sizes)");
    parser.add_optional_argument<std::size_t>(group, "hg-b-bfs-stride")
        .default_values(2uz)
        .help("Vertex shift between consecutive hyperedges (smaller = denser)");
    parser.add_flag("hg-b-bfs-l").help("Execute the benchmark for list models (bidirectional)");
    parser.add_flag("hg-b-bfs-al")
        .help("Execute the benchmark for asymmetric list models (WARNING: High execution times "
              "expected)");
    parser.add_flag("hg-b-bfs-m").help("Execute the benchmark for matrix models");
}

void register_benchmarks(const argon::argument_parser& parser) {
    const auto n_hedges = static_cast<int64_t>(parser.value<std::size_t>("hg-b-bfs-e"));
    const auto layer_width =
        static_cast<int64_t>(parser.value<std::size_t>("hg-b-bfs-layer-width"));
    const auto stride = static_cast<int64_t>(parser.value<std::size_t>("hg-b-bfs-stride"));

    using gl_list = gl::list_graph<gl::directed_t>;
    using gl_flat_list = gl::flat_list_graph<gl::directed_t>;
    using gl_matrix = gl::matrix_graph<gl::directed_t>;
    using gl_flat_matrix = gl::flat_matrix_graph<gl::directed_t>;

    using hgl_list = hgl::list_hypergraph<hgl::repr::bidirectional_t, hgl::bf_directed_t>;
    using hgl_flat_list = hgl::flat_list_hypergraph<hgl::repr::bidirectional_t, hgl::bf_directed_t>;

    using hgl_v_list = hgl::list_hypergraph<hgl::repr::vertex_major_t, hgl::bf_directed_t>;
    using hgl_v_flat_list =
        hgl::flat_list_hypergraph<hgl::repr::vertex_major_t, hgl::bf_directed_t>;

    using hgl_e_list = hgl::list_hypergraph<hgl::repr::hyperedge_major_t, hgl::bf_directed_t>;
    using hgl_e_flat_list =
        hgl::flat_list_hypergraph<hgl::repr::hyperedge_major_t, hgl::bf_directed_t>;

    using hgl_v_matrix = hgl::matrix_hypergraph<hgl::repr::vertex_major_t, hgl::bf_directed_t>;
    using hgl_e_matrix = hgl::matrix_hypergraph<hgl::repr::hyperedge_major_t, hgl::bf_directed_t>;

    using hgl_v_flat_matrix =
        hgl::flat_matrix_hypergraph<hgl::repr::vertex_major_t, hgl::bf_directed_t>;
    using hgl_e_flat_matrix =
        hgl::flat_matrix_hypergraph<hgl::repr::hyperedge_major_t, hgl::bf_directed_t>;

    if (parser.value<bool>("hg-b-bfs-l")) {
        benchmark::RegisterBenchmark("b_bfs/HGL/list/bidir", bm_hgl_backward_bfs<hgl_list>)
            ->Args({n_hedges, layer_width, stride})
            ->Unit(benchmark::kMillisecond);
        benchmark::RegisterBenchmark("b_bfs/HGL/flat_list/bidir", bm_hgl_backward_bfs<hgl_flat_list>)
            ->Args({n_hedges, layer_width, stride})
            ->Unit(benchmark::kMillisecond);

        benchmark::
            RegisterBenchmark("b_bfs/INCIDENCE/list", bm_gl_incidence_backward_bfs<hgl_list, gl_list>)
                ->Args({n_hedges, layer_width, stride})
                ->Unit(benchmark::kMillisecond);
        benchmark::
            RegisterBenchmark("b_bfs/INCIDENCE/flat_list", bm_gl_incidence_backward_bfs<hgl_list, gl_flat_list>)
                ->Args({n_hedges, layer_width, stride})
                ->Unit(benchmark::kMillisecond);
    }

    if (parser.value<bool>("hg-b-bfs-al")) {
        benchmark::RegisterBenchmark("bfs/HGL/list/v_major", bm_hgl_backward_bfs<hgl_v_list>)
            ->Args({n_hedges, layer_width, stride})
            ->Unit(benchmark::kMillisecond);
        benchmark::
            RegisterBenchmark("bfs/HGL/flat_list/v_major", bm_hgl_backward_bfs<hgl_v_flat_list>)
                ->Args({n_hedges, layer_width, stride})
                ->Unit(benchmark::kMillisecond);

        benchmark::RegisterBenchmark("bfs/HGL/list/e_major", bm_hgl_backward_bfs<hgl_e_list>)
            ->Args({n_hedges, layer_width, stride})
            ->Unit(benchmark::kMillisecond);
        benchmark::
            RegisterBenchmark("bfs/HGL/flat_list/e_major", bm_hgl_backward_bfs<hgl_e_flat_list>)
                ->Args({n_hedges, layer_width, stride})
                ->Unit(benchmark::kMillisecond);
    }

    if (parser.value<bool>("hg-b-bfs-m")) {
        benchmark::RegisterBenchmark("b_bfs/HGL/matrix/v_major", bm_hgl_backward_bfs<hgl_v_matrix>)
            ->Args({n_hedges, layer_width, stride})
            ->Unit(benchmark::kMillisecond);
        benchmark::RegisterBenchmark("b_bfs/HGL/matrix/e_major", bm_hgl_backward_bfs<hgl_e_matrix>)
            ->Args({n_hedges, layer_width, stride})
            ->Unit(benchmark::kMillisecond);

        benchmark::
            RegisterBenchmark("b_bfs/HGL/flat_matrix/v_major", bm_hgl_backward_bfs<hgl_v_flat_matrix>)
                ->Args({n_hedges, layer_width, stride})
                ->Unit(benchmark::kMillisecond);
        benchmark::
            RegisterBenchmark("b_bfs/HGL/flat_matrix/e_major", bm_hgl_backward_bfs<hgl_e_flat_matrix>)
                ->Args({n_hedges, layer_width, stride})
                ->Unit(benchmark::kMillisecond);

        benchmark::
            RegisterBenchmark("b_bfs/INCIDENCE/matrix", bm_gl_incidence_backward_bfs<hgl_list, gl_matrix>)
                ->Args({n_hedges, layer_width, stride})
                ->Unit(benchmark::kMillisecond);
        benchmark::
            RegisterBenchmark("b_bfs/INCIDENCE/flat_matrix", bm_gl_incidence_backward_bfs<hgl_list, gl_flat_matrix>)
                ->Args({n_hedges, layer_width, stride})
                ->Unit(benchmark::kMillisecond);
    }
}

namespace {
bool _registered = []() {
    gl_bench::runner::get().add_suite(
        "hg-b-bfs", suite{.add_args = add_args, .register_benchmarks = register_benchmarks}
    );
    return true;
}();
} // namespace

} // namespace gl_bench::hg_b_bfs
