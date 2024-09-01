#include "namespaces.hpp"

#include <gl/util/pow.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_util");

TEST_CASE("upow should return the `base ^ exponent` operation result") {
    lib_t::size_type base, exponent, expected_result;

    SUBCASE(""){ base = 2;  exponent = 3;  expected_result = 8; }
    SUBCASE(""){ base = 5;  exponent = 0;  expected_result = 1; }
    SUBCASE(""){ base = 10; exponent = 2;  expected_result = 100; }
    SUBCASE(""){ base = 3;  exponent = 4;  expected_result = 81; }
    SUBCASE(""){ base = 7;  exponent = 1;  expected_result = 7; }
    SUBCASE(""){ base = 2;  exponent = 10; expected_result = 1024; }

    CAPTURE(base);
    CAPTURE(exponent);
    CAPTURE(expected_result);

    CHECK_EQ(lib::util::upow(base, exponent), expected_result);
}

TEST_CASE("upow_sum function test") {
    lib_t::size_type base, i_begin, i_end, expected_result;

    SUBCASE(""){ base = 0;  i_begin = 2;  i_end = 5;  expected_result = 0; }
    SUBCASE(""){ base = 0;  i_begin = 0;  i_end = 5;  expected_result = 1; }
    SUBCASE(""){ base = 2;  i_begin = 0;  i_end = 3;  expected_result = 15; }
    SUBCASE(""){ base = 2;  i_begin = 2;  i_end = 5;  expected_result = 60; }
    SUBCASE(""){ base = 1;  i_begin = 0;  i_end = 5;  expected_result = 6; }
    SUBCASE(""){ base = 3;  i_begin = 1;  i_end = 3;  expected_result = 39; }

    CAPTURE(base);
    CAPTURE(i_begin);
    CAPTURE(i_end);
    CAPTURE(expected_result);

    CHECK_EQ(lib::util::upow_sum(base, i_begin, i_end), expected_result);
}

TEST_SUITE_END(); // test_util

}
