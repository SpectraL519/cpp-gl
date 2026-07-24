// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/decl/repr_tags.hpp"
#include "gl/graph_traits.hpp"
#include "gl/impl/specialized/adjacency_list.hpp"
#include "gl/types/flat_jagged_vector.hpp"

#include <algorithm>
#include <format>
#include <ranges>
#include <vector>

namespace gl::impl::specialized {

template <traits::c_flat_list_graph_traits GraphTraits>
class directed_flat_adjacency_list {
public:
    using traits_type = GraphTraits;
    using id_type = typename traits_type::id_type;
    using item_type = incidence_item<id_type>;

    // --- degree getters ---

    [[nodiscard]] gl_attr_force_inline size_type degree(this auto&& self, id_type vertex_id) {
        return self.in_degree(vertex_id) + self.out_degree(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type in_degree(this auto&& self, id_type vertex_id) {
        return static_cast<size_type>(
            std::ranges::count(self._list.data_view(), vertex_id, &item_type::vertex_id)
        );
    }

    [[nodiscard]] gl_attr_force_inline size_type out_degree(this auto&& self, id_type vertex_id) {
        return self._list.segment_size(to_idx(vertex_id));
    }

    [[nodiscard]] std::vector<size_type> degree_map(this auto&& self) {
        std::vector<size_type> degree_map(self._list.size(), 0uz);

        for (auto idx = 0uz; idx < self._list.size(); ++idx) {
            degree_map[idx] += self._list.segment_size(idx);
            for (auto item : self._list[idx])
                ++degree_map[to_idx(item.vertex_id)];
        }

        return degree_map;
    }

    [[nodiscard]] std::vector<size_type> in_degree_map(this auto&& self) {
        std::vector<size_type> in_degree_map(self._list.size(), 0uz);
        for (auto item : self._list.data_view())
            ++in_degree_map[to_idx(item.vertex_id)];
        return in_degree_map;
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> out_degree_map(this auto&& self) {
        std::vector<size_type> out_degree;
        out_degree.reserve(self._list.size());
        for (auto idx = 0uz; idx < self._list.size(); ++idx)
            out_degree.push_back(self._list.segment_size(idx));
        return out_degree;
    }

    // --- edge modifiers ---

    gl_attr_force_inline void add_edge(
        this auto&& self, id_type edge_id, id_type source_id, id_type target_id
    ) {
        self._list.emplace_back(to_idx(source_id), target_id, edge_id);
    }

    void add_edges_from(
        this auto&& self,
        const traits::c_forward_range_of<id_type> auto& edge_ids,
        id_type source_id,
        const traits::c_forward_range_of<id_type> auto& target_ids
    ) {
        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids))
            self._list.emplace_back(to_idx(source_id), target_id, edge_id);
    }

protected:
    // --- vertex modifiers ---

    std::vector<id_type> _remove_vertex_impl(this auto&& self, id_type vertex_id) {
        const auto vertex_idx = to_idx(vertex_id);

        // extract out-edges
        auto removed_edges =
            self._list[vertex_idx] | std::views::transform(&item_type::edge_id)
            | std::ranges::to<std::vector>();

        // rebuild the graph (faster then shifting the entire data block for each removed edge)
        typename adjacency_list_impl_traits<traits_type>::storage_type new_list;
        new_list.reserve_segments(self._list.size() - 1uz);
        new_list.reserve_data(self._list.data_size() - self._list[vertex_idx].size());

        std::vector<item_type> buffer;
        for (auto idx = 0uz; idx < self._list.size(); ++idx) {
            if (idx == vertex_idx)
                continue;

            buffer.clear();
            for (auto item : self._list[idx]) {
                if (item.vertex_id == vertex_id)
                    removed_edges.push_back(item.edge_id); // remove in-edge
                else
                    buffer.push_back(item);
            }
            new_list.push_back(buffer);
        }

        self._list = std::move(new_list);
        return removed_edges;
    }

    // --- edge modifiers ---

    gl_attr_force_inline void _remove_edge_impl(this auto&& self, const auto& edge) {
        const auto edge_src = to_idx(edge.source());
        auto segment = self._list[edge_src];
        const auto it = detail::strict_find<item_type>(segment, edge);
        const auto pos = static_cast<size_type>(std::distance(segment.begin(), it));
        self._list.erase(edge_src, pos);
    }

    // --- edge getters ---

    [[nodiscard]] auto _in_edges_impl(this auto&& self, id_type vertex_id) {
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

template <traits::c_flat_list_graph_traits GraphTraits>
class undirected_flat_adjacency_list {
public:
    using traits_type = GraphTraits;
    using id_type = typename traits_type::id_type;
    using item_type = incidence_item<id_type>;

    // --- degree getters ---

    [[nodiscard]] size_type degree(this auto&& self, id_type vertex_id) {
        size_type degree = 0uz;
        for (auto item : self._list[to_idx(vertex_id)])
            degree += 1uz + static_cast<size_type>(item.vertex_id == vertex_id);
        return degree;
    }

    [[nodiscard]] gl_attr_force_inline size_type in_degree(this auto&& self, id_type vertex_id) {
        return self.degree(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type out_degree(this auto&& self, id_type vertex_id) {
        return self.degree(vertex_id);
    }

    [[nodiscard]] std::vector<size_type> degree_map(this auto&& self) {
        std::vector<size_type> degree_map;
        degree_map.reserve(self._list.size());
        for (id_type id = initial_id; id < self._list.size(); ++id)
            degree_map.push_back(self.degree(id));
        return degree_map;
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> in_degree_map(this auto&& self) {
        return self.degree_map();
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> out_degree_map(this auto&& self) {
        return self.degree_map();
    }

    // --- edge modifiers ---

    void add_edge(this auto&& self, id_type edge_id, id_type source_id, id_type target_id) {
        self._list.push_back(to_idx(source_id), {target_id, edge_id});
        if (target_id != source_id)
            self._list.push_back(to_idx(target_id), {source_id, edge_id});
    }

    void add_edges_from(
        this auto&& self,
        const traits::c_forward_range_of<id_type> auto& edge_ids,
        id_type source_id,
        const traits::c_forward_range_of<id_type> auto& target_ids
    ) {
        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids)) {
            self._list.push_back(to_idx(source_id), {target_id, edge_id});
            if (source_id != target_id)
                self._list.push_back(to_idx(target_id), {source_id, edge_id});
        }
    }

protected:
    // --- vertex modifiers ---

    std::vector<id_type> _remove_vertex_impl(this auto&& self, id_type vertex_id) {
        const auto vertex_idx = to_idx(vertex_id);

        // all removed edges are stored in the vertex's list segment
        auto removed_edges =
            self._list[vertex_idx] | std::views::transform(&item_type::edge_id)
            | std::ranges::to<std::vector>();

        // rebuild the graph (faster then shifting the entire data block for each removed edge)
        typename adjacency_list_impl_traits<traits_type>::storage_type new_list;
        new_list.reserve_segments(self._list.size() - 1uz);
        new_list.reserve_data(self._list.data_size() - self._list[vertex_idx].size());

        std::vector<item_type> buffer;
        for (auto idx = 0uz; idx < self._list.size(); ++idx) {
            if (idx == vertex_idx)
                continue;

            buffer.clear();
            for (auto item : self._list[idx])
                if (item.vertex_id != vertex_id)
                    buffer.push_back(item);

            new_list.push_back(buffer);
        }

        self._list = std::move(new_list);
        return removed_edges;
    }

    // --- edge modifiers ---

    void _remove_edge_impl(this auto&& self, const auto& edge) {
        const auto src_idx = to_idx(edge.source());
        const auto tgt_idx = to_idx(edge.target());

        // remove from the source segment
        {
            auto inc_edges = self._list[src_idx];
            const auto it = detail::strict_find<item_type>(inc_edges, edge);
            const auto pos = static_cast<size_type>(std::distance(inc_edges.begin(), it));
            self._list.erase(src_idx, pos);
        }

        if (not edge.is_loop()) {
            auto inc_edges = self._list[tgt_idx];
            const auto it = detail::strict_find<item_type>(inc_edges, edge);
            const auto pos = static_cast<size_type>(std::distance(inc_edges.begin(), it));
            self._list.erase(tgt_idx, pos);
        }
    }

    // --- edge getters ---

    [[nodiscard]] gl_attr_force_inline auto _in_edges_impl(this auto&& self, id_type vertex_id) {
        return std::views::all(self._list[to_idx(vertex_id)]);
    }
};

template <traits::c_flat_list_graph_traits GraphTraits>
struct adjacency_list_impl_traits<GraphTraits> {
    using type = std::conditional_t<
        traits::c_directed_graph_traits<GraphTraits>,
        directed_flat_adjacency_list<GraphTraits>,
        undirected_flat_adjacency_list<GraphTraits>>;

    template <typename ItemType>
    using storage_type = flat_jagged_vector<ItemType>;
};

} // namespace gl::impl::specialized
