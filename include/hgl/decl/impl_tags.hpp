// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/decl/impl_tags.hpp
/// @brief Forward declares implementation tags for different hypergraph representations.

#pragma once

#include "hgl/impl/layout_tags.hpp"
#include "hgl/types.hpp"

namespace hgl {

namespace impl {

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

} // namespace impl

namespace traits {

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of the @ref hgl::impl::list_t "impl::list_t" tag.
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_list_impl = c_instantiation_of<T, impl::list_t>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of the @ref hgl::impl::flat_list_t "impl::flat_list_t" tag.
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_flat_list_impl = c_instantiation_of<T, impl::flat_list_t>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is a valid incidence list implementation tag (either standard or flattened).
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_incidence_list_impl =
    c_hypergraph_list_impl<T> or c_hypergraph_flat_list_impl<T>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of the @ref hgl::impl::matrix_t "impl::matrix_t" tag.
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_matrix_impl = c_instantiation_of<T, impl::matrix_t>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is an instantiation of the @ref hgl::impl::flat_matrix_t "impl::flat_matrix_t" tag.
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_flat_matrix_impl = c_instantiation_of<T, impl::flat_matrix_t>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is a valid incidence matrix implementation tag (either standard or flattened).
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_incidence_matrix_impl =
    c_hypergraph_matrix_impl<T> or c_hypergraph_flat_matrix_impl<T>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is one of the defined hypergraph implementation tags.
///
/// This concept is used to constrain template parameters that are expected to be
/// specific hypergraph implementation tags, ensuring type safety and clear intent in template usage.
///
/// The valid tags include:
/// - @ref hgl::impl::list_t "impl::list_t": Represents a standard incidence list implementation.
/// - @ref hgl::impl::flat_list_t "impl::flat_list_t": Represents a flattened incidence list implementation.
/// - @ref hgl::impl::matrix_t "impl::matrix_t": Represents a standard incidence matrix implementation.
/// - @ref hgl::impl::flat_matrix_t "impl::flat_matrix_t": Represents a flattened incidence matrix implementation.
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_impl_tag =
    c_hypergraph_incidence_list_impl<T> or c_hypergraph_incidence_matrix_impl<T>;

} // namespace traits

} // namespace hgl
