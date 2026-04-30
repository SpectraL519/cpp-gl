// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/constants.hpp
/// @brief Defines common constant values and types used across the CPP-GL library.

#pragma once

#include "gl/types/core.hpp"

#include <limits>

namespace gl {

/// @ingroup GL-Core
/// @brief A constant representing the initial ID value of 0 for graph elements.
/// @tparam IdType The type of the ID, which must satisfy the [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) concept.
/// ### See Also
/// - [**c_id_type**](gl_concepts.md#gl-traits-c-id-type)
/// - @ref gl::initial_id_t "initial_id_t"
/// - @ref gl::initial_id "initial_id"
template <traits::c_id_type IdType>
inline constexpr IdType initial_id_v{0};

/// @ingroup GL-Core
/// @brief A helper type that can be implicitly converted to the initial ID value of 0 for any valid ID type.
/// ### See Also
/// - [**c_id_type**](gl_concepts.md#gl-traits-c-id-type)
/// - @ref gl::initial_id_v "initial_id_v"
/// - @ref gl::initial_id "initial_id"
struct initial_id_t {
    /// @brief Implicitly converts to the initial ID value of 0 for any type that satisfies the [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) concept.
    template <traits::c_id_type IdType>
    [[nodiscard]] constexpr operator IdType() const noexcept {
        return initial_id_v<IdType>;
    }
};

/// @ingroup GL-Core
/// @brief An `initial_id_t` tag constant that can be used to represent the initial ID value of 0 for graph elements in a type-safe manner.
///
/// ### Example Usage
/// ```cpp
/// using vertex_id_t = std::uint32_t;
/// vertex_id_t v1 = gl::initial_id; // (1)!
/// ```
///
/// 1\. The `initial_id` constant can be implicitly converted to the `v1`'s type (`vertex_id_t`), resulting in `v1` being initialized to the value of 0.
///
/// ### See Also
/// - [**c_id_type**](gl_concepts.md#gl-traits-c-id-type)
/// - @ref gl::initial_id_v "initial_id_v"
/// - @ref gl::initial_id_t "initial_id_t"
inline constexpr initial_id_t initial_id{};

// --- invalid id ---

/// @ingroup GL-Core
/// @brief A constant representing the invalid ID value for graph elements, defined as the maximum value of the specified ID type.
/// @tparam IdType The type of the ID, which must satisfy the [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) concept.
/// ### See Also
/// - [**c_id_type**](gl_concepts.md#gl-traits-c-id-type)
/// - @ref gl::invalid_id_t "invalid_id_t"
/// - @ref gl::invalid_id "invalid_id"
template <traits::c_id_type IdType>
inline constexpr IdType invalid_id_v{std::numeric_limits<IdType>::max()};

/// @ingroup GL-Core
/// @brief A helper type that can be implicitly converted to the invalid ID value for any valid ID type.
/// ### See Also
/// - [**c_id_type**](gl_concepts.md#gl-traits-c-id-type)
/// - @ref gl::invalid_id_v "invalid_id_v"
/// - @ref gl::invalid_id "invalid_id"
struct invalid_id_t {
    /// @brief Implicitly converts to the invalid ID value for any type that satisfies the [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) concept.
    template <traits::c_id_type IdType>
    [[nodiscard]] constexpr operator IdType() const noexcept {
        return invalid_id_v<IdType>;
    }

    /// @brief Equality comparison operator to compare an ID value with the invalid ID constant.
    ///
    /// This operator allows for direct comparison between an ID value and the `invalid_id` constant,
    /// enabling easy checking if a given ID is invalid without needing to explicitly reference
    /// the `invalid_id_v` constant for the specific ID type.
    ///
    /// @tparam IdType The type of the ID, which must satisfy the [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) concept.
    /// @param lhs The ID value to compare againsyt the invalid ID constant.
    /// @param rhs The `invalid_id` constant (of type `invalid_id_t`) to compare with the ID value.
    /// @return Returns `true` if `lhs` is equal to the invalid ID value for its type, and `false` otherwise.
    template <traits::c_id_type IdType>
    [[nodiscard]] friend constexpr bool operator==(const IdType& lhs, invalid_id_t) noexcept {
        return lhs == invalid_id_v<IdType>;
    }
};

/// @ingroup GL-Core
/// @brief An `invalid_id_t` tag constant that can be used to represent the invalid ID value for graph elements in a type-safe manner.
///
/// ### Example Usage
/// ```cpp
/// using vertex_id_t = std::uint32_t;
/// vertex_id_t v1 = gl::invalid_id; // (1)!
/// ```
///
/// 1\. The `invalid_id` constant can be implicitly converted to the `v1`'s type (`vertex_id_t`), resulting in `v1` being initialized to the maximum value of `vertex_id_t`, which represents an invalid ID.
///
/// ### See Also
/// - [**c_id_type**](gl_concepts.md#gl-traits-c-id-type)
/// - @ref gl::invalid_id_v "invalid_id_v"
/// - @ref gl::invalid_id_t "invalid_id_t"
inline constexpr invalid_id_t invalid_id{};

} // namespace gl
