// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/diagnostics.hpp"
#include "gl/attributes/force_inline.hpp"
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

template <traits::c_adjacency_matrix_graph_traits GraphTraits>
class adjacency_matrix;

namespace detail {

[[nodiscard]] auto& strict_get(auto& id_matrix, const auto& edge) {
    // get the edge and validate the address
    const auto [source_id, target_id] = edge.incident_vertices();
    auto& edge_id = id_matrix[to_idx(source_id)][to_idx(target_id)];
    if (edge.id() != edge_id)
        throw std::invalid_argument(std::format(
            "Got invalid edge [id = {} | vertices = ({}, {})]", edge.id(), source_id, target_id
        ));

    return edge_id;
}

template <traits::c_id_type IdType>
inline void check_edge_override(
    const auto& id_matrix, const IdType source_id, const IdType target_id
) {
    if (const auto edge_id = id_matrix[to_idx(source_id)][to_idx(target_id)]; edge_id != invalid_id)
        throw std::logic_error(std::format(
            "Cannot override an existing edge: [id = {}, vertices = ({}, {})]",
            edge_id,
            source_id,
            target_id
        ));
}

} // namespace detail

template <traits::c_matrix_graph_traits GraphTraits>
class directed_adjacency_matrix {
public:
    using traits_type = GraphTraits;
    using id_type = typename traits_type::id_type;
    using vertex_type = typename traits_type::vertex_type;
    using edge_type = typename traits_type::edge_type;
    using storage_type = std::vector<std::vector<id_type>>;

    // --- vertex getters ---

    [[nodiscard]] auto successor_ids(this const auto& self, id_type vertex_id) {
        return self._matrix[to_idx(vertex_id)] | std::views::enumerate
             | std::views::filter([](auto entry) {
                   auto [target_id, edge_id] = entry;
                   return edge_id != invalid_id;
               })
             | std::views::transform([](auto entry) {
                   auto [target_id, edge_id] = entry;
                   return static_cast<id_type>(target_id);
               });
    }

    [[nodiscard]] auto predecessor_ids(this const auto& self, id_type vertex_id) {
        return std::views::iota(initial_id_v<id_type>, static_cast<id_type>(self._matrix.size()))
             | std::views::filter([&self, v_idx = to_idx(vertex_id)](const auto src_id) {
                   return self._matrix[to_idx(src_id)][v_idx] != invalid_id;
               });
    }

    [[nodiscard]] auto neighbor_ids(this const auto& self, id_type vertex_id) {
        return util::concat(self.successor_ids(vertex_id), self.predecessor_ids(vertex_id));
    }

    // --- degree getters ---

    [[nodiscard]] gl_attr_force_inline size_type degree(this const auto& self, id_type vertex_id) {
        size_type deg = 0uz;
        const auto vertex_idx = to_idx(vertex_id);
        for (auto v_idx = 0uz; v_idx < self._matrix.size(); ++v_idx)
            deg += static_cast<size_type>(self._matrix[vertex_idx][v_idx] != invalid_id)
                 + static_cast<size_type>(self._matrix[v_idx][vertex_idx] != invalid_id);

        return deg;
    }

    GL_SUPPRESS_WARNING_BEGIN("-Wsign-conversion")

    [[nodiscard]] gl_attr_force_inline size_type
    in_degree(this const auto& self, id_type vertex_id) {
        return static_cast<size_type>(std::ranges::count_if(
            self._matrix,
            [vertex_id](const auto& row) { return row[to_idx(vertex_id)] != invalid_id; }
        ));
    }

    GL_SUPPRESS_WARNING_END

    [[nodiscard]] gl_attr_force_inline size_type
    out_degree(this const auto& self, id_type vertex_id) {
        return self._matrix[to_idx(vertex_id)].size()
             - static_cast<size_type>(
                   std::ranges::count(self._matrix[to_idx(vertex_id)], invalid_id_v<id_type>)
             );
    }

    [[nodiscard]] std::vector<size_type> degree_map(this const auto& self) {
        std::vector<size_type> degree_map(self._matrix.size(), 0uz);

        for (auto src_idx = 0uz; src_idx < self._matrix.size(); ++src_idx) {
            for (auto tgt_idx = 0uz; tgt_idx < self._matrix.size(); ++tgt_idx) {
                if (self._matrix[src_idx][tgt_idx] != invalid_id) {
                    ++degree_map[src_idx];
                    ++degree_map[tgt_idx];
                }
            }
        }

        return degree_map;
    }

    [[nodiscard]] std::vector<size_type> in_degree_map(this const auto& self) {
        std::vector<size_type> in_degree_map(self._matrix.size(), 0uz);

        for (const auto& row : self._matrix)
            for (auto [target_id, edge_id] : std::views::enumerate(row))
                in_degree_map[static_cast<size_type>(target_id)] +=
                    static_cast<size_type>(edge_id != invalid_id);

        return in_degree_map;
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> out_degree_map(this const auto& self
    ) {
        return std::views::iota(initial_id_v<id_type>, static_cast<id_type>(self._matrix.size()))
             | std::views::transform([&self](id_type id) { return self.out_degree(id); })
             | std::ranges::to<std::vector>();
    }

    // --- edge modifiers ---

    gl_attr_force_inline void add_edge(
        this auto& self, id_type edge_id, id_type source_id, id_type target_id
    ) {
        detail::check_edge_override(self._matrix, source_id, target_id);
        self._matrix[to_idx(source_id)][to_idx(target_id)] = edge_id;
    }

    void add_edges_from(
        this auto& self,
        const traits::c_forward_range_of<id_type> auto& edge_ids,
        id_type source_id,
        const traits::c_forward_range_of<id_type> auto& target_ids
    ) {
        for (const auto target_id : target_ids)
            detail::check_edge_override(self._matrix, source_id, target_id);

        auto& matrix_source_row = self._matrix[to_idx(source_id)];
        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids))
            matrix_source_row[to_idx(target_id)] = edge_id;
    }

    // --- comparison ---

    [[nodiscard]] friend bool
    operator==(const directed_adjacency_matrix&, const directed_adjacency_matrix&) = default;

protected:
    void _init(this auto& self, size_type n_vertices) {
        self._matrix.resize(n_vertices);
        for (auto& row : self._matrix)
            row.resize(n_vertices, invalid_id);
    }

    // --- vertex modifiers ---

    void _add_vertex_impl(this auto& self) {
        for (auto& row : self._matrix)
            row.emplace_back(invalid_id);
        self._matrix.emplace_back(self._matrix.size() + 1uz, invalid_id);
    }

    void _add_vertices_impl(this auto& self, size_type n) {
        const auto new_n_vertices = self._matrix.size() + n;
        for (auto& row : self._matrix)
            row.resize(new_n_vertices, invalid_id);
        for (auto _ = 0uz; _ < n; ++_)
            self._matrix.emplace_back(new_n_vertices, invalid_id);
    }

    std::vector<id_type> _remove_vertex_impl(this auto& self, id_type vertex_id) {
        const auto vertex_idx = to_idx(vertex_id);

        auto removed_edges =
            self._matrix[vertex_idx]
            | std::views::filter([](auto edge_id) { return edge_id != invalid_id; })
            | std::ranges::to<std::vector>();

        const auto vertex_pos = to_diff(vertex_id);
        self._matrix.erase(self._matrix.begin() + vertex_pos);
        for (auto& row : self._matrix) {
            if (const auto edge_id = row[vertex_idx]; edge_id != invalid_id)
                removed_edges.push_back(edge_id);
            row.erase(row.begin() + vertex_pos);
        }

        return removed_edges;
    }

    // --- edge modifiers ---

    gl_attr_force_inline void _remove_edge_impl(this auto& self, const auto& edge) {
        detail::strict_get(self._matrix, edge) = invalid_id;
    }

    // --- edge getters ---

    [[nodiscard]] gl_attr_force_inline id_type
    _get_entry_impl(this const auto& self, id_type source_id, id_type target_id) {
        return self._matrix[to_idx(source_id)][to_idx(target_id)];
    }
};

template <traits::c_matrix_graph_traits GraphTraits>
class undirected_adjacency_matrix {
public:
    using traits_type = GraphTraits;
    using id_type = typename traits_type::id_type;
    using vertex_type = typename traits_type::vertex_type;
    using edge_type = typename traits_type::edge_type;
    using storage_type = std::vector<std::vector<id_type>>;

    // --- vertex getters ---

    [[nodiscard]] auto neighbor_ids(this const auto& self, id_type vertex_id) {
        return self._matrix[to_idx(vertex_id)] | std::views::enumerate
             | std::views::filter([](auto entry) {
                   auto [target_id, edge_id] = entry;
                   return edge_id != invalid_id;
               })
             | std::views::transform([](auto entry) {
                   auto [target_id, edge_id] = entry;
                   return static_cast<id_type>(target_id);
               });
    }

    [[nodiscard]] gl_attr_force_inline auto predecessor_ids(
        this const auto& self, id_type vertex_id
    ) {
        return self.neighbor_ids(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto successor_ids(this const auto& self, id_type vertex_id) {
        return self.neighbor_ids(vertex_id);
    }

    // --- degree getters ---

    [[nodiscard]] gl_attr_force_inline size_type degree(this const auto& self, id_type vertex_id) {
        const auto vertex_idx = to_idx(vertex_id);
        return self._matrix.size()
             - static_cast<size_type>(
                   std::ranges::count(self._matrix[vertex_idx], invalid_id_v<id_type>)
             )
             + static_cast<size_type>(self._matrix[vertex_idx][vertex_idx] != invalid_id);
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
        std::vector<size_type> degree_map(self._matrix.size(), 0uz);

        for (auto src_idx = 0uz; src_idx < self._matrix.size(); ++src_idx) {
            for (auto tgt_idx = 0uz; tgt_idx <= src_idx; ++tgt_idx) {
                if (self._matrix[src_idx][tgt_idx] != invalid_id) {
                    ++degree_map[src_idx];
                    ++degree_map[tgt_idx];
                }
            }
        }

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

    void add_edge(this auto& self, id_type edge_id, id_type source_id, id_type target_id) {
        detail::check_edge_override(self._matrix, source_id, target_id);

        const auto source_idx = to_idx(source_id);
        const auto target_idx = to_idx(target_id);

        self._matrix[source_idx][target_idx] = edge_id;
        if (target_idx != source_idx)
            self._matrix[target_idx][source_idx] = edge_id;
    }

    void add_edges_from(
        this auto& self,
        const traits::c_forward_range_of<id_type> auto& edge_ids,
        id_type source_id,
        const traits::c_forward_range_of<id_type> auto& target_ids
    ) {
        for (const auto target_id : target_ids)
            detail::check_edge_override(self._matrix, source_id, target_id);

        const auto source_idx = to_idx(source_id);
        auto& matrix_source_row = self._matrix[source_idx];

        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids)) {
            const auto target_idx = to_idx(target_id);
            matrix_source_row[target_idx] = edge_id;
            if (target_idx != source_idx)
                self._matrix[target_idx][source_idx] = edge_id;
        }
    }

    // --- comparison ---

    [[nodiscard]] friend bool
    operator==(const undirected_adjacency_matrix&, const undirected_adjacency_matrix&) = default;

protected:
    void _init(this auto& self, size_type n_vertices) {
        self._matrix.resize(n_vertices);
        for (auto& row : self._matrix)
            row.resize(n_vertices, invalid_id);
    }

    // --- vertex modifiers ---

    void _add_vertex_impl(this auto& self) {
        for (auto& row : self._matrix)
            row.emplace_back(invalid_id);
        self._matrix.emplace_back(self._matrix.size() + 1uz, invalid_id);
    }

    void _add_vertices_impl(this auto& self, size_type n) {
        const auto new_n_vertices = self._matrix.size() + n;
        for (auto& row : self._matrix)
            row.resize(new_n_vertices, invalid_id);
        for (auto _ = 0uz; _ < n; ++_)
            self._matrix.emplace_back(new_n_vertices, invalid_id);
    }

    std::vector<id_type> _remove_vertex_impl(this auto& self, id_type vertex_id) {
        const auto vertex_idx = to_idx(vertex_id);

        const auto removed_edges =
            self._matrix[vertex_idx]
            | std::views::filter([](auto edge_id) { return edge_id != invalid_id; })
            | std::ranges::to<std::vector>();

        const auto vertex_pos = to_diff(vertex_id);
        self._matrix.erase(self._matrix.begin() + vertex_pos);
        for (auto& row : self._matrix)
            row.erase(row.begin() + vertex_pos);

        return removed_edges;
    }

    // --- edge modifiers ---

    void _remove_edge_impl(this auto& self, const auto& edge) {
        if (edge.is_loop()) {
            detail::strict_get(self._matrix, edge) = invalid_id;
        }
        else {
            detail::strict_get(self._matrix, edge) = invalid_id;
            self._matrix[to_idx(edge.target())][to_idx(edge.source())] = invalid_id;
        }
    }

    // --- edge getters ---

    [[nodiscard]] gl_attr_force_inline id_type
    _get_entry_impl(this const auto& self, id_type source_id, id_type target_id) {
        return self._matrix[to_idx(source_id)][to_idx(target_id)];
    }
};

template <traits::c_adjacency_matrix_graph_traits GraphTraits>
struct adjacency_matrix_base {
    using type = void;
};

template <traits::c_adjacency_matrix_graph_traits GraphTraits>
using adjacency_matrix_base_t = typename adjacency_matrix_base<GraphTraits>::type;

template <traits::c_matrix_graph_traits GraphTraits>
struct adjacency_matrix_base<GraphTraits> {
    using type = std::conditional_t<
        traits::c_directed_graph_traits<GraphTraits>,
        directed_adjacency_matrix<GraphTraits>,
        undirected_adjacency_matrix<GraphTraits>>;
};

} // namespace gl::impl
