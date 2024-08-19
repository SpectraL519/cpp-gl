#pragma once

#include "gl/types/iterator_range.hpp"
#include "gl/types/non_null_iterator.hpp"
#include "gl/types/types.hpp"
#include "specialized/adjacency_matrix.hpp"

#include <vector>

namespace gl::impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_matrix {
public:
    using vertex_type = typename GraphTraits::vertex_type;
    using vertex_ptr_type = typename GraphTraits::vertex_ptr_type;

    using edge_type = typename GraphTraits::edge_type;
    using edge_ptr_type = typename GraphTraits::edge_ptr_type;
    using edge_directional_tag = typename GraphTraits::edge_directional_tag;

    using edge_set_type = std::vector<edge_ptr_type>;
    using edge_iterator_type = types::non_null_iterator<type_traits::iterator_type<edge_set_type>>;
    using edge_const_iterator_type =
        types::non_null_iterator<type_traits::const_iterator_type<edge_set_type>>;

    // TODO: reverese iterators should be available for bidirectional ranges

    using type = std::vector<edge_set_type>;

private:
    using specialized = typename specialized::matrix_impl_traits<adjacency_matrix>::type;
    friend specialized;

public:
    adjacency_matrix(const adjacency_matrix&) = delete;
    adjacency_matrix& operator=(const adjacency_matrix&) = delete;

    adjacency_matrix() = default;

    adjacency_matrix(const types::size_type n_vertices) : _matrix(n_vertices) {
        // initialize a full n x n matrix with null elements
        for (auto& row : this->_matrix) {
            row.reserve(n_vertices);
            std::generate_n(std::back_inserter(row), n_vertices, _make_null_edge);
        }
    }

    adjacency_matrix(adjacency_matrix&&) = default;
    adjacency_matrix& operator=(adjacency_matrix&&) = default;

    ~adjacency_matrix() = default;

    [[nodiscard]] inline types::size_type n_vertices() const {
        return this->_matrix.size();
    }

    [[nodiscard]] inline types::size_type n_unique_edges() const {
        return this->_n_unique_edges;
    }

    void add_vertex() {
        for (auto& row : this->_matrix)
            row.push_back(_make_null_edge());
        auto& new_row = this->_matrix.emplace_back();
        new_row.reserve(this->n_vertices());
        std::generate_n(std::back_inserter(new_row), this->n_vertices(), _make_null_edge);
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
    types::size_type _n_unique_edges = 0ull;
};

} // namespace gl::impl
