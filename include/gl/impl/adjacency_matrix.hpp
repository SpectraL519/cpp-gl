#pragma once

#include "gl/graph_traits.hpp"
#include "gl/types/iterator_range.hpp"
#include "gl/types/type_traits.hpp"
#include "gl/types/types.hpp"
#include "specialized/adjacency_matrix.hpp"

#include <vector>

namespace gl::impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_matrix {
public:
    using vertex_type = type_traits::vertex_type<GraphTraits>;
    using vertex_ptr_type = type_traits::vertex_ptr_type<GraphTraits>;

    using edge_type = type_traits::edge_type<GraphTraits>;
    using edge_ptr_type = type_traits::edge_ptr_type<GraphTraits>;
    using edge_directional_tag = type_traits::edge_directional_tag<GraphTraits>;

    using edge_set_type = std::vector<edge_ptr_type>;
    using edge_iterator_type = typename edge_set_type::iterator;
    using edge_const_iterator_type = typename edge_set_type::const_iterator;

    // TODO: reverese iterators should be available for bidirectional ranges

    using type = std::vector<edge_set_type>;

private:
    using specialized = specialized::impl_type<adjacency_matrix>;
    friend specialized;

public:
    adjacency_matrix(const adjacency_matrix&) = delete;
    adjacency_matrix& operator=(const adjacency_matrix&) = delete;

    adjacency_matrix() = default;

    adjacency_matrix(const types::size_type no_vertices) : _matrix(no_vertices) {
        specialized::construct(*this, no_vertices);
    }

    adjacency_matrix(adjacency_matrix&&) = default;
    adjacency_matrix& operator=(adjacency_matrix&&) = default;

    ~adjacency_matrix() = default;

    [[nodiscard]] inline types::size_type no_vertices() const {
        return this->_matrix.size();
    }

    [[nodiscard]] inline types::size_type no_unique_edges() const {
        return this->_no_unique_edges;
    }

private:
    type _matrix = {};
    types::size_type _no_unique_edges = 0ull;
};

} // namespace gl::impl
