#pragma once

#include "testing/gl/types.hpp"

#include <gl/constants.hpp>
#include <gl/types/core.hpp>

#include <ranges>

#define IC inline constexpr

namespace gl_testing::constants {

// n_elements for simple tests
IC gl::size_type n_elements = 3ull;

// n_elements for graph topology tests
IC gl::size_type n_elements_top = 10ull;

// n_elements for graph algorithm tests
IC gl::size_type n_elements_alg = 10ull;
IC gl::size_type depth = 5ull;

IC gl::size_type first_elem_idx = 0uz;
IC gl::size_type out_of_rng_idx = n_elements;

IC gl::id_type v1_id = 0uz;
IC gl::id_type v2_id = 1uz;
IC gl::id_type v3_id = 2uz;
IC gl::id_type invalid_id = gl::constants::invalid_id;

IC auto vertex_id_view = std::views::iota(0ull, n_elements);

IC visited_property visited{true};
IC visited_property not_visited{false};

IC used_property used{true};
IC used_property not_used{false};

} // namespace gl_testing::constants
