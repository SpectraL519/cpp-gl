// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/types/core.hpp"
#include "hgl/constants.hpp"
#include "hgl/decl/impl_tags.hpp"
#include "hgl/directional_tags.hpp"
#include "hgl/impl/bf_incidence.hpp"
#include "hgl/impl/layout_tags.hpp"
#include "hgl/types.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>

#ifdef HGL_TESTING
namespace hgl_testing {
struct test_flat_incidence_matrix;
} // namespace hgl_testing
#endif

namespace hgl {

namespace detail {

template <traits::c_hypergraph_impl_tag TargetImplTag, traits::c_hypergraph_impl_tag SourceImplTag>
struct to_impl;

} // namespace detail

namespace impl {

template <
    traits::c_hypergraph_directional_tag DirectionalTag,
    traits::c_hypergraph_flat_matrix_impl ImplTag>
class flat_incidence_matrix;

template <traits::c_hypergraph_flat_matrix_impl ImplTag>
requires traits::c_hypergraph_asymmetric_layout_tag<typename ImplTag::layout_tag>
class flat_incidence_matrix<hgl::undirected_t, ImplTag> final {
public:
    using directional_tag = hgl::undirected_t;
    using implementation_tag = ImplTag;
    using layout_tag = typename implementation_tag::layout_tag;
    using id_type = typename implementation_tag::id_type;

    flat_incidence_matrix() = default;

    flat_incidence_matrix(const size_type n_vertices, const size_type n_hyperedges)
    : _matrix(
          layout_tag::major(n_vertices, n_hyperedges),
          layout_tag::minor(n_vertices, n_hyperedges),
          false
      ) {}

    flat_incidence_matrix(const flat_incidence_matrix&) = default;
    flat_incidence_matrix& operator=(const flat_incidence_matrix&) = default;

    flat_incidence_matrix(flat_incidence_matrix&&) noexcept = default;
    flat_incidence_matrix& operator=(flat_incidence_matrix&&) noexcept = default;

    ~flat_incidence_matrix() = default;

    // --- vertex methods ---

    gl_attr_force_inline void add_vertices(const size_type n) noexcept {
        this->_add<element_type::vertex>(n);
    }

    gl_attr_force_inline void remove_vertex(const id_type vertex_id) noexcept {
        this->_remove<element_type::vertex>(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const id_type vertex_id
    ) const noexcept {
        return this->_incident_with<element_type::vertex>(vertex_id);
    }

    [[nodiscard]] size_type degree(const id_type vertex_id) const noexcept {
        return this->_count<element_type::vertex>(vertex_id);
    }

    [[nodiscard]] std::vector<size_type> degree_map(const size_type n_vertices) const noexcept {
        return this->_count_map<element_type::vertex>(n_vertices);
    }

    // --- hyperedge methods ---

    gl_attr_force_inline void add_hyperedges(const size_type n) noexcept {
        this->_add<element_type::hyperedge>(n);
    }

    gl_attr_force_inline void remove_hyperedge(const id_type hyperedge_id) noexcept {
        this->_remove<element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const id_type hyperedge_id
    ) const noexcept {
        return this->_incident_with<element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] size_type hyperedge_size(const id_type hyperedge_id) const noexcept {
        return this->_count<element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] std::vector<size_type> hyperedge_size_map(const size_type n_hyperedges
    ) const noexcept {
        return this->_count_map<element_type::hyperedge>(n_hyperedges);
    }

    // --- binding methods ---

    gl_attr_force_inline void bind(const id_type vertex_id, const id_type hyperedge_id) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[to_idx(major_id), to_idx(minor_id)] = true;
    }

    gl_attr_force_inline void unbind(const id_type vertex_id, const id_type hyperedge_id) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[to_idx(major_id), to_idx(minor_id)] = false;
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[to_idx(major_id), to_idx(minor_id)];
    }

    // --- comparison ---

    [[nodiscard]] friend bool
    operator==(const flat_incidence_matrix&, const flat_incidence_matrix&) = default;

    // --- friend declarations ---

    template <
        traits::c_hypergraph_impl_tag TargetImplTag,
        traits::c_hypergraph_impl_tag SourceImplTag>
    friend struct hgl::detail::to_impl;

#ifdef HGL_TESTING
    friend struct hgl_testing::test_flat_incidence_matrix;
#endif

private:
    // using matrix_row_type = std::vector<bool>;
    using hypergraph_storage_type = flat_matrix<bool>;

    template <element_type Element>
    void _add(const size_type n) noexcept {
        if constexpr (Element == layout_tag::major_element) // add major
            this->_matrix.resize(this->_matrix.n_rows() + n, this->_matrix.n_cols(), false);
        else // add minor
            this->_matrix.resize(this->_matrix.n_rows(), this->_matrix.n_cols() + n, false);
    }

    template <element_type Element>
    void _remove(const id_type id) noexcept {
        if constexpr (Element == layout_tag::major_element) // remove major
            this->_matrix.erase_row(to_idx(id));
        else // remove minor
            this->_matrix.erase_col(to_idx(id));
    }

    template <element_type Element>
    gl_attr_force_inline auto _incident_with(const id_type id) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // incident with major
            return std::views::iota(initial_id_v<id_type>, this->_matrix.n_cols())
                 | std::views::filter([row = this->_matrix[to_idx(id)]](const id_type minor_id) {
                       return row[to_diff(minor_id)];
                   });
        }
        else { // incident with minor
            return std::views::iota(initial_id_v<id_type>, this->_matrix.n_rows())
                 | std::views::filter([this, minor_idx = to_idx(id)](const id_type major_id) {
                       return this->_matrix[to_idx(major_id), minor_idx];
                   });
        }
    }

    template <element_type Element>
    [[nodiscard]] gl_attr_force_inline size_type _count(const id_type id) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // count major
            return static_cast<size_type>(std::ranges::count(this->_matrix[to_idx(id)], true));
        }
        else { // count minor
            const auto pos = to_diff(id);
            size_type count = 0uz;
            for (const auto& row : this->_matrix)
                count += static_cast<size_type>(row[pos]);
            return count;
        }
    }

    template <element_type Element>
    [[nodiscard]] std::vector<size_type> _count_map(const size_type n_elements) const noexcept {
        std::vector<size_type> size_map(n_elements, 0uz);

        if constexpr (Element == layout_tag::major_element) { // count map major
            const size_type limit = std::min(n_elements, this->_matrix.n_rows());
            for (auto i = 0uz; i < limit; ++i)
                size_map[i] = static_cast<size_type>(std::ranges::count(this->_matrix[i], true));
        }
        else { // count map minor
            const size_type limit = std::min(n_elements, this->_matrix.n_cols());
            for (const auto& row : this->_matrix)
                for (auto j = 0uz; j < limit; ++j)
                    size_map[j] += static_cast<size_type>(row[to_diff(j)]);
        }
        return size_map;
    }

    hypergraph_storage_type _matrix;
};

template <traits::c_hypergraph_flat_matrix_impl ImplTag>
requires traits::c_hypergraph_asymmetric_layout_tag<typename ImplTag::layout_tag>
class flat_incidence_matrix<hgl::bf_directed_t, ImplTag> final {
public:
    using directional_tag = hgl::bf_directed_t;
    using implementation_tag = ImplTag;
    using layout_tag = typename implementation_tag::layout_tag;
    using id_type = typename implementation_tag::id_type;

public:
    flat_incidence_matrix() = default;

    flat_incidence_matrix(const size_type n_vertices, const size_type n_hyperedges)
    : _matrix(
          layout_tag::major(n_vertices, n_hyperedges),
          layout_tag::minor(n_vertices, n_hyperedges),
          bf_incidence::none
      ) {}

    flat_incidence_matrix(const flat_incidence_matrix&) = default;
    flat_incidence_matrix& operator=(const flat_incidence_matrix&) = default;

    flat_incidence_matrix(flat_incidence_matrix&&) noexcept = default;
    flat_incidence_matrix& operator=(flat_incidence_matrix&&) noexcept = default;

    ~flat_incidence_matrix() = default;

    // --- vertex methods : general ---

    gl_attr_force_inline void add_vertices(const size_type n) noexcept {
        this->_add<element_type::vertex>(n);
    }

    gl_attr_force_inline void remove_vertex(const id_type vertex_id) noexcept {
        this->_remove<element_type::vertex>(vertex_id);
    }

    // --- vertex methods : incidence queries ---

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const id_type vertex_id
    ) const noexcept {
        return this->_query<element_type::vertex>(vertex_id, bf_is_incident);
    }

    [[nodiscard]] size_type degree(const id_type vertex_id) const noexcept {
        return this->_count<element_type::vertex>(vertex_id, bf_is_incident);
    }

    [[nodiscard]] std::vector<size_type> degree_map(const size_type n_vertices) const noexcept {
        return this->_count_map<element_type::vertex>(n_vertices, bf_is_incident);
    }

    [[nodiscard]] gl_attr_force_inline auto out_hyperedges(const id_type vertex_id) const noexcept {
        return this->_query<element_type::vertex>(vertex_id, bf_is_tail);
    }

    [[nodiscard]] size_type out_degree(const id_type vertex_id) const noexcept {
        return this->_count<element_type::vertex>(vertex_id, bf_is_tail);
    }

    [[nodiscard]] std::vector<size_type> out_degree_map(const size_type n_vertices) const noexcept {
        return this->_count_map<element_type::vertex>(n_vertices, bf_is_tail);
    }

    [[nodiscard]] gl_attr_force_inline auto in_hyperedges(const id_type vertex_id) const noexcept {
        return this->_query<element_type::vertex>(vertex_id, bf_is_head);
    }

    [[nodiscard]] size_type in_degree(const id_type vertex_id) const noexcept {
        return this->_count<element_type::vertex>(vertex_id, bf_is_head);
    }

    [[nodiscard]] std::vector<size_type> in_degree_map(const size_type n_vertices) const noexcept {
        return this->_count_map<element_type::vertex>(n_vertices, bf_is_head);
    }

    // --- hyperedge methods : general ---

    gl_attr_force_inline void add_hyperedges(const size_type n) noexcept {
        this->_add<element_type::hyperedge>(n);
    }

    gl_attr_force_inline void remove_hyperedge(const id_type hyperedge_id) noexcept {
        this->_remove<element_type::hyperedge>(hyperedge_id);
    }

    // --- hyperedge methods : incidence queries ---

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const id_type hyperedge_id
    ) const noexcept {
        return this->_query<element_type::hyperedge>(hyperedge_id, bf_is_incident);
    }

    [[nodiscard]] size_type hyperedge_size(const id_type hyperedge_id) const noexcept {
        return this->_count<element_type::hyperedge>(hyperedge_id, bf_is_incident);
    }

    [[nodiscard]] std::vector<size_type> hyperedge_size_map(const size_type n_hyperedges
    ) const noexcept {
        return this->_count_map<element_type::hyperedge>(n_hyperedges, bf_is_incident);
    }

    [[nodiscard]] gl_attr_force_inline auto tail_vertices(const id_type hyperedge_id
    ) const noexcept {
        return this->_query<element_type::hyperedge>(hyperedge_id, bf_is_tail);
    }

    [[nodiscard]] size_type tail_size(const id_type hyperedge_id) const noexcept {
        return this->_count<element_type::hyperedge>(hyperedge_id, bf_is_tail);
    }

    [[nodiscard]] std::vector<size_type> tail_size_map(const size_type n_hyperedges
    ) const noexcept {
        return this->_count_map<element_type::hyperedge>(n_hyperedges, bf_is_tail);
    }

    [[nodiscard]] gl_attr_force_inline auto head_vertices(const id_type hyperedge_id
    ) const noexcept {
        return this->_query<element_type::hyperedge>(hyperedge_id, bf_is_head);
    }

    [[nodiscard]] size_type head_size(const id_type hyperedge_id) const noexcept {
        return this->_count<element_type::hyperedge>(hyperedge_id, bf_is_head);
    }

    [[nodiscard]] std::vector<size_type> head_size_map(const size_type n_hyperedges
    ) const noexcept {
        return this->_count_map<element_type::hyperedge>(n_hyperedges, bf_is_head);
    }

    // --- binding methods ---

    gl_attr_force_inline void bind_tail(
        const id_type vertex_id, const id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[to_idx(major_id), to_idx(minor_id)] = bf_incidence::backward;
    }

    gl_attr_force_inline void bind_head(
        const id_type vertex_id, const id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[to_idx(major_id), to_idx(minor_id)] = bf_incidence::forward;
    }

    gl_attr_force_inline void unbind(const id_type vertex_id, const id_type hyperedge_id) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[to_idx(major_id), to_idx(minor_id)] = bf_incidence::none;
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[to_idx(major_id), to_idx(minor_id)] != bf_incidence::none;
    }

    [[nodiscard]] gl_attr_force_inline bool is_tail(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[to_idx(major_id), to_idx(minor_id)] == bf_incidence::backward;
    }

    [[nodiscard]] gl_attr_force_inline bool is_head(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[to_idx(major_id), to_idx(minor_id)] == bf_incidence::forward;
    }

    // --- comparison ---

    [[nodiscard]] friend bool
    operator==(const flat_incidence_matrix&, const flat_incidence_matrix&) = default;

    // --- friend declarations ---

    template <
        traits::c_hypergraph_impl_tag TargetImplTag,
        traits::c_hypergraph_impl_tag SourceImplTag>
    friend struct hgl::detail::to_impl;

#ifdef HGL_TESTING
    friend struct hgl_testing::test_flat_incidence_matrix;
#endif

private:
    // --- storage management ---

    using hypergraph_storage_type = flat_matrix<bf_incidence>;

    template <element_type Element>
    void _add(const size_type n) noexcept {
        if constexpr (Element == layout_tag::major_element) // add major
            this->_matrix.resize(
                this->_matrix.n_rows() + n, this->_matrix.n_cols(), bf_incidence::none
            );
        else // add minor
            this->_matrix.resize(
                this->_matrix.n_rows(), this->_matrix.n_cols() + n, bf_incidence::none
            );
    }

    template <element_type Element>
    void _remove(const id_type id) noexcept {
        if constexpr (Element == layout_tag::major_element) // remove major
            this->_matrix.erase_row(to_idx(id));
        else // remove minor
            this->_matrix.erase_col(to_idx(id));
    }

    template <element_type Element>
    [[nodiscard]] gl_attr_force_inline auto _query(
        const id_type id, std::predicate<bf_incidence> auto&& pred
    ) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // query major
            return std::views::iota(initial_id_v<id_type>, this->_matrix.n_cols())
                 | std::views::filter(
                       [row = this->_matrix[to_idx(id)],
                        pred = std::forward<decltype(pred)>(pred)](const id_type minor_id) {
                           return pred(row[to_diff(minor_id)]);
                       }
                 );
        }
        else { // query minor
            return std::views::iota(initial_id_v<id_type>, this->_matrix.n_rows())
                 | std::views::filter(
                       [this, minor_idx = to_idx(id), pred = std::forward<decltype(pred)>(pred)](
                           const id_type major_id
                       ) { return pred(this->_matrix[to_idx(major_id), minor_idx]); }
                 );
        }
    }

    template <element_type Element>
    [[nodiscard]] gl_attr_force_inline size_type
    _count(const id_type id, std::predicate<bf_incidence> auto&& pred) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // count major
            size_type count = 0uz;
            for (const bf_incidence t : this->_matrix[to_idx(id)])
                count += static_cast<size_type>(pred(t));
            return count;
        }
        else { // count minor
            const auto pos = to_diff(id);
            size_type count = 0uz;
            for (const auto& row : this->_matrix)
                count += static_cast<size_type>(pred(row[pos]));
            return count;
        }
    }

    template <element_type Element>
    [[nodiscard]] std::vector<size_type> _count_map(
        const size_type n_elements, std::predicate<bf_incidence> auto&& pred
    ) const noexcept {
        std::vector<size_type> size_map(n_elements, 0uz);

        if constexpr (Element == layout_tag::major_element) { // count map major
            const size_type limit = std::min(n_elements, this->_matrix.n_rows());
            for (auto i = 0uz; i < limit; ++i)
                for (const bf_incidence val : this->_matrix[i])
                    size_map[i] += static_cast<size_type>(pred(val));
        }
        else { // count map minor
            const size_type limit = std::min(n_elements, this->_matrix.n_cols());
            for (const auto& row : this->_matrix)
                for (auto j = 0uz; j < limit; ++j)
                    size_map[j] += static_cast<size_type>(pred(row[to_diff(j)]));
        }
        return size_map;
    }

    hypergraph_storage_type _matrix;
};

} // namespace impl
} // namespace hgl
