#pragma once

#include "gl/detail/concepts.hpp"
#include "gl/types/types.hpp"

#include <vector>

namespace gl::impl {

template <>
class adjacency_list {
public:
    using vertex_type = VertexType;
    using vertex_ptr_type = traits::vertex_ptr_type<traits_type>;

    using vertex_set_type = std::vector<vertex_ptr_type>;
    using vertex_iterator_type = typename vertex_set_type::iterator;
    using vertex_const_iterator_type = typename vertex_set_type::const_iterator;
    using vertex_reverse_iterator_type = typename vertex_set_type::reverse_iterator;
    using vertex_const_reverse_iterator_type = typename vertex_set_type::const_reverse_iterator;

    using edge_type = traits::edge_type<traits_type>;
    using edge_ptr_type = traits::edge_ptr_type<traits_type>;
    using edge_directional_tag = traits::edge_directional_tag<traits_type>;

    using edge_set_type = std::vector<edge_ptr_type>;
    using edge_iterator_type = typename edge_set_type::iterator;
    using edge_const_iterator_type = typename edge_set_type::const_iterator;
    using edge_reverse_iterator_type = typename edge_set_type::reverse_iterator;
    using edge_const_reverse_iterator_type = typename edge_set_type::const_reverse_iterator;

    using type = std::vector<edge_set_type>;

    adjacency_list() = default;

    adjacency_list(const types::size_type no_vertices) : _self(no_vertices) {}

    [[nodiscard]] inline types::size_type size() const {
        return this->_list.size();
    }

private:
    type _list = {};
};

} // namespace gl::impl
