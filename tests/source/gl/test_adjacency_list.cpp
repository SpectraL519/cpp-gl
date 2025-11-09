#include "testing/gl/constants.hpp"
#include "testing/gl/functional.hpp"
#include "testing/gl/transforms.hpp"

#include <gl/graph_traits.hpp>
#include <gl/impl/adjacency_list.hpp>

#include <doctest.h>

#include <algorithm>
#include <functional>

namespace gl_testing {

TEST_SUITE_BEGIN("test_adjacency_list");

TEST_CASE_TEMPLATE_DEFINE(
    "directional_tag-independent tests", SutType, edge_directional_tag_sut_template
) {
    SUBCASE("constructed with the n_vertices parameter should properly initialize the adjacency "
            "list") {
        SutType sut{constants::n_elements};
        std::ranges::for_each(constants::vertex_id_view, [&sut](const gl::types::id_type vertex_id) {
            CHECK_EQ(sut.adjacent_edges(vertex_id).size(), constants::zero_elements);
        });
    }

    SUBCASE("add_vertex should properly extend the current adjacency list") {
        SutType sut{};
        for (gl::types::size_type i = constants::zero; i < constants::n_elements; i++) {
            sut.add_vertex();
            CHECK_EQ(sut.adjacent_edges(i).size(), constants::zero_elements);
        }
    }

    SUBCASE("add_vertices(n) should properly extend the current adjacency list") {
        SutType sut{};
        sut.add_vertices(constants::n_elements);
        for (gl::types::size_type i = constants::zero; i < constants::n_elements; i++)
            CHECK_EQ(sut.adjacent_edges(i).size(), constants::zero_elements);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    edge_directional_tag_sut_template,
    gl::impl::adjacency_list<gl::list_graph_traits<gl::directed_t>>, // directed adj list
    gl::impl::adjacency_list<gl::list_graph_traits<gl::undirected_t>> // undirected adj list
);

namespace {

constexpr gl::types::size_type n_incident_edges_for_fully_connected_vertex =
    constants::n_elements - constants::one_element;

} // namespace

struct test_directed_adjacency_list {
    using edge_type = gl::directed_edge<>;
    using sut_type = gl::impl::adjacency_list<gl::list_graph_traits<gl::directed_t>>;

    test_directed_adjacency_list() {}

    gl::types::id_type add_edge(
        const gl::types::id_type first_id, const gl::types::id_type target_id
    ) {
        const auto new_edge_id = this->n_edges++;
        sut.add_edge(new_edge_id, first_id, target_id);
        return new_edge_id;
    }

    void fully_connect_vertex(const gl::types::id_type first_id, const bool no_loops = true) {
        for (const auto target_id : constants::vertex_id_view) {
            if (target_id == first_id and no_loops)
                continue;

            add_edge(first_id, target_id);
        }
    }

    void init_complete_graph(const bool no_loops = true) {
        for (const auto first_id : constants::vertex_id_view)
            fully_connect_vertex(first_id, no_loops);

        if (no_loops)
            REQUIRE_EQ(this->n_edges, n_unique_edges_in_full_graph);
        else
            REQUIRE_EQ(this->n_edges, constants::n_elements * constants::n_elements);
    }

    sut_type sut{constants::n_elements};
    gl::types::size_type n_edges = 0uz;

    const gl::types::size_type n_unique_edges_in_full_graph =
        n_incident_edges_for_fully_connected_vertex * constants::n_elements;
};

TEST_CASE_FIXTURE(
    test_directed_adjacency_list, "add_edge should add the edge only to the source vertex list"
) {
    const auto new_edge_id = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    REQUIRE_EQ(this->n_edges, constants::one_element);

    const auto adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
    CHECK_EQ(adjacent_edges_1.size(), constants::one_element);
    CHECK(std::ranges::contains(adjacent_edges_1, new_edge_id, &edge_type::id));

    const auto adjacent_edges_2 = sut.adjacent_edges(constants::vertex_id_2);
    CHECK_EQ(adjacent_edges_2.size(), constants::zero_elements);
    CHECK_FALSE(std::ranges::contains(adjacent_edges_2, new_edge_id, &edge_type::id));
}

// TEST_CASE_FIXTURE(
//     test_directed_adjacency_list,
//     "has_edge(id, id) should return true if there is an edge in the graph which connects vertices "
//     "with the given ids in the specified direction"
// ) {
//     add_edge(constants::vertex_id_1, constants::vertex_id_2);

//     CHECK(sut.has_edge(constants::vertex_id_1, constants::vertex_id_2));
//     CHECK_FALSE(sut.has_edge(constants::vertex_id_2, constants::vertex_id_1));
//     CHECK_FALSE(sut.has_edge(constants::vertex_id_1, constants::vertex_id_3));
//     CHECK_FALSE(sut.has_edge(constants::vertex_id_2, constants::vertex_id_3));
// }

// TEST_CASE_FIXTURE(
//     test_directed_adjacency_list,
//     "has_edge(edge) should return true if the given edge is present in the graph"
// ) {
//     const auto& valid_edge = add_edge(constants::vertex_id_1, constants::vertex_id_2);
//     CHECK(sut.has_edge(valid_edge));

//     const edge_type invalid_edge{constants::vertex_id_1, constants::vertex_id_2};
//     CHECK_FALSE(sut.has_edge(invalid_edge));

//     // edge connecting vertices not connected in the actual graph
//     const edge_type not_present_edge{constants::vertex_id_2, constants::vertex_id_3};
//     CHECK_FALSE(sut.has_edge(not_present_edge));
// }

// TEST_CASE_FIXTURE(
//     test_directed_adjacency_list,
//     "get_edge(id, id) should return nullopt if there is no edge connecting the given vertices"
// ) {
//     CHECK_FALSE(sut.get_edge(constants::vertex_id_1, constants::vertex_id_2));
// }

// TEST_CASE_FIXTURE(
//     test_directed_adjacency_list,
//     "get_edge(id, id) should return the first valid edge if the given vertices are connected"
// ) {
//     const auto& edge_1 = add_edge(constants::vertex_id_1, constants::vertex_id_2);
//     const auto& edge_2 = add_edge(constants::vertex_id_1, constants::vertex_id_2);

//     const auto edge_opt = sut.get_edge(constants::vertex_id_1, constants::vertex_id_2);
//     REQUIRE(edge_opt.has_value());
//     CHECK_EQ(&edge_opt->get(), &edge_1);
//     CHECK_NE(&edge_opt->get(), &edge_2);

//     CHECK_FALSE(sut.get_edge(constants::vertex_id_2, constants::vertex_id_2));
// }

// TEST_CASE_FIXTURE(
//     test_directed_adjacency_list,
//     "get_edges(id, id) should return an empty if there is no edge connecting the given vertices"
// ) {
//     CHECK(sut.get_edges(constants::vertex_id_1, constants::vertex_id_2).empty());
// }

// TEST_CASE_FIXTURE(
//     test_directed_adjacency_list,
//     "get_edges(id, id) should return a valid edge view if the given vertices are connected"
// ) {
//     std::vector<gl::types::const_ref_wrap<edge_type>> expected_edges;
//     for (auto _ = constants::first_element_idx; _ < constants::n_elements; _++)
//         expected_edges.push_back(std::cref(add_edge(constants::vertex_id_1, constants::vertex_id_2))
//         );

//     constexpr auto address_projection = [](const auto& edge_ref) { return &edge_ref.get(); };

//     CHECK(std::ranges::equal(
//         sut.get_edges(constants::vertex_id_1, constants::vertex_id_2),
//         expected_edges,
//         std::ranges::equal_to{},
//         address_projection,
//         address_projection
//     ));

//     CHECK(sut.get_edges(constants::vertex_id_2, constants::vertex_id_2).empty());
// }

// TEST_CASE_FIXTURE(test_directed_adjacency_list, "remove_edge should throw when an edge is invalid") {
//     // not existing edge between valid vertices
//     CHECK_THROWS_AS(
//         sut.remove_edge(edge_type{constants::vertex_id_1, constants::vertex_id_2}),
//         std::invalid_argument
//     );
// }

// TEST_CASE_FIXTURE(
//     test_directed_adjacency_list, "remove_edge should remove the edge from the source vertex's list"
// ) {
//     fully_connect_vertex(constants::vertex_id_1);

//     auto adjacent_edges = sut.adjacent_edges(constants::vertex_id_1);
//     REQUIRE_EQ(sut.n_unique_edges(), n_incident_edges_for_fully_connected_vertex);
//     REQUIRE_EQ(adjacent_edges.distance(), n_incident_edges_for_fully_connected_vertex);

//     const auto& edge_to_remove = adjacent_edges[constants::first_element_idx];

//     sut.remove_edge(edge_to_remove);
//     REQUIRE_EQ(
//         sut.n_unique_edges(), n_incident_edges_for_fully_connected_vertex - constants::one_element
//     );

//     adjacent_edges = sut.adjacent_edges(constants::first_element_idx);
//     REQUIRE_EQ(
//         adjacent_edges.distance(),
//         n_incident_edges_for_fully_connected_vertex - constants::one_element
//     );
//     // validate that the adjacent edges list has been properly aligned
//     CHECK_EQ(
//         std::ranges::find(
//             adjacent_edges, &edge_to_remove, transforms::address_projection<edge_type>{}
//         ),
//         adjacent_edges.end()
//     );
// }

// TEST_CASE_FIXTURE(
//     test_directed_adjacency_list,
//     "{in/out}_degree should return the number of edges incident {to/from} the given vertex"
// ) {
//     init_complete_graph();

//     std::function<gl::types::size_type(const gl::types::id_type)> deg_proj;

//     SUBCASE("in_degree") {
//         deg_proj = [this](const auto vertex_id) { return sut.in_degree(vertex_id); };
//     }

//     SUBCASE("out_degree") {
//         deg_proj = [this](const auto vertex_id) { return sut.out_degree(vertex_id); };
//     }

//     CAPTURE(deg_proj);

//     CHECK(std::ranges::all_of(
//         constants::vertex_id_view,
//         [](const auto deg) { return deg == n_incident_edges_for_fully_connected_vertex; },
//         deg_proj
//     ));

//     add_edge(constants::vertex_id_1, constants::vertex_id_1);

//     CHECK_EQ(
//         deg_proj(constants::vertex_id_1),
//         n_incident_edges_for_fully_connected_vertex + constants::one
//     );
// }

// TEST_CASE_FIXTURE(
//     test_directed_adjacency_list,
//     "degree should return the number of edges incident with the given vertex"
// ) {
//     init_complete_graph();
//     const auto deg_proj = [this](const auto vertex_id) { return sut.degree(vertex_id); };

//     CHECK(std::ranges::all_of(
//         constants::vertex_id_view,
//         [](const auto deg) {
//             return deg == constants::two * n_incident_edges_for_fully_connected_vertex;
//         },
//         deg_proj
//     ));

//     add_edge(constants::vertex_id_1, constants::vertex_id_1);

//     CHECK_EQ(
//         deg_proj(constants::vertex_id_1),
//         constants::two * (n_incident_edges_for_fully_connected_vertex + constants::one)
//     );
// }

// TEST_CASE_FIXTURE(
//     test_directed_adjacency_list,
//     "{in/out}_degree_map should return a map of numbers of edges incident {to/from} the "
//     "corresponding vertices"
// ) {
//     init_complete_graph(false);
//     const auto expected_deg = constants::n_elements;

//     std::vector<gl::types::id_type> degree_map;

//     SUBCASE("in_degree") {
//         degree_map = sut.in_degree_map();
//     }

//     SUBCASE("out_degree") {
//         degree_map = sut.out_degree_map();
//     }

//     CAPTURE(degree_map);

//     REQUIRE_EQ(degree_map.size(), constants::n_elements);
//     CHECK_EQ(std::ranges::count(degree_map, expected_deg), constants::n_elements);
// }

// TEST_CASE_FIXTURE(
//     test_directed_adjacency_list,
//     "degree_map should return a map of the numbers of edges incident with the corresponding "
//     "vertices"
// ) {
//     init_complete_graph(false);
//     const auto expected_deg = constants::n_elements * constants::two;

//     std::vector<gl::types::id_type> degree_map = sut.degree_map();

//     REQUIRE_EQ(degree_map.size(), constants::n_elements);
//     CHECK_EQ(std::ranges::count(degree_map, expected_deg), constants::n_elements);
// }

// TEST_CASE_FIXTURE(
//     test_directed_adjacency_list,
//     "remove_vertex should remove the given vertex and all edges incident with it"
// ) {
//     init_complete_graph();

//     const auto removed_vertex_id = constants::first_element_idx;
//     sut.remove_vertex(removed_vertex_id);

//     constexpr auto n_vertices_after_remove = constants::n_elements - constants::one_element;
//     constexpr auto n_incident_edges_after_remove =
//         n_incident_edges_for_fully_connected_vertex - constants::one_element;

//     REQUIRE_EQ(sut.n_vertices(), n_vertices_after_remove);
//     REQUIRE_EQ(sut.n_unique_edges(), n_vertices_after_remove * n_incident_edges_after_remove);

//     for (const auto vertex_id :
//          constants::vertex_id_view | std::views::take(n_vertices_after_remove)) {
//         const auto adjacent_edges = sut.adjacent_edges(vertex_id);
//         REQUIRE_EQ(adjacent_edges.distance(), n_incident_edges_after_remove);
//         CHECK_FALSE(std::ranges::any_of(adjacent_edges, [removed_vertex_id](const auto& edge) {
//             return edge.is_incident_with(removed_vertex_id);
//         }));
//     }
// }

// struct test_undirected_adjacency_list {
//     using edge_type = gl::undirected_edge<>;
//     using edge_ptr_type = gl::undirected_t::edge_ptr_type<edge_type>;
//     using sut_type = gl::impl::adjacency_list<gl::list_graph_traits<gl::undirected_t>>;

//     test_undirected_adjacency_list() {}

//     const edge_type& add_edge(
//         const gl::types::id_type first_id, const gl::types::id_type target_id
//     ) {
//         return sut.add_edge(this->n_edges++, first_id, target_id);
//     }

//     void fully_connect_vertex(const gl::types::id_type first_id, const bool no_loops = true) {
//         for (const auto target_id : constants::vertex_id_view) {
//             if (target_id == first_id and no_loops)
//                 continue;

//             add_edge(first_id, target_id);
//         }
//     }

//     void init_complete_graph(const bool no_loops = true) {
//         for (const auto first_id : constants::vertex_id_view) {
//             const auto bound = no_loops ? first_id : first_id + constants::one;
//             for (const auto target_id : std::views::iota(constants::vertex_id_1, bound))
//                 add_edge(first_id, target_id);
//         }

//         if (no_loops)
//             REQUIRE_EQ(this->n_edges, n_unique_edges_in_full_graph);
//         else
//             REQUIRE_EQ(this->n_edges, n_unique_edges_in_full_graph + constants::n_elements);
//     }

//     sut_type sut{constants::n_elements};
//     gl::types::size_type n_edges = 0uz;

//     const gl::types::size_type n_unique_edges_in_full_graph =
//         (n_incident_edges_for_fully_connected_vertex * constants::n_elements) / 2;
// };

// TEST_CASE_FIXTURE(
//     test_undirected_adjacency_list, "add_edge should add the edge to the lists of both vertices"
// ) {
//     const auto& new_edge = add_edge(constants::vertex_id_1, constants::vertex_id_2);
//     REQUIRE(new_edge.is_incident_from(constants::vertex_id_1));
//     REQUIRE(new_edge.is_incident_to(constants::vertex_id_2));

//     REQUIRE_EQ(sut.n_unique_edges(), constants::one_element);

//     const auto adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
//     const auto adjacent_edges_2 = sut.adjacent_edges(constants::vertex_id_2);

//     REQUIRE_EQ(adjacent_edges_1.distance(), constants::one_element);
//     REQUIRE_EQ(adjacent_edges_2.distance(), constants::one_element);

//     const auto& new_edge_extracted_1 = adjacent_edges_1[constants::first_element_idx];
//     CHECK_EQ(&new_edge_extracted_1, &new_edge);

//     const auto& new_edge_extracted_2 = adjacent_edges_2[constants::first_element_idx];
//     CHECK_EQ(&new_edge_extracted_2, &new_edge);
// }

// TEST_CASE_FIXTURE(
//     test_undirected_adjacency_list,
//     "add_edge should add the edge once to the vertex list if the edge is a loop"
// ) {
//     const auto& new_edge = add_edge(constants::vertex_id_1, constants::vertex_id_1);
//     REQUIRE_EQ(sut.n_unique_edges(), constants::one_element);
//     REQUIRE(new_edge.is_loop());
//     REQUIRE(new_edge.is_incident_from(constants::vertex_id_1));

//     const auto adjacent_edges = sut.adjacent_edges(constants::vertex_id_1);
//     REQUIRE_EQ(adjacent_edges.distance(), constants::one_element);

//     const auto& new_edge_extracted_1 = adjacent_edges[constants::first_element_idx];
//     CHECK_EQ(&new_edge_extracted_1, &new_edge);
// }

// TEST_CASE_FIXTURE(
//     test_undirected_adjacency_list,
//     "has_edge(id, id) should return true if there is an edge in the graph which connects vertices "
//     "with the given ids in any direction"
// ) {
//     add_edge(constants::vertex_id_1, constants::vertex_id_2);

//     CHECK(sut.has_edge(constants::vertex_id_1, constants::vertex_id_2));
//     CHECK(sut.has_edge(constants::vertex_id_2, constants::vertex_id_1));
//     CHECK_FALSE(sut.has_edge(constants::vertex_id_1, constants::vertex_id_3));
//     CHECK_FALSE(sut.has_edge(constants::vertex_id_2, constants::vertex_id_3));
// }

// TEST_CASE_FIXTURE(
//     test_undirected_adjacency_list,
//     "has_edge(edge_ptr) should return true if the given edge is present in the graph"
// ) {
//     const auto& valid_edge = add_edge(constants::vertex_id_1, constants::vertex_id_2);
//     CHECK(sut.has_edge(valid_edge));

//     const edge_type invalid_edge{constants::vertex_id_1, constants::vertex_id_2};
//     CHECK_FALSE(sut.has_edge(invalid_edge));

//     // edge connecting vertices not connected in the actual graph
//     const edge_type not_present_edge{constants::vertex_id_2, constants::vertex_id_3};
//     CHECK_FALSE(sut.has_edge(not_present_edge));
// }

// TEST_CASE_FIXTURE(
//     test_undirected_adjacency_list,
//     "get_edge(id, id) should return nullopt if there is no edge connecting the given vertices"
// ) {
//     CHECK_FALSE(sut.get_edge(constants::vertex_id_1, constants::vertex_id_2));
// }

// TEST_CASE_FIXTURE(
//     test_undirected_adjacency_list,
//     "get_edge(id, id) should return the first valid edge if the given vertices are connected"
// ) {
//     const auto& edge_1 = add_edge(constants::vertex_id_1, constants::vertex_id_2);
//     const auto& edge_2 = add_edge(constants::vertex_id_1, constants::vertex_id_2);

//     const auto edge_opt_1 = sut.get_edge(constants::vertex_id_1, constants::vertex_id_2);
//     REQUIRE(edge_opt_1.has_value());
//     CHECK_EQ(&edge_opt_1->get(), &edge_1);
//     CHECK_NE(&edge_opt_1->get(), &edge_2);

//     const auto edge_opt_2 = sut.get_edge(constants::vertex_id_2, constants::vertex_id_1);
//     REQUIRE(edge_opt_2.has_value());
//     CHECK_EQ(&edge_opt_2->get(), &edge_1);
//     CHECK_NE(&edge_opt_2->get(), &edge_2);
// }

// TEST_CASE_FIXTURE(
//     test_undirected_adjacency_list,
//     "get_edges(id, id) should return an empty if there is no edge connecting the given vertices"
// ) {
//     CHECK(sut.get_edges(constants::vertex_id_1, constants::vertex_id_2).empty());
// }

// TEST_CASE_FIXTURE(
//     test_undirected_adjacency_list,
//     "get_edges(id, id) should return a valid edge view if the given vertices are connected"
// ) {
//     std::vector<gl::types::const_ref_wrap<edge_type>> expected_edges;
//     for (auto _ = constants::first_element_idx; _ < constants::n_elements; _++)
//         expected_edges.push_back(std::cref(add_edge(constants::vertex_id_1, constants::vertex_id_2))
//         );

//     constexpr auto address_projection = [](const auto& edge_ref) { return &edge_ref.get(); };

//     CHECK(std::ranges::equal(
//         sut.get_edges(constants::vertex_id_1, constants::vertex_id_2),
//         expected_edges,
//         std::ranges::equal_to{},
//         address_projection,
//         address_projection
//     ));

//     CHECK(std::ranges::equal(
//         sut.get_edges(constants::vertex_id_2, constants::vertex_id_1),
//         expected_edges,
//         std::ranges::equal_to{},
//         address_projection,
//         address_projection
//     ));
// }

// TEST_CASE_FIXTURE(
//     test_undirected_adjacency_list, "remove_edge should throw when an edge is invalid"
// ) {
//     // not existing edge between valid vertices
//     CHECK_THROWS_AS(
//         sut.remove_edge(edge_type{constants::vertex_id_1, constants::vertex_id_2}),
//         std::invalid_argument
//     );
// }

// TEST_CASE_FIXTURE(
//     test_undirected_adjacency_list,
//     "remove_edge should remove the edge from both the first and second vertices' list"
// ) {
//     fully_connect_vertex(constants::vertex_id_1);

//     auto adjacent_edges_first = sut.adjacent_edges(constants::vertex_id_1);
//     REQUIRE_EQ(sut.n_unique_edges(), n_incident_edges_for_fully_connected_vertex);
//     REQUIRE_EQ(adjacent_edges_first.distance(), n_incident_edges_for_fully_connected_vertex);

//     const auto& edge_to_remove = adjacent_edges_first[constants::first_element_idx];

//     const auto target_id = edge_to_remove.second();
//     REQUIRE_EQ(sut.adjacent_edges(target_id).distance(), constants::one_element);

//     sut.remove_edge(edge_to_remove);
//     REQUIRE_EQ(
//         sut.n_unique_edges(), n_incident_edges_for_fully_connected_vertex - constants::one_element
//     );

//     // validate that the first adjacent edges list has been properly aligned
//     adjacent_edges_first = sut.adjacent_edges(constants::first_element_idx);
//     REQUIRE_EQ(
//         adjacent_edges_first.distance(),
//         n_incident_edges_for_fully_connected_vertex - constants::one_element
//     );
//     CHECK_EQ(
//         std::ranges::find(
//             adjacent_edges_first, &edge_to_remove, transforms::address_projection<edge_type>{}
//         ),
//         adjacent_edges_first.end()
//     );

//     // validate that the second adjacent edges list has been properly aligned
//     const auto adjacent_edges_second = sut.adjacent_edges(target_id);
//     REQUIRE_EQ(adjacent_edges_second.distance(), constants::zero_elements);
//     CHECK_EQ(
//         std::ranges::find(
//             adjacent_edges_second, &edge_to_remove, transforms::address_projection<edge_type>{}
//         ),
//         adjacent_edges_second.end()
//     );
// }

// TEST_CASE_FIXTURE(
//     test_undirected_adjacency_list,
//     "{in_/out_/}degree should return the number of edges incident {to/from} the given vertex"
// ) {
//     init_complete_graph();

//     std::function<gl::types::size_type(const gl::types::id_type)> deg_proj;

//     SUBCASE("degree") {
//         deg_proj = [this](const auto vertex_id) { return sut.degree(vertex_id); };
//     }

//     SUBCASE("in_degree") {
//         deg_proj = [this](const auto vertex_id) { return sut.in_degree(vertex_id); };
//     }

//     SUBCASE("out_degree") {
//         deg_proj = [this](const auto vertex_id) { return sut.out_degree(vertex_id); };
//     }

//     CAPTURE(deg_proj);

//     CHECK(std::ranges::all_of(
//         constants::vertex_id_view,
//         [](const auto deg) { return deg == n_incident_edges_for_fully_connected_vertex; },
//         deg_proj
//     ));

//     add_edge(constants::vertex_id_1, constants::vertex_id_1);

//     CHECK_EQ(
//         deg_proj(constants::vertex_id_1),
//         n_incident_edges_for_fully_connected_vertex + constants::two // loops counted twice
//     );
// }

// TEST_CASE_FIXTURE(
//     test_undirected_adjacency_list,
//     "{in_/out_/}degree_map should return a map of numbers of edges incident {to/from/with} the "
//     "corresponding vertices"
// ) {
//     init_complete_graph(false);
//     const auto expected_deg = constants::n_elements + 1;

//     std::vector<gl::types::id_type> degree_map;

//     SUBCASE("in_degree") {
//         degree_map = sut.in_degree_map();
//     }

//     SUBCASE("out_degree") {
//         degree_map = sut.out_degree_map();
//     }

//     SUBCASE("degree") {
//         degree_map = sut.degree_map();
//     }

//     CAPTURE(degree_map);

//     REQUIRE_EQ(degree_map.size(), constants::n_elements);
//     CHECK_EQ(std::ranges::count(degree_map, expected_deg), constants::n_elements);
// }

// TEST_CASE_FIXTURE(
//     test_undirected_adjacency_list,
//     "remove_vertex should remove the given vertex and all edges incident with it"
// ) {
//     init_complete_graph();

//     const auto removed_vertex_id = constants::first_element_idx;
//     sut.remove_vertex(removed_vertex_id);

//     constexpr auto n_vertices_after_remove = constants::n_elements - constants::one_element;
//     constexpr auto n_incident_edges_after_remove =
//         n_incident_edges_for_fully_connected_vertex - constants::one_element;

//     REQUIRE_EQ(sut.n_vertices(), n_vertices_after_remove);
//     REQUIRE_EQ(sut.n_unique_edges(), (n_vertices_after_remove * n_incident_edges_after_remove) / 2);

//     for (const auto vertex_id :
//          constants::vertex_id_view | std::views::take(n_vertices_after_remove)) {
//         const auto adjacent_edges = sut.adjacent_edges(vertex_id);
//         REQUIRE_EQ(adjacent_edges.distance(), n_incident_edges_after_remove);
//         CHECK_FALSE(std::ranges::any_of(adjacent_edges, [removed_vertex_id](const auto& edge) {
//             return edge.is_incident_with(removed_vertex_id);
//         }));
//     }
// }

TEST_SUITE_END(); // test_adjacency_list

} // namespace gl_testing
