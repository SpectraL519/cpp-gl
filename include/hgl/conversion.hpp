// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/conversion.hpp
/// @brief Defines utilities for hypergraph representation model conversion and projection into standard graphs.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/conversion.hpp"
#include "gl/graph.hpp"
#include "gl/types/core.hpp"
#include "hgl/directional_tags.hpp"
#include "hgl/hypergraph.hpp"
#include "hgl/types.hpp"

#include <algorithm>
#include <ranges>
#include <utility>

namespace hgl {

namespace traits {

/// @ingroup HGL-Traits
/// @brief Utility trait type used to swap the representation tag of a hypergraph traits or hypergraph type.
/// ### See Also:
/// - @ref hgl::to "to" : For the function that utilizes this trait to perform hypergraph conversions between different representations.
template <typename HT, traits::c_hypergraph_repr_tag NewReprTag>
requires c_hypergraph<HT> or c_instantiation_of<HT, hypergraph_traits>
struct swap_repr_tag;

/// @ingroup HGL-Traits
/// @brief Specialization of @ref hgl::traits::swap_repr_tag "swap_repr_tag" for the @ref hgl::hypergraph_traits "hypergraph_traits" type.
template <
    traits::c_hypergraph_directional_tag Dir,
    traits::c_properties VP,
    traits::c_properties EP,
    traits::c_hypergraph_repr_tag OldReprTag,
    traits::c_hypergraph_repr_tag NewReprTag>
struct swap_repr_tag<hypergraph_traits<Dir, VP, EP, OldReprTag>, NewReprTag> {
    using type = hypergraph_traits<Dir, VP, EP, NewReprTag>;
};

/// @ingroup HGL-Traits
/// @brief Specialization of @ref hgl::traits::swap_repr_tag "swap_repr_tag" for the @ref hgl::hypergraph "hypergraph" type.
template <
    traits::c_hypergraph_directional_tag Dir,
    traits::c_properties VP,
    traits::c_properties EP,
    traits::c_hypergraph_repr_tag OldReprTag,
    traits::c_hypergraph_repr_tag NewReprTag>
struct swap_repr_tag<hypergraph<hypergraph_traits<Dir, VP, EP, OldReprTag>>, NewReprTag> {
    using type = hypergraph<hypergraph_traits<Dir, VP, EP, NewReprTag>>;
};

/// @ingroup HGL-Traits
/// @brief Alias template for easier usage of the @ref hl::traits::swap_repr_tag "swap_repr_tag" trait to resolve the swapped type directly.
/// ### See Also:
/// - @ref hgl::to "to" : For the function that utilizes this trait to perform hypergraph conversions between different representations.
template <typename HT, traits::c_hypergraph_repr_tag NewReprTag>
requires c_hypergraph<HT> or c_instantiation_of<HT, hypergraph_traits>
using swap_repr_tag_t = typename swap_repr_tag<HT, NewReprTag>::type;

} // namespace traits

// --- Hypergraph Representation Model Conversion ---

namespace detail {

template <traits::c_hypergraph_repr_tag TargetReprTag, traits::c_hypergraph_repr_tag SourceReprTag>
struct to_impl {
    template <typename TargetHypergraph, typename SourceHypergraph>
    static void convert(TargetHypergraph& target, SourceHypergraph& source) {
        target._impl.add_vertices(source.n_vertices());
        target._impl.add_hyperedges(source.n_hyperedges());

        if constexpr (traits::c_undirected_hypergraph<TargetHypergraph>) {
            for (const auto eid : source.hyperedge_ids())
                for (const auto vid : source.incident_vertex_ids(eid))
                    target._impl.bind(vid, eid);
        }
        else {
            for (const auto eid : source.hyperedge_ids()) {
                for (const auto vid : source.tail_ids(eid))
                    target._impl.bind_tail(vid, eid);
                for (const auto vid : source.head_ids(eid))
                    target._impl.bind_head(vid, eid);
            }
        }
    }
};

// Conversion: identity
template <traits::c_hypergraph_repr_tag ReprTag>
struct to_impl<ReprTag, ReprTag> {
    template <typename TargetHypergraph, typename SourceHypergraph>
    static void convert(TargetHypergraph& target, SourceHypergraph& source) {
        target._impl = std::move(source._impl);
    }
};

// Conversion: list -> flat list (same layout)
template <traits::c_hypergraph_layout_tag LayoutTag>
struct to_impl<repr::flat_list_t<LayoutTag>, repr::list_t<LayoutTag>> {
    template <typename TargetHypergraph, typename SourceHypergraph>
    static void convert(TargetHypergraph& target, SourceHypergraph& source) {
        using dir_tag = typename std::decay_t<decltype(target)>::directional_tag;

        auto& target_impl = target._impl;
        auto& source_impl = source._impl;

        if constexpr (std::same_as<LayoutTag, repr::bidirectional_t>) {
            convert_asym<dir_tag>(target_impl._v_list, source_impl._v_list);
            convert_asym<dir_tag>(target_impl._e_list, source_impl._e_list);
        }
        else {
            convert_asym<dir_tag>(target_impl, source_impl);
        }
    }

    template <std::same_as<undirected_t> DirTag>
    static void convert_asym(auto& target_impl, auto& source_impl) {
        auto& target_list = target_impl._storage;
        auto& source_list = source_impl._major_storage;

        std::size_t total_items = 0uz;
        for (const auto& adj : source_list)
            total_items += adj.size();

        target_list.reserve_segments(source_list.size());
        target_list.reserve_data(total_items);

        for (auto& adj : source_list)
            target_list.push_back(std::move(adj));
    }

    template <std::same_as<bf_directed_t> DirTag>
    static void convert_asym(auto& target_impl, auto& source_impl) {
        // convert tail
        auto& target_tail = target_impl._tail_storage;
        auto& source_tail = source_impl._tail_storage;
        std::size_t total_tail_size = 0uz;
        for (const auto& adj : source_tail)
            total_tail_size += adj.size();

        target_tail.reserve_segments(source_tail.size());
        target_tail.reserve_data(total_tail_size);
        for (auto& adj : source_tail)
            target_tail.push_back(std::move(adj));

        // convert head
        auto& target_head = target_impl._head_storage;
        auto& source_head = source_impl._head_storage;

        std::size_t total_head_size = 0uz;
        for (const auto& adj : source_head)
            total_head_size += adj.size();

        target_head.reserve_segments(source_head.size());
        target_head.reserve_data(total_head_size);
        for (auto& adj : source_head)
            target_head.push_back(std::move(adj));
    }
};

// Conversion: flat list -> list (same layout)
template <traits::c_hypergraph_layout_tag LayoutTag>
struct to_impl<repr::list_t<LayoutTag>, repr::flat_list_t<LayoutTag>> {
    template <typename TargetHypergraph, typename SourceHypergraph>
    static void convert(TargetHypergraph& target, SourceHypergraph& source) {
        using dir_tag = typename std::decay_t<decltype(target)>::directional_tag;

        auto& target_impl = target._impl;
        auto& source_impl = source._impl;

        if constexpr (std::same_as<LayoutTag, repr::bidirectional_t>) {
            convert_asym<dir_tag>(target_impl._v_list, source_impl._v_list);
            convert_asym<dir_tag>(target_impl._e_list, source_impl._e_list);
        }
        else {
            convert_asym<dir_tag>(target_impl, source_impl);
        }
    }

    template <std::same_as<undirected_t> DirTag>
    static void convert_asym(auto& target_impl, auto& source_impl) {
        auto& target_list = target_impl._major_storage;
        auto& source_list = source_impl._storage;

        target_list.reserve(source_list.size());
        for (auto adj : source_list)
            target_list.emplace_back(adj.begin(), adj.end());
    }

    template <std::same_as<bf_directed_t> DirTag>
    static void convert_asym(auto& target_impl, auto& source_impl) {
        // conver tail
        auto& target_tail = target_impl._tail_storage;
        auto& source_tail = source_impl._tail_storage;

        target_tail.reserve(source_tail.size());
        for (auto adj : source_tail)
            target_tail.emplace_back(adj.begin(), adj.end());

        // convert head
        auto& target_head = target_impl._head_storage;
        auto& source_head = source_impl._head_storage;

        target_head.reserve(source_head.size());
        for (auto adj : source_head)
            target_head.emplace_back(adj.begin(), adj.end());
    }
};

} // namespace detail

/// @ingroup HGL-Core
/// @brief Converts a hypergraph from one representation model to another.
///
/// This function efficiently transforms a hypergraph's underlying memory representation (e.g., from a standard incidence list to a flattened incidence list) while preserving its exact topology, properties, and identifiers.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :---------- | :--------- |
/// | TargetReprTag | The representation tag of the desired target representation (e.g., `hgl::repr::flat_list_t`). | [**c_hypergraph_repr_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-repr-tag) |
/// | Hypergraph | The type of the source hypergraph, which will be automatically deduced from the function argument. | [**c_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph) |
///
/// @param source The hypergraph to convert. After the operation it will be left in a valid, empty state.
/// @return A new hypergraph containing the moved data, structured according to `TargetReprTag`.
///
/// ### See Also
/// - @ref hgl::traits::swap_repr_tag "swap_repr_tag" : For the trait used to resolve the target hypergraph type with the swapped representation tag.
template <traits::c_hypergraph_repr_tag TargetReprTag, traits::c_hypergraph Hypergraph>
[[nodiscard]] auto to(Hypergraph&& source) {
    using source_traits = typename Hypergraph::traits_type;
    using source_impl_tag = typename source_traits::representation_tag;

    using target_traits = traits::swap_repr_tag_t<source_traits, TargetReprTag>;
    using target_hypergraph = hypergraph<target_traits>;

    target_hypergraph target;

    detail::to_impl<TargetReprTag, source_impl_tag>::convert(target, source);

    target._n_vertices = std::exchange(source._n_vertices, 0uz);
    target._n_hyperedges = std::exchange(source._n_hyperedges, 0uz);
    target._vertex_properties = std::move(source._vertex_properties);
    target._hyperedge_properties = std::move(source._hyperedge_properties);
    source._impl = typename Hypergraph::representation_type();

    return target;
}

// --- Hypergraph to Hypergraph Conversion ---

/// @ingroup HGL-Core
/// @brief Computes the projection (clique expansion) of an *undirected* hypergraph.
///
/// Projects the hypergraph onto a standard undirected graph. Each hyperedge in the hypergraph is expanded
/// into a clique (a fully connected subgraph) connecting all of its incident vertices in the resulting graph.
/// Duplicate edges generated by multiple hyperedges overlapping on the same vertices are collapsed into a single edge.
///
/// > [!NOTE] Performance for Flat List Graphs
/// >
/// > If the requested target graph `G` satisfies [**c_flat_list_graph**](gl_concepts.md#gl-traits-c-flat-list-graph),
/// > an optimized overload is automatically selected. It internally constructs a standard adjacency list graph
/// > first, and then utilizes the @ref gl::to conversion to flatten it. This is significantly faster than
/// > inserting edges one-by-one into a flat representation.
///
/// @tparam G The target standard graph type to construct. Must satisfy [**c_undirected_graph**](gl_concepts.md#gl-traits-c-undirected-graph).
/// @param h The source undirected hypergraph.
/// @return A standard undirected graph representing the projection.
template <gl::traits::c_undirected_graph G>
[[nodiscard]] G projection(const traits::c_undirected_hypergraph auto& h) {
    using edge_vertices = homogeneous_pair<typename G::id_type>;
    std::vector<edge_vertices> edges;

    for (const auto eid : h.hyperedge_ids()) {
        const auto clique_vertices = h.incident_vertex_ids(eid) | std::ranges::to<std::vector>();
        if (clique_vertices.size() == 1uz) {
            const auto v = clique_vertices.front();
            edges.emplace_back(v, v);
            continue;
        }

        for (auto i = 0uz; i < clique_vertices.size(); i++) {
            for (auto j = 0uz; j < i; j++) {
                const auto [u, v] = std::minmax(clique_vertices[i], clique_vertices[j]);
                edges.emplace_back(u, v);
            }
        }
    }

    std::ranges::sort(edges);
    const auto rem = std::ranges::unique(edges);
    edges.erase(rem.begin(), rem.end());

    G g{h.n_vertices()};
    for (const auto& edge : edges)
        g.add_edge(edge.first, edge.second);
    return g;
}

template <gl::traits::c_undirected_graph G>
requires std::same_as<typename G::traits_type::representation_tag, gl::repr::flat_list_t>
[[nodiscard]] G projection(const traits::c_undirected_hypergraph auto& h) {
    using list_graph = gl::traits::swap_repr_tag_t<G, gl::repr::list_t>;
    return gl::to<gl::repr::flat_list_t>(projection<list_graph>(h));
}

/// @ingroup HGL-Core
/// @brief Computes the projection of a *BF-directed* hypergraph.
///
/// Projects the hypergraph onto a standard directed graph. For each hyperedge, directed edges are created
/// from every vertex in the hyperedge's tail (source) to every vertex in its head (destination).
/// Duplicate edges generated by multiple hyperedges overlapping on the same vertices are collapsed into a single edge.
///
/// > [!NOTE] Performance for Flat List Graphs
/// >
/// > If the requested target graph `G` satisfies [**c_flat_list_graph**](gl_concepts.md#gl-traits-c-flat-list-graph),
/// > an optimized overload is automatically selected. It internally constructs a standard adjacency list graph
/// > first, and then utilizes the @ref gl::to conversion to flatten it. This is significantly faster than
/// > inserting edges one-by-one into a flat representation.
///
/// @tparam G The target standard graph type to construct. Must satisfy [**c_directed_graph**](gl_concepts.md#gl-traits-c-directed-graph).
/// @param h The source bf-directed hypergraph.
/// @return A standard directed graph representing the projection.
template <gl::traits::c_directed_graph G>
[[nodiscard]] G projection(const traits::c_bf_directed_hypergraph auto& h) {
    using edge_vertices = homogeneous_pair<typename G::id_type>;
    std::vector<edge_vertices> edges;

    for (const auto eid : h.hyperedge_ids()) {
        auto sources = h.tail_ids(eid);
        const auto targets = h.head_ids(eid) | std::ranges::to<std::vector>();
        for (const auto u : sources)
            for (const auto v : targets)
                edges.emplace_back(u, v);
    }

    std::ranges::sort(edges);
    const auto rem = std::ranges::unique(edges);
    edges.erase(rem.begin(), rem.end());

    G g{h.n_vertices()};
    for (const auto& [u, v] : edges)
        g.add_edge(u, v);

    return g;
}

template <gl::traits::c_directed_graph G>
requires std::same_as<typename G::traits_type::representation_tag, gl::repr::flat_list_t>
[[nodiscard]] G projection(const traits::c_bf_directed_hypergraph auto& h) {
    using list_graph = gl::traits::swap_repr_tag_t<G, gl::repr::list_t>;
    return gl::to<gl::repr::flat_list_t>(projection<list_graph>(h));
}

/// @ingroup HGL-Core
/// @brief Computes the bipartite incidence graph representation of an *undirected* hypergraph.
///
/// Converts the hypergraph into a standard bipartite graph where both the original vertices and the
/// original hyperedges are represented as standard graph vertices. Undirected edges are created between
/// a vertex node and a hyperedge node if they are incident.
///
/// > [!NOTE] ID Shifting
/// >
/// > To ensure uniqueness in the resulting graph, the IDs of the hyperedge nodes are shifted by `h.n_vertices()`.
/// > For example, hyperedge ID `0` becomes vertex ID `h.n_vertices() + 0` in the resulting graph.
///
/// > [!NOTE] Performance for Flat List Graphs
/// >
/// > If the requested target graph `G` satisfies [**c_flat_list_graph**](gl_concepts.md#gl-traits-c-flat-list-graph),
/// > an optimized overload is automatically selected. It internally constructs a standard adjacency list graph
/// > first, and then utilizes the @ref gl::to conversion to flatten it. This is significantly faster than
/// > inserting edges one-by-one into a flat representation.
///
/// @tparam G The target standard graph type to construct. Must satisfy [**c_undirected_graph**](gl_concepts.md#gl-traits-c-undirected-graph).
/// @param h The source undirected hypergraph.
/// @return A standard undirected bipartite graph representing the incidence structure.
template <gl::traits::c_undirected_graph G>
[[nodiscard]] G incidence_graph(const traits::c_undirected_hypergraph auto& h) {
    using g_id_type = typename G::id_type;

    G g{h.n_vertices() + h.n_hyperedges()};
    const auto align_edge_id =
        [shift = static_cast<g_id_type>(h.n_vertices())](const auto eid) -> g_id_type {
        return eid + shift;
    };

    for (const auto vid : h.vertex_ids()) {
        const auto targets =
            h.incident_hyperedge_ids(vid) | std::views::transform(align_edge_id)
            | std::ranges::to<std::vector>();
        g.add_edges_from(vid, targets);
    }

    return g;
}

template <gl::traits::c_undirected_graph G>
requires std::same_as<typename G::traits_type::representation_tag, gl::repr::flat_list_t>
[[nodiscard]] G incidence_graph(const traits::c_undirected_hypergraph auto& h) {
    using list_graph = gl::traits::swap_repr_tag_t<G, gl::repr::list_t>;
    return gl::to<gl::repr::flat_list_t>(incidence_graph<list_graph>(h));
}

/// @ingroup HGL-Core
/// @brief Computes the bipartite incidence graph representation of a *BF-directed* hypergraph.
///
/// Converts the hypergraph into a standard bipartite directed graph. Directed edges are created
/// from original *tail* vertex nodes to the hyperedge nodes, and from the hyperedge nodes to the
/// original *head* vertex nodes.
///
/// > [!NOTE] ID Shifting
/// >
/// > To ensure uniqueness in the resulting graph, the IDs of the hyperedge nodes are shifted by `h.n_vertices()`.
/// > For example, hyperedge ID `0` becomes vertex ID `h.n_vertices() + 0` in the resulting graph.
///
/// > [!NOTE] Performance for Flat List Graphs
/// >
/// > If the requested target graph `G` satisfies [**c_flat_list_graph**](gl_concepts.md#gl-traits-c-flat-list-graph),
/// > an optimized overload is automatically selected. It internally constructs a standard adjacency list graph
/// > first, and then utilizes the @ref gl::to conversion to flatten it. This is significantly faster than
/// > inserting edges one-by-one into a flat representation.
///
/// @tparam G The target standard graph type to construct. Must satisfy [**c_directed_graph**](gl_concepts.md#gl-traits-c-directed-graph).
/// @param h The source bf-directed hypergraph.
/// @return A standard directed bipartite graph representing the incidence structure.
template <gl::traits::c_directed_graph G>
[[nodiscard]] G incidence_graph(const traits::c_bf_directed_hypergraph auto& h) {
    using g_id_type = typename G::id_type;

    G g{h.n_vertices() + h.n_hyperedges()};
    const auto align_edge_id =
        [shift = static_cast<g_id_type>(h.n_vertices())](const auto eid) -> g_id_type {
        return eid + shift;
    };

    for (const auto vid : h.vertex_ids()) {
        const auto targets =
            h.out_hyperedge_ids(vid) | std::views::transform(align_edge_id)
            | std::ranges::to<std::vector<g_id_type>>();
        g.add_edges_from(vid, targets);
    }
    for (const auto eid : h.hyperedge_ids()) {
        const auto targets = h.head_ids(eid) | std::ranges::to<std::vector<g_id_type>>();
        g.add_edges_from(align_edge_id(eid), targets);
    }

    return g;
}

template <gl::traits::c_directed_graph G>
requires std::same_as<typename G::traits_type::representation_tag, gl::repr::flat_list_t>
[[nodiscard]] G incidence_graph(const traits::c_bf_directed_hypergraph auto& h) {
    using list_graph = gl::traits::swap_repr_tag_t<G, gl::repr::list_t>;
    return gl::to<gl::repr::flat_list_t>(incidence_graph<list_graph>(h));
}

} // namespace hgl
