#pragma once

#include "gl/graph_traits.hpp"
#include "gl/types/iterator_range.hpp"
#include "gl/types/non_null_iterator.hpp"
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
    using edge_iterator_type = types::non_null_iterator<type_traits::iterator_type<edge_set_type>>;
    using edge_const_iterator_type =
        types::non_null_iterator<type_traits::const_iterator_type<edge_set_type>>;

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
        // initialize a full n x n matrix with null elements
        for (auto& row : this->_matrix) {
            row.reserve(no_vertices);
            std::generate_n(std::back_inserter(row), no_vertices, _make_null_edge);
        }
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

    void add_vertex() {
        for (auto& row : this->_matrix)
            row.push_back(_make_null_edge());
        auto& new_row = this->_matrix.emplace_back();
        new_row.reserve(this->no_vertices());
        std::generate_n(std::back_inserter(new_row), this->no_vertices(), _make_null_edge);
    }

    inline void add_edge(edge_ptr_type edge) {
        specialized::add_edge(*this, std::move(edge));
    }

    inline void remove_vertex(const vertex_ptr_type& vertex) {
        specialized::remove_vertex(*this, vertex);
    }

    inline void remove_edge(const edge_ptr_type& edge) {
        specialized::remove_edge(*this, edge);
    }

    [[nodiscard]] inline types::iterator_range<edge_iterator_type> adjacent_edges(
        const types::id_type vertex_id
    ) {
        auto& row = this->_matrix.at(vertex_id);
        return make_iterator_range(non_null_begin(row), non_null_end(row));
    }

    [[nodiscard]] inline types::iterator_range<edge_const_iterator_type> adjacent_edges_c(
        const types::id_type vertex_id
    ) const {
        const auto& row = this->_matrix.at(vertex_id);
        return make_iterator_range(non_null_cbegin(row), non_null_cend(row));
    }

private:
    static constexpr edge_ptr_type _make_null_edge() {
        return nullptr;
    }

    type _matrix = {};
    types::size_type _no_unique_edges = 0ull;
};

} // namespace gl::impl
