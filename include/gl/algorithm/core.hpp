// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm/core.hpp
/// @brief Core data structures and types used to control and track graph algorithm execution.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/constants.hpp"
#include "gl/graph.hpp"
#include "gl/types/core.hpp"

#include <functional>

namespace gl::algorithm {

// --- general types ---

/// @ingroup GL-Algorithm
/// @brief A tag type used to explicitly indicate the absence of a callback function.
///
/// > [!NOTE] Performance vs. Empty Lambdas
/// >
/// > Passing `empty_callback{}` is **not** the same as passing an empty lambda (e.g., `[]{}`).
/// > Using this explicit tag type allows internal algorithms to use compile-time checks to
/// > completely eliminate the callback invocation branch at compile time. This guarantees
/// > strict zero-cost abstractions, vastly improves performance in unoptimized/debug builds,
/// > and speeds up compilation times.
struct empty_callback {};

/// @ingroup GL-Algorithm GL-Types
/// @brief Represents a generic tri-state decision for control flow.
///
/// Used by custom predicates to determine how to proceed with a given item, execution step, or operation.
/// While heavily utilized in graph traversal algorithms, it is entirely decoupled from graph-specific
/// logic and can be used in any generic context requiring explicit accept/reject/abort semantics.
struct decision {
    /// @brief The underlying evaluation states.
    enum class eval : std::uint8_t {
        accept, /// < Proceed with the current element or operation.
        reject, /// < Skip the current element but continue the overall process.
        abort /// < Immediately terminate the entire process or algorithm.
    };
    using enum eval;

    /// @brief Constructs a decision from an explicit evaluation state.
    /// @param value The tri-state evaluation.
    constexpr decision(const eval value) : value(value) {}

    /// @brief Constructs a decision from a boolean value.
    /// @param value `true` maps to `accept`, `false` maps to `reject`.
    constexpr decision(const bool value) : value(value ? eval::accept : eval::reject) {}

    /// @brief Assigns a boolean value to the decision.
    /// @param value `true` maps to `accept`, `false` maps to `reject`.
    /// @return A reference to this decision.
    constexpr decision& operator=(const bool value) {
        this->value = value ? eval::accept : eval::reject;
        return *this;
    }

    /// @brief Evaluates the decision as a boolean.
    /// @return `true` if the decision is `accept`, `false` otherwise.
    [[nodiscard]] constexpr operator bool() const {
        return this->value == eval::accept;
    }

    /// @brief Compares the decision against a specific evaluation state.
    /// @param value The state to compare against.
    /// @return `true` if the states match.
    [[nodiscard]] constexpr bool operator==(const eval value) const {
        return this->value == value;
    }

    /// @brief The stored evaluation state.
    eval value;
};

/// @ingroup GL-Algorithm
/// @brief Tag used to statically dictate whether an algorithm should return a constructed result or execute purely for side effects.
///
/// > [!NOTE] Namespace Availability
/// >
/// > Because this enum uses the `using enum` declaration, its members (`ret` and `noret`) are
/// > injected directly into the `gl::algorithm` namespace. Hence, you can use `gl::algorithm::ret`
/// > and `gl::algorithm::noret` directly without the `result_discriminator::` scope.
///
/// ### See Also
/// - @ref gl::algorithm::result_type "result_type"
/// - @ref gl::algorithm::non_void_result_type "non_void_result_type"
enum class result_discriminator : bool {
    ret = true, /// < The algorithm should build and return a result (e.g., a predecessor map).
    noret = false /// < The algorithm will purely invoke callbacks and return void.
};
using enum result_discriminator;

/// @ingroup GL-Algorithm
/// @brief Resolves to the specified `ResultType` if `Result` is `ret`, otherwise resolves to `void`.
///
/// ### See Also
/// - @ref gl::algorithm::result_discriminator "result_discriminator"
/// - @ref gl::algorithm::non_void_result_type "non_void_result_type"
template <result_discriminator Result, typename ResultType>
using result_type = std::conditional_t<Result == algorithm::ret, ResultType, void>;

/// @ingroup GL-Algorithm
/// @brief Resolves to the specified `ResultType` if `Result` is `ret`, otherwise resolves to `std::monostate`.
///
/// Useful for returning dummy values from conditionally compiled algorithm branches.
///
/// ### See Also
/// - @ref gl::algorithm::result_discriminator "result_discriminator"
/// - @ref gl::algorithm::result_type "result_type"
template <result_discriminator Result, typename ResultType>
using non_void_result_type =
    std::conditional_t<Result == algorithm::ret, ResultType, std::monostate>;

// --- traversal types ---

/// @ingroup GL-Algorithm
/// @brief Maps a vertex ID to its predecessor's ID in a traversal tree.
/// @tparam GraphType The type of the graph being traversed.
template <traits::c_graph G>
using predecessors_map = std::vector<id_t<G>>;

/// @ingroup GL-Algorithm
/// @brief Represents an active node in a search container (e.g., a BFS queue or DFS stack).
/// @tparam GraphType The type of the graph being searched.
template <traits::c_graph G>
struct search_node {
    using id_type = id_t<G>;

    /// @brief Constructs a search node acting as a root (predecessor is itself).
    /// @param vertex_id The ID of the vertex.
    search_node(id_type vertex_id) : vertex_id(vertex_id), pred_id(vertex_id) {}

    /// @brief Constructs a search node with an explicit predecessor.
    /// @param vertex_id The ID of the vertex.
    /// @param pred_id The ID of the vertex's predecessor.
    search_node(id_type vertex_id, id_type pred_id) : vertex_id(vertex_id), pred_id(pred_id) {}

    /// @brief Checks if this node is the root of a search tree.
    /// @return `true` if the node is valid and its predecessor is itself, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool is_root() const noexcept {
        return this->vertex_id != invalid_id and this->vertex_id == this->pred_id;
    }

    /// @brief The ID of the vertex currently being searched.
    id_type vertex_id;
    /// @brief The ID of the predecessor from which this vertex was reached.
    id_type pred_id;
};

// --- constants ---

/// @ingroup GL-Algorithm
/// @brief Constant representing the absence of a root vertex ID for a specific ID type.
///
/// ### See Also
/// - @ref gl::invalid_id_v "gl::invalid_id_v"
/// - @ref gl::algorithm::no_root_t "no_root_t"
/// - @ref gl::algorithm::no_root "no_root"
template <traits::c_id_type IdType>
inline constexpr IdType no_root_v = invalid_id_v<IdType>;

/// @ingroup GL-Algorithm
/// @brief Tag type providing an implicit conversion to the appropriate `no_root_v` for any numeric ID type.
///
/// ### See Also
/// - @ref gl::algorithm::no_root_v "no_root_v"
/// - @ref gl::algorithm::no_root "no_root"
struct no_root_t {
    /// @brief Implicitly converts to the numeric `no_root_v` constant.
    template <traits::c_id_type IdType>
    [[nodiscard]] constexpr operator IdType() const noexcept {
        return no_root_v<IdType>;
    }

    /// @brief Checks if a given ID matches the `no_root_v` constant.
    template <traits::c_id_type IdType>
    [[nodiscard]] friend constexpr bool operator==(const IdType& lhs, no_root_t) noexcept {
        return lhs == no_root_v<IdType>;
    }
};

/// @ingroup GL-Algorithm
/// @brief Global constant representing the absence of a root vertex.
///
/// ### See Also
/// - @ref gl::algorithm::no_root_v "no_root_v"
/// - @ref gl::algorithm::no_root_t "no_root_t"
inline constexpr no_root_t no_root{};

} // namespace gl::algorithm
