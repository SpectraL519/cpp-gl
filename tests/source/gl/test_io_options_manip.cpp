#include "doctest.h"

#include <gl/io/options_manip.hpp>

#include <sstream>

namespace gl_testing {

TEST_SUITE_BEGIN("test_io_options_manip");

struct test_io_options_manip {
    using sut_type = gl::io::options_manip;

    test_io_options_manip() {
        REQUIRE_FALSE(gl::io::is_option_set(ss1, bit_position_1));
        REQUIRE_FALSE(gl::io::is_option_set(ss2, bit_position_1));
    }

    std::stringstream ss1;
    std::stringstream ss2;

    static constexpr gl::io::bit_position_type bit_position_1 = 0uz;
    static constexpr gl::io::bit_position_type bit_position_2 = 1uz;
    static constexpr gl::io::bit_position_type bit_position_3 = 2uz;

    static constexpr gl::io::iword_type options_bitmask =
        (gl::io::iword_bit << bit_position_1) | (gl::io::iword_bit << bit_position_2);
};

enum class bit_pos_enum : gl::io::bit_position_type {
    bit_position_1 = test_io_options_manip::bit_position_1,
    bit_position_2 = test_io_options_manip::bit_position_2
};

TEST_CASE_FIXTURE(
    test_io_options_manip, "should properly handle single option operations (istream/ostream)"
) {
    ss1 << gl::io::set_options(bit_position_1);
    CHECK(gl::io::is_option_set(ss1, bit_position_1));
    CHECK_FALSE(gl::io::is_option_set(ss2, bit_position_1));

    ss1 << gl::io::clear_options(bit_position_1);
    CHECK_FALSE(gl::io::is_option_set(ss1, bit_position_1));

    ss2 >> gl::io::set_options(bit_position_1);
    CHECK(gl::io::is_option_set(ss2, bit_position_1));
    CHECK_FALSE(gl::io::is_option_set(ss1, bit_position_1));

    ss2 >> gl::io::clear_options(bit_position_1);
    CHECK_FALSE(gl::io::is_option_set(ss2, bit_position_1));
}

TEST_CASE_FIXTURE(test_io_options_manip, "should properly handle variadic option operations") {
    ss1 << gl::io::set_options(bit_position_1, bit_position_2);
    CHECK(gl::io::are_options_set(ss1, bit_position_1, bit_position_2));
    CHECK(gl::io::are_options_set(ss1, options_bitmask));

    ss1 << gl::io::clear_options(bit_position_1, bit_position_2);
    CHECK_FALSE(gl::io::are_options_set(ss1, bit_position_1, bit_position_2));
    CHECK_FALSE(gl::io::is_option_set(ss1, bit_position_1));
}

TEST_CASE_FIXTURE(
    test_io_options_manip, "should properly handle initializer list option operations"
) {
    const auto bit_positions = {bit_position_1, bit_position_2};

    ss1 << gl::io::set_options(bit_positions);
    CHECK(gl::io::are_options_set(ss1, bit_positions));
    CHECK(gl::io::are_options_set(ss1, options_bitmask));

    ss1 << gl::io::clear_options(bit_positions);
    CHECK_FALSE(gl::io::are_options_set(ss1, bit_positions));
    CHECK_FALSE(gl::io::is_option_set(ss1, bit_position_1));
}

TEST_CASE_FIXTURE(
    test_io_options_manip, "should properly handle enum option operations (variadic & list)"
) {
    ss1 << gl::io::set_options(bit_pos_enum::bit_position_1, bit_pos_enum::bit_position_2);
    CHECK(gl::io::are_options_set(ss1, bit_pos_enum::bit_position_1, bit_pos_enum::bit_position_2));
    CHECK(gl::io::are_options_set(ss1, options_bitmask));

    ss1 << gl::io::clear_options(bit_pos_enum::bit_position_1, bit_pos_enum::bit_position_2);
    CHECK_FALSE(gl::io::is_option_set(ss1, bit_pos_enum::bit_position_1));

    const auto enum_list = {bit_pos_enum::bit_position_1, bit_pos_enum::bit_position_2};
    ss1 >> gl::io::set_options(enum_list);
    CHECK(gl::io::are_options_set(ss1, enum_list));
    CHECK(gl::io::are_options_set(ss1, options_bitmask));
}

TEST_CASE_FIXTURE(
    test_io_options_manip, "should handle simultaneous set and clear operations (layout triads)"
) {
    // init state
    ss1 << gl::io::set_options(bit_position_1, bit_position_2);
    constexpr auto complex_manip = sut_type(
        gl::io::iword_bit << bit_position_3, // set mask
        gl::io::iword_bit << bit_position_1 // clear mask
    );

    ss1 << complex_manip;
    CHECK_FALSE(gl::io::is_option_set(ss1, bit_position_1));
    CHECK(gl::io::is_option_set(ss1, bit_position_2));
    CHECK(gl::io::is_option_set(ss1, bit_position_3));
}

TEST_CASE_FIXTURE(test_io_options_manip, "should properly reset/clear all options (default state)") {
    ss1 << gl::io::set_options(bit_position_1, bit_position_2, bit_position_3);

    constexpr auto reset_manip = sut_type(0ul, ~static_cast<gl::io::iword_type>(0));
    ss1 << reset_manip;

    CHECK_FALSE(gl::io::is_option_set(ss1, bit_position_1));
    CHECK_FALSE(gl::io::is_option_set(ss1, bit_position_2));
    CHECK_FALSE(gl::io::is_option_set(ss1, bit_position_3));
    CHECK_FALSE(gl::io::are_options_set(ss1, options_bitmask));
}

TEST_SUITE_END(); // test_io_options_manip

} // namespace gl_testing
