// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

// TODO:
// - impl::add_edge should accept: id, first, second
// - impl::add_edges_from should accept: edge_ids view, source_id, target_id_range
// - impl::remove_edge should accept: id, first, second

#pragma once

#include "constants.hpp"
#include "graph_traits.hpp"
#include "impl/impl_tags.hpp"
#include "io/stream_options_manipulator.hpp"
#include "types/iterator_range.hpp"
#include "views.hpp"

#include <set>

namespace gl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits = graph_traits<>>
class graph final {
public:
    using traits_type = GraphTraits;
    using implementation_tag = typename traits_type::implementation_tag;

    using implementation_type = typename implementation_tag::template type<traits_type>;
    friend implementation_type;

    using vertex_type = typename traits_type::vertex_type;
    using vertex_properties_type = typename traits_type::vertex_properties_type;
    using vertex_properties_map_type = std::conditional_t<
        type_traits::is_default_properties_type_v<vertex_properties_type>,
        types::empty_properties_map,
        std::vector<std::unique_ptr<vertex_properties_type>>>;

    // TODO: reverese iterators should be available for bidirectional ranges

    using edge_type = typename traits_type::edge_type;
    using edge_ptr_type = typename traits_type::edge_ptr_type; // TODO: remove
    using edge_directional_tag = typename traits_type::edge_directional_tag;
    using edge_properties_type = typename traits_type::edge_properties_type;

    using edge_properties_map_type = std::conditional_t<
        type_traits::is_default_properties_type_v<vertex_properties_type>,
        types::empty_properties_map,
        std::vector<std::unique_ptr<edge_properties_type>>>;

    graph(const graph&) = delete;
    graph& operator=(const graph&) = delete;

    graph() = default;

    explicit graph(const types::size_type n_vertices)
    requires(type_traits::is_default_properties_type_v<vertex_properties_type>)
    : _n_vertices(n_vertices), _impl(n_vertices) {}

    explicit graph(const types::size_type n_vertices)
    requires(not type_traits::is_default_properties_type_v<vertex_properties_type>)
    : _n_vertices(n_vertices), _impl(n_vertices) {
        this->_vertex_properties.reserve(n_vertices);
        for (auto id : this->vertex_ids())
            this->_vertex_properties.push_back(std::make_unique<vertex_properties_type>());
    }

    graph(graph&&) = default;
    graph& operator=(graph&&) = default;

    ~graph() = default;

    // --- general methods ---

    [[nodiscard]] gl_attr_force_inline types::size_type n_vertices() const {
        return this->_n_vertices;
    }

    [[nodiscard]] gl_attr_force_inline types::size_type n_unique_edges() const {
        return this->_edges.size();
    }

    // --- vertex methods ---

    [[nodiscard]] gl_attr_force_inline auto vertices() const
    requires(type_traits::is_default_properties_type_v<vertex_properties_type>)
    {
        return this->vertex_ids()
             | std::views::transform([](const types::id_type id) { return vertex_descriptor{id}; });
    }

    [[nodiscard]] gl_attr_force_inline auto vertices() const
    requires(not type_traits::is_default_properties_type_v<vertex_properties_type>)
    {
        return this->_vertex_properties | std::views::enumerate
             | std::views::transform([](const auto& x) {
                   const auto& [id, ptr] = x;
                   return vertex_descriptor{static_cast<types::id_type>(id), *ptr};
               });
    }

    [[nodiscard]] gl_attr_force_inline std::ranges::iota_view<types::id_type, types::id_type>
    vertex_ids() const {
        return std::views::iota(constants::initial_id, this->_n_vertices);
    }

    // clang-format off
    // gl_attr_force_inline misplacement

    [[nodiscard]] gl_attr_force_inline vertex_type get_vertex(const types::id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        if constexpr (type_traits::is_default_properties_type_v<vertex_properties_type>)
            return vertex_descriptor{vertex_id};
        else
            return vertex_descriptor{vertex_id, *this->_vertex_properties[vertex_id]};
    }

    // clang-format on

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const types::id_type vertex_id) const {
        return vertex_id < this->_n_vertices;
    }

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const vertex_type& vertex) const {
        return this->has_vertex(vertex.id());
    }

    vertex_type add_vertex() {
        this->_impl.add_vertex();
        const auto new_vertex_id = this->_n_vertices++;

        if constexpr (type_traits::is_default_properties_type_v<vertex_properties_type>)
            return vertex_descriptor{new_vertex_id};
        else {
            this->_vertex_properties.push_back(std::make_unique<vertex_properties_type>());
            return vertex_descriptor{new_vertex_id, *this->_vertex_properties.back()};
        }
    }

    vertex_type add_vertex(vertex_properties_type properties)
    requires(not type_traits::is_default_properties_type_v<vertex_properties_type>)
    {
        this->_impl.add_vertex();
        this->_vertex_properties.push_back(
            std::make_unique<vertex_properties_type>(std::move(properties))
        );
        return vertex_descriptor{this->_n_vertices++, *this->_vertex_properties.back()};
    }

    void add_vertices(const types::size_type n) {
        this->_impl.add_vertices(n);
        this->_n_vertices += n;

        if constexpr (not type_traits::is_default_properties_type_v<vertex_properties_type>) {
            const auto old_size = this->_vertex_properties.size();
            this->_vertex_properties.reserve(this->_n_vertices);
            for (types::size_type i = old_size; i < this->_n_vertices; ++i)
                this->_vertex_properties.push_back(std::make_unique<vertex_properties_type>());
        }
    }

    void add_vertices_with(
        const type_traits::c_sized_range_of<vertex_properties_type> auto& properties_range
    )
    requires(not type_traits::is_default_properties_type_v<vertex_properties_type>)
    {
        const auto n = std::ranges::size(properties_range);

        this->_impl.add_vertices(n);
        this->_n_vertices += n;

        if constexpr (not type_traits::is_default_properties_type_v<vertex_properties_type>) {
            for (auto& properties : properties_range) {
                this->_vertex_properties.emplace_back(
                    std::make_unique<vertex_properties_type>(properties)
                );
            }
        }
    }

    gl_attr_force_inline void remove_vertex(const types::size_type vertex_id) {
        this->_verify_vertex_id(vertex_id);
        this->_remove_vertex_impl(vertex_id);
    }

    gl_attr_force_inline void remove_vertex(const vertex_type& vertex) {
        this->remove_vertex(vertex.id());
    }

    void remove_vertices_from(
        const type_traits::c_sized_range_of<types::id_type> auto& vertex_id_range
    ) {
        // sorts the ids in a descending order and removes duplicate ids
        std::set<types::id_type, std::greater<types::id_type>> vertex_id_set(
            std::ranges::begin(vertex_id_range), std::ranges::end(vertex_id_range)
        );

        // TODO: optimize
        for (const auto vertex_id : vertex_id_set)
            this->_remove_vertex_impl(vertex_id);
    }

    void remove_vertices_from(const type_traits::c_sized_range_of<vertex_type> auto& vertex_range) {
        // TODO: optimize
        // sort the ids in a descending order and removes duplicate ids
        std::set<vertex_type> vertex_set(
            std::ranges::begin(vertex_range), std::ranges::end(vertex_range)
        );
        for (const auto& vertex : vertex_set)
            this->_remove_vertex_impl(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline types::size_type in_degree(const types::id_type vertex_id
    ) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.in_degree(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type in_degree(const vertex_type& vertex) const {
        return this->in_degree(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline std::vector<types::size_type> in_degree_map() const {
        return this->_impl.in_degree_map();
    }

    [[nodiscard]] gl_attr_force_inline types::size_type out_degree(const types::id_type vertex_id
    ) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.out_degree(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type out_degree(const vertex_type& vertex
    ) const {
        return this->out_degree(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline std::vector<types::size_type> out_degree_map() const {
        return this->_impl.out_degree_map();
    }

    [[nodiscard]] gl_attr_force_inline types::size_type degree(const types::id_type vertex_id
    ) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.degree(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type degree(const vertex_type& vertex) const {
        return this->degree(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline std::vector<types::size_type> degree_map() const {
        return this->_impl.degree_map();
    }

    // TODO: add a get_vertex_properties function

    [[nodiscard]] gl_attr_force_inline auto vertex_properties_map() const noexcept
    requires(not type_traits::is_default_properties_type_v<vertex_properties_type>)
    {
        return views::deref(this->_vertex_properties);
    }

    // --- edge methods ---

    // clang-format off
    // gl_attr_force_inline misplacement

    const edge_type add_edge(
        const types::id_type first_id, const types::id_type second_id
    ) {
        this->_verify_vertex_id(first_id);
        this->_verify_vertex_id(second_id);

        const auto new_edge_id = this->_n_unique_edges++;
        this->_impl.add_edge(new_edge_id, first_id, second_id);

        if constexpr (type_traits::is_default_properties_type_v<edge_properties_type>) {
            return edge_type{new_edge_id, first_id, second_id};
        }
        else {
            auto& p = this->_edge_properties.emplace_back(std::make_unique<edge_properties_type>());
            return edge_type{new_edge_id, first_id, second_id, *p};
        }
    }

    const edge_type add_edge(
        const types::id_type first_id,
        const types::id_type second_id,
        const edge_properties_type& properties
    )
    requires(not type_traits::is_default_properties_type_v<edge_properties_type>)
    {
        this->_verify_vertex_id(first_id);
        this->_verify_vertex_id(second_id);

        const auto new_edge_id = this->_n_unique_edges++;
        this->_impl.add_edge(new_edge_id, first_id, second_id);

        auto& p = this->_edge_properties.emplace_back(std::make_unique<edge_properties_type>());
        return edge_type{new_edge_id, first_id, second_id, *p};
    }

    // clang-format on

    const gl_attr_force_inline edge_type
    add_edge(const vertex_type& first, const vertex_type& second) {
        return this->add_edge(first.id(), second.id());
    }

    const gl_attr_force_inline edge_type add_edge(
        const vertex_type& first, const vertex_type& second, const edge_properties_type& properties
    )
    requires(not type_traits::is_default_properties_type_v<edge_properties_type>)
    {
        return this->add_edge(first.id(), second.id(), properties);
    }

    void add_edges_from(
        const types::id_type source_id,
        const type_traits::c_sized_range_of<types::id_type> auto& target_id_range
    ) {
        // TODO: validate no duplicate target ids
        this->_verify_vertex_id(source_id);

        for (const auto target_id : target_id_range) {
            this->_verify_vertex_id(target_id);
            if constexpr (not type_traits::is_default_properties_type_v<edge_properties_type>)
                this->_edge_properties.emplace_back(std::make_unique<edge_properties_type>());
        }

        const auto prev_n_edges = this->_n_unique_edges;
        this->_n_unique_edges += std::ranges::size(target_id_range);
        this->_impl.add_edges_from(
            std::views::iota(prev_n_edges, this->_n_unique_edges), source_id, target_id_range
        );
    }

    // TODO: range of convertible_to<const vertex_type&>
    gl_attr_force_inline void add_edges_from(
        const vertex_type& source,
        const type_traits::c_sized_range_of<vertex_type> auto& target_range
    ) {
        // TODO: validate no duplicate targets
        this->_verify_vertex_id(source.id());

        for (const auto& target : target_range) {
            this->_verify_vertex_id(target.id());
            if constexpr (not type_traits::is_default_properties_type_v<edge_properties_type>)
                this->_edge_properties.emplace_back(std::make_unique<edge_properties_type>());
        }

        const auto prev_n_edges = this->_n_unique_edges;
        this->_n_unique_edges += std::ranges::size(target_range);
        this->_impl.add_edges_from(
            std::views::iota(prev_n_edges, this->_n_unique_edges),
            source.id(),
            target_range | std::views::transform(&vertex_type::id)
        );
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(
        const types::id_type first_id, const types::id_type second_id
    ) const {
        this->_verify_vertex_id(first_id);
        this->_verify_vertex_id(second_id);
        return this->_impl.has_edge(first_id, second_id);
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(
        const vertex_type& first, const vertex_type& second
    ) const {
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
        return this->_impl.get_edge(first.id(), second.id());
    }

    [[nodiscard]] inline std::vector<edge_type> get_edges(
        const types::id_type first_id, const types::id_type second_id
    ) const {
        this->_verify_vertex_id(first_id);
        this->_verify_vertex_id(second_id);
        return this->_impl.get_edges(first_id, second_id);
    }

    [[nodiscard]] std::vector<types::const_ref_wrap<edge_type>> get_edges(
        const vertex_type& first, const vertex_type& second
    ) const {
        return this->get_edges(first.id(), second.id());
    }

    gl_attr_force_inline void remove_edge(const edge_type& edge) {
        this->_verify_edge(edge);
        if constexpr (not type_traits::is_default_properties_type_v<edge_properties_type>)
            this->_edge_properties.erase(this->_edge_properties.begin() + edge.id());
        this->_impl.remove_edge(edge);
    }

    // TODO: range of convertible_to<const edge_type&>
    inline void remove_edges_from(const type_traits::c_range_of<edge_type> auto& edges) {
        // TODO: optimize
        for (const auto& edge_ref : edges)
            this->_impl.remove_edge(edge_ref.get());
    }

    [[nodiscard]] inline auto adjacent_edges(const types::id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.adjacent_edges(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto adjacent_edges(const vertex_type& vertex) const {
        return this->adjacent_edges(vertex.id());
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

    [[nodiscard]] gl_attr_force_inline bool are_incident(
        const vertex_type& first, const vertex_type& second
    ) const {
        return this->are_incident(first.id(), second.id());
    }

    [[nodiscard]] bool are_incident(const vertex_type& vertex, const edge_type& edge) const {
        this->_verify_vertex_id(vertex.id());
        this->_verify_edge(edge);
        return edge.is_incident_with(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline bool are_incident(
        const edge_type& edge, const vertex_type& vertex
    ) const {
        return this->are_incident(vertex, edge);
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

    void _verify_edge(const edge_type& edge) const {
        if (not this->has_edge(edge))
            throw std::invalid_argument(std::format(
                "Got invalid edge [vertices = ({}, {}) | addr = {}]",
                edge.first(),
                edge.second(),
                io::format(&edge)
            ));
    }

    // --- vertex methods ---

    void _remove_vertex_impl(const types::id_type vertex_id) {
        this->_impl.remove_vertex(vertex_id);
        this->_n_vertices--;

        // update vertex ids in edges
        for (auto id : this->vertex_ids()) {
            for (auto& edge : this->_impl.adjacent_edges(id)) {
                edge._vertices.first -= (edge._vertices.first > vertex_id);
                edge._vertices.second -= (edge._vertices.second > vertex_id);
            }
        }

        if constexpr (not type_traits::is_default_properties_type_v<vertex_properties_type>)
            this->_vertex_properties.erase(
                std::next(std::begin(this->_vertex_properties), vertex_id)
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

        for (const auto& vertex : this->vertices()) {
            os << "- " << vertex << "\n  adjacent edges:\n";
            for (const auto& edge : this->_impl.adjacent_edges(vertex.id()))
                os << "\t- " << edge << '\n';
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
                    os << vertex._properties << '\n';

        if constexpr (type_traits::c_writable<typename edge_type::properties_type>) {
            if (with_edge_properties) {
                const auto print_incident_edges = [this, &os](const types::id_type vertex_id) {
                    for (const auto& edge : this->_impl.adjacent_edges(vertex_id)) {
                        if (edge.first() != vertex_id)
                            continue; // vertex is not the source
                        os << edge.first() << ' ' << edge.second() << ' ' << edge._properties
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
                if (edge.first() != vertex_id)
                    continue; // vertex is not the source
                os << edge.first() << ' ' << edge.second() << '\n';
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
                for (types::size_type i = constants::begin_idx; i < n_vertices; ++i)
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

                for (types::size_type i = constants::begin_idx; i < n_edges; ++i) {
                    is >> first_id >> second_id >> properties;
                    this->add_edge(first_id, second_id, properties);
                }
            }
        }
        else {
            // read the edges
            types::id_type first_id, second_id;

            for (types::size_type i = constants::begin_idx; i < n_edges; ++i) {
                is >> first_id >> second_id;
                this->add_edge(first_id, second_id);
            }
        }
    }

    types::size_type _n_vertices = 0uz;
    types::size_type _n_unique_edges = 0uz;

    [[no_unique_address]] vertex_properties_map_type _vertex_properties{};
    [[no_unique_address]] edge_properties_map_type _edge_properties{};

    implementation_type _impl{};
};

} // namespace gl
