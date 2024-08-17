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

template <typename Container, lib::it_range_cache CacheType>
struct test_iterator_range_type_params {
    using container_type = Container;
    static constexpr lib::it_range_cache cache_type = CacheType;
};

template <typename TypeParams>
struct test_iterator_range {
    using container_type = typename TypeParams::container_type;
    using iterator_type = lib_tt::iterator_type<container_type>;
    using sut_type = lib_t::iterator_range<iterator_type, TypeParams::cache_type>;

    test_iterator_range() : container(size), sut(container.begin(), container.end()) {
        std::iota(container.begin(), container.end(), first_element);
    }

    static constexpr std::size_t size = 3ull;
    static constexpr std::size_t first_element = 0ull;
    container_type container;

    sut_type sut;
};

TEST_CASE_TEMPLATE_DEFINE("iterator_range tests", TypeParams, type_params_template) {
    using fixture_type = test_iterator_range<TypeParams>;
    using container_type = typename fixture_type::container_type;
    using iterator_type = typename fixture_type::iterator_type;
    using sut_type = typename fixture_type::sut_type;
    constexpr lib::it_range_cache cache_type = sut_type::cache_type;

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
        sut_type range_constructed_sut =
            lib::make_iterator_range<container_type, cache_type>(container);

        CHECK_EQ(range_constructed_sut.begin(), std::ranges::begin(container));
        CHECK_EQ(range_constructed_sut.end(), std::ranges::end(container));

#if __cplusplus >= 202302L
        CHECK_EQ(range_constructed_sut.cbegin(), std::ranges::cbegin(container));
        CHECK_EQ(range_constructed_sut.cend(), std::ranges::cend(container));
#endif
    }

    SUBCASE("equality operator should return true only when both iterators are equal") {
        sut_type range{sut};
        REQUIRE_EQ(sut, range);

        range = sut_type{std::next(sut.begin()), sut.end()};
        CHECK_NE(sut, range);

        range = sut_type{sut.begin(), std::next(sut.begin(), sut.distance() - 1)};
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

    SUBCASE("make_iterator_range should return a properly initialized iterator_range") {
        const auto range =
            lib::make_iterator_range<iterator_type, cache_type>(container.begin(), container.end());
        CHECK(std::ranges::equal(range, container));
    }

    SUBCASE("make_iterator_range should return an iterator_range properly initialized with the "
            "container") {
        const auto range = lib::make_iterator_range<container_type, cache_type>(container);
        CHECK(std::ranges::equal(range, container));
    }

    SUBCASE("make_const_iterator_range should return an iterator_range properly initialized with "
            "the container") {
        const auto range = lib::make_const_iterator_range<container_type, cache_type>(container);
        CHECK(std::ranges::equal(range, container));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    type_params_template,
    // cache_type::none
    test_iterator_range_type_params<
        std::vector<std::size_t>,
        lib::it_range_cache::none>, // random access iterator
    test_iterator_range_type_params<
        std::list<std::size_t>,
        lib::it_range_cache::none>, // bidirectional iterator
    test_iterator_range_type_params<
        std::forward_list<std::size_t>,
        lib::it_range_cache::none>, // forward iterator
    // cache_type::lazy
    test_iterator_range_type_params<
        std::vector<std::size_t>,
        lib::it_range_cache::lazy>, // random access iterator
    test_iterator_range_type_params<
        std::list<std::size_t>,
        lib::it_range_cache::lazy>, // bidirectional iterator
    test_iterator_range_type_params<
        std::forward_list<std::size_t>,
        lib::it_range_cache::lazy>, // forward iterator
    // cache_type::eager
    test_iterator_range_type_params<
        std::vector<std::size_t>,
        lib::it_range_cache::eager>, // random access iterator
    test_iterator_range_type_params<
        std::list<std::size_t>,
        lib::it_range_cache::eager>, // bidirectional iterator
    test_iterator_range_type_params<
        std::forward_list<std::size_t>,
        lib::it_range_cache::eager> // forward iterator
);

TEST_SUITE_END(); // test_iterator_range

} // namespace gl_testing
