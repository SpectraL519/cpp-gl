// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/impl/layout_tags.hpp"
#include "hgl/types.hpp"

namespace hgl {

namespace impl {

// TODO: add default LayoutTag values

template <traits::c_hypergraph_layout_tag LayoutTag, traits::c_id_type IdType = default_id_type>
struct list_t;

template <traits::c_hypergraph_layout_tag LayoutTag, traits::c_id_type IdType = default_id_type>
struct flat_list_t;

template <
    traits::c_hypergraph_asymmetric_layout_tag LayoutTag,
    traits::c_id_type IdType = default_id_type>
struct matrix_t;

} // namespace impl

namespace traits {

template <typename T>
concept c_hypergraph_list_impl = c_instantiation_of<T, impl::list_t>;

template <typename T>
concept c_hypergraph_flat_list_impl = c_instantiation_of<T, impl::flat_list_t>;

template <typename T>
concept c_hypergraph_incidence_list_impl =
    c_hypergraph_list_impl<T> or c_hypergraph_flat_list_impl<T>;

template <typename T>
concept c_hypergraph_matrix_impl = c_instantiation_of<T, impl::matrix_t>;

template <typename T>
concept c_hypergraph_incidence_matrix_impl = c_hypergraph_matrix_impl<T>;

template <typename T>
concept c_hypergraph_impl_tag =
    c_hypergraph_incidence_list_impl<T> or c_hypergraph_incidence_matrix_impl<T>;

} // namespace traits

} // namespace hgl
