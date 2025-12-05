// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/types/types.hpp"

#include <algorithm>
#include <vector>

#ifdef HGL_TESTING
namespace hgl_testing {
struct test_hyperedge_list;
} // namespace hgl_testing
#endif

namespace hgl::impl {

// *** BENCHMARKS ***
// - vertex removal: https://quick-bench.com/q/w6QztI4UIZMv6D7cKYbFtSoA9oY
// - complete hypergraph generation: https://quick-bench.com/q/yLyh1XQoJ1Wg63FM-2ZIpx7SDRA
// - element searching: https://quick-bench.com/q/hk8SOkwk1IfaBCEP_NdFEwWQulU
//   binary_search = lower_bound + template equality check with std::invoke

class undirected_hyperedge_list final {
public:
    using hyperedge_storage_type = std::vector<types::id_type>;
    using hypergraph_storage_type = std::vector<hyperedge_storage_type>;

    undirected_hyperedge_list(const undirected_hyperedge_list&) = delete;
    undirected_hyperedge_list& operator=(const undirected_hyperedge_list&) = delete;

    undirected_hyperedge_list() = default;

    undirected_hyperedge_list(
        [[maybe_unused]] const types::size_type n_vertices, const types::size_type n_hyperedges
    )
    : _storage{n_hyperedges} {}

    undirected_hyperedge_list(undirected_hyperedge_list&&) = default;
    undirected_hyperedge_list& operator=(undirected_hyperedge_list&&) = default;

    ~undirected_hyperedge_list() = default;

    // --- vertex methods ---

    gl_attr_force_inline void add_vertices(const types::size_type) const noexcept {}

    void remove_vertex(const types::id_type vertex_id) noexcept {
        for (auto& hyperedge_vertices : this->_storage)
            this->_unbind_impl(hyperedge_vertices, vertex_id);
    }

    // --- hyperedge methods ---

    gl_attr_force_inline void add_hyperedges(const types::size_type n) noexcept {
        this->_storage.resize(this->_storage.size() + n);
    }

    gl_attr_force_inline void remove_hyperedge(const types::id_type hyperedge_id) noexcept {
        this->_storage.erase(this->_storage.begin() + hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type hyperedge_size(
        const types::id_type hyperedge_id
    ) const noexcept {
        return this->_storage[hyperedge_id].size();
    }

    [[nodiscard]] gl_attr_force_inline auto hyperedge_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return std::views::all(this->_storage[hyperedge_id]);
    }

    // --- binding methods ---

    void bind(const types::id_type hyperedge_id, const types::id_type vertex_id) noexcept {
        auto& hyperedge_vertices = this->_storage[hyperedge_id];

        // insert the id at the correct position to keep the vertex-id collection sorted
        const auto it = std::ranges::lower_bound(hyperedge_vertices, vertex_id);
        if (it == hyperedge_vertices.end() or *it != vertex_id)
            hyperedge_vertices.insert(it, vertex_id);
    }

    gl_attr_force_inline void unbind(
        const types::id_type hyperedge_id, const types::id_type vertex_id
    ) noexcept {
        this->_unbind_impl(this->_storage[hyperedge_id], vertex_id);
    }

    [[nodiscard]] bool are_bound(const types::id_type hyperedge_id, const types::id_type vertex_id)
        const noexcept {
        auto& hyperedge_vertices = this->_storage[hyperedge_id];
        const auto vertex_it = std::ranges::lower_bound(hyperedge_vertices, vertex_id);
        return vertex_it != hyperedge_vertices.end() and *vertex_it == vertex_id;
    }

#ifdef HGL_TESTING
    friend struct hgl_testing::test_hyperedge_list;
#endif

private:
    void _unbind_impl(
        hyperedge_storage_type& hyperedge_vertices, const types::id_type vertex_id
    ) noexcept {
        const auto vertex_it = std::ranges::lower_bound(hyperedge_vertices, vertex_id);
        if (vertex_it != hyperedge_vertices.end() and *vertex_it == vertex_id)
            hyperedge_vertices.erase(vertex_it);
    }

    hypergraph_storage_type _storage;
};

} // namespace hgl::impl
