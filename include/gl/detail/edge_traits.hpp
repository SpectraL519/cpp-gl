#pragma once

#include "../edge_tags.hpp"
#include "../vertex_descriptor.hpp"

namespace gl {

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_edge_directional_tag EdgeTag,
    detail::c_properties Properties>
class edge_descriptor;

namespace detail {

template <c_edge_directional_tag EdgeTag>
struct edge_traits {
    using tag_type = EdgeTag;

    template <c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, tag_type>)
    static bool is_incident_from(const EdgeType&, const typename EdgeType::vertex_type&);

    template <c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, tag_type>)
    static bool is_incident_to(const EdgeType&, const typename EdgeType::vertex_type&);
};

template <>
struct edge_traits<undirected_t> {
    using tag_type = undirected_t;

    template <c_instantiation_of<edge_descriptor> EdgeType>
    static bool is_incident_from(const EdgeType& edge, const typename EdgeType::vertex_type& vertex)
    requires(std::same_as<typename EdgeType::directional_tag, tag_type>) {
        return edge.incident_vertex(vertex) != nullptr;
    }

    template <c_instantiation_of<edge_descriptor> EdgeType>
    static bool is_incident_to(const EdgeType& edge, const typename EdgeType::vertex_type& vertex)
    requires(std::same_as<typename EdgeType::directional_tag, tag_type>) {
        return edge.incident_vertex(vertex) != nullptr;
    }
};

template <>
struct edge_traits<directed_t> {
    using tag_type = directed_t;

    template <c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, tag_type>)
    static bool is_incident_from(const EdgeType& edge, const typename EdgeType::vertex_type& vertex) {
        return *vertex == *edge._vertices.first;
    }

    template <c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, tag_type>)
    static bool is_incident_to(const EdgeType& edge, const typename EdgeType::vertex_type& vertex) {
        return *vertex == *edge._vertices.second;
    }
};

template <c_instantiation_of<edge_traits> EdgeTraits>
using edge_tag_type = typename EdgeTraits::tag_type;

} // namespace detail

} // namespace gl
