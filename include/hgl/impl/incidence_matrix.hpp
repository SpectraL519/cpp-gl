// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/types/core.hpp"
#include "hgl/constants.hpp"
#include "hgl/decl/impl_tags.hpp"
#include "hgl/directional_tags.hpp"
#include "hgl/impl/layout_tags.hpp"
#include "hgl/types.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <vector>

#ifdef HGL_TESTING
namespace hgl_testing {
struct test_incidence_matrix;
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
    traits::c_hypergraph_matrix_impl ImplTag>
class incidence_matrix;

template <traits::c_hypergraph_matrix_impl ImplTag>
requires traits::c_hypergraph_asymmetric_layout_tag<typename ImplTag::layout_tag>
class incidence_matrix<hgl::undirected_t, ImplTag> final {
public:
    using directional_tag = hgl::undirected_t;
    using implementation_tag = ImplTag;
    using layout_tag = typename implementation_tag::layout_tag;
    using id_type = typename implementation_tag::id_type;

    incidence_matrix() = default;

    incidence_matrix(const size_type n_vertices, const size_type n_hyperedges)
    : _matrix_row_size{layout_tag::minor(n_vertices, n_hyperedges)},
      _matrix(
          layout_tag::major(n_vertices, n_hyperedges), matrix_row_type(_matrix_row_size, false)
      ) {}

    incidence_matrix(const incidence_matrix&) = default;
    incidence_matrix& operator=(const incidence_matrix&) = default;

    incidence_matrix(incidence_matrix&&) noexcept = default;
    incidence_matrix& operator=(incidence_matrix&&) noexcept = default;

    ~incidence_matrix() = default;

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
        this->_matrix[to_idx(major_id)][to_idx(minor_id)] = true;
    }

    gl_attr_force_inline void unbind(const id_type vertex_id, const id_type hyperedge_id) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[to_idx(major_id)][to_idx(minor_id)] = false;
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[to_idx(major_id)][to_idx(minor_id)];
    }

    // --- comparison ---

    [[nodiscard]] friend bool operator==(const incidence_matrix&, const incidence_matrix&) =
        default;

    // --- friend declarations ---

    template <
        traits::c_hypergraph_impl_tag TargetImplTag,
        traits::c_hypergraph_impl_tag SourceImplTag>
    friend struct hgl::detail::to_impl;

#ifdef HGL_TESTING
    friend struct hgl_testing::test_incidence_matrix;
#endif

private:
    using matrix_row_type = std::vector<bool>;
    using hypergraph_storage_type = std::vector<matrix_row_type>;

    template <element_type Element>
    void _add(const size_type n) noexcept {
        if constexpr (Element == layout_tag::major_element) { // add major
            this->_matrix.resize(
                this->_matrix.size() + n, matrix_row_type(this->_matrix_row_size, false)
            );
        }
        else { // add minor
            this->_matrix_row_size += n;
            for (auto& row : this->_matrix)
                row.resize(this->_matrix_row_size, false);
        }
    }

    template <element_type Element>
    void _remove(const id_type id) noexcept {
        if constexpr (Element == layout_tag::major_element) { // remove major
            this->_matrix.erase(this->_matrix.begin() + static_cast<std::ptrdiff_t>(id));
        }
        else { // remove minor
            if (this->_matrix_row_size == 0uz)
                return;
            this->_matrix_row_size--;
            for (auto& row : this->_matrix) {
                row.erase(row.begin() + static_cast<std::ptrdiff_t>(id));
            }
        }
    }

    template <element_type Element>
    gl_attr_force_inline auto _incident_with(const id_type id) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // incident with major
            return std::views::iota(initial_id_v<id_type>, this->_matrix_row_size)
                 | std::views::filter([&row = this->_matrix[to_idx(id)]](id_type minor_id) {
                       return row[to_idx(minor_id)];
                   });
        }
        else { // incident with minor
            return std::views::iota(initial_id_v<id_type>, this->_matrix.size())
                 | std::views::filter([this, minor_idx = to_idx(id)](id_type major_id) {
                       return this->_matrix[to_idx(major_id)][minor_idx];
                   });
        }
    }

    template <element_type Element>
    [[nodiscard]] gl_attr_force_inline size_type _count(const id_type id) const noexcept {
        size_type count = 0uz;
        if constexpr (Element == layout_tag::major_element) { // count major
            for (const bool bit : this->_matrix[to_idx(id)])
                count += static_cast<size_type>(bit);
        }
        else { // count minor
            const auto idx = to_idx(id);
            for (const auto& row : this->_matrix)
                count += static_cast<size_type>(row[idx]);
        }
        return count;
    }

    template <element_type Element>
    [[nodiscard]] std::vector<size_type> _count_map(const size_type n_elements) const noexcept {
        std::vector<size_type> size_map(n_elements, 0uz);

        if constexpr (Element == layout_tag::major_element) { // count map major
            const size_type limit = std::min(n_elements, this->_matrix.size());
            for (auto i = 0uz; i < limit; ++i) {
                size_map[i] = static_cast<size_type>(std::ranges::count(this->_matrix[i], true));
            }
        }
        else { // count map minor
            const size_type limit = std::min(n_elements, this->_matrix_row_size);
            for (const auto& row : this->_matrix)
                for (auto j = 0uz; j < limit; ++j)
                    if (row[j])
                        ++size_map[j];
        }
        return size_map;
    }

    size_type _matrix_row_size = 0uz;
    hypergraph_storage_type _matrix;
};

template <traits::c_hypergraph_matrix_impl ImplTag>
requires traits::c_hypergraph_asymmetric_layout_tag<typename ImplTag::layout_tag>
class incidence_matrix<hgl::bf_directed_t, ImplTag> final {
public:
    using directional_tag = hgl::bf_directed_t;
    using implementation_tag = ImplTag;
    using layout_tag = typename implementation_tag::layout_tag;
    using id_type = typename implementation_tag::id_type;

    incidence_matrix() = default;

    incidence_matrix(const size_type n_vertices, const size_type n_hyperedges)
    : _matrix_row_size{layout_tag::minor(n_vertices, n_hyperedges)},
      _matrix(
          layout_tag::major(n_vertices, n_hyperedges),
          matrix_row_type(_matrix_row_size, incidence_type::none)
      ) {}

    incidence_matrix(const incidence_matrix&) = default;
    incidence_matrix& operator=(const incidence_matrix&) = default;

    incidence_matrix(incidence_matrix&&) noexcept = default;
    incidence_matrix& operator=(incidence_matrix&&) noexcept = default;

    ~incidence_matrix() = default;

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
        return this->_query<element_type::vertex>(vertex_id, _is_incident);
    }

    [[nodiscard]] size_type degree(const id_type vertex_id) const noexcept {
        return this->_count<element_type::vertex>(vertex_id, _is_incident);
    }

    [[nodiscard]] std::vector<size_type> degree_map(const size_type n_vertices) const noexcept {
        return this->_count_map<element_type::vertex>(n_vertices, _is_incident);
    }

    [[nodiscard]] gl_attr_force_inline auto out_hyperedges(const id_type vertex_id) const noexcept {
        return this->_query<element_type::vertex>(vertex_id, _is_tail);
    }

    [[nodiscard]] size_type out_degree(const id_type vertex_id) const noexcept {
        return this->_count<element_type::vertex>(vertex_id, _is_tail);
    }

    [[nodiscard]] std::vector<size_type> out_degree_map(const size_type n_vertices) const noexcept {
        return this->_count_map<element_type::vertex>(n_vertices, _is_tail);
    }

    [[nodiscard]] gl_attr_force_inline auto in_hyperedges(const id_type vertex_id) const noexcept {
        return this->_query<element_type::vertex>(vertex_id, _is_head);
    }

    [[nodiscard]] size_type in_degree(const id_type vertex_id) const noexcept {
        return this->_count<element_type::vertex>(vertex_id, _is_head);
    }

    [[nodiscard]] std::vector<size_type> in_degree_map(const size_type n_vertices) const noexcept {
        return this->_count_map<element_type::vertex>(n_vertices, _is_head);
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
        return this->_query<element_type::hyperedge>(hyperedge_id, _is_incident);
    }

    [[nodiscard]] size_type hyperedge_size(const id_type hyperedge_id) const noexcept {
        return this->_count<element_type::hyperedge>(hyperedge_id, _is_incident);
    }

    [[nodiscard]] std::vector<size_type> hyperedge_size_map(const size_type n_hyperedges
    ) const noexcept {
        return this->_count_map<element_type::hyperedge>(n_hyperedges, _is_incident);
    }

    [[nodiscard]] gl_attr_force_inline auto tail_vertices(const id_type hyperedge_id
    ) const noexcept {
        return this->_query<element_type::hyperedge>(hyperedge_id, _is_tail);
    }

    [[nodiscard]] size_type tail_size(const id_type hyperedge_id) const noexcept {
        return this->_count<element_type::hyperedge>(hyperedge_id, _is_tail);
    }

    [[nodiscard]] std::vector<size_type> tail_size_map(const size_type n_hyperedges
    ) const noexcept {
        return this->_count_map<element_type::hyperedge>(n_hyperedges, _is_tail);
    }

    [[nodiscard]] gl_attr_force_inline auto head_vertices(const id_type hyperedge_id
    ) const noexcept {
        return this->_query<element_type::hyperedge>(hyperedge_id, _is_head);
    }

    [[nodiscard]] size_type head_size(const id_type hyperedge_id) const noexcept {
        return this->_count<element_type::hyperedge>(hyperedge_id, _is_head);
    }

    [[nodiscard]] std::vector<size_type> head_size_map(const size_type n_hyperedges
    ) const noexcept {
        return this->_count_map<element_type::hyperedge>(n_hyperedges, _is_head);
    }

    // --- binding methods ---

    gl_attr_force_inline void bind_tail(
        const id_type vertex_id, const id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[to_idx(major_id)][to_idx(minor_id)] = incidence_type::backward;
    }

    gl_attr_force_inline void bind_head(
        const id_type vertex_id, const id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[to_idx(major_id)][to_idx(minor_id)] = incidence_type::forward;
    }

    gl_attr_force_inline void unbind(const id_type vertex_id, const id_type hyperedge_id) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[to_idx(major_id)][to_idx(minor_id)] = incidence_type::none;
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[to_idx(major_id)][to_idx(minor_id)] != incidence_type::none;
    }

    [[nodiscard]] gl_attr_force_inline bool is_tail(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[to_idx(major_id)][to_idx(minor_id)] == incidence_type::backward;
    }

    [[nodiscard]] gl_attr_force_inline bool is_head(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[to_idx(major_id)][to_idx(minor_id)] == incidence_type::forward;
    }

    // --- comparison ---

    [[nodiscard]] friend bool operator==(const incidence_matrix&, const incidence_matrix&) =
        default;

    // --- friend declarations ---

    template <
        traits::c_hypergraph_impl_tag TargetImplTag,
        traits::c_hypergraph_impl_tag SourceImplTag>
    friend struct hgl::detail::to_impl;

#ifdef HGL_TESTING
    friend struct hgl_testing::test_incidence_matrix;
#endif

private:
    // --- incidence utility ---

    enum class incidence_type : std::int8_t {
        none = 0, // v not in E
        backward = -1, // v in Tail(E)
        forward = 1, // v in Head(E)
    };

    static constexpr auto _is_incident = [](const incidence_type t) {
        return t != incidence_type::none;
    };

    static constexpr auto _is_tail = [](const incidence_type t) {
        return t == incidence_type::backward;
    };

    static constexpr auto _is_head = [](const incidence_type t) {
        return t == incidence_type::forward;
    };

    // --- storage management ---

    using matrix_row_type = std::vector<incidence_type>;
    using hypergraph_storage_type = std::vector<matrix_row_type>;

    template <element_type Element>
    void _add(const size_type n) noexcept {
        if constexpr (Element == layout_tag::major_element) { // add major
            this->_matrix.resize(
                this->_matrix.size() + n,
                matrix_row_type(this->_matrix_row_size, incidence_type::none)
            );
        }
        else { // add minor
            this->_matrix_row_size += n;
            for (auto& row : this->_matrix)
                row.resize(this->_matrix_row_size, incidence_type::none);
        }
    }

    template <element_type Element>
    void _remove(const id_type id) noexcept {
        if constexpr (Element == layout_tag::major_element) { // remove major
            this->_matrix.erase(this->_matrix.begin() + static_cast<std::ptrdiff_t>(id));
        }
        else { // remove minor
            if (this->_matrix_row_size == 0uz)
                return;
            this->_matrix_row_size--;
            for (auto& row : this->_matrix)
                row.erase(row.begin() + static_cast<std::ptrdiff_t>(id));
        }
    }

    template <element_type Element>
    [[nodiscard]] gl_attr_force_inline auto _query(
        const id_type id, std::predicate<incidence_type> auto&& pred
    ) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // query major
            return std::views::iota(initial_id_v<id_type>, this->_matrix_row_size)
                 | std::views::filter([&row = this->_matrix[to_idx(id)], pred](id_type minor_id) {
                       return pred(row[to_idx(minor_id)]);
                   });
        }
        else { // query minor
            return std::views::iota(initial_id_v<id_type>, this->_matrix.size())
                 | std::views::filter([this, minor_idx = to_idx(id), pred](id_type major_id) {
                       return pred(this->_matrix[to_idx(major_id)][minor_idx]);
                   });
        }
    }

    template <element_type Element>
    [[nodiscard]] gl_attr_force_inline size_type
    _count(const id_type id, std::predicate<incidence_type> auto&& pred) const noexcept {
        size_type count = 0uz;
        if constexpr (Element == layout_tag::major_element) { // count major
            for (const incidence_type t : this->_matrix[to_idx(id)])
                count += static_cast<size_type>(pred(t));
        }
        else { // count minor
            const auto idx = to_idx(id);
            for (const auto& row : this->_matrix)
                count += static_cast<size_type>(pred(row[idx]));
        }
        return count;
    }

    template <element_type Element>
    [[nodiscard]] std::vector<size_type> _count_map(
        const size_type n_elements, std::predicate<incidence_type> auto&& pred
    ) const noexcept {
        std::vector<size_type> size_map(n_elements, 0uz);

        if constexpr (Element == layout_tag::major_element) { // count map major
            const size_type limit = std::min(n_elements, this->_matrix.size());
            for (auto i = 0uz; i < limit; ++i)
                for (const auto val : this->_matrix[i])
                    if (pred(val))
                        ++size_map[i];
        }
        else { // count map minor
            for (const auto& row : this->_matrix) {
                const size_type limit = std::min(n_elements, this->_matrix_row_size);
                for (auto j = 0uz; j < limit; ++j)
                    if (pred(row[j]))
                        ++size_map[j];
            }
        }

        return size_map;
    }

    size_type _matrix_row_size = 0uz;
    hypergraph_storage_type _matrix;
};

} // namespace impl
} // namespace hgl
