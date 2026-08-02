#include "doctest.h"
#include "testing/common/functional.hpp"
#include "testing/sgl/constants.hpp"

#include <sgl/sgl.hpp>

#include <algorithm>
#include <cmath>
#include <vector>

namespace {

using namespace sgl_testing::constants;

template <typename Sketch>
[[nodiscard]] bool row_max_matches(const Sketch& sut, const typename Sketch::id_type id) {
    const auto row = sut.sketch_of(id);
    const auto expected = *std::ranges::max_element(row.S);
    return *row.max_s == expected;
}

template <typename Sketch>
[[nodiscard]] bool any_slot_updated(const Sketch& sut, const typename Sketch::id_type id) {
    using weight_type = typename Sketch::weight_type;
    const auto row = sut.sketch_of(id);
    return std::ranges::any_of(row.S, [](const auto s) {
        return s < sgl::sketch_infinity_v<weight_type>;
    });
}

} // namespace

TEST_CASE_TEMPLATE_DEFINE(
    "common binary sketch tests", TraitsType, common_binary_sketch_traits_template
) {
    using sut_type = sgl::sketch<TraitsType>;
    using weight_type = typename sut_type::weight_type;

    SUBCASE("default construction uses default sketch width and no vertices") {
        const sut_type sut;
        CHECK_EQ(sut.width(), sgl::default_sketch_width);
        CHECK_EQ(sut.n_vertices(), 0uz);
        CHECK_FALSE(sut.has_vertex(v0));
    }

    SUBCASE("construction with explicit width") {
        const sut_type sut{sketch_width};
        CHECK_EQ(sut.width(), sketch_width);
        CHECK_EQ(sut.n_vertices(), 0uz);
    }

    SUBCASE("construction with width < 2 throws") {
        CHECK_THROWS_AS(sut_type{invalid_width}, std::invalid_argument);
    }

    SUBCASE("update_edge inserts endpoints and updates sketch slots") {
        sut_type sut{sketch_width};
        sut.update_edge(v0, v1, unit_weight);

        CHECK(sut.has_vertex(v0));
        CHECK(any_slot_updated(sut, v0));
        CHECK(row_max_matches(sut, v0));

        if constexpr (sut_type::traits_type::is_undirected) {
            CHECK(sut.has_vertex(v1));
            CHECK(any_slot_updated(sut, v1));
            CHECK(row_max_matches(sut, v1));
            CHECK_EQ(sut.n_vertices(), 2uz);
        }
        else {
            CHECK_FALSE(sut.has_vertex(v1));
            CHECK_EQ(sut.n_vertices(), 1uz);
        }
    }

    SUBCASE("update_edge with non-positive weight throws") {
        sut_type sut{sketch_width};
        CHECK_THROWS_AS(sut.update_edge(v0, v1, zero_weight), std::invalid_argument);
        CHECK_THROWS_AS(sut.update_edge(v0, v1, -unit_weight), std::invalid_argument);
        if (sut.has_vertex(v0))
            CHECK_FALSE(any_slot_updated(sut, v0));
    }

    SUBCASE("estimate_degree is positive after updates") {
        sut_type sut{sketch_width};
        sut.update_edge(v0, v1, unit_weight);

        CHECK(sut.estimate_degree(v0) > static_cast<weight_type>(0));
        if constexpr (sut_type::traits_type::is_undirected)
            CHECK(sut.estimate_degree(v1) > static_cast<weight_type>(0));
    }

    SUBCASE("merge takes elementwise minimum of sketch rows") {
        sut_type left{sketch_width};
        sut_type right{sketch_width};

        left.update_edge(v0, v1, unit_weight);
        right.update_edge(v0, v2, unit_weight);

        const auto left_before = left.sketch_of(v0);
        std::vector<weight_type> left_s(left_before.S.begin(), left_before.S.end());

        left.merge(right);
        const auto merged = left.sketch_of(v0);
        const auto right_row = right.sketch_of(v0);

        for (sgl::size_type j = 0uz; j < sketch_width; ++j)
            CHECK_EQ(merged.S[j], std::min(left_s[j], right_row.S[j]));

        CHECK(row_max_matches(left, v0));

        if constexpr (sut_type::traits_type::is_undirected)
            CHECK(left.has_vertex(v2));
    }

    SUBCASE("merge with different widths throws") {
        sut_type left{sketch_width};
        sut_type right{sketch_width * 2uz};
        CHECK_THROWS_AS(left.merge(right), std::invalid_argument);
    }

    SUBCASE("clear resets vertices") {
        sut_type sut{sketch_width};
        sut.update_edge(v0, v1, unit_weight);
        REQUIRE(sut.n_vertices() > 0uz);

        sut.clear();
        CHECK_EQ(sut.n_vertices(), 0uz);
        CHECK_FALSE(sut.has_vertex(v0));
    }

    SUBCASE("sketch_of for missing vertex throws") {
        const sut_type sut{sketch_width};
        CHECK_THROWS_AS(discard(sut.sketch_of(v0)), std::invalid_argument);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    common_binary_sketch_traits_template,
    sgl::undirected_sketch_traits<>,
    sgl::directed_sketch_traits<>
);

TEST_CASE("multi sketch update_edge distinguishes occurrences") {
    using sut_type = sgl::multi_sketch<>;

    sut_type sut{sketch_width};
    sut.update_edge(v0, v1, 0u, unit_weight);
    sut.update_edge(v0, v1, 1u, unit_weight);

    CHECK(sut.has_vertex(v0));
    CHECK(sut.has_vertex(v1));
    CHECK(any_slot_updated(sut, v0));
    CHECK(row_max_matches(sut, v0));
    CHECK(sut.estimate_degree(v0) > 0.0);
}

TEST_CASE("labeled sketch update_edge accepts labels") {
    using sut_type = sgl::labeled_sketch<>;

    sut_type sut{sketch_width};
    sut.update_edge(v0, v1, 42u, unit_weight);

    CHECK(sut.has_vertex(v0));
    CHECK(any_slot_updated(sut, v0));
    CHECK(row_max_matches(sut, v0));
}

TEST_CASE("hyper sketch update_edge inserts all endpoints") {
    using sut_type = sgl::hyper_sketch<>;

    sut_type sut{sketch_width};
    const std::vector<sgl::default_id_type> endpoints{v0, v1, v2, v1};

    sut.update_edge(endpoints, unit_weight);

    CHECK_EQ(sut.n_vertices(), 3uz);
    CHECK(sut.has_vertex(v0));
    CHECK(sut.has_vertex(v1));
    CHECK(sut.has_vertex(v2));
    CHECK(any_slot_updated(sut, v0));
    CHECK(any_slot_updated(sut, v1));
    CHECK(any_slot_updated(sut, v2));
    CHECK(row_max_matches(sut, v0));
}

TEST_CASE("hyper sketch rejects empty endpoint set") {
    using sut_type = sgl::hyper_sketch<>;

    sut_type sut{sketch_width};
    const std::vector<sgl::default_id_type> empty;
    CHECK_THROWS_AS(sut.update_edge(empty, unit_weight), std::invalid_argument);
}
