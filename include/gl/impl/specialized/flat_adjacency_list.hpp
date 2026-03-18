// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/decl/impl_tags.hpp"
#include "gl/graph_traits.hpp"
#include "gl/impl/specialized/adjacency_list.hpp"
#include "gl/types/segment_vector.hpp"

#include <algorithm>
#include <format>
#include <ranges>
#include <vector>

namespace gl::impl::specialized {

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::c_directed_edge<typename AdjacencyList::edge_type>)
struct directed_flat_adjacency_list {
    using impl_type = AdjacencyList;
    using edge_type = typename impl_type::edge_type;

    [[nodiscard]] static auto in_edges(const impl_type& self, const types::id_type vertex_id) {
        std::vector<adjacency_list_item> in_edges;
        for (auto src_id = constants::initial_id; src_id < self._list.size(); ++src_id) {
            auto in_edges_view =
                self._list[src_id] | std::views::filter([tgt_id = vertex_id](const auto& item) {
                    return item.vertex_id == tgt_id;
                })
                | std::views::transform([src_id](const auto& item) {
                      return adjacency_list_item{src_id, item.edge_id};
                  });
            in_edges.insert(in_edges.end(), in_edges_view.begin(), in_edges_view.end());
        }
        return in_edges;
    }

    [[nodiscard]] static types::size_type in_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return std::ranges::count(self._list.data(), vertex_id, &adjacency_list_item::vertex_id);
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
        std::vector<types::size_type> in_degree_map(self._list.size(), 0uz);
        for (const auto& item : self._list.data())
            ++in_degree_map[item.vertex_id];
        return in_degree_map;
    }

    [[nodiscard]] gl_attr_force_inline static std::vector<types::size_type> out_degree_map(
        const impl_type& self
    ) {
        std::vector<types::size_type> out_degree;
        out_degree.reserve(self._list.size());
        for (auto id = constants::initial_id; id < self._list.size(); ++id)
            out_degree.push_back(self._list.segment_size(id));
        return out_degree;
    }

    [[nodiscard]] static std::vector<types::size_type> degree_map(const impl_type& self) {
        std::vector<types::size_type> degree_map(self._list.size(), 0uz);

        for (auto id = constants::initial_id; id < self._list.size(); ++id) {
            degree_map[id] += self._list.segment_size(id);
            for (const auto& item : self._list[id])
                ++degree_map[item.vertex_id];
        }

        return degree_map;
    }

    static std::vector<types::id_type> remove_vertex(
        impl_type& self, const types::id_type vertex_id
    ) {
        std::vector<types::id_type> removed_edges;

        // extract out-edges
        for (const auto& item : self._list[vertex_id])
            removed_edges.push_back(item.edge_id);

        // rebuild the graph (faster then shifting the entire data block for each removed edge)
        typename impl_type::adjacency_list_type new_list;
        new_list.reserve_segments(self._list.size() - 1uz);
        new_list.reserve_data(self._list.data_size() - self._list[vertex_id].size());

        std::vector<adjacency_list_item> buffer;
        for (auto id = constants::initial_id; id < self._list.size(); ++id) {
            if (id == vertex_id)
                continue;

            buffer.clear();
            for (const auto& item : self._list[id]) {
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

    gl_attr_force_inline static void add_edge(
        impl_type& self, types::id_type edge_id, types::id_type source_id, types::id_type target_id
    ) {
        self._list.push_back(source_id, {target_id, edge_id});
    }

    static void add_edges_from(
        impl_type& self,
        const type_traits::c_forward_range_of<types::id_type> auto& edge_ids,
        const types::id_type source_id,
        const type_traits::c_forward_range_of<types::id_type> auto& target_ids
    ) {
        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids))
            self._list.push_back(source_id, {target_id, edge_id});
    }

    gl_attr_force_inline static void remove_edge(impl_type& self, const edge_type& edge) {
        auto segment = self._list[edge.source()];
        const auto it = detail::strict_find(segment, edge);
        const auto pos = static_cast<std::size_t>(std::distance(segment.begin(), it));
        self._list.erase(edge.source(), pos);
    }
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::c_undirected_edge<typename AdjacencyList::edge_type>)
struct undirected_flat_adjacency_list {
    using impl_type = AdjacencyList;
    using edge_type = typename impl_type::edge_type;

    [[nodiscard]] gl_attr_force_inline static auto in_edges(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return self._list[vertex_id];
    }

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
        types::size_type degree = 0uz;
        for (const auto& item : self._list[vertex_id])
            degree += 1uz + static_cast<types::size_type>(item.vertex_id == vertex_id);
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
        for (auto id = constants::initial_id; id < self._list.size(); ++id)
            degree_map.push_back(degree(self, id));
        return degree_map;
    }

    static std::vector<types::id_type> remove_vertex(
        impl_type& self, const types::id_type vertex_id
    ) {
        // all removed edges are stored in the vertex's list segment
        auto removed_edges =
            self._list[vertex_id] | std::views::transform(&adjacency_list_item::edge_id)
            | std::ranges::to<std::vector>();

        // rebuild the graph (faster then shifting the entire data block for each removed edge)
        typename impl_type::adjacency_list_type new_list;
        new_list.reserve_segments(self._list.size() - 1uz);
        const auto estimated_new_size =
            self._list.data_size() - (self._list[vertex_id].size() * 2uz);
        new_list.reserve_data(estimated_new_size);

        std::vector<adjacency_list_item> buffer;
        for (types::id_type id = constants::initial_id; id < self._list.size(); ++id) {
            if (id == vertex_id)
                continue;

            buffer.clear();
            for (const auto& item : self._list[id])
                if (item.vertex_id != vertex_id)
                    buffer.push_back(item);

            new_list.push_back(buffer);
        }

        self._list = std::move(new_list);
        return removed_edges;
    }

    static void add_edge(
        impl_type& self, types::id_type edge_id, types::id_type source_id, types::id_type target_id
    ) {
        self._list.push_back(source_id, {target_id, edge_id});
        if (target_id != source_id)
            self._list.push_back(target_id, {source_id, edge_id});
    }

    static void add_edges_from(
        impl_type& self,
        const type_traits::c_forward_range_of<types::id_type> auto& edge_ids,
        const types::id_type source_id,
        const type_traits::c_forward_range_of<types::id_type> auto& target_ids
    ) {
        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids)) {
            self._list.push_back(source_id, {target_id, edge_id});
            if (source_id != target_id)
                self._list.push_back(target_id, {source_id, edge_id});
        }
    }

    static void remove_edge(impl_type& self, const edge_type& edge) {
        if (edge.is_loop()) {
            auto adj_edges = self._list[edge.source()];
            const auto it = detail::strict_find(adj_edges, edge);
            const auto pos = static_cast<types::size_type>(std::distance(adj_edges.begin(), it));
            self._list.erase(edge.source(), pos);
        }
        else {
            auto adj_edges_source = self._list[edge.source()];
            const auto it1 = detail::strict_find(adj_edges_source, edge);
            const auto pos1 =
                static_cast<types::size_type>(std::distance(adj_edges_source.begin(), it1));
            self._list.erase(edge.source(), pos1);

            auto adj_edges_target = self._list[edge.target()];
            const auto it2 = detail::strict_find(adj_edges_target, edge);
            const auto pos2 =
                static_cast<types::size_type>(std::distance(adj_edges_target.begin(), it2));
            self._list.erase(edge.target(), pos2);
        }
    }
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires type_traits::c_directed_edge<typename AdjacencyList::edge_type>
     and std::same_as<typename AdjacencyList::implementation_tag, flat_list_t>
struct adjacency_list_impl_traits<AdjacencyList> {
    using type = directed_flat_adjacency_list<AdjacencyList>;

    template <typename ItemType>
    using storage_type = types::segment_vector<ItemType>;
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires type_traits::c_undirected_edge<typename AdjacencyList::edge_type>
     and std::same_as<typename AdjacencyList::implementation_tag, flat_list_t>
struct adjacency_list_impl_traits<AdjacencyList> {
    using type = undirected_flat_adjacency_list<AdjacencyList>;

    template <typename ItemType>
    using storage_type = types::segment_vector<ItemType>;
};

} // namespace gl::impl::specialized
