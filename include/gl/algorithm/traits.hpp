// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"

#include <functional>

namespace gl::traits {

template <typename F>
concept c_empty_callback = std::same_as<F, algorithm::empty_callback>;

template <typename F, typename ReturnType, typename... Args>
concept c_callback = std::is_invocable_r_v<ReturnType, F, Args...>;

template <typename F, typename ReturnType, typename... Args>
concept c_optional_callback = c_empty_callback<F> or c_callback<F, ReturnType, Args...>;

template <typename F, typename... Args>
concept c_predicate = std::predicate<F, Args...>;

template <typename F, typename... Args>
concept c_optional_predicate = c_empty_callback<F> or c_predicate<F, Args...>;

template <typename F, typename... Args>
concept c_decision_predicate = std::is_invocable_r_v<algorithm::decision, F, Args...>;

template <typename F, typename... Args>
concept c_optional_decision_predicate = c_empty_callback<F> or c_decision_predicate<F, Args...>;

} // namespace gl::traits
