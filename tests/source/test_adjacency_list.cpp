#include "constants.hpp"
#include "functional.hpp"
#include "transforms.hpp"

#include <gl/graph_traits.hpp>
#include <gl/impl/adjacency_list.hpp>

#include <doctest.h>

#include <algorithm>

namespace gl_testing {

TEST_SUITE_BEGIN("test_adjacency_list");

TEST_CASE_TEMPLATE_DEFINE(
    "directional_tag-independent tests", SutType, edge_directional_tag_sut_template
) {
    SUBCASE("should be initialized with no vertices and no edges by default") {
        SutType sut{};
        CHECK_EQ(sut.n_vertices(), constants::zero_elements);
        CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);
    }

    SUBCASE("constructed with the n_vertices parameter should properly initialize the adjacency "
            "list") {
        SutType sut{constants::n_elements};
        REQUIRE_EQ(sut.n_vertices(), constants::n_elements);
        REQUIRE_EQ(sut.n_unique_edges(), constants::zero_elements);

        std::ranges::for_each(constants::vertex_id_view, [&sut](const lib_t::id_type vertex_id) {
            CHECK_EQ(sut.adjacent_edges(vertex_id).distance(), constants::zero_elements);
        });
    }

    SUBCASE("add_vertex should properly extend the current adjacency list") {
        SutType sut{};
        constexpr lib_t::size_type target_n_vertices = constants::n_elements;

        for (lib_t::size_type n_vertices = constants::one_element; n_vertices <= target_n_vertices;
             n_vertices++) {
            sut.add_vertex();
            CHECK_EQ(sut.n_vertices(), n_vertices);
        }

        CHECK_EQ(sut.n_vertices(), target_n_vertices);
        CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    edge_directional_tag_sut_template,
    lib_i::adjacency_list<lib::graph_traits<lib::directed_t>>, // directed adj list
    lib_i::adjacency_list<lib::graph_traits<lib::undirected_t>> // undirected adj list
);

namespace {

constexpr lib_t::size_type n_incident_edges_for_fully_connected_vertex =
    constants::n_elements - constants::one_element;

} // namespace

struct test_directed_adjacency_list {
    using vertex_type = lib::vertex_descriptor<>;
    using edge_type = lib::directed_edge<vertex_type>;
    using edge_ptr_type = lib::directed_t::edge_ptr_type<edge_type>;
    using sut_type = lib_i::adjacency_list<lib::graph_traits<lib::directed_t>>;

    test_directed_adjacency_list() {
        for (const auto id : constants::vertex_id_view)
            vertices.emplace_back(id);
    }

    const edge_type& add_edge(const lib_t::id_type first_id, const lib_t::id_type second_id) {
        return sut.add_edge(lib::make_edge<edge_type>(vertices[first_id], vertices[second_id]));
    }

    void fully_connect_vertex(const lib_t::id_type first_id) {
        for (const auto second_id : constants::vertex_id_view)
            if (second_id != first_id)
                add_edge(first_id, second_id);
    }

    void initialize_full_graph() {
        for (const auto first_id : constants::vertex_id_view)
            fully_connect_vertex(first_id);

        REQUIRE_EQ(sut.n_unique_edges(), n_unique_edges_in_full_graph);
    }

    sut_type sut{constants::n_elements};
    std::vector<vertex_type> vertices;

    const lib_t::size_type n_unique_edges_in_full_graph =
        n_incident_edges_for_fully_connected_vertex * constants::n_elements;
};

TEST_CASE_FIXTURE(
    test_directed_adjacency_list, "add_edge should add the edge only to the source vertex list"
) {
    const auto& new_edge = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    REQUIRE_EQ(sut.n_unique_edges(), constants::one_element);

    REQUIRE(new_edge.is_incident_from(vertices[constants::vertex_id_1]));
    REQUIRE(new_edge.is_incident_to(vertices[constants::vertex_id_2]));

    const auto adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
    CHECK_EQ(adjacent_edges_1.distance(), constants::one_element);
    CHECK_EQ(sut.adjacent_edges(constants::vertex_id_2).distance(), constants::zero_elements);

    const auto& new_edge_extracted = adjacent_edges_1.element_at(constants::first_element_idx);
    CHECK_EQ(&new_edge_extracted, &new_edge);
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_list, "has_edge(id, id) should return false if either id is invalid"
) {
    CHECK_FALSE(sut.has_edge(constants::out_of_range_elemenet_idx, constants::vertex_id_2));
    CHECK_FALSE(sut.has_edge(constants::vertex_id_1, constants::out_of_range_elemenet_idx));
    CHECK_FALSE(
        sut.has_edge(constants::out_of_range_elemenet_idx, constants::out_of_range_elemenet_idx)
    );
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_list,
    "has_edge(id, id) should return true if there is an edge in the graph which connects vertices "
    "with the given ids in the specified direction"
) {
    add_edge(constants::vertex_id_1, constants::vertex_id_2);

    CHECK(sut.has_edge(constants::vertex_id_1, constants::vertex_id_2));
    CHECK_FALSE(sut.has_edge(constants::vertex_id_2, constants::vertex_id_1));
    CHECK_FALSE(sut.has_edge(constants::vertex_id_1, constants::vertex_id_3));
    CHECK_FALSE(sut.has_edge(constants::vertex_id_2, constants::vertex_id_3));
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_list,
    "has_edge(edge_ptr) should return true if the given edge is present in the graph"
) {
    const auto& valid_edge = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    CHECK(sut.has_edge(valid_edge));

    const edge_type invalid_edge{
        vertices[constants::vertex_id_1], vertices[constants::vertex_id_2]
    };
    CHECK_FALSE(sut.has_edge(invalid_edge));

    // edge connecting vertices not connected in the actual graph
    const edge_type not_present_edge{
        vertices[constants::vertex_id_2], vertices[constants::vertex_id_3]
    };
    CHECK_FALSE(sut.has_edge(not_present_edge));

    const vertex_type out_of_range_vertex{constants::out_of_range_elemenet_idx};
    CHECK_FALSE(sut.has_edge(edge_type{out_of_range_vertex, vertices[constants::vertex_id_2]}));
    CHECK_FALSE(sut.has_edge(edge_type{vertices[constants::vertex_id_1], out_of_range_vertex}));
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_list, "get_edge(id, id) should return nullopt if either id is invalid"
) {
    CHECK_FALSE(sut.get_edge(constants::out_of_range_elemenet_idx, constants::vertex_id_2));
    CHECK_FALSE(sut.get_edge(constants::vertex_id_1, constants::out_of_range_elemenet_idx));
    CHECK_FALSE(
        sut.get_edge(constants::out_of_range_elemenet_idx, constants::out_of_range_elemenet_idx)
    );
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_list,
    "get_edge(id, id) should return nullopt if there is no edge connecting the given vertices"
) {
    CHECK_FALSE(sut.get_edge(constants::vertex_id_1, constants::vertex_id_2));
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_list,
    "get_edge(id, id) should return the first valid edge if the given vertices are connected"
) {
    const auto& edge_1 = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    const auto& edge_2 = add_edge(constants::vertex_id_1, constants::vertex_id_2);

    const auto edge_opt = sut.get_edge(constants::vertex_id_1, constants::vertex_id_2);

    REQUIRE(edge_opt.has_value());
    CHECK_EQ(&edge_opt->get(), &edge_1);
    CHECK_NE(&edge_opt->get(), &edge_2);

    CHECK_FALSE(sut.get_edge(constants::vertex_id_2, constants::vertex_id_2));
}

TEST_CASE_FIXTURE(test_directed_adjacency_list, "remove_edge should throw when an edge is invalid") {
    const vertex_type out_of_range_vertex{constants::out_of_range_elemenet_idx};

    CHECK_THROWS_AS(
        sut.remove_edge(edge_type{out_of_range_vertex, vertices[constants::vertex_id_2]}),
        std::out_of_range
    );

    // the edge with an invalid vertex will not be found in the list
    CHECK_THROWS_AS(
        sut.remove_edge(edge_type{vertices[constants::vertex_id_1], out_of_range_vertex}),
        std::invalid_argument
    );

    // not existing edge between valid vertices
    CHECK_THROWS_AS(
        sut.remove_edge(
            edge_type{vertices[constants::vertex_id_1], vertices[constants::vertex_id_2]}
        ),
        std::invalid_argument
    );
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_list, "remove_edge should remove the edge from the source vertex's list"
) {
    fully_connect_vertex(constants::vertex_id_1);

    auto adjacent_edges = sut.adjacent_edges(constants::vertex_id_1);
    REQUIRE_EQ(sut.n_unique_edges(), n_incident_edges_for_fully_connected_vertex);
    REQUIRE_EQ(adjacent_edges.distance(), n_incident_edges_for_fully_connected_vertex);

    const auto& edge_to_remove = adjacent_edges.element_at(constants::first_element_idx);

    sut.remove_edge(edge_to_remove);
    REQUIRE_EQ(
        sut.n_unique_edges(), n_incident_edges_for_fully_connected_vertex - constants::one_element
    );

    adjacent_edges = sut.adjacent_edges(constants::first_element_idx);
    REQUIRE_EQ(
        adjacent_edges.distance(),
        n_incident_edges_for_fully_connected_vertex - constants::one_element
    );
    // validate that the adjacent edges list has been properly aligned
    CHECK_EQ(
        std::ranges::find(
            adjacent_edges, &edge_to_remove, transforms::address_projection<edge_type>{}
        ),
        adjacent_edges.end()
    );
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_list,
    "remove_vertex should remove the given vertex and all edges incident with it"
) {
    initialize_full_graph();

    const auto& removed_vertex = vertices[constants::first_element_idx];
    sut.remove_vertex(removed_vertex);

    constexpr auto n_vertices_after_remove = constants::n_elements - constants::one_element;
    constexpr auto n_incident_edges_after_remove =
        n_incident_edges_for_fully_connected_vertex - constants::one_element;

    REQUIRE_EQ(sut.n_vertices(), n_vertices_after_remove);
    REQUIRE_EQ(sut.n_unique_edges(), n_vertices_after_remove * n_incident_edges_after_remove);

    for (const auto vertex_id :
         constants::vertex_id_view | std::views::take(n_vertices_after_remove)) {
        const auto adjacent_edges = sut.adjacent_edges(vertex_id);
        REQUIRE_EQ(adjacent_edges.distance(), n_incident_edges_after_remove);
        CHECK_FALSE(std::ranges::any_of(adjacent_edges, [&removed_vertex](const auto& edge) {
            return edge.is_incident_with(removed_vertex);
        }));
    }
}

struct test_undirected_adjacency_list {
    using vertex_type = lib::vertex_descriptor<>;
    using edge_type = lib::undirected_edge<vertex_type>;
    using edge_ptr_type = lib::undirected_t::edge_ptr_type<edge_type>;
    using sut_type = lib_i::adjacency_list<lib::graph_traits<lib::undirected_t>>;

    test_undirected_adjacency_list() {
        for (const auto id : constants::vertex_id_view)
            vertices.emplace_back(id);
    }

    const edge_type& add_edge(const lib_t::id_type first_id, const lib_t::id_type second_id) {
        return sut.add_edge(lib::make_edge<edge_type>(vertices[first_id], vertices[second_id]));
    }

    void fully_connect_vertex(const lib_t::id_type first_id) {
        for (const auto second_id : constants::vertex_id_view)
            if (second_id != first_id)
                add_edge(first_id, second_id);
    }

    void initialize_full_graph() {
        for (const auto first_id : constants::vertex_id_view)
            for (const auto second_id : std::views::iota(constants::vertex_id_1, first_id))
                add_edge(first_id, second_id);

        REQUIRE_EQ(sut.n_unique_edges(), n_unique_edges_in_full_graph);
    }

    sut_type sut{constants::n_elements};
    std::vector<vertex_type> vertices;

    const lib_t::size_type n_unique_edges_in_full_graph =
        (n_incident_edges_for_fully_connected_vertex * constants::n_elements) / 2;
};

TEST_CASE_FIXTURE(
    test_undirected_adjacency_list, "add_edge should add the edge to the lists of both vertices"
) {
    const auto& new_edge = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    REQUIRE(new_edge.is_incident_from(vertices[constants::vertex_id_1]));
    REQUIRE(new_edge.is_incident_to(vertices[constants::vertex_id_2]));

    REQUIRE_EQ(sut.n_unique_edges(), constants::one_element);

    const auto adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
    const auto adjacent_edges_2 = sut.adjacent_edges(constants::vertex_id_2);

    REQUIRE_EQ(adjacent_edges_1.distance(), constants::one_element);
    REQUIRE_EQ(adjacent_edges_2.distance(), constants::one_element);

    const auto& new_edge_extracted_1 = adjacent_edges_1.element_at(constants::first_element_idx);
    CHECK_EQ(&new_edge_extracted_1, &new_edge);

    const auto& new_edge_extracted_2 = adjacent_edges_2.element_at(constants::first_element_idx);
    CHECK_EQ(&new_edge_extracted_2, &new_edge);
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_list,
    "add_edge should add the edge once to the vertex list if the edge is a loop"
) {
    const auto& new_edge = add_edge(constants::vertex_id_1, constants::vertex_id_1);
    REQUIRE_EQ(sut.n_unique_edges(), constants::one_element);
    REQUIRE(new_edge.is_loop());
    REQUIRE(new_edge.is_incident_from(vertices[constants::vertex_id_1]));

    const auto adjacent_edges = sut.adjacent_edges(constants::vertex_id_1);
    REQUIRE_EQ(adjacent_edges.distance(), constants::one_element);

    const auto& new_edge_extracted_1 = adjacent_edges.element_at(constants::first_element_idx);
    CHECK_EQ(&new_edge_extracted_1, &new_edge);
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_list, "has_edge(id, id) should return false if either id is invalid"
) {
    CHECK_FALSE(sut.has_edge(constants::out_of_range_elemenet_idx, constants::vertex_id_2));
    CHECK_FALSE(sut.has_edge(constants::vertex_id_1, constants::out_of_range_elemenet_idx));
    CHECK_FALSE(
        sut.has_edge(constants::out_of_range_elemenet_idx, constants::out_of_range_elemenet_idx)
    );
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_list,
    "has_edge(id, id) should return true if there is an edge in the graph which connects vertices "
    "with the given ids in any direction"
) {
    add_edge(constants::vertex_id_1, constants::vertex_id_2);

    CHECK(sut.has_edge(constants::vertex_id_1, constants::vertex_id_2));
    CHECK(sut.has_edge(constants::vertex_id_2, constants::vertex_id_1));
    CHECK_FALSE(sut.has_edge(constants::vertex_id_1, constants::vertex_id_3));
    CHECK_FALSE(sut.has_edge(constants::vertex_id_2, constants::vertex_id_3));
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_list,
    "has_edge(edge_ptr) should return true if the given edge is present in the graph"
) {
    const auto& valid_edge = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    CHECK(sut.has_edge(valid_edge));

    const edge_type invalid_edge{
        vertices[constants::vertex_id_1], vertices[constants::vertex_id_2]
    };
    CHECK_FALSE(sut.has_edge(invalid_edge));

    // edge connecting vertices not connected in the actual graph
    const edge_type not_present_edge{
        vertices[constants::vertex_id_2], vertices[constants::vertex_id_3]
    };
    CHECK_FALSE(sut.has_edge(not_present_edge));

    const vertex_type out_of_range_vertex{constants::out_of_range_elemenet_idx};
    CHECK_FALSE(sut.has_edge(edge_type{out_of_range_vertex, vertices[constants::vertex_id_2]}));
    CHECK_FALSE(sut.has_edge(edge_type{vertices[constants::vertex_id_1], out_of_range_vertex}));
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_list, "get_edge(id, id) should return nullopt if either id is invalid"
) {
    CHECK_FALSE(sut.get_edge(constants::out_of_range_elemenet_idx, constants::vertex_id_2));
    CHECK_FALSE(sut.get_edge(constants::vertex_id_1, constants::out_of_range_elemenet_idx));
    CHECK_FALSE(
        sut.get_edge(constants::out_of_range_elemenet_idx, constants::out_of_range_elemenet_idx)
    );
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_list,
    "get_edge(id, id) should return nullopt if there is no edge connecting the given vertices"
) {
    CHECK_FALSE(sut.get_edge(constants::vertex_id_1, constants::vertex_id_2));
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_list,
    "get_edge(id, id) should return the first valid edge if the given vertices are connected"
) {
    const auto& edge_1 = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    const auto& edge_2 = add_edge(constants::vertex_id_1, constants::vertex_id_2);

    const auto edge_opt_1 = sut.get_edge(constants::vertex_id_1, constants::vertex_id_2);
    REQUIRE(edge_opt_1.has_value());
    CHECK_EQ(&edge_opt_1->get(), &edge_1);
    CHECK_NE(&edge_opt_1->get(), &edge_2);

    const auto edge_opt_2 = sut.get_edge(constants::vertex_id_2, constants::vertex_id_1);
    REQUIRE(edge_opt_2.has_value());
    CHECK_EQ(&edge_opt_2->get(), &edge_1);
    CHECK_NE(&edge_opt_2->get(), &edge_2);
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_list, "remove_edge should throw when an edge is invalid"
) {
    const vertex_type out_of_range_vertex{constants::out_of_range_elemenet_idx};

    CHECK_THROWS_AS(
        sut.remove_edge(edge_type{out_of_range_vertex, vertices[constants::vertex_id_2]}),
        std::out_of_range
    );
    CHECK_THROWS_AS(
        sut.remove_edge(edge_type{vertices[constants::vertex_id_1], out_of_range_vertex}),
        std::out_of_range
    );

    // not existing edge between valid vertices
    CHECK_THROWS_AS(
        sut.remove_edge(
            edge_type{vertices[constants::vertex_id_1], vertices[constants::vertex_id_2]}
        ),
        std::invalid_argument
    );
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_list,
    "remove_edge should remove the edge from both the first and second vertices' list"
) {
    fully_connect_vertex(constants::vertex_id_1);

    auto adjacent_edges_first = sut.adjacent_edges(constants::vertex_id_1);
    REQUIRE_EQ(sut.n_unique_edges(), n_incident_edges_for_fully_connected_vertex);
    REQUIRE_EQ(adjacent_edges_first.distance(), n_incident_edges_for_fully_connected_vertex);

    const auto& edge_to_remove = adjacent_edges_first.element_at(constants::first_element_idx);

    const auto second_id = edge_to_remove.second().id();
    REQUIRE_EQ(sut.adjacent_edges(second_id).distance(), constants::one_element);

    sut.remove_edge(edge_to_remove);
    REQUIRE_EQ(
        sut.n_unique_edges(), n_incident_edges_for_fully_connected_vertex - constants::one_element
    );

    // validate that the first adjacent edges list has been properly aligned
    adjacent_edges_first = sut.adjacent_edges(constants::first_element_idx);
    REQUIRE_EQ(
        adjacent_edges_first.distance(),
        n_incident_edges_for_fully_connected_vertex - constants::one_element
    );
    CHECK_EQ(
        std::ranges::find(
            adjacent_edges_first, &edge_to_remove, transforms::address_projection<edge_type>{}
        ),
        adjacent_edges_first.end()
    );

    // validate that the second adjacent edges list has been properly aligned
    const auto adjacent_edges_second = sut.adjacent_edges(second_id);
    REQUIRE_EQ(adjacent_edges_second.distance(), constants::zero_elements);
    CHECK_EQ(
        std::ranges::find(
            adjacent_edges_second, &edge_to_remove, transforms::address_projection<edge_type>{}
        ),
        adjacent_edges_second.end()
    );
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_list,
    "remove_vertex should remove the given vertex and all edges incident with it"
) {
    initialize_full_graph();

    const auto& removed_vertex = vertices[constants::first_element_idx];
    sut.remove_vertex(removed_vertex);

    constexpr auto n_vertices_after_remove = constants::n_elements - constants::one_element;
    constexpr auto n_incident_edges_after_remove =
        n_incident_edges_for_fully_connected_vertex - constants::one_element;

    REQUIRE_EQ(sut.n_vertices(), n_vertices_after_remove);
    REQUIRE_EQ(sut.n_unique_edges(), (n_vertices_after_remove * n_incident_edges_after_remove) / 2);

    for (const auto vertex_id :
         constants::vertex_id_view | std::views::take(n_vertices_after_remove)) {
        const auto adjacent_edges = sut.adjacent_edges(vertex_id);
        REQUIRE_EQ(adjacent_edges.distance(), n_incident_edges_after_remove);
        CHECK_FALSE(std::ranges::any_of(adjacent_edges, [&removed_vertex](const auto& edge) {
            return edge.is_incident_with(removed_vertex);
        }));
    }
}

TEST_SUITE_END(); // test_adjacency_list

} // namespace gl_testing
