#include "functional.hpp"

#include <gl/types/properties.hpp>

#include <doctest.h>

#include <algorithm>
#include <sstream>
#include <vector>

namespace gl_testing {

TEST_SUITE_BEGIN("test_properties");

struct test_name_property {
    using sut_type = lib_t::name_property;
    using value_type = typename sut_type::value_type;

    const value_type value = "element name";
    sut_type sut{value};

    std::stringstream ss;
};

TEST_CASE_FIXTURE(
    test_name_property, "name() should return the name the porperty was initialized with"
) {
    CHECK_EQ(sut.name(), value);
}

TEST_CASE_FIXTURE(
    test_name_property, "operator<< should insert a quoted string to the output stream"
) {
    ss << sut;

    std::stringstream quoted_ss;
    quoted_ss << std::quoted(value);

    CHECK_EQ(ss.str(), quoted_ss.str());
}

TEST_CASE_FIXTURE(
    test_name_property, "operator>> should read the first word of a non-quoted string"
) {
    const std::string first_word = "input";
    const std::string input_name = first_word + " element name";

    ss << input_name;

    ss >> sut;
    CHECK_EQ(sut.name(), first_word);
}

TEST_CASE_FIXTURE(
    test_name_property, "operator>> should read a quoted string from the input stream"
) {
    const std::string input_name = "input element name";
    ss << std::quoted(input_name);

    ss >> sut;
    CHECK_EQ(sut.name(), input_name);
}

struct test_dynamic_properties {
    using sut_type = lib_t::dynamic_properties;
    using key_type = typename sut_type::key_type;

    struct compound_value {
        int x;
        double y;

        bool operator==(const compound_value&) const = default;
    };

    sut_type sut;

    const key_type key{"key"};
    const key_type not_present_key{"not present key"};
    const compound_value value{1, 1.1};
    const compound_value new_value{2, 2.2};
};

TEST_CASE_FIXTURE(test_dynamic_properties, "the underlying map should be empty by default") {
    CHECK(sut.underlying().empty());
}

TEST_CASE_FIXTURE(test_dynamic_properties, "is_present should return false for a not present key") {
    CHECK_FALSE(sut.is_present(not_present_key));
}

TEST_CASE_FIXTURE(test_dynamic_properties, "is_present should return true for a present key") {
    sut.underlying()[key] = value;
    CHECK(sut.is_present(key));
}

TEST_CASE_FIXTURE(test_dynamic_properties, "get should throw for a not present key") {
    CHECK_THROWS_AS(func::discard_result(sut.get<int>(not_present_key)), std::out_of_range);
}

TEST_CASE_FIXTURE(test_dynamic_properties, "get should throw for an invalid value type") {
    sut.underlying()[key] = value;
    CHECK_THROWS_AS(func::discard_result(sut.get<double>(key)), std::bad_any_cast);
}

TEST_CASE_FIXTURE(
    test_dynamic_properties,
    "get should return a reference to the underlying object for a valid key and type"
) {
    sut.underlying()[key] = value;

    compound_value& value_ref = sut.get<compound_value>(key);
    REQUIRE_EQ(value_ref, value);

    value_ref = new_value;
    CHECK_EQ(sut.get<compound_value>(key), new_value);
}

TEST_CASE_FIXTURE(
    test_dynamic_properties, "set should properly set/override the value for the given key"
) {
    sut.set<compound_value>(key, value);
    REQUIRE(sut.is_present(key));
    CHECK_EQ(sut.get<compound_value>(key), value);

    sut.set<compound_value>(key, new_value);
    REQUIRE(sut.is_present(key));
    CHECK_EQ(sut.get<compound_value>(key), new_value);
}

TEST_CASE_FIXTURE(
    test_dynamic_properties, "mvset should properly set/override the value for the given key"
) {
    sut.mvset<compound_value>(key, compound_value{value});
    REQUIRE(sut.is_present(key));
    CHECK_EQ(sut.get<compound_value>(key), value);

    sut.mvset<compound_value>(key, compound_value{new_value});
    REQUIRE(sut.is_present(key));
    CHECK_EQ(sut.get<compound_value>(key), new_value);
}

TEST_CASE_FIXTURE(
    test_dynamic_properties, "emplace should properly set/override the value for the given key"
) {
    sut.emplace<compound_value>(key, value.x, value.y);
    REQUIRE(sut.is_present(key));
    CHECK_EQ(sut.get<compound_value>(key), value);

    sut.emplace<compound_value>(key, new_value.x, new_value.y);
    REQUIRE(sut.is_present(key));
    CHECK_EQ(sut.get<compound_value>(key), new_value);
}

TEST_CASE_FIXTURE(test_dynamic_properties, "remove should properly erase the key-value pair") {
    sut.set<compound_value>(key, value);
    REQUIRE(sut.is_present(key));

    sut.remove(key);
    REQUIRE_FALSE(sut.is_present(key));
}

struct test_binary_color {
    using sut_type = lib_t::binary_color;
    using color = sut_type::color_value;

    static constexpr color out_of_bounds_color =
        static_cast<color>(lib::util::to_underlying(color::unset) + 1);
};

TEST_CASE_FIXTURE(test_binary_color, "should be unset by default") {
    sut_type sut{};
    REQUIRE_EQ(sut, color::unset);
}

TEST_CASE_FIXTURE(test_binary_color, "out of bounds values should be restricted to `unset`") {
    sut_type sut{out_of_bounds_color};

    REQUIRE_EQ(sut, color::unset);
    CHECK_EQ(sut.to_underlying(), lib::util::to_underlying(color::unset));
}

TEST_CASE_FIXTURE(
    test_binary_color, "is_set should return false for any value other than black and white"
) {
    REQUIRE_FALSE(sut_type{}.is_set());
    REQUIRE_FALSE(sut_type{out_of_bounds_color}.is_set());
}

TEST_CASE_FIXTURE(test_binary_color, "operator bool should be equivalent to is_set") {
    const std::vector<sut_type> colors{
        color::black, color::white, color::unset, out_of_bounds_color
    };

    CHECK(std::ranges::all_of(colors, [](const sut_type& c) {
        return static_cast<bool>(c) == c.is_set();
    }));
}

TEST_SUITE_END(); // test_properties

} // namespace gl_testing
