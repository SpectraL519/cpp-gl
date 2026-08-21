// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/directional_tags.hpp
/// @brief Defines tag types used to specify the directionality of a hypergraph.

#pragma once

#include "hgl/traits.hpp"

namespace hgl {

/// @ingroup HGL-Core
/// @brief Tag type specifying that a hypergraph is undirected.
///
/// In an undirected hypergraph, a hyperedge is strictly defined as a set of incident vertices,
/// with no distinction between origin and destination.
struct undirected_t {
    /// @brief Self type identity for internal metaprogramming use.
    using type = std::type_identity_t<undirected_t>;
};

enum class disjointness : bool { enforced = true, relaxed = false };

/// @ingroup HGL-Core
/// @brief Tag type specifying that a hypergraph is backward-forward (BF) directed.
///
/// In a BF-directed hypergraph, each hyperedge maps a distinct set of *tail* vertices (origins)
/// to a distinct set of *head* vertices (destinations).
template <disjointness Disjointness = disjointness::enforced>
struct bf_directed_t {
    /// @brief Self type identity for internal metaprogramming use.
    using type = std::type_identity_t<bf_directed_t>;

    static constexpr bool is_disjoint = std::to_underlying(Disjointness);
};

namespace traits {

/// @ingroup HGL-Traits
/// @brief Validates if a type is a valid hypergraph directional tag.
///
/// The valid hypergraph directional tags are @ref hgl::undirected_t "undirected_t" and
/// @ref hgl::bf_directed_t "bf_directed_t". This concept is used to constrain template
/// parameters intended to specify the directionality of a hypergraph.
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_directional_tag =
    std::same_as<T, undirected_t> or c_instantiation_of<bf_directed_t>;

} // namespace traits

} // namespace hgl
