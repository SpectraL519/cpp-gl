#pragma once

#include "detail/bfs_impl.hpp"

namespace gl::algorithm {

template <
    type_traits::c_directed_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = empty_callback>
[[nodiscard]] std::vector<types::id_type> topological_sort(
    const GraphType& graph,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;

    // prepare the vertex in degree list
    const auto vertex_in_deg_view =
        graph.vertex_ids() | std::views::transform([&graph](const auto vertex_id) {
            return graph.in_degree(vertex_id);
        });
    const std::vector<types::size_type> vertex_in_deg_list(
        vertex_in_deg_view.begin(), vertex_in_deg_view.end()
    );

    // prepare the initial queue content view (source vertices)
    const auto source_vertex_view =
        vertex_in_deg_list | [](const auto in_deg) { return in_deg == constants::default_size; };

    std::vector<types::id_type> topological_order;
    topological_order.reserve(graph.n_vertices());

    detail::bfs_impl(
        graph,
        std::vector<detail::vertex_info>(source_vertex_view.begin(), source_vertex_view.end()),
        detail::always_predicate(),
        [&topological_order](const vertex_type& vertex) {
            topological_order.push_back(vetex.id());
        },
        [&vertex_in_deg_list](const vertex_type& vertex) {
            return --vertex_in_deg_list[vertex.id()] == constants::default_size;
        },
        pre_visit,
        post_visit
    );

    return topological_order;
}

} // namespace gl::algorithm
