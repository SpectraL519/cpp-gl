// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/hypergraph.hpp
/// @brief Defines the core generic `hypergraph` container and its associated traits and utilities.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/traits.hpp"
#include "gl/types/core.hpp"
#include "hgl/constants.hpp"
#include "hgl/directional_tags.hpp"
#include "hgl/hypergraph_traits.hpp"
#include "hgl/io/core.hpp"
#include "hgl/io/hypergraph_fmt_traits.hpp"
#include "hgl/repr/repr_tags.hpp"

#include <algorithm>
#include <initializer_list>
#include <set>
#include <type_traits>
#include <vector>

namespace hgl {

template <traits::c_instantiation_of<hypergraph_traits> HypergraphTraits = hypergraph_traits<>>
class hypergraph;

// --- general hypergraph utility ---

namespace traits {

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is an instantiation of the generic @ref hgl::hypergraph "hypergraph" class.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_hypergraph = c_instantiation_of<std::remove_cvref_t<H>, hypergraph>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a constant instantiation of the generic @ref hgl::hypergraph "hypergraph" class.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_const_hypergraph = c_hypergraph<H> and std::is_const_v<std::remove_reference_t<H>>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a mutable instantiation of the generic @ref hgl::hypergraph "hypergraph" class.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_mut_hypergraph = c_hypergraph<H> and not std::is_const_v<std::remove_reference_t<H>>;

} // namespace traits

/// @ingroup HGL-Core
/// @brief Extracts the underlying unqualified hypergraph type by removing reference and cv-qualifiers.
template <traits::c_hypergraph H>
using val_t = std::remove_cvref_t<H>;

/// @ingroup HGL-Core
/// @brief Resolves the identifier type associated with the given hypergraph type.
template <traits::c_hypergraph H>
using id_t = typename val_t<H>::id_type;

/// @ingroup HGL-Core
/// @brief Resolves the appropriate vertex descriptor type (mutable or const) based on the hypergraph's constness.
template <traits::c_hypergraph H>
using vertex_t = std::conditional_t<
    std::is_const_v<std::remove_reference_t<H>>,
    typename val_t<H>::const_vertex_type,
    typename val_t<H>::vertex_type>;

/// @ingroup HGL-Core
/// @brief Resolves the appropriate vertex properties type (mutable or const) based on the hypergraph's constness.
template <traits::c_hypergraph H>
using vertex_properties_t = std::conditional_t<
    std::is_const_v<std::remove_reference_t<H>>,
    const typename val_t<H>::vertex_properties_type,
    typename val_t<H>::vertex_properties_type>;

/// @ingroup HGL-Core
/// @brief Resolves the appropriate hyperedge descriptor type (mutable or const) based on the hypergraph's constness.
template <traits::c_hypergraph H>
using hyperedge_t = std::conditional_t<
    std::is_const_v<std::remove_reference_t<H>>,
    typename val_t<H>::const_hyperedge_type,
    typename val_t<H>::hyperedge_type>;

/// @ingroup HGL-Core
/// @brief Resolves the appropriate hyperedge properties type (mutable or const) based on the hypergraph's constness.
template <traits::c_hypergraph H>
using hyperedge_properties_t = std::conditional_t<
    std::is_const_v<std::remove_reference_t<H>>,
    const typename val_t<H>::hyperedge_properties_type,
    typename val_t<H>::hyperedge_properties_type>;

namespace traits {

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph is undirected.
/// @see @ref hgl::undirected_t "undirected_t" for the directional tag used to constrain this concept.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_undirected_hypergraph =
    c_hypergraph<H> and std::same_as<typename val_t<H>::directional_tag, undirected_t>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph is backward-forward (bf) directed.
/// @see @ref hgl::bf_directed_t "bf_directed_t" for the directional tag used to constrain this concept.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_bf_directed_hypergraph =
    c_hypergraph<H> and std::same_as<typename val_t<H>::directional_tag, bf_directed_t>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph uses a standard incidence list representation.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_list_hypergraph =
    c_hypergraph<H> and c_hypergraph_list_repr<typename val_t<H>::representation_tag>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph uses a flattened incidence list representation.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_flat_list_hypergraph =
    c_hypergraph<H> and c_hypergraph_flat_list_repr<typename val_t<H>::representation_tag>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph uses any incidence list representation (standard or flattened).
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_incidence_list_hypergraph =
    c_hypergraph<H> and c_hypergraph_incidence_list_repr<typename val_t<H>::representation_tag>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph uses a standard incidence matrix representation.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_matrix_hypergraph =
    c_hypergraph<H> and c_hypergraph_matrix_repr<typename val_t<H>::representation_tag>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph uses a flattened incidence matrix representation.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_flat_matrix_hypergraph =
    c_hypergraph<H> and c_hypergraph_flat_matrix_repr<typename val_t<H>::representation_tag>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph uses any incidence matrix representation (standard or flattened).
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_incidence_matrix_hypergraph =
    c_hypergraph<H> and c_hypergraph_incidence_matrix_repr<typename val_t<H>::representation_tag>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a mutable or immutable vertex descriptor associated with the given hypergraph.
/// @tparam V The type of the vertex descriptor.
/// @tparam H The type of the hypergraph.
template <typename V, typename H>
concept c_vertex =
    c_hypergraph<H>
    and c_one_of<
        std::remove_cvref_t<V>,
        typename val_t<H>::vertex_type,
        typename val_t<H>::const_vertex_type>;

template <typename R, typename H>
concept c_vertex_forward_range = c_forward_range<R> and c_vertex<std::ranges::range_value_t<R>, H>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a mutable or immutable hyperedge descriptor associated with the given hypergraph.
/// @tparam E The type of the hyperedge descriptor.
/// @tparam H The type of the hypergraph.
template <typename E, typename H>
concept c_hyperedge =
    c_hypergraph<H>
    and c_one_of<
        std::remove_cvref_t<E>,
        typename val_t<H>::hyperedge_type,
        typename val_t<H>::const_hyperedge_type>;

template <typename R, typename H>
concept c_hyperedge_forward_range =
    c_forward_range<R> and c_hyperedge<std::ranges::range_value_t<R>, H>;

} // namespace traits

/// @ingroup HGL-Core
/// @brief Creates a deep copy of a given hypergraph.
/// @tparam Hypergraph The concrete hypergraph type.
/// @param source The hypergraph to clone.
/// @return A duplicated instance of the source hypergraph.
template <traits::c_hypergraph Hypergraph>
[[nodiscard]] Hypergraph clone(const Hypergraph& source);

/// @ingroup HGL-Core
/// @brief Converts a hypergraph to a different representation type (e.g., from incidence list to incidence matrix).
/// @tparam TargetReprTag The representation tag defining the target storage mechanism.
/// @tparam Hypergraph The concrete hypergraph type of the source.
/// @param source The hypergraph to convert.
/// @return A new hypergraph matching the target representation type with identical topology and properties.
template <traits::c_hypergraph_repr_tag TargetReprTag, traits::c_hypergraph Hypergraph>
requires(not std::is_lvalue_reference_v<Hypergraph>)
[[nodiscard]] auto to(Hypergraph&& source);

namespace detail {

/// @brief Internal structure for dispatching the hypergraph representation conversion logic.
template <traits::c_hypergraph_repr_tag TargetReprTag, traits::c_hypergraph_repr_tag SourceReprTag>
struct to_impl;

} // namespace detail

/// @ingroup HGL-Core
/// @brief The generic hypergraph container using a policy-based design.
///
/// This class relies on the provided `HypergraphTraits` to determine its behavior, element
/// property types, and the underlying memory representation. It exposes a unified API for
/// adding, removing, and iterating over vertices and hyperedges regardless of the backend.
///
/// ### Key Features
/// - **Policy-based design**: Behavior and representation are determined by `HypergraphTraits`.
/// - **Zero-cost Abstractions**: Core query logic is resolved at compile time through representation tags and static dispatch, removing unnecessary overhead.
/// - **Configurable directionality**: Support for both undirected and BF-directed hypergraphs.
/// - **Multiple representations**: Choose the underlying memory model and its layout to achieve the best performance for your needs:
///   - @ref hgl::repr::list_t "list_t": Standard incidence list.
///   - @ref hgl::repr::flat_list_t "flat_list_t": Flattened incidence list.
///   - @ref hgl::repr::matrix_t "matrix_t": Standard incidence matrix.
///   - @ref hgl::repr::flat_matrix_t "flat_matrix_t": Flattened incidence matrix.
/// - **Property support**: Vertices and hyperedges can carry arbitrary properties.
/// - **Unified API**: Consistent interface regardless of the underlying representation.
/// - **Standard Range Support**: Exposes lightweight views compliant with C++20 `std::ranges`, enabling functional-style iteration and algorithms.
///
/// ### Basic Definitions
/// A hypergraph \f$H = (V, E)\f$ consists of a set of vertices \f$V\f$ and a set of hyperedges \f$E\f$.
///
/// - For undirected graphs, a hyperedge is a subset of the vertex set. Formally \f$E \subseteq 2^V\f$ and \f$e \in E \implies e \subseteq V\f$.
/// - For BF-directed graphs, a hyperedge is an ordered pair of disjoint subsets of the vertex set - the *tail* (sources) and *head* (targets) of the hyperedge.
///   Formally \f$e = (T_e, H_e)\f$ where \f$T_e, H_e \subset V \land T_e \cap H_e = \emptyset\f$.
///
/// ### Example Usage
/// ```cpp
/// hgl::undirected_hypergraph<> h(4, 2); // (1)!
///
/// h.bind({0, 1, 2}, 0); // (2)!
///
/// auto v_first = h.vertex(0); // (3)!
/// auto v_last = h.vertex(3);
/// auto e1 = h.hyperedge(1);
/// h.bind({v_first, v_last}, e1); // (4)!
///
/// std::cout << "Vertices: " << h.n_vertices() << '\n'; // (5)!
/// std::cout << "Hyperedges: " << h.n_hyperedges() << '\n';
///
/// for (auto v : h.vertices()) { // (6)!
///     for (auto e : h.incident_hyperedges(v)) {
///         process(v, e);
///     }
/// }
///
/// std::cout << "Topology:\n" << h << '\n'; // (7)!
/// ```
///
/// 1\. Instantiate an undirected hypergraph with 4 vertices and 2 hyperedges.
///
/// 2\. Bind vertices with IDs 0, 1 and 2 to the hyperedge with ID 0.
///
/// 3\. Retrieve the vertex and hyperedge descriptors in the hypergraph using the dedicated getters.
///
/// 4\. Bind the given vertices to the edge using descriptor objects.
///
/// 5\. Query the hypergraph's properties.
///
/// 6\. Iterate over the hypergraph's vertices and then over the incident edges of each vertex.
///
/// 7\.  Utilize the builtin I/O stream support of the `hypergraph` class to print its topology to the console.
///
/// ### API Design: IDs vs. Descriptors
/// The `hypergraph` class exposes a dual API to accommodate different performance and ergonomic needs:
///
/// - **Inputs**: Most query methods are overloaded to accept either a raw `id_type` or a `vertex_type`/`hyperedge_type` descriptor. They are functionally identical.
/// - **Outputs**: Methods ending in `_ids` (e.g., `incident_vertex_ids`) return views of raw integral IDs. Methods without this suffix (e.g., `incident_vertices`) automatically map those IDs to the proper descriptor objects.
/// - **Performance**: Descriptor-returning methods incur a slight overhead if the hypergraph utilizes properties, as the property reference must be fetched and bound to each descriptor. If you only need topology, prefer the `_ids` variants.
///
/// ### Descriptor Invalidation Behavior
/// The hypergraph maintains the following invalidation semantics:
///
/// - **Vertex addition**: Does not invalidate vertex IDs. However, property references stored in existing vertex descriptors may be invalidated. Has no effect hyperedge descriptors.
/// - **Vertex removal**: May invalidate vertex descriptors, IDs, and property references. Has no effect hyperedge descriptors.
/// - **Edge addition**: Does not invalidate hyperedge IDs. However, property references stored in existing hyperedge descriptors may be invalidated. Has no effect on vertex descriptors.
/// - **Edge removal**: Invalidates hyperedge descriptors, IDs, and property references. Has no effect on vertex descriptors.
/// - **Property access**: References to vertex or edge properties obtained from the map may be invalidated by modifications to the hypergraph structure.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :---------- | :--------- |
/// | HypergraphTraits | The core configuration type specifying the behavior and representation of the hypergraph.  | [**c_instantiation_of<hypergraph_traits>**](gl_concepts.md#gl-traits-c-instantiation-of) |
///
/// ### See Also
/// - @ref hgl::undirected_hypergraph "undirected_hypergraph" : Convenience alias for undirected hypergraphs.
/// - @ref hgl::bf_directed_hypergraph "bf_directed_hypergraph" : Convenience alias for BF-directed hypergraphs.
/// - @ref hgl::clone "clone" : Create a deep copy of a hypergraph.
/// - @ref hgl::to "to" : Convert a hypergraph to a different representation.
///
/// > [!IMPORTANT] Copy Semantics
/// >
/// > `hypergraph` supports move semantics but disables copy assignment to prevent accidental expensive copies.
/// > Use @ref hgl::clone "clone" to explicitly copy a hypergraph.
///
/// > [!WARNING] Const Correctness & Properties (API Note)
/// >
/// > Currently, a `const` hypergraph guarantees **structural immutability** (vertices and hyperedges cannot be added or removed).
/// > However, vertex and hyperedge property maps are internally treated as `mutable`. This means that property payloads
/// > can still be modified through a `const hypergraph&`. Strict const-correct overloads for property access are planned
/// > for a future release. Proceed with caution in multi-threaded contexts.
template <traits::c_instantiation_of<hypergraph_traits> HypergraphTraits>
class hypergraph final {
public:
    /// @brief The traits type specifying the hypergraph's behavior and representation.
    using traits_type = HypergraphTraits;

    /// @brief Type tag specifying the directionality of the hypergraph.
    using directional_tag = typename traits_type::directional_tag;
    /// @brief Type tag indicating the underlying representation model.
    using representation_tag = typename traits_type::representation_tag;

    /// @brief Integral type used to identify vertices and hyperedges.
    using id_type = typename traits_type::id_type;

    /// @brief The descriptor type representing a vertex.
    using vertex_type = typename traits_type::vertex_type;
    /// @brief The descriptor type representing an immutable vertex.
    using const_vertex_type = typename traits_type::const_vertex_type;
    /// @brief The user-defined property payload type associated with vertices.
    using vertex_properties_type = typename traits_type::vertex_properties_type;

    /// @brief The descriptor type representing a hyperedge.
    using hyperedge_type = typename traits_type::hyperedge_type;
    /// @brief The descriptor type representing an immutable hyperedge.
    using const_hyperedge_type = typename traits_type::const_hyperedge_type;
    /// @brief The user-defined property payload type associated with hyperedges.
    using hyperedge_properties_type = typename traits_type::hyperedge_properties_type;

private:
    using representation_type =
        typename representation_tag::template representation_type<directional_tag>;

    using vertex_properties_map_type = std::conditional_t<
        traits::c_empty_properties<vertex_properties_type>,
        empty_properties_map,
        std::vector<vertex_properties_type>>;

    using hyperedge_properties_map_type = std::conditional_t<
        traits::c_empty_properties<hyperedge_properties_type>,
        empty_properties_map,
        std::vector<hyperedge_properties_type>>;

public:
    /// @brief Constructs a hypergraph with the given number of vertices and hyperedges (empty by default).
    /// @param n_vertices The initial number of vertices.
    /// @param n_hyperedges The initial number of hyperedges.
    ///
    /// > [!NOTE] Memory Management
    /// >
    /// > Constructing a non-empty hypergraph and retrieving descriptors of the hypergraph's elements provides
    /// > significantly better performance than adding elements one by one (especially for large hypergraphs)
    /// > due to the ability to allocate the required memory for the internal incidence representation, which
    /// > drastically reduces the overhead associated with reallocations associated with adding elements sequentially.
    explicit hypergraph(const size_type n_vertices = 0uz, const size_type n_hyperedges = 0uz)
    : _n_vertices(n_vertices), _n_hyperedges(n_hyperedges), _impl(n_vertices, n_hyperedges) {
        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.resize(n_vertices);

        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>)
            this->_hyperedge_properties.resize(n_hyperedges);
    }

    /// @brief Default move constructor.
    hypergraph(hypergraph&&) noexcept = default;
    /// @brief Default move assignment operator.
    hypergraph& operator=(hypergraph&&) noexcept = default;

    /// @brief Default destructor.
    ~hypergraph() = default;

    /// @brief Hypergraph copy assignment is disabled to avoid accidental copies. Use @ref hgl::clone "clone" instead.
    hypergraph& operator=(const hypergraph&) = delete;

    // --- size methods ---

    /// @brief Returns the total number of vertices in the hypergraph.
    /// @return The vertex count: $|V|$.
    [[nodiscard]] gl_attr_force_inline size_type n_vertices() const noexcept {
        return this->_n_vertices;
    }

    /// @brief Returns the total number of hyperedges in the hypergraph.
    /// @return The hyperedge count: $|E|$.
    [[nodiscard]] gl_attr_force_inline size_type n_hyperedges() const noexcept {
        return this->_n_hyperedges;
    }

    // --- vertex modifiers ---

    /// @brief Adds a new, default-initialized vertex to the hypergraph.
    /// @return A descriptor of the newly created vertex.
    /// @copydetails detail::hypergraph_doc_anchors::add_vertex_note()
    vertex_type add_vertex() {
        this->_impl.add_vertices(1uz);
        const auto new_vertex_id = static_cast<id_type>(this->_n_vertices++);

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_type{new_vertex_id, this->_vertex_properties.emplace_back()};
        else
            return vertex_type{new_vertex_id};
    }

    /// @brief Adds a new vertex with the given properties to the hypergraph.
    /// @param properties The property payload for the new vertex.
    /// @return A descriptor of the newly created vertex.
    /// @copydetails detail::hypergraph_doc_anchors::add_vertex_note()
    vertex_type add_vertex_with(vertex_properties_type properties)
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        this->_impl.add_vertices(1uz);
        return vertex_type{
            static_cast<id_type>(this->_n_vertices++),
            this->_vertex_properties.emplace_back(std::move(properties))
        };
    }

    /// @brief Adds multiple default-initialized vertices to the hypergraph.
    /// @param n The number of vertices to add.
    /// @copydetails detail::hypergraph_doc_anchors::add_vertex_note()
    void add_vertices(const size_type n) {
        this->_impl.add_vertices(n);
        this->_n_vertices += n;

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.resize(this->_n_vertices);
    }

    /// @brief Adds multiple vertices based on a range of property payloads.
    /// @param properties_rng A forward range of properties to initialize the new vertices with.
    /// @copydetails detail::hypergraph_doc_anchors::add_vertex_note()
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

    /// @brief Removes a vertex by its ID, unbinding it from its incident hyperedges.
    /// @param vertex_id The ID of the vertex to remove.
    /// @throws std::invalid_argument If the ID is invalid.
    /// @copydetails detail::hypergraph_doc_anchors::remove_vertex_wrn()
    gl_attr_force_inline void remove_vertex(const id_type vertex_id) {
        this->_remove_vertex_impl(vertex_id);
    }

    /// @brief Removes a vertex using its descriptor, unbinding it from its incident hyperedges.
    /// @param vertex The descriptor of the vertex to remove.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    /// @copydetails detail::hypergraph_doc_anchors::remove_vertex_wrn()
    gl_attr_force_inline void remove_vertex(traits::c_vertex<hypergraph> auto vertex) {
        this->remove_vertex(vertex.id());
    }

    /// @brief Removes a range of vertices using their IDs.
    /// @param vertex_id_rng A forward range containing the IDs of vertices to remove.
    /// @throws std::invalid_argument If any vertex ID in the range is invalid.
    /// @copydetails detail::hypergraph_doc_anchors::remove_vertex_wrn()
    void remove_vertices(const traits::c_forward_range_of<id_type> auto& vertex_id_rng) {
        auto vertex_ids = vertex_id_rng | std::ranges::to<std::vector>();
        // Sort in descending order and remove duplicates to prevent index shifting bugs during erasure
        std::ranges::sort(vertex_ids, std::greater<>{});
        vertex_ids.erase(std::ranges::unique(vertex_ids).begin(), vertex_ids.end());

        if (not vertex_ids.empty())
            this->_verify_vertex_id(vertex_ids.front());

        for (const auto vertex_id : vertex_ids)
            this->_remove_vertex_impl(vertex_id);
    }

    /// @brief Removes a range of vertices using their descriptors.
    /// @tparam VertexRng A forward range type containing hypergraph's vertex descriptors.
    /// @param vertex_rng A forward range containing the descriptors of vertices to remove.
    /// @throws std::invalid_argument If any vertex descriptor is invalid.
    /// @copydetails detail::hypergraph_doc_anchors::remove_vertex_wrn()
    gl_attr_force_inline void remove_vertices(
        const traits::c_vertex_forward_range<hypergraph> auto& vertex_rng
    ) {
        this->remove_vertices(vertex_rng | std::views::transform(util::to_id));
    }

    // --- vertex getters ---

    /// @brief Checks if a vertex with the given ID exists in the hypergraph.
    /// @param vertex_id The ID to check.
    /// @return `true` if the vertex exists, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool has_vertex(const id_type vertex_id) const {
        return vertex_id < this->_n_vertices;
    }

    /// @brief Checks if the vertex referenced by the provided descriptor exists in the hypergraph.
    /// @param vertex The vertex descriptor to check.
    /// @return `true` if the vertex exists, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool has_vertex(traits::c_vertex<hypergraph> auto vertex
    ) const {
        return this->has_vertex(vertex.id());
    }

    /// @brief Returns a descriptor of the vertex with the given *bounds-checked* ID.
    /// @param vertex_id The ID of the vertex.
    /// @return The corresponding vertex descriptor.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    template <typename Self>
    [[nodiscard]] vertex_t<Self> vertex(this Self& self, const id_type vertex_id) {
        self._verify_vertex_id(vertex_id);
        return self.vertex_unchecked(vertex_id);
    }

    /// @brief Returns a descriptor of the vertex with the given bounds-checked ID.
    ///
    /// > [!NOTE] API Note
    /// >
    /// > Calling `hypergraph.at(hgl::vertex, id)` is equivalent to calling `hypergraph.vertex(id)`.
    /// > However, the `at` methods of the `hypergraph` class are designed to be called in generic
    /// > functions that can operate on vertices and hyperedges alike.
    ///
    /// @param vertex_id The ID of the vertex.
    /// @return The corresponding `vertex_descriptor`.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline vertex_t<Self> at(
        this Self& self, vertex_tag, const id_type vertex_id
    ) {
        return self.vertex(vertex_id);
    }

    /// @brief Returns a descriptor of the vertex with the given ID without bounds checking.
    /// @param vertex_id The ID of the vertex.
    /// @return The corresponding vertex descriptor.
    ///
    /// > [!WARNING] Undefined Behavior
    /// >
    /// > No bounds checking is performed. Passing an invalid ID results in Undefined Behavior.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline vertex_t<Self> vertex_unchecked(
        this Self& self, const id_type vertex_id
    ) noexcept {
        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_t<Self>{vertex_id, self._vertex_properties[vertex_id]};
        else
            return vertex_t<Self>{vertex_id};
    }

    /// @brief Returns a descriptor of the vertex with the given ID without bounds checking.
    /// @param vertex_id The ID of the vertex.
    /// @return The corresponding vertex descriptor.
    ///
    /// > [!NOTE] API Note
    /// >
    /// > Calling `hypergraph[hgl::vertex, id]` is equivalent to calling `hypergraph.vertex_unchecked(id)`.
    /// > However, the subscript operators of the `hypergraph` class are designed to be called in generic
    /// > functions that can operate on vertices and hyperedges alike.
    ///
    /// > [!WARNING] Undefined Behavior
    /// >
    /// > No bounds checking is performed. Passing an invalid ID results in Undefined Behavior.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline vertex_t<Self> operator[](
        this Self& self, vertex_tag, const id_type vertex_id
    ) noexcept {
        return self.vertex_unchecked(vertex_id);
    }

    /// @brief Returns a lazily evaluated, random-access view of all vertex descriptors in the hypergraph.
    /// @return A view yielding descriptors for every vertex.
    [[nodiscard]] gl_attr_force_inline auto vertices(this auto& self) noexcept {
        return self.vertex_ids() | std::views::transform(self._create_vertex_descriptor());
    }

    /// @brief Returns a lazily evaluated, random-access view of all active vertex IDs in the hypergraph.
    /// @return A view yielding all valid vertex IDs.
    [[nodiscard]] gl_attr_force_inline auto vertex_ids() const noexcept {
        return std::views::iota(initial_id_v<id_type>, this->_n_vertices);
    }

    /// @brief Retrieves a reference to the properties of a specified vertex.
    /// @param vertex_id The ID of the vertex.
    /// @return A mutable reference to the vertex's properties.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline vertex_properties_t<Self>& vertex_properties(
        this Self& self, const id_type vertex_id
    )
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        self._verify_vertex_id(vertex_id);
        return self._vertex_properties[vertex_id];
    }

    /// @brief Retrieves a lazily evaluated, random-access view over all vertex properties in the hypergraph.
    /// @return A view mapping each active vertex index to its property.
    [[nodiscard]] gl_attr_force_inline auto vertex_properties_map(this auto& self) noexcept
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        return std::views::all(self._vertex_properties);
    }

    // --- hyperedge modifiers ---

    /// @brief Adds a new, default-initialized hyperedge to the hypergraph.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    hyperedge_type add_hyperedge() {
        this->_impl.add_hyperedges(1uz);
        const auto new_hyperedge_id = static_cast<id_type>(this->_n_hyperedges++);

        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>)
            return hyperedge_type{new_hyperedge_id, this->_hyperedge_properties.emplace_back()};
        else
            return hyperedge_type{new_hyperedge_id};
    }

    /// @brief Adds a new hyperedge with the given properties to the hypergraph.
    /// @param properties The property payload for the new hyperedge.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    hyperedge_type add_hyperedge_with(hyperedge_properties_type properties)
    requires(traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        this->_impl.add_hyperedges(1uz);
        return hyperedge_type{
            static_cast<id_type>(this->_n_hyperedges++),
            this->_hyperedge_properties.emplace_back(std::move(properties))
        };
    }

    /// @brief Adds a new *undirected* hyperedge and immediately binds a range of vertices to it.
    /// @param vertex_id_rng A forward range of vertex IDs to bind to the new hyperedge.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    hyperedge_type add_hyperedge(const traits::c_forward_range_of<id_type> auto& vertex_id_rng)
    requires std::same_as<directional_tag, undirected_t>
    {
        auto he = this->add_hyperedge();
        this->bind(vertex_id_rng, he.id());
        return he;
    }

    /// @brief Adds a new *undirected* hyperedge and immediately binds a list of vertices to it.
    /// @param vertex_ids An initializer list of vertex IDs to bind to the new hyperedge.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    gl_attr_force_inline hyperedge_type add_hyperedge(std::initializer_list<id_type> vertex_ids)
    requires std::same_as<directional_tag, undirected_t>
    {
        return this->add_hyperedge(std::views::all(vertex_ids));
    }

    /// @brief Adds a new *undirected* hyperedge and immediately binds a range of vertices to it.
    /// @param vertex_rng A forward range of vertex descriptors to bind to the new hyperedge.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    gl_attr_force_inline hyperedge_type
    add_hyperedge(const traits::c_vertex_forward_range<hypergraph> auto& vertex_rng)
    requires std::same_as<directional_tag, undirected_t>
    {
        return this->add_hyperedge(vertex_rng | std::views::transform(util::to_id));
    }

    /// @brief Adds a new *undirected* hyperedge and immediately binds a list of vertices to it.
    /// @tparam V The vertex descriptor type.
    /// @param vertices An initializer list of vertex descriptors to bind to the new hyperedge.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    template <traits::c_vertex<hypergraph> V>
    gl_attr_force_inline hyperedge_type add_hyperedge(std::initializer_list<V> vertices)
    requires std::same_as<directional_tag, undirected_t>
    {
        return this->add_hyperedge(std::views::all(vertices));
    }

    /// @brief Adds a new *undirected* hyperedge with the given properties and immediately binds a range of vertices to it.
    /// @param vertex_id_rng A forward range of vertex IDs to bind to the new hyperedge.
    /// @param properties The property payload for the new hyperedge.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    hyperedge_type add_hyperedge_with(
        const traits::c_forward_range_of<id_type> auto& vertex_id_rng,
        hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, undirected_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        auto he = this->add_hyperedge_with(std::move(properties));
        this->bind(vertex_id_rng, he.id());
        return he;
    }

    /// @brief Adds a new *undirected* hyperedge with the given properties and immediately binds a list of vertices to it.
    /// @param vertex_ids An initializer list of vertex IDs to bind to the new hyperedge.
    /// @param properties The property payload for the new hyperedge.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    hyperedge_type add_hyperedge_with(
        std::initializer_list<id_type> vertex_ids, hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, undirected_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return this->add_hyperedge_with(std::views::all(vertex_ids), std::move(properties));
    }

    /// @brief Adds a new *undirected* hyperedge with the given properties and immediately binds a range of vertices to it.
    /// @param vertex_rng A forward range of vertex descriptors to bind to the new hyperedge.
    /// @param properties The property payload for the new hyperedge.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    gl_attr_force_inline hyperedge_type add_hyperedge_with(
        const traits::c_vertex_forward_range<hypergraph> auto& vertex_rng,
        hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, undirected_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return this->add_hyperedge_with(
            vertex_rng | std::views::transform(util::to_id), std::move(properties)
        );
    }

    /// @brief Adds a new *undirected* hyperedge with the given properties and immediately binds a list of vertices to it.
    /// @tparam V The vertex descriptor type.
    /// @param vertices An initializer list of vertex descriptors to bind to the new hyperedge.
    /// @param properties The property payload for the new hyperedge.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    template <traits::c_vertex<hypergraph> V>
    hyperedge_type add_hyperedge_with(
        std::initializer_list<V> vertices, hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, undirected_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return this->add_hyperedge_with(std::views::all(vertices), std::move(properties));
    }

    /// @brief Adds a new *BF-directed* hyperedge and immediately binds vertices to its *tail* and *head*.
    /// @param tail_id_rng A forward range of vertex IDs to bind to the new hyperedge's *tail*.
    /// @param head_id_rng A forward range of vertex IDs to bind to the new hyperedge's *head*.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    hyperedge_type add_hyperedge(
        const traits::c_forward_range_of<id_type> auto& tail_id_rng,
        const traits::c_forward_range_of<id_type> auto& head_id_rng
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        auto he = this->add_hyperedge();
        this->bind_tail(tail_id_rng, he.id());
        this->bind_head(head_id_rng, he.id());
        return he;
    }

    /// @brief Adds a new *BF-directed* hyperedge and immediately binds vertices to its *tail* and *head*.
    /// @param tail_ids An initializer of vertex IDs to bind to the new hyperedge's *tail*.
    /// @param head_ids An initializer of vertex IDs to bind to the new hyperedge's *head*.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    gl_attr_force_inline hyperedge_type
    add_hyperedge(std::initializer_list<id_type> tail_ids, std::initializer_list<id_type> head_ids)
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->add_hyperedge(std::views::all(tail_ids), std::views::all(head_ids));
    }

    /// @brief Adds a new *BF-directed* hyperedge and immediately binds vertices to its *tail* and *head*.
    /// @param tail_rng A forward range of vertex descriptors to bind to the new hyperedge's *tail*.
    /// @param head_rng A forward range of vertex descriptors to bind to the new hyperedge's *head*.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    gl_attr_force_inline hyperedge_type add_hyperedge(
        const traits::c_vertex_forward_range<hypergraph> auto& tail_rng,
        const traits::c_vertex_forward_range<hypergraph> auto& head_rng
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->add_hyperedge(
            tail_rng | std::views::transform(util::to_id),
            head_rng | std::views::transform(util::to_id)
        );
    }

    /// @brief Adds a new *BF-directed* hyperedge and immediately binds vertices to its *tail* and *head*.
    /// @tparam VT The tail vertex descriptor type.
    /// @tparam VH The head vertex descriptor type.
    /// @param tail An intializer list of vertex descriptors to bind to the new hyperedge's *tail*.
    /// @param head An intializer list of vertex descriptors to bind to the new hyperedge's *head*.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    template <traits::c_vertex<hypergraph> VT, traits::c_vertex<hypergraph> VH>
    gl_attr_force_inline hyperedge_type
    add_hyperedge(std::initializer_list<VT> tail, std::initializer_list<VH> head)
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->add_hyperedge(std::views::all(tail), std::views::all(head));
    }

    /// @brief Adds a new *BF-directed* hyperedge and immediately binds vertices to its *tail* and *head*.
    /// @param tail_id_rng A forward range of vertex IDs to bind to the new hyperedge's *tail*.
    /// @param head_id_rng A forward range of vertex IDs to bind to the new hyperedge's *head*.
    /// @param properties The property payload for the new hyperedge.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    hyperedge_type add_hyperedge_with(
        const traits::c_forward_range_of<id_type> auto& tail_id_rng,
        const traits::c_forward_range_of<id_type> auto& head_id_rng,
        hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, bf_directed_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        auto he = this->add_hyperedge_with(std::move(properties));
        this->bind_tail(tail_id_rng, he.id());
        this->bind_head(head_id_rng, he.id());
        return he;
    }

    /// @brief Adds a new *BF-directed* hyperedge and immediately binds vertices to its *tail* and *head*.
    /// @param tail_ids An initializer list of vertex IDs to bind to the new hyperedge's *tail*.
    /// @param head_ids An initializer list of vertex IDs to bind to the new hyperedge's *head*.
    /// @param properties The property payload for the new hyperedge.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    gl_attr_force_inline hyperedge_type add_hyperedge_with(
        std::initializer_list<id_type> tail_ids,
        std::initializer_list<id_type> head_ids,
        hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, bf_directed_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return this->add_hyperedge_with(
            std::views::all(tail_ids), std::views::all(head_ids), std::move(properties)
        );
    }

    /// @brief Adds a new *BF-directed* hyperedge and immediately binds vertices to its *tail* and *head*.
    /// @param tail_rng A forward range of vertex descriptors to bind to the new hyperedge's *tail*.
    /// @param head_rng A forward range of vertex descriptors to bind to the new hyperedge's *head*.
    /// @param properties The property payload for the new hyperedge.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    gl_attr_force_inline hyperedge_type add_hyperedge_with(
        const traits::c_vertex_forward_range<hypergraph> auto& tail_rng,
        const traits::c_vertex_forward_range<hypergraph> auto& head_rng,
        hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, bf_directed_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return this->add_hyperedge_with(
            tail_rng | std::views::transform(util::to_id),
            head_rng | std::views::transform(util::to_id),
            std::move(properties)
        );
    }

    /// @brief Adds a new *BF-directed* hyperedge and immediately binds vertices to its *tail* and *head*.
    /// @tparam VT The tail vertex descriptor type.
    /// @tparam VH The head vertex descriptor type.
    /// @param tail An initializer list of vertex descriptor to bind to the new hyperedge's *tail*.
    /// @param head An initializer list of vertex descriptor to bind to the new hyperedge's *head*.
    /// @param properties The property payload for the new hyperedge.
    /// @return A descriptor of the newly created hyperedge.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    template <traits::c_vertex<hypergraph> VT, traits::c_vertex<hypergraph> VH>
    gl_attr_force_inline hyperedge_type add_hyperedge_with(
        std::initializer_list<VT> tail,
        std::initializer_list<VH> head,
        hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, bf_directed_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return this->add_hyperedge_with(
            std::views::all(tail), std::views::all(head), std::move(properties)
        );
    }

    /// @brief Adds multiple default-initialized hyperedges to the hypergraph.
    /// @param n The number of hyperedges to add.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    void add_hyperedges(const size_type n) {
        this->_impl.add_hyperedges(n);
        this->_n_hyperedges += n;

        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>)
            this->_hyperedge_properties.resize(this->_n_hyperedges);
    }

    /// @brief Adds multiple hyperedges based on a range of property payloads.
    /// @param properties_rng A forward range of properties to initialize the new hyperedges with.
    /// @copydetails detail::hypergraph_doc_anchors::add_hyperedge_note()
    void add_hyperedges_with(
        const traits::c_sized_range_of<hyperedge_properties_type> auto& properties_rng
    )
    requires(traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        const auto n = std::ranges::size(properties_rng);

        this->_impl.add_hyperedges(n);
        this->_n_hyperedges += n;

        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>)
            this->_hyperedge_properties.insert(
                this->_hyperedge_properties.end(),
                std::ranges::begin(properties_rng),
                std::ranges::end(properties_rng)
            );
    }

    /// @brief Removes a hyperedge by its ID, unbinding it from its incident vertices.
    /// @param hyperedge_id The ID of the hyperedge to remove.
    /// @throws std::invalid_argument If the ID is invalid.
    /// @copydetails detail::hypergraph_doc_anchors::remove_hyperedge_wrn()
    gl_attr_force_inline void remove_hyperedge(const id_type hyperedge_id) {
        this->_remove_hyperedge_impl(hyperedge_id);
    }

    /// @brief Removes a hyperedge using its descriptor, unbinding it from its incident vertices.
    /// @param hyperedge The descriptor of the hyperedge to remove.
    /// @throws std::invalid_argument If the ID is invalid.
    /// @copydetails detail::hypergraph_doc_anchors::remove_hyperedge_wrn()
    gl_attr_force_inline void remove_hyperedge(traits::c_hyperedge<hypergraph> auto hyperedge) {
        this->remove_hyperedge(hyperedge.id());
    }

    /// @brief Removes a range of hyperedges using their IDs.
    /// @param hyperedge_id_rng A forward range containing the IDs of hyperedges to remove.
    /// @throws std::invalid_argument If any hyperedge ID in the range is invalid.
    /// @copydetails detail::hypergraph_doc_anchors::remove_hyperedge_wrn()
    void remove_hyperedges(const traits::c_forward_range_of<id_type> auto& hyperedge_id_rng) {
        auto hyperedge_ids = hyperedge_id_rng | std::ranges::to<std::vector>();
        // Sort in descending order and remove duplicates to prevent index shifting bugs during erasure
        std::ranges::sort(hyperedge_ids, std::greater<>{});
        hyperedge_ids.erase(std::ranges::unique(hyperedge_ids).begin(), hyperedge_ids.end());

        if (not hyperedge_ids.empty())
            this->_verify_hyperedge_id(hyperedge_ids.front());

        // TODO: optimize
        for (const auto hyperedge_id : hyperedge_ids)
            this->_remove_hyperedge_impl(hyperedge_id);
    }

    /// @brief Removes a range of hyperedges using their descriptors.
    /// @param hyperedge_id_rng A forward range containing the descriptors of hyperedges to remove.
    /// @throws std::invalid_argument If any hyperedge ID in the range is invalid.
    /// @copydetails detail::hypergraph_doc_anchors::remove_hyperedge_wrn()
    gl_attr_force_inline void remove_hyperedges(
        const traits::c_hyperedge_forward_range<hypergraph> auto& hyperedge_rng
    ) {
        this->remove_hyperedges(hyperedge_rng | std::views::transform(util::to_id));
    }

    // --- hyperedge getters ---

    /// @brief Checks if a hyperedge with the given ID exists in the hypergraph.
    /// @param hyperedge_id The ID to check.
    /// @return `true` if the hyperedge exists, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool has_hyperedge(const id_type hyperedge_id) const {
        return hyperedge_id < this->_n_hyperedges;
    }

    /// @brief Checks if the hyperedge referenced by the provided descriptor exists in the hypergraph.
    /// @param hyperedge The descriptor to check.
    /// @return `true` if the hyperedge exists, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool has_hyperedge(
        traits::c_hyperedge<hypergraph> auto hyperedge
    ) const {
        return this->has_hyperedge(hyperedge.id());
    }

    /// @brief Returns a descriptor of the hyperedge with the given *bounds-checked* ID.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @return The corresponding hyperedge descriptor.
    /// @throws std::invalid_argument If the hyperedge ID is invalid.
    template <typename Self>
    [[nodiscard]] hyperedge_t<Self> hyperedge(this Self& self, const id_type hyperedge_id) {
        self._verify_hyperedge_id(hyperedge_id);
        return self.hyperedge_unchecked(hyperedge_id);
    }

    /// @brief Returns a descriptor of the hyperedge with the given *bounds-checked* ID.
    ///
    /// > [!NOTE] API Note
    /// >
    /// > Calling `hypergraph.at(hgl::hyperedge, id)` is equivalent to calling `hypergraph.hyperedge(id)`.
    /// > However, the `at` methods of the `hypergraph` class are designed to be called in generic
    /// > functions that can operate on hyperedges and vertices alike.
    ///
    /// @param hyperedge_id The ID of the hyperedge.
    /// @return The corresponding hyperedge descriptor.
    /// @throws std::invalid_argument If the hyperedge ID is invalid.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline hyperedge_t<Self> at(
        this Self& self, hyperedge_tag, const id_type hyperedge_id
    ) {
        return self.hyperedge(hyperedge_id);
    }

    /// @brief Returns a descriptor of the hyperedge with the given ID without bounds checking.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @return The corresponding hyperedge descriptor.
    ///
    /// > [!WARNING] Undefined Behavior
    /// >
    /// > No bounds checking is performed. Passing an invalid ID results in Undefined Behavior.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline hyperedge_t<Self> hyperedge_unchecked(
        this Self& self, const id_type hyperedge_id
    ) {
        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>)
            return hyperedge_t<Self>{hyperedge_id, self._hyperedge_properties[hyperedge_id]};
        else
            return hyperedge_t<Self>{hyperedge_id};
    }

    /// @brief Returns a descriptor of the hyperedge with the given ID without bounds checking.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @return The corresponding hyperedge descriptor.
    ///
    /// > [!NOTE] API Note
    /// >
    /// > Calling `hypergraph[hgl::hyperedge, id]` is equivalent to calling `hypergraph.hyperedge_unchecked(id)`.
    /// > However, the subscript operators of the `hypergraph` class are designed to be called in generic
    /// > functions that can operate on hyperedges and vertices alike.
    ///
    /// > [!WARNING] Undefined Behavior
    /// >
    /// > No bounds checking is performed. Passing an invalid ID results in Undefined Behavior.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline hyperedge_t<Self> operator[](
        this Self& self, hyperedge_tag, const id_type hyperedge_id
    ) {
        return self.hyperedge_unchecked(hyperedge_id);
    }

    /// @brief Returns a lazily evaluated, random-access view of all hyperedge descriptors in the hypergraph.
    /// @return A view yielding descriptors for every hyperedge.
    [[nodiscard]] gl_attr_force_inline auto hyperedges(this auto& self) noexcept {
        return self.hyperedge_ids() | std::views::transform(self._create_hyperedge_descriptor());
    }

    /// @brief Returns a lazily evaluated, random-access view of all active hyperedge IDs in the hypergraph.
    /// @return A view yielding all valid hyperedge IDs.
    [[nodiscard]] gl_attr_force_inline auto hyperedge_ids() const noexcept {
        return std::views::iota(initial_id_v<id_type>, this->_n_hyperedges);
    }

    /// @brief Retrieves a reference to the properties of a specified hyperedge.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @return A mutable reference to the hyperedge's properties.
    /// @throws std::invalid_argument If the hyperedge ID is invalid.
    template <typename Self>
    [[nodiscard]] gl_attr_force_inline hyperedge_properties_t<Self>& hyperedge_properties(
        this Self& self, const id_type hyperedge_id
    )
    requires(traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        self._verify_hyperedge_id(hyperedge_id);
        return self._hyperedge_properties[hyperedge_id];
    }

    /// @brief Retrieves a lazily evaluated, random-access view over all hyperedge properties in the hypergraph.
    /// @return A view mapping each active hyperedge index to its property.
    [[nodiscard]] gl_attr_force_inline auto hyperedge_properties_map(this auto& self) noexcept
    requires(traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return std::views::all(self._hyperedge_properties);
    }

    // --- incidence modifiers ---

    /// @brief Binds a vertex to a hyperedge in an undirected hypergraph.
    /// @param vertex_id The ID of the vertex.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @throws std::invalid_argument If either ID is invalid.
    void bind(const id_type vertex_id, const id_type hyperedge_id)
    requires std::same_as<directional_tag, undirected_t>
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        this->_impl.bind(vertex_id, hyperedge_id);
    }

    /// @brief Binds a vertex to a hyperedge in an undirected hypergraph.
    /// @param vertex The descriptor of the vertex.
    /// @param hyperedge The descriptor of the hyperedge.
    /// @throws std::invalid_argument If either descriptor is invalid.
    gl_attr_force_inline void bind(
        traits::c_vertex<hypergraph> auto vertex, traits::c_hyperedge<hypergraph> auto hyperedge
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex.id(), hyperedge.id());
    }

    /// @brief Binds a range of vertices to a single hyperedge in an undirected hypergraph.
    /// @param vertex_id_rng A forward range of vertex IDs.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @throws std::invalid_argument If either the hyperedge ID or any of the vertex IDs is invalid.
    void bind(
        const traits::c_forward_range_of<id_type> auto& vertex_id_rng, const id_type hyperedge_id
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->_verify_hyperedge_id(hyperedge_id);
        for (const auto vertex_id : vertex_id_rng) {
            this->_verify_vertex_id(vertex_id);
            this->_impl.bind(vertex_id, hyperedge_id);
        }
    }

    /// @brief Binds a list of vertices to a single hyperedge in an undirected hypergraph.
    /// @param vertex_ids An initializer list of vertex IDs.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @throws std::invalid_argument If either the hyperedge ID or any of the vertex IDs is invalid.
    gl_attr_force_inline void bind(
        std::initializer_list<id_type> vertex_ids, const id_type hyperedge_id
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(std::views::all(vertex_ids), hyperedge_id);
    }

    /// @brief Binds a range of vertices to a single hyperedge in an undirected hypergraph.
    /// @param vertex_rng A forward range of vertex descriptors.
    /// @param hyperedge The descriptor of the hyperedge.
    /// @throws std::invalid_argument If either the hyperedge descriptor or any of the vertex descriptors is invalid.
    gl_attr_force_inline void bind(
        const traits::c_vertex_forward_range<hypergraph> auto& vertex_rng,
        traits::c_hyperedge<hypergraph> auto hyperedge
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex_rng | std::views::transform(util::to_id), hyperedge.id());
    }

    /// @brief Binds a list of vertices to a single hyperedge in an undirected hypergraph.
    /// @tparam V The vertex descriptor type.
    /// @param vertices An initializer list of vertex descriptors.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @throws std::invalid_argument If either the hyperedge descriptor or any of the vertex descriptors is invalid.
    template <traits::c_vertex<hypergraph> V>
    gl_attr_force_inline void bind(
        std::initializer_list<V> vertices, traits::c_hyperedge<hypergraph> auto hyperedge
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(std::views::all(vertices), hyperedge);
    }

    /// @brief Binds a single vertex to a range of hyperedges in an undirected hypergraph.
    /// @param vertex_id The ID of the vertex.
    /// @param hyperedge_id_rng A forward range of hyperedge IDs.
    /// @throws std::invalid_argument If either the vertex ID or any of the hyperedge IDs is invalid.
    void bind(
        const id_type vertex_id, const traits::c_forward_range_of<id_type> auto& hyperedge_id_rng
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->_verify_vertex_id(vertex_id);
        for (const auto hyperedge_id : hyperedge_id_rng) {
            this->_verify_hyperedge_id(hyperedge_id);
            this->_impl.bind(vertex_id, hyperedge_id);
        }
    }

    /// @brief Binds a single vertex to a list of hyperedges in an undirected hypergraph.
    /// @param vertex_id The ID of the vertex.
    /// @param hyperedge_id_rng An initializer list of hyperedge IDs.
    /// @throws std::invalid_argument If either the vertex ID or any of the hyperedge IDs is invalid.
    gl_attr_force_inline void bind(
        const id_type vertex_id, std::initializer_list<id_type> hyperedge_ids
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex_id, std::views::all(hyperedge_ids));
    }

    /// @brief Binds a single vertex to a range of hyperedges in an undirected hypergraph.
    /// @param vertex_id The descriptor of the vertex.
    /// @param hyperedge_id_rng A forward range of hyperedge descriptors.
    /// @throws std::invalid_argument If either the vertex descriptor or any of the hyperedge descriptors is invalid.
    gl_attr_force_inline void bind(
        traits::c_vertex<hypergraph> auto vertex,
        const traits::c_hyperedge_forward_range<hypergraph> auto& hyperedge_rng
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex.id(), hyperedge_rng | std::views::transform(util::to_id));
    }

    /// @brief Binds a single vertex to a list of hyperedges in an undirected hypergraph.
    /// @tparam E The hyperedge descriptor type.
    /// @param vertex_id The descriptor of the vertex.
    /// @param hyperedge_id_rng An initializer list of hyperedge descriptors.
    /// @throws std::invalid_argument If either the vertex descriptor or any of the hyperedge descriptors is invalid.
    template <traits::c_hyperedge<hypergraph> E>
    gl_attr_force_inline void bind(
        traits::c_vertex<hypergraph> auto vertex, std::initializer_list<E> hyperedges
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex, std::views::all(hyperedges));
    }

    /// @brief Binds a vertex to the *tail* of a hyperedge in a BF-directed hypergraph.
    /// @param vertex_id The ID of the vertex.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @throws std::invalid_argument If either ID is invalid.
    void bind_tail(const id_type vertex_id, const id_type hyperedge_id)
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        this->_impl.bind_tail(vertex_id, hyperedge_id);
    }

    /// @brief Binds a vertex to the *tail* of a hyperedge in a BF-directed hypergraph.
    /// @param vertex The descriptor of the vertex.
    /// @param hyperedge The descriptor of the hyperedge.
    /// @throws std::invalid_argument If either descriptor is invalid.
    gl_attr_force_inline void bind_tail(
        traits::c_vertex<hypergraph> auto vertex, traits::c_hyperedge<hypergraph> auto hyperedge
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_tail(vertex.id(), hyperedge.id());
    }

    /// @brief Binds a range of vertices to the *tail* of a single hyperedge in a BF-directed hypergraph.
    /// @param vertex_id_rng A forward range of vertex IDs.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @throws std::invalid_argument If either the hyperedge ID or any of the vertex IDs is invalid.
    void bind_tail(
        const traits::c_forward_range_of<id_type> auto& vertex_id_rng, const id_type hyperedge_id
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_hyperedge_id(hyperedge_id);
        for (const auto vertex_id : vertex_id_rng) {
            this->_verify_vertex_id(vertex_id);
            this->_impl.bind_tail(vertex_id, hyperedge_id);
        }
    }

    /// @brief Binds a list of vertices to the *tail* of a single hyperedge in a BF-directed hypergraph.
    /// @param vertex_ids An initializer list of vertex IDs.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @throws std::invalid_argument If either the hyperedge ID or any of the vertex IDs is invalid.
    gl_attr_force_inline void bind_tail(
        std::initializer_list<id_type> vertex_ids, const id_type hyperedge_id
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_tail(std::views::all(vertex_ids), hyperedge_id);
    }

    /// @brief Binds a range of vertices to the *tail* of a single hyperedge in a BF-directed hypergraph.
    /// @param vertex_rng A forward range of vertex descriptors.
    /// @param hyperedge The descriptor of the hyperedge.
    /// @throws std::invalid_argument If either the hyperedge descriptor or any of the vertex descriptors is invalid.
    gl_attr_force_inline void bind_tail(
        const traits::c_vertex_forward_range<hypergraph> auto& vertex_rng,
        traits::c_hyperedge<hypergraph> auto hyperedge
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_tail(vertex_rng | std::views::transform(util::to_id), hyperedge.id());
    }

    /// @brief Binds a range of vertices to the *tail* of a single hyperedge in a BF-directed hypergraph.
    /// @tparam V The vertex descriptor type.
    /// @param vertices An initializer list of vertex descriptors.
    /// @param hyperedge The descriptor of the hyperedge.
    /// @throws std::invalid_argument If either the hyperedge descriptor or any of the vertex descriptors is invalid.
    template <traits::c_vertex<hypergraph> V>
    gl_attr_force_inline void bind_tail(
        std::initializer_list<V> vertices, traits::c_hyperedge<hypergraph> auto hyperedge
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_tail(std::views::all(vertices), hyperedge);
    }

    /// @brief Binds a single vertex to the *tail* of a range of hyperedges in a BF-directed hypergraph.
    /// @param vertex_id The ID of the vertex.
    /// @param hyperedge_id_rng A forward range of hyperedge IDs.
    /// @throws std::invalid_argument If either the vertex ID or any of the hyperedge IDs is invalid.
    void bind_tail(
        const id_type vertex_id, const traits::c_forward_range_of<id_type> auto& hyperedge_id_rng
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_vertex_id(vertex_id);
        for (const auto hyperedge_id : hyperedge_id_rng) {
            this->_verify_hyperedge_id(hyperedge_id);
            this->_impl.bind_tail(vertex_id, hyperedge_id);
        }
    }

    /// @brief Binds a single vertex to the *tail* of a range of hyperedges in a BF-directed hypergraph.
    /// @param vertex_id The ID of the vertex.
    /// @param hyperedge_ids An initializer list of hyperedge IDs.
    /// @throws std::invalid_argument If either the vertex ID or any of the hyperedge IDs is invalid.
    gl_attr_force_inline void bind_tail(
        const id_type vertex_id, std::initializer_list<id_type> hyperedge_ids
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_tail(vertex_id, std::views::all(hyperedge_ids));
    }

    /// @brief Binds a single vertex to the *tail* of a range of hyperedges in a BF-directed hypergraph.
    /// @param vertex The descriptor of the vertex.
    /// @param hyperedge_rng A forward range of hyperedge descriptors.
    /// @throws std::invalid_argument If either the vertex descriptor or any of the hyperedge descriptors is invalid.
    gl_attr_force_inline void bind_tail(
        traits::c_vertex<hypergraph> auto vertex,
        const traits::c_hyperedge_forward_range<hypergraph> auto& hyperedge_rng
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_tail(vertex.id(), hyperedge_rng | std::views::transform(util::to_id));
    }

    /// @brief Binds a single vertex to the *tail* of a range of hyperedges in a BF-directed hypergraph.
    /// @tparam E The hyperedge descriptor type.
    /// @param vertex The descriptor of the vertex.
    /// @param hyperedges An initializer list of hyperedge descriptors.
    /// @throws std::invalid_argument If either the vertex descriptor or any of the hyperedge descriptors is invalid.
    template <traits::c_hyperedge<hypergraph> E>
    gl_attr_force_inline void bind_tail(
        traits::c_vertex<hypergraph> auto vertex, std::initializer_list<E> hyperedges
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_tail(vertex, std::views::all(hyperedges));
    }

    /// @brief Binds a vertex to the *head* of a hyperedge in a BF-directed hypergraph.
    /// @param vertex_id The ID of the vertex.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @throws std::invalid_argument If either ID is invalid.
    void bind_head(const id_type vertex_id, const id_type hyperedge_id)
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        this->_impl.bind_head(vertex_id, hyperedge_id);
    }

    /// @brief Binds a vertex to the *head* of a hyperedge in a BF-directed hypergraph.
    /// @param vertex The descriptor of the vertex.
    /// @param hyperedge The descriptor of the hyperedge.
    /// @throws std::invalid_argument If either descriptor is invalid.
    gl_attr_force_inline void bind_head(
        traits::c_vertex<hypergraph> auto vertex, traits::c_hyperedge<hypergraph> auto hyperedge
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_head(vertex.id(), hyperedge.id());
    }

    /// @brief Binds a range of vertices to the *head* of a single hyperedge in a BF-directed hypergraph.
    /// @param vertex_id_rng A forward range of vertex IDs.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @throws std::invalid_argument If either the hyperedge ID or any of the vertex IDs is invalid.
    void bind_head(
        const traits::c_forward_range_of<id_type> auto& vertex_id_rng, const id_type hyperedge_id
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_hyperedge_id(hyperedge_id);
        for (const auto vertex_id : vertex_id_rng) {
            this->_verify_vertex_id(vertex_id);
            this->_impl.bind_head(vertex_id, hyperedge_id);
        }
    }

    /// @brief Binds a list of vertices to the *head* of a single hyperedge in a BF-directed hypergraph.
    /// @param vertex_ids An initializer list of vertex IDs.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @throws std::invalid_argument If either the hyperedge ID or any of the vertex IDs is invalid.
    gl_attr_force_inline void bind_head(
        std::initializer_list<id_type> vertex_ids, const id_type hyperedge_id
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_head(std::views::all(vertex_ids), hyperedge_id);
    }

    /// @brief Binds a range of vertices to the *head* of a single hyperedge in a BF-directed hypergraph.
    /// @param vertex_rng A forward range of vertex descriptors.
    /// @param hyperedge The descriptor of the hyperedge.
    /// @throws std::invalid_argument If either the hyperedge descriptor or any of the vertex descriptors is invalid.
    gl_attr_force_inline void bind_head(
        const traits::c_vertex_forward_range<hypergraph> auto& vertex_rng,
        traits::c_hyperedge<hypergraph> auto hyperedge
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_head(vertex_rng | std::views::transform(util::to_id), hyperedge.id());
    }

    /// @brief Binds a range of vertices to the *head* of a single hyperedge in a BF-directed hypergraph.
    /// @tparam V The vertex descriptor type.
    /// @param vertices An initializer list of vertex descriptors.
    /// @param hyperedge The descriptor of the hyperedge.
    /// @throws std::invalid_argument If either the hyperedge descriptor or any of the vertex descriptors is invalid.
    template <traits::c_vertex<hypergraph> V>
    gl_attr_force_inline void bind_head(
        std::initializer_list<V> vertices, traits::c_hyperedge<hypergraph> auto hyperedge
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_head(std::views::all(vertices), hyperedge);
    }

    /// @brief Binds a single vertex to the *head* of a range of hyperedges in a BF-directed hypergraph.
    /// @param vertex_id The ID of the vertex.
    /// @param hyperedge_id_rng A forward range of hyperedge IDs.
    /// @throws std::invalid_argument If either the vertex ID or any of the hyperedge IDs is invalid.
    void bind_head(
        const id_type vertex_id, const traits::c_forward_range_of<id_type> auto& hyperedge_id_rng
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_vertex_id(vertex_id);
        for (const auto hyperedge_id : hyperedge_id_rng) {
            this->_verify_hyperedge_id(hyperedge_id);
            this->_impl.bind_head(vertex_id, hyperedge_id);
        }
    }

    /// @brief Binds a single vertex to the *head* of a range of hyperedges in a BF-directed hypergraph.
    /// @param vertex_id The ID of the vertex.
    /// @param hyperedge_ids An initializer list of hyperedge IDs.
    /// @throws std::invalid_argument If either the vertex ID or any of the hyperedge IDs is invalid.
    gl_attr_force_inline void bind_head(
        const id_type vertex_id, std::initializer_list<id_type> hyperedge_ids
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_head(vertex_id, std::views::all(hyperedge_ids));
    }

    /// @brief Binds a single vertex to the *head* of a range of hyperedges in a BF-directed hypergraph.
    /// @param vertex The descriptor of the vertex.
    /// @param hyperedge_rng A forward range of hyperedge descriptors.
    /// @throws std::invalid_argument If either the vertex descriptor or any of the hyperedge descriptors is invalid.
    gl_attr_force_inline void bind_head(
        traits::c_vertex<hypergraph> auto vertex,
        const traits::c_hyperedge_forward_range<hypergraph> auto& hyperedge_rng
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_head(vertex.id(), hyperedge_rng | std::views::transform(util::to_id));
    }

    /// @brief Binds a single vertex to the *head* of a range of hyperedges in a BF-directed hypergraph.
    /// @tparam E The hyperedge descriptor type.
    /// @param vertex The descriptor of the vertex.
    /// @param hyperedges An initializer list of hyperedge descriptors.
    /// @throws std::invalid_argument If either the vertex descriptor or any of the hyperedge descriptors is invalid.
    template <traits::c_hyperedge<hypergraph> E>
    gl_attr_force_inline void bind_head(
        traits::c_vertex<hypergraph> auto vertex, std::initializer_list<E> hyperedges
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->bind_head(vertex, std::views::all(hyperedges));
    }

    /// @brief Unbinds a vertex from a hyperedge.
    /// @param vertex_id The ID of the vertex.
    /// @param hyperedge_id The ID of the hyperedge.
    void unbind(const id_type vertex_id, const id_type hyperedge_id) {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        this->_impl.unbind(vertex_id, hyperedge_id);
    }

    /// @brief Unbinds a vertex from a hyperedge.
    /// @param vertex The descriptor of the vertex.
    /// @param hyperedge The descriptor of the hyperedge.
    gl_attr_force_inline void unbind(
        traits::c_vertex<hypergraph> auto vertex, traits::c_hyperedge<hypergraph> auto hyperedge
    ) {
        this->unbind(vertex.id(), hyperedge.id());
    }

    // --- incidence validators ---

    /// @brief Evaluates whether the given vertex and hyperedge are incident.
    ///
    /// ### Formal Definition
    /// - **Undirected Hypergraphs:** A vertex $v$ and a hyperedge $e$ are incident if \f$v \in e\f$.
    /// - **BF-directed Hypergraphs:** A vertex $v$ and a hyperedge $e$ are incident if \f$v \in T(e) \lor v \in H(e)\f$.
    ///
    /// @param vertex_id The ID of the vertex.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @return `true` if the vertex belongs to the hyperedge (in any direction), `false` otherwise.
    [[nodiscard]] bool are_incident(const id_type vertex_id, const id_type hyperedge_id) const {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.are_bound(vertex_id, hyperedge_id);
    }

    /// @brief Evaluates whether a vertex and a hyperedge are currently incident using descriptors.
    /// @param vertex The `vertex_descriptor` mapping to the vertex.
    /// @param hyperedge The `hyperedge_descriptor` mapping to the hyperedge.
    /// @return `true` if the vertex belongs to the hyperedge, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool are_incident(
        traits::c_vertex<hypergraph> auto vertex, traits::c_hyperedge<hypergraph> auto hyperedge
    ) const {
        return this->are_incident(vertex.id(), hyperedge.id());
    }

    /// @brief Evaluates whether a vertex belongs to the *tail* of a hyperedge in a BF-directed hypergraph.
    ///
    /// Formally, the function evaluates whether \f$v \in T(e)\f$ or is one of the sources of the hyperedge.
    ///
    /// @param vertex_id The ID of the vertex.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @return `true` if the vertex is in the *tail* set of the hyperedge, `false` otherwise.
    [[nodiscard]] bool is_tail(const id_type vertex_id, const id_type hyperedge_id) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.is_tail(vertex_id, hyperedge_id);
    }

    /// @brief Evaluates whether a vertex belongs to the *tail* of a hyperedge in a BF-directed hypergraph.
    ///
    /// Formally, the function evaluates whether \f$v \in T(e)\f$ or is one of the sources of the hyperedge.
    ///
    /// @param vertex The `vertex_descriptor` mapping to the vertex.
    /// @param hyperedge The `hyperedge_descriptor` mapping to the hyperedge.
    /// @return `true` if the vertex is in the *tail* set of the hyperedge, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool is_tail(
        traits::c_vertex<hypergraph> auto vertex, traits::c_hyperedge<hypergraph> auto hyperedge
    ) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->is_tail(vertex.id(), hyperedge.id());
    }

    /// @brief Evaluates whether a vertex belongs to the *head* of a hyperedge in a BF-directed hypergraph.
    ///
    /// Formally, the function evaluates whether \f$v \in H(e)\f$ or is one of the targets of the hyperedge.
    ///
    /// @param vertex_id The ID of the vertex.
    /// @param hyperedge_id The ID of the hyperedge.
    /// @return `true` if the vertex is in the *head* set of the hyperedge, `false` otherwise.
    [[nodiscard]] bool is_head(const id_type vertex_id, const id_type hyperedge_id) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.is_head(vertex_id, hyperedge_id);
    }

    /// @brief Evaluates whether a vertex belongs to the *head* of a hyperedge in a BF-directed hypergraph.
    ///
    /// Formally, the function evaluates whether \f$v \in H(e)\f$ or is one of the targets of the hyperedge.
    ///
    /// @param vertex The `vertex_descriptor` mapping to the vertex.
    /// @param hyperedge The `hyperedge_descriptor` mapping to the hyperedge.
    /// @return `true` if the vertex is in the *head* set of the hyperedge, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool is_head(
        traits::c_vertex<hypergraph> auto vertex, traits::c_hyperedge<hypergraph> auto hyperedge
    ) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->is_head(vertex.id(), hyperedge.id());
    }

    // --- incidence getters ---

    /// @brief Retrieves all hyperedges incident with a vertex (\f$\{e in E : v \in e\}\f$).
    /// @param vertex_id The vertex ID.
    /// @return A view representing the set of incident hyperedges.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(
        this auto& self, const id_type vertex_id
    ) {
        return self.incident_hyperedge_ids(vertex_id)
             | std::views::transform(self._create_hyperedge_descriptor());
    }

    /// @brief Retrieves all hyperedges incident with a vertex (\f$\{e in E : v \in e\}\f$).
    /// @param vertex The vertex descriptor.
    /// @return A view representing the set of incident hyperedges.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(
        this auto& self, traits::c_vertex<hypergraph> auto vertex
    ) {
        return self.incident_hyperedges(vertex.id());
    }

    /// @brief Retrieves IDs of all hyperedges incident with a vertex (\f$\{e in E : v \in e\}\f$).
    /// @param vertex_id The vertex ID.
    /// @return A view representing the set of incident hyperedge IDs.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] auto incident_hyperedge_ids(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.incident_hyperedges(vertex_id);
    }

    /// @brief Retrieves IDs of all hyperedges incident with a vertex (\f$\{e in E : v \in e\}\f$).
    /// @param vertex The vertex descriptor.
    /// @return A view representing the set of incident hyperedge IDs.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto incident_hyperedge_ids(
        traits::c_vertex<hypergraph> auto vertex
    ) const {
        return this->incident_hyperedge_ids(vertex.id());
    }

    /// @brief Calculates the degree of a vertex in the hypergraph.
    /// @copydetails detail::hypergraph_doc_anchors::degree()
    /// @param vertex_id The ID of the vertex.
    /// @return The degree of the vertex.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] size_type degree(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.degree(vertex_id);
    }

    /// @brief Calculates the degree of a vertex in the hypergraph.
    /// @copydetails detail::hypergraph_doc_anchors::degree()
    /// @param vertex The descriptor of the vertex.
    /// @return The degree of the vertex.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline size_type degree(traits::c_vertex<hypergraph> auto vertex
    ) const {
        return this->degree(vertex.id());
    }

    /// @brief Returns a mapped array of degrees for all vertices.
    /// @return A vector where the index aligns with the vertex ID containing its degree.
    [[nodiscard]] std::vector<size_type> degree_map() const {
        return this->_impl.degree_map(this->_n_vertices);
    }

    /// @brief Retrieves all outgoing (tail-bound) hyperedges of a vertex in a *BF-directed* hypergraph (\f$\{e \in E : v \in T(e)\}\f$).
    /// @param vertex_id The vertex ID.
    /// @return A view representing the set of outgoing hyperedges.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto out_hyperedges(this auto& self, const id_type vertex_id)
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return self.out_hyperedge_ids(vertex_id)
             | std::views::transform(self._create_hyperedge_descriptor());
    }

    /// @brief Retrieves all outgoing (tail-bound) hyperedges of a vertex in a *BF-directed* hypergraph (\f$\{e \in E : v \in T(e)\}\f$).
    /// @param vertex The vertex descriptor.
    /// @return A view representing the set of outgoing hyperedges.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto out_hyperedges(
        this auto& self, traits::c_vertex<hypergraph> auto vertex
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return self.out_hyperedges(vertex.id());
    }

    /// @brief Retrieves IDs of all outgoing (tail-bound) hyperedges of a vertex in a *BF-directed* hypergraph (\f$\{e \in E : v \in T(e)\}\f$).
    /// @param vertex_id The vertex ID.
    /// @return A view representing the set of outgoing hyperedge IDs.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] auto out_hyperedge_ids(const id_type vertex_id) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.out_hyperedges(vertex_id);
    }

    /// @brief Retrieves IDs of all outgoing (tail-bound) hyperedges of a vertex in a *BF-directed* hypergraph (\f$\{e \in E : v \in T(e)\}\f$).
    /// @param vertex The vertex descriptor.
    /// @return A view representing the set of outgoing hyperedge IDs.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto out_hyperedge_ids(
        traits::c_vertex<hypergraph> auto vertex
    ) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->out_hyperedge_ids(vertex.id());
    }

    /// @brief Calculates the out-degree of a vertex in the *BF-directed* hypergraph.
    /// @copydetails detail::hypergraph_doc_anchors::out_degree()
    /// @param vertex_id The ID of the vertex.
    /// @return The out-degree of the vertex.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] size_type out_degree(const id_type vertex_id) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.out_degree(vertex_id);
    }

    /// @brief Calculates the out-degree of a vertex in the *BF-directed* hypergraph.
    /// @copydetails detail::hypergraph_doc_anchors::out_degree()
    /// @param vertex The descriptor of the vertex.
    /// @return The out-degree of the vertex.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline size_type out_degree(traits::c_vertex<hypergraph> auto vertex
    ) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->out_degree(vertex.id());
    }

    /// @brief Returns a mapped array of out-degrees for all vertices.
    /// @return A vector where the index aligns with the vertex ID containing its out-degree.
    [[nodiscard]] std::vector<size_type> out_degree_map() const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->_impl.out_degree_map(this->_n_vertices);
    }

    /// @brief Retrieves all incoming (head-bound) hyperedges of a vertex in a *BF-directed* hypergraph (\f$\{e \in E : v \in H(e)\}\f$).
    /// @param vertex_id The vertex ID.
    /// @return A view representing the set of incoming hyperedges.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto in_hyperedges(this auto& self, const id_type vertex_id)
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return self.in_hyperedge_ids(vertex_id)
             | std::views::transform(self._create_hyperedge_descriptor());
    }

    /// @brief Retrieves all incoming (head-bound) hyperedges of a vertex in a *BF-directed* hypergraph (\f$\{e \in E : v \in H(e)\}\f$).
    /// @param vertex The vertex descriptor.
    /// @return A view representing the set of incoming hyperedges.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto in_hyperedges(
        this auto& self, traits::c_vertex<hypergraph> auto vertex
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return self.in_hyperedges(vertex.id());
    }

    /// @brief Retrieves IDs of all incoming (head-bound) hyperedges of a vertex in a *BF-directed* hypergraph (\f$\{e \in E : v \in H(e)\}\f$).
    /// @param vertex_id The vertex ID.
    /// @return A view representing the set of incoming hyperedge IDs.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] auto in_hyperedge_ids(const id_type vertex_id) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.in_hyperedges(vertex_id);
    }

    /// @brief Retrieves IDs of all incoming (head-bound) hyperedges of a vertex in a *BF-directed* hypergraph (\f$\{e \in E : v \in H(e)\}\f$).
    /// @param vertex The vertex descriptor.
    /// @return A view representing the set of incoming hyperedge IDs.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto in_hyperedge_ids(
        traits::c_vertex<hypergraph> auto vertex
    ) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->in_hyperedge_ids(vertex.id());
    }

    /// @brief Calculates the in-degree of a vertex in the *BF-directed* hypergraph.
    /// @copydetails detail::hypergraph_doc_anchors::in_degree()
    /// @param vertex_id The ID of the vertex.
    /// @return The in-degree of the vertex.
    /// @throws std::invalid_argument If the vertex ID is invalid.
    [[nodiscard]] size_type in_degree(const id_type vertex_id) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.in_degree(vertex_id);
    }

    /// @brief Calculates the in-degree of a vertex in the *BF-directed* hypergraph.
    /// @copydetails detail::hypergraph_doc_anchors::in_degree()
    /// @param vertex The descriptor of the vertex.
    /// @return The in-degree of the vertex.
    /// @throws std::invalid_argument If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline size_type in_degree(traits::c_vertex<hypergraph> auto vertex
    ) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->in_degree(vertex.id());
    }

    /// @brief Returns a mapped array of in-degrees for all vertices.
    /// @return A vector where the index aligns with the vertex ID containing its in-degree.
    [[nodiscard]] std::vector<size_type> in_degree_map() const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->_impl.in_degree_map(this->_n_vertices);
    }

    /// @brief Retrieves all vertices incident with a hyperedge ($e$).
    /// @param hyperedge_id The hyperedge ID.
    /// @return A view representing the set of incident vertices.
    /// @throws std::invalid_argument If the hyperedge ID is invalid.
    [[nodiscard]] auto incident_vertices(this auto& self, const id_type hyperedge_id) {
        return self.incident_vertex_ids(hyperedge_id)
             | std::views::transform(self._create_vertex_descriptor());
    }

    /// @brief Retrieves all vertices incident with a hyperedge ($e$).
    /// @param hyperedge The hyperedge descriptor.
    /// @return A view representing the set of incident vertices.
    /// @throws std::invalid_argument If the hyperedge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto incident_vertices(
        this auto& self, traits::c_hyperedge<hypergraph> auto hyperedge
    ) {
        return self.incident_vertices(hyperedge.id());
    }

    /// @brief Retrieves IDs of all vertices incident with a hyperedge ($e$).
    /// @param hyperedge_id The hyperedge ID.
    /// @return A view representing the set of incident vertex IDs.
    /// @throws std::invalid_argument If the hyperedge ID is invalid.
    [[nodiscard]] auto incident_vertex_ids(const id_type hyperedge_id) const {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.incident_vertices(hyperedge_id);
    }

    /// @brief Retrieves IDs of all vertices incident with a hyperedge ($e$).
    /// @param hyperedge The hyperedge descriptor.
    /// @return A view representing the set of incident vertex IDs.
    /// @throws std::invalid_argument If the hyperedge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto incident_vertex_ids(
        traits::c_hyperedge<hypergraph> auto hyperedge
    ) const {
        return this->incident_vertex_ids(hyperedge.id());
    }

    /// @brief Retrieves the size (number of incident vertices) of the given hyperedge.
    ///
    /// - For undirected hypergraphs this is equivalent to $\vert e \vert$.
    /// - For BF-directed hypergraphs this is equivalent to $\vert T(e) \vert + \vert H(e) \vert$
    ///
    /// @param hyperedge_id The ID of the hyperedge.
    /// @return The size of the hyperedge.
    /// @throws std::invalid_argument If the hyperedge ID is invalid.
    [[nodiscard]] size_type hyperedge_size(const id_type hyperedge_id) const {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.hyperedge_size(hyperedge_id);
    }

    /// @brief Retrieves the size (number of incident vertices) of the given hyperedge.
    ///
    /// - For undirected hypergraphs this is equivalent to $\vert e \vert$.
    /// - For BF-directed hypergraphs this is equivalent to $\vert T(e) \vert + \vert H(e) \vert$
    ///
    /// @param hyperedge The descriptor of the hyperedge.
    /// @return The size of the hyperedge.
    /// @throws std::invalid_argument If the hyperedge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline size_type
    hyperedge_size(traits::c_hyperedge<hypergraph> auto hyperedge) const {
        return this->hyperedge_size(hyperedge.id());
    }

    /// @brief Returns a mapped array of hyperedge sizes.
    /// @return A vector where the index aligns with the hyperedge ID containing its size.
    [[nodiscard]] std::vector<size_type> hyperedge_size_map() const {
        return this->_impl.hyperedge_size_map(this->_n_hyperedges);
    }

    /// @brief Retrieves all vertices in the *tail* set of a hyperedge ($T(e)$).
    /// @param hyperedge_id The hypepredge ID.
    /// @return A view representing the set of the hyperedge's tail vertices.
    /// @throws std::invalid_argument If the hyperedge ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto tail(this auto& self, const id_type hyperedge_id)
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return self.tail_ids(hyperedge_id)
             | std::views::transform(self._create_vertex_descriptor());
    }

    /// @brief Retrieves all vertices in the *tail* set of a hyperedge ($T(e)$).
    /// @param hyperedge The hyperedge descriptor.
    /// @return A view representing the set of the hyperedge's tail vertices.
    /// @throws std::invalid_argument If the hyperedge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto tail(
        this auto& self, traits::c_hyperedge<hypergraph> auto hyperedge
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return self.tail(hyperedge.id());
    }

    /// @brief Retrieves IDs all vertices in the *tail* set of a hyperedge ($T(e)$).
    /// @param hyperedge_id The hypepredge ID.
    /// @return A view representing the set of the hyperedge's tail vertex IDs.
    /// @throws std::invalid_argument If the hyperedge ID is invalid.
    [[nodiscard]] auto tail_ids(const id_type hyperedge_id) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.tail(hyperedge_id);
    }

    /// @brief Retrieves IDs all vertices in the *tail* set of a hyperedge ($T(e)$).
    /// @param hyperedge The hypepredge descriptor.
    /// @return A view representing the set of the hyperedge's tail vertex IDs.
    /// @throws std::invalid_argument If the hyperedge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto tail_ids(traits::c_hyperedge<hypergraph> auto hyperedge
    ) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->tail_ids(hyperedge.id());
    }

    /// @brief Retrieves the size of the *BF-directed* hyperedge's *tail* set ($\vert T(e) \vert).
    /// @param hyperedge_id The ID of the hyperedge.
    /// @return The size of the hyperedge's *tail* set.
    /// @throws std::invalid_argument If the hyperedge ID is invalid.
    [[nodiscard]] size_type tail_size(const id_type hyperedge_id) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.tail_size(hyperedge_id);
    }

    /// @brief Retrieves the size of the *BF-directed* hyperedge's *tail* set ($\vert T(e) \vert).
    /// @param hyperedge The descriptor of the hyperedge.
    /// @return The size of the hyperedge's *tail* set.
    /// @throws std::invalid_argument If the hyperedge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline size_type
    tail_size(traits::c_hyperedge<hypergraph> auto hyperedge) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->tail_size(hyperedge.id());
    }

    /// @brief Returns a mapped array of sizes of the *tail* sets of hyperedged in a *BF-directed* hypergraph.
    /// @return A vector where the index aligns with the hyperedge ID containing its *tail* set size.
    [[nodiscard]] std::vector<size_type> tail_size_map() const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->_impl.tail_size_map(this->_n_hyperedges);
    }

    /// @brief Retrieves all vertices in the *head* set of a hyperedge ($H(e)$).
    /// @param hyperedge_id The hypepredge ID.
    /// @return A view representing the set of the hyperedge's head vertices.
    /// @throws std::invalid_argument If the hyperedge ID is invalid.
    [[nodiscard]] gl_attr_force_inline auto head(this auto& self, const id_type hyperedge_id)
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return self.head_ids(hyperedge_id)
             | std::views::transform(self._create_vertex_descriptor());
    }

    /// @brief Retrieves all vertices in the *head* set of a hyperedge ($H(e)$).
    /// @param hyperedge The hyperedge descriptor.
    /// @return A view representing the set of the hyperedge's head vertices.
    /// @throws std::invalid_argument If the hyperedge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto head(
        this auto& self, traits::c_hyperedge<hypergraph> auto hyperedge
    )
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return self.head(hyperedge.id());
    }

    /// @brief Retrieves IDs all vertices in the *head* set of a hyperedge ($H(e)$).
    /// @param hyperedge_id The hypepredge ID.
    /// @return A view representing the set of the hyperedge's head vertex IDs.
    /// @throws std::invalid_argument If the hyperedge ID is invalid.
    [[nodiscard]] auto head_ids(const id_type hyperedge_id) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.head(hyperedge_id);
    }

    /// @brief Retrieves IDs all vertices in the *head* set of a hyperedge ($H(e)$).
    /// @param hyperedge The hypepredge descriptor.
    /// @return A view representing the set of the hyperedge's head vertex IDs.
    /// @throws std::invalid_argument If the hyperedge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline auto head_ids(traits::c_hyperedge<hypergraph> auto hyperedge
    ) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->head_ids(hyperedge.id());
    }

    /// @brief Retrieves the size of the *BF-directed* hyperedge's *head* set ($\vert H(e) \vert).
    /// @param hyperedge_id The ID of the hyperedge.
    /// @return The size of the hyperedge's *head* set.
    /// @throws std::invalid_argument If the hyperedge ID is invalid.
    [[nodiscard]] size_type head_size(const id_type hyperedge_id) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.head_size(hyperedge_id);
    }

    /// @brief Retrieves the size of the *BF-directed* hyperedge's *head* set ($\vert H(e) \vert).
    /// @param hyperedge The descriptor of the hyperedge.
    /// @return The size of the hyperedge's *head* set.
    /// @throws std::invalid_argument If the hyperedge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline size_type
    head_size(traits::c_hyperedge<hypergraph> auto hyperedge) const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->head_size(hyperedge.id());
    }

    /// @brief Returns a mapped array of sizes of the *head* sets of hyperedged in a *BF-directed* hypergraph.
    /// @return A vector where the index aligns with the hyperedge ID containing its *head* set size.
    [[nodiscard]] std::vector<size_type> head_size_map() const
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        return this->_impl.head_size_map(this->_n_hyperedges);
    }

    // --- comparison ---

    /// @brief Compares two hypergraphs for strict structural and property equality.
    /// @param lhs The left operand.
    /// @param rhs The right operand.
    /// @return `true` if both graphs represent the exact same topology and properties, `false` otherwise.
    [[nodiscard]] friend bool operator==(const hypergraph& lhs, const hypergraph& rhs) noexcept {
        if (lhs._n_vertices != rhs._n_vertices or lhs._n_hyperedges != rhs._n_hyperedges)
            return false;

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            if (lhs._vertex_properties != rhs._vertex_properties)
                return false;

        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>)
            if (lhs._hyperedge_properties != rhs._hyperedge_properties)
                return false;

        return lhs._impl == rhs._impl;
    }

    // --- I/O utility ---

    /// @brief Helper structure used to properly format an individual hyperedge within the hypergraph's context into an output stream.
    struct hyperedge_formatter {
    public:
        /// @brief The hypergraph owning the hyperedge.
        const hypergraph& hg;
        /// @brief The hyperedge to be formatted.
        const_hyperedge_type hyperedge;

        /// @brief Stream insertion operator for undirected hyperedges.
        friend std::ostream& operator<<(std::ostream& os, const hyperedge_formatter& proxy)
        requires std::same_as<directional_tag, undirected_t>
        {
            using io::detail::option_bit;

            if (io::is_option_set(os, option_bit::verbose)) {
                os << "[id: " << proxy.hyperedge.id() << " | vertices: "
                   << io::set_formatter(proxy.hg.incident_vertex_ids(proxy.hyperedge.id()));
                if constexpr (traits::c_writable<hyperedge_properties_type>)
                    if (io::is_option_set(os, option_bit::with_connection_properties))
                        os << " | " << proxy.hyperedge.properties();
                os << ']';
            }
            else { // concise
                os << io::set_formatter(proxy.hg.incident_vertex_ids(proxy.hyperedge.id()));
                if constexpr (traits::c_writable<hyperedge_properties_type>)
                    if (io::is_option_set(os, option_bit::with_connection_properties))
                        os << '[' << proxy.hyperedge.properties() << ']';
            }

            return os;
        }

        /// @brief Stream insertion operator for BF-directed hyperedges.
        friend std::ostream& operator<<(std::ostream& os, const hyperedge_formatter& proxy)
        requires(std::same_as<directional_tag, bf_directed_t>)
        {
            using io::detail::option_bit;

            if (io::is_option_set(os, option_bit::verbose)) {
                os << "[id: " << proxy.hyperedge.id()
                   << " | tail: " << io::set_formatter(proxy.hg.tail_ids(proxy.hyperedge.id()))
                   << ", head: " << io::set_formatter(proxy.hg.head_ids(proxy.hyperedge.id()));
                if constexpr (traits::c_writable<hyperedge_properties_type>)
                    if (io::is_option_set(os, option_bit::with_connection_properties))
                        os << " | " << proxy.hyperedge.properties();
                os << "]";
            }
            else { // concise
                os << '(' << io::set_formatter(proxy.hg.tail_ids(proxy.hyperedge.id())) << " -> "
                   << io::set_formatter(proxy.hg.head_ids(proxy.hyperedge.id())) << ')';
                if constexpr (traits::c_writable<hyperedge_properties_type>)
                    if (io::is_option_set(os, option_bit::with_connection_properties))
                        os << '[' << proxy.hyperedge.properties() << ']';
            }

            return os;
        }
    };

    /// @brief Returns a formatter object that safely encapsulates a hyperedge for stream output.
    /// @param hyperedge The hyperedge to format.
    /// @return A @ref hyperedge_formatter structure prepared for standard stream insertion.
    [[nodiscard]] hyperedge_formatter fmt(traits::c_hyperedge<hypergraph> auto hyperedge) const {
        return hyperedge_formatter{*this, hyperedge};
    }

    /// @brief Formats and outputs the entire hypergraph structure to a standard output stream.
    ///
    /// The generated string representation of the hypergraph depends on the currently active formatting options of the stream.
    ///
    /// @param os The target output stream.
    /// @param hg The hypergraph instance to write.
    /// @return The stream reference for chaining.
    friend std::ostream& operator<<(std::ostream& os, const hypergraph& hg) {
        using enum io::detail::option_bit;

        if (gl::io::is_option_set(os, spec_fmt))
            return hg._hgsf_write(os);

        if (gl::io::is_option_set(os, verbose))
            return hg._verbose_write(os);

        return hg._concise_write(os);
    }

    /// @brief Deserializes hypergraph structure data from an input stream (using the HGSF format).
    /// @param is The source input stream.
    /// @param g The hypergraph instance to populate.
    /// @return The stream reference for chaining.
    gl_attr_force_inline friend std::istream& operator>>(std::istream& is, hypergraph& hg) {
        return hg._hgsf_read(is);
    }

    // --- friend declarations ---

    /// @brief Creates a deep copy of a given hypergraph.
    template <traits::c_hypergraph Hypergraph>
    friend Hypergraph clone(const Hypergraph& source);

    /// @brief Converts a hypergraph to a different representation type.
    template <traits::c_hypergraph_repr_tag TargetReprTag, traits::c_hypergraph Hypergraph>
    requires(not std::is_lvalue_reference_v<Hypergraph>)
    friend auto to(Hypergraph&& source);

    /// @brief Internal structure for dispatching representation conversion.
    template <
        traits::c_hypergraph_repr_tag TargetReprTag,
        traits::c_hypergraph_repr_tag SourceReprTag>
    friend struct detail::to_impl;

private:
    hypergraph(const hypergraph& other) = default;

    // --- vertex methods ---

    gl_attr_force_inline void _verify_vertex_id(const id_type vertex_id) const {
        if (not this->has_vertex(vertex_id))
            throw std::invalid_argument(std::format("Got invalid vertex id [{}]", vertex_id));
    }

    void _remove_vertex_impl(const id_type vertex_id) {
        if (not this->has_vertex(vertex_id))
            return;

        this->_impl.remove_vertex(vertex_id);
        this->_n_vertices--;
        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.erase(this->_vertex_properties.begin() + vertex_id);
    }

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

    // --- hyperedge methods ---

    gl_attr_force_inline void _verify_hyperedge_id(const id_type hyperedge_id) const {
        if (not this->has_hyperedge(hyperedge_id))
            throw std::invalid_argument(std::format("Got invalid hyperedge id [{}]", hyperedge_id));
    }

    void _remove_hyperedge_impl(const id_type hyperedge_id) {
        if (not this->has_hyperedge(hyperedge_id))
            return;

        this->_impl.remove_hyperedge(hyperedge_id);
        this->_n_hyperedges--;
        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>)
            this->_hyperedge_properties.erase(this->_hyperedge_properties.begin() + hyperedge_id);
    }

    template <typename Self>
    gl_attr_force_inline auto _create_hyperedge_descriptor(this Self&) noexcept
    requires(traits::c_empty_properties<hyperedge_properties_type>)
    {
        return [](const id_type id) { return hyperedge_t<Self>{id}; };
    }

    template <typename Self>
    gl_attr_force_inline auto _create_hyperedge_descriptor(this Self& self) noexcept
    requires(traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return [&pmap = self._hyperedge_properties](const id_type id) {
            return hyperedge_t<Self>{id, pmap[to_idx(id)]};
        };
    }

    // --- I/O utility ---

    std::ostream& _verbose_write(std::ostream& os) const {
        using enum io::detail::option_bit;
        using fmt_traits = io::detail::hypergraph_fmt_traits<directional_tag>;

        os << "type: " << fmt_traits::type << ", |V| = " << this->_n_vertices
           << ", |E| = " << this->_n_hyperedges << '\n';

        os << "vertices: ";
        if constexpr (traits::c_writable<vertex_properties_type>) {
            if (io::is_option_set(os, with_vertex_properties)) {
                os << '\n';
                for (const auto& vertex : this->vertices())
                    os << "  - " << vertex << '\n';
            }
            else {
                os << io::implicit_range(this->_n_vertices) << '\n';
            }
        }
        else {
            os << io::implicit_range(this->_n_vertices) << '\n';
        }

        if (this->_n_hyperedges == 0uz) {
            os << "hyperedges: {}";
        }
        else {
            os << "hyperedges:\n";
            for (const auto& edge : this->hyperedges())
                os << "  - " << this->fmt(edge) << '\n';
        }

        return os;
    }

    std::ostream& _concise_write(std::ostream& os) const {
        using enum io::detail::option_bit;

        os << "V = ";
        if constexpr (traits::c_writable<vertex_properties_type>) {
            if (io::is_option_set(os, with_vertex_properties))
                os << io::multiline_set_formatter(this->vertices()) << '\n';
            else
                os << io::implicit_range(this->_n_vertices) << '\n';
        }
        else {
            os << io::implicit_range(this->_n_vertices) << '\n';
        }

        if (this->_n_hyperedges == 0uz) {
            os << "E = {}\n";
        }
        else {
            auto hyperedges = std::views::transform(this->hyperedges(), [this](auto hyperedge) {
                return this->fmt(hyperedge);
            });

            os << "E = " << io::multiline_set_formatter(hyperedges) << '\n';
        }

        return os;
    }

    std::ostream& _hgsf_write(std::ostream& os) const {
        using enum io::detail::option_bit;
        using fmt_traits = io::detail::hypergraph_fmt_traits<directional_tag>;

        const bool with_v_props = io::is_option_set(os, with_vertex_properties);
        const bool with_he_props = io::is_option_set(os, with_connection_properties);

        // print hypergraph metadata
        os << fmt_traits::discriminator << ' ' << this->_n_vertices << ' ' << this->_n_hyperedges
           << ' ' << static_cast<int>(with_v_props) << ' ' << static_cast<int>(with_he_props)
           << '\n';

        if constexpr (traits::c_writable<vertex_properties_type>)
            if (with_v_props)
                for (const auto& vertex : this->vertices())
                    os << vertex.properties() << '\n';

        for (const auto& hyperedge : this->hyperedges()) {
            const auto he_id = hyperedge.id();

            if constexpr (std::same_as<directional_tag, undirected_t>) {
                os << this->hyperedge_size(he_id);
                for (const auto v : this->incident_vertex_ids(he_id))
                    os << ' ' << v;
            }
            else if constexpr (std::same_as<directional_tag, bf_directed_t>) {
                os << this->tail_size(he_id) << ' ' << this->head_size(he_id);
                for (const auto v : this->tail_ids(he_id))
                    os << ' ' << v;
                for (const auto v : this->head_ids(he_id))
                    os << ' ' << v;
            }

            if constexpr (traits::c_writable<hyperedge_properties_type>) {
                if (with_he_props)
                    os << ' ' << hyperedge.properties();
            }

            os << '\n';
        }

        return os;
    }

    std::istream& _hgsf_read(std::istream& is) {
        using fmt_traits = io::detail::hypergraph_fmt_traits<directional_tag>;

        int dir_discr;
        is >> dir_discr;

        if (dir_discr != fmt_traits::discriminator)
            throw std::ios_base::failure(std::format(
                "Invalid hypergraph specification: directional specifier {} does not match "
                "expected {}",
                dir_discr,
                fmt_traits::discriminator
            ));

        // read hypergraph metadata
        id_type n_vertices, n_hyperedges;
        is >> n_vertices >> n_hyperedges;

        bool with_v_props, with_he_props;
        is >> with_v_props >> with_he_props;

        if (with_v_props) {
            if constexpr (not traits::c_readable<vertex_properties_type>) {
                throw std::ios_base::failure(
                    "Invalid hypergraph specification: vertex_properties=true "
                    "when vertex_properties_type is not readable"
                );
            }
            else {
                std::vector<vertex_properties_type> vertex_properties(n_vertices);
                for (auto i = 0uz; i < n_vertices; ++i)
                    is >> vertex_properties[i];
                this->add_vertices_with(vertex_properties);
            }
        }
        else {
            this->add_vertices(n_vertices);
        }

        if (with_he_props) {
            if constexpr (not traits::c_readable<hyperedge_properties_type>) {
                throw std::ios_base::failure(
                    "Invalid hypergraph specification: hyperedge_properties=true "
                    "when hyperedge_properties_type is not readable"
                );
            }
        }

        this->_read_hyperedges(is, n_hyperedges, with_he_props);

        return is;
    }

    void _read_hyperedges(std::istream& is, const size_type n_hyperedges, const bool with_he_props)
    requires std::same_as<directional_tag, undirected_t>
    {
        for (auto _ = 0uz; _ < n_hyperedges; ++_) {
            size_type size;
            is >> size;

            std::vector<id_type> v_ids(size);
            for (auto i = 0uz; i < size; ++i)
                is >> v_ids[i];

            id_type new_he_id;
            if constexpr (traits::c_readable<hyperedge_properties_type>) {
                if (with_he_props) {
                    hyperedge_properties_type props;
                    is >> props;
                    new_he_id = this->add_hyperedge_with(v_ids, std::move(props)).id();
                }
                else {
                    new_he_id = this->add_hyperedge(v_ids).id();
                }
            }
            else {
                new_he_id = this->add_hyperedge(v_ids).id();
            }
        }
    }

    void _read_hyperedges(std::istream& is, const size_type n_hyperedges, const bool with_he_props)
    requires(std::same_as<directional_tag, bf_directed_t>)
    {
        for (auto _ = 0uz; _ < n_hyperedges; ++_) {
            size_type tail_size, head_size;
            is >> tail_size >> head_size;

            std::vector<id_type> tail_ids(tail_size);
            for (auto i = 0uz; i < tail_size; ++i)
                is >> tail_ids[i];

            std::vector<id_type> head_ids(head_size);
            for (auto i = 0uz; i < head_size; ++i)
                is >> head_ids[i];

            id_type new_he_id;
            if constexpr (traits::c_readable<hyperedge_properties_type>) {
                if (with_he_props) {
                    hyperedge_properties_type props;
                    is >> props;
                    new_he_id = this->add_hyperedge_with(tail_ids, head_ids, std::move(props)).id();
                }
                else {
                    new_he_id = this->add_hyperedge(tail_ids, head_ids).id();
                }
            }
            else {
                new_he_id = this->add_hyperedge(tail_ids, head_ids).id();
            }
        }
    }

    // --- data members ---

    size_type _n_vertices = 0uz;
    size_type _n_hyperedges = 0uz;

    representation_type _impl{};

    [[no_unique_address]] vertex_properties_map_type _vertex_properties{};
    [[no_unique_address]] hyperedge_properties_map_type _hyperedge_properties{};
};

// --- general hypergraph utility ---

/// @ingroup HGL-Core
/// @brief Creates a deep copy of the given hypergraph.
/// @tparam Hypergraph The type of the hypergraph.
/// @param source The hypergraph instance to clone.
/// @return A newly constructed hypergraph containing identical vertices, hyperedges and properties (if applicable).
template <traits::c_hypergraph Hypergraph>
[[nodiscard]] Hypergraph clone(const Hypergraph& source) {
    return Hypergraph(source);
}

/// @ingroup HGL-Core
/// @brief Convenience alias for an undirected hypergraph.
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_hypergraph_repr_tag ReprTag = repr::list_t<>>
using undirected_hypergraph =
    hypergraph<undirected_hypergraph_traits<VertexProperties, HyperedgeProperties, ReprTag>>;

/// @ingroup HGL-Core
/// @brief Convenience alias for a BF-directed hypergraph.
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_hypergraph_repr_tag ReprTag = repr::list_t<>>
using bf_directed_hypergraph =
    hypergraph<bf_directed_hypergraph_traits<VertexProperties, HyperedgeProperties, ReprTag>>;

/// @ingroup HGL-Core
/// @brief Convenience alias for a hypergraph utilizing a standard incidence list representation model.
template <
    traits::c_hypergraph_layout_tag LayoutTag = repr::bidirectional_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using list_hypergraph = hypergraph<
    list_hypergraph_traits<LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType>>;

/// @ingroup HGL-Core
/// @brief Convenience alias for a hypergraph utilizing a flat incidence list representation model.
template <
    traits::c_hypergraph_layout_tag LayoutTag = repr::bidirectional_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using flat_list_hypergraph = hypergraph<flat_list_hypergraph_traits<
    LayoutTag,
    DirectionalTag,
    VertexProperties,
    HyperedgeProperties,
    IdType>>;

/// @ingroup HGL-Core
/// @brief Convenience alias for a hypergraph utilizing a standard incidence matrix representation model.
template <
    traits::c_hypergraph_layout_tag LayoutTag = repr::bidirectional_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using matrix_hypergraph = hypergraph<
    matrix_hypergraph_traits<LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType>>;

/// @ingroup HGL-Core
/// @brief Convenience alias for a hypergraph utilizing a flat incidence matrix representation model.
template <
    traits::c_hypergraph_layout_tag LayoutTag = repr::bidirectional_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using flat_matrix_hypergraph = hypergraph<flat_matrix_hypergraph_traits<
    LayoutTag,
    DirectionalTag,
    VertexProperties,
    HyperedgeProperties,
    IdType>>;

namespace detail::hypergraph_doc_anchors {

/// > [!IMPORTANT] ID Stability
/// >
/// > Adding vertices does **not** invalidate existing vertex IDs. **However**, property references stored in existing vertex descriptors may be invalidated.
void add_vertex_note();

/// > [!WARNING] Descriptor and ID Invalidation
/// >
/// > Removing a vertex invalidates:
/// > - All vertex descriptors and IDs for vertices with higher IDs (they shift down).
/// > - References to all properties associated with the vertices with IDs shifted as a result of the removal operation.
/// >
/// > Proceed with caution when maintaining external vertex IDs, descriptors or properties.
void remove_vertex_wrn();

/// > [!IMPORTANT] ID Stability
/// >
/// > Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated.
void add_hyperedge_note();

/// > [!WARNING] Descriptor and ID Invalidation
/// >
/// > Removing a hyperedge invalidates:
/// > - All hyperedge descriptors and IDs for hyperedges with higher IDs (they shift down).
/// > - References to all properties associated with the hyperedges with IDs shifted as a result of the removal operation.
/// >
/// > Proceed with caution when maintaining external hyperedge IDs, descriptors, or properties.
void remove_hyperedge_wrn();

// --- definitions ---

/// ### Formal Definition
/// The degree of a vertex in a hypergraph is the total number of hyperedges incident with a vertex.
///
/// \f[
/// deg(v) =
/// \begin{cases}
/// \vert\{e \in E : v \in e\}\vert & \text{if } H \text{ is undirected}
/// \\ deg_{in}(v) + deg_{out}(v) & \text{if } H \text{ is BF-directed}
/// \end{cases}
/// \f]
void degree();

/// ### Formal Definition
/// The in-degree of a vertex in a *BF-directed* hypergraph is the number of hyperedges forward-incident
/// with the vertex (such that the vertex belongs to the head of the hyperedge).
///
/// \f[
/// deg_{in}(v) = \vert\{e \in E : v \in H(e)\}\vert
/// \f]
void in_degree();

/// ### Formal Definition
/// The out-degree of a vertex in a *BF-directed* hypergraph is the number of hyperedges backward-incident
/// with the vertex (such that the vertex belongs to the tail of the hyperedge).
///
/// \f[
/// deg_{in}(v) = \vert\{e \in E : v \in H(e)\}\vert
/// \f]
void out_degree();

} // namespace detail::hypergraph_doc_anchors
} // namespace hgl
