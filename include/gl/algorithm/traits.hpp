// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm/traits.hpp
/// @brief C++20 concepts for validating custom callbacks and predicates passed to graph algorithms.

#pragma once

#include "gl/algorithm/core.hpp"

#include <functional>

namespace gl::traits {

/// @ingroup GL GL-Traits
/// @brief Concept checking if a given type is the @ref gl::algorithm::empty_callback "empty_callback" tag.
template <typename F>
concept c_empty_callback = std::same_as<F, algorithm::empty_callback>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is callable with specific arguments and returns a specific type.
/// @tparam F The callable type.
/// @tparam ReturnType The expected return type.
/// @tparam Args The parameter types the callable must accept.
template <typename F, typename ReturnType, typename... Args>
concept c_callback = std::is_invocable_r_v<ReturnType, F, Args...>;

/// @ingroup GL GL-Traits
/// @brief Concept allowing either a valid callback or the explicit absence of one through the use of @ref gl::algorithm::empty_callback "empty_callback".
template <typename F, typename ReturnType, typename... Args>
concept c_optional_callback = c_empty_callback<F> or c_callback<F, ReturnType, Args...>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is a boolean predicate callable with specific arguments.
template <typename F, typename... Args>
concept c_predicate = std::predicate<F, Args...>;

/// @ingroup GL GL-Traits
/// @brief Concept allowing either a valid boolean predicate or the explicit absence of one through the use of @ref gl::algorithm::empty_callback "empty_callback".
template <typename F, typename... Args>
concept c_optional_predicate = c_empty_callback<F> or c_predicate<F, Args...>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is a predicate returning a @ref gl::algorithm::decision "decision".
template <typename F, typename... Args>
concept c_decision_predicate = std::is_invocable_r_v<algorithm::decision, F, Args...>;

/// @ingroup GL GL-Traits
/// @brief Concept allowing either a valid decision predicate or the explicit absence of one.
template <typename F, typename... Args>
concept c_optional_decision_predicate = c_empty_callback<F> or c_decision_predicate<F, Args...>;

} // namespace gl::traits
