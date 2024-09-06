#pragma once

#include "gl/graph.hpp"

#include <functional>
#include <vector>

namespace gl {

namespace algorithm {

struct no_return {};

struct empty_callback {};

template <type_traits::c_graph GraphType, typename ReturnType, typename... Args>
using vertex_callback = std::function<ReturnType(const typename GraphType::vertex_type&, Args...)>;

template <type_traits::c_graph GraphType, typename ReturnType, typename... Args>
using edge_callback = std::function<ReturnType(const typename GraphType::edge_type&, Args...)>;

} // namespace algorithm

namespace type_traits {

template <typename T>
struct is_alg_no_return_type : std::is_same<T, algorithm::no_return> {};

template <typename T>
inline constexpr bool is_alg_no_return_type_v = is_alg_no_return_type<T>::value;

template <typename T>
concept c_alg_return_graph = c_graph<T> or std::same_as<T, algorithm::no_return>;

template <c_alg_return_graph ReturnGraphType>
using alg_return_type =
    std::conditional_t<std::is_same_v<ReturnGraphType, algorithm::no_return>, void, ReturnGraphType>;

template <typename F>
struct is_empty_callback : std::is_same<F, algorithm::empty_callback> {};

template <typename F>
inline constexpr bool is_empty_callback_v = is_empty_callback<F>::value;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_vertex_callback =
    is_empty_callback_v<F>
    or std::convertible_to<F, algorithm::vertex_callback<GraphType, ReturnType, Args...>>;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_edge_callback =
    is_empty_callback_v<F>
    or std::convertible_to<F, algorithm::edge_callback<GraphType, ReturnType, Args...>>;

} // namespace type_traits

} // namespace gl
