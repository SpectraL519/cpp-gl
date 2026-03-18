// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/decl/impl_tags.hpp"
#include "gl/graph_traits.hpp"

#include <algorithm>
#include <vector>

namespace gl::impl {

template <type_traits::c_adjacency_matrix_graph_traits GraphTraits>
class adjacency_matrix;

namespace specialized {

namespace detail {

[[nodiscard]] types::id_type& strict_get(auto& id_matrix, const auto& edge) {
    // get the edge and validate the address
    const auto [source_id, target_id] = edge.incident_vertices();
    auto& edge_id = id_matrix[source_id][target_id];
    if (edge.id() != edge_id)
        throw std::invalid_argument(std::format(
            "Got invalid edge [id = {} | vertices = ({}, {})]", edge.id(), source_id, target_id
        ));

    return edge_id;
}

inline void check_edge_override(
    const auto& id_matrix, const types::id_type source_id, const types::id_type target_id
) {
    if (const auto edge_id = id_matrix[source_id][target_id]; edge_id != constants::invalid_id)
        throw std::logic_error(std::format(
            "Cannot override an existing edge: [id = {}, vertices = ({}, {})]",
            edge_id,
            source_id,
            target_id
        ));
}

} // namespace detail

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::c_directed_edge<typename AdjacencyMatrix::edge_type>)
struct directed_adjacency_matrix {
    using impl_type = AdjacencyMatrix;
    using vertex_type = typename impl_type::vertex_type;
    using edge_type = typename impl_type::edge_type;

    [[nodiscard]] gl_attr_force_inline static types::size_type in_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return std::ranges::count_if(self._matrix, [vertex_id](const auto& row) {
            return row[vertex_id] != constants::invalid_id;
        });
    }

    [[nodiscard]] gl_attr_force_inline static types::size_type out_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return self._matrix[vertex_id].size()
             - std::ranges::count(self._matrix[vertex_id], constants::invalid_id);
    }

    [[nodiscard]] gl_attr_force_inline static types::size_type degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        types::size_type deg = 0uz;
        for (types::id_type v_id = constants::initial_id; v_id < self._matrix.size(); ++v_id)
            deg += static_cast<types::size_type>(
                       self._matrix[vertex_id][v_id] != constants::invalid_id
                   )
                 + static_cast<types::size_type>(
                       self._matrix[v_id][vertex_id] != constants::invalid_id
                 );

        return deg;
    }

    [[nodiscard]] static std::vector<types::size_type> in_degree_map(const impl_type& self) {
        std::vector<types::size_type> in_degree_map(self._matrix.size(), 0uz);

        for (const auto& row : self._matrix)
            for (auto [target_id, edge_id] : std::views::enumerate(row))
                in_degree_map[target_id] +=
                    static_cast<types::size_type>(edge_id != constants::invalid_id);

        return in_degree_map;
    }

    [[nodiscard]] static std::vector<types::size_type> out_degree_map(const impl_type& self) {
        return std::views::iota(constants::initial_id, self._matrix.size())
             | std::views::transform([&](types::id_type id) { return out_degree(self, id); })
             | std::ranges::to<std::vector>();
    }

    [[nodiscard]] static std::vector<types::size_type> degree_map(const impl_type& self) {
        std::vector<types::size_type> degree_map(self._matrix.size(), 0uz);

        for (types::id_type source_id = constants::initial_id; source_id < self._matrix.size();
             ++source_id) {
            for (types::id_type target_id = constants::initial_id; target_id < self._matrix.size();
                 ++target_id) {
                if (self._matrix[source_id][target_id] != constants::invalid_id) {
                    ++degree_map[source_id];
                    ++degree_map[target_id];
                }
            }
        }

        return degree_map;
    }

    static std::vector<types::id_type> remove_vertex(
        impl_type& self, const types::id_type vertex_id
    ) {
        auto removed_edges_view =
            self._matrix[vertex_id]
            | std::views::filter([](auto edge_id) { return edge_id != constants::invalid_id; });

        // TODO: use std::ranges::to (requires newer compiler)
        std::vector<types::id_type> removed_edges(
            removed_edges_view.begin(), removed_edges_view.end()
        );

        self._matrix.erase(std::next(std::begin(self._matrix), vertex_id));

        for (auto& row : self._matrix) {
            if (const auto edge_id = row[vertex_id]; edge_id != constants::invalid_id)
                removed_edges.push_back(edge_id);
            row.erase(std::next(std::begin(row), vertex_id));
        }

        return removed_edges;
    }

    static inline void add_edge(
        impl_type& self, types::id_type edge_id, types::id_type source_id, types::id_type target_id
    ) {
        detail::check_edge_override(self._matrix, source_id, target_id);
        self._matrix[source_id][target_id] = edge_id;
    }

    static void add_edges_from(
        impl_type& self,
        const type_traits::c_forward_range_of<types::id_type> auto& edge_ids,
        const types::id_type source_id,
        const type_traits::c_forward_range_of<types::id_type> auto& target_ids
    ) {
        for (const auto target_id : target_ids)
            detail::check_edge_override(self._matrix, source_id, target_id);

        auto& matrix_source_row = self._matrix[source_id];
        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids))
            matrix_source_row[target_id] = edge_id;
    }

    static inline void remove_edge(impl_type& self, const edge_type& edge) {
        detail::strict_get(self._matrix, edge) = constants::invalid_id;
    }
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::c_undirected_edge<typename AdjacencyMatrix::edge_type>)
struct undirected_adjacency_matrix {
    using impl_type = AdjacencyMatrix;
    using vertex_type = typename impl_type::vertex_type;
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

    [[nodiscard]] gl_attr_force_inline static types::size_type degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return self._matrix.size()
             - std::ranges::count(self._matrix[vertex_id], constants::invalid_id)
             + static_cast<types::size_type>(
                   self._matrix[vertex_id][vertex_id] != constants::invalid_id
             );
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
        std::vector<types::size_type> degree_map(self._matrix.size(), 0uz);

        for (types::id_type source_id = constants::initial_id; source_id < self._matrix.size();
             ++source_id) {
            for (types::id_type target_id = constants::initial_id; target_id <= source_id;
                 ++target_id) {
                if (self._matrix[source_id][target_id] != constants::invalid_id) {
                    ++degree_map[source_id];
                    ++degree_map[target_id];
                }
            }
        }

        return degree_map;
    }

    static std::vector<types::id_type> remove_vertex(
        impl_type& self, const types::id_type vertex_id
    ) {
        auto removed_edges_view =
            self._matrix[vertex_id]
            | std::views::filter([](auto edge_id) { return edge_id != constants::invalid_id; });
        // TODO: use std::ranges::to (requires newer compiler)
        std::vector<types::id_type> removed_edges(
            removed_edges_view.begin(), removed_edges_view.end()
        );

        self._matrix.erase(std::next(std::begin(self._matrix), vertex_id));
        for (auto& row : self._matrix)
            row.erase(std::next(std::begin(row), vertex_id));

        return removed_edges;
    }

    static void add_edge(
        impl_type& self, types::id_type edge_id, types::id_type source_id, types::id_type target_id
    ) {
        detail::check_edge_override(self._matrix, source_id, target_id);

        self._matrix[source_id][target_id] = edge_id;
        if (target_id != source_id)
            self._matrix[target_id][source_id] = edge_id;
    }

    static void add_edges_from(
        impl_type& self,
        const type_traits::c_forward_range_of<types::id_type> auto& edge_ids,
        const types::id_type source_id,
        const type_traits::c_forward_range_of<types::id_type> auto& target_ids
    ) {
        for (const auto target_id : target_ids)
            detail::check_edge_override(self._matrix, source_id, target_id);

        auto& matrix_source_row = self._matrix[source_id];
        for (auto [edge_id, target_id] : std::views::zip(edge_ids, target_ids)) {
            matrix_source_row[target_id] = edge_id;
            if (target_id != source_id)
                self._matrix[target_id][source_id] = edge_id;
        }
    }

    static void remove_edge(impl_type& self, const edge_type& edge) {
        if (edge.is_loop()) {
            detail::strict_get(self._matrix, edge) = constants::invalid_id;
        }
        else {
            detail::strict_get(self._matrix, edge) = constants::invalid_id;
            // if the edge was found in the first matrix cell,
            // it will also be present in the second matrix cell
            self._matrix[edge.target()][edge.source()] = constants::invalid_id;
        }
    }
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
struct matrix_impl_traits {
    using type = void;
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::c_directed_edge<typename AdjacencyMatrix::edge_type>)
struct matrix_impl_traits<AdjacencyMatrix> {
    using type = directed_adjacency_matrix<AdjacencyMatrix>;
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::c_undirected_edge<typename AdjacencyMatrix::edge_type>)
struct matrix_impl_traits<AdjacencyMatrix> {
    using type = undirected_adjacency_matrix<AdjacencyMatrix>;
};

} // namespace specialized

} // namespace gl::impl
