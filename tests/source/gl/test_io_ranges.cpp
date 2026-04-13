#include "doctest.h"

#include <gl/io/ranges.hpp>

#include <sstream>
#include <string>
#include <vector>

namespace gl_testing {

TEST_SUITE_BEGIN("test_io_range_formatter");

struct test_io_ranges {
    std::stringstream ss;

    template <typename Formatter>
    std::string format(Formatter&& fmt) {
        ss.str("");
        ss.clear();
        ss << std::forward<Formatter>(fmt);
        return ss.str();
    }
};

struct test_io_range_formatter : public test_io_ranges {
    std::vector<int> empty_vec{};
    std::vector<int> single_vec{42};
    std::vector<int> multi_vec{1, 2, 3};
};

TEST_CASE_FIXTURE(test_io_range_formatter, "should properly format generic ranges") {
    // Default formatting
    CHECK(format(gl::io::range_formatter{empty_vec}) == "[]");
    CHECK(format(gl::io::range_formatter{single_vec}) == "[42]");
    CHECK(format(gl::io::range_formatter{multi_vec}) == "[1, 2, 3]");

    // Custom separators and bounds
    CHECK(format(gl::io::range_formatter{multi_vec, " | ", "<", ">"}) == "<1 | 2 | 3>");
}

TEST_CASE_FIXTURE(test_io_range_formatter, "should properly format sets using set_formatter") {
    CHECK(format(gl::io::set_formatter(empty_vec)) == "{}");
    CHECK(format(gl::io::set_formatter(single_vec)) == "{42}");
    CHECK(format(gl::io::set_formatter(multi_vec)) == "{1, 2, 3}");

    // Custom separator
    CHECK(format(gl::io::set_formatter(multi_vec, "; ")) == "{1; 2; 3}");
}

TEST_CASE_FIXTURE(
    test_io_range_formatter, "set_formatter should safely handle rvalues and views in set_formatter"
) {
    // Rvalue temporary containers
    CHECK(format(gl::io::set_formatter(std::vector<int>{7, 8, 9})) == "{7, 8, 9}");

    // Lazy std::ranges::views
    auto transformed_view = multi_vec | std::views::transform([](int x) { return x * 10; });
    CHECK(format(gl::io::set_formatter(transformed_view)) == "{10, 20, 30}");
}

TEST_CASE_FIXTURE(test_io_range_formatter, "should properly format multiline sets") {
    CHECK(format(gl::io::multiline_set_formatter(empty_vec)) == "{\n  \n}");
    CHECK(format(gl::io::multiline_set_formatter(single_vec)) == "{\n  42\n}");
    CHECK(format(gl::io::multiline_set_formatter(multi_vec)) == "{\n  1,\n  2,\n  3\n}");
}

struct test_io_implicit_range_formatter : public test_io_ranges {};

TEST_CASE_FIXTURE(
    test_io_implicit_range_formatter, "should properly format zero-based implicit ranges"
) {
    // Exclusive (default)
    CHECK(format(gl::io::implicit_range(0uz)) == "{}");
    CHECK(format(gl::io::implicit_range(1uz)) == "{0}");
    CHECK(format(gl::io::implicit_range(2uz)) == "{0, 1}");
    CHECK(format(gl::io::implicit_range(5uz)) == "{0, ..., 4}");

    // Inclusive
    CHECK(format(gl::io::implicit_range(0uz, true)) == "{0}");
    CHECK(format(gl::io::implicit_range(1uz, true)) == "{0, 1}");
    CHECK(format(gl::io::implicit_range(4uz, true)) == "{0, ..., 4}");
}

TEST_CASE_FIXTURE(
    test_io_implicit_range_formatter, "should properly format arbitrary bound implicit ranges"
) {
    // Signed integers (exclusive)
    CHECK(format(gl::io::implicit_range(10, 10)) == "{}");
    CHECK(format(gl::io::implicit_range(10, 11)) == "{10}");
    CHECK(format(gl::io::implicit_range(10, 12)) == "{10, 11}");
    CHECK(format(gl::io::implicit_range(10, 15)) == "{10, ..., 14}");

    // Signed integers (inclusive)
    CHECK(format(gl::io::implicit_range(10, 10, true)) == "{10}");
    CHECK(format(gl::io::implicit_range(10, 11, true)) == "{10, 11}");
    CHECK(format(gl::io::implicit_range(10, 14, true)) == "{10, ..., 14}");

    // Negative ranges
    CHECK(format(gl::io::implicit_range(-5, -2)) == "{-5, ..., -3}");
    CHECK(format(gl::io::implicit_range(-2, 0)) == "{-2, -1}");
}

TEST_SUITE_END(); // test_io_range_formatter

} // namespace gl_testing
