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

namespace hgl {

// --- core types ---

/// @ingroup HGL-Types
/// @brief @copybrief gl::size_type
///
/// Used primarily for indices, counts, and sizes of hypergraph components.
/// @see gl::size_type
using gl::size_type;

/// @ingroup HGL-Types
/// @brief The default unsigned integer type used for vertex and hyperedge identifiers.
/// @see gl::default_id_type
using gl::default_id_type;

/// @ingroup HGL-Types
/// @brief @copybrief gl::to_idx
/// @see gl::to_idx
using gl::to_idx;

/// @ingroup HGL-Types
/// @brief @copybrief gl::to_diff
/// @see gl::to_diff
using gl::to_diff;

// --- generic data structures ---

/// @ingroup HGL-Types
/// @brief @copybrief gl::flat_jagged_vector
/// @see gl::flat_jagged_vector
using gl::flat_jagged_vector;

/// @ingroup HGL-Types
/// @brief @copybrief gl::flat_matrix
/// @see gl::flat_matrix
using gl::flat_matrix;

/// @ingroup HGL-Types
/// @brief @copybrief gl::homogeneous_pair
/// @see gl::homogeneous_pair
using gl::homogeneous_pair;

// --- property types ---

/// @ingroup HGL-Core
/// @brief @copybrief gl::empty_properties
///
/// > [!IMPORTANT]
/// >
/// > This type is used as a default `properties_type` for hypergraph components that do not require any user-defined data.
/// > It serves as a marker to indicate that the component is *property-less* and can be optimized accordingly.
///
/// @see gl::empty_properties
using gl::empty_properties;

/// @ingroup HGL-Core
/// @brief @copybrief gl::empty_properties_map
///
/// > [!NOTE]
/// >
/// > This type is used internally by the library to optimize storage for hypergraph components that have no properties.
///
/// @see gl::empty_properties_map
using gl::empty_properties_map;

/// @ingroup HGL-Core
/// @brief @copybrief gl::name_property
/// @see gl::name_property
using gl::name_property;

/// @ingroup HGL-Core
/// @brief @copybrief gl::dynamic_properties
/// @see gl::dynamic_properties
using gl::dynamic_properties;

/// @ingroup HGL-Core
/// @brief A property struct providing arithmetic weight for hyperedges or vertices.
/// @see gl::weight_property
using gl::weight_property;

} // namespace hgl
