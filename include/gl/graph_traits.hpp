// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/graph_traits.hpp
/// @brief Defines the primary graph traits structure and related type aliases for different graph configurations.

#pragma once

#include "gl/api.hpp"
#include "gl/decl/graph_traits.hpp"
#include "gl/decl/repr_tags.hpp"
#include "gl/directional_tags.hpp"
#include "gl/edge_descriptor.hpp"
#include "gl/types/core.hpp"

#include <type_traits>

namespace gl {

/// @ingroup GL-Core
/// @brief Primary graph traits structure that encapsulates all necessary type information for graph representations.
///
/// This structure serves as the central point for defining the properties and types associated with a graph,
/// including directionality, vertex and edge properties, API validation policy, as well as representation and identifier types.
/// It is designed to be flexible and extensible, allowing users to customize their graph types by specifying different traits.
///
/// ### Template Parameters
/// | Parameter        | Description | Default value | Constraints |
/// | :--------------- | :---------- | :------------ | :---------- |
/// | DirectionalTag   | Specifies whether the graph is directed or undirected. | @ref gl::directed_t "directed_t" | [**c_graph_directional_tag**](gl_concepts.md#gl-traits-c-graph-directional-tag) |
/// | VertexProperties | A type representing the properties associated with vertices in the graph. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | EdgeProperties   | A type representing the properties associated with edges in the graph. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | ReprTag          | Specifies the graph representation type (e.g., adjacency list, adjacency matrix). | @ref gl::repr::list_t "repr::list_t" | [**c_graph_repr_tag**](gl_concepts.md#gl-traits-c-graph-repr-tag) |
/// | ApiPolicyTag     | Specifies the API safety and validation policy. | @ref gl::api::strict_t "api::strict_t" | [**c_api_policy_tag**](gl_concepts.md#gl-traits-c-api-policy-tag) |
/// | IdType           | The type used for vertex and edge identifiers. | @ref gl::default_id_type "default_id_type" | [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) |
///
/// ### See Also
/// - Available representation tags:
///   - @ref gl::repr::list_t "repr::list_t" : A standard adjacency list representation.
///   - @ref gl::repr::flat_list_t "repr::flat_list_t" : A flattened adjacency list representation.
///   - @ref gl::repr::matrix_t "repr::matrix_t" : A standard adjacency matrix representation.
///   - @ref gl::repr::flat_matrix_t "repr::flat_matrix_t" : A flattened adjacency matrix representation.
/// - @ref gl::directed_t "directed_t", @ref gl::undirected_t "undirected_t" : For the directional tags used to specify graph directionality.
/// - @ref gl::api::strict_t "api::strict_t", @ref gl::api::relaxed_t "api::relaxed_t" : For tags configuring API validation logic.
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
    traits::c_graph_repr_tag ReprTag = repr::list_t,
    traits::c_api_policy_tag ApiPolicyTag = api::strict_t,
    traits::c_id_type IdType = default_id_type>
struct graph_traits {
    /// @brief The tag indicating the graph's directionality (directed or undirected).
    using directional_tag = DirectionalTag;
    /// @brief The tag indicating the graph's representation type (e.g., adjacency list, adjacency matrix).
    using representation_tag = ReprTag;
    /// @brief The tag indicating the active API validation and safety policy.
    using api_policy_tag = ApiPolicyTag;
    /// @brief The type of graph element indentifiers (i.e. vertex and edge IDs).
    using id_type = IdType;

    /// @brief The type of properties associated with the vertex descriptor.
    using vertex_properties_type = std::remove_cvref_t<VertexProperties>;
    /// @brief The descriptor type representing a vertex of a graph.
    using vertex_type = vertex_descriptor<vertex_properties_type, id_type>;
    /// @brief The descriptor type representing an immutable vertex of a graph.
    using const_vertex_type = vertex_descriptor<const vertex_properties_type, id_type>;

    /// @brief The type of properties associated with the edge descriptor.
    using edge_properties_type = std::remove_cvref_t<EdgeProperties>;
    /// @brief The descriptor type representing an edge of a graph.
    using edge_type = edge_descriptor<directional_tag, edge_properties_type, id_type>;
    /// @brief The descriptor type representing an immutable edge of a graph.
    using const_edge_type = edge_descriptor<directional_tag, const edge_properties_type, id_type>;
};

/// @ingroup GL-Core
/// @brief Type alias for graph traits with an adjacency list representation.
///
/// This alias simplifies the specification of graph traits for graphs that use an adjacency list representation,
/// allowing users to easily define their graph types with the desired properties and directionality while defaulting
/// to the adjacency list representation.
///
/// > [!NOTE] Template parameters
/// >
/// > The template parameters for this alias are the same as those for @ref gl::graph_traits "graph_traits", with the
/// > `ReprTag` parameter fixed to @ref gl::repr::list_t "repr::list_t". This means that when using `list_graph_traits`,
/// > only the `DirectionalTag`, `VertexProperties`, `EdgeProperties`, `ApiPolicyTag`, and `IdType` parameters need to be specified.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_api_policy_tag ApiPolicyTag = api::strict_t,
    traits::c_id_type IdType = default_id_type>
using list_graph_traits =
    graph_traits<DirectionalTag, VertexProperties, EdgeProperties, repr::list_t, ApiPolicyTag, IdType>;

/// @ingroup GL-Core
/// @brief Type alias for graph traits with a flattened adjacency list representation.
///
/// This alias simplifies the specification of graph traits for graphs that use a flattened adjacency list representation,
/// allowing users to easily define their graph types with the desired properties and directionality while defaulting
/// to the flattened adjacency list representation.
///
/// > [!NOTE] Template parameters
/// >
/// > The template parameters for this alias are the same as those for @ref gl::graph_traits "graph_traits", with the
/// > `ReprTag` parameter fixed to @ref gl::repr::flat_list_t "repr::flat_list_t". This means that when using `flat_list_graph_traits`,
/// > only the `DirectionalTag`, `VertexProperties`, `EdgeProperties`, `ApiPolicyTag`, and `IdType` parameters need to be specified.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_api_policy_tag ApiPolicyTag = api::strict_t,
    traits::c_id_type IdType = default_id_type>
using flat_list_graph_traits = graph_traits<
    DirectionalTag,
    VertexProperties,
    EdgeProperties,
    repr::flat_list_t,
    ApiPolicyTag,
    IdType>;

/// @ingroup GL-Core
/// @brief Type alias for graph traits with an adjacency matrix representation.
///
/// This alias simplifies the specification of graph traits for graphs that use an adjacency matrix representation,
/// allowing users to easily define their graph types with the desired properties and directionality while defaulting
/// to the adjacency matrix representation.
///
/// > [!NOTE] Template parameters
/// >
/// > The template parameters for this alias are the same as those for @ref gl::graph_traits "graph_traits", with the
/// > `ReprTag` parameter fixed to @ref gl::repr::matrix_t "repr::matrix_t". This means that when using `matrix_graph_traits`,
/// > only the `DirectionalTag`, `VertexProperties`, `EdgeProperties`, `ApiPolicyTag`, and `IdType` parameters need to be specified.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_api_policy_tag ApiPolicyTag = api::strict_t,
    traits::c_id_type IdType = default_id_type>
using matrix_graph_traits = graph_traits<
    DirectionalTag,
    VertexProperties,
    EdgeProperties,
    repr::matrix_t,
    ApiPolicyTag,
    IdType>;

/// @ingroup GL-Core
/// @brief Type alias for graph traits with a flattened adjacency matrix representation.
///
/// This alias simplifies the specification of graph traits for graphs that use a flattened adjacency matrix representation,
/// allowing users to easily define their graph types with the desired properties and directionality while defaulting
/// to the flattened adjacency matrix representation.
///
/// > [!NOTE] Template parameters
/// >
/// > The template parameters for this alias are the same as those for @ref gl::graph_traits "graph_traits", with the
/// > `ReprTag` parameter fixed to @ref gl::repr::flat_matrix_t "repr::flat_matrix_t". This means that when using `flat_matrix_graph_traits`,
/// > only the `DirectionalTag`, `VertexProperties`, `EdgeProperties`, `ApiPolicyTag`, and `IdType` parameters need to be specified.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_api_policy_tag ApiPolicyTag = api::strict_t,
    traits::c_id_type IdType = default_id_type>
using flat_matrix_graph_traits = graph_traits<
    DirectionalTag,
    VertexProperties,
    EdgeProperties,
    repr::flat_matrix_t,
    ApiPolicyTag,
    IdType>;

/// @ingroup GL-Core
/// @brief Type alias for graph traits with an directed graph configuration.
///
/// This alias simplifies the specification of graph traits for directed graphs, allowing users to easily define their
/// graph types with the desired properties and representation while defaulting to a directed configuration.
///
/// > [!NOTE] Template parameters
/// > The template parameters for this alias are the same as those for @ref gl::graph_traits "graph_traits", with the
/// > `DirectionalTag` parameter fixed to @ref gl::directed_t "directed_t". This means that when using `directed_graph_traits`,
/// > only the `VertexProperties`, `EdgeProperties`, `ReprTag`, `ApiPolicyTag`, and `IdType` parameters need to be specified.
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_repr_tag ReprTag = repr::list_t,
    traits::c_api_policy_tag ApiPolicyTag = api::strict_t,
    traits::c_id_type IdType = default_id_type>
using directed_graph_traits =
    graph_traits<directed_t, VertexProperties, EdgeProperties, ReprTag, ApiPolicyTag, IdType>;

/// @ingroup GL-Core
/// @brief Type alias for graph traits with an undirected graph configuration.
///
/// This alias simplifies the specification of graph traits for undirected graphs, allowing users to easily define their
/// graph types with the desired properties and representation while defaulting to an undirected configuration.
///
/// > [!NOTE] Template parameters
/// > The template parameters for this alias are the same as those for @ref gl::graph_traits "graph_traits", with the
/// > `DirectionalTag` parameter fixed to @ref gl::undirected_t "undirected_t". This means that when using `undirected_graph_traits`,
/// > only the `VertexProperties`, `EdgeProperties`, `ReprTag`, `ApiPolicyTag`, and `IdType` parameters need to be specified.
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_repr_tag ReprTag = repr::list_t,
    traits::c_api_policy_tag ApiPolicyTag = api::strict_t,
    traits::c_id_type IdType = default_id_type>
using undirected_graph_traits =
    graph_traits<undirected_t, VertexProperties, EdgeProperties, ReprTag, ApiPolicyTag, IdType>;

namespace traits {

/// @ingroup GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with a list representation.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_list_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::representation_tag, repr::list_t>;

/// @ingroup GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with a flattened adjacency list representation.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_flat_list_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::representation_tag, repr::flat_list_t>;

/// @ingroup GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with an adjacency list representation (either standard or flattened).
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_adjacency_list_graph_traits =
    c_list_graph_traits<TraitsType> or c_flat_list_graph_traits<TraitsType>;

/// @ingroup GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with a matrix representation.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_matrix_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::representation_tag, repr::matrix_t>;

/// @ingroup GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with a flattened adjacency matrix representation.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_flat_matrix_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::representation_tag, repr::flat_matrix_t>;

/// @ingroup GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with a matrix representation (either standard or flattened).
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_adjacency_matrix_graph_traits =
    c_matrix_graph_traits<TraitsType> or c_flat_matrix_graph_traits<TraitsType>;

/// @ingroup GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with a directed graph configuration.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_directed_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::directional_tag, directed_t>;

/// @ingroup GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::graph_traits "graph_traits" with an undirected graph configuration.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_undirected_graph_traits =
    c_instantiation_of<TraitsType, graph_traits>
    and std::same_as<typename TraitsType::directional_tag, undirected_t>;

} // namespace traits

} // namespace gl
