#include "constants.hpp"
#include "functional.hpp"
#include "transforms.hpp"

#include <gl/impl/adjacency_matrix.hpp>
#include <gl/graph_traits.hpp>

#include <doctest.h>

#include <algorithm>
#include <functional>

namespace gl_testing {

TEST_SUITE_BEGIN("test_adjacency_matrix");

TEST_CASE_TEMPLATE_DEFINE(
    "directional_tag-independent tests", SutType, edge_directional_tag_sut_template
) {
    SUBCASE("should be initialized with no vertices and no edges by default") {
        SutType sut{};
        CHECK_EQ(sut.no_vertices(), constants::zero_elements);
        CHECK_EQ(sut.no_unique_edges(), constants::zero_elements);
    }

    SUBCASE("constructed with the no_vertices parameter should properly initialize the adjacency "
            "matrix") {
        SutType sut{constants::no_elements};
        REQUIRE_EQ(sut.no_vertices(), constants::no_elements);
        REQUIRE_EQ(sut.no_unique_edges(), constants::zero_elements);

        std::ranges::for_each(constants::vertex_id_view, [&sut](const lib_t::id_type vertex_id) {
            CHECK_EQ(sut.adjacent_edges_c(vertex_id).distance(), constants::zero_elements);
        });
    }

    SUBCASE("add_vertex should properly extend the current adjacency matrix") {
        SutType sut{};
        constexpr lib_t::size_type target_no_vertices = constants::no_elements;

        for (lib_t::size_type no_vertices = constants::one_element;
             no_vertices <= target_no_vertices;
             no_vertices++) {
            sut.add_vertex();
            CHECK_EQ(sut.no_vertices(), no_vertices);
        }

        CHECK_EQ(sut.no_vertices(), target_no_vertices);
        CHECK_EQ(sut.no_unique_edges(), constants::zero_elements);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    edge_directional_tag_sut_template,
    lib_i::adjacency_matrix<lib::graph_traits<lib::directed_t>>, // directed adj list
    lib_i::adjacency_matrix<lib::graph_traits<lib::undirected_t>> // undirected adj list
);

namespace {

constexpr lib_t::size_type no_incident_edges_for_fully_connected_vertex =
    constants::no_elements - constants::one_element;

} // namespace

struct test_directed_adjacency_matrix {
    using vertex_type = lib::vertex_descriptor<>;
    using edge_type = lib::directed_edge<vertex_type>;
    using sut_type = lib_i::adjacency_matrix<lib::graph_traits<lib::directed_t>>;

    test_directed_adjacency_matrix() {
        for (const auto id : constants::vertex_id_view)
            vertices.push_back(std::make_shared<vertex_type>(id));
    }

    void add_edge(const lib_t::id_type first_id, const lib_t::id_type second_id) {
        sut.add_edge(lib::make_edge<edge_type>(vertices[first_id], vertices[second_id]));
    }

    void fully_connect_vertex(const lib_t::id_type first_id) {
        for (const auto second_id : constants::vertex_id_view)
            if (second_id != first_id)
                add_edge(first_id, second_id);
    }

    void prepare_full_graph() {
        for (const auto first_id : constants::vertex_id_view)
            fully_connect_vertex(first_id);

        REQUIRE_EQ(sut.no_unique_edges(), no_unique_edges_in_full_graph);
    }

    sut_type sut{constants::no_elements};
    std::vector<std::shared_ptr<vertex_type>> vertices;

    static constexpr lib_t::size_type no_unique_edges_in_full_graph =
        no_incident_edges_for_fully_connected_vertex * constants::no_elements;
};

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix, "add_edge should add the edge only to the source vertex list"
) {
    add_edge(constants::vertex_id_1, constants::vertex_id_2);

    REQUIRE_EQ(sut.no_unique_edges(), constants::one_element);

    const auto adjacent_edges_1 = sut.adjacent_edges_c(constants::vertex_id_1);
    REQUIRE_EQ(adjacent_edges_1.distance(), constants::one_element);
    REQUIRE_EQ(sut.adjacent_edges_c(constants::vertex_id_2).distance(), constants::zero_elements);

    const auto& new_edge = adjacent_edges_1.element_at(constants::first_element_idx);
    CHECK(new_edge->is_incident_from(vertices[constants::vertex_id_1]));
    CHECK(new_edge->is_incident_to(vertices[constants::vertex_id_2]));
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix,
    "remove_edge should remove the edge from the source vertex's list"
) {
    fully_connect_vertex(constants::vertex_id_1);

    auto adjacent_edges = sut.adjacent_edges(constants::vertex_id_1);
    REQUIRE_EQ(sut.no_unique_edges(), no_incident_edges_for_fully_connected_vertex);
    REQUIRE_EQ(adjacent_edges.distance(), no_incident_edges_for_fully_connected_vertex);

    const auto& edge_to_remove = adjacent_edges.element_at(constants::first_element_idx);
    const auto edge_to_remove_addr = edge_to_remove.get();

    sut.remove_edge(edge_to_remove);
    REQUIRE_EQ(
        sut.no_unique_edges(), no_incident_edges_for_fully_connected_vertex - constants::one_element
    );

    // validate that the adjacent edges list has been properly aligned
    adjacent_edges = sut.adjacent_edges(constants::vertex_id_1);
    REQUIRE_EQ(
        adjacent_edges.distance(),
        no_incident_edges_for_fully_connected_vertex - constants::one_element
    );
    CHECK_EQ(
        std::ranges::find(
            adjacent_edges, edge_to_remove_addr, transforms::address_projection<edge_type>{}
        ),
        adjacent_edges.end()
    );
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix,
    "remove_vertex should remove the given vertex and all edges incident with it"
) {
    prepare_full_graph();

    const auto& removed_vertex = vertices[constants::first_element_idx];
    sut.remove_vertex(removed_vertex);

    constexpr auto no_vertices_after_remove = constants::no_elements - constants::one_element;
    constexpr auto no_incident_edges_after_remove =
        no_incident_edges_for_fully_connected_vertex - constants::one_element;

    REQUIRE_EQ(sut.no_vertices(), no_vertices_after_remove);
    REQUIRE_EQ(sut.no_unique_edges(), no_vertices_after_remove * no_incident_edges_after_remove);

    for (const auto vertex_id :
         constants::vertex_id_view | std::views::take(no_vertices_after_remove)) {
        const auto adjacent_edges = sut.adjacent_edges_c(vertex_id);
        REQUIRE_EQ(adjacent_edges.distance(), no_incident_edges_after_remove);
        CHECK_FALSE(std::ranges::any_of(adjacent_edges, [&removed_vertex](const auto& edge) {
            return edge->is_incident_with(removed_vertex);
        }));
    }
}

struct test_undirected_adjacency_matrix {
    using vertex_type = lib::vertex_descriptor<>;
    using edge_type = lib::undirected_edge<vertex_type>;
    using sut_type = lib_i::adjacency_matrix<lib::graph_traits<lib::undirected_t>>;

    test_undirected_adjacency_matrix() {
        for (const auto id : constants::vertex_id_view)
            vertices.push_back(std::make_shared<vertex_type>(id));
    }

    void add_edge(const lib_t::id_type first_id, const lib_t::id_type second_id) {
        sut.add_edge(lib::make_edge<edge_type>(vertices[first_id], vertices[second_id]));
    }

    void fully_connect_vertex(const lib_t::id_type first_id) {
        for (const auto second_id : constants::vertex_id_view)
            if (second_id != first_id)
                add_edge(first_id, second_id);
    }

    void prepare_full_graph() {
        for (const auto first_id : constants::vertex_id_view)
            for (const auto second_id : std::views::iota(constants::vertex_id_1, first_id))
                add_edge(first_id, second_id);

        REQUIRE_EQ(sut.no_unique_edges(), no_unique_edges_in_full_graph);
    }

    sut_type sut{constants::no_elements};
    std::vector<std::shared_ptr<vertex_type>> vertices;

    static constexpr lib_t::size_type no_unique_edges_in_full_graph =
        (no_incident_edges_for_fully_connected_vertex * constants::no_elements) / 2;
};

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix, "add_edge should add the edge to the lists of both vertices"
) {
    add_edge(constants::vertex_id_1, constants::vertex_id_2);

    REQUIRE_EQ(sut.no_unique_edges(), constants::one_element);

    const auto adjacent_edges_1 = sut.adjacent_edges_c(constants::vertex_id_1);
    const auto adjacent_edges_2 = sut.adjacent_edges_c(constants::vertex_id_2);

    REQUIRE_EQ(adjacent_edges_1.distance(), constants::one_element);
    REQUIRE_EQ(adjacent_edges_2.distance(), constants::one_element);

    const auto& new_edge_1 = adjacent_edges_1.element_at(constants::first_element_idx);
    CHECK(new_edge_1->is_incident_from(vertices[constants::vertex_id_1]));
    CHECK(new_edge_1->is_incident_to(vertices[constants::vertex_id_2]));

    const auto& new_edge_2 = adjacent_edges_2.element_at(constants::first_element_idx);
    CHECK_EQ(new_edge_1.get(), new_edge_2.get());
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
    "add_edge should add the edge once to the vertex list if the edge is a loop"
) {
    add_edge(constants::vertex_id_1, constants::vertex_id_1);

    REQUIRE_EQ(sut.no_unique_edges(), constants::one_element);

    const auto adjacent_edges = sut.adjacent_edges_c(constants::vertex_id_1);
    REQUIRE_EQ(adjacent_edges.distance(), constants::one_element);

    CHECK(adjacent_edges.element_at(constants::first_element_idx)->is_loop());
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
    "remove_edge should remove the edge from both the first and second vertices' list"
) {
    fully_connect_vertex(constants::vertex_id_1);

    auto adjacent_edges_first = sut.adjacent_edges(constants::vertex_id_1);
    REQUIRE_EQ(sut.no_unique_edges(), no_incident_edges_for_fully_connected_vertex);
    REQUIRE_EQ(adjacent_edges_first.distance(), no_incident_edges_for_fully_connected_vertex);

    const auto& edge_to_remove = adjacent_edges_first.element_at(constants::first_element_idx);
    const auto edge_to_remove_addr = edge_to_remove.get();

    const auto second_id = edge_to_remove->second()->id();
    REQUIRE_EQ(sut.adjacent_edges_c(second_id).distance(), constants::one_element);

    sut.remove_edge(edge_to_remove);
    REQUIRE_EQ(
        sut.no_unique_edges(), no_incident_edges_for_fully_connected_vertex - constants::one_element
    );

    // validate that the first adjacent edges list has been properly aligned
    adjacent_edges_first = sut.adjacent_edges(constants::vertex_id_1);
    REQUIRE_EQ(
        adjacent_edges_first.distance(),
        no_incident_edges_for_fully_connected_vertex - constants::one_element
    );
    CHECK_EQ(
        std::ranges::find(
            adjacent_edges_first, edge_to_remove_addr, transforms::address_projection<edge_type>{}
        ),
        adjacent_edges_first.end()
    );

    // validate that the second adjacent edges list has been properly aligned
    const auto adjacent_edges_second = sut.adjacent_edges_c(second_id);
    REQUIRE_EQ(adjacent_edges_second.distance(), constants::zero_elements);
    CHECK_EQ(
        std::ranges::find(
            adjacent_edges_second, edge_to_remove_addr, transforms::address_projection<edge_type>{}
        ),
        adjacent_edges_second.end()
    );
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
    "remove_vertex should remove the given vertex and all edges incident with it"
) {
    prepare_full_graph();

    const auto& removed_vertex = vertices[constants::first_element_idx];
    sut.remove_vertex(removed_vertex);

    constexpr auto no_vertices_after_remove = constants::no_elements - constants::one_element;
    constexpr auto no_incident_edges_after_remove =
        no_incident_edges_for_fully_connected_vertex - constants::one_element;

    REQUIRE_EQ(sut.no_vertices(), no_vertices_after_remove);
    REQUIRE_EQ(
        sut.no_unique_edges(), (no_vertices_after_remove * no_incident_edges_after_remove) / 2
    );

    for (const auto vertex_id :
         constants::vertex_id_view | std::views::take(no_vertices_after_remove)) {
        const auto adjacent_edges = sut.adjacent_edges_c(vertex_id);
        REQUIRE_EQ(adjacent_edges.distance(), no_incident_edges_after_remove);
        CHECK_FALSE(std::ranges::any_of(adjacent_edges, [&removed_vertex](const auto& edge) {
            return edge->is_incident_with(removed_vertex);
        }));
    }
}

TEST_SUITE_END(); // test_adjacency_matrix

} // namespace gl_testing
