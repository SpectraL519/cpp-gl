#pragma once

#include "gl/types/types.hpp"
#include "gl/types/type_traits.hpp"
#include "gl/edge_descriptor.hpp"

#include <vector>

namespace gl::impl {

// TODO: should be parametrized with graph_traits
template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
class adjacency_list {
public:
    using vertex_type = typename EdgeType::vertex_type;
    using vertex_ptr_type = typename EdgeType::vertex_ptr_type;

    using vertex_set_type = std::vector<vertex_ptr_type>;
    using vertex_iterator_type = type_traits::iterator_type<vertex_set_type>;
    using vertex_const_iterator_type = type_traits::const_iterator_type<vertex_set_type>;

    using edge_type = EdgeType;
    using edge_directional_tag = typename edge_type::directional_tag;
    using edge_ptr_type = typename edge_directional_tag::template edge_ptr_type<edge_type>;

    using edge_set_type = std::vector<edge_ptr_type>;
    using edge_iterator_type = typename edge_set_type::iterator;
    using edge_const_iterator_type = typename edge_set_type::const_iterator;

    using type = std::vector<edge_set_type>;

    adjacency_list() = default;

    adjacency_list(const types::size_type no_vertices) : _list(no_vertices) {}

    [[nodiscard]] inline types::size_type size() const {
        return this->_list.size();
    }

private:
    type _list = {};
};

} // namespace gl::impl
