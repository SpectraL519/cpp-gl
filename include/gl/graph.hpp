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

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is an instantiation of the generic @ref "gl::graph" graph class.
template <typename G>
concept c_graph = c_instantiation_of<G, graph>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a graph is directed.
/// @see gl::directed_t "directed_t" : For the directional tag used to specify directed graph configuration.
template <typename G>
concept c_directed_graph = c_graph<G> and c_directed_edge<typename G::edge_type>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a graph is undirected.
/// @see gl::undirected_t "undirected_t" : For the directional tag used to specify undirected graph configuration.
template <typename G>
concept c_undirected_graph = c_graph<G> and c_undirected_edge<typename G::edge_type>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a graph utilizes the standard adjacency list implementation.
/// @see gl::impl::list_t "list_t" : For the implementation tag used to specify the standard adjacency list representation.
template <typename G>
concept c_list_graph = c_graph<G> and std::same_as<typename G::implementation_tag, impl::list_t>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a graph utilizes the flattened adjacency list implementation.
/// @see gl::impl::flat_list_t "flat_list_t" : For the implementation tag used to specify the flattened adjacency list representation.
template <typename G>
concept c_flat_list_graph =
    c_graph<G> and std::same_as<typename G::implementation_tag, impl::flat_list_t>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a graph utilizes any list-based adjacency implementation.
/// ### See Also
/// - @ref gl::impl::list_t "list_t" : For the implementation tag used to specify the standard adjacency list representation.
/// - @ref gl::impl::flat_list_t "flat_list_t" : For the implementation tag used to specify the flattened adjacency list representation.
template <typename G>
concept c_adjacency_list_graph = c_list_graph<G> or c_flat_list_graph<G>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a graph utilizes the standard adjacency matrix implementation.
/// @see gl::impl::matrix_t "matrix_t" : For the implementation tag used to specify the standard adjacency matrix representation.
template <typename G>
concept c_matrix_graph =
    c_graph<G> and std::same_as<typename G::implementation_tag, impl::matrix_t>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a graph utilizes the flattened adjacency matrix implementation.
/// @see gl::impl::flat_matrix_t "flat_matrix_t" : For the implementation tag used to specify the flattened adjacency matrix representation.
template <typename G>
concept c_flat_matrix_graph =
    c_graph<G> and std::same_as<typename G::implementation_tag, impl::flat_matrix_t>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a graph utilizes any matrix-based adjacency implementation.
/// ### See Also
/// - @ref gl::impl::matrix_t "matrix_t" : For the implementation tag used to specify the standard adjacency matrix representation.
/// - @ref gl::impl::flat_matrix_t "flat_matrix_t" : For the implementation tag used to specify the flattened adjacency matrix representation.
template <typename G>
concept c_adjacency_matrix_graph = c_matrix_graph<G> or c_flat_matrix_graph<G>;

} // namespace traits

template <traits::c_graph Graph>
[[nodiscard]] Graph clone(const Graph& source);

template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph Graph>
[[nodiscard]] auto to(Graph&& source);

namespace detail {

template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph_impl_tag SourceImplTag>
struct to_impl;

} // namespace detail

/// @ingroup GL GL-Core
/// @brief The primary graph container using a policy-based architecture.
///
/// `graph` relies on the provided `GraphTraits` to determine its behavior, properties
/// (such as directionality), and underlying memory representation. It exposes a unified
/// API for adding, removing, and iterating over vertices and edges regardless of the backend.
///
/// ### Key Features
/// - **Policy-based design**: Behavior and representation are determined by `GraphTraits`.
/// - **Flexible directionality**: Support for both directed and undirected graphs.
/// - **Multiple representations**: Choose the underlying memory model that best suits your algorithmic and cache-locality needs:
///   - @ref gl::impl::list_t "list_t": Standard adjacency list.
///   - @ref gl::impl::flat_list_t "flat_list_t": Flattened adjacency list.
///   - @ref gl::impl::matrix_t "matrix_t": Standard adjacency matrix.
///   - @ref gl::impl::flat_matrix_t "flat_matrix_t": Flattened adjacency matrix.
/// - **Property support**: Vertices and edges can carry arbitrary properties.
/// - **Unified API**: Consistent interface regardless of the underlying implementation.
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
///     // (4)!
///     std::cout << "Vertices: " << g.n_vertices() << '\n';
///     std::cout << "Edges: " << g.n_edges() << '\n';
///
///     for (auto neighbor : g.neighbors(v0)) // (5)!
///         process(neighbor);
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
/// ### API Design: IDs vs. Descriptors
/// The graph exposes a dual API to accommodate different performance and ergonomic needs:
///
/// - **Inputs**: Most query methods are overloaded to accept either a raw `id_type` or a `vertex_type`/`edge_type` descriptor. They are functionally identical.
/// - **Outputs**: Methods ending in `_ids` (e.g., `neighbor_ids`) return views of raw integral IDs. Methods without this suffix (e.g., `neighbors`) automatically map those IDs to the proper descriptor objects.
/// - **Performance**: Descriptor-returning methods incur a slight overhead if the graph utilizes rich properties, as the descriptor must fetch the property payload. If you only need topology, prefer the `_ids` variants.
///
/// ### Descriptor Invalidation Behavior
///
/// The graph maintains the following invalidation semantics:
/// - **Vertex addition**: Does not invalidate vertex IDs. However, property references stored in vertex descriptors may be invalidated.
/// - **Vertex removal**: Invalidates vertex descriptors, IDs, and property references. Subsequent vertex IDs may shift depending on the implementation.
/// - **Edge addition**: Does not invalidate vertex or edge IDs. However, property references stored in edge descriptors may be invalidated.
/// - **Edge removal**: Invalidates edge descriptors, IDs, and property references. Vertex descriptors remain valid.
/// - **Property access**: References to vertex or edge properties obtained from the map may be invalidated by modifications to the graph structure.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | GraphTraits | Traits struct specifying behavior and representation | An instantiation of @ref gl::graph_traits "graph_traits" |
///
/// ### See Also
/// - @ref gl::directed_graph "directed_graph" : Convenience alias for directed graphs with standard list-based representation.
/// - @ref gl::undirected_graph "undirected_graph" : Convenience alias for undirected graphs.
/// - @ref gl::clone "clone" : Create a deep copy of a graph.
/// - @ref gl::to "to" : Convert a graph to a different implementation.
///
/// > [!IMPORTANT] Const Correctness
/// >
/// > `graph` supports move semantics but disables copy assignment to prevent accidental expensive copies.
/// > Use @ref gl::clone "clone" to explicitly copy a graph.
template <traits::c_instantiation_of<graph_traits> GraphTraits>
class graph final {
public:
    /// @brief Traits type specifying the behavior and representation.
    using traits_type = GraphTraits;

    /// @brief Type tag specifying the directionality of the graph.
    using directional_tag = typename traits_type::directional_tag;
    /// @brief Type tag indicating the underlying implementation model.
    using implementation_tag = typename traits_type::implementation_tag;

    /// @brief The internal implementation structure managing the adjacency logic.
    using implementation_type = typename implementation_tag::template type<traits_type>;
    friend implementation_type;

    /// @brief Integral type used to identify vertices and edges.
    using id_type = typename traits_type::id_type;

    /// @brief The descriptor type representing a vertex.
    using vertex_type = typename traits_type::vertex_type;
    /// @brief Type representing the properties attached to a vertex.
    using vertex_properties_type = typename traits_type::vertex_properties_type;
    /// @brief Type mapping vertex IDs to their respective properties.
    using vertex_properties_map_type = std::conditional_t<
        traits::c_empty_properties<vertex_properties_type>,
        empty_properties_map,
        std::vector<vertex_properties_type>>;

    /// @brief The descriptor type representing an edge.
    using edge_type = typename traits_type::edge_type;
    /// @brief Type representing the properties attached to an edge.
    using edge_properties_type = typename traits_type::edge_properties_type;

    /// @brief Type mapping edge IDs to their respective properties.
    using edge_properties_map_type = std::conditional_t<
        traits::c_empty_properties<edge_properties_type>,
        empty_properties_map,
        std::vector<edge_properties_type>>;

    /// @brief Default constructor creates an empty graph.
    graph() = default;

    /// @brief Constructs an empty graph with a pre-allocated number of vertices.
    /// @param n_vertices The initial number of vertices to allocate space for.
    explicit graph(const size_type n_vertices) : _n_vertices(n_vertices), _impl(n_vertices) {
        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.resize(n_vertices);
    }

    /// @brief Move constructor transfers ownership from another graph.
    graph(graph&&) noexcept = default;
    /// @brief Move assignment transfers ownership from another graph.
    graph& operator=(graph&&) noexcept = default;

    /// @brief Destructor cleans up graph memory.
    ~graph() = default;

    /// @brief Graph copy assignment is disabled to avoid accidental copies. Use @ref gl::clone "clone" instead.
    graph& operator=(const graph&) = delete;

    // --- size methods ---

    /// @brief Returns the total number of vertices in the graph.
    /// @return The vertex count.
    [[nodiscard]] gl_attr_force_inline size_type n_vertices() const noexcept {
        return this->_n_vertices;
    }

    /// @brief Returns the total number of edges in the graph.
    /// @return The edge count.
    [[nodiscard]] gl_attr_force_inline size_type n_edges() const noexcept {
        return this->_n_edges;
    }

    // --- vertex modifiers ---

    /// @brief Adds a new, default-initialized vertex to the graph.
    /// @return A descriptor for the newly created vertex.
    ///
    /// > [!IMPORTANT] ID Stability
    /// >
    /// > Adding vertices does **not** invalidate existing vertex IDs. **However**, property references stored in vertex descriptors may be invalidated.
    vertex_type add_vertex() {
        this->_impl.add_vertex();
        const auto new_vertex_id = static_cast<id_type>(this->_n_vertices++);

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_descriptor{new_vertex_id, this->_vertex_properties.emplace_back()};
        else
            return vertex_descriptor{new_vertex_id};
    }

    /// @brief Adds a new vertex with specific properties.
    /// @param properties The property payload for the new vertex.
    /// @return A descriptor for the newly created vertex.
    ///
    /// > [!IMPORTANT] ID Stability
    /// >
    /// > Adding vertices does **not** invalidate existing vertex IDs. **However**, property references stored in vertex descriptors may be invalidated.
    vertex_type add_vertex_with(vertex_properties_type properties)
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        this->_impl.add_vertex();
        return vertex_descriptor{
            static_cast<id_type>(this->_n_vertices++),
            this->_vertex_properties.emplace_back(std::move(properties))
        };
    }

    /// @brief Adds a specified number of default-initialized vertices to the graph en masse.
    /// @param n The number of vertices to add.
    ///
    /// > [!IMPORTANT] ID Stability
    /// >
    /// > Adding vertices does **not** invalidate existing vertex IDs. **However**, property references stored in vertex descriptors may be invalidated.
    void add_vertices(const size_type n) {
        this->_impl.add_vertices(n);
        this->_n_vertices += n;

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.resize(this->_n_vertices);
    }

    /// @brief Adds multiple vertices based on a range of property payloads.
    /// @param properties_rng A range of properties to initialize the new vertices with.
    ///
    /// > [!IMPORTANT] ID Stability
    /// >
    /// > Adding vertices does **not** invalidate existing vertex IDs. **However**, property references stored in vertex descriptors may be invalidated.
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
    /// @throws std::out_of_range If the ID is invalid.
    ///
    /// > [!WARNING] Descriptor and ID Invalidation
    /// >
    /// > Removing a vertex invalidates:
    /// > - All vertex descriptors and IDs for vertices with higher IDs (they shift down).
    /// > - All edge descriptors and IDs for edges incident to this vertex.
    /// > - All references to vertex and edge properties obtained from the property maps.
    /// > - References to vertex properties obtained via `vertex_properties()`.
    /// >
    /// > Proceed with caution when maintaining external vertex IDs or edge descriptors.
    gl_attr_force_inline void remove_vertex(const id_type vertex_id) {
        this->_verify_vertex_id(vertex_id);
        this->_remove_vertex_impl(vertex_id);
    }

    /// @brief Removes a vertex using its descriptor.
    /// @param vertex The descriptor of the vertex to remove.
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    ///
    /// > [!WARNING] Descriptor and ID Invalidation
    /// >
    /// > Removing a vertex invalidates:
    /// > - All vertex descriptors and IDs for vertices with higher IDs (they shift down).
    /// > - All edge descriptors and IDs for edges incident to this vertex.
    /// > - All references to vertex and edge properties obtained from the property maps.
    /// > - References to vertex properties obtained via `vertex_properties()`.
    gl_attr_force_inline void remove_vertex(vertex_type vertex) {
        this->remove_vertex(vertex.id());
    }

    /// @brief Removes a range of vertices using their IDs.
    /// @param vertex_id_rng A forward range containing the IDs of vertices to remove.
    /// @throws std::out_of_range If any vertex ID in the range is invalid.
    ///
    /// > [!WARNING] Descriptor and ID Invalidation
    /// >
    /// > Removing vertices invalidates:
    /// > - All vertex descriptors and IDs for vertices with higher IDs (they shift down).
    /// > - All edge descriptors and IDs for edges incident to removed vertices.
    /// > - All references to vertex and edge properties obtained from the property maps.
    void remove_vertices_from(const traits::c_forward_range_of<id_type> auto& vertex_id_rng) {
        // sorts the ids in a descending order and removes duplicate ids
        std::set<id_type, std::greater<>> vertex_id_set(
            std::ranges::begin(vertex_id_rng), std::ranges::end(vertex_id_rng)
        );

        // TODO: optimize
        for (auto vertex_id : vertex_id_set)
            this->_remove_vertex_impl(vertex_id);
    }

    /// @brief Removes a range of vertices using their descriptors.
    /// @param vertex_rng A sized range containing the descriptors of vertices to remove.
    /// @throws std::out_of_range If any vertex descriptor is invalid.
    ///
    /// > [!WARNING] Descriptor and ID Invalidation
    /// >
    /// > Removing vertices invalidates:
    /// > - All vertex descriptors and IDs for vertices with higher IDs (they shift down).
    /// > - All edge descriptors and IDs for edges incident to removed vertices.
    /// > - All references to vertex and edge properties obtained from the property maps.
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

    /// @brief Checks if a vertex with the given ID exists in the graph.
    /// @param vertex_id The ID to check.
    /// @return `true` if it exists, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool has_vertex(const id_type vertex_id) const {
        return vertex_id < this->_n_vertices;
    }

    /// @brief Checks if the given vertex descriptor is valid in the graph.
    /// @param vertex The vertex descriptor to check.
    /// @return `true` if it exists, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool has_vertex(vertex_type vertex) const {
        return this->has_vertex(vertex.id());
    }

    /// @brief Returns a vertex descriptor bounds-checked by ID.
    /// @param vertex_id The ID of the vertex.
    /// @return The corresponding vertex descriptor.
    /// @throws std::out_of_range If the ID is invalid.
    [[nodiscard]] vertex_type vertex(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->vertex_unchecked(vertex_id);
    }

    /// @brief Returns a vertex descriptor bounds-checked by ID (alias for `vertex`).
    /// @param vertex_id The ID of the vertex.
    /// @return The corresponding vertex descriptor.
    /// @throws std::out_of_range If the ID is invalid.
    [[nodiscard]] gl_attr_force_inline vertex_type at(const id_type vertex_id) const {
        return this->vertex(vertex_id);
    }

    /// @brief Returns a vertex descriptor without bounds checking.
    /// @param vertex_id The ID of the vertex.
    /// @return The corresponding vertex descriptor.
    ///
    /// > [!WARNING] Undefined Behavior
    /// >
    /// > No bounds checking is performed. Passing an invalid ID results in Undefined Behavior.
    [[nodiscard]] gl_attr_force_inline vertex_type vertex_unchecked(const id_type vertex_id
    ) const noexcept {
        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_descriptor{vertex_id, this->_vertex_properties[vertex_id]};
        else
            return vertex_descriptor{vertex_id};
    }

    /// @brief Returns a vertex descriptor without bounds checking (array access style).
    /// @param vertex_id The ID of the vertex.
    /// @return The corresponding vertex descriptor.
    [[nodiscard]] gl_attr_force_inline vertex_type operator[](const id_type vertex_id
    ) const noexcept {
        return this->vertex_unchecked(vertex_id);
    }

    /// @brief Returns a lazily evaluated view of all vertex descriptors in the graph.
    /// @return A view yielding descriptors for every vertex.
    [[nodiscard]] gl_attr_force_inline auto vertices() const noexcept {
        return this->vertex_ids() | std::views::transform(this->_create_vertex_descriptor());
    }

    /// @brief Returns a lazily evaluated view of all active vertex IDs in the graph.
    /// @return A view yielding all valid vertex IDs.
    [[nodiscard]] gl_attr_force_inline auto vertex_ids() const noexcept {
        return std::views::iota(initial_id_v<id_type>, this->_n_vertices);
    }

    /// @brief Retrieves the neighbor vertex IDs for a specific vertex.
    ///
    /// ### Formal Definition
    /// The neighborhood \f$N(v)\f$ of a vertex \f$v\f$ is the set of all its adjacent vertices:
    ///
    /// \f$
    /// N(v) =
    /// \begin{cases}
    /// \{u \in V : \{u, v\} \in E\} & \text{if } G \text{ is undirected}
    /// \\\\ \{u \in V : (u, v) \in E(G) \lor (v, u) \in E(G)\} & \text{if } G \text{ is directed}
    /// \end{cases}
    /// \f$
    ///
    /// @param vertex_id The ID of the source vertex.
    /// @return A view of all adjacent vertex IDs.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto neighbors(const id_type vertex_id) const {
        return this->neighbor_ids(vertex_id)
             | std::views::transform(this->_create_vertex_descriptor());
    }

    /// @brief Retrieves the neighbor vertex descriptors for a specific vertex.
    /// @copydetails neighbors(const id_type) const
    /// @param vertex The source vertex descriptor.
    /// @return A view of all adjacent vertex descriptors.
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto neighbors(vertex_type vertex) const {
        return this->neighbors(vertex.id());
    }

    /// @brief Retrieves the neighbor vertex IDs for a specific vertex.
    /// @param vertex_id The ID of the source vertex.
    /// @return A view of all adjacent vertex IDs.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto neighbor_ids(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.neighbor_ids(vertex_id);
    }

    /// @brief Retrieves the neighbor vertex IDs for a specific vertex.
    /// @param vertex The source vertex descriptor.
    /// @return A view of all adjacent vertex IDs.
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto neighbor_ids(vertex_type vertex) const {
        return this->neighbor_ids(vertex.id());
    }

    /// @brief Retrieves the predecessor vertex IDs for a vertex.
    ///
    /// ### Formal Definition
    /// The set of predecessors (in-neighborhood) \f$N_{in}(v)\f$ is defined as:
    ///
    /// \f$
    /// N_{in}(v) =
    /// \begin{cases}
    /// N(v) & \text{if } G \text{ is undirected}
    /// \\\\ \{u \in V(G) : (u, v) \in E(G)\} & \text{if } G \text{ is directed}
    /// \end{cases}
    /// \f$
    ///
    /// @param vertex_id The ID of the target vertex.
    /// @return A view of all predecessor vertex IDs.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto predecessors(const id_type vertex_id) const {
        return this->predecessor_ids(vertex_id)
             | std::views::transform(this->_create_vertex_descriptor());
    }

    /// @brief Retrieves the predecessor vertex descriptors (incoming edges) for a vertex.
    /// @param vertex The target vertex descriptor.
    /// @return A view of all predecessor vertex descriptors.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto predecessors(vertex_type vertex) const {
        return this->predecessors(vertex.id());
    }

    /// @brief Retrieves the predecessor vertex IDs for a vertex.
    /// @param vertex_id The ID of the target vertex.
    /// @return A view of all predecessor vertex IDs.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto predecessor_ids(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.predecessor_ids(vertex_id);
    }

    /// @brief Retrieves the predecessor vertex IDs for a vertex.
    /// @param vertex The target vertex descriptor.
    /// @return A view of all predecessor vertex IDs.
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto predecessor_ids(vertex_type vertex) const {
        return this->predecessor_ids(vertex.id());
    }

    /// @brief Retrieves the successor vertex IDs for a vertex.
    ///
    /// ### Formal Definition
    /// The set of successors (out-neighborhood) \f$N_{out}(v)\f$ is defined as:
    ///
    /// \f$
    /// N_{out}(v) =
    /// \begin{cases}
    /// N(v) & \text{if } G \text{ is undirected}
    /// \\\\ \{u \in V(G) : (v, u) \in E(G)\} & \text{if } G \text{ is directed}
    /// \end{cases}
    /// \f$
    ///
    /// @param vertex_id The ID of the source vertex.
    /// @return A view of all successor vertex IDs.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto successors(const id_type vertex_id) const {
        return this->successor_ids(vertex_id)
             | std::views::transform(this->_create_vertex_descriptor());
    }

    /// @brief Retrieves the successor vertex descriptors (outgoing edges) for a vertex.
    /// @param vertex The source vertex descriptor.
    /// @return A view of all successor vertex descriptors.
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto successors(vertex_type vertex) const {
        return this->successors(vertex.id());
    }

    /// @brief Retrieves the successor vertex IDs for a vertex.
    /// @param vertex_id The ID of the source vertex.
    /// @return A view of all successor vertex IDs.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto successor_ids(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.successor_ids(vertex_id);
    }

    /// @brief Retrieves the successor vertex IDs for a vertex.
    /// @param vertex The source vertex descriptor.
    /// @return A view of all successor vertex IDs.
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto successor_ids(vertex_type vertex) const {
        return this->successor_ids(vertex.id());
    }

    /// @brief Retrieves a mutable reference to a vertex's properties.
    /// @param id The ID of the vertex.
    /// @return A reference to the properties attached to the vertex.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline vertex_properties_type& vertex_properties(const id_type id
    ) const
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        this->_verify_vertex_id(id);
        return this->_vertex_properties[id];
    }

    /// @brief Retrieves a view over all vertex properties in the graph.
    /// @return A view mapping each active vertex index to its property.
    [[nodiscard]] gl_attr_force_inline auto vertex_properties_map() const noexcept
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        return std::views::all(this->_vertex_properties);
    }

    // --- degree getters ---

    /// @brief Calculates the total degree of a vertex.
    ///
    /// The degree is the total number of edge endpoints connected to the vertex.
    /// For both directed and undirected graphs, a self-loop contributes **2** to the total degree.
    ///
    /// ### Formal Definition
    /// The formal calculation, accounting for the set of loops \f$L(v)\f$, is defined as:
    ///
    /// \f$
    /// deg(v) =
    /// \begin{cases}
    /// deg_{in}(v) + deg_{out}(v) & \text{if } G \text{ is directed} \\
    /// 2 \cdot |L(v)| + |E(v) \setminus L(v)| & \text{if } G \text{ is undirected}
    /// \end{cases}
    /// \f$
    ///
    /// @param vertex_id The ID of the vertex.
    /// @return The total degree.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline size_type degree(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.degree(vertex_id);
    }

    /// @brief Calculates the total degree of a vertex.
    /// @param vertex The vertex descriptor.
    /// @return The total degree.
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline size_type degree(vertex_type vertex) const {
        return this->degree(vertex.id());
    }

    /// @brief Returns a mapped array of total degrees for all vertices.
    /// @return A vector where the index aligns with the vertex ID containing its degree.
    [[nodiscard]] gl_attr_force_inline std::vector<size_type> degree_map() const {
        return this->_impl.degree_map();
    }

    /// @brief Calculates the in-degree (incoming edges) for a vertex.
    ///
    /// The in-degree is the number of edges directed into the vertex.
    ///
    /// ### Formal Definition
    ///
    /// \f$
    /// deg_{in}(v) =
    /// \begin{cases}
    /// deg(v) & \text{if } G \text{ is undirected}
    /// \\\\ |E_{in}(v)| = |\{u \in V : (u, v) \in E\}| & \text{if } G \text{ is directed}
    /// \end{cases}
    /// \f$
    ///
    /// @param vertex_id The ID of the vertex.
    /// @return The in-degree.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline size_type in_degree(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.in_degree(vertex_id);
    }

    /// @brief Calculates the in-degree (incoming edges) for a vertex.
    /// @param vertex The vertex descriptor.
    /// @return The in-degree.
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline size_type in_degree(vertex_type vertex) const {
        return this->in_degree(vertex.id());
    }

    /// @brief Returns a mapped array of in-degrees for all vertices.
    /// @return A vector where the index aligns with the vertex ID containing its in-degree.
    [[nodiscard]] gl_attr_force_inline std::vector<size_type> in_degree_map() const {
        return this->_impl.in_degree_map();
    }

    /// @brief Calculates the out-degree (outgoing edges) for a vertex.
    ///
    /// The out-degree is the number of edges directed out of the vertex.
    ///
    /// ### Formal Definition
    ///
    /// \f$
    /// deg_{out}(v) =
    /// \begin{cases}
    /// deg(v) & \text{if } G \text{ is undirected}
    /// \\\\ |E_{out}(v)| = |\{u \in V : (v, u) \in E\}| & \text{if } G \text{ is directed}
    /// \end{cases}
    /// \f$
    ///
    /// @param vertex_id The ID of the vertex.
    /// @return The out-degree.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline size_type out_degree(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.out_degree(vertex_id);
    }

    /// @brief Calculates the out-degree (outgoing edges) for a vertex.
    /// @param vertex The vertex descriptor.
    /// @return The out-degree.
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline size_type out_degree(vertex_type vertex) const {
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
    /// @throws std::out_of_range If either vertex ID is invalid.
    ///
    /// > [!IMPORTANT] ID Stability
    /// >
    /// > Adding edges does **not** invalidate vertex or edge IDs. **However**, property references stored in edge descriptors may be invalidated.
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

    /// @brief Creates an edge carrying specific properties between a source and target vertex.
    /// @param source_id The ID of the source vertex.
    /// @param target_id The ID of the target vertex.
    /// @param properties The property payload to attach to the edge.
    /// @return A descriptor representing the newly created edge.
    /// @throws std::out_of_range If either vertex ID is invalid.
    ///
    /// > [!IMPORTANT] ID Stability
    /// >
    /// > Adding edges does **not** invalidate vertex or edge IDs. **However**, property references stored in edge descriptors may be invalidated.
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

    /// @brief Creates an edge between a source and target vertex.
    /// @param source The source vertex descriptor.
    /// @param target The target vertex descriptor.
    /// @return A descriptor representing the newly created edge.
    gl_attr_force_inline edge_type add_edge(vertex_type source, vertex_type target) {
        return this->add_edge(source.id(), target.id());
    }

    /// @brief Creates an edge carrying specific properties between a source and target vertex.
    /// @param source The source vertex descriptor.
    /// @param target The target vertex descriptor.
    /// @param properties The property payload to attach to the edge.
    /// @return A descriptor representing the newly created edge.
    gl_attr_force_inline edge_type add_edge_with(
        vertex_type source, vertex_type target, const edge_properties_type& properties
    )
    requires(traits::c_non_empty_properties<edge_properties_type>)
    {
        return this->add_edge_with(source.id(), target.id(), properties);
    }

    // clang-format on

    /// @brief Dispatches multiple edge insertions connecting one source to many targets.
    /// @param source_id The ID of the source vertex.
    /// @param target_id_rng A sized range of target vertex IDs.
    /// @throws std::out_of_range If the source ID or any target ID is invalid.
    ///
    /// > [!IMPORTANT] ID Stability
    /// >
    /// > Adding edges does **not** invalidate vertex or edge IDs. **However**, property references stored in edge descriptors may be invalidated.
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
    /// @param source The source vertex descriptor.
    /// @param target_rng A sized range of target vertex descriptors.
    /// @throws std::out_of_range If the source descriptor or any target descriptor is invalid.
    ///
    /// > [!IMPORTANT] ID Stability
    /// >
    /// > Adding edges does **not** invalidate vertex or edge IDs. **However**, property references stored in edge descriptors may be invalidated.
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

    /// @brief Removes a specific edge from the graph.
    /// @param edge The descriptor of the edge to remove.
    ///
    /// > [!WARNING] Edge Descriptor Invalidation
    /// >
    /// > Removing an edge invalidates:
    /// > - All edge descriptors and IDs for edges with higher IDs (they shift down).
    /// > - References to edge properties obtained via `edge_properties()`.
    /// > - References to edge properties obtained from `edge_properties_map()`.
    /// >
    /// > Vertex descriptors and IDs remain valid.
    ///
    /// @throws std::invalid_argument If the edge descriptor is invalid;
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
    /// > [!WARNING] Edge Descriptor Invalidation
    /// >
    /// > Removing edges invalidates:
    /// > - All edge descriptors and IDs for edges with higher IDs (they shift down).
    /// > - References to edge properties obtained via `edge_properties()`.
    /// > - References to edge properties obtained from `edge_properties_map()`.
    /// >
    /// > Vertex descriptors and IDs remain valid.
    ///
    /// > [!NOTE] Operation Safety
    /// >
    /// > If the edges list is empty or contains no valid (in the context of the graph instance),
    /// > the operation has no effect on the graph's structure.
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
    /// @return `true` if the edge exists, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool has_edge(const edge_type& edge) const {
        return this->_impl.has_edge(edge);
    }

    /// @brief Checks if there is any connecting edge from source to target.
    /// @param source_id The source vertex ID.
    /// @param target_id The target vertex ID.
    /// @return `true` if an edge exists, `false` otherwise.
    /// @throws std::out_of_range If either vertex ID is invalid.
    [[nodiscard]] bool has_edge(const id_type source_id, const id_type target_id) const {
        this->_verify_vertex_id(source_id);
        this->_verify_vertex_id(target_id);
        return this->_impl.has_edge(source_id, target_id);
    }

    /// @brief Checks if there is any connecting edge from source to target.
    /// @param source The source vertex descriptor.
    /// @param target The target vertex descriptor.
    /// @return `true` if an edge exists, `false` otherwise.
    /// @throws std::out_of_range If either vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline bool has_edge(vertex_type source, vertex_type target) const {
        return this->has_edge(source.id(), target.id());
    }

    /// @brief Retrieves an edge (if it exists) connecting the source to the target.
    /// @param source_id The source vertex ID.
    /// @param target_id The target vertex ID.
    /// @return An `std::optional` containing the edge descriptor if found, `std::nullopt` otherwise.
    /// @throws std::out_of_range If either vertex ID is invalid.
    [[nodiscard]] std::optional<edge_type> edge(const id_type source_id, const id_type target_id)
        const {
        this->_verify_vertex_id(source_id);
        this->_verify_vertex_id(target_id);

        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return this->_impl.edge(source_id, target_id, this->_edge_properties);
        else
            return this->_impl.edge(source_id, target_id);
    }

    /// @brief Retrieves an edge (if it exists) connecting the source to the target.
    /// @param source The source vertex descriptor.
    /// @param target The target vertex descriptor.
    /// @return An `std::optional` containing the edge descriptor if found, `std::nullopt` otherwise.
    /// @throws std::out_of_range If either vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline std::optional<edge_type> edge(
        vertex_type source, vertex_type target
    ) const {
        return this->edge(source.id(), target.id());
    }

    /// @brief Retrieves all parallel edges connecting the source to the target.
    /// @param source_id The source vertex ID.
    /// @param target_id The target vertex ID.
    /// @return A vector populated with the descriptors of all edges linking the two vertices.
    /// @throws std::out_of_range If either vertex ID is invalid.
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

    /// @brief Retrieves all parallel edges connecting the source to the target.
    /// @param source The source vertex descriptor.
    /// @param target The target vertex descriptor.
    /// @return A vector populated with the descriptors of all edges linking the two vertices.
    /// @throws std::out_of_range If either vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline std::vector<edge_type> edges(
        vertex_type source, vertex_type target
    ) const {
        return this->edges(source.id(), target.id());
    }

    /// @brief Retrieves all incident edges attached to a vertex.
    /// @param vertex_id The vertex ID.
    /// @return A view or container representing the incident edges.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] inline auto incident_edges(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return this->_impl.incident_edges(vertex_id, this->_edge_properties);
        else
            return this->_impl.incident_edges(vertex_id);
    }

    /// @brief Retrieves all incident edges attached to a vertex.
    /// @param vertex The vertex descriptor.
    /// @return A view or container representing the incident edges.
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto incident_edges(vertex_type vertex) const {
        return this->incident_edges(vertex.id());
    }

    /// @brief Retrieves all incoming edges entering a vertex.
    /// @param vertex_id The vertex ID.
    /// @return A view or container representing the incoming edges.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] inline auto in_edges(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return this->_impl.in_edges(vertex_id, this->_edge_properties);
        else
            return this->_impl.in_edges(vertex_id);
    }

    /// @brief Retrieves all incoming edges entering a vertex.
    /// @param vertex The vertex descriptor.
    /// @return A view or container representing the incoming edges.
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto in_edges(vertex_type vertex) const {
        return this->in_edges(vertex.id());
    }

    /// @brief Retrieves all outgoing edges leaving a vertex.
    /// @param vertex_id The vertex ID.
    /// @return A view or container representing the outgoing edges.
    /// @throws std::out_of_range If the vertex ID is invalid.
    [[nodiscard]] inline auto out_edges(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        if constexpr (traits::c_non_empty_properties<edge_properties_type>)
            return this->_impl.out_edges(vertex_id, this->_edge_properties);
        else
            return this->_impl.out_edges(vertex_id);
    }

    /// @brief Retrieves all outgoing edges leaving a vertex.
    /// @param vertex The vertex descriptor.
    /// @return A view or container representing the outgoing edges.
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto out_edges(vertex_type vertex) const {
        return this->out_edges(vertex.id());
    }

    /// @brief Retrieves a mutable reference to an edge's properties.
    /// @param id The ID of the edge.
    /// @return A reference to the properties attached to the edge.
    /// @throws std::out_of_range If the edge ID is invalid.
    [[nodiscard]] edge_properties_type& edge_properties(const id_type id) const
    requires(traits::c_non_empty_properties<edge_properties_type>)
    {
        if (id >= this->_n_edges)
            throw std::out_of_range(std::format("Got invalid edge id [{}]", id));

        return this->_edge_properties[id];
    }

    /// @brief Retrieves a view over all edge properties in the graph.
    /// @return A view mapping each active edge index to its property.
    [[nodiscard]] gl_attr_force_inline auto edge_properties_map() const noexcept
    requires(traits::c_non_empty_properties<edge_properties_type>)
    {
        return std::views::all(this->_edge_properties);
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
    /// @param first_id The ID of the first vertex.
    /// @param second_id The ID of the second vertex.
    /// @return `true` if they are adjacent, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool are_adjacent(
        const id_type first_id, const id_type second_id
    ) const {
        if constexpr (traits::c_undirected_graph<graph>)
            return this->has_edge(first_id, second_id);
        else
            return this->has_edge(first_id, second_id) or this->has_edge(second_id, first_id);
    }

    /// @brief Checks if two vertices are strictly adjacent.
    /// @param first The first vertex descriptor.
    /// @param second The second vertex descriptor.
    /// @return `true` if they are adjacent, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool are_adjacent(vertex_type first, vertex_type second)
        const {
        return this->are_adjacent(first.id(), second.id());
    }

    /// @brief Checks if two distinct edges share at least one incident vertex.
    ///
    /// ### Formal definition
    /// Edges $e$ and $f$ are adjacent if they share at least one endpoint: \f$e \cap f \ne \emptyset\f$.
    ///
    /// @param edge_1 The first edge descriptor.
    /// @param edge_2 The second edge descriptor.
    /// @return `true` if they are adjacent (share a vertex), `false` otherwise.
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
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    /// @throws std::invalid_argument If the edge descriptor is invalid.
    [[nodiscard]] bool are_incident(vertex_type vertex, const edge_type& edge) const {
        this->_verify_vertex_id(vertex.id());
        this->_verify_edge(edge);
        return edge.is_incident_with(vertex.id());
    }

    /// @brief Checks if a vertex forms one of the endpoints of an edge.
    ///
    /// A convenience overload of the `are_adjacent` method. It is equivalent to `are_incident(vertex, edge)`
    ///
    /// @param edge The edge descriptor.
    /// @param vertex The vertex descriptor.
    /// @return `true` if the vertex is incident to the edge, `false` otherwise.
    /// @throws std::out_of_range If the vertex descriptor is invalid.
    /// @throws std::invalid_argument If the edge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline bool are_incident(const edge_type& edge, vertex_type vertex)
        const {
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

    /// @brief Serializes the graph's string representation to an output stream.
    ///
    /// Depending on active stream flags, this outputs in verbose, concise, or GSF formats.
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

    /// @brief Deserializes graph structure data from an input stream (GSF format).
    /// @param is The source input stream.
    /// @param g The graph instance to populate.
    /// @return The stream reference for chaining.
    friend gl_attr_force_inline std::istream& operator>>(std::istream& is, graph& g) {
        return g._gsf_read(is);
    }

    // --- friend declarations ---

    /// @brief Friend declaration providing access for deep cloning.
    template <traits::c_graph Graph>
    friend Graph clone(const Graph& source);

    /// @brief Friend declaration providing access for graph target conversions.
    template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph Graph>
    friend auto to(Graph&& source);

    /// @brief Internal friend structure for `to` conversion operations.
    template <traits::c_graph_impl_tag TargetImplTag, traits::c_graph_impl_tag SourceImplTag>
    friend struct detail::to_impl;

private:
    using fmt_traits = io::detail::graph_fmt_traits<directional_tag>;

    graph(const graph& other) = default;

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

/// @ingroup GL GL-Core
/// @brief Creates a deep copy of the given graph.
/// @tparam Graph The type of the graph.
/// @param source The graph instance to clone.
/// @return A newly constructed graph containing identical vertices, edges and properties (if applicable).
template <traits::c_graph Graph>
[[nodiscard]] Graph clone(const Graph& source) {
    return Graph(source);
}

/// @ingroup GL GL-Types
/// @brief Convenience alias for defining a standard directed graph.
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_impl_tag ImplTag = impl::list_t,
    traits::c_id_type IdType = default_id_type>
using directed_graph =
    graph<directed_graph_traits<VertexProperties, EdgeProperties, ImplTag, IdType>>;

/// @ingroup GL GL-Types
/// @brief Convenience alias for defining a standard undirected graph.
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_graph_impl_tag ImplTag = impl::list_t,
    traits::c_id_type IdType = default_id_type>
using undirected_graph =
    graph<undirected_graph_traits<VertexProperties, EdgeProperties, ImplTag, IdType>>;

/// @ingroup GL GL-Types
/// @brief Convenience alias for defining a graph utilizing an adjacency list.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using list_graph =
    graph<list_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType>>;

/// @ingroup GL GL-Types
/// @brief Convenience alias for defining a graph utilizing an adjacency matrix.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using matrix_graph =
    graph<matrix_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType>>;

/// @ingroup GL GL-Types
/// @brief Convenience alias for defining a graph utilizing a flattened adjacency list.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties EdgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using flat_list_graph =
    graph<flat_list_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType>>;

/// @ingroup GL GL-Types
/// @brief Convenience alias for defining a graph utilizing a flattened adjacency matrix.
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

/// @ingroup GL GL-Core
/// @brief Utility trait to resolve the underlying distance or weight numeric type for a graph.
template <traits::c_graph GraphType>
struct vertex_distance {
    /// @brief Resolves to the default distance type if no specific weight property is detected.
    using type = default_vertex_distance_type;
};

/// @ingroup GL GL-Core
/// @brief Specialization resolving the specific weight type when edge properties contain weight attributes.
template <traits::c_graph GraphType>
requires(traits::c_weight_properties_type<typename GraphType::edge_properties_type>)
struct vertex_distance<GraphType> {
    /// @brief Resolves to the `weight_type` inherently mapped to the graph's edges.
    using type = typename GraphType::edge_properties_type::weight_type;
};

/// @ingroup GL GL-Core
/// @brief Convenience alias to retrieve the appropriate distance numeric type from a graph structure.
template <traits::c_graph GraphType>
using vertex_distance_type = typename vertex_distance<GraphType>::type;

/// @ingroup GL GL-Core
/// @brief Helper utility to safely retrieve the weight payload of an edge.
///
/// Automatically returns a constant default if the graph representation carries no attached weight properties.
///
/// @tparam GraphType The underlying type of the graph.
/// @param edge The edge descriptor to extract weight from.
/// @return The specified edge weight or the unweighted graph default (1).
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
