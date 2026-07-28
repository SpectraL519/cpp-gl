// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/algorithm/core.hpp
/// @brief Core data structures and types used to control and track hypergraph algorithm execution.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/traits.hpp"
#include "gl/algorithm/util.hpp"
#include "gl/traits.hpp"
#include "hgl/hypergraph.hpp"
#include "hgl/traits.hpp"
#include "hgl/types.hpp"

#include <ranges>

namespace hgl {

namespace algorithm {

// -- GL core ---

/// @ingroup HGL-Algorithm
/// @copybrief gl::algorithm::empty_callback
/// ### See Also
/// - @ref gl::algorithm::empty_callback : For a more detailed description.
using empty_callback = gl::algorithm::empty_callback;

/// @ingroup HGL-Algorithm
/// @copybrief gl::algorithm::decision
/// ### See Also
/// - @ref gl::algorithm::decision : For the full type definition.
using decision = gl::algorithm::decision;

/// @ingroup HGL-Algorithm
/// @copybrief gl::algorithm::result_discriminator
/// ### See Also
/// - @ref gl::algorithm::result_discriminator : For the full type definition.
using result_discriminator = gl::algorithm::result_discriminator;
using enum result_discriminator;

/// @ingroup HGL-Algorithm
/// @copybrief gl::algorithm::result_type
/// ### See Also
/// - @ref gl::algorithm::result_type : For the full type definition.
template <result_discriminator Result, typename ResultType>
using result_type = gl::algorithm::result_type<Result, ResultType>;

/// @ingroup HGL-Algorithm
/// @copybrief gl::algorithm::non_void_result_type
/// @see gl::algorithm::non_void_result_type : For the full type definition.
template <result_discriminator Result, typename ResultType>
using non_void_result_type = gl::algorithm::non_void_result_type<Result, ResultType>;

/// @ingroup HGL-Algorithm
/// @copybrief gl::algorithm::no_root_v
/// ### See Also
/// - @ref gl::algorithm::no_root_v
template <traits::c_id_type IdType>
inline constexpr IdType no_root_v = gl::algorithm::no_root_v<IdType>;

/// @ingroup HGL-Algorithm
/// @copybrief gl::algorithm::no_root_t
/// @see gl::algorithm::no_root_t
using no_root_t = gl::algorithm::no_root_t;

/// @ingroup HGL-Algorithm
/// @copybrief gl::algorithm::no_root
/// ### See Also
/// - @ref gl::algorithm::no_root
inline constexpr no_root_t no_root = gl::algorithm::no_root;

// --- traversal types ---

/// @ingroup HGL-Algorithm
/// @brief Represents an active node in a search container (e.g., a BFS queue or DFS stack) for hypergraph traversals.
/// @tparam H The type of the hypergraph being searched. Must satisfy [**c_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph).
template <traits::c_hypergraph H>
struct search_node {
    /// @brief The identifier type of the hypergraph elements.
    using id_type = typename H::id_type;

    /// @brief Default constructor creates an invalid node.
    search_node() = default;

    /// @brief Constructs a *root* search node (predecessor is itself, no incident hyperedge).
    /// @param vertex_id The ID of the root vertex.
    search_node(id_type vertex_id)
    : vertex_id(vertex_id), pred_id(vertex_id), hyperedge_id(invalid_id) {}

    /// @brief Constructs a search node with an explicit predecessor vertex and the connecting hyperedge.
    /// @param vertex_id The ID of the currently reached vertex.
    /// @param pred_id The ID of the predecessor vertex from which this vertex was reached.
    /// @param hyperedge_id The ID of the hyperedge connecting the predecessor to this vertex.
    search_node(id_type vertex_id, id_type pred_id, id_type hyperedge_id)
    : vertex_id(vertex_id), pred_id(pred_id), hyperedge_id(hyperedge_id) {}

    /// @brief Checks if this node is the root of a search tree.
    /// @return `true` if the node is valid and its predecessor is itself, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool is_root() const noexcept {
        return this->vertex_id != invalid_id and this->vertex_id == this->pred_id;
    }

    /// @brief The ID of the current vertex.
    id_type vertex_id = invalid_id;
    /// @brief The ID of the predecessor from which this vertex was reached.
    id_type pred_id = invalid_id;
    /// @brief The ID of the hyperedge via which this vertex was reached from the predecessor.
    id_type hyperedge_id = invalid_id;
};

/// @ingroup HGL-Algorithm
/// @brief A flat, index-mapped representation of a hypergraph search tree.
///
/// The $i$-th element corresponds to the vertex with `id == i`. The tree topology is formed implicitly,
/// as each @ref hgl::algorithm::search_node "search_node" stores the ID of its predecessor and the
/// connecting hyperedge, enabling \f$O(1)\f$ lookups and and \f$O(\vert V \vert)\f$ path reconstruction.
///
/// @tparam H The type of the hypergraph being searched.
template <traits::c_hypergraph H>
using search_tree = std::vector<search_node<H>>;

} // namespace algorithm

namespace traits {

using gl::traits::c_callback;
using gl::traits::c_decision_predicate;
using gl::traits::c_empty_callback;
using gl::traits::c_optional_callback;
using gl::traits::c_optional_decision_predicate;
using gl::traits::c_optional_predicate;
using gl::traits::c_predicate;


/// @ingroup HGL-Traits
/// @brief Validates if a type is a valid hypergraph search tree (a random access range of @ref hgl::algorithm::search_node "search_node"s).
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_search_tree =
    c_random_access_range<T>
    and c_instantiation_of<std::ranges::range_value_t<T>, algorithm::search_node>;

} // namespace traits

namespace algorithm {

// --- generic algorithm traits ---

/// @ingroup HGL-Algorithm
/// @brief Specifies the direction of traversal for *BF-directed* hypergraphs.
///
/// > [!IMPORTANT] API Note
/// >
/// > - To ensure API simplicity, the `traversal_direction` is used for undirected hypergraphs as well by
/// > the generic traversal templates. However, due to the structural nature of undirected hypergraphs,
/// > both direction values implicitly yield the exact same traversal pattern for undirected hypergraphs.
/// >
/// > - The library utilizes C++20's `using enum` feature for the `traversal_direction` enum type, allowing
/// > you to access these tags directly via `hgl::algorithm::forward` and `hgl::algorithm::backward`.
enum class traversal_direction : bool {
    forward, ///< Traverse following the forward star (tail to head).
    backward ///< Traverse following the backward star (head to tail).
};
using enum traversal_direction;

/// @ingroup HGL-Algorithm
/// @brief Policy defining how to extract incident hyperedges and target vertices during traversal.
///
/// This template is specialized based on the hypergraph's directionality to route standard
/// traversal algorithms over the correct incidence structures (e.g., following tails to heads).
///
/// @tparam H The type of the hypergraph.
/// @tparam Dir The direction of traversal.
template <traits::c_hypergraph H, traversal_direction Dir>
struct traversal_policy;

/// @ingroup HGL-Algorithm
/// @brief Traversal policy specialization for undirected hypergraphs.
template <traits::c_undirected_hypergraph H, traversal_direction Dir>
struct traversal_policy<H, Dir> {
    /// @brief Retrieves the hyperedges incident to the given vertex.
    static auto target_hyperedges(const H& h, typename H::id_type v_id) {
        return h.incident_hyperedge_ids(v_id);
    }

    /// @brief Retrieves the vertices incident to the given hyperedge.
    static auto target_vertices(const H& h, typename H::id_type he_id) {
        return h.incident_vertex_ids(he_id);
    }
};

/// @ingroup HGL-Algorithm
/// @brief Traversal policy specialization for forward searches on BF-directed hypergraphs.
template <traits::c_bf_directed_hypergraph H>
struct traversal_policy<H, traversal_direction::forward> {
    /// @brief Retrieves the hyperedges originating from the given vertex (forward star).
    static auto target_hyperedges(const H& h, typename H::id_type v_id) {
        return h.out_hyperedge_ids(v_id); // forward star
    }

    /// @brief Retrieves the vertices targeted by the given hyperedge (head nodes).
    static auto target_vertices(const H& h, typename H::id_type he_id) {
        return h.head_ids(he_id);
    }
};

/// @ingroup HGL-Algorithm
/// @brief Traversal policy specialization for backward searches on BF-directed hypergraphs.
template <traits::c_bf_directed_hypergraph H>
struct traversal_policy<H, traversal_direction::backward> {
    /// @brief Retrieves the hyperedges entering the given vertex (backward star).
    static auto target_hyperedges(const H& h, typename H::id_type v_id) {
        return h.in_hyperedge_ids(v_id); // backward star
    }

    /// @brief Retrieves the vertices originating the given hyperedge (tail nodes).
    static auto target_vertices(const H& h, typename H::id_type he_id) {
        return h.tail_ids(he_id);
    }
};

} // namespace algorithm
} // namespace hgl
