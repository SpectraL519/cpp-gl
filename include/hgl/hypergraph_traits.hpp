// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/hypergraph_traits.hpp
/// @brief Defines the primary hypergraph traits structure and related type aliases for different configurations.

#pragma once

#include "gl/types/core.hpp"
#include "hgl/directional_tags.hpp"
#include "hgl/hypergraph_elements.hpp"
#include "hgl/repr/layout_tags.hpp"
#include "hgl/repr/repr_tags.hpp"

namespace hgl {

/// @ingroup HGL-Core
/// @brief Primary hypergraph traits structure that encapsulates all necessary type information for hypergraph representations.
///
/// This structure serves as the central point for defining the properties and types associated with a hypergraph,
/// including directionality, element properties, as well as representation and identifier types. It provides a clean,
/// extensible way to customize hypergraph behavior.
///
/// ### Template Parameters
/// | Parameter | Description | Default | Constraint |
/// | :-------- | :---------- | :------ | :--------- |
/// | DirectionalTag | Specifies whether the hypergraph is undirected or bf_directed. | @ref hgl::undirected_t "undirected_t" | [**c_hypergraph_directional_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-directional-tag) |
/// | VertexProperties | The type of properties attached to each vertex. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | HyperedgeProperties | The type of properties attached to each hyperedge. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | ReprTag | Specifies the internal container representation for incidence. | @ref hgl::repr::list_t "repr::list_t<>" | [**c_hypergraph_repr_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-repr-tag) |
template <
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_hypergraph_repr_tag ReprTag = repr::list_t<>>
struct hypergraph_traits {
    /// @brief The directional tag indicating the hypergraph's orientation.
    using directional_tag = DirectionalTag;
    /// @brief The representation tag defining the internal storage mechanism.
    using representation_tag = ReprTag;
    /// @brief The layout tag governing major/minor element ordering.
    using layout_tag = typename representation_tag::layout_tag;
    /// @brief The integer type used for element identifiers.
    using id_type = typename representation_tag::id_type;

    /// @brief The fully resolved type representing a vertex descriptor.
    using vertex_type = vertex_descriptor<VertexProperties, id_type>;
    /// @brief The property payload type associated with vertices.
    using vertex_properties_type = typename vertex_type::properties_type;

    /// @brief The fully resolved type representing a hyperedge descriptor.
    using hyperedge_type = hyperedge_descriptor<HyperedgeProperties, id_type>;
    /// @brief The property payload type associated with hyperedges.
    using hyperedge_properties_type = typename hyperedge_type::properties_type;
};

/// @ingroup HGL-Core
/// @brief Convenience alias for `hypergraph_traits` using a standard incidence list representation.
///
/// ### Template Parameters
/// | Parameter | Description | Default | Constraint |
/// | :-------- | :---------- | :------ | :--------- |
/// | LayoutTag | Memory orientation for the list structures. | @ref hgl::repr::bidirectional_t "bidirectional_t" | [**c_hypergraph_layout_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-layout-tag) |
/// | DirectionalTag | Specifies whether the hypergraph is undirected or bf_directed. | @ref hgl::undirected_t "undirected_t" | [**c_hypergraph_directional_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-directional-tag) |
/// | VertexProperties | The type of properties attached to each vertex. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | HyperedgeProperties | The type of properties attached to each hyperedge. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | IdType | The integer type used for element identifiers. | @ref gl::default_id_type "default_id_type" | [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) |
template <
    traits::c_hypergraph_layout_tag LayoutTag = repr::bidirectional_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using list_hypergraph_traits = hypergraph_traits<
    DirectionalTag,
    VertexProperties,
    HyperedgeProperties,
    repr::list_t<LayoutTag, IdType>>;

/// @ingroup HGL-Core
/// @brief Convenience alias for `hypergraph_traits` using a flattened incidence list representation.
///
/// ### Template Parameters
/// | Parameter | Description | Default | Constraint |
/// | :-------- | :---------- | :------ | :--------- |
/// | LayoutTag | Memory orientation for the flat list structures. | @ref hgl::repr::bidirectional_t "bidirectional_t" | [**c_hypergraph_layout_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-layout-tag) |
/// | DirectionalTag | Specifies whether the hypergraph is undirected or bf_directed. | @ref hgl::undirected_t "undirected_t" | [**c_hypergraph_directional_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-directional-tag) |
/// | VertexProperties | The type of properties attached to each vertex. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | HyperedgeProperties | The type of properties attached to each hyperedge. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | IdType | The integer type used for element identifiers. | @ref gl::default_id_type "default_id_type" | [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) |
template <
    traits::c_hypergraph_layout_tag LayoutTag = repr::bidirectional_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using flat_list_hypergraph_traits = hypergraph_traits<
    DirectionalTag,
    VertexProperties,
    HyperedgeProperties,
    repr::flat_list_t<LayoutTag, IdType>>;

/// @ingroup HGL-Core
/// @brief Convenience alias for `hypergraph_traits` using a standard incidence matrix representation.
///
/// ### Template Parameters
/// | Parameter | Description | Default | Constraint |
/// | :-------- | :---------- | :------ | :--------- |
/// | LayoutTag | Memory orientation (must be asymmetric). | @ref hgl::repr::hyperedge_major_t "hyperedge_major_t" | [**c_hypergraph_asymmetric_layout_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-asymmetric-layout-tag) |
/// | DirectionalTag | Specifies whether the hypergraph is undirected or bf_directed. | @ref hgl::undirected_t "undirected_t" | [**c_hypergraph_directional_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-directional-tag) |
/// | VertexProperties | The type of properties attached to each vertex. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | HyperedgeProperties | The type of properties attached to each hyperedge. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | IdType | The integer type used for element identifiers. | @ref gl::default_id_type "default_id_type" | [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) |
template <
    traits::c_hypergraph_asymmetric_layout_tag LayoutTag = repr::hyperedge_major_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using matrix_hypergraph_traits = hypergraph_traits<
    DirectionalTag,
    VertexProperties,
    HyperedgeProperties,
    repr::matrix_t<LayoutTag, IdType>>;

/// @ingroup HGL-Core
/// @brief Convenience alias for `hypergraph_traits` using a flattened incidence matrix representation.
///
/// ### Template Parameters
/// | Parameter | Description | Default | Constraint |
/// | :-------- | :---------- | :------ | :--------- |
/// | LayoutTag | Memory orientation (must be asymmetric). | @ref hgl::repr::hyperedge_major_t "hyperedge_major_t" | [**c_hypergraph_asymmetric_layout_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-asymmetric-layout-tag) |
/// | DirectionalTag | Specifies whether the hypergraph is undirected or bf_directed. | @ref hgl::undirected_t "undirected_t" | [**c_hypergraph_directional_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-directional-tag) |
/// | VertexProperties | The type of properties attached to each vertex. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | HyperedgeProperties | The type of properties attached to each hyperedge. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | IdType | The integer type used for element identifiers. | @ref gl::default_id_type "default_id_type" | [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) |
template <
    traits::c_hypergraph_asymmetric_layout_tag LayoutTag = repr::hyperedge_major_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using flat_matrix_hypergraph_traits = hypergraph_traits<
    DirectionalTag,
    VertexProperties,
    HyperedgeProperties,
    repr::flat_matrix_t<LayoutTag, IdType>>;

/// @ingroup HGL-Core
/// @brief Type alias for undirected hypergraph traits with configurable properties and representation.
///
/// ### Template Parameters
/// | Parameter | Description | Default | Constraint |
/// | :-------- | :---------- | :------ | :--------- |
/// | VertexProperties | The type of properties attached to each vertex. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | HyperedgeProperties | The type of properties attached to each hyperedge. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | ReprTag | Specifies the internal container representation for incidence. | @ref hgl::repr::list_t "repr::list_t<>" | [**c_hypergraph_repr_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-repr-tag) |
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_hypergraph_repr_tag ReprTag = repr::list_t<>>
using undirected_hypergraph_traits =
    hypergraph_traits<undirected_t, VertexProperties, HyperedgeProperties, ReprTag>;

/// @ingroup HGL-Core
/// @brief Type alias for bf-directed hypergraph traits with configurable properties and representation.
///
/// ### Template Parameters
/// | Parameter | Description | Default | Constraint |
/// | :-------- | :---------- | :------ | :--------- |
/// | VertexProperties | The type of properties attached to each vertex. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | HyperedgeProperties | The type of properties attached to each hyperedge. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | ReprTag | Specifies the internal container representation for incidence. | @ref hgl::repr::list_t "repr::list_t<>" | [**c_hypergraph_repr_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-repr-tag) |
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_hypergraph_repr_tag ReprTag = repr::list_t<>>
using bf_directed_hypergraph_traits =
    hypergraph_traits<bf_directed_t, VertexProperties, HyperedgeProperties, ReprTag>;

namespace traits {

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of @ref hgl::hypergraph_traits "hypergraph_traits" with a standard incidence list representation.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_list_hypergraph_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and c_hypergraph_list_repr<typename TraitsType::representation_tag>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of @ref hgl::hypergraph_traits "hypergraph_traits" with a flattened incidence list representation.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_flat_list_hypergraph_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and c_hypergraph_flat_list_repr<typename TraitsType::representation_tag>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of @ref hgl::hypergraph_traits "hypergraph_traits" with an incidence list representation (either standard or flattened).
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_incidence_list_hypergraph_traits =
    c_list_hypergraph_traits<TraitsType> or c_flat_list_hypergraph_traits<TraitsType>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of @ref hgl::hypergraph_traits "hypergraph_traits" with a standard incidence matrix representation.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_matrix_hypergraph_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and c_hypergraph_matrix_repr<typename TraitsType::representation_tag>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of @ref hgl::hypergraph_traits "hypergraph_traits" with a flattened incidence matrix representation.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_flat_matrix_hypergraph_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and c_hypergraph_flat_matrix_repr<typename TraitsType::representation_tag>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of @ref hgl::hypergraph_traits "hypergraph_traits" with an incidence matrix representation (either standard or flattened).
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_incidence_matrix_hypergraph_traits =
    c_matrix_hypergraph_traits<TraitsType> or c_flat_matrix_hypergraph_traits<TraitsType>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of @ref hgl::hypergraph_traits "hypergraph_traits" with an undirected configuration.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_undirected_hypergraph_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and std::same_as<typename TraitsType::directional_tag, undirected_t>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of @ref hgl::hypergraph_traits "hypergraph_traits" with a bf_directed configuration.
/// @tparam TraitsType The type to evaluate against the concept.
template <typename TraitsType>
concept c_bf_directed_hypergraph_traits =
    c_instantiation_of<TraitsType, hypergraph_traits>
    and std::same_as<typename TraitsType::directional_tag, bf_directed_t>;

} // namespace traits

} // namespace hgl
