// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/decl/impl_tags.hpp"
#include "gl/graph.hpp"
#include "gl/impl/impl_tags.hpp"
#include "gl/types/type_traits.hpp"

#include <utility>

namespace gl {

namespace type_traits {

template <typename GT, type_traits::c_graph_impl_tag NewImplTag>
requires c_graph<GT> or c_instantiation_of<GT, graph_traits>
struct swap_impl_tag;

template <
    type_traits::c_edge_directional_tag Dir,
    type_traits::c_properties VP,
    type_traits::c_properties EP,
    type_traits::c_graph_impl_tag OldImplTag,
    type_traits::c_graph_impl_tag NewImplTag>
struct swap_impl_tag<graph_traits<Dir, VP, EP, OldImplTag>, NewImplTag> {
    using type = graph_traits<Dir, VP, EP, NewImplTag>;
};

template <
    type_traits::c_edge_directional_tag Dir,
    type_traits::c_properties VP,
    type_traits::c_properties EP,
    type_traits::c_graph_impl_tag OldImplTag,
    type_traits::c_graph_impl_tag NewImplTag>
struct swap_impl_tag<graph<graph_traits<Dir, VP, EP, OldImplTag>>, NewImplTag> {
    using type = graph<graph_traits<Dir, VP, EP, NewImplTag>>;
};

template <typename GT, type_traits::c_graph_impl_tag NewImplTag>
requires c_graph<GT> or c_instantiation_of<GT, graph_traits>
using swap_impl_tag_t = typename swap_impl_tag<GT, NewImplTag>::type;

} // namespace type_traits

namespace detail {

template <type_traits::c_graph_impl_tag TargetImplTag, type_traits::c_graph_impl_tag SourceImplTag>
struct to_impl {
    template <typename TargetGraph, typename SourceGraph>
    static void convert(TargetGraph& target, SourceGraph& source) {
        target._impl.add_vertices(source.order());

        for (const auto u : source.vertex_ids()) {
            for (const auto& edge : source.out_edges(u)) {
                if constexpr (type_traits::c_undirected_graph<SourceGraph>)
                    if (edge.source() > edge.target())
                        continue; // prevent double insertion

                target._impl.add_edge(edge.id(), edge.source(), edge.target());
            }
        }
    }
};

// Conversion: identity
template <type_traits::c_graph_impl_tag ImplTag>
struct to_impl<ImplTag, ImplTag> {
    template <typename TargetGraph, typename SourceGraph>
    static void convert(TargetGraph& target, SourceGraph& source) {
        target._impl = std::move(source._impl);
    }
};

// Conversion: list -> flat list
template <>
struct to_impl<impl::flat_list_t, impl::list_t> {
    template <typename TargetGraph, typename SourceGraph>
    static void convert(TargetGraph& target, SourceGraph& source) {
        auto& target_list = target._impl._list;
        auto& source_list = source._impl._list;

        std::size_t total_items = 0uz;
        for (const auto& adj : source_list)
            total_items += adj.size();

        target_list.reserve_segments(source_list.size());
        target_list.reserve_data(total_items);

        for (auto& adj : source_list)
            target_list.push_back(std::move(adj));
    }
};

// Conversion: flat list -> list
template <>
struct to_impl<impl::list_t, impl::flat_list_t> {
    template <typename TargetGraph, typename SourceGraph>
    static void convert(TargetGraph& target, SourceGraph& source) {
        auto& target_list = target._impl._list;
        auto& source_list = source._impl._list;

        target_list.reserve(source_list.size());
        for (auto adj : source_list)
            target_list.emplace_back(adj.begin(), adj.end());
    }
};

} // namespace detail

/// @brief Converts a graph from one implementation layout to another.
/// @tparam TargetImplTag The desired implementation tag (e.g., gl::impl::flat_list_t)
/// @tparam Graph The automatically deduced type of the source graph
/// @param source The graph to convert. After the operation it will be left in a valid, empty state.
/// @return A new graph containing the moved data, structured according to TargetImplTag.
template <type_traits::c_graph_impl_tag TargetImplTag, type_traits::c_graph Graph>
[[nodiscard]] auto to(Graph&& source) {
    using source_traits = typename Graph::traits_type;
    using source_impl_tag = typename source_traits::implementation_tag;

    using target_traits = type_traits::swap_impl_tag_t<source_traits, TargetImplTag>;
    using target_graph = graph<target_traits>;

    target_graph target;

    detail::to_impl<TargetImplTag, source_impl_tag>::convert(target, source);

    target._n_vertices = std::exchange(source._n_vertices, 0uz);
    target._n_edges = std::exchange(source._n_edges, 0uz);
    target._vertex_properties = std::move(source._vertex_properties);
    target._edge_properties = std::move(source._edge_properties);
    source._impl = typename Graph::implementation_type();

    return target;
}

} // namespace gl
