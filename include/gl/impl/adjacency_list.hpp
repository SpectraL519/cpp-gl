#pragma once

#include "gl/graph_traits.hpp"
#include "gl/types/iterator_range.hpp"
#include "gl/types/type_traits.hpp"
#include "gl/types/types.hpp"

#include <vector>

namespace gl::impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_list {
public:
    using vertex_type = type_traits::vertex_type<GraphTraits>;
    using vertex_ptr_type = type_traits::vertex_ptr_type<GraphTraits>;

    using edge_type = type_traits::edge_type<GraphTraits>;
    using edge_ptr_type = type_traits::edge_ptr_type<GraphTraits>;
    using edge_directional_tag = type_traits::edge_directional_tag<GraphTraits>;

    using edge_set_type = std::vector<edge_ptr_type>;
    using edge_iterator_type = typename edge_set_type::iterator;
    using edge_const_iterator_type = typename edge_set_type::const_iterator;

    // TODO: reverese iterators should be available only for bidirectional ranges

    using type = std::vector<edge_set_type>;

    adjacency_list() = default;

    adjacency_list(const types::size_type no_vertices) : _list(no_vertices) {}

    // TODO: delete copy constructor and operator

    adjacency_list(const adjacency_list&) = default;
    adjacency_list(adjacency_list&&) = default;

    adjacency_list& operator=(const adjacency_list&) = default;
    adjacency_list& operator=(adjacency_list&&) = default;

    ~adjacency_list() = default;

    [[nodiscard]] inline types::size_type size() const {
        return this->_list.size();
    }

    inline void add_vertex() {
        this->_list.push_back(edge_set_type{});
    }

    inline void remove_vertex(const types::id_type vertex_id) {
        /*
        TODO: remove edges incident to/from the removed vertex
        * for directed graphs:
            * iterate over all vertices
            * for each vertex remove all edges incident with the removed vertex
        * for undirected graphs:
            * iterate over edges adjacent with the removed vertex
            * for each edge e get other vertex v
            * remove all edges incident with the removed vertex in the adj list for vertex v
        */

        this->_list.erase(std::next(std::begin(this->_list), vertex_id));
    }

    // TODO: add tests
    void add_edge(edge_ptr_type edge)
    requires(type_traits::is_directed_v<edge_type>)
    {
        this->_list.at(edge->first()->id()).push_back(std::move(edge));
    }

    void add_edge(edge_ptr_type edge)
    requires(type_traits::is_undirected_v<edge_type>)
    {
        this->_list.at(edge->first()->id()).push_back(edge);
        this->_list.at(edge->second()->id()).push_back(edge);
    }

    [[nodiscard]] inline types::iterator_range<edge_iterator_type> adjacent_edges(
        const types::id_type vertex_id
    ) {
        return make_iterator_range(this->_list.at(vertex_id));
    }

private:
    type _list = {};
};

} // namespace gl::impl
