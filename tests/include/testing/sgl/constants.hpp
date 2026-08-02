#pragma once

#include <sgl/types.hpp>

#define IC inline constexpr

namespace sgl_testing::constants {

IC sgl::size_type sketch_width = 16uz;
IC sgl::size_type invalid_width = 1uz;

IC sgl::default_id_type v0 = 0u;
IC sgl::default_id_type v1 = 1u;
IC sgl::default_id_type v2 = 2u;
IC sgl::default_id_type v3 = 3u;

IC sgl::default_weight_type unit_weight = 1.0;
IC sgl::default_weight_type zero_weight = 0.0;

} // namespace sgl_testing::constants
