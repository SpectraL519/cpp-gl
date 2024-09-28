#pragma once

#include "namespaces.hpp"
#include "types.hpp"

#include <gl/graph.hpp>
#include <gl/graph_file_io.hpp>

namespace fs = std::filesystem;

namespace gl_testing::alg_common {

inline const fs::path data_path(TEST_DATA_PATH);

template <std::integral I>
[[nodiscard]] std::vector<I> read_integral_list(
    const lib_t::size_type n, const fs::path& file_path
) {
    std::vector<I> list(n);

    std::ifstream file(file_path);
    if (not file) {
        throw std::filesystem::filesystem_error(
            "Cannot read file",
            file_path,
            std::make_error_code(std::errc::no_such_file_or_directory)
        );
    }

    for (lib_t::size_type i = 0; i < n; i++)
        file >> list[i];

    return list;
}

template <lib_tt::c_graph GraphType1, lib_tt::c_graph GraphType2>
[[nodiscard]] auto are_vertices_equivalent(const GraphType1& g1, const GraphType2& g2) {
    return [&](const lib_t::id_type vertex_id) {
        const auto adj_edges_1 = g1.adjacent_edges(vertex_id);
        const auto adj_edges_2 = g2.adjacent_edges(vertex_id);

        if (adj_edges_2.distance() != adj_edges_1.distance())
            return false;

        for (const auto& edge : adj_edges_1) {
            if (not g2.has_edge(edge.first_id(), edge.second_id()))
                return false;
        }

        for (const auto& edge : adj_edges_2) {
            if (not g1.has_edge(edge.first_id(), edge.second_id()))
                return false;
        }

        return true;
    };
}

template <lib_tt::c_instantiation_of<lib::vertex_descriptor> VertexType>
requires(std::same_as<typename VertexType::properties_type, types::visited_property>)
struct vertex_visited_projection {
    [[nodiscard]] bool operator()(const VertexType& vertex) const {
        return vertex.properties.visited;
    }
};

} // namespace gl_testing::alg_common
