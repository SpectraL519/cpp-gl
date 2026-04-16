// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/decl/impl_tags.hpp"
#include "gl/graph.hpp"
#include "gl/impl/impl_tags.hpp"
#include "gl/traits.hpp"
#include "gl/types/core.hpp"

#include <utility>

namespace gl {

namespace traits {

template <typename GT, traits::c_graph_impl_tag NewImplTag>
requires c_graph<GT> or c_instantiation_of<GT, graph_traits>
struct swap_impl_tag;

template <
    traits::c_graph_directional_tag Dir,
    traits::c_properties VP,
    traits::c_properties EP,
    traits::c_graph_impl_tag OldImplTag,
    traits::c_graph_impl_tag NewImplTag,
    traits::c_id_type IdType>
struct swap_impl_tag<graph_traits<Dir, VP, EP, OldImplTag, IdType>, NewImplTag> {
    using type = graph_traits<Dir, VP, EP, NewImplTag, IdType>;
};

template <
    traits::c_graph_directional_tag Dir,
    traits::c_properties VP,
    traits::c_properties EP,
    traits::c_graph_impl_tag OldImplTag,
    traits::c_graph_impl_tag NewImplTag,
    traits::c_id_type IdType>
struct swap_impl_tag<graph<graph_traits<Dir, VP, EP, OldImplTag, IdType>>, NewImplTag> {
    using type = graph<graph_traits<Dir, VP, EP, NewImplTag, IdType>>;
};

template <typename GT, traits::c_graph_impl_tag NewImplTag>
requires c_graph<GT> or c_instantiation_of<GT, graph_traits>
using swap_impl_tag_t = typename swap_impl_tag<GT, NewImplTag>::type;

} // namespace traits

namespace detail {

template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph_impl_tag SourceImplTag>
struct to_impl {
    template <typename TargetGraph, typename SourceGraph>
    static void convert(TargetGraph& target, SourceGraph& source) {
        target._impl.add_vertices(source.n_vertices());

        for (const auto u : source.vertex_ids()) {
            for (const auto& edge : source.out_edges(u)) {
                if constexpr (traits::c_undirected_graph<SourceGraph>)
                    if (edge.source() > edge.target())
                        continue; // prevent double insertion

                target._impl.add_edge(edge.id(), edge.source(), edge.target());
            }
        }
    }
};

// Conversion: identity
template <traits::c_graph_impl_tag ImplTag>
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
        for (const auto& inc : source_list)
            total_items += inc.size();

        target_list.reserve_segments(source_list.size());
        target_list.reserve_data(total_items);

        for (auto& inc : source_list)
            target_list.push_back(std::move(inc));
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
        for (auto inc : source_list)
            target_list.emplace_back(inc.begin(), inc.end());
    }
};

// Conversion: matrix -> flat matrix
template <>
struct to_impl<impl::flat_matrix_t, impl::matrix_t> {
    template <typename TargetGraph, typename SourceGraph>
    static void convert(TargetGraph& target, SourceGraph& source) {
        auto& target_matrix = target._impl._matrix;
        auto& source_matrix = source._impl._matrix;

        const auto n_vertices = source_matrix.size();
        target_matrix.resize(n_vertices, n_vertices);

        auto* target_ptr = target_matrix.data_ptr();
        for (auto& row : source_matrix)
            target_ptr = std::ranges::move(row, target_ptr).out;
    }
};

// Conversion: flat matrix -> matrix
template <>
struct to_impl<impl::matrix_t, impl::flat_matrix_t> {
    template <typename TargetGraph, typename SourceGraph>
    static void convert(TargetGraph& target, SourceGraph& source) {
        auto& target_matrix = target._impl._matrix;
        auto& source_matrix = source._impl._matrix;

        target_matrix.reserve(source_matrix.n_rows());
        for (auto row : source_matrix.rows())
            target_matrix.emplace_back(row.begin(), row.end());
    }
};

} // namespace detail

/// @brief Converts a graph from one implementation model to another.
/// @tparam TargetImplTag The desired implementation tag (e.g., gl::impl::flat_list_t)
/// @tparam Graph The automatically deduced type of the source graph
/// @param source The graph to convert. After the operation it will be left in a valid, empty state.
/// @return A new graph containing the moved data, structured according to TargetImplTag.
template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph Graph>
[[nodiscard]] auto to(Graph&& source) {
    using source_traits = typename Graph::traits_type;
    using source_impl_tag = typename source_traits::implementation_tag;

    using target_traits = traits::swap_impl_tag_t<source_traits, TargetImplTag>;
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
