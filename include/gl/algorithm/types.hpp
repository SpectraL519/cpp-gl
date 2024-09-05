#pragma once

#include "gl/graph.hpp"

#include <functional>
#include <vector>

namespace gl {

namespace algorithm {

struct no_return {};

struct empty_callback {};

template <type_traits::c_instantiation_of<vertex_descriptor> VertexType>
using vertex_callback = std::function<void(const VertexType&)>;

template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
using edge_callback = std::function<void(const EdgeType&)>;

using visited_list = std::vector<bool>;

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

template <typename F, typename VertexType>
concept c_vertex_callback =
    c_one_of<F, algorithm::empty_callback, algorithm::vertex_callback<VertexType>>;

template <typename F, typename EdgeType>
concept c_edge_callback =
    c_one_of<F, algorithm::empty_callback, algorithm::edge_callback<EdgeType>>;

} // namespace type_traits

} // namespace gl
