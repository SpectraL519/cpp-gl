// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/types/types.hpp"

#include <algorithm>
#include <ranges>
#include <vector>

#ifdef HGL_TESTING
namespace hgl_testing {
struct test_vertex_list;
} // namespace hgl_testing
#endif

namespace hgl::impl {

class undirected_vertex_list final {
public:
    using vertex_storage_type = std::vector<types::id_type>;
    using hypergraph_storage_type = std::vector<vertex_storage_type>;

    undirected_vertex_list(const undirected_vertex_list&) = delete;
    undirected_vertex_list& operator=(const undirected_vertex_list&) = delete;

    undirected_vertex_list() = default;

    undirected_vertex_list(
        const types::size_type n_vertices, [[maybe_unused]] const types::size_type n_hyperedges
    )
    : _storage{n_vertices} {}

    undirected_vertex_list(undirected_vertex_list&&) = default;
    undirected_vertex_list& operator=(undirected_vertex_list&&) = default;

    ~undirected_vertex_list() = default;

    // --- vertex methods ---

    gl_attr_force_inline void add_vertices(const types::size_type n) noexcept {
        this->_storage.resize(this->_storage.size() + n);
    }

    gl_attr_force_inline void remove_vertex(const types::id_type vertex_id) noexcept {
        this->_storage.erase(this->_storage.begin() + vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        return std::views::all(this->_storage[vertex_id]);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type degree(const types::id_type vertex_id
    ) const noexcept {
        return this->_storage[vertex_id].size();
    }

    // --- hyperedge methods ---

    // clang-format off

    gl_attr_force_inline void add_hyperedges([[maybe_unused]] const types::size_type) const noexcept {}

    // clang-format on

    void remove_hyperedge(const types::id_type hyperedge_id) noexcept {
        for (auto& vertex : this->_storage) {
            auto hyperedge_it = std::ranges::lower_bound(vertex, hyperedge_id);
            if (hyperedge_it != vertex.end() and *hyperedge_it == hyperedge_id)
                hyperedge_it = vertex.erase(hyperedge_it); // unbind the hyperedge
            while (hyperedge_it != vertex.end())
                --(*hyperedge_it++); // decrement ids > hyperedge_id
        }
    }

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return std::views::iota(0uz, this->_storage.size())
             | std::views::filter([this, hyperedge_id](types::id_type vid) {
                   return this->_are_bound_impl(this->_storage[vid], hyperedge_id);
               });
    }

    [[nodiscard]] types::size_type hyperedge_size(const types::id_type hyperedge_id
    ) const noexcept {
        types::size_type size = 0uz;
        for (const auto& vertex : this->_storage)
            if (this->_are_bound_impl(vertex, hyperedge_id))
                ++size;
        return size;
    }

    // --- binding methods ---

    void bind(const types::id_type vertex_id, const types::id_type hyperedge_id) noexcept {
        auto& vertex = this->_storage[vertex_id];

        // insert the id at the correct position to keep the hyperedge-id collection sorted
        const auto it = std::ranges::lower_bound(vertex, hyperedge_id);
        if (it == vertex.end() or *it != hyperedge_id)
            vertex.insert(it, hyperedge_id);
    }

    gl_attr_force_inline void unbind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        auto& vertex = this->_storage[vertex_id];

        const auto hyperedge_it = std::ranges::lower_bound(vertex, hyperedge_id);
        if (hyperedge_it != vertex.end() and *hyperedge_it == hyperedge_id)
            vertex.erase(hyperedge_it);
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        return this->_are_bound_impl(this->_storage[vertex_id], hyperedge_id);
    }

#ifdef HGL_TESTING
    friend struct hgl_testing::test_vertex_list;
#endif

private:
    [[nodiscard]] bool _are_bound_impl(
        const vertex_storage_type& vertex, const types::id_type hyperedge_id
    ) const noexcept {
        const auto hyperedge_it = std::ranges::lower_bound(vertex, hyperedge_id);
        return hyperedge_it != vertex.end() and *hyperedge_it == hyperedge_id;
    }

    hypergraph_storage_type _storage;
};

} // namespace hgl::impl
