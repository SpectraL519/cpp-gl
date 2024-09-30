#pragma once

#include "gl/graph_utility.hpp"

#include <functional>
#include <vector>

namespace gl {

namespace types {

struct no_return {};

struct empty_callback {};

template <type_traits::c_graph GraphType, typename ReturnType, typename... Args>
using vertex_callback = std::function<ReturnType(const typename GraphType::vertex_type&, Args...)>;

template <type_traits::c_graph GraphType, typename ReturnType, typename... Args>
using edge_callback = std::function<ReturnType(const typename GraphType::edge_type&, Args...)>;

} // namespace types

namespace type_traits {

template <typename T>
concept c_alg_no_return_type = std::same_as<T, types::no_return>;

template <typename T>
concept c_alg_return_graph_type = c_graph<T> or c_alg_no_return_type<T>;

template <typename F>
concept c_empty_callback = std::same_as<F, types::empty_callback>;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_vertex_callback =
    c_empty_callback<F>
    or std::convertible_to<F, types::vertex_callback<GraphType, ReturnType, Args...>>;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_edge_callback =
    c_empty_callback<F>
    or std::convertible_to<F, types::edge_callback<GraphType, ReturnType, Args...>>;

} // namespace type_traits

namespace algorithm::detail {

template <type_traits::c_alg_return_graph_type ReturnGraphType>
using alg_return_graph_type =
    std::conditional_t<type_traits::c_alg_no_return_type<ReturnGraphType>, void, ReturnGraphType>;

} // namespace algorithm::detail

} // namespace gl
