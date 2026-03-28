#pragma once

#include "types.hpp"

#include <hgl/constants.hpp>
#include <hgl/types.hpp>

#define IC inline constexpr

namespace hgl_testing::constants {

IC hgl::size_type n_vertices = 3uz;
IC hgl::size_type n_hyperedges = 4uz;

IC hgl::default_id_type id1 = hgl::constants::initial_id<hgl::default_id_type>;
IC hgl::default_id_type id2 = id1 + 1uz;
IC hgl::default_id_type id3 = id2 + 1uz;
IC hgl::default_id_type id4 = id3 + 1uz;
IC hgl::default_id_type out_of_rng_vid = n_vertices;
IC hgl::default_id_type out_of_rng_eid = n_hyperedges;

IC auto vertex_ids_view =
    std::views::iota(hgl::constants::initial_id<hgl::default_id_type>, n_vertices);
IC auto hyperedge_ids_view =
    std::views::iota(hgl::constants::initial_id<hgl::default_id_type>, n_hyperedges);

IC boolean_property p_true{true};
IC boolean_property p_false{false};

} // namespace hgl_testing::constants
