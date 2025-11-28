// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hypergraph_traits.hpp"

namespace hgl {

template <type_traits::c_instantiation_of<hypergraph_traits> HypergraphTraits = hypergraph_traits<>>
class hypergraph final {
public:
    using traits_type = HypergraphTraits;
    using implementation_tag = typename traits_type::implementation_tag;

    using vertex_type = typename traits_type::vertex_type;
    using vertex_properties_type = typename traits_type::vertex_properties_type;

    using hyperedge_type = typename traits_type::hyperedge_type;
    using hyperedge_properties_type = typename traits_type::hyperedge_properties_type;

    hypergraph(const hypergraph&) = delete;
    hypergraph& operator=(const hypergraph&) = delete;

    hypergraph() = default;

private:
    types::size_type _n_vertices = 0uz;
    types::size_type _n_hyperedges = 0uz;
};

} // namespace hgl
