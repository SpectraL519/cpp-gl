// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/directional_tags.hpp"

#include <string_view>

namespace gl::io::detail {

template <typename DirectionalTag>
struct graph_fmt_traits;

template <>
struct graph_fmt_traits<undirected_t> {
    static constexpr int discriminator = 0;
    static constexpr std::string_view type = "undirected";
    static constexpr std::string_view out_edges = "incident edges";
};

template <>
struct graph_fmt_traits<directed_t> {
    static constexpr int discriminator = 1;
    static constexpr std::string_view type = "directed";
    static constexpr std::string_view out_edges = "outgoing edges";
};

} // namespace gl::io::detail
