#include "constants.hpp"

#include <gl/topologies.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph_topology_builders");

namespace predicate {

template <lib_tt::c_graph_type GraphType>
[[nodiscard]] auto is_vertex_fully_connected(const GraphType& graph) {
    using vertex_type = typename GraphType::vertex_type;
    return [&graph](const vertex_type& source_vertex) {
        return std::ranges::all_of(graph.vertices(), [&](const vertex_type& destination_vertex) {
            return source_vertex == destination_vertex
                or graph.has_edge(source_vertex, destination_vertex);
        });
    };
}

template <lib_tt::c_graph_type GraphType>
[[nodiscard]] auto is_vertex_not_connected(const GraphType& graph) {
    using vertex_type = typename GraphType::vertex_type;
    return [&graph](const vertex_type& source_vertex) {
        return std::ranges::none_of(graph.vertices(), [&](const vertex_type& destination_vertex) {
            return graph.has_edge(source_vertex, destination_vertex);
        });
    };
}

template <lib_tt::c_graph_type GraphType>
[[nodiscard]] auto is_vertex_connected_to_next_only(const GraphType& graph) {
    using vertex_type = typename GraphType::vertex_type;
    return [&graph](const vertex_type& source_vertex) {
        const auto& next_vertex =
            graph.get_vertex((source_vertex.id() + constants::one_element) % graph.n_vertices());

        return std::ranges::all_of(graph.vertices(), [&](const auto& vertex) {
            return (vertex == next_vertex) == graph.has_edge(source_vertex, vertex);
        });
    };
}

template <lib_tt::c_graph_type GraphType>
[[nodiscard]] auto is_vertex_connected_to_prev_only(const GraphType& graph) {
    using vertex_type = typename GraphType::vertex_type;
    return [&graph](const vertex_type& source_vertex) {
        const auto& prev_vertex = graph.get_vertex(
            (source_vertex.id() + graph.n_vertices() - constants::one_element) % graph.n_vertices()
        );

        return std::ranges::all_of(graph.vertices(), [&](const auto& vertex) {
            return (vertex == prev_vertex) == graph.has_edge(source_vertex, vertex);
        });
    };
}

template <lib_tt::c_graph_type GraphType>
[[nodiscard]] auto is_vertex_connected_to_id_adjacent(const GraphType& graph) {
    using vertex_type = typename GraphType::vertex_type;
    return [&graph](const vertex_type& source_vertex) {
        const auto& next_vertex =
            graph.get_vertex((source_vertex.id() + constants::one_element) % graph.n_vertices());

        const auto& prev_vertex = graph.get_vertex(
            (source_vertex.id() + graph.n_vertices() - constants::one_element) % graph.n_vertices()
        );

        return std::ranges::all_of(graph.vertices(), [&](const auto& vertex) {
            return (vertex == prev_vertex or vertex == next_vertex)
                == graph.has_edge(source_vertex, vertex);
        });
    };
}

/*

template <lib_tt::c_graph_type GraphType>
[[nodiscard]] auto predicate(const GraphType& graph) {
    using vertex_type = typename GraphType::vertex_type;
    return [&graph](const vertex_type&) {};
}

*/

} // namespace predicate

TEST_CASE_TEMPLATE_DEFINE(
    "directional_tag-independent graph topology builders tests", GraphType, graph_type_template
) {
    using graph_type = GraphType;
    using vertex_type = typename graph_type::vertex_type;
    using edge_type = typename graph_type::edge_type;

    SUBCASE("clique(n_vertices) should build a fully connected graph of size n_vertices") {
        const auto clique = lib::topology::clique<graph_type>(constants::n_elements_top);
        CHECK(std::ranges::all_of(clique.vertices(), predicate::is_vertex_fully_connected(clique)));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    graph_type_template,
    lib::graph<lib::list_graph_traits<lib::directed_t>>, // directed adjacency list
    lib::graph<lib::list_graph_traits<lib::undirected_t>>, // undirected adjacency list
    lib::graph<lib::matrix_graph_traits<lib::directed_t>>, // directed adjacency matrix
    lib::graph<lib::matrix_graph_traits<lib::undirected_t>> // undirected adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "directed graph specific topology builders tests", GraphType, directed_graph_type_template
) {
    using graph_type = GraphType;
    using vertex_type = typename graph_type::vertex_type;
    using edge_type = typename graph_type::edge_type;

    SUBCASE("cycle(n_vertices) should build a one-way cycle graph of size n_vertices") {
        const auto cycle = lib::topology::cycle<graph_type>(constants::n_elements_top);
        CHECK(std::ranges::all_of(
            cycle.vertices(), predicate::is_vertex_connected_to_next_only(cycle)
        ));
    }

    SUBCASE("bidirectional_cycle(n_vertices) should build a two-way cycle graph of size n_vertices"
    ) {
        const auto cycle =
            lib::topology::bidirectional_cycle<graph_type>(constants::n_elements_top);
        CHECK(std::ranges::all_of(
            cycle.vertices(), predicate::is_vertex_connected_to_id_adjacent(cycle)
        ));
    }

    SUBCASE("path(n_vertices) should build a one-way path graph of size n_vertices") {
        const auto path = lib::topology::path<graph_type>(constants::n_elements_top);
        const auto n_source_vertices = path.n_vertices() - constants::one_element;

        REQUIRE(std::ranges::all_of(
            path.vertices() | std::views::take(n_source_vertices),
            predicate::is_vertex_connected_to_next_only(path)
        ));
        CHECK(predicate::is_vertex_not_connected(path)(path.vertices().element_at(n_source_vertices)
        ));
    }

    SUBCASE("bidirectional_path(n_vertices) should build a two-way path graph of size n_vertices") {
        const auto path = lib::topology::bidirectional_path<graph_type>(constants::n_elements_top);
        const auto n_source_vertices = path.n_vertices() - constants::one_element;

        const auto vertices = path.vertices();

        REQUIRE(std::ranges::all_of(
            std::ranges::next(vertices.begin(), constants::one_element),
            std::ranges::next(vertices.begin(), n_source_vertices),
            predicate::is_vertex_connected_to_id_adjacent(path)
        ));
        CHECK(predicate::is_vertex_connected_to_next_only(path)(*path.vertices().begin()));
        CHECK(predicate::is_vertex_connected_to_prev_only(path)(
            path.vertices().element_at(n_source_vertices)
        ));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    directed_graph_type_template,
    lib::graph<lib::list_graph_traits<lib::directed_t>>, // adjacency list
    lib::graph<lib::matrix_graph_traits<lib::directed_t>> // adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "undirected graph specific topology builders tests", GraphType, undirected_graph_type_template
) {
    using graph_type = GraphType;
    using vertex_type = typename graph_type::vertex_type;
    using edge_type = typename graph_type::edge_type;

    SUBCASE("cycle(n_vertices) should build a two-way cycle graph of size n_vertices") {
        const auto cycle = lib::topology::cycle<graph_type>(constants::n_elements_top);
        CHECK(std::ranges::all_of(
            cycle.vertices(), predicate::is_vertex_connected_to_id_adjacent(cycle)
        ));
    }

    SUBCASE("path(n_vertices) should build a two-way path graph of size n_vertices") {
        const auto path = lib::topology::path<graph_type>(constants::n_elements_top);
        const auto n_source_vertices = path.n_vertices() - constants::one_element;

        const auto vertices = path.vertices();

        REQUIRE(std::ranges::all_of(
            std::ranges::next(vertices.begin(), constants::one_element),
            std::ranges::next(vertices.begin(), n_source_vertices),
            predicate::is_vertex_connected_to_id_adjacent(path)
        ));
        CHECK(predicate::is_vertex_connected_to_next_only(path)(*path.vertices().begin()));
        CHECK(predicate::is_vertex_connected_to_prev_only(path)(
            path.vertices().element_at(n_source_vertices)
        ));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    undirected_graph_type_template,
    lib::graph<lib::list_graph_traits<lib::undirected_t>>, // adjacency list
    lib::graph<lib::matrix_graph_traits<lib::undirected_t>> // adjacency matrix
);

TEST_SUITE_END(); // test_graph_topology_builders

} // namespace gl_testing
