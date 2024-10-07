#include "constants.hpp"

#include <gl/io/stream_options_manipulator.hpp>

#include <doctest.h>

#include <sstream>

namespace gl_testing {

TEST_SUITE_BEGIN("test_stream_options_manipulator");

struct test_stream_options_manipulator {
    test_stream_options_manipulator() {
        REQUIRE_FALSE(lib::io::is_option_set(ss1, bit_position_1));
        REQUIRE_FALSE(lib::io::is_option_set(ss2, bit_position_1));
    }

    std::stringstream ss1;
    std::stringstream ss2;

    static constexpr lib::io::bit_position_type bit_position_1 = constants::first_element_idx;
    static constexpr lib::io::bit_position_type bit_position_2 = bit_position_1 + constants::one;
    static constexpr lib::io::iword_type options_bitmask =
        (lib::io::iword_bit << bit_position_1) | (lib::io::iword_bit << bit_position_2);
};

TEST_CASE_FIXTURE(
    test_stream_options_manipulator, "should properly handle istream option operations per stream"
) {
    ss1 >> lib::io::set_option(bit_position_1);
    CHECK(lib::io::is_option_set(ss1, bit_position_1));
    CHECK_FALSE(lib::io::is_option_set(ss2, bit_position_1));

    ss2 >> lib::io::set_option(bit_position_1);
    CHECK(lib::io::is_option_set(ss1, bit_position_1));
    CHECK(lib::io::is_option_set(ss2, bit_position_1));

    ss1 >> lib::io::unset_option(bit_position_1);
    CHECK_FALSE(lib::io::is_option_set(ss1, bit_position_1));
    CHECK(lib::io::is_option_set(ss2, bit_position_1));

    ss2 >> lib::io::unset_option(bit_position_1);
    CHECK_FALSE(lib::io::is_option_set(ss1, bit_position_1));
    CHECK_FALSE(lib::io::is_option_set(ss2, bit_position_1));
}

TEST_CASE_FIXTURE(
    test_stream_options_manipulator, "should properly handle ostream option operations per stream"
) {
    ss1 << lib::io::set_option(bit_position_1);
    CHECK(lib::io::is_option_set(ss1, bit_position_1));
    CHECK_FALSE(lib::io::is_option_set(ss2, bit_position_1));

    ss2 << lib::io::set_option(bit_position_1);
    CHECK(lib::io::is_option_set(ss1, bit_position_1));
    CHECK(lib::io::is_option_set(ss2, bit_position_1));

    ss1 << lib::io::unset_option(bit_position_1);
    CHECK_FALSE(lib::io::is_option_set(ss1, bit_position_1));
    CHECK(lib::io::is_option_set(ss2, bit_position_1));

    ss2 << lib::io::unset_option(bit_position_1);
    CHECK_FALSE(lib::io::is_option_set(ss1, bit_position_1));
    CHECK_FALSE(lib::io::is_option_set(ss2, bit_position_1));
}

enum class BitPositionEnum : lib::io::bit_position_type {
    bit_position_1 = test_stream_options_manipulator::bit_position_1,
    bit_position_2 = test_stream_options_manipulator::bit_position_2
};

TEST_CASE_FIXTURE(
    test_stream_options_manipulator,
    "should properly handle istream option operations for bit position lists"
) {
    const auto bit_positions = {bit_position_1, bit_position_2};

    ss1 >> lib::io::set_options(bit_positions);
    CHECK(lib::io::are_options_set(ss1, bit_positions));
    CHECK(lib::io::are_options_set(ss1, options_bitmask));

    ss1 >> lib::io::unset_options(bit_positions);
    CHECK_FALSE(lib::io::are_options_set(ss1, bit_positions));
    CHECK_FALSE(lib::io::are_options_set(ss1, options_bitmask));

    ss1 >> lib::io::set_options({bit_position_1});
    CHECK(lib::io::is_option_set(ss1, bit_position_1));
    CHECK(lib::io::are_options_set(ss1, {bit_position_1}));
    CHECK_FALSE(lib::io::is_option_set(ss1, bit_position_2));
    CHECK_FALSE(lib::io::are_options_set(ss1, {bit_position_2}));
    CHECK_FALSE(lib::io::are_options_set(ss1, bit_positions));
    CHECK_FALSE(lib::io::are_options_set(ss1, options_bitmask));
}

TEST_CASE_FIXTURE(
    test_stream_options_manipulator,
    "should properly handle osream option operations for bit position lists"
) {
    const auto bit_positions = {bit_position_1, bit_position_2};

    ss1 << lib::io::set_options(bit_positions);
    CHECK(lib::io::are_options_set(ss1, bit_positions));
    CHECK(lib::io::are_options_set(ss1, options_bitmask));

    ss1 << lib::io::unset_options(bit_positions);
    CHECK_FALSE(lib::io::are_options_set(ss1, bit_positions));
    CHECK_FALSE(lib::io::are_options_set(ss1, options_bitmask));

    ss1 << lib::io::set_options({bit_position_1});
    CHECK(lib::io::is_option_set(ss1, bit_position_1));
    CHECK(lib::io::are_options_set(ss1, {bit_position_1}));
    CHECK_FALSE(lib::io::is_option_set(ss1, bit_position_2));
    CHECK_FALSE(lib::io::are_options_set(ss1, {bit_position_2}));
    CHECK_FALSE(lib::io::are_options_set(ss1, bit_positions));
    CHECK_FALSE(lib::io::are_options_set(ss1, options_bitmask));
}

TEST_CASE_FIXTURE(
    test_stream_options_manipulator,
    "should properly handle istream option operations for enum bit position lists"
) {
    const auto bit_positions = {BitPositionEnum::bit_position_1, BitPositionEnum::bit_position_2};

    ss1 >> lib::io::set_options(bit_positions);
    CHECK(lib::io::are_options_set(ss1, bit_positions));
    CHECK(lib::io::are_options_set(ss1, options_bitmask));

    ss1 >> lib::io::unset_options(bit_positions);
    CHECK_FALSE(lib::io::are_options_set(ss1, bit_positions));
    CHECK_FALSE(lib::io::are_options_set(ss1, options_bitmask));

    ss1 >> lib::io::set_options({BitPositionEnum::bit_position_1});
    CHECK(lib::io::is_option_set(ss1, BitPositionEnum::bit_position_1));
    CHECK(lib::io::are_options_set(ss1, {BitPositionEnum::bit_position_1}));
    CHECK_FALSE(lib::io::is_option_set(ss1, BitPositionEnum::bit_position_2));
    CHECK_FALSE(lib::io::are_options_set(ss1, {BitPositionEnum::bit_position_2}));
    CHECK_FALSE(lib::io::are_options_set(ss1, bit_positions));
    CHECK_FALSE(lib::io::are_options_set(ss1, options_bitmask));
}

TEST_CASE_FIXTURE(
    test_stream_options_manipulator,
    "should properly handle ostream option operations for enum bit position lists"
) {
    const auto bit_positions = {BitPositionEnum::bit_position_1, BitPositionEnum::bit_position_2};

    ss1 << lib::io::set_options(bit_positions);
    CHECK(lib::io::are_options_set(ss1, bit_positions));
    CHECK(lib::io::are_options_set(ss1, options_bitmask));

    ss1 << lib::io::unset_options(bit_positions);
    CHECK_FALSE(lib::io::are_options_set(ss1, bit_positions));
    CHECK_FALSE(lib::io::are_options_set(ss1, options_bitmask));

    ss1 << lib::io::set_options({BitPositionEnum::bit_position_1});
    CHECK(lib::io::is_option_set(ss1, BitPositionEnum::bit_position_1));
    CHECK(lib::io::are_options_set(ss1, {BitPositionEnum::bit_position_1}));
    CHECK_FALSE(lib::io::is_option_set(ss1, BitPositionEnum::bit_position_2));
    CHECK_FALSE(lib::io::are_options_set(ss1, {BitPositionEnum::bit_position_2}));
    CHECK_FALSE(lib::io::are_options_set(ss1, bit_positions));
    CHECK_FALSE(lib::io::are_options_set(ss1, options_bitmask));
}

TEST_SUITE_END(); // test_stream_options_manipulator

} // namespace gl_testing
