#pragma once

#include "namespaces.hpp"
#include "types.hpp"

#include <gl/types/types.hpp>

#include <ranges>

namespace gl_testing::constants {

constexpr lib_t::size_type two = 2ull;

constexpr lib_t::size_type n_elements = 3ull;
constexpr lib_t::size_type zero_elements = 0ull;
constexpr lib_t::size_type one_element = 1ull;

// n_elements for graph topology tests
constexpr lib_t::size_type n_elements_top = 10ull;

constexpr lib_t::size_type first_element_idx = 0ull;
constexpr lib_t::size_type last_element_idx = n_elements - one_element;
constexpr lib_t::size_type out_of_range_elemenet_idx = n_elements;

constexpr lib_t::id_type vertex_id_1 = first_element_idx;
constexpr lib_t::id_type vertex_id_2 = vertex_id_1 + one_element;
constexpr lib_t::id_type vertex_id_3 = vertex_id_2 + one_element;

constexpr auto vertex_id_view = std::views::iota(first_element_idx, n_elements);

constexpr types::it_distance_type empty_distance = 0;

constexpr types::visited_property visited{true};
constexpr types::visited_property not_visited{false};

constexpr types::used_property used{true};
constexpr types::used_property not_used{false};

} // namespace gl_testing::constants
