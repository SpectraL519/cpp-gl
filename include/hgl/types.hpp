// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/types.hpp
/// @brief Core type definitions, generic data structures, and properties for the HGL module.

#pragma once

#include "gl/types/core.hpp"
#include "gl/types/flat_jagged_vector.hpp"
#include "gl/types/flat_matrix.hpp"
#include "gl/types/properties.hpp"
#include "hgl/traits.hpp"

namespace hgl {

// --- core types ---

/// @ingroup HGL-Types
/// @brief @copybrief gl::size_type
///
/// Used primarily for indices, counts, and sizes of hypergraph components.
/// @see gl::size_type
using size_type = gl::size_type;

/// @ingroup HGL-Types
/// @brief The default unsigned integer type used for vertex and hyperedge identifiers.
/// @see gl::default_id_type
using default_id_type = gl::default_id_type;

/// @ingroup HGL-Types
/// @brief @copybrief gl::to_idx
/// @see gl::to_idx
using gl::to_idx;

/// @ingroup HGL-Types
/// @brief @copybrief gl::to_diff
/// @see gl::to_diff
using gl::to_diff;

namespace traits {

/// @ingroup HGL-Traits
/// @brief Concept defining the requirements for an identifier type.
/// ### See Also
/// - [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) : For the full concept documentation in the GL module.
using gl::traits::c_id_type;

} // namespace traits

// --- generic data structures ---

/// @ingroup HGL-Types
/// @brief @copybrief gl::flat_jagged_vector
/// ### See Also
/// - @ref gl::flat_jagged_vector : For the full type definition.
template <std::semiregular T>
using flat_jagged_vector = gl::flat_jagged_vector<T>;

/// @ingroup HGL-Types
/// @brief @copybrief gl::flat_matrix
/// ### See Also
/// - @ref gl::flat_matrix "gl::flat_matrix" : For the full type definition.
template <std::semiregular T>
using flat_matrix = gl::flat_matrix<T>;

/// @ingroup HGL-Types
/// @brief @copybrief gl::homogeneous_pair
/// ### See Also
/// - @ref gl::homogeneous_pair : For the full type definition.
template <typename T>
using homogeneous_pair = gl::homogeneous_pair<T>;

// --- property types ---

/// @ingroup HGL-Core
/// @brief @copybrief gl::empty_properties
///
/// > [!IMPORTANT]
/// >
/// > This type is used as a default `properties_type` for hypergraph components that do not require any user-defined data.
/// > It serves as a marker to indicate that the component is *property-less* and can be optimized accordingly.
///
/// ### See Also
/// - @ref gl::empty_properties : For the full type definition.
using empty_properties = gl::empty_properties;

/// @ingroup HGL-Core
/// @brief @copybrief gl::empty_properties_map
///
/// > [!NOTE]
/// >
/// > This type is used internally by the library to optimize storage for hypergraph components that have no properties.
///
/// ### See Also
/// - @ref gl::empty_properties_map : For the full type definition.
using empty_properties_map = gl::empty_properties_map;

/// @ingroup HGL-Core
/// @brief @copybrief gl::name_property
/// ### See Also
/// - @ref gl::name_property : For the full type definition.
using name_property = gl::name_property;

/// @ingroup HGL-Core
/// @brief @copybrief gl::dynamic_properties
/// ### See Also
/// - @ref gl::dynamic_properties : For the full type definition.
using dynamic_properties = gl::dynamic_properties;

/// @ingroup HGL-Core
/// @brief A property struct providing arithmetic weight for hyperedges or vertices.
/// ### See Also
/// - @ref gl::weight_property : For the full type definition.
template <traits::c_arithmetic WeightType = double>
using weight_property = gl::weight_property<WeightType>;

namespace traits {

/// @ingroup HGL-Traits
/// @brief Requirements for properties that support binary coloring algorithms.
/// ### See Also
/// - [**c_binary_color_properties_type**](gl_concepts.md#gl-traits-c-binary-color-properties-type) : For the full concept documentation in the GL module.
using gl::traits::c_binary_color_properties_type;

/// @ingroup HGL-Traits
/// @brief Validates if a type is specifically the empty properties tag.
/// ### See Also
/// - [**c_empty_properties**](gl_concepts.md#gl-traits-c-empty-properties) : For the full concept documentation in the GL module.
using gl::traits::c_empty_properties;

/// @ingroup HGL-Traits
/// @brief Checks if a type or component has a nested `properties_type` that is the empty properties tag.
/// ### See Also
/// - [**c_has_empty_properties**](gl_concepts.md#gl-traits-c-has-empty-properties) : For the full concept documentation in the GL module.
using gl::traits::c_has_empty_properties;

/// @ingroup HGL-Traits
/// @brief Checks if a type or component has a nested `properties_type` that is not the empty properties tag.
/// ### See Also
/// - [**c_has_non_empty_properties**](gl_concepts.md#gl-traits-c-has-non_empty_properties) : For the full concept documentation in the GL module.
using gl::traits::c_has_non_empty_properties;

/// @ingroup HGL-Traits
/// @brief Validates if a property type contains actual user-defined data.
/// ### See Also
/// - [**c_non_empty_properties**](gl_concepts.md#gl-traits-c-non_empty_properties) : For the full concept documentation in the GL module.
using gl::traits::c_non_empty_properties;

/// @ingroup HGL-Traits
/// @brief Defines the minimal requirements for a type to be used as a property.
/// ### See Also
/// - [**c_properties**](gl_concepts.md#gl-traits-c-properties) : For the full concept documentation in the GL module.
using gl::traits::c_properties;

/// @ingroup HGL-Traits
/// @brief Requirements for properties that support arithmetic weight values.
/// ### See Also
/// - [**c_weight_properties_type**](gl_concepts.md#gl-traits-c-weight-properties_type) : For the full concept documentation in the GL module.
using gl::traits::c_weight_properties_type;

} // namespace traits

} // namespace hgl
