// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

namespace hgl {

namespace impl {

struct edge_list_t {};

struct adjacency_list_t {};

struct incidence_matrix_t {};

} // namespace impl

namespace type_traits {

template <typename T>
concept c_hypergraph_impl_tag =
    c_one_of<T, impl::edge_list_t, impl::adjacency_list_t, impl::incidence_matrix_t>;

} // namespace type_traits

} // namespace hgl
