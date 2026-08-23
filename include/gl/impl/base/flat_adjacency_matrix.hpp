// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/diagnostics.hpp"
#include "gl/attributes/force_inline.hpp"
#include "gl/constants.hpp"
#include "gl/decl/repr_tags.hpp"
#include "gl/graph_traits.hpp"
#include "gl/impl/base/adjacency_matrix.hpp"
#include "gl/types/core.hpp"
#include "gl/types/flat_matrix.hpp"

#include <algorithm>
#include <cstddef>
#include <format>
#include <ranges>
#include <vector>

namespace gl::impl {

namespace detail {

template <traits::c_api_policy_tag ApiPolicyTag, traits::c_id_type IdType>
[[nodiscard]] auto& get_edge_entry(flat_matrix<IdType>& id_matrix, const auto& edge) {
    // get the edge and validate the address
    const auto [source_id, target_id] = edge.incident_vertices();
    auto& edge_id = id_matrix[to_idx(source_id), to_idx(target_id)];

    if constexpr (std::same_as<ApiPolicyTag, api::strict_t>)
        if (edge.id() != edge_id)
            throw std::invalid_argument(std::format(
                "Got invalid edge [id = {} | vertices = ({}, {})]", edge.id(), source_id, target_id
            ));

    return edge_id;
}

template <traits::c_api_policy_tag ApiPolicyTag, traits::c_id_type IdType>
inline void check_edge_override(
    const flat_matrix<IdType>& id_matrix, const IdType source_id, const IdType target_id
) {
    if constexpr (std::same_as<ApiPolicyTag, api::strict_t>)
        if (const auto edge_id = id_matrix[to_idx(source_id), to_idx(target_id)];
            edge_id != invalid_id)
            throw std::logic_error(std::format(
                "Cannot override an existing edge: [id = {}, vertices = ({}, {})]",
                edge_id,
                source_id,
                target_id
            ));
}

} // namespace detail

template <traits::c_flat_matrix_graph_traits GraphTraits>
class directed_flat_adjacency_matrix {
public:
    using traits_type = GraphTraits;
    using api_policy_tag = typename traits_type::api_policy_tag;
    using id_type = typename traits_type::id_type;
    using vertex_type = typename traits_type::vertex_type;
    using edge_type = typename traits_type::edge_type;
    using storage_type = flat_matrix<id_type>;

    // --- vertex getters ---

    [[nodiscard]] gl_attr_force_inline auto predecessor_ids(
        this const auto& self, id_type vertex_id
    ) {
        return std::views::iota(initial_id_v<id_type>, static_cast<id_type>(self._matrix.n_rows()))
             | std::views::filter([&self, v_idx = to_idx(vertex_id)](const auto r_id) {
                   return self._matrix[to_idx(r_id), v_idx] != invalid_id;
               });
    }

    [[nodiscard]] gl_attr_force_inline auto successor_ids(this const auto& self, id_type vertex_id) {
        return std::views::iota(initial_id_v<id_type>, static_cast<id_type>(self._matrix.n_cols()))
             | std::views::filter([&self, v_idx = to_idx(vertex_id)](const auto c_id) {
                   return self._matrix[v_idx, to_idx(c_id)] != invalid_id;
               });
    }

    [[nodiscard]] gl_attr_force_inline auto neighbor_ids(this const auto& self, id_type vertex_id) {
        return util::concat(self.predecessor_ids(vertex_id), self.successor_ids(vertex_id));
    }

    // --- degree getters ---

    [[nodiscard]] gl_attr_force_inline size_type degree(this const auto& self, id_type vertex_id) {
        size_type deg = 0uz;
        const auto vertex_idx = to_idx(vertex_id);
        const auto row = self._matrix[vertex_idx];
        const auto col = self._matrix.col(vertex_idx);

        const auto n_rows_bound = to_diff(self._matrix.n_rows());
        for (auto v_pos = 0z; v_pos < n_rows_bound; ++v_pos)
            deg += static_cast<size_type>(row[v_pos] != invalid_id)
                 + static_cast<size_type>(col[v_pos] != invalid_id);

        return deg;
    }

    [[nodiscard]] gl_attr_force_inline size_type
    in_degree(this const auto& self, id_type vertex_id) {
        return static_cast<size_type>(std::ranges::count_if(
            self._matrix.col(to_idx(vertex_id)), [](auto edge_id) { return edge_id != invalid_id; }
        ));
    }

    [[nodiscard]] gl_attr_force_inline size_type
    out_degree(this const auto& self, id_type vertex_id) {
        const auto row = self._matrix[to_idx(vertex_id)];
        return row.size() - static_cast<size_type>(std::ranges::count(row, invalid_id_v<id_type>));
    }

    [[nodiscard]] std::vector<size_type> degree_map(this const auto& self) {
        std::vector<size_type> degree_map(self._matrix.n_rows(), 0uz);

        for (auto src_idx = 0uz; src_idx < self._matrix.n_rows(); ++src_idx) {
            for (auto tgt_idx = 0uz; tgt_idx < self._matrix.n_cols(); ++tgt_idx) {
                if (self._matrix[src_idx, tgt_idx] != invalid_id) {
                    ++degree_map[src_idx];
                    ++degree_map[tgt_idx];
                }
            }
        }

        return degree_map;
    }

    [[nodiscard]] std::vector<size_type> in_degree_map(this const auto& self) {
        std::vector<size_type> in_degree_map(self._matrix.n_rows(), 0uz);

        for (const auto row : self._matrix.rows())
            for (auto [target_id, edge_id] : std::views::enumerate(row))
                in_degree_map[static_cast<size_type>(target_id)] +=
                    static_cast<size_type>(edge_id != invalid_id);

        return in_degree_map;
    }

    [[nodiscard]] std::vector<size_type> out_degree_map(this const auto& self) {
        return std::views::iota(initial_id_v<id_type>, static_cast<id_type>(self._matrix.n_rows()))
             | std::views::transform([&self](id_type id) { return self.out_degree(id); })
             | std::ranges::to<std::vector>();
    }

    // --- edge modifiers ---

    gl_attr_force_inline void add_edge(
        this auto& self, id_type edge_id, id_type source_id, id_type target_id
    ) {
        detail::check_edge_override<api_policy_tag>(self._matrix, source_id, target_id);
        self._matrix[to_idx(source_id), to_idx(target_id)] = edge_id;
    }

    void add_edges_from(
        this auto& self,
        const traits::c_forward_range_of<id_type> auto& edge_ids,
        id_type source_id,
        const traits::c_forward_range_of<id_type> auto& target_ids
    ) {
        for (const auto target_id : target_ids)
            detail::check_edge_override<api_policy_tag>(self._matrix, source_id, target_id);

        auto matrix_source_row = self._matrix[to_idx(source_id)];
        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids))
            matrix_source_row[to_diff(target_id)] = edge_id;
    }

    // --- comparison ---

    [[nodiscard]] friend bool
    operator==(const directed_flat_adjacency_matrix&, const directed_flat_adjacency_matrix&) =
        default;

protected:
    gl_attr_force_inline void _init(this auto& self, size_type n_vertices) {
        self._matrix = storage_type(n_vertices, n_vertices, invalid_id);
    }

    // --- vertex modifiers ---

    void _add_vertex_impl(this auto& self) {
        const auto new_size = self._matrix.n_rows() + 1uz;
        self._matrix.resize(new_size, new_size, invalid_id);
    }

    void _add_vertices_impl(this auto& self, size_type n) {
        const auto new_size = self._matrix.n_rows() + n;
        self._matrix.resize(new_size, new_size, invalid_id);
    }

    std::vector<id_type> _remove_vertex_impl(this auto& self, id_type vertex_id) {
        const auto vertex_idx = to_idx(vertex_id);
        std::vector<id_type> removed_edges;
        removed_edges.reserve(self._matrix.size() * 2uz);

        // extract out-edges
        for (auto edge_id : self._matrix[vertex_idx])
            if (edge_id != invalid_id)
                removed_edges.push_back(edge_id);

        // extract in-edges
        const auto col = self._matrix.col(vertex_idx);
        for (auto r_idx = 0uz; r_idx < self._matrix.n_rows(); ++r_idx) {
            if (r_idx == vertex_idx)
                continue;

            const auto edge_id = col[to_diff(r_idx)];
            if (edge_id != invalid_id)
                removed_edges.push_back(edge_id);
        }

        // elegantly remove from the 2D grid
        self._matrix.erase_row(vertex_idx);
        self._matrix.erase_col(vertex_idx);

        return removed_edges;
    }

    // --- edge modifiers ---

    gl_attr_force_inline void _remove_edge_impl(this auto& self, const auto& edge) {
        detail::get_edge_entry<api_policy_tag>(self._matrix, edge) = invalid_id;
    }

    // --- edge getters ---

    [[nodiscard]] gl_attr_force_inline id_type
    _get_entry_impl(this const auto& self, id_type source_id, id_type target_id) {
        return self._matrix[to_idx(source_id), to_idx(target_id)];
    }
};

template <traits::c_flat_matrix_graph_traits GraphTraits>
class undirected_flat_adjacency_matrix {
public:
    using traits_type = GraphTraits;
    using api_policy_tag = typename traits_type::api_policy_tag;
    using id_type = typename traits_type::id_type;
    using vertex_type = typename traits_type::vertex_type;
    using edge_type = typename traits_type::edge_type;
    using storage_type = flat_matrix<id_type>;

    // --- vertex getters ---

    [[nodiscard]] gl_attr_force_inline auto neighbor_ids(this const auto& self, id_type vertex_id) {
        return std::views::iota(initial_id_v<id_type>, static_cast<id_type>(self._matrix.n_cols()))
             | std::views::filter([&self, v_idx = to_idx(vertex_id)](const auto c_id) {
                   return self._matrix[v_idx, to_idx(c_id)] != invalid_id;
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
        const auto row = self._matrix[vertex_idx];
        return self._matrix.n_cols()
             - static_cast<size_type>(std::ranges::count(row, invalid_id_v<id_type>))
             + static_cast<size_type>(self._matrix[vertex_idx, vertex_idx] != invalid_id);
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
        std::vector<size_type> degree_map(self._matrix.n_rows(), 0uz);

        for (auto src_idx = 0uz; src_idx < self._matrix.n_rows(); ++src_idx) {
            for (auto tgt_idx = 0uz; tgt_idx <= src_idx; ++tgt_idx) {
                if (self._matrix[src_idx, tgt_idx] != invalid_id) {
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
        detail::check_edge_override<api_policy_tag>(self._matrix, source_id, target_id);

        const auto source_idx = to_idx(source_id);
        const auto target_idx = to_idx(target_id);

        self._matrix[source_idx, target_idx] = edge_id;
        if (target_idx != source_idx)
            self._matrix[target_idx, source_idx] = edge_id;
    }

    void add_edges_from(
        this auto& self,
        const traits::c_forward_range_of<id_type> auto& edge_ids,
        id_type source_id,
        const traits::c_forward_range_of<id_type> auto& target_ids
    ) {
        for (const auto target_id : target_ids)
            detail::check_edge_override<api_policy_tag>(self._matrix, source_id, target_id);

        const auto source_idx = to_idx(source_id);
        auto matrix_source_row = self._matrix[source_idx];

        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids)) {
            const auto target_idx = to_idx(target_id);
            matrix_source_row[to_diff(target_id)] = edge_id;
            if (target_idx != source_idx)
                self._matrix[target_idx, source_idx] = edge_id;
        }
    }

    // --- comparison ---

    [[nodiscard]] friend bool
    operator==(const undirected_flat_adjacency_matrix&, const undirected_flat_adjacency_matrix&) =
        default;

protected:
    gl_attr_force_inline void _init(this auto& self, size_type n_vertices) {
        self._matrix = storage_type(n_vertices, n_vertices, invalid_id);
    }

    // --- vertex modifiers ---

    void _add_vertex_impl(this auto& self) {
        const auto new_size = self._matrix.n_rows() + 1uz;
        self._matrix.resize(new_size, new_size, invalid_id);
    }

    void _add_vertices_impl(this auto& self, size_type n) {
        const auto new_size = self._matrix.n_rows() + n;
        self._matrix.resize(new_size, new_size, invalid_id);
    }

    std::vector<id_type> _remove_vertex_impl(this auto& self, id_type vertex_id) {
        const auto vertex_idx = to_idx(vertex_id);

        std::vector<id_type> removed_edges;
        for (auto edge_id : self._matrix[vertex_idx]) {
            if (edge_id != invalid_id)
                removed_edges.push_back(edge_id);
        }

        self._matrix.erase_row(vertex_idx);
        self._matrix.erase_col(vertex_idx);

        return removed_edges;
    }

    // --- edge modifiers ---

    void _remove_edge_impl(this auto& self, const auto& edge) {
        if (edge.is_loop()) {
            detail::get_edge_entry<api_policy_tag>(self._matrix, edge) = invalid_id;
        }
        else {
            detail::get_edge_entry<api_policy_tag>(self._matrix, edge) = invalid_id;
            self._matrix[to_idx(edge.target()), to_idx(edge.source())] = invalid_id;
        }
    }

    // --- edge getters ---

    [[nodiscard]] gl_attr_force_inline id_type
    _get_entry_impl(this const auto& self, id_type source_id, id_type target_id) {
        return self._matrix[to_idx(source_id), to_idx(target_id)];
    }
};

template <traits::c_flat_matrix_graph_traits GraphTraits>
struct adjacency_matrix_base<GraphTraits> {
    using type = std::conditional_t<
        traits::c_directed_graph_traits<GraphTraits>,
        directed_flat_adjacency_matrix<GraphTraits>,
        undirected_flat_adjacency_matrix<GraphTraits>>;
};

} // namespace gl::impl
