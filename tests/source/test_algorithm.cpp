#include "constants.hpp"

#include <gl/algorithms.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

#include <iostream>

namespace gl_testing {

TEST_SUITE_BEGIN("test_algorithm");

TEST_CASE("breadth_first_search_no_return") {
    using graph_type = lib::graph<>;

    graph_type graph;
    std::vector<lib_t::id_type> expected_visit_order;

    SUBCASE("single vertex graph") {
        graph = lib::topology::clique<graph_type>(constants::zero_elements);
        expected_visit_order = {};
    }

    SUBCASE("single vertex graph") {
        graph = lib::topology::clique<graph_type>(constants::one_element);
        expected_visit_order = {0};
    }

    SUBCASE("clique") {
        graph = lib::topology::clique<graph_type>(constants::n_elements_alg);
        for (auto id = lib::constants::initial_id; id < constants::n_elements_alg; id++)
            expected_visit_order.push_back(id);
    }

    SUBCASE("path graph") {
        graph = lib::topology::bidirectional_path<graph_type>(constants::n_elements_alg);
        for (auto id = lib::constants::initial_id; id < constants::n_elements_alg; id++)
            expected_visit_order.push_back(id);
    }

    SUBCASE("full bipartite graph") {
        /*
        A = {0, 1, 2}
        B = {3, 4}
        root = 0 -> connected to B -> connected to A (root already visited)
        */
        graph = lib::topology::full_bipartite<graph_type>(constants::three, constants::two);
        expected_visit_order = {0, 3, 4, 1, 2};
    }

    CAPTURE(graph);
    CAPTURE(expected_visit_order);

    std::vector<lib_t::id_type> visited_vertices;
    lib::algorithm::breadth_first_search<lib::algorithm::no_return>(
        graph,
        lib::algorithm::no_root_vertex,
        [&](const auto& vertex) { // previsit
            visited_vertices.push_back(vertex.id());
        }
    );

    CHECK(std::ranges::equal(visited_vertices, expected_visit_order));
}

TEST_CASE("breadth_virst_search") {
    using graph_type = lib::graph<>;
    using vertex_type = typename graph_type::vertex_type;

    const auto graph = lib::topology::full_bipartite<graph_type>(constants::three, constants::two);

    std::cout << "[no return]\n";
    lib::algorithm::breadth_first_search<lib::algorithm::no_return>(
        graph,
        lib::algorithm::no_root_vertex,
        [](const gl::vertex_descriptor<std::monostate>& vertex) {
            std::cout << "visiting: " << vertex.id() << std::endl;
        }
    );

    std::cout << "\n[return]\n";
    const auto search_tree = lib::algorithm::breadth_first_search<graph_type>(graph);
    for (const auto& vertex : search_tree.vertices()) {
        std::cout << vertex.id() << " :";
        for (const auto& edge : search_tree.adjacent_edges(vertex))
            std::cout << " " << edge.incident_vertex(vertex).id();
        std::cout << std::endl;
    }
}

TEST_SUITE_END(); // test_algorithm

} // namespace gl_testing
