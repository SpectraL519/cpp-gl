#pragma once

#include "gl/impl/impl_tags_decl.hpp"

#include <algorithm>
#include <format>

namespace gl::impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_list;

namespace specialized {

namespace detail {

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList, typename AddressProjection>
[[nodiscard]] typename AdjacencyList::edge_iterator_type::iterator_type strict_find(
    typename AdjacencyList::edge_set_type& edge_set, const typename AdjacencyList::edge_type* edge
) {
    const auto it = std::ranges::find(edge_set, edge, AddressProjection{});
    if (it == edge_set.end())
        throw std::invalid_argument(std::format(
            "Got invalid edge [vertices = ({}, {}) | addr = {}]",
            edge->first().id(),
            edge->second().id(),
            types::formatter(edge)
        ));

    return it;
}

} // namespace detail

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_directed_v<typename AdjacencyList::edge_type>)
struct directed_adjacency_list {
    using impl_type = AdjacencyList;
    using vertex_type = typename impl_type::vertex_type;
    using edge_type = typename impl_type::edge_type;
    using edge_ptr_type = typename impl_type::edge_ptr_type;

    struct address_projection {
        auto operator()(const edge_type& edge) {
            return &edge;
        }

        auto operator()(const edge_ptr_type& edge) {
            return edge.get();
        }
    };

    static void remove_vertex(impl_type& self, const vertex_type& vertex) {
        for (auto& adj_edges : self._list) {
            const auto rem_subrange = std::ranges::remove_if(
                adj_edges, [&vertex](const auto& edge) { return edge->is_incident_with(vertex); }
            );
            self._n_unique_edges -= std::ranges::distance(rem_subrange.begin(), rem_subrange.end());
            adj_edges.erase(rem_subrange.begin(), rem_subrange.end());
        }
        self._list.erase(std::next(std::begin(self._list), vertex.id()));
    }

    static const edge_type& add_edge(impl_type& self, edge_ptr_type edge) {
        auto& adjacent_edges_first = self._list.at(edge->first().id());
        adjacent_edges_first.push_back(std::move(edge));
        self._n_unique_edges++;
        return *adjacent_edges_first.back();
    }

    [[nodiscard]] static inline bool has_edge(
        const impl_type& self, const types::id_type first_id, const types::id_type second_id
    ) {
        const auto& adjacent_edges = self._list[first_id];
        return std::ranges::find(
                   adjacent_edges, second_id, [](const auto& edge) { return edge->second().id(); }
               )
            != adjacent_edges.cend();
    }

    static void remove_edge(impl_type& self, const edge_type& edge) {
        auto& adj_edges = self._list.at(edge.first().id());
        adj_edges.erase(detail::strict_find<impl_type, address_projection>(adj_edges, &edge));
        self._n_unique_edges--;
    }
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_undirected_v<typename AdjacencyList::edge_type>)
struct undirected_adjacency_list {
    using impl_type = AdjacencyList;
    using vertex_type = typename impl_type::vertex_type;
    using edge_type = typename impl_type::edge_type;
    using edge_ptr_type = typename impl_type::edge_ptr_type;

    struct address_projection {
        auto operator()(const edge_type& edge) {
            return &edge;
        }

        auto operator()(const edge_ptr_type& edge) {
            return edge.get();
        }
    };

    static void remove_vertex(impl_type& self, const vertex_type& vertex) {
        // TODO: optimize for multiedges
        // * the edges adjacent to incident_vertex should be processed once

        const auto vertex_id = vertex.id();

        for (const auto& edge : self._list.at(vertex_id)) {
            const auto& incident_vertex = edge->incident_vertex(vertex);
            if (incident_vertex == vertex)
                continue; // loop: will be removed with the vertex's list

            auto& adj_edges = self._list.at(incident_vertex.id());
            const auto rem_subrange = std::ranges::remove_if(
                adj_edges, [&vertex](const auto& edge) { return edge->is_incident_with(vertex); }
            );
            adj_edges.erase(rem_subrange.begin(), rem_subrange.end());
        }

        self._n_unique_edges -= self._list.at(vertex_id).size();
        self._list.erase(std::next(std::begin(self._list), vertex_id));
    }

    static const edge_type& add_edge(impl_type& self, edge_ptr_type edge) {
        auto& adjacent_edges_first = self._list.at(edge->first().id());

        if (not edge->is_loop())
            self._list.at(edge->second().id()).push_back(edge);
        adjacent_edges_first.push_back(std::move(edge));

        self._n_unique_edges++;
        return *adjacent_edges_first.back();
    }

    [[nodiscard]] static inline bool has_edge(
        const impl_type& self, const types::id_type first_id, const types::id_type second_id
    ) {
        const auto& adjacent_edges = self._list[first_id];
        return std::ranges::find_if(
                   adjacent_edges,
                   [second_id](const auto& edge) {
                       return edge->second().id() == second_id or edge->first().id() == second_id;
                   }
               )
            != adjacent_edges.cend();
    }

    static void remove_edge(impl_type& self, const edge_type& edge) {
        if (edge.is_loop()) {
            auto& adj_edges_first = self._list.at(edge.first().id());
            adj_edges_first.erase(
                detail::strict_find<impl_type, address_projection>(adj_edges_first, &edge)
            );
        }
        else {
            auto& adj_edges_first = self._list.at(edge.first().id());
            auto& adj_edges_second = self._list.at(edge.second().id());

            adj_edges_first.erase(
                detail::strict_find<impl_type, address_projection>(adj_edges_first, &edge)
            );
            // if the edge was found in the first list, it will be in the second list
            adj_edges_second.erase(std::ranges::find(adj_edges_second, &edge, address_projection{})
            );
        }
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
