#include "gl/directional_tags.hpp"
#include "gl/graph_traits.hpp"
#include "gl/types/core.hpp"
#include "testing/gl/constants.hpp"
#include "testing/gl/functional.hpp"
#include "testing/gl/types.hpp"

#include <gl/graph.hpp>
#include <gl/util/ranges.hpp>

#include <doctest.h>

#include <algorithm>
#include <cstddef>
#include <ranges>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph");

template <
    gl::traits::c_instantiation_of<gl::graph_traits> TraitsType,
    gl::traits::c_properties VertexProperties>
using add_vertex_property = gl::graph_traits<
    typename TraitsType::directional_tag,
    VertexProperties,
    typename TraitsType::edge_properties_type,
    typename TraitsType::implementation_tag>;

template <
    gl::traits::c_instantiation_of<gl::graph_traits> TraitsType,
    gl::traits::c_properties EdgeProperties>
using add_edge_property = gl::graph_traits<
    typename TraitsType::directional_tag,
    typename TraitsType::vertex_properties_type,
    EdgeProperties,
    typename TraitsType::implementation_tag>;

template <
    gl::traits::c_instantiation_of<gl::graph_traits> TraitsType,
    gl::traits::c_properties Properties>
using add_properties = gl::graph_traits<
    typename TraitsType::directional_tag,
    Properties,
    Properties,
    typename TraitsType::implementation_tag>;

template <typename TraitsType>
struct test_graph {
    using traits_type = TraitsType;
    using sut_type = gl::graph<traits_type>;
    using vertex_type = typename sut_type::vertex_type;

    template <gl::traits::c_instantiation_of<gl::graph> GraphType>
    requires(gl::traits::c_directed_graph<GraphType>)
    void init_complete_graph(GraphType& graph) {
        const auto vertices = graph.vertices();
        for (const auto first : vertices)
            for (const auto second : vertices)
                if (first != second)
                    graph.add_edge(first, second);

        const gl::size_type n_unique_edges_in_full_graph =
            n_incident_edges_for_fully_connected_vertex(graph) * graph.order();

        REQUIRE_EQ(graph.size(), n_unique_edges_in_full_graph);
        validate_full_graph_edges(graph);
    }

    template <gl::traits::c_instantiation_of<gl::graph> GraphType>
    requires(gl::traits::c_undirected_graph<GraphType>)
    void init_complete_graph(GraphType& graph) {
        const auto vertices = graph.vertices();
        for (const auto first : vertices)
            for (const auto second : vertices)
                if (first < second)
                    graph.add_edge(first, second);

        const gl::size_type n_unique_edges_in_full_graph =
            (n_incident_edges_for_fully_connected_vertex(graph) * graph.order()) / 2;

        REQUIRE_EQ(graph.size(), n_unique_edges_in_full_graph);
        validate_full_graph_edges(graph);
    }

    template <gl::traits::c_instantiation_of<gl::graph> GraphType>
    void validate_full_graph_edges(const GraphType& graph) {
        REQUIRE(std::ranges::all_of(
            graph.vertex_ids(),
            [&graph, expected_n_edges = n_incident_edges_for_fully_connected_vertex(graph)](
                const gl::default_id_type vertex_id
            ) { return static_cast<std::size_t>(gl::util::range_size(graph.adjacent_edges(vertex_id))) == expected_n_edges; }
        ));
    }

    template <gl::traits::c_instantiation_of<gl::graph> GraphType>
    gl::size_type n_incident_edges_for_fully_connected_vertex(const GraphType& graph) {
        return graph.order() - 1uz;
    }

    const vertex_type out_of_range_vertex{constants::out_of_rng_idx};
    const vertex_type invalid_vertex{gl::invalid_id}; // remove?
};

using vertex_id_list = std::vector<gl::default_id_type>;

inline constexpr auto get_id = [](auto&& element) -> gl::default_id_type { return element.id(); };

TEST_CASE_TEMPLATE_DEFINE("graph structure tests", TraitsType, graph_traits_template) {
    using fixture_type = test_graph<TraitsType>;
    using sut_type = typename fixture_type::sut_type;
    using traits_type = typename fixture_type::traits_type;
    using vertex_type = typename sut_type::vertex_type;
    using edge_type = typename sut_type::edge_type;

    fixture_type fixture;

    // --- general tests ---

    SUBCASE("graph should be initialized with no vertices and no edges by default") {
        sut_type sut;

        CHECK_EQ(sut.order(), 0uz);
        CHECK_EQ(sut.size(), 0uz);
    }

    SUBCASE("graph constructed with n_vertices parameter should contain n_vertices vertices and no "
            "edges") {
        sut_type sut{constants::n_elements};

        REQUIRE(std::ranges::equal(
            sut.vertices() | std::views::transform(get_id), constants::vertex_id_view
        ));

        REQUIRE(std::ranges::equal(sut.vertex_ids(), constants::vertex_id_view));

        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::out_of_rng_idx)), std::out_of_range
        );

        CHECK(std::ranges::all_of(
            constants::vertex_id_view,
            [&sut](const gl::default_id_type vertex_id) {
                return sut.adjacent_edges(vertex_id).empty();
            }
        ));
    }

    // --- vertex method tests ---

    SUBCASE("add_vertex should return a vertex_descriptor with an incremented id and no edges") {
        sut_type sut;

        constexpr gl::size_type target_n_vertices = constants::n_elements;
        for (auto v_id = 0u; v_id < target_n_vertices; v_id++) {
            const auto vertex = sut.add_vertex();
            CHECK_EQ(vertex.id(), v_id);
            CHECK_EQ(sut.order(), v_id + 1u);
            CHECK(sut.adjacent_edges(v_id).empty());
        }

        CHECK_EQ(sut.order(), target_n_vertices);
    }

    SUBCASE("add_vertex_with should initialize a new vertex with the input properties structure") {
        using properties_traits_type = add_vertex_property<traits_type, visited_property>;
        gl::graph<properties_traits_type> sut;

        const auto vertex = sut.add_vertex_with(constants::visited);
        REQUIRE_EQ(sut.order(), 1uz);

        CHECK_EQ(vertex.id(), constants::v1_id);
        CHECK_EQ(vertex.properties(), constants::visited);
    }

    SUBCASE("add_vertices(n) should properly extend the current adjacency list") {
        sut_type sut{};
        sut.add_vertices(constants::n_elements);

        CHECK_EQ(sut.order(), constants::n_elements);
        CHECK_EQ(sut.size(), 0uz);
    }

    SUBCASE("add_vertices_with should properly extend the current adjacency list with the given "
            "properties") {
        using properties_traits_type = add_vertex_property<traits_type, visited_property>;
        gl::graph<properties_traits_type> sut;

        const std::vector<visited_property> properties_list{
            constants::visited, constants::not_visited, constants::visited
        };
        const auto expected_n_vertices = properties_list.size();

        sut.add_vertices_with(properties_list);

        REQUIRE_EQ(sut.order(), expected_n_vertices);
        CHECK_EQ(sut.size(), 0uz);

        CHECK(std::ranges::equal(
            sut.vertices(),
            properties_list,
            std::ranges::equal_to{},
            [](const auto vertex) { return vertex.properties(); }
        ));
    }

    SUBCASE("has_vertex(id) should return true when a vertex with the given id is present in "
            "the graph") {
        sut_type sut{constants::n_elements};

        CHECK(std::ranges::all_of(constants::vertex_id_view, [&sut](const auto vertex_id) {
            return sut.has_vertex(vertex_id);
        }));
        CHECK_FALSE(sut.has_vertex(constants::out_of_rng_idx));
    }

    SUBCASE("get_vertex should throw if the given id is invalid") {
        sut_type sut{constants::n_elements};
        CHECK_THROWS_AS(static_cast<void>(sut.get_vertex(static_cast<gl::default_id_type>(sut.order()))), std::out_of_range);
    }

    SUBCASE("get_vertex should return a vertex with the given id") {
        sut_type sut;
        const auto added_vertex = sut.add_vertex();
        CHECK_EQ(sut.get_vertex(added_vertex.id()), added_vertex);
    }

    SUBCASE("vertices should return the correct vertex collection view") {
        sut_type sut{constants::n_elements};

        CHECK(std::ranges::equal(
            sut.vertices(), constants::vertex_id_view, std::ranges::equal_to{}, get_id
        ));
    }

    SUBCASE("vertex_ids should return a correct view") {
        sut_type sut{constants::n_elements};
        CHECK(std::ranges::equal(sut.vertex_ids(), constants::vertex_id_view));
    }

    SUBCASE("remove_vertex(vertex) should throw if the id of the given is invalid") {
        sut_type sut{constants::n_elements};
        CHECK_THROWS_AS(sut.remove_vertex(fixture.out_of_range_vertex), std::out_of_range);
    }

    SUBCASE("remove_vertex(vertex) should remove the given vertex and align ids of remaining "
            "vertices") {
        sut_type sut{constants::n_elements};
        fixture.init_complete_graph(sut);

        sut.remove_vertex(constants::v1_id);

        constexpr gl::size_type n_vertices_after_remove = constants::n_elements - 1uz;
        const auto expected_n_incident_edges =
            fixture.n_incident_edges_for_fully_connected_vertex(sut);

        const auto vertex_id_view = sut.vertex_ids();

        REQUIRE(std::ranges::equal(
            vertex_id_view, std::views::iota(constants::v1_id, n_vertices_after_remove)
        ));
        REQUIRE(std::ranges::all_of(
            vertex_id_view,
            [&sut, expected_n_incident_edges](const gl::default_id_type vertex_id) {
                return static_cast<std::size_t>(gl::util::range_size(sut.adjacent_edges(vertex_id)))
                    == expected_n_incident_edges;
            }
        ));

        CHECK_THROWS_AS(discard_result(sut.get_vertex(n_vertices_after_remove)), std::out_of_range);
    }

    SUBCASE("remove_vertex(id) should throw if the given id is invalid") {
        sut_type sut{constants::n_elements};
        CHECK_THROWS_AS(sut.remove_vertex(constants::out_of_rng_idx), std::out_of_range);
    }

    SUBCASE("remove_vertex(id) should remove the given vertex and align ids of remaining vertices"
    ) {
        sut_type sut{constants::n_elements};
        fixture.init_complete_graph(sut);

        sut.remove_vertex(constants::v1_id);

        constexpr gl::size_type n_vertices_after_remove = constants::n_elements - 1uz;
        const auto expected_n_incident_edges =
            fixture.n_incident_edges_for_fully_connected_vertex(sut);

        const auto vertex_id_view = sut.vertex_ids();

        REQUIRE(std::ranges::equal(
            vertex_id_view, std::views::iota(constants::v1_id, n_vertices_after_remove)
        ));
        REQUIRE(std::ranges::all_of(
            vertex_id_view,
            [&sut, expected_n_incident_edges](const gl::default_id_type vertex_id) {
                return static_cast<std::size_t>(gl::util::range_size(sut.adjacent_edges(vertex_id)))
                    == expected_n_incident_edges;
            }
        ));

        CHECK_THROWS_AS(discard_result(sut.get_vertex(n_vertices_after_remove)), std::out_of_range);
    }

    SUBCASE("remove_vetices_from(ids) should properly remove elements at given indices (ignoring "
            "duplicate indices)") {
        constexpr auto n_vertices = constants::n_elements + 1uz;

        sut_type sut{n_vertices};
        fixture.init_complete_graph(sut);

        sut.remove_vertices_from(
            vertex_id_list{constants::v1_id, constants::v3_id, constants::v1_id}
        );

        constexpr auto expected_n_vertices = n_vertices - 2uz;
        REQUIRE_EQ(sut.order(), expected_n_vertices);

        constexpr auto expected_n_adjacent_edges = expected_n_vertices - 1uz;
        CHECK(std::ranges::all_of(
            sut.vertices(),
            [&sut, expected_n_adjacent_edges](const auto& vertex) {
                return gl::util::range_size(sut.adjacent_edges(vertex))
                    == expected_n_adjacent_edges;
            }
        ));
    }

    SUBCASE("remove_vetices_from(vertices) should properly remove elements at given indices "
            "(ignoring duplicate vertex references)") {
        constexpr auto n_vertices = constants::n_elements + 1uz;

        sut_type sut{n_vertices};
        fixture.init_complete_graph(sut);

        const auto v1 = sut.get_vertex(constants::v1_id);
        const auto v3 = sut.get_vertex(constants::v3_id);

        sut.remove_vertices_from(std::vector<vertex_type>{v1, v3, v1});

        constexpr auto expected_n_vertices = n_vertices - 2uz;
        REQUIRE_EQ(sut.order(), expected_n_vertices);

        constexpr auto expected_n_adjacent_edges = expected_n_vertices - 1uz;
        CHECK(std::ranges::all_of(
            sut.vertices(),
            [&sut, expected_n_adjacent_edges](const auto& vertex) {
                return gl::util::range_size(sut.adjacent_edges(vertex))
                    == expected_n_adjacent_edges;
            }
        ));
    }

    // --- edge method tests ---

    SUBCASE("edge method tests for default properties type") {
        sut_type sut{constants::n_elements};

        const auto vertices = sut.vertices();
        const auto vertex_1 = vertices[constants::v1_id];
        const auto vertex_2 = vertices[constants::v2_id];
        const auto vertex_3 = vertices[constants::v3_id];

        SUBCASE("add_edge(ids) should throw if either vertex id is invalid") {
            CHECK_THROWS_AS(
                sut.add_edge(constants::out_of_rng_idx, constants::v2_id), std::out_of_range
            );
            CHECK_THROWS_AS(
                sut.add_edge(constants::v1_id, constants::out_of_rng_idx), std::out_of_range
            );
        }

        SUBCASE("add_edge(ids) should properly add the new edge") {
            const auto new_edge = sut.add_edge(constants::v1_id, constants::v2_id);
            REQUIRE(new_edge.is_incident_from(constants::v1_id));
            REQUIRE(new_edge.is_incident_to(constants::v2_id));

            REQUIRE_EQ(sut.size(), 1uz);

            auto adjacent_edges_1 = sut.adjacent_edges(constants::v1_id);
            CHECK_EQ(gl::util::range_size(adjacent_edges_1), 1uz);
            const auto new_edge_extracted_1 = *std::ranges::begin(adjacent_edges_1);
            CHECK_EQ(new_edge_extracted_1, new_edge);

            auto adjacent_edges_2 = sut.adjacent_edges(constants::v2_id);
            if constexpr (gl::traits::c_undirected_edge<edge_type>) {
                CHECK_EQ(gl::util::range_size(adjacent_edges_2), 1uz);
                const auto new_edge_extracted_2 = *std::ranges::begin(adjacent_edges_2);
                CHECK_EQ(new_edge_extracted_2, new_edge);
            }
            else {
                CHECK_EQ(gl::util::range_size(adjacent_edges_2), 0uz);
            }
        }

        SUBCASE("add_edge(vertices) should throw if either vertex is invalid") {
            CHECK_THROWS_AS(sut.add_edge(fixture.out_of_range_vertex, vertex_2), std::out_of_range);
            CHECK_THROWS_AS(sut.add_edge(vertex_1, fixture.out_of_range_vertex), std::out_of_range);
        }

        SUBCASE("add_edge(vertices) should properly add the new edge") {
            const auto new_edge = sut.add_edge(vertex_1, vertex_2);
            REQUIRE(new_edge.is_incident_from(vertex_1.id()));
            REQUIRE(new_edge.is_incident_to(vertex_2.id()));

            REQUIRE_EQ(sut.size(), 1uz);

            auto adjacent_edges_1 = sut.adjacent_edges(constants::v1_id);
            CHECK_EQ(gl::util::range_size(adjacent_edges_1), 1uz);
            const auto new_edge_extracted_1 = *std::ranges::begin(adjacent_edges_1);
            CHECK_EQ(new_edge_extracted_1, new_edge);

            auto adjacent_edges_2 = sut.adjacent_edges(constants::v2_id);
            if constexpr (gl::traits::c_undirected_edge<edge_type>) {
                CHECK_EQ(gl::util::range_size(adjacent_edges_2), 1uz);
                const auto new_edge_extracted_2 = *std::ranges::begin(adjacent_edges_2);
                CHECK_EQ(new_edge_extracted_2, new_edge);
            }
            else {
                CHECK_EQ(gl::util::range_size(adjacent_edges_2), 0uz);
            }
        }

        SUBCASE("add_edges_from(ids) should throw if any id is invalid and not extend the graph") {
            REQUIRE_EQ(sut.size(), 0uz);

            CHECK_THROWS_AS(
                sut.add_edges_from(constants::out_of_rng_idx, vertex_id_list{}), std::out_of_range
            );
            CHECK_EQ(sut.size(), 0uz);

            CHECK_THROWS_AS(
                sut.add_edges_from(
                    constants::v1_id, vertex_id_list{constants::v2_id, constants::out_of_rng_idx}
                ),
                std::out_of_range
            );
            CHECK_EQ(sut.size(), 0uz);
        }

        SUBCASE("add_edges_from(ids) should properly extend the graph if all ids are valid") {
            REQUIRE_EQ(sut.size(), 0uz);

            constexpr auto source_id = constants::v1_id;
            const std::vector<gl::default_id_type> target_id_list{
                constants::v1_id, constants::v2_id, constants::v3_id
            };

            sut.add_edges_from(source_id, target_id_list);

            REQUIRE_EQ(sut.size(), constants::n_elements);
            CHECK(std::ranges::all_of(target_id_list, [&sut, source_id](const auto vertex_id) {
                return sut.has_edge(source_id, vertex_id);
            }));
        }

        SUBCASE("add_edges_from(vertices) should throw if any vertex is invalid and not extend the "
                "graph") {
            REQUIRE_EQ(sut.size(), 0uz);

            CHECK_THROWS_AS(
                sut.add_edges_from(fixture.out_of_range_vertex, std::vector<vertex_type>{}),
                std::out_of_range
            );
            CHECK_EQ(sut.size(), 0uz);

            CHECK_THROWS_AS(
                sut.add_edges_from(
                    vertex_1, std::vector<vertex_type>{vertex_2, fixture.out_of_range_vertex}
                ),
                std::out_of_range
            );
            CHECK_EQ(sut.size(), 0uz);
        }

        SUBCASE("add_edges_from(vertices) should properly extend the graph if all ids are valid") {
            REQUIRE_EQ(sut.size(), 0uz);

            const auto source = vertex_1;
            const std::vector<vertex_type> target_list{vertex_1, vertex_2, vertex_3};

            sut.add_edges_from(source, target_list);

            REQUIRE_EQ(sut.size(), constants::n_elements);
            CHECK(std::ranges::all_of(target_list, [&sut, &source](const auto& vertex) {
                return sut.has_edge(source, vertex);
            }));
        }

        SUBCASE("remove_edge should properly remove the edge for both incident vertices") {
            const auto added_edge = sut.add_edge(vertex_1, vertex_2);

            REQUIRE_EQ(sut.size(), 1uz);

            auto adjacent_edges_1 = sut.adjacent_edges(constants::v1_id);
            auto adjacent_edges_2 = sut.adjacent_edges(constants::v2_id);

            REQUIRE_EQ(gl::util::range_size(adjacent_edges_1), 1uz);
            if constexpr (gl::traits::c_undirected_edge<edge_type>)
                REQUIRE_EQ(gl::util::range_size(adjacent_edges_2), 1uz);

            sut.remove_edge(added_edge);
            CHECK_EQ(sut.size(), 0uz);

            adjacent_edges_1 = sut.adjacent_edges(constants::v1_id);
            adjacent_edges_2 = sut.adjacent_edges(constants::v2_id);

            CHECK_EQ(gl::util::range_size(adjacent_edges_1), 0uz);
            CHECK_EQ(gl::util::range_size(adjacent_edges_2), 0uz);
        }

        SUBCASE("remove_edges should properly erase all given edges") {
            REQUIRE_EQ(sut.size(), 0uz);

            const auto edge_1 = sut.add_edge(vertex_1, vertex_2);
            const auto edge_2 = sut.add_edge(vertex_2, vertex_3);
            const auto edge_3 = sut.add_edge(vertex_3, vertex_1);

            // an additional edge to verify that only the given edges are removed
            const auto vertex_4 = sut.add_vertex();
            const auto edge_4 = sut.add_edge(vertex_1, vertex_4);

            REQUIRE_EQ(sut.size(), constants::n_elements + 1uz);

            std::vector<edge_type> edges_to_remove{edge_1, edge_2, edge_3};

            REQUIRE(std::ranges::all_of(edges_to_remove, [&sut](const auto& edge) {
                return sut.has_edge(edge);
            }));
            REQUIRE(sut.has_edge(edge_4));

            sut.remove_edges(edges_to_remove);

            CHECK_EQ(sut.size(), 1uz);
            CHECK_FALSE(sut.has_edge(vertex_1, vertex_2));
            CHECK_FALSE(sut.has_edge(vertex_2, vertex_3));
            CHECK_FALSE(sut.has_edge(vertex_3, vertex_1));
            CHECK(sut.has_edge(edge_4.source(), edge_4.target()));
        }
    }

    SUBCASE("edge method tests for non-default properties type") {
        using properties_traits_type = add_edge_property<traits_type, used_property>;
        using property_edge_type = typename properties_traits_type::edge_type;
        gl::graph<properties_traits_type> sut{constants::n_elements};

        const auto vertices = sut.vertices();
        const auto vertex_1 = vertices[constants::v1_id];
        const auto vertex_2 = vertices[constants::v2_id];
        const auto vertex_3 = vertices[constants::v3_id];

        SUBCASE("add_edge_with(ids, property) should throw if either vertex id is invalid") {
            CHECK_THROWS_AS(
                sut.add_edge_with(constants::out_of_rng_idx, constants::v2_id, constants::used),
                std::out_of_range
            );
            CHECK_THROWS_AS(
                sut.add_edge_with(constants::v1_id, constants::out_of_rng_idx, constants::used),
                std::out_of_range
            );
        }

        SUBCASE("add_edge_with(ids, property) should properly add the new edge") {
            const auto new_edge =
                sut.add_edge_with(constants::v1_id, constants::v2_id, constants::used);
            REQUIRE(new_edge.is_incident_from(constants::v1_id));
            REQUIRE(new_edge.is_incident_to(constants::v2_id));
            REQUIRE_EQ(new_edge.properties(), constants::used);

            REQUIRE_EQ(sut.size(), 1uz);

            auto adjacent_edges_1 = sut.adjacent_edges(constants::v1_id);
            CHECK_EQ(gl::util::range_size(adjacent_edges_1), 1uz);
            const auto new_edge_extracted_1 = *std::ranges::begin(adjacent_edges_1);
            CHECK_EQ(new_edge_extracted_1, new_edge);

            auto adjacent_edges_2 = sut.adjacent_edges(constants::v2_id);
            if constexpr (gl::traits::c_undirected_edge<edge_type>) {
                CHECK_EQ(gl::util::range_size(adjacent_edges_2), 1uz);
                const auto new_edge_extracted_2 = *std::ranges::begin(adjacent_edges_2);
                CHECK_EQ(new_edge_extracted_2, new_edge);
            }
            else {
                CHECK_EQ(gl::util::range_size(adjacent_edges_2), 0uz);
            }
        }

        SUBCASE("add_edge_with(vertices, property) should throw if either vertex is invalid") {
            CHECK_THROWS_AS(
                sut.add_edge_with(fixture.out_of_range_vertex, vertex_2, constants::used),
                std::out_of_range
            );
            CHECK_THROWS_AS(
                sut.add_edge_with(vertex_1, fixture.out_of_range_vertex, constants::used),
                std::out_of_range
            );
        }

        SUBCASE("add_edge_with(vertices, property) should properly add the new edge") {
            const auto new_edge = sut.add_edge_with(vertex_1, vertex_2, constants::used);
            REQUIRE(new_edge.is_incident_from(vertex_1.id()));
            REQUIRE(new_edge.is_incident_to(vertex_2.id()));
            REQUIRE_EQ(new_edge.properties(), constants::used);

            REQUIRE_EQ(sut.size(), 1uz);

            auto adjacent_edges_1 = sut.adjacent_edges(constants::v1_id);
            CHECK_EQ(gl::util::range_size(adjacent_edges_1), 1uz);
            const auto new_edge_extracted_1 = *std::ranges::begin(adjacent_edges_1);
            CHECK_EQ(new_edge_extracted_1, new_edge);

            auto adjacent_edges_2 = sut.adjacent_edges(constants::v2_id);
            if constexpr (gl::traits::c_undirected_edge<edge_type>) {
                CHECK_EQ(gl::util::range_size(adjacent_edges_2), 1uz);
                const auto new_edge_extracted_2 = *std::ranges::begin(adjacent_edges_2);
                CHECK_EQ(new_edge_extracted_2, new_edge);
            }
            else {
                CHECK_EQ(gl::util::range_size(adjacent_edges_2), 0uz);
            }
        }

        SUBCASE("remove_edge should properly remove the edge for both incident vertices") {
            const auto added_edge = sut.add_edge_with(vertex_1, vertex_2, constants::used);

            REQUIRE_EQ(sut.size(), 1uz);

            auto adjacent_edges_1 = sut.adjacent_edges(constants::v1_id);
            auto adjacent_edges_2 = sut.adjacent_edges(constants::v2_id);

            REQUIRE_EQ(gl::util::range_size(adjacent_edges_1), 1uz);
            if constexpr (gl::traits::c_undirected_edge<edge_type>)
                REQUIRE_EQ(gl::util::range_size(adjacent_edges_2), 1uz);

            sut.remove_edge(added_edge);
            CHECK_EQ(sut.size(), 0uz);

            adjacent_edges_1 = sut.adjacent_edges(constants::v1_id);
            adjacent_edges_2 = sut.adjacent_edges(constants::v2_id);

            CHECK_EQ(gl::util::range_size(adjacent_edges_1), 0uz);
            CHECK_EQ(gl::util::range_size(adjacent_edges_2), 0uz);
        }

        SUBCASE("remove_edges should properly erase all given edges") {
            REQUIRE_EQ(sut.size(), 0uz);

            const auto edge_1 = sut.add_edge_with(vertex_1, vertex_2, constants::not_used);
            const auto edge_2 = sut.add_edge_with(vertex_2, vertex_3, constants::not_used);
            const auto edge_3 = sut.add_edge_with(vertex_3, vertex_1, constants::not_used);

            // an additional edge to verify that only the given edges are removed
            const auto vertex_4 = sut.add_vertex();
            const auto edge_4 = sut.add_edge_with(vertex_1, vertex_4, constants::used);

            REQUIRE_EQ(sut.size(), constants::n_elements + 1uz);

            const std::vector<property_edge_type> edges_to_remove{edge_1, edge_2, edge_3};

            REQUIRE(std::ranges::all_of(edges_to_remove, [&sut](const auto& edge) {
                return sut.has_edge(edge);
            }));
            REQUIRE(sut.has_edge(edge_4));

            sut.remove_edges(edges_to_remove);

            CHECK_EQ(sut.size(), 1uz);
            CHECK_FALSE(sut.has_edge(vertex_1, vertex_2));
            CHECK_FALSE(sut.has_edge(vertex_2, vertex_3));
            CHECK_FALSE(sut.has_edge(vertex_3, vertex_1));
            CHECK(sut.has_edge(edge_4.source(), edge_4.target()));
        }
    }

    SUBCASE("has_edge(vertex, vertex) should throw if one of the vertices is invalid") {
        sut_type sut{constants::n_elements};

        const auto vd_1 = sut.get_vertex(constants::v1_id);
        const auto vd_2 = sut.get_vertex(constants::v2_id);

        CHECK_THROWS_AS(
            discard_result(sut.has_edge(fixture.out_of_range_vertex, vd_2)), std::out_of_range
        );
        CHECK_THROWS_AS(
            discard_result(sut.has_edge(vd_1, fixture.out_of_range_vertex)), std::out_of_range
        );
    }

    SUBCASE("has_edge(vertex, vertex) should return true if there is an edge connecting the given "
            "vertices in the graph") {
        sut_type sut{constants::n_elements};

        const auto vd_1 = sut.get_vertex(constants::v1_id);
        const auto vd_2 = sut.get_vertex(constants::v2_id);
        const auto vd_3 = sut.get_vertex(constants::v3_id);

        sut.add_edge(vd_1, vd_2);

        CHECK(sut.has_edge(vd_1, vd_2));
        CHECK_FALSE(sut.has_edge(vd_1, vd_3));
        CHECK_FALSE(sut.has_edge(vd_2, vd_3));
    }

    SUBCASE("get_edge(vertex, vertex) should throw if either vertex is invalid") {
        sut_type sut{constants::n_elements};
        const auto valid_vertex = sut.get_vertex(constants::v1_id);

        const vertex_type out_of_range_vertex{constants::out_of_rng_idx};
        CHECK_THROWS_AS(
            discard_result(sut.get_edge(valid_vertex, out_of_range_vertex)), std::out_of_range
        );
        CHECK_THROWS_AS(
            discard_result(sut.get_edge(out_of_range_vertex, valid_vertex)), std::out_of_range
        );
        CHECK_THROWS_AS(
            discard_result(sut.get_edge(out_of_range_vertex, out_of_range_vertex)),
            std::out_of_range
        );
    }

    SUBCASE("get_edge(vertex, vertex) should return nullopt if the given vertices are not incident"
    ) {
        sut_type sut{constants::n_elements};
        CHECK_FALSE(sut.get_edge(sut.get_vertex(constants::v1_id), sut.get_vertex(constants::v2_id))
        );
    }

    SUBCASE("get_edge(vertex, vertex) should return a valid edge if the given vetices are incident"
    ) {
        sut_type sut{constants::n_elements};
        const auto vd_1 = sut.get_vertex(constants::v1_id);
        const auto vd_2 = sut.get_vertex(constants::v2_id);

        const auto edge = sut.add_edge(vd_1, vd_2);

        const auto edge_opt_1 = sut.get_edge(vd_1, vd_2);
        REQUIRE(edge_opt_1.has_value());
        CHECK_EQ(*edge_opt_1, edge);

        if constexpr (gl::traits::c_undirected_edge<edge_type>) {
            const auto edge_opt_2 = sut.get_edge(vd_2, vd_1);
            REQUIRE(edge_opt_2.has_value());
            CHECK_EQ(*edge_opt_2, edge);
        }
        else {
            CHECK_FALSE(sut.get_edge(vd_2, vd_1).has_value());
        }
    }

    SUBCASE("get_edges(id, id) should return an empty list if either id is invalid") {
        sut_type sut{constants::n_elements};

        CHECK_THROWS_AS(
            discard_result(sut.get_edges(constants::out_of_rng_idx, constants::v2_id)),
            std::out_of_range
        );
        CHECK_THROWS_AS(
            discard_result(sut.get_edges(constants::v1_id, constants::out_of_rng_idx)),
            std::out_of_range
        );
    }

    SUBCASE("get_edges(id, id) should return an empty vector if the given vertices are not incident"
    ) {
        sut_type sut{constants::n_elements};
        CHECK(sut.get_edges(constants::v1_id, constants::v2_id).empty());
    }

    SUBCASE("get_edges(id, id) should return a valid edge reference vector if the given vertices "
            "are incident") {
        sut_type sut{constants::n_elements};
        std::vector<edge_type> expected_edges;

        if constexpr (std::same_as<typename sut_type::implementation_tag, gl::impl::list_t>) {
            for (auto _ = 0uz; _ < constants::n_elements; _++)
                expected_edges.emplace_back(sut.add_edge(constants::v1_id, constants::v2_id));
        }
        else {
            expected_edges.emplace_back(sut.add_edge(constants::v1_id, constants::v2_id));
        }

        CHECK(std::ranges::equal(sut.get_edges(constants::v1_id, constants::v2_id), expected_edges)
        );

        if constexpr (gl::traits::c_directed_edge<edge_type>) {
            CHECK(sut.get_edges(constants::v2_id, constants::v2_id).empty());
        }
        else {
            CHECK(std::ranges::equal(
                sut.get_edges(constants::v2_id, constants::v1_id), expected_edges
            ));
        }
    }

    SUBCASE("get_edges(vertex, vertex) should throw if either vertex is invalid") {
        sut_type sut{constants::n_elements};

        const auto vd_1 = sut.get_vertex(constants::v1_id);
        const auto vd_2 = sut.get_vertex(constants::v2_id);

        CHECK_THROWS_AS(
            discard_result(sut.get_edges(fixture.out_of_range_vertex, vd_2)), std::out_of_range
        );
        CHECK_THROWS_AS(
            discard_result(sut.get_edges(vd_1, fixture.out_of_range_vertex)), std::out_of_range
        );
    }

    SUBCASE("adjacent_edges(id) should throw if the vertex_id is invalid") {
        sut_type sut{constants::n_elements};
        CHECK_THROWS_AS(
            discard_result(sut.adjacent_edges(constants::out_of_rng_idx)), std::out_of_range
        );
    }

    SUBCASE("adjacent_edges(id) should return a proper iterator range for a valid vertex") {
        sut_type sut{1uz};

        CHECK_NOTHROW([&sut]() { CHECK_EQ(gl::util::range_size(sut.adjacent_edges(0uz)), 0uz); }());
    }

    SUBCASE("adjacent_edges(vertex) should throw if the vertex is invalid") {
        sut_type sut{constants::n_elements};

        CHECK_THROWS_AS(
            discard_result(sut.adjacent_edges(fixture.out_of_range_vertex)), std::out_of_range
        );
    }

    SUBCASE("adjacent_edges(vertex) should return a proper iterator range for a valid vertex") {
        sut_type sut{1uz};
        const auto vertex = sut.get_vertex(0uz);

        CHECK_NOTHROW([&sut, &vertex]() {
            CHECK_EQ(gl::util::range_size(sut.adjacent_edges(vertex)), 0uz);
        }());
    }

    // --- comparison and cloning ---

    const auto set_properties = [](auto& sut) {
        for (auto vertex : sut.vertices())
            vertex.properties() = std::format("vertex_{}", vertex.id());

        for (auto&& [id, edge_property] : std::views::enumerate(sut.edge_properties_map()))
            edge_property = std::format("edge_{}", id);
    };

    using p_graph_traits = add_properties<TraitsType, gl::name_property>;
    using p_sut_type = gl::graph<p_graph_traits>;

    const auto create_test_p_hypergraph = [&set_properties]() {
        p_sut_type sut(constants::n_elements);
        sut.add_edge(constants::v1_id, constants::v2_id);
        sut.add_edge(constants::v2_id, constants::v3_id);
        set_properties(sut);
        return sut;
    };

    SUBCASE("equality operator should properly compare graphs") {
        auto sut1 = create_test_p_hypergraph();
        auto sut2 = create_test_p_hypergraph();

        SUBCASE("identical graphs are equal") {
            CHECK_EQ(sut1, sut2);
        }

        SUBCASE("graphs with different orders are not equal") {
            sut2.add_vertex();
            CHECK_NE(sut1, sut2);
        }

        SUBCASE("graphs with different connections are not equal") {
            sut2.add_edge(constants::v1_id, constants::v3_id);
            CHECK_NE(sut1, sut2);
        }

        SUBCASE("graphs with different vertex properties are not equal") {
            sut2.get_vertex_properties(0uz) = "dummy";
            CHECK_NE(sut1, sut2);
        }

        SUBCASE("hypergraph with different hyperedge properties are not equal") {
            sut2.get_edge_properties(0uz) = "dummy";
            CHECK_NE(sut1, sut2);
        }
    }

    SUBCASE("clone should return an exact copy of the source graph") {
        const auto sut1 = create_test_p_hypergraph();
        const auto sut2 = gl::clone(sut1);
        CHECK_EQ(sut1, sut2);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    graph_traits_template,
    gl::list_graph_traits<gl::directed_t>, // directed adjacency list
    gl::list_graph_traits<gl::undirected_t>, // undirected adjacency list
    gl::flat_list_graph_traits<gl::directed_t>, // directed flat adjacency list
    gl::flat_list_graph_traits<gl::undirected_t>, // undirected flat adjacency list
    gl::matrix_graph_traits<gl::directed_t>, // directed adjacency matrix
    gl::matrix_graph_traits<gl::undirected_t>, // undirected adjacency matrix
    gl::matrix_graph_traits<gl::directed_t>, // directed adj matrix
    gl::matrix_graph_traits<gl::undirected_t> // undirected adj matrix
);

TEST_CASE_TEMPLATE_DEFINE("properties getter tests", TraitsType, property_graph_traits_template) {
    using sut_type = gl::graph<TraitsType>;

    sut_type sut{constants::n_elements};
    for (auto vertex : sut.vertices()) {
        vertex.properties() = std::format("vertex_{}", vertex.id());
        const auto target_id = static_cast<gl::default_id_type>((vertex.id() + 1uz) % constants::n_elements);
        sut.add_edge(vertex.id(), target_id).properties() = std::format("edge_{}", vertex.id());
    }

    auto vmap = sut.vertex_properties_map();
    CHECK(vmap.size() == constants::n_elements);
    for (auto [id, property] : std::views::zip(sut.vertex_ids(), vmap)) {
        CHECK_EQ(property, std::format("vertex_{}", id));
        CHECK_EQ(vmap[id], std::format("vertex_{}", id));
        CHECK_EQ(sut.get_vertex_properties(id), std::format("vertex_{}", id));
    }

    CHECK_THROWS_AS(
        discard_result(sut.get_vertex_properties(constants::out_of_rng_idx)), std::out_of_range
    );

    auto emap = sut.edge_properties_map();
    CHECK(emap.size() == constants::n_elements);
    for (auto [id, property] : std::views::enumerate(emap)) {
        CHECK_EQ(property, std::format("edge_{}", id));
        CHECK_EQ(emap[id], std::format("edge_{}", id));
        CHECK_EQ(sut.get_edge_properties(static_cast<gl::default_id_type>(id)), std::format("edge_{}", id));
    }

    CHECK_THROWS_AS(
        discard_result(sut.get_edge_properties(constants::out_of_rng_idx)), std::out_of_range
    );
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    property_graph_traits_template,
    gl::list_graph_traits<
        gl::directed_t,
        gl::name_property,
        gl::name_property>, // directed adjacency list
    gl::list_graph_traits<
        gl::undirected_t,
        gl::name_property,
        gl::name_property>, // undirected adjacency list
    gl::flat_list_graph_traits<
        gl::directed_t,
        gl::name_property,
        gl::name_property>, // directed flat adjacency list
    gl::flat_list_graph_traits<
        gl::undirected_t,
        gl::name_property,
        gl::name_property>, // undirected flat adjacency list
    gl::matrix_graph_traits<
        gl::directed_t,
        gl::name_property,
        gl::name_property>, // directed adjacency matrix
    gl::matrix_graph_traits<
        gl::undirected_t,
        gl::name_property,
        gl::name_property>, // undirected adjacency matrix
    gl::flat_matrix_graph_traits<
        gl::directed_t,
        gl::name_property,
        gl::name_property>, // directed flat adjacency matrix
    gl::flat_matrix_graph_traits<
        gl::undirected_t,
        gl::name_property,
        gl::name_property> // undirected flat adjacency matrix
);

TEST_SUITE_END(); // test_graph

} // namespace gl_testing
