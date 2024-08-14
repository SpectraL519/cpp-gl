#include "functional.hpp"

#include <gl/types/iterator_range.hpp>

#include <doctest.h>

#include <algorithm>
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

    static constexpr std::size_t size = 3ull;
    static constexpr std::size_t first_element = 0ull;
    container_type container;

    lib_t::iterator_range<iterator_type> sut;
};

TEST_CASE_TEMPLATE_DEFINE("common iterator_range tests", ContainerType, container_type_template) {
    using container_type = ContainerType;
    using fixture_type = test_iterator_range_common<container_type>;
    using iterator_type = typename fixture_type::iterator_type;

    fixture_type fixture;

    auto& sut = fixture.sut;
    auto& container = fixture.container;

    SUBCASE("should properly initialze the begin and end iterators") {
        CHECK_EQ(sut.begin(), container.begin());
        CHECK_EQ(sut.end(), container.end());

#if __cplusplus >= 202302L
        CHECK_EQ(sut.cbegin(), container.cbegin());
        CHECK_EQ(sut.cend(), container.cend());
#endif
    }

    SUBCASE("should properly initialize the begin and end iterator for a range constructor") {
        lib_t::iterator_range<iterator_type> range_constructed_sut{container};

        CHECK_EQ(range_constructed_sut.begin(), std::ranges::begin(container));
        CHECK_EQ(range_constructed_sut.end(), std::ranges::end(container));

#if __cplusplus >= 202302L
        CHECK_EQ(range_constructed_sut.cbegin(), std::ranges::cbegin(container));
        CHECK_EQ(range_constructed_sut.cend(), std::ranges::cend(container));
#endif
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

    SUBCASE("should be compatible with std functions") {
        CHECK(std::equal(std::begin(sut), std::end(sut), container.begin()));

#if __cplusplus >= 202302L
        CHECK(std::equal(std::cbegin(sut), std::cend(sut), container.cbegin()));
#endif
    }

    SUBCASE("should be compatible with std::ranges functions") {
        CHECK(std::ranges::equal(sut, container));
    }

    SUBCASE("distance should return the distance between begin and end iterators") {
        CHECK_EQ(sut.distance(), fixture_type::size);
    }

    SUBCASE("element_at should throw when index is out of range") {
        CHECK_THROWS_AS(
            func::discard_result(sut.element_at(fixture_type::size)), std::out_of_range
        );
    }

    SUBCASE("element_at should return a reference to the correct element") {
        iterator_type it = container.begin();
        for (std::size_t n = 0; n < fixture_type::size; n++)
            CHECK_EQ(std::addressof(sut.element_at(n)), std::addressof(*it++));
    }

    SUBCASE("advance_begin should move the begin iterator by the the specified offset") {
        for (std::ptrdiff_t n = 0ull; n < static_cast<std::ptrdiff_t>(fixture_type::size); n++) {
            SUBCASE(("n = " + std::to_string(n)).c_str()) {
                iterator_type it = std::ranges::next(container.begin(), n);

                sut.advance_begin(n);
                CHECK_EQ(sut.distance(), std::ranges::distance(it, container.end()));
                CHECK_EQ(std::addressof(*sut.begin()), std::addressof(*it));
            }
        }
    }

    SUBCASE("advance_begin should move the begin iterator by the the specified offset for negative "
            "difference") {
        lib_t::iterator_range<iterator_type> narrow_range{container.end(), container.end()};

        for (std::ptrdiff_t n = 0ull; n < static_cast<std::ptrdiff_t>(fixture_type::size); n++) {
            const auto neg_n = -n;
            SUBCASE(("n = " + std::to_string(neg_n)).c_str()) {
                iterator_type it = std::ranges::next(container.end(), neg_n);

                narrow_range.advance_begin(neg_n);
                CHECK_EQ(narrow_range.distance(), std::ranges::distance(it, container.end()));
                CHECK_EQ(std::addressof(*narrow_range.begin()), std::addressof(*it));
            }
        }
    }

    SUBCASE("advance_end should move the end iterator by the specified offset") {
        lib_t::iterator_range<iterator_type> narrow_range{container.begin(), container.begin()};

        for (std::ptrdiff_t n = 0ull; n < static_cast<std::ptrdiff_t>(fixture_type::size); n++) {
            SUBCASE(("n = " + std::to_string(n)).c_str()) {
                iterator_type it = std::ranges::next(container.begin(), n);

                narrow_range.advance_end(n);
                CHECK_EQ(narrow_range.distance(), std::ranges::distance(container.begin(), it));
                CHECK_EQ(std::addressof(*narrow_range.end()), std::addressof(*it));
            }
        }
    }

    SUBCASE("advance_end should move the begin iterator by the the specified offset for negative "
            "difference") {
        for (std::ptrdiff_t n = 0ull; n < static_cast<std::ptrdiff_t>(fixture_type::size); n++) {
            const auto neg_n = -n;
            SUBCASE(("n = " + std::to_string(neg_n)).c_str()) {
                iterator_type it = std::ranges::next(container.end(), neg_n);

                sut.advance_end(neg_n);
                CHECK_EQ(sut.distance(), std::ranges::distance(container.begin(), it));
                CHECK_EQ(std::addressof(*sut.end()), std::addressof(*it));
            }
        }
    }

    SUBCASE("make_iterator_range should return a properly initialized iterator_range") {
        const auto range = lib::make_iterator_range(container.begin(), container.end());
        CHECK_EQ(range, sut);
    }

    SUBCASE("make_iterator_range should return an iterator_range properly initialized with the "
            "container") {
        const auto range = lib::make_iterator_range(container);
        CHECK_EQ(range, sut);
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
