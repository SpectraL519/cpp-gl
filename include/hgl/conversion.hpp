// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/graph.hpp"
#include "gl/types/types.hpp"
#include "hgl/hypergraph.hpp"

#include <algorithm>
#include <ranges>

namespace hgl {

template <type_traits::c_undirected_graph G>
[[nodiscard]] G projection(const type_traits::c_undirected_hypergraph auto& h) {
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

template <type_traits::c_directed_graph G>
[[nodiscard]] G projection(const type_traits::c_bf_directed_hypergraph auto& h) {
    using edge_vertices = std::pair<types::id_type, types::id_type>;
    std::vector<edge_vertices> edges;

    for (const auto eid : h.hyperedge_ids()) {
        auto sources = h.tail_vertex_ids(eid);
        const auto targets = h.head_vertex_ids(eid) | std::ranges::to<std::vector>();
        for (const auto u : sources)
            for (const auto v : targets)
                edges.emplace_back(u, v);
    }

    std::ranges::sort(edges);
    const auto rem = std::ranges::unique(edges);
    edges.erase(rem.begin(), rem.end());

    G g{h.order()};
    for (const auto& [u, v] : edges)
        g.add_edge(u, v);

    return g;
}

template <type_traits::c_undirected_graph G>
[[nodiscard]] G incidence_graph(const type_traits::c_undirected_hypergraph auto& h) {
    G g{h.order() + h.size()};
    const auto align_edge_id = [shift = h.order()](const auto eid) { return eid + shift; };

    for (const auto vid : h.vertex_ids()) {
        const auto targets =
            h.incident_hyperedge_ids(vid) | std::views::transform(align_edge_id)
            | std::ranges::to<std::vector>();
        g.add_edges_from(vid, targets);
    }

    return g;
}

template <type_traits::c_directed_graph G>
[[nodiscard]] G incidence_graph(const type_traits::c_bf_directed_hypergraph auto& h) {
    G g{h.order() + h.size()};
    const auto align_edge_id = [shift = h.order()](const auto eid) { return eid + shift; };

    for (const auto vid : h.vertex_ids()) {
        const auto targets =
            h.out_hyperedge_ids(vid) | std::views::transform(align_edge_id)
            | std::ranges::to<std::vector>();
        g.add_edges_from(vid, targets);
    }
    for (const auto eid : h.hyperedge_ids()) {
        const auto targets = h.head_vertex_ids(eid) | std::ranges::to<std::vector>();
        g.add_edges_from(align_edge_id(eid), targets);
    }

    return g;
}

} // namespace hgl
