#include <gl/types/iterator_range.hpp>

#include <doctest.h>

#include <forward_list>
#include <list>
#include <numeric>
#include <vector>

namespace gl_testing {

TEST_SUITE_BEGIN("test_iterator_range");

template <typename Container>
struct test_iterator_range_common {
    using container_type = Container;
    using iterator_type = typename container_type::iterator;

    test_iterator_range_common() : container(size), sut(container.begin(), container.end()) {
        std::iota(container.begin(), container.end(), first_element);
    }

    static constexpr std::size_t size = 5ull;
    static constexpr std::size_t first_element = 0ull;
    container_type container;

    lib_t::iterator_range<iterator_type> sut;
};

TEST_CASE_TEMPLATE_DEFINE("common iterator_range tests", ContainerType, container_type_template) {
    using fixture_type = test_iterator_range_common<ContainerType>;
    using iterator_type = typename fixture_type::iterator_type;

    fixture_type fixture;

    auto& sut = fixture.sut;
    auto& container = fixture.container;

    SUBCASE("should properly initialze the begin and end iterators") {
        CHECK_EQ(sut.begin(), container.begin());
        CHECK_EQ(sut.end(), container.end());
    }

    SUBCASE("should properly initialize the begin and end iterator for a range constructor") {
        lib_t::iterator_range<iterator_type> range_constructed_sut{container};

        CHECK_EQ(sut.begin(), std::ranges::begin(container));
        CHECK_EQ(sut.end(), std::ranges::end(container));
    }

    SUBCASE("equality operator should return true only when both iterators are equal") {
        lib_t::iterator_range<iterator_type> range{sut};
        REQUIRE_EQ(sut, range);

        range = lib_t::iterator_range<iterator_type>{std::next(sut.begin()), sut.end()};
        CHECK_NE(sut, range);

        range = lib_t::iterator_range<iterator_type>{
            sut.begin(), std::next(sut.begin(), sut.distance() - 1)
        };
        CHECK_NE(sut, range);
    }

    SUBCASE("should be compatible with range based loops") {
        std::size_t element = fixture_type::first_element;

        for (const std::size_t range_element : sut)
            CHECK_EQ(range_element, element++);
    }

    SUBCASE("distance should return the distance between begin and end iterators") {
        CHECK_EQ(sut.distance(), fixture_type::size);
    }

    SUBCASE("get should return a reference to the correct element") {
        iterator_type it = container.begin();
        for (std::ptrdiff_t n = 0; n < fixture_type::size; n++)
            CHECK_EQ(std::addressof(sut.get(n)), std::addressof(*it++));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    container_type_template,
    std::vector<std::size_t>, // random access iterator
    std::list<std::size_t>, // bidirectional iterator
    std::forward_list<std::size_t> // forward iterator
);

TEST_SUITE_END(); // test_iterator_range

} // namespace gl_testing
