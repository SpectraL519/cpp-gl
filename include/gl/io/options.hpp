#pragma once

#include "handlers.hpp"

namespace gl::io {

enum class option : bit_position_type {
    verbose = 0ul,
    with_vertex_properties = 1ul,
    with_edge_properties = 2ul,
    gsf = 3ul // graph specification format
};

inline const stream_option_manipulator verbose = set_option(option::verbose);
inline const stream_option_manipulator concise = unset_option(option::verbose);

inline const stream_option_manipulator with_vertex_properties =
    set_option(option::with_vertex_properties);
inline const stream_option_manipulator without_vertex_properties =
    unset_option(option::with_vertex_properties);

inline const stream_option_manipulator with_edge_properties =
    set_option(option::with_edge_properties);
inline const stream_option_manipulator without_edge_properties =
    unset_option(option::with_edge_properties);

inline const stream_option_manipulator with_properties =
    set_options({option::with_vertex_properties, option::with_edge_properties});
inline const stream_option_manipulator without_properties =
    unset_options({option::with_vertex_properties, option::with_edge_properties});

inline const stream_option_manipulator enable_gsf = set_option(option::gsf);
inline const stream_option_manipulator disable_gsf = unset_option(option::gsf);

} // namespace gl::io
