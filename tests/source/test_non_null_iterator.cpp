#include "constants.hpp"
#include "functional.hpp"

#include <gl/types/iterator_range.hpp>
#include <gl/types/non_null_iterator.hpp>

#include <doctest.h>

#include <algorithm>
#include <deque>
#include <forward_list>
#include <list>

namespace gl_testing {

TEST_SUITE_BEGIN("test_non_null_iterator");

struct data {
    lib_t::id_type id;
    std::string str;

    friend bool operator==(const data&, const data&) = default;
};

using data_uptr = std::unique_ptr<data>;
using data_sptr = std::shared_ptr<data>;

template <typename Container>
struct test_non_null_iterator {
    using container_type = Container;
    using data_ptr_type = typename container_type::value_type;
    using sut_type = lib_t::non_null_iterator<typename container_type::iterator>;

    struct reference_projection {
        auto operator()(const data_ptr_type& data_ptr) {
            return *data_ptr;
        }
    };

    test_non_null_iterator() {
        for (auto i = constants::first_element_idx; i < constants::n_elements; i++) {
            container.push_front(nullptr);
            container.push_front(data_ptr_type{
                new data{i, std::to_string(i)}
            });
            container.push_front(nullptr);

            non_null_container.push_front(data_ptr_type{
                new data{i, std::to_string(i)}
            });
        }
    }

    static constexpr std::size_t size = 3ull;
    static constexpr std::size_t first_element = 0ull;
    container_type container;
    container_type non_null_container;
};

TEST_CASE_TEMPLATE_DEFINE(
    "non_null_iterator should provide an 'equivalent' iterator range as a non null container",
    ContainerType,
    container_type_template
) {
    using container_type = ContainerType;
    using fixture_type = test_non_null_iterator<container_type>;
    using sut_type = typename fixture_type::sut_type;
    using reference_projection = typename fixture_type::reference_projection;

    static_assert(std::forward_iterator<sut_type>);

    fixture_type fixture;

    const auto sut_begin = lib::non_null_begin(fixture.container);
    const auto sut_end = lib::non_null_end(fixture.container);

    const auto sut_range = lib::make_iterator_range(sut_begin, sut_end);
    const auto non_null_range = lib::make_iterator_range(fixture.non_null_container);

    CHECK(std::ranges::equal(
        sut_range,
        non_null_range,
        std::ranges::equal_to{},
        reference_projection{},
        reference_projection{}
    ));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    container_type_template,
    std::deque<data_uptr>, // random access iterator
    std::deque<data_sptr>, // random access iterator
    std::list<data_uptr>, // bidirectional iterator
    std::list<data_sptr>, // bidirectional iterator
    std::forward_list<data_uptr>, // bidirectional iterator
    std::forward_list<data_sptr> // bidirectional iterator
);

// TODO: add specific tests covering the individual methods

TEST_SUITE_END(); // test_non_null_iterator

} // namespace gl_testing
