#include "constants.hpp"
#include "functional.hpp"
#include "transforms.hpp"
#include "types.hpp"

#include <gl/graph.hpp>

#include <doctest.h>

#include <algorithm>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph");

template <typename TraitsType>
struct test_graph {
    using traits_type = TraitsType;
    using sut_type = lib::graph<traits_type>;
    using vertex_type = typename sut_type::vertex_type;

    template <lib_tt::c_instantiation_of<lib::graph> GraphType>
    requires(lib_tt::is_directed_v<typename GraphType::edge_type>)
    void initialize_full_graph(GraphType& graph) {
        const auto vertices = graph.vertices();
        for (const auto& first : vertices)
            for (const auto& second : vertices)
                if (first != second)
                    graph.add_edge(first, second);

        const lib_t::size_type n_unique_edges_in_full_graph =
            n_incident_edges_for_fully_connected_vertex(graph) * graph.n_vertices();

        REQUIRE_EQ(graph.n_unique_edges(), n_unique_edges_in_full_graph);
        validate_full_graph_edges(graph);
    }

    template <lib_tt::c_instantiation_of<lib::graph> GraphType>
    requires(lib_tt::is_undirected_v<typename GraphType::edge_type>)
    void initialize_full_graph(GraphType& graph) {
        const auto vertices = graph.vertices();
        for (const auto& first : vertices)
            for (const auto& second : vertices)
                if (first < second)
                    graph.add_edge(first, second);

        const lib_t::size_type n_unique_edges_in_full_graph =
            (n_incident_edges_for_fully_connected_vertex(graph) * graph.n_vertices()) / 2;

        REQUIRE_EQ(graph.n_unique_edges(), n_unique_edges_in_full_graph);
        validate_full_graph_edges(graph);
    }

    template <lib_tt::c_instantiation_of<lib::graph> GraphType>
    void validate_full_graph_edges(const GraphType& graph) {
        REQUIRE(std::ranges::all_of(
            graph.vertex_ids(),
            [this, &graph, expected_n_edges = n_incident_edges_for_fully_connected_vertex(graph)](
                const lib_t::id_type vertex_id
            ) { return graph.adjacent_edges(vertex_id).distance() == expected_n_edges; }
        ));
    }

    template <lib_tt::c_instantiation_of<lib::graph> GraphType>
    lib_t::size_type n_incident_edges_for_fully_connected_vertex(const GraphType& graph) {
        return graph.n_vertices() - constants::one_element;
    }

    const vertex_type out_of_range_vertex{constants::out_of_range_elemenet_idx};
    const vertex_type invalid_vertex{constants::vertex_id_1};
};

template <
    lib_tt::c_instantiation_of<lib::graph_traits> TraitsType,
    lib_tt::c_properties VertexProperties>
using add_vertex_property = lib::graph_traits<
    typename TraitsType::edge_directional_tag,
    VertexProperties,
    typename TraitsType::edge_properties_type,
    typename TraitsType::implementation_tag>;

template <
    lib_tt::c_instantiation_of<lib::graph_traits> TraitsType,
    lib_tt::c_properties EdgeProperties>
using add_edge_property = lib::graph_traits<
    typename TraitsType::edge_directional_tag,
    typename TraitsType::vertex_properties_type,
    EdgeProperties,
    typename TraitsType::implementation_tag>;

using vertex_id_list = std::vector<lib_t::id_type>;

template <lib_tt::c_instantiation_of<lib::vertex_descriptor> VertexType>
using vertex_ref_list = std::vector<lib_t::const_ref_wrap<VertexType>>;

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

        CHECK_EQ(sut.n_vertices(), constants::zero_elements);
        CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);
    }

    SUBCASE("graph constructed with n_vertices parameter should properly initialize the vertex "
            "list with no edges") {
        sut_type sut{constants::n_elements};

        REQUIRE(std::ranges::equal(
            sut.vertices() | std::views::transform(transforms::extract_vertex_id<>),
            constants::vertex_id_view
        ));

        REQUIRE(std::ranges::equal(sut.vertex_ids(), constants::vertex_id_view));

        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::out_of_range_elemenet_idx)),
            std::out_of_range
        );

        CHECK(std::ranges::all_of(constants::vertex_id_view, [&sut](const lib_t::id_type vertex_id) {
            return not static_cast<bool>(sut.adjacent_edges(vertex_id).distance());
        }));
    }

    // --- vertex method tests ---

    SUBCASE("add_vertex should return a vertex_descriptor with an incremented id and no edges") {
        sut_type sut;

        constexpr lib_t::size_type target_n_vertices = constants::n_elements;
        for (lib_t::id_type v_id = constants::zero_elements; v_id < target_n_vertices; v_id++) {
            const auto& vertex = sut.add_vertex();
            CHECK_EQ(vertex.id(), v_id);
            CHECK_EQ(sut.n_vertices(), v_id + constants::one_element);
            CHECK_EQ(sut.adjacent_edges(v_id).distance(), constants::empty_distance);
        }

        CHECK_EQ(sut.n_vertices(), target_n_vertices);
    }

    SUBCASE("add_vertex should initialize a new vertex with the input properties structure") {
        using properties_traits_type = add_vertex_property<traits_type, types::visited_property>;
        lib::graph<properties_traits_type> sut;

        const auto& vertex = sut.add_vertex(constants::visited);
        REQUIRE_EQ(sut.n_vertices(), constants::one_element);

        CHECK_EQ(vertex.id(), constants::vertex_id_1);
        CHECK_EQ(vertex.properties, constants::visited);
    }

    SUBCASE("add_vertices(n) should properly extend the current adjacency list") {
        sut_type sut{};
        sut.add_vertices(constants::n_elements);

        CHECK_EQ(sut.n_vertices(), constants::n_elements);
        CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);
    }

    SUBCASE("add_vertices_with should properly extend the current adjacency list with the given "
            "properties") {
        using properties_traits_type = add_vertex_property<traits_type, types::visited_property>;
        lib::graph<properties_traits_type> sut;

        const std::vector<types::visited_property> properties_list{
            constants::visited, constants::not_visited, constants::visited
        };
        const auto expected_n_vertices = properties_list.size();

        sut.add_vertices_with(properties_list);

        REQUIRE_EQ(sut.n_vertices(), expected_n_vertices);
        CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);

        CHECK(std::ranges::equal(
            sut.vertices(),
            properties_list,
            std::ranges::equal_to{},
            [](const auto& vertex) { return vertex.properties; }
        ));
    }

    SUBCASE("has_vertex(id) should return true when a vertex with the given id is present in "
            "the graph") {
        sut_type sut{constants::n_elements};

        CHECK(std::ranges::all_of(constants::vertex_id_view, [&sut](const auto vertex_id) {
            return sut.has_vertex(vertex_id);
        }));
        CHECK_FALSE(sut.has_vertex(constants::out_of_range_elemenet_idx));
    }

    SUBCASE("has_vertex(vertex) should return true when a vertex with the given id is present in "
            "the graph") {
        sut_type sut{constants::n_elements};

        CHECK(std::ranges::all_of(constants::vertex_id_view, [&sut](const auto vertex_id) {
            return sut.has_vertex(sut.get_vertex(vertex_id));
        }));
        CHECK(std::ranges::none_of(constants::vertex_id_view, [&sut](const auto vertex_id) {
            return sut.has_vertex(vertex_type{vertex_id});
        }));
        CHECK_FALSE(sut.has_vertex(fixture.out_of_range_vertex));
    }

    SUBCASE("get_vertex should throw if the given id is invalid") {
        sut_type sut{constants::n_elements};
        CHECK_THROWS_AS(static_cast<void>(sut.get_vertex(sut.n_vertices())), std::out_of_range);
    }

    SUBCASE("get_vertex should return a vertex with the given id") {
        sut_type sut;
        const auto& added_vertex = sut.add_vertex();
        CHECK_EQ(&sut.get_vertex(added_vertex.id()), &added_vertex);
    }

    SUBCASE("vertices should return the correct vertex list iterator range") {
        sut_type sut{constants::n_elements};

        // clang-format off

        CHECK(std::ranges::equal(
            sut.vertices(),
            constants::vertex_id_view,
            std::ranges::equal_to{},
            transforms::extract_vertex_id<vertex_type>
        ));

        // clang-format on
    }

    SUBCASE("vertex_ids should return the correct vertex list iterator range") {
        sut_type sut{constants::n_elements};
        CHECK(std::ranges::equal(sut.vertex_ids(), constants::vertex_id_view));
    }

    SUBCASE("remove_vertex(vertex) should throw if the id of the given is invalid") {
        sut_type sut{constants::n_elements};
        CHECK_THROWS_AS(sut.remove_vertex(fixture.out_of_range_vertex), std::out_of_range);
    }

    SUBCASE("remove_vertex(vertex) should throw if the id is valid but the address is not") {
        sut_type sut{constants::n_elements};
        CHECK_THROWS_AS(sut.remove_vertex(fixture.invalid_vertex), std::invalid_argument);
    }

    SUBCASE("remove_vertex(vertex) should remove the given vertex and align ids of remaining "
            "vertices") {
        sut_type sut{constants::n_elements};
        fixture.initialize_full_graph(sut);

        sut.remove_vertex(constants::vertex_id_1);

        constexpr lib_t::size_type n_vertices_after_remove =
            constants::n_elements - constants::one_element;
        const auto expected_n_incident_edges =
            fixture.n_incident_edges_for_fully_connected_vertex(sut);

        const auto vertex_id_view = sut.vertex_ids();

        REQUIRE(std::ranges::equal(
            vertex_id_view, std::views::iota(constants::vertex_id_1, n_vertices_after_remove)
        ));
        REQUIRE(std::ranges::all_of(
            vertex_id_view,
            [&sut, expected_n_incident_edges](const lib_t::id_type vertex_id) {
                return sut.adjacent_edges(vertex_id).distance() == expected_n_incident_edges;
            }
        ));

        CHECK_THROWS_AS(
            func::discard_result(sut.get_vertex(n_vertices_after_remove)), std::out_of_range
        );
    }

    SUBCASE("remove_vertex(id) should throw if the given id is invalid") {
        sut_type sut{constants::n_elements};
        CHECK_THROWS_AS(sut.remove_vertex(constants::out_of_range_elemenet_idx), std::out_of_range);
    }

    SUBCASE("remove_vertex(id) should remove the given vertex and align ids of remaining vertices"
    ) {
        sut_type sut{constants::n_elements};
        fixture.initialize_full_graph(sut);

        sut.remove_vertex(constants::vertex_id_1);

        constexpr lib_t::size_type n_vertices_after_remove =
            constants::n_elements - constants::one_element;
        const auto expected_n_incident_edges =
            fixture.n_incident_edges_for_fully_connected_vertex(sut);

        const auto vertex_id_view = sut.vertex_ids();

        REQUIRE(std::ranges::equal(
            vertex_id_view, std::views::iota(constants::vertex_id_1, n_vertices_after_remove)
        ));
        REQUIRE(std::ranges::all_of(
            vertex_id_view,
            [&sut, expected_n_incident_edges](const lib_t::id_type vertex_id) {
                return sut.adjacent_edges(vertex_id).distance() == expected_n_incident_edges;
            }
        ));

        CHECK_THROWS_AS(
            func::discard_result(sut.get_vertex(n_vertices_after_remove)), std::out_of_range
        );
    }

    SUBCASE("remove_vetices_from(ids) should properly remove elements at given indices (ignoring "
            "duplicate indices)") {
        constexpr auto n_vertices = constants::n_elements + constants::one_element;

        sut_type sut{n_vertices};
        fixture.initialize_full_graph(sut);

        sut.remove_vertices_from(
            vertex_id_list{constants::vertex_id_1, constants::vertex_id_3, constants::vertex_id_1}
        );

        constexpr auto expected_n_vertices = n_vertices - constants::two;
        REQUIRE_EQ(sut.n_vertices(), expected_n_vertices);

        constexpr auto expected_n_adjacent_edges = expected_n_vertices - constants::one;
        CHECK(std::ranges::all_of(
            sut.vertices(),
            [&sut, expected_n_adjacent_edges](const auto& vertex) {
                return sut.adjacent_edges(vertex).distance() == expected_n_adjacent_edges;
            }
        ));
    }

    SUBCASE("remove_vetices_from(vertices) should properly remove elements at given indices "
            "(ignoring duplicate vertex references)") {
        constexpr auto n_vertices = constants::n_elements + constants::one_element;

        sut_type sut{n_vertices};
        fixture.initialize_full_graph(sut);

        const auto& v1 = sut.get_vertex(constants::vertex_id_1);
        const auto& v3 = sut.get_vertex(constants::vertex_id_3);

        sut.remove_vertices_from(vertex_ref_list<vertex_type>{v1, v3, v1});

        constexpr auto expected_n_vertices = n_vertices - constants::two;
        REQUIRE_EQ(sut.n_vertices(), expected_n_vertices);

        constexpr auto expected_n_adjacent_edges = expected_n_vertices - constants::one;
        CHECK(std::ranges::all_of(
            sut.vertices(),
            [&sut, expected_n_adjacent_edges](const auto& vertex) {
                return sut.adjacent_edges(vertex).distance() == expected_n_adjacent_edges;
            }
        ));
    }

    // --- edge method tests ---

    SUBCASE("edge method tests for default properties type") {
        sut_type sut{constants::n_elements};

        const auto vertices = sut.vertices();
        const auto& vertex_1 = vertices[constants::vertex_id_1];
        const auto& vertex_2 = vertices[constants::vertex_id_2];
        const auto& vertex_3 = vertices[constants::vertex_id_3];

        SUBCASE("add_edge(ids) should throw if either vertex id is invalid") {
            CHECK_THROWS_AS(
                sut.add_edge(constants::out_of_range_elemenet_idx, constants::vertex_id_2),
                std::out_of_range
            );
            CHECK_THROWS_AS(
                sut.add_edge(constants::vertex_id_1, constants::out_of_range_elemenet_idx),
                std::out_of_range
            );
        }

        SUBCASE("add_edge(ids) should properly add the new edge") {
            const auto& new_edge = sut.add_edge(constants::vertex_id_1, constants::vertex_id_2);
            REQUIRE(new_edge.is_incident_from(vertices[constants::vertex_id_1]));
            REQUIRE(new_edge.is_incident_to(vertices[constants::vertex_id_2]));

            REQUIRE_EQ(sut.n_unique_edges(), constants::one_element);

            const auto adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
            CHECK_EQ(adjacent_edges_1.distance(), constants::one_element);
            const auto& new_edge_extracted_1 = adjacent_edges_1[constants::first_element_idx];
            CHECK_EQ(&new_edge_extracted_1, &new_edge);

            const auto adjacent_edges_2 = sut.adjacent_edges(constants::vertex_id_2);
            if constexpr (lib_tt::is_undirected_v<edge_type>) {
                CHECK_EQ(adjacent_edges_2.distance(), constants::one_element);
                const auto& new_edge_extracted_2 = adjacent_edges_2[constants::first_element_idx];
                CHECK_EQ(&new_edge_extracted_2, &new_edge);
            }
            else {
                CHECK_EQ(adjacent_edges_2.distance(), constants::zero_elements);
            }
        }

        SUBCASE("add_edge(vertices) should throw if either vertex is invalid") {
            CHECK_THROWS_AS(sut.add_edge(fixture.out_of_range_vertex, vertex_2), std::out_of_range);
            CHECK_THROWS_AS(sut.add_edge(vertex_1, fixture.out_of_range_vertex), std::out_of_range);

            CHECK_THROWS_AS(sut.add_edge(fixture.invalid_vertex, vertex_2), std::invalid_argument);
            CHECK_THROWS_AS(sut.add_edge(vertex_1, fixture.invalid_vertex), std::invalid_argument);
        }

        SUBCASE("add_edge(vertices) should properly add the new edge") {
            const auto& new_edge = sut.add_edge(vertex_1, vertex_2);
            REQUIRE(new_edge.is_incident_from(vertex_1));
            REQUIRE(new_edge.is_incident_to(vertex_2));

            REQUIRE_EQ(sut.n_unique_edges(), constants::one_element);

            const auto adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
            CHECK_EQ(adjacent_edges_1.distance(), constants::one_element);
            const auto& new_edge_extracted_1 = adjacent_edges_1[constants::first_element_idx];
            CHECK_EQ(&new_edge_extracted_1, &new_edge);

            const auto adjacent_edges_2 = sut.adjacent_edges(constants::vertex_id_2);
            if constexpr (lib_tt::is_undirected_v<edge_type>) {
                CHECK_EQ(adjacent_edges_2.distance(), constants::one_element);
                const auto& new_edge_extracted_2 = adjacent_edges_2[constants::first_element_idx];
                CHECK_EQ(&new_edge_extracted_2, &new_edge);
            }
            else {
                CHECK_EQ(adjacent_edges_2.distance(), constants::zero_elements);
            }
        }

        SUBCASE("add_edges_from(ids) should throw if any id is invalid and not extend the graph") {
            REQUIRE_EQ(sut.n_unique_edges(), constants::zero_elements);

            CHECK_THROWS_AS(
                sut.add_edges_from(constants::out_of_range_elemenet_idx, vertex_id_list{}),
                std::out_of_range
            );
            CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);

            CHECK_THROWS_AS(
                sut.add_edges_from(
                    constants::vertex_id_1,
                    vertex_id_list{constants::vertex_id_2, constants::out_of_range_elemenet_idx}
                ),
                std::out_of_range
            );
            CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);
        }

        SUBCASE("add_edges_from(ids) should properly extend the graph if all ids are valid") {
            REQUIRE_EQ(sut.n_unique_edges(), constants::zero_elements);

            constexpr auto source_id = constants::vertex_id_1;
            const std::vector<lib_t::id_type> target_id_list{
                constants::vertex_id_1, constants::vertex_id_2, constants::vertex_id_3
            };

            sut.add_edges_from(source_id, target_id_list);

            REQUIRE_EQ(sut.n_unique_edges(), constants::n_elements);
            CHECK(std::ranges::all_of(target_id_list, [&sut, source_id](const auto vertex_id) {
                return sut.has_edge(source_id, vertex_id);
            }));
        }

        SUBCASE("add_edges_from(vertices) should throw if any vertex is invalid and not extend the "
                "graph") {
            REQUIRE_EQ(sut.n_unique_edges(), constants::zero_elements);

            CHECK_THROWS_AS(
                sut.add_edges_from(fixture.out_of_range_vertex, vertex_ref_list<vertex_type>{}),
                std::out_of_range
            );
            CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);

            CHECK_THROWS_AS(
                sut.add_edges_from(fixture.invalid_vertex, vertex_ref_list<vertex_type>{}),
                std::invalid_argument
            );
            CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);

            CHECK_THROWS_AS(
                sut.add_edges_from(
                    vertex_1, vertex_ref_list<vertex_type>{vertex_2, fixture.out_of_range_vertex}
                ),
                std::out_of_range
            );
            CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);

            CHECK_THROWS_AS(
                sut.add_edges_from(
                    vertex_1, vertex_ref_list<vertex_type>{vertex_2, fixture.invalid_vertex}
                ),
                std::invalid_argument
            );
            CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);
        }

        SUBCASE("add_edges_from(vertices) should properly extend the graph if all ids are valid") {
            REQUIRE_EQ(sut.n_unique_edges(), constants::zero_elements);

            const auto& source = vertex_1;
            const vertex_ref_list<vertex_type> target_list{vertex_1, vertex_2, vertex_3};

            sut.add_edges_from(source, target_list);

            REQUIRE_EQ(sut.n_unique_edges(), constants::n_elements);
            CHECK(std::ranges::all_of(target_list, [&sut, &source](const auto& vertex) {
                return sut.has_edge(source, vertex);
            }));
        }

        SUBCASE("remove_edge should properly remove the edge for both incident vertices") {
            const auto& added_edge = sut.add_edge(vertex_1, vertex_2);

            REQUIRE_EQ(sut.n_unique_edges(), constants::one_element);

            auto adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
            auto adjacent_edges_2 = sut.adjacent_edges(constants::vertex_id_2);

            REQUIRE_EQ(adjacent_edges_1.distance(), constants::one_element);
            if constexpr (lib_tt::is_undirected_v<edge_type>)
                REQUIRE_EQ(adjacent_edges_2.distance(), constants::one_element);

            sut.remove_edge(added_edge);
            CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);

            adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
            adjacent_edges_2 = sut.adjacent_edges(constants::vertex_id_2);

            CHECK_EQ(adjacent_edges_1.distance(), constants::zero_elements);
            CHECK_EQ(adjacent_edges_2.distance(), constants::zero_elements);
        }

        SUBCASE("remove_edges_from should properly erase all given edges") {
            REQUIRE_EQ(sut.n_unique_edges(), constants::zero_elements);

            const auto& edge_1 = sut.add_edge(vertex_1, vertex_2);
            const auto& edge_2 = sut.add_edge(vertex_2, vertex_3);
            const auto& edge_3 = sut.add_edge(vertex_3, vertex_1);

            // an additional edge to verify that only the given edges are removed
            const auto& vertex_4 = sut.add_vertex();
            const auto& edge_4 = sut.add_edge(vertex_1, vertex_4);

            REQUIRE_EQ(sut.n_unique_edges(), constants::n_elements + constants::one_element);

            std::vector<lib_t::const_ref_wrap<edge_type>> edges_to_remove{edge_1, edge_2, edge_3};

            const auto has_edge = [&sut](const auto& edge_ref) {
                return sut.has_edge(edge_ref.get());
            };

            REQUIRE(std::ranges::all_of(edges_to_remove, has_edge));
            REQUIRE(sut.has_edge(edge_4));

            sut.remove_edges_from(edges_to_remove);

            CHECK_EQ(sut.n_unique_edges(), constants::one_element);
            CHECK_FALSE(sut.has_edge(vertex_1, vertex_2));
            CHECK_FALSE(sut.has_edge(vertex_2, vertex_3));
            CHECK_FALSE(sut.has_edge(vertex_3, vertex_1));
            CHECK(sut.has_edge(edge_4));
        }
    }

    SUBCASE("edge method tests for non-default properties type") {
        using properties_traits_type = add_edge_property<traits_type, types::used_property>;
        using property_edge_type = typename properties_traits_type::edge_type;
        lib::graph<properties_traits_type> sut{constants::n_elements};

        const auto vertices = sut.vertices();
        const auto& vertex_1 = vertices[constants::vertex_id_1];
        const auto& vertex_2 = vertices[constants::vertex_id_2];
        const auto& vertex_3 = vertices[constants::vertex_id_3];

        SUBCASE("add_edge(ids) should throw if either vertex id is invalid") {
            CHECK_THROWS_AS(
                sut.add_edge(
                    constants::out_of_range_elemenet_idx, constants::vertex_id_2, constants::used
                ),
                std::out_of_range
            );
            CHECK_THROWS_AS(
                sut.add_edge(
                    constants::vertex_id_1, constants::out_of_range_elemenet_idx, constants::used
                ),
                std::out_of_range
            );
        }

        SUBCASE("add_edge(ids) should properly add the new edge") {
            const auto& new_edge =
                sut.add_edge(constants::vertex_id_1, constants::vertex_id_2, constants::used);
            REQUIRE(new_edge.is_incident_from(vertices[constants::vertex_id_1]));
            REQUIRE(new_edge.is_incident_to(vertices[constants::vertex_id_2]));
            REQUIRE_EQ(new_edge.properties, constants::used);

            REQUIRE_EQ(sut.n_unique_edges(), constants::one_element);

            const auto adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
            CHECK_EQ(adjacent_edges_1.distance(), constants::one_element);
            const auto& new_edge_extracted_1 = adjacent_edges_1[constants::first_element_idx];
            CHECK_EQ(&new_edge_extracted_1, &new_edge);

            const auto adjacent_edges_2 = sut.adjacent_edges(constants::vertex_id_2);
            if constexpr (lib_tt::is_undirected_v<edge_type>) {
                CHECK_EQ(adjacent_edges_2.distance(), constants::one_element);
                const auto& new_edge_extracted_2 = adjacent_edges_2[constants::first_element_idx];
                CHECK_EQ(&new_edge_extracted_2, &new_edge);
            }
            else {
                CHECK_EQ(adjacent_edges_2.distance(), constants::zero_elements);
            }
        }

        SUBCASE("add_edge(vertices) should throw if either vertex is invalid") {
            CHECK_THROWS_AS(
                sut.add_edge(fixture.out_of_range_vertex, vertex_2, constants::used),
                std::out_of_range
            );
            CHECK_THROWS_AS(
                sut.add_edge(vertex_1, fixture.out_of_range_vertex, constants::used),
                std::out_of_range
            );

            CHECK_THROWS_AS(
                sut.add_edge(fixture.invalid_vertex, vertex_2, constants::used),
                std::invalid_argument
            );
            CHECK_THROWS_AS(
                sut.add_edge(vertex_1, fixture.invalid_vertex, constants::used),
                std::invalid_argument
            );
        }

        SUBCASE("add_edge(vertices) should properly add the new edge") {
            const auto& new_edge = sut.add_edge(vertex_1, vertex_2, constants::used);
            REQUIRE(new_edge.is_incident_from(vertex_1));
            REQUIRE(new_edge.is_incident_to(vertex_2));
            REQUIRE_EQ(new_edge.properties, constants::used);

            REQUIRE_EQ(sut.n_unique_edges(), constants::one_element);

            const auto adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
            CHECK_EQ(adjacent_edges_1.distance(), constants::one_element);
            const auto& new_edge_extracted_1 = adjacent_edges_1[constants::first_element_idx];
            CHECK_EQ(&new_edge_extracted_1, &new_edge);

            const auto adjacent_edges_2 = sut.adjacent_edges(constants::vertex_id_2);
            if constexpr (lib_tt::is_undirected_v<edge_type>) {
                CHECK_EQ(adjacent_edges_2.distance(), constants::one_element);
                const auto& new_edge_extracted_2 = adjacent_edges_2[constants::first_element_idx];
                CHECK_EQ(&new_edge_extracted_2, &new_edge);
            }
            else {
                CHECK_EQ(adjacent_edges_2.distance(), constants::zero_elements);
            }
        }

        SUBCASE("remove_edge should properly remove the edge for both incident vertices") {
            const auto& added_edge = sut.add_edge(vertex_1, vertex_2, constants::used);

            REQUIRE_EQ(sut.n_unique_edges(), constants::one_element);

            auto adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
            auto adjacent_edges_2 = sut.adjacent_edges(constants::vertex_id_2);

            REQUIRE_EQ(adjacent_edges_1.distance(), constants::one_element);
            if constexpr (lib_tt::is_undirected_v<edge_type>)
                REQUIRE_EQ(adjacent_edges_2.distance(), constants::one_element);

            sut.remove_edge(added_edge);
            CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);

            adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
            adjacent_edges_2 = sut.adjacent_edges(constants::vertex_id_2);

            CHECK_EQ(adjacent_edges_1.distance(), constants::zero_elements);
            CHECK_EQ(adjacent_edges_2.distance(), constants::zero_elements);
        }

        SUBCASE("remove_edges_from should properly erase all given edges") {
            REQUIRE_EQ(sut.n_unique_edges(), constants::zero_elements);

            const auto& edge_1 = sut.add_edge(vertex_1, vertex_2, constants::not_used);
            const auto& edge_2 = sut.add_edge(vertex_2, vertex_3, constants::not_used);
            const auto& edge_3 = sut.add_edge(vertex_3, vertex_1, constants::not_used);

            // an additional edge to verify that only the given edges are removed
            const auto& vertex_4 = sut.add_vertex();
            const auto& edge_4 = sut.add_edge(vertex_1, vertex_4, constants::used);

            REQUIRE_EQ(sut.n_unique_edges(), constants::n_elements + constants::one_element);

            const std::vector<lib_t::const_ref_wrap<property_edge_type>> edges_to_remove{
                edge_1, edge_2, edge_3
            };

            const auto has_edge = [&sut](const auto& edge_ref) {
                return sut.has_edge(edge_ref.get());
            };

            REQUIRE(std::ranges::all_of(edges_to_remove, has_edge));
            REQUIRE(sut.has_edge(edge_4));

            sut.remove_edges_from(edges_to_remove);

            CHECK_EQ(sut.n_unique_edges(), constants::one_element);
            CHECK_FALSE(sut.has_edge(vertex_1, vertex_2));
            CHECK_FALSE(sut.has_edge(vertex_2, vertex_3));
            CHECK_FALSE(sut.has_edge(vertex_3, vertex_1));
            CHECK(sut.has_edge(edge_4));
        }
    }

    SUBCASE("has_edge(vertex, vertex) should throw if one of the vertices is invalid") {
        sut_type sut{constants::n_elements};

        const auto& vd_1 = sut.get_vertex(constants::vertex_id_1);
        const auto& vd_2 = sut.get_vertex(constants::vertex_id_2);

        CHECK_THROWS_AS(
            func::discard_result(sut.has_edge(fixture.out_of_range_vertex, vd_2)), std::out_of_range
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.has_edge(vd_1, fixture.out_of_range_vertex)), std::out_of_range
        );

        CHECK_THROWS_AS(
            func::discard_result(sut.has_edge(fixture.invalid_vertex, vd_2)), std::invalid_argument
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.has_edge(vd_1, fixture.invalid_vertex)), std::invalid_argument
        );
    }

    SUBCASE("has_edge(vertex, vertex) should return true if there is an edge connecting the given "
            "vertices in the graph") {
        sut_type sut{constants::n_elements};

        const auto& vd_1 = sut.get_vertex(constants::vertex_id_1);
        const auto& vd_2 = sut.get_vertex(constants::vertex_id_2);
        const auto& vd_3 = sut.get_vertex(constants::vertex_id_3);

        sut.add_edge(vd_1, vd_2);

        CHECK(sut.has_edge(vd_1, vd_2));
        CHECK_FALSE(sut.has_edge(vd_1, vd_3));
        CHECK_FALSE(sut.has_edge(vd_2, vd_3));
    }

    SUBCASE("get_edge(vertex, vertex) should return nullopt if either vertex is invalid") {
        sut_type sut{constants::n_elements};
        const auto& valid_vertex = sut.get_vertex(constants::vertex_id_1);

        const vertex_type out_of_range_vertex{constants::out_of_range_elemenet_idx};
        CHECK_FALSE(sut.get_edge(valid_vertex, out_of_range_vertex));
        CHECK_FALSE(sut.get_edge(out_of_range_vertex, valid_vertex));
        CHECK_FALSE(sut.get_edge(out_of_range_vertex, out_of_range_vertex));

        const vertex_type invalid_vertex{constants::vertex_id_1};
        CHECK_FALSE(sut.get_edge(valid_vertex, invalid_vertex));
        CHECK_FALSE(sut.get_edge(invalid_vertex, valid_vertex));
        CHECK_FALSE(sut.get_edge(invalid_vertex, invalid_vertex));
    }

    SUBCASE("get_edge(vertex, vertex) should return nullopt if the given vertices are not incident"
    ) {
        sut_type sut{constants::n_elements};
        CHECK_FALSE(sut.get_edge(
            sut.get_vertex(constants::vertex_id_1), sut.get_vertex(constants::vertex_id_2)
        ));
    }

    SUBCASE("get_edge(vertex, vertex) should return a valid edge if the given vetices are incident"
    ) {
        sut_type sut{constants::n_elements};
        const auto& vd_1 = sut.get_vertex(constants::vertex_id_1);
        const auto& vd_2 = sut.get_vertex(constants::vertex_id_2);

        const auto& edge = sut.add_edge(vd_1, vd_2);

        const auto edge_opt_1 = sut.get_edge(vd_1, vd_2);
        REQUIRE(edge_opt_1.has_value());
        CHECK_EQ(&edge_opt_1->get(), &edge);

        if constexpr (lib_tt::is_undirected_v<edge_type>) {
            const auto edge_opt_2 = sut.get_edge(vd_2, vd_1);
            REQUIRE(edge_opt_2.has_value());
            CHECK_EQ(&edge_opt_2->get(), &edge);
        }
        else {
            CHECK_FALSE(sut.get_edge(vd_2, vd_1).has_value());
        }
    }

    SUBCASE("get_edges(id, id) should return an empty vector if either id is invalid") {
        sut_type sut{constants::n_elements};

        CHECK(sut.get_edges(constants::out_of_range_elemenet_idx, constants::vertex_id_2).empty());
        CHECK(sut.get_edges(constants::vertex_id_1, constants::out_of_range_elemenet_idx).empty());

        // clang-format off

        CHECK(sut.get_edges(
            constants::out_of_range_elemenet_idx, constants::out_of_range_elemenet_idx
        ).empty());

        // clang-format on
    }

    SUBCASE("get_edges(id, id) should return an empty vector if the given vertices are not incident"
    ) {
        sut_type sut{constants::n_elements};
        CHECK(sut.get_edges(constants::vertex_id_1, constants::vertex_id_2).empty());
    }

    SUBCASE("get_edges(id, id) should return a valid edge reference vector if the given vertices "
            "are incident") {
        sut_type sut{constants::n_elements};
        std::vector<lib_t::const_ref_wrap<edge_type>> expected_edges;

        if constexpr (std::same_as<typename sut_type::implementation_tag, lib_i::list_t>) {
            for (auto _ = constants::first_element_idx; _ < constants::n_elements; _++)
                expected_edges.push_back(
                    std::cref(sut.add_edge(constants::vertex_id_1, constants::vertex_id_2))
                );
        }
        else {
            expected_edges.push_back(
                std::cref(sut.add_edge(constants::vertex_id_1, constants::vertex_id_2))
            );
        }

        constexpr auto address_projection = [](const auto& edge_ref) { return &edge_ref.get(); };

        CHECK(std::ranges::equal(
            sut.get_edges(constants::vertex_id_1, constants::vertex_id_2),
            expected_edges,
            std::ranges::equal_to{},
            address_projection,
            address_projection
        ));

        if constexpr (lib_tt::is_directed_v<edge_type>) {
            CHECK(sut.get_edges(constants::vertex_id_2, constants::vertex_id_2).empty());
        }
        else {
            CHECK(std::ranges::equal(
                sut.get_edges(constants::vertex_id_2, constants::vertex_id_1),
                expected_edges,
                std::ranges::equal_to{},
                address_projection,
                address_projection
            ));
        }
    }

    SUBCASE("get_edges(vertex, vertex) should throw if either vertex is invalid") {
        sut_type sut{constants::n_elements};

        const auto& vd_1 = sut.get_vertex(constants::vertex_id_1);
        const auto& vd_2 = sut.get_vertex(constants::vertex_id_2);

        CHECK_THROWS_AS(
            func::discard_result(sut.get_edges(fixture.out_of_range_vertex, vd_2)),
            std::out_of_range
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.get_edges(vd_1, fixture.out_of_range_vertex)),
            std::out_of_range
        );

        CHECK_THROWS_AS(
            func::discard_result(sut.get_edges(fixture.invalid_vertex, vd_2)), std::invalid_argument
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.get_edges(vd_1, fixture.invalid_vertex)), std::invalid_argument
        );
    }

    SUBCASE("adjacent_edges(id) should throw if the vertex_id is invalid") {
        sut_type sut{constants::n_elements};
        CHECK_THROWS_AS(
            func::discard_result(sut.adjacent_edges(constants::out_of_range_elemenet_idx)),
            std::out_of_range
        );
    }

    SUBCASE("adjacent_edges(id) should return a proper iterator range for a valid vertex") {
        sut_type sut{constants::one_element};

        CHECK_NOTHROW([&sut]() {
            CHECK_EQ(
                sut.adjacent_edges(constants::first_element_idx).distance(),
                constants::zero_elements
            );
        }());
    }

    SUBCASE("adjacent_edges(vertex) should throw if the vertex is invalid") {
        sut_type sut{constants::n_elements};

        CHECK_THROWS_AS(
            func::discard_result(sut.adjacent_edges(fixture.out_of_range_vertex)), std::out_of_range
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.adjacent_edges(fixture.invalid_vertex)), std::invalid_argument
        );
    }

    SUBCASE("adjacent_edges(vertex) should return a proper iterator range for a valid vertex") {
        sut_type sut{constants::one_element};
        const auto& vertex = sut.get_vertex(constants::first_element_idx);

        CHECK_NOTHROW([&sut, &vertex]() {
            CHECK_EQ(sut.adjacent_edges(vertex).distance(), constants::zero_elements);
        }());
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    graph_traits_template,
    lib::list_graph_traits<lib::directed_t>, // directed adjacency list
    lib::list_graph_traits<lib::undirected_t>, // undirected adjacency list
    lib::matrix_graph_traits<lib::directed_t>, // directed adjacency matrix
    lib::matrix_graph_traits<lib::undirected_t> // undirected adjacency matrix
);

TEST_SUITE_END(); // test_graph

} // namespace gl_testing
