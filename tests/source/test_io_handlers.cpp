#include "constants.hpp"

#include <gl/io/io_handlers.hpp>

#include <doctest.h>

#include <sstream>

namespace gl_testing {

TEST_SUITE_BEGIN("test_io_handlers");

struct test_io_handlers {
    test_io_handlers() {
        REQUIRE_FALSE(lib::io::has_property_at(ss1, property_bit_position));
        REQUIRE_FALSE(lib::io::has_property_at(ss2, property_bit_position));
    }

    std::stringstream ss1;
    std::stringstream ss2;

    const lib::io::bit_position_type property_bit_position = constants::first_element_idx;
};

TEST_CASE_FIXTURE(
    test_io_handlers,
    "stream_property_manipulator should properly handle istream property operations"
) {
    ss1 >> lib::io::set_property_at(property_bit_position);
    CHECK(lib::io::has_property_at(ss1, property_bit_position));
    CHECK_FALSE(lib::io::has_property_at(ss2, property_bit_position));

    ss2 >> lib::io::set_property_at(property_bit_position);
    CHECK(lib::io::has_property_at(ss1, property_bit_position));
    CHECK(lib::io::has_property_at(ss2, property_bit_position));

    ss1 >> lib::io::unset_property_at(property_bit_position);
    CHECK_FALSE(lib::io::has_property_at(ss1, property_bit_position));
    CHECK(lib::io::has_property_at(ss2, property_bit_position));

    ss2 >> lib::io::unset_property_at(property_bit_position);
    CHECK_FALSE(lib::io::has_property_at(ss1, property_bit_position));
    CHECK_FALSE(lib::io::has_property_at(ss2, property_bit_position));
}

TEST_CASE_FIXTURE(
    test_io_handlers,
    "stream_property_manipulator should properly handle ostream property operations"
) {
    ss1 << lib::io::set_property_at(property_bit_position);
    CHECK(lib::io::has_property_at(ss1, property_bit_position));
    CHECK_FALSE(lib::io::has_property_at(ss2, property_bit_position));

    ss2 << lib::io::set_property_at(property_bit_position);
    CHECK(lib::io::has_property_at(ss1, property_bit_position));
    CHECK(lib::io::has_property_at(ss2, property_bit_position));

    ss1 << lib::io::unset_property_at(property_bit_position);
    CHECK_FALSE(lib::io::has_property_at(ss1, property_bit_position));
    CHECK(lib::io::has_property_at(ss2, property_bit_position));

    ss2 << lib::io::unset_property_at(property_bit_position);
    CHECK_FALSE(lib::io::has_property_at(ss1, property_bit_position));
    CHECK_FALSE(lib::io::has_property_at(ss2, property_bit_position));
}

TEST_SUITE_END(); // test_io_handlers

} // namespace gl_testing
