// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "types/type_traits.hpp"
#include "types/types.hpp"

namespace hgl {

struct undirected_t {
    using type = std::type_identity_t<undirected_t>;
};

struct bf_directed_t {
    using type = std::type_identity_t<bf_directed_t>;
};

namespace type_traits {

template <typename T>
concept c_hyperedge_directional_tag = c_one_of<T, undirected_t, bf_directed_t>;

} // namespace type_traits

template <
    type_traits::c_hyperedge_directional_tag DirectionalTag,
    type_traits::c_properties Properties>
class hyperedge_descriptor;

namespace type_traits {

template <typename HE>
concept c_undirected_hyperedge =
    c_instantiation_of<HE, hyperedge_descriptor>
    and std::same_as<typename HE::directional_tag, undirected_t>;

template <typename HE>
concept c_bf_directed_hyperedge =
    c_instantiation_of<HE, hyperedge_descriptor>
    and std::same_as<typename HE::directional_tag, bf_directed_t>;

} // namespace type_traits

} // namespace hgl
