// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/graph.hpp
/// @brief Defines the core generic `graph` container and its associated traits and utilities.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/constants.hpp"
#include "gl/directional_tags.hpp"
#include "gl/graph_traits.hpp"
#include "gl/io/graph_fmt_traits.hpp"
#include "gl/io/options.hpp"
#include "gl/io/options_manip.hpp"
#include "gl/repr_tags.hpp"
#include "gl/traits.hpp"
#include "gl/util/ranges.hpp"

#include <set>
#include <stdexcept>

namespace gl {

template <traits::c_instantiation_of<graph_traits> GraphTraits = graph_traits<>>
class graph;

// --- general graph utility ---

namespace traits {

/// @ingroup GL-Traits
/// @brief Concept checking if a type is an instantiation of the generic @ref "gl::graph" graph class.
template <typename G>
concept c_graph = c_instantiation_of<std::remove_cvref_t<G>, graph>;

/// @ingroup GL-Traits
/// @brief Concept checking if a type is a mutable instantiation of the generic @ref "gl::graph" graph class.
template <typename G>
concept c_mut_graph = c_graph<G> and not std::is_const_v<std::remove_reference_t<G>>;

template <c_graph G>
using graph_val_t = std::remove_cvref_t<G>;

/// @ingroup GL-Traits
/// @brief Concept checking if a graph is directed.
/// @see gl::directed_t "directed_t" : For the directional tag used to specify directed graph configuration.
template <typename G>
concept c_directed_graph =
    c_graph<G> and std::same_as<typename graph_val_t<G>::directional_tag, directed_t>;

/// @ingroup GL-Traits
/// @brief Concept checking if a graph is undirected.
/// @see gl::undirected_t "undirected_t" : For the directional tag used to specify undirected graph configuration.
template <typename G>
concept c_undirected_graph =
    c_graph<G> and std::same_as<typename graph_val_t<G>::directional_tag, undirected_t>;

/// @ingroup GL-Traits
/// @brief Concept checking if a graph utilizes the standard adjacency list representation.
/// @see gl::repr::list_t "list_t" : For the representation tag used to specify the standard adjacency list representation.
template <typename G>
concept c_list_graph =
    c_graph<G> and std::same_as<typename graph_val_t<G>::representation_tag, repr::list_t>;

/// @ingroup GL-Traits
/// @brief Concept checking if a graph utilizes the flattened adjacency list representation.
/// @see gl::repr::flat_list_t "flat_list_t" : For the representation tag used to specify the flattened adjacency list representation.
template <typename G>
concept c_flat_list_graph =
    c_graph<G> and std::same_as<typename graph_val_t<G>::representation_tag, repr::flat_list_t>;

/// @ingroup GL-Traits
/// @brief Concept checking if a graph utilizes any list-based adjacency representation.
/// ### See Also
/// - @ref gl::repr::list_t "list_t" : For the representation tag used to specify the standard adjacency list representation.
/// - @ref gl::repr::flat_list_t "flat_list_t" : For the representation tag used to specify the flattened adjacency list representation.
template <typename G>
concept c_adjacency_list_graph = c_list_graph<G> or c_flat_list_graph<G>;

/// @ingroup GL-Traits
/// @brief Concept checking if a graph utilizes the standard adjacency matrix representation.
/// @see gl::repr::matrix_t "matrix_t" : For the representation tag used to specify the standard adjacency matrix representation.
template <typename G>
concept c_matrix_graph =
    c_graph<G> and std::same_as<typename graph_val_t<G>::representation_tag, repr::matrix_t>;

/// @ingroup GL-Traits
/// @brief Concept checking if a graph utilizes the flattened adjacency matrix representation.
/// @see gl::repr::flat_matrix_t "flat_matrix_t" : For the representation tag used to specify the flattened adjacency matrix representation.
template <typename G>
concept c_flat_matrix_graph =
    c_graph<G> and std::same_as<typename graph_val_t<G>::representation_tag, repr::flat_matrix_t>;

/// @ingroup GL-Traits
/// @brief Concept checking if a graph utilizes any matrix-based adjacency representation.
/// ### See Also
/// - @ref gl::repr::matrix_t "matrix_t" : For the representation tag used to specify the standard adjacency matrix representation.
/// - @ref gl::repr::flat_matrix_t "flat_matrix_t" : For the representation tag used to specify the flattened adjacency matrix representation.
template <typename G>
concept c_adjacency_matrix_graph = c_matrix_graph<G> or c_flat_matrix_graph<G>;

/// @ingroup GL-Traits
/// @brief Concept checking if a type is a mutable or immutable vertex descriptor associated with the given graph.
/// @tparam V The type of the vertex descriptor.
/// @tparam G The type of the graph.
template <typename V, typename G>
concept c_graph_vertex =
    c_graph<G>
    and c_one_of<
        std::remove_cvref_t<V>,
        typename graph_val_t<G>::vertex_type,
        typename graph_val_t<G>::const_vertex_type>;

/// @ingroup GL-Traits
/// @brief Concept checking if a type is a mutable or immutable edge descriptor associated with the given graph.
/// @tparam E The type of the edge descriptor.
/// @tparam G The type of the graph.
template <typename E, typename G>
concept c_graph_edge =
    c_graph<G>
    and c_one_of<
        std::remove_cvref_t<E>,
        typename graph_val_t<G>::edge_type,
        typename graph_val_t<G>::const_edge_type>;

} // namespace traits

template <traits::c_graph G>
using id_t = typename traits::graph_val_t<G>::id_type;

template <traits::c_graph G>
using vertex_t = std::conditional_t<
    std::is_const_v<std::remove_reference_t<G>>,
    typename traits::graph_val_t<G>::const_vertex_type,
    typename traits::graph_val_t<G>::vertex_type>;

template <traits::c_graph G>
using vertex_properties_t = std::conditional_t<
    std::is_const_v<std::remove_reference_t<G>>,
    const typename traits::graph_val_t<G>::vertex_properties_type,
    typename traits::graph_val_t<G>::vertex_properties_type>;

template <traits::c_graph G>
using edge_t = std::conditional_t<
    std::is_const_v<std::remove_reference_t<G>>,
    typename traits::graph_val_t<G>::const_edge_type,
    typename traits::graph_val_t<G>::edge_type>;

template <traits::c_graph G>
using edge_properties_t = std::conditional_t<
    std::is_const_v<std::remove_reference_t<G>>,
    const typename traits::graph_val_t<G>::edge_properties_type,
    typename traits::graph_val_t<G>::edge_properties_type>;

template <traits::c_graph Graph>
[[nodiscard]] Graph clone(const Graph& source);

template <traits::c_graph_repr_tag TargetImplTag, traits::c_graph Graph>
requires(not std::is_lvalue_reference_v<Graph>)
[[nodiscard]] auto to(Graph&& source);

namespace detail {

template <traits::c_graph_repr_tag TargetImplTag, traits::c_graph_repr_tag SourceImplTag>
struct to_impl;

} // namespace detail

/// @ingroup GL-Core
/// @brief The generic graph container using a policy-based architecture.
///
/// This class relies on the provided `GraphTraits` to determine its behavior, element
/// property types, and the underlying memory representation. It exposes a unified
/// API for adding, removing, and iterating over vertices and edges regardless of the backend.
///
/// ### Key Features
/// - **Policy-based design**: Behavior and representation are determined by `GraphTraits`.
/// - **Zero-cost Abstractions**: Core query logic is resolved at compile time through representation tags and static dispatch, removing unnecessary overhead.
/// - **Configurable directionality**: Support for both directed and undirected graphs.
/// - **Multiple representations**: Choose the underlying memory model that best suits your algorithmic and cache-locality needs:
///   - @ref gl::repr::list_t "list_t": Standard adjacency list.
///   - @ref gl::repr::flat_list_t "flat_list_t": Flattened adjacency list.
///   - @ref gl::repr::matrix_t "matrix_t": Standard adjacency matrix.
///   - @ref gl::repr::flat_matrix_t "flat_matrix_t": Flattened adjacency matrix.
/// - **Property support**: Vertices and edges can carry arbitrary properties.
/// - **Unified API**: Consistent interface regardless of the underlying representation.
/// - **Standard Range Support**: Exposes lightweight views compliant with C++20 `std::ranges`, enabling functional-style iteration and algorithms.
///
/// ### Basic Definitions
/// A graph \f$G = (V, E)\f$ consists of a set of vertices \f$V\f$ and a set of edges \f$E\f$.
///
/// - For undirected graphs, edges are unordered pairs \f$\{u, v\}\f$ where \f$u, v \in V\f$.
/// - For directed graphs, edges are ordered pairs \f$(u, v)\f$ where \f$u, v \in V\f$.
///
/// ### Example Usage
/// ```cpp
/// #include <gl/graph.hpp>
///
/// #include <iostream>
///
/// int main() {
///     gl::directed_graph<> g; // (1)!
///
///     auto v0 = g.add_vertex(); // (2)!
///     auto v1 = g.add_vertex();
///     auto v2 = g.add_vertex();
///
///     auto e01 = g.add_edge(v0, v1); // (3)!
///     auto e12 = g.add_edge(v1, v2);
///     auto e20 = g.add_edge(v2, v0);
///
///     std::cout << "Vertices: " << g.n_vertices() << '\n'; // (4)!
///     std::cout << "Edges: " << g.n_edges() << '\n';
///
///     for (auto neighbor : g.neighbors(v0)) // (5)!
///         process(neighbor);
///
///     std::cout << "Topology:\n" << g << '\n'; // (6)!
///
///     return 0;
/// }
/// ```
///
/// 1\. Create a directed graph with no properties.
///
/// 2\. Add vertices to the graph. **NOTE:** This is safe because the graph has no vertex properties and the `add_vertex` operation does not invalidate IDs.
///
/// 3\. Add edges to the graph. **NOTE:** This is safe because the graph has no edge properties and the `add_edge` operation does not invalidate IDs.
///
/// 4\. Query the graph's properties.
///
/// 5\. Iterate over neighbors of `v0`.
///
/// 6\. Utilize the builtin I/O stream support of the `graph` class to print its topology to the console.
///
/// ### API Design: IDs vs. Descriptors
/// The `graph` class exposes a dual API to accommodate different performance and ergonomic needs:
///
/// - **Inputs**: Most query methods are overloaded to accept either a raw `id_type` or a vertex/edge descriptor types. They are functionally identical.
/// - **Outputs**: Methods ending in `_ids` (e.g., `neighbor_ids`) return views of raw integral IDs. Methods without this suffix (e.g., `neighbors`) automatically map those IDs to the proper descriptor objects.
/// - **Performance**: Descriptor-returning methods incur a slight overhead if the graph utilizes properties, as the property reference must be fetched and bound to each descriptor. If you only need topology, prefer the `_ids` variants.
///
/// ### Descriptor Invalidation Behavior
/// The graph maintains the following invalidation semantics:
///
/// - **Vertex addition**: Does not invalidate vertex IDs. However, property references stored in existing vertex descriptors may be invalidated. Has no effect on edge descriptors.
/// - **Vertex removal**: May invalidate both vertex and edge descriptors, IDs, and property references.
/// - **Edge addition**: Does not invalidate vertex or edge IDs. However, property references stored in edge descriptors may be invalidated.
/// - **Edge removal**: Invalidates edge descriptors, IDs, and property references. Has no effect on vertex descriptors.
/// - **Property access**: References to vertex or edge properties obtained from the map may be invalidated by modifications to the graph structure.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | GraphTraits | Traits struct specifying the behavior and representation of the graph. | An instantiation of @ref gl::graph_traits "graph_traits" |
///
/// ### See Also
/// - @ref gl::directed_graph "directed_graph" : Convenience alias for directed graphs.
/// - @ref gl::undirected_graph "undirected_graph" : Convenience alias for undirected graphs.
/// - @ref gl::clone "clone" : Create a deep copy of a graph.
/// - @ref gl::to "to" : Convert a graph to a different representation.
///
/// > [!IMPORTANT] Copy Semantics
/// >
/// > `graph` supports move semantics but disables copy assignment to prevent accidental expensive copies.
/// > Use @ref gl::clone "clone" to explicitly copy a graph.
///
/// > [!WARNING] Const Correctness & Properties (API Note)
/// >
/// > Currently, a `const` graph guarantees **structural immutability** (vertices and edges cannot be added or removed).
/// > However, vertex and edge property maps are internally treated as `mutable`. This means that property payloads
/// > can still be modified through a `const graph&`. Strict const-correct overloads for property access are planned
/// > for a future release. Proceed with caution in multi-threaded contexts.
template <traits::c_instantiation_of<graph_traits> GraphTraits>
class graph final {
public:
    /// @brief Traits type specifying the behavior and representation.
    using traits_type = GraphTraits;

    /// @brief Type tag specifying the directionality of the graph.
    using directional_tag = typename traits_type::directional_tag;
    /// @brief Type tag indicating the underlying representation model.
    using representation_tag = typename traits_type::representation_tag;

    /// @brief Integral type used to identify vertices and edges.
    using id_type = typename traits_type::id_type;

    /// @brief The descriptor type representing a vertex of the graph.
    using vertex_type = typename traits_type::vertex_type;
    /// @brief The descriptor type representing an immutable vertex of the graph.
    using const_vertex_type = typename traits_type::const_vertex_type;
    /// @brief Type representing the properties attached to a vertex.
    using vertex_properties_type = typename traits_type::vertex_properties_type;

    /// @brief The descriptor type representing an edge of the graph.
    using edge_type = typename traits_type::edge_type;
    /// @brief The descriptor type representing an immutable edge of the graph.
    using const_edge_type = typename traits_type::const_edge_type;
    /// @brief Type representing the properties attached to an edge.
    using edge_properties_type = typename traits_type::edge_properties_type;

private:
    using implementation_type = typename representation_tag::template type<traits_type>;
    friend implementation_type;

    using vertex_properties_map_type = std::conditional_t<
        traits::c_empty_properties<vertex_properties_type>,
        empty_properties_map,
        std::vector<vertex_properties_type>>;

    using edge_properties_map_type = std::conditional_t<
        traits::c_empty_properties<edge_properties_type>,
        empty_properties_map,
        std::vector<edge_properties_type>>;

public:
    /// @brief Default constructor creates an empty graph.
    graph() = default;

    /// @brief Constructs an empty graph with a pre-allocated number of vertices.
    /// @param n_vertices The initial number of vertices to allocate space for.
    explicit graph(const size_type n_vertices) : _n_vertices(n_vertices), _impl(n_vertices) {
        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.resize(n_vertices);
    }

    /// @brief Default move constructor.
    graph(graph&&) noexcept = default;
    /// @brief Default move assignment operator.
    graph& operator=(graph&&) noexcept = default;

    /// @brief Default destructor.
    ~graph() = default;

    /// @brief Graph copy assignment is disabled to avoid accidental copies. Use @ref gl::clone "clone" instead.
    graph& operator=(const graph&) = delete;

    // --- size methods ---

    /// @brief Returns the total number of vertices in the graph.
    /// @return The vertex count: $|V|$.
    [[nodiscard]] gl_attr_force_inline size_type n_vertices() const noexcept {
        return this->_n_vertices;
    }

    /// @brief Returns the total number of edges in the graph.
    /// @return The edge count: $|E|$.
    [[nodiscard]] gl_attr_force_inline size_type n_edges() const noexcept {
        return this->_n_edges;
    }

    // --- vertex modifiers ---

    /// @brief Adds a new, default-initialized vertex to the graph.
    /// @return A descriptor of the newly created vertex.
    /// @copydetails detail::graph_doc_anchors::add_vertex_note()
    vertex_type add_vertex() {
        this->_impl.add_vertex();
        const auto new_vertex_id = static_cast<id_type>(this->_n_vertices++);

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_type{new_vertex_id, this->_vertex_properties.emplace_back()};
        else
            return vertex_type{new_vertex_id};
    }

    /// @brief Adds a new vertex with the given properties to the graph.
    /// @param properties The property payload for the new vertex.
    /// @return A descriptor of the newly created vertex.
    /// @copydetails detail::graph_doc_anchors::add_vertex_note()
    vertex_type add_vertex_with(vertex_properties_type properties)
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        this->_impl.add_vertex();
        return vertex_type{
            static_cast<id_type>(this->_n_vertices++),
            this->_vertex_properties.emplace_back(std::move(properties))
        };
    }

    /// @brief Adds a specified number of default-initialized vertices to the graph.
    /// @param n The number of vertices to add.
    /// @copydetails detail::graph_doc_anchors::add_vertex_note()
    void add_vertices(const size_type n) {
        this->_impl.add_vertices(n);
        this->_n_vertices += n;

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.resize(this->_n_vertices);
    }

    /// @brief Adds multiple vertices based on a range of property payloads.
    /// @param properties_rng A range of properties to initialize the new vertices with.
    /// @copydetails detail::graph_doc_anchors::add_vertex_note()
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

    /// @brief Removes a vertex by its ID, removing all associated incident edges.
    /// @param vertex_id The ID of the vertex to remove.
    /// @throws std::invalid_argument If the ID is invalid.
    /// @copydetails detail::graph_doc_anchors::remove_vertex_wrn()
    void remove_vertex(const id_type vertex_id) {
        this->_verify_vertex_id(vertex_id);
        this->_remove_vertex_impl(vertex_id);
    }

    /// @brief Removes a vertex using its descriptor.
    /// @param vertex The descriptor of the vertex to remove.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    /// @copydetails detail::graph_doc_anchors::remove_vertex_wrn()
    gl_attr_force_inline void remove_vertex(traits::c_graph_vertex<graph> auto vertex) {
        this->remove_vertex(vertex.id());
    }

    /// @brief Removes a range of vertices using their IDs.
    /// @param vertex_id_rng A forward range containing the IDs of vertices to remove.
    /// @throws std::invalid_argument If any vertex ID in the range is invalid.
    /// @copydetails detail::graph_doc_anchors::remove_vertex_wrn()
    void remove_vertices(const traits::c_forward_range_of<id_type> auto& vertex_id_rng) {
        // TODO: optimize
        // sorts the ids in a descending order and removes duplicate ids
        std::set<id_type, std::greater<>> vertex_id_set(
            std::ranges::begin(vertex_id_rng), std::ranges::end(vertex_id_rng)
        );
        if (not vertex_id_set.empty())
            this->_verify_vertex_id(*vertex_id_set.begin());

        for (auto vertex_id : vertex_id_set)
            this->_remove_vertex_impl(vertex_id);
    }

    /// @brief Removes a range of vertices using their descriptors.
    /// @tparam VertexRng A forward range type containing graph's vertex descriptors.
    /// @param vertex_rng A sized range containing the descriptors of vertices to remove.
    /// @throws std::invalid_argument If any vertex descriptor is invalid.
    /// @copydetails detail::graph_doc_anchors::remove_vertex_wrn()
    template <typename VertexRng>
    requires(traits::c_forward_range<VertexRng> and traits::c_graph_vertex<std::ranges::range_value_t<VertexRng>, graph>)
    gl_attr_force_inline void remove_vertices(const VertexRng& vertex_rng) {
        this->remove_vertices(
            vertex_rng | std::views::transform([](const auto& v) { return v.id(); })
        );
    }

    // --- vertex getters ---

    /// @brief Checks if a vertex with the given ID exists in the graph.
    /// @param vertex_id The ID to check.
    /// @return `true` if it exists, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool has_vertex(const id_type vertex_id) const {
        return vertex_id < this->_n_vertices;
    }

    /// @brief Checks if the given vertex descriptor is valid in the graph.
    /// @param vertex The vertex descriptor to check.
    /// @return `true` if it exists, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool has_vertex(traits::c_graph_vertex<graph> auto vertex
    ) const {
        return this->has_vertex(vertex.id());
    }

    /// @brief Returns a vertex descriptor bounds-checked by ID.
    /// @param vertex_id The ID of the vertex.
    /// @return The corresponding vertex descriptor (const or mutable).
    /// @throws std::invalid_argument If the ID is invalid.
    template <typename Self>
    [[nodiscard]] vertex_t<Self> vertex(this Self& self, const id_type vertex_id) {
        self._verify_vertex_id(vertex_id);
        return self.vertex_unchecked(vertex_id);
    }

    /// @brief Returns a vertex descriptor bounds-checked by ID (alias for `vertex`).
    /// @param vertex_id The ID of the vertex.
    /// @return The corresponding vertex descriptor.
    /// @throws std::invalid_argument If the ID is invalid.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline vertex_t<Self> at(this Self& self, const id_type vertex_id) {
        return self.vertex(vertex_id);
    }

    /// @brief Returns a vertex descriptor without bounds checking.
    /// @param vertex_id The ID of the vertex.
    /// @return The corresponding vertex descriptor.
    ///
    /// > [!WARNING] Undefined Behavior
    /// >
    /// > No bounds checking is performed. Passing an invalid ID results in Undefined Behavior.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline vertex_t<Self> vertex_unchecked(
        this Self& self, const id_type vertex_id
    ) {
        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_t<Self>{vertex_id, self._vertex_properties[vertex_id]};
        else
            return vertex_t<Self>{vertex_id};
    }

    /// @brief Returns a vertex descriptor without bounds checking (array access style).
    /// @param vertex_id The ID of the vertex.
    /// @return The corresponding vertex descriptor.
    ///
    /// > [!WARNING] Undefined Behavior
    /// >
    /// > No bounds checking is performed. Passing an invalid ID results in Undefined Behavior.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline vertex_t<Self> operator[](
        this Self& self, const id_type vertex_id
    ) noexcept {
        return self.vertex_unchecked(vertex_id);
    }

    /// @brief Returns a lazily evaluated view of all vertex descriptors in the graph.
    /// @return A view yielding descriptors for every vertex.
    [[nodiscard]] gl_attr_force_inline auto vertices(this auto& self) noexcept {
        return self.vertex_ids() | std::views::transform(self._create_vertex_descriptor());
    }

    /// @brief Returns a lazily evaluated view of all active vertex IDs in the graph.
    /// @return A view yielding all valid vertex IDs.
    [[nodiscard]] gl_attr_force_inline auto vertex_ids() const noexcept {
        return std::views::iota(initial_id_v<id_type>, this->_n_vertices);
    }

    /// @brief Retrieves the neighbor vertex descriptors for a specific vertex.
    /// @copydetails detail::graph_doc_anchors::neighbors()
    /// @param vertex_id The ID of the source vertex.
    /// @return A view of all adjacent vertex descriptors.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto neighbors(this auto& self, const id_type vertex_id) {
        return self.neighbor_ids(vertex_id)
             | std::views::transform(self._create_vertex_descriptor());
    }

    /// @brief Retrieves the neighbor vertex descriptors for a specific vertex.
    /// @copydetails detail::graph_doc_anchors::neighbors()
    /// @param vertex The source vertex descriptor.
    /// @return A view of all adjacent vertex descriptors.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto neighbors(
        this auto& self, traits::c_graph_vertex<graph> auto vertex
    ) {
        return self.neighbors(vertex.id());
    }

    /// @brief Retrieves the neighbor vertex IDs for a specific vertex.
    /// @copydetails detail::graph_doc_anchors::neighbors()
    /// @param vertex_id The ID of the source vertex.
    /// @return A view of all adjacent vertex IDs.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto neighbor_ids(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.neighbor_ids(vertex_id);
    }

    /// @brief Retrieves the neighbor vertex IDs for a specific vertex.
    /// @copydetails detail::graph_doc_anchors::neighbors()
    /// @param vertex The source vertex descriptor.
    /// @return A view of all adjacent vertex IDs.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto neighbor_ids(traits::c_graph_vertex<graph> auto vertex
    ) const {
        return this->neighbor_ids(vertex.id());
    }

    /// @brief Retrieves the predecessor vertex descriptors (incoming edges) for a vertex.
    /// @copydetails detail::graph_doc_anchors::predecessors()
    /// @param vertex_id The ID of the target vertex.
    /// @return A view of all predecessor vertex descriptors.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto predecessors(this auto& self, const id_type vertex_id) {
        return self.predecessor_ids(vertex_id)
             | std::views::transform(self._create_vertex_descriptor());
    }

    /// @brief Retrieves the predecessor vertex descriptors (incoming edges) for a vertex.
    /// @copydetails detail::graph_doc_anchors::predecessors()
    /// @param vertex The target vertex descriptor.
    /// @return A view of all predecessor vertex descriptors.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto predecessors(
        this auto& self, traits::c_graph_vertex<graph> auto vertex
    ) {
        return self.predecessors(vertex.id());
    }

    /// @brief Retrieves the predecessor vertex IDs for a vertex.
    /// @copydetails detail::graph_doc_anchors::predecessors()
    /// @param vertex_id The ID of the target vertex.
    /// @return A view of all predecessor vertex IDs.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto predecessor_ids(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.predecessor_ids(vertex_id);
    }

    /// @brief Retrieves the predecessor vertex IDs for a vertex.
    /// @copydetails detail::graph_doc_anchors::predecessors()
    /// @param vertex The target vertex descriptor.
    /// @return A view of all predecessor vertex IDs.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto predecessor_ids(
        traits::c_graph_vertex<graph> auto vertex
    ) const {
        return this->predecessor_ids(vertex.id());
    }

    /// @brief Retrieves the successor vertex descriptors (outgoing edges) for a vertex.
    /// @copydetails detail::graph_doc_anchors::successors()
    /// @param vertex_id The ID of the source vertex.
    /// @return A view of all successor vertex descriptors.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto successors(this auto& self, const id_type vertex_id) {
        return self.successor_ids(vertex_id)
             | std::views::transform(self._create_vertex_descriptor());
    }

    /// @brief Retrieves the successor vertex descriptors (outgoing edges) for a vertex.
    /// @copydetails detail::graph_doc_anchors::successors()
    /// @param vertex The source vertex descriptor.
    /// @return A view of all successor vertex descriptors.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto successors(
        this auto& self, traits::c_graph_vertex<graph> auto vertex
    ) {
        return self.successors(vertex.id());
    }

    /// @brief Retrieves the successor vertex IDs for a vertex.
    /// @copydetails detail::graph_doc_anchors::successors()
    /// @param vertex_id The ID of the source vertex.
    /// @return A view of all successor vertex IDs.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto successor_ids(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.successor_ids(vertex_id);
    }

    /// @brief Retrieves the successor vertex IDs for a vertex.
    /// @copydetails detail::graph_doc_anchors::successors()
    /// @param vertex The source vertex descriptor.
    /// @return A view of all successor vertex IDs.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto successor_ids(traits::c_graph_vertex<graph> auto vertex
    ) const {
        return this->successor_ids(vertex.id());
    }

    /// @brief Retrieves a mutable reference to a vertex's properties.
    /// @param id The ID of the vertex.
    /// @return A reference to the properties attached to the vertex.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline vertex_properties_t<Self>& vertex_properties(
        this Self& self, const id_type id
    )
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        self._verify_vertex_id(id);
        return self._vertex_properties[id];
    }

    /// @brief Retrieves a random-access view over all vertex properties in the graph.
    /// @return A view mapping each active vertex index to its property.
    [[nodiscard]] gl_attr_force_inline auto vertex_properties_map(this auto& self) noexcept
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        return std::views::all(self._vertex_properties);
    }

    // --- degree getters ---

    /// @brief Calculates the total degree of a vertex.
    /// @copydetails detail::graph_doc_anchors::degree()
    /// @param vertex_id The ID of the vertex.
    /// @return The total degree.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline size_type degree(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.degree(vertex_id);
    }

    /// @brief Calculates the total degree of a vertex.
    /// @copydetails detail::graph_doc_anchors::degree()
    /// @param vertex The vertex descriptor.
    /// @return The total degree.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline size_type degree(traits::c_graph_vertex<graph> auto vertex
    ) const {
        return this->degree(vertex.id());
    }

    /// @brief Returns a mapped array of total degrees for all vertices.
    /// @return A vector where the index aligns with the vertex ID containing its degree.
    [[nodiscard]] gl_attr_force_inline std::vector<size_type> degree_map() const {
        return this->_impl.degree_map();
    }

    /// @brief Calculates the in-degree (incoming edges) for a vertex.
    /// @copydetails detail::graph_doc_anchors::in_degree()
    /// @param vertex_id The ID of the vertex.
    /// @return The in-degree.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline size_type in_degree(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.in_degree(vertex_id);
    }

    /// @brief Calculates the in-degree (incoming edges) for a vertex.
    /// @copydetails detail::graph_doc_anchors::in_degree()
    /// @param vertex The vertex descriptor.
    /// @return The in-degree.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline size_type in_degree(traits::c_graph_vertex<graph> auto vertex
    ) const {
        return this->in_degree(vertex.id());
    }

    /// @brief Returns a mapped array of in-degrees for all vertices.
    /// @return A vector where the index aligns with the vertex ID containing its in-degree.
    [[nodiscard]] gl_attr_force_inline std::vector<size_type> in_degree_map() const {
        return this->_impl.in_degree_map();
    }

    /// @brief Calculates the out-degree (outgoing edges) for a vertex.
    /// @copydetails detail::graph_doc_anchors::out_degree()
    /// @param vertex_id The ID of the vertex.
    /// @return The out-degree.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline size_type out_degree(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.out_degree(vertex_id);
    }

    /// @brief Calculates the out-degree (outgoing edges) for a vertex.
    /// @copydetails detail::graph_doc_anchors::out_degree()
    /// @param vertex The vertex descriptor.
    /// @return The out-degree.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline size_type
    out_degree(traits::c_graph_vertex<graph> auto vertex) const {
        return this->out_degree(vertex.id());
    }

    /// @brief Returns a mapped array of out-degrees for all vertices.
    /// @return A vector where the index aligns with the vertex ID containing its out-degree.
    [[nodiscard]] gl_attr_force_inline std::vector<size_type> out_degree_map() const {
        return this->_impl.out_degree_map();
    }

    // --- edge modifiers ---

    /// @brief Creates an edge between a source and target vertex.
    /// @param source_id The ID of the source vertex.
    /// @param target_id The ID of the target vertex.
    /// @return A descriptor representing the newly created edge.
    /// @throws std::invalid_argument If either vertex ID is invalid.
    /// @copydetails detail::graph_doc_anchors::add_edge_note()
    edge_type add_edge(const id_type source_id, const id_type target_id) {
        this->_verify_vertex_ids(source_id, target_id);

        const auto new_edge_id = static_cast<id_type>(this->_n_edges++);
        this->_impl.add_edge(new_edge_id, source_id, target_id);

        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return edge_type{
                new_edge_id, source_id, target_id, this->_edge_properties.emplace_back()
            };
        else
            return edge_type{new_edge_id, source_id, target_id};
    }

    /// @brief Creates an edge carrying specific properties between a source and target vertex.
    /// @param source_id The ID of the source vertex.
    /// @param target_id The ID of the target vertex.
    /// @param properties The property payload to attach to the edge.
    /// @return A descriptor representing the newly created edge.
    /// @throws std::invalid_argument If either vertex ID is invalid.
    /// @copydetails detail::graph_doc_anchors::add_edge_note()
    edge_type add_edge_with(
        const id_type source_id, const id_type target_id, edge_properties_type properties
    )
    requires(traits::c_non_empty_properties<edge_properties_type>)
    {
        this->_verify_vertex_ids(source_id, target_id);

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

    /// @brief Creates an edge between a source and target vertex.
    /// @param source The source vertex descriptor.
    /// @param target The target vertex descriptor.
    /// @return A descriptor representing the newly created edge.
    /// @throws std::invalid_argument If either vertex descriptor is invalid.
    /// @copydetails detail::graph_doc_anchors::add_edge_note()
    gl_attr_force_inline edge_type add_edge(traits::c_graph_vertex<graph> auto source, traits::c_graph_vertex<graph> auto target) {
        return this->add_edge(source.id(), target.id());
    }

    /// @brief Creates an edge carrying specific properties between a source and target vertex.
    /// @param source The source vertex descriptor.
    /// @param target The target vertex descriptor.
    /// @param properties The property payload to attach to the edge.
    /// @return A descriptor representing the newly created edge.
    /// @throws std::invalid_argument If either vertex descriptor is invalid.
    /// @copydetails detail::graph_doc_anchors::add_edge_note()
    gl_attr_force_inline edge_type add_edge_with(
        traits::c_graph_vertex<graph> auto source, traits::c_graph_vertex<graph> auto target, const edge_properties_type& properties
    )
    requires(traits::c_non_empty_properties<edge_properties_type>)
    {
        return this->add_edge_with(source.id(), target.id(), properties);
    }

    // clang-format on

    /// @brief Dispatches multiple edge insertions connecting one source to many targets.
    /// @param source_id The ID of the source vertex.
    /// @param target_id_rng A sized range of target vertex IDs.
    /// @throws std::invalid_argument If any vertex ID is invalid.
    /// @copydetails detail::graph_doc_anchors::add_edge_note()
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

    /// @brief Dispatches multiple edge insertions connecting one source to many targets.
    /// @tparam TargetRng A sized range type containing graph's vertex descriptors.
    /// @param source The source vertex descriptor.
    /// @param target_rng A sized range of target vertex descriptors.
    /// @throws std::invalid_argument If any vertex ID is invalid.
    /// @copydetails detail::graph_doc_anchors::add_edge_note()
    template <typename TargetRng>
    requires(traits::c_sized_range<TargetRng> and traits::c_graph_vertex<std::ranges::range_value_t<TargetRng>, graph>)
    void add_edges_from(traits::c_graph_vertex<graph> auto source, const TargetRng& target_rng) {
        using rng_vertex_type = std::ranges::range_value_t<TargetRng>;

        this->_verify_vertex_id(source.id());
        for (auto target : target_rng)
            this->_verify_vertex_id(target.id());

        const auto prev_n_edges = this->_n_edges;
        this->_n_edges += std::ranges::size(target_rng);
        this->_impl.add_edges_from(
            std::views::iota(static_cast<id_type>(prev_n_edges), this->_n_edges),
            source.id(),
            target_rng | std::views::transform(&rng_vertex_type::id)
        );


        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            this->_edge_properties.resize(this->_n_edges);
    }

    /// @brief Removes a specific edge from the graph.
    /// @param edge The descriptor of the edge to remove.
    /// @throws std::invalid_argument If the edge descriptor is invalid;
    /// @copydetails detail::graph_doc_anchors::remove_edge_wrn()
    void remove_edge(const edge_type& edge) {
        this->_verify_edge(edge);
        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            this->_edge_properties.erase(this->_edge_properties.begin() + edge.id());
        this->_impl.remove_edge(edge);
        this->_n_edges--;
    }

    /// @brief Removes a range of edges from the graph.
    /// @param edges A range containing descriptors of the edges to remove.
    ///
    /// > [!NOTE] Operation Safety
    /// >
    /// > If the edges list is empty or contains no valid (in the context of the graph instance),
    /// > the operation has no effect on the graph's structure.
    ///
    /// @copydetails detail::graph_doc_anchors::remove_edge_wrn()
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

    /// @brief Returns a lazily evaluated view of all active edge IDs in the graph.
    /// @return A view yielding all valid edge IDs.
    [[nodiscard]] gl_attr_force_inline auto edge_ids() const noexcept {
        return std::views::iota(initial_id_v<id_type>, this->_n_edges);
    }

    /// @brief Verifies if the exact specified edge exists.
    /// @param edge The edge descriptor to verify.
    /// @return `true` if the edge exists within the graph, `false` otherwise.
    [[nodiscard]] bool has_edge(const edge_type& edge) const {
        if (not this->_is_valid_edge(edge))
            return false;

        return this->_impl.has_edge(edge);
    }

    /// @brief Checks if there is any connecting edge from source to target.
    /// @param source_id The source vertex ID.
    /// @param target_id The target vertex ID.
    /// @return `true` if an edge exists, `false` otherwise.
    /// @throws std::invalid_argument If either vertex ID is invalid.
    [[nodiscard]] bool has_edge(const id_type source_id, const id_type target_id) const {
        this->_verify_vertex_ids(source_id, target_id);
        return this->_impl.has_edge(source_id, target_id);
    }

    /// @brief Checks if there is any connecting edge from source to target.
    /// @param source The source vertex descriptor.
    /// @param target The target vertex descriptor.
    /// @return `true` if an edge exists, `false` otherwise.
    /// @throws std::invalid_argument If either vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline bool has_edge(
        traits::c_graph_vertex<graph> auto source, traits::c_graph_vertex<graph> auto target
    ) const {
        return this->has_edge(source.id(), target.id());
    }

    /// @brief Retrieves an edge (if it exists) connecting the source to the target.
    /// @param source_id The source vertex ID.
    /// @param target_id The target vertex ID.
    /// @return An optional containing the edge descriptor if it exists, or std::nullopt otherwise.
    /// @throws std::invalid_argument If either vertex ID is invalid.
    template <typename Self>
    [[nodiscard]] std::optional<edge_t<Self>> edge(
        this Self& self, const id_type source_id, const id_type target_id
    ) {
        self._verify_vertex_ids(source_id, target_id);
        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return self._impl.template edge<edge_t<Self>>(
                source_id, target_id, self._edge_properties
            );
        else
            return self._impl.template edge<edge_t<Self>>(source_id, target_id);
    }

    /// @brief Retrieves an edge (if it exists) connecting the source to the target.
    /// @param source The source vertex descriptor.
    /// @param target The target vertex descriptor.
    /// @return An optional containing the edge descriptor if it exists, or std::nullopt otherwise.
    /// @throws std::invalid_argument If either vertex descriptor is invalid.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline std::optional<edge_t<Self>> edge(
        this Self& self,
        traits::c_graph_vertex<graph> auto source,
        traits::c_graph_vertex<graph> auto target
    ) {
        return self.edge(source.id(), target.id());
    }

    /// @brief Retrieves all parallel edges connecting the source to the target.
    /// @param source_id The source vertex ID.
    /// @param target_id The target vertex ID.
    /// @return A vector populated with the descriptors of all edges linking the two vertices.
    /// @throws std::invalid_argument If either vertex ID is invalid.
    template <typename Self>
    [[nodiscard]] inline std::vector<edge_t<Self>> edges(
        this Self& self, const id_type source_id, const id_type target_id
    ) {
        self._verify_vertex_ids(source_id, target_id);
        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return self._impl.template edges<edge_t<Self>>(
                source_id, target_id, self._edge_properties
            );
        else
            return self._impl.template edges<edge_t<Self>>(source_id, target_id);
    }

    /// @brief Retrieves all parallel edges connecting the source to the target.
    /// @param source The source vertex descriptor.
    /// @param target The target vertex descriptor.
    /// @return A vector populated with the descriptors of all edges linking the two vertices.
    /// @throws std::invalid_argument If either vertex descriptor is invalid.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline std::vector<edge_t<Self>> edges(
        this Self& self,
        traits::c_graph_vertex<graph> auto source,
        traits::c_graph_vertex<graph> auto target
    ) {
        return self.edges(source.id(), target.id());
    }

    /// @brief Retrieves all edges incident with a vertex.
    /// @param vertex_id The vertex ID.
    /// @return A view representing the set of incident edges.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    template <typename Self>
    [[nodiscard]] inline auto incident_edges(this Self& self, const id_type vertex_id) {
        self._verify_vertex_id(vertex_id);
        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return self._impl.template incident_edges<edge_t<Self>>(
                vertex_id, self._edge_properties
            );
        else
            return self._impl.template incident_edges<edge_t<Self>>(vertex_id);
    }

    /// @brief Retrieves all edges incident with a vertex.
    /// @param vertex The vertex descriptor.
    /// @return A view representing the set of incident edges.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline auto incident_edges(
        this Self& self, traits::c_graph_vertex<graph> auto vertex
    ) {
        return self.incident_edges(vertex.id());
    }

    /// @brief Retrieves all incoming edges of a vertex (going into the vertex).
    /// @param vertex_id The vertex ID.
    /// @return A view representing the set of incoming edges.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    template <typename Self>
    [[nodiscard]] inline auto in_edges(this Self& self, const id_type vertex_id) {
        self._verify_vertex_id(vertex_id);
        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return self._impl.template in_edges<edge_t<Self>>(vertex_id, self._edge_properties);
        else
            return self._impl.template in_edges<edge_t<Self>>(vertex_id);
    }

    /// @brief Retrieves all incoming edges of a vertex (going into the vertex).
    /// @param vertex The vertex descriptor.
    /// @return A view representing the set of incoming edges.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline auto in_edges(
        this Self& self, traits::c_graph_vertex<graph> auto vertex
    ) {
        return self.in_edges(vertex.id());
    }

    /// @brief Retrieves all outgoing edges of a vertex (going out of the vertex).
    /// @param vertex_id The vertex ID.
    /// @return A view representing the set of outgoing edges.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    template <typename Self>
    [[nodiscard]] inline auto out_edges(this Self& self, const id_type vertex_id) {
        self._verify_vertex_id(vertex_id);
        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return self._impl.template out_edges<edge_t<Self>>(vertex_id, self._edge_properties);
        else
            return self._impl.template out_edges<edge_t<Self>>(vertex_id);
    }

    /// @brief Retrieves all outgoing edges of a vertex (going out of the vertex).
    /// @param vertex The vertex descriptor.
    /// @return A view representing the set of outgoing edges.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto out_edges(
        this auto& self, traits::c_graph_vertex<graph> auto vertex
    ) {
        return self.out_edges(vertex.id());
    }

    /// @brief Retrieves a mutable reference to an edge's properties.
    /// @param id The ID of the edge.
    /// @return A reference to the properties attached to the edge.
    /// @throws std::invalid_argument If the edge ID is invalid.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline edge_properties_t<Self>& edge_properties(
        this Self& self, const id_type id
    )
    requires(traits::c_non_empty_properties<edge_properties_type>)
    {
        if (id >= self._n_edges)
            throw std::invalid_argument(std::format("Got invalid edge id [{}]", id));

        return self._edge_properties[id];
    }

    /// @brief Retrieves a view over all edge properties in the graph.
    /// @return A view mapping each active edge index to its property.
    [[nodiscard]] gl_attr_force_inline auto edge_properties_map(this auto& self) noexcept
    requires(traits::c_non_empty_properties<edge_properties_type>)
    {
        return std::views::all(self._edge_properties);
    }

    // --- adjacency and incidence methods ---

    /// @brief Checks if two vertices are strictly adjacent.
    ///
    /// This method checks for a connection between the given vertices in either direction:
    /// - For undirected graphs this is equivalent to `has_edge(first_id, second_id)`
    /// - For directed graphs the result is true if either `has_edge(first_id, second_id)` or
    ///   `has_edge(second_id, first_id)` is true.
    ///
    /// ### Formal definition
    /// Vertices $u$ and $v$ are adjacent if there exists an edge connecting them in the graph:
    ///
    /// - For undirected graphs: $\{u, v\} in E$
    /// - For directed graphs: $(u, v) \in E \lor (v, u) \in E$
    ///
    /// @param source_id The ID of the source vertex.
    /// @param target_id The ID of the target vertex.
    /// @return `true` if the given vertices are adjacent, `false` otherwise.
    /// @throws std::invalid_argument If either vertex ID is invalid.
    [[nodiscard]] bool are_adjacent(const id_type source_id, const id_type target_id) const {
        this->_verify_vertex_ids(source_id, target_id);
        if constexpr (traits::c_undirected_graph<graph>)
            return this->has_edge(source_id, target_id);
        else
            return this->has_edge(source_id, target_id) or this->has_edge(target_id, source_id);
    }

    /// @brief Checks if two vertices are strictly adjacent.
    ///
    /// This method checks for a connection between the given vertices in either direction:
    /// - For undirected graphs this is equivalent to `has_edge(first, second)`
    /// - For directed graphs the result is true if either `has_edge(first, second)` or
    ///   `has_edge(second, first)` is true.
    ///
    /// ### Formal definition
    /// Vertices $u$ and $v$ are adjacent if there exists an edge connecting them in the graph:
    ///
    /// - For undirected graphs: $\{u, v\} in E$
    /// - For directed graphs: $(u, v) \in E \lor (v, u) \in E$
    ///
    /// @param source The source vertex descriptor.
    /// @param target The target vertex descriptor.
    /// @return `true` if the given vertices are adjacent, `false` otherwise.
    /// @throws std::invalid_argument If either vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline bool are_adjacent(
        traits::c_graph_vertex<graph> auto source, traits::c_graph_vertex<graph> auto target
    ) const {
        return this->are_adjacent(source.id(), target.id());
    }

    /// @brief Checks if two distinct edges share at least one incident vertex.
    ///
    /// ### Formal definition
    /// Edges $e$ and $f$ are adjacent if they share at least one endpoint: \f$e \cap f \ne \emptyset\f$.
    ///
    /// @param edge_1 The first edge descriptor.
    /// @param edge_2 The second edge descriptor.
    /// @return `true` if the given edges are adjacent, `false` otherwise.
    /// @throws std::invalid_argument If either edge descriptor is invalid.
    [[nodiscard]] bool are_adjacent(const edge_type& edge_1, const edge_type& edge_2) const {
        this->_verify_edge(edge_1);
        this->_verify_edge(edge_2);

        if (edge_1.id() == edge_2.id())
            return false;
        return edge_1.is_incident_with(edge_2.source()) or edge_1.is_incident_with(edge_2.target());
    }

    /// @brief Checks if a vertex forms one of the endpoints of an edge.
    ///
    /// ### Formal Definition
    /// A vertex \f$v\f$ is incident to an edge \f$e\f$ if \f$v\f$ is an element of the endpoint set of \f$e\f$ (\f$v \in e\f$).
    /// Assuming \f$e\f$ connects vertices \f$u\f$ and \f$w\f$: \f$v = u \lor v = w\f$
    ///
    /// @param vertex The vertex descriptor.
    /// @param edge The edge descriptor.
    /// @return `true` if the vertex is incident to the edge, `false` otherwise.
    /// @throws std::invalid_argument If either the vertex or the edge descriptor is invalid.
    [[nodiscard]] bool are_incident(
        traits::c_graph_vertex<graph> auto vertex, const edge_type& edge
    ) const {
        this->_verify_vertex_id(vertex.id());
        this->_verify_edge(edge);
        return edge.is_incident_with(vertex.id());
    }

    /// @brief Checks if a vertex forms one of the endpoints of an edge.
    ///
    /// A convenience overload of the `are_incident` method. It is equivalent to `are_incident(vertex, edge)`
    ///
    /// @param edge The edge descriptor.
    /// @param vertex The vertex descriptor.
    /// @return `true` if the vertex is incident to the edge, `false` otherwise.
    /// @throws std::invalid_argument If either the vertex or the edge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline bool are_incident(
        const edge_type& edge, traits::c_graph_vertex<graph> auto vertex
    ) const {
        return this->are_incident(vertex, edge);
    }

    // --- comparison ---

    /// @brief Compares two graphs for strict structural and property equality.
    /// @param lhs The left operand.
    /// @param rhs The right operand.
    /// @return `true` if both graphs represent the exact same topology and properties, `false` otherwise.
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

    /// @brief Formats and outputs the entire graph structure to a standard output stream.
    ///
    /// The generated string representation of the graph depends on the currently active formatting options of the stream.
    ///
    /// @param os The target output stream.
    /// @param g The graph instance to write.
    /// @return The stream reference for chaining.
    friend std::ostream& operator<<(std::ostream& os, const graph& g) {
        using io::detail::option_bit;

        if (io::is_option_set(os, option_bit::spec_fmt))
            return g._gsf_write(os);

        if (io::is_option_set(os, option_bit::verbose))
            return g._verbose_write(os);
        else
            return g._concise_write(os);
    }

    /// @brief Deserializes graph structure data from an input stream (using the GSF format).
    /// @param is The source input stream.
    /// @param g The graph instance to populate.
    /// @return The stream reference for chaining.
    gl_attr_force_inline friend std::istream& operator>>(std::istream& is, graph& g) {
        return g._gsf_read(is);
    }

    // --- friend declarations ---

    /// @brief Friend declaration providing access for deep cloning.
    template <traits::c_graph Graph>
    friend Graph clone(const Graph& source);

    /// @brief Friend declaration providing access for graph target conversions.
    template <traits::c_graph_repr_tag TargetImplTag, traits::c_graph Graph>
    requires(not std::is_lvalue_reference_v<Graph>)
    friend auto to(Graph&& source);

    /// @brief Internal friend structure for `to` conversion operations.
    template <traits::c_graph_repr_tag TargetImplTag, traits::c_graph_repr_tag SourceImplTag>
    friend struct detail::to_impl;

private:
    graph(const graph& other) = default;

    // --- element validation ---

    gl_attr_force_inline void _verify_vertex_id(const id_type vertex_id) const {
        if (not this->has_vertex(vertex_id))
            throw std::invalid_argument(std::format("Got invalid vertex id [{}]", vertex_id));
    }

    gl_attr_force_inline void _verify_vertex_ids(const std::same_as<id_type> auto... vertex_ids
    ) const {
        (this->_verify_vertex_id(vertex_ids), ...);
    }

    void _verify_edge(const edge_type& edge) const {
        if (not this->_is_valid_edge(edge))
            throw std::invalid_argument(std::format(
                "Got invalid edge [id = {}, vertices = ({}, {})]",
                edge.id(),
                edge.source(),
                edge.target()
            ));
    }

    [[nodiscard]] bool _is_valid_edge(const edge_type& edge) const noexcept {
        return edge.id() < this->_n_edges and this->has_vertex(edge.source())
           and this->has_vertex(edge.target());
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

    template <typename Self>
    gl_attr_force_inline auto _create_vertex_descriptor(this Self&) noexcept
    requires(traits::c_empty_properties<vertex_properties_type>)
    {
        return [](const id_type id) { return vertex_t<Self>{id}; };
    }

    template <typename Self>
    gl_attr_force_inline auto _create_vertex_descriptor(this Self& self) noexcept
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        return [&pmap = self._vertex_properties](const id_type id) {
            return vertex_t<Self>{id, pmap[to_idx(id)]};
        };
    }

    // --- I/O utility ---

    using fmt_traits = io::detail::graph_fmt_traits<directional_tag>;

    template <traits::c_graph G>
    struct concise_target_formatter {
        edge_t<G> edge;
        id_t<G> src_id;
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

    template <typename Self>
    std::ostream& _concise_write(this Self&& self, std::ostream& os) {
        using enum io::detail::option_bit;

        for (auto src : self.vertices()) {
            auto tgts = std::views::transform(
                self.out_edges(src.id()),
                [src_id = src.id(), with_props = io::is_option_set(os, with_connection_properties)](
                    const edge_t<Self>& edge
                ) { return concise_target_formatter<Self>{edge, src_id, with_props}; }
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

    [[no_unique_address]] vertex_properties_map_type _vertex_properties{};
    [[no_unique_address]] edge_properties_map_type _edge_properties{};
};

// --- general graph utility ---

/// @ingroup GL-Core
/// @brief Creates a deep copy of the given graph.
/// @tparam Graph The type of the graph.
/// @param source The graph instance to clone.
/// @return A newly constructed graph containing identical vertices, edges and properties (if applicable).
template <traits::c_graph Graph>
[[nodiscard]] Graph clone(const Graph& source) {
    return Graph(source);
}

/// @ingroup GL-Core
/// @brief Convenience alias for defining a directed graph.
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_repr_tag ReprTag = repr::list_t,
    traits::c_id_type IdType = default_id_type>
using directed_graph =
    graph<directed_graph_traits<VertexProperties, EdgeProperties, ReprTag, IdType>>;

/// @ingroup GL-Core
/// @brief Convenience alias for defining a nundirected graph.
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_repr_tag ReprTag = repr::list_t,
    traits::c_id_type IdType = default_id_type>
using undirected_graph =
    graph<undirected_graph_traits<VertexProperties, EdgeProperties, ReprTag, IdType>>;

/// @ingroup GL-Core
/// @brief Convenience alias for defining a graph utilizing an adjacency list representation model.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using list_graph =
    graph<list_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType>>;

/// @ingroup GL-Core
/// @brief Convenience alias for defining a graph utilizing an adjacency matrix representation model.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using matrix_graph =
    graph<matrix_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType>>;

/// @ingroup GL-Core
/// @brief Convenience alias for defining a graph utilizing a flattened adjacency list representation model.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using flat_list_graph =
    graph<flat_list_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType>>;

/// @ingroup GL-Core
/// @brief Convenience alias for defining a graph utilizing a flattened adjacency matrix representation model.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using flat_matrix_graph =
    graph<flat_matrix_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType>>;

// --- vertex distance utility ---

/// @brief Default numeric type representing distances between vertices in unweighted graphs.
using default_vertex_distance_type = std::int64_t;

/// @ingroup GL-Core
/// @brief Utility trait to resolve the underlying distance or weight numeric type for a graph.
template <traits::c_graph G>
struct vertex_distance {
    /// @brief Resolves to the default distance type if no specific weight property is detected.
    using type = default_vertex_distance_type;
};

/// @ingroup GL-Core
/// @brief Specialization resolving the specific weight type when edge properties contain weight attributes.
template <traits::c_graph G>
requires(traits::c_weight_properties_type<edge_properties_t<G>>)
struct vertex_distance<G> {
    /// @brief Resolves to the `weight_type` inherently mapped to the graph's edges.
    using type = typename edge_properties_t<G>::weight_type;
};

/// @ingroup GL-Core
/// @brief Convenience alias to retrieve the appropriate distance numeric type from a graph structure.
template <traits::c_graph G>
using vertex_distance_t = typename vertex_distance<G>::type;

/// @ingroup GL-Core
/// @brief Helper utility to safely retrieve the weight payload of an edge.
///
/// Automatically returns a constant default if the graph representation carries no attached weight properties.
///
/// @tparam G The underlying type of the graph.
/// @param edge The edge descriptor to extract weight from.
/// @return The specified edge weight or the unweighted graph default (1).
template <traits::c_graph G>
[[nodiscard]] gl_attr_force_inline vertex_distance_t<G> get_weight(
    const traits::c_graph_edge<G> auto& edge
) {
    if constexpr (traits::c_weight_properties_type<edge_properties_t<G>>)
        return edge.properties().weight;
    else
        return static_cast<default_vertex_distance_type>(1ll);
}

namespace detail::graph_doc_anchors {

// --- callouts ---

/// > [!IMPORTANT] ID Stability
/// >
/// > Adding vertices does **not** invalidate existing vertex IDs. **However**, property references stored in existing vertex descriptors may be invalidated.
void add_vertex_note();

/// > [!WARNING] Descriptor and ID Invalidation
/// >
/// > Removing a vertex invalidates:
/// > - All vertex descriptors and IDs for vertices with higher IDs (they shift down).
/// > - All edge descriptors and IDs for edges incident to this vertex.
/// > - All references to vertex and edge properties obtained from the property maps.
/// > - References to vertex properties obtained via `vertex_properties()`.
/// >
/// > Proceed with caution when maintaining external vertex IDs or edge descriptors.
void remove_vertex_wrn();

/// > [!IMPORTANT] ID Stability
/// >
/// > Adding edges does **not** invalidate vertex or edge IDs. **However**, property references stored in existing edge descriptors may be invalidated.
void add_edge_note();

/// > [!WARNING] Edge Descriptor Invalidation
/// >
/// > Removing an edge invalidates:
/// > - All edge descriptors and IDs for edges with higher IDs (they shift down).
/// > - References to edge properties obtained via `edge_properties()`.
/// > - References to edge properties obtained from `edge_properties_map()`.
/// >
/// > Vertex descriptors and IDs remain valid.
void remove_edge_wrn();

// --- definitions ---

/// ### Formal Definition
///
/// The neighborhood \f$N(v)\f$ of a vertex \f$v\f$ is the set of all its adjacent vertices:
///
/// \f[
/// N(v) =
/// \begin{cases}
/// \{u \in V : \{u, v\} \in E\} & \text{if } G \text{ is undirected}
/// \\\\ \{u \in V : (u, v) \in E \lor (v, u) \in E\} & \text{if } G \text{ is directed}
/// \end{cases}
/// \f]
void neighbors();

/// ### Formal Definition
/// The set of predecessors (in-neighborhood) \f$N_{in}(v)\f$ is defined as:
///
/// \f[
/// N_{in}(v) =
/// \begin{cases}
/// N(v) & \text{if } G \text{ is undirected}
/// \\\\ \{u \in V : (u, v) \in E\} & \text{if } G \text{ is directed}
/// \end{cases}
/// \f]
void predecessors();

/// ### Formal Definition
/// The set of successors (out-neighborhood) \f$N_{out}(v)\f$ is defined as:
///
/// \f[
/// N_{out}(v) =
/// \begin{cases}
/// N(v) & \text{if } G \text{ is undirected}
/// \\\\ \{u \in V : (v, u) \in E\} & \text{if } G \text{ is directed}
/// \end{cases}
/// \f]
void successors();

/// The degree is the total number of edge endpoints connected to the vertex.
/// For both directed and undirected graphs, a self-loop contributes **2** to the total degree.
///
/// ### Formal Definition
/// The formal calculation, accounting for the set of loops \f$L(v)\f$, is defined as:
///
/// \f[
/// deg(v) =
/// \begin{cases}
/// deg_{in}(v) + deg_{out}(v) & \text{if } G \text{ is directed}
/// \\ 2 \cdot |L(v)| + |E(v) \setminus L(v)| & \text{if } G \text{ is undirected}
/// \end{cases}
/// \f]
void degree();

/// The in-degree is the number of edges directed into the vertex.
///
/// ### Formal Definition
///
/// \f[
/// deg_{in}(v) =
/// \begin{cases}
/// deg(v) & \text{if } G \text{ is undirected}
/// \\\\ |E_{in}(v)| = |\{u \in V : (u, v) \in E\}| & \text{if } G \text{ is directed}
/// \end{cases}
/// \f]
void in_degree();

/// The out-degree is the number of edges directed out of the vertex.
///
/// ### Formal Definition
///
/// \f[
/// deg_{out}(v) =
/// \begin{cases}
/// deg(v) & \text{if } G \text{ is undirected}
/// \\\\ |E_{out}(v)| = |\{u \in V : (v, u) \in E\}| & \text{if } G \text{ is directed}
/// \end{cases}
/// \f]
void out_degree();

} // namespace detail::graph_doc_anchors
} // namespace gl
