// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/io/core.hpp
/// @brief Core I/O utilities, stream manipulators, and range formatters for hypergraphs.

#pragma once

#include "gl/io/options.hpp"
#include "gl/io/options_manip.hpp"
#include "gl/io/ranges.hpp"
#include "hgl/directional_tags.hpp"

#include <string_view>

namespace hgl::io {

/// @ingroup HGL-IO
/// @copybrief gl::io::range_formatter
/// @see gl::io::range_formatter
template <std::ranges::range R>
using range_formatter = gl::io::range_formatter<R>;

/// @ingroup HGL-IO
/// @copybrief gl::io::implicit_range
/// @see gl::io::implicit_range
using gl::io::implicit_range;

/// @ingroup HGL-IO
/// @copybrief gl::io::implicit_range_formatter
/// @see gl::io::implicit_range_formatter
template <std::integral T>
using implicit_range_formatter = gl::io::implicit_range_formatter<T>;

/// @ingroup HGL-IO
/// @copybrief gl::io::set_formatter
/// @see gl::io::set_formatter
using gl::io::set_formatter;

/// @ingroup HGL-IO
/// @copybrief gl::io::multiline_set_formatter
/// @see gl::io::multiline_set_formatter
using gl::io::multiline_set_formatter;

/// @ingroup HGL-IO
/// @copybrief gl::io::are_options_set
/// @see gl::io::are_options_set
using gl::io::are_options_set;

/// @ingroup HGL-IO
/// @copybrief gl::io::clear_options
/// @see gl::io::clear_options
using gl::io::clear_options;

/// @ingroup HGL-IO
/// @copybrief gl::io::is_option_set
/// @see gl::io::is_option_set
using gl::io::is_option_set;

/// @ingroup HGL-IO
/// @copybrief gl::io::options_manip
/// @see gl::io::options_manip
using options_manip = gl::io::options_manip;

/// @ingroup HGL-IO
/// @copybrief gl::io::set_options
/// @see gl::io::set_options
using gl::io::set_options;

namespace detail {

using gl::io::detail::build_mask;
using gl::io::detail::option_bit;

} // namespace detail

/// @ingroup HGL-IO
/// @brief @ref hgl::io::options_manip "Stream manipulator" to enable concise hypergraph formatting.
///
/// Clears all layout-specific flags to default back to a compact representation.
///
/// @hideinitializer
inline constexpr options_manip concise = gl::io::concise;

/// @ingroup HGL-IO
/// @brief @ref hgl::io::options_manip "Stream manipulator" to enable verbose hypergraph formatting.
///
/// Modifies the stream state to output detailed structural information.
///
/// @hideinitializer
inline constexpr options_manip spec_fmt = gl::io::spec_fmt;

/// @ingroup HGL-IO
/// @brief @ref hgl::io::options_manip "Stream manipulator" to enable the Hypergraph Specification Format (HGSF).
///
/// Modifies the stream to output or expect data matching the precise internal parsing format used for serialization and deserialization.
///
/// @hideinitializer
inline constexpr options_manip verbose = gl::io::verbose;

/// @ingroup HGL-IO
/// @brief @ref hgl::io::options_manip "Stream manipulator" to enable the processing of vertex properties.
/// @hideinitializer
inline constexpr options_manip with_vertex_properties = gl::io::with_vertex_properties;

/// @ingroup HGL-IO
/// @brief @ref hgl::io::options_manip "Stream manipulator" to disable the processing of vertex properties.
/// @hideinitializer
inline constexpr options_manip without_vertex_properties = gl::io::without_vertex_properties;

/// @ingroup HGL-IO
/// @brief @ref hgl::io::options_manip "Stream manipulator" to enable the processing of hyperedge properties.
/// @hideinitializer
inline constexpr options_manip with_hyperedge_properties =
    set_options(detail::option_bit::with_connection_properties);

/// @ingroup HGL-IO
/// @brief @ref hgl::io::options_manip "Stream manipulator" to disable the processing of hyperedge properties.
/// @hideinitializer
inline constexpr options_manip without_hyperedge_properties =
    clear_options(detail::option_bit::with_connection_properties);

/// @ingroup HGL-IO
/// @brief @ref hgl::io::options_manip "Stream manipulator" to enable the processing of both vertex and hyperedge properties simultaneously.
/// @hideinitializer
inline constexpr options_manip with_properties = gl::io::with_properties;

/// @ingroup HGL-IO
/// @brief @ref hgl::io::options_manip "Stream manipulator" to disable the processing of both vertex and hyperedge properties simultaneously.
/// @hideinitializer
inline constexpr options_manip without_properties = gl::io::without_properties;

/// @ingroup HGL-IO
/// @brief @ref hgl::io::options_manip "Stream manipulator" to reset all custom graph formatting flags back to their default states.
/// @hideinitializer
inline constexpr options_manip default_options = gl::io::default_options;

} // namespace hgl::io
