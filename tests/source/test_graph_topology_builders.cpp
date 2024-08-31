#include "constants.hpp"

#include <gl/topologies.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph_topology_builders");

TEST_CASE_TEMPLATE_DEFINE(
    "graph topology builder functions should return a properly constructed graph",
    GraphType,
    graph_type_template
) {
    using graph_type = GraphType;
    using vertex_type = typename graph_type::vertex_type;
    using edge_type = typename graph_type::edge_type;

    SUBCASE("clique(n_vertices) should build a fully connected graph of size n_vertices") {
        const auto clique = lib::topology::clique<graph_type>(constants::n_elements_top);

        const auto is_vertex_fully_connected = [&clique](const vertex_type& source_vertex) {
            return std::ranges::all_of(
                clique.vertices(),
                [&](const vertex_type& destination_vertex) {
                    return source_vertex == destination_vertex
                        or clique.has_edge(source_vertex, destination_vertex);
                }
            );
        };

        CHECK(std::ranges::all_of(clique.vertices(), is_vertex_fully_connected));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    graph_type_template,
    lib::graph<lib::list_graph_traits<lib::directed_t>>, // directed adjacency list
    lib::graph<lib::list_graph_traits<lib::undirected_t>>, // undirected adjacency list
    lib::graph<lib::matrix_graph_traits<lib::directed_t>>, // directed adjacency matrix
    lib::graph<lib::matrix_graph_traits<lib::undirected_t>> // undirected adjacency matrix
);

TEST_SUITE_END(); // test_graph_topology_builders

} // namespace gl_testing
