#pragma once

#include "io_handlers.hpp"

namespace gl::io {

enum class option : bit_position_type {
    with_vertex_properties = 0ul,
    with_edge_properties = 1ul
};

} // namespace gl::io
