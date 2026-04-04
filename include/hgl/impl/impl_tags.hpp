// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/decl/impl_tags.hpp"
#include "hgl/impl/flat_incidence_list.hpp"
#include "hgl/impl/flat_incidence_matrix.hpp"
#include "hgl/impl/incidence_list.hpp"
#include "hgl/impl/incidence_matrix.hpp"
#include "hgl/impl/layout_tags.hpp"

namespace hgl::impl {

template <traits::c_hypergraph_layout_tag LayoutTag, traits::c_id_type IdType>
struct list_t {
    using type = list_t<LayoutTag, IdType>;

    using layout_tag = LayoutTag;
    using id_type = IdType;

    template <traits::c_hypergraph_directional_tag DirectionalTag>
    using implementation_type = incidence_list<DirectionalTag, type>;
};

template <traits::c_hypergraph_layout_tag LayoutTag, traits::c_id_type IdType>
struct flat_list_t {
    using type = flat_list_t<LayoutTag, IdType>;

    using layout_tag = LayoutTag;
    using id_type = IdType;

    template <traits::c_hypergraph_directional_tag DirectionalTag>
    using implementation_type = flat_incidence_list<DirectionalTag, type>;
};

template <traits::c_hypergraph_asymmetric_layout_tag LayoutTag, traits::c_id_type IdType>
struct matrix_t {
    using type = matrix_t<LayoutTag, IdType>;

    using layout_tag = LayoutTag;
    using id_type = IdType;

    template <traits::c_hypergraph_directional_tag DirectionalTag>
    using implementation_type = incidence_matrix<DirectionalTag, type>;
};

template <traits::c_hypergraph_asymmetric_layout_tag LayoutTag, traits::c_id_type IdType>
struct flat_matrix_t {
    using type = flat_matrix_t<LayoutTag, IdType>;

    using layout_tag = LayoutTag;
    using id_type = IdType;

    template <traits::c_hypergraph_directional_tag DirectionalTag>
    using implementation_type = flat_incidence_matrix<DirectionalTag, type>;
};

} // namespace hgl::impl
