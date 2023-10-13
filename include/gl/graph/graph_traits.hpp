#pragma once

#include "gl/graph/detail/abstract_graph_base.hpp"
#include "gl/utility/types.hpp"



namespace gl {

// valid graph type trait
template <typename Graph>
concept gl_graph_c = requires {
    { Graph::is_directed() } -> std::same_as<bool>;
    std::is_base_of_v<
        detail::abstract_graph_base<
            Graph::is_directed(),
            typename Graph::vertex_type,
            typename Graph::container_specifier
        >,
        Graph
    >;
};

// directed specifier traits
constexpr directed_specifier_type directed = true;
constexpr directed_specifier_type undirected = false;

} // namespace gl
