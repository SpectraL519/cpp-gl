#include "constants.hpp"
#include "functional.hpp"

#include <gl/types/dereferencing_iterator.hpp>
#include <gl/types/iterator_range.hpp>

#include <doctest.h>

#include <algorithm>
#include <deque>
#include <forward_list>
#include <list>
#include <vector>

namespace gl_testing {

TEST_SUITE_BEGIN("test_dereferencing_iterator");

// TODO: add specific tests covering the individual methods

struct data {
    lib_t::id_type id;
    std::string str;

    friend bool operator==(const data&, const data&) = default;
};

using data_uptr = std::unique_ptr<data>;
using data_sptr = std::shared_ptr<data>;
using data_rptr = data*;

template <typename Container>
struct test_dereferencing_iterator {
    using container_type = Container;
    using data_ptr_type = typename container_type::value_type;
    using sut_type = lib_t::dereferencing_iterator<typename container_type::iterator>;

    struct address_projection {
        auto operator()(data& data) const {
            return &data;
        }

        auto operator()(const data_ptr_type& data_ptr) const
        requires(lib_tt::c_strong_smart_ptr<data_ptr_type>)
        {
            return data_ptr.get();
        }

        auto operator()(data_ptr_type data_ptr) const
        requires(not lib_tt::c_strong_smart_ptr<data_ptr_type>)
        {
            return data_ptr;
        }
    };

    test_dereferencing_iterator() {
        for (auto i = constants::first_element_idx; i < constants::n_elements; i++) {
            container.push_front(data_ptr_type{
                new data{i, std::to_string(i)}
            });
        }
    }

    container_type container;
};

TEST_CASE_TEMPLATE_DEFINE(
    "dereferencing_iterator should provide an 'equivalent' iterator range as a reference type "
    "range",
    ContainerType,
    container_type_template
) {
    using container_type = ContainerType;
    using fixture_type = test_dereferencing_iterator<container_type>;
    using sut_type = typename fixture_type::sut_type;
    using address_projection = typename fixture_type::address_projection;

    static_assert(std::forward_iterator<sut_type>);

    fixture_type fixture;

    SUBCASE("normal iterator") {
        const auto sut_range = lib::make_iterator_range(
            lib::deref_begin(fixture.container), lib::deref_end(fixture.container)
        );
        const auto ptr_range = lib::make_iterator_range(fixture.container);

        CHECK(std::ranges::equal(
            sut_range, ptr_range, std::ranges::equal_to{}, address_projection{}, address_projection{}
        ));
    }

    SUBCASE("const iterator") {
        const auto sut_crange = lib::make_iterator_range(
            lib::deref_cbegin(fixture.container), lib::deref_cend(fixture.container)
        );
        const auto ptr_crange = lib::make_const_iterator_range(fixture.container);

        CHECK(std::ranges::equal(
            sut_crange,
            ptr_crange,
            std::ranges::equal_to{},
            address_projection{},
            address_projection{}
        ));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    container_type_template,
    std::deque<data_uptr>, // random access iterator
    std::deque<data_sptr>, // random access iterator
    std::deque<data_rptr>, // random access iterator
    std::list<data_uptr>, // bidirectional iterator
    std::list<data_sptr>, // bidirectional iterator
    std::list<data_rptr>, // bidirectional iterator
    std::forward_list<data_uptr>, // forward iterator
    std::forward_list<data_sptr>, // forward iterator
    std::forward_list<data_rptr> // forward iterator
);

TEST_CASE_TEMPLATE_DEFINE(
    "dereferencing_iterator should provide an 'equivalent' decrementable iterator range"
    "as a reference type decrementable range",
    ContainerType,
    bidir_container_type_template
) {
    using container_type = ContainerType;
    using fixture_type = test_dereferencing_iterator<container_type>;
    using sut_type = typename fixture_type::sut_type;
    using address_projection = typename fixture_type::address_projection;

    static_assert(std::bidirectional_iterator<sut_type>);

    fixture_type fixture;

    SUBCASE("normal iterator") {
        // Create dereferencing iterator range
        auto sut_it = lib::deref_end(fixture.container);
        auto sut_begin = lib::deref_begin(fixture.container);

        auto ptr_it = std::ranges::end(fixture.container);
        auto ptr_begin = std::ranges::begin(fixture.container);

        const auto exepcted_size = std::ranges::distance(ptr_begin, ptr_it);

        // Collect elements while iterating backwards
        std::vector<data_rptr> sut_elements;
        std::vector<data_rptr> ptr_elements;
        const address_projection proj{};

        do {
            sut_elements.push_back(proj(*(--sut_it)));
            ptr_elements.push_back(proj(*(--ptr_it)));
        } while (sut_it != sut_begin && ptr_it != ptr_begin);

        REQUIRE_EQ(sut_it, sut_begin);
        REQUIRE_EQ(ptr_it, ptr_begin);

        REQUIRE_EQ(sut_elements.size(), exepcted_size);
        REQUIRE_EQ(ptr_elements.size(), exepcted_size);

        CHECK_EQ(sut_elements, ptr_elements);
    }

    SUBCASE("const iterator") {
        // Create dereferencing iterator range
        auto sut_it = lib::deref_cend(fixture.container);
        auto sut_begin = lib::deref_cbegin(fixture.container);

        auto ptr_it = std::ranges::cend(fixture.container);
        auto ptr_begin = std::ranges::cbegin(fixture.container);

        const auto exepcted_size = std::ranges::distance(ptr_begin, ptr_it);

        // Collect elements while iterating backwards
        std::vector<data_rptr> sut_elements;
        std::vector<data_rptr> ptr_elements;
        const address_projection proj{};

        do {
            sut_elements.push_back(proj(*(--sut_it)));
            ptr_elements.push_back(proj(*(--ptr_it)));
        } while (sut_it != sut_begin and ptr_it != ptr_begin);

        REQUIRE_EQ(sut_it, sut_begin);
        REQUIRE_EQ(ptr_it, ptr_begin);

        REQUIRE_EQ(sut_elements.size(), exepcted_size);
        REQUIRE_EQ(ptr_elements.size(), exepcted_size);

        CHECK_EQ(sut_elements, ptr_elements);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    bidir_container_type_template,
    std::deque<data_uptr>, // random access iterator
    std::deque<data_sptr>, // random access iterator
    std::deque<data_rptr>, // random access iterator
    std::list<data_uptr>, // bidirectional iterator
    std::list<data_sptr>, // bidirectional iterator
    std::list<data_rptr> // bidirectional iterator
);

TEST_SUITE_END(); // test_dereferencing_iterator

} // namespace gl_testing
