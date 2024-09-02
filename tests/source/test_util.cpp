#include "namespaces.hpp"

#include <gl/util/enum.hpp>
#include <gl/util/pow.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_util");

TEST_CASE("upow should return the `base ^ exponent` operation result") {
    lib_t::size_type base, exponent, expected_result;

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

    CHECK_EQ(lib::util::upow(base, exponent), expected_result);
}

TEST_CASE("upow_sum function test") {
    lib_t::size_type base, i_begin, i_end, expected_result;

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

    CHECK_EQ(lib::util::upow_sum(base, i_begin, i_end), expected_result);
}

TEST_CASE("to_underlying should return the underlying value of an enum class") {
    using underlying_type = int;
    constexpr underlying_type value_1 = 1;
    constexpr underlying_type value_2 = 2;
    constexpr underlying_type out_of_bounds_value = 2;

    enum class Enum : underlying_type { value_1 = value_1, value_2 = value_2 };

    CHECK_EQ(lib::util::to_underlying(Enum::value_1), value_1);
    CHECK_EQ(lib::util::to_underlying(Enum::value_2), value_2);
    CHECK_EQ(lib::util::to_underlying(static_cast<Enum>(out_of_bounds_value)), out_of_bounds_value);
}

TEST_SUITE_END(); // test_util

} // namespace gl_testing
