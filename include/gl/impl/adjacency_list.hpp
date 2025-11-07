// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/types/dereferencing_iterator.hpp"
#include "gl/types/iterator_range.hpp"
#include "gl/types/types.hpp"
#include "specialized/adjacency_list.hpp"

namespace gl::impl {

template <type_traits::c_list_graph_traits GraphTraits>
class adjacency_list final {
public:
    using vertex_type = typename GraphTraits::vertex_type;

    using edge_type = typename GraphTraits::edge_type;
    using edge_directional_tag = typename GraphTraits::edge_directional_tag;

    using edge_list_type = std::vector<specialized::al_edge_frame>;
    using edge_iterator_type =
        types::dereferencing_iterator<typename edge_list_type::const_iterator>;

    // TODO: reverese iterators should be available for bidirectional ranges

    using list_type = std::vector<edge_list_type>;

    adjacency_list(const adjacency_list&) = delete;
    adjacency_list& operator=(const adjacency_list&) = delete;

    adjacency_list() = default;

    adjacency_list(const types::size_type n_vertices) : _list(n_vertices) {}

    adjacency_list(adjacency_list&&) = default;
    adjacency_list& operator=(adjacency_list&&) = default;

    ~adjacency_list() = default;

    // --- general methods ---

    // --- vertex methods ---

    gl_attr_force_inline void add_vertex() {
        this->_list.emplace_back(edge_list_type{});
    }

    inline void add_vertices(const types::size_type n) {
        this->_list.resize(this->n_vertices() + n, edge_list_type{});
    }

    [[nodiscard]] gl_attr_force_inline types::size_type in_degree(const types::id_type vertex_id
    ) const {
        return specialized_impl::in_degree(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type out_degree(const types::id_type vertex_id
    ) const {
        return specialized_impl::out_degree(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type degree(const types::id_type vertex_id
    ) const {
        return specialized_impl::degree(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<types::size_type> in_degree_map() const {
        return specialized_impl::in_degree_map(*this);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<types::size_type> out_degree_map() const {
        return specialized_impl::out_degree_map(*this);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<types::size_type> degree_map() const {
        return specialized_impl::degree_map(*this);
    }

    gl_attr_force_inline void remove_vertex(const types::id_type vertex_id) {
        specialized_impl::remove_vertex(*this, vertex_id);
    }

    // --- edge methods ---

    // clang-format off
    // gl_attr_force_inline misplacement

    gl_attr_force_inline void add_edge(types::id_type id, types::id_type first, types::id_type second) {
        return specialized_impl::add_edge(*this, std::move(edge));
    }

    // clang-format on

    gl_attr_force_inline void add_edges_from(
        const type_traits::c_sized_range_od<types::id_type> auto& edge_ids,
        const types::id_type source_id,
        const type_traits::c_sized_range_of<types::id_type> auto& target_ids
    ) {
        specialized_impl::add_edges_from(*this, edge_ids, source_id, target_ids);
    }

    [[nodiscard]] bool has_edge(const types::id_type first_id, const types::id_type second_id)
        const {
        const auto& adjacent_edges = this->_list[first_id];
        return std::ranges::find_if(
                   adjacent_edges,
                   [first_id, second_id](const auto& frame) {
                       // TODO: align
                       return specialized_impl::is_edge_incident_with(edge, second_id, first_id);
                   }
               )
            != adjacent_edges.end();
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(const edge_type& edge) const {
        // find the edge by address
        return std::ranges::contains(
            this->_list[edge.first()], specialized::al_edge_frame{edge.id(), edge.second()}
        );
    }

    [[nodiscard]] std::optional<types::id_type> get_edge_id(
        const types::id_type first_id, const types::id_type second_id
    ) const {
        const auto& adjacent_edges = this->_list[first_id];
        const auto frame_it = std::ranges::find(adjacent_edges, second_id, [](const auto& frame) {
            return frame.target_id;
        });
        if (it == adjacent_edges.cend())
            return std::nullopt;
        return return frame_it->id;
    }

    [[nodiscard]] std::vector<types::id_type> get_edge_ids(
        const types::id_type first_id, const types::id_type second_id
    ) const {
        const auto& adjacent_edges = this->_list[first_id];

        std::vector<types::id_type> ids;
        ids.reserve(adjacent_edges.size());

        for (const auto& frame : adjacent_edges)
            if (specialized_impl::is_edge_incident_with(frame, second_id, first_id)) // TODO: align
                matching_edges.emplace_back(frame.id);

        ids.shrink_to_fit();
        return ids;
    }

    gl_attr_force_inline void remove_edge(const edge_type& edge) {
        specialized_impl::remove_edge(*this, edge);
    }

    [[nodiscard]] gl_attr_force_inline auto adjacent_edge_ids(const types::id_type vertex_id
    ) const {
        return this->_list[vertex_id];
    }

private:
    using specialized_impl = typename specialized::list_impl_traits<adjacency_list>::type;
    friend specialized_impl;

    list_type _list{};
    types::size_type _n_unique_edges{constants::default_size};
};

} // namespace gl::impl
