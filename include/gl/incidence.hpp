#pragma once

#include "gl/attributes/force_inline.hpp"
#include "graph.hpp"

namespace gl {

// TODO: modify these methods to take the graph as parameter
// requires adding a has_vertex(vertex_ptr) method to the graph class

template <type_traits::c_instantiation_of<graph> GraphType>
[[nodiscard]] gl_attr_force_inline bool are_incident(
    const typename GraphType::vertex_ptr_type& vertex, const typename GraphType::edge_ptr_type& edge
) {
    return edge->is_incident_with(vertex);
}

template <type_traits::c_instantiation_of<graph> GraphType>
[[nodiscard]] gl_attr_force_inline bool are_incident(
    const typename GraphType::edge_ptr_type& edge, const typename GraphType::vertex_ptr_type& vertex
) {
    return edge->is_incident_with(vertex);
}

template <type_traits::c_instantiation_of<graph> GraphType>
[[nodiscard]] gl_attr_force_inline bool are_incident(
    const typename GraphType::edge_ptr_type& edge_1, const typename GraphType::edge_ptr_type& edge_2
) {
    return edge_1->is_incident_with(edge_2->first()) or edge_1->is_incident_with(edge_2->second());
}

} // namespace gl
