#pragma once

#include "gl/graph_traits.hpp"
#include "gl/types/type_traits.hpp"
#include "gl/types/types.hpp"

#include <vector>

namespace gl::impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_list {
public:
    using vertex_type = type_traits::vertex_type<GraphTraits>;
    using vertex_ptr_type = type_traits::vertex_ptr_type<GraphTraits>;

    using vertex_set_type = std::vector<vertex_ptr_type>;
    using vertex_iterator_type = type_traits::iterator_type<vertex_set_type>;
    using vertex_const_iterator_type = type_traits::const_iterator_type<vertex_set_type>;

    using edge_type = type_traits::edge_type<GraphTraits>;
    using edge_ptr_type = type_traits::edge_ptr_type<GraphTraits>;
    using edge_directional_tag = type_traits::edge_directional_tag<GraphTraits>;

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
