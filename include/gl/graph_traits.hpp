#pragma once

#include "edge_descriptor.hpp"
#include "gl/impl/impl_tags_decl.hpp"
#include "vertex_descriptor.hpp"

#include <memory>

namespace gl {

template <
    type_traits::c_edge_directional_tag EdgeDirectionalTag = directed_t,
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties,
    type_traits::c_graph_impl_tag ImplTag = impl::list_t>
struct graph_traits {
    using vertex_type = vertex_descriptor<VertexProperties>;
    using vertex_ptr_type = types::vertex_ptr_type<vertex_type>;
    using vertex_properties_type = typename vertex_type::properties_type;

    using edge_type = edge_descriptor<vertex_type, EdgeDirectionalTag, EdgeProperties>;
    using edge_ptr_type = types::edge_ptr_type<edge_type>;
    using edge_directional_tag = typename edge_type::directional_tag;
    using edge_properties_type = typename edge_type::properties_type;

    using implementation_tag = ImplTag;
};

template <
    type_traits::c_edge_directional_tag EdgeDirectionalTag = directed_t,
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties>
using list_graph_traits =
    graph_traits<EdgeDirectionalTag, VertexProperties, EdgeProperties, impl::list_t>;

template <
    type_traits::c_edge_directional_tag EdgeDirectionalTag = directed_t,
    type_traits::c_properties VertexProperties = types::empty_properties,
    type_traits::c_properties EdgeProperties = types::empty_properties>
using matrix_graph_traits =
    graph_traits<EdgeDirectionalTag, VertexProperties, EdgeProperties, impl::matrix_t>;

namespace type_traits {

template <typename T>
concept c_list_graph_traits =
    c_instantiation_of<T, graph_traits>
    and std::same_as<typename T::implementation_tag, impl::list_t>;

template <typename T>
concept c_matrix_graph_traits =
    c_instantiation_of<T, graph_traits>
    and std::same_as<typename T::implementation_tag, impl::matrix_t>;

} // namespace type_traits

} // namespace gl
