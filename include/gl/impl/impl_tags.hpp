// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/decl/impl_tags.hpp"
#include "gl/impl/adjacency_list.hpp"
#include "gl/impl/adjacency_matrix.hpp"

namespace gl::impl {

struct list_t {
    template <traits::c_instantiation_of<graph_traits> GraphTraits>
    requires(std::same_as<typename GraphTraits::implementation_tag, list_t>)
    using type = adjacency_list<GraphTraits>;
};

struct flat_list_t {
    template <traits::c_instantiation_of<graph_traits> GraphTraits>
    requires(std::same_as<typename GraphTraits::implementation_tag, flat_list_t>)
    using type = adjacency_list<GraphTraits>;
};

struct matrix_t {
    template <traits::c_instantiation_of<graph_traits> GraphTraits>
    requires(std::same_as<typename GraphTraits::implementation_tag, matrix_t>)
    using type = adjacency_matrix<GraphTraits>;
};

struct flat_matrix_t {
    template <traits::c_instantiation_of<graph_traits> GraphTraits>
    requires(std::same_as<typename GraphTraits::implementation_tag, flat_matrix_t>)
    using type = adjacency_matrix<GraphTraits>;
};

} // namespace gl::impl
