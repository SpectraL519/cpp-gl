#include "hgl/hypergraph_elements.hpp"

#include <doctest.h>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_hypergraph_elements");

static_assert(std::same_as<hgl::vertex_descriptor<>, gl::vertex_descriptor<>>);

struct test_hyperedge_descriptor {
    using sut_type = hgl::hyperedge_descriptor<>;

    static constexpr hgl::types::id_type id1 = 0ull;
    static constexpr hgl::types::id_type id2 = 1ull;

    sut_type he1{id1};
    sut_type he2{id2};
};

TEST_CASE_FIXTURE(
    test_hyperedge_descriptor, "hyperedges should be equal only when they have the same id"
) {
    CHECK_EQ(he1, he1);
    CHECK_NE(he1, he2);
}

TEST_CASE_FIXTURE(test_hyperedge_descriptor, "hyperedge should be valid only if it has a valid id") {
    CHECK(he1);
    CHECK(he2.is_valid());
    CHECK_FALSE(HyperedgeType::invalid());
}

TEST_CASE_FIXTURE(test_hyperedge_descriptor, "id() should return the id of the hyperedge") {
    CHECK_EQ(he1.id(), id1);
    CHECK_EQ(he2.id(), id2);
}

TEST_CASE_FIXTURE(test_hyperedge_descriptor, "hyperedge descriptors should be invalid by default") {
    CHECK_FALSE(sut_type{}.is_valid());
    CHECK_EQ(sut_type{}.id(), hgl::constants::invalid_id);
}

// TODO: properties tests

TEST_SUITE_END(); // test_hypergraph_elements

} // namespace hgl_testing
