// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/io/format.hpp"
#include "gl/io/options.hpp"
#include "gl/io/stream_options_manipulator.hpp"
#include "hgl/directional_tags.hpp"

#include <string_view>

namespace hgl::io {

using gl::io::range_formatter;
using gl::io::set_formatter;

using gl::io::are_options_set;
using gl::io::is_option_set;
using gl::io::set_option;
using gl::io::set_options;
using gl::io::stream_options_manipulator;
using gl::io::unset_option;
using gl::io::unset_options;

namespace detail {

using gl::io::detail::get_options_bitmask;
using gl::io::detail::option_bit;

} // namespace detail

using gl::io::concise;
using gl::io::verbose;

using gl::io::with_vertex_properties;
using gl::io::without_vertex_properties;

inline const stream_options_manipulator with_hyperedge_properties =
    set_option(detail::option_bit::with_connection_properties);
inline const stream_options_manipulator without_hyperedge_properties =
    unset_option(detail::option_bit::with_connection_properties);

using gl::io::with_properties;
using gl::io::without_properties;

using gl::io::default_options;

namespace detail {

template <typename DirectionalTag>
struct hypergraph_fmt_traits;

template <>
struct hypergraph_fmt_traits<bf_directed_t> {
    static constexpr std::string_view type = "BF-directed";
};

template <>
struct hypergraph_fmt_traits<undirected_t> {
    static constexpr std::string_view type = "undirected";
};

} // namespace detail

} // namespace hgl::io
