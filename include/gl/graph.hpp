// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "constants.hpp"
#include "graph_traits.hpp"
#include "impl/impl_tags.hpp"
#include "io/stream_options_manipulator.hpp"
#include "types/iterator_range.hpp"

#include <set>

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

    using vetex_list_type = std::vector<vertex_ptr_type>;
    using vertex_iterator_type =
        types::dereferencing_iterator<typename vetex_list_type::const_iterator>;

    // TODO: reverese iterators should be available for bidirectional ranges

    using edge_type = typename traits_type::edge_type;
    using edge_ptr_type = typename traits_type::edge_ptr_type;
    using edge_directional_tag = typename traits_type::edge_directional_tag;
    using edge_properties_type = typename traits_type::edge_properties_type;

    using edge_list_type = typename implementation_type::edge_list_type;
    using edge_iterator_type = typename implementation_type::edge_iterator_type;

    graph(const graph&) = delete;
    graph& operator=(const graph&) = delete;

    graph() = default;

    graph(const types::size_type n_vertices) : _impl(n_vertices) {
        this->_vertices.reserve(n_vertices);
        for (auto vertex_id = constants::initial_id; vertex_id < n_vertices; vertex_id++)
            this->_vertices.push_back(detail::make_vertex<vertex_type>(vertex_id));
    }

    graph(graph&&) = default;
    graph& operator=(graph&&) = default;

    ~graph() = default;

    // --- general methods ---

    [[nodiscard]] gl_attr_force_inline types::size_type n_vertices() const {
        return this->_vertices.size();
    }

    [[nodiscard]] gl_attr_force_inline types::size_type n_unique_edges() const {
        return this->_impl.n_unique_edges();
    }

    // --- vertex methods ---

    [[nodiscard]] gl_attr_force_inline types::iterator_range<vertex_iterator_type> vertices(
    ) const {
        return make_iterator_range(deref_cbegin(this->_vertices), deref_cend(this->_vertices));
    }

    [[nodiscard]] gl_attr_force_inline std::ranges::iota_view<types::id_type, types::id_type>
    vertex_ids() const {
        return std::views::iota(constants::initial_id, this->n_vertices());
    }

    // clang-format off
    // gl_attr_force_inline misplacement

    [[nodiscard]] gl_attr_force_inline const vertex_type& get_vertex(
        const types::id_type vertex_id
    ) const {
        this->_verify_vertex_id(vertex_id);
        return *this->_vertices[vertex_id];
    }

    // clang-format on

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const types::id_type vertex_id) const {
        return vertex_id < this->n_vertices();
    }

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const vertex_type& vertex) const {
        return this->has_vertex(vertex.id()) and &vertex == this->_vertices[vertex.id()].get();
    }

    const vertex_type& add_vertex() {
        this->_impl.add_vertex();
        this->_vertices.push_back(detail::make_vertex<vertex_type>(this->n_vertices()));
        return *this->_vertices.back();
    }

    const vertex_type& add_vertex(const vertex_properties_type& properties)
    requires(not type_traits::is_default_properties_type_v<vertex_properties_type>)
    {
        this->_impl.add_vertex();
        this->_vertices.push_back(detail::make_vertex<vertex_type>(this->n_vertices(), properties));
        return *this->_vertices.back();
    }

    void add_vertices(const types::size_type n) {
        this->_impl.add_vertices(n);
        this->_vertices.reserve(this->n_vertices() + n);

        for (types::size_type _ = constants::begin_idx; _ < n; _++)
            this->_vertices.push_back(detail::make_vertex<vertex_type>(this->n_vertices()));
    }

    template <type_traits::c_sized_range_of<vertex_properties_type> VertexPropertiesRange>
    void add_vertices_with(const VertexPropertiesRange& properties_range) {
        const auto n = std::ranges::size(properties_range);

        this->_impl.add_vertices(n);
        this->_vertices.reserve(this->n_vertices() + n);

        for (const auto& properties : properties_range)
            this->_vertices.push_back(
                detail::make_vertex<vertex_type>(this->n_vertices(), properties)
            );
    }

    gl_attr_force_inline void remove_vertex(const types::size_type vertex_id) {
        this->_remove_vertex_impl(this->get_vertex(vertex_id));
    }

    inline void remove_vertex(const vertex_type& vertex) {
        this->_verify_vertex(vertex);
        this->_remove_vertex_impl(vertex);
    }

    template <type_traits::c_sized_range_of<types::id_type> IdRange>
    void remove_vertices_from(const IdRange& vertex_id_range) {
        // sorts the ids in a descending order and removes duplicate ids
        std::set<types::id_type, std::greater<types::id_type>> vertex_id_set(
            std::ranges::begin(vertex_id_range), std::ranges::end(vertex_id_range)
        );

        // TODO: optimize
        for (const auto vertex_id : vertex_id_set)
            this->_remove_vertex_impl(this->get_vertex(vertex_id));
    }

    template <type_traits::c_sized_range_of<types::const_ref_wrap<vertex_type>> VertexRefRange>
    void remove_vertices_from(const VertexRefRange& vertex_ref_range) {
        // TODO [C++26]: replace with std::greater
        struct vertex_ref_greater_comparator {
            [[nodiscard]] bool operator()(
                const types::const_ref_wrap<vertex_type>& lhs,
                const types::const_ref_wrap<vertex_type>& rhs
            ) const {
                return lhs.get() > rhs.get();
            }
        };

        // sorts the ids in a descending order and removes duplicate ids
        std::set<types::const_ref_wrap<vertex_type>, vertex_ref_greater_comparator> vertex_ref_set(
            std::ranges::begin(vertex_ref_range), std::ranges::end(vertex_ref_range)
        );

        // TODO: optimize
        for (const auto& vertex_ref : vertex_ref_set)
            this->_remove_vertex_impl(vertex_ref.get());
    }

    [[nodiscard]] gl_attr_force_inline types::size_type in_degree(const vertex_type& vertex) const {
        this->_verify_vertex(vertex);
        return this->_impl.in_degree(vertex);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type in_degree(const types::id_type vertex_id
    ) const {
        return this->_impl.in_degree(this->get_vertex(vertex_id));
    }

    [[nodiscard]] gl_attr_force_inline types::size_type out_degree(const vertex_type& vertex
    ) const {
        this->_verify_vertex(vertex);
        return this->_impl.out_degree(vertex);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type out_degree(const types::id_type vertex_id
    ) const {
        return this->_impl.out_degree(this->get_vertex(vertex_id));
    }

    [[nodiscard]] gl_attr_force_inline types::size_type degree(const vertex_type& vertex) const {
        if constexpr (type_traits::is_directed_v<edge_type>)
            return this->_impl.in_degree(vertex) + this->_impl.out_degree(vertex);
        else
            return this->out_degree(vertex);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type degree(const types::id_type vertex_id
    ) const {
        if constexpr (type_traits::is_directed_v<edge_type>) {
            const auto& vertex = this->get_vertex(vertex_id);
            return this->_impl.in_degree(vertex) + this->_impl.out_degree(vertex);
        }
        else
            return this->_impl.out_degree(this->get_vertex(vertex_id));
    }

    // --- edge methods ---

    // clang-format off
    // gl_attr_force_inline misplacement

    const edge_type& add_edge(
        const types::id_type first_id, const types::id_type second_id
    ) {
        this->_verify_vertex_id(first_id);
        this->_verify_vertex_id(second_id);
        return this->_impl.add_edge(
            detail::make_edge<edge_type>(this->get_vertex(first_id), this->get_vertex(second_id))
        );
    }

    const edge_type& add_edge(
        const types::id_type first_id,
        const types::id_type second_id,
        const edge_properties_type& properties
    )
    requires(not type_traits::is_default_properties_type_v<edge_properties_type>)
    {
        this->_verify_vertex_id(first_id);
        this->_verify_vertex_id(second_id);
        return this->_impl.add_edge(detail::make_edge<edge_type>(
            this->get_vertex(first_id), this->get_vertex(second_id), properties
        ));
    }

    // clang-format on

    const edge_type& add_edge(const vertex_type& first, const vertex_type& second) {
        this->_verify_vertex(first);
        this->_verify_vertex(second);
        return this->_impl.add_edge(detail::make_edge<edge_type>(first, second));
    }

    const edge_type& add_edge(
        const vertex_type& first, const vertex_type& second, const edge_properties_type& properties
    )
    requires(not type_traits::is_default_properties_type_v<edge_properties_type>)
    {
        this->_verify_vertex(first);
        this->_verify_vertex(second);
        return this->_impl.add_edge(detail::make_edge<edge_type>(first, second, properties));
    }

    template <type_traits::c_sized_range_of<types::id_type> IdRange>
    void add_edges_from(const types::id_type source_id, const IdRange& destination_id_range) {
        const auto& source = this->get_vertex(source_id);

        std::vector<edge_ptr_type> new_edges;
        new_edges.reserve(std::ranges::size(destination_id_range));

        for (const auto destination_id : destination_id_range) {
            new_edges.push_back(
                detail::make_edge<edge_type>(source, this->get_vertex(destination_id))
            );
        }
        this->_impl.add_edges_from(source_id, std::move(new_edges));
    }

    template <type_traits::c_sized_range_of<types::const_ref_wrap<vertex_type>> VertexRefRange>
    void add_edges_from(const vertex_type& source, const VertexRefRange& destination_range) {
        this->_verify_vertex(source);

        std::vector<edge_ptr_type> new_edges;
        new_edges.reserve(std::ranges::size(destination_range));

        for (const auto& destination_ref : destination_range) {
            const auto& destination = destination_ref.get();
            this->_verify_vertex(destination);
            new_edges.push_back(detail::make_edge<edge_type>(source, destination));
        }
        this->_impl.add_edges_from(source.id(), std::move(new_edges));
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(
        const types::id_type first_id, const types::id_type second_id
    ) const {
        return this->_impl.has_edge(first_id, second_id);
    }

    [[nodiscard]] bool has_edge(const vertex_type& first, const vertex_type& second) const {
        this->_verify_vertex(first);
        this->_verify_vertex(second);
        return this->has_edge(first.id(), second.id());
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(const edge_type& edge) const {
        return this->_impl.has_edge(edge);
    }

    [[nodiscard]] gl_attr_force_inline types::optional_cref<edge_type> get_edge(
        const types::id_type first_id, const types::id_type second_id
    ) const {
        return this->_impl.get_edge(first_id, second_id);
    }

    [[nodiscard]] types::optional_cref<edge_type> get_edge(
        const vertex_type& first, const vertex_type& second
    ) const {
        if (not (this->has_vertex(first) and this->has_vertex(second)))
            return std::nullopt;

        // TODO: optimize this so that the vertex ids are not checked twice
        return this->_impl.get_edge(first.id(), second.id());
    }

    [[nodiscard]] inline std::vector<types::const_ref_wrap<edge_type>> get_edges(
        const types::id_type first_id, const types::id_type second_id
    ) const {
        using edge_ref_set = std::vector<types::const_ref_wrap<edge_type>>;

        if constexpr (std::same_as<implementation_tag, impl::list_t>) {
            return this->_impl.get_edges(first_id, second_id);
        }
        else {
            const auto edge_opt = this->_impl.get_edge(first_id, second_id);
            return edge_opt.has_value() ? edge_ref_set{edge_opt.value()} : edge_ref_set{};
        }
    }

    [[nodiscard]] std::vector<types::const_ref_wrap<edge_type>> get_edges(
        const vertex_type& first, const vertex_type& second
    ) const {
        this->_verify_vertex(first);
        this->_verify_vertex(second);
        return this->get_edges(first.id(), second.id());
    }

    gl_attr_force_inline void remove_edge(const edge_type& edge) {
        this->_impl.remove_edge(edge);
    }

    template <type_traits::c_range_of<types::const_ref_wrap<edge_type>> EdgeRefRange>
    inline void remove_edges_from(const EdgeRefRange edges) {
        for (const auto& edge_ref : edges)
            this->_impl.remove_edge(edge_ref.get());
    }

    [[nodiscard]] inline types::iterator_range<edge_iterator_type> adjacent_edges(
        const types::id_type vertex_id
    ) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.adjacent_edges(vertex_id);
    }

    [[nodiscard]] inline types::iterator_range<edge_iterator_type> adjacent_edges(
        const vertex_type& vertex
    ) const {
        this->_verify_vertex(vertex);
        return this->_impl.adjacent_edges(vertex.id());
    }

    // --- incidence methods ---

    [[nodiscard]] bool are_incident(const types::id_type first_id, const types::id_type second_id)
        const {
        this->_verify_vertex_id(first_id);

        if (first_id == second_id)
            return true;

        this->_verify_vertex_id(second_id);

        if constexpr (type_traits::is_directed_v<edge_type>)
            return this->has_edge(first_id, second_id) or this->has_edge(second_id, first_id);
        else
            return this->has_edge(first_id, second_id);
    }

    [[nodiscard]] bool are_incident(const vertex_type& first, const vertex_type& second) const {
        this->_verify_vertex(first);
        this->_verify_vertex(second);

        if (first == second)
            return true;

        if constexpr (type_traits::is_directed_v<edge_type>)
            return this->has_edge(first.id(), second.id())
                or this->has_edge(second.id(), first.id());
        else
            return this->has_edge(first.id(), second.id());
    }

    [[nodiscard]] bool are_incident(const vertex_type& vertex, const edge_type& edge) const {
        this->_verify_vertex(vertex);
        this->_verify_edge(edge);
        return edge.is_incident_with(vertex);
    }

    [[nodiscard]] bool are_incident(const edge_type& edge, const vertex_type& vertex) const {
        this->_verify_vertex(vertex);
        this->_verify_edge(edge);
        return edge.is_incident_with(vertex);
    }

    [[nodiscard]] bool are_incident(const edge_type& edge_1, const edge_type& edge_2) const {
        this->_verify_edge(edge_1);
        this->_verify_edge(edge_2);
        return edge_1.is_incident_with(edge_2.first()) or edge_1.is_incident_with(edge_2.second());
    }

    friend std::ostream& operator<<(std::ostream& os, const graph& g) {
        if (io::is_option_set(os, io::graph_option::gsf)) {
            g._gsf_write(os);
            return os;
        }

        if (io::is_option_set(os, io::graph_option::verbose))
            g._verbose_write(os);
        else
            g._concise_write(os);

        return os;
    }

    friend inline std::istream& operator>>(std::istream& is, graph& g) {
        g._gsf_read(is);
        return is;
    }

private:
    [[nodiscard]] static constexpr std::string _directed_type_str() {
        return type_traits::is_directed_v<edge_type> ? "directed" : "undirected";
    }

    // --- graph element verification methods ---

    gl_attr_force_inline void _verify_vertex_id(const types::id_type vertex_id) const {
        if (not this->has_vertex(vertex_id))
            throw std::out_of_range(std::format("Got invalid vertex id [{}]", vertex_id));
    }

    void _verify_vertex(const vertex_type& vertex) const {
        const auto vertex_id = vertex.id();
        const auto& self_vertex = this->get_vertex(vertex_id);

        if (&vertex != &self_vertex)
            throw std::invalid_argument(std::format(
                "Got invalid vertex [id = {} | expected addr = {} | actual addr = {}]",
                vertex_id,
                io::format(&self_vertex),
                io::format(&vertex)
            ));
    }

    void _verify_edge(const edge_type& edge) const {
        if (not this->has_edge(edge))
            throw std::invalid_argument(std::format(
                "Got invalid edge [vertices = ({}, {}) | addr = {}]",
                edge.first_id(),
                edge.second_id(),
                io::format(&edge)
            ));
    }

    // --- vertex methods ---

    void _remove_vertex_impl(const vertex_type& vertex) {
        const auto vertex_id = vertex.id();
        this->_impl.remove_vertex(vertex);
        this->_vertices.erase(std::next(std::begin(this->_vertices), vertex_id));

        // align ids of remainig vertices
        std::for_each(
            std::next(std::begin(this->_vertices), vertex_id),
            this->_vertices.end(),
            [](auto& v) { v->_id--; }
        );
    }

    // --- io methods ---

    void _verbose_write(std::ostream& os) const {
        os << std::format(
            "type: {}\nnumber of vertices: {}\nnumber of edges: {}\nvertices:\n",
            _directed_type_str(),
            this->n_vertices(),
            this->n_unique_edges()
        );

        constexpr bool within_context = true;
        for (const auto& vertex : this->vertices()) {
            os << "- " << vertex << "\n  adjacent edges:\n";
            for (const auto& edge : this->_impl.adjacent_edges(vertex.id())) {
                os << "\t- ";
                edge._write(os, within_context);
                os << '\n';
            }
        }
    }

    void _concise_write(std::ostream& os) const {
        os << std::format(
            "{} {} {}\n", _directed_type_str(), this->n_vertices(), this->n_unique_edges()
        );

        for (const auto& vertex : this->vertices()) {
            os << "- " << vertex << " :";
            for (const auto& edge : this->_impl.adjacent_edges(vertex.id()))
                os << ' ' << edge;
            os << '\n';
        }
    }

    void _gsf_write(std::ostream& os) const {
        const bool with_vertex_properties =
            io::is_option_set(os, io::graph_option::with_vertex_properties);
        const bool with_edge_properties =
            io::is_option_set(os, io::graph_option::with_edge_properties);

        // print graph size
        os << std::format(
            "{} {} {} {} {}\n",
            static_cast<int>(type_traits::is_directed_v<edge_type>),
            this->n_vertices(),
            this->n_unique_edges(),
            static_cast<int>(with_vertex_properties),
            static_cast<int>(with_edge_properties)
        );

        if constexpr (type_traits::c_writable<typename vertex_type::properties_type>)
            if (with_vertex_properties)
                for (const auto& vertex : this->vertices())
                    os << vertex.properties << '\n';

        if constexpr (type_traits::c_writable<typename edge_type::properties_type>) {
            if (with_edge_properties) {
                const auto print_incident_edges = [this, &os](const types::id_type vertex_id) {
                    for (const auto& edge : this->_impl.adjacent_edges(vertex_id)) {
                        if (edge.first_id() != vertex_id)
                            continue; // vertex is not the source
                        os << edge.first_id() << ' ' << edge.second_id() << ' ' << edge.properties
                           << '\n';
                    }
                };

                for (const auto vertex_id : this->vertex_ids())
                    print_incident_edges(vertex_id);

                return;
            }
        }

        const auto print_incident_edges = [this, &os](const types::id_type vertex_id) {
            for (const auto& edge : this->_impl.adjacent_edges(vertex_id)) {
                if (edge.first_id() != vertex_id)
                    continue; // vertex is not the source
                os << edge.first_id() << ' ' << edge.second_id() << '\n';
            }
        };

        for (const auto vertex_id : this->vertex_ids())
            print_incident_edges(vertex_id);
    }

    void _gsf_read(std::istream& is) {
        bool directed;
        is >> directed;

        if (directed != type_traits::is_directed_v<edge_type>)
            throw std::ios_base::failure(std::format(
                "Invalid graph specification: directional tag does not match - should be {}",
                _directed_type_str()
            ));

        // read initial graph parameters
        types::id_type n_vertices, n_edges;
        is >> n_vertices >> n_edges;

        bool with_vertex_properties, with_edge_properties;
        is >> with_vertex_properties >> with_edge_properties;

        if (with_vertex_properties) {
            if constexpr (not type_traits::c_readable<vertex_properties_type>) {
                throw std::ios_base::failure(
                    "Invalid graph specification: vertex_properties=true "
                    "when vertex_properties_type is not readable"
                );
            }
            else {
                // read vertex properties and use them to initialze the vertices
                std::vector<vertex_properties_type> vertex_properties(n_vertices);
                for (types::size_type i = constants::begin_idx; i < n_vertices; i++)
                    is >> vertex_properties[i];
                this->add_vertices_with(vertex_properties);
            }
        }
        else {
            // initialize the vertices with default (or none) properties
            this->add_vertices(n_vertices);
        }

        if (with_edge_properties) {
            if constexpr (not type_traits::c_readable<edge_properties_type>) {
                throw std::ios_base::failure(
                    "Invalid graph specification: edge_properties=true "
                    "when edge_properties_type is not readable"
                );
            }
            else {
                // read edges with their properties
                types::id_type first_id, second_id;
                edge_properties_type properties;

                for (types::size_type i = constants::begin_idx; i < n_edges; i++) {
                    is >> first_id >> second_id >> properties;
                    this->add_edge(first_id, second_id, properties);
                }
            }
        }
        else {
            // read the edges
            types::id_type first_id, second_id;

            for (types::size_type i = constants::begin_idx; i < n_edges; i++) {
                is >> first_id >> second_id;
                this->add_edge(first_id, second_id);
            }
        }
    }

    vetex_list_type _vertices{};
    implementation_type _impl{};
};

} // namespace gl
