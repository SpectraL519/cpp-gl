#pragma once

#include "edge_descriptor.hpp"
#include "gl/impl/impl_tags_decl.hpp"
#include "types/default_types.hpp"
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
    using vertex_ptr_type = std::shared_ptr<vertex_type>;
    using vertex_properties_type = typename vertex_type::properties_type;

    using edge_type = edge_descriptor<vertex_type, EdgeDirectionalTag, EdgeProperties>;
    using edge_directional_tag = typename edge_type::directional_tag;
    using edge_ptr_type = typename edge_directional_tag::template edge_ptr_type<edge_type>;
    using edge_properties_type = typename edge_type::properties_type;

    using implementation_tag = ImplTag;
};

namespace type_traits {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
using vertex_type = typename GraphTraits::vertex_type;

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
using vertex_ptr_type = typename GraphTraits::vertex_ptr_type;

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
using vertex_properties_type = typename GraphTraits::vertex_properties_type;

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
using edge_directional_tag = typename GraphTraits::edge_directional_tag;

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
using edge_type = typename GraphTraits::edge_type;

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
using edge_ptr_type = typename GraphTraits::edge_ptr_type;

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
using edge_properties_type = typename GraphTraits::edge_properties_type;

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
using implementation_tag = typename GraphTraits::implementation_tag;

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
using implementation_type = typename implementation_tag<GraphTraits>::template type<GraphTraits>;

} // namespace type_traits

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

} // namespace gl
