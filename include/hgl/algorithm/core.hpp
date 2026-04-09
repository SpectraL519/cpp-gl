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

using gl::algorithm::decision;
using gl::algorithm::result_discriminator;
using enum result_discriminator;

using gl::algorithm::empty_callback;
using gl::algorithm::non_void_return_type;
using gl::algorithm::return_type;

using gl::algorithm::no_root;
using gl::algorithm::no_root_t;
using gl::algorithm::no_root_v;

// --- basic types ---

template <traits::c_hypergraph HypergraphType>
struct traversal_context {
    using id_type = typename HypergraphType::id_type;

    traversal_context(id_type id) : id(id), pred_id(id), hyperedge_id(invalid_id) {}

    traversal_context(id_type id, id_type pred_id, id_type hyperedge_id)
    : id(id), pred_id(pred_id), hyperedge_id(hyperedge_id) {}

    // if id == pred_id and hyperedge_id is invalid then id is the id of the root vertex
    id_type id;
    id_type pred_id;
    id_type hyperedge_id;
};

template <hgl::traits::c_hypergraph HypergraphType>
using predecessors_map = std::vector<traversal_context<HypergraphType>>;

// --- generic algorithm traits ---

template <hgl::traits::c_hypergraph HypergraphType>
struct traversal_traits;

template <hgl::traits::c_undirected_hypergraph HypergraphType>
struct traversal_traits<HypergraphType> {
    static auto out_hyperedges(const HypergraphType& h, typename HypergraphType::id_type v_id) {
        return h.incident_hyperedge_ids(v_id);
    }

    static auto target_vertices(const HypergraphType& h, typename HypergraphType::id_type he_id) {
        return h.incident_vertex_ids(he_id);
    }
};

template <hgl::traits::c_bf_directed_hypergraph HypergraphType>
struct traversal_traits<HypergraphType> {
    static auto out_hyperedges(const HypergraphType& h, typename HypergraphType::id_type v_id) {
        return h.out_hyperedge_ids(v_id);
    }

    static auto target_vertices(const HypergraphType& h, typename HypergraphType::id_type he_id) {
        return h.head_vertex_ids(he_id);
    }
};

} // namespace hgl::algorithm

// TODO! Validate `const Callback&` vs `Callback&&` in alg templates
