// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/graph_traits.hpp"
#include "gl/impl/specialized/adjacency_list.hpp"
#include "gl/impl/specialized/flat_adjacency_list.hpp"
#include "gl/types/core.hpp"

#ifdef GL_TESTING
namespace gl_testing {
struct test_adjacency_list;
} // namespace gl_testing
#endif

namespace gl {

namespace detail {

template <traits::c_graph_repr_tag TargetImplTag, traits::c_graph_repr_tag SourceImplTag>
struct to_impl;

} // namespace detail

namespace impl {

template <traits::c_adjacency_list_graph_traits GraphTraits>
class adjacency_list final : public specialized::adjacency_list_base_t<GraphTraits> {
public:
    using traits_type = GraphTraits;
    using base_type = specialized::adjacency_list_base_t<traits_type>;
    using id_type = typename traits_type::id_type;
    using item_type = typename base_type::item_type;
    using storage_type = typename base_type::storage_type;

    adjacency_list() = default;

    explicit adjacency_list(size_type n_vertices) : _list(n_vertices) {}

    adjacency_list(const adjacency_list&) = default;
    adjacency_list& operator=(const adjacency_list&) = default;

    adjacency_list(adjacency_list&&) noexcept = default;
    adjacency_list& operator=(adjacency_list&&) noexcept = default;

    ~adjacency_list() = default;

    // --- vertex modifiers ---

    gl_attr_force_inline void add_vertex() {
        this->_list.resize(this->_list.size() + 1uz);
    }

    gl_attr_force_inline void add_vertices(size_type n) {
        this->_list.resize(this->_list.size() + n);
    }

    std::vector<id_type> remove_vertex(id_type vertex_id) {
        auto removed_edge_ids = this->_remove_vertex_impl(vertex_id);
        this->_remap_element_ids(vertex_id, removed_edge_ids);
        return removed_edge_ids;
    }

    // --- vertex getters ---

    [[nodiscard]] auto neighbor_ids(id_type vertex_id) const {
        if constexpr (traits::c_directed_graph_traits<GraphTraits>)
            return util::concat(this->successor_ids(vertex_id), this->predecessor_ids(vertex_id));
        else
            return this->_list[to_idx(vertex_id)] | std::views::transform(&item_type::vertex_id);
    }

    [[nodiscard]] auto predecessor_ids(id_type vertex_id) const {
        if constexpr (traits::c_directed_graph_traits<GraphTraits>)
            return this->_in_edges_impl(vertex_id) | std::views::transform(&item_type::vertex_id);
        else
            return this->neighbor_ids(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto successor_ids(id_type vertex_id) const {
        if constexpr (traits::c_directed_graph_traits<GraphTraits>)
            return this->_list[to_idx(vertex_id)] | std::views::transform(&item_type::vertex_id);
        else
            return this->neighbor_ids(vertex_id);
    }

    // --- edge modifiers ---

    void remove_edge(const auto& edge) {
        this->_remove_edge_impl(edge);
        for (auto&& inc : this->_list)
            for (auto& item : inc)
                item.edge_id -= static_cast<id_type>(item.edge_id > edge.id());
    }

    std::vector<id_type> remove_edges(const traits::c_range auto& edges) {
        for (const auto& edge : edges)
            this->_remove_edge_impl(edge);

        auto removed_edge_ids =
            edges | std::views::transform([](const auto& edge) { return edge.id(); })
            | std::ranges::to<std::vector>();
        this->_remap_element_ids(invalid_id, removed_edge_ids);
        return removed_edge_ids;
    }

    // --- edge getters ---

    [[nodiscard]] gl_attr_force_inline bool has_edge(id_type source_id, id_type target_id) const {
        return std::ranges::contains(
            this->_list[to_idx(source_id)], target_id, &item_type::vertex_id
        );
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(const auto& edge) const {
        return std::ranges::contains(
            this->_list[to_idx(edge.source())], item_type{edge.target(), edge.id()}
        );
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] std::optional<EdgeType> edge(id_type source_id, id_type target_id) const
    requires(traits::c_has_empty_properties<EdgeType>)
    {
        const auto& out_edges = this->_list[to_idx(source_id)];
        const auto item_it = std::ranges::find(out_edges, target_id, &item_type::vertex_id);
        if (item_it == out_edges.cend())
            return std::nullopt;
        return std::make_optional<EdgeType>(item_it->edge_id, source_id, target_id);
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] std::optional<EdgeType> edge(
        id_type source_id, id_type target_id, auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<EdgeType>)
    {
        const auto& out_edges = this->_list[to_idx(source_id)];
        const auto item_it = std::ranges::find(out_edges, target_id, &item_type::vertex_id);
        if (item_it == out_edges.cend())
            return std::nullopt;
        return std::make_optional<EdgeType>(
            item_it->edge_id, source_id, target_id, edge_properties_map[to_idx(item_it->edge_id)]
        );
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] std::vector<EdgeType> edges(id_type source_id, id_type target_id) const
    requires(traits::c_has_empty_properties<EdgeType>)
    {
        return this->_list[source_id]
             | std::views::filter([&target_id](auto item) { return item.vertex_id == target_id; })
             | std::views::transform([source_id](auto item) {
                   return EdgeType{item.edge_id, source_id, item.vertex_id};
               })
             | std::ranges::to<std::vector>();
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] std::vector<EdgeType> edges(
        id_type source_id, id_type target_id, auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<EdgeType>)
    {
        return this->_list[to_idx(source_id)]
             | std::views::filter([&target_id](auto item) { return item.vertex_id == target_id; })
             | std::views::transform([source_id, &edge_properties_map](auto item) {
                   return EdgeType{
                       item.edge_id,
                       source_id,
                       item.vertex_id,
                       edge_properties_map[to_idx(item.edge_id)]
                   };
               })
             | std::ranges::to<std::vector>();
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
        return this->_in_edges_impl(vertex_id) | std::views::transform([vertex_id](auto item) {
                   return EdgeType{item.edge_id, item.vertex_id, vertex_id};
               });
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] gl_attr_force_inline auto in_edges(id_type vertex_id, auto& edge_properties_map)
        const
    requires(traits::c_has_non_empty_properties<EdgeType>)
    {
        return this->_in_edges_impl(vertex_id)
             | std::views::transform([vertex_id, &edge_properties_map](auto item) {
                   return EdgeType{
                       item.edge_id,
                       item.vertex_id,
                       vertex_id,
                       edge_properties_map[to_idx(item.edge_id)]
                   };
               });
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] gl_attr_force_inline auto out_edges(id_type vertex_id) const
    requires(traits::c_has_empty_properties<EdgeType>)
    {
        return this->_list[to_idx(vertex_id)] | std::views::transform([vertex_id](auto item) {
                   return EdgeType{item.edge_id, vertex_id, item.vertex_id};
               });
    }

    template <typename EdgeType = typename traits_type::edge_type>
    [[nodiscard]] gl_attr_force_inline auto out_edges(id_type vertex_id, auto& edge_properties_map)
        const
    requires(traits::c_has_non_empty_properties<EdgeType>)
    {
        return this->_list[to_idx(vertex_id)]
             | std::views::transform([vertex_id, &edge_properties_map](auto item) {
                   return EdgeType{
                       item.edge_id,
                       vertex_id,
                       item.vertex_id,
                       edge_properties_map[to_idx(item.edge_id)]
                   };
               });
    }

    // --- comparison ---

    [[nodiscard]] friend bool operator==(const adjacency_list&, const adjacency_list&) = default;

    // --- friend declarations ---

    friend base_type;

    template <traits::c_graph_repr_tag TargetImplTag, traits::c_graph_repr_tag SourceImplTag>
    friend struct gl::detail::to_impl;

#ifdef GL_TESTING
    friend struct gl_testing::test_adjacency_list;
#endif

private:
    void _remap_element_ids(id_type removed_vertex_id, std::vector<id_type>& removed_edge_ids) {
        std::ranges::sort(removed_edge_ids);
        removed_edge_ids.erase(
            std::ranges::unique(removed_edge_ids).begin(), removed_edge_ids.end()
        );

        for (auto&& inc : this->_list) {
            for (auto& edge_item : inc) {
                auto it = std::ranges::lower_bound(removed_edge_ids, edge_item.edge_id);
                if (it != removed_edge_ids.end() and *it == edge_item.edge_id)
                    edge_item.edge_id = invalid_id; // edge was removed
                else
                    // shift by the number of removed IDs < edge-id
                    edge_item.edge_id -= static_cast<id_type>(it - removed_edge_ids.begin());

                // align the vertex id
                edge_item.vertex_id -= edge_item.vertex_id > removed_vertex_id;
            }
        }
    }

    storage_type _list{};
};

} // namespace impl
} // namespace gl
