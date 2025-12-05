#include "testing/hgl/constants.hpp"

#include <doctest.h>
#include <hgl/impl/hyperedge_list.hpp>

#include <algorithm>
#include <ranges>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_hyperedge_list");

struct test_hyperedge_list {
    template <typename HyperedgeList>
    typename HyperedgeList::hypergraph_storage_type& storage(HyperedgeList& sut) const noexcept {
        return sut._storage;
    }
};

struct test_undirected_hyperedge_list : public test_hyperedge_list {
    using sut_type = hgl::impl::undirected_hyperedge_list;
};

TEST_CASE_FIXTURE(test_undirected_hyperedge_list, "should initialize empty storage by default") {
    sut_type sut{};
    CHECK(storage(sut).empty());
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_list,
    "initialization with size parameters should properly initialize storage"
) {
    sut_type sut(constants::n_vertices, constants::n_hyperedges);
    CHECK_EQ(storage(sut).size(), constants::n_hyperedges);
    CHECK(std::ranges::all_of(storage(sut), [](const auto& hyperedge_storage) {
        return hyperedge_storage.empty();
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_list, "add_hyperedges should properly extend the hypergraph storage"
) {
    sut_type sut{};
    sut.add_hyperedges(constants::n_hyperedges);
    CHECK_EQ(storage(sut).size(), constants::n_hyperedges);
    CHECK(std::ranges::all_of(storage(sut), [](const auto& hyperedge_storage) {
        return hyperedge_storage.empty();
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_list,
    "remove_hyperedge should properly erase the proper hyperedge storage entry from the hypergraph"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    REQUIRE_EQ(storage(sut).size(), constants::n_hyperedges);

    sut.remove_hyperedge(constants::id1);
    CHECK_EQ(storage(sut).size(), constants::n_hyperedges - 1uz);
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_list, "incident_vertices should remove an empty view by default"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    CHECK(std::ranges::all_of(constants::hyperedge_ids_view, [&sut](const auto hyperedge_id) {
        return std::ranges::empty(sut.incident_vertices(hyperedge_id));
    }));
}

TEST_CASE_FIXTURE(test_undirected_hyperedge_list, "hyperedge_size should return 0 by default") {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    CHECK(std::ranges::all_of(constants::hyperedge_ids_view, [&sut](const auto hyperedge_id) {
        return sut.hyperedge_size(hyperedge_id) == 0uz;
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_list,
    "bind should add the vertex to the given hyperedge's storage only if the they are not bound"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    REQUIRE(std::ranges::empty(sut.incident_vertices(constants::id1)));

    sut.bind(constants::id1, constants::id1);
    const auto vertices1 = sut.incident_vertices(constants::id1) | std::ranges::to<std::vector>();
    CHECK_EQ(sut.hyperedge_size(constants::id1), 1uz);
    CHECK_EQ(std::ranges::size(vertices1), 1uz);
    CHECK(std::ranges::contains(vertices1, constants::id1));
    CHECK(std::ranges::equal(vertices1, storage(sut)[constants::id1]));

    sut.bind(constants::id1, constants::id1);
    const auto vertices2 = sut.incident_vertices(constants::id1) | std::ranges::to<std::vector>();
    CHECK_EQ(std::ranges::size(vertices2), 1uz);
    CHECK(std::ranges::equal(vertices2, vertices1));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_list,
    "unbind should remove the vertex from the given hyperedge's storage only if the they are bound"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    REQUIRE(std::ranges::empty(sut.incident_vertices(constants::id1)));

    sut.bind(constants::id1, constants::id1);
    const auto vertices1 = sut.incident_vertices(constants::id1) | std::ranges::to<std::vector>();
    REQUIRE_EQ(sut.hyperedge_size(constants::id1), 1uz);
    REQUIRE(std::ranges::contains(vertices1, constants::id1));

    sut.unbind(constants::id1, constants::id2);
    const auto vertices2 = sut.incident_vertices(constants::id1) | std::ranges::to<std::vector>();
    REQUIRE_EQ(std::ranges::size(vertices2), 1uz);
    REQUIRE(std::ranges::equal(vertices2, vertices1));

    sut.unbind(constants::id1, constants::id1);
    CHECK(std::ranges::empty(sut.incident_vertices(constants::id1)));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_list,
    "are_bound should return true only when the given vertex is present in the hyperedge's storage"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    REQUIRE(std::ranges::empty(sut.incident_vertices(constants::id1)));

    sut.bind(constants::id1, constants::id1);
    REQUIRE_EQ(sut.hyperedge_size(constants::id1), 1uz);
    REQUIRE(std::ranges::contains(sut.incident_vertices(constants::id1), constants::id1));

    CHECK(sut.are_bound(constants::id1, constants::id1));
    CHECK_FALSE(sut.are_bound(constants::id1, constants::id2));
    CHECK_FALSE(sut.are_bound(constants::id2, constants::id1));
}

TEST_CASE_FIXTURE(test_undirected_hyperedge_list, "add_vertices should do nothing") {
    sut_type sut{};
    sut.add_vertices(constants::n_vertices);
    CHECK(storage(sut).empty());
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_list,
    "remove_vertex should properly unbind (if necessary) the given vertex and align ids > vertex_id"
) {
    constexpr hgl::types::size_type n_vertices = 4uz, n_hyperedges = 1uz;
    constexpr hgl::types::id_type hyperedge_id = constants::id1;

    sut_type sut{n_vertices, n_hyperedges};
    sut.bind(constants::id2, hyperedge_id);
    sut.bind(constants::id4, hyperedge_id);

    hgl::types::id_type rem_vid;
    std::vector<hgl::types::id_type> expected_vertices;

    SUBCASE("not present vertex < first incident vertex") {
        rem_vid = constants::id1;
        expected_vertices = {constants::id2 - 1uz, constants::id4 - 1uz};
    }

    SUBCASE("present vertex = first incident vertex") {
        rem_vid = constants::id2;
        expected_vertices = {constants::id4 - 1uz};
    }

    SUBCASE("not present vertex > first incident vertex") {
        rem_vid = constants::id3;
        expected_vertices = {constants::id2, constants::id4 - 1uz};
    }

    SUBCASE("present vertex = last incident vertex") {
        rem_vid = constants::id4;
        expected_vertices = {constants::id2};
    }

    SUBCASE("not present vertex > last incident vertex") {
        rem_vid = constants::id4 + 1uz;
        expected_vertices = {constants::id2, constants::id4};
    }

    CAPTURE(rem_vid);
    CAPTURE(expected_vertices);

    sut.remove_vertex(rem_vid);
    CHECK(std::ranges::equal(sut.incident_vertices(hyperedge_id), expected_vertices));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_list,
    "remove_vertex should unbind the given vertex from all hyperedges and align ids > vertex_id"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    for (const auto vid : constants::vertex_ids_view)
        for (const auto heid : constants::hyperedge_ids_view)
            sut.bind(vid, heid);

    constexpr auto vertex_id = constants::id1;
    REQUIRE(std::ranges::all_of(constants::hyperedge_ids_view, [&sut](const auto hyperedge_id) {
        return sut.are_bound(vertex_id, hyperedge_id);
    }));

    sut.remove_vertex(vertex_id);
    CHECK(std::ranges::all_of(constants::hyperedge_ids_view, [&sut](const auto hyperedge_id) {
        return std::ranges::equal(
            sut.incident_vertices(hyperedge_id),
            std::views::iota(constants::id1, constants::n_vertices - 1uz)
        );
    }));
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_list,
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
    test_undirected_hyperedge_list,
    "degree should return the number of the vertex's incident hyperedges"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};

    constexpr auto vertex_id = constants::id1;
    REQUIRE_EQ(sut.degree(vertex_id), 0uz);

    for (const auto hyperedge_id : constants::hyperedge_ids_view)
        sut.bind(vertex_id, hyperedge_id);
    CHECK_EQ(sut.degree(vertex_id), constants::n_hyperedges);
}

TEST_SUITE_END(); // test_hyperedge_list

} // namespace hgl_testing
