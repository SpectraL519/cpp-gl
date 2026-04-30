// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/topology/binary_tree.hpp
/// @brief Generators for regular and bidirectional binary tree topologies.

#pragma once

#include "gl/constants.hpp"
#include "gl/conversion.hpp"
#include "gl/graph.hpp"
#include "gl/util/math.hpp"

#include <initializer_list>

namespace gl::topology {

namespace detail {

[[nodiscard]] gl_attr_force_inline auto get_bintree_target_ids(traits::c_id_type auto source_id) {
    using id_type = std::decay_t<decltype(source_id)>;
    return std::make_pair(
        static_cast<id_type>(2) * source_id + static_cast<id_type>(1),
        static_cast<id_type>(2) * source_id + static_cast<id_type>(2)
    );
}

constexpr size_type min_non_trivial_bin_tree_depth = 2uz;

} // namespace detail

/// @ingroup GL-Topology
/// @brief Generates a regular (regular) binary tree of a specified depth.
///
/// A regular binary tree is a tree where all internal vertices have exactly two children
/// and all leaf vertices are at the same depth. For a given depth \f$d\f$, the graph
/// will contain exactly \f$2^d - 1\f$ vertices.
///
/// If the requested `GraphType` is directed, edges are created pointing from the parent
/// vertex to its children.
///
/// > [!NOTE] Performance for Flat List Graphs
/// >
/// > If the requested `GraphType` satisfies [**c_flat_list_graph**](gl_concepts.md#gl-traits-c-flat-list-graph),
/// > an optimized overload is automatically selected. It internally constructs a standard adjacency list graph
/// > first, and then utilizes the @ref gl::to "to" conversion to flatten it. This is significantly faster than
/// > inserting edges one-by-one into a flat representation.
///
/// @tparam GraphType The target graph type to generate.
/// @param depth The depth (number of levels) of the binary tree. A depth of `1` yields a single root vertex.
/// @return A newly constructed graph representing the regular binary tree.
template <traits::c_graph GraphType>
[[nodiscard]] GraphType regular_binary_tree(size_type depth) {
    using id_type = typename GraphType::id_type;

    if (depth < detail::min_non_trivial_bin_tree_depth)
        return GraphType{depth};

    constexpr size_type base = 2uz;
    constexpr size_type i_begin = 0uz;
    const size_type i_end = depth - 1uz;

    const auto n_vertices = util::upow_sum(base, i_begin, i_end);
    GraphType graph{n_vertices};

    const auto n_source_vertices = n_vertices - util::upow(base, i_end);

    for (id_type source_id = initial_id; source_id < n_source_vertices; ++source_id) {
        const auto target_ids = detail::get_bintree_target_ids(source_id);
        graph.add_edges_from(
            source_id, std::initializer_list<id_type>{target_ids.first, target_ids.second}
        );
    }

    return graph;
}

template <traits::c_flat_list_graph GraphType>
[[nodiscard]] GraphType regular_binary_tree(size_type depth) {
    using base_graph_type = traits::swap_impl_tag_t<GraphType, impl::list_t>;
    return to<impl::flat_list_t>(regular_binary_tree<base_graph_type>(depth));
}

/// @ingroup GL-Topology
/// @brief Generates a regular binary tree with bidirectional edges.
///
/// For directed graphs, this function ensures that for every parent-to-child edge,
/// a reciprocal child-to-parent edge is also created.
///
/// For undirected graphs, this function simply falls back to @ref gl::topology::regular_binary_tree "regular_binary_tree",
/// as undirected edges are inherently bidirectional.
///
/// > [!NOTE] Performance for Flat List Graphs
/// >
/// > If the requested `GraphType` satisfies [**c_flat_list_graph**](gl_concepts.md#gl-traits-c-flat-list-graph),
/// > an optimized overload is automatically selected. It internally constructs a standard adjacency list graph
/// > first, and then utilizes the @ref gl::to "to" conversion to flatten it. This is significantly faster than
/// > inserting edges one-by-one into a flat representation.
///
/// @tparam GraphType The target graph type to generate.
/// @param depth The depth of the binary tree.
/// @return A newly constructed graph representing the bidirectional binary tree.
template <traits::c_graph GraphType>
[[nodiscard]] GraphType bidirectional_regular_binary_tree(size_type depth) {
    if constexpr (traits::c_directed_graph<GraphType>) {
        using id_type = typename GraphType::id_type;

        if (depth < detail::min_non_trivial_bin_tree_depth)
            return GraphType{depth};

        constexpr size_type base = 2uz;
        constexpr size_type i_begin = 0uz;
        const size_type i_end = depth - 1uz;

        const auto n_vertices = util::upow_sum(base, i_begin, i_end);
        GraphType graph{n_vertices};

        const auto n_source_vertices = n_vertices - util::upow(base, i_end);

        for (id_type source_id = initial_id; source_id < n_source_vertices; ++source_id) {
            const auto target_ids = detail::get_bintree_target_ids(source_id);
            graph.add_edges_from(
                source_id, std::initializer_list<id_type>{target_ids.first, target_ids.second}
            );
            graph.add_edge(target_ids.first, source_id);
            graph.add_edge(target_ids.second, source_id);
        }

        return graph;
    }
    else {
        return regular_binary_tree<GraphType>(depth);
    }
}

template <traits::c_flat_list_graph GraphType>
[[nodiscard]] GraphType bidirectional_regular_binary_tree(size_type depth) {
    using base_graph_type = traits::swap_impl_tag_t<GraphType, impl::list_t>;
    return to<impl::flat_list_t>(bidirectional_regular_binary_tree<base_graph_type>(depth));
}

} // namespace gl::topology
