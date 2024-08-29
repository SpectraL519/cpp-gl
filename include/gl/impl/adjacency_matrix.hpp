#pragma once

#include "gl/types/dereferencing_iterator.hpp"
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

    using edge_type = typename GraphTraits::edge_type;
    using edge_ptr_type = typename GraphTraits::edge_ptr_type;
    using edge_directional_tag = typename GraphTraits::edge_directional_tag;

    using edge_set_type = std::vector<edge_ptr_type>;
    using edge_iterator_type = types::dereferencing_iterator<
        types::non_null_iterator<typename edge_set_type::const_iterator>>;

    // TODO: reverese iterators should be available for bidirectional ranges

    using type = std::vector<edge_set_type>;

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

    // --- general methods ---

    [[nodiscard]] gl_attr_force_inline types::size_type n_vertices() const {
        return this->_matrix.size();
    }

    [[nodiscard]] gl_attr_force_inline types::size_type n_unique_edges() const {
        return this->_n_unique_edges;
    }

    // --- vertex methods ---

    void add_vertex() {
        for (auto& row : this->_matrix)
            row.push_back(_make_null_edge());
        auto& new_row = this->_matrix.emplace_back();
        new_row.reserve(this->n_vertices());
        std::generate_n(std::back_inserter(new_row), this->n_vertices(), _make_null_edge);
    }

    gl_attr_force_inline void remove_vertex(const vertex_type& vertex) {
        sepcialized_impl::remove_vertex(*this, vertex);
    }

    // --- edge methods ---

    // clang-format off
    // gl_attr_force_inline misplacement

    gl_attr_force_inline const edge_type& add_edge(edge_ptr_type edge) {
        return sepcialized_impl::add_edge(*this, std::move(edge));
    }

    // clang-format on

    [[nodiscard]] gl_attr_force_inline bool has_edge(
        const types::id_type first_id, const types::id_type second_id
    ) const {
        return this->_matrix[first_id][second_id] != nullptr;
    }

    [[nodiscard]] bool has_edge(const edge_type& edge) const {
        const auto first_id = edge.first().id();
        const auto second_id = edge.second().id();

        if (not (this->_is_valid_vertex_id(first_id) and this->_is_valid_vertex_id(second_id)))
            return false;

        const auto& matrix_element = this->_matrix[first_id][second_id];
        return matrix_element != nullptr and &edge == matrix_element.get();
    }

    [[nodiscard]] types::optional_ref<const edge_type> get_edge(
        const types::id_type first_id, const types::id_type second_id
    ) const {
        if (not (this->_is_valid_vertex_id(first_id) and this->_is_valid_vertex_id(second_id)))
            return std::nullopt;

        const auto& matrix_element = this->_matrix[first_id][second_id];
        if (not matrix_element)
            return std::nullopt;
        return std::cref(*matrix_element);
    }

    gl_attr_force_inline void remove_edge(const edge_type& edge) {
        sepcialized_impl::remove_edge(*this, edge);
    }

    [[nodiscard]] inline types::iterator_range<edge_iterator_type> adjacent_edges(
        const types::id_type vertex_id
    ) const {
        const auto& row = this->_matrix[vertex_id];
        return make_iterator_range(
            types::dereferencing_iterator(non_null_cbegin(row)),
            types::dereferencing_iterator(non_null_cend(row))
        );
    }

private:
    using sepcialized_impl = typename specialized::matrix_impl_traits<adjacency_matrix>::type;
    friend sepcialized_impl;

    static constexpr edge_ptr_type _make_null_edge() {
        return nullptr;
    }

    [[nodiscard]] gl_attr_force_inline bool _is_valid_vertex_id(const types::id_type vertex_id
    ) const {
        return vertex_id < this->_matrix.size();
    }

    type _matrix{};
    types::size_type _n_unique_edges{0ull};
};

} // namespace gl::impl
