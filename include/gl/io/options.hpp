// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/io/options.hpp
/// @brief Pre-defined stream manipulators for configuring graph I/O formatting.

#pragma once

#include "gl/io/options_manip.hpp"

namespace gl::io {

namespace detail {

enum class option_bit : bit_position_type {
    verbose = 0u,
    spec_fmt = 1u,
    with_vertex_properties = 2u,
    with_connection_properties = 3u
};

inline constexpr iword_type layout_options_mask =
    detail::build_mask(detail::option_bit::verbose, detail::option_bit::spec_fmt);

} // namespace detail

/// @ingroup GL GL-IO
/// @brief @ref gl::io::options_manip "Stream manipulator" to enable concise graph formatting.
///
/// Clears all layout-specific flags to default back to a compact representation.
///
/// @hideinitializer
inline constexpr options_manip concise{0ul, detail::layout_options_mask};

/// @ingroup GL GL-IO
/// @brief @ref gl::io::options_manip "Stream manipulator" to enable verbose graph formatting.
///
/// Modifies the stream state to output detailed structural information.
///
/// @hideinitializer
inline constexpr options_manip verbose{
    detail::build_mask(detail::option_bit::verbose), detail::layout_options_mask
};

/// @ingroup GL GL-IO
/// @brief @ref gl::io::options_manip "Stream manipulator" to enable the Graph Specification Format (GSF).
///
/// Modifies the stream to output or expect data matching the precise internal parsing format used for serialization and deserialization.
///
/// @hideinitializer
inline constexpr options_manip spec_fmt{
    detail::build_mask(detail::option_bit::spec_fmt), detail::layout_options_mask
};

/// @ingroup GL GL-IO
/// @brief @ref gl::io::options_manip "Stream manipulator" to enable the processing of vertex properties.
/// @hideinitializer
inline constexpr options_manip with_vertex_properties =
    set_options(detail::option_bit::with_vertex_properties);

/// @ingroup GL GL-IO
/// @brief @ref gl::io::options_manip "Stream manipulator" to disable the processing of vertex properties.
/// @hideinitializer
inline constexpr options_manip without_vertex_properties =
    clear_options(detail::option_bit::with_vertex_properties);

/// @ingroup GL GL-IO
/// @brief @ref gl::io::options_manip "Stream manipulator" to enable the processing of edge properties.
/// @hideinitializer
inline constexpr options_manip with_edge_properties =
    set_options(detail::option_bit::with_connection_properties);

/// @ingroup GL GL-IO
/// @brief @ref gl::io::options_manip "Stream manipulator" to disable the processing of edge properties.
/// @hideinitializer
inline constexpr options_manip without_edge_properties =
    clear_options(detail::option_bit::with_connection_properties);

/// @ingroup GL GL-IO
/// @brief @ref gl::io::options_manip "Stream manipulator" to enable the processing of both vertex and edge properties simultaneously.
/// @hideinitializer
inline constexpr options_manip with_properties = set_options(
    detail::option_bit::with_vertex_properties, detail::option_bit::with_connection_properties
);

/// @ingroup GL GL-IO
/// @brief @ref gl::io::options_manip "Stream manipulator" to disable the processing of both vertex and edge properties simultaneously.
/// @hideinitializer
inline constexpr options_manip without_properties = clear_options(
    detail::option_bit::with_vertex_properties, detail::option_bit::with_connection_properties
);

/// @ingroup GL GL-IO
/// @brief @ref gl::io::options_manip "Stream manipulator" to reset all custom graph formatting flags back to their default states.
/// @hideinitializer
inline constexpr options_manip default_options{0ul, ~static_cast<iword_type>(0)};

} // namespace gl::io
