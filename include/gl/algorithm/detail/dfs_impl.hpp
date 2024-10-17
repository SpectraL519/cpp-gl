// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "common.hpp"

#include <stack>
#include <vector>

namespace gl::algorithm::detail {

template <
    type_traits::c_graph GraphType,
    type_traits::c_sized_range_of<types::vertex_info> InitStackRangeType =
        std::vector<types::vertex_info>,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = types::empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = types::empty_callback>
bool dfs_impl(
    const GraphType& graph,
        const InitStackRangeType& initial_stack_content,
    const types::vertex_callback<GraphType, bool>& visit_vertex_pred,
    const types::vertex_callback<GraphType, void, types::id_type>& visit,
    const types::
        vertex_callback<GraphType, std::optional<bool>, const typename GraphType::edge_type&>&
            enque_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    if (initial_stack_content.size() == constants::default_size)
        return true;

#ifdef ALG_DEQUE
    using vertex_stack_type = std::vector<types::vertex_info>;
    vertex_stack_type vertex_stack;
    // const auto avg_vdeg = 2 * graph.n_unique_edges() / graph.n_vertices();
    vertex_stack.reserve(graph.n_vertices());
    std::cout << "init capacity: " << vertex_stack.capacity() << std::endl;

    for (const auto& vinfo : initial_stack_content)
        vertex_stack.push_back(vinfo);
#else
    // prepare the vertex stack
    using vertex_stack_type = std::stack<types::vertex_info>;

    vertex_stack_type vertex_stack;
    for (const auto& vinfo : initial_stack_content)
        vertex_stack.push(vinfo);
#endif

    // search the graph
    while (not vertex_stack.empty()) {
#ifdef ALG_DEQUE
        const auto vinfo = vertex_stack.back();
        vertex_stack.pop_back();
#else
        const auto vinfo = vertex_stack.top();
        vertex_stack.pop();
#endif

        const auto& vertex = graph.get_vertex(vinfo.id);
        if (not visit_vertex_pred(vertex))
            continue;

        if constexpr (not type_traits::c_empty_callback<PreVisitCallback>)
            pre_visit(vertex);

        visit(vertex, vinfo.source_id);

        for (const auto& edge : graph.adjacent_edges(vinfo.id)) {
            const auto& incident_vertex = edge.incident_vertex(vertex);
            const auto enqueue = enque_vertex_pred(incident_vertex, edge);
            if (not enqueue.has_value())
                return false;

            if (enqueue.value())
#ifdef ALG_DEQUE
                vertex_stack.emplace_back(incident_vertex.id(), vinfo.id);
#else
                vertex_stack.emplace(incident_vertex.id(), vinfo.id);
#endif
        }

        if constexpr (not type_traits::c_empty_callback<PostVisitCallback>)
            post_visit(vertex);
    }

#ifdef ALG_DEQUE
    std::cout << "final capacity: " << vertex_stack.capacity() << std::endl;
#endif

    return true;
}

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = types::empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = types::empty_callback>
void rdfs_impl(
    const GraphType& graph,
    const typename GraphType::vertex_type& vertex,
    const types::id_type source_id,
    const types::vertex_callback<GraphType, bool>& visit_vertex_pred,
    const types::vertex_callback<GraphType, void, types::id_type>& visit,
    const types::vertex_callback<GraphType, bool, const typename GraphType::edge_type&>&
        enque_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    if (not visit_vertex_pred(vertex))
        return;

    if constexpr (not type_traits::c_empty_callback<PreVisitCallback>)
        pre_visit(vertex);

    visit(vertex, source_id);

    // recursively search vertices adjacent to the current vertex
    const auto vertex_id = vertex.id();
    for (const auto& edge : graph.adjacent_edges(vertex_id)) {
        const auto& incident_vertex = edge.incident_vertex(vertex);
        if (enque_vertex_pred(incident_vertex, edge))
            rdfs_impl(
                graph,
                incident_vertex,
                vertex_id,
                visit_vertex_pred,
                visit,
                enque_vertex_pred,
                pre_visit,
                post_visit
            );
    }

    if constexpr (not type_traits::c_empty_callback<PostVisitCallback>)
        post_visit(vertex);
}

} // namespace gl::algorithm::detail
