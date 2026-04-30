// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/decl/repr_tags.hpp
/// @brief Forward declares representation tags for different graph representations.

#pragma once

#include "gl/traits.hpp"

namespace gl {

namespace repr {

struct list_t;

struct flat_list_t;

struct matrix_t;

struct flat_matrix_t;

} // namespace repr

namespace traits {

/// @ingroup GL-Traits
/// @brief Validates if a type is one of the defined graph representation tags.
///
/// This concept is used to constrain template parameters that are expected to be
/// specific graph representation tags, ensuring type safety and clear intent in template usage.
///
/// The valid tags include:
/// - @ref gl::repr::list_t "repr::list_t": Represents a standard adjacency list representation.
/// - @ref gl::repr::flat_list_t "repr::flat_list_t": Represents a flattened adjacency list representation.
/// - @ref gl::repr::matrix_t "repr::matrix_t": Represents a standard adjacency matrix representation.
/// - @ref gl::repr::flat_matrix_t "repr::flat_matrix_t": Represents a flattened adjacency matrix representation.
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_graph_repr_tag =
    c_one_of<T, repr::list_t, repr::flat_list_t, repr::matrix_t, repr::flat_matrix_t>;

} // namespace traits

} // namespace gl
