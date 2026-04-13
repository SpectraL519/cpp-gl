// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/io/stream_options_manipulator.hpp"

namespace gl::io {

namespace detail {

enum class option_bit : bit_position_type {
    verbose = 0u,
    with_vertex_properties = 1u,
    with_connection_properties = 2u,
    specification_fmt = 3u
};

} // namespace detail

inline const stream_options_manipulator verbose = set_option(detail::option_bit::verbose);
inline const stream_options_manipulator concise = unset_option(detail::option_bit::verbose);

inline const stream_options_manipulator with_vertex_properties =
    set_option(detail::option_bit::with_vertex_properties);
inline const stream_options_manipulator without_vertex_properties =
    unset_option(detail::option_bit::with_vertex_properties);

inline const stream_options_manipulator with_edge_properties =
    set_option(detail::option_bit::with_connection_properties);
inline const stream_options_manipulator without_edge_properties =
    unset_option(detail::option_bit::with_connection_properties);

inline const stream_options_manipulator with_properties = set_options(
    {detail::option_bit::with_vertex_properties, detail::option_bit::with_connection_properties}
);
inline const stream_options_manipulator without_properties = unset_options(
    {detail::option_bit::with_vertex_properties, detail::option_bit::with_connection_properties}
);

// TODO: rename to _spec_fmt
inline const stream_options_manipulator enable_gsf =
    set_option(detail::option_bit::specification_fmt);
inline const stream_options_manipulator disable_gsf =
    unset_option(detail::option_bit::specification_fmt);

inline const stream_options_manipulator default_options =
    unset_options(~static_cast<iword_type>(0));

} // namespace gl::io
