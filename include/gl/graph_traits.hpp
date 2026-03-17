// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/decl/graph_traits.hpp"
#include "gl/decl/impl_tags.hpp"
#include "gl/edge_descriptor.hpp"

namespace gl {

template <
    type_traits::c_edge_directional_tag EdgeDirectionalTag = directed_t,
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties,
    type_traits::c_graph_impl_tag ImplTag = impl::list_t>
struct graph_traits {
    using vertex_type = vertex_descriptor<VertexProperties>;
    using vertex_properties_type = typename vertex_type::properties_type;

    using edge_type = edge_descriptor<EdgeDirectionalTag, EdgeProperties>;
    using edge_directional_tag = typename edge_type::directional_tag;
    using edge_properties_type = typename edge_type::properties_type;

    using implementation_tag = ImplTag;
};

template <
    type_traits::c_edge_directional_tag EdgeDirectionalTag = directed_t,
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties>
using list_graph_traits =
    graph_traits<EdgeDirectionalTag, VertexProperties, EdgeProperties, impl::list_t>;

template <
    type_traits::c_edge_directional_tag EdgeDirectionalTag = directed_t,
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties>
using flat_list_graph_traits =
    graph_traits<EdgeDirectionalTag, VertexProperties, EdgeProperties, impl::flat_list_t>;

template <
    type_traits::c_edge_directional_tag EdgeDirectionalTag = directed_t,
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties>
using matrix_graph_traits =
    graph_traits<EdgeDirectionalTag, VertexProperties, EdgeProperties, impl::matrix_t>;

template <
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties,
    type_traits::c_graph_impl_tag ImplTag = impl::list_t>
using directed_graph_traits = graph_traits<directed_t, VertexProperties, EdgeProperties, ImplTag>;

template <
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties,
    type_traits::c_graph_impl_tag ImplTag = impl::list_t>
using undirected_graph_traits =
    graph_traits<undirected_t, VertexProperties, EdgeProperties, ImplTag>;

namespace type_traits {

template <typename TraitsType>
concept c_list_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and c_one_of<typename TraitsType::implementation_tag, impl::list_t, impl::flat_list_t>;

template <typename TraitsType>
concept c_matrix_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::implementation_tag, impl::matrix_t>;

template <typename TraitsType>
concept c_directed_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::edge_directional_tag, directed_t>;

template <typename TraitsType>
concept c_undirected_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::edge_directional_tag, undirected_t>;

} // namespace type_traits

} // namespace gl
