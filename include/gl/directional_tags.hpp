// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/directional_tags.hpp
/// @brief Defines tag types used to specify the directionality of a graph.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/traits.hpp"
#include "gl/types/properties.hpp"

namespace gl {

struct directed_t;

struct undirected_t;

namespace traits {

/// @ingroup GL GL-Traits
/// @brief Validates if a type is a valid graph directional tag.
///
/// The valid graph directional tags are @ref gl::directed_t "directed_t" and @ref gl::undirected_t "undirected_t".
/// This concept is used to constrain template parameters that are intended to specify the directionality of a graph,
/// ensuring that only valid tags can be used in such contexts.
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_graph_directional_tag = c_one_of<T, directed_t, undirected_t>;

} // namespace traits

template <
    traits::c_graph_directional_tag DirectionalTag,
    traits::c_properties Properties,
    traits::c_id_type IdType>
class edge_descriptor;

namespace traits {

/// @ingroup GL GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::edge_descriptor "edge_descriptor" with the @ref gl::directed_t "directed_t" tag.
/// @tparam T The type to evaluate against the concept.
template <typename E>
concept c_directed_edge =
    c_instantiation_of<E, edge_descriptor>
    and std::same_as<typename E::directional_tag, directed_t>;

/// @ingroup GL GL-Traits
/// @brief Concept to validate if a type is an instantiation of @ref gl::edge_descriptor "edge_descriptor" with the @ref gl::undirected_t "undirected_t" tag.
/// @tparam T The type to evaluate against the concept.
template <typename E>
concept c_undirected_edge =
    c_instantiation_of<E, edge_descriptor>
    and std::same_as<typename E::directional_tag, undirected_t>;

} // namespace traits

/// @ingroup GL GL-Core
/// @headerfile gl/directional_tags.hpp
/// @brief The tag type representing a directed graph configuration.
///
/// This tag is used to indicate that a graph is directed, meaning that its edges
/// have a specific direction from a source vertex to a target vertex.
///
/// ### See Also
/// - @ref gl::undirected_t "undirected_t" : For the tag representing an undirected graph configuration.
/// - [**c_graph_directional_tag**](gl_traits.md#gl-traits-c-graph-directional-tag) : For the concept used to validate graph directional tags.
/// - @ref gl::edge_descriptor "edge_descriptor" : For the edge descriptor type defined based on the graph traits, which includes the directional tag as part of its definition.
struct directed_t {
    /// @brief A type identity alias for the directed_t tag, allowing for easier type comparisons and trait evaluations.
    using type = std::type_identity_t<directed_t>;

    /// @brief Determines if a given edge is incident from a specified vertex in a directed graph.
    ///
    /// Validates whether the provided vertex ID corresponds specifically to the **source** vertex of the edge.
    ///
    /// @tparam EdgeType The type of the edge descriptor, which must be an instantiation of @ref gl::edge_descriptor "edge_descriptor" with the directed tag.
    /// @return For a directed edge $(u, v)$ and a vertex ID `vertex_id`, returns `true` if `vertex_id` corresponds to $u$ (the source vertex), and `false` otherwise.
    template <traits::c_directed_edge EdgeType>
    [[nodiscard]] gl_attr_force_inline static bool is_incident_from(
        const EdgeType& edge, typename EdgeType::id_type vertex_id
    ) {
        return vertex_id == edge._vertices.first;
    }

    /// @brief Determines if a given edge is incident to a specified vertex in a directed graph.
    ///
    /// Validates whether the provided vertex ID corresponds specifically to the **target** vertex of the edge.
    ///
    /// @tparam EdgeType The type of the edge descriptor, which must be an instantiation of @ref gl::edge_descriptor "edge_descriptor" with the directed tag.
    /// @return For a directed edge $(u, v)$ and a vertex ID `vertex_id`, returns `true` if `vertex_id` corresponds to $v$ (the target vertex), and `false` otherwise.
    template <traits::c_directed_edge EdgeType>
    [[nodiscard]] gl_attr_force_inline static bool is_incident_to(
        const EdgeType& edge, typename EdgeType::id_type vertex_id
    ) {
        return vertex_id == edge._vertices.second;
    }
};

/// @ingroup GL GL-Core
/// @headerfile gl/directional_tags.hpp
/// @brief The tag type representing an undirected graph configuration.
///
/// This tag is used to indicate that a graph is undirected, meaning that its edges do not have
/// a specific direction and can be traversed in both directions between the connected vertices.
///
/// ### See Also
/// - @ref gl::directed_t "directed_t" : For the tag representing a directed graph configuration.
/// - [**c_graph_directional_tag**](gl_traits.md#gl-traits-c-graph-directional-tag) : For the concept used to validate graph directional tags.
/// - @ref gl::edge_descriptor "edge_descriptor" : For the edge descriptor type defined based on the graph traits, which includes the directional tag as part of its definition.
struct undirected_t {
    /// @brief A type identity alias for the undirected_t tag, allowing for easier type comparisons and trait evaluations.
    using type = std::type_identity_t<undirected_t>;

    /// @brief Determines if a given edge is incident from a specified vertex in an undirected graph.
    ///
    /// Validates whether the provided vertex ID corresponds to either of the vertices connected by the edge,
    /// since in an undirected graph, both vertices are incident *with* the edge, hence they are both considered
    /// to be incident from the edge.
    ///
    /// @tparam EdgeType The type of the edge descriptor, which must be an instantiation of @ref gl::edge_descriptor "edge_descriptor" with the undirected tag.
    /// @return For an undirected edge \f$\{u, v\}\f$, and a vertex ID `vertex_id`, returns `true` if `vertex_id` corresponds to either $u$ or $v$, and `false` otherwise.
    template <traits::c_undirected_edge EdgeType>
    [[nodiscard]] gl_attr_force_inline static bool is_incident_from(
        const EdgeType& edge, typename EdgeType::id_type vertex_id
    ) {
        return edge.is_incident_with(vertex_id);
    }

    /// @brief Determines if a given edge is incident to a specified vertex in an undirected graph.
    ///
    /// Validates whether the provided vertex ID corresponds to either of the vertices connected by the edge,
    /// since in an undirected graph, both vertices are incident *with* the edge, hence they are both considered
    /// to be incident to the edge.
    ///
    /// @tparam EdgeType The type of the edge descriptor, which must be an instantiation of @ref gl::edge_descriptor "edge_descriptor" with the undirected tag.
    /// @return For an undirected edge \f$\{u, v\}\f$, and a vertex ID `vertex_id`, returns `true` if `vertex_id` corresponds to either $u$ or $v$, and `false` otherwise.
    template <traits::c_undirected_edge EdgeType>
    [[nodiscard]] gl_attr_force_inline static bool is_incident_to(
        const EdgeType& edge, typename EdgeType::id_type vertex_id
    ) {
        return edge.is_incident_with(vertex_id);
    }
};

} // namespace gl
