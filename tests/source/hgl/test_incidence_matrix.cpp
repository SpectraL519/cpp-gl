#include "testing/hgl/constants.hpp"

#include <doctest.h>
#include <hgl/impl/incidence_matrix.hpp>

#include <algorithm>
#include <ranges>
#include <vector>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_incidence_matrix");

struct test_incidence_matrix {
    template <typename IncidenceMatrix>
    typename IncidenceMatrix::hypergraph_storage_type& matrix(IncidenceMatrix& sut) const noexcept {
        return sut._matrix;
    }
};

struct test_undirected_vertex_major_incidence_matrix : public test_incidence_matrix {
    using sut_type = hgl::impl::incidence_matrix<hgl::undirected_t, hgl::impl::vertex_major_t>;
};

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix, "should initialize empty matrix by default"
) {
    sut_type sut{};
    CHECK(matrix(sut).empty());
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix,
    "initialization with size parameters should properly initialize the matrix"
) {
    sut_type sut(constants::n_vertices, constants::n_hyperedges);
    CHECK_EQ(matrix(sut).size(), constants::n_vertices);
    CHECK(std::ranges::all_of(matrix(sut), [](const auto& row) {
        return row.size() == constants::n_hyperedges;
    }));
    CHECK(std::ranges::all_of(matrix(sut), [](const auto& row) {
        return std::ranges::none_of(row, [](bool bit) { return bit; });
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix, "add_vertices should properly extend matrix"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    const auto initial_size = matrix(sut).size();

    sut.add_vertices(2uz);

    CHECK_EQ(matrix(sut).size(), initial_size + 2uz);
    CHECK(std::ranges::all_of(matrix(sut), [](const auto& row) {
        return row.size() == constants::n_hyperedges;
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix,
    "remove_vertex should properly remove the row and implicitly shift vertex IDs"
) {
    constexpr hgl::types::size_type n_vertices = 5uz, n_hyperedges = 1uz;
    constexpr hgl::types::id_type hyperedge_id = constants::id1;

    sut_type sut{n_vertices, n_hyperedges};
    sut.bind(constants::id2, hyperedge_id);
    sut.bind(constants::id4, hyperedge_id);

    hgl::types::id_type rem_vid;
    hgl::types::size_type expected_hyperedge_size;
    std::vector<hgl::types::id_type> expected_vertices;

    SUBCASE("not present vertex < first incident vertex") {
        rem_vid = constants::id1;
        expected_hyperedge_size = 2uz;
        expected_vertices = {constants::id2 - 1uz, constants::id4 - 1uz};
    }

    SUBCASE("present vertex = first incident vertex") {
        rem_vid = constants::id2;
        expected_hyperedge_size = 1uz;
        expected_vertices = {constants::id4 - 1uz};
    }

    SUBCASE("not present vertex > first incident vertex") {
        rem_vid = constants::id3;
        expected_hyperedge_size = 2uz;
        expected_vertices = {constants::id2, constants::id4 - 1uz};
    }

    SUBCASE("present vertex = last incident vertex") {
        rem_vid = constants::id4;
        expected_hyperedge_size = 1uz;
        expected_vertices = {constants::id2};
    }

    SUBCASE("not present vertex > last incident vertex") {
        rem_vid = constants::id4 + 1uz;
        expected_hyperedge_size = 2uz;
        expected_vertices = {constants::id2, constants::id4};
    }

    CAPTURE(rem_vid);
    CAPTURE(expected_hyperedge_size);
    CAPTURE(expected_vertices);

    sut.remove_vertex(rem_vid);
    CHECK_EQ(matrix(sut).size(), n_vertices - 1uz);
    CHECK_EQ(sut.hyperedge_size(hyperedge_id), expected_hyperedge_size);
    CHECK(std::ranges::equal(sut.incident_vertices(hyperedge_id), expected_vertices));
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix,
    "incident_hyperedges should return a view of the vertex's incident hyperedge ids"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    constexpr auto vertex_id = constants::id1;
    REQUIRE(std::ranges::empty(sut.incident_hyperedges(vertex_id)));

    for (const auto hyperedge_id : constants::hyperedge_ids_view)
        sut.bind(vertex_id, hyperedge_id);

    CHECK(std::ranges::equal(sut.incident_hyperedges(vertex_id), constants::hyperedge_ids_view));
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix,
    "degree should return the number of the vertex's incident hyperedges"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    constexpr auto vertex_id = constants::id1;
    REQUIRE_EQ(sut.degree(vertex_id), 0uz);

    for (const auto hyperedge_id : constants::hyperedge_ids_view)
        sut.bind(vertex_id, hyperedge_id);

    CHECK_EQ(sut.degree(vertex_id), constants::n_hyperedges);
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix,
    "add_hyperedges should properly extend the hypergraph matrix (resize columns)"
) {
    sut_type sut{constants::n_vertices, 0uz};
    REQUIRE_EQ(matrix(sut).size(), constants::n_vertices);
    REQUIRE(std::ranges::all_of(matrix(sut), [](const auto& row) { return row.size() == 0uz; }));

    sut.add_hyperedges(constants::n_hyperedges);
    CHECK(std::ranges::all_of(matrix(sut), [](const auto& row) {
        return row.size() == constants::n_hyperedges;
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix,
    "remove_hyperedge should properly erase the proper hyperedge matrix entries (column)"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    sut.remove_hyperedge(constants::id1);

    CHECK_EQ(matrix(sut).size(), constants::n_vertices);
    CHECK(std::ranges::all_of(matrix(sut), [](const auto& row) {
        return row.size() == constants::n_hyperedges - 1uz;
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix,
    "remove_hyperedge should properly remove the column and implicitly shift hyperedge IDs"
) {
    constexpr hgl::types::size_type n_vertices = 1uz, n_hyperedges = 5uz;
    constexpr hgl::types::id_type vertex_id = constants::id1;

    sut_type sut{n_vertices, n_hyperedges};
    sut.bind(vertex_id, constants::id2);
    sut.bind(vertex_id, constants::id4);

    hgl::types::id_type rem_heid;
    hgl::types::size_type expected_vertex_degree;
    std::vector<hgl::types::id_type> expected_hyperedges;

    SUBCASE("not present hyperedge < first incident hyperedge") {
        rem_heid = constants::id1;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2 - 1uz, constants::id4 - 1uz};
    }

    SUBCASE("present hyperedge = first incident hyperedge") {
        rem_heid = constants::id2;
        expected_vertex_degree = 1uz;
        expected_hyperedges = {constants::id4 - 1uz};
    }

    SUBCASE("not present hyperedge > first incident hyperedge") {
        rem_heid = constants::id3;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2, constants::id4 - 1uz};
    }

    SUBCASE("present hyperedge = last incident hyperedge") {
        rem_heid = constants::id4;
        expected_vertex_degree = 1uz;
        expected_hyperedges = {constants::id2};
    }

    SUBCASE("not present hyperedge > last incident hyperedge") {
        rem_heid = constants::id4 + 1uz;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2, constants::id4};
    }

    CAPTURE(rem_heid);
    CAPTURE(expected_vertex_degree);
    CAPTURE(expected_hyperedges);

    sut.remove_hyperedge(rem_heid);
    CHECK_EQ(matrix(sut)[vertex_id].size(), n_hyperedges - 1uz);
    CHECK_EQ(sut.degree(vertex_id), expected_vertex_degree);
    CHECK(std::ranges::equal(sut.incident_hyperedges(vertex_id), expected_hyperedges));
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix,
    "incident_vertices should return an empty view by default"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    CHECK(std::ranges::all_of(constants::hyperedge_ids_view, [&sut](const auto hyperedge_id) {
        return std::ranges::empty(sut.incident_vertices(hyperedge_id));
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix, "hyperedge_size should return 0 by default"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    CHECK(std::ranges::all_of(constants::hyperedge_ids_view, [&sut](const auto hyperedge_id) {
        return sut.hyperedge_size(hyperedge_id) == 0uz;
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix, "bind should set the corresponding bit"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    REQUIRE(std::ranges::empty(sut.incident_vertices(constants::id1)));

    sut.bind(constants::id1, constants::id1);

    CHECK(matrix(sut)[constants::id1][constants::id1]);

    const auto vertices1 = sut.incident_vertices(constants::id1) | std::ranges::to<std::vector>();
    CHECK_EQ(sut.hyperedge_size(constants::id1), 1uz);
    CHECK_EQ(std::ranges::size(vertices1), 1uz);
    CHECK(std::ranges::contains(vertices1, constants::id1));

    sut.bind(constants::id1, constants::id1);
    const auto vertices2 = sut.incident_vertices(constants::id1) | std::ranges::to<std::vector>();
    CHECK_EQ(std::ranges::size(vertices2), 1uz);
    CHECK(std::ranges::equal(vertices2, vertices1));
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix, "unbind should clear the corresponding bit"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    sut.bind(constants::id1, constants::id1);
    REQUIRE_EQ(sut.hyperedge_size(constants::id1), 1uz);
    REQUIRE(matrix(sut)[constants::id1][constants::id1] == true);

    sut.unbind(constants::id1, constants::id2);
    CHECK_EQ(sut.hyperedge_size(constants::id1), 1uz);

    sut.unbind(constants::id1, constants::id1);
    CHECK(std::ranges::empty(sut.incident_vertices(constants::id1)));
    CHECK(matrix(sut)[constants::id1][constants::id1] == false);
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix,
    "are_bound should return true only when the bit is set"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    sut.bind(constants::id1, constants::id1);

    CHECK(sut.are_bound(constants::id1, constants::id1));
    CHECK_FALSE(sut.are_bound(constants::id1, constants::id2));
    CHECK_FALSE(sut.are_bound(constants::id2, constants::id1));
}

struct test_undirected_hyperedge_major_incidence_matrix : public test_incidence_matrix {
    using sut_type = hgl::impl::incidence_matrix<hgl::undirected_t, hgl::impl::hyperedge_major_t>;
};

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix, "should initialize empty matrix by default"
) {
    sut_type sut{};
    CHECK(matrix(sut).empty());
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix,
    "initialization with size parameters should properly initialize the matrix"
) {
    sut_type sut(constants::n_vertices, constants::n_hyperedges);
    CHECK_EQ(matrix(sut).size(), constants::n_hyperedges);
    CHECK(std::ranges::all_of(matrix(sut), [](const auto& row) {
        return row.size() == constants::n_vertices;
    }));
    CHECK(std::ranges::all_of(matrix(sut), [](const auto& row) {
        return std::ranges::none_of(row, [](bool bit) { return bit; });
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix,
    "add_vertices should properly extend the hypergraph matrix (resize columns)"
) {
    sut_type sut{0uz, constants::n_hyperedges};
    REQUIRE_EQ(matrix(sut).size(), constants::n_hyperedges);
    REQUIRE(std::ranges::all_of(matrix(sut), [](const auto& row) { return row.size() == 0uz; }));

    sut.add_vertices(constants::n_vertices);
    CHECK(std::ranges::all_of(matrix(sut), [](const auto& row) {
        return row.size() == constants::n_vertices;
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix,
    "remove_vertex should properly erase the proper vertex matrix entries (column)"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    sut.remove_vertex(constants::id1);

    CHECK_EQ(matrix(sut).size(), constants::n_hyperedges);
    CHECK(std::ranges::all_of(matrix(sut), [](const auto& row) {
        return row.size() == constants::n_vertices - 1uz;
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix,
    "remove_vertex should properly remove the column and implicitly shift vertex IDs"
) {
    constexpr hgl::types::size_type n_vertices = 5uz, n_hyperedges = 1uz;
    constexpr hgl::types::id_type hyperedge_id = constants::id1;

    sut_type sut{n_vertices, n_hyperedges};
    sut.bind(constants::id2, hyperedge_id);
    sut.bind(constants::id4, hyperedge_id);

    hgl::types::id_type rem_vid;
    hgl::types::size_type expected_hyperedge_size;
    std::vector<hgl::types::id_type> expected_vertices;

    SUBCASE("not present vertex < first incident vertex") {
        rem_vid = constants::id1;
        expected_hyperedge_size = 2uz;
        expected_vertices = {constants::id2 - 1uz, constants::id4 - 1uz};
    }

    SUBCASE("present vertex = first incident vertex") {
        rem_vid = constants::id2;
        expected_hyperedge_size = 1uz;
        expected_vertices = {constants::id4 - 1uz};
    }

    SUBCASE("not present vertex > first incident vertex") {
        rem_vid = constants::id3;
        expected_hyperedge_size = 2uz;
        expected_vertices = {constants::id2, constants::id4 - 1uz};
    }

    SUBCASE("present vertex = last incident vertex") {
        rem_vid = constants::id4;
        expected_hyperedge_size = 1uz;
        expected_vertices = {constants::id2};
    }

    SUBCASE("not present vertex > last incident vertex") {
        rem_vid = constants::id4 + 1uz;
        expected_hyperedge_size = 2uz;
        expected_vertices = {constants::id2, constants::id4};
    }

    CAPTURE(rem_vid);
    CAPTURE(expected_hyperedge_size);
    CAPTURE(expected_vertices);

    sut.remove_vertex(rem_vid);
    CHECK_EQ(matrix(sut)[hyperedge_id].size(), n_vertices - 1uz);
    CHECK_EQ(sut.hyperedge_size(hyperedge_id), expected_hyperedge_size);
    CHECK(std::ranges::equal(sut.incident_vertices(hyperedge_id), expected_vertices));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix,
    "incident_hyperedges should return a view of the vertex's incident hyperedge ids"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    constexpr auto vertex_id = constants::id1;
    REQUIRE(std::ranges::empty(sut.incident_hyperedges(vertex_id)));

    for (const auto hyperedge_id : constants::hyperedge_ids_view)
        sut.bind(vertex_id, hyperedge_id);

    CHECK(std::ranges::equal(sut.incident_hyperedges(vertex_id), constants::hyperedge_ids_view));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix,
    "degree should return the number of the vertex's incident hyperedges"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    constexpr auto vertex_id = constants::id1;
    REQUIRE_EQ(sut.degree(vertex_id), 0uz);

    for (const auto hyperedge_id : constants::hyperedge_ids_view)
        sut.bind(vertex_id, hyperedge_id);

    CHECK_EQ(sut.degree(vertex_id), constants::n_hyperedges);
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix, "add_hyperedges should properly extend matrix"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    const auto initial_size = matrix(sut).size();

    sut.add_hyperedges(2uz);

    CHECK_EQ(matrix(sut).size(), initial_size + 2uz);
    CHECK(std::ranges::all_of(matrix(sut), [](const auto& row) {
        return row.size() == constants::n_vertices;
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix,
    "remove_hyperedge should properly remove the row and implicitly shift hyperedge IDs"
) {
    constexpr hgl::types::size_type n_vertices = 1uz, n_hyperedges = 5uz;
    constexpr hgl::types::id_type vertex_id = constants::id1;

    sut_type sut{n_vertices, n_hyperedges};
    sut.bind(vertex_id, constants::id2);
    sut.bind(vertex_id, constants::id4);

    hgl::types::id_type rem_heid;
    hgl::types::size_type expected_vertex_degree;
    std::vector<hgl::types::id_type> expected_hyperedges;

    SUBCASE("not present hyperedge < first incident hyperedge") {
        rem_heid = constants::id1;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2 - 1uz, constants::id4 - 1uz};
    }

    SUBCASE("present hyperedge = first incident hyperedge") {
        rem_heid = constants::id2;
        expected_vertex_degree = 1uz;
        expected_hyperedges = {constants::id4 - 1uz};
    }

    SUBCASE("not present hyperedge > first incident hyperedge") {
        rem_heid = constants::id3;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2, constants::id4 - 1uz};
    }

    SUBCASE("present hyperedge = last incident hyperedge") {
        rem_heid = constants::id4;
        expected_vertex_degree = 1uz;
        expected_hyperedges = {constants::id2};
    }

    SUBCASE("not present hyperedge > last incident hyperedge") {
        rem_heid = constants::id4 + 1uz;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2, constants::id4};
    }

    CAPTURE(rem_heid);
    CAPTURE(expected_vertex_degree);
    CAPTURE(expected_hyperedges);

    sut.remove_hyperedge(rem_heid);
    CHECK_EQ(matrix(sut).size(), n_hyperedges - 1uz);
    CHECK_EQ(sut.degree(vertex_id), expected_vertex_degree);
    CHECK(std::ranges::equal(sut.incident_hyperedges(vertex_id), expected_hyperedges));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix,
    "incident_vertices should return an empty view by default"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    CHECK(std::ranges::all_of(constants::hyperedge_ids_view, [&sut](const auto hyperedge_id) {
        return std::ranges::empty(sut.incident_vertices(hyperedge_id));
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix, "hyperedge_size should return 0 by default"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    CHECK(std::ranges::all_of(constants::hyperedge_ids_view, [&sut](const auto hyperedge_id) {
        return sut.hyperedge_size(hyperedge_id) == 0uz;
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix, "bind should set the corresponding bit"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    REQUIRE(std::ranges::empty(sut.incident_vertices(constants::id1)));

    sut.bind(constants::id1, constants::id1);

    CHECK(matrix(sut)[constants::id1][constants::id1]);

    const auto vertices1 = sut.incident_vertices(constants::id1) | std::ranges::to<std::vector>();
    CHECK_EQ(sut.hyperedge_size(constants::id1), 1uz);
    CHECK_EQ(std::ranges::size(vertices1), 1uz);
    CHECK(std::ranges::contains(vertices1, constants::id1));

    sut.bind(constants::id1, constants::id1);
    const auto vertices2 = sut.incident_vertices(constants::id1) | std::ranges::to<std::vector>();
    CHECK_EQ(std::ranges::size(vertices2), 1uz);
    CHECK(std::ranges::equal(vertices2, vertices1));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix, "unbind should clear the corresponding bit"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    sut.bind(constants::id1, constants::id1);
    REQUIRE_EQ(sut.hyperedge_size(constants::id1), 1uz);
    REQUIRE(matrix(sut)[constants::id1][constants::id1] == true);

    sut.unbind(constants::id1, constants::id2);
    CHECK_EQ(sut.hyperedge_size(constants::id1), 1uz);

    sut.unbind(constants::id1, constants::id1);
    CHECK(std::ranges::empty(sut.incident_vertices(constants::id1)));
    CHECK(matrix(sut)[constants::id1][constants::id1] == false);
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix,
    "are_bound should return true only when the bit is set"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    sut.bind(constants::id1, constants::id1);

    CHECK(sut.are_bound(constants::id1, constants::id1));
    CHECK_FALSE(sut.are_bound(constants::id1, constants::id2));
    CHECK_FALSE(sut.are_bound(constants::id2, constants::id1));
}

TEST_SUITE_END(); // test_incidence_matrix

} // namespace hgl_testing
