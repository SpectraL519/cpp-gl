#pragma once

#include "doctest.h"

#include <hgl/hypergraph.hpp>

#include <algorithm>

namespace hgl_testing {

template <hgl::traits::c_hypergraph HypergraphType>
void verify_hypergraph_structure(const HypergraphType& actual, const HypergraphType& expected) {
    REQUIRE_EQ(actual.order(), expected.order());
    REQUIRE_EQ(actual.size(), expected.size());

    for (const auto& he_actual : actual.hyperedges()) {
        const auto id = he_actual.id();
        REQUIRE(expected.has_hyperedge(id));

        if constexpr (std::same_as<typename HypergraphType::directional_tag, hgl::undirected_t>) {
            auto actual_vs = actual.incident_vertex_ids(id) | std::ranges::to<std::vector>();
            auto expected_vs = expected.incident_vertex_ids(id) | std::ranges::to<std::vector>();
            CHECK(std::ranges::is_permutation(actual_vs, expected_vs));
        }
        else if constexpr (std::same_as<
                               typename HypergraphType::directional_tag,
                               hgl::bf_directed_t>) {
            auto actual_tail = actual.tail_vertex_ids(id) | std::ranges::to<std::vector>();
            auto expected_tail = expected.tail_vertex_ids(id) | std::ranges::to<std::vector>();
            CHECK(std::ranges::is_permutation(actual_tail, expected_tail));

            auto actual_head = actual.head_vertex_ids(id) | std::ranges::to<std::vector>();
            auto expected_head = expected.head_vertex_ids(id) | std::ranges::to<std::vector>();
            CHECK(std::ranges::is_permutation(actual_head, expected_head));
        }
    }
}

template <hgl::traits::c_hypergraph HypergraphType>
void verify_vertex_properties(const HypergraphType& actual, const HypergraphType& expected) {
    const auto properties_proj = [](const auto& item) { return item.properties(); };

    CHECK(std::ranges::equal(
        actual.vertices(),
        expected.vertices(),
        std::ranges::equal_to{},
        properties_proj,
        properties_proj
    ));
}

template <hgl::traits::c_hypergraph HypergraphType>
void verify_hyperedge_properties(const HypergraphType& actual, const HypergraphType& expected) {
    const auto properties_proj = [](const auto& item) { return item.properties(); };

    CHECK(std::ranges::equal(
        actual.hyperedges(),
        expected.hyperedges(),
        std::ranges::equal_to{},
        properties_proj,
        properties_proj
    ));
}

} // namespace hgl_testing
