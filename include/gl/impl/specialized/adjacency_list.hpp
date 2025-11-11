// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/decl/impl_tags.hpp"

#include <algorithm>
#include <format>
#include <iostream>
#include <ranges>
#include <vector>

namespace gl::impl {

template <type_traits::c_list_graph_traits GraphTraits>
class adjacency_list;

namespace specialized {

// adjacency list edge item
struct edge_list_item { // TODO: rename to adjacency_list_item
    types::id_type id;
    types::id_type target_id;

    [[nodiscard]] bool operator==(const edge_list_item&) const = default;
};

namespace detail {

[[nodiscard]] auto strict_find(
    type_traits::c_range_of<edge_list_item> auto& edge_list, const auto& edge
) {
    // find the edge by address
    const auto it = std::ranges::find(edge_list, edge.id(), &edge_list_item::id);
    if (it == edge_list.end())
        throw std::invalid_argument(std::format(
            "Got invalid edge [id = {} | vertices = ({}, {})]",
            edge.id(),
            edge.first(),
            edge.second()
        ));

    return it;
}

} // namespace detail

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_directed_v<typename AdjacencyList::edge_type>)
struct directed_adjacency_list {
    using impl_type = AdjacencyList;
    using edge_type = typename impl_type::edge_type;

    [[nodiscard]] static types::size_type in_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        types::size_type in_deg = constants::default_size;
        for (const auto& adjacent_edges : self._list)
            in_deg += std::ranges::count(adjacent_edges, vertex_id, &edge_list_item::target_id);

        return in_deg;
    }

    [[nodiscard]] gl_attr_force_inline static types::size_type out_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return self._list[vertex_id].size();
    }

    [[nodiscard]] gl_attr_force_inline static types::size_type degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return in_degree(self, vertex_id) + out_degree(self, vertex_id);
    }

    [[nodiscard]] static std::vector<types::size_type> in_degree_map(const impl_type& self) {
        std::vector<types::id_type> in_degree_map(self._list.size(), constants::zero);

        for (types::id_type id = constants::initial_id; id < self._list.size(); ++id) {
            std::ranges::for_each(self._list[id], [&in_degree_map](const auto& item) {
                ++in_degree_map[item.target_id];
            });
        }

        return in_degree_map;
    }

    [[nodiscard]] gl_attr_force_inline static std::vector<types::size_type> out_degree_map(
        const impl_type& self
    ) {
        return self._list
             | std::views::transform([](const auto& adj_edges) { return adj_edges.size(); })
             | std::ranges::to<std::vector<types::size_type>>();
    }

    [[nodiscard]] static std::vector<types::size_type> degree_map(const impl_type& self) {
        std::vector<types::id_type> degree_map(self._list.size(), constants::zero);

        for (types::id_type id = constants::initial_id; id < self._list.size(); ++id) {
            degree_map[id] += self._list[id].size();
            std::ranges::for_each(self._list[id], [&degree_map](const auto& item) {
                ++degree_map[item.target_id];
            });
        }

        return degree_map;
    }

    static std::vector<types::id_type> remove_vertex(
        impl_type& self, const types::id_type vertex_id
    ) {
        auto removed_edges =
            self._list[vertex_id] | std::views::transform(&edge_list_item::id)
            | std::ranges::to<std::vector>();

        // remove all edges incident to the vertex
        for (types::id_type id = constants::initial_id; id < self._list.size(); ++id) {
            auto& adj_edges = self._list[id];
            if (id == vertex_id or adj_edges.empty())
                continue;

            const auto removed_subrng =
                std::ranges::remove_if(adj_edges, [vertex_id, &removed_edges](const auto& item) {
                    if (item.target_id == vertex_id) {
                        removed_edges.push_back(item.id);
                        return true;
                    }
                    return false;
                });
            adj_edges.erase(removed_subrng.begin(), removed_subrng.end());
        }

        // remove the list of edges incident from the vertex entirely
        self._list.erase(std::next(std::begin(self._list), vertex_id));
        return removed_edges;
    }

    static gl_attr_force_inline void add_edge(
        impl_type& self, types::id_type edge_id, types::id_type source_id, types::id_type target_id
    ) {
        self._list[source_id].emplace_back(edge_id, target_id);
    }

    static void add_edges_from(
        impl_type& self,
        const type_traits::c_sized_range_of<types::id_type> auto& edge_ids,
        const types::id_type source_id,
        const type_traits::c_sized_range_of<types::id_type> auto& target_ids
    ) {
        auto& adjacent_edges_source = self._list[source_id];
        adjacent_edges_source.reserve(adjacent_edges_source.size() + target_ids.size());

        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids))
            adjacent_edges_source.emplace_back(edge_id, target_id);
    }

    static gl_attr_force_inline void remove_edge(impl_type& self, const edge_type& edge) {
        auto& adj_edges = self._list[edge.first()];
        adj_edges.erase(detail::strict_find(adj_edges, edge));
    }
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_undirected_v<typename AdjacencyList::edge_type>)
struct undirected_adjacency_list {
    using impl_type = AdjacencyList;
    using edge_type = typename impl_type::edge_type;

    [[nodiscard]] gl_attr_force_inline static types::size_type in_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return degree(self, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline static types::size_type out_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return degree(self, vertex_id);
    }

    [[nodiscard]] static types::size_type degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        types::size_type degree = constants::default_size;
        for (const auto& item : self._list[vertex_id])
            degree += constants::one + static_cast<types::size_type>(item.target_id == vertex_id);
        return degree;
    }

    [[nodiscard]] gl_attr_force_inline static std::vector<types::size_type> in_degree_map(
        const impl_type& self
    ) {
        return degree_map(self);
    }

    [[nodiscard]] gl_attr_force_inline static std::vector<types::size_type> out_degree_map(
        const impl_type& self
    ) {
        return degree_map(self);
    }

    [[nodiscard]] static std::vector<types::size_type> degree_map(const impl_type& self) {
        std::vector<types::size_type> degree_map;
        degree_map.reserve(self._list.size());
        for (types::id_type id = constants::initial_id; id < self._list.size(); ++id)
            degree_map.push_back(degree(self, id));
        return degree_map;
    }

    static std::vector<types::id_type> remove_vertex(
        impl_type& self, const types::id_type vertex_id
    ) {
        // remove all edges incident with the vertex (scan only the selected vertices)
        for (const auto& item : self._list[vertex_id]) {
            if (item.target_id == vertex_id)
                continue; // will be removed with the vertex's list

            auto& adj_edges = self._list[item.target_id];
            const auto removed_subrng = std::ranges::remove_if(
                adj_edges, [vertex_id](const auto& item) { return item.target_id == vertex_id; }
            );
            adj_edges.erase(removed_subrng.begin(), removed_subrng.end());
        }

        // remove the list of edges incident from the vertex entirely
        const auto removed_edges =
            self._list[vertex_id] | std::views::transform(&edge_list_item::id)
            | std::ranges::to<std::vector>();
        self._list.erase(std::next(std::begin(self._list), vertex_id));
        return removed_edges;
    }

    static void add_edge(
        impl_type& self, types::id_type edge_id, types::id_type source_id, types::id_type target_id
    ) {
        self._list[source_id].emplace_back(edge_id, target_id);
        if (target_id != source_id)
            self._list[target_id].emplace_back(edge_id, source_id);
    }

    static void add_edges_from(
        impl_type& self,
        const type_traits::c_sized_range_of<types::id_type> auto& edge_ids,
        const types::id_type source_id,
        const type_traits::c_sized_range_of<types::id_type> auto& target_ids
    ) {
        auto& adjacent_edges_source = self._list[source_id];
        adjacent_edges_source.reserve(adjacent_edges_source.size() + target_ids.size());

        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids)) {
            adjacent_edges_source.emplace_back(edge_id, target_id);
            if (source_id != target_id)
                self._list[target_id].emplace_back(edge_id, source_id);
        }
    }

    static void remove_edge(impl_type& self, const edge_type& edge) {
        auto& adj_edges_first = self._list[edge.first()];
        auto& adj_edges_second = self._list[edge.second()];

        if (edge.is_loop()) {
            adj_edges_first.erase(detail::strict_find(adj_edges_first, edge));
        }
        else {
            adj_edges_first.erase(detail::strict_find(adj_edges_first, edge));
            adj_edges_second.erase(detail::strict_find(adj_edges_second, edge));
        }
    }
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
struct list_impl_traits {
    using type = void;
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_directed_v<typename AdjacencyList::edge_type>)
struct list_impl_traits<AdjacencyList> {
    using type = directed_adjacency_list<AdjacencyList>;
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_undirected_v<typename AdjacencyList::edge_type>)
struct list_impl_traits<AdjacencyList> {
    using type = undirected_adjacency_list<AdjacencyList>;
};

} // namespace specialized

} // namespace gl::impl
