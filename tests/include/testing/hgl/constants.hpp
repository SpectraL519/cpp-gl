#pragma once

#include "types.hpp"

#include <hgl/constants.hpp>
#include <hgl/types/types.hpp>

#define IC inline constexpr

namespace hgl_testing::constants {

IC hgl::types::size_type n_vertices = 5uz;
IC hgl::types::size_type out_of_rng_vid = n_vertices;

IC auto vertex_ids_view = std::views::iota(hgl::constants::initial_id, n_vertices);

IC types::boolean_property p_true{true};
IC types::boolean_property p_false{false};

} // namespace hgl_testing::constants
