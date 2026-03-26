// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/conversion.hpp"
#include "gl/graph.hpp"

namespace gl::topology {

template <traits::c_graph GraphType>
[[nodiscard]] GraphType clique(size_type n_vertices) {
    using id_type = typename GraphType::id_type;

    GraphType graph{n_vertices};

    for (auto source_id = constants::initial_id<id_type>; source_id < n_vertices; ++source_id) {
        for (auto target_id = constants::initial_id<id_type>; target_id < source_id; ++target_id) {
            graph.add_edge(source_id, target_id);
            if constexpr (traits::c_directed_graph<GraphType>)
                graph.add_edge(target_id, source_id);
        }
    }

    return graph;
}

template <traits::c_flat_list_graph GraphType>
[[nodiscard]] GraphType clique(size_type n_vertices) {
    using base_graph_type = traits::swap_impl_tag_t<GraphType, impl::list_t>;
    return to<impl::flat_list_t>(clique<base_graph_type>(n_vertices));
}

} // namespace gl::topology
