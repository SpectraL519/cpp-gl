// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/graph.hpp"

namespace gl::topology {

template <type_traits::c_graph GraphType>
[[nodiscard]] GraphType path(const types::size_type n_vertices) {
    GraphType graph{n_vertices};

    for (types::id_type source_id = 0uz; source_id < n_vertices - 1uz; ++source_id)
        graph.add_edge(source_id, source_id + 1uz);

    return graph;
}

template <type_traits::c_graph GraphType>
[[nodiscard]] GraphType bidirectional_path(const types::size_type n_vertices) {
    if constexpr (type_traits::c_directed_graph<GraphType>) {
        GraphType graph{n_vertices};

        for (types::id_type source_id = 0uz; source_id < n_vertices - 1uz; ++source_id) {
            const auto target_id = source_id + 1uz;
            graph.add_edge(source_id, target_id);
            graph.add_edge(target_id, source_id);
        }

        return graph;
    }
    else {
        return path<GraphType>(n_vertices);
    }
}

} // namespace gl::topology
