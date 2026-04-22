// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/constants.hpp"
#include "gl/directional_tags.hpp"
#include "gl/graph_traits.hpp"
#include "gl/impl/impl_tags.hpp"
#include "gl/io/graph_fmt_traits.hpp"
#include "gl/io/options.hpp"
#include "gl/io/options_manip.hpp"
#include "gl/traits.hpp"
#include "gl/util/ranges.hpp"

#include <set>

namespace gl {

template <traits::c_instantiation_of<graph_traits> GraphTraits = graph_traits<>>
class graph;

// --- general graph utility ---

namespace traits {

template <typename G>
concept c_graph = c_instantiation_of<G, graph>;

template <typename G>
concept c_directed_graph = c_graph<G> and c_directed_edge<typename G::edge_type>;

template <typename G>
concept c_undirected_graph = c_graph<G> and c_undirected_edge<typename G::edge_type>;

template <typename G>
concept c_list_graph = c_graph<G> and std::same_as<typename G::implementation_tag, impl::list_t>;

template <typename G>
concept c_flat_list_graph =
    c_graph<G> and std::same_as<typename G::implementation_tag, impl::flat_list_t>;

template <typename G>
concept c_adjacency_list_graph = c_list_graph<G> or c_flat_list_graph<G>;

template <typename G>
concept c_matrix_graph =
    c_graph<G> and std::same_as<typename G::implementation_tag, impl::matrix_t>;

template <typename G>
concept c_adjacency_matrix_graph = c_matrix_graph<G>;

} // namespace traits

template <traits::c_graph Graph>
[[nodiscard]] Graph clone(const Graph& source);

template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph Graph>
[[nodiscard]] auto to(Graph&& source);

namespace detail {

template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph_impl_tag SourceImplTag>
struct to_impl;

} // namespace detail

/// @brief A general-purpose graph container.
/// @ingroup gl
///
/// This class represents a highly customizable graph data structure configured by the provided
/// `GraphTraits`. It serves as the primary interface for managing vertices, edges, and their properties.
///
/// @tparam GraphTraits An instantiation of @ref gl::graph_traits
///
/// ### Mathematics
/// Inline math: \f$V = E - F + 2\f$
/// Big-O: \f$\mathcal{O}(|V| + |E|)\f$
///
/// Display math:
///
/// \f$
/// \sum_{v \in V} \text{deg}(v) = 2 |E|
/// \f$
///
/// Complex environment:
///
/// \f$
/// A_{i,j} = \begin{cases} 1 & \text{if } (i,j) \in E \\ 0 & \text{otherwise} \end{cases}
/// \f$
///
/// ### Code Example
/// ```cpp
/// gl::directed_graph<> g;
/// auto v1 = g.add_vertex();
/// auto v2 = g.add_vertex();
/// g.add_edge(v1, v2);
/// for (const auto v : graph.vertices()) {
///     std::cout << v << ": ";
///     for (const auto u : graph.neighbors(v)) {
///         std::cout << u << " ";
///     }
/// }
/// ```
///
/// ### References
/// For a general overview and integration instructions, see the [Project Overview](/#overview)
/// or the [Installation Guide](/#installing-the-library).
/// A simple reference [MAIN PAGE](/)
///
/// > [!NOTE]
/// > Highlights information that users should take into account, even when skimming.
///
/// > [!TIP]
/// > Optional information to help a user be more successful.
///
/// > [!IMPORTANT]
/// > Crucial information necessary for users to succeed.
///
/// > [!WARNING]
/// > Critical content demanding immediate user attention due to potential risks.
///
/// > [!CAUTION]
/// > Negative potential consequences of an action.
template <traits::c_instantiation_of<graph_traits> GraphTraits>
class graph final {
public:
    using traits_type = GraphTraits;

    using directional_tag = typename traits_type::directional_tag;
    using implementation_tag = typename traits_type::implementation_tag;

    using implementation_type = typename implementation_tag::template type<traits_type>;
    friend implementation_type;

    using id_type = typename traits_type::id_type;

    using vertex_type = typename traits_type::vertex_type;
    using vertex_properties_type = typename traits_type::vertex_properties_type;
    using vertex_properties_map_type = std::conditional_t<
        traits::c_empty_properties<vertex_properties_type>,
        empty_properties_map,
        std::vector<vertex_properties_type>>;

    using edge_type = typename traits_type::edge_type;
    using edge_properties_type = typename traits_type::edge_properties_type;

    using edge_properties_map_type = std::conditional_t<
        traits::c_empty_properties<edge_properties_type>,
        empty_properties_map,
        std::vector<edge_properties_type>>;

    graph& operator=(const graph&) = delete;

    graph() = default;

    explicit graph(const size_type n_vertices) : _n_vertices(n_vertices), _impl(n_vertices) {
        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.resize(n_vertices);
    }

    graph(graph&&) noexcept = default;
    graph& operator=(graph&&) noexcept = default;

    ~graph() = default;

    // --- size methods ---

    [[nodiscard]] gl_attr_force_inline size_type n_vertices() const noexcept {
        return this->_n_vertices;
    }

    [[nodiscard]] gl_attr_force_inline size_type n_edges() const noexcept {
        return this->_n_edges;
    }

    // --- vertex modifiers ---

    vertex_type add_vertex() {
        this->_impl.add_vertex();
        const auto new_vertex_id = static_cast<id_type>(this->_n_vertices++);

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_descriptor{new_vertex_id, this->_vertex_properties.emplace_back()};
        else
            return vertex_descriptor{new_vertex_id};
    }

    vertex_type add_vertex_with(vertex_properties_type properties)
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        this->_impl.add_vertex();
        return vertex_descriptor{
            static_cast<id_type>(this->_n_vertices++),
            this->_vertex_properties.emplace_back(std::move(properties))
        };
    }

    void add_vertices(const size_type n) {
        this->_impl.add_vertices(n);
        this->_n_vertices += n;

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.resize(this->_n_vertices);
    }

    void add_vertices_with(
        const traits::c_sized_range_of<vertex_properties_type> auto& properties_rng
    )
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        const auto n = std::ranges::size(properties_rng);

        this->_impl.add_vertices(n);
        this->_n_vertices += n;

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.insert(
                this->_vertex_properties.end(),
                std::ranges::begin(properties_rng),
                std::ranges::end(properties_rng)
            );
    }

    gl_attr_force_inline void remove_vertex(const id_type vertex_id) {
        this->_verify_vertex_id(vertex_id);
        this->_remove_vertex_impl(vertex_id);
    }

    gl_attr_force_inline void remove_vertex(vertex_type vertex) {
        this->remove_vertex(vertex.id());
    }

    void remove_vertices_from(const traits::c_forward_range_of<id_type> auto& vertex_id_rng) {
        // sorts the ids in a descending order and removes duplicate ids
        std::set<id_type, std::greater<>> vertex_id_set(
            std::ranges::begin(vertex_id_rng), std::ranges::end(vertex_id_rng)
        );

        // TODO: optimize
        for (auto vertex_id : vertex_id_set)
            this->_remove_vertex_impl(vertex_id);
    }

    void remove_vertices_from(const traits::c_sized_range_of<vertex_type> auto& vertex_rng) {
        // TODO: optimize
        // sort the ids in a descending order and removes duplicate ids
        std::set<vertex_type, std::greater<vertex_type>> vertex_set(
            std::ranges::begin(vertex_rng), std::ranges::end(vertex_rng)
        );
        for (auto vertex : vertex_set)
            this->_remove_vertex_impl(vertex.id());
    }

    // --- vertex getters ---

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const id_type vertex_id) const {
        return vertex_id < this->_n_vertices;
    }

    [[nodiscard]] gl_attr_force_inline bool has_vertex(vertex_type vertex) const {
        return this->has_vertex(vertex.id());
    }

    [[nodiscard]] vertex_type vertex(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->vertex_unchecked(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline vertex_type at(const id_type vertex_id) const {
        return this->vertex(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline vertex_type vertex_unchecked(const id_type vertex_id
    ) const noexcept {
        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_descriptor{vertex_id, this->_vertex_properties[vertex_id]};
        else
            return vertex_descriptor{vertex_id};
    }

    [[nodiscard]] gl_attr_force_inline vertex_type operator[](const id_type vertex_id
    ) const noexcept {
        return this->vertex_unchecked(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto vertices() const noexcept {
        return this->vertex_ids() | std::views::transform(this->_create_vertex_descriptor());
    }

    [[nodiscard]] gl_attr_force_inline auto vertex_ids() const noexcept {
        return std::views::iota(initial_id_v<id_type>, this->_n_vertices);
    }

    [[nodiscard]] gl_attr_force_inline auto neighbors(const id_type vertex_id) const {
        return this->neighbor_ids(vertex_id)
             | std::views::transform(this->_create_vertex_descriptor());
    }

    [[nodiscard]] gl_attr_force_inline auto neighbors(vertex_type vertex) const {
        return this->neighbors(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline auto neighbor_ids(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.neighbor_ids(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto neighbor_ids(vertex_type vertex) const {
        return this->neighbor_ids(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline auto predecessors(const id_type vertex_id) const {
        return this->predecessor_ids(vertex_id)
             | std::views::transform(this->_create_vertex_descriptor());
    }

    [[nodiscard]] gl_attr_force_inline auto predecessors(vertex_type vertex) const {
        return this->predecessors(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline auto predecessor_ids(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.predecessor_ids(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto predecessor_ids(vertex_type vertex) const {
        return this->predecessor_ids(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline auto successors(const id_type vertex_id) const {
        return this->successor_ids(vertex_id)
             | std::views::transform(this->_create_vertex_descriptor());
    }

    [[nodiscard]] gl_attr_force_inline auto successors(vertex_type vertex) const {
        return this->successors(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline auto successor_ids(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.successor_ids(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto successor_ids(vertex_type vertex) const {
        return this->successor_ids(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline vertex_properties_type& vertex_properties(const id_type id
    ) const
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        this->_verify_vertex_id(id);
        return this->_vertex_properties[id];
    }

    [[nodiscard]] gl_attr_force_inline auto vertex_properties_map() const noexcept
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        return std::views::all(this->_vertex_properties);
    }

    // --- degree getters ---

    [[nodiscard]] gl_attr_force_inline size_type degree(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.degree(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type degree(vertex_type vertex) const {
        return this->degree(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> degree_map() const {
        return this->_impl.degree_map();
    }

    [[nodiscard]] gl_attr_force_inline size_type in_degree(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.in_degree(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type in_degree(vertex_type vertex) const {
        return this->in_degree(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> in_degree_map() const {
        return this->_impl.in_degree_map();
    }

    [[nodiscard]] gl_attr_force_inline size_type out_degree(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.out_degree(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type out_degree(vertex_type vertex) const {
        return this->out_degree(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> out_degree_map() const {
        return this->_impl.out_degree_map();
    }

    // --- edge modifiers ---

    edge_type add_edge(const id_type source_id, const id_type target_id) {
        this->_verify_vertex_id(source_id);
        this->_verify_vertex_id(target_id);

        const auto new_edge_id = static_cast<id_type>(this->_n_edges++);
        this->_impl.add_edge(new_edge_id, source_id, target_id);

        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return edge_type{
                new_edge_id, source_id, target_id, this->_edge_properties.emplace_back()
            };
        else
            return edge_type{new_edge_id, source_id, target_id};
    }

    edge_type add_edge_with(
        const id_type source_id, const id_type target_id, edge_properties_type properties
    )
    requires(traits::c_non_empty_properties<edge_properties_type>)
    {
        this->_verify_vertex_id(source_id);
        this->_verify_vertex_id(target_id);

        const auto new_edge_id = static_cast<id_type>(this->_n_edges++);
        this->_impl.add_edge(new_edge_id, source_id, target_id);

        return edge_type{
            new_edge_id,
            source_id,
            target_id,
            this->_edge_properties.emplace_back(std::move(properties))
        };
    }

    // clang-format off
    // gl_attr_force_inline misplacement

    gl_attr_force_inline const edge_type add_edge(vertex_type source, vertex_type target) {
        return this->add_edge(source.id(), target.id());
    }

    gl_attr_force_inline const edge_type add_edge_with(
        vertex_type source, vertex_type target, const edge_properties_type& properties
    )
    requires(traits::c_non_empty_properties<edge_properties_type>)
    {
        return this->add_edge_with(source.id(), target.id(), properties);
    }

    // clang-format on

    void add_edges_from(
        const id_type source_id, const traits::c_sized_range_of<id_type> auto& target_id_rng
    ) {
        this->_verify_vertex_id(source_id);
        for (auto target_id : target_id_rng)
            this->_verify_vertex_id(target_id);

        const auto prev_n_edges = this->_n_edges;
        this->_n_edges += std::ranges::size(target_id_rng);
        this->_impl.add_edges_from(
            std::views::iota(static_cast<id_type>(prev_n_edges), this->_n_edges),
            source_id,
            target_id_rng
        );

        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            this->_edge_properties.resize(this->_n_edges);
    }

    void add_edges_from(
        vertex_type source, const traits::c_sized_range_of<vertex_type> auto& target_rng
    ) {
        this->_verify_vertex_id(source.id());
        for (auto target : target_rng)
            this->_verify_vertex_id(target.id());

        const auto prev_n_edges = this->_n_edges;
        this->_n_edges += std::ranges::size(target_rng);
        this->_impl.add_edges_from(
            std::views::iota(static_cast<id_type>(prev_n_edges), this->_n_edges),
            source.id(),
            target_rng | std::views::transform(&vertex_type::id)
        );


        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            this->_edge_properties.resize(this->_n_edges);
    }

    void remove_edge(const edge_type& edge) {
        this->_verify_edge(edge);
        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            this->_edge_properties.erase(this->_edge_properties.begin() + edge.id());
        this->_impl.remove_edge(edge);
        this->_n_edges--;
    }

    void remove_edges(const traits::c_range_of<edge_type> auto& edges) {
        const auto removed_edge_ids = this->_impl.remove_edges(edges);
        this->_n_edges -= removed_edge_ids.size();

        if constexpr (traits::c_non_empty_properties<edge_properties_type>) {
            // IDs are sorted and do not contain duplicates
            for (auto edge_id : std::views::reverse(removed_edge_ids))
                this->_edge_properties.erase(this->_edge_properties.begin() + edge_id);
        }
    }

    // --- edge getters ---

    [[nodiscard]] gl_attr_force_inline auto edge_ids() const noexcept {
        return std::views::iota(initial_id_v<id_type>, this->_n_edges);
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(const edge_type& edge) const {
        return this->_impl.has_edge(edge);
    }

    [[nodiscard]] bool has_edge(const id_type source_id, const id_type target_id) const {
        this->_verify_vertex_id(source_id);
        this->_verify_vertex_id(target_id);
        return this->_impl.has_edge(source_id, target_id);
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(vertex_type source, vertex_type target) const {
        return this->has_edge(source.id(), target.id());
    }

    [[nodiscard]] std::optional<edge_type> edge(const id_type source_id, const id_type target_id)
        const {
        this->_verify_vertex_id(source_id);
        this->_verify_vertex_id(target_id);

        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return this->_impl.edge(source_id, target_id, this->_edge_properties);
        else
            return this->_impl.edge(source_id, target_id);
    }

    [[nodiscard]] gl_attr_force_inline std::optional<edge_type> edge(
        vertex_type source, vertex_type target
    ) const {
        return this->edge(source.id(), target.id());
    }

    [[nodiscard]] inline std::vector<edge_type> edges(
        const id_type source_id, const id_type target_id
    ) const {
        this->_verify_vertex_id(source_id);
        this->_verify_vertex_id(target_id);

        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return this->_impl.edges(source_id, target_id, this->_edge_properties);
        else
            return this->_impl.edges(source_id, target_id);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<edge_type> edges(
        vertex_type source, vertex_type target
    ) const {
        return this->edges(source.id(), target.id());
    }

    [[nodiscard]] inline auto incident_edges(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return this->_impl.incident_edges(vertex_id, this->_edge_properties);
        else
            return this->_impl.incident_edges(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_edges(vertex_type vertex) const {
        return this->incident_edges(vertex.id());
    }

    [[nodiscard]] inline auto in_edges(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return this->_impl.in_edges(vertex_id, this->_edge_properties);
        else
            return this->_impl.in_edges(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto in_edges(vertex_type vertex) const {
        return this->in_edges(vertex.id());
    }

    [[nodiscard]] inline auto out_edges(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return this->_impl.out_edges(vertex_id, this->_edge_properties);
        else
            return this->_impl.out_edges(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto out_edges(vertex_type vertex) const {
        return this->out_edges(vertex.id());
    }

    [[nodiscard]] edge_properties_type& edge_properties(const id_type id) const
    requires(traits::c_non_empty_properties<edge_properties_type>)
    {
        if (id >= this->_n_edges)
            throw std::out_of_range(std::format("Got invalid edge id [{}]", id));

        return this->_edge_properties[id];
    }

    [[nodiscard]] gl_attr_force_inline auto edge_properties_map() const noexcept
    requires(traits::c_non_empty_properties<edge_properties_type>)
    {
        return std::views::all(this->_edge_properties);
    }

    // --- adjacency and incidence methods ---

    [[nodiscard]] gl_attr_force_inline bool are_adjacent(
        const id_type source_id, const id_type target_id
    ) const {
        if constexpr (traits::c_undirected_graph<graph>)
            return this->has_edge(source_id, target_id);
        else
            return this->has_edge(source_id, target_id) or this->has_edge(target_id, source_id);
    }

    [[nodiscard]] gl_attr_force_inline bool are_adjacent(vertex_type source, vertex_type target)
        const {
        return this->are_adjacent(source.id(), target.id());
    }

    [[nodiscard]] bool are_adjacent(const edge_type& edge_1, const edge_type& edge_2) const {
        this->_verify_edge(edge_1);
        this->_verify_edge(edge_2);

        if (edge_1.id() == edge_2.id())
            return false;
        return edge_1.is_incident_with(edge_2.source()) or edge_1.is_incident_with(edge_2.target());
    }

    [[nodiscard]] bool are_incident(vertex_type vertex, const edge_type& edge) const {
        this->_verify_vertex_id(vertex.id());
        this->_verify_edge(edge);
        return edge.is_incident_with(vertex.id());
    }

    [[nodiscard]] gl_attr_force_inline bool are_incident(const edge_type& edge, vertex_type vertex)
        const {
        return this->are_incident(vertex, edge);
    }

    // --- comparison ---

    [[nodiscard]] friend bool operator==(const graph& lhs, const graph& rhs) noexcept {
        if (lhs._n_vertices != rhs._n_vertices or lhs._n_edges != rhs._n_edges)
            return false;

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            if (lhs._vertex_properties != rhs._vertex_properties)
                return false;

        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            if (lhs._edge_properties != rhs._edge_properties)
                return false;

        return lhs._impl == rhs._impl;
    }

    // --- stream operators ---

    friend std::ostream& operator<<(std::ostream& os, const graph& g) {
        using io::detail::option_bit;

        if (io::is_option_set(os, option_bit::spec_fmt))
            return g._gsf_write(os);

        if (io::is_option_set(os, option_bit::verbose))
            return g._verbose_write(os);
        else
            return g._concise_write(os);
    }

    friend gl_attr_force_inline std::istream& operator>>(std::istream& is, graph& g) {
        return g._gsf_read(is);
    }

    // --- friend declarations ---

    template <traits::c_graph Graph>
    friend Graph clone(const Graph& source);

    template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph Graph>
    friend auto to(Graph&& source);

    template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph_impl_tag SourceImplTag>
    friend struct detail::to_impl;

private:
    using fmt_traits = io::detail::graph_fmt_traits<directional_tag>;

    graph(const graph& other)
    : _n_vertices{other._n_vertices},
      _n_edges{other._n_edges},
      _impl{other._impl},
      _vertex_properties{other._vertex_properties},
      _edge_properties{other._edge_properties} {}

    // --- element validation ---

    gl_attr_force_inline void _verify_vertex_id(const id_type vertex_id) const {
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

    // --- vertex modifiers ---

    void _remove_vertex_impl(const id_type vertex_id) {
        const auto removed_edge_ids = this->_impl.remove_vertex(vertex_id);
        this->_n_vertices--;
        this->_n_edges -= removed_edge_ids.size();

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.erase(this->_vertex_properties.begin() + vertex_id);

        if constexpr (traits::c_non_empty_properties<edge_properties_type>) {
            // IDs are sorted and do not contain duplicates
            for (auto edge_id : std::views::reverse(removed_edge_ids))
                this->_edge_properties.erase(this->_edge_properties.begin() + edge_id);
        }
    }

    // --- transformations ---

    gl_attr_force_inline auto _create_vertex_descriptor() const noexcept
    requires(traits::c_empty_properties<vertex_properties_type>)
    {
        return [](const id_type id) { return vertex_type{id}; };
    }

    gl_attr_force_inline auto _create_vertex_descriptor() const noexcept
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        return [&pmap = this->_vertex_properties](const id_type id) {
            return vertex_type{id, pmap[to_idx(id)]};
        };
    }

    // --- I/O utility ---

    struct concise_target_formatter {
        edge_type edge;
        id_type src_id;
        bool with_props;

        friend std::ostream& operator<<(std::ostream& os, const concise_target_formatter& proxy) {
            os << proxy.edge.other(proxy.src_id);
            if constexpr (traits::c_writable<edge_properties_type>)
                if (proxy.with_props)
                    os << '[' << proxy.edge.properties() << ']';

            return os;
        }
    };

    std::ostream& _verbose_write(std::ostream& os) const {
        os << "type: " << fmt_traits::type << ", |V| = " << this->_n_vertices
           << ", |E| = " << this->_n_edges << '\n';
        for (auto vertex : this->vertices()) {
            os << "- " << vertex << "\n  " << fmt_traits::out_edges << ":\n";
            for (const auto& edge : this->out_edges(vertex.id()))
                os << "\t- " << edge << '\n';
        }
        return os;
    }

    std::ostream& _concise_write(std::ostream& os) const {
        using enum io::detail::option_bit;

        for (auto src : this->vertices()) {
            auto tgts = std::views::transform(
                this->out_edges(src.id()),
                [src_id = src.id(),
                 with_props = io::is_option_set(os, with_connection_properties)](const auto& edge) {
                    return concise_target_formatter{edge, src_id, with_props};
                }
            );
            os << src << " : " << io::range_formatter(tgts) << '\n';
        }

        return os;
    }

    std::ostream& _gsf_write(std::ostream& os) const {
        using enum io::detail::option_bit;

        const bool with_v_props = io::is_option_set(os, with_vertex_properties);
        const bool with_e_props = io::is_option_set(os, with_connection_properties);

        // print graph metadata
        os << traits::c_directed_edge<edge_type> << ' ' << this->_n_vertices << ' '
           << this->_n_edges << ' ' << static_cast<int>(with_v_props) << ' '
           << static_cast<int>(with_e_props) << '\n';

        if constexpr (traits::c_writable<vertex_properties_type>)
            if (with_v_props)
                for (auto vertex : this->vertices())
                    os << vertex.properties() << '\n';

        if constexpr (traits::c_writable<edge_properties_type>) {
            if (with_e_props) {
                const auto print_out_edges = [this, &os](const id_type vertex_id) {
                    for (const auto& edge : this->out_edges(vertex_id)) {
                        if constexpr (std::same_as<directional_tag, undirected_t>)
                            if (edge.other(vertex_id) > vertex_id)
                                continue; // deduplicate edges
                        os << edge.source() << ' ' << edge.target() << ' ' << edge.properties()
                           << '\n';
                    }
                };

                for (auto vertex_id : this->vertex_ids())
                    print_out_edges(vertex_id);

                return os;
            }
        }

        const auto print_out_edges = [this, &os](const id_type vertex_id) {
            for (const auto& edge : this->out_edges(vertex_id)) {
                if constexpr (std::same_as<directional_tag, undirected_t>)
                    if (edge.other(vertex_id) > vertex_id)
                        continue; // deduplicate edges
                os << edge.source() << ' ' << edge.target() << '\n';
            }
        };

        for (auto vertex_id : this->vertex_ids())
            print_out_edges(vertex_id);

        return os;
    }

    std::istream& _gsf_read(std::istream& is) {
        using fmt_traits = io::detail::graph_fmt_traits<directional_tag>;

        int dir_discr;
        is >> dir_discr;

        if (dir_discr != fmt_traits::discriminator)
            throw std::ios_base::failure(std::format(
                "Invalid hypergraph specification: directional specifier {} does not match "
                "expected {}",
                dir_discr,
                fmt_traits::discriminator
            ));

        // read graph metadata
        id_type n_vertices, n_edges;
        is >> n_vertices >> n_edges;

        bool with_vertex_properties, with_edge_properties;
        is >> with_vertex_properties >> with_edge_properties;

        if (with_vertex_properties) {
            if constexpr (not traits::c_readable<vertex_properties_type>) {
                throw std::ios_base::failure(
                    "Invalid graph specification: vertex_properties=true "
                    "when vertex_properties_type is not readable"
                );
            }
            else {
                // read vertex properties and use them to initialze the vertices
                std::vector<vertex_properties_type> vertex_properties(n_vertices);
                for (auto i = 0uz; i < n_vertices; ++i)
                    is >> vertex_properties[i];
                this->add_vertices_with(vertex_properties);
            }
        }
        else {
            // initialize the vertices with default (or none) properties
            this->add_vertices(n_vertices);
        }

        if (with_edge_properties) {
            if constexpr (not traits::c_readable<edge_properties_type>) {
                throw std::ios_base::failure(
                    "Invalid graph specification: edge_properties=true "
                    "when edge_properties_type is not readable"
                );
            }
            else {
                // read edges with their properties
                id_type source_id, target_id;
                edge_properties_type properties;

                for (auto _ = 0uz; _ < n_edges; ++_) {
                    is >> source_id >> target_id >> properties;
                    this->add_edge_with(source_id, target_id, properties);
                }
            }
        }
        else {
            // read the edges
            id_type source_id, target_id;

            for (auto _ = 0uz; _ < n_edges; ++_) {
                is >> source_id >> target_id;
                this->add_edge(source_id, target_id);
            }
        }

        return is;
    }

    // --- data members ---

    size_type _n_vertices = 0uz;
    size_type _n_edges = 0uz;

    implementation_type _impl{};

    /// @todo Replace mutability with proper const-correct getter overloads to ensure thread safety guarantees associated with the const qualifier
    [[no_unique_address]] mutable vertex_properties_map_type _vertex_properties{};

    /// @todo Replace mutability with proper const-correct getter overloads to ensure thread safety guarantees associated with the const qualifier
    [[no_unique_address]] mutable edge_properties_map_type _edge_properties{};
};

// --- general graph utility ---

template <traits::c_graph Graph>
[[nodiscard]] Graph clone(const Graph& source) {
    return Graph(source);
}

template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_impl_tag ImplTag = impl::list_t,
    traits::c_id_type IdType = default_id_type>
using directed_graph =
    graph<directed_graph_traits<VertexProperties, EdgeProperties, ImplTag, IdType>>;

template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_impl_tag ImplTag = impl::list_t,
    traits::c_id_type IdType = default_id_type>
using undirected_graph =
    graph<undirected_graph_traits<VertexProperties, EdgeProperties, ImplTag, IdType>>;

template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using list_graph =
    graph<list_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType>>;

template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using matrix_graph =
    graph<matrix_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType>>;

template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using flat_list_graph =
    graph<flat_list_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType>>;

template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using flat_matrix_graph =
    graph<flat_matrix_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType>>;

// --- vertex distance utility ---

using default_vertex_distance_type = std::int64_t;

template <traits::c_graph GraphType>
struct vertex_distance {
    using type = default_vertex_distance_type;
};

template <traits::c_graph GraphType>
requires(traits::c_weight_properties_type<typename GraphType::edge_properties_type>)
struct vertex_distance<GraphType> {
    using type = typename GraphType::edge_properties_type::weight_type;
};

template <traits::c_graph GraphType>
using vertex_distance_type = typename vertex_distance<GraphType>::type;

template <traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline vertex_distance_type<GraphType> get_weight(
    const typename GraphType::edge_type& edge
) {
    if constexpr (traits::c_weight_properties_type<typename GraphType::edge_properties_type>)
        return edge.properties().weight;
    else
        return static_cast<default_vertex_distance_type>(1ll);
}

} // namespace gl
