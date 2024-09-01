#pragma once

#include "gl/constants.hpp"
#include "gl/graph.hpp"
#include "gl/util/pow.hpp"

namespace gl::topology {

template <type_traits::c_graph_type GraphType>
[[nodiscard]] GraphType complete_binary_tree(const types::size_type depth) {
    constexpr types::size_type min_non_trivial_depth = 2ull;

    if (depth < min_non_trivial_depth)
        return GraphType{depth};

    constexpr types::size_type base = 2ull;
    constexpr types::size_type i_begin = 0ull;
    const types::size_type i_end = depth - 1ull;

    const auto n_vertices = util::upow_sum(base, i_begin, i_end);
    GraphType graph{n_vertices};

    return graph;
}

// template <type_traits::c_undirected_graph_type GraphType>
// [[nodiscard]] GraphType full_bipartite(
//     const types::size_type n_vertices_a, const types::size_type n_vertices_b
// ) {
//     const auto n_vertices = n_vertices_a + n_vertices_b;
//     GraphType graph{n_vertices};


//     for (types::id_type source_id = constants::initial_id; source_id < n_vertices_a; source_id++)
//         for (types::id_type destination_id = n_vertices_a; destination_id < n_vertices;
//              destination_id++)
//             graph.add_edge(source_id, destination_id);

//     return graph;
// }

} // namespace gl::topology
