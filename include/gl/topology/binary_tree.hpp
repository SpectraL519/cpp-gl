#pragma once

#include "gl/constants.hpp"
#include "gl/graph.hpp"
#include "gl/util/pow.hpp"

namespace gl::topology {

namespace detail {

[[nodiscard]] gl_attr_force_inline auto get_binary_destination_ids(const types::size_type source_id
) {
    return std::make_pair(
        constants::two * source_id + constants::one, constants::two * source_id + constants::two
    );
}

using vertex_id_range = std::vector<types::id_type>;

} // namespace detail

template <type_traits::c_graph_type GraphType>
[[nodiscard]] GraphType complete_binary_tree(const types::size_type depth) {
    constexpr types::size_type min_non_trivial_depth = 2ull;

    if (depth < min_non_trivial_depth)
        return GraphType{depth};

    constexpr types::size_type base = constants::two;
    constexpr types::size_type i_begin = constants::zero;
    const types::size_type i_end = depth - constants::one;

    const auto n_vertices = util::upow_sum(constants::two, i_begin, i_end);
    GraphType graph{n_vertices};

    const auto n_source_vertices = n_vertices - util::upow(base, i_end);

    for (types::id_type source_id = constants::zero; source_id < n_source_vertices; source_id++) {
        const auto destination_ids = detail::get_binary_destination_ids(source_id);
        graph.add_edges_from(source_id, detail::vertex_id_range{destination_ids.first, destination_ids.second});
    }

    return graph;
}

template <type_traits::c_graph_type GraphType>
[[nodiscard]] GraphType bidirectional_complete_binary_tree(const types::size_type depth) {
    if constexpr (type_traits::is_directed_v<GraphType>) {
        constexpr types::size_type min_non_trivial_depth = 2ull;

        if (depth < min_non_trivial_depth)
            return GraphType{depth};

        constexpr types::size_type base = constants::two;
        constexpr types::size_type i_begin = constants::zero;
        const types::size_type i_end = depth - constants::one;

        const auto n_vertices = util::upow_sum(constants::two, i_begin, i_end);
        GraphType graph{n_vertices};

        const auto n_source_vertices = n_vertices - util::upow(base, i_end);

        for (types::id_type source_id = constants::zero; source_id < n_source_vertices;
             source_id++) {
            const auto destination_ids = detail::get_binary_destination_ids(source_id);
            graph.add_edges_from(source_id, detail::vertex_id_range{destination_ids.first, destination_ids.second});
            graph.add_edge(destination_ids.first, source_id);
            graph.add_edge(destination_ids.second, source_id);
        }

        return graph;
    }
    else {
        return complete_binary_tree<GraphType>(depth);
    }
}

} // namespace gl::topology
