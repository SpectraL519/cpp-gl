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

    [[nodiscard]] constexpr bool operator==(const incidence_item&) const noexcept = default;
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
class directed_adjacency_list {
public:
    using traits_type = GraphTraits;
    using id_type = typename traits_type::id_type;
    using item_type = incidence_item<id_type>;
    using storage_type = std::vector<std::vector<item_type>>;

    // --- degree getters ---

    [[nodiscard]] gl_attr_force_inline size_type degree(this const auto& self, id_type vertex_id) {
        return self.in_degree(vertex_id) + self.out_degree(vertex_id);
    }

    [[nodiscard]] size_type in_degree(this const auto& self, id_type vertex_id) {
        size_type in_deg = 0uz;
        for (const auto& out_edges : self._list)
            in_deg += static_cast<size_type>(
                std::ranges::count(out_edges, vertex_id, &item_type::vertex_id)
            );

        return in_deg;
    }

    [[nodiscard]] gl_attr_force_inline size_type
    out_degree(this const auto& self, id_type vertex_id) {
        return self._list[to_idx(vertex_id)].size();
    }

    [[nodiscard]] std::vector<size_type> degree_map(this const auto& self) {
        std::vector<size_type> degree_map(self._list.size(), 0uz);

        for (auto idx = 0uz; idx < self._list.size(); ++idx) {
            degree_map[idx] += self._list[idx].size();
            for (auto item : self._list[idx])
                ++degree_map[to_idx(item.vertex_id)];
        }

        return degree_map;
    }

    [[nodiscard]] std::vector<size_type> in_degree_map(this const auto& self) {
        std::vector<size_type> in_degree_map(self._list.size(), 0uz);

        for (const auto& inc_edges : self._list)
            for (auto item : inc_edges)
                ++in_degree_map[to_idx(item.vertex_id)];

        return in_degree_map;
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> out_degree_map(this const auto& self
    ) {
        return self._list | std::views::transform([](auto&& inc_edges) { return inc_edges.size(); })
             | std::ranges::to<std::vector<size_type>>();
    }

    // --- edge modifiers ---

    gl_attr_force_inline void add_edge(
        this auto& self, id_type edge_id, id_type source_id, id_type target_id
    ) {
        self._list[to_idx(source_id)].emplace_back(target_id, edge_id);
    }

    void add_edges_from(
        this auto& self,
        const traits::c_forward_range_of<id_type> auto& edge_ids,
        id_type source_id,
        const traits::c_forward_range_of<id_type> auto& target_ids
    ) {
        auto& inc_edges_source = self._list[to_idx(source_id)];
        inc_edges_source.reserve(inc_edges_source.size() + std::ranges::size(target_ids));

        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids))
            inc_edges_source.emplace_back(target_id, edge_id);
    }

    // --- comparison ---

    [[nodiscard]] friend bool
    operator==(const directed_adjacency_list&, const directed_adjacency_list&) = default;

protected:
    // --- vertex modifiers ---

    std::vector<id_type> _remove_vertex_impl(this auto& self, id_type vertex_id) {
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

        self._list.erase(self._list.begin() + to_diff(vertex_id));
        return removed_edges;
    }

    // --- edge modifiers ---

    gl_attr_force_inline void _remove_edge_impl(this auto& self, const auto& edge) {
        auto& inc_edges = self._list[to_idx(edge.source())];
        inc_edges.erase(detail::strict_find<item_type>(inc_edges, edge));
    }

    // --- edge getters ---

    [[nodiscard]] auto _in_edges_impl(this const auto& self, id_type vertex_id) {
        std::vector<item_type> in_edges;
        for (id_type src_id = initial_id; src_id < self._list.size(); ++src_id) {
            auto in_edges_view =
                self._list[to_idx(src_id)] | std::views::filter([tgt_id = vertex_id](auto item) {
                    return item.vertex_id == tgt_id;
                })
                | std::views::transform([src_id](auto item) {
                      return item_type{src_id, item.edge_id};
                  });
            in_edges.insert(in_edges.end(), in_edges_view.begin(), in_edges_view.end());
        }
        return in_edges;
    }
};

template <traits::c_list_graph_traits GraphTraits>
class undirected_adjacency_list {
public:
    using traits_type = GraphTraits;
    using id_type = typename traits_type::id_type;
    using item_type = incidence_item<id_type>;
    using storage_type = std::vector<std::vector<item_type>>;

    // --- degree getters ---

    [[nodiscard]] size_type degree(this const auto& self, id_type vertex_id) {
        size_type degree = 0uz;
        for (auto item : self._list[to_idx(vertex_id)])
            degree += 1uz + static_cast<size_type>(item.vertex_id == vertex_id);
        return degree;
    }

    [[nodiscard]] gl_attr_force_inline size_type
    in_degree(this const auto& self, id_type vertex_id) {
        return self.degree(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type
    out_degree(this const auto& self, id_type vertex_id) {
        return self.degree(vertex_id);
    }

    [[nodiscard]] std::vector<size_type> degree_map(this const auto& self) {
        std::vector<size_type> degree_map;
        degree_map.reserve(self._list.size());
        for (id_type id = initial_id; id < self._list.size(); ++id)
            degree_map.push_back(self.degree(id));
        return degree_map;
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> in_degree_map(this const auto& self) {
        return self.degree_map();
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> out_degree_map(this const auto& self
    ) {
        return self.degree_map();
    }

    // --- edge modifiers ---

    gl_attr_force_inline void add_edge(
        this auto& self, id_type edge_id, id_type source_id, id_type target_id
    ) {
        self._list[to_idx(source_id)].emplace_back(target_id, edge_id);
        if (target_id != source_id)
            self._list[to_idx(target_id)].emplace_back(source_id, edge_id);
    }

    void add_edges_from(
        this auto& self,
        const traits::c_forward_range_of<id_type> auto& edge_ids,
        id_type source_id,
        const traits::c_forward_range_of<id_type> auto& target_ids
    ) {
        auto& inc_edges_source = self._list[to_idx(source_id)];
        inc_edges_source.reserve(inc_edges_source.size() + std::ranges::size(target_ids));

        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids)) {
            inc_edges_source.emplace_back(target_id, edge_id);
            if (source_id != target_id)
                self._list[to_idx(target_id)].emplace_back(source_id, edge_id);
        }
    }

    // --- comparison ---

    [[nodiscard]] friend bool
    operator==(const undirected_adjacency_list&, const undirected_adjacency_list&) = default;

protected:
    // --- vertex modifiers ---

    std::vector<id_type> _remove_vertex_impl(this auto& self, id_type vertex_id) {
        const auto vertex_idx = to_idx(vertex_id);

        for (auto item : self._list[vertex_idx]) {
            if (item.vertex_id == vertex_id)
                continue;

            auto& inc_edges = self._list[to_idx(item.vertex_id)];
            const auto removed_subrng = std::ranges::remove_if(inc_edges, [vertex_id](auto item) {
                return item.vertex_id == vertex_id;
            });
            inc_edges.erase(removed_subrng.begin(), removed_subrng.end());
        }

        const auto removed_edges =
            self._list[vertex_idx] | std::views::transform(&item_type::edge_id)
            | std::ranges::to<std::vector>();
        self._list.erase(self._list.begin() + to_diff(vertex_id));
        return removed_edges;
    }

    // --- edge modifiers ---

    void _remove_edge_impl(this auto& self, const auto& edge) {
        auto& inc_edges_first = self._list[to_idx(edge.source())];
        auto& inc_edges_second = self._list[to_idx(edge.target())];

        inc_edges_first.erase(detail::strict_find<item_type>(inc_edges_first, edge));
        if (not edge.is_loop())
            inc_edges_second.erase(detail::strict_find<item_type>(inc_edges_second, edge));
    }

    // --- edge getters ---

    [[nodiscard]] gl_attr_force_inline auto _in_edges_impl(
        this const auto& self, id_type vertex_id
    ) {
        return std::views::all(self._list[to_idx(vertex_id)]);
    }
};

template <traits::c_adjacency_list_graph_traits GraphTraits>
struct adjacency_list_base {
    using type = void;
};

template <traits::c_adjacency_list_graph_traits GraphTraits>
using adjacency_list_base_t = typename adjacency_list_base<GraphTraits>::type;

template <traits::c_list_graph_traits GraphTraits>
struct adjacency_list_base<GraphTraits> {
    using type = std::conditional_t<
        traits::c_directed_graph_traits<GraphTraits>,
        directed_adjacency_list<GraphTraits>,
        undirected_adjacency_list<GraphTraits>>;
};

} // namespace specialized

} // namespace gl::impl
