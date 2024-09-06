#include "constants.hpp"

#include <gl/algorithms.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

#include <iostream>

namespace gl_testing {

TEST_SUITE_BEGIN("test_algorithm");

TEST_CASE_TEMPLATE_DEFINE(
    "breadth_first_search no return should properly traverse the graph",
    GraphType,
    bfs_no_return_graph_template
) {
    using graph_type = lib::graph<>;

    graph_type graph;
    std::vector<lib_t::id_type> expected_previsit_order;

    SUBCASE("single vertex graph") {
        graph = lib::topology::clique<graph_type>(constants::zero_elements);
        expected_previsit_order = {};
    }

    SUBCASE("single vertex graph") {
        graph = lib::topology::clique<graph_type>(constants::one_element);
        expected_previsit_order = {0};
    }

    SUBCASE("clique") {
        graph = lib::topology::clique<graph_type>(constants::n_elements_alg);
        for (auto id = lib::constants::initial_id; id < constants::n_elements_alg; id++)
            expected_previsit_order.push_back(id);
    }

    SUBCASE("path graph") {
        graph = lib::topology::bidirectional_path<graph_type>(constants::n_elements_alg);
        for (auto id = lib::constants::initial_id; id < constants::n_elements_alg; id++)
            expected_previsit_order.push_back(id);
    }

    SUBCASE("full bipartite graph") {
        /*
        A = {0, 1, 2}
        B = {3, 4}
        root = 0 -> connected to B -> connected to A (root already visited)
        */
        graph = lib::topology::full_bipartite<graph_type>(constants::three, constants::two);
        expected_previsit_order = {0, 3, 4, 1, 2};
    }

    CAPTURE(graph);
    CAPTURE(expected_previsit_order);

    std::vector<lib_t::id_type> expected_postvisit_order = expected_previsit_order;

    std::vector<lib_t::id_type> previsit_order, postvisit_order;
    lib::algorithm::breadth_first_search<lib::algorithm::no_return>(
        graph,
        lib::algorithm::no_root_vertex,
        [&](const auto& vertex) { // previsit
            previsit_order.push_back(vertex.id());
        },
        [&](const auto& vertex) { // postvisit
            postvisit_order.push_back(vertex.id());
        }
    );

    CHECK(std::ranges::equal(previsit_order, expected_previsit_order));
    CHECK(std::ranges::equal(postvisit_order, expected_postvisit_order));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    bfs_no_return_graph_template,
    lib::graph<lib::list_graph_traits<lib::directed_t>>, // directed adjacency list
    lib::graph<lib::list_graph_traits<lib::undirected_t>>, // undirected adjacency list
    lib::graph<lib::matrix_graph_traits<lib::directed_t>>, // directed adjacency matrix
    lib::graph<lib::matrix_graph_traits<lib::undirected_t>> // undirected adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "breadth_first_search no return with root vertex should properly traverse the graph",
    GraphType,
    bfs_no_return_with_root_graph_template
) {
    using graph_type = lib::graph<>;

    graph_type graph;
    std::optional<lib_t::id_type> root_vertex_id;
    std::deque<lib_t::id_type> expected_previsit_order;

    SUBCASE("single vertex graph") {
        graph = lib::topology::clique<graph_type>(constants::one_element);
        root_vertex_id.emplace(constants::vertex_id_1);
        expected_previsit_order = {0};
    }

    SUBCASE("clique") {
        graph = lib::topology::clique<graph_type>(constants::n_elements_alg);
        root_vertex_id.emplace(constants::vertex_id_3);

        for (auto id = lib::constants::initial_id; id < constants::n_elements_alg; id++)
            expected_previsit_order.push_back(id);
        expected_previsit_order.erase(
            std::next(expected_previsit_order.begin(), constants::vertex_id_3)
        );
        expected_previsit_order.push_front(constants::vertex_id_3);
    }

    CAPTURE(graph);
    CAPTURE(root_vertex_id);
    CAPTURE(expected_previsit_order);

    std::deque<lib_t::id_type> expected_postvisit_order = expected_previsit_order;

    std::vector<lib_t::id_type> previsit_order, postvisit_order;
    lib::algorithm::breadth_first_search<lib::algorithm::no_return>(
        graph,
        root_vertex_id,
        [&](const auto& vertex) { // previsit
            previsit_order.push_back(vertex.id());
        },
        [&](const auto& vertex) { // postvisit
            postvisit_order.push_back(vertex.id());
        }
    );

    CHECK(std::ranges::equal(previsit_order, expected_previsit_order));
    CHECK(std::ranges::equal(postvisit_order, expected_postvisit_order));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    bfs_no_return_with_root_graph_template,
    lib::graph<lib::list_graph_traits<lib::directed_t>>, // directed adjacency list
    lib::graph<lib::list_graph_traits<lib::undirected_t>>, // undirected adjacency list
    lib::graph<lib::matrix_graph_traits<lib::directed_t>>, // directed adjacency matrix
    lib::graph<lib::matrix_graph_traits<lib::undirected_t>> // undirected adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "breadth_first_search with return should properly traverse the graph",
    GraphType,
    bfs_return_graph_template
) {
    using graph_type = GraphType;
    using vertex_type = typename graph_type::vertex_type;

    const auto graph = lib::topology::complete_binary_tree<graph_type>(constants::three);
    const auto search_tree = lib::algorithm::breadth_first_search<graph_type>(graph);

    REQUIRE_EQ(search_tree.n_vertices(), graph.n_vertices());

    const auto vertex_id_view = std::views::iota(constants::first_element_idx, graph.n_vertices());

    // verify that for each vertex the edges are equivalent to those in the original graph
    CHECK(std::ranges::all_of(vertex_id_view, [&](const lib_t::id_type id) {
        const auto adj_edges_graph = graph.adjacent_edges(id);
        const auto adj_edges_search_tree = search_tree.adjacent_edges(id);

        if (adj_edges_search_tree.distance() != adj_edges_graph.distance())
            return false;

        for (auto [graph_edge_it, search_tree_edge_it] =
                 std::make_pair(adj_edges_graph.begin(), adj_edges_search_tree.begin());
             graph_edge_it != adj_edges_graph.end();
             graph_edge_it++, search_tree_edge_it++) {
            // verify that the edges match
            const auto search_tree_incident_vertex_id =
                search_tree_edge_it->incident_vertex(search_tree.get_vertex(id)).id();
            const auto graph_incident_vertex_id =
                graph_edge_it->incident_vertex(graph.get_vertex(id)).id();

            if (search_tree_incident_vertex_id != graph_incident_vertex_id)
                return false;
        }

        return true;
    }));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    bfs_return_graph_template,
    lib::graph<lib::list_graph_traits<lib::directed_t>>, // directed adjacency list
    lib::graph<lib::list_graph_traits<lib::undirected_t>>, // undirected adjacency list
    lib::graph<lib::matrix_graph_traits<lib::directed_t>>, // directed adjacency matrix
    lib::graph<lib::matrix_graph_traits<lib::undirected_t>> // undirected adjacency matrix
);

TEST_SUITE_END(); // test_algorithm

} // namespace gl_testing
