#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/types/iterator_range.hpp"
#include "gl/types/types.hpp"
#include "specialized/adjacency_list.hpp"

#include <vector>

namespace gl::impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_list {
public:
    using vertex_type = typename GraphTraits::vertex_type;
    using vertex_ptr_type = typename GraphTraits::vertex_ptr_type;

    using edge_type = typename GraphTraits::edge_type;
    using edge_ptr_type = typename GraphTraits::edge_ptr_type;
    using edge_directional_tag = typename GraphTraits::edge_directional_tag;

    using edge_set_type = std::vector<edge_ptr_type>;
    using edge_iterator_type = typename edge_set_type::iterator;
    using edge_const_iterator_type = typename edge_set_type::const_iterator;

    // TODO: reverese iterators should be available for bidirectional ranges

    using type = std::vector<edge_set_type>;

    adjacency_list(const adjacency_list&) = delete;
    adjacency_list& operator=(const adjacency_list&) = delete;

    adjacency_list() = default;

    adjacency_list(const types::size_type n_vertices) : _list(n_vertices) {}

    adjacency_list(adjacency_list&&) = default;
    adjacency_list& operator=(adjacency_list&&) = default;

    ~adjacency_list() = default;

    [[nodiscard]] gl_attr_force_inline types::size_type n_vertices() const {
        return this->_list.size();
    }

    [[nodiscard]] gl_attr_force_inline types::size_type n_unique_edges() const {
        return this->_n_unique_edges;
    }

    gl_attr_force_inline void add_vertex() {
        this->_list.push_back(edge_set_type{});
    }

    // clang-format off
    // gl_attr_force_inline misplacement

    gl_attr_force_inline const edge_ptr_type& add_edge(edge_ptr_type edge) {
        return specialized::add_edge(*this, std::move(edge));
    }

    // clang-format on

    gl_attr_force_inline void remove_vertex(const vertex_ptr_type& vertex) {
        specialized::remove_vertex(*this, vertex);
    }

    gl_attr_force_inline void remove_edge(const edge_ptr_type& edge) {
        specialized::remove_edge(*this, edge);
    }

    [[nodiscard]] gl_attr_force_inline types::iterator_range<edge_const_iterator_type>
    adjacent_edges(const types::id_type vertex_id) const {
        return make_const_iterator_range(this->_list.at(vertex_id));
    }

    [[nodiscard]] gl_attr_force_inline types::iterator_range<edge_iterator_type> adjacent_edges_mut(
        const types::id_type vertex_id
    ) {
        return make_iterator_range(this->_list.at(vertex_id));
    }

private:
    using specialized = typename specialized::list_impl_traits<adjacency_list>::type;
    friend specialized;

    struct address_projection {
        gl_attr_force_inline auto operator()(const edge_ptr_type& edge) const {
            return edge.get();
        }
    };

    type _list = {};
    types::size_type _n_unique_edges = 0ull;
};

} // namespace gl::impl
