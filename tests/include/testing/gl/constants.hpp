#pragma once

#include "testing/gl/types.hpp"

#include <gl/constants.hpp>
#include <gl/types/core.hpp>

#include <ranges>

#define IC inline constexpr

namespace gl_testing::constants {

IC gl::size_type zero = 0ull;
IC gl::size_type one = 1ull;
IC gl::size_type two = 2ull;
IC gl::size_type three = 3ull;

IC gl::size_type n_elements = three;
IC gl::size_type zero_elements = zero;
IC gl::size_type one_element = one;

// n_elements for graph topology tests
IC gl::size_type n_elements_top = 10ull;

// n_elements for graph algorithm tests
IC gl::size_type n_elements_alg = 10ull;
IC gl::size_type depth = 5ull;

IC gl::size_type first_element_idx = zero;
IC gl::size_type last_element_idx = n_elements - one_element;
IC gl::size_type out_of_range_element_idx = n_elements;

IC gl::id_type vertex_id_1 = first_element_idx;
IC gl::id_type vertex_id_2 = vertex_id_1 + one_element;
IC gl::id_type vertex_id_3 = vertex_id_2 + one_element;
IC gl::id_type invalid_id = gl::constants::invalid_id;

IC auto vertex_id_view = std::views::iota(first_element_idx, n_elements);

IC visited_property visited{true};
IC visited_property not_visited{false};

IC used_property used{true};
IC used_property not_used{false};

} // namespace gl_testing::constants
