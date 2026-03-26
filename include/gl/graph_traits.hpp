// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/decl/graph_traits.hpp"
#include "gl/decl/impl_tags.hpp"
#include "gl/edge_descriptor.hpp"
#include "gl/types/core.hpp"

namespace gl {

template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_impl_tag ImplTag = impl::list_t,
    traits::c_id_type IdType = default_id_type>
struct graph_traits {
    using directional_tag = DirectionalTag;
    using implementation_tag = ImplTag;
    using id_type = IdType;

    using vertex_type = vertex_descriptor<VertexProperties, id_type>;
    using vertex_properties_type = typename vertex_type::properties_type;

    using edge_type = edge_descriptor<DirectionalTag, EdgeProperties, id_type>;
    using edge_properties_type = typename edge_type::properties_type;
};

template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using list_graph_traits =
    graph_traits<DirectionalTag, VertexProperties, EdgeProperties, impl::list_t, IdType>;

template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using flat_list_graph_traits =
    graph_traits<DirectionalTag, VertexProperties, EdgeProperties, impl::flat_list_t, IdType>;

template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using matrix_graph_traits =
    graph_traits<DirectionalTag, VertexProperties, EdgeProperties, impl::matrix_t, IdType>;

template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_impl_tag ImplTag = impl::list_t,
    traits::c_id_type IdType = default_id_type>
using directed_graph_traits =
    graph_traits<directed_t, VertexProperties, EdgeProperties, ImplTag, IdType>;

template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_impl_tag ImplTag = impl::list_t,
    traits::c_id_type IdType = default_id_type>
using undirected_graph_traits =
    graph_traits<undirected_t, VertexProperties, EdgeProperties, ImplTag, IdType>;

namespace traits {

template <typename TraitsType>
concept c_list_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::implementation_tag, impl::list_t>;

template <typename TraitsType>
concept c_flat_list_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::implementation_tag, impl::flat_list_t>;

template <typename TraitsType>
concept c_adjacency_list_graph_traits =
    c_list_graph_traits<TraitsType> or c_flat_list_graph_traits<TraitsType>;

template <typename TraitsType>
concept c_matrix_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::implementation_tag, impl::matrix_t>;

template <typename TraitsType>
concept c_adjacency_matrix_graph_traits = c_matrix_graph_traits<TraitsType>;

template <typename TraitsType>
concept c_directed_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::directional_tag, directed_t>;

template <typename TraitsType>
concept c_undirected_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::directional_tag, undirected_t>;

} // namespace traits

} // namespace gl
