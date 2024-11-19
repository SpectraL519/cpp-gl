#pragma once

#include "constants.hpp"
#include "namespaces.hpp"
#include "types.hpp"

#include <gl/algorithms.hpp>
#include <gl/graph.hpp>
#include <gl/graph_file_io.hpp>

namespace fs = std::filesystem;

namespace gl_testing::alg_common {

inline const fs::path data_path(TEST_DATA_PATH);

template <typename T>
requires(lib_tt::c_readable<T>)
[[nodiscard]] std::vector<T> load_list(const lib_t::size_type n, const fs::path& file_path) {
    std::vector<T> list(n);

    std::ifstream file(file_path);
    if (not file) {
        throw std::filesystem::filesystem_error(
            "Cannot read file",
            file_path,
            std::make_error_code(std::errc::no_such_file_or_directory)
        );
    }

    for (lib_t::size_type i = 0; i < n; ++i)
        file >> list[i];

    return list;
}

[[nodiscard]] inline auto has_correct_bin_predecessor(
    const lib::algorithm::predecessors_descriptor& pd
) {
    return [&pd](const lib_t::id_type vertex_id) {
        if (not pd.is_reachable(vertex_id))
            return false;

        if (vertex_id == constants::first_element_idx)
            return pd[vertex_id] == vertex_id;

        return pd[vertex_id] == ((vertex_id - constants::one) / constants::two);
    };
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
