// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/traits.hpp"
#include "gl/algorithm/util.hpp"
#include "hgl/hypergraph.hpp"
#include "hgl/traits.hpp"
#include "hgl/types.hpp"

namespace hgl::algorithm {

// -- GL core ---

using gl::algorithm::empty_callback;

using gl::algorithm::decision;

using gl::algorithm::result_discriminator;
using enum result_discriminator;

using gl::algorithm::non_void_return_type;
using gl::algorithm::return_type;

using gl::algorithm::no_root;
using gl::algorithm::no_root_t;
using gl::algorithm::no_root_v;

// --- traversal types ---

template <traits::c_hypergraph H>
struct search_node {
    using id_type = typename H::id_type;

    search_node() = default;

    search_node(id_type vertex_id)
    : vertex_id(vertex_id), pred_id(vertex_id), hyperedge_id(invalid_id) {}

    search_node(id_type vertex_id, id_type pred_id, id_type hyperedge_id)
    : vertex_id(vertex_id), pred_id(pred_id), hyperedge_id(hyperedge_id) {}

    [[nodiscard]] gl_attr_force_inline bool is_root() const noexcept {
        return this->vertex_id != invalid_id and this->vertex_id == this->pred_id;
    }

    id_type vertex_id = invalid_id;
    id_type pred_id = invalid_id;
    id_type hyperedge_id = invalid_id;
};

template <hgl::traits::c_hypergraph H>
using search_tree = std::vector<search_node<H>>;

// --- generic algorithm traits ---

template <hgl::traits::c_hypergraph H>
struct traversal_policy;

template <hgl::traits::c_undirected_hypergraph H>
struct traversal_policy<H> {
    static auto target_hyperedges(const H& h, typename H::id_type v_id) {
        return h.incident_hyperedge_ids(v_id);
    }

    static auto target_vertices(const H& h, typename H::id_type he_id) {
        return h.incident_vertex_ids(he_id);
    }
};

template <hgl::traits::c_bf_directed_hypergraph H>
struct traversal_policy<H> {
    static auto target_hyperedges(const H& h, typename H::id_type v_id) {
        return h.out_hyperedge_ids(v_id);
    }

    static auto target_vertices(const H& h, typename H::id_type he_id) {
        return h.head_vertex_ids(he_id);
    }
};

} // namespace hgl::algorithm

// TODO! Validate `const Callback&` vs `Callback&&` in alg templates
