#include "constants.hpp"

#include <gl/topologies.hpp>

#include <doctest.h>

#include <iostream>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph_topology_builders");

namespace {

template <lib_tt::c_graph_type GraphType>
[[nodiscard]] lib_t::size_type n_unique_edges_for_bidir_topology(
    const lib_t::size_type n_connections
) {
    if constexpr (lib_tt::is_directed_v<GraphType>)
        return n_connections;
    else
        return n_connections / constants::two;
}

template <lib_tt::c_graph_type GraphType>
void verify_graph_size(
    const GraphType& graph,
    const lib_t::size_type expected_n_vertices,
    const lib_t::size_type expected_n_connections
) {
    REQUIRE_EQ(graph.n_vertices(), expected_n_vertices);
    REQUIRE_EQ(graph.n_unique_edges(), expected_n_connections);
}

template <lib_tt::c_graph_type GraphType>
void verify_bidir_graph_size(
    const GraphType& graph,
    const lib_t::size_type expected_n_vertices,
    const lib_t::size_type expected_n_connections
) {
    REQUIRE_EQ(graph.n_vertices(), expected_n_vertices);
    REQUIRE_EQ(
        graph.n_unique_edges(), n_unique_edges_for_bidir_topology<GraphType>(expected_n_connections)
    );
}

} // namespace

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
[[nodiscard]] auto is_vertex_not_connected_to_any_from(
    const GraphType& graph, const auto& vertex_it_range
) {
    using vertex_type = typename GraphType::vertex_type;
    return [&](const vertex_type& source_vertex) {
        return std::ranges::none_of(vertex_it_range, [&](const auto& vertex) {
            return vertex != source_vertex
               and (graph.has_edge(source_vertex, vertex) or graph.has_edge(vertex, source_vertex));
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

template <lib_tt::c_graph_type GraphType>
[[nodiscard]] auto is_connected_to_binary_chlidren(const GraphType& graph) {
    using vertex_type = typename GraphType::vertex_type;
    return [&graph](const vertex_type& source_vertex) {
        const auto destination_ids =
            lib::topology::detail::get_binary_destination_ids(source_vertex.id());

        if (destination_ids.first >= graph.n_vertices())
            // no need to check second as second = first + 1
            return graph.adjacent_edges(source_vertex).distance() == constants::zero;

        const auto& destination_1 = graph.get_vertex(destination_ids.first);
        const auto& destination_2 = graph.get_vertex(destination_ids.second);

        return std::ranges::all_of(
            graph.vertices(),
            [&](const auto& vertex) {
                if (vertex == destination_1 or vertex == destination_2)
                    return graph.has_edge(source_vertex, vertex) and not graph.has_edge(vertex, source_vertex);
                return not graph.has_edge(source_vertex, vertex);
            }
        );
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

        const auto expected_n_connections =
            constants::n_elements_top * (constants::n_elements_top - constants::one_element);
        verify_bidir_graph_size(clique, constants::n_elements_top, expected_n_connections);

        CHECK(std::ranges::all_of(clique.vertices(), predicate::is_vertex_fully_connected(clique)));
    }

    SUBCASE("full_bipartite(n_vertices_a, n_vertices_b) should build a full bipartite graph with "
            "vertex sets of sizes n_vertices_a and n_vertices_b respectively") {
        const auto full_bipatite = lib::topology::full_bipartite<graph_type>(
            constants::n_elements_top, constants::n_elements
        );

        const auto expected_n_vertices = constants::n_elements_top + constants::n_elements;
        // `2x` is required to account for adding edges both ways
        const auto expected_n_connections =
            constants::two * constants::n_elements_top * constants::n_elements;
        verify_bidir_graph_size(full_bipatite, expected_n_vertices, expected_n_connections);

        const auto vertices = full_bipatite.vertices();

        const auto vertices_a = vertices | std::views::take(constants::n_elements_top);
        const auto vertices_b = lib::make_iterator_range(
            std::ranges::next(vertices.begin(), constants::n_elements_top),
            std::ranges::next(vertices.begin(), expected_n_vertices)
        );

        // verify that all vertices from A are connected to all vertices from B and vice versa
        CHECK(std::ranges::all_of(
            vertices_a | std::views::take(constants::n_elements_top),
            [&](const vertex_type& source_vertex) {
                return std::ranges::all_of(vertices_b, [&](const auto& vertex) {
                    return full_bipatite.has_edge(source_vertex, vertex)
                       and full_bipatite.has_edge(vertex, source_vertex);
                });
            }
        ));

        CHECK(std::ranges::all_of(
            vertices_a,
            predicate::is_vertex_not_connected_to_any_from<graph_type>(full_bipatite, vertices_a)
        ));
        CHECK(std::ranges::all_of(
            vertices_b,
            predicate::is_vertex_not_connected_to_any_from<graph_type>(full_bipatite, vertices_b)
        ));
    }

    SUBCASE("complete_binary_tree(depth) should return a complete binay tree with the given depth"
    ) {
        SUBCASE("depth = 0 : empty graph") {
            const auto complete_bin_tree =
                lib::topology::complete_binary_tree<graph_type>(constants::zero);
            REQUIRE_EQ(complete_bin_tree.n_vertices(), constants::zero_elements);
            REQUIRE_EQ(complete_bin_tree.n_unique_edges(), constants::zero_elements);
        }

        SUBCASE("depth = 1 : graph with one vertex and no edges") {
            const auto complete_bin_tree =
                lib::topology::complete_binary_tree<graph_type>(constants::one);
            REQUIRE_EQ(complete_bin_tree.n_vertices(), constants::one_element);
            REQUIRE_EQ(complete_bin_tree.n_unique_edges(), constants::zero_elements);
        }
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
        verify_graph_size(cycle, constants::n_elements_top, constants::n_elements_top);

        CHECK(std::ranges::all_of(
            cycle.vertices(), predicate::is_vertex_connected_to_next_only(cycle)
        ));
    }

    SUBCASE("bidirectional_cycle(n_vertices) should build a two-way cycle graph of size n_vertices"
    ) {
        const auto cycle =
            lib::topology::bidirectional_cycle<graph_type>(constants::n_elements_top);
        verify_graph_size(
            cycle, constants::n_elements_top, constants::two * constants::n_elements_top
        );

        CHECK(std::ranges::all_of(
            cycle.vertices(), predicate::is_vertex_connected_to_id_adjacent(cycle)
        ));
    }

    SUBCASE("path(n_vertices) should build a one-way path graph of size n_vertices") {
        const auto path = lib::topology::path<graph_type>(constants::n_elements_top);
        const auto n_source_vertices = path.n_vertices() - constants::one_element;

        verify_graph_size(path, constants::n_elements_top, n_source_vertices);

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

        verify_graph_size(path, constants::n_elements_top, constants::two * n_source_vertices);

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

    SUBCASE("complete_binary_tree(depth) should return a complete binay tree with the given depth"
    ) {
        const auto bin_tree = lib::topology::complete_binary_tree<graph_type>(constants::three);

        const auto expected_n_vertices =
            lib::util::upow_sum(constants::two, constants::zero, constants::two);
        const auto expected_n_connections = expected_n_vertices - constants::one;
        verify_graph_size(bin_tree, expected_n_vertices, expected_n_connections);

        CHECK(std::ranges::all_of(
            bin_tree.vertices(),
            predicate::is_connected_to_binary_chlidren(bin_tree)
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
        graph_type cycle;

        SUBCASE("cycle builder") {
            cycle = lib::topology::cycle<graph_type>(constants::n_elements_top);
        }

        SUBCASE("bidirectional_cycle builder") {
            cycle = lib::topology::bidirectional_cycle<graph_type>(constants::n_elements_top);
        }

        CAPTURE(cycle);

        verify_graph_size(cycle, constants::n_elements_top, constants::n_elements_top);

        CHECK(std::ranges::all_of(
            cycle.vertices(), predicate::is_vertex_connected_to_id_adjacent(cycle)
        ));
    }

    SUBCASE("path(n_vertices) should build a two-way path graph of size n_vertices") {
        graph_type path;

        SUBCASE("path builder") {
            path = lib::topology::path<graph_type>(constants::n_elements_top);
        }

        SUBCASE("bidirectional_path builder") {
            path = lib::topology::bidirectional_path<graph_type>(constants::n_elements_top);
        }

        CAPTURE(path);

        const auto n_source_vertices = path.n_vertices() - constants::one_element;
        verify_graph_size(path, constants::n_elements_top, n_source_vertices);

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

    // SUBCASE("complete_binary_tree(depth) should return a complete binay tree with the given depth"
    // ) {
    //     const auto bin_tree = lib::topology::complete_binary_tree<graph_type>(constants::three);

    //     const auto expected_n_vertices =
    //         lib::util::upow_sum(constants::two, constants::zero, constants::two);
    //     const auto expected_n_connections = expected_n_vertices - constants::one;
    //     verify_graph_size(bin_tree, expected_n_vertices, expected_n_connections);

    //     CHECK(std::ranges::all_of(
    //         bin_tree.vertices(),
    //         predicate::is_connected_to_binary_chlidren<graph_type, true>(bin_tree)
    //     ));
    // }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    undirected_graph_type_template,
    lib::graph<lib::list_graph_traits<lib::undirected_t>>, // adjacency list
    lib::graph<lib::matrix_graph_traits<lib::undirected_t>> // adjacency matrix
);

TEST_SUITE_END(); // test_graph_topology_builders

} // namespace gl_testing
