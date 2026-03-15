// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/graph_traits.hpp"
#include "gl/impl/impl_tags.hpp"
#include "gl/io/stream_options_manipulator.hpp"
#include "gl/util/ranges.hpp"

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
        type_traits::c_empty_properties<vertex_properties_type>,
        types::empty_properties_map,
        std::vector<std::unique_ptr<vertex_properties_type>>>;

    using edge_type = typename traits_type::edge_type;
    using edge_directional_tag = typename traits_type::edge_directional_tag;
    using edge_properties_type = typename traits_type::edge_properties_type;

    using edge_properties_map_type = std::conditional_t<
        type_traits::c_empty_properties<edge_properties_type>,
        types::empty_properties_map,
        std::vector<std::unique_ptr<edge_properties_type>>>;

    graph(const graph&) = delete;
    graph& operator=(const graph&) = delete;

    graph() = default;

    explicit graph(const types::size_type n_vertices) : _n_vertices(n_vertices), _impl(n_vertices) {
        if constexpr (type_traits::c_non_empty_properties<vertex_properties_type>) {
            this->_vertex_properties.reserve(n_vertices);
            for (const auto _ : this->vertex_ids())
                this->_vertex_properties.push_back(std::make_unique<vertex_properties_type>());
        }
    }

    graph(graph&&) = default;
    graph& operator=(graph&&) = default;

    ~graph() = default;

    // --- general methods ---

    [[nodiscard]] gl_attr_force_inline types::size_type order() const noexcept {
        return this->_n_vertices;
    }

    [[nodiscard]] gl_attr_force_inline types::size_type size() const noexcept {
        return this->_n_edges;
    }

    // --- vertex methods ---

    [[nodiscard]] gl_attr_force_inline auto vertices() const
    requires(type_traits::c_empty_properties<vertex_properties_type>)
    {
        return this->vertex_ids()
             | std::views::transform([](const types::id_type id) { return vertex_descriptor{id}; });
    }

    [[nodiscard]] gl_attr_force_inline auto vertices() const
    requires(type_traits::c_non_empty_properties<vertex_properties_type>)
    {
        return this->_vertex_properties | std::views::enumerate
             | std::views::transform([](const auto& x) {
                   const auto& [id, ptr] = x;
                   return vertex_descriptor{static_cast<types::id_type>(id), *ptr};
               });
    }

    [[nodiscard]] gl_attr_force_inline auto vertex_ids() const noexcept {
        return std::views::iota(constants::initial_id, this->_n_vertices);
    }

    [[nodiscard]] vertex_type get_vertex(const types::id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        if constexpr (type_traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_descriptor{vertex_id, *this->_vertex_properties[vertex_id]};
        else
            return vertex_descriptor{vertex_id};
    }

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const types::id_type vertex_id) const {
        return vertex_id < this->_n_vertices;
    }

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const vertex_type& vertex) const {
        return this->has_vertex(vertex.id());
    }

    const vertex_type add_vertex() {
        this->_impl.add_vertex();
        const auto new_vertex_id = this->_n_vertices++;

        if constexpr (type_traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_descriptor{
                new_vertex_id,
                *this->_vertex_properties.emplace_back(std::make_unique<vertex_properties_type>())
            };
        else
            return vertex_descriptor{new_vertex_id};
    }

    const vertex_type add_vertex_with(vertex_properties_type properties)
    requires(type_traits::c_non_empty_properties<vertex_properties_type>)
    {
        this->_impl.add_vertex();
        return vertex_descriptor{
            this->_n_vertices++,
            *this->_vertex_properties.emplace_back(
                std::make_unique<vertex_properties_type>(std::move(properties))
            )
        };
    }

    void add_vertices(const types::size_type n) {
        this->_impl.add_vertices(n);
        this->_n_vertices += n;

        if constexpr (type_traits::c_non_empty_properties<vertex_properties_type>) {
            const auto old_size = this->_vertex_properties.size();
            this->_vertex_properties.reserve(this->_n_vertices);
            for (types::size_type i = old_size; i < this->_n_vertices; ++i)
                this->_vertex_properties.push_back(std::make_unique<vertex_properties_type>());
        }
    }

    void add_vertices_with(
        const type_traits::c_sized_range_of<vertex_properties_type> auto& properties_range
    )
    requires(type_traits::c_non_empty_properties<vertex_properties_type>)
    {
        const auto n = std::ranges::size(properties_range);

        this->_impl.add_vertices(n);
        this->_n_vertices += n;

        if constexpr (type_traits::c_non_empty_properties<vertex_properties_type>) {
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
        const type_traits::c_forward_range_of<types::id_type> auto& vertex_id_range
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
        std::set<vertex_type, std::greater<vertex_type>> vertex_set(
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

    [[nodiscard]] inline auto at(const types::id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        if constexpr (type_traits::c_non_empty_properties<edge_properties_type>)
            return this->_impl.at(vertex_id, this->_edge_properties);
        else
            return this->_impl.at(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto at(const vertex_type& vertex) const {
        return this->at(vertex.id());
    }

    [[nodiscard]] inline auto adjacent_edges(const types::id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        if constexpr (type_traits::c_non_empty_properties<edge_properties_type>)
            return this->_impl.adjacent_edges(vertex_id, this->_edge_properties);
        else
            return this->_impl.adjacent_edges(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto adjacent_edges(const vertex_type& vertex) const {
        return this->adjacent_edges(vertex.id());
    }

    // --- edge methods ---

    const edge_type add_edge(const types::id_type source_id, const types::id_type target_id) {
        this->_verify_vertex_id(source_id);
        this->_verify_vertex_id(target_id);

        const auto new_edge_id = this->_n_edges++;
        this->_impl.add_edge(new_edge_id, source_id, target_id);

        if constexpr (type_traits::c_non_empty_properties<edge_properties_type>) {
            const auto& p =
                this->_edge_properties.emplace_back(std::make_unique<edge_properties_type>());
            return edge_type{new_edge_id, source_id, target_id, *p};
        }
        else {
            return edge_type{new_edge_id, source_id, target_id};
        }
    }

    const edge_type add_edge_with(
        const types::id_type source_id,
        const types::id_type target_id,
        const edge_properties_type& properties
    )
    requires(type_traits::c_non_empty_properties<edge_properties_type>)
    {
        this->_verify_vertex_id(source_id);
        this->_verify_vertex_id(target_id);

        const auto new_edge_id = this->_n_edges++;
        this->_impl.add_edge(new_edge_id, source_id, target_id);

        auto& p =
            this->_edge_properties.emplace_back(std::make_unique<edge_properties_type>(properties));
        return edge_type{new_edge_id, source_id, target_id, *p};
    }

    // clang-format off
    // gl_attr_force_inline misplacement

    gl_attr_force_inline const edge_type
    add_edge(const vertex_type& source, const vertex_type& target) {
        return this->add_edge(source.id(), target.id());
    }

    gl_attr_force_inline const edge_type add_edge_with(
        const vertex_type& source, const vertex_type& target, const edge_properties_type& properties
    )
    requires(type_traits::c_non_empty_properties<edge_properties_type>)
    {
        return this->add_edge_with(source.id(), target.id(), properties);
    }

    // clang-format on

    void add_edges_from(
        const types::id_type source_id,
        const type_traits::c_sized_range_of<types::id_type> auto& target_id_range
    ) {
        this->_verify_vertex_id(source_id);

        for (const auto target_id : target_id_range) {
            this->_verify_vertex_id(target_id);
            if constexpr (type_traits::c_non_empty_properties<edge_properties_type>)
                this->_edge_properties.emplace_back(std::make_unique<edge_properties_type>());
        }

        const auto prev_n_edges = this->_n_edges;
        this->_n_edges += std::ranges::size(target_id_range);
        this->_impl.add_edges_from(
            std::views::iota(prev_n_edges, this->_n_edges), source_id, target_id_range
        );
    }

    gl_attr_force_inline void add_edges_from(
        const vertex_type& source,
        const type_traits::c_sized_range_of<vertex_type> auto& target_range
    ) {
        this->_verify_vertex_id(source.id());

        for (const auto& target : target_range) {
            this->_verify_vertex_id(target.id());
            if constexpr (type_traits::c_non_empty_properties<edge_properties_type>)
                this->_edge_properties.emplace_back(std::make_unique<edge_properties_type>());
        }

        const auto prev_n_edges = this->_n_edges;
        this->_n_edges += std::ranges::size(target_range);
        this->_impl.add_edges_from(
            std::views::iota(prev_n_edges, this->_n_edges),
            source.id(),
            target_range | std::views::transform(&vertex_type::id)
        );
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(
        const types::id_type source_id, const types::id_type target_id
    ) const {
        this->_verify_vertex_id(source_id);
        this->_verify_vertex_id(target_id);
        return this->_impl.has_edge(source_id, target_id);
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(
        const vertex_type& source, const vertex_type& target
    ) const {
        return this->has_edge(source.id(), target.id());
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(const edge_type& edge) const {
        return this->_impl.has_edge(edge);
    }

    [[nodiscard]] gl_attr_force_inline std::optional<edge_type> get_edge(
        const types::id_type source_id, const types::id_type target_id
    ) const {
        this->_verify_vertex_id(source_id);
        this->_verify_vertex_id(target_id);

        if constexpr (type_traits::c_non_empty_properties<edge_properties_type>)
            return this->_impl.get_edge(source_id, target_id, this->_edge_properties);
        else
            return this->_impl.get_edge(source_id, target_id);
    }

    [[nodiscard]] gl_attr_force_inline std::optional<edge_type> get_edge(
        const vertex_type& source, const vertex_type& target
    ) const {
        return this->get_edge(source.id(), target.id());
    }

    [[nodiscard]] inline std::vector<edge_type> get_edges(
        const types::id_type source_id, const types::id_type target_id
    ) const {
        this->_verify_vertex_id(source_id);
        this->_verify_vertex_id(target_id);

        if constexpr (type_traits::c_non_empty_properties<edge_properties_type>)
            return this->_impl.get_edges(source_id, target_id, this->_edge_properties);
        else
            return this->_impl.get_edges(source_id, target_id);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<edge_type> get_edges(
        const vertex_type& source, const vertex_type& target
    ) const {
        return this->get_edges(source.id(), target.id());
    }

    gl_attr_force_inline void remove_edge(const edge_type& edge) {
        this->_verify_edge(edge);
        if constexpr (type_traits::c_non_empty_properties<edge_properties_type>)
            this->_edge_properties.erase(this->_edge_properties.begin() + edge.id());
        this->_impl.remove_edge(edge);
        this->_n_edges--;
    }

    inline void remove_edges(const type_traits::c_range_of<edge_type> auto& edges) {
        const auto removed_edge_ids = this->_impl.remove_edges(edges);
        this->_n_edges -= removed_edge_ids.size();

        if constexpr (type_traits::c_non_empty_properties<edge_properties_type>) {
            // IDs are sorted and do not contain duplicates
            for (const auto edge_id : std::views::reverse(removed_edge_ids))
                this->_edge_properties.erase(this->_edge_properties.begin() + edge_id);
        }
    }

    // --- incidence methods ---

    [[nodiscard]] bool are_incident(const types::id_type source_id, const types::id_type target_id)
        const {
        this->_verify_vertex_id(source_id);
        if (source_id == target_id)
            return true;

        this->_verify_vertex_id(target_id);

        if constexpr (type_traits::c_directed_edge<edge_type>)
            return this->has_edge(source_id, target_id) or this->has_edge(target_id, source_id);
        else
            return this->has_edge(source_id, target_id);
    }

    [[nodiscard]] gl_attr_force_inline bool are_incident(
        const vertex_type& source, const vertex_type& target
    ) const {
        return this->are_incident(source.id(), target.id());
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
        return edge_1.is_incident_with(edge_2.source()) or edge_1.is_incident_with(edge_2.target());
    }

    // --- property methods ---

    [[nodiscard]] gl_attr_force_inline auto vertex_properties_map() const noexcept
    requires(type_traits::c_non_empty_properties<vertex_properties_type>)
    {
        return util::deref_view(this->_vertex_properties);
    }

    [[nodiscard]] gl_attr_force_inline vertex_properties_type& get_vertex_properties(
        const types::id_type id
    ) const
    requires(type_traits::c_non_empty_properties<vertex_properties_type>)
    {
        this->_verify_vertex_id(id);
        return *this->_vertex_properties[id];
    }

    [[nodiscard]] gl_attr_force_inline auto edge_properties_map() const noexcept
    requires(type_traits::c_non_empty_properties<edge_properties_type>)
    {
        return util::deref_view(this->_edge_properties);
    }

    [[nodiscard]] edge_properties_type& get_edge_properties(const types::id_type id) const
    requires(type_traits::c_non_empty_properties<edge_properties_type>)
    {
        if (id >= this->_n_edges)
            throw std::out_of_range(std::format("Got invalid edge id [{}]", id));

        return *this->_edge_properties[id];
    }

    // --- stream operators ---

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
        return type_traits::c_directed_edge<edge_type> ? "directed" : "undirected";
    }

    // --- graph element verification methods ---

    gl_attr_force_inline void _verify_vertex_id(const types::id_type vertex_id) const {
        if (not this->has_vertex(vertex_id))
            throw std::out_of_range(std::format("Got invalid vertex id [{}]", vertex_id));
    }

    void _verify_edge(const edge_type& edge) const {
        if (edge.id() >= this->_n_edges or not this->has_vertex(edge.source())
            or not this->has_vertex(edge.target()))
            throw std::invalid_argument(std::format(
                "Got invalid edge [id = {}, vertices = ({}, {})]",
                edge.id(),
                edge.source(),
                edge.target()
            ));
    }

    // --- vertex methods ---

    void _remove_vertex_impl(const types::id_type vertex_id) {
        const auto removed_edge_ids = this->_impl.remove_vertex(vertex_id);
        this->_n_vertices--;
        this->_n_edges -= removed_edge_ids.size();

        if constexpr (type_traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.erase(this->_vertex_properties.begin() + vertex_id);

        if constexpr (type_traits::c_non_empty_properties<edge_properties_type>) {
            // IDs are sorted and do not contain duplicates
            for (const auto& edge_id : std::views::reverse(removed_edge_ids))
                this->_edge_properties.erase(this->_edge_properties.begin() + edge_id);
        }
    }

    // --- io methods ---

    void _verbose_write(std::ostream& os) const {
        os << std::format(
            "type: {}\nnumber of vertices: {}\nnumber of edges: {}\nvertices:\n",
            _directed_type_str(),
            this->order(),
            this->size()
        );

        for (const auto& vertex : this->vertices()) {
            os << "- " << vertex << "\n  adjacent edges:\n";
            for (const auto& edge : this->adjacent_edges(vertex.id()))
                os << "\t- " << edge << '\n';
        }
    }

    void _concise_write(std::ostream& os) const {
        os << std::format("{} {} {}\n", _directed_type_str(), this->order(), this->size());

        for (const auto& vertex : this->vertices()) {
            os << "- " << vertex << " :";
            for (const auto& edge : this->adjacent_edges(vertex.id()))
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
            static_cast<int>(type_traits::c_directed_edge<edge_type>),
            this->order(),
            this->size(),
            static_cast<int>(with_vertex_properties),
            static_cast<int>(with_edge_properties)
        );

        if constexpr (type_traits::c_writable<typename vertex_type::properties_type>)
            if (with_vertex_properties)
                for (const auto& vertex : this->vertices())
                    os << vertex.properties() << '\n';

        if constexpr (type_traits::c_writable<typename edge_type::properties_type>) {
            if (with_edge_properties) {
                const auto print_incident_edges = [this, &os](const types::id_type vertex_id) {
                    for (const auto& edge : this->adjacent_edges(vertex_id)) {
                        if (edge.source() != vertex_id)
                            continue; // vertex is not the source
                        os << edge.source() << ' ' << edge.target() << ' ' << edge.properties()
                           << '\n';
                    }
                };

                for (const auto vertex_id : this->vertex_ids())
                    print_incident_edges(vertex_id);

                return;
            }
        }

        const auto print_incident_edges = [this, &os](const types::id_type vertex_id) {
            for (const auto& edge : this->adjacent_edges(vertex_id)) {
                if (edge.source() != vertex_id)
                    continue; // vertex is not the source
                os << edge.source() << ' ' << edge.target() << '\n';
            }
        };

        for (const auto vertex_id : this->vertex_ids())
            print_incident_edges(vertex_id);
    }

    void _gsf_read(std::istream& is) {
        bool directed;
        is >> directed;

        if (directed != type_traits::c_directed_edge<edge_type>)
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
                for (types::id_type i = 0uz; i < n_vertices; ++i)
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
                types::id_type source_id, target_id;
                edge_properties_type properties;

                for (types::size_type _ = 0uz; _ < n_edges; ++_) {
                    is >> source_id >> target_id >> properties;
                    this->add_edge_with(source_id, target_id, properties);
                }
            }
        }
        else {
            // read the edges
            types::id_type source_id, target_id;

            for (types::size_type _ = 0uz; _ < n_edges; ++_) {
                is >> source_id >> target_id;
                this->add_edge(source_id, target_id);
            }
        }
    }

    types::size_type _n_vertices = 0uz;
    types::size_type _n_edges = 0uz;

    [[no_unique_address]] vertex_properties_map_type _vertex_properties{};
    [[no_unique_address]] edge_properties_map_type _edge_properties{};

    implementation_type _impl{};
};

// --- general graph utility ---

namespace type_traits {

template <typename G>
concept c_graph = c_instantiation_of<G, graph>;

template <typename G>
concept c_directed_graph = c_graph<G> and c_directed_edge<typename G::edge_type>;

template <typename G>
concept c_undirected_graph = c_graph<G> and c_undirected_edge<typename G::edge_type>;

} // namespace type_traits

// --- utility associated with graph's elements' properties ---

namespace types {

using default_vertex_distance_type = std::int64_t;

template <type_traits::c_graph GraphType>
struct vertex_distance {
    using type = default_vertex_distance_type;
};

template <type_traits::c_graph GraphType>
requires(type_traits::c_weight_properties_type<typename GraphType::edge_properties_type>)
struct vertex_distance<GraphType> {
    using type = typename GraphType::edge_properties_type::weight_type;
};

template <type_traits::c_graph GraphType>
using vertex_distance_type = typename vertex_distance<GraphType>::type;

} // namespace types

template <type_traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline types::vertex_distance_type<GraphType> get_weight(
    const typename GraphType::edge_type& edge
) {
    if constexpr (type_traits::c_weight_properties_type<typename GraphType::edge_properties_type>)
        return edge.properties().weight;
    else
        return static_cast<types::default_vertex_distance_type>(1ll);
}

} // namespace gl
