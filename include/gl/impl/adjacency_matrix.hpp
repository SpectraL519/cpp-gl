// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/constants.hpp"
#include "gl/impl/specialized/adjacency_matrix.hpp"
#include "gl/impl/specialized/flat_adjacency_matrix.hpp"
#include "gl/types/core.hpp"

#ifdef GL_TESTING
namespace gl_testing {
struct test_adjacency_matrix;
} // namespace gl_testing
#endif

namespace gl {

namespace detail {

template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph_impl_tag SourceImplTag>
struct to_impl;

} // namespace detail

namespace impl {

template <traits::c_adjacency_matrix_graph_traits GraphTraits>
class adjacency_matrix final {
public:
    using implementation_tag = typename GraphTraits::implementation_tag;
    using id_type = typename GraphTraits::id_type;

    using vertex_type = typename GraphTraits::vertex_type;
    using edge_type = typename GraphTraits::edge_type;

    using adjacency_storage_type = typename specialized::adjacency_matrix_impl_traits<
        adjacency_matrix>::template storage_type<id_type>;

    adjacency_matrix() = default;

    explicit adjacency_matrix(size_type n_vertices) {
        specialized_impl::init(*this, n_vertices);
    }

    adjacency_matrix(const adjacency_matrix&) = default;
    adjacency_matrix& operator=(const adjacency_matrix&) = default;

    adjacency_matrix(adjacency_matrix&&) noexcept = default;
    adjacency_matrix& operator=(adjacency_matrix&&) noexcept = default;

    ~adjacency_matrix() = default;

    // --- vertex modifiers ---

    gl_attr_force_inline void add_vertex() {
        specialized_impl::add_vertex(*this);
    }

    gl_attr_force_inline void add_vertices(size_type n) {
        specialized_impl::add_vertices(*this, n);
    }

    std::vector<id_type> remove_vertex(id_type vertex_id) {
        auto removed_edge_ids = specialized_impl::remove_vertex(*this, vertex_id);
        this->_remap_element_ids(removed_edge_ids);
        return removed_edge_ids;
    }

    // --- vertex getters ---

    [[nodiscard]] gl_attr_force_inline auto neighbor_ids(id_type vertex_id) const {
        return specialized_impl::neighbor_ids(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto predecessor_ids(id_type vertex_id) const {
        return specialized_impl::predecessor_ids(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto successor_ids(id_type vertex_id) const {
        return specialized_impl::successor_ids(*this, vertex_id);
    }

    // --- degree getters ---

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> degree_map() const {
        return specialized_impl::degree_map(*this);
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

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> in_degree_map() const {
        return specialized_impl::in_degree_map(*this);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> out_degree_map() const {
        return specialized_impl::out_degree_map(*this);
    }

    // --- edge modifiers ---

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

    void remove_edge(const edge_type& edge) {
        specialized_impl::remove_edge(*this, edge);
        for (auto&& row : this->_matrix)
            for (auto& edge_id : row)
                if (edge_id != invalid_id and edge_id > edge.id())
                    edge_id--;
    }

    std::vector<id_type> remove_edges(const traits::c_range_of<edge_type> auto& edges) {
        for (const auto& edge : edges)
            specialized_impl::remove_edge(*this, edge);
        auto removed_edge_ids =
            edges | std::views::transform([](const auto& edge) { return edge.id(); })
            | std::ranges::to<std::vector>();
        this->_remap_element_ids(removed_edge_ids);
        return removed_edge_ids;
    }

    // --- edge getters ---

    [[nodiscard]] gl_attr_force_inline bool has_edge(id_type source_id, id_type target_id) const {
        return specialized_impl::get_entry(*this, source_id, target_id) != invalid_id;
    }

    [[nodiscard]] bool has_edge(const edge_type& edge) const {
        return specialized_impl::get_entry(*this, edge.source(), edge.target()) == edge.id();
    }

    [[nodiscard]] std::optional<edge_type> edge(id_type source_id, id_type target_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        const auto edge_id = specialized_impl::get_entry(*this, source_id, target_id);
        if (edge_id == invalid_id)
            return std::nullopt;
        return std::make_optional<edge_type>(edge_id, source_id, target_id);
    }

    [[nodiscard]] std::optional<edge_type> edge(
        id_type source_id, id_type target_id, auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        const auto edge_id = specialized_impl::get_entry(*this, source_id, target_id);
        if (edge_id == invalid_id)
            return std::nullopt;
        return std::make_optional<edge_type>(
            edge_id, source_id, target_id, edge_properties_map[to_idx(edge_id)]
        );
    }

    [[nodiscard]] std::vector<edge_type> edges(id_type source_id, id_type target_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        const auto edge_id = specialized_impl::get_entry(*this, source_id, target_id);
        if (edge_id == invalid_id)
            return std::vector<edge_type>();
        return std::vector<edge_type>{
            edge_type{edge_id, source_id, target_id}
        };
    }

    [[nodiscard]] std::vector<edge_type> edges(
        id_type source_id, id_type target_id, auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        const auto edge_id = specialized_impl::get_entry(*this, source_id, target_id);
        if (edge_id == invalid_id)
            return std::vector<edge_type>();
        return std::vector<edge_type>{
            edge_type{edge_id, source_id, target_id, edge_properties_map[to_idx(edge_id)]}
        };
    }

    [[nodiscard]] gl_attr_force_inline auto incident_edges(id_type vertex_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        return specialized_impl::incident_edges(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_edges(
        id_type vertex_id, auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        return specialized_impl::incident_edges(*this, vertex_id, edge_properties_map);
    }

    [[nodiscard]] gl_attr_force_inline auto in_edges(id_type vertex_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        return std::views::iota(initial_id_v<id_type>, this->_matrix.size())
             | std::views::filter([this, vertex_id](auto source_id) {
                   return specialized_impl::get_entry(*this, source_id, vertex_id) != invalid_id;
               })
             | std::views::transform([this, vertex_id](auto source_id) {
                   return edge_type{
                       specialized_impl::get_entry(*this, source_id, vertex_id),
                       source_id,
                       vertex_id
                   };
               });
    }

    [[nodiscard]] gl_attr_force_inline auto in_edges(id_type vertex_id, auto& edge_properties_map)
        const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        return std::views::iota(initial_id_v<id_type>, this->_matrix.size())
             | std::views::filter([this, vertex_id](auto source_id) {
                   return specialized_impl::get_entry(*this, source_id, vertex_id) != invalid_id;
               })
             | std::views::transform([this, vertex_id, &edge_properties_map](auto source_id) {
                   const auto edge_id = specialized_impl::get_entry(*this, source_id, vertex_id);
                   return edge_type{
                       edge_id, source_id, vertex_id, edge_properties_map[to_idx(edge_id)]
                   };
               });
    }

    [[nodiscard]] gl_attr_force_inline auto out_edges(id_type vertex_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        return this->_matrix[to_idx(vertex_id)] | std::views::enumerate
             | std::views::filter([](auto entry) {
                   auto [_, edge_id] = entry;
                   return edge_id != invalid_id;
               })
             | std::views::transform([vertex_id](auto entry) {
                   auto [target_id, edge_id] = entry;
                   return edge_type{edge_id, vertex_id, static_cast<id_type>(target_id)};
               });
    }

    [[nodiscard]] gl_attr_force_inline auto out_edges(id_type vertex_id, auto& edge_properties_map)
        const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        return this->_matrix[to_idx(vertex_id)] | std::views::enumerate
             | std::views::filter([](auto entry) {
                   const auto [_, edge_id] = entry;
                   return edge_id != invalid_id;
               })
             | std::views::transform([vertex_id, &edge_properties_map](auto entry) {
                   const auto [target_id, edge_id] = entry;
                   return edge_type{
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

    template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph_impl_tag SourceImplTag>
    friend struct gl::detail::to_impl;

#ifdef GL_TESTING
    friend struct gl_testing::test_adjacency_matrix;
#endif

private:
    using specialized_impl =
        typename specialized::adjacency_matrix_impl_traits<adjacency_matrix>::type;
    friend specialized_impl;

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

    adjacency_storage_type _matrix{};
};

} // namespace impl
} // namespace gl
