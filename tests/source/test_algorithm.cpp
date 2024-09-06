#include "constants.hpp"

#include <gl/algorithms.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

#include <iostream>

namespace gl_testing {

TEST_SUITE_BEGIN("test_algorithm");

TEST_CASE("breadth_virst_search") {
    using graph_type = lib::graph<>;
    using vertex_type = typename graph_type::vertex_type;

    const auto graph = lib::topology::full_bipartite<graph_type>(constants::three, constants::two);

    std::cout << "[no return]\n";
    lib::algorithm::breadth_first_search<lib::algorithm::no_return>(
        graph,
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
