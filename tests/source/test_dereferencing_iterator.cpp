#include "constants.hpp"
#include "functional.hpp"

#include <gl/types/dereferencing_iterator.hpp>
#include <gl/types/iterator_range.hpp>

#include <doctest.h>

#include <algorithm>
#include <deque>
#include <forward_list>
#include <list>

namespace gl_testing {

TEST_SUITE_BEGIN("test_dereferencing_iterator");

struct data {
    lib_t::id_type id;
    std::string str;

    friend bool operator==(const data&, const data&) = default;
};

using data_uptr = std::unique_ptr<data>;
using data_sptr = std::shared_ptr<data>;
using data_rawptr = data*;

template <typename Container>
struct test_dereferencing_iterator {
    using container_type = Container;
    using data_ptr_type = typename container_type::value_type;
    using sut_type = lib_t::dereferencing_iterator<typename container_type::iterator>;

    struct address_projection {
        auto operator()(const data& data) {
            return &data;
        }

        auto operator()(const data_ptr_type& data_ptr)
        requires(lib_tt::c_strong_smart_ptr<data_ptr_type>)
        {
            return data_ptr.get();
        }

        auto operator()(const data_ptr_type data_ptr)
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
    std::deque<data_rawptr>, // random access iterator
    std::list<data_uptr>, // bidirectional iterator
    std::list<data_sptr>, // bidirectional iterator
    std::list<data_rawptr>, // bidirectional iterator
    std::forward_list<data_uptr>, // bidirectional iterator
    std::forward_list<data_sptr>, // bidirectional iterator
    std::forward_list<data_rawptr> // bidirectional iterator
);

// TODO: add specific tests covering the individual methods

TEST_SUITE_END(); // test_dereferencing_iterator

} // namespace gl_testing
