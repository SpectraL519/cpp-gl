#pragma once

#include "namespaces.hpp"
#include "types.hpp"

#include <gl/types/types.hpp>

#include <ranges>

#define IC inline constexpr

namespace gl_testing::constants {

IC lib_t::size_type zero = 0ull;
IC lib_t::size_type one = 1ull;
IC lib_t::size_type two = 2ull;
IC lib_t::size_type three = 3ull;

IC lib_t::size_type n_elements = three;
IC lib_t::size_type zero_elements = zero;
IC lib_t::size_type one_element = one;

// n_elements for graph topology tests
IC lib_t::size_type n_elements_top = 10ull;

IC lib_t::size_type first_element_idx = zero;
IC lib_t::size_type last_element_idx = n_elements - one_element;
IC lib_t::size_type out_of_range_elemenet_idx = n_elements;

IC lib_t::id_type vertex_id_1 = first_element_idx;
IC lib_t::id_type vertex_id_2 = vertex_id_1 + one_element;
IC lib_t::id_type vertex_id_3 = vertex_id_2 + one_element;

IC auto vertex_id_view = std::views::iota(first_element_idx, n_elements);

IC types::it_distance_type empty_distance = static_cast<types::it_distance_type>(zero);

IC types::visited_property visited{true};
IC types::visited_property not_visited{false};

IC types::used_property used{true};
IC types::used_property not_used{false};

} // namespace gl_testing::constants
