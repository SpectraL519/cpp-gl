#pragma once

#include "gl/graph_traits.hpp"
#include "gl/types/iterator_range.hpp"
#include "gl/types/type_traits.hpp"
#include "gl/types/types.hpp"

#include <algorithm>
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

    // TODO: reverese iterators should be available for bidirectional ranges

    using type = std::vector<edge_set_type>;

    adjacency_list(const adjacency_list&) = delete;
    adjacency_list& operator=(const adjacency_list&) = delete;

    adjacency_list() = default;

    adjacency_list(const types::size_type no_vertices) : _list(no_vertices) {}

    adjacency_list(adjacency_list&&) = default;
    adjacency_list& operator=(adjacency_list&&) = default;

    ~adjacency_list() = default;

    [[nodiscard]] inline types::size_type no_vertices() const {
        return this->_list.size();
    }

    [[nodiscard]] inline types::size_type no_unique_edges() const {
        return this->_no_unique_edges;
    }

    inline void add_vertex() {
        this->_list.push_back(edge_set_type{});
    }

    void remove_vertex(const vertex_ptr_type& vertex)
    requires(type_traits::is_directed_v<edge_type>)
    {
        for (auto& adjacent_edges : this->_list) {
            const auto rem_subrange =
                std::ranges::remove_if(adjacent_edges, [&vertex](const auto& edge) {
                    return edge->is_incident_with(vertex);
                });
            this->_no_unique_edges -=
                std::ranges::distance(rem_subrange.begin(), rem_subrange.end());
            adjacent_edges.erase(rem_subrange.begin(), rem_subrange.end());
        }
        this->_list.erase(std::next(std::begin(this->_list), vertex->id()));
    }

    void remove_vertex(const vertex_ptr_type& vertex)
    requires(type_traits::is_undirected_v<edge_type>)
    {
        // TODO: optimize for multiedges
        // * the edges adjacent to incident_vertex should be processed once

        const auto vertex_id = vertex->id();

        for (const auto& edge : this->_list.at(vertex_id)) {
            const auto incident_vertex = edge->incident_vertex(vertex);
            if (*incident_vertex == *vertex)
                continue; // loop

            auto& adjacent_edges = this->_list.at(incident_vertex->id());
            const auto rem_subrange =
                std::ranges::remove_if(adjacent_edges, [&vertex](const auto& edge) {
                    return edge->is_incident_with(vertex);
                });
            adjacent_edges.erase(rem_subrange.begin(), rem_subrange.end());
        }

        this->_no_unique_edges -= this->_list.at(vertex_id).size();
        this->_list.erase(std::next(std::begin(this->_list), vertex_id));
    }

    void add_edge(edge_ptr_type edge)
    requires(type_traits::is_directed_v<edge_type>)
    {
        this->_list.at(edge->first()->id()).push_back(std::move(edge));
        this->_no_unique_edges++;
    }

    void remove_edge(const edge_ptr_type& edge)
    requires(type_traits::is_directed_v<edge_type>)
    {
        auto& adj_edges = this->_list.at(edge->first()->id());
        adj_edges.erase(std::ranges::find(adj_edges, edge.get(), address_projection{}));
        this->_no_unique_edges--;
    }

    void add_edge(edge_ptr_type edge)
    requires(type_traits::is_undirected_v<edge_type>)
    {
        this->_list.at(edge->first()->id()).push_back(edge);
        this->_list.at(edge->second()->id()).push_back(edge);
        this->_no_unique_edges++;
    }

    void remove_edge(const edge_ptr_type& edge)
    requires(type_traits::is_undirected_v<edge_type>)
    {
        const auto edge_addr = edge.get();
        auto& adj_edges_first = this->_list.at(edge->first()->id());
        auto& adj_edges_second = this->_list.at(edge->second()->id());

        adj_edges_first.erase(std::ranges::find(adj_edges_first, edge_addr, address_projection{}));
        adj_edges_second.erase(std::ranges::find(adj_edges_second, edge_addr, address_projection{})
        );
        this->_no_unique_edges--;
    }

    [[nodiscard]] inline types::iterator_range<edge_iterator_type> adjacent_edges(
        const types::id_type vertex_id
    ) {
        return make_iterator_range(this->_list.at(vertex_id));
    }

private:
    struct address_projection {
        auto operator()(const edge_ptr_type& edge) const {
            return edge.get();
        }
    };

    type _list = {};
    types::size_type _no_unique_edges = 0ull;
};

} // namespace gl::impl
