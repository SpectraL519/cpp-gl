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
/// @copybrief gl::algorithm::empty_extension
/// ### See Also
/// - @ref gl::algorithm::empty_extension : For the original GL module's type documentation.
/// - @ref hgl::algorithm::search_node : For the definition of the algorithm search node type.
using empty_extension = gl::algorithm::empty_extension;

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
/// @tparam Extension An optional payload type attached to the node for state tracking (must satisfy `std::semiregular`).
/// ### See Also
/// - @ref hgl::algorithm::root_node "hgl::algorithm::root_node" : For the full definition of the root search node builder function.
template <traits::c_hypergraph H, std::semiregular Extension = empty_extension>
struct search_node {
    /// @brief The underlying value type of the hypergraph being searched.
    using hypergraph_type = val_t<H>;
    /// @brief The identifier type of the hypergraph elements.
    using id_type = id_t<H>;
    /// @brief The type of the custom state-tracking payload attached to this node.
    using extension_type = Extension;

    /// @brief The ID of the current vertex.
    id_type vertex_id = invalid_id;
    /// @brief The ID of the predecessor from which this vertex was reached.
    id_type pred_id = invalid_id;
    /// @brief The ID of the hyperedge via which this vertex was reached from the predecessor.
    id_type hyperedge_id = invalid_id;
    /// @brief Custom state-tracking payload.
    [[no_unique_address]] extension_type ext = {};

    /// @brief Checks if this node is the root of a search tree.
    /// @return `true` if the node is valid and its predecessor is itself, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool is_root() const noexcept {
        return this->vertex_id != invalid_id and this->vertex_id == this->pred_id;
    }

    /// @brief Explicitly converts this node to a search node with a different extension type.
    ///
    /// This allows for safe, seamless slicing and up-casting between stateful and stateless
    /// search nodes during algorithm execution. The new extension is default-initialized.
    ///
    /// @tparam OtherExt The target extension type.
    /// @return A new search node preserving the topology but with the target extension type.
    template <std::semiregular OtherExt>
    requires(not std::same_as<Extension, OtherExt>)
    [[nodiscard]] gl_attr_force_inline explicit operator search_node<H, OtherExt>() const noexcept {
        return search_node<H, OtherExt>{this->vertex_id, this->pred_id, this->hyperedge_id};
    }
};

/// @ingroup HGL-Algorithm
/// @brief Free function builder that creates a search node acting as the root of a search tree.
///
/// This utility provides clean, unambiguous aggregate initialization semantics for root nodes
/// (where the vertex is strictly its own predecessor, and the connecting hyperedge is invalid)
/// at algorithmic call sites.
///
/// @tparam H The type of the hypergraph being searched.
/// @tparam Extension The type of the custom state-tracking payload attached to the node.
/// @param root_id The ID of the root vertex.
/// @param ext An optional state-tracking extension payload.
/// @return A fully initialized @ref hgl::algorithm::search_node "search_node" acting as a root.
template <traits::c_hypergraph H, std::semiregular Extension = empty_extension>
[[nodiscard]] gl_attr_force_inline search_node<val_t<H>, Extension> root_node(
    id_t<H> root_id, Extension ext = {}
) {
    return search_node<val_t<H>, Extension>{root_id, root_id, invalid_id, std::move(ext)};
}

/// @ingroup HGL-Algorithm
/// @brief A flat, index-mapped representation of a hypergraph search tree.
///
/// This structure is a simple wrapper around a `std::vector` of nodes, storing the
/// resulting topology of a hypergraph traversal. The $i$-th element in the `nodes`
/// vector implicitly corresponds to the vertex with `id == i`.
///
/// @tparam H The type of the hypergraph being searched. Must satisfy [**c_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph).
template <traits::c_hypergraph H>
struct search_tree {
    /// @brief The underlying hypergraph type.
    using hypergraph_type = val_t<H>;
    /// @brief The identifier type of the hypergraph elements.
    using id_type = id_t<H>;

    /// @brief Represents a static link in the traversal tree.
    struct node {
        id_type pred_id = invalid_id; ///< The ID of the predecessor vertex.
        id_type hyperedge_id = invalid_id; ///< The ID of the connecting hyperedge.
    };

    /// @brief Default constructor creating an empty search tree.
    search_tree() = default;

    /// @brief Constructs a search tree allocated for a specific number of vertices.
    /// @param n_vertices The total number of vertices in the hypergraph.
    explicit search_tree(const std::size_t n_vertices) : nodes(n_vertices) {}

    /// @brief Checks if a specific vertex was reached during the traversal.
    /// @param vertex_id The ID of the vertex to check.
    /// @return `true` if the vertex has a valid assigned predecessor, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool is_reachable(const id_type vertex_id) const noexcept {
        return this->nodes[vertex_id].pred_id != invalid_id;
    }

    /// @brief Checks if a specific vertex acts as a root in the search tree.
    /// @param vertex_id The ID of the vertex to check.
    /// @return `true` if the vertex is its own predecessor, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool is_root(const id_type vertex_id) const noexcept {
        const auto pred = this->nodes[vertex_id].pred_id;
        return pred != invalid_id and pred == vertex_id;
    }

    /// @brief The underlying container mapping vertex IDs to their traversal tree nodes.
    std::vector<node> nodes;
};

} // namespace algorithm

namespace traits {

/// @ingroup HGL-Traits
/// @brief Concept checking if a given type is the empty_callback tag.
/// ### See Also
/// - [**c_empty_callback**](gl_concepts.md#gl-traits-c-empty-callback) : For the full concept documentation in the GL module.
using gl::traits::c_empty_callback;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is callable with specific arguments and returns a specific type.
/// ### See Also
/// - [**c_callback**](gl_concepts.md#gl-traits-c-callback) : For the full concept documentation in the GL module.
using gl::traits::c_callback;

/// @ingroup HGL-Traits
/// @brief Concept allowing either a valid callback or the explicit absence of one through the use of empty_callback.
/// ### See Also
/// - [**c_optional_callback**](gl_concepts.md#gl-traits-c-optional-callback) : For the full concept documentation in the GL module.
using gl::traits::c_optional_callback;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a boolean predicate callable with specific arguments.
/// ### See Also
/// - [**c_predicate**](gl_concepts.md#gl-traits-c-predicate) : For the full concept documentation in the GL module.
using gl::traits::c_predicate;

/// @ingroup HGL-Traits
/// @brief Concept allowing either a valid boolean predicate or the explicit absence of one through the use of empty_callback.
/// ### See Also
/// - [**c_optional_predicate**](gl_concepts.md#gl-traits-c-optional-predicate) : For the full concept documentation in the GL module.
using gl::traits::c_optional_predicate;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a predicate returning a decision.
/// ### See Also
/// - [**c_decision_predicate**](gl_concepts.md#gl-traits-c-decision-predicate) : For the full concept documentation in the GL module.
using gl::traits::c_decision_predicate;

/// @ingroup HGL-Traits
/// @brief Concept allowing either a valid decision predicate or the explicit absence of one.
/// ### See Also
/// - [**c_optional_decision_predicate**](gl_concepts.md#gl-traits-c-optional-decision-predicate) : For the full concept documentation in the GL module.
using gl::traits::c_optional_decision_predicate;

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
    static auto target_hyperedges(const val_t<H>& h, id_t<H> v_id) {
        return h.incident_hyperedge_ids(v_id);
    }

    /// @brief Retrieves the vertices incident to the given hyperedge.
    static auto target_vertices(const val_t<H>& h, id_t<H> he_id) {
        return h.incident_vertex_ids(he_id);
    }
};

/// @ingroup HGL-Algorithm
/// @brief Traversal policy specialization for forward searches on BF-directed hypergraphs.
template <traits::c_bf_directed_hypergraph H>
struct traversal_policy<H, traversal_direction::forward> {
    /// @brief Retrieves the hyperedges originating from the given vertex (forward star).
    static auto target_hyperedges(const val_t<H>& h, id_t<H> v_id) {
        return h.out_hyperedge_ids(v_id); // forward star
    }

    /// @brief Retrieves the vertices targeted by the given hyperedge (head nodes).
    static auto target_vertices(const val_t<H>& h, id_t<H> he_id) {
        return h.head_ids(he_id);
    }
};

/// @ingroup HGL-Algorithm
/// @brief Traversal policy specialization for backward searches on BF-directed hypergraphs.
template <traits::c_bf_directed_hypergraph H>
struct traversal_policy<H, traversal_direction::backward> {
    /// @brief Retrieves the hyperedges entering the given vertex (backward star).
    static auto target_hyperedges(const val_t<H>& h, id_t<H> v_id) {
        return h.in_hyperedge_ids(v_id); // backward star
    }

    /// @brief Retrieves the vertices originating the given hyperedge (tail nodes).
    static auto target_vertices(const val_t<H>& h, id_t<H> he_id) {
        return h.tail_ids(he_id);
    }
};

} // namespace algorithm
} // namespace hgl
