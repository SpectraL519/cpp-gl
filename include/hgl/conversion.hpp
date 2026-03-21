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
#include <utility>

namespace hgl {

namespace type_traits {

template <typename HT, type_traits::c_hypergraph_impl_tag NewImplTag>
requires c_hypergraph<HT> or c_instantiation_of<HT, hypergraph_traits>
struct swap_impl_tag;

template <
    type_traits::c_hypergraph_directional_tag Dir,
    type_traits::c_properties VP,
    type_traits::c_properties EP,
    type_traits::c_hypergraph_impl_tag OldImplTag,
    type_traits::c_hypergraph_impl_tag NewImplTag>
struct swap_impl_tag<hypergraph_traits<Dir, VP, EP, OldImplTag>, NewImplTag> {
    using type = hypergraph_traits<Dir, VP, EP, NewImplTag>;
};

template <
    type_traits::c_hypergraph_directional_tag Dir,
    type_traits::c_properties VP,
    type_traits::c_properties EP,
    type_traits::c_hypergraph_impl_tag OldImplTag,
    type_traits::c_hypergraph_impl_tag NewImplTag>
struct swap_impl_tag<hypergraph<hypergraph_traits<Dir, VP, EP, OldImplTag>>, NewImplTag> {
    using type = hypergraph<hypergraph_traits<Dir, VP, EP, NewImplTag>>;
};

template <typename HT, type_traits::c_hypergraph_impl_tag NewImplTag>
requires c_hypergraph<HT> or c_instantiation_of<HT, hypergraph_traits>
using swap_impl_tag_t = typename swap_impl_tag<HT, NewImplTag>::type;

} // namespace type_traits

// --- Hypergraph Representation Model Conversion ---

namespace detail {

template <
    type_traits::c_hypergraph_impl_tag TargetImplTag,
    type_traits::c_hypergraph_impl_tag SourceImplTag>
struct to_impl {
    template <typename TargetHypergraph, typename SourceHypergraph>
    static void convert(TargetHypergraph& target, SourceHypergraph& source) {
        // TODO: generic impl
    }
};

// Conversion: identity
template <type_traits::c_graph_impl_tag ImplTag>
struct to_impl<ImplTag, ImplTag> {
    template <typename TargetHypergraph, typename SourceHypergraph>
    static void convert(TargetHypergraph& target, SourceHypergraph& source) {
        target._impl = std::move(source._impl);
    }
};

// Conversion: list -> flat list
template <
    type_traits::c_instantiation_of<impl::flat_list_t> FlatListTag,
    type_traits::c_instantiation_of<impl::list_t> ListTag>
struct to_impl<FlatListTag, ListTag> {
    template <typename TargetHypergraph, typename SourceHypergraph>
    static void convert(TargetHypergraph& target, SourceHypergraph& source) {
        // TODO
    }
};

// Conversion: flat list -> list
template <
    type_traits::c_instantiation_of<impl::list_t> ListTag,
    type_traits::c_instantiation_of<impl::flat_list_t> FlatListTag>
struct to_impl<ListTag, FlatListTag> {
    template <typename TargetHypergraph, typename SourceHypergraph>
    static void convert(TargetHypergraph& target, SourceHypergraph& source) {
        // TODO
    }
};

} // namespace detail

/// @brief Converts a hypergraph from one implementation model to another.
/// @tparam TargetImplTag The desired implementation tag (e.g., gl::impl::flat_list_t)
/// @tparam Hypergraph The automatically deduced type of the source hypergraph
/// @param source The hypergraph to convert. After the operation it will be left in a valid, empty state.
/// @return A new hypergraph containing the moved data, structured according to TargetImplTag.
template <type_traits::c_graph_impl_tag TargetImplTag, type_traits::c_graph Hypergraph>
[[nodiscard]] auto to(Hypergraph&& source) {
    using source_traits = typename Hypergraph::traits_type;
    using source_impl_tag = typename source_traits::implementation_tag;

    using target_traits = type_traits::swap_impl_tag_t<source_traits, TargetImplTag>;
    using target_hypergraph = hypergraph<target_traits>;

    target_hypergraph target;

    detail::to_impl<TargetImplTag, source_impl_tag>::convert(target, source);

    target._n_vertices = std::exchange(source._n_vertices, 0uz);
    target._n_hyperedges = std::exchange(source._n_hyperedges, 0uz);
    target._vertex_properties = std::move(source._vertex_properties);
    target._hyperedge_properties = std::move(source._hyperedge_properties);
    source._impl = typename Hypergraph::implementation_type();

    return target;
}

// --- Hypergraph to Hypergraph Conversion ---

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
