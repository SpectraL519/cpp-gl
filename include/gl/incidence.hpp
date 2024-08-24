#pragma once

#include "gl/attributes/force_inline.hpp"
#include "graph.hpp"

namespace gl {

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

} // namespace gl
