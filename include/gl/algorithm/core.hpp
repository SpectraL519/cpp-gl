// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/graph.hpp"
#include "gl/types/core.hpp"

#include <functional>

namespace gl::algorithm {

// --- types ---

enum class result_discriminator : bool { ret = true, noret = false };
using enum result_discriminator;

using predecessors_map = std::vector<types::id_type>;

struct vertex_info {
    vertex_info(types::id_type id) : id(id), pred_id(id) {}

    vertex_info(types::id_type id, types::id_type pred_id) : id(id), pred_id(pred_id) {}

    // if id == pred_id then vertex_id is the id of the starting vertex
    // TODO: add has_pred/is_root method?
    types::id_type id;
    types::id_type pred_id;
};

struct predicate_result {
    enum class eval : std::uint8_t { ok, not_ok, unknown };
    using enum eval;

    constexpr predicate_result(const eval value) : value(value) {}

    constexpr predicate_result(const bool value) : value(value ? eval::ok : eval::not_ok) {}

    constexpr predicate_result& operator=(const bool value) {
        this->value = value ? eval::ok : eval::not_ok;
        return *this;
    }

    [[nodiscard]] constexpr operator bool() const {
        return this->value == eval::ok;
    }

    [[nodiscard]] constexpr bool operator==(const eval value) const {
        return this->value == value;
    }

    eval value;
};

struct empty_callback {};

template <result_discriminator ResultDiscriminator, typename ReturnType>
using return_type = std::conditional_t<ResultDiscriminator == algorithm::ret, ReturnType, void>;

template <result_discriminator ResultDiscriminator, typename ReturnType>
using non_void_return_type =
    std::conditional_t<ResultDiscriminator == algorithm::ret, ReturnType, std::monostate>;

// --- constants ---

inline constexpr types::id_type no_root_vertex = constants::invalid_id;

} // namespace gl::algorithm
