#include "testing/hgl/constants.hpp"

#include <doctest.h>
#include <hgl/impl/hyperedge_list.hpp>

#include <algorithm>

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
    "remove_hyperedge should properly erase the proper hyperedge storage from the hypergraph"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    REQUIRE_EQ(storage(sut).size(), constants::n_hyperedges);

    sut.remove_hyperedge(constants::id1);
    CHECK_EQ(storage(sut).size(), constants::n_hyperedges - 1uz);
}

TEST_CASE_FIXTURE(
    test_undirected_hyperedge_list, "hyperedge_vertices should remove an empty view by default"
) {
    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    CHECK(std::ranges::all_of(constants::hyperedge_ids_view, [&sut](const auto hyperedge_id) {
        return std::ranges::empty(sut.hyperedge_vertices(hyperedge_id));
    }));
}

TEST_SUITE_END(); // test_hyperedge_list

} // namespace hgl_testing
