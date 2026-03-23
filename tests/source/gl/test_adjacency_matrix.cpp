#include "testing/gl/constants.hpp"
#include "testing/gl/functional.hpp"

#include <gl/graph_traits.hpp>
#include <gl/impl/adjacency_matrix.hpp>
#include <gl/util/ranges.hpp>

#include <doctest.h>

#include <algorithm>
#include <functional>

namespace gl_testing {

TEST_SUITE_BEGIN("test_adjacency_matrix");

struct test_adjacency_matrix {
    [[nodiscard]] auto& get(auto& sut) const {
        return sut._matrix;
    }

    gl::types::size_type size(const auto& sut) const {
        return sut._matrix.size();
    }

    gl::types::size_type next_edge_id = 0uz;
};

inline constexpr auto is_valid_id = [](const auto& id) { return id != constants::invalid_id; };

TEST_CASE_TEMPLATE_DEFINE(
    "directional_tag-independent tests", SutType, directional_tag_sut_template
) {
    test_adjacency_matrix fixture;

    SUBCASE("should be initialized with no vertices and no edges by default") {
        SutType sut{};
        CHECK_EQ(fixture.size(sut), constants::zero_elements);
    }

    SUBCASE("constructed with the n_vertices parameter should properly initialize the adjacency "
            "matrix") {
        SutType sut{constants::n_elements};
        REQUIRE_EQ(fixture.size(sut), constants::n_elements);
        CHECK(std::ranges::all_of(fixture.get(sut), [](const auto& matrix_row) {
            return std::ranges::count_if(matrix_row, is_valid_id) == constants::zero;
        }));
    }

    SUBCASE("add_vertex should properly extend the current adjacency matrix") {
        SutType sut{};
        constexpr gl::types::size_type target_n_vertices = constants::n_elements;

        for (gl::types::size_type n_vertices = constants::one_element;
             n_vertices <= target_n_vertices;
             n_vertices++) {
            sut.add_vertex();
            CHECK_EQ(fixture.size(sut), n_vertices);
        }

        CHECK_EQ(fixture.size(sut), target_n_vertices);
        CHECK(std::ranges::all_of(fixture.get(sut), [](const auto& matrix_row) {
            return std::ranges::count_if(matrix_row, is_valid_id) == constants::zero;
        }));
    }

    SUBCASE("add_vertices(n) should properly extend the current adjacency list") {
        SutType sut{};
        sut.add_vertices(constants::n_elements);

        CHECK_EQ(fixture.size(sut), constants::n_elements);
        CHECK(std::ranges::all_of(fixture.get(sut), [](const auto& matrix_row) {
            return std::ranges::count_if(matrix_row, is_valid_id) == constants::zero;
        }));
    }

    SUBCASE("add_edge should throw an error if the vertices are already incident") {
        using edge_type = typename SutType::edge_type;

        SutType sut{constants::n_elements};
        sut.add_edge(fixture.next_edge_id++, constants::vertex_id_1, constants::vertex_id_2);
        REQUIRE(sut.has_edge(constants::vertex_id_1, constants::vertex_id_2));

        CHECK_THROWS_AS(
            sut.add_edge(fixture.next_edge_id++, constants::vertex_id_1, constants::vertex_id_2),
            std::logic_error
        );
    }

    SUBCASE("add_edges_from should throw an error if the vertices are already incident") {
        using edge_type = typename SutType::edge_type;

        SutType sut{constants::n_elements};
        const auto target_ids = {
            constants::vertex_id_1, constants::vertex_id_2, constants::vertex_id_3
        };

        sut.add_edges_from(
            std::views::iota(0uz, target_ids.size()), constants::vertex_id_1, target_ids
        );

        REQUIRE(std::ranges::all_of(constants::vertex_id_view, [&sut](const auto target_id) {
            return sut.has_edge(constants::vertex_id_1, target_id);
        }));

        std::ranges::for_each(target_ids, [&sut, &fixture](const auto target_id) {
            CHECK_THROWS_AS(
                sut.add_edge(fixture.next_edge_id++, constants::vertex_id_1, target_id),
                std::logic_error
            );
        });
    }

    SUBCASE("equality operator should correctly comparge matrices") {
        SutType sut1(constants::n_elements);
        sut1.add_edge(fixture.next_edge_id++, constants::vertex_id_1, constants::vertex_id_2);
        sut1.add_edge(fixture.next_edge_id++, constants::vertex_id_2, constants::vertex_id_3);

        SUBCASE("identical matrices are equal") {
            const SutType sut2 = sut1;
            CHECK_EQ(sut1, sut2);
        }

        SUBCASE("matrices with different dimmensions are not equal") {
            const SutType sut2{constants::n_elements + 1uz};
            CHECK_NE(sut1, sut2);
        }

        SUBCASE("matrices with different connections are not equal") {
            SutType sut2 = sut1;
            sut2.add_edge(fixture.next_edge_id++, constants::vertex_id_1, constants::vertex_id_3);
            CHECK_NE(sut1, sut2);
        }

        SUBCASE("matrices with different connection ids are not equal") {
            SutType sut2 = sut1;
            fixture.get(sut2)[constants::vertex_id_1][constants::vertex_id_2] =
                fixture.next_edge_id++;
            CHECK_NE(sut1, sut2);
        }
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    directional_tag_sut_template,
    gl::impl::adjacency_matrix<gl::matrix_graph_traits<gl::directed_t>>, // directed adj list
    gl::impl::adjacency_matrix<gl::matrix_graph_traits<gl::undirected_t>> // undirected adj list
);

namespace {

constexpr gl::types::size_type n_incident_edges_for_fully_connected_vertex =
    constants::n_elements - constants::one_element;

} // namespace

struct test_directed_adjacency_matrix : public test_adjacency_matrix {
    using edge_type = gl::directed_edge<>;
    using sut_type = gl::impl::adjacency_matrix<gl::matrix_graph_traits<gl::directed_t>>;

    edge_type add_edge(const gl::types::id_type source_id, const gl::types::id_type target_id) {
        const auto new_edge_id = this->next_edge_id++;
        sut.add_edge(new_edge_id, source_id, target_id);
        return edge_type{new_edge_id, source_id, target_id};
    }

    void fully_connect_vertex(const gl::types::id_type source_id, const bool no_loops = true) {
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
            REQUIRE(std::ranges::all_of(get(sut), [&](const auto& matrix_row) {
                return std::ranges::count_if(matrix_row, is_valid_id)
                    == n_incident_edges_for_fully_connected_vertex;
            }));
        else
            REQUIRE(std::ranges::all_of(get(sut), [&](const auto& matrix_row) {
                return std::ranges::count_if(matrix_row, is_valid_id)
                    == n_incident_edges_for_fully_connected_vertex + constants::one;
            }));
    }

    sut_type sut{constants::n_elements};

    static constexpr gl::types::size_type n_unique_edges_in_full_graph =
        n_incident_edges_for_fully_connected_vertex * constants::n_elements;
};

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix, "add_edge should add the edge only to the source vertex list"
) {
    const auto new_edge = add_edge(constants::vertex_id_1, constants::vertex_id_2);

    REQUIRE(new_edge.is_incident_from(constants::vertex_id_1));
    REQUIRE(new_edge.is_incident_to(constants::vertex_id_2));

    auto adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
    CHECK_EQ(gl::util::range_size(adjacent_edges_1), constants::one_element);
    CHECK_EQ(
        gl::util::range_size(sut.adjacent_edges(constants::vertex_id_2)), constants::zero_elements
    );

    const auto& new_edge_extracted = *std::ranges::begin(adjacent_edges_1);
    CHECK_EQ(new_edge_extracted, new_edge);
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix,
    "at should return a view equivalent to the matrix row of the given vertex"
) {
    for (const auto vertex_id : std::views::iota(0uz, constants::n_elements)) {
        const auto edge = add_edge(vertex_id, (vertex_id + 1) % constants::n_elements);
        auto row_view = sut.at(vertex_id);

        REQUIRE_EQ(std::ranges::count_if(row_view, &edge_type::is_valid), 1uz);

        const auto edge_it = std::ranges::find_if(row_view, &edge_type::is_valid);
        REQUIRE_NE(edge_it, row_view.end());
        REQUIRE_EQ(*edge_it, edge);
    }
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix,
    "adjacent_edges should return a filtered view of edges adjacent with the given vertex"
) {
    const auto vertex_id = constants::vertex_id_1;
    const auto edge = add_edge(vertex_id, constants::vertex_id_2);
    auto adjacent_edges = sut.adjacent_edges(vertex_id);

    REQUIRE_EQ(gl::util::range_size(adjacent_edges), 1uz);
    CHECK_EQ(*std::ranges::begin(adjacent_edges), edge);
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix,
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
    test_directed_adjacency_matrix,
    "has_edge(edge_ptr) should return true if the given edge is present in the graph"
) {
    const auto valid_edge = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    CHECK(sut.has_edge(valid_edge));

    const edge_type invalid_edge{
        constants::invalid_id, constants::vertex_id_1, constants::vertex_id_2
    };
    CHECK_FALSE(sut.has_edge(invalid_edge));

    // edge connecting vertices not connected in the actual graph
    const edge_type not_present_edge{
        valid_edge.id(), constants::vertex_id_2, constants::vertex_id_3
    };
    CHECK_FALSE(sut.has_edge(not_present_edge));
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix,
    "get_edge(id, id) should return nullopt if there is no edge connecting the given vertices"
) {
    CHECK_FALSE(sut.get_edge(constants::vertex_id_1, constants::vertex_id_2));
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix,
    "get_edge(id, id) should return a valid edge if the given vertices are connected"
) {
    const auto edge_1 = add_edge(constants::vertex_id_1, constants::vertex_id_2);

    const auto edge_opt = sut.get_edge(constants::vertex_id_1, constants::vertex_id_2);
    REQUIRE(edge_opt.has_value());
    CHECK_EQ(*edge_opt, edge_1);

    CHECK_FALSE(sut.get_edge(constants::vertex_id_2, constants::vertex_id_2));
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix, "remove_edge should throw when an edge is invalid"
) {
    // not existing edge between valid vertices
    CHECK_THROWS_AS(
        sut.remove_edge(edge_type{next_edge_id++, constants::vertex_id_1, constants::vertex_id_2}),
        std::invalid_argument
    );
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix,
    "remove_edge should remove the edge from the source vertex's list"
) {
    fully_connect_vertex(constants::vertex_id_1);

    auto adjacent_edges = sut.adjacent_edges(constants::vertex_id_1);
    REQUIRE_EQ(gl::util::range_size(adjacent_edges), n_incident_edges_for_fully_connected_vertex);

    const auto edge_to_remove = *std::ranges::begin(adjacent_edges);
    sut.remove_edge(edge_to_remove);

    // validate that the adjacent edges list has been properly aligned
    adjacent_edges = sut.adjacent_edges(constants::vertex_id_1);
    REQUIRE_EQ(
        gl::util::range_size(adjacent_edges),
        n_incident_edges_for_fully_connected_vertex - constants::one_element
    );
    CHECK_EQ(std::ranges::find(adjacent_edges, edge_to_remove), adjacent_edges.end());
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix, "in_edges should return edges where the vertex is the target"
) {
    const auto edge1 = add_edge(constants::vertex_id_2, constants::vertex_id_1);
    const auto edge2 = add_edge(constants::vertex_id_3, constants::vertex_id_1);

    const auto in_edges = sut.in_edges(constants::vertex_id_1) | std::ranges::to<std::vector>();

    REQUIRE_EQ(in_edges.size(), 2uz);
    CHECK(std::ranges::contains(in_edges, edge1));
    CHECK(std::ranges::contains(in_edges, edge2));
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix, "out_edges should return edges where the vertex is the source"
) {
    const auto edge1 = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    const auto edge2 = add_edge(constants::vertex_id_1, constants::vertex_id_3);

    const auto out_edges = sut.out_edges(constants::vertex_id_1) | std::ranges::to<std::vector>();

    REQUIRE_EQ(out_edges.size(), 2uz);
    CHECK(std::ranges::contains(out_edges, edge1));
    CHECK(std::ranges::contains(out_edges, edge2));
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix,
    "{in/out}_degree should return the number of edges incident {to/from} the given vertex"
) {
    init_complete_graph();

    std::function<gl::types::size_type(const gl::types::id_type)> deg_proj;

    SUBCASE("in_degree") {
        deg_proj = [this](const auto vertex_id) { return sut.in_degree(vertex_id); };
    }

    SUBCASE("out_degree") {
        deg_proj = [this](const auto vertex_id) { return sut.out_degree(vertex_id); };
    }

    CAPTURE(deg_proj);

    CHECK(std::ranges::all_of(
        constants::vertex_id_view,
        [](const auto deg) { return deg == n_incident_edges_for_fully_connected_vertex; },
        deg_proj
    ));

    add_edge(constants::vertex_id_1, constants::vertex_id_1);

    CHECK_EQ(
        deg_proj(constants::vertex_id_1),
        n_incident_edges_for_fully_connected_vertex + constants::one
    );
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix,
    "degree should return the number of edges incident with the given vertex"
) {
    init_complete_graph();
    const auto deg_proj = [this](const auto vertex_id) { return sut.degree(vertex_id); };

    CHECK(std::ranges::all_of(
        constants::vertex_id_view,
        [](const auto deg) {
            return deg == constants::two * n_incident_edges_for_fully_connected_vertex;
        },
        deg_proj
    ));

    add_edge(constants::vertex_id_1, constants::vertex_id_1);

    CHECK_EQ(
        deg_proj(constants::vertex_id_1),
        constants::two * (n_incident_edges_for_fully_connected_vertex + constants::one)
    );
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix,
    "{in/out}_degree_map should return a map of numbers of edges incident {to/from} the "
    "corresponding vertices"
) {
    init_complete_graph(false);
    const auto expected_deg = constants::n_elements;

    std::vector<gl::types::id_type> degree_map;

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

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix,
    "degree_map should return a map of the numbers of edges incident with the corresponding "
    "vertices"
) {
    init_complete_graph(false);
    const auto expected_deg = constants::n_elements * constants::two;

    std::vector<gl::types::id_type> degree_map = sut.degree_map();

    REQUIRE_EQ(degree_map.size(), constants::n_elements);
    CHECK_EQ(std::ranges::count(degree_map, expected_deg), constants::n_elements);
}

TEST_CASE_FIXTURE(
    test_directed_adjacency_matrix,
    "remove_vertex should remove the given vertex and all edges incident with it"
) {
    const auto edge1 = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    const auto edge3 = add_edge(constants::vertex_id_2, constants::vertex_id_1);
    const auto edge2 = add_edge(constants::vertex_id_1, constants::vertex_id_3);
    const auto edge4 = add_edge(constants::vertex_id_3, constants::vertex_id_1);

    const auto edge5 = add_edge(constants::vertex_id_2, constants::vertex_id_3);
    const auto edge6 = add_edge(constants::vertex_id_3, constants::vertex_id_2);

    const auto removed_vertex_id = constants::vertex_id_1;
    const auto removed_edge_ids = sut.remove_vertex(removed_vertex_id);

    constexpr gl::types::size_type n_removed_edges = 4uz;
    REQUIRE_EQ(removed_edge_ids.size(), n_removed_edges);
    for (const auto edge_id : {edge1.id(), edge2.id(), edge3.id(), edge4.id()})
        CHECK(std::ranges::contains(removed_edge_ids, edge_id));
    for (const auto edge_id : {edge5.id(), edge6.id()})
        CHECK_FALSE(std::ranges::contains(removed_edge_ids, edge_id));

    // Check the structure of the graph considering the aligned IDs
    CHECK_EQ(size(sut), constants::n_elements - 1uz);

    auto adj_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
    CHECK_EQ(gl::util::range_size(adj_edges_1), 1uz);
    CHECK_EQ((*std::ranges::begin(adj_edges_1)).id(), edge5.id() - n_removed_edges);
    CHECK_EQ((*std::ranges::begin(adj_edges_1)).target(), constants::vertex_id_2);

    auto adj_edges_2 = sut.adjacent_edges(constants::vertex_id_2);
    CHECK_EQ(gl::util::range_size(adj_edges_2), 1uz);
    CHECK_EQ((*std::ranges::begin(adj_edges_2)).id(), edge6.id() - n_removed_edges);
    CHECK_EQ((*std::ranges::begin(adj_edges_2)).target(), constants::vertex_id_1);
}

struct test_undirected_adjacency_matrix : public test_adjacency_matrix {
    using edge_type = gl::undirected_edge<>;
    using sut_type = gl::impl::adjacency_matrix<gl::matrix_graph_traits<gl::undirected_t>>;

    edge_type add_edge(const gl::types::id_type source_id, const gl::types::id_type target_id) {
        const auto new_edge_id = this->next_edge_id++;
        sut.add_edge(new_edge_id, source_id, target_id);
        return edge_type{new_edge_id, source_id, target_id};
    }

    void fully_connect_vertex(const gl::types::id_type source_id, const bool no_loops = true) {
        for (const auto target_id : constants::vertex_id_view) {
            if (target_id == source_id and no_loops)
                continue;

            add_edge(source_id, target_id);
        }
    }

    void init_complete_graph(const bool no_loops = true) {
        for (const auto source_id : constants::vertex_id_view) {
            const auto bound = no_loops ? source_id : source_id + constants::one;
            for (const auto target_id : std::views::iota(constants::vertex_id_1, bound))
                add_edge(source_id, target_id);
        }

        if (no_loops)
            REQUIRE(std::ranges::all_of(get(sut), [&](const auto& matrix_row) {
                return std::ranges::count_if(matrix_row, is_valid_id)
                    == n_incident_edges_for_fully_connected_vertex;
            }));
        else
            REQUIRE(std::ranges::all_of(get(sut), [&](const auto& matrix_row) {
                return std::ranges::count_if(matrix_row, is_valid_id)
                    == n_incident_edges_for_fully_connected_vertex + constants::one;
            }));
    }

    sut_type sut{constants::n_elements};

    static constexpr gl::types::size_type n_unique_edges_in_full_graph =
        (n_incident_edges_for_fully_connected_vertex * constants::n_elements) / 2;
};

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix, "add_edge should add the edge to the lists of both vertices"
) {
    const auto new_edge = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    REQUIRE(new_edge.is_incident_from(constants::vertex_id_1));
    REQUIRE(new_edge.is_incident_to(constants::vertex_id_2));

    auto adjacent_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
    auto adjacent_edges_2 = sut.adjacent_edges(constants::vertex_id_2);

    REQUIRE_EQ(gl::util::range_size(adjacent_edges_1), constants::one_element);
    REQUIRE_EQ(gl::util::range_size(adjacent_edges_2), constants::one_element);

    const auto new_edge_extracted_1 = *std::ranges::begin(adjacent_edges_1);
    CHECK_EQ(new_edge_extracted_1, new_edge);

    const auto new_edge_extracted_2 = *std::ranges::begin(adjacent_edges_2);
    CHECK_EQ(new_edge_extracted_2, new_edge);
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
    "add_edge should add the edge once to the vertex list if the edge is a loop"
) {
    const auto new_edge = add_edge(constants::vertex_id_1, constants::vertex_id_1);
    REQUIRE(new_edge.is_loop());
    REQUIRE(new_edge.is_incident_from(constants::vertex_id_1));

    auto adjacent_edges = sut.adjacent_edges(constants::vertex_id_1);
    REQUIRE_EQ(gl::util::range_size(adjacent_edges), constants::one_element);

    const auto new_edge_extracted_1 = *std::ranges::begin(adjacent_edges);
    CHECK_EQ(new_edge_extracted_1, new_edge);
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
    "at should return a view equivalent to the matrix row of the given vertex"
) {
    const auto edge1 = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    const auto edge2 = add_edge(constants::vertex_id_2, constants::vertex_id_3);
    const auto edge3 = add_edge(constants::vertex_id_3, constants::vertex_id_1);

    auto v1_row_view = sut.at(constants::vertex_id_1);
    REQUIRE_EQ(std::ranges::count_if(v1_row_view, &edge_type::is_valid), 2uz);
    CHECK_EQ(v1_row_view[constants::vertex_id_2], edge1);
    CHECK_EQ(v1_row_view[constants::vertex_id_3], edge3);

    auto v2_row_view = sut.at(constants::vertex_id_2);
    REQUIRE_EQ(std::ranges::count_if(v2_row_view, &edge_type::is_valid), 2uz);
    CHECK_EQ(v2_row_view[constants::vertex_id_1], edge1);
    CHECK_EQ(v2_row_view[constants::vertex_id_3], edge2);

    auto v3_row_view = sut.at(constants::vertex_id_3);
    REQUIRE_EQ(std::ranges::count_if(v3_row_view, &edge_type::is_valid), 2uz);
    CHECK_EQ(v3_row_view[constants::vertex_id_1], edge3);
    CHECK_EQ(v3_row_view[constants::vertex_id_2], edge2);
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
    "adjacent_edges should return a filtered view of edges adjacent with the given vertex"
) {
    const auto vertex_id = constants::vertex_id_1;
    const auto edge = add_edge(vertex_id, constants::vertex_id_2);
    auto adjacent_edges = sut.adjacent_edges(vertex_id);

    REQUIRE_EQ(gl::util::range_size(adjacent_edges), 1uz);
    CHECK_EQ(*std::ranges::begin(adjacent_edges), edge);
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
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
    test_undirected_adjacency_matrix,
    "has_edge(edge_ptr) should return true if the given edge is present in the graph"
) {
    const auto valid_edge = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    CHECK(sut.has_edge(valid_edge));

    const edge_type invalid_edge{
        constants::invalid_id, constants::vertex_id_1, constants::vertex_id_2
    };
    CHECK_FALSE(sut.has_edge(invalid_edge));

    // edge connecting vertices not connected in the actual graph
    const edge_type not_present_edge{
        valid_edge.id(), constants::vertex_id_2, constants::vertex_id_3
    };
    CHECK_FALSE(sut.has_edge(not_present_edge));
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
    "get_edge(id, id) should return nullopt if there is no edge connecting the given vertices"
) {
    CHECK_FALSE(sut.get_edge(constants::vertex_id_1, constants::vertex_id_2));
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
    "get_edge(id, id) should return a valid edge if the given vertices are connected"
) {
    const auto edge_1 = add_edge(constants::vertex_id_1, constants::vertex_id_2);

    const auto edge_opt_1 = sut.get_edge(constants::vertex_id_1, constants::vertex_id_2);
    REQUIRE(edge_opt_1.has_value());
    CHECK_EQ(*edge_opt_1, edge_1);

    const auto edge_opt_2 = sut.get_edge(constants::vertex_id_2, constants::vertex_id_1);
    REQUIRE(edge_opt_2.has_value());
    CHECK_EQ(*edge_opt_2, edge_1);
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix, "remove_edge should throw when an edge is invalid"
) {
    // not existing edge between valid vertices
    CHECK_THROWS_AS(
        sut.remove_edge(edge_type{next_edge_id++, constants::vertex_id_1, constants::vertex_id_2}),
        std::invalid_argument
    );
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
    "remove_edge should remove the edge from both the first and second vertices' list"
) {
    fully_connect_vertex(constants::vertex_id_1);

    auto adjacent_edges_first = sut.adjacent_edges(constants::vertex_id_1);
    REQUIRE_EQ(
        gl::util::range_size(adjacent_edges_first), n_incident_edges_for_fully_connected_vertex
    );

    const auto edge_to_remove = *std::ranges::begin(adjacent_edges_first);

    const auto target_id = edge_to_remove.target();
    REQUIRE_EQ(gl::util::range_size(sut.adjacent_edges(target_id)), constants::one_element);

    sut.remove_edge(edge_to_remove);

    // validate that the first adjacent edges list has been properly aligned
    adjacent_edges_first = sut.adjacent_edges(constants::vertex_id_1);
    REQUIRE_EQ(
        gl::util::range_size(adjacent_edges_first),
        n_incident_edges_for_fully_connected_vertex - constants::one_element
    );
    CHECK_EQ(std::ranges::find(adjacent_edges_first, edge_to_remove), adjacent_edges_first.end());

    // validate that the second adjacent edges list has been properly aligned
    auto adjacent_edges_second = sut.adjacent_edges(target_id);
    REQUIRE_EQ(gl::util::range_size(adjacent_edges_second), constants::zero_elements);
    CHECK_EQ(std::ranges::find(adjacent_edges_second, edge_to_remove), adjacent_edges_second.end());
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
    "in_edges and out_edges should return the same edges for undirected graphs"
) {
    add_edge(constants::vertex_id_1, constants::vertex_id_2);
    add_edge(constants::vertex_id_1, constants::vertex_id_3);

    const auto in_edges = sut.in_edges(constants::vertex_id_1) | std::ranges::to<std::vector>();
    const auto out_edges = sut.out_edges(constants::vertex_id_1) | std::ranges::to<std::vector>();

    CHECK(std::ranges::equal(in_edges, sut.adjacent_edges(constants::vertex_id_1)));
    CHECK(std::ranges::equal(out_edges, sut.adjacent_edges(constants::vertex_id_1)));
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
    "{in_/out_/}degree should return the number of edges incident {to/from/with} the given vertex"
) {
    init_complete_graph();

    std::function<gl::types::size_type(const gl::types::id_type)> deg_proj;

    SUBCASE("degree") {
        deg_proj = [this](const auto vertex_id) { return sut.degree(vertex_id); };
    }

    SUBCASE("in_degree") {
        deg_proj = [this](const auto vertex_id) { return sut.in_degree(vertex_id); };
    }

    SUBCASE("out_degree") {
        deg_proj = [this](const auto vertex_id) { return sut.out_degree(vertex_id); };
    }

    CAPTURE(deg_proj);

    CHECK(std::ranges::all_of(
        constants::vertex_id_view,
        [](const auto deg) { return deg == n_incident_edges_for_fully_connected_vertex; },
        deg_proj
    ));

    add_edge(constants::vertex_id_1, constants::vertex_id_1);

    CHECK_EQ(
        deg_proj(constants::vertex_id_1),
        n_incident_edges_for_fully_connected_vertex + constants::two // loops counted twice
    );
}

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
    "{in_/out_/}degree_map should return a map of numbers of edges incident {to/from/with} the "
    "corresponding vertices"
) {
    init_complete_graph(false);
    const auto expected_deg = constants::n_elements + 1;

    std::vector<gl::types::id_type> degree_map;

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

TEST_CASE_FIXTURE(
    test_undirected_adjacency_matrix,
    "remove_vertex should remove the given vertex and all edges incident with it"
) {
    const auto edge1 = add_edge(constants::vertex_id_1, constants::vertex_id_2);
    const auto edge2 = add_edge(constants::vertex_id_1, constants::vertex_id_3);
    const auto edge3 = add_edge(constants::vertex_id_2, constants::vertex_id_3);

    const auto removed_vertex_id = constants::first_element_idx;
    const auto removed_edge_ids = sut.remove_vertex(removed_vertex_id);

    constexpr gl::types::size_type n_removed_edges = 2uz;
    REQUIRE_EQ(removed_edge_ids.size(), n_removed_edges);
    for (const auto edge_id : {edge1.id(), edge2.id()})
        CHECK(std::ranges::contains(removed_edge_ids, edge_id));
    CHECK_FALSE(std::ranges::contains(removed_edge_ids, edge3.id()));

    // Check the structure of the graph considering the aligned IDs
    CHECK_EQ(size(sut), constants::n_elements - 1uz);

    auto adj_edges_1 = sut.adjacent_edges(constants::vertex_id_1);
    CHECK_EQ(gl::util::range_size(adj_edges_1), 1uz);
    CHECK_EQ((*std::ranges::begin(adj_edges_1)).id(), edge3.id() - n_removed_edges);
    CHECK_EQ((*std::ranges::begin(adj_edges_1)).target(), constants::vertex_id_2);

    auto adj_edges_2 = sut.adjacent_edges(constants::vertex_id_2);
    CHECK_EQ(gl::util::range_size(adj_edges_2), 1uz);
    CHECK_EQ((*std::ranges::begin(adj_edges_2)).id(), edge3.id() - n_removed_edges);
    CHECK_EQ((*std::ranges::begin(adj_edges_2)).target(), constants::vertex_id_1);
}

TEST_SUITE_END(); // test_adjacency_matrix

} // namespace gl_testing
