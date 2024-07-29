#pragma once

#include "detail/concepts.hpp"

#include <memory>

namespace gl {

struct directed_t;

struct undirected_t;

namespace detail {

template <typename T>
concept c_edge_directional_tag = c_one_of<T, directed_t, undirected_t>;

} // namespace detail

template <detail::c_properties Properties>
class vertex_descriptor;

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_edge_directional_tag EdgeTag,
    detail::c_properties Properties>
class edge_descriptor;

struct directed_t {
    using type = std::type_identity_t<directed_t>;

    template <detail::c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, type>)
    using edge_ptr_type = std::unique_ptr<EdgeType>;

    template <detail::c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, type>)
    static bool is_incident_from(
        const EdgeType& edge, const typename EdgeType::vertex_ptr_type& vertex
    ) {
        return *vertex == *edge._vertices.first;
    }

    template <detail::c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, type>)
    static bool is_incident_to(
        const EdgeType& edge, const typename EdgeType::vertex_ptr_type& vertex
    ) {
        return *vertex == *edge._vertices.second;
    }
};

struct undirected_t {
    using type = std::type_identity_t<undirected_t>;

    template <detail::c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, type>)
    using edge_ptr_type = std::shared_ptr<EdgeType>;

    template <detail::c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, type>)
    static bool is_incident_from(
        const EdgeType& edge, const typename EdgeType::vertex_ptr_type& vertex
    ) {
        return edge.incident_vertex(vertex) != nullptr;
    }

    template <detail::c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, type>)
    static bool is_incident_to(
        const EdgeType& edge, const typename EdgeType::vertex_ptr_type& vertex
    ) {
        return edge.incident_vertex(vertex) != nullptr;
    }
};

} // namespace gl
