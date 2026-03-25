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

    template <typename IncidenceMatrix>
    struct incidence_descriptor {
        using type = std::conditional_t<
            std::same_as<typename IncidenceMatrix::directional_tag, hgl::bf_directed_t>,
            typename IncidenceMatrix::incidence_type,
            bool>;
    };

    template <typename IncidenceMatrix>
    using incidence_descriptor_type = typename incidence_descriptor<IncidenceMatrix>::type;
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
    test_undirected_vertex_major_incidence_matrix, "add_vertices should properly extend the matrix"
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
    constexpr hgl::size_type n_vertices = 5uz, n_hyperedges = 1uz;
    constexpr hgl::id_type hyperedge_id = constants::id1;

    sut_type sut{n_vertices, n_hyperedges};
    sut.bind(constants::id2, hyperedge_id);
    sut.bind(constants::id4, hyperedge_id);

    hgl::id_type rem_vid;
    hgl::size_type expected_hyperedge_size;
    std::vector<hgl::id_type> expected_vertices;

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
    constexpr hgl::size_type n_vertices = 1uz, n_hyperedges = 5uz;
    constexpr hgl::id_type vertex_id = constants::id1;

    sut_type sut{n_vertices, n_hyperedges};
    sut.bind(vertex_id, constants::id2);
    sut.bind(vertex_id, constants::id4);

    hgl::id_type rem_eid;
    hgl::size_type expected_vertex_degree;
    std::vector<hgl::id_type> expected_hyperedges;

    SUBCASE("not present hyperedge < first incident hyperedge") {
        rem_eid = constants::id1;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2 - 1uz, constants::id4 - 1uz};
    }

    SUBCASE("present hyperedge = first incident hyperedge") {
        rem_eid = constants::id2;
        expected_vertex_degree = 1uz;
        expected_hyperedges = {constants::id4 - 1uz};
    }

    SUBCASE("not present hyperedge > first incident hyperedge") {
        rem_eid = constants::id3;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2, constants::id4 - 1uz};
    }

    SUBCASE("present hyperedge = last incident hyperedge") {
        rem_eid = constants::id4;
        expected_vertex_degree = 1uz;
        expected_hyperedges = {constants::id2};
    }

    SUBCASE("not present hyperedge > last incident hyperedge") {
        rem_eid = constants::id4 + 1uz;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2, constants::id4};
    }

    CAPTURE(rem_eid);
    CAPTURE(expected_vertex_degree);
    CAPTURE(expected_hyperedges);

    sut.remove_hyperedge(rem_eid);
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

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix,
    "element size map getters should return maps of properly calculated element sizes"
) {
    constexpr auto n_elements = 5ull;
    sut_type sut{n_elements, n_elements};

    constexpr auto is_zero = [](const auto& size) { return size == 0ull; };
    REQUIRE(std::ranges::all_of(sut.degree_map(n_elements), is_zero));
    REQUIRE(std::ranges::all_of(sut.hyperedge_size_map(n_elements), is_zero));

    for (std::size_t i = 0uz; i < n_elements; i++)
        for (std::size_t j = 0uz; j <= i; j++)
            sut.bind(i, j);

    const auto deg_map = sut.degree_map(n_elements);
    const auto esize_map = sut.hyperedge_size_map(n_elements);
    for (std::size_t i = 0uz; i < n_elements; i++) {
        CHECK_EQ(deg_map[i], i + 1uz);
        CHECK_EQ(esize_map[i], n_elements - i);
    }
}

TEST_CASE_FIXTURE(
    test_undirected_vertex_major_incidence_matrix,
    "equality operator should correctly compare undirected incidence matrices"
) {
    sut_type sut1{constants::n_vertices, constants::n_hyperedges};
    sut1.bind(constants::id1, constants::id1);
    sut1.bind(constants::id2, constants::id1);

    SUBCASE("identical matrices are equal") {
        const sut_type sut2 = sut1;
        CHECK_EQ(sut1, sut2);
    }

    SUBCASE("matrices with different bindings are not equal") {
        sut_type sut2 = sut1;
        sut2.bind(constants::id3, constants::id1);
        CHECK_NE(sut1, sut2);
    }

    SUBCASE("matrices with different dimensions are not equal") {
        sut_type sut2{constants::n_vertices + 1uz, constants::n_hyperedges};
        sut2.bind(constants::id1, constants::id1);
        sut2.bind(constants::id2, constants::id1);
        CHECK_NE(sut1, sut2);
    }
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
    constexpr hgl::size_type n_vertices = 5uz, n_hyperedges = 1uz;
    constexpr hgl::id_type hyperedge_id = constants::id1;

    sut_type sut{n_vertices, n_hyperedges};
    sut.bind(constants::id2, hyperedge_id);
    sut.bind(constants::id4, hyperedge_id);

    hgl::id_type rem_vid;
    hgl::size_type expected_hyperedge_size;
    std::vector<hgl::id_type> expected_vertices;

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
    constexpr hgl::size_type n_vertices = 1uz, n_hyperedges = 5uz;
    constexpr hgl::id_type vertex_id = constants::id1;

    sut_type sut{n_vertices, n_hyperedges};
    sut.bind(vertex_id, constants::id2);
    sut.bind(vertex_id, constants::id4);

    hgl::id_type rem_eid;
    hgl::size_type expected_vertex_degree;
    std::vector<hgl::id_type> expected_hyperedges;

    SUBCASE("not present hyperedge < first incident hyperedge") {
        rem_eid = constants::id1;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2 - 1uz, constants::id4 - 1uz};
    }

    SUBCASE("present hyperedge = first incident hyperedge") {
        rem_eid = constants::id2;
        expected_vertex_degree = 1uz;
        expected_hyperedges = {constants::id4 - 1uz};
    }

    SUBCASE("not present hyperedge > first incident hyperedge") {
        rem_eid = constants::id3;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2, constants::id4 - 1uz};
    }

    SUBCASE("present hyperedge = last incident hyperedge") {
        rem_eid = constants::id4;
        expected_vertex_degree = 1uz;
        expected_hyperedges = {constants::id2};
    }

    SUBCASE("not present hyperedge > last incident hyperedge") {
        rem_eid = constants::id4 + 1uz;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2, constants::id4};
    }

    CAPTURE(rem_eid);
    CAPTURE(expected_vertex_degree);
    CAPTURE(expected_hyperedges);

    sut.remove_hyperedge(rem_eid);
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

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix,
    "element size map getters should return maps of properly calculated element sizes"
) {
    constexpr auto n_elements = 5ull;
    sut_type sut{n_elements, n_elements};

    constexpr auto is_zero = [](const auto& size) { return size == 0ull; };
    REQUIRE(std::ranges::all_of(sut.degree_map(n_elements), is_zero));
    REQUIRE(std::ranges::all_of(sut.hyperedge_size_map(n_elements), is_zero));

    for (std::size_t i = 0uz; i < n_elements; i++)
        for (std::size_t j = 0uz; j <= i; j++)
            sut.bind(i, j);

    const auto deg_map = sut.degree_map(n_elements);
    const auto esize_map = sut.hyperedge_size_map(n_elements);
    for (std::size_t i = 0uz; i < n_elements; i++) {
        CHECK_EQ(deg_map[i], i + 1uz);
        CHECK_EQ(esize_map[i], n_elements - i);
    }
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_major_incidence_matrix,
    "equality operator should correctly compare undirected incidence matrices"
) {
    sut_type sut1{constants::n_vertices, constants::n_hyperedges};
    sut1.bind(constants::id1, constants::id1);
    sut1.bind(constants::id2, constants::id1);

    SUBCASE("identical matrices are equal") {
        const sut_type sut2 = sut1;
        CHECK_EQ(sut1, sut2);
    }

    SUBCASE("matrices with different bindings are not equal") {
        sut_type sut2 = sut1;
        sut2.bind(constants::id3, constants::id1);
        CHECK_NE(sut1, sut2);
    }

    SUBCASE("matrices with different dimensions are not equal") {
        sut_type sut2{constants::n_vertices, constants::n_hyperedges + 1uz};
        sut2.bind(constants::id1, constants::id1);
        sut2.bind(constants::id2, constants::id1);
        CHECK_NE(sut1, sut2);
    }
}

struct test_bf_directed_incidence_matrix : public test_incidence_matrix {
    template <typename SutType>
    auto is_incident_pred() {
        using incidence_type = incidence_descriptor_type<SutType>;
        return [](const incidence_type t) { return t != incidence_type::none; };
    }

    auto altbind_to_vertex(
        auto& sut, const hgl::id_type vertex_id, const hgl::size_type n_hyperedges
    ) {
        std::vector<hgl::id_type> tail_bound, head_bound;

        for (std::size_t i = 0uz; i < n_hyperedges; ++i) {
            if (i % 2 == 0) {
                sut.bind_tail(vertex_id, i);
                tail_bound.push_back(i);
            }
            else {
                sut.bind_head(vertex_id, i);
                head_bound.push_back(i);
            }
        }

        return std::make_pair(std::move(tail_bound), std::move(head_bound));
    }

    auto altbind_to_hyperedge(
        auto& sut, const hgl::id_type hyperedge_id, const hgl::size_type n_vertices
    ) {
        std::vector<hgl::id_type> tail_bound, head_bound;

        for (std::size_t i = 0uz; i < n_vertices; ++i) {
            if (i % 2 == 0) {
                sut.bind_tail(i, hyperedge_id);
                tail_bound.push_back(i);
            }
            else {
                sut.bind_head(i, hyperedge_id);
                head_bound.push_back(i);
            }
        }

        return std::make_pair(std::move(tail_bound), std::move(head_bound));
    }
};

struct test_bf_directed_vertex_major_incidence_matrix : public test_bf_directed_incidence_matrix {
    using sut_type = hgl::impl::incidence_matrix<hgl::bf_directed_t, hgl::impl::vertex_major_t>;
    using incidence_type = incidence_descriptor_type<sut_type>;
};

TEST_CASE_FIXTURE(
    test_bf_directed_vertex_major_incidence_matrix, "should initialize empty matrix by default"
) {
    sut_type sut{};
    CHECK(matrix(sut).empty());
}

TEST_CASE_FIXTURE(
    test_bf_directed_vertex_major_incidence_matrix,
    "initialization with size parameters should properly initialize the matrix"
) {
    sut_type sut(constants::n_vertices, constants::n_hyperedges);
    CHECK_EQ(matrix(sut).size(), constants::n_vertices);
    CHECK(std::ranges::all_of(matrix(sut), [](const auto& row) {
        return row.size() == constants::n_hyperedges;
    }));
    CHECK(std::ranges::all_of(matrix(sut), [this](const auto& row) {
        return std::ranges::none_of(row, is_incident_pred<sut_type>());
    }));
}

TEST_CASE_FIXTURE(
    test_bf_directed_vertex_major_incidence_matrix, "add_vertices should properly extend the matrix"
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
    test_bf_directed_vertex_major_incidence_matrix,
    "remove_vertex should properly remove the row and implicitly shift vertex IDs"
) {
    constexpr hgl::size_type n_vertices = 5uz, n_hyperedges = 1uz;
    constexpr hgl::id_type hyperedge_id = constants::id1;

    sut_type sut{n_vertices, n_hyperedges};
    sut.bind_tail(constants::id2, hyperedge_id);
    sut.bind_head(constants::id4, hyperedge_id);

    hgl::id_type rem_vid;
    hgl::size_type expected_hyperedge_size;
    std::vector<hgl::id_type> expected_vertices;

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
    test_bf_directed_vertex_major_incidence_matrix,
    "incident_hyperedges should return a view of the vertex's incident hyperedge ids,"
    "out_hyperedges should return a view of the vertex's outgoing hyperedge ids (v in T(e)),"
    "in_hyperedges should return a view of the vertex's incoming hyperedge ids (v in H(e))"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    constexpr auto vertex_id = constants::id1;
    REQUIRE(std::ranges::empty(sut.incident_hyperedges(vertex_id)));

    const auto [tail_bound_hyperedges, head_bound_hyperedges] =
        altbind_to_vertex(sut, vertex_id, constants::n_hyperedges);

    CHECK(std::ranges::equal(sut.incident_hyperedges(vertex_id), constants::hyperedge_ids_view));
    CHECK(std::ranges::equal(sut.out_hyperedges(vertex_id), tail_bound_hyperedges));
    CHECK(std::ranges::equal(sut.in_hyperedges(vertex_id), head_bound_hyperedges));
}

TEST_CASE_FIXTURE(
    test_bf_directed_vertex_major_incidence_matrix,
    "degree should return the number of the vertex's incident hyperedges,"
    "out_degree should return the number of the vertex's outgoing hyperedges (v in T(e)),"
    "in_degree should return the number of the vertex's incoming hyperedges (v in H(e))"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    constexpr auto vertex_id = constants::id1;
    REQUIRE_EQ(sut.degree(vertex_id), 0uz);

    const auto [tail_bound_hyperedges, head_bound_hyperedges] =
        altbind_to_vertex(sut, vertex_id, constants::n_hyperedges);

    CHECK_EQ(sut.degree(vertex_id), constants::n_hyperedges);
    CHECK_EQ(sut.out_degree(vertex_id), tail_bound_hyperedges.size());
    CHECK_EQ(sut.in_degree(vertex_id), head_bound_hyperedges.size());
}

TEST_CASE_FIXTURE(
    test_bf_directed_vertex_major_incidence_matrix,
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
    test_bf_directed_vertex_major_incidence_matrix,
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
    test_bf_directed_vertex_major_incidence_matrix,
    "remove_hyperedge should properly remove the column and implicitly shift hyperedge IDs"
) {
    constexpr hgl::size_type n_vertices = 1uz, n_hyperedges = 5uz;
    constexpr hgl::id_type vertex_id = constants::id1;

    sut_type sut{n_vertices, n_hyperedges};
    sut.bind_tail(vertex_id, constants::id2);
    sut.bind_head(vertex_id, constants::id4);

    hgl::id_type rem_eid;
    hgl::size_type expected_vertex_degree;
    std::vector<hgl::id_type> expected_hyperedges;

    SUBCASE("not present hyperedge < first incident hyperedge") {
        rem_eid = constants::id1;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2 - 1uz, constants::id4 - 1uz};
    }

    SUBCASE("present hyperedge = first incident hyperedge") {
        rem_eid = constants::id2;
        expected_vertex_degree = 1uz;
        expected_hyperedges = {constants::id4 - 1uz};
    }

    SUBCASE("not present hyperedge > first incident hyperedge") {
        rem_eid = constants::id3;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2, constants::id4 - 1uz};
    }

    SUBCASE("present hyperedge = last incident hyperedge") {
        rem_eid = constants::id4;
        expected_vertex_degree = 1uz;
        expected_hyperedges = {constants::id2};
    }

    SUBCASE("not present hyperedge > last incident hyperedge") {
        rem_eid = constants::id4 + 1uz;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2, constants::id4};
    }

    CAPTURE(rem_eid);
    CAPTURE(expected_vertex_degree);
    CAPTURE(expected_hyperedges);

    sut.remove_hyperedge(rem_eid);
    CHECK_EQ(matrix(sut)[vertex_id].size(), n_hyperedges - 1uz);
    CHECK_EQ(sut.degree(vertex_id), expected_vertex_degree);
    CHECK(std::ranges::equal(sut.incident_hyperedges(vertex_id), expected_hyperedges));
}

TEST_CASE_FIXTURE(
    test_bf_directed_vertex_major_incidence_matrix,
    "incident_vertices should return a view of the hyperedge's incident vertex ids, "
    "tail_vertices should return a view of the hyperedge's tail vertex ids: T(e), "
    "head_vertices should return a view of the hyperedge's head vertex ids: H(e)"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    constexpr auto hyperedge_id = constants::id1;
    REQUIRE(std::ranges::empty(sut.incident_vertices(hyperedge_id)));

    const auto [tail_bound_vertices, head_bound_vertices] =
        altbind_to_hyperedge(sut, hyperedge_id, constants::n_vertices);

    CHECK(std::ranges::equal(sut.incident_vertices(hyperedge_id), constants::vertex_ids_view));
    CHECK(std::ranges::equal(sut.tail_vertices(hyperedge_id), tail_bound_vertices));
    CHECK(std::ranges::equal(sut.head_vertices(hyperedge_id), head_bound_vertices));
}

TEST_CASE_FIXTURE(
    test_bf_directed_vertex_major_incidence_matrix,
    "hyperedge_size should return the number of the hyperedge's incident vertices, "
    "tail_size should return the number of the hyperedge's tail vertices: |T(e)|, "
    "head_size should return the number of the hyperedge's head vertices: |H(e)|"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    constexpr auto hyperedge_id = constants::id1;
    REQUIRE_EQ(sut.hyperedge_size(hyperedge_id), 0uz);

    const auto [tail_bound_vertices, head_bound_vertices] =
        altbind_to_hyperedge(sut, hyperedge_id, constants::n_vertices);

    CHECK_EQ(sut.hyperedge_size(hyperedge_id), constants::n_vertices);
    CHECK_EQ(sut.tail_size(hyperedge_id), tail_bound_vertices.size());
    CHECK_EQ(sut.head_size(hyperedge_id), head_bound_vertices.size());
}

TEST_CASE_FIXTURE(
    test_bf_directed_vertex_major_incidence_matrix,
    "bind_tail should set the corresponding matrix entry to backward incidence"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    REQUIRE(std::ranges::empty(sut.incident_vertices(constants::id1)));

    sut.bind_tail(constants::id1, constants::id1);

    CHECK_EQ(matrix(sut)[constants::id1][constants::id1], incidence_type::backward);

    const auto vertices = sut.tail_vertices(constants::id1) | std::ranges::to<std::vector>();
    CHECK_EQ(sut.tail_size(constants::id1), 1uz);
    CHECK_EQ(std::ranges::size(vertices), 1uz);
    CHECK(std::ranges::contains(vertices, constants::id1));
}

TEST_CASE_FIXTURE(
    test_bf_directed_vertex_major_incidence_matrix,
    "bind_head should set the corresponding matrix entry to forward incidence"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    REQUIRE(std::ranges::empty(sut.incident_vertices(constants::id1)));

    sut.bind_head(constants::id1, constants::id1);

    CHECK_EQ(matrix(sut)[constants::id1][constants::id1], incidence_type::forward);

    const auto vertices = sut.head_vertices(constants::id1) | std::ranges::to<std::vector>();
    CHECK_EQ(sut.head_size(constants::id1), 1uz);
    CHECK_EQ(std::ranges::size(vertices), 1uz);
    CHECK(std::ranges::contains(vertices, constants::id1));
}

TEST_CASE_FIXTURE(
    test_bf_directed_vertex_major_incidence_matrix, "unbind should clear the corresponding bit"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    SUBCASE("tail bound") {
        sut.bind_tail(constants::id1, constants::id1);
    }
    SUBCASE("head bound") {
        sut.bind_head(constants::id1, constants::id1);
    }
    CAPTURE(sut);

    REQUIRE_EQ(sut.hyperedge_size(constants::id1), 1uz);
    REQUIRE_NE(matrix(sut)[constants::id1][constants::id1], incidence_type::none);

    sut.unbind(constants::id1, constants::id2);
    CHECK_EQ(sut.hyperedge_size(constants::id1), 1uz);

    sut.unbind(constants::id1, constants::id1);
    CHECK(std::ranges::empty(sut.incident_vertices(constants::id1)));
    CHECK_EQ(matrix(sut)[constants::id1][constants::id1], incidence_type::none);
}

TEST_CASE_FIXTURE(
    test_bf_directed_vertex_major_incidence_matrix,
    "are_bound, is_tail, is_head should return true only when the corresponding matrix entry is "
    "set to a valid, matching incidence type"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    sut.bind_tail(constants::id1, constants::id1);
    sut.bind_head(constants::id2, constants::id1);

    CHECK(sut.are_bound(constants::id1, constants::id1));
    CHECK(sut.is_tail(constants::id1, constants::id1));
    CHECK_FALSE(sut.is_head(constants::id1, constants::id1));

    CHECK(sut.are_bound(constants::id2, constants::id1));
    CHECK_FALSE(sut.is_tail(constants::id2, constants::id1));
    CHECK(sut.is_head(constants::id2, constants::id1));

    CHECK_FALSE(sut.are_bound(constants::id3, constants::id1));
    CHECK_FALSE(sut.is_tail(constants::id3, constants::id1));
    CHECK_FALSE(sut.is_head(constants::id3, constants::id1));
}

TEST_CASE_FIXTURE(
    test_bf_directed_vertex_major_incidence_matrix,
    "element size map getters should return maps of properly calculated element sizes"
) {
    constexpr auto n_elements = 5ull;
    sut_type sut{n_elements, n_elements};

    constexpr auto is_zero = [](const auto& size) { return size == 0ull; };
    REQUIRE(std::ranges::all_of(sut.degree_map(n_elements), is_zero));
    REQUIRE(std::ranges::all_of(sut.out_degree_map(n_elements), is_zero));
    REQUIRE(std::ranges::all_of(sut.in_degree_map(n_elements), is_zero));
    REQUIRE(std::ranges::all_of(sut.hyperedge_size_map(n_elements), is_zero));
    REQUIRE(std::ranges::all_of(sut.tail_size_map(n_elements), is_zero));
    REQUIRE(std::ranges::all_of(sut.head_size_map(n_elements), is_zero));

    SUBCASE("tail bind") {
        for (std::size_t i = 0uz; i < n_elements; i++)
            for (std::size_t j = 0uz; j <= i; j++)
                sut.bind_tail(i, j);

        const auto deg_map = sut.degree_map(n_elements);
        const auto out_deg_map = sut.out_degree_map(n_elements);
        const auto esize_map = sut.hyperedge_size_map(n_elements);
        const auto tsize_map = sut.tail_size_map(n_elements);
        for (std::size_t i = 0uz; i < n_elements; i++) {
            CHECK_EQ(deg_map[i], i + 1uz);
            CHECK_EQ(out_deg_map[i], i + 1uz);
            CHECK_EQ(esize_map[i], n_elements - i);
            CHECK_EQ(tsize_map[i], n_elements - i);
        }
        CHECK(std::ranges::all_of(sut.in_degree_map(n_elements), is_zero));
        CHECK(std::ranges::all_of(sut.head_size_map(n_elements), is_zero));
    }

    SUBCASE("head bind") {
        for (std::size_t i = 0uz; i < n_elements; i++)
            for (std::size_t j = 0uz; j <= i; j++)
                sut.bind_head(i, j);

        const auto deg_map = sut.degree_map(n_elements);
        const auto in_deg_map = sut.in_degree_map(n_elements);
        const auto esize_map = sut.hyperedge_size_map(n_elements);
        const auto hsize_map = sut.head_size_map(n_elements);
        for (std::size_t i = 0uz; i < n_elements; i++) {
            CHECK_EQ(deg_map[i], i + 1uz);
            CHECK_EQ(in_deg_map[i], i + 1uz);
            CHECK_EQ(esize_map[i], n_elements - i);
            CHECK_EQ(hsize_map[i], n_elements - i);
        }
        CHECK(std::ranges::all_of(sut.out_degree_map(n_elements), is_zero));
        CHECK(std::ranges::all_of(sut.tail_size_map(n_elements), is_zero));
    }

    // diagonal = tail, everything else is head
    for (std::size_t i = 0uz; i < n_elements; i++) {
        for (std::size_t j = 0uz; j <= i; j++) {
            if (i == j)
                sut.bind_tail(i, j);
            else
                sut.bind_head(i, j);
        }
    }

    const auto deg_map = sut.degree_map(n_elements);
    const auto out_deg_map = sut.out_degree_map(n_elements);
    const auto in_deg_map = sut.in_degree_map(n_elements);

    const auto esize_map = sut.hyperedge_size_map(n_elements);
    const auto tsize_map = sut.tail_size_map(n_elements);
    const auto hsize_map = sut.head_size_map(n_elements);

    for (std::size_t i = 0uz; i < n_elements; i++) {
        CHECK_EQ(deg_map[i], i + 1uz);
        CHECK_EQ(out_deg_map[i], 1uz);
        CHECK_EQ(in_deg_map[i], i);

        CHECK_EQ(esize_map[i], n_elements - i);
        CHECK_EQ(tsize_map[i], 1uz);
        CHECK_EQ(hsize_map[i], n_elements - i - 1uz);
    }
}

TEST_CASE_FIXTURE(
    test_bf_directed_vertex_major_incidence_matrix,
    "equality operator should correctly compare directed incidence matrices"
) {
    sut_type sut1{constants::n_vertices, constants::n_hyperedges};
    sut1.bind_tail(constants::id1, constants::id1);
    sut1.bind_head(constants::id2, constants::id1);

    SUBCASE("identical matrices are equal") {
        const sut_type sut2 = sut1;
        CHECK_EQ(sut1, sut2);
    }

    SUBCASE("matrices with different bindings are not equal") {
        sut_type sut2 = sut1;
        sut2.bind_tail(constants::id3, constants::id1);
        CHECK_NE(sut1, sut2);
    }

    SUBCASE("matrices with swapped tail/head bindings are not equal") {
        sut_type sut2{constants::n_vertices, constants::n_hyperedges};
        sut2.bind_head(constants::id1, constants::id1);
        sut2.bind_tail(constants::id2, constants::id1);
        CHECK_NE(sut1, sut2);
    }

    SUBCASE("matrices with different dimensions are not equal") {
        sut_type sut2{constants::n_vertices + 1uz, constants::n_hyperedges};
        sut2.bind_tail(constants::id1, constants::id1);
        sut2.bind_head(constants::id2, constants::id1);
        CHECK_NE(sut1, sut2);
    }
}

struct test_bf_directed_hyperedge_major_incidence_matrix
: public test_bf_directed_incidence_matrix {
    using sut_type = hgl::impl::incidence_matrix<hgl::bf_directed_t, hgl::impl::hyperedge_major_t>;
    using incidence_type = incidence_descriptor_type<sut_type>;
};

TEST_CASE_FIXTURE(
    test_bf_directed_hyperedge_major_incidence_matrix, "should initialize empty matrix by default"
) {
    sut_type sut{};
    CHECK(matrix(sut).empty());
}

TEST_CASE_FIXTURE(
    test_bf_directed_hyperedge_major_incidence_matrix,
    "initialization with size parameters should properly initialize the matrix"
) {
    sut_type sut(constants::n_vertices, constants::n_hyperedges);
    CHECK_EQ(matrix(sut).size(), constants::n_hyperedges);
    CHECK(std::ranges::all_of(matrix(sut), [](const auto& row) {
        return row.size() == constants::n_vertices;
    }));
    CHECK(std::ranges::all_of(matrix(sut), [this](const auto& row) {
        return std::ranges::none_of(row, is_incident_pred<sut_type>());
    }));
}

TEST_CASE_FIXTURE(
    test_bf_directed_hyperedge_major_incidence_matrix,
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
    test_bf_directed_hyperedge_major_incidence_matrix,
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
    test_bf_directed_hyperedge_major_incidence_matrix,
    "remove_vertex should properly remove the column and implicitly shift vertex IDs"
) {
    constexpr hgl::size_type n_vertices = 5uz, n_hyperedges = 1uz;
    constexpr hgl::id_type hyperedge_id = constants::id1;

    sut_type sut{n_vertices, n_hyperedges};
    sut.bind_tail(constants::id2, hyperedge_id);
    sut.bind_head(constants::id4, hyperedge_id);

    hgl::id_type rem_vid;
    hgl::size_type expected_hyperedge_size;
    std::vector<hgl::id_type> expected_vertices;

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
    test_bf_directed_hyperedge_major_incidence_matrix,
    "incident_hyperedges should return a view of the vertex's incident hyperedge ids,"
    "out_hyperedges should return a view of the vertex's outgoing hyperedge ids (v in T(e)),"
    "in_hyperedges should return a view of the vertex's incoming hyperedge ids (v in H(e))"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    constexpr auto vertex_id = constants::id1;
    REQUIRE(std::ranges::empty(sut.incident_hyperedges(vertex_id)));

    const auto [tail_bound_hyperedges, head_bound_hyperedges] =
        altbind_to_vertex(sut, vertex_id, constants::n_hyperedges);

    CHECK(std::ranges::equal(sut.incident_hyperedges(vertex_id), constants::hyperedge_ids_view));
    CHECK(std::ranges::equal(sut.out_hyperedges(vertex_id), tail_bound_hyperedges));
    CHECK(std::ranges::equal(sut.in_hyperedges(vertex_id), head_bound_hyperedges));
}

TEST_CASE_FIXTURE(
    test_bf_directed_hyperedge_major_incidence_matrix,
    "degree should return the number of the vertex's incident hyperedges,"
    "out_degree should return the number of the vertex's outgoing hyperedges (v in T(e)),"
    "in_degree should return the number of the vertex's incoming hyperedges (v in H(e))"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    constexpr auto vertex_id = constants::id1;
    REQUIRE_EQ(sut.degree(vertex_id), 0uz);

    const auto [tail_bound_hyperedges, head_bound_hyperedges] =
        altbind_to_vertex(sut, vertex_id, constants::n_hyperedges);

    CHECK_EQ(sut.degree(vertex_id), constants::n_hyperedges);
    CHECK_EQ(sut.out_degree(vertex_id), tail_bound_hyperedges.size());
    CHECK_EQ(sut.in_degree(vertex_id), head_bound_hyperedges.size());
}

TEST_CASE_FIXTURE(
    test_bf_directed_hyperedge_major_incidence_matrix,
    "add_hyperedges should properly extend matrix"
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
    test_bf_directed_hyperedge_major_incidence_matrix,
    "remove_hyperedge should properly remove the row and implicitly shift hyperedge IDs"
) {
    constexpr hgl::size_type n_vertices = 1uz, n_hyperedges = 5uz;
    constexpr hgl::id_type vertex_id = constants::id1;

    sut_type sut{n_vertices, n_hyperedges};
    sut.bind_tail(vertex_id, constants::id2);
    sut.bind_head(vertex_id, constants::id4);

    hgl::id_type rem_eid;
    hgl::size_type expected_vertex_degree;
    std::vector<hgl::id_type> expected_hyperedges;

    SUBCASE("not present hyperedge < first incident hyperedge") {
        rem_eid = constants::id1;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2 - 1uz, constants::id4 - 1uz};
    }

    SUBCASE("present hyperedge = first incident hyperedge") {
        rem_eid = constants::id2;
        expected_vertex_degree = 1uz;
        expected_hyperedges = {constants::id4 - 1uz};
    }

    SUBCASE("not present hyperedge > first incident hyperedge") {
        rem_eid = constants::id3;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2, constants::id4 - 1uz};
    }

    SUBCASE("present hyperedge = last incident hyperedge") {
        rem_eid = constants::id4;
        expected_vertex_degree = 1uz;
        expected_hyperedges = {constants::id2};
    }

    SUBCASE("not present hyperedge > last incident hyperedge") {
        rem_eid = constants::id4 + 1uz;
        expected_vertex_degree = 2uz;
        expected_hyperedges = {constants::id2, constants::id4};
    }

    CAPTURE(rem_eid);
    CAPTURE(expected_vertex_degree);
    CAPTURE(expected_hyperedges);

    sut.remove_hyperedge(rem_eid);
    CHECK_EQ(matrix(sut).size(), n_hyperedges - 1uz);
    CHECK_EQ(sut.degree(vertex_id), expected_vertex_degree);
    CHECK(std::ranges::equal(sut.incident_hyperedges(vertex_id), expected_hyperedges));
}

TEST_CASE_FIXTURE(
    test_bf_directed_hyperedge_major_incidence_matrix,
    "incident_vertices should return a view of the hyperedge's incident vertex ids, "
    "tail_vertices should return a view of the hyperedge's tail vertex ids: T(e), "
    "head_vertices should return a view of the hyperedge's head vertex ids: H(e)"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    constexpr auto hyperedge_id = constants::id1;
    REQUIRE(std::ranges::empty(sut.incident_vertices(hyperedge_id)));

    const auto [tail_bound_vertices, head_bound_vertices] =
        altbind_to_hyperedge(sut, hyperedge_id, constants::n_vertices);

    CHECK(std::ranges::equal(sut.incident_vertices(hyperedge_id), constants::vertex_ids_view));
    CHECK(std::ranges::equal(sut.tail_vertices(hyperedge_id), tail_bound_vertices));
    CHECK(std::ranges::equal(sut.head_vertices(hyperedge_id), head_bound_vertices));
}

TEST_CASE_FIXTURE(
    test_bf_directed_hyperedge_major_incidence_matrix,
    "hyperedge_size should return the number of the hyperedge's incident vertices, "
    "tail_size should return the number of the hyperedge's tail vertices: |T(e)|, "
    "head_size should return the number of the hyperedge's head vertices: |H(e)|"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    constexpr auto hyperedge_id = constants::id1;
    REQUIRE_EQ(sut.hyperedge_size(hyperedge_id), 0uz);

    const auto [tail_bound_vertices, head_bound_vertices] =
        altbind_to_hyperedge(sut, hyperedge_id, constants::n_vertices);

    CHECK_EQ(sut.hyperedge_size(hyperedge_id), constants::n_vertices);
    CHECK_EQ(sut.tail_size(hyperedge_id), tail_bound_vertices.size());
    CHECK_EQ(sut.head_size(hyperedge_id), head_bound_vertices.size());
}

TEST_CASE_FIXTURE(
    test_bf_directed_hyperedge_major_incidence_matrix,
    "bind_tail should set the corresponding matrix entry to backward incidence"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    REQUIRE(std::ranges::empty(sut.incident_vertices(constants::id1)));

    sut.bind_tail(constants::id1, constants::id1);

    CHECK_EQ(matrix(sut)[constants::id1][constants::id1], incidence_type::backward);

    const auto vertices = sut.tail_vertices(constants::id1) | std::ranges::to<std::vector>();
    CHECK_EQ(sut.tail_size(constants::id1), 1uz);
    CHECK_EQ(std::ranges::size(vertices), 1uz);
    CHECK(std::ranges::contains(vertices, constants::id1));
}

TEST_CASE_FIXTURE(
    test_bf_directed_hyperedge_major_incidence_matrix,
    "bind_head should set the corresponding matrix entry to forward incidence"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    REQUIRE(std::ranges::empty(sut.incident_vertices(constants::id1)));

    sut.bind_head(constants::id1, constants::id1);

    CHECK_EQ(matrix(sut)[constants::id1][constants::id1], incidence_type::forward);

    const auto vertices = sut.head_vertices(constants::id1) | std::ranges::to<std::vector>();
    CHECK_EQ(sut.head_size(constants::id1), 1uz);
    CHECK_EQ(std::ranges::size(vertices), 1uz);
    CHECK(std::ranges::contains(vertices, constants::id1));
}

TEST_CASE_FIXTURE(
    test_bf_directed_hyperedge_major_incidence_matrix, "unbind should clear the corresponding bit"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    SUBCASE("tail bound") {
        sut.bind_tail(constants::id1, constants::id1);
    }
    SUBCASE("head bound") {
        sut.bind_head(constants::id1, constants::id1);
    }
    CAPTURE(sut);

    REQUIRE_EQ(sut.hyperedge_size(constants::id1), 1uz);
    REQUIRE_NE(matrix(sut)[constants::id1][constants::id1], incidence_type::none);

    sut.unbind(constants::id1, constants::id2);
    CHECK_EQ(sut.hyperedge_size(constants::id1), 1uz);

    sut.unbind(constants::id1, constants::id1);
    CHECK(std::ranges::empty(sut.incident_vertices(constants::id1)));
    CHECK_EQ(matrix(sut)[constants::id1][constants::id1], incidence_type::none);
}

TEST_CASE_FIXTURE(
    test_bf_directed_hyperedge_major_incidence_matrix,
    "are_bound, is_tail, is_head should return true only when the corresponding matrix entry is "
    "set to a valid, matching incidence type"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    sut.bind_tail(constants::id1, constants::id1);
    sut.bind_head(constants::id2, constants::id1);

    CHECK(sut.are_bound(constants::id1, constants::id1));
    CHECK(sut.is_tail(constants::id1, constants::id1));
    CHECK_FALSE(sut.is_head(constants::id1, constants::id1));

    CHECK(sut.are_bound(constants::id2, constants::id1));
    CHECK_FALSE(sut.is_tail(constants::id2, constants::id1));
    CHECK(sut.is_head(constants::id2, constants::id1));

    CHECK_FALSE(sut.are_bound(constants::id3, constants::id1));
    CHECK_FALSE(sut.is_tail(constants::id3, constants::id1));
    CHECK_FALSE(sut.is_head(constants::id3, constants::id1));
}

TEST_CASE_FIXTURE(
    test_bf_directed_hyperedge_major_incidence_matrix,
    "element size map getters should return maps of properly calculated element sizes"
) {
    constexpr auto n_elements = 5ull;
    sut_type sut{n_elements, n_elements};

    constexpr auto is_zero = [](const auto& size) { return size == 0ull; };
    REQUIRE(std::ranges::all_of(sut.degree_map(n_elements), is_zero));
    REQUIRE(std::ranges::all_of(sut.out_degree_map(n_elements), is_zero));
    REQUIRE(std::ranges::all_of(sut.in_degree_map(n_elements), is_zero));
    REQUIRE(std::ranges::all_of(sut.hyperedge_size_map(n_elements), is_zero));
    REQUIRE(std::ranges::all_of(sut.tail_size_map(n_elements), is_zero));
    REQUIRE(std::ranges::all_of(sut.head_size_map(n_elements), is_zero));

    SUBCASE("tail bind") {
        for (std::size_t i = 0uz; i < n_elements; i++)
            for (std::size_t j = 0uz; j <= i; j++)
                sut.bind_tail(i, j);

        const auto deg_map = sut.degree_map(n_elements);
        const auto out_deg_map = sut.out_degree_map(n_elements);
        const auto esize_map = sut.hyperedge_size_map(n_elements);
        const auto tsize_map = sut.tail_size_map(n_elements);
        for (std::size_t i = 0uz; i < n_elements; i++) {
            CHECK_EQ(deg_map[i], i + 1uz);
            CHECK_EQ(out_deg_map[i], i + 1uz);
            CHECK_EQ(esize_map[i], n_elements - i);
            CHECK_EQ(tsize_map[i], n_elements - i);
        }
        CHECK(std::ranges::all_of(sut.in_degree_map(n_elements), is_zero));
        CHECK(std::ranges::all_of(sut.head_size_map(n_elements), is_zero));
    }

    SUBCASE("head bind") {
        for (std::size_t i = 0uz; i < n_elements; i++)
            for (std::size_t j = 0uz; j <= i; j++)
                sut.bind_head(i, j);

        const auto deg_map = sut.degree_map(n_elements);
        const auto in_deg_map = sut.in_degree_map(n_elements);
        const auto esize_map = sut.hyperedge_size_map(n_elements);
        const auto hsize_map = sut.head_size_map(n_elements);
        for (std::size_t i = 0uz; i < n_elements; i++) {
            CHECK_EQ(deg_map[i], i + 1uz);
            CHECK_EQ(in_deg_map[i], i + 1uz);
            CHECK_EQ(esize_map[i], n_elements - i);
            CHECK_EQ(hsize_map[i], n_elements - i);
        }
        CHECK(std::ranges::all_of(sut.out_degree_map(n_elements), is_zero));
        CHECK(std::ranges::all_of(sut.tail_size_map(n_elements), is_zero));
    }

    // diagonal = tail, everything else is head
    for (std::size_t i = 0uz; i < n_elements; i++) {
        for (std::size_t j = 0uz; j <= i; j++) {
            if (i == j)
                sut.bind_tail(i, j);
            else
                sut.bind_head(i, j);
        }
    }

    const auto deg_map = sut.degree_map(n_elements);
    const auto out_deg_map = sut.out_degree_map(n_elements);
    const auto in_deg_map = sut.in_degree_map(n_elements);

    const auto esize_map = sut.hyperedge_size_map(n_elements);
    const auto tsize_map = sut.tail_size_map(n_elements);
    const auto hsize_map = sut.head_size_map(n_elements);

    for (std::size_t i = 0uz; i < n_elements; i++) {
        CHECK_EQ(deg_map[i], i + 1uz);
        CHECK_EQ(out_deg_map[i], 1uz);
        CHECK_EQ(in_deg_map[i], i);

        CHECK_EQ(esize_map[i], n_elements - i);
        CHECK_EQ(tsize_map[i], 1uz);
        CHECK_EQ(hsize_map[i], n_elements - i - 1uz);
    }
}

TEST_CASE_FIXTURE(
    test_bf_directed_hyperedge_major_incidence_matrix,
    "equality operator should correctly compare directed incidence matrices"
) {
    sut_type sut1{constants::n_vertices, constants::n_hyperedges};
    sut1.bind_tail(constants::id1, constants::id1);
    sut1.bind_head(constants::id2, constants::id1);

    SUBCASE("identical matrices are equal") {
        const sut_type sut2 = sut1;
        CHECK_EQ(sut1, sut2);
    }

    SUBCASE("matrices with different bindings are not equal") {
        sut_type sut2 = sut1;
        sut2.bind_tail(constants::id3, constants::id1);
        CHECK_NE(sut1, sut2);
    }

    SUBCASE("matrices with swapped tail/head bindings are not equal") {
        sut_type sut2{constants::n_vertices, constants::n_hyperedges};
        sut2.bind_head(constants::id1, constants::id1);
        sut2.bind_tail(constants::id2, constants::id1);
        CHECK_NE(sut1, sut2);
    }

    SUBCASE("matrices with different dimensions are not equal") {
        sut_type sut2{constants::n_vertices, constants::n_hyperedges + 1uz};
        sut2.bind_tail(constants::id1, constants::id1);
        sut2.bind_head(constants::id2, constants::id1);
        CHECK_NE(sut1, sut2);
    }
}

TEST_SUITE_END(); // test_incidence_matrix

} // namespace hgl_testing
