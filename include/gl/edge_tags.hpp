#pragma once

#include "types/type_traits.hpp"

#include <memory>

namespace gl {

struct directed_t;

struct undirected_t;

namespace types::traits {

template <typename T>
concept c_edge_directional_tag = c_one_of<T, directed_t, undirected_t>;

} // namespace types::traits

template <type_traits::c_properties Properties>
class vertex_descriptor;

template <
    type_traits::c_instantiation_of<vertex_descriptor> VertexType,
    type_traits::c_edge_directional_tag EdgeTag,
    type_traits::c_properties Properties>
class edge_descriptor;

struct directed_t {
    using type = std::type_identity_t<directed_t>;

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, type>)
    using edge_ptr_type = std::unique_ptr<EdgeType>;

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, type>)
    static bool is_incident_from(
        const EdgeType& edge, const typename EdgeType::vertex_ptr_type& vertex
    ) {
        return *vertex == *edge._vertices.first;
    }

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, type>)
    static bool is_incident_to(
        const EdgeType& edge, const typename EdgeType::vertex_ptr_type& vertex
    ) {
        return *vertex == *edge._vertices.second;
    }
};

struct undirected_t {
    using type = std::type_identity_t<undirected_t>;

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, type>)
    using edge_ptr_type = std::shared_ptr<EdgeType>;

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, type>)
    static bool is_incident_from(
        const EdgeType& edge, const typename EdgeType::vertex_ptr_type& vertex
    ) {
        return edge.incident_vertex(vertex) != nullptr;
    }

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(std::same_as<typename EdgeType::directional_tag, type>)
    static bool is_incident_to(
        const EdgeType& edge, const typename EdgeType::vertex_ptr_type& vertex
    ) {
        return edge.incident_vertex(vertex) != nullptr;
    }
};

} // namespace gl
