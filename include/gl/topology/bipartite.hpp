// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/conversion.hpp"
#include "gl/graph.hpp"
#include "gl/impl/impl_tags.hpp"

namespace gl::topology {

template <traits::c_graph GraphType>
[[nodiscard]] GraphType biclique(
    const types::size_type n_vertices_a, const types::size_type n_vertices_b
) {
    const auto n_vertices = n_vertices_a + n_vertices_b;
    GraphType graph{n_vertices};

    for (types::id_type source_id = constants::initial_id; source_id < n_vertices_a; ++source_id) {
        for (types::id_type target_id = n_vertices_a; target_id < n_vertices; ++target_id) {
            graph.add_edge(source_id, target_id);
            if constexpr (traits::c_directed_graph<GraphType>)
                graph.add_edge(target_id, source_id);
        }
    }

    return graph;
}

template <traits::c_flat_list_graph GraphType>
[[nodiscard]] GraphType biclique(
    const types::size_type n_vertices_a, const types::size_type n_vertices_b
) {
    using base_graph_type = traits::swap_impl_tag_t<GraphType, impl::list_t>;
    return to<impl::flat_list_t>(biclique<base_graph_type>(n_vertices_a, n_vertices_b));
}

} // namespace gl::topology
