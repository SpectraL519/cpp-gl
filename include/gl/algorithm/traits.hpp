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
concept c_optional_callback = c_empty_callback<F> or std::is_invocable_r_v<ReturnType, F, Args...>;

template <typename F, typename ReturnType, typename... Args>
concept c_id_callback = std::is_invocable_r_v<ReturnType, F, const types::id_type, Args...>;

template <typename F, typename ReturnType, typename... Args>
concept c_optional_id_callback = c_optional_callback<F, ReturnType, const types::id_type, Args...>;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_edge_callback =
    c_graph<GraphType>
    and std::is_invocable_r_v<ReturnType, F, const typename GraphType::edge_type&, Args...>;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_optional_edge_callback =
    c_graph<GraphType>
    and c_optional_callback<F, ReturnType, const typename GraphType::edge_type&, Args...>;

} // namespace gl::traits
