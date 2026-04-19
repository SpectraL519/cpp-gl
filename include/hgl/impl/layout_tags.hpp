// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/hypergraph_elements.hpp"
#include "hgl/traits.hpp"
#include "hgl/types.hpp"

#include <concepts>

namespace hgl {

namespace impl {

struct vertex_major_t {
    using major_element = vertex_t;
    using minor_element = hyperedge_t;

    template <std::regular T>
    [[nodiscard]] static constexpr T major(
        const T& vertex_el, [[maybe_unused]] const T& hyperedge_el
    ) noexcept {
        return vertex_el;
    }

    template <std::regular T>
    [[nodiscard]] static constexpr T minor(
        [[maybe_unused]] const T& vertex_el, const T& hyperedge_el
    ) noexcept {
        return hyperedge_el;
    }

    template <std::regular T>
    [[nodiscard]] static constexpr homogeneous_pair<T> majmin(
        const T& vertex_el, const T& hyperedge_el
    ) noexcept {
        return std::make_pair(vertex_el, hyperedge_el);
    }
};

struct hyperedge_major_t {
    using major_element = hyperedge_t;
    using minor_element = vertex_t;

    template <std::regular T>
    [[nodiscard]] static constexpr T major(
        [[maybe_unused]] const T& vertex_el, const T& hyperedge_el
    ) noexcept {
        return hyperedge_el;
    }

    template <std::regular T>
    [[nodiscard]] static constexpr T minor(
        const T& vertex_el, [[maybe_unused]] const T& hyperedge_el
    ) noexcept {
        return vertex_el;
    }

    template <std::regular T>
    [[nodiscard]] static constexpr homogeneous_pair<T> majmin(
        const T& vertex_el, const T& hyperedge_el
    ) noexcept {
        return std::make_pair(hyperedge_el, vertex_el);
    }
};

struct bidirectional_t {};

} // namespace impl

namespace traits {

template <typename T>
concept c_hypergraph_layout_tag =
    c_one_of<T, impl::vertex_major_t, impl::hyperedge_major_t, impl::bidirectional_t>;

template <typename T>
concept c_hypergraph_asymmetric_layout_tag =
    c_one_of<T, impl::vertex_major_t, impl::hyperedge_major_t>;

} // namespace traits

template <traits::c_hypergraph_asymmetric_layout_tag LT>
using major_element_t = typename LT::major_element;

template <traits::c_hypergraph_asymmetric_layout_tag LT>
using minor_element_t = typename LT::minor_element;

} // namespace hgl
