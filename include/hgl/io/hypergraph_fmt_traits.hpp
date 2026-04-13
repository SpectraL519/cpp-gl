// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/directional_tags.hpp"

#include <string_view>

namespace hgl::io::detail {

template <typename DirectionalTag>
struct hypergraph_fmt_traits;

template <>
struct hypergraph_fmt_traits<undirected_t> {
    static constexpr int discriminator = 0;
    static constexpr std::string_view type = "undirected";
};

template <>
struct hypergraph_fmt_traits<bf_directed_t> {
    static constexpr int discriminator = 1;
    static constexpr std::string_view type = "BF-directed";
};

} // namespace hgl::io::detail
