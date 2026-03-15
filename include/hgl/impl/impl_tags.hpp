// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/directional_tags.hpp"
#include "hgl/impl/incidence_list.hpp"
#include "hgl/impl/incidence_matrix.hpp"
#include "hgl/impl/layout_tags.hpp"
#include "hgl/types/type_traits.hpp"
#include "hgl/types/types.hpp"

namespace hgl {

namespace impl {

template <type_traits::c_hypergraph_layout_tag LayoutTag>
struct list_t {
    using layout_tag = LayoutTag;

    template <type_traits::c_hypergraph_directional_tag DirectionalTag>
    using implementation_type = incidence_list<DirectionalTag, LayoutTag>;
};

template <type_traits::c_hypergraph_asymmetric_layout_tag LayoutTag>
struct matrix_t {
    using layout_tag = LayoutTag;

    template <type_traits::c_hypergraph_directional_tag DirectionalTag>
    using implementation_type = incidence_matrix<DirectionalTag, LayoutTag>;
};

} // namespace impl

namespace type_traits {

template <typename T>
concept c_hypergraph_list_impl = c_instantiation_of<T, impl::list_t>;

template <typename T>
concept c_hypergraph_matrix_impl = c_instantiation_of<T, impl::matrix_t>;

template <typename T>
concept c_hypergraph_impl_tag = c_hypergraph_list_impl<T> or c_hypergraph_matrix_impl<T>;

} // namespace type_traits

} // namespace hgl
