#include <gl/types/iterator_range.hpp>

#include <doctest.h>

#include <numeric>
#include <vector>

namespace gl_testing {

TEST_SUITE_BEGIN("test_iterator_range");

struct test_iterator_range {
    test_iterator_range() : container(size) {
        std::iota(container.begin(), container.end(), first_element);
    }

    static constexpr std::size_t size = 5ull;
    static constexpr std::size_t first_element = 0ull;
    std::vector<std::size_t> container;
};

TEST_CASE_FIXTURE(test_iterator_range, "should properly initialze the begin and end iterators") {
    lib_t::iterator_range sut{container.begin(), container.end()};

    CHECK_EQ(sut.begin(), container.begin());
    CHECK_EQ(sut.end(), container.end());
}

TEST_CASE_FIXTURE(test_iterator_range, "should be compatible with range based loops") {
    lib_t::iterator_range sut{container.begin(), container.end()};
    std::size_t element = first_element;

    for (const std::size_t range_element : sut)
        CHECK_EQ(range_element, element++);
}

TEST_SUITE_END(); // test_iterator_range

} // namespace gl_testing
