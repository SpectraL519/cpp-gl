#pragma once

#include "graph_traits.hpp"

#include <algorithm>

namespace gl {

template <detail::c_instantiation_of<graph_traits> GraphTraits = graph_traits<>>
class graph {
public:
    using traits_type = GraphTraits;

    using vertex_type = traits::vertex_type<traits_type>;
    using vertex_ptr_type = traits::vertex_ptr_type<traits_type>;
    using vertex_properties_type = traits::vertex_properties_type<traits_type>;

    using edge_directional_tag = traits::edge_directional_tag<traits_type>;
    using edge_type = traits::edge_type<traits_type>;
    using edge_ptr_type = traits::edge_ptr_type<traits_type>;
    using edge_properties_type = traits::edge_properties_type<traits_type>;

    graph() = default;

    graph(const std::size_t no_vertices) : _vertices(graph::_generate_vertices(no_vertices)) {}

    graph(const graph&) = default;
    graph(graph&&) = default;

    graph& operator=(const graph&) = default;
    graph& operator=(graph&&) = default;

    ~graph() = default;

    inline vertex_ptr_type& add_vertex() {
        return this->_vertices.emplace_back(std::make_shared<vertex_type>(this->no_vertices()));
    }

    inline vertex_ptr_type& add_vertex(const vertex_properties_type& properties)
    requires(not detail::is_default_properties_type_v<vertex_properties_type>)
    {
        return this->_vertices.emplace_back(
            std::make_shared<vertex_type>(this->no_vertices(), properties)
        );
    }

    [[nodiscard]] inline std::size_t no_vertices() const {
        return this->_vertices.size();
    }

private:
    static std::vector<vertex_ptr_type> _generate_vertices(const std::size_t no_vertices) {
        return std::views::iota(0ull, no_vertices)
             | std::views::transform([](const std::size_t vertex_id) {
                   return std::make_shared<vertex>(vertex_id);
               })
             | std::ranges::to<std::vector>();
    }

    std::vector<vertex_ptr_type> _vertices = {};
};

} // namespace gl
