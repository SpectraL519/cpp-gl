// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/constants.hpp
/// @brief Defines common constant values and types used across the HGL module.

#pragma once

#include "gl/constants.hpp"
#include "hgl/traits.hpp"

namespace hgl {

/// @ingroup HGL-Core
/// @brief A constant representing the initial ID value of 0 for hypergraph elements.
/// @see gl::initial_id_v
template <traits::c_id_type IdType>
inline constexpr IdType initial_id_v = gl::initial_id_v<IdType>;

/// @ingroup HGL-Core
/// @brief A helper type that can be implicitly converted to the initial ID value of 0 for any valid ID type.
/// @see gl::initial_id_t
using initial_id_t = gl::initial_id_t;

/// @ingroup HGL-Core
/// @brief An `initial_id_t` tag constant that can be used to represent the initial ID value of 0 for hypergraph elements in a type-safe manner.
/// @see gl::initial_id
inline constexpr initial_id_t initial_id = gl::initial_id;

/// @ingroup HGL-Core
/// @brief A constant representing the invalid ID value for hypergraph elements, defined as the maximum value of the specified ID type.
/// @see gl::invalid_id_v
template <traits::c_id_type IdType>
inline constexpr IdType invalid_id_v = gl::invalid_id_v<IdType>;

/// @ingroup HGL-Core
/// @brief A helper type that can be implicitly converted to the invalid ID value for any valid ID type.
/// @see gl::invalid_id_t
using invalid_id_t = gl::invalid_id_t;

/// @ingroup HGL-Core
/// @brief An `invalid_id_t` tag constant that can be used to represent the invalid ID value for hypergraph elements in a type-safe manner.
/// @see gl::invalid_id
inline constexpr invalid_id_t invalid_id = gl::invalid_id;

} // namespace hgl
