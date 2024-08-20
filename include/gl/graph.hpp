#pragma once

#include "graph_traits.hpp"
#include "impl/impl_tags.hpp"
#include "types/iterator_range.hpp"
#include "types/type_traits.hpp"
#include "types/types.hpp"

#include <algorithm>

#ifdef GL_TESTING

namespace gl_testing {
template <typename TraitsType>
struct test_graph;
} // namespace gl_testing

#endif

namespace gl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits = graph_traits<>>
class graph {
public:
    using traits_type = GraphTraits;

    using implementation_tag = typename traits_type::implementation_tag;
    using implementation_type = typename implementation_tag::template type<traits_type>;

    using vertex_type = typename traits_type::vertex_type;
    using vertex_ptr_type = typename traits_type::vertex_ptr_type;
    using vertex_properties_type = typename traits_type::vertex_properties_type;

    using vertex_set_type = std::vector<vertex_ptr_type>;
    using vertex_iterator_type = typename vertex_set_type::iterator;
    using vertex_const_iterator_type = typename vertex_set_type::const_iterator;

    // TODO: reverese iterators should be available for bidirectional ranges

    using edge_type = typename traits_type::edge_type;
    using edge_ptr_type = typename traits_type::edge_ptr_type;
    using edge_directional_tag = typename traits_type::edge_directional_tag;
    using edge_properties_type = typename traits_type::edge_properties_type;

    using edge_set_type = typename implementation_type::edge_set_type;
    using edge_iterator_type = typename implementation_type::edge_iterator_type;
    using edge_const_iterator_type = typename implementation_type::edge_const_iterator_type;

#ifdef GL_TESTING
    friend struct ::gl_testing::test_graph<traits_type>;
#endif

    graph(const graph&) = delete;
    graph& operator=(const graph&) = delete;

    graph() = default;

    graph(const types::size_type n_vertices) : _vertices(n_vertices), _impl(n_vertices) {
        types::id_type vertex_id = 0ull;
        std::ranges::generate(this->_vertices, [&vertex_id]() {
            return std::make_shared<vertex_type>(vertex_id++);
        });
    }

    graph(graph&&) = default;
    graph& operator=(graph&&) = default;

    ~graph() = default;

    [[nodiscard]] inline types::size_type n_vertices() const {
        return this->_vertices.size();
    }

    [[nodiscard]] inline types::size_type n_unique_edges() const {
        return this->_impl.n_unique_edges();
    }

    inline vertex_ptr_type add_vertex() {
        this->_impl.add_vertex();
        return this->_vertices.emplace_back(std::make_shared<vertex_type>(this->n_vertices()));
    }

    inline vertex_ptr_type add_vertex(const vertex_properties_type& properties)
    requires(not type_traits::is_default_properties_type_v<vertex_properties_type>)
    {
        this->_impl.add_vertex();
        return this->_vertices.emplace_back(
            std::make_shared<vertex_type>(this->n_vertices(), properties)
        );
    }

    [[nodiscard]] inline const vertex_ptr_type& get_vertex(const types::id_type vertex_id) const {
        return this->_vertices.at(vertex_id);
    }

    void remove_vertex(const types::size_type vertex_id) {
        const auto& vertex = this->get_vertex(vertex_id);
        this->_impl.remove_vertex(vertex);
        this->_vertices.erase(std::next(std::begin(this->_vertices), vertex_id));

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

    [[nodiscard]] inline types::iterator_range<edge_iterator_type> adjacent_edges(
        const types::id_type vertex_id
    ) {
        return this->_impl.adjacent_edges(vertex_id);
    }

    [[nodiscard]] inline types::iterator_range<edge_const_iterator_type> adjacent_edges_c(
        const types::id_type vertex_id
    ) const {
        return this->_impl.adjacent_edges_c(vertex_id);
    }

private:
    vertex_set_type _vertices = {};
    implementation_type _impl = {};
};

} // namespace gl
