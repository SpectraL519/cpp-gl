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

TEST_CASE_TEMPLATE_DEFINE(
    "directional_tag-independent tests", HyperedgeType, hyperedge_directional_tag_template
) {
    test_hyperedge_descriptor fixture;
    const HyperedgeType he1{fixture.id1};
    const HyperedgeType he2{fixture.id2};

    SUBCASE("hyperedges should be equal only when they have the same id") {
        CHECK_EQ(he1, he1);
        CHECK_NE(he1, he2);
    }

    SUBCASE("hyperedge should be valid only if it has a valid id") {
        CHECK(he1);
        CHECK(he2.is_valid());
        CHECK_FALSE(HyperedgeType::invalid());
    }

    SUBCASE("id() should return the id of the hyperedge") {
        CHECK_EQ(he1.id(), fixture.id1);
        CHECK_EQ(he2.id(), fixture.id2);
    }

    SUBCASE("hyperedge descriptors should be invalid by default") {
        CHECK_FALSE(HyperedgeType{}.is_valid());
        CHECK_EQ(HyperedgeType{}.id(), hgl::constants::invalid_id);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    hyperedge_directional_tag_template,
    hgl::undirected_hyperedge<>, // default undirected edge
    hgl::bf_directed_hyperedge<> // default directed edge
);

TEST_SUITE_END(); // test_hypergraph_elements

} // namespace hgl_testing
