#pragma once

#include "gl/impl/impl_tags_decl.hpp"

#include <algorithm>

namespace gl::impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_list;

namespace specialized {

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_directed_v<typename AdjacencyList::edge_type>)
struct directed_adjacency_list {
    using impl_type = AdjacencyList;
    using vertex_ptr_type = typename impl_type::vertex_ptr_type;
    using edge_ptr_type = typename impl_type::edge_ptr_type;

    static void remove_vertex(impl_type& self, const vertex_ptr_type& vertex) {
        for (auto& adj_edges : self._list) {
            const auto rem_subrange = std::ranges::remove_if(
                adj_edges, [&vertex](const auto& edge) { return edge->is_incident_with(vertex); }
            );
            self._n_unique_edges -= std::ranges::distance(rem_subrange.begin(), rem_subrange.end());
            adj_edges.erase(rem_subrange.begin(), rem_subrange.end());
        }
        self._list.erase(std::next(std::begin(self._list), vertex->id()));
    }

    static inline const edge_ptr_type& add_edge(impl_type& self, edge_ptr_type edge) {
        self._n_unique_edges++;
        return self._list.at(edge->first()->id()).emplace_back(std::move(edge));
    }

    [[nodiscard]] static inline bool has_edge(
        const impl_type& self, const types::id_type first_id, const types::id_type second_id
    ) {
        // no need to check first - exception will be thrown in the at() function
        if (second_id >= self._list.size())
            throw std::out_of_range(std::format("Got invalid vertex id [{}]", second_id));

        const auto& adjacent_edges = self._list.at(first_id);
        return std::ranges::find(
                   adjacent_edges, second_id, [](const auto& edge) { return edge->second()->id(); }
               )
            != adjacent_edges.cend();
    }

    static void remove_edge(impl_type& self, const edge_ptr_type& edge) {
        using addr_proj = typename impl_type::address_projection;

        auto& adj_edges = self._list.at(edge->first()->id());
        adj_edges.erase(std::ranges::find(adj_edges, edge.get(), addr_proj{}));
        self._n_unique_edges--;
    }
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_undirected_v<typename AdjacencyList::edge_type>)
struct undirected_adjacency_list {
    using impl_type = AdjacencyList;
    using vertex_ptr_type = typename impl_type::vertex_ptr_type;
    using edge_ptr_type = typename impl_type::edge_ptr_type;

    static void remove_vertex(impl_type& self, const vertex_ptr_type& vertex) {
        // TODO: optimize for multiedges
        // * the edges adjacent to incident_vertex should be processed once

        const auto vertex_id = vertex->id();

        for (const auto& edge : self._list.at(vertex_id)) {
            const auto& incident_vertex = edge->incident_vertex(vertex);
            if (incident_vertex == vertex)
                continue; // loop: will be removed with the vertex's list

            auto& adj_edges = self._list.at(incident_vertex->id());
            const auto rem_subrange = std::ranges::remove_if(
                adj_edges, [&vertex](const auto& edge) { return edge->is_incident_with(vertex); }
            );
            adj_edges.erase(rem_subrange.begin(), rem_subrange.end());
        }

        self._n_unique_edges -= self._list.at(vertex_id).size();
        self._list.erase(std::next(std::begin(self._list), vertex_id));
    }

    static const edge_ptr_type& add_edge(impl_type& self, edge_ptr_type edge) {
        self._n_unique_edges++;
        if (not edge->is_loop())
            self._list.at(edge->second()->id()).push_back(edge);
        return self._list.at(edge->first()->id()).emplace_back(edge);
    }

    [[nodiscard]] static inline bool has_edge(
        const impl_type& self, const types::id_type first_id, const types::id_type second_id
    ) {
        // no need to check first - exception will be thrown in the at() function
        if (second_id >= self._list.size())
            throw std::out_of_range(std::format("Got invalid vertex id [{}]", second_id));

        const auto& adjacent_edges = self._list.at(first_id);
        return std::ranges::find_if(
                   adjacent_edges,
                   [second_id](const auto& edge) {
                       return edge->second()->id() == second_id or edge->first()->id() == second_id;
                   }
               )
            != adjacent_edges.cend();
    }

    static void remove_edge(impl_type& self, const edge_ptr_type& edge) {
        using addr_proj = typename impl_type::address_projection;

        const auto edge_addr = edge.get();
        auto& adj_edges_first = self._list.at(edge->first()->id());
        auto& adj_edges_second = self._list.at(edge->second()->id());

        adj_edges_first.erase(std::ranges::find(adj_edges_first, edge_addr, addr_proj{}));
        if (not edge->is_loop())
            adj_edges_second.erase(std::ranges::find(adj_edges_second, edge_addr, addr_proj{}));
        self._n_unique_edges--;
    }
};

// common utility

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
struct list_impl_traits {
    using type = void;
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_directed_v<typename AdjacencyList::edge_type>)
struct list_impl_traits<AdjacencyList> {
    using type = directed_adjacency_list<AdjacencyList>;
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_undirected_v<typename AdjacencyList::edge_type>)
struct list_impl_traits<AdjacencyList> {
    using type = undirected_adjacency_list<AdjacencyList>;
};

} // namespace specialized

} // namespace gl::impl
