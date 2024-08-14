#pragma once

#include "gl/graph_traits.hpp"
#include "gl/types/iterator_range.hpp"
#include "gl/types/type_traits.hpp"
#include "gl/types/types.hpp"
#include "specialized/adjacency_list.hpp"

#include <vector>

namespace gl::impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_list {
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
    using specialized = specialized::impl_type<adjacency_list>;
    friend specialized;

public:
    adjacency_list(const adjacency_list&) = delete;
    adjacency_list& operator=(const adjacency_list&) = delete;

    adjacency_list() = default;

    adjacency_list(const types::size_type no_vertices) : _list(no_vertices) {}

    adjacency_list(adjacency_list&&) = default;
    adjacency_list& operator=(adjacency_list&&) = default;

    ~adjacency_list() = default;

    [[nodiscard]] inline types::size_type no_vertices() const {
        return this->_list.size();
    }

    [[nodiscard]] inline types::size_type no_unique_edges() const {
        return this->_no_unique_edges;
    }

    inline void add_vertex() {
        this->_list.push_back(edge_set_type{});
    }

    inline void remove_vertex(const vertex_ptr_type& vertex) {
        specialized::remove_vertex(*this, vertex);
    }

    inline void add_edge(edge_ptr_type edge) {
        specialized::add_edge(*this, std::move(edge));
    }

    inline void remove_edge(const edge_ptr_type& edge) {
        specialized::template remove_edge<address_projection>(*this, edge);
    }

    [[nodiscard]] inline types::iterator_range<edge_iterator_type> adjacent_edges(
        const types::id_type vertex_id
    ) {
        return make_iterator_range(this->_list.at(vertex_id));
    }

    [[nodiscard]] inline types::iterator_range<edge_const_iterator_type> adjacent_edges_c(
        const types::id_type vertex_id
    ) const {
        const auto& adjacent_edges = this->_list.at(vertex_id);
        return make_iterator_range(adjacent_edges.cbegin(), adjacent_edges.cend());
    }

private:
    struct address_projection {
        auto operator()(const edge_ptr_type& edge) const {
            return edge.get();
        }
    };

    type _list = {};
    types::size_type _no_unique_edges = 0ull;
};

} // namespace gl::impl
