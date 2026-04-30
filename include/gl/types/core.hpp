// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/types/core.hpp
/// @brief Core type definitions, traits, and utilities for the GL library.

#pragma once

#include "gl/attributes/force_inline.hpp"

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>

namespace gl {

/// @ingroup GL-Core
/// @brief Type alias for the standard size type used throughout the library.
///
/// Used primarily for indices, counts, and sizes of graph components.
using size_type = std::size_t;

/// @ingroup GL-Core
/// @brief The default unsigned integer type used for vertex and edge identifiers.
using default_id_type = std::uint32_t;

namespace traits {

/// @ingroup GL-Traits
/// @brief Concept defining the requirements for an identifier type.
///
/// Ensures that any custom ID type provided to the graph library is an
/// unsigned integral type to guarantee safe indexing and representation.
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_id_type = std::unsigned_integral<T>;

} // namespace traits

/// @ingroup GL-Util
/// @brief Converts a valid identifier to a standard size type (index).
///
/// Provides a safe, explicit cast from any unsigned integral ID type
/// into a `size_type`, commonly used for array/vector lookups.
///
/// @param id The identifier to convert.
/// @return The identifier statically cast to `size_type`.
[[nodiscard]] gl_attr_force_inline constexpr size_type to_idx(traits::c_id_type auto id) noexcept {
    return static_cast<size_type>(id);
}

/// @ingroup GL-Util
/// @brief Converts an integral value to a standard pointer difference type.
///
/// Useful for safe pointer arithmetic and offset calculations within graph data structures.
///
/// @param i The integral value to convert.
/// @return The value statically cast to `std::ptrdiff_t`.
[[nodiscard]] gl_attr_force_inline constexpr std::ptrdiff_t to_diff(std::integral auto i) noexcept {
    return static_cast<std::ptrdiff_t>(i);
}

/// @ingroup GL-Types
/// @brief A type alias for a `std::pair` where both elements are of the exact same type.
/// @tparam `T` The type of both the `first` and `second` elements in the pair.
template <typename T>
using homogeneous_pair = std::pair<T, T>;

} // namespace gl
