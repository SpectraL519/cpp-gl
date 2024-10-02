// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "io/stream_options_manipulator.hpp"

namespace gl {
namespace io {

enum class graph_option : bit_position_type {
    verbose = 0ul,
    with_vertex_properties = 1ul,
    with_edge_properties = 2ul,
    gsf = 3ul // graph specification format
};

inline const stream_options_manipulator verbose = set_option(graph_option::verbose);
inline const stream_options_manipulator concise = unset_option(graph_option::verbose);

inline const stream_options_manipulator with_vertex_properties =
    set_option(graph_option::with_vertex_properties);
inline const stream_options_manipulator without_vertex_properties =
    unset_option(graph_option::with_vertex_properties);

inline const stream_options_manipulator with_edge_properties =
    set_option(graph_option::with_edge_properties);
inline const stream_options_manipulator without_edge_properties =
    unset_option(graph_option::with_edge_properties);

inline const stream_options_manipulator with_properties =
    set_options({graph_option::with_vertex_properties, graph_option::with_edge_properties});
inline const stream_options_manipulator without_properties =
    unset_options({graph_option::with_vertex_properties, graph_option::with_edge_properties});

inline const stream_options_manipulator enable_gsf = set_option(graph_option::gsf);
inline const stream_options_manipulator disable_gsf = unset_option(graph_option::gsf);

} // namespace io

} // namespace gl
