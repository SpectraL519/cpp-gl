#pragma once

#include "graph_traits.hpp"
#include "types/types.hpp"

#include <algorithm>
#include <iostream>

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

    graph(const types::size_type no_vertices) : _vertices{no_vertices} {
        types::id_type vertex_id = 0ull;
        std::ranges::generate(this->_vertices, [&vertex_id]() {
            return std::make_shared<vertex_type>(vertex_id++);
        });
    }

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

    inline void remove_vertex(const vertex_ptr_type& vertex) {
        const auto vertex_id = vertex->id();
        this->_vertices.erase(std::next(std::begin(this->_vertices), vertex_id));

        // align ids of remainig vertices
        std::for_each(
            std::next(std::begin(this->_vertices), vertex_id),
            this->_vertices.end(),
            [](auto& v) { v->_set_id(v->id() - 1ull); }
        );
    }

    [[nodiscard]] inline types::size_type no_vertices() const {
        return this->_vertices.size();
    }

    [[nodiscard]] inline const vertex_ptr_type& get_vertex(const types::id_type vertex_id) const {
        return this->_vertices.at(vertex_id);
    }

private:
    std::vector<vertex_ptr_type> _vertices = {};
};

} // namespace gl
