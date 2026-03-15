// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/graph.hpp"
#include "gl/types/types.hpp"
#include "hgl/hypergraph.hpp"

#include <algorithm>

namespace hgl {

/*
0. Common prefix: make_, to_, as_

1. Clique expansion:
- make_clique_graph
- make_projection, project, project_as<G>

2. Star expansion / bipartite representation
- make_bipartite_graph
- make_star_graph
- make_incidence_graph
- make_flow_graph (for BF hypergraphs)

3. Line graph
- make_line_graph
*/

template <type_traits::c_undirected_graph G>
[[nodiscard]] G make_clique_graph(const type_traits::c_undirected_hypergraph auto& h) noexcept {
    using edge_vertices = std::pair<types::id_type, types::id_type>;
    std::vector<edge_vertices> edges;

    for (const auto eid : h.hyperedge_ids()) {
        const auto clique_vertices = h.incident_vertex_ids(eid) | std::ranges::to<std::vector>();
        for (std::size_t i = 0uz; i < clique_vertices.size(); i++) {
            for (std::size_t j = 0uz; j < i; j++) {
                const auto [u, v] = std::minmax(clique_vertices[i], clique_vertices[j]);
                edges.emplace_back(u, v);
            }
        }
    }

    std::ranges::sort(edges);
    const auto rem = std::ranges::unique(edges);
    edges.erase(rem.begin(), rem.end());

    G g{h.order()};
    for (const auto& edge : edges)
        g.add_edge(edge.first, edge.second);
    return g;
}

} // namespace hgl
