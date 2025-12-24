// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/types/type_traits.hpp"
#include "hgl/types/types.hpp"

#include <concepts>

namespace hgl {

namespace impl {

// element_discriminator ?
enum class element_type : bool { vertex, hyperedge };

struct vertex_major_t {
    static constexpr element_type major_element = element_type::vertex;
    static constexpr element_type minor_element = element_type::hyperedge;

    template <std::regular T>
    [[nodiscard]] static constexpr T major(const T& vertex_el, const T& hyperedge_el) noexcept {
        return vertex_el;
    }

    template <std::regular T>
    [[nodiscard]] static constexpr T minor(const T& vertex_el, const T& hyperedge_el) noexcept {
        return hyperedge_el;
    }

    template <std::regular T>
    [[nodiscard]] static constexpr types::homogeneous_pair<T> majmin(
        const T& vertex_el, const T& hyperedge_el
    ) noexcept {
        return std::make_pair(vertex_el, hyperedge_el);
    }
};

struct hyperedge_major_t {
    static constexpr element_type major_element = element_type::hyperedge;
    static constexpr element_type minor_element = element_type::vertex;

    template <std::regular T>
    [[nodiscard]] static constexpr T major(const T& vertex_el, const T& hyperedge_el) noexcept {
        return hyperedge_el;
    }

    template <std::regular T>
    [[nodiscard]] static constexpr T minor(const T& vertex_el, const T& hyperedge_el) noexcept {
        return vertex_el;
    }

    template <std::regular T>
    [[nodiscard]] static constexpr types::homogeneous_pair<T> majmin(
        const T& vertex_el, const T& hyperedge_el
    ) noexcept {
        return std::make_pair(hyperedge_el, vertex_el);
    }
};

} // namespace impl

namespace type_traits {

template <typename T>
concept c_hypergraph_layout_tag = c_one_of<T, impl::vertex_major_t, impl::hyperedge_major_t>;

} // namespace type_traits

} // namespace hgl
