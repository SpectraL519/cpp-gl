// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/impl/layout_tags.hpp
/// @brief Defines layout tags that dictate the memory orientation and primary indexing of incidence structures.

#pragma once

#include "hgl/hypergraph_elements.hpp"
#include "hgl/traits.hpp"
#include "hgl/types.hpp"

#include <concepts>

namespace hgl {

namespace impl {

/// @ingroup HGL-Core
/// @brief Layout tag designating vertices as the primary structural dimension of the incidence representation.
struct vertex_major_t {
    /// @brief The major element type for this layout.
    using major_element = vertex_t;
    /// @brief The minor element type for this layout.
    using minor_element = hyperedge_t;

    /// @brief Retrieves the major element from the provided arguments based on the layout rules.
    template <std::regular T>
    [[nodiscard]] static constexpr T major(
        const T& vertex_el, [[maybe_unused]] const T& hyperedge_el
    ) noexcept {
        return vertex_el;
    }

    /// @brief Retrieves the minor element from the provided arguments based on the layout rules.
    template <std::regular T>
    [[nodiscard]] static constexpr T minor(
        [[maybe_unused]] const T& vertex_el, const T& hyperedge_el
    ) noexcept {
        return hyperedge_el;
    }

    /// @brief Packages data associated with the vertex and hyperedge elements into a strictly ordered major-minor pair.
    /// @tparam T The type associated with hypergraph's elements to be ordered.
    template <std::regular T>
    [[nodiscard]] static constexpr homogeneous_pair<T> majmin(
        const T& vertex_el, const T& hyperedge_el
    ) noexcept {
        return std::make_pair(vertex_el, hyperedge_el);
    }
};

/// @ingroup HGL-Core
/// @brief Layout tag designating hyperedges as the primary structural dimension of the incidence representation.
struct hyperedge_major_t {
    /// @brief The major element type for this layout.
    using major_element = hyperedge_t;
    /// @brief The minor element type for this layout.
    using minor_element = vertex_t;

    /// @brief Retrieves the major element from the provided arguments based on the layout rules.
    template <std::regular T>
    [[nodiscard]] static constexpr T major(
        [[maybe_unused]] const T& vertex_el, const T& hyperedge_el
    ) noexcept {
        return hyperedge_el;
    }

    /// @brief Retrieves the minor element from the provided arguments based on the layout rules.
    template <std::regular T>
    [[nodiscard]] static constexpr T minor(
        const T& vertex_el, [[maybe_unused]] const T& hyperedge_el
    ) noexcept {
        return vertex_el;
    }

    /// @brief Packages data associated with the vertex and hyperedge elements into a strictly ordered major-minor pair.
    /// @tparam T The type associated with hypergraph's elements to be ordered.
    template <std::regular T>
    [[nodiscard]] static constexpr homogeneous_pair<T> majmin(
        const T& vertex_el, const T& hyperedge_el
    ) noexcept {
        return std::make_pair(hyperedge_el, vertex_el);
    }
};

/// @ingroup HGL-Core
/// @brief Layout tag representing a symmetric or fully bidirectional incidence structural layout.
struct bidirectional_t {};

} // namespace impl

namespace traits {

/// @ingroup HGL-Traits
/// @brief Validates if a type is a valid hypergraph layout tag.
///
/// The valid hypergraph layout tags are @ref hgl::impl::vertex_major_t "vertex_major_t",
/// @ref hgl::impl::hyperedge_major_t "hyperedge_major_t", and @ref hgl::impl::bidirectional_t "bidirectional_t".
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_layout_tag =
    c_one_of<T, impl::vertex_major_t, impl::hyperedge_major_t, impl::bidirectional_t>;

/// @ingroup HGL-Traits
/// @brief Validates if a type is a valid asymmetric hypergraph layout tag.
///
/// Symmetric or bidirectional layouts are excluded. Valid tags are @ref hgl::impl::vertex_major_t "vertex_major_t"
/// and @ref hgl::impl::hyperedge_major_t "hyperedge_major_t".
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_asymmetric_layout_tag =
    c_one_of<T, impl::vertex_major_t, impl::hyperedge_major_t>;

} // namespace traits

/// @ingroup HGL-Traits
/// @brief Extracts the major element tag type associated with a specific asymmetric layout.
/// @tparam LT The asymmetric layout tag type.
template <traits::c_hypergraph_asymmetric_layout_tag LT>
using major_element_t = typename LT::major_element;

/// @ingroup HGL-Traits
/// @brief Extracts the minor element tag type associated with a specific asymmetric layout.
/// @tparam LT The asymmetric layout tag type.
template <traits::c_hypergraph_asymmetric_layout_tag LT>
using minor_element_t = typename LT::minor_element;

} // namespace hgl
