// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/decl/repr_tags.hpp"
#include "gl/graph_traits.hpp"
#include "gl/traits.hpp"
#include "gl/types/core.hpp"
#include "gl/util/ranges.hpp"

#include <algorithm>
#include <cstddef>
#include <format>
#include <iostream>
#include <ranges>
#include <vector>

namespace gl::impl {

template <traits::c_adjacency_list_graph_traits GraphTraits>
class adjacency_list;

namespace specialized {

template <traits::c_id_type IdType>
struct incidence_item {
    using id_type = IdType;

    id_type vertex_id;
    id_type edge_id;

    [[nodiscard]] bool operator==(const incidence_item&) const = default;
};

namespace detail {

template <traits::c_instantiation_of<incidence_item> AdjListItem>
[[nodiscard]] auto strict_find(traits::c_range_of<AdjListItem> auto& edge_list, const auto& edge) {
    const auto it = std::ranges::find(edge_list, edge.id(), &AdjListItem::edge_id);
    if (it == edge_list.end())
        throw std::invalid_argument(std::format(
            "Got invalid edge [id = {} | vertices = ({}, {})]",
            edge.id(),
            edge.source(),
            edge.target()
        ));

    return it;
}

} // namespace detail

template <traits::c_list_graph_traits GraphTraits>
struct directed_adjacency_list {
    using traits_type = GraphTraits;
    using impl_type = adjacency_list<traits_type>;
    using id_type = typename impl_type::id_type;
    using item_type = incidence_item<id_type>;

    // --- vertex modifiers ---

    static std::vector<id_type> remove_vertex(impl_type& self, id_type vertex_id) {
        const auto vertex_idx = to_idx(vertex_id);

        auto removed_edges =
            self._list[vertex_idx] | std::views::transform(&item_type::edge_id)
            | std::ranges::to<std::vector>();

        // remove all edges incident to the vertex
        for (auto idx = 0uz; idx < self._list.size(); ++idx) {
            auto& inc_edges = self._list[idx];
            if (idx == vertex_idx or inc_edges.empty())
                continue;

            const auto removed_subrng =
                std::ranges::remove_if(inc_edges, [vertex_id, &removed_edges](auto item) {
                    if (item.vertex_id == vertex_id) {
                        removed_edges.push_back(item.edge_id);
                        return true;
                    }
                    return false;
                });
            inc_edges.erase(removed_subrng.begin(), removed_subrng.end());
        }

        // remove the list of edges incident from the vertex entirely
        self._list.erase(self._list.begin() + to_diff(vertex_id));
        return removed_edges;
    }

    // --- vertex getters ---

    [[nodiscard]] static auto neighbor_ids(const impl_type& self, id_type vertex_id) {
        return util::concat(successor_ids(self, vertex_id), predecessor_ids(self, vertex_id));
    }

    [[nodiscard]] static auto predecessor_ids(const impl_type& self, id_type vertex_id) {
        return in_edges(self, vertex_id) | std::views::transform(&item_type::vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline static auto successor_ids(
        const impl_type& self, id_type vertex_id
    ) {
        return self._list[to_idx(vertex_id)] | std::views::transform(&item_type::vertex_id);
    }

    // --- degree getters ---

    [[nodiscard]] gl_attr_force_inline static size_type degree(
        const impl_type& self, id_type vertex_id
    ) {
        return in_degree(self, vertex_id) + out_degree(self, vertex_id);
    }

    [[nodiscard]] static size_type in_degree(const impl_type& self, id_type vertex_id) {
        size_type in_deg = 0uz;
        for (const auto& out_edges : self._list)
            in_deg += static_cast<size_type>(
                std::ranges::count(out_edges, vertex_id, &item_type::vertex_id)
            );

        return in_deg;
    }

    [[nodiscard]] gl_attr_force_inline static size_type out_degree(
        const impl_type& self, id_type vertex_id
    ) {
        return self._list[to_idx(vertex_id)].size();
    }

    [[nodiscard]] static std::vector<size_type> degree_map(const impl_type& self) {
        std::vector<size_type> degree_map(self._list.size(), 0uz);

        for (auto idx = 0uz; idx < self._list.size(); ++idx) {
            degree_map[idx] += self._list[idx].size();
            std::ranges::for_each(self._list[idx], [&degree_map](auto item) {
                ++degree_map[to_idx(item.vertex_id)];
            });
        }

        return degree_map;
    }

    [[nodiscard]] static std::vector<size_type> in_degree_map(const impl_type& self) {
        std::vector<size_type> in_degree_map(self._list.size(), 0uz);

        for (const auto& inc_edges : self._list)
            for (auto item : inc_edges)
                ++in_degree_map[to_idx(item.vertex_id)];

        return in_degree_map;
    }

    [[nodiscard]] gl_attr_force_inline static std::vector<size_type> out_degree_map(
        const impl_type& self
    ) {
        return self._list
             | std::views::transform([](const auto& inc_edges) { return inc_edges.size(); })
             | std::ranges::to<std::vector<size_type>>();
    }

    // --- edge modifiers ---

    gl_attr_force_inline static void add_edge(
        impl_type& self, id_type edge_id, id_type source_id, id_type target_id
    ) {
        self._list[to_idx(source_id)].emplace_back(target_id, edge_id);
    }

    static void add_edges_from(
        impl_type& self,
        const traits::c_forward_range_of<id_type> auto& edge_ids,
        id_type source_id,
        const traits::c_forward_range_of<id_type> auto& target_ids
    ) {
        auto& inc_edges_source = self._list[to_idx(source_id)];
        inc_edges_source.reserve(inc_edges_source.size() + target_ids.size());

        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids))
            inc_edges_source.emplace_back(target_id, edge_id);
    }

    gl_attr_force_inline static void remove_edge(impl_type& self, const auto& edge) {
        auto& inc_edges = self._list[to_idx(edge.source())];
        inc_edges.erase(detail::strict_find<item_type>(inc_edges, edge));
    }

    // --- edge getters ---

    template <typename EdgeType = typename impl_type::traits_type::edge_type>
    [[nodiscard]] gl_attr_force_inline static auto incident_edges(
        const impl_type& self, id_type vertex_id
    ) {
        return util::concat(
            self.template in_edges<EdgeType>(vertex_id),
            self.template out_edges<EdgeType>(vertex_id)
        );
    }

    template <typename EdgeType = typename impl_type::traits_type::edge_type>
    [[nodiscard]] gl_attr_force_inline static auto incident_edges(
        const impl_type& self, id_type vertex_id, auto& edge_properties_map
    ) {
        return util::concat(
            self.template in_edges<EdgeType>(vertex_id, edge_properties_map),
            self.template out_edges<EdgeType>(vertex_id, edge_properties_map)
        );
    }

    [[nodiscard]] static auto in_edges(const impl_type& self, id_type vertex_id) {
        std::vector<item_type> in_edges;
        for (id_type src_id = initial_id; src_id < self._list.size(); ++src_id) {
            auto in_edges_view =
                self._list[to_idx(src_id)] | std::views::filter([tgt_id = vertex_id](auto item) {
                    return item.vertex_id == tgt_id;
                })
                | std::views::transform([src_id](auto item) {
                      return incidence_item{src_id, item.edge_id};
                  });
            in_edges.insert(in_edges.end(), in_edges_view.begin(), in_edges_view.end());
        }
        return in_edges;
    }
};

template <traits::c_list_graph_traits GraphTraits>
struct undirected_adjacency_list {
    using traits_type = GraphTraits;
    using impl_type = adjacency_list<traits_type>;
    using id_type = typename impl_type::id_type;
    using item_type = incidence_item<id_type>;

    // --- vertex modifiers ---

    static std::vector<id_type> remove_vertex(impl_type& self, id_type vertex_id) {
        const auto vertex_idx = to_idx(vertex_id);

        // remove all edges incident with the vertex (scan only the selected vertices)
        for (auto item : self._list[vertex_idx]) {
            if (item.vertex_id == vertex_id)
                continue; // will be removed with the vertex's list

            auto& inc_edges = self._list[to_idx(item.vertex_id)];
            const auto removed_subrng = std::ranges::remove_if(inc_edges, [vertex_id](auto item) {
                return item.vertex_id == vertex_id;
            });
            inc_edges.erase(removed_subrng.begin(), removed_subrng.end());
        }

        // remove the list of edges incident from the vertex entirely
        const auto removed_edges =
            self._list[vertex_idx] | std::views::transform(&item_type::edge_id)
            | std::ranges::to<std::vector>();
        self._list.erase(self._list.begin() + to_diff(vertex_id));
        return removed_edges;
    }

    // --- vertex getters ---

    [[nodiscard]] gl_attr_force_inline static auto neighbor_ids(
        const impl_type& self, id_type vertex_id
    ) {
        return self._list[to_idx(vertex_id)] | std::views::transform(&item_type::vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline static auto predecessor_ids(
        const impl_type& self, id_type vertex_id
    ) {
        return neighbor_ids(self, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline static auto successor_ids(
        const impl_type& self, id_type vertex_id
    ) {
        return neighbor_ids(self, vertex_id);
    }

    // --- degree getters ---

    [[nodiscard]] static size_type degree(const impl_type& self, id_type vertex_id) {
        size_type degree = 0uz;
        for (auto item : self._list[to_idx(vertex_id)])
            degree += 1uz + static_cast<size_type>(item.vertex_id == vertex_id);
        return degree;
    }

    [[nodiscard]] gl_attr_force_inline static size_type in_degree(
        const impl_type& self, id_type vertex_id
    ) {
        return degree(self, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline static size_type out_degree(
        const impl_type& self, id_type vertex_id
    ) {
        return degree(self, vertex_id);
    }

    [[nodiscard]] static std::vector<size_type> degree_map(const impl_type& self) {
        std::vector<size_type> degree_map;
        degree_map.reserve(self._list.size());
        for (id_type id = initial_id; id < self._list.size(); ++id)
            degree_map.push_back(degree(self, id));
        return degree_map;
    }

    [[nodiscard]] gl_attr_force_inline static std::vector<size_type> in_degree_map(
        const impl_type& self
    ) {
        return degree_map(self);
    }

    [[nodiscard]] gl_attr_force_inline static std::vector<size_type> out_degree_map(
        const impl_type& self
    ) {
        return degree_map(self);
    }

    // --- edge modifiers ---

    static void add_edge(impl_type& self, id_type edge_id, id_type source_id, id_type target_id) {
        self._list[to_idx(source_id)].emplace_back(target_id, edge_id);
        if (target_id != source_id)
            self._list[to_idx(target_id)].emplace_back(source_id, edge_id);
    }

    static void add_edges_from(
        impl_type& self,
        const traits::c_forward_range_of<id_type> auto& edge_ids,
        id_type source_id,
        const traits::c_forward_range_of<id_type> auto& target_ids
    ) {
        auto& inc_edges_source = self._list[to_idx(source_id)];
        inc_edges_source.reserve(inc_edges_source.size() + target_ids.size());

        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids)) {
            inc_edges_source.emplace_back(target_id, edge_id);
            if (source_id != target_id)
                self._list[to_idx(target_id)].emplace_back(source_id, edge_id);
        }
    }

    static void remove_edge(impl_type& self, const auto& edge) {
        auto& inc_edges_first = self._list[to_idx(edge.source())];
        auto& inc_edges_second = self._list[to_idx(edge.target())];

        inc_edges_first.erase(detail::strict_find<item_type>(inc_edges_first, edge));
        if (not edge.is_loop())
            inc_edges_second.erase(detail::strict_find<item_type>(inc_edges_second, edge));
    }

    // --- edge getters ---

    template <typename EdgeType = typename impl_type::traits_type::edge_type>
    [[nodiscard]] gl_attr_force_inline static auto incident_edges(
        const impl_type& self, id_type vertex_id
    ) {
        return self.template out_edges<EdgeType>(vertex_id);
    }

    template <typename EdgeType = typename impl_type::traits_type::edge_type>
    [[nodiscard]] gl_attr_force_inline static auto incident_edges(
        const impl_type& self, id_type vertex_id, auto& edge_properties_map
    ) {
        return self.template out_edges<EdgeType>(vertex_id, edge_properties_map);
    }

    [[nodiscard]] gl_attr_force_inline static auto in_edges(
        const impl_type& self, id_type vertex_id
    ) {
        return std::views::all(self._list[to_idx(vertex_id)]);
    }
};

template <traits::c_adjacency_list_graph_traits GraphTraits>
struct adjacency_list_impl_traits {
    using type = void;

    template <typename ItemType>
    using storage_type = void;
};

template <traits::c_list_graph_traits GraphTraits>
struct adjacency_list_impl_traits<GraphTraits> {
    using type = std::conditional_t<
        traits::c_directed_graph_traits<GraphTraits>,
        directed_adjacency_list<GraphTraits>,
        undirected_adjacency_list<GraphTraits>>;

    template <typename ItemType>
    using storage_type = std::vector<std::vector<ItemType>>;
};

} // namespace specialized

} // namespace gl::impl
