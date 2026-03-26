// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/impl/specialized/adjacency_matrix.hpp"
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
    using row_type = std::vector<id_type>;
    using matrix_type = std::vector<row_type>;

    adjacency_matrix() = default;

    explicit adjacency_matrix(size_type n_vertices) : _matrix(n_vertices) {
        // initialize a full n x n matrix with null elements
        for (auto& row : this->_matrix)
            row.resize(n_vertices, constants::invalid_id<id_type>);
    }

    adjacency_matrix(const adjacency_matrix&) = default;
    adjacency_matrix& operator=(const adjacency_matrix&) = default;

    adjacency_matrix(adjacency_matrix&&) noexcept = default;
    adjacency_matrix& operator=(adjacency_matrix&&) noexcept = default;

    ~adjacency_matrix() = default;

    // --- vertex methods ---

    void add_vertex() {
        for (auto& row : this->_matrix)
            row.push_back(constants::invalid_id<id_type>);
        this->_matrix.emplace_back(this->_matrix.size() + 1uz, constants::invalid_id<id_type>);
    }

    void add_vertices(size_type n) {
        const auto new_n_vertices = this->_matrix.size() + n;

        for (auto& row : this->_matrix)
            row.resize(new_n_vertices, constants::invalid_id<id_type>);

        for (auto _ = 0uz; _ < n; ++_)
            this->_matrix.emplace_back(new_n_vertices, constants::invalid_id<id_type>);
    }

    [[nodiscard]] gl_attr_force_inline size_type in_degree(id_type vertex_id) const {
        return specialized_impl::in_degree(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type out_degree(id_type vertex_id) const {
        return specialized_impl::out_degree(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type degree(id_type vertex_id) const {
        return specialized_impl::degree(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> in_degree_map() const {
        return specialized_impl::in_degree_map(*this);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> out_degree_map() const {
        return specialized_impl::out_degree_map(*this);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> degree_map() const {
        return specialized_impl::degree_map(*this);
    }

    std::vector<id_type> remove_vertex(id_type vertex_id) {
        auto removed_edge_ids = specialized_impl::remove_vertex(*this, vertex_id);
        this->_remap_element_ids(removed_edge_ids);
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
        return this->_matrix[source_id][target_id] != constants::invalid_id<id_type>;
    }

    [[nodiscard]] bool has_edge(const edge_type& edge) const {
        return this->_matrix[edge.source()][edge.target()] == edge.id();
    }

    [[nodiscard]] std::optional<edge_type> get_edge(id_type source_id, id_type target_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        const auto edge_id = this->_matrix[source_id][target_id];
        if (edge_id == constants::invalid_id<id_type>)
            return std::nullopt;
        return std::make_optional<edge_type>(edge_id, source_id, target_id);
    }

    [[nodiscard]] std::optional<edge_type> get_edge(
        id_type source_id, id_type target_id, const auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        const auto edge_id = this->_matrix[source_id][target_id];
        if (edge_id == constants::invalid_id<id_type>)
            return std::nullopt;
        return std::make_optional<edge_type>(
            edge_id, source_id, target_id, *edge_properties_map[edge_id]
        );
    }

    [[nodiscard]] std::vector<edge_type> get_edges(id_type source_id, id_type target_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        const auto edge_id = this->_matrix[source_id][target_id];
        if (edge_id == constants::invalid_id<id_type>)
            return std::vector<edge_type>();
        return std::vector<edge_type>{
            edge_type{edge_id, source_id, target_id}
        };
    }

    [[nodiscard]] std::vector<edge_type> get_edges(
        id_type source_id, id_type target_id, const auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        const auto edge_id = this->_matrix[source_id][target_id];
        if (edge_id == constants::invalid_id<id_type>)
            return std::vector<edge_type>();
        return std::vector<edge_type>{
            edge_type{edge_id, source_id, target_id, *edge_properties_map[edge_id]}
        };
    }

    gl_attr_force_inline void remove_edge(const edge_type& edge) {
        specialized_impl::remove_edge(*this, edge);
        for (auto& row : this->_matrix)
            for (auto& edge_id : row)
                if (edge_id != constants::invalid_id<id_type> and edge_id > edge.id())
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

    [[nodiscard]] gl_attr_force_inline auto adjacent_edges(id_type vertex_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        return this->out_edges(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto adjacent_edges(
        id_type vertex_id, const auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        return this->out_edges(vertex_id, edge_properties_map);
    }

    [[nodiscard]] gl_attr_force_inline auto in_edges(id_type vertex_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        return std::views::iota(constants::initial_id<id_type>, this->_matrix.size())
             | std::views::filter([this, vertex_id](const auto source_id) {
                   return this->_matrix[source_id][vertex_id] != constants::invalid_id<id_type>;
               })
             | std::views::transform([this, vertex_id](const auto source_id) {
                   return edge_type{this->_matrix[source_id][vertex_id], source_id, vertex_id};
               });
    }

    [[nodiscard]] gl_attr_force_inline auto in_edges(
        id_type vertex_id, const auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        return std::views::iota(constants::initial_id<id_type>, this->_matrix.size())
             | std::views::filter([this, vertex_id](const auto source_id) {
                   return this->_matrix[source_id][vertex_id] != constants::invalid_id<id_type>;
               })
             | std::views::transform([this, vertex_id, &edge_properties_map](const auto source_id) {
                   const auto edge_id = this->_matrix[source_id][vertex_id];
                   return edge_type{edge_id, source_id, vertex_id, *edge_properties_map[edge_id]};
               });
    }

    [[nodiscard]] gl_attr_force_inline auto out_edges(id_type vertex_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        return this->_matrix[vertex_id] | std::views::enumerate
             | std::views::filter([](const auto& edge_info) {
                   const auto& [target_id, edge_id] = edge_info;
                   return edge_id != constants::invalid_id<id_type>;
               })
             | std::views::transform([vertex_id](const auto& edge_info) {
                   const auto& [target_id, edge_id] = edge_info;
                   return edge_type{edge_id, vertex_id, static_cast<id_type>(target_id)};
               });
    }

    [[nodiscard]] gl_attr_force_inline auto out_edges(
        id_type vertex_id, const auto& edge_properties_map
    ) const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        return this->_matrix[vertex_id] | std::views::enumerate
             | std::views::filter([](const auto& edge_info) {
                   const auto& [target_id, edge_id] = edge_info;
                   return edge_id != constants::invalid_id<id_type>;
               })
             | std::views::transform([vertex_id, &edge_properties_map](const auto& edge_info) {
                   const auto& [target_id, edge_id] = edge_info;
                   return edge_type{
                       edge_id,
                       vertex_id,
                       static_cast<id_type>(target_id),
                       *edge_properties_map[edge_id]
                   };
               });
    }

    // --- access operators ---

    [[nodiscard]] gl_attr_force_inline auto at(id_type vertex_id) const
    requires(traits::c_has_empty_properties<edge_type>)
    {
        return this->_matrix[vertex_id] | std::views::enumerate
             | std::views::transform([vertex_id](const auto& edge_info) {
                   const auto& [target_id, edge_id] = edge_info;
                   return edge_id == constants::invalid_id<id_type>
                            ? edge_type::invalid()
                            : edge_type{edge_id, vertex_id, static_cast<id_type>(target_id)};
               });
    }

    [[nodiscard]] gl_attr_force_inline auto at(id_type vertex_id, const auto& edge_properties_map)
        const
    requires(traits::c_has_non_empty_properties<edge_type>)
    {
        return this->_matrix[vertex_id] | std::views::enumerate
             | std::views::transform([vertex_id, &edge_properties_map](const auto& edge_info) {
                   const auto& [target_id, edge_id] = edge_info;
                   return edge_id == constants::invalid_id<id_type>
                            ? edge_type::invalid()
                            : edge_type{
                                  edge_id,
                                  vertex_id,
                                  static_cast<id_type>(target_id),
                                  *edge_properties_map[edge_id]
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

        for (auto& row : this->_matrix) {
            for (auto& edge_id : row) {
                if (edge_id == constants::invalid_id<id_type>)
                    continue;

                auto it = std::ranges::lower_bound(removed_edge_ids, edge_id);
                if (it != removed_edge_ids.end() and *it == edge_id)
                    edge_id = constants::invalid_id<id_type>; // edge was removed
                else
                    // shift by the number of removed IDs < edge-id
                    edge_id -= std::ranges::distance(removed_edge_ids.begin(), it);
            }
        }
    }

    matrix_type _matrix{};
};

} // namespace impl
} // namespace gl
