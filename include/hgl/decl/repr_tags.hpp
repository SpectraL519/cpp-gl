// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/decl/repr_tags.hpp
/// @brief Forward declares representation tags for different hypergraph models.

#pragma once

#include "hgl/repr/layout_tags.hpp"
#include "hgl/types.hpp"

namespace hgl {

namespace repr {

template <
    traits::c_hypergraph_layout_tag LayoutTag = bidirectional_t,
    traits::c_id_type IdType = default_id_type>
struct list_t;

template <
    traits::c_hypergraph_layout_tag LayoutTag = bidirectional_t,
    traits::c_id_type IdType = default_id_type>
struct flat_list_t;

template <
    traits::c_hypergraph_asymmetric_layout_tag LayoutTag = hyperedge_major_t,
    traits::c_id_type IdType = default_id_type>
struct matrix_t;

template <
    traits::c_hypergraph_asymmetric_layout_tag LayoutTag = hyperedge_major_t,
    traits::c_id_type IdType = default_id_type>
struct flat_matrix_t;

} // namespace repr

namespace traits {

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of the @ref hgl::repr::list_t "repr::list_t" tag.
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_list_repr = c_instantiation_of<T, repr::list_t>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of the @ref hgl::repr::flat_list_t "repr::flat_list_t" tag.
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_flat_list_repr = c_instantiation_of<T, repr::flat_list_t>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is a valid incidence list representation tag (either standard or flattened).
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_incidence_list_repr =
    c_hypergraph_list_repr<T> or c_hypergraph_flat_list_repr<T>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of the @ref hgl::repr::matrix_t "repr::matrix_t" tag.
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_matrix_repr = c_instantiation_of<T, repr::matrix_t>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of the @ref hgl::repr::flat_matrix_t "repr::flat_matrix_t" tag.
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_flat_matrix_repr = c_instantiation_of<T, repr::flat_matrix_t>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is a valid incidence matrix representation tag (either standard or flattened).
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_incidence_matrix_repr =
    c_hypergraph_matrix_repr<T> or c_hypergraph_flat_matrix_repr<T>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is one of the defined hypergraph representation tags.
///
/// This concept is used to constrain template parameters that are expected to be
/// specific hypergraph representation tags, ensuring type safety and clear intent in template usage.
///
/// The valid tags include:
/// - @ref hgl::repr::list_t "repr::list_t": Represents a standard incidence list representation.
/// - @ref hgl::repr::flat_list_t "repr::flat_list_t": Represents a flattened incidence list representation.
/// - @ref hgl::repr::matrix_t "repr::matrix_t": Represents a standard incidence matrix representation.
/// - @ref hgl::repr::flat_matrix_t "repr::flat_matrix_t": Represents a flattened incidence matrix representation.
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_repr_tag =
    c_hypergraph_incidence_list_repr<T> or c_hypergraph_incidence_matrix_repr<T>;

} // namespace traits

} // namespace hgl
