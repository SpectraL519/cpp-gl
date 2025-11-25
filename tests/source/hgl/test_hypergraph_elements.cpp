#include "hgl/hypergraph_elements.hpp"

#include <doctest.h>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_hypergraph_elements");

static_assert(std::same_as<hgl::vertex_descriptor<>, gl::vertex_descriptor<>>);

struct test_hyperedge_descriptor {
    static constexpr hgl::types::id_type id1 = 0ull;
    static constexpr hgl::types::id_type id2 = 1ull;
};

TEST_CASE_FIXTURE(
    test_hyperedge_descriptor,
    "is_undirected should return true only for hyperedges with undirected directional tag"
) {
    CHECK(hgl::undirected_hyperedge<>(id1).is_undirected());
    CHECK_FALSE(hgl::bf_directed_hyperedge<>(id1).is_undirected());
}

TEST_CASE_FIXTURE(
    test_hyperedge_descriptor,
    "is_bf_directed should return true only for hyperedges with bf-directed directional tag"
) {
    CHECK(hgl::bf_directed_hyperedge<>(id1).is_bf_directed());
    CHECK_FALSE(hgl::undirected_hyperedge<>(id1).is_bf_directed());
}

TEST_SUITE_END(); // test_hypergraph_elements

} // namespace hgl_testing
