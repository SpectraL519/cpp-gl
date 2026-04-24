// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/graph_traits.hpp
/// @brief Defines the primary graph traits structure and related type aliases for different graph configurations.

#pragma once

#include "gl/decl/graph_traits.hpp"
#include "gl/decl/impl_tags.hpp"
#include "gl/directional_tags.hpp"
#include "gl/edge_descriptor.hpp"
#include "gl/types/core.hpp"

namespace gl {

/// @ingroup GL GL-Core
/// @brief Primary graph traits structure that encapsulates all necessary type information for graph implementations.
///
/// This structure serves as the central point for defining the properties and types associated with a graph,
/// including directionality, vertex and edge properties, as well as implementation and identifier types. It is designed
/// to be flexible and extensible, allowing users to customize their graph types by specifying different traits.
///
/// ### Template Parameters
/// | Parameter        | Description | Default value | Constraints |
/// | :--------------- | :---------- | :------------ | :---------- |
/// | DirectionalTag   | Specifies whether the graph is directed or undirected. | @ref gl::directed_t "directed_t" | [**c_graph_directional_tag**](gl_traits.md#gl-traits-c-graph-directional-tag) |
/// | VertexProperties | A type representing the properties associated with vertices in the graph. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_traits.md#gl-traits-c-properties) |
/// | EdgeProperties   | A type representing the properties associated with edges in the graph. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_traits.md#gl-traits-c-properties) |
/// | ImplTag          | Specifies the graph implementation type (e.g., adjacency list, adjacency matrix). | @ref gl::impl::list_t "impl::list_t" | [**c_graph_impl_tag**](gl_traits.md#gl-traits-c-graph-impl-tag) |
/// | IdType           | The type used for vertex and edge identifiers. | @ref gl::default_id_type "default_id_type" | [**c_id_type**](gl_traits.md#gl-traits-c-id-type) |
///
/// ### See Also
/// - Available implementation tags:
///   - @ref gl::impl::list_t "impl::list_t" : A standard adjacency list implementation.
///   - @ref gl::impl::flat_list_t "impl::flat_list_t" : A flattened adjacency list implementation.
///   - @ref gl::impl::matrix_t "impl::matrix_t" : A standard adjacency matrix implementation.
///   - @ref gl::impl::flat_matrix_t "impl::flat_matrix_t" : A flattened adjacency matrix implementation.
/// - @ref gl::directed_t "directed_t", @ref gl::undirected_t "undirected_t" : For the directional tags used to specify graph directionality.
/// - @ref gl::vertex_descriptor "vertex_descriptor" : For the vertex descriptor type defined based on the graph traits.
/// - @ref gl::edge_descriptor "edge_descriptor" : For the edge descriptor type defined based on the graph traits.
/// - @ref gl::empty_properties "empty_properties" : For the default empty properties type used when no custom properties are needed.
/// - Convenience type aliases for the `graph_traits` type:
///   - @ref gl::list_graph_traits "list_graph_traits",
///   - @ref gl::flat_list_graph_traits "flat_list_graph_traits",
///   - @ref gl::matrix_graph_traits "matrix_graph_traits",
///   - @ref gl::flat_matrix_graph_traits "flat_matrix_graph_traits",
///   - @ref gl::directed_graph_traits "directed_graph_traits",
///   - @ref gl::undirected_graph_traits "undirected_graph_traits"
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_impl_tag ImplTag = impl::list_t,
    traits::c_id_type IdType = default_id_type>
struct graph_traits {
    /// @brief The tag indicating the graph's directionality (directed or undirected).
    using directional_tag = DirectionalTag;
    /// @brief The tag indicating the graph's implementation type (e.g., adjacency list, adjacency matrix).
    using implementation_tag = ImplTag;
    /// @brief The type of graph element indentifiers (i.e. vertex and edge IDs).
    using id_type = IdType;

    /// @brief The vertex descriptor type associated with this graph, defined based on the specified vertex properties and identifier type.
    using vertex_type = vertex_descriptor<VertexProperties, id_type>;
    /// @brief The type of properties associated with the vertex descriptor.
    using vertex_properties_type = typename vertex_type::properties_type;

    /// @brief The edge descriptor type associated with this graph, defined based on the specified edge properties and identifier type.
    using edge_type = edge_descriptor<DirectionalTag, EdgeProperties, id_type>;
    /// @brief The type of properties associated with the edge descriptor.
    using edge_properties_type = typename edge_type::properties_type;
};

/// @ingroup GL GL-Core
/// @brief Type alias for graph traits with an adjacency list implementation.
///
/// This alias simplifies the specification of graph traits for graphs that use an adjacency list representation,
/// allowing users to easily define their graph types with the desired properties and directionality while defaulting
/// to the adjacency list implementation.
///
/// > [!NOTE] Template parameters
/// >
/// > The template parameters for this alias are the same as those for @ref gl::graph_traits "graph_traits", with the
/// > `ImplTag` parameter fixed to @ref gl::impl::list_t "impl::list_t". This means that when using `list_graph_traits`,
/// > only the `DirectionalTag`, `VertexProperties`, `EdgeProperties`, and `IdType` parameters need to be specified.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using list_graph_traits =
    graph_traits<DirectionalTag, VertexProperties, EdgeProperties, impl::list_t, IdType>;

/// @ingroup GL GL-Core
/// @brief Type alias for graph traits with a flattened adjacency list implementation.
///
/// This alias simplifies the specification of graph traits for graphs that use a flattened adjacency list representation,
/// allowing users to easily define their graph types with the desired properties and directionality while defaulting
/// to the flattened adjacency list implementation.
///
/// > [!NOTE] Template parameters
/// >
/// > The template parameters for this alias are the same as those for @ref gl::graph_traits "graph_traits", with the
/// > `ImplTag` parameter fixed to @ref gl::impl::flat_list_t "impl::flat_list_t". This means that when using `flat_list_graph_traits`,
/// > only the `DirectionalTag`, `VertexProperties`, `EdgeProperties`, and `IdType` parameters need to be specified.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using flat_list_graph_traits =
    graph_traits<DirectionalTag, VertexProperties, EdgeProperties, impl::flat_list_t, IdType>;

/// @ingroup GL GL-Core
/// @brief Type alias for graph traits with an adjacency matrix implementation.
///
/// This alias simplifies the specification of graph traits for graphs that use an adjacency matrix representation,
/// allowing users to easily define their graph types with the desired properties and directionality while defaulting
/// to the adjacency matrix implementation.
///
/// > [!NOTE] Template parameters
/// >
/// > The template parameters for this alias are the same as those for @ref gl::graph_traits "graph_traits", with the
/// > `ImplTag` parameter fixed to @ref gl::impl::matrix_t "impl::matrix_t". This means that when using `matrix_graph_traits`,
/// > only the `DirectionalTag`, `VertexProperties`, `EdgeProperties`, and `IdType` parameters need to be specified.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using matrix_graph_traits =
    graph_traits<DirectionalTag, VertexProperties, EdgeProperties, impl::matrix_t, IdType>;

/// @ingroup GL GL-Core
/// @brief Type alias for graph traits with a flattened adjacency matrix implementation.
///
/// This alias simplifies the specification of graph traits for graphs that use a flattened adjacency matrix representation,
/// allowing users to easily define their graph types with the desired properties and directionality while defaulting
/// to the flattened adjacency matrix implementation.
///
/// > [!NOTE] Template parameters
/// >
/// > The template parameters for this alias are the same as those for @ref gl::graph_traits "graph_traits", with the
/// > `ImplTag` parameter fixed to @ref gl::impl::flat_matrix_t "impl::flat_matrix_t". This means that when using `flat_matrix_graph_traits`,
/// > only the `DirectionalTag`, `VertexProperties`, `EdgeProperties`, and `IdType` parameters need to be specified.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using flat_matrix_graph_traits =
    graph_traits<DirectionalTag, VertexProperties, EdgeProperties, impl::flat_matrix_t, IdType>;

/// @ingroup GL GL-Core
/// @brief Type alias for graph traits with an directed graph configuration.
///
/// This alias simplifies the specification of graph traits for directed graphs, allowing users to easily define their
/// graph types with the desired properties and implementation while defaulting to a directed configuration.
///
/// > [!NOTE] Template parameters
/// > The template parameters for this alias are the same as those for @ref gl::graph_traits "graph_traits", with the
/// > `DirectionalTag` parameter fixed to @ref gl::directed_t "directed_t". This means that when using `directed_graph_traits`,
/// > only the `VertexProperties`, `EdgeProperties`, 'ImplTag', and `IdType` parameters need to be specified.
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_impl_tag ImplTag = impl::list_t,
    traits::c_id_type IdType = default_id_type>
using directed_graph_traits =
    graph_traits<directed_t, VertexProperties, EdgeProperties, ImplTag, IdType>;

/// @ingroup GL GL-Core
/// @brief Type alias for graph traits with an undirected graph configuration.
///
/// This alias simplifies the specification of graph traits for undirected graphs, allowing users to easily define their
/// graph types with the desired properties and implementation while defaulting to an undirected configuration.
///
/// > [!NOTE] Template parameters
/// > The template parameters for this alias are the same as those for @ref gl::graph_traits "graph_traits", with the
/// > `DirectionalTag` parameter fixed to @ref gl::undirected_t "undirected_t". This means that when using `undirected_graph_traits`,
/// > only the `VertexProperties`, `EdgeProperties`, 'ImplTag', and `IdType` parameters need to be specified.
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_impl_tag ImplTag = impl::list_t,
    traits::c_id_type IdType = default_id_type>
using undirected_graph_traits =
    graph_traits<undirected_t, VertexProperties, EdgeProperties, ImplTag, IdType>;

namespace traits {

/// @ingroup GL GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with a list implementation.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_list_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::implementation_tag, impl::list_t>;

/// @ingroup GL GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with a flattened adjacency list implementation.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_flat_list_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::implementation_tag, impl::flat_list_t>;

/// @ingroup GL GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with an adjacency list implementation (either standard or flattened).
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_adjacency_list_graph_traits =
    c_list_graph_traits<TraitsType> or c_flat_list_graph_traits<TraitsType>;

/// @ingroup GL GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with a matrix implementation.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_matrix_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::implementation_tag, impl::matrix_t>;

/// @ingroup GL GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with a flattened adjacency matrix implementation.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_flat_matrix_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::implementation_tag, impl::flat_matrix_t>;

/// @ingroup GL GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with a matrix implementation (either standard or flattened).
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_adjacency_matrix_graph_traits =
    c_matrix_graph_traits<TraitsType> or c_flat_matrix_graph_traits<TraitsType>;

/// @ingroup GL GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with a directed graph configuration.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_directed_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::directional_tag, directed_t>;

/// @ingroup GL GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with an undirected graph configuration.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_undirected_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::directional_tag, undirected_t>;

} // namespace traits

} // namespace gl
