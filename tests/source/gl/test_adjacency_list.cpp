#include "testing/gl/constants.hpp"
#include "testing/gl/functional.hpp"

#include <gl/graph_traits.hpp>
#include <gl/impl/adjacency_list.hpp>

#include <doctest.h>

#include <algorithm>
#include <functional>
#include <vector>

namespace gl_testing {

TEST_SUITE_BEGIN("test_adjacency_list");

struct test_adjacency_list {
    [[nodiscard]] auto& get(auto& sut) const {
        return sut._list;
    }

    gl::size_type size(const auto& sut) const {
        return sut._list.size();
    }

    gl::default_id_type next_edge_id = 0u;
};

TEST_CASE_TEMPLATE_DEFINE("common adjacency list tests", SutType, common_adj_list_template) {
    test_adjacency_list fixture;

    SUBCASE("should be initialized with no vertices and no edges by default") {
        SutType sut{};
        CHECK_EQ(fixture.size(sut), 0uz);
    }

    SUBCASE("constructed with the n_vertices parameter should properly initialize the adjacency "
            "list") {
        SutType sut{constants::n_elements};
        REQUIRE_EQ(fixture.size(sut), constants::n_elements);
        CHECK(std::ranges::all_of(fixture.get(sut), [](const auto& adj_items) {
            return adj_items.empty();
        }));
    }

    SUBCASE("add_vertex should properly extend the current adjacency list") {
        SutType sut{};
        constexpr gl::size_type target_n_vertices = constants::n_elements;

        for (gl::size_type n_vertices = 1uz; n_vertices <= target_n_vertices; n_vertices++) {
            sut.add_vertex();
            CHECK_EQ(fixture.size(sut), n_vertices);
        }

        CHECK_EQ(fixture.size(sut), target_n_vertices);
        CHECK(std::ranges::all_of(fixture.get(sut), [](const auto& adj_items) {
            return adj_items.empty();
        }));
    }

    SUBCASE("add_vertices(n) should properly extend the current adjacency list") {
        SutType sut{};
        sut.add_vertices(constants::n_elements);

        CHECK_EQ(fixture.size(sut), constants::n_elements);
        CHECK(std::ranges::all_of(fixture.get(sut), [](const auto& adj_items) {
            return adj_items.empty();
        }));
    }

    SUBCASE("equality operator should correctly comparge matrices") {
        SutType sut1(constants::n_elements);
        sut1.add_edge(fixture.next_edge_id++, constants::v1_id, constants::v2_id);
        sut1.add_edge(fixture.next_edge_id++, constants::v2_id, constants::v3_id);

        SUBCASE("identical lists are equal") {
            const SutType sut2 = sut1;
            CHECK_EQ(sut1, sut2);
        }

        SUBCASE("lists with different sizes are not equal") {
            const SutType sut2{constants::n_elements + 1uz};
            CHECK_NE(sut1, sut2);
        }

        SUBCASE("lists with different connections are not equal") {
            SutType sut2 = sut1;
            sut2.add_edge(fixture.next_edge_id++, constants::v1_id, constants::v3_id);
            CHECK_NE(sut1, sut2);
        }

        SUBCASE("lists with different connection ids are not equal") {
            SutType sut2 = sut1;
            fixture.get(sut2)[constants::v1_id].front().edge_id = fixture.next_edge_id++;
            CHECK_NE(sut1, sut2);
        }
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    common_adj_list_template,
    gl::impl::adjacency_list<gl::list_graph_traits<gl::directed_t>>, // directed adj list
    gl::impl::adjacency_list<gl::list_graph_traits<gl::undirected_t>>, // undirected adj list
    gl::impl::adjacency_list<gl::flat_list_graph_traits<gl::directed_t>>, // directed flat adj list
    gl::impl::adjacency_list<gl::flat_list_graph_traits<gl::undirected_t>> // undirected flat adj list
);

namespace {

constexpr gl::size_type n_incident_edges_for_fully_connected_vertex = constants::n_elements - 1uz;

} // namespace

template <typename SutType>
struct test_directed_adjacency_list : public test_adjacency_list {
    using sut_type = SutType;
    using edge_type = typename sut_type::edge_type;

    edge_type add_edge(const auto source_id, const auto target_id) {
        const auto new_edge_id = this->next_edge_id++;
        sut.add_edge(new_edge_id, source_id, target_id);
        return edge_type{new_edge_id, source_id, target_id};
    }

    void fully_connect_vertex(const auto source_id, const bool no_loops = true) {
        for (const auto target_id : constants::vertex_id_view) {
            if (target_id == source_id and no_loops)
                continue;
            add_edge(source_id, target_id);
        }
    }

    void init_complete_graph(const bool no_loops = true) {
        for (const auto source_id : constants::vertex_id_view)
            fully_connect_vertex(source_id, no_loops);

        if (no_loops)
            REQUIRE(std::ranges::all_of(get(sut), [&](const auto& adj_items) {
                return adj_items.size() == n_incident_edges_for_fully_connected_vertex;
            }));
        else
            REQUIRE(std::ranges::all_of(get(sut), [&](const auto& adj_items) {
                return adj_items.size() == n_incident_edges_for_fully_connected_vertex + 1uz;
            }));
    }

    sut_type sut{constants::n_elements};
};

TEST_CASE_TEMPLATE_DEFINE("directed adjacency list tests", SutType, directed_adj_list_template) {
    using fixture_type = test_directed_adjacency_list<SutType>;
    using edge_type = typename fixture_type::edge_type;

    fixture_type fixture;
    auto& sut = fixture.sut;

    const auto size = [&fixture](const auto& sut) { return fixture.size(sut); };
    const auto add_edge = [&fixture](const auto source_id, const auto target_id) {
        return fixture.add_edge(source_id, target_id);
    };
    const auto fully_connect_vertex = [&fixture](const auto source_id, const bool no_loops = true) {
        fixture.fully_connect_vertex(source_id, no_loops);
    };
    const auto init_complete_graph = [&fixture](const bool no_loops = true) {
        fixture.init_complete_graph(no_loops);
    };

    SUBCASE("add_edge should add the edge only to the source vertex list") {
        const auto new_edge = add_edge(constants::v1_id, constants::v2_id);
        REQUIRE(new_edge.is_incident_from(constants::v1_id));
        REQUIRE(new_edge.is_incident_to(constants::v2_id));

        const auto incident_edges_1 = sut.incident_edges(constants::v1_id);
        CHECK_EQ(incident_edges_1.size(), 1uz);
        CHECK_EQ(sut.incident_edges(constants::v2_id).size(), 0uz);

        const auto& new_edge_extracted = incident_edges_1[0uz];
        CHECK_EQ(new_edge_extracted, new_edge);
    }

    SUBCASE("at should return the incident edges of a vertex") {
        init_complete_graph();
        for (const auto vertex_id : std::views::iota(constants::v1_id, constants::n_elements))
            CHECK(std::ranges::equal(sut.at(vertex_id), sut.incident_edges(vertex_id)));
    }

    SUBCASE("has_edge(id, id) should return true if there is an edge in the graph which connects "
            "vertices with the given ids in the specified direction") {
        add_edge(constants::v1_id, constants::v2_id);

        CHECK(sut.has_edge(constants::v1_id, constants::v2_id));
        CHECK_FALSE(sut.has_edge(constants::v2_id, constants::v1_id));
        CHECK_FALSE(sut.has_edge(constants::v1_id, constants::v3_id));
        CHECK_FALSE(sut.has_edge(constants::v2_id, constants::v3_id));
    }

    SUBCASE("has_edge(edge) should return true if the given edge is present in the graph") {
        const auto valid_edge = add_edge(constants::v1_id, constants::v2_id);
        CHECK(sut.has_edge(valid_edge));

        const edge_type invalid_edge{gl::invalid_id, constants::v1_id, constants::v2_id};
        CHECK_FALSE(sut.has_edge(invalid_edge));

        // edge connecting vertices not connected in the actual graph
        const edge_type not_present_edge{valid_edge.id(), constants::v2_id, constants::v3_id};
        CHECK_FALSE(sut.has_edge(not_present_edge));
    }

    SUBCASE("get_edge(id, id) should return nullopt if there is no edge connecting the given "
            "vertices") {
        CHECK_FALSE(sut.get_edge(constants::v1_id, constants::v2_id));
    }

    SUBCASE("get_edge(id, id) should return the first valid edge if the given vertices are "
            "connected") {
        const auto& edge_1 = add_edge(constants::v1_id, constants::v2_id);
        const auto& edge_2 = add_edge(constants::v1_id, constants::v2_id);

        const auto edge_opt = sut.get_edge(constants::v1_id, constants::v2_id);
        REQUIRE(edge_opt.has_value());
        CHECK_EQ(*edge_opt, edge_1);
        CHECK_NE(*edge_opt, edge_2);

        CHECK_FALSE(sut.get_edge(constants::v2_id, constants::v2_id));
    }

    SUBCASE("get_edges(id, id) should return an empty if there is no edge connecting the given "
            "vertices") {
        CHECK(sut.get_edges(constants::v1_id, constants::v2_id).empty());
    }

    SUBCASE("get_edges(id, id) should return a valid edge view if the given vertices are connected"
    ) {
        std::vector<edge_type> expected_edges;
        for (auto _ = 0uz; _ < constants::n_elements; _++)
            expected_edges.push_back(add_edge(constants::v1_id, constants::v2_id));

        CHECK(std::ranges::equal(
            sut.get_edges(constants::v1_id, constants::v2_id),
            expected_edges,
            std::ranges::equal_to{}
        ));

        CHECK(sut.get_edges(constants::v2_id, constants::v2_id).empty());
    }

    SUBCASE("remove_edge should throw when an edge is invalid") {
        // not existing edge between valid vertices
        const edge_type not_existing_edge{gl::invalid_id, constants::v1_id, constants::v2_id};
        CHECK_THROWS_AS(sut.remove_edge(not_existing_edge), std::invalid_argument);
    }

    SUBCASE("remove_edge should remove the edge from the source vertex's list") {
        fully_connect_vertex(constants::v1_id);

        auto incident_edges = sut.incident_edges(constants::v1_id);
        REQUIRE_EQ(incident_edges.size(), n_incident_edges_for_fully_connected_vertex);

        const auto& edge_to_remove = incident_edges[0uz];
        sut.remove_edge(edge_to_remove);

        incident_edges = sut.incident_edges(constants::v1_id);
        REQUIRE_EQ(incident_edges.size(), n_incident_edges_for_fully_connected_vertex - 1uz);
        // validate that the incident edges list has been properly aligned
        CHECK_EQ(std::ranges::find(incident_edges, edge_to_remove), incident_edges.end());
    }

    SUBCASE("in_edges should return edges where the vertex is the target") {
        const auto edge1 = add_edge(constants::v2_id, constants::v1_id);
        const auto edge2 = add_edge(constants::v3_id, constants::v1_id);

        const auto in_edges = sut.in_edges(constants::v1_id) | std::ranges::to<std::vector>();

        REQUIRE_EQ(in_edges.size(), 2uz);
        CHECK(std::ranges::contains(in_edges, edge1));
        CHECK(std::ranges::contains(in_edges, edge2));
    }

    SUBCASE("out_edges should return edges where the vertex is the source") {
        const auto edge1 = add_edge(constants::v1_id, constants::v2_id);
        const auto edge2 = add_edge(constants::v1_id, constants::v3_id);

        const auto out_edges = sut.out_edges(constants::v1_id) | std::ranges::to<std::vector>();

        REQUIRE_EQ(out_edges.size(), 2uz);
        CHECK(std::ranges::contains(out_edges, edge1));
        CHECK(std::ranges::contains(out_edges, edge2));
    }

    SUBCASE("{in/out}_degree should return the number of edges incident {to/from} the given vertex"
    ) {
        init_complete_graph();

        std::function<gl::size_type(const gl::default_id_type)> deg_proj;

        SUBCASE("in_degree") {
            deg_proj = [&sut](const auto vertex_id) { return sut.in_degree(vertex_id); };
        }

        SUBCASE("out_degree") {
            deg_proj = [&sut](const auto vertex_id) { return sut.out_degree(vertex_id); };
        }

        CAPTURE(deg_proj);

        CHECK(std::ranges::all_of(
            constants::vertex_id_view,
            [](const auto deg) { return deg == n_incident_edges_for_fully_connected_vertex; },
            deg_proj
        ));

        add_edge(constants::v1_id, constants::v1_id);

        CHECK_EQ(deg_proj(constants::v1_id), n_incident_edges_for_fully_connected_vertex + 1uz);
    }

    SUBCASE("degree should return the number of edges incident with the given vertex") {
        init_complete_graph();
        const auto deg_proj = [&sut](const auto vertex_id) { return sut.degree(vertex_id); };

        CHECK(std::ranges::all_of(
            constants::vertex_id_view,
            [](const auto deg) { return deg == 2uz * n_incident_edges_for_fully_connected_vertex; },
            deg_proj
        ));

        add_edge(constants::v1_id, constants::v1_id);

        CHECK_EQ(
            deg_proj(constants::v1_id), 2uz * (n_incident_edges_for_fully_connected_vertex + 1uz)
        );
    }

    SUBCASE("{in/out}_degree_map should return a map of numbers of edges incident {to/from} the "
            "corresponding vertices") {
        init_complete_graph(false);
        const auto expected_deg = constants::n_elements;

        std::vector<gl::size_type> degree_map;

        SUBCASE("in_degree") {
            degree_map = sut.in_degree_map();
        }

        SUBCASE("out_degree") {
            degree_map = sut.out_degree_map();
        }

        CAPTURE(degree_map);

        REQUIRE_EQ(degree_map.size(), constants::n_elements);
        CHECK_EQ(std::ranges::count(degree_map, expected_deg), constants::n_elements);
    }

    SUBCASE("degree_map should return a map of the numbers of edges incident with the "
            "corresponding "
            "vertices") {
        init_complete_graph(false);
        const auto expected_deg = constants::n_elements * 2uz;

        std::vector<gl::size_type> degree_map = sut.degree_map();

        REQUIRE_EQ(degree_map.size(), constants::n_elements);
        CHECK_EQ(std::ranges::count(degree_map, expected_deg), constants::n_elements);
    }

    SUBCASE("remove_vertex should remove the given vertex and all edges incident with it") {
        const auto edge1 = add_edge(constants::v1_id, constants::v2_id);
        const auto edge3 = add_edge(constants::v2_id, constants::v1_id);
        const auto edge2 = add_edge(constants::v1_id, constants::v3_id);
        const auto edge4 = add_edge(constants::v3_id, constants::v1_id);

        const auto edge5 = add_edge(constants::v2_id, constants::v3_id);
        const auto edge6 = add_edge(constants::v3_id, constants::v2_id);

        const auto removed_vertex_id = constants::v1_id;
        const auto removed_edge_ids = sut.remove_vertex(removed_vertex_id);

        constexpr gl::size_type n_removed_edges = 4uz;
        REQUIRE_EQ(removed_edge_ids.size(), n_removed_edges);
        for (const auto edge_id : {edge1.id(), edge2.id(), edge3.id(), edge4.id()})
            CHECK(std::ranges::contains(removed_edge_ids, edge_id));
        for (const auto edge_id : {edge5.id(), edge6.id()})
            CHECK_FALSE(std::ranges::contains(removed_edge_ids, edge_id));

        // Check the structure of the graph considering the aligned IDs
        CHECK_EQ(size(sut), constants::n_elements - 1uz);

        const auto adj_edges_1 = sut.incident_edges(constants::v1_id);
        CHECK_EQ(adj_edges_1.size(), 1uz);
        CHECK_EQ(adj_edges_1.front().id(), edge5.id() - n_removed_edges);
        CHECK_EQ(adj_edges_1.front().target(), constants::v2_id);

        const auto adj_edges_2 = sut.incident_edges(constants::v2_id);
        CHECK_EQ(adj_edges_2.size(), 1uz);
        CHECK_EQ(adj_edges_2.front().id(), edge6.id() - n_removed_edges);
        CHECK_EQ(adj_edges_2.front().target(), constants::v1_id);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    directed_adj_list_template,
    gl::impl::adjacency_list<gl::list_graph_traits<gl::directed_t>>, // normal adj list
    gl::impl::adjacency_list<gl::flat_list_graph_traits<gl::directed_t>> // flat adj list
);

template <typename SutType>
struct test_undirected_adjacency_list : public test_adjacency_list {
    using sut_type = SutType;
    using edge_type = typename sut_type::edge_type;

    edge_type add_edge(const auto source_id, const auto target_id) {
        const auto new_edge_id = this->next_edge_id++;
        sut.add_edge(new_edge_id, source_id, target_id);
        return edge_type{new_edge_id, source_id, target_id};
    }

    void fully_connect_vertex(const auto source_id, const bool no_loops = true) {
        for (const auto target_id : constants::vertex_id_view) {
            if (target_id == source_id and no_loops)
                continue;

            add_edge(source_id, target_id);
        }
    }

    void init_complete_graph(const bool no_loops = true) {
        for (const auto source_id : constants::vertex_id_view) {
            const auto bound = no_loops ? source_id : source_id + 1u;
            for (const auto target_id : std::views::iota(constants::v1_id, bound))
                add_edge(source_id, target_id);
        }

        if (no_loops)
            REQUIRE(std::ranges::all_of(get(sut), [&](const auto& adj_items) {
                return adj_items.size() == n_incident_edges_for_fully_connected_vertex;
            }));
        else
            REQUIRE(std::ranges::all_of(get(sut), [&](const auto& adj_items) {
                return adj_items.size() == n_incident_edges_for_fully_connected_vertex + 1uz;
            }));
    }

    sut_type sut{constants::n_elements};

    const gl::size_type n_unique_edges_in_full_graph =
        (n_incident_edges_for_fully_connected_vertex * constants::n_elements) / 2uz;
};

TEST_CASE_TEMPLATE_DEFINE("undirected adjacency list tests", SutType, undirected_adj_list_template) {
    using fixture_type = test_undirected_adjacency_list<SutType>;
    using edge_type = typename fixture_type::edge_type;

    fixture_type fixture;
    auto& sut = fixture.sut;

    const auto size = [&fixture](const auto& sut) { return fixture.size(sut); };
    const auto add_edge = [&fixture](const auto source_id, const auto target_id) {
        return fixture.add_edge(source_id, target_id);
    };
    const auto fully_connect_vertex = [&fixture](const auto source_id, const bool no_loops = true) {
        fixture.fully_connect_vertex(source_id, no_loops);
    };
    const auto init_complete_graph = [&fixture](const bool no_loops = true) {
        fixture.init_complete_graph(no_loops);
    };

    SUBCASE("add_edge should add the edge to the lists of both vertices") {
        const auto new_edge = add_edge(constants::v1_id, constants::v2_id);
        REQUIRE(new_edge.is_incident_from(constants::v1_id));
        REQUIRE(new_edge.is_incident_to(constants::v2_id));

        const auto incident_edges_1 = sut.incident_edges(constants::v1_id);
        const auto incident_edges_2 = sut.incident_edges(constants::v2_id);

        REQUIRE_EQ(incident_edges_1.size(), 1uz);
        REQUIRE_EQ(incident_edges_2.size(), 1uz);

        const auto& new_edge_extracted_1 = incident_edges_1[0uz];
        CHECK_EQ(new_edge_extracted_1, new_edge);

        const auto& new_edge_extracted_2 = incident_edges_2[0uz];
        CHECK_EQ(new_edge_extracted_2, new_edge);
    }

    SUBCASE("add_edge should add the edge once to the vertex list if the edge is a loop") {
        const auto new_edge = add_edge(constants::v1_id, constants::v1_id);
        REQUIRE(new_edge.is_loop());
        REQUIRE(new_edge.is_incident_from(constants::v1_id));

        const auto incident_edges = sut.incident_edges(constants::v1_id);
        REQUIRE_EQ(incident_edges.size(), 1uz);

        const auto& new_edge_extracted_1 = incident_edges[0uz];
        CHECK_EQ(new_edge_extracted_1, new_edge);
    }

    SUBCASE("at should return the incident edges of a vertex") {
        init_complete_graph();
        for (const auto vertex_id : std::views::iota(constants::v1_id, constants::n_elements))
            CHECK(std::ranges::equal(sut.at(vertex_id), sut.incident_edges(vertex_id)));
    }

    SUBCASE("has_edge(id, id) should return true if there is an edge in the graph which connects "
            "vertices with the given ids in any direction") {
        add_edge(constants::v1_id, constants::v2_id);

        CHECK(sut.has_edge(constants::v1_id, constants::v2_id));
        CHECK(sut.has_edge(constants::v2_id, constants::v1_id));
        CHECK_FALSE(sut.has_edge(constants::v1_id, constants::v3_id));
        CHECK_FALSE(sut.has_edge(constants::v2_id, constants::v3_id));
    }

    SUBCASE("has_edge(edge_ptr) should return true if the given edge is present in the graph") {
        const auto& valid_edge = add_edge(constants::v1_id, constants::v2_id);
        CHECK(sut.has_edge(valid_edge));

        const edge_type invalid_edge{gl::invalid_id, constants::v1_id, constants::v2_id};
        CHECK_FALSE(sut.has_edge(invalid_edge));

        // edge connecting vertices not connected in the actual graph
        const edge_type not_present_edge{valid_edge.id(), constants::v2_id, constants::v3_id};
        CHECK_FALSE(sut.has_edge(not_present_edge));
    }

    SUBCASE("get_edge(id, id) should return nullopt if there is no edge connecting the given "
            "vertices") {
        CHECK_FALSE(sut.get_edge(constants::v1_id, constants::v2_id));
    }

    SUBCASE("get_edge(id, id) should return the first valid edge if the given vertices are "
            "connected") {
        const auto edge_1 = add_edge(constants::v1_id, constants::v2_id);
        const auto edge_2 = add_edge(constants::v1_id, constants::v2_id);

        const auto edge_opt_1 = sut.get_edge(constants::v1_id, constants::v2_id);
        REQUIRE(edge_opt_1.has_value());
        CHECK_EQ(*edge_opt_1, edge_1);
        CHECK_NE(*edge_opt_1, edge_2);

        const auto edge_opt_2 = sut.get_edge(constants::v2_id, constants::v1_id);
        REQUIRE(edge_opt_2.has_value());
        CHECK_EQ(*edge_opt_2, edge_1);
        CHECK_NE(*edge_opt_2, edge_2);
    }

    SUBCASE("get_edges(id, id) should return an empty if there is no edge connecting the given "
            "vertices") {
        CHECK(sut.get_edges(constants::v1_id, constants::v2_id).empty());
    }

    SUBCASE("get_edges(id, id) should return a valid edge view if the given vertices are connected"
    ) {
        std::vector<edge_type> expected_edges;
        for (auto _ = 0uz; _ < constants::n_elements; _++)
            expected_edges.push_back(add_edge(constants::v1_id, constants::v2_id));

        CHECK(std::ranges::equal(
            sut.get_edges(constants::v1_id, constants::v2_id),
            expected_edges,
            std::ranges::equal_to{}
        ));

        CHECK(std::ranges::equal(
            sut.get_edges(constants::v2_id, constants::v1_id),
            expected_edges,
            std::ranges::equal_to{}
        ));
    }

    SUBCASE("remove_edge should throw when an edge is invalid") {
        // not existing edge between valid vertices
        const edge_type not_existing_edge{gl::invalid_id, constants::v1_id, constants::v2_id};
        CHECK_THROWS_AS(sut.remove_edge(not_existing_edge), std::invalid_argument);
    }

    SUBCASE("remove_edge should remove the edge from both the first and second vertices' list") {
        fully_connect_vertex(constants::v1_id);

        auto incident_edges_first = sut.incident_edges(constants::v1_id);
        REQUIRE_EQ(incident_edges_first.size(), n_incident_edges_for_fully_connected_vertex);

        const auto& edge_to_remove = incident_edges_first[0uz];

        const auto target_id = edge_to_remove.target();
        REQUIRE_EQ(sut.incident_edges(target_id).size(), 1uz);

        sut.remove_edge(edge_to_remove);

        // validate that the first incident edges list has been properly aligned
        incident_edges_first = sut.incident_edges(0uz);
        REQUIRE_EQ(incident_edges_first.size(), n_incident_edges_for_fully_connected_vertex - 1uz);
        CHECK_EQ(
            std::ranges::find(incident_edges_first, edge_to_remove), incident_edges_first.end()
        );

        // validate that the second adjacent edges list has been properly aligned
        const auto incident_edges_second = sut.incident_edges(target_id);
        REQUIRE_EQ(incident_edges_second.size(), 0uz);
        CHECK_EQ(
            std::ranges::find(incident_edges_second, edge_to_remove), incident_edges_second.end()
        );
    }

    SUBCASE("in_edges and out_edges should return the same edges for undirected graphs") {
        add_edge(constants::v1_id, constants::v2_id);
        add_edge(constants::v1_id, constants::v3_id);

        const auto in_edges = sut.in_edges(constants::v1_id) | std::ranges::to<std::vector>();
        const auto out_edges = sut.out_edges(constants::v1_id) | std::ranges::to<std::vector>();

        CHECK(std::ranges::equal(in_edges, sut.incident_edges(constants::v1_id)));
        CHECK(std::ranges::equal(out_edges, sut.incident_edges(constants::v1_id)));
    }

    SUBCASE("{in_/out_/}degree should return the number of edges incident {to/from} the given "
            "vertex") {
        init_complete_graph();

        std::function<gl::size_type(const gl::default_id_type)> deg_proj;

        SUBCASE("degree") {
            deg_proj = [&sut](const auto vertex_id) { return sut.degree(vertex_id); };
        }

        SUBCASE("in_degree") {
            deg_proj = [&sut](const auto vertex_id) { return sut.in_degree(vertex_id); };
        }

        SUBCASE("out_degree") {
            deg_proj = [&sut](const auto vertex_id) { return sut.out_degree(vertex_id); };
        }

        CAPTURE(deg_proj);

        CHECK(std::ranges::all_of(
            constants::vertex_id_view,
            [](const auto deg) { return deg == n_incident_edges_for_fully_connected_vertex; },
            deg_proj
        ));

        add_edge(constants::v1_id, constants::v1_id);

        CHECK_EQ(
            deg_proj(constants::v1_id),
            n_incident_edges_for_fully_connected_vertex + 2uz // loops counted twice
        );
    }

    SUBCASE("{in_/out_/}degree_map should return a map of numbers of edges incident {to/from/with} "
            "the "
            "corresponding vertices") {
        init_complete_graph(false);
        const auto expected_deg = constants::n_elements + 1;

        std::vector<gl::size_type> degree_map;

        SUBCASE("in_degree") {
            degree_map = sut.in_degree_map();
        }

        SUBCASE("out_degree") {
            degree_map = sut.out_degree_map();
        }

        SUBCASE("degree") {
            degree_map = sut.degree_map();
        }

        CAPTURE(degree_map);

        REQUIRE_EQ(degree_map.size(), constants::n_elements);
        CHECK_EQ(std::ranges::count(degree_map, expected_deg), constants::n_elements);
    }

    SUBCASE("remove_vertex should remove the given vertex and all edges incident with it") {
        const auto edge1 = add_edge(constants::v1_id, constants::v2_id);
        const auto edge3 = add_edge(constants::v2_id, constants::v1_id);
        const auto edge2 = add_edge(constants::v1_id, constants::v3_id);
        const auto edge4 = add_edge(constants::v3_id, constants::v1_id);

        const auto edge5 = add_edge(constants::v2_id, constants::v3_id);

        const auto removed_vertex_id = 0uz;
        const auto removed_edge_ids = sut.remove_vertex(removed_vertex_id);

        constexpr gl::size_type n_removed_edges = 4uz;
        REQUIRE_EQ(removed_edge_ids.size(), n_removed_edges);
        for (const auto edge_id : {edge1.id(), edge2.id(), edge3.id(), edge4.id()})
            CHECK(std::ranges::contains(removed_edge_ids, edge_id));
        CHECK_FALSE(std::ranges::contains(removed_edge_ids, edge5.id()));

        // Check the structure of the graph considering the aligned IDs
        CHECK_EQ(size(sut), constants::n_elements - 1uz);

        const auto adj_edges_1 = sut.incident_edges(constants::v1_id);
        CHECK_EQ(adj_edges_1.size(), 1uz);
        CHECK_EQ(adj_edges_1.front().id(), edge5.id() - n_removed_edges);
        CHECK_EQ(adj_edges_1.front().target(), constants::v2_id);

        const auto adj_edges_2 = sut.incident_edges(constants::v2_id);
        CHECK_EQ(adj_edges_2.size(), 1uz);
        CHECK_EQ(adj_edges_2.front().id(), edge5.id() - n_removed_edges);
        CHECK_EQ(adj_edges_2.front().target(), constants::v1_id);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    undirected_adj_list_template,
    gl::impl::adjacency_list<gl::list_graph_traits<gl::undirected_t>>, // normal adj list
    gl::impl::adjacency_list<gl::flat_list_graph_traits<gl::undirected_t>> // flat adj list
);

TEST_SUITE_END(); // test_adjacency_list

} // namespace gl_testing
