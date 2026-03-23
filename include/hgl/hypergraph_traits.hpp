// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/directional_tags.hpp"
#include "hgl/hypergraph_elements.hpp"
#include "hgl/impl/impl_tags.hpp"
#include "hgl/impl/layout_tags.hpp"

namespace hgl {

template <
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = types::empty_properties,
    traits::c_properties HyperedgeProperties = types::empty_properties,
    traits::c_hypergraph_impl_tag ImplTag = impl::list_t<impl::hyperedge_major_t>>
struct hypergraph_traits {
    using directional_tag = DirectionalTag;
    using implementation_tag = ImplTag;

    using vertex_type = vertex_descriptor<VertexProperties>;
    using vertex_properties_type = typename vertex_type::properties_type;

    using hyperedge_type = hyperedge_descriptor<HyperedgeProperties>;
    using hyperedge_properties_type = typename hyperedge_type::properties_type;
};

template <
    traits::c_hypergraph_layout_tag LayoutTag = impl::hyperedge_major_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = types::empty_properties,
    traits::c_properties HyperedgeProperties = types::empty_properties>
using list_hypergraph_traits =
    hypergraph_traits<DirectionalTag, VertexProperties, HyperedgeProperties, impl::list_t<LayoutTag>>;

template <
    traits::c_hypergraph_layout_tag LayoutTag = impl::hyperedge_major_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = types::empty_properties,
    traits::c_properties HyperedgeProperties = types::empty_properties>
using flat_list_hypergraph_traits = hypergraph_traits<
    DirectionalTag,
    VertexProperties,
    HyperedgeProperties,
    impl::flat_list_t<LayoutTag>>;

template <
    traits::c_hypergraph_asymmetric_layout_tag LayoutTag = impl::hyperedge_major_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = types::empty_properties,
    traits::c_properties HyperedgeProperties = types::empty_properties>
using matrix_hypergraph_traits = hypergraph_traits<
    DirectionalTag,
    VertexProperties,
    HyperedgeProperties,
    impl::matrix_t<LayoutTag>>;

template <
    traits::c_properties VertexProperties = types::empty_properties,
    traits::c_properties HyperedgeProperties = types::empty_properties,
    traits::c_hypergraph_impl_tag ImplTag = impl::list_t<impl::hyperedge_major_t>>
using undirected_hypergraph_traits =
    hypergraph_traits<undirected_t, VertexProperties, HyperedgeProperties, ImplTag>;

template <
    traits::c_properties VertexProperties = types::empty_properties,
    traits::c_properties HyperedgeProperties = types::empty_properties,
    traits::c_hypergraph_impl_tag ImplTag = impl::list_t<impl::hyperedge_major_t>>
using bf_directed_hypergraph_traits =
    hypergraph_traits<bf_directed_t, VertexProperties, HyperedgeProperties, ImplTag>;

namespace traits {

template <typename TraitsType>
concept c_list_hypergraph_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and c_hypergraph_list_impl<typename TraitsType::implementation_tag>;

template <typename TraitsType>
concept c_flat_list_hypergraph_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and c_hypergraph_flat_list_impl<typename TraitsType::implementation_tag>;

template <typename TraitsType>
concept c_incidence_list_hypergraph_traits =
    c_list_hypergraph_traits<TraitsType> or c_flat_list_hypergraph_traits<TraitsType>;

template <typename TraitsType>
concept c_matrix_hypergraph_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and c_hypergraph_matrix_impl<typename TraitsType::implementation_tag>;

template <typename TypeTraits>
concept c_incidence_matrix_hypergraph_traits = c_matrix_hypergraph_traits<TypeTraits>;

template <typename TraitsType>
concept c_undirected_hypergraph_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and std::same_as<typename TraitsType::directional_tag, undirected_t>;

template <typename TraitsType>
concept c_bf_directed_hypergraph_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and std::same_as<typename TraitsType::directional_tag, bf_directed_t>;

} // namespace traits

} // namespace hgl
