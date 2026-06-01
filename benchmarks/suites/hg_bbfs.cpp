#include "runner.hpp"
#include "suite.hpp"

#include <gl/algorithm.hpp>
#include <gl/graph.hpp>

#include <benchmark/benchmark.h>
#include <hgl/algorithm.hpp>
#include <hgl/conversion.hpp>
#include <hgl/hypergraph.hpp>

namespace gl_bench::hg_b_bfs {

// --- BF-Directed Hypergraph Topology Generator ---

template <hgl::traits::c_hypergraph HypergraphType>
HypergraphType gen_bf_chain_hypergraph(
    const std::size_t num_hyperedges, const std::size_t layer_width
) {
    using id_type = typename HypergraphType::id_type;

    // A layered chain: Hyperedge `e` connects layer `e` (tail) to layer `e+1` (head).
    const auto n_vertices = static_cast<id_type>((num_hyperedges + 1) * layer_width);
    const auto n_hyperedges = static_cast<id_type>(num_hyperedges);

    HypergraphType hgraph{n_vertices, n_hyperedges};

    for (id_type e = 0; e < n_hyperedges; ++e) {
        std::vector<id_type> tail;
        std::vector<id_type> head;
        tail.reserve(layer_width);
        head.reserve(layer_width);

        const auto tail_start = static_cast<id_type>(e * layer_width);
        const auto head_start = static_cast<id_type>((e + 1) * layer_width);

        for (std::size_t k = 0; k < layer_width; ++k) {
            tail.push_back(tail_start + static_cast<id_type>(k));
            head.push_back(head_start + static_cast<id_type>(k));
        }

        hgraph.bind_tail(tail, e);
        hgraph.bind_head(head, e);
    }

    return hgraph;
}

// --- Native HGL Backward BFS Benchmark ---

template <hgl::traits::c_bf_directed_hypergraph Hypergraph>
void bm_hgl_backward_bfs(benchmark::State& state) {
    const auto n_hedges = static_cast<std::size_t>(state.range(0));
    const auto layer_width = static_cast<std::size_t>(state.range(1));

    auto hg = gen_bf_chain_hypergraph<Hypergraph>(n_hedges, layer_width);

    // Initial roots: All vertices in Layer 0
    std::vector<typename Hypergraph::id_type> roots;
    roots.reserve(layer_width);
    for (std::size_t i = 0; i < layer_width; ++i) {
        roots.push_back(static_cast<typename Hypergraph::id_type>(i));
    }

    for (auto _ : state) {
        // Run native B-BFS
        auto search_tree = hgl::algorithm::backward_bfs(hg, roots);
        benchmark::DoNotOptimize(search_tree);
    }

    state.counters["Vertices"] = static_cast<double>(hg.n_vertices());
    state.counters["Hyperedges"] = static_cast<double>(hg.n_hyperedges());
    state.counters["Incidences"] = static_cast<double>(n_hedges * layer_width * 2);
}

// --- GL Incidence Graph Backward BFS Equivalent ---

template <
    hgl::traits::c_bf_directed_hypergraph Hypergraph,
    gl::traits::c_directed_graph IncidenceGraph>
void bm_gl_incidence_backward_bfs(benchmark::State& state) {
    using id_type = typename Hypergraph::id_type;

    const auto n_hedges = static_cast<std::size_t>(state.range(0));
    const auto layer_width = static_cast<std::size_t>(state.range(1));

    // Memory Guard for Matrix Representations
    if constexpr (gl::traits::c_adjacency_matrix_graph<IncidenceGraph>) {
        const auto n_vertices = (n_hedges + 1) * layer_width;
        const auto ig_vertices = n_vertices + n_hedges;

        // Skip if matrix requires > 4.5 GB of contiguous RAM
        if (ig_vertices > 35000) {
            state.SkipWithError("Matrix requires > 4.5 GB of memory; skipping.");
            return;
        }
    }

    auto hg = gen_bf_chain_hypergraph<Hypergraph>(n_hedges, layer_width);
    auto ig = hgl::incidence_graph<IncidenceGraph>(hg);

    // Create the search nodes for the initial queue from Layer 0
    std::vector<gl::algorithm::search_node<IncidenceGraph>> root_nodes;
    root_nodes.reserve(layer_width);
    for (std::size_t i = 0; i < layer_width; ++i) {
        root_nodes.emplace_back(static_cast<typename IncidenceGraph::id_type>(i));
    }

    for (auto _ : state) {
        std::vector<bool> visited_v(hg.n_vertices(), false);
        std::vector<gl::size_type> tail_unvisited(hg.n_hyperedges());

        // The in-degree of a hyperedge node in a BF-directed incidence graph is exactly its tail size
        for (std::size_t i = 0; i < hg.n_hyperedges(); ++i) {
            tail_unvisited[i] = ig.in_degree(static_cast<id_type>(hg.n_vertices() + i));
        }

        // 1. Visit Vertex Predicate: Ignore already visited original vertices
        auto visit_vertex_pred = [&](typename IncidenceGraph::id_type v) {
            if (v < hg.n_vertices()) {
                return not visited_v[v];
            }
            return true; // Hyperedge nodes bypass this check (handled by enqueue predicate)
        };

        // 2. Visit Callback: Mark original vertices as visited
        auto visit =
            [&](typename IncidenceGraph::id_type v, typename IncidenceGraph::id_type /*p*/) {
                if (v < hg.n_vertices()) {
                    visited_v[v] = true;
                }
                return true;
            };

        // 3. Enqueue Predicate: The Blocking B-Reachability Logic
        auto enqueue_node_pred =
            [&](typename IncidenceGraph::id_type target_id, const auto& /*edge*/) {
                if (target_id >= hg.n_vertices()) {
                    // If it's a hyperedge node, decrement its blocking counter
                    const auto he_idx = target_id - hg.n_vertices();
                    return static_cast<gl::algorithm::decision>(--tail_unvisited[he_idx] == 0);
                }
                else {
                    // If it's a vertex node, enqueue only if unvisited
                    return static_cast<gl::algorithm::decision>(not visited_v[target_id]);
                }
            };

        // Execute the custom BFS logic over the incidence graph
        bool completed =
            gl::algorithm::bfs(ig, root_nodes, visit_vertex_pred, visit, enqueue_node_pred);
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
    parser.add_optional_argument<std::size_t>(group, "hg-b-bfs-width")
        .default_values(100uz)
        .help("Number of vertices per layer (tail and head sizes)");
}

void register_benchmarks(const argon::argument_parser& parser) {
    const auto n_hedges = static_cast<int64_t>(parser.value<std::size_t>("hg-b-bfs-e"));
    const auto layer_width = static_cast<int64_t>(parser.value<std::size_t>("hg-b-bfs-width"));

    // Standard Incidence Graphs MUST be directed for BF-Directed Hypergraphs
    using gl_list = gl::list_graph<gl::directed_t>;
    using gl_flat_list = gl::flat_list_graph<gl::directed_t>;
    using gl_matrix = gl::matrix_graph<gl::directed_t>;
    using gl_flat_matrix = gl::flat_matrix_graph<gl::directed_t>;

    // BF-Directed Hypergraphs
    using hgl_list = hgl::list_hypergraph<hgl::repr::bidirectional_t, hgl::bf_directed_t>;
    using hgl_flat_list = hgl::flat_list_hypergraph<hgl::repr::bidirectional_t, hgl::bf_directed_t>;

    using hgl_v_matrix = hgl::matrix_hypergraph<hgl::repr::vertex_major_t, hgl::bf_directed_t>;
    using hgl_e_matrix = hgl::matrix_hypergraph<hgl::repr::hyperedge_major_t, hgl::bf_directed_t>;

    using hgl_v_flat_matrix =
        hgl::flat_matrix_hypergraph<hgl::repr::vertex_major_t, hgl::bf_directed_t>;
    using hgl_e_flat_matrix =
        hgl::flat_matrix_hypergraph<hgl::repr::hyperedge_major_t, hgl::bf_directed_t>;

    benchmark::RegisterBenchmark("b_bfs/HGL/list", bm_hgl_backward_bfs<hgl_list>)
        ->Args({n_hedges, layer_width})
        ->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("b_bfs/HGL/flat_list", bm_hgl_backward_bfs<hgl_flat_list>)
        ->Args({n_hedges, layer_width})
        ->Unit(benchmark::kMillisecond);

    benchmark::RegisterBenchmark("b_bfs/HGL/matrix/v_major", bm_hgl_backward_bfs<hgl_v_matrix>)
        ->Args({n_hedges, layer_width})
        ->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("b_bfs/HGL/matrix/e_major", bm_hgl_backward_bfs<hgl_e_matrix>)
        ->Args({n_hedges, layer_width})
        ->Unit(benchmark::kMillisecond);

    benchmark::
        RegisterBenchmark("b_bfs/HGL/flat_matrix/v_major", bm_hgl_backward_bfs<hgl_v_flat_matrix>)
            ->Args({n_hedges, layer_width})
            ->Unit(benchmark::kMillisecond);
    benchmark::
        RegisterBenchmark("b_bfs/HGL/flat_matrix/e_major", bm_hgl_backward_bfs<hgl_e_flat_matrix>)
            ->Args({n_hedges, layer_width})
            ->Unit(benchmark::kMillisecond);

    benchmark::
        RegisterBenchmark("b_bfs/INCIDENCE/list", bm_gl_incidence_backward_bfs<hgl_list, gl_list>)
            ->Args({n_hedges, layer_width})
            ->Unit(benchmark::kMillisecond);
    benchmark::
        RegisterBenchmark("b_bfs/INCIDENCE/flat_list", bm_gl_incidence_backward_bfs<hgl_list, gl_flat_list>)
            ->Args({n_hedges, layer_width})
            ->Unit(benchmark::kMillisecond);
    benchmark::
        RegisterBenchmark("b_bfs/INCIDENCE/matrix", bm_gl_incidence_backward_bfs<hgl_list, gl_matrix>)
            ->Args({n_hedges, layer_width})
            ->Unit(benchmark::kMillisecond);
    benchmark::
        RegisterBenchmark("b_bfs/INCIDENCE/flat_matrix", bm_gl_incidence_backward_bfs<hgl_list, gl_flat_matrix>)
            ->Args({n_hedges, layer_width})
            ->Unit(benchmark::kMillisecond);
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
