// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/constants.hpp"
#include "gl/graph.hpp"
#include "gl/types/core.hpp"

#include <functional>

namespace gl::algorithm {

// --- general types ---

struct empty_callback {};

struct decision {
    enum class eval : std::uint8_t { accept, reject, abort };
    using enum eval;

    constexpr decision(const eval value) : value(value) {}

    constexpr decision(const bool value) : value(value ? eval::accept : eval::reject) {}

    constexpr decision& operator=(const bool value) {
        this->value = value ? eval::accept : eval::reject;
        return *this;
    }

    [[nodiscard]] constexpr operator bool() const {
        return this->value == eval::accept;
    }

    [[nodiscard]] constexpr bool operator==(const eval value) const {
        return this->value == value;
    }

    eval value;
};

enum class result_discriminator : bool { ret = true, noret = false };
using enum result_discriminator;

template <result_discriminator Result, typename ReturnType>
using return_type = std::conditional_t<Result == algorithm::ret, ReturnType, void>;

template <result_discriminator Result, typename ReturnType>
using non_void_return_type =
    std::conditional_t<Result == algorithm::ret, ReturnType, std::monostate>;

// --- traversal types ---

template <traits::c_graph GraphType>
using predecessors_map = std::vector<typename GraphType::id_type>;

template <traits::c_graph GraphType>
struct search_node {
    using id_type = typename GraphType::id_type;

    search_node(id_type vertex_id) : vertex_id(vertex_id), pred_id(vertex_id) {}

    search_node(id_type vertex_id, id_type pred_id) : vertex_id(vertex_id), pred_id(pred_id) {}

    [[nodiscard]] gl_attr_force_inline bool is_root() const noexcept {
        return this->vertex_id != invalid_id and this->vertex_id == this->pred_id;
    }

    id_type vertex_id;
    id_type pred_id;
};

// --- constants ---

template <traits::c_id_type IdType>
inline constexpr IdType no_root_v = invalid_id_v<IdType>;

struct no_root_t {
    template <traits::c_id_type IdType>
    [[nodiscard]] constexpr operator IdType() const noexcept {
        return no_root_v<IdType>;
    }

    template <traits::c_id_type IdType>
    [[nodiscard]] friend constexpr bool operator==(const IdType& lhs, no_root_t) noexcept {
        return lhs == no_root_v<IdType>;
    }
};

inline constexpr no_root_t no_root{};

} // namespace gl::algorithm
