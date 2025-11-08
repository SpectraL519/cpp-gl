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

    using edge_list_type = std::vector<specialized::edge_list_item>;
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

    [[nodiscard]] gl_attr_force_inline bool has_edge(
        const types::id_type source_id, const types::id_type target_id
    ) const {
        return std::ranges::contains(this->_list[source_id], target_id, [](const auto& item) {
            return item.target_id;
        });
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(const edge_type& edge) const {
        return std::ranges::contains(
            this->_list[edge.first()], specialized::edge_list_item{edge.id(), edge.second()}
        );
    }

    [[nodiscard]] std::optional<edge_type> get_edge(
        const types::id_type source_id, const types::id_type target_id
    ) const {
        const auto& adjacent_edges = this->_list[source_id];
        const auto item_it = std::ranges::find(adjacent_edges, target_id, [](const auto& item) {
            return item.target_id;
        });
        if (item_it == adjacent_edges.cend())
            return std::nullopt;
        return std::make_optional<edge_type>(source_id, target_id);
    }

    [[nodiscard]] std::optional<edge_type> get_edge(
        const types::id_type source_id,
        const types::id_type target_id,
        const auto& edge_properties_map
    ) const {
        const auto& adjacent_edges = this->_list[source_id];
        const auto item_it = std::ranges::find(adjacent_edges, target_id, [](const auto& item) {
            return item.target_id;
        });
        if (item_it == adjacent_edges.cend())
            return std::nullopt;
        return std::make_optional<edge_type>(
            item_it->id, source_id, target_id, edge_properties_map[item_it->id]
        );
    }

    [[nodiscard]] auto get_edges(const types::id_type source_id, const types::id_type target_id)
        const {
        return this->_list[source_id] | std::views::filter([&target_id](const auto& item) {
                   return item.target_id == target_id;
               })
             | std::views::transform([source_id](const auto& item) {
                   return edge_type{item.id, source_id, item.target_id};
               });
    }

    [[nodiscard]] auto get_edges(
        const types::id_type source_id,
        const types::id_type target_id,
        const auto& edge_properties_map
    ) const {
        return this->_list[source_id] | std::views::filter([&target_id](const auto& item) {
                   return item.target_id == target_id;
               })
             | std::views::transform([source_id, &edge_properties_map](const auto& item) {
                   return edge_type{
                       item.id, source_id, item.target_id, edge_properties_map[item.id]
                   };
               });
    }

    gl_attr_force_inline void remove_edge(const edge_type& edge) {
        specialized_impl::remove_edge(*this, edge);
    }

    [[nodiscard]] gl_attr_force_inline auto adjacent_edges(const types::id_type vertex_id) const {
        return this->_list[vertex_id] | std::views::transform([vertex_id](const auto& item) {
                   return edge_type{item.id, vertex_id, item.target_id};
               });
    }

    [[nodiscard]] gl_attr_force_inline auto adjacent_edges(
        const types::id_type vertex_id, const auto& edge_properties_map
    ) const {
        return this->_list[vertex_id]
             | std::views::transform([vertex_id, &edge_properties_map](const auto& item) {
                   return edge_type{
                       item.id, vertex_id, item.target_id, edge_properties_map[item.id]
                   };
               });
    }

private:
    using specialized_impl = typename specialized::list_impl_traits<adjacency_list>::type;
    friend specialized_impl;

    list_type _list{};
    types::size_type _n_unique_edges{constants::default_size};
};

} // namespace gl::impl
