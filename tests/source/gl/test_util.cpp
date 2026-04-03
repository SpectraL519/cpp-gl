#include "doctest.h"

#include <gl/util/math.hpp>
#include <gl/util/ranges.hpp>

#include <algorithm>
#include <ranges>
#include <vector>

namespace gl_testing {

TEST_SUITE_BEGIN("test_util");

TEST_CASE("upow should return the `base ^ exponent` operation result") {
    gl::size_type base, exponent, expected_result;

    // clang-format off

    SUBCASE(""){ base = 2;  exponent = 3;  expected_result = 8; }
    SUBCASE(""){ base = 5;  exponent = 0;  expected_result = 1; }
    SUBCASE(""){ base = 10; exponent = 2;  expected_result = 100; }
    SUBCASE(""){ base = 3;  exponent = 4;  expected_result = 81; }
    SUBCASE(""){ base = 7;  exponent = 1;  expected_result = 7; }
    SUBCASE(""){ base = 2;  exponent = 10; expected_result = 1024; }

    // clang-format on

    CAPTURE(base);
    CAPTURE(exponent);
    CAPTURE(expected_result);

    CHECK_EQ(gl::util::upow(base, exponent), expected_result);
}

TEST_CASE("upow_sum function test") {
    gl::size_type base, i_begin, i_end, expected_result;

    // clang-format off

    SUBCASE(""){ base = 0;  i_begin = 2;  i_end = 5;  expected_result = 0; }   // 0^2 + 0^3 + 0^4 + 0^5 = 0 + 0 + 0 + 0 = 0
    SUBCASE(""){ base = 0;  i_begin = 0;  i_end = 5;  expected_result = 1; }   // 0^0 + 0^1 + 0^2 + 0^3 + 0^4 + 0^5 = 1 + 0 + 0 + 0 + 0 + 0 = 1
    SUBCASE(""){ base = 3;  i_begin = 0;  i_end = 0;  expected_result = 1; }   // 3^0 = 1
    SUBCASE(""){ base = 2;  i_begin = 2;  i_end = 2;  expected_result = 4; }   // 2^2 = 4
    SUBCASE(""){ base = 1;  i_begin = 0;  i_end = 5;  expected_result = 6; }   // 1^0 + 1^1 + 1^2 + 1^3 + 1^4 + 1^5 = 1 + 1 + 1 + 1 + 1 + 1 = 6
    SUBCASE(""){ base = 2;  i_begin = 0;  i_end = 3;  expected_result = 15; }  // 2^0 + 2^1 + 2^2 + 2^3 = 1 + 2 + 4 + 8 = 15
    SUBCASE(""){ base = 3;  i_begin = 1;  i_end = 3;  expected_result = 39; }  // 3^1 + 3^2 + 3^3 = 3 + 9 + 27 = 39
    SUBCASE(""){ base = 2;  i_begin = 2;  i_end = 5;  expected_result = 60; }  // 2^2 + 2^3 + 2^4 + 2^5 = 4 + 8 + 16 + 32 = 60

    // clang-format on

    CAPTURE(base);
    CAPTURE(i_begin);
    CAPTURE(i_end);
    CAPTURE(expected_result);

    CHECK_EQ(gl::util::upow_sum(base, i_begin, i_end), expected_result);
}

struct test_concat_view {
    std::vector<int> v1;
    std::vector<int> v2;
    std::vector<int> expected;
};

TEST_CASE_FIXTURE(test_concat_view, "concat should sequentially combine two identical range types") {
    SUBCASE("Both ranges are non-empty") {
        v1 = {1, 2, 3};
        v2 = {4, 5, 6};
        expected = {1, 2, 3, 4, 5, 6};
    }
    SUBCASE("First range is empty") {
        v1 = {};
        v2 = {4, 5, 6};
        expected = {4, 5, 6};
    }
    SUBCASE("Second range is empty") {
        v1 = {1, 2, 3};
        v2 = {};
        expected = {1, 2, 3};
    }
    SUBCASE("Both ranges are empty") {
        v1 = {};
        v2 = {};
        expected = {};
    }

    auto concat_vw = gl::util::concat(v1, v2);

    CHECK_EQ(std::ranges::distance(concat_vw), expected.size());
    CHECK(std::ranges::equal(concat_vw, expected));

    const auto concat_vec = concat_vw | std::ranges::to<std::vector>();
    CHECK_EQ(concat_vec.size(), expected.size());
    CHECK(std::ranges::equal(concat_vec, expected));
}

TEST_CASE_FIXTURE(test_concat_view, "concat_view satisfies C++20 range concepts") {
    v1 = {1, 2};
    v2 = {3, 4};

    auto concat_vw = gl::util::concat(v1, v2);

    static_assert(std::ranges::view<decltype(concat_vw)>);
    static_assert(std::ranges::forward_range<decltype(concat_vw)>);

    CHECK_EQ(std::ranges::distance(concat_vw), 4);
}

TEST_CASE_FIXTURE(test_concat_view, "concat_view propagates const correctness") {
    v1 = {10, 20};
    v2 = {30, 40};

    const auto& cv1 = v1;
    const auto& cv2 = v2;

    auto concat_vw = gl::util::concat(cv1, cv2);

    int sum = 0;
    for (const auto& val : concat_vw)
        sum += val;

    CHECK_EQ(sum, 100);
}

TEST_CASE("concat should seamlessly bridge heterogeneous view types") {
    auto vw1 = std::views::iota(1, 4);
    std::vector<int> vw2 = {4, 5, 6};

    auto concat_vw = gl::util::concat(vw1, vw2);
    std::vector<int> expected = {1, 2, 3, 4, 5, 6};

    CHECK_EQ(std::ranges::distance(concat_vw), expected.size());
    CHECK(std::ranges::equal(concat_vw, expected));
}

TEST_SUITE_END(); // test_util

} // namespace gl_testing
