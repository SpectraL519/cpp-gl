// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/graph_utility.hpp"

#include <functional>

namespace gl {

namespace types {

struct no_return {};

struct empty_callback {};

template <type_traits::c_graph GraphType, typename ReturnType, typename... Args>
using vertex_callback = std::function<ReturnType(const typename GraphType::vertex_type&, Args...)>;

template <type_traits::c_graph GraphType, typename ReturnType, typename... Args>
using edge_callback = std::function<ReturnType(const typename GraphType::edge_type&, Args...)>;

struct vertex_info {
    vertex_info(types::id_type id) : id(id), source_id(id) {}

    vertex_info(types::id_type id, types::id_type source_id) : id(id), source_id(source_id) {}

    // if id == source_id then vertex_id is the id of the starting vertex
    types::id_type id;
    types::id_type source_id;
};

template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
struct edge_info {
    using edge_type = EdgeType;

    types::const_ref_wrap<edge_type> edge;
    types::id_type source_id;
};

} // namespace types

namespace type_traits {

template <typename T>
concept c_alg_no_return_type = std::same_as<T, types::no_return>;

template <typename T>
concept c_alg_return_graph_type = c_graph<T> or c_alg_no_return_type<T>;

template <typename F>
concept c_empty_callback = std::same_as<F, types::empty_callback>;

template <typename F, typename ReturnType, typename... Args>
concept c_optional_callback = c_empty_callback<F> or std::is_invocable_r_v<ReturnType, F, Args...>;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_vertex_callback =
    std::is_invocable_r_v<ReturnType, F, const typename GraphType::vertex_type&, Args...>;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_edge_callback =
    std::is_invocable_r_v<ReturnType, F, const typename GraphType::edge_type&, Args...>;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_optional_vertex_callback =
    c_optional_callback<F, ReturnType, const typename GraphType::vertex_type&, Args...>;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_optional_edge_callback =
    c_optional_callback<F, ReturnType, const typename GraphType::edge_type&, Args...>;

} // namespace type_traits

namespace algorithm::detail {

template <type_traits::c_alg_return_graph_type ReturnGraphType>
using alg_return_graph_type =
    std::conditional_t<type_traits::c_alg_no_return_type<ReturnGraphType>, void, ReturnGraphType>;

} // namespace algorithm::detail

} // namespace gl
