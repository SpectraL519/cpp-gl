#pragma once

#include "attributes/force_inline.hpp"
#include "decl/graph_traits.hpp"
#include "edge_tags.hpp"
#include "graph_io.hpp"
#include "types/properties.hpp"
#include "types/type_traits.hpp"
#include "types/types.hpp"

#include <compare>
#include <format>

namespace gl {

template <type_traits::c_properties Properties = types::empty_properties>
class vertex_descriptor {
public:
    using type = std::type_identity_t<vertex_descriptor<Properties>>;
    using properties_type = Properties;

    template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
    friend class graph;

    vertex_descriptor() = delete;
    vertex_descriptor(const vertex_descriptor&) = delete;
    vertex_descriptor& operator=(const vertex_descriptor&) = delete;

    explicit vertex_descriptor(const types::id_type id) : _id(id) {}

    explicit vertex_descriptor(const types::id_type id, const properties_type& properties)
    requires(not type_traits::is_default_properties_type_v<properties_type>)
    : _id(id), properties(properties) {}

    vertex_descriptor(vertex_descriptor&&) = default;
    vertex_descriptor& operator=(vertex_descriptor&&) = default;

    ~vertex_descriptor() = default;

    gl_attr_force_inline bool operator==(const vertex_descriptor& other) const {
        return this->_id == other._id;
    }

    gl_attr_force_inline std::strong_ordering operator<=>(const vertex_descriptor& other) const {
        return this->_id <=> other._id;
    }

    [[nodiscard]] gl_attr_force_inline types::id_type id() const {
        return this->_id;
    }

    [[no_unique_address]] mutable properties_type properties{};

    friend inline std::ostream& operator<<(std::ostream& os, const vertex_descriptor& vertex) {
        vertex._write(os);
        return os;
    }

private:
    void _write(std::ostream& os) const {
        if constexpr (not type_traits::c_writable<properties_type>) {
            this->_write_no_properties(os);
            return;
        }
        else {
            if (not io::is_option_set(os, io::graph_option::with_vertex_properties)) {
                this->_write_no_properties(os);
                return;
            }

            if (io::is_option_set(os, io::graph_option::verbose)) {
                os << "[id: " << this->_id << " | properties: " << this->properties << "]";
            }
            else {
                os << "[" << this->_id << " | " << this->properties << "]";
            }
        }
    }

    void _write_no_properties(std::ostream& os) const {
        if (io::is_option_set(os, io::graph_option::verbose)) {
            os << std::format("[id: {}]", this->_id);
        }
        else {
            os << this->_id;
        }
    }

    types::id_type _id;
};

template <type_traits::c_properties Properties = types::empty_properties>
using vertex = vertex_descriptor<Properties>;

namespace types {

template <type_traits::c_instantiation_of<vertex_descriptor> VertexType>
using vertex_ptr_type = std::unique_ptr<VertexType>;

} // namespace types

namespace detail {

template <type_traits::c_instantiation_of<vertex_descriptor> VertexType>
[[nodiscard]] gl_attr_force_inline types::vertex_ptr_type<VertexType> make_vertex(
    const types::id_type id
) {
    return std::make_unique<VertexType>(id);
}

template <type_traits::c_instantiation_of<vertex_descriptor> VertexType>
[[nodiscard]] gl_attr_force_inline types::vertex_ptr_type<VertexType> make_vertex(
    const types::id_type id, const typename VertexType::properties_type& properties
) {
    return std::make_unique<VertexType>(id, properties);
}

} // namespace detail

} // namespace gl
