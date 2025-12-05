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

    // clang-format off

    gl_attr_force_inline void add_vertices([[maybe_unused]] const types::size_type) const noexcept {}

    // clang-format on

    void remove_vertex(const types::id_type vertex_id) noexcept {
        for (auto& hyperedge : this->_storage) {
            auto vertex_it = std::ranges::lower_bound(hyperedge, vertex_id);
            if (vertex_it != hyperedge.end() and *vertex_it == vertex_id)
                vertex_it = hyperedge.erase(vertex_it); // unbind the vertex
            while (vertex_it != hyperedge.end())
                --(*vertex_it++); // decrement ids > vertex_id
        }
    }

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        return std::views::iota(0uz, this->_storage.size())
             | std::views::filter([this, vertex_id](types::id_type hid) {
                   return this->_are_bound_impl(this->_storage[hid], vertex_id);
               });
    }

    [[nodiscard]] types::size_type degree(const types::id_type vertex_id) const noexcept {
        types::size_type deg = 0uz;
        for (const auto& hyperedge : this->_storage)
            if (this->_are_bound_impl(hyperedge, vertex_id))
                ++deg;
        return deg;
    }

    // --- hyperedge methods ---

    gl_attr_force_inline void add_hyperedges(const types::size_type n) noexcept {
        this->_storage.resize(this->_storage.size() + n);
    }

    gl_attr_force_inline void remove_hyperedge(const types::id_type hyperedge_id) noexcept {
        this->_storage.erase(this->_storage.begin() + hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return std::views::all(this->_storage[hyperedge_id]);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type hyperedge_size(
        const types::id_type hyperedge_id
    ) const noexcept {
        return this->_storage[hyperedge_id].size();
    }

    // --- binding methods ---

    void bind(const types::id_type vertex_id, const types::id_type hyperedge_id) noexcept {
        auto& hyperedge = this->_storage[hyperedge_id];

        // insert the id at the correct position to keep the vertex-id collection sorted
        const auto it = std::ranges::lower_bound(hyperedge, vertex_id);
        if (it == hyperedge.end() or *it != vertex_id)
            hyperedge.insert(it, vertex_id);
    }

    gl_attr_force_inline void unbind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        auto& hyperedge = this->_storage[hyperedge_id];
        const auto vertex_it = std::ranges::lower_bound(hyperedge, vertex_id);
        if (vertex_it != hyperedge.end() and *vertex_it == vertex_id)
            hyperedge.erase(vertex_it);
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        return this->_are_bound_impl(this->_storage[hyperedge_id], vertex_id);
    }

#ifdef HGL_TESTING
    friend struct hgl_testing::test_hyperedge_list;
#endif

private:
    [[nodiscard]] bool _are_bound_impl(
        const hyperedge_storage_type& hyperedge, const types::id_type vertex_id
    ) const noexcept {
        const auto vertex_it = std::ranges::lower_bound(hyperedge, vertex_id);
        return vertex_it != hyperedge.end() and *vertex_it == vertex_id;
    }

    hypergraph_storage_type _storage;
};

} // namespace hgl::impl
