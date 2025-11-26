// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hypergraph_elements.hpp"
#include "impl/impl_tags.hpp"

namespace hgl {

template <
    type_traits::c_hyperedge_directional_tag HyperedgeDirectionalTag = undirected_t,
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties HyperedgeProperties = types::empty_properties,
    type_traits::c_hypergraph_impl_tag ImplTag = impl::edge_list_t>
struct hypergraph_traits {
    using vertex_type = vertex_descriptor<VertexProperties>;
    using vertex_properties_type = typename vertex_type::properties_type;

    using hyperedge_type = hyperedge_descriptor<HyperedgeDirectionalTag, HyperedgeProperties>;
    using hyperedge_directional_tag = typename hyperedge_type::directional_tag;
    using hyperedge_properties_type = typename hyperedge_type::properties_type;

    using implementation_tag = ImplTag;
};

template <
    type_traits::c_hyperedge_directional_tag HyperedgeDirectionalTag = undirected_t,
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties>
using edge_list_hg_traits =
    hypergraph_traits<HyperedgeDirectionalTag, VertexProperties, EdgeProperties, impl::edge_list_t>;

template <
    type_traits::c_hyperedge_directional_tag HyperedgeDirectionalTag = undirected_t,
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties>
using adjacency_list_hg_traits = hypergraph_traits<
    HyperedgeDirectionalTag,
    VertexProperties,
    EdgeProperties,
    impl::adjacency_list_t>;

template <
    type_traits::c_hyperedge_directional_tag HyperedgeDirectionalTag = undirected_t,
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties>
using incidence_matrix_hg_traits = hypergraph_traits<
    HyperedgeDirectionalTag,
    VertexProperties,
    EdgeProperties,
    impl::incidence_matrix_t>;

template <
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties,
    type_traits::c_hypergraph_impl_tag ImplTag = impl::edge_list_t>
using undirected_hg_traits =
    hypergraph_traits<undirected_t, VertexProperties, EdgeProperties, ImplTag>;

template <
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties,
    type_traits::c_hypergraph_impl_tag ImplTag = impl::edge_list_t>
using bf_directed_hg_traits =
    hypergraph_traits<bf_directed_t, VertexProperties, EdgeProperties, ImplTag>;

namespace type_traits {

template <typename TraitsType>
concept c_edge_list_hg_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and std::same_as<typename TraitsType::implementation_tag, impl::edge_list_t>;

template <typename TraitsType>
concept c_adjacency_list_hg_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and std::same_as<typename TraitsType::implementation_tag, impl::adjacency_list_t>;

template <typename TraitsType>
concept c_incidence_matrix_hg_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and std::same_as<typename TraitsType::implementation_tag, impl::incidence_matrix_t>;

template <typename TraitsType>
concept c_undirected_hg_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and std::same_as<typename TraitsType::hyperedge_directional_tag, undirected_t>;

template <typename TraitsType>
concept c_bf_directed_hg_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and std::same_as<typename TraitsType::hyperedge_directional_tag, bf_directed_t>;

} // namespace type_traits

} // namespace hgl
