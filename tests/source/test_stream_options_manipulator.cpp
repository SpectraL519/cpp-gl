#include "constants.hpp"

#include <gl/io/stream_options_manipulator.hpp>

#include <doctest.h>

#include <sstream>

namespace gl_testing {

TEST_SUITE_BEGIN("test_stream_options_manipulator");

struct test_stream_options_manipulator {
    test_stream_options_manipulator() {
        REQUIRE_FALSE(lib::io::is_option_set(ss1, property_bit_position));
        REQUIRE_FALSE(lib::io::is_option_set(ss2, property_bit_position));
    }

    std::stringstream ss1;
    std::stringstream ss2;

    const lib::io::bit_position_type property_bit_position = constants::first_element_idx;
};

TEST_CASE_FIXTURE(
    test_stream_options_manipulator, "stream_options_manipulator should properly handle istream property operations"
) {
    ss1 >> lib::io::set_option(property_bit_position);
    CHECK(lib::io::is_option_set(ss1, property_bit_position));
    CHECK_FALSE(lib::io::is_option_set(ss2, property_bit_position));

    ss2 >> lib::io::set_option(property_bit_position);
    CHECK(lib::io::is_option_set(ss1, property_bit_position));
    CHECK(lib::io::is_option_set(ss2, property_bit_position));

    ss1 >> lib::io::unset_option(property_bit_position);
    CHECK_FALSE(lib::io::is_option_set(ss1, property_bit_position));
    CHECK(lib::io::is_option_set(ss2, property_bit_position));

    ss2 >> lib::io::unset_option(property_bit_position);
    CHECK_FALSE(lib::io::is_option_set(ss1, property_bit_position));
    CHECK_FALSE(lib::io::is_option_set(ss2, property_bit_position));
}

TEST_CASE_FIXTURE(
    test_stream_options_manipulator, "stream_options_manipulator should properly handle ostream property operations"
) {
    ss1 << lib::io::set_option(property_bit_position);
    CHECK(lib::io::is_option_set(ss1, property_bit_position));
    CHECK_FALSE(lib::io::is_option_set(ss2, property_bit_position));

    ss2 << lib::io::set_option(property_bit_position);
    CHECK(lib::io::is_option_set(ss1, property_bit_position));
    CHECK(lib::io::is_option_set(ss2, property_bit_position));

    ss1 << lib::io::unset_option(property_bit_position);
    CHECK_FALSE(lib::io::is_option_set(ss1, property_bit_position));
    CHECK(lib::io::is_option_set(ss2, property_bit_position));

    ss2 << lib::io::unset_option(property_bit_position);
    CHECK_FALSE(lib::io::is_option_set(ss1, property_bit_position));
    CHECK_FALSE(lib::io::is_option_set(ss2, property_bit_position));
}

TEST_SUITE_END(); // test_stream_options_manipulator

} // namespace gl_testing
