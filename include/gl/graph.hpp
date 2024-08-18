#pragma once

#include "graph_traits.hpp"
#include "impl/impl_tags.hpp"
#include "types/iterator_range.hpp"
#include "types/type_traits.hpp"
#include "types/types.hpp"

#include <algorithm>

#ifdef GL_TESTING

namespace gl_testing {
struct test_graph;
} // namespace gl_testing

#endif

namespace gl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits = graph_traits<>>
class graph {
public:
    using traits_type = GraphTraits;

    using vertex_type = type_traits::vertex_type<traits_type>;
    using vertex_ptr_type = type_traits::vertex_ptr_type<traits_type>;
    using vertex_properties_type = type_traits::vertex_properties_type<traits_type>;

    using vertex_set_type = std::vector<vertex_ptr_type>;
    using vertex_iterator_type = type_traits::iterator_type<vertex_set_type>;
    using vertex_const_iterator_type = type_traits::const_iterator_type<vertex_set_type>;

    // TODO: reverese iterators should be available for bidirectional ranges

    using edge_type = type_traits::edge_type<traits_type>;
    using edge_ptr_type = type_traits::edge_ptr_type<traits_type>;
    using edge_directional_tag = type_traits::edge_directional_tag<traits_type>;
    using edge_properties_type = type_traits::edge_properties_type<traits_type>;

    using impl_tag = type_traits::implementation_tag<traits_type>;
    using impl_type = type_traits::implementation_type<traits_type>;

#ifdef GL_TESTING
    friend struct ::gl_testing::test_graph;
#endif

    graph(const graph&) = delete;
    graph& operator=(const graph&) = delete;

    graph() = default;

    graph(const types::size_type no_vertices) : _vertices(no_vertices) {
        types::id_type vertex_id = 0ull;
        std::ranges::generate(this->_vertices, [&vertex_id]() {
            return std::make_shared<vertex_type>(vertex_id++);
        });
    }

    graph(graph&&) = default;
    graph& operator=(graph&&) = default;

    ~graph() = default;

    [[nodiscard]] inline types::size_type no_vertices() const {
        return this->_vertices.size();
    }

    // [[nodiscard]] inline types::size_type no_unique_edges() const {
    //     return this->_impl.no_unique_edges();
    // }

    inline vertex_ptr_type add_vertex() {
        // this->_impl.add_vertex();
        return this->_vertices.emplace_back(std::make_shared<vertex_type>(this->no_vertices()));
    }

    inline vertex_ptr_type add_vertex(const vertex_properties_type& properties)
    requires(not type_traits::is_default_properties_type_v<vertex_properties_type>)
    {
        return this->_vertices.emplace_back(
            std::make_shared<vertex_type>(this->no_vertices(), properties)
        );
    }

    [[nodiscard]] inline const vertex_ptr_type& get_vertex(const types::id_type vertex_id) const {
        return this->_vertices.at(vertex_id);
    }

    void remove_vertex(const vertex_ptr_type& vertex) {
        const auto vertex_id = vertex->id();
        this->_vertices.erase(std::next(std::begin(this->_vertices), vertex_id));
        // TODO: remove vertex from adjacency impl

        // align ids of remainig vertices
        std::for_each(
            std::next(std::begin(this->_vertices), vertex_id),
            this->_vertices.end(),
            [](auto& v) { v->_id--; }
        );
    }

    [[nodiscard]] inline types::iterator_range<vertex_iterator_type> vertices() {
        return make_iterator_range(this->_vertices);
    }

    [[nodiscard]] inline types::iterator_range<vertex_const_iterator_type> c_vertices() const {
        return make_const_iterator_range(this->_vertices);
    }

private:
    vertex_set_type _vertices = {};
    impl_type _impl;
};

} // namespace gl
