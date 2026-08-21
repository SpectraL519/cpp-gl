// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/api.hpp
/// @brief Defines the API configuration policy tags and related concepts for toggling internal validation.

#pragma once

#include "gl/traits.hpp"

namespace gl::api {

/// @ingroup GL-Core
/// @brief Tag specifying a strict API policy.
///
/// Enables strict bounds-checking and input validation. Invalid operations will throw exceptions.
struct strict_t {};

/// @ingroup GL-Core
/// @brief Tag specifying a relaxed API policy.
///
/// Disables bounds-checking for maximum performance. Passing invalid inputs results in undefined behavior.
struct relaxed_t {};

} // namespace gl::api

namespace gl::traits {

/// @ingroup GL-Traits
/// @brief Concept checking if a type is a valid API policy tag.
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_api_policy_tag = c_one_of<std::remove_cvref_t<T>, api::strict_t, api::relaxed_t>;

} // namespace gl::traits
