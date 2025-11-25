#pragma once

#include "constants.hpp"
#include "types.hpp"

#include <gl/algorithm.hpp>
#include <gl/graph.hpp>
#include <gl/graph_file_io.hpp>

namespace fs = std::filesystem;

namespace gl_testing::alg_common {

inline const fs::path data_path(TEST_DATA_PATH);

template <typename T>
requires(gl::type_traits::c_readable<T>)
[[nodiscard]] std::vector<T> load_list(const gl::types::size_type n, const fs::path& file_path) {
    std::vector<T> list(n);

    std::ifstream file(file_path);
    if (not file) {
        throw std::filesystem::filesystem_error(
            "Cannot read file",
            file_path,
            std::make_error_code(std::errc::no_such_file_or_directory)
        );
    }

    for (gl::types::size_type i = 0; i < n; ++i)
        file >> list[i];

    return list;
}

[[nodiscard]] inline auto has_correct_bin_predecessor(
    const gl::algorithm::predecessors_descriptor& pd
) {
    return [&pd](const gl::types::id_type vertex_id) {
        if (not pd.is_reachable(vertex_id))
            return false;

        if (vertex_id == constants::first_element_idx)
            return pd[vertex_id] == vertex_id;

        return pd[vertex_id] == ((vertex_id - constants::one) / constants::two);
    };
}

template <gl::type_traits::c_instantiation_of<gl::vertex_descriptor> VertexType>
requires(std::same_as<typename VertexType::properties_type, types::visited_property>)
struct vertex_visited_projection {
    [[nodiscard]] bool operator()(const VertexType& vertex) const {
        return vertex.properties().visited;
    }
};

} // namespace gl_testing::alg_common
