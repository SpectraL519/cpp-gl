// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/diagnostics.hpp"
#include "gl/attributes/force_inline.hpp"
#include "gl/constants.hpp"
#include "gl/decl/impl_tags.hpp"
#include "gl/graph_traits.hpp"
#include "gl/impl/specialized/adjacency_matrix.hpp"
#include "gl/types/core.hpp"
#include "gl/types/flat_matrix.hpp"

#include <algorithm>
#include <cstddef>
#include <format>
#include <ranges>
#include <vector>

namespace gl::impl::specialized {

namespace detail {

template <traits::c_id_type IdType>
[[nodiscard]] auto& strict_get(flat_matrix<IdType>& id_matrix, const auto& edge) {
    // get the edge and validate the address
    const auto [source_id, target_id] = edge.incident_vertices();
    auto& edge_id = id_matrix[to_idx(source_id), to_idx(target_id)];
    if (edge.id() != edge_id)
        throw std::invalid_argument(std::format(
            "Got invalid edge [id = {} | vertices = ({}, {})]", edge.id(), source_id, target_id
        ));

    return edge_id;
}

template <traits::c_id_type IdType>
inline void check_edge_override(
    const flat_matrix<IdType>& id_matrix, const IdType source_id, const IdType target_id
) {
    if (const auto edge_id = id_matrix[to_idx(source_id), to_idx(target_id)]; edge_id != invalid_id)
        throw std::logic_error(std::format(
            "Cannot override an existing edge: [id = {}, vertices = ({}, {})]",
            edge_id,
            source_id,
            target_id
        ));
}

} // namespace detail

template <traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(traits::c_directed_edge<typename AdjacencyMatrix::edge_type>)
struct directed_flat_adjacency_matrix {
    using impl_type = AdjacencyMatrix;
    using id_type = typename impl_type::id_type;
    using storage_type = typename impl_type::adjacency_storage_type;

    using vertex_type = typename impl_type::vertex_type;
    using edge_type = typename impl_type::edge_type;

    // --- general ---

    gl_attr_force_inline static void init(impl_type& self, size_type n_vertices) {
        self._matrix = storage_type(n_vertices, n_vertices, invalid_id);
    }

    gl_attr_force_inline static id_type get_entry(
        const impl_type& self, id_type source_id, id_type target_id
    ) {
        return self._matrix[to_idx(source_id), to_idx(target_id)];
    }

    // --- vertex modifiers ---

    static void add_vertex(impl_type& self) {
        const auto new_size = self._matrix.n_rows() + 1uz;
        self._matrix.resize(new_size, new_size, invalid_id);
    }

    static void add_vertices(impl_type& self, size_type n) {
        const auto new_size = self._matrix.n_rows() + n;
        self._matrix.resize(new_size, new_size, invalid_id);
    }

    static std::vector<id_type> remove_vertex(impl_type& self, id_type vertex_id) {
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

    // --- vertex getters ---

    [[nodiscard]] gl_attr_force_inline static auto neighbor_ids(
        const impl_type& self, id_type vertex_id
    ) {
        return util::concat(predecessor_ids(self, vertex_id), successor_ids(self, vertex_id));
    }

    [[nodiscard]] gl_attr_force_inline static auto predecessor_ids(
        const impl_type& self, id_type vertex_id
    ) {
        return std::views::iota(initial_id_v<id_type>, static_cast<id_type>(self._matrix.n_rows()))
             | std::views::filter([&self, v_idx = to_idx(vertex_id)](const auto r_id) {
                   return self._matrix[to_idx(r_id), v_idx] != invalid_id;
               });
    }

    [[nodiscard]] gl_attr_force_inline static auto successor_ids(
        const impl_type& self, id_type vertex_id
    ) {
        return std::views::iota(initial_id_v<id_type>, static_cast<id_type>(self._matrix.n_cols()))
             | std::views::filter([&self, v_idx = to_idx(vertex_id)](const auto c_id) {
                   return self._matrix[v_idx, to_idx(c_id)] != invalid_id;
               });
    }

    // --- degree getters ---

    [[nodiscard]] gl_attr_force_inline static size_type degree(
        const impl_type& self, id_type vertex_id
    ) {
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

    [[nodiscard]] gl_attr_force_inline static size_type in_degree(
        const impl_type& self, id_type vertex_id
    ) {
        return static_cast<size_type>(std::ranges::count_if(
            self._matrix.col(to_idx(vertex_id)), [](auto edge_id) { return edge_id != invalid_id; }
        ));
    }

    [[nodiscard]] gl_attr_force_inline static size_type out_degree(
        const impl_type& self, id_type vertex_id
    ) {
        const auto row = self._matrix[to_idx(vertex_id)];
        return row.size() - static_cast<size_type>(std::ranges::count(row, invalid_id_v<id_type>));
    }

    [[nodiscard]] static std::vector<size_type> degree_map(const impl_type& self) {
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

    [[nodiscard]] static std::vector<size_type> in_degree_map(const impl_type& self) {
        std::vector<size_type> in_degree_map(self._matrix.n_rows(), 0uz);

        for (const auto row : self._matrix.rows())
            for (auto [target_id, edge_id] : std::views::enumerate(row))
                in_degree_map[static_cast<size_type>(target_id)] +=
                    static_cast<size_type>(edge_id != invalid_id);

        return in_degree_map;
    }

    [[nodiscard]] static std::vector<size_type> out_degree_map(const impl_type& self) {
        return std::views::iota(initial_id_v<id_type>, static_cast<id_type>(self._matrix.n_rows()))
             | std::views::transform([&](id_type id) { return out_degree(self, id); })
             | std::ranges::to<std::vector>();
    }

    // --- edge modifiers ---

    static inline void add_edge(
        impl_type& self, id_type edge_id, id_type source_id, id_type target_id
    ) {
        detail::check_edge_override(self._matrix, source_id, target_id);
        self._matrix[to_idx(source_id), to_idx(target_id)] = edge_id;
    }

    static void add_edges_from(
        impl_type& self,
        const traits::c_forward_range_of<id_type> auto& edge_ids,
        id_type source_id,
        const traits::c_forward_range_of<id_type> auto& target_ids
    ) {
        for (const auto target_id : target_ids)
            detail::check_edge_override(self._matrix, source_id, target_id);

        auto matrix_source_row = self._matrix[to_idx(source_id)];
        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids))
            matrix_source_row[to_diff(target_id)] = edge_id;
    }

    gl_attr_force_inline static void remove_edge(impl_type& self, const edge_type& edge) {
        detail::strict_get(self._matrix, edge) = invalid_id;
    }

    // --- edge getters ---

    [[nodiscard]] gl_attr_force_inline static auto incident_edges(
        const impl_type& self, id_type vertex_id
    ) {
        return util::concat(self.in_edges(vertex_id), self.out_edges(vertex_id));
    }

    [[nodiscard]] gl_attr_force_inline static auto incident_edges(
        const impl_type& self, id_type vertex_id, auto& edge_properties_map
    ) {
        return util::concat(
            self.in_edges(vertex_id, edge_properties_map),
            self.out_edges(vertex_id, edge_properties_map)
        );
    }
};

template <traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(traits::c_undirected_edge<typename AdjacencyMatrix::edge_type>)
struct undirected_flat_adjacency_matrix {
    using impl_type = AdjacencyMatrix;
    using id_type = typename impl_type::id_type;
    using storage_type = typename impl_type::adjacency_storage_type;

    using vertex_type = typename impl_type::vertex_type;
    using edge_type = typename impl_type::edge_type;

    // --- general ---

    gl_attr_force_inline static void init(impl_type& self, size_type n_vertices) {
        self._matrix = storage_type(n_vertices, n_vertices, invalid_id);
    }

    gl_attr_force_inline static id_type get_entry(
        const impl_type& self, id_type source_id, id_type target_id
    ) {
        return self._matrix[to_idx(source_id), to_idx(target_id)];
    }

    // --- vertex modifiers ---

    static void add_vertex(impl_type& self) {
        const auto new_size = self._matrix.n_rows() + 1uz;
        self._matrix.resize(new_size, new_size, invalid_id);
    }

    static void add_vertices(impl_type& self, size_type n) {
        const auto new_size = self._matrix.n_rows() + n;
        self._matrix.resize(new_size, new_size, invalid_id);
    }

    static std::vector<id_type> remove_vertex(impl_type& self, id_type vertex_id) {
        const auto vertex_idx = to_idx(vertex_id);

        const auto removed_edges =
            self._matrix[vertex_idx]
            | std::views::filter([](auto edge_id) { return edge_id != invalid_id; })
            | std::ranges::to<std::vector>();

        self._matrix.erase_row(vertex_idx);
        self._matrix.erase_col(vertex_idx);

        return removed_edges;
    }

    // --- vertex getters ---

    [[nodiscard]] gl_attr_force_inline static auto neighbor_ids(
        const impl_type& self, id_type vertex_id
    ) {
        return std::views::iota(initial_id_v<id_type>, static_cast<id_type>(self._matrix.n_cols()))
             | std::views::filter([&self, v_idx = to_idx(vertex_id)](const auto c_id) {
                   return self._matrix[v_idx, to_idx(c_id)] != invalid_id;
               });
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

    [[nodiscard]] gl_attr_force_inline static size_type degree(
        const impl_type& self, id_type vertex_id
    ) {
        const auto vertex_idx = to_idx(vertex_id);
        const auto row = self._matrix[vertex_idx];
        return self._matrix.n_cols()
             - static_cast<size_type>(std::ranges::count(row, invalid_id_v<id_type>))
             + static_cast<size_type>(self._matrix[vertex_idx, vertex_idx] != invalid_id);
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
        detail::check_edge_override(self._matrix, source_id, target_id);

        const auto source_idx = to_idx(source_id);
        const auto target_idx = to_idx(target_id);

        self._matrix[source_idx, target_idx] = edge_id;
        if (target_idx != source_idx)
            self._matrix[target_idx, source_idx] = edge_id;
    }

    static void add_edges_from(
        impl_type& self,
        const traits::c_forward_range_of<id_type> auto& edge_ids,
        id_type source_id,
        const traits::c_forward_range_of<id_type> auto& target_ids
    ) {
        for (const auto target_id : target_ids)
            detail::check_edge_override(self._matrix, source_id, target_id);

        const auto source_idx = to_idx(source_id);
        auto matrix_source_row = self._matrix[source_idx];

        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids)) {
            const auto target_idx = to_idx(target_id);
            matrix_source_row[to_diff(target_id)] = edge_id;
            if (target_idx != source_idx)
                self._matrix[target_idx, source_idx] = edge_id;
        }
    }

    static void remove_edge(impl_type& self, const edge_type& edge) {
        if (edge.is_loop()) {
            detail::strict_get(self._matrix, edge) = invalid_id;
        }
        else {
            detail::strict_get(self._matrix, edge) = invalid_id;
            self._matrix[to_idx(edge.target()), to_idx(edge.source())] = invalid_id;
        }
    }

    // --- edge getters ---

    [[nodiscard]] gl_attr_force_inline static auto incident_edges(
        const impl_type& self, id_type vertex_id
    ) {
        return self.out_edges(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline static auto incident_edges(
        const impl_type& self, id_type vertex_id, auto& edge_properties_map
    ) {
        return self.out_edges(vertex_id, edge_properties_map);
    }
};

template <traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires traits::c_directed_edge<typename AdjacencyMatrix::edge_type>
     and std::same_as<typename AdjacencyMatrix::implementation_tag, flat_matrix_t>
struct adjacency_matrix_impl_traits<AdjacencyMatrix> {
    using type = directed_flat_adjacency_matrix<AdjacencyMatrix>;

    template <traits::c_id_type IdType>
    using storage_type = flat_matrix<IdType>;
};

template <traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires traits::c_undirected_edge<typename AdjacencyMatrix::edge_type>
     and std::same_as<typename AdjacencyMatrix::implementation_tag, flat_matrix_t>
struct adjacency_matrix_impl_traits<AdjacencyMatrix> {
    using type = undirected_flat_adjacency_matrix<AdjacencyMatrix>;

    template <traits::c_id_type IdType>
    using storage_type = flat_matrix<IdType>;
};

} // namespace gl::impl::specialized
