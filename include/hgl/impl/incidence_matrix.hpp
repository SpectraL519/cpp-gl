// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

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

template <
    type_traits::c_hypergraph_impl_tag TargetImplTag,
    type_traits::c_hypergraph_impl_tag SourceImplTag>
struct to_impl;

} // namespace detail

namespace impl {

template <
    type_traits::c_hypergraph_directional_tag DirectionalTag,
    type_traits::c_hypergraph_asymmetric_layout_tag LayoutTag>
class incidence_matrix;

template <type_traits::c_hypergraph_asymmetric_layout_tag LayoutTag>
class incidence_matrix<hgl::undirected_t, LayoutTag> final {
public:
    using directional_tag = hgl::undirected_t;
    using layout_tag = LayoutTag;

    incidence_matrix() = default;

    incidence_matrix(const types::size_type n_vertices, const types::size_type n_hyperedges)
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

    gl_attr_force_inline void add_vertices(const types::size_type n) noexcept {
        this->_add<impl::element_type::vertex>(n);
    }

    gl_attr_force_inline void remove_vertex(const types::id_type vertex_id) noexcept {
        this->_remove<impl::element_type::vertex>(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        return this->_incident_with<impl::element_type::vertex>(vertex_id);
    }

    [[nodiscard]] types::size_type degree(const types::id_type vertex_id) const noexcept {
        return this->_count<impl::element_type::vertex>(vertex_id);
    }

    [[nodiscard]] std::vector<types::size_type> degree_map(const types::size_type n_vertices
    ) const noexcept {
        return this->_count_map<impl::element_type::vertex>(n_vertices);
    }

    // --- hyperedge methods ---

    gl_attr_force_inline void add_hyperedges(const types::size_type n) noexcept {
        this->_add<impl::element_type::hyperedge>(n);
    }

    gl_attr_force_inline void remove_hyperedge(const types::id_type hyperedge_id) noexcept {
        this->_remove<impl::element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return this->_incident_with<impl::element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] types::size_type hyperedge_size(const types::id_type hyperedge_id
    ) const noexcept {
        return this->_count<impl::element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] std::vector<types::size_type> hyperedge_size_map(
        const types::size_type n_hyperedges
    ) const noexcept {
        return this->_count_map<impl::element_type::hyperedge>(n_hyperedges);
    }

    // --- binding methods ---

    gl_attr_force_inline void bind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[major_id][minor_id] = true;
    }

    gl_attr_force_inline void unbind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[major_id][minor_id] = false;
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[major_id][minor_id];
    }

    // --- comparison ---

    [[nodiscard]] friend bool operator==(const incidence_matrix&, const incidence_matrix&) =
        default;

    // --- friend declarations ---

    template <
        type_traits::c_hypergraph_impl_tag TargetImplTag,
        type_traits::c_hypergraph_impl_tag SourceImplTag>
    friend struct hgl::detail::to_impl;

#ifdef HGL_TESTING
    friend struct hgl_testing::test_incidence_matrix;
#endif

private:
    using matrix_row_type = std::vector<bool>;
    using hypergraph_storage_type = std::vector<matrix_row_type>;

    template <impl::element_type Element>
    void _add(const types::size_type n) noexcept {
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

    template <impl::element_type Element>
    void _remove(const types::id_type id) noexcept {
        if constexpr (Element == layout_tag::major_element) { // remove major
            this->_matrix.erase(this->_matrix.begin() + static_cast<std::ptrdiff_t>(id));
        }
        else { // remove minor
            if (this->_matrix_row_size == 0)
                return;
            this->_matrix_row_size--;
            for (auto& row : this->_matrix) {
                row.erase(row.begin() + static_cast<std::ptrdiff_t>(id));
            }
        }
    }

    template <impl::element_type Element>
    gl_attr_force_inline auto _incident_with(const types::id_type id) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // incident with major
            return std::views::iota(0uz, this->_matrix_row_size)
                 | std::views::filter([&row = this->_matrix[id]](const types::id_type minor_id) {
                       return row[minor_id];
                   });
        }
        else { // incident with minor
            return std::views::iota(0uz, this->_matrix.size())
                 | std::views::filter([this, minor_id = id](const types::id_type major_id) {
                       return this->_matrix[major_id][minor_id];
                   });
        }
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline types::size_type _count(const types::id_type id
    ) const noexcept {
        types::size_type count = 0uz;
        if constexpr (Element == layout_tag::major_element) { // count major
            for (const bool bit : this->_matrix[id])
                count += static_cast<types::size_type>(bit);
        }
        else { // count minor
            for (const auto& row : this->_matrix)
                count += static_cast<types::size_type>(row[id]);
        }
        return count;
    }

    template <impl::element_type Element>
    [[nodiscard]] std::vector<types::size_type> _count_map(const types::size_type n_elements
    ) const noexcept {
        std::vector<types::size_type> size_map(n_elements, 0uz);

        if constexpr (Element == layout_tag::major_element) { // count map major
            const types::size_type limit =
                std::min(n_elements, static_cast<types::size_type>(this->_matrix.size()));
            for (types::size_type i = 0uz; i < limit; ++i) {
                size_map[i] =
                    static_cast<types::size_type>(std::ranges::count(this->_matrix[i], true));
            }
        }
        else { // count map minor
            const types::size_type limit = std::min(n_elements, this->_matrix_row_size);
            for (const auto& row : this->_matrix) {
                for (types::size_type j = 0uz; j < limit; ++j) {
                    if (row[j]) {
                        ++size_map[j];
                    }
                }
            }
        }
        return size_map;
    }

    types::size_type _matrix_row_size = 0uz;
    hypergraph_storage_type _matrix;
};

template <type_traits::c_hypergraph_asymmetric_layout_tag LayoutTag>
class incidence_matrix<hgl::bf_directed_t, LayoutTag> final {
public:
    using directional_tag = hgl::bf_directed_t;
    using layout_tag = LayoutTag;

    incidence_matrix() = default;

    incidence_matrix(const types::size_type n_vertices, const types::size_type n_hyperedges)
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

    gl_attr_force_inline void add_vertices(const types::size_type n) noexcept {
        this->_add<impl::element_type::vertex>(n);
    }

    gl_attr_force_inline void remove_vertex(const types::id_type vertex_id) noexcept {
        this->_remove<impl::element_type::vertex>(vertex_id);
    }

    // --- vertex methods : incidence queries ---

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        return this->_query<impl::element_type::vertex>(vertex_id, _is_incident);
    }

    [[nodiscard]] types::size_type degree(const types::id_type vertex_id) const noexcept {
        return this->_count<impl::element_type::vertex>(vertex_id, _is_incident);
    }

    [[nodiscard]] std::vector<types::size_type> degree_map(const types::size_type n_vertices
    ) const noexcept {
        return this->_count_map<impl::element_type::vertex>(n_vertices, _is_incident);
    }

    [[nodiscard]] gl_attr_force_inline auto out_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        return this->_query<impl::element_type::vertex>(vertex_id, _is_tail);
    }

    [[nodiscard]] types::size_type out_degree(const types::id_type vertex_id) const noexcept {
        return this->_count<impl::element_type::vertex>(vertex_id, _is_tail);
    }

    [[nodiscard]] std::vector<types::size_type> out_degree_map(const types::size_type n_vertices
    ) const noexcept {
        return this->_count_map<impl::element_type::vertex>(n_vertices, _is_tail);
    }

    [[nodiscard]] gl_attr_force_inline auto in_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        return this->_query<impl::element_type::vertex>(vertex_id, _is_head);
    }

    [[nodiscard]] types::size_type in_degree(const types::id_type vertex_id) const noexcept {
        return this->_count<impl::element_type::vertex>(vertex_id, _is_head);
    }

    [[nodiscard]] std::vector<types::size_type> in_degree_map(const types::size_type n_vertices
    ) const noexcept {
        return this->_count_map<impl::element_type::vertex>(n_vertices, _is_head);
    }

    // --- hyperedge methods : general ---

    gl_attr_force_inline void add_hyperedges(const types::size_type n) noexcept {
        this->_add<impl::element_type::hyperedge>(n);
    }

    gl_attr_force_inline void remove_hyperedge(const types::id_type hyperedge_id) noexcept {
        this->_remove<impl::element_type::hyperedge>(hyperedge_id);
    }

    // --- hyperedge methods : incidence queries ---

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return this->_query<impl::element_type::hyperedge>(hyperedge_id, _is_incident);
    }

    [[nodiscard]] types::size_type hyperedge_size(const types::id_type hyperedge_id
    ) const noexcept {
        return this->_count<impl::element_type::hyperedge>(hyperedge_id, _is_incident);
    }

    [[nodiscard]] std::vector<types::size_type> hyperedge_size_map(
        const types::size_type n_hyperedges
    ) const noexcept {
        return this->_count_map<impl::element_type::hyperedge>(n_hyperedges, _is_incident);
    }

    [[nodiscard]] gl_attr_force_inline auto tail_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return this->_query<impl::element_type::hyperedge>(hyperedge_id, _is_tail);
    }

    [[nodiscard]] types::size_type tail_size(const types::id_type hyperedge_id) const noexcept {
        return this->_count<impl::element_type::hyperedge>(hyperedge_id, _is_tail);
    }

    [[nodiscard]] std::vector<types::size_type> tail_size_map(const types::size_type n_hyperedges
    ) const noexcept {
        return this->_count_map<impl::element_type::hyperedge>(n_hyperedges, _is_tail);
    }

    [[nodiscard]] gl_attr_force_inline auto head_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return this->_query<impl::element_type::hyperedge>(hyperedge_id, _is_head);
    }

    [[nodiscard]] types::size_type head_size(const types::id_type hyperedge_id) const noexcept {
        return this->_count<impl::element_type::hyperedge>(hyperedge_id, _is_head);
    }

    [[nodiscard]] std::vector<types::size_type> head_size_map(const types::size_type n_hyperedges
    ) const noexcept {
        return this->_count_map<impl::element_type::hyperedge>(n_hyperedges, _is_head);
    }

    // --- binding methods ---

    gl_attr_force_inline void bind_tail(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[major_id][minor_id] = incidence_type::backward;
    }

    gl_attr_force_inline void bind_head(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[major_id][minor_id] = incidence_type::forward;
    }

    gl_attr_force_inline void unbind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[major_id][minor_id] = incidence_type::none;
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[major_id][minor_id] != incidence_type::none;
    }

    [[nodiscard]] gl_attr_force_inline bool is_tail(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[major_id][minor_id] == incidence_type::backward;
    }

    [[nodiscard]] gl_attr_force_inline bool is_head(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[major_id][minor_id] == incidence_type::forward;
    }

    // --- comparison ---

    [[nodiscard]] friend bool operator==(const incidence_matrix&, const incidence_matrix&) =
        default;

    // --- friend declarations ---

    template <
        type_traits::c_hypergraph_impl_tag TargetImplTag,
        type_traits::c_hypergraph_impl_tag SourceImplTag>
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

    template <impl::element_type Element>
    void _add(const types::size_type n) noexcept {
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

    template <impl::element_type Element>
    void _remove(const types::id_type id) noexcept {
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

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline auto _query(
        const types::id_type id, std::predicate<incidence_type> auto&& pred
    ) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // query major
            return std::views::iota(0uz, this->_matrix_row_size)
                 | std::views::filter([&row = this->_matrix[id], pred](const types::id_type minor_id
                                      ) { return pred(row[minor_id]); });
        }
        else { // query minor
            return std::views::iota(0uz, this->_matrix.size())
                 | std::views::filter([this, minor_id = id, pred](const types::id_type major_id) {
                       return pred(this->_matrix[major_id][minor_id]);
                   });
        }
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline types::size_type _count(
        const types::id_type id, std::predicate<incidence_type> auto&& pred
    ) const noexcept {
        types::size_type count = 0uz;
        if constexpr (Element == layout_tag::major_element) { // count major
            for (const incidence_type t : this->_matrix[id])
                count += static_cast<types::size_type>(pred(t));
        }
        else { // count minor
            for (const auto& row : this->_matrix)
                count += static_cast<types::size_type>(pred(row[id]));
        }
        return count;
    }

    template <impl::element_type Element>
    [[nodiscard]] std::vector<types::size_type> _count_map(
        const types::size_type n_elements, std::predicate<incidence_type> auto&& pred
    ) const noexcept {
        std::vector<types::size_type> size_map(n_elements, 0uz);

        if constexpr (Element == layout_tag::major_element) { // count map major
            const types::size_type limit =
                std::min(n_elements, static_cast<types::size_type>(this->_matrix.size()));
            for (types::size_type i = 0uz; i < limit; ++i) {
                for (const auto val : this->_matrix[i]) {
                    if (pred(val)) {
                        ++size_map[i];
                    }
                }
            }
        }
        else { // count map minor
            for (const auto& row : this->_matrix) {
                const types::size_type limit = std::min(n_elements, this->_matrix_row_size);
                for (types::size_type j = 0uz; j < limit; ++j) {
                    if (pred(row[j])) {
                        ++size_map[j];
                    }
                }
            }
        }

        return size_map;
    }

    types::size_type _matrix_row_size = 0uz;
    hypergraph_storage_type _matrix;
};

} // namespace impl
} // namespace hgl
