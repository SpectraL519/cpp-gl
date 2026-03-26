#pragma once

#include "testing/gl/types.hpp"

#include <gl/constants.hpp>
#include <gl/types/core.hpp>

#include <ranges>

#define IC inline constexpr

namespace gl_testing::constants {

// n_elements for simple tests
IC gl::size_type n_elements = 3uz;

// n_elements for graph topology tests
IC gl::size_type n_elements_top = 10uz;

// n_elements for graph algorithm tests
IC gl::size_type n_elements_alg = 10uz;
IC gl::size_type depth = 5uz;

IC gl::size_type first_elem_idx = 0uz;
IC gl::size_type out_of_rng_idx = n_elements;

IC gl::default_id_type v1_id = 0u;
IC gl::default_id_type v2_id = 1u;
IC gl::default_id_type v3_id = 2u;

IC auto vertex_id_view = std::views::iota(gl::initial_id_v<gl::default_id_type>, n_elements);

IC visited_property visited{true};
IC visited_property not_visited{false};

IC used_property used{true};
IC used_property not_used{false};

} // namespace gl_testing::constants
