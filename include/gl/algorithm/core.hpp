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

template <traits::c_graph GraphType>
using predecessors_map = std::vector<typename GraphType::id_type>;

template <traits::c_graph GraphType>
struct vertex_info {
    using id_type = typename GraphType::id_type;

    vertex_info(id_type id) : id(id), pred_id(id) {}

    vertex_info(id_type id, id_type pred_id) : id(id), pred_id(pred_id) {}

    // if id == pred_id then vertex_id is the id of the starting vertex
    id_type id;
    id_type pred_id;
};

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

struct empty_callback {};

template <result_discriminator ResultDiscriminator, typename ReturnType>
using return_type = std::conditional_t<ResultDiscriminator == algorithm::ret, ReturnType, void>;

template <result_discriminator ResultDiscriminator, typename ReturnType>
using non_void_return_type =
    std::conditional_t<ResultDiscriminator == algorithm::ret, ReturnType, std::monostate>;

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
