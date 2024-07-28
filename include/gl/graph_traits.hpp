#pragma once

#include "detail/concepts.hpp"
#include "detail/default_types.hpp"
#include "edge_descriptor.hpp"
#include "edge_tags.hpp"
#include "vertex_descriptor.hpp"

#include <memory>

namespace gl {

template <
    detail::c_edge_directional_tag EdgeDirectionalTag = directed_t,
    detail::c_properties VertexProperties = detail::empty_properties,
    detail::c_properties EdgeProperties = detail::empty_properties>
struct graph_traits {
    using vertex_type = vertex_descriptor<VertexProperties>;
    using vertex_ptr_type = std::shared_ptr<vertex_type>;
    using vertex_properties_type = typename vertex_type::properties_type;

    using edge_directional_tag = EdgeDirectionalTag;
    using edge_type = edge_descriptor<vertex_type, edge_directional_tag, EdgeProperties>;
    using edge_ptr_type = typename edge_directional_tag::template edge_ptr_type<edge_type>;
    using edge_properties_type = typename edge_type::properties_type;
};

namespace traits {

template <detail::c_instantiation_of<graph_traits> GraphTraits>
using vertex_type = typename GraphTraits::vertex_type;

template <detail::c_instantiation_of<graph_traits> GraphTraits>
using vertex_ptr_type = typename GraphTraits::vertex_ptr_type;

template <detail::c_instantiation_of<graph_traits> GraphTraits>
using vertex_properties_type = typename GraphTraits::vertex_properties_type;

template <detail::c_instantiation_of<graph_traits> GraphTraits>
using edge_directional_tag = typename GraphTraits::edge_directional_tag;

template <detail::c_instantiation_of<graph_traits> GraphTraits>
using edge_type = typename GraphTraits::edge_type;

template <detail::c_instantiation_of<graph_traits> GraphTraits>
using edge_ptr_type = typename GraphTraits::edge_ptr_type;

template <detail::c_instantiation_of<graph_traits> GraphTraits>
using edge_properties_type = typename GraphTraits::edge_properties_type;

} // namespace traits

} // namespace gl
