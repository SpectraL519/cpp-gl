// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/constants.hpp"
#include "gl/impl/base/adjacency_matrix.hpp"
#include "gl/impl/base/flat_adjacency_matrix.hpp"
#include "gl/types/core.hpp"

#ifdef GL_TESTING
namespace gl_testing {
struct test_adjacency_matrix;
} // namespace gl_testing
#endif

namespace gl {

namespace detail {

template <traits::c_graph_repr_tag TargetImplTag, traits::c_graph_repr_tag SourceImplTag>
struct to_impl;

} // namespace detail

namespace impl {

template <traits::c_adjacency_matrix_graph_traits GraphTraits>
class adjacency_matrix final : public adjacency_matrix_base_t<GraphTraits> {
public:
    using traits_type = GraphTraits;
    using base_type = adjacency_matrix_base_t<traits_type>;
    using representation_tag = typename traits_type::representation_tag;
    using id_type = typename traits_type::id_type;

    using vertex_type = typename traits_type::vertex_type;
    using edge_type = typename traits_type::edge_type;

    using storage_type = typename base_type::storage_type;

    adjacency_matrix() = default;

    explicit adjacency_matrix(size_type n_vertices) {
        this->_init(n_vertices);
    }

    adjacency_matrix(const adjacency_matrix&) = default;
    adjacency_matrix& operator=(const adjacency_matrix&) = default;

    adjacency_matrix(adjacency_matrix&&) noexcept = default;
    adjacency_matrix& operator=(adjacency_matrix&&) noexcept = default;

    ~adjacency_matrix() = default;

    // --- vertex modifiers ---

    gl_attr_force_inline void add_vertex() {
        this->_add_vertex_impl();
    }

    gl_attr_force_inline void add_vertices(size_type n) {
        this->_add_vertices_impl(n);
    }

    std::vector<id_type> remove_vertex(id_type vertex_id) {
        auto removed_edge_ids = this->_remove_vertex_impl(vertex_id);
        this->_remap_element_ids(removed_edge_ids);
        return removed_edge_ids;
    }

    // --- edge modifiers ---

    void remove_edge(const auto& edge) {
        this->_remove_edge_impl(edge);
        for (auto&& row : this->_matrix)
            for (auto& edge_id : row)
                if (edge_id != invalid_id and edge_id > edge.id())
                    edge_id--;
    }

    std::vector<id_type> remove_edges(const traits::c_range auto& edges) {
        for (const auto& edge : edges)
            this->_remove_edge_impl(edge);
        auto removed_edge_ids =
            edges | std::views::transform([](const auto& edge) { return edge.id(); })
            | std::ranges::to<std::vector>();
        this->_remap_element_ids(removed_edge_ids);
        return removed_edge_ids;
    }

    // --- edge getters ---

    [[nodiscard]] gl_attr_force_inline bool has_edge(id_type source_id, id_type target_id) const {
        return this->_get_entry_impl(source_id, target_id) != invalid_id;
    }

    [[nodiscard]] bool has_edge(const auto& edge) const {
        return this->_get_entry_impl(edge.source(), edge.target()) == edge.id();
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] std::optional<EdgeType> edge(id_type source_id, id_type target_id) const
    requires(traits::c_has_empty_properties<EdgeType>)
    {
        const auto edge_id = this->_get_entry_impl(source_id, target_id);
        if (edge_id == invalid_id)
            return std::nullopt;
        return std::make_optional<EdgeType>(edge_id, source_id, target_id);
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] std::optional<EdgeType> edge(
        id_type source_id, id_type target_id, auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<EdgeType>)
    {
        const auto edge_id = this->_get_entry_impl(source_id, target_id);
        if (edge_id == invalid_id)
            return std::nullopt;
        return std::make_optional<EdgeType>(
            edge_id, source_id, target_id, edge_properties_map[to_idx(edge_id)]
        );
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] std::vector<EdgeType> edges(id_type source_id, id_type target_id) const
    requires(traits::c_has_empty_properties<EdgeType>)
    {
        const auto edge_id = this->_get_entry_impl(source_id, target_id);
        if (edge_id == invalid_id)
            return std::vector<EdgeType>();
        return std::vector<EdgeType>{
            EdgeType{edge_id, source_id, target_id}
        };
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] std::vector<EdgeType> edges(
        id_type source_id, id_type target_id, auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<EdgeType>)
    {
        const auto edge_id = this->_get_entry_impl(source_id, target_id);
        if (edge_id == invalid_id)
            return std::vector<EdgeType>();
        return std::vector<EdgeType>{
            EdgeType{edge_id, source_id, target_id, edge_properties_map[to_idx(edge_id)]}
        };
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] gl_attr_force_inline auto incident_edges(id_type vertex_id) const
    requires(traits::c_has_empty_properties<EdgeType>)
    {
        if constexpr (traits::c_directed_graph_traits<traits_type>) {
            return util::concat(
                this->template in_edges<EdgeType>(vertex_id),
                this->template out_edges<EdgeType>(vertex_id)
            );
        }
        else {
            return this->template out_edges<EdgeType>(vertex_id);
        }
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] gl_attr_force_inline auto incident_edges(
        id_type vertex_id, auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<EdgeType>)
    {
        if constexpr (traits::c_directed_graph_traits<traits_type>) {
            return util::concat(
                this->template in_edges<EdgeType>(vertex_id, edge_properties_map),
                this->template out_edges<EdgeType>(vertex_id, edge_properties_map)
            );
        }
        else {
            return this->template out_edges<EdgeType>(vertex_id, edge_properties_map);
        }
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] gl_attr_force_inline auto in_edges(id_type vertex_id) const
    requires(traits::c_has_empty_properties<EdgeType>)
    {
        return std::views::iota(initial_id_v<id_type>, static_cast<id_type>(this->_matrix.size()))
             | std::views::filter([this, vertex_id](auto source_id) {
                   return this->_get_entry_impl(source_id, vertex_id) != invalid_id;
               })
             | std::views::transform([this, vertex_id](auto source_id) {
                   return EdgeType{
                       this->_get_entry_impl(source_id, vertex_id), source_id, vertex_id
                   };
               });
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] gl_attr_force_inline auto in_edges(id_type vertex_id, auto& edge_properties_map)
        const
    requires(traits::c_has_non_empty_properties<EdgeType>)
    {
        return std::views::iota(initial_id_v<id_type>, static_cast<id_type>(this->_matrix.size()))
             | std::views::filter([this, vertex_id](auto source_id) {
                   return this->_get_entry_impl(source_id, vertex_id) != invalid_id;
               })
             | std::views::transform([this, vertex_id, &edge_properties_map](auto source_id) {
                   const auto edge_id = this->_get_entry_impl(source_id, vertex_id);
                   return EdgeType{
                       edge_id, source_id, vertex_id, edge_properties_map[to_idx(edge_id)]
                   };
               });
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] gl_attr_force_inline auto out_edges(id_type vertex_id) const
    requires(traits::c_has_empty_properties<EdgeType>)
    {
        return this->_matrix[to_idx(vertex_id)] | std::views::enumerate
             | std::views::filter([](auto entry) {
                   auto [_, edge_id] = entry;
                   return edge_id != invalid_id;
               })
             | std::views::transform([vertex_id](auto entry) {
                   auto [target_id, edge_id] = entry;
                   return EdgeType{edge_id, vertex_id, static_cast<id_type>(target_id)};
               });
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] gl_attr_force_inline auto out_edges(id_type vertex_id, auto& edge_properties_map)
        const
    requires(traits::c_has_non_empty_properties<EdgeType>)
    {
        return this->_matrix[to_idx(vertex_id)] | std::views::enumerate
             | std::views::filter([](auto entry) {
                   const auto [_, edge_id] = entry;
                   return edge_id != invalid_id;
               })
             | std::views::transform([vertex_id, &edge_properties_map](auto entry) {
                   const auto [target_id, edge_id] = entry;
                   return EdgeType{
                       edge_id,
                       vertex_id,
                       static_cast<id_type>(target_id),
                       edge_properties_map[to_idx(edge_id)]
                   };
               });
    }

    // --- comparison ---

    [[nodiscard]] friend bool operator==(const adjacency_matrix&, const adjacency_matrix&) =
        default;

    // --- friend declarations ---

    friend base_type;

    template <traits::c_graph_repr_tag TargetImplTag, traits::c_graph_repr_tag SourceImplTag>
    friend struct gl::detail::to_impl;

#ifdef GL_TESTING
    friend struct gl_testing::test_adjacency_matrix;
#endif

private:
    void _remap_element_ids(std::vector<id_type>& removed_edge_ids) {
        std::ranges::sort(removed_edge_ids);
        removed_edge_ids.erase(
            std::ranges::unique(removed_edge_ids).begin(), removed_edge_ids.end()
        );

        for (auto&& row : this->_matrix) {
            for (auto& edge_id : row) {
                if (edge_id == invalid_id)
                    continue;

                auto it = std::ranges::lower_bound(removed_edge_ids, edge_id);
                if (it != removed_edge_ids.end() and *it == edge_id)
                    edge_id = invalid_id; // edge was removed
                else
                    // shift by the number of removed IDs < edge-id
                    edge_id -=
                        static_cast<id_type>(std::ranges::distance(removed_edge_ids.begin(), it));
            }
        }
    }

    storage_type _matrix{};
};

} // namespace impl
} // namespace gl
