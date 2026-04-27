// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/decl/impl_tags.hpp
/// @brief Forward declares implementation tags for different graph representations.

#pragma once

#include "gl/traits.hpp"

namespace gl {

namespace impl {

struct list_t;

struct flat_list_t;

struct matrix_t;

struct flat_matrix_t;

} // namespace impl

namespace traits {

/// @ingroup GL GL-Traits
/// @brief Validates if a type is one of the defined graph implementation tags.
///
/// This concept is used to constrain template parameters that are expected to be
/// specific graph implementation tags, ensuring type safety and clear intent in template usage.
///
/// The valid tags include:
/// - @ref gl::impl::list_t "impl::list_t": Represents a standard adjacency list implementation.
/// - @ref gl::impl::flat_list_t "impl::flat_list_t": Represents a flattened adjacency list implementation.
/// - @ref gl::impl::matrix_t "impl::matrix_t": Represents a standard adjacency matrix implementation.
/// - @ref gl::impl::flat_matrix_t "impl::flat_matrix_t": Represents a flattened adjacency matrix implementation.
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_graph_impl_tag =
    c_one_of<T, impl::list_t, impl::flat_list_t, impl::matrix_t, impl::flat_matrix_t>;

} // namespace traits

} // namespace gl
