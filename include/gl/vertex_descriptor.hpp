#pragma once

#include "detail/concepts.hpp"
#include "types/types.hpp"
#include "types/default_types.hpp"
#include "edge_tags.hpp"

#include <compare>

namespace gl {

template <
    detail::c_edge_directional_tag EdgeDirectionalTag,
    detail::c_properties VertexProperties,
    detail::c_properties EdgeProperties>
struct graph_traits;

template <detail::c_instantiation_of<graph_traits> GraphTraits>
class graph;

template <detail::c_properties Properties = types::empty_properties>
class vertex_descriptor {
public:
    using type = std::type_identity_t<vertex_descriptor<Properties>>;
    using properties_type = Properties;

    template <detail::c_instantiation_of<graph_traits> GraphTraits>
    friend class graph;

    vertex_descriptor() = delete;

    explicit vertex_descriptor(const types::id_type id) : _id(id) {}

    explicit vertex_descriptor(const types::id_type id, const properties_type& properties)
    requires(not detail::is_default_properties_type_v<properties_type>)
    : _id(id), properties(properties) {}

    vertex_descriptor(const vertex_descriptor&) = default;
    vertex_descriptor(vertex_descriptor&&) = default;

    vertex_descriptor& operator=(const vertex_descriptor&) = default;
    vertex_descriptor& operator=(vertex_descriptor&&) = default;

    ~vertex_descriptor() = default;

    bool operator==(const vertex_descriptor& other) const {
        return this->_id == other._id;
    }

    std::strong_ordering operator<=>(const vertex_descriptor& other) const {
        return this->_id <=> other._id;
    }

    [[nodiscard]] inline types::id_type id() const {
        return this->_id;
    }

    properties_type properties = properties_type{};

private:
    inline void _set_id(const types::id_type id) {
        // TODO: test this method [requires: define a friend testing class in #ifdef clause]
        this->_id = id;
    }

    types::id_type _id;
};

template <detail::c_properties Properties = types::empty_properties>
using vertex = vertex_descriptor<Properties>;

} // namespace gl
