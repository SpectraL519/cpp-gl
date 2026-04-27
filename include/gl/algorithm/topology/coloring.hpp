// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm/topology/coloring.hpp
/// @brief Algorithms for detecting and computing graph colorings.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/templates/bfs.hpp"

namespace gl::algorithm {

/// @ingroup GL GL-Algorithm
/// @brief Alias for a container mapping vertex indices to their calculated binary (bipartite) colors.
using bicoloring_type = std::vector<binary_color>;

/// @ingroup GL GL-Algorithm
/// @brief Attempts to compute a valid bipartite (2-color) coloring for the given graph.
///
/// This algorithm utilizes the generic @ref gl::algorithm::bfs "bfs" template to traverse the graph and
/// alternate colors between adjacent vertices. If an edge connects two vertices of the same color
/// (indicating an odd-length cycle), the graph is not bipartite, and the search immediately aborts.
///
/// ### Example Usage
/// ```cpp
/// if (auto coloring = gl::algorithm::bipartite_coloring(graph)) // (1)!
///     gl::algorithm::apply_coloring(graph, *coloring); // (2)!
/// else
///     std::cout << "Graph contains an odd cycle and is not bipartite.\n";
/// ```
///
/// 1\. Attempts to find a valid 2-coloring for the graph. Returns `std::nullopt` if impossible.
///
/// 2\. If successful, directly modifies the graph's internal vertex properties to store the colors.
///     **NOTE:** This operation is only available if the property type of the graph's vertices satisfies [**c_binary_color_properties_type**](gl_concepts.md#gl-traits-c-binary-color-properties-type).
///
/// > [!INFO] Algorithmic Complexity
/// >
/// > The time complexity depends entirely on the underlying representation of `GraphType`:
/// > - **Adjacency List Representations**: \f$O(|V| + |E|)\f$
/// >   - *Includes:* @ref gl::impl::list_t "list_t" and @ref gl::impl::flat_list_t "flat_list_t".
/// > - **Adjacency Matrix Representations**: \f$O(|V|^2)\f$
/// >   - *Includes:* @ref gl::impl::matrix_t "matrix_t" and @ref gl::impl::flat_matrix_t "flat_matrix_t".
/// >   - *Note:* Iterating over adjacent vertices requires scanning the entire \f$|V|\f$-length matrix row.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the graph being traversed. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept. |
/// | PreVisitCallback | Type of the callable executed immediately before a vertex is officially visited. | Must be one of:<br/>- `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after all adjacent edges of a vertex are evaluated. | Must be one of:<br/>- `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
///
/// @param graph The graph to evaluate.
/// @param pre_visit Hook executed immediately before the internal visit logic.
/// @param post_visit Hook executed after all adjacent edges of the current vertex have been enqueued.
/// @return An `std::optional` containing the @ref gl::algorithm::bicoloring_type "bicoloring_type" map if the graph is bipartite. Returns `std::nullopt` otherwise.
/// ### See Also
/// - @ref gl::algorithm::is_bipartite "is_bipartite"
/// - @ref gl::algorithm::apply_coloring "apply_coloring"
/// @hideparams
template <
    traits::c_graph G,
    traits::c_optional_callback<void, typename G::id_type> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PostVisitCallback = empty_callback>
[[nodiscard]] std::optional<bicoloring_type> bipartite_coloring(
    const G& graph, PreVisitCallback pre_visit = {}, PostVisitCallback post_visit = {}
) {
    using edge_type = typename G::edge_type;

    bicoloring_type coloring(graph.n_vertices(), binary_color::value::unset);
    for (const auto root_id : graph.vertex_ids()) {
        if (coloring[root_id].is_set())
            continue;

        // color the root vertex
        coloring[root_id] = bin_color_value::black;

        const bool is_bipartite = bfs(
            graph,
            init_range<G>(root_id),
            empty_callback{}, // visit predicate
            empty_callback{}, // visit callback
            [&coloring](typename G::id_type vertex_id, const edge_type& in_edge)
                -> decision { // enqueue predicate
                if (in_edge.is_loop())
                    return decision::abort; // graph is not bipartite

                const auto pred_id = in_edge.other(vertex_id);

                auto& v_color = coloring[to_idx(vertex_id)];
                auto p_color = coloring[to_idx(pred_id)];

                if (v_color == p_color)
                    return decision::abort; // graph is not bipartite

                if (not v_color.is_set()) {
                    v_color = p_color.next();
                    return true;
                }

                return false;
            },
            pre_visit,
            post_visit
        );

        if (not is_bipartite)
            return std::nullopt;
    }

    return coloring;
}

/// @ingroup GL GL-Algorithm
/// @brief Convenience wrapper for the @ref gl::algorithm::bipartite_coloring "bipartite_coloring" algorithm to check if a graph is bipartite without extracting the exact coloring map.
/// @param graph The graph to evaluate.
/// @return `true` if the graph is bipartite (2-colorable), `false` otherwise.
/// @see @ref gl::algorithm::apply_coloring "apply_coloring"
[[nodiscard]] gl_attr_force_inline bool is_bipartite(const traits::c_graph auto& graph) {
    return bipartite_coloring(graph).has_value();
}

/// @ingroup GL GL-Algorithm
/// @brief Applies a computed range of binary colors to the property payload of each vertex in the graph.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the graph to modify. Must have compatible color properties. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept, and its properties must satisfy [**c_binary_color_properties_type**](gl_concepts.md#gl-traits-c-binary-color-properties-type). |
/// | ColorRange | The type of the range containing the computed colors. | Must satisfy the [**c_sized_range_of**](gl_concepts.md#gl-traits-c-sized-range-of) concept for `binary_color`. |
///
/// @param graph The mutable graph instance whose properties will be updated.
/// @param color_range A sized range (e.g., @ref gl::algorithm::bicoloring_type "bicoloring_type") matching the vertex count.
/// @return `true` if the coloring was successfully applied, `false` if the size of the range does not match the graph's vertex count.
/// ### See Also
/// - @ref gl::algorithm::bipartite_coloring "bipartite_coloring"
/// - @ref gl::algorithm::is_bipartite "is_bipartite"
template <traits::c_graph G, traits::c_sized_range_of<binary_color> ColorRange>
requires(traits::c_binary_color_properties_type<typename G::vertex_properties_type>)
bool apply_coloring(G& graph, const ColorRange& color_range) {
    if (std::ranges::size(color_range) != graph.n_vertices())
        return false;

    auto vertices = graph.vertices(); // store the view to extend its lifetime
    for (const auto& [vertex, color] : std::views::zip(vertices, color_range))
        vertex.properties().color = color;

    return true;
}

} // namespace gl::algorithm
