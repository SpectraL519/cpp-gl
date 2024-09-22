#pragma once

#include "io_handlers.hpp"

namespace gl::io {

enum class option : bit_position_type {
    verbose = 0ul,
    with_vertex_properties = 1ul,
    with_edge_properties = 2ul
};

inline const stream_option_manipulator verbose = set_option(option::verbose);
inline const stream_option_manipulator concise = unset_option(option::verbose);

inline const stream_option_manipulator with_vertex_properties = set_option(option::with_vertex_properties);
inline const stream_option_manipulator without_vertex_properties = unset_option(option::with_vertex_properties);

inline const stream_option_manipulator with_edge_properties = set_option(option::with_edge_properties);
inline const stream_option_manipulator without_edge_properties = unset_option(option::with_edge_properties);

} // namespace gl::io
