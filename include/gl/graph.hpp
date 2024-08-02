#pragma once

#include "graph_traits.hpp"
#include "types/iterator_range.hpp"
#include "types/types.hpp"

#include <algorithm>
#include <iostream>

#ifdef GL_TESTING

namespace gl_testing {
struct test_graph;
} // namespace gl_testing

#endif

namespace gl {

template <detail::c_instantiation_of<graph_traits> GraphTraits = graph_traits<>>
class graph {
public:
    using traits_type = GraphTraits;

    using vertex_type = traits::vertex_type<traits_type>;
    using vertex_properties_type = traits::vertex_properties_type<traits_type>;
    using vertex_ptr_type = traits::vertex_ptr_type<traits_type>;
    using vertex_list_type = std::vector<vertex_ptr_type>;
    using vertex_iterator_type = typename vertex_list_type::iterator;
    using vertex_const_iterator_type = typename vertex_list_type::const_iterator;

    using edge_directional_tag = traits::edge_directional_tag<traits_type>;
    using edge_type = traits::edge_type<traits_type>;
    using edge_ptr_type = traits::edge_ptr_type<traits_type>;
    using edge_properties_type = traits::edge_properties_type<traits_type>;

#ifdef GL_TESTING
    friend struct ::gl_testing::test_graph;
#endif

    graph() = default;

    graph(const types::size_type no_vertices) : _vertices(no_vertices) {
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

    [[nodiscard]] inline types::size_type no_vertices() const {
        return this->_vertices.size();
    }

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

    [[nodiscard]] inline const vertex_ptr_type& get_vertex(const types::id_type vertex_id) const {
        return this->_vertices.at(vertex_id);
    }

    [[nodiscard]] inline types::iterator_range<vertex_iterator_type> vertex_range() {
        return make_iterator_range(this->_vertices);
    }

    [[nodiscard]] inline types::iterator_range<vertex_const_iterator_type> vertex_crange() const {
        return make_iterator_range(this->_vertices.cbegin(), this->_vertices.cend());
    }

    void remove_vertex(const vertex_ptr_type& vertex) {
        const auto vertex_id = vertex->id();
        this->_vertices.erase(std::next(std::begin(this->_vertices), vertex_id));

        // align ids of remainig vertices
        std::for_each(
            std::next(std::begin(this->_vertices), vertex_id),
            this->_vertices.end(),
            [](auto& v) { v->_set_id(v->id() - 1ull); }
        );
    }

private:
    vertex_list_type _vertices = {};
};

} // namespace gl
