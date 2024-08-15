#pragma once

#include "gl/graph_traits.hpp"
#include "gl/types/type_traits.hpp"

#include <algorithm>

namespace gl::impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_list;

namespace specialized {

// directed adjacency_list

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_directed_v<typename AdjacencyList::edge_type>)
struct directed_adjacency_list {
    using impl_type = AdjacencyList;

    static void add_edge(impl_type& self, typename impl_type::edge_ptr_type edge) {
        self._list.at(edge->first()->id()).push_back(std::move(edge));
        self._no_unique_edges++;
    }

    static void remove_vertex(impl_type& self, const typename impl_type::vertex_ptr_type& vertex) {
        for (auto& adj_edges : self._list) {
            const auto rem_subrange = std::ranges::remove_if(
                adj_edges, [&vertex](const auto& edge) { return edge->is_incident_with(vertex); }
            );
            self._no_unique_edges -=
                std::ranges::distance(rem_subrange.begin(), rem_subrange.end());
            adj_edges.erase(rem_subrange.begin(), rem_subrange.end());
        }
        self._list.erase(std::next(std::begin(self._list), vertex->id()));
    }

    template <typename IdProjection>
    static void remove_edge(impl_type& self, const typename impl_type::edge_ptr_type& edge) {
        auto& adj_edges = self._list.at(edge->first()->id());
        adj_edges.erase(std::ranges::find(adj_edges, edge.get(), IdProjection{}));
        self._no_unique_edges--;
    }
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_undirected_v<typename AdjacencyList::edge_type>)
struct undirected_adjacency_list {
    using impl_type = AdjacencyList;

    static void add_edge(impl_type& self, typename impl_type::edge_ptr_type edge) {
        self._list.at(edge->first()->id()).push_back(edge);
        if (not edge->is_loop())
            self._list.at(edge->second()->id()).push_back(edge);
        self._no_unique_edges++;
    }

    static void remove_vertex(impl_type& self, const typename impl_type::vertex_ptr_type& vertex) {
        // TODO: optimize for multiedges
        // * the edges adjacent to incident_vertex should be processed once

        const auto vertex_id = vertex->id();

        for (const auto& edge : self._list.at(vertex_id)) {
            const auto incident_vertex = edge->incident_vertex(vertex);
            if (*incident_vertex == *vertex)
                continue; // loop: will be removed with the vertex's list

            auto& adj_edges = self._list.at(incident_vertex->id());
            const auto rem_subrange = std::ranges::remove_if(
                adj_edges, [&vertex](const auto& edge) { return edge->is_incident_with(vertex); }
            );
            adj_edges.erase(rem_subrange.begin(), rem_subrange.end());
        }

        self._no_unique_edges -= self._list.at(vertex_id).size();
        self._list.erase(std::next(std::begin(self._list), vertex_id));
    }

    template <typename IdProjection>
    static void remove_edge(impl_type& self, const typename impl_type::edge_ptr_type& edge) {
        const auto edge_addr = edge.get();
        auto& adj_edges_first = self._list.at(edge->first()->id());
        auto& adj_edges_second = self._list.at(edge->second()->id());

        adj_edges_first.erase(std::ranges::find(adj_edges_first, edge_addr, IdProjection{}));
        adj_edges_second.erase(std::ranges::find(adj_edges_second, edge_addr, IdProjection{}));
        self._no_unique_edges--;
    }
};

// common utility

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
struct list_impl_traits {
    using specialized_type = void;
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_directed_v<typename AdjacencyList::edge_type>)
struct list_impl_traits<AdjacencyList> {
    using specialized_type = directed_adjacency_list<AdjacencyList>;
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_undirected_v<typename AdjacencyList::edge_type>)
struct list_impl_traits<AdjacencyList> {
    using specialized_type = undirected_adjacency_list<AdjacencyList>;
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
using impl_type = typename list_impl_traits<AdjacencyList>::specialized_type;

} // namespace specialized

} // namespace gl::impl
