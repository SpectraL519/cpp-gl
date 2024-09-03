#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/constants.hpp"
#include "gl/types/dereferencing_iterator.hpp"
#include "gl/types/iterator_range.hpp"
#include "gl/types/types.hpp"
#include "specialized/adjacency_list.hpp"

#include <iostream>
#include <vector>

namespace gl::impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_list {
public:
    using vertex_type = typename GraphTraits::vertex_type;

    using edge_type = typename GraphTraits::edge_type;
    using edge_ptr_type = typename GraphTraits::edge_ptr_type;
    using edge_directional_tag = typename GraphTraits::edge_directional_tag;

    using edge_list_type = std::vector<edge_ptr_type>;
    using edge_iterator_type =
        types::dereferencing_iterator<typename edge_list_type::const_iterator>;

    // TODO: reverese iterators should be available for bidirectional ranges

    using type = std::vector<edge_list_type>;

    adjacency_list(const adjacency_list&) = delete;
    adjacency_list& operator=(const adjacency_list&) = delete;

    adjacency_list() = default;

    adjacency_list(const types::size_type n_vertices) : _list(n_vertices) {}

    adjacency_list(adjacency_list&&) = default;
    adjacency_list& operator=(adjacency_list&&) = default;

    ~adjacency_list() = default;

    // --- general methods ---

    [[nodiscard]] gl_attr_force_inline types::size_type n_vertices() const {
        return this->_list.size();
    }

    [[nodiscard]] gl_attr_force_inline types::size_type n_unique_edges() const {
        return this->_n_unique_edges;
    }

    // --- vertex methods ---

    gl_attr_force_inline void add_vertex() {
        this->_list.push_back(edge_list_type{});
    }

    inline void add_vertices(const types::size_type n) {
        this->_list.reserve(this->n_vertices() + n);
        for (types::size_type _ = constants::begin_idx; _ < n; _++)
            this->_list.push_back(edge_list_type{});
    }

    gl_attr_force_inline void remove_vertex(const vertex_type& vertex) {
        specialized_impl::remove_vertex(*this, vertex);
    }

    // --- edge methods ---

    // clang-format off
    // gl_attr_force_inline misplacement

    gl_attr_force_inline const edge_type& add_edge(edge_ptr_type edge) {
        return specialized_impl::add_edge(*this, std::move(edge));
    }

    // clang-format on

    gl_attr_force_inline void add_edges_from(
        const types::id_type source_id, std::vector<edge_ptr_type> new_edges
    ) {
        specialized_impl::add_edges_from(*this, source_id, std::move(new_edges));
    }

    [[nodiscard]] bool has_edge(const types::id_type first_id, const types::id_type second_id)
        const {
        if (not (this->_is_valid_vertex_id(first_id) and this->_is_valid_vertex_id(second_id)))
            return false;

        const auto& adjacent_edges = this->_list[first_id];
        return std::ranges::find_if(
                   adjacent_edges,
                   [first_id, second_id](const auto& edge) {
                       return specialized_impl::is_edge_incident_to(edge, second_id, first_id);
                   }
               )
            != adjacent_edges.end();
    }

    [[nodiscard]] bool has_edge(const edge_type& edge) const {
        const auto first_id = edge.first().id();
        if (not (
                this->_is_valid_vertex_id(first_id)
                and this->_is_valid_vertex_id(edge.second().id())
            ))
            return false;

        const auto& adjacent_edges = this->_list[first_id];
        return std::ranges::find(
                   adjacent_edges, &edge, typename specialized_impl::address_projection{}
               )
            != adjacent_edges.end();
    }

    [[nodiscard]] types::optional_ref<const edge_type> get_edge(
        const types::id_type first_id, const types::id_type second_id
    ) const {
        if (not (this->_is_valid_vertex_id(first_id) and this->_is_valid_vertex_id(second_id)))
            return std::nullopt;

        const auto& adjacent_edges = this->_list[first_id];
        const auto it =
            std::ranges::find_if(adjacent_edges, [first_id, second_id](const auto& edge) {
                return specialized_impl::is_edge_incident_to(edge, second_id, first_id);
            });

        if (it == adjacent_edges.cend())
            return std::nullopt;
        return std::cref(**it);
    }

    [[nodiscard]] auto get_edges(const types::id_type first_id, const types::id_type second_id)
        const {
        using edge_ref_set = std::vector<types::const_ref_wrap<edge_type>>;

        if (not (this->_is_valid_vertex_id(first_id) and this->_is_valid_vertex_id(second_id)))
            return edge_ref_set{};

        const auto& adjacent_edges = this->_list[first_id];

        edge_ref_set matching_edges{};
        matching_edges.reserve(adjacent_edges.size());

        for (const auto& edge : adjacent_edges)
            if (specialized_impl::is_edge_incident_to(edge, second_id, first_id))
                matching_edges.push_back(std::cref(*edge));

        matching_edges.shrink_to_fit();
        return matching_edges;
    }

    gl_attr_force_inline void remove_edge(const edge_type& edge) {
        specialized_impl::remove_edge(*this, edge);
    }

    [[nodiscard]] gl_attr_force_inline types::iterator_range<edge_iterator_type> adjacent_edges(
        const types::id_type vertex_id
    ) const {
        const auto& adjacent_edges = this->_list[vertex_id];
        return make_iterator_range(deref_cbegin(adjacent_edges), deref_cend(adjacent_edges));
    }

private:
    using specialized_impl = typename specialized::list_impl_traits<adjacency_list>::type;
    friend specialized_impl;

    [[nodiscard]] gl_attr_force_inline bool _is_valid_vertex_id(const types::id_type vertex_id
    ) const {
        return vertex_id < this->_list.size();
    }

    type _list{};
    types::size_type _n_unique_edges{constants::default_size};
};

} // namespace gl::impl
