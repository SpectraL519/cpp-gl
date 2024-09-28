#pragma once

#include "detail/bfs_impl.hpp"
#include "gl/types/properties.hpp"

namespace gl::algorithm {

template <
    type_traits::c_directed_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = empty_callback>
[[nodiscard]] std::optional<std::vector<types::binary_color>> bipartite_coloring(
    const GraphType& graph,
    const std::optional<types::id_type>& root_vertex_id_opt = no_root_vertex,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;
    using color = types::binary_color::value;

    std::vector<types::binary_color> coloring(graph.n_vertices(), color::unset);


}

} // namespace gl::algorithm
