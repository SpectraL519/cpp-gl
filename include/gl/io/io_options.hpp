#pragma once

#include "io_handlers.hpp"

namespace gl::io {

enum class option : bit_position_type {
    verbose = 0ul,
    with_vertex_properties = 1ul,
    with_edge_properties = 2ul
};

} // namespace gl::io
