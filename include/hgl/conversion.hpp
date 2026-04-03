// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

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

template <typename HT, traits::c_hypergraph_impl_tag NewImplTag>
requires c_hypergraph<HT> or c_instantiation_of<HT, hypergraph_traits>
struct swap_impl_tag;

template <
    traits::c_hypergraph_directional_tag Dir,
    traits::c_properties VP,
    traits::c_properties EP,
    traits::c_hypergraph_impl_tag OldImplTag,
    traits::c_hypergraph_impl_tag NewImplTag>
struct swap_impl_tag<hypergraph_traits<Dir, VP, EP, OldImplTag>, NewImplTag> {
    using type = hypergraph_traits<Dir, VP, EP, NewImplTag>;
};

template <
    traits::c_hypergraph_directional_tag Dir,
    traits::c_properties VP,
    traits::c_properties EP,
    traits::c_hypergraph_impl_tag OldImplTag,
    traits::c_hypergraph_impl_tag NewImplTag>
struct swap_impl_tag<hypergraph<hypergraph_traits<Dir, VP, EP, OldImplTag>>, NewImplTag> {
    using type = hypergraph<hypergraph_traits<Dir, VP, EP, NewImplTag>>;
};

template <typename HT, traits::c_hypergraph_impl_tag NewImplTag>
requires c_hypergraph<HT> or c_instantiation_of<HT, hypergraph_traits>
using swap_impl_tag_t = typename swap_impl_tag<HT, NewImplTag>::type;

} // namespace traits

// --- Hypergraph Representation Model Conversion ---

namespace detail {

template <traits::c_hypergraph_impl_tag TargetImplTag, traits::c_hypergraph_impl_tag SourceImplTag>
struct to_impl {
    template <typename TargetHypergraph, typename SourceHypergraph>
    static void convert(TargetHypergraph& target, SourceHypergraph& source) {
        target._impl.add_vertices(source.order());
        target._impl.add_hyperedges(source.size());

        if constexpr (traits::c_undirected_hypergraph<TargetHypergraph>) {
            for (const auto eid : source.hyperedge_ids())
                for (const auto vid : source.incident_vertex_ids(eid))
                    target._impl.bind(vid, eid);
        }
        else {
            for (const auto eid : source.hyperedge_ids()) {
                for (const auto vid : source.tail_vertex_ids(eid))
                    target._impl.bind_tail(vid, eid);
                for (const auto vid : source.head_vertex_ids(eid))
                    target._impl.bind_head(vid, eid);
            }
        }
    }
};

// Conversion: identity
template <traits::c_hypergraph_impl_tag ImplTag>
struct to_impl<ImplTag, ImplTag> {
    template <typename TargetHypergraph, typename SourceHypergraph>
    static void convert(TargetHypergraph& target, SourceHypergraph& source) {
        target._impl = std::move(source._impl);
    }
};

// Conversion: list -> flat list (same layout)
template <traits::c_hypergraph_layout_tag LayoutTag>
struct to_impl<impl::flat_list_t<LayoutTag>, impl::list_t<LayoutTag>> {
    template <typename TargetHypergraph, typename SourceHypergraph>
    static void convert(TargetHypergraph& target, SourceHypergraph& source) {
        using dir_tag = typename std::decay_t<decltype(target)>::directional_tag;

        auto& target_impl = target._impl;
        auto& source_impl = source._impl;

        if constexpr (std::same_as<LayoutTag, impl::bidirectional_t>) {
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
struct to_impl<impl::list_t<LayoutTag>, impl::flat_list_t<LayoutTag>> {
    template <typename TargetHypergraph, typename SourceHypergraph>
    static void convert(TargetHypergraph& target, SourceHypergraph& source) {
        using dir_tag = typename std::decay_t<decltype(target)>::directional_tag;

        auto& target_impl = target._impl;
        auto& source_impl = source._impl;

        if constexpr (std::same_as<LayoutTag, impl::bidirectional_t>) {
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

/// @brief Converts a hypergraph from one implementation model to another.
/// @tparam TargetImplTag The desired implementation tag (e.g., gl::impl::flat_list_t)
/// @tparam Hypergraph The automatically deduced type of the source hypergraph
/// @param source The hypergraph to convert. After the operation it will be left in a valid, empty state.
/// @return A new hypergraph containing the moved data, structured according to TargetImplTag.
template <traits::c_hypergraph_impl_tag TargetImplTag, traits::c_hypergraph Hypergraph>
[[nodiscard]] auto to(Hypergraph&& source) {
    using source_traits = typename Hypergraph::traits_type;
    using source_impl_tag = typename source_traits::implementation_tag;

    using target_traits = traits::swap_impl_tag_t<source_traits, TargetImplTag>;
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

template <gl::traits::c_undirected_graph G>
[[nodiscard]] G projection(const traits::c_undirected_hypergraph auto& h) {
    using edge_vertices = hgl::homogeneous_pair<typename G::id_type>;
    std::vector<edge_vertices> edges;

    for (const auto eid : h.hyperedge_ids()) {
        const auto clique_vertices = h.incident_vertex_ids(eid) | std::ranges::to<std::vector>();
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

    G g{h.order()};
    for (const auto& edge : edges)
        g.add_edge(edge.first, edge.second);
    return g;
}

template <gl::traits::c_undirected_graph G>
requires std::same_as<typename G::traits_type::implementation_tag, gl::impl::flat_list_t>
[[nodiscard]] G projection(const traits::c_undirected_hypergraph auto& h) {
    using list_graph = gl::traits::swap_impl_tag_t<G, gl::impl::list_t>;
    return gl::to<gl::impl::flat_list_t>(projection<list_graph>(h));
}

template <gl::traits::c_directed_graph G>
[[nodiscard]] G projection(const traits::c_bf_directed_hypergraph auto& h) {
    using edge_vertices = hgl::homogeneous_pair<typename G::id_type>;
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

template <gl::traits::c_directed_graph G>
requires std::same_as<typename G::traits_type::implementation_tag, gl::impl::flat_list_t>
[[nodiscard]] G projection(const traits::c_bf_directed_hypergraph auto& h) {
    using list_graph = gl::traits::swap_impl_tag_t<G, gl::impl::list_t>;
    return gl::to<gl::impl::flat_list_t>(projection<list_graph>(h));
}

template <gl::traits::c_undirected_graph G>
[[nodiscard]] G incidence_graph(const traits::c_undirected_hypergraph auto& h) {
    using g_id_type = typename G::id_type;

    G g{h.order() + h.size()};
    const auto align_edge_id = [shift = static_cast<g_id_type>(h.order())](const auto eid) -> g_id_type {
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
requires std::same_as<typename G::traits_type::implementation_tag, gl::impl::flat_list_t>
[[nodiscard]] G incidence_graph(const traits::c_undirected_hypergraph auto& h) {
    using list_graph = gl::traits::swap_impl_tag_t<G, gl::impl::list_t>;
    return gl::to<gl::impl::flat_list_t>(incidence_graph<list_graph>(h));
}

template <gl::traits::c_directed_graph G>
[[nodiscard]] G incidence_graph(const traits::c_bf_directed_hypergraph auto& h) {
    using g_id_type = typename G::id_type;

    G g{h.order() + h.size()};
    const auto align_edge_id = [shift = static_cast<g_id_type>(h.order())](const auto eid) -> g_id_type {
        return eid + shift;
    };

    for (const auto vid : h.vertex_ids()) {
        const auto targets =
            h.out_hyperedge_ids(vid) | std::views::transform(align_edge_id)
            | std::ranges::to<std::vector<g_id_type>>();
        g.add_edges_from(vid, targets);
    }
    for (const auto eid : h.hyperedge_ids()) {
        const auto targets = h.head_vertex_ids(eid) | std::ranges::to<std::vector<g_id_type>>();
        g.add_edges_from(align_edge_id(eid), targets);
    }

    return g;
}

template <gl::traits::c_directed_graph G>
requires std::same_as<typename G::traits_type::implementation_tag, gl::impl::flat_list_t>
[[nodiscard]] G incidence_graph(const traits::c_bf_directed_hypergraph auto& h) {
    using list_graph = gl::traits::swap_impl_tag_t<G, gl::impl::list_t>;
    return gl::to<gl::impl::flat_list_t>(incidence_graph<list_graph>(h));
}

} // namespace hgl
