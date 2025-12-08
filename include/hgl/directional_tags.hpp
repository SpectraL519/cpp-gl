// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/types/type_traits.hpp"

namespace hgl {

struct undirected_t {
    using type = std::type_identity_t<undirected_t>;
};

struct bf_directed_t {
    using type = std::type_identity_t<bf_directed_t>;
};

namespace type_traits {

template <typename T>
concept c_hypergraph_directional_tag = c_one_of<T, undirected_t, bf_directed_t>;

} // namespace type_traits

} // namespace hgl
