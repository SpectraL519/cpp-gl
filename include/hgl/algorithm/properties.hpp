// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/algorithm/properties.hpp
/// @brief Defines lightweight, functional utilities to evaluate the global structural properties of a hypergraph.

#pragma once

#include "hgl/hypergraph.hpp"
#include "hgl/util.hpp"

namespace hgl::algorithm {

// --- degree bounds ---

/// @ingroup HGL-Algorithm
/// @brief Calculates the maximum degree among all vertices in a hypergraph.
[[nodiscard]] size_type max_degree(const traits::c_hypergraph auto& hypergraph) noexcept {
    const auto degrees = hypergraph.degree_map();
    return degrees.empty() ? 0uz : *std::ranges::max_element(degrees);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the minimum degree among all vertices in a hypergraph.
[[nodiscard]] size_type min_degree(const traits::c_hypergraph auto& hypergraph) noexcept {
    const auto degrees = hypergraph.degree_map();
    return degrees.empty() ? 0uz : *std::ranges::min_element(degrees);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the maximum out-degree among all vertices in a *BF-directed* hypergraph.
[[nodiscard]] size_type max_out_degree(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto degrees = hypergraph.out_degree_map();
    return degrees.empty() ? 0uz : *std::ranges::max_element(degrees);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the minimum out-degree among all vertices in a *BF-directed* hypergraph.
[[nodiscard]] size_type min_out_degree(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto degrees = hypergraph.out_degree_map();
    return degrees.empty() ? 0uz : *std::ranges::min_element(degrees);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the maximum in-degree among all vertices in a *BF-directed* hypergraph.
[[nodiscard]] size_type max_in_degree(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto degrees = hypergraph.in_degree_map();
    return degrees.empty() ? 0uz : *std::ranges::max_element(degrees);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the minimum in-degree among all vertices in a *BF-directed* hypergraph.
[[nodiscard]] size_type min_in_degree(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto degrees = hypergraph.in_degree_map();
    return degrees.empty() ? 0uz : *std::ranges::min_element(degrees);
}

// --- hyperedge size bounds ---

/// @ingroup HGL-Algorithm
/// @brief Calculates the rank (maximum size of any hyperedge) of a hypergraph.
[[nodiscard]] size_type rank(const traits::c_hypergraph auto& hypergraph) noexcept {
    const auto sizes = hypergraph.hyperedge_size_map();
    return sizes.empty() ? 0uz : *std::ranges::max_element(sizes);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the corank (minimum size of any hyperedge) of a hypergraph.
[[nodiscard]] size_type corank(const traits::c_hypergraph auto& hypergraph) noexcept {
    const auto sizes = hypergraph.hyperedge_size_map();
    return sizes.empty() ? 0uz : *std::ranges::min_element(sizes);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the maximum tail size among all hyperedges in a *BF-directed* hypergraph.
[[nodiscard]] size_type max_tail_size(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto sizes = hypergraph.tail_size_map();
    return sizes.empty() ? 0uz : *std::ranges::max_element(sizes);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the minimum tail size among all hyperedges in a *BF-directed* hypergraph.
[[nodiscard]] size_type min_tail_size(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto sizes = hypergraph.tail_size_map();
    return sizes.empty() ? 0uz : *std::ranges::min_element(sizes);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the maximum head size among all hyperedges in a *BF-directed* hypergraph.
[[nodiscard]] size_type max_head_size(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto sizes = hypergraph.head_size_map();
    return sizes.empty() ? 0uz : *std::ranges::max_element(sizes);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the minimum head size among all hyperedges in a *BF-directed* hypergraph.
[[nodiscard]] size_type min_head_size(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto sizes = hypergraph.head_size_map();
    return sizes.empty() ? 0uz : *std::ranges::min_element(sizes);
}

// --- regularity ---

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given hypergraph is $k$-regular (all vertices have a degree of $k$).
[[nodiscard]] bool is_regular(
    const traits::c_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.degree_map(), k);
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given hypergraph is structurally regular (all vertices have the same degree).
[[nodiscard]] bool is_regular(const traits::c_hypergraph auto& hypergraph) noexcept {
    return util::is_constant(hypergraph.degree_map());
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given *BF-directed* hypergraph is out-$k$-regular (all vertices have an out-degree of $k$).
[[nodiscard]] bool is_out_regular(
    const traits::c_bf_directed_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.out_degree_map(), k);
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given *BF-directed* hypergraph is out-regular (all vertices have the same out-degree).
[[nodiscard]] bool is_out_regular(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    return util::is_constant(hypergraph.out_degree_map());
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given *BF-directed* hypergraph is in-$k$-regular (all vertices have an in-degree of $k$).
[[nodiscard]] bool is_in_regular(
    const traits::c_bf_directed_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.in_degree_map(), k);
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given *BF-directed* hypergraph is in-regular (all vertices have the same in-degree).
[[nodiscard]] bool is_in_regular(const traits::c_bf_directed_hypergraph auto& hypergraph) noexcept {
    return util::is_constant(hypergraph.in_degree_map());
}

// --- uniformity ---

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given hypergraph is $k$-uniform (all hyperedges have a size of $k$).
[[nodiscard]] bool is_uniform(
    const traits::c_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.hyperedge_size_map(), k);
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given hypergraph is structurally uniform (all hyperedges have the exact same size).
[[nodiscard]] bool is_uniform(const traits::c_hypergraph auto& hypergraph) noexcept {
    return util::is_constant(hypergraph.hyperedge_size_map());
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given *BF-directed* hypergraph is tail-$k$-uniform (all hyperedges have a tail size of $k$).
[[nodiscard]] bool is_tail_uniform(
    const traits::c_bf_directed_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.tail_size_map(), k);
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given *BF-directed* hypergraph is tail-uniform (all hyperedges have the exact same tail size).
[[nodiscard]] bool is_tail_uniform(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    return util::is_constant(hypergraph.tail_size_map());
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given *BF-directed* hypergraph is head-$k$-uniform (all hyperedges have a head size of $k$).
[[nodiscard]] bool is_head_uniform(
    const traits::c_bf_directed_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.head_size_map(), k);
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given *BF-directed* hypergraph is head-uniform (all hyperedges have the exact same head size).
[[nodiscard]] bool is_head_uniform(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    return util::is_constant(hypergraph.head_size_map());
}

} // namespace hgl::algorithm
