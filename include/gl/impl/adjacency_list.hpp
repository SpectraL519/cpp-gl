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

template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph_impl_tag SourceImplTag>
struct to_impl;

} // namespace detail

namespace impl {

template <traits::c_adjacency_list_graph_traits GraphTraits>
class adjacency_list final {
public:
    using implementation_tag = typename GraphTraits::implementation_tag;
    using id_type = typename GraphTraits::id_type;

    using vertex_type = typename GraphTraits::vertex_type;
    using edge_type = typename GraphTraits::edge_type;
    using item_type = specialized::incidence_item<id_type>;
    using adjacency_storage_type = typename specialized::adjacency_list_impl_traits<
        adjacency_list>::template storage_type<item_type>;

    adjacency_list() = default;

    explicit adjacency_list(size_type n_vertices) : _list(n_vertices) {}

    adjacency_list(const adjacency_list&) = default;
    adjacency_list& operator=(const adjacency_list&) = default;

    adjacency_list(adjacency_list&&) noexcept = default;
    adjacency_list& operator=(adjacency_list&&) noexcept = default;

    ~adjacency_list() = default;

    // --- vertex methods ---

    gl_attr_force_inline void add_vertex() {
        this->_list.resize(this->_list.size() + 1uz);
    }

    inline void add_vertices(size_type n) {
        this->_list.resize(this->_list.size() + n);
    }

    [[nodiscard]] gl_attr_force_inline size_type degree(id_type vertex_id) const {
        return specialized_impl::degree(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type in_degree(id_type vertex_id) const {
        return specialized_impl::in_degree(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type out_degree(id_type vertex_id) const {
        return specialized_impl::out_degree(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> degree_map() const {
        return specialized_impl::degree_map(*this);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> in_degree_map() const {
        return specialized_impl::in_degree_map(*this);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> out_degree_map() const {
        return specialized_impl::out_degree_map(*this);
    }

    std::vector<id_type> remove_vertex(id_type vertex_id) {
        auto removed_edge_ids = specialized_impl::remove_vertex(*this, vertex_id);
        this->_remap_element_ids(vertex_id, removed_edge_ids);
        return removed_edge_ids;
    }

    // --- edge methods ---

    gl_attr_force_inline void add_edge(id_type id, id_type source_id, id_type target_id) {
        specialized_impl::add_edge(*this, id, source_id, target_id);
    }

    gl_attr_force_inline void add_edges_from(
        const traits::c_forward_range_of<id_type> auto& edge_ids,
        id_type source_id,
        const traits::c_forward_range_of<id_type> auto& target_ids
    ) {
        specialized_impl::add_edges_from(*this, edge_ids, source_id, target_ids);
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(id_type source_id, id_type target_id) const {
        return std::ranges::contains(
            this->_list[to_idx(source_id)], target_id, &item_type::vertex_id
        );
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(const edge_type& edge) const {
        return std::ranges::contains(
            this->_list[to_idx(edge.source())], item_type{edge.target(), edge.id()}
        );
    }

    [[nodiscard]] std::optional<edge_type> get_edge(id_type source_id, id_type target_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        const auto& incident_edges = this->_list[to_idx(source_id)];
        const auto item_it = std::ranges::find(incident_edges, target_id, &item_type::vertex_id);
        if (item_it == incident_edges.cend())
            return std::nullopt;
        return std::make_optional<edge_type>(item_it->edge_id, source_id, target_id);
    }

    [[nodiscard]] std::optional<edge_type> get_edge(
        id_type source_id, id_type target_id, const auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        const auto& incident_edges = this->_list[to_idx(source_id)];
        const auto item_it = std::ranges::find(incident_edges, target_id, [](const auto& item) {
            return item.vertex_id;
        });
        if (item_it == incident_edges.cend())
            return std::nullopt;
        return std::make_optional<edge_type>(
            item_it->edge_id, source_id, target_id, *edge_properties_map[to_idx(item_it->edge_id)]
        );
    }

    [[nodiscard]] std::vector<edge_type> get_edges(id_type source_id, id_type target_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        return this->_list[source_id] | std::views::filter([&target_id](const auto& item) {
                   return item.vertex_id == target_id;
               })
             | std::views::transform([source_id](const auto& item) {
                   return edge_type{item.edge_id, source_id, item.vertex_id};
               })
             | std::ranges::to<std::vector>();
    }

    [[nodiscard]] std::vector<edge_type> get_edges(
        id_type source_id, id_type target_id, const auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        return this->_list[source_id] | std::views::filter([&target_id](const auto& item) {
                   return item.vertex_id == target_id;
               })
             | std::views::transform([source_id, &edge_properties_map](const auto& item) {
                   return edge_type{
                       item.edge_id, source_id, item.vertex_id, *edge_properties_map[item.edge_id]
                   };
               })
             | std::ranges::to<std::vector>();
    }

    gl_attr_force_inline void remove_edge(const edge_type& edge) {
        specialized_impl::remove_edge(*this, edge);
        for (auto&& inc : this->_list)
            for (auto& item : inc)
                item.edge_id -= static_cast<id_type>(item.edge_id > edge.id());
    }

    std::vector<id_type> remove_edges(const traits::c_range_of<edge_type> auto& edges) {
        for (const auto& edge : edges)
            specialized_impl::remove_edge(*this, edge);
        auto removed_edge_ids =
            edges | std::views::transform([](const auto& edge) { return edge.id(); })
            | std::ranges::to<std::vector>();
        this->_remap_element_ids(invalid_id, removed_edge_ids);
        return removed_edge_ids;
    }

    [[nodiscard]] gl_attr_force_inline auto incident_edges(id_type vertex_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        return this->out_edges(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_edges(
        id_type vertex_id, const auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        return this->out_edges(vertex_id, edge_properties_map);
    }

    [[nodiscard]] gl_attr_force_inline auto in_edges(id_type vertex_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        return specialized_impl::in_edges(*this, vertex_id)
             | std::views::transform([vertex_id](const auto& item) {
                   return edge_type{item.edge_id, item.vertex_id, vertex_id};
               });
    }

    [[nodiscard]] gl_attr_force_inline auto in_edges(
        id_type vertex_id, const auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        return specialized_impl::in_edges(*this, vertex_id)
             | std::views::transform([vertex_id, &edge_properties_map](const auto& item) {
                   return edge_type{
                       item.edge_id,
                       item.vertex_id,
                       vertex_id,
                       *edge_properties_map[to_idx(item.edge_id)]
                   };
               });
    }

    [[nodiscard]] gl_attr_force_inline auto out_edges(id_type vertex_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        return this->_list[to_idx(vertex_id)]
             | std::views::transform([vertex_id](const auto& item) {
                   return edge_type{item.edge_id, vertex_id, item.vertex_id};
               });
    }

    [[nodiscard]] gl_attr_force_inline auto out_edges(
        id_type vertex_id, const auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        return this->_list[to_idx(vertex_id)]
             | std::views::transform([vertex_id, &edge_properties_map](const auto& item) {
                   return edge_type{
                       item.edge_id,
                       vertex_id,
                       item.vertex_id,
                       *edge_properties_map[to_idx(item.edge_id)]
                   };
               });
    }

    // --- access operators ---

    [[nodiscard]] gl_attr_force_inline auto at(id_type vertex_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        return this->incident_edges(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto at(id_type vertex_id, const auto& edge_properties_map)
        const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        return this->incident_edges(vertex_id, edge_properties_map);
    }

    // --- comparison ---

    [[nodiscard]] friend bool operator==(const adjacency_list&, const adjacency_list&) = default;

    // --- friend declarations ---

    template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph_impl_tag SourceImplTag>
    friend struct gl::detail::to_impl;

#ifdef GL_TESTING
    friend struct gl_testing::test_adjacency_list;
#endif

private:
    using specialized_impl = typename specialized::adjacency_list_impl_traits<adjacency_list>::type;
    friend specialized_impl;

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

    adjacency_storage_type _list{};
};

} // namespace impl
} // namespace gl
