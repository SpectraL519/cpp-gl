// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/decl/impl_tags.hpp"

#include <algorithm>
#include <format>
#include <unordered_set>
#include <vector>

namespace gl::impl {

template <type_traits::c_list_graph_traits GraphTraits>
class adjacency_list;

namespace specialized {

namespace detail {

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList, typename AddressProjection>
requires std::is_invocable_r_v<
    const typename AdjacencyList::edge_type*,
    AddressProjection,
    const typename AdjacencyList::edge_ptr_type&>
[[nodiscard]] typename AdjacencyList::edge_iterator_type::iterator_type strict_find(
    typename AdjacencyList::edge_list_type& edge_set, const typename AdjacencyList::edge_type* edge
) {
    // find the edge by address
    const auto it = std::ranges::find(edge_set, edge, AddressProjection{});
    if (it == edge_set.end())
        throw std::invalid_argument(std::format(
            "Got invalid edge [vertices = ({}, {}) | addr = {}]",
            edge->first_id(),
            edge->second_id(),
            io::format(edge)
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
    using edge_list_type = typename impl_type::edge_list_type;
    using edge_iterator_type = typename impl_type::edge_iterator_type::iterator_type;

    struct address_projection {
        [[nodiscard]] gl_attr_force_inline auto operator()(const edge_type& edge) {
            return &edge;
        }

        [[nodiscard]] gl_attr_force_inline auto operator()(const edge_ptr_type& edge) {
            return edge.get();
        }
    };

    [[nodiscard]] static types::size_type in_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        types::size_type in_deg = constants::default_size;
        for (types::id_type id = constants::initial_id; id < self._list.size(); ++id) {
            const auto& adj_edges = self._list[id];
            if (adj_edges.empty())
                continue;

            in_deg += std::ranges::count(adj_edges, vertex_id, [](const auto& edge) {
                return edge->second_id();
            });
        }

        return in_deg;
    }

    [[nodiscard]] gl_attr_force_inline static types::size_type out_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return self._list[vertex_id].size();
    }

    [[nodiscard]] gl_attr_force_inline static types::size_type degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return in_degree(self, vertex_id) + out_degree(self, vertex_id);
    }

    [[nodiscard]] static std::vector<types::size_type> in_degree_map(const impl_type& self) {
        std::vector<types::id_type> in_degree_map(self._list.size(), constants::zero);

        for (types::id_type id = constants::initial_id; id < self._list.size(); ++id) {
            std::ranges::for_each(self._list[id], [&in_degree_map](const auto& edge) {
                ++in_degree_map[edge->second_id()];
            });
        }

        return in_degree_map;
    }

    [[nodiscard]] gl_attr_force_inline static std::vector<types::size_type> out_degree_map(
        const impl_type& self
    ) {
        const auto out_degree_view =
            self._list
            | std::views::transform([](const auto& adj_edges) { return adj_edges.size(); });
        return std::vector<types::size_type>(out_degree_view.begin(), out_degree_view.end());
    }

    [[nodiscard]] static std::vector<types::size_type> degree_map(const impl_type& self) {
        std::vector<types::id_type> degree_map(self._list.size(), constants::zero);

        for (types::id_type id = constants::initial_id; id < self._list.size(); ++id) {
            degree_map[id] += self._list[id].size();

            // update in degrees
            std::ranges::for_each(self._list[id], [&degree_map](const auto& edge) {
                ++degree_map[edge->second_id()];
            });
        }

        return degree_map;
    }

    static void remove_vertex(impl_type& self, const vertex_type& vertex) {
        const auto vertex_id = vertex.id();

        // remove all edges incident to the vertex
        for (types::id_type id = constants::initial_id; id < self._list.size(); ++id) {
            auto& adj_edges = self._list[id];
            if (id == vertex_id or adj_edges.empty())
                continue;

            const auto rem_subrange = std::ranges::remove_if(
                adj_edges, [&vertex](const auto& edge) { return edge->is_incident_with(vertex); }
            );
            self._n_unique_edges -= std::ranges::distance(rem_subrange.begin(), rem_subrange.end());
            adj_edges.erase(rem_subrange.begin(), rem_subrange.end());
        }

        // remove the list of edges incident from the vertex entirely
        self._n_unique_edges -= self._list[vertex_id].size();
        self._list.erase(std::next(std::begin(self._list), vertex_id));
    }

    static const edge_type& add_edge(impl_type& self, edge_ptr_type edge) {
        auto& adjacent_edges_first = self._list[edge->first_id()];
        adjacent_edges_first.push_back(std::move(edge));
        ++self._n_unique_edges;
        return *adjacent_edges_first.back();
    }

    static void add_edges_from(
        impl_type& self, const types::id_type source_id, std::vector<edge_ptr_type> new_edges
    ) {
        auto& adjacent_edges_source = self._list[source_id];
        adjacent_edges_source.reserve(adjacent_edges_source.size() + new_edges.size());

        for (auto& edge : new_edges)
            adjacent_edges_source.push_back(std::move(edge));

        self._n_unique_edges += new_edges.size();
    }

    [[nodiscard]] gl_attr_force_inline static bool is_edge_incident_to(
        const edge_ptr_type& edge,
        const types::id_type vertex_id,
        [[maybe_unused]] const types::id_type source_id
    ) {
        /*
        there is no need to check source_id because the implementation will not allow
        adding an edge for which first_id() is not the index in the adjacency
        list at which the edge is located, but the parameter is necessary to match the
        function signature for undirected adjacency list
        */
        return edge->second_id() == vertex_id;
    }

    static void remove_edge(impl_type& self, const edge_type& edge) {
        auto& adj_edges = self._list.at(edge.first_id());
        adj_edges.erase(detail::strict_find<impl_type, address_projection>(adj_edges, &edge));
        --self._n_unique_edges;
    }
};

template <type_traits::c_instantiation_of<adjacency_list> AdjacencyList>
requires(type_traits::is_undirected_v<typename AdjacencyList::edge_type>)
struct undirected_adjacency_list {
    using impl_type = AdjacencyList;
    using vertex_type = typename impl_type::vertex_type;
    using edge_type = typename impl_type::edge_type;
    using edge_ptr_type = typename impl_type::edge_ptr_type;
    using edge_list_type = typename impl_type::edge_list_type;
    using edge_iterator_type = typename impl_type::edge_iterator_type::iterator_type;

    struct address_projection {
        [[nodiscard]] gl_attr_force_inline auto operator()(const edge_type& edge) {
            return &edge;
        }

        [[nodiscard]] gl_attr_force_inline auto operator()(const edge_ptr_type& edge) {
            return edge.get();
        }
    };

    [[nodiscard]] gl_attr_force_inline static types::size_type in_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return degree(self, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline static types::size_type out_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return degree(self, vertex_id);
    }

    [[nodiscard]] static types::size_type degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        types::size_type degree = constants::default_size;
        for (const auto& edge : self._list[vertex_id])
            degree += constants::one + static_cast<types::size_type>(edge->is_loop());
        return degree;
    }

    [[nodiscard]] gl_attr_force_inline static std::vector<types::size_type> in_degree_map(
        const impl_type& self
    ) {
        return degree_map(self);
    }

    [[nodiscard]] gl_attr_force_inline static std::vector<types::size_type> out_degree_map(
        const impl_type& self
    ) {
        return degree_map(self);
    }

    [[nodiscard]] static std::vector<types::size_type> degree_map(const impl_type& self) {
        std::vector<types::size_type> degree_map;
        degree_map.reserve(self._list.size());

        for (types::id_type id = constants::initial_id; id < self._list.size(); ++id)
            degree_map.push_back(degree(self, id));

        return degree_map;
    }

    static void remove_vertex(impl_type& self, const vertex_type& vertex) {
        const auto vertex_id = vertex.id();
        std::unordered_set<types::id_type> incident_vertex_id_set;

        // select the vertices incident with the removed vertex
        for (const auto& edge : self._list[vertex_id]) {
            if (edge->is_loop())
                continue; // will be removed with the vertex's list
            incident_vertex_id_set.insert(edge->incident_vertex(vertex).id());
        }

        // remove all edges incident with the vertex (scan only the selected vertices)
        for (const auto& incident_vertex_id : incident_vertex_id_set) {
            auto& adj_edges = self._list[incident_vertex_id];
            const auto rem_subrange = std::ranges::remove_if(
                adj_edges, [&vertex](const auto& edge) { return edge->is_incident_with(vertex); }
            );
            adj_edges.erase(rem_subrange.begin(), rem_subrange.end());
        }

        // remove the list of edges incident from the vertex entirely
        self._n_unique_edges -= self._list[vertex_id].size();
        self._list.erase(std::next(std::begin(self._list), vertex_id));
    }

    static const edge_type& add_edge(impl_type& self, edge_ptr_type edge) {
        auto& adjacent_edges_first = self._list[edge->first_id()];

        if (not edge->is_loop())
            self._list[edge->second_id()].push_back(edge);
        adjacent_edges_first.push_back(std::move(edge));

        ++self._n_unique_edges;
        return *adjacent_edges_first.back();
    }

    static void add_edges_from(
        impl_type& self, const types::id_type source_id, std::vector<edge_ptr_type> new_edges
    ) {
        auto& adjacent_edges_source = self._list[source_id];
        adjacent_edges_source.reserve(adjacent_edges_source.size() + new_edges.size());

        for (auto& edge : new_edges) {
            if (not edge->is_loop())
                self._list[edge->second_id()].push_back(edge);
            adjacent_edges_source.push_back(std::move(edge));
        }

        self._n_unique_edges += new_edges.size();
    }

    [[nodiscard]] inline static bool is_edge_incident_to(
        const edge_ptr_type& edge, const types::id_type vertex_id, const types::id_type source_id
    ) {
        if (edge->first_id() == source_id)
            return edge->second_id() == vertex_id;
        else if (edge->second_id() == source_id)
            return edge->first_id() == vertex_id;
        return false;
    }

    static void remove_edge(impl_type& self, const edge_type& edge) {
        if (edge.is_loop()) {
            auto& adj_edges_first = self._list.at(edge.first_id());
            adj_edges_first.erase(
                detail::strict_find<impl_type, address_projection>(adj_edges_first, &edge)
            );
        }
        else {
            auto& adj_edges_first = self._list.at(edge.first_id());
            auto& adj_edges_second = self._list.at(edge.second_id());

            adj_edges_first.erase(
                detail::strict_find<impl_type, address_projection>(adj_edges_first, &edge)
            );
            // if the edge was found in the first list, it will also be present in the second list
            adj_edges_second.erase(std::ranges::find(adj_edges_second, &edge, address_projection{})
            );
        }
        --self._n_unique_edges;
    }
};

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
