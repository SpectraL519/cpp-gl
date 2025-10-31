// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/graph_utility.hpp"

#include <functional>

namespace gl {

namespace algorithm {

// TODO: align docs
enum class result_discriminator : bool { ret = true, noret = false };
using enum result_discriminator;

struct empty_callback {};

struct vertex_info {
    vertex_info(types::id_type id) : id(id), source_id(id) {}

    vertex_info(types::id_type id, types::id_type source_id) : id(id), source_id(source_id) {}

    // if id == source_id then vertex_id is the id of the starting vertex
    types::id_type id;
    types::id_type source_id;
};

template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
struct edge_info {
    using edge_type = EdgeType;

    types::const_ref_wrap<edge_type> edge;
    types::id_type source_id;
};

struct predecessors_descriptor {
    using predecessor_type = std::optional<types::id_type>;

    predecessors_descriptor(const types::size_type n_vertices) : predecessors(n_vertices) {
        predecessors.shrink_to_fit();
    }

    virtual ~predecessors_descriptor() = default;

    [[nodiscard]] gl_attr_force_inline bool is_reachable(const types::id_type vertex_id) const {
        return this->at(vertex_id).has_value();
    }

    [[nodiscard]] const predecessor_type& operator[](const types::size_type i) const {
        return this->predecessors[i];
    }

    [[nodiscard]] predecessor_type& operator[](const types::size_type i) {
        return this->predecessors[i];
    }

    [[nodiscard]] const predecessor_type& at(const types::size_type i) const {
        return this->predecessors.at(i);
    }

    [[nodiscard]] predecessor_type& at(const types::size_type i) {
        return this->predecessors.at(i);
    }

    std::vector<predecessor_type> predecessors;
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

} // namespace algorithm

namespace type_traits {

template <typename F>
concept c_empty_callback = std::same_as<F, algorithm::empty_callback>;

template <typename F, typename ReturnType, typename... Args>
concept c_optional_callback = c_empty_callback<F> or std::is_invocable_r_v<ReturnType, F, Args...>;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_vertex_callback =
    std::is_invocable_r_v<ReturnType, F, const typename GraphType::vertex_type&, Args...>;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_edge_callback =
    std::is_invocable_r_v<ReturnType, F, const typename GraphType::edge_type&, Args...>;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_optional_vertex_callback =
    c_optional_callback<F, ReturnType, const typename GraphType::vertex_type&, Args...>;

template <typename F, typename GraphType, typename ReturnType, typename... Args>
concept c_optional_edge_callback =
    c_optional_callback<F, ReturnType, const typename GraphType::edge_type&, Args...>;

} // namespace type_traits

namespace algorithm::impl {

template <result_discriminator ResultDiscriminator, typename ReturnType>
using alg_return_type = std::conditional_t<ResultDiscriminator == algorithm::ret, ReturnType, void>;

template <result_discriminator ResultDiscriminator, typename ReturnType>
using alg_return_type_non_void =
    std::conditional_t<ResultDiscriminator == algorithm::ret, ReturnType, std::monostate>;

} // namespace algorithm::impl

} // namespace gl
