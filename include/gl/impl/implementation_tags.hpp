#pragma once

#include "gl/types/type_traits.hpp"

namespace gl {

namespace impl {

struct list_t;

struct matrix_t;

} // namespace impl

namespace type_traits {

template <typename T>
concept c_graph_impl_tag = c_one_of<T, impl::list_t, impl::matrix_t>;

} // namespace type_traits

template <
    type_traits::c_edge_directional_tag EdgeDirectionalTag,
    type_traits::c_properties VertexProperties,
    type_traits::c_properties EdgeProperties,
    type_traits::c_graph_impl_tag ImplTag>
struct graph_traits;

namespace impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_list;

struct list_t {
    template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
    requires(std::same_as<typename GraphTraits::implementation_tag, list_t>)
    using type = adjacency_list<GraphTraits>;
};

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_matrix;

struct matrix_t {
    template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
    requires(std::same_as<typename GraphTraits::implementation_tag, matrix_t>)
    using type = adjacency_matrix<GraphTraits>;
};

} // namespace impl

} // namespace gl
