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
#include "hgl/impl/impl_tags.hpp"
#include "hgl/io/core.hpp"
#include "hgl/io/hypergraph_fmt_traits.hpp"
#include "hgl/util.hpp"

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
concept c_hypergraph = c_instantiation_of<H, hypergraph>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph is undirected.
/// @see @ref hgl::undirected_t "undirected_t" for the directional tag used to constrain this concept.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_undirected_hypergraph =
    c_hypergraph<H> and std::same_as<typename H::directional_tag, undirected_t>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph is backward-forward (bf) directed.
/// @see @ref hgl::bf_directed_t "bf_directed_t" for the directional tag used to constrain this concept.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_bf_directed_hypergraph =
    c_hypergraph<H> and std::same_as<typename H::directional_tag, bf_directed_t>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph uses a standard incidence list implementation.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_list_hypergraph =
    c_hypergraph<H> and c_hypergraph_list_impl<typename H::implementation_tag>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph uses a flattened incidence list implementation.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_flat_list_hypergraph =
    c_hypergraph<H> and c_hypergraph_flat_list_impl<typename H::implementation_tag>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph uses any incidence list implementation (standard or flattened).
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_incidence_list_hypergraph =
    c_hypergraph<H> and c_hypergraph_incidence_list_impl<typename H::implementation_tag>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph uses a standard incidence matrix implementation.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_matrix_hypergraph =
    c_hypergraph<H> and c_hypergraph_matrix_impl<typename H::implementation_tag>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph uses a flattened incidence matrix implementation.
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_flat_matrix_hypergraph =
    c_hypergraph<H> and c_hypergraph_flat_matrix_impl<typename H::implementation_tag>;

/// @ingroup HGL-Traits
/// @brief Concept checking if a hypergraph uses any incidence matrix implementation (standard or flattened).
/// @tparam H The type to evaluate against the concept.
template <typename H>
concept c_incidence_matrix_hypergraph =
    c_hypergraph<H> and c_hypergraph_incidence_matrix_impl<typename H::implementation_tag>;

} // namespace traits

/// @ingroup HGL-Core
/// @brief Creates a deep copy of a given hypergraph.
/// @tparam Hypergraph The concrete hypergraph type.
/// @param source The hypergraph to clone.
/// @return A duplicated instance of the source hypergraph.
template <traits::c_hypergraph Hypergraph>
[[nodiscard]] Hypergraph clone(const Hypergraph& source);

/// @ingroup HGL-Core
/// @brief Converts a hypergraph to a different implementation type (e.g., from incidence list to incidence matrix).
/// @tparam TargetImplTag The implementation tag defining the target storage mechanism.
/// @tparam Hypergraph The concrete hypergraph type of the source.
/// @param source The hypergraph to convert.
/// @return A new hypergraph matching the target implementation type with identical topology and properties.
template <traits::c_hypergraph_impl_tag TargetImplTag, traits::c_hypergraph Hypergraph>
[[nodiscard]] auto to(Hypergraph&& source);

namespace detail {

/// @brief Internal structure for dispatching the hypergraph implementation conversion logic.
template <traits::c_hypergraph_impl_tag TargetImplTag, traits::c_hypergraph_impl_tag SourceImplTag>
struct to_impl;

} // namespace detail

/// @ingroup HGL-Core
/// @brief The primary, highly configurable generic container for representing n-ary hypergraphs.
///
/// **Module:** Part of the @ref HGL-Core "Core Hypergraph Components" group.
///
/// Unlike standard graphs where edges connect exactly two vertices, hypergraphs generalize this
/// concept by allowing a single hyperedge to connect any number of vertices simultaneously.
/// The `hgl::hypergraph` template provides a strictly type-safe, highly optimized, and conceptually
/// robust API for modeling both undirected and backward-forward (bf) directed hypergraphs.
///
/// ### Key Features
/// - **Zero-cost Abstractions**: Core query logic is resolved at compile time through layout tags and static dispatch, removing unnecessary overhead.
/// - **Multiple Internal Representations**: Supports both incidence matrices (optimal for dense, rigid structures) and incidence lists (optimal for sparse, dynamic structures), further separated into standard and flat-memory contiguous variants.
/// - **Arbitrary Properties Injection**: Allows seamless integration of completely custom, user-defined data structures (properties) directly into vertices and hyperedges without inheriting from intrusive base classes.
/// - **Standard Range Support**: Exposes lightweight views compliant with C++20 `std::ranges`, enabling functional-style iteration and algorithms.
///
/// ### Basic Definitions
/// - **Hyperedge**: A generalized edge that can connect any subset of vertices.
/// - **Incidence**: The fundamental relationship in a hypergraph. A vertex is "incident to" a hyperedge if it is contained within that hyperedge's set.
/// - **Undirected**: A hyperedge is simply a set of incident vertices.
/// - **BF-Directed**: A hyperedge maps a distinct subset of vertices (the **Tail**) to another distinct subset of vertices (the **Head**).
///
/// ### Example Usage
/// ```cpp
/// // 1. Define a directed hypergraph with string names on vertices and weights on hyperedges.
/// using traits = hgl::hypergraph_traits<
///     hgl::bf_directed_t, gl::name_property, gl::weight_property
/// >;
/// hgl::hypergraph<traits> h;
///
/// // 2. Add properties via the descriptor dereference operator.
/// auto v0 = h.add_vertex(); v0->name = "Source"; // (1)!
/// auto v1 = h.add_vertex(); v1->name = "Target A";
/// auto v2 = h.add_vertex(); v2->name = "Target B";
///
/// // 3. For directed hypergraphs, specify Tail vertices and Head vertices.
/// auto e = h.add_hyperedge({v0}, {v1, v2}); // (2)!
/// e->weight = 5.5;
///
/// std::cout << gl::io::verbose << gl::io::with_properties; // (3)!
/// std::cout << "Hypergraph:\n" << h << '\n';
/// ```
///
/// 1. Vertices are created and returned as safe @ref hgl::vertex_descriptor "vertex_descriptor" wrappers.
/// 2. Hyperedges are created and returned as safe @ref hgl::hyperedge_descriptor "hyperedge_descriptor" wrappers.
/// 3. Standard GL stream manipulators format both graph and hypergraph output identically.
///
/// ### Template Parameters
/// | Parameter | Description | Default | Constraint |
/// | :-------- | :---------- | :------ | :--------- |
/// | HypergraphTraits | The core configuration object dictating the directionality, properties, identifier types, and internal memory layout of the hypergraph. | @ref hgl::hypergraph_traits "hypergraph_traits<>" | [**c_instantiation_of<hypergraph_traits>**](hgl_concepts.md#hgl-traits-c-instantiation-of) |
///
/// ### See Also
/// - @ref hgl::hypergraph_traits "hypergraph_traits" for configuring the underlying properties and tags.
/// - @ref gl::io::options_manip "options_manip" for custom stream formatting options.
template <traits::c_instantiation_of<hypergraph_traits> HypergraphTraits>
class hypergraph final {
public:
    /// @brief The configured traits type governing this hypergraph's parameters.
    using traits_type = HypergraphTraits;
    /// @brief The directional tag indicating if the hypergraph is undirected or bf_directed.
    using directional_tag = typename traits_type::directional_tag;
    /// @brief The implementation tag defining the internal storage mechanism.
    using implementation_tag = typename traits_type::implementation_tag;
    /// @brief The instantiated underlying implementation class matching the directional tag.
    using implementation_type =
        typename implementation_tag::template implementation_type<directional_tag>;
    /// @brief The underlying integer type used for identifiers.
    using id_type = typename traits_type::id_type;

    /// @brief The fully resolved type representing a vertex descriptor.
    using vertex_type = typename traits_type::vertex_type;
    /// @brief The user-defined property payload type associated with vertices.
    using vertex_properties_type = typename traits_type::vertex_properties_type;
    /// @brief The container type used for storing vertex properties internally.
    using vertex_properties_map_type = std::conditional_t<
        traits::c_empty_properties<vertex_properties_type>,
        empty_properties_map,
        std::vector<vertex_properties_type>>;

    /// @brief The fully resolved type representing a hyperedge descriptor.
    using hyperedge_type = typename traits_type::hyperedge_type;
    /// @brief The user-defined property payload type associated with hyperedges.
    using hyperedge_properties_type = typename traits_type::hyperedge_properties_type;
    /// @brief The container type used for storing hyperedge properties internally.
    using hyperedge_properties_map_type = std::conditional_t<
        traits::c_empty_properties<hyperedge_properties_type>,
        empty_properties_map,
        std::vector<hyperedge_properties_type>>;

    /// @brief Deleted copy assignment operator to prevent trivial overwrites (use `hgl::clone` instead).
    hypergraph& operator=(const hypergraph&) = delete;

    /// @brief Constructs an empty hypergraph, optionally pre-allocating capacity.
    /// @param n_vertices The initial capacity or fixed count of vertices (especially relevant for matrix implementations).
    /// @param n_hyperedges The initial capacity or fixed count of hyperedges.
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

    // --- size methods ---

    /// @brief Retrieves the current number of valid vertices in the hypergraph.
    /// @return The total vertex count.
    [[nodiscard]] gl_attr_force_inline size_type n_vertices() const noexcept {
        return this->_n_vertices;
    }

    /// @brief Retrieves the current number of valid hyperedges in the hypergraph.
    /// @return The total hyperedge count.
    [[nodiscard]] gl_attr_force_inline size_type n_hyperedges() const noexcept {
        return this->_n_hyperedges;
    }

    // --- vertex modifiers ---

    /// @brief Adds a single new property-less vertex to the hypergraph.
    /// @return A safe `vertex_descriptor` pointing to the newly created vertex.
    vertex_type add_vertex() {
        this->_impl.add_vertices(1uz);
        const auto new_vertex_id = static_cast<id_type>(this->_n_vertices++);

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_type{new_vertex_id, this->_vertex_properties.emplace_back()};
        else
            return vertex_type{new_vertex_id};
    }

    /// @brief Adds a single new vertex to the hypergraph and initializes its properties.
    /// @param properties The property payload to assign to the new vertex.
    /// @return A safe `vertex_descriptor` pointing to the newly created vertex.
    vertex_type add_vertex_with(vertex_properties_type properties)
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        this->_impl.add_vertices(1uz);
        return vertex_type{
            static_cast<id_type>(this->_n_vertices++),
            this->_vertex_properties.emplace_back(std::move(properties))
        };
    }

    /// @brief Efficiently adds multiple property-less vertices to the hypergraph.
    /// @param n The number of vertices to add.
    void add_vertices(const size_type n) {
        this->_impl.add_vertices(n);
        this->_n_vertices += n;

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.resize(this->_n_vertices);
    }

    /// @brief Efficiently adds multiple vertices to the hypergraph, initializing them with a range of properties.
    /// @param properties_rng A range containing the property payloads to assign to the newly created vertices.
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

    /// @brief Safely removes a vertex and completely purges it from all incident hyperedges.
    /// @param vertex_id The raw identifier of the vertex to remove.
    gl_attr_force_inline void remove_vertex(const id_type vertex_id) {
        this->_remove_vertex_impl(vertex_id);
    }

    /// @brief Safely removes a vertex and completely purges it from all incident hyperedges.
    /// @param vertex The `vertex_descriptor` wrapping the vertex to remove.
    gl_attr_force_inline void remove_vertex(const vertex_type& vertex) {
        this->remove_vertex(vertex.id());
    }

    /// @brief Removes multiple vertices specified by a range of raw identifiers.
    /// @param vertex_id_rng A range of raw vertex identifiers to be removed.
    void remove_vertices(const traits::c_forward_range_of<id_type> auto& vertex_id_rng) {
        // sorts ids in a descending n_vertices and removes duplicate ids
        std::set<id_type, std::greater<id_type>> vertex_id_set(
            std::ranges::begin(vertex_id_rng), std::ranges::end(vertex_id_rng)
        );

        // TODO: optimize
        for (const auto vertex_id : vertex_id_set)
            this->_remove_vertex_impl(vertex_id);
    }

    /// @brief Removes multiple vertices specified by a range of vertex descriptors.
    /// @param vertex_rng A range of `vertex_descriptor`s representing the vertices to be removed.
    void remove_vertices(const traits::c_sized_range_of<vertex_type> auto& vertex_rng) {
        // sort vertices in a descending n_vertices (by id) and removes duplicate ids
        std::set<vertex_type, std::greater<vertex_type>> vertex_set(
            std::ranges::begin(vertex_rng), std::ranges::end(vertex_rng)
        );

        // TODO: optimize
        for (const auto& vertex : vertex_set)
            this->_remove_vertex_impl(vertex.id());
    }

    // --- vertex getters ---

    /// @brief Checks if a vertex with the given raw ID exists in the hypergraph.
    /// @param vertex_id The raw identifier to check.
    /// @return `true` if the vertex exists, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool has_vertex(const id_type vertex_id) const {
        return vertex_id < this->_n_vertices;
    }

    /// @brief Checks if the vertex referenced by the provided descriptor exists in the hypergraph.
    /// @param vertex The vertex descriptor to check.
    /// @return `true` if the vertex exists, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool has_vertex(vertex_type vertex) const {
        return this->has_vertex(vertex.id());
    }

    /// @brief Safely wraps a raw vertex ID into a `vertex_descriptor`, bounds-checking the ID.
    /// @param vertex_id The raw numeric identifier of the vertex.
    /// @return The constructed `vertex_descriptor`.
    /// @throws std::invalid_argument If the `vertex_id` is out of bounds.
    [[nodiscard]] vertex_type vertex(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->vertex_unchecked(vertex_id);
    }

    /// @brief Accesses a `vertex_descriptor` using a tag-based accessor.
    /// @param vertex_id The raw numeric identifier of the vertex.
    /// @return The constructed `vertex_descriptor`.
    /// @throws std::invalid_argument If the `vertex_id` is out of bounds.
    [[nodiscard]] gl_attr_force_inline vertex_type at(vertex_t, const id_type vertex_id) const {
        return this->vertex(vertex_id);
    }

    /// @brief Wraps a raw vertex ID into a `vertex_descriptor` without performing bounds checking.
    /// @param vertex_id The raw numeric identifier of the vertex.
    /// @return The constructed `vertex_descriptor`.
    [[nodiscard]] gl_attr_force_inline vertex_type vertex_unchecked(const id_type vertex_id) const {
        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_type{vertex_id, this->_vertex_properties[vertex_id]};
        else
            return vertex_type{vertex_id};
    }

    /// @brief Subscript operator to access a `vertex_descriptor` without bounds checking.
    /// @param vertex_id The raw numeric identifier of the vertex.
    /// @return The constructed `vertex_descriptor`.
    [[nodiscard]] gl_attr_force_inline vertex_type
    operator[](vertex_t, const id_type vertex_id) const {
        return this->vertex_unchecked(vertex_id);
    }

    /// @brief Returns a transformed range wrapping all valid vertices into `vertex_descriptor`s.
    /// @return A lazily evaluated view of all `vertex_descriptor`s.
    [[nodiscard]] gl_attr_force_inline auto vertices() const noexcept {
        return this->vertex_ids() | std::views::transform(this->_create_vertex_descriptor());
    }

    /// @brief Returns a lightweight, lazily evaluated range over all valid raw vertex IDs.
    /// @return A view of all vertex identifiers.
    [[nodiscard]] gl_attr_force_inline auto vertex_ids() const noexcept {
        return std::views::iota(initial_id_v<id_type>, this->_n_vertices);
    }

    /// @brief Retrieves a reference to the properties of a specified vertex.
    /// @param vertex_id The identifier of the vertex.
    /// @return A mutable reference to the vertex's properties.
    /// @throws std::invalid_argument If the `vertex_id` is out of bounds.
    [[nodiscard]] gl_attr_force_inline vertex_properties_type& vertex_properties(
        const id_type vertex_id
    ) const
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        this->_verify_vertex_id(vertex_id);
        return this->_vertex_properties[vertex_id];
    }

    /// @brief Returns a view over the internal vertex properties container.
    /// @return A standard view over the continuous block of property payloads.
    [[nodiscard]] gl_attr_force_inline auto vertex_properties_map() const noexcept
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        return std::views::all(this->_vertex_properties);
    }

    // --- hyperedge modifiers ---

    /// @brief Adds a single new, empty, property-less hyperedge to the hypergraph.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    hyperedge_type add_hyperedge() {
        this->_impl.add_hyperedges(1uz);
        const auto new_hyperedge_id = static_cast<id_type>(this->_n_hyperedges++);

        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>)
            return hyperedge_type{new_hyperedge_id, this->_hyperedge_properties.emplace_back()};
        else
            return hyperedge_type{new_hyperedge_id};
    }

    /// @brief Adds a single new empty hyperedge and initializes its properties.
    /// @param properties The property payload to assign to the new hyperedge.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    hyperedge_type add_hyperedge_with(hyperedge_properties_type properties)
    requires(traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        this->_impl.add_hyperedges(1uz);
        return hyperedge_type{
            static_cast<id_type>(this->_n_hyperedges++),
            this->_hyperedge_properties.emplace_back(std::move(properties))
        };
    }

    /// @brief Adds a new hyperedge and immediately binds a range of vertices to it (undirected).
    /// @param vertex_id_rng A range of raw vertex identifiers to incident with the new hyperedge.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    hyperedge_type add_hyperedge(const traits::c_forward_range_of<id_type> auto& vertex_id_rng)
    requires std::same_as<directional_tag, undirected_t>
    {
        auto he = this->add_hyperedge();
        this->bind(vertex_id_rng, he.id());
        return he;
    }

    /// @brief Adds a new hyperedge and immediately binds an initializer list of vertices to it (undirected).
    /// @param vertex_ids An initializer list of raw vertex identifiers.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    gl_attr_force_inline hyperedge_type add_hyperedge(std::initializer_list<id_type> vertex_ids)
    requires std::same_as<directional_tag, undirected_t>
    {
        return this->add_hyperedge(std::views::all(vertex_ids));
    }

    /// @brief Adds a new hyperedge and immediately binds a range of vertex descriptors to it (undirected).
    /// @param vertex_rng A range of `vertex_descriptor` objects.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    gl_attr_force_inline hyperedge_type
    add_hyperedge(const traits::c_forward_range_of<vertex_type> auto& vertex_rng)
    requires std::same_as<directional_tag, undirected_t>
    {
        return this->add_hyperedge(vertex_rng | std::views::transform(&vertex_type::id));
    }

    /// @brief Adds a new hyperedge and immediately binds an initializer list of vertex descriptors to it (undirected).
    /// @param vertices An initializer list of `vertex_descriptor` objects.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    gl_attr_force_inline hyperedge_type add_hyperedge(std::initializer_list<vertex_type> vertices)
    requires std::same_as<directional_tag, undirected_t>
    {
        return this->add_hyperedge(std::views::all(vertices));
    }

    /// @brief Adds a new hyperedge, initializes properties, and binds a range of vertices to it (undirected).
    /// @param vertex_id_rng A range of raw vertex identifiers.
    /// @param properties The property payload to assign.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
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

    /// @brief Adds a new hyperedge, initializes properties, and binds an initializer list of vertices to it (undirected).
    /// @param vertex_ids An initializer list of raw vertex identifiers.
    /// @param properties The property payload to assign.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    hyperedge_type add_hyperedge_with(
        std::initializer_list<id_type> vertex_ids, hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, undirected_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return this->add_hyperedge_with(std::views::all(vertex_ids), std::move(properties));
    }

    /// @brief Adds a new hyperedge, initializes properties, and binds a range of vertex descriptors to it (undirected).
    /// @param vertex_rng A range of `vertex_descriptor` objects.
    /// @param properties The property payload to assign.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    gl_attr_force_inline hyperedge_type add_hyperedge_with(
        const traits::c_forward_range_of<vertex_type> auto& vertex_rng,
        hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, undirected_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return this->add_hyperedge_with(
            vertex_rng | std::views::transform(&vertex_type::id), std::move(properties)
        );
    }

    /// @brief Adds a new hyperedge, initializes properties, and binds an initializer list of vertex descriptors to it (undirected).
    /// @param vertices An initializer list of `vertex_descriptor` objects.
    /// @param properties The property payload to assign.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    hyperedge_type add_hyperedge_with(
        std::initializer_list<vertex_type> vertices, hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, undirected_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return this->add_hyperedge_with(std::views::all(vertices), std::move(properties));
    }

    /// @brief Adds a new hyperedge and immediately binds tail and head vertices to it (bf-directed).
    /// @param tail_id_rng A range of raw vertex identifiers forming the Tail.
    /// @param head_id_rng A range of raw vertex identifiers forming the Head.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    hyperedge_type add_hyperedge(
        const traits::c_forward_range_of<id_type> auto& tail_id_rng,
        const traits::c_forward_range_of<id_type> auto& head_id_rng
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        auto he = this->add_hyperedge();
        this->bind_tail(tail_id_rng, he.id());
        this->bind_head(head_id_rng, he.id());
        return he;
    }

    /// @brief Adds a new hyperedge and immediately binds tail and head vertices to it (bf-directed).
    /// @param tail_ids An initializer list of raw vertex identifiers forming the Tail.
    /// @param head_ids An initializer list of raw vertex identifiers forming the Head.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    gl_attr_force_inline hyperedge_type
    add_hyperedge(std::initializer_list<id_type> tail_ids, std::initializer_list<id_type> head_ids)
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->add_hyperedge(std::views::all(tail_ids), std::views::all(head_ids));
    }

    /// @brief Adds a new hyperedge and immediately binds tail and head vertices to it (bf-directed).
    /// @param tail_rng A range of `vertex_descriptor`s forming the Tail.
    /// @param head_rng A range of `vertex_descriptor`s forming the Head.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    gl_attr_force_inline hyperedge_type add_hyperedge(
        const traits::c_forward_range_of<vertex_type> auto& tail_rng,
        const traits::c_forward_range_of<vertex_type> auto& head_rng
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->add_hyperedge(
            tail_rng | std::views::transform(&vertex_type::id),
            head_rng | std::views::transform(&vertex_type::id)
        );
    }

    /// @brief Adds a new hyperedge and immediately binds tail and head vertices to it (bf-directed).
    /// @param tail An initializer list of `vertex_descriptor`s forming the Tail.
    /// @param head An initializer list of `vertex_descriptor`s forming the Head.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    gl_attr_force_inline hyperedge_type
    add_hyperedge(std::initializer_list<vertex_type> tail, std::initializer_list<vertex_type> head)
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->add_hyperedge(std::views::all(tail), std::views::all(head));
    }

    /// @brief Adds a new hyperedge, initializes properties, and binds tail and head vertices to it (bf-directed).
    /// @param tail_id_rng A range of raw vertex identifiers forming the Tail.
    /// @param head_id_rng A range of raw vertex identifiers forming the Head.
    /// @param properties The property payload to assign.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
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

    /// @brief Adds a new hyperedge, initializes properties, and binds tail and head vertices to it (bf-directed).
    /// @param tail_ids An initializer list of raw vertex identifiers forming the Tail.
    /// @param head_ids An initializer list of raw vertex identifiers forming the Head.
    /// @param properties The property payload to assign.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
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

    /// @brief Adds a new hyperedge, initializes properties, and binds tail and head vertices to it (bf-directed).
    /// @param tail_rng A range of `vertex_descriptor`s forming the Tail.
    /// @param head_rng A range of `vertex_descriptor`s forming the Head.
    /// @param properties The property payload to assign.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    gl_attr_force_inline hyperedge_type add_hyperedge_with(
        const traits::c_forward_range_of<vertex_type> auto& tail_rng,
        const traits::c_forward_range_of<vertex_type> auto& head_rng,
        hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, bf_directed_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return this->add_hyperedge_with(
            tail_rng | std::views::transform(&vertex_type::id),
            head_rng | std::views::transform(&vertex_type::id),
            std::move(properties)
        );
    }

    /// @brief Adds a new hyperedge, initializes properties, and binds tail and head vertices to it (bf-directed).
    /// @param tail An initializer list of `vertex_descriptor`s forming the Tail.
    /// @param head An initializer list of `vertex_descriptor`s forming the Head.
    /// @param properties The property payload to assign.
    /// @return A safe `hyperedge_descriptor` pointing to the newly created hyperedge.
    gl_attr_force_inline hyperedge_type add_hyperedge_with(
        std::initializer_list<vertex_type> tail,
        std::initializer_list<vertex_type> head,
        hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, bf_directed_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return this->add_hyperedge_with(
            std::views::all(tail), std::views::all(head), std::move(properties)
        );
    }

    /// @brief Efficiently adds multiple property-less hyperedges to the hypergraph.
    /// @param n The number of empty hyperedges to add.
    void add_hyperedges(const size_type n) {
        this->_impl.add_hyperedges(n);
        this->_n_hyperedges += n;

        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>)
            this->_hyperedge_properties.resize(this->_n_hyperedges);
    }

    /// @brief Efficiently adds multiple hyperedges to the hypergraph, initializing them with a range of properties.
    /// @param properties_rng A range containing the property payloads to assign to the newly created hyperedges.
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

    /// @brief Safely removes a hyperedge and completely unbinds it from all incident vertices.
    /// @param hyperedge_id The raw identifier of the hyperedge to remove.
    gl_attr_force_inline void remove_hyperedge(const id_type hyperedge_id) {
        this->_remove_hyperedge_impl(hyperedge_id);
    }

    /// @brief Safely removes a hyperedge and completely unbinds it from all incident vertices.
    /// @param hyperedge The `hyperedge_descriptor` wrapping the hyperedge to remove.
    gl_attr_force_inline void remove_hyperedge(const hyperedge_type& hyperedge) {
        this->remove_hyperedge(hyperedge.id());
    }

    /// @brief Removes multiple hyperedges specified by a range of raw identifiers.
    /// @param hyperedge_id_rng A range of raw hyperedge identifiers to be removed.
    void remove_hyperedges_from(const traits::c_forward_range_of<id_type> auto& hyperedge_id_rng) {
        // sorts ids in a descending n_vertices and removes duplicate ids
        std::set<id_type, std::greater<id_type>> hyperedge_id_set(
            std::ranges::begin(hyperedge_id_rng), std::ranges::end(hyperedge_id_rng)
        );

        // TODO: optimize
        for (const auto hyperedge_id : hyperedge_id_set)
            this->_remove_hyperedge_impl(hyperedge_id);
    }

    /// @brief Removes multiple hyperedges specified by a range of hyperedge descriptors.
    /// @param hyperedge_rng A range of `hyperedge_descriptor`s representing the hyperedges to be removed.
    void remove_hyperedges_from(const traits::c_sized_range_of<hyperedge_type> auto& hyperedge_rng
    ) {
        // sort hyperedges in a descending n_vertices (by id) and removes duplicate ids
        std::set<hyperedge_type, std::greater<hyperedge_type>> hyperedge_set(
            std::ranges::begin(hyperedge_rng), std::ranges::end(hyperedge_rng)
        );

        // TODO: optimize
        for (const auto& hyperedge : hyperedge_set)
            this->_remove_hyperedge_impl(hyperedge.id());
    }

    // --- hyperedge getters ---

    /// @brief Checks if a hyperedge with the given raw ID exists in the hypergraph.
    /// @param hyperedge_id The raw identifier to check.
    /// @return `true` if the hyperedge exists, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool has_hyperedge(const id_type hyperedge_id) const {
        return hyperedge_id < this->_n_hyperedges;
    }

    /// @brief Checks if the hyperedge referenced by the provided descriptor exists in the hypergraph.
    /// @param hyperedge The hyperedge descriptor to check.
    /// @return `true` if the hyperedge exists, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool has_hyperedge(hyperedge_type hyperedge) const {
        return this->has_hyperedge(hyperedge.id());
    }

    /// @brief Safely wraps a raw hyperedge ID into a `hyperedge_descriptor`, bounds-checking the ID.
    /// @param hyperedge_id The raw numeric identifier of the hyperedge.
    /// @return The constructed `hyperedge_descriptor`.
    /// @throws std::invalid_argument If the `hyperedge_id` is out of bounds.
    [[nodiscard]] hyperedge_type hyperedge(const id_type hyperedge_id) const {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->hyperedge_unchecked(hyperedge_id);
    }

    /// @brief Accesses a `hyperedge_descriptor` using a tag-based accessor.
    /// @param hyperedge_id The raw numeric identifier of the hyperedge.
    /// @return The constructed `hyperedge_descriptor`.
    /// @throws std::invalid_argument If the `hyperedge_id` is out of bounds.
    [[nodiscard]] gl_attr_force_inline hyperedge_type
    at(hyperedge_t, const id_type hyperedge_id) const {
        return this->hyperedge(hyperedge_id);
    }

    /// @brief Wraps a raw hyperedge ID into a `hyperedge_descriptor` without performing bounds checking.
    /// @param hyperedge_id The raw numeric identifier of the hyperedge.
    /// @return The constructed `hyperedge_descriptor`.
    [[nodiscard]] gl_attr_force_inline hyperedge_type hyperedge_unchecked(const id_type hyperedge_id
    ) const {
        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>)
            return hyperedge_type{hyperedge_id, this->_hyperedge_properties[hyperedge_id]};
        else
            return hyperedge_type{hyperedge_id};
    }

    /// @brief Subscript operator to access a `hyperedge_descriptor` without bounds checking.
    /// @param hyperedge_id The raw numeric identifier of the hyperedge.
    /// @return The constructed `hyperedge_descriptor`.
    [[nodiscard]] gl_attr_force_inline hyperedge_type
    operator[](hyperedge_t, const id_type hyperedge_id) const {
        return this->hyperedge_unchecked(hyperedge_id);
    }

    /// @brief Returns a transformed range wrapping all valid hyperedges into `hyperedge_descriptor`s.
    /// @return A lazily evaluated view of all `hyperedge_descriptor`s.
    [[nodiscard]] gl_attr_force_inline auto hyperedges() const noexcept {
        return this->hyperedge_ids() | std::views::transform(this->_create_hyperedge_descriptor());
    }

    /// @brief Returns a lightweight, lazily evaluated range over all valid raw hyperedge IDs.
    /// @return A view of all hyperedge identifiers.
    [[nodiscard]] gl_attr_force_inline auto hyperedge_ids() const noexcept {
        return std::views::iota(initial_id_v<id_type>, this->_n_hyperedges);
    }

    /// @brief Retrieves a reference to the properties of a specified hyperedge.
    /// @param id The identifier of the hyperedge.
    /// @return A mutable reference to the hyperedge's properties.
    /// @throws std::invalid_argument If the `id` is out of bounds.
    [[nodiscard]] gl_attr_force_inline hyperedge_properties_type& hyperedge_properties(
        const id_type id
    ) const
    requires(traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        this->_verify_hyperedge_id(id);
        return this->_hyperedge_properties[id];
    }

    /// @brief Returns a view over the internal hyperedge properties container.
    /// @return A standard view over the continuous block of property payloads.
    [[nodiscard]] gl_attr_force_inline auto hyperedge_properties_map() const noexcept
    requires(traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return std::views::all(this->_hyperedge_properties);
    }

    // --- incidence modifiers ---

    /// @brief Binds a vertex to a hyperedge in an undirected hypergraph.
    /// @param vertex_id The raw identifier of the vertex.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    void bind(const id_type vertex_id, const id_type hyperedge_id)
    requires std::same_as<directional_tag, undirected_t>
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        this->_impl.bind(vertex_id, hyperedge_id);
    }

    /// @brief Binds a vertex to a hyperedge in an undirected hypergraph using descriptors.
    /// @param vertex The `vertex_descriptor` mapping to the vertex.
    /// @param hyperedge The `hyperedge_descriptor` mapping to the hyperedge.
    gl_attr_force_inline void bind(const vertex_type& vertex, const hyperedge_type& hyperedge)
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex.id(), hyperedge.id());
    }

    /// @brief Binds a range of vertices to a single hyperedge in an undirected hypergraph.
    /// @param vertex_id_rng A range of raw vertex identifiers.
    /// @param hyperedge_id The raw identifier of the hyperedge.
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

    /// @brief Binds an initializer list of vertices to a single hyperedge in an undirected hypergraph.
    /// @param vertex_ids An initializer list of raw vertex identifiers.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    gl_attr_force_inline void bind(
        std::initializer_list<id_type> vertex_ids, const id_type hyperedge_id
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(std::views::all(vertex_ids), hyperedge_id);
    }

    /// @brief Binds a range of vertex descriptors to a single hyperedge in an undirected hypergraph.
    /// @param vertex_rng A range of `vertex_descriptor`s.
    /// @param hyperedge The `hyperedge_descriptor` wrapping the target hyperedge.
    gl_attr_force_inline void bind(
        const traits::c_forward_range_of<vertex_type> auto& vertex_rng,
        const hyperedge_type& hyperedge
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex_rng | std::views::transform(&vertex_type::id), hyperedge.id());
    }

    /// @brief Binds an initializer list of vertex descriptors to a single hyperedge in an undirected hypergraph.
    /// @param vertices An initializer list of `vertex_descriptor`s.
    /// @param hyperedge The `hyperedge_descriptor` wrapping the target hyperedge.
    gl_attr_force_inline void bind(
        std::initializer_list<vertex_type> vertices, const hyperedge_type& hyperedge
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(std::views::all(vertices), hyperedge);
    }

    /// @brief Binds a single vertex to a range of hyperedges in an undirected hypergraph.
    /// @param vertex_id The raw identifier of the vertex.
    /// @param hyperedge_id_rng A range of raw hyperedge identifiers.
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

    /// @brief Binds a single vertex to an initializer list of hyperedges in an undirected hypergraph.
    /// @param vertex_id The raw identifier of the vertex.
    /// @param hyperedge_ids An initializer list of raw hyperedge identifiers.
    gl_attr_force_inline void bind(
        const id_type vertex_id, std::initializer_list<id_type> hyperedge_ids
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex_id, std::views::all(hyperedge_ids));
    }

    /// @brief Binds a single vertex descriptor to a range of hyperedge descriptors in an undirected hypergraph.
    /// @param vertex The target `vertex_descriptor`.
    /// @param hyperedge_rng A range of `hyperedge_descriptor`s.
    gl_attr_force_inline void bind(
        const vertex_type& vertex,
        const traits::c_forward_range_of<hyperedge_type> auto& hyperedge_rng
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex.id(), hyperedge_rng | std::views::transform(&hyperedge_type::id));
    }

    /// @brief Binds a single vertex descriptor to an initializer list of hyperedge descriptors in an undirected hypergraph.
    /// @param vertex The target `vertex_descriptor`.
    /// @param hyperedges An initializer list of `hyperedge_descriptor`s.
    gl_attr_force_inline void bind(
        const vertex_type& vertex, std::initializer_list<hyperedge_type> hyperedges
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex, std::views::all(hyperedges));
    }

    /// @brief Binds a vertex to the tail (source) of a bf-directed hyperedge.
    /// @param vertex_id The raw identifier of the vertex.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    void bind_tail(const id_type vertex_id, const id_type hyperedge_id)
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        this->_impl.bind_tail(vertex_id, hyperedge_id);
    }

    /// @brief Binds a vertex descriptor to the tail (source) of a bf-directed hyperedge descriptor.
    /// @param vertex The `vertex_descriptor` mapping to the vertex.
    /// @param hyperedge The `hyperedge_descriptor` mapping to the hyperedge.
    gl_attr_force_inline void bind_tail(const vertex_type& vertex, const hyperedge_type& hyperedge)
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(vertex.id(), hyperedge.id());
    }

    /// @brief Binds a range of vertices to the tail of a single bf-directed hyperedge.
    /// @param vertex_id_rng A range of raw vertex identifiers.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    void bind_tail(
        const traits::c_forward_range_of<id_type> auto& vertex_id_rng, const id_type hyperedge_id
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_hyperedge_id(hyperedge_id);
        for (const auto vertex_id : vertex_id_rng) {
            this->_verify_vertex_id(vertex_id);
            this->_impl.bind_tail(vertex_id, hyperedge_id);
        }
    }

    /// @brief Binds an initializer list of vertices to the tail of a single bf-directed hyperedge.
    /// @param vertex_ids An initializer list of raw vertex identifiers.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    gl_attr_force_inline void bind_tail(
        std::initializer_list<id_type> vertex_ids, const id_type hyperedge_id
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(std::views::all(vertex_ids), hyperedge_id);
    }

    /// @brief Binds a range of vertex descriptors to the tail of a single bf-directed hyperedge descriptor.
    /// @param vertex_rng A range of `vertex_descriptor`s.
    /// @param hyperedge The `hyperedge_descriptor` wrapping the target hyperedge.
    gl_attr_force_inline void bind_tail(
        const traits::c_forward_range_of<vertex_type> auto& vertex_rng,
        const hyperedge_type& hyperedge
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(vertex_rng | std::views::transform(&vertex_type::id), hyperedge.id());
    }

    /// @brief Binds an initializer list of vertex descriptors to the tail of a single bf-directed hyperedge descriptor.
    /// @param vertices An initializer list of `vertex_descriptor`s.
    /// @param hyperedge The `hyperedge_descriptor` wrapping the target hyperedge.
    gl_attr_force_inline void bind_tail(
        std::initializer_list<vertex_type> vertices, const hyperedge_type& hyperedge
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(std::views::all(vertices), hyperedge);
    }

    /// @brief Binds a single vertex to the tails of a range of bf-directed hyperedges.
    /// @param vertex_id The raw identifier of the vertex.
    /// @param hyperedge_id_rng A range of raw hyperedge identifiers.
    void bind_tail(
        const id_type vertex_id, const traits::c_forward_range_of<id_type> auto& hyperedge_id_rng
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        for (const auto hyperedge_id : hyperedge_id_rng) {
            this->_verify_hyperedge_id(hyperedge_id);
            this->_impl.bind_tail(vertex_id, hyperedge_id);
        }
    }

    /// @brief Binds a single vertex to the tails of an initializer list of bf-directed hyperedges.
    /// @param vertex_id The raw identifier of the vertex.
    /// @param hyperedge_ids An initializer list of raw hyperedge identifiers.
    gl_attr_force_inline void bind_tail(
        const id_type vertex_id, std::initializer_list<id_type> hyperedge_ids
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(vertex_id, std::views::all(hyperedge_ids));
    }

    /// @brief Binds a single vertex descriptor to the tails of a range of bf-directed hyperedge descriptors.
    /// @param vertex The target `vertex_descriptor`.
    /// @param hyperedge_rng A range of `hyperedge_descriptor`s.
    gl_attr_force_inline void bind_tail(
        const vertex_type& vertex,
        const traits::c_forward_range_of<hyperedge_type> auto& hyperedge_rng
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(vertex.id(), hyperedge_rng | std::views::transform(&hyperedge_type::id));
    }

    /// @brief Binds a single vertex descriptor to the tails of an initializer list of bf-directed hyperedge descriptors.
    /// @param vertex The target `vertex_descriptor`.
    /// @param hyperedges An initializer list of `hyperedge_descriptor`s.
    gl_attr_force_inline void bind_tail(
        const vertex_type& vertex, std::initializer_list<hyperedge_type> hyperedges
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(vertex, std::views::all(hyperedges));
    }

    /// @brief Binds a vertex to the head (destination) of a bf-directed hyperedge.
    /// @param vertex_id The raw identifier of the vertex.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    void bind_head(const id_type vertex_id, const id_type hyperedge_id)
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        this->_impl.bind_head(vertex_id, hyperedge_id);
    }

    /// @brief Binds a vertex descriptor to the head (destination) of a bf-directed hyperedge descriptor.
    /// @param vertex The `vertex_descriptor` mapping to the vertex.
    /// @param hyperedge The `hyperedge_descriptor` mapping to the hyperedge.
    gl_attr_force_inline void bind_head(const vertex_type& vertex, const hyperedge_type& hyperedge)
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(vertex.id(), hyperedge.id());
    }

    /// @brief Binds a range of vertices to the head of a single bf-directed hyperedge.
    /// @param vertex_id_rng A range of raw vertex identifiers.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    void bind_head(
        const traits::c_forward_range_of<id_type> auto& vertex_id_rng, const id_type hyperedge_id
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_hyperedge_id(hyperedge_id);
        for (const auto vertex_id : vertex_id_rng) {
            this->_verify_vertex_id(vertex_id);
            this->_impl.bind_head(vertex_id, hyperedge_id);
        }
    }

    /// @brief Binds an initializer list of vertices to the head of a single bf-directed hyperedge.
    /// @param vertex_ids An initializer list of raw vertex identifiers.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    gl_attr_force_inline void bind_head(
        std::initializer_list<id_type> vertex_ids, const id_type hyperedge_id
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(std::views::all(vertex_ids), hyperedge_id);
    }

    /// @brief Binds a range of vertex descriptors to the head of a single bf-directed hyperedge descriptor.
    /// @param vertex_rng A range of `vertex_descriptor`s.
    /// @param hyperedge The `hyperedge_descriptor` wrapping the target hyperedge.
    gl_attr_force_inline void bind_head(
        const traits::c_forward_range_of<vertex_type> auto& vertex_rng,
        const hyperedge_type& hyperedge
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(vertex_rng | std::views::transform(&vertex_type::id), hyperedge.id());
    }

    /// @brief Binds an initializer list of vertex descriptors to the head of a single bf-directed hyperedge descriptor.
    /// @param vertices An initializer list of `vertex_descriptor`s.
    /// @param hyperedge The `hyperedge_descriptor` wrapping the target hyperedge.
    gl_attr_force_inline void bind_head(
        std::initializer_list<vertex_type> vertices, const hyperedge_type& hyperedge
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(std::views::all(vertices), hyperedge);
    }

    /// @brief Binds a single vertex to the heads of a range of bf-directed hyperedges.
    /// @param vertex_id The raw identifier of the vertex.
    /// @param hyperedge_id_rng A range of raw hyperedge identifiers.
    void bind_head(
        const id_type vertex_id, const traits::c_forward_range_of<id_type> auto& hyperedge_id_rng
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        for (const auto hyperedge_id : hyperedge_id_rng) {
            this->_verify_hyperedge_id(hyperedge_id);
            this->_impl.bind_head(vertex_id, hyperedge_id);
        }
    }

    /// @brief Binds a single vertex to the heads of an initializer list of bf-directed hyperedges.
    /// @param vertex_id The raw identifier of the vertex.
    /// @param hyperedge_ids An initializer list of raw hyperedge identifiers.
    gl_attr_force_inline void bind_head(
        const id_type vertex_id, std::initializer_list<id_type> hyperedge_ids
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(vertex_id, std::views::all(hyperedge_ids));
    }

    /// @brief Binds a single vertex descriptor to the heads of a range of bf-directed hyperedge descriptors.
    /// @param vertex The target `vertex_descriptor`.
    /// @param hyperedge_rng A range of `hyperedge_descriptor`s.
    gl_attr_force_inline void bind_head(
        const vertex_type& vertex,
        const traits::c_forward_range_of<hyperedge_type> auto& hyperedge_rng
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(vertex.id(), hyperedge_rng | std::views::transform(&hyperedge_type::id));
    }

    /// @brief Binds a single vertex descriptor to the heads of an initializer list of bf-directed hyperedge descriptors.
    /// @param vertex The target `vertex_descriptor`.
    /// @param hyperedges An initializer list of `hyperedge_descriptor`s.
    gl_attr_force_inline void bind_head(
        const vertex_type& vertex, std::initializer_list<hyperedge_type> hyperedges
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(vertex, std::views::all(hyperedges));
    }

    /// @brief Unbinds a vertex from a hyperedge entirely.
    /// @param vertex_id The raw identifier of the vertex.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    void unbind(const id_type vertex_id, const id_type hyperedge_id) {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        this->_impl.unbind(vertex_id, hyperedge_id);
    }

    /// @brief Unbinds a vertex from a hyperedge entirely using descriptors.
    /// @param vertex The `vertex_descriptor` mapping to the vertex.
    /// @param hyperedge The `hyperedge_descriptor` mapping to the hyperedge.
    gl_attr_force_inline void unbind(const vertex_type& vertex, const hyperedge_type& hyperedge) {
        this->unbind(vertex.id(), hyperedge.id());
    }

    // --- incidence validators ---

    /// @brief Evaluates whether a vertex and a hyperedge are currently incident.
    /// @param vertex_id The raw identifier of the vertex.
    /// @param hyperedge_id The raw identifier of the hyperedge.
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
        const vertex_type& vertex, const hyperedge_type& hyperedge
    ) const {
        return this->are_incident(vertex.id(), hyperedge.id());
    }

    /// @brief Evaluates whether a vertex is in the tail (source) of a bf-directed hyperedge.
    /// @param vertex_id The raw identifier of the vertex.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    /// @return `true` if the vertex is in the tail set, `false` otherwise.
    [[nodiscard]] bool is_tail(const id_type vertex_id, const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.is_tail(vertex_id, hyperedge_id);
    }

    /// @brief Evaluates whether a vertex descriptor is in the tail of a bf-directed hyperedge descriptor.
    /// @param vertex The `vertex_descriptor` mapping to the vertex.
    /// @param hyperedge The `hyperedge_descriptor` mapping to the hyperedge.
    /// @return `true` if the vertex is in the tail set, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool is_tail(
        const vertex_type& vertex, const hyperedge_type& hyperedge
    ) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->is_tail(vertex.id(), hyperedge.id());
    }

    /// @brief Evaluates whether a vertex is in the head (destination) of a bf-directed hyperedge.
    /// @param vertex_id The raw identifier of the vertex.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    /// @return `true` if the vertex is in the head set, `false` otherwise.
    [[nodiscard]] bool is_head(const id_type vertex_id, const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.is_head(vertex_id, hyperedge_id);
    }

    /// @brief Evaluates whether a vertex descriptor is in the head of a bf-directed hyperedge descriptor.
    /// @param vertex The `vertex_descriptor` mapping to the vertex.
    /// @param hyperedge The `hyperedge_descriptor` mapping to the hyperedge.
    /// @return `true` if the vertex is in the head set, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool is_head(
        const vertex_type& vertex, const hyperedge_type& hyperedge
    ) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->is_head(vertex.id(), hyperedge.id());
    }

    // --- incidence getters ---

    /// @brief Returns a transformed range of fully bound `hyperedge_descriptor`s incident to the specified vertex.
    /// @param vertex_id The raw identifier of the vertex.
    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const id_type vertex_id) {
        return this->incident_hyperedge_ids(vertex_id)
             | std::views::transform(this->_create_hyperedge_descriptor());
    }

    /// @brief Returns a transformed range of fully bound `hyperedge_descriptor`s incident to the specified vertex descriptor.
    /// @param vertex The target `vertex_descriptor`.
    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const vertex_type& vertex) {
        return this->incident_hyperedges(vertex.id());
    }

    /// @brief Returns a lightweight range of raw hyperedge IDs incident to the specified vertex.
    /// @param vertex_id The raw identifier of the vertex.
    [[nodiscard]] auto incident_hyperedge_ids(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.incident_hyperedges(vertex_id);
    }

    /// @brief Returns a lightweight range of raw hyperedge IDs incident to the specified vertex descriptor.
    /// @param vertex The target `vertex_descriptor`.
    [[nodiscard]] gl_attr_force_inline auto incident_hyperedge_ids(const vertex_type& vertex
    ) const {
        return this->incident_hyperedge_ids(vertex.id());
    }

    /// @brief Retrieves the total degree (number of incident hyperedges) of a vertex.
    /// @param vertex_id The raw identifier of the vertex.
    [[nodiscard]] size_type degree(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.degree(vertex_id);
    }

    /// @brief Retrieves the total degree (number of incident hyperedges) of a vertex descriptor.
    /// @param vertex The target `vertex_descriptor`.
    [[nodiscard]] gl_attr_force_inline size_type degree(const vertex_type& vertex) const {
        return this->degree(vertex.id());
    }

    /// @brief Generates a contiguous degree map for all valid vertices in the hypergraph.
    [[nodiscard]] std::vector<size_type> degree_map() const {
        return this->_impl.degree_map(this->_n_vertices);
    }

    /// @brief Returns a transformed range of fully bound `hyperedge_descriptor`s originating from the specified vertex (bf-directed).
    /// @param vertex_id The raw identifier of the vertex.
    [[nodiscard]] gl_attr_force_inline auto out_hyperedges(const id_type vertex_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->out_hyperedge_ids(vertex_id)
             | std::views::transform(this->_create_hyperedge_descriptor());
    }

    /// @brief Returns a transformed range of fully bound `hyperedge_descriptor`s originating from the specified vertex descriptor (bf-directed).
    /// @param vertex The target `vertex_descriptor`.
    [[nodiscard]] gl_attr_force_inline auto out_hyperedges(const vertex_type& vertex) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->out_hyperedges(vertex.id());
    }

    /// @brief Returns a lightweight range of raw hyperedge IDs originating from the specified vertex (bf-directed).
    /// @param vertex_id The raw identifier of the vertex.
    [[nodiscard]] auto out_hyperedge_ids(const id_type vertex_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.out_hyperedges(vertex_id);
    }

    /// @brief Returns a lightweight range of raw hyperedge IDs originating from the specified vertex descriptor (bf-directed).
    /// @param vertex The target `vertex_descriptor`.
    [[nodiscard]] gl_attr_force_inline auto out_hyperedge_ids(const vertex_type& vertex) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->out_hyperedge_ids(vertex.id());
    }

    /// @brief Retrieves the out-degree (number of hyperedges where the vertex is in the Tail) of a vertex.
    /// @param vertex_id The raw identifier of the vertex.
    [[nodiscard]] size_type out_degree(const id_type vertex_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.out_degree(vertex_id);
    }

    /// @brief Retrieves the out-degree (number of hyperedges where the vertex is in the Tail) of a vertex descriptor.
    /// @param vertex The target `vertex_descriptor`.
    [[nodiscard]] gl_attr_force_inline size_type out_degree(const vertex_type& vertex) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->out_degree(vertex.id());
    }

    /// @brief Generates a contiguous out-degree map for all valid vertices in the bf-directed hypergraph.
    [[nodiscard]] std::vector<size_type> out_degree_map() const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->_impl.out_degree_map(this->_n_vertices);
    }

    /// @brief Returns a transformed range of fully bound `hyperedge_descriptor`s entering the specified vertex (bf-directed).
    /// @param vertex_id The raw identifier of the vertex.
    [[nodiscard]] gl_attr_force_inline auto in_hyperedges(const id_type vertex_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->in_hyperedge_ids(vertex_id)
             | std::views::transform(this->_create_hyperedge_descriptor());
    }

    /// @brief Returns a transformed range of fully bound `hyperedge_descriptor`s entering the specified vertex descriptor (bf-directed).
    /// @param vertex The target `vertex_descriptor`.
    [[nodiscard]] gl_attr_force_inline auto in_hyperedges(const vertex_type& vertex) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->in_hyperedges(vertex.id());
    }

    /// @brief Returns a lightweight range of raw hyperedge IDs entering the specified vertex (bf-directed).
    /// @param vertex_id The raw identifier of the vertex.
    [[nodiscard]] auto in_hyperedge_ids(const id_type vertex_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.in_hyperedges(vertex_id);
    }

    /// @brief Returns a lightweight range of raw hyperedge IDs entering the specified vertex descriptor (bf-directed).
    /// @param vertex The target `vertex_descriptor`.
    [[nodiscard]] gl_attr_force_inline auto in_hyperedge_ids(const vertex_type& vertex) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->in_hyperedge_ids(vertex.id());
    }

    /// @brief Retrieves the in-degree (number of hyperedges where the vertex is in the Head) of a vertex.
    /// @param vertex_id The raw identifier of the vertex.
    [[nodiscard]] size_type in_degree(const id_type vertex_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.in_degree(vertex_id);
    }

    /// @brief Retrieves the in-degree (number of hyperedges where the vertex is in the Head) of a vertex descriptor.
    /// @param vertex The target `vertex_descriptor`.
    [[nodiscard]] gl_attr_force_inline size_type in_degree(const vertex_type& vertex) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->in_degree(vertex.id());
    }

    /// @brief Generates a contiguous in-degree map for all valid vertices in the bf-directed hypergraph.
    [[nodiscard]] std::vector<size_type> in_degree_map() const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->_impl.in_degree_map(this->_n_vertices);
    }

    /// @brief Returns a transformed range of fully bound `vertex_descriptor`s incident to the specified hyperedge.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    [[nodiscard]] auto incident_vertices(const id_type hyperedge_id) const {
        return this->incident_vertex_ids(hyperedge_id)
             | std::views::transform(this->_create_vertex_descriptor());
    }

    /// @brief Returns a transformed range of fully bound `vertex_descriptor`s incident to the specified hyperedge descriptor.
    /// @param hyperedge The target `hyperedge_descriptor`.
    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const hyperedge_type& hyperedge
    ) const {
        return this->incident_vertices(hyperedge.id());
    }

    /// @brief Returns a lightweight range of raw vertex IDs incident to the specified hyperedge.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    [[nodiscard]] auto incident_vertex_ids(const id_type hyperedge_id) const {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.incident_vertices(hyperedge_id);
    }

    /// @brief Returns a lightweight range of raw vertex IDs incident to the specified hyperedge descriptor.
    /// @param hyperedge The target `hyperedge_descriptor`.
    [[nodiscard]] gl_attr_force_inline auto incident_vertex_ids(const hyperedge_type& hyperedge
    ) const {
        return this->incident_vertex_ids(hyperedge.id());
    }

    /// @brief Retrieves the size (total number of incident vertices) of a hyperedge.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    [[nodiscard]] size_type hyperedge_size(const id_type hyperedge_id) const {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.hyperedge_size(hyperedge_id);
    }

    /// @brief Retrieves the size (total number of incident vertices) of a hyperedge descriptor.
    /// @param hyperedge The target `hyperedge_descriptor`.
    [[nodiscard]] gl_attr_force_inline size_type hyperedge_size(const hyperedge_type& hyperedge
    ) const {
        return this->hyperedge_size(hyperedge.id());
    }

    /// @brief Generates a contiguous map of sizes for all valid hyperedges in the hypergraph.
    [[nodiscard]] std::vector<size_type> hyperedge_size_map() const {
        return this->_impl.hyperedge_size_map(this->_n_hyperedges);
    }

    /// @brief Returns a transformed range of fully bound `vertex_descriptor`s in the tail of the specified bf-directed hyperedge.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    [[nodiscard]] gl_attr_force_inline auto tail(const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->tail_ids(hyperedge_id)
             | std::views::transform(this->_create_vertex_descriptor());
    }

    /// @brief Returns a transformed range of fully bound `vertex_descriptor`s in the tail of the specified bf-directed hyperedge descriptor.
    /// @param hyperedge The target `hyperedge_descriptor`.
    [[nodiscard]] gl_attr_force_inline auto tail(const hyperedge_type& hyperedge) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->tail(hyperedge.id());
    }

    /// @brief Returns a lightweight range of raw vertex IDs in the tail of the specified bf-directed hyperedge.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    [[nodiscard]] auto tail_ids(const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.tail(hyperedge_id);
    }

    /// @brief Returns a lightweight range of raw vertex IDs in the tail of the specified bf-directed hyperedge descriptor.
    /// @param hyperedge The target `hyperedge_descriptor`.
    [[nodiscard]] gl_attr_force_inline auto tail_ids(const hyperedge_type& hyperedge) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->tail_ids(hyperedge.id());
    }

    /// @brief Retrieves the tail size (number of tail vertices) of a bf-directed hyperedge.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    [[nodiscard]] size_type tail_size(const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.tail_size(hyperedge_id);
    }

    /// @brief Retrieves the tail size (number of tail vertices) of a bf-directed hyperedge descriptor.
    /// @param hyperedge The target `hyperedge_descriptor`.
    [[nodiscard]] gl_attr_force_inline size_type tail_size(const hyperedge_type& hyperedge) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->tail_size(hyperedge.id());
    }

    /// @brief Generates a contiguous map of tail sizes for all valid bf-directed hyperedges.
    [[nodiscard]] std::vector<size_type> tail_size_map() const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->_impl.tail_size_map(this->_n_hyperedges);
    }

    /// @brief Returns a transformed range of fully bound `vertex_descriptor`s in the head of the specified bf-directed hyperedge.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    [[nodiscard]] gl_attr_force_inline auto head(const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->head_ids(hyperedge_id)
             | std::views::transform(this->_create_vertex_descriptor());
    }

    /// @brief Returns a transformed range of fully bound `vertex_descriptor`s in the head of the specified bf-directed hyperedge descriptor.
    /// @param hyperedge The target `hyperedge_descriptor`.
    [[nodiscard]] gl_attr_force_inline auto head(const hyperedge_type& hyperedge) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->head(hyperedge.id());
    }

    /// @brief Returns a lightweight range of raw vertex IDs in the head of the specified bf-directed hyperedge.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    [[nodiscard]] auto head_ids(const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.head(hyperedge_id);
    }

    /// @brief Returns a lightweight range of raw vertex IDs in the head of the specified bf-directed hyperedge descriptor.
    /// @param hyperedge The target `hyperedge_descriptor`.
    [[nodiscard]] gl_attr_force_inline auto head_ids(const hyperedge_type& hyperedge) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->head_ids(hyperedge.id());
    }

    /// @brief Retrieves the head size (number of head vertices) of a bf-directed hyperedge.
    /// @param hyperedge_id The raw identifier of the hyperedge.
    [[nodiscard]] size_type head_size(const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.head_size(hyperedge_id);
    }

    /// @brief Retrieves the head size (number of head vertices) of a bf-directed hyperedge descriptor.
    /// @param hyperedge The target `hyperedge_descriptor`.
    [[nodiscard]] gl_attr_force_inline size_type head_size(const hyperedge_type& hyperedge) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->head_size(hyperedge.id());
    }

    /// @brief Generates a contiguous map of head sizes for all valid bf-directed hyperedges.
    [[nodiscard]] std::vector<size_type> head_size_map() const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->_impl.head_size_map(this->_n_hyperedges);
    }

    // --- comparison ---

    /// @brief Compares two hypergraphs for strict equality, checking topologies and properties.
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

    /// @brief Helper structure used to correctly format an individual hyperedge into an output stream.
    struct hyperedge_formatter {
    public:
        /// @brief The hypergraph owning the hyperedge.
        const hypergraph& hg;
        /// @brief The hyperedge descriptor to be formatted.
        const hyperedge_type hyperedge;

        /// @brief Stream insertion operator for undirected hyperedge formatters.
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

        /// @brief Stream insertion operator for bf-directed hyperedge formatters.
        friend std::ostream& operator<<(std::ostream& os, const hyperedge_formatter& proxy)
        requires std::same_as<directional_tag, bf_directed_t>
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

    /// @brief Retrieves a formatter object that safely encapsulates a hyperedge for stream output.
    /// @param hyperedge The target `hyperedge_descriptor`.
    /// @return A `hyperedge_formatter` structure prepared for standard stream insertion.
    [[nodiscard]] hyperedge_formatter display(const hyperedge_type& hyperedge) const {
        return hyperedge_formatter{*this, hyperedge};
    }

    /// @brief Formats and outputs the entire hypergraph structure to a standard output stream.
    ///
    /// Behaves dynamically based on whether the `gl::io::verbose` or `gl::io::spec_fmt` stream manipulators are set.
    ///
    /// @param os The target output stream.
    /// @param hg The hypergraph instance to serialize or print.
    /// @return The stream reference for chaining.
    friend std::ostream& operator<<(std::ostream& os, const hypergraph& hg) {
        using enum io::detail::option_bit;

        if (gl::io::is_option_set(os, spec_fmt))
            return hg._hgsf_write(os);

        if (gl::io::is_option_set(os, verbose))
            return hg._verbose_write(os);

        return hg._concise_write(os);
    }

    /// @brief Reads and deserializes an entire hypergraph from a given stream using HGSF format.
    /// @param is The input stream containing HGSF formatted data.
    /// @param hg The hypergraph instance to populate.
    /// @return The stream reference for chaining.
    friend gl_attr_force_inline std::istream& operator>>(std::istream& is, hypergraph& hg) {
        return hg._hgsf_read(is);
    }

    // --- friend declarations ---

    /// @brief Creates a deep copy of a given hypergraph.
    template <traits::c_hypergraph Hypergraph>
    friend Hypergraph clone(const Hypergraph& source);

    /// @brief Converts a hypergraph to a different implementation type.
    template <traits::c_hypergraph_impl_tag TargetImplTag, traits::c_hypergraph Hypergraph>
    friend auto to(Hypergraph&& source);

    /// @brief Internal structure for dispatching implementation conversion.
    template <
        traits::c_hypergraph_impl_tag TargetImplTag,
        traits::c_hypergraph_impl_tag SourceImplTag>
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

    gl_attr_force_inline auto _create_hyperedge_descriptor() const noexcept
    requires(traits::c_empty_properties<hyperedge_properties_type>)
    {
        return [](const id_type id) { return hyperedge_type{id}; };
    }

    gl_attr_force_inline auto _create_hyperedge_descriptor() const noexcept
    requires(traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return [&pmap = this->_hyperedge_properties](const id_type id) {
            return hyperedge_type{id, pmap[to_idx(id)]};
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
                os << "  - " << this->display(edge) << '\n';
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
                return this->display(hyperedge);
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
    requires std::same_as<directional_tag, bf_directed_t>
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

    /// @brief The current count of initialized vertices.
    size_type _n_vertices = 0uz;
    /// @brief The current count of initialized hyperedges.
    size_type _n_hyperedges = 0uz;

    /// @brief The underlying container implementation (matrix or list).
    implementation_type _impl{};

    /// @todo Replace mutability with proper const-correct getter overloads to ensure thread safety guarantees associated with the const qualifier
    [[no_unique_address]] mutable vertex_properties_map_type _vertex_properties{};

    /// @todo Replace mutability with proper const-correct getter overloads to ensure thread safety guarantees associated with the const qualifier
    [[no_unique_address]] mutable hyperedge_properties_map_type _hyperedge_properties{};
};

// --- general hypergraph utility ---

template <traits::c_hypergraph Hypergraph>
[[nodiscard]] Hypergraph clone(const Hypergraph& source) {
    return Hypergraph(source);
}

/// @ingroup HGL-Core
/// @brief Convenience alias for an undirected hypergraph.
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_hypergraph_impl_tag ImplTag = impl::list_t<>>
using undirected_hypergraph =
    hypergraph<undirected_hypergraph_traits<VertexProperties, HyperedgeProperties, ImplTag>>;

/// @ingroup HGL-Core
/// @brief Convenience alias for a bf-directed hypergraph.
template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_hypergraph_impl_tag ImplTag = impl::list_t<>>
using bf_directed_hypergraph =
    hypergraph<bf_directed_hypergraph_traits<VertexProperties, HyperedgeProperties, ImplTag>>;

/// @ingroup HGL-Core
/// @brief Convenience alias for a hypergraph backed by a standard incidence list.
template <
    traits::c_hypergraph_layout_tag LayoutTag = impl::bidirectional_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using list_hypergraph = hypergraph<
    list_hypergraph_traits<LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType>>;

/// @ingroup HGL-Core
/// @brief Convenience alias for a hypergraph backed by a flat incidence list.
template <
    traits::c_hypergraph_layout_tag LayoutTag = impl::bidirectional_t,
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
/// @brief Convenience alias for a hypergraph backed by a standard incidence matrix.
template <
    traits::c_hypergraph_layout_tag LayoutTag = impl::bidirectional_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using matrix_hypergraph = hypergraph<
    matrix_hypergraph_traits<LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType>>;

/// @ingroup HGL-Core
/// @brief Convenience alias for a hypergraph backed by a flat incidence matrix.
template <
    traits::c_hypergraph_layout_tag LayoutTag = impl::bidirectional_t,
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

// --- degree bounds ---

/// @ingroup HGL-Algorithm
/// @brief Calculates the maximum degree among all vertices in a hypergraph.
[[nodiscard]] size_type max_degree(const traits::c_hypergraph auto& hypergraph) noexcept {
    const auto degrees = hypergraph.degree_map();
    return degrees.empty() ? 0uz : *std::ranges::max_element(degrees);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the minimum degree among all vertices in a hypergraph.
[[nodiscard]] size_type min_degree(const traits::c_hypergraph auto& hypergraph) noexcept {
    const auto degrees = hypergraph.degree_map();
    return degrees.empty() ? 0uz : *std::ranges::min_element(degrees);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the maximum out-degree among all vertices in a bf-directed hypergraph.
[[nodiscard]] size_type max_out_degree(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto degrees = hypergraph.out_degree_map();
    return degrees.empty() ? 0uz : *std::ranges::max_element(degrees);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the minimum out-degree among all vertices in a bf-directed hypergraph.
[[nodiscard]] size_type min_out_degree(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto degrees = hypergraph.out_degree_map();
    return degrees.empty() ? 0uz : *std::ranges::min_element(degrees);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the maximum in-degree among all vertices in a bf-directed hypergraph.
[[nodiscard]] size_type max_in_degree(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto degrees = hypergraph.in_degree_map();
    return degrees.empty() ? 0uz : *std::ranges::max_element(degrees);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the minimum in-degree among all vertices in a bf-directed hypergraph.
[[nodiscard]] size_type min_in_degree(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto degrees = hypergraph.in_degree_map();
    return degrees.empty() ? 0uz : *std::ranges::min_element(degrees);
}

// --- hyperedge size bounds ---

/// @ingroup HGL-Algorithm
/// @brief Calculates the rank of a hypergraph, defined as the maximum size of any valid hyperedge.
[[nodiscard]] size_type rank(const traits::c_hypergraph auto& hypergraph) noexcept {
    const auto sizes = hypergraph.hyperedge_size_map();
    return sizes.empty() ? 0uz : *std::ranges::max_element(sizes);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the corank of a hypergraph, defined as the minimum size of any valid hyperedge.
[[nodiscard]] size_type corank(const traits::c_hypergraph auto& hypergraph) noexcept {
    const auto sizes = hypergraph.hyperedge_size_map();
    return sizes.empty() ? 0uz : *std::ranges::min_element(sizes);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the maximum tail size among all hyperedges in a bf-directed hypergraph.
[[nodiscard]] size_type max_tail_size(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto sizes = hypergraph.tail_size_map();
    return sizes.empty() ? 0uz : *std::ranges::max_element(sizes);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the minimum tail size among all hyperedges in a bf-directed hypergraph.
[[nodiscard]] size_type min_tail_size(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto sizes = hypergraph.tail_size_map();
    return sizes.empty() ? 0uz : *std::ranges::min_element(sizes);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the maximum head size among all hyperedges in a bf-directed hypergraph.
[[nodiscard]] size_type max_head_size(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto sizes = hypergraph.head_size_map();
    return sizes.empty() ? 0uz : *std::ranges::max_element(sizes);
}

/// @ingroup HGL-Algorithm
/// @brief Calculates the minimum head size among all hyperedges in a bf-directed hypergraph.
[[nodiscard]] size_type min_head_size(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto sizes = hypergraph.head_size_map();
    return sizes.empty() ? 0uz : *std::ranges::min_element(sizes);
}

// --- regularity ---

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given hypergraph is $k$-regular (all vertices have a degree of $k$).
[[nodiscard]] bool is_regular(
    const traits::c_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.degree_map(), k);
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given hypergraph is structurally regular (all vertices have the same degree).
[[nodiscard]] bool is_regular(const traits::c_hypergraph auto& hypergraph) noexcept {
    return util::is_constant(hypergraph.degree_map());
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given bf-directed hypergraph is out-$k$-regular (all vertices have an out-degree of $k$).
[[nodiscard]] bool is_out_regular(
    const traits::c_bf_directed_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.out_degree_map(), k);
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given bf-directed hypergraph is out-regular (all vertices have the same out-degree).
[[nodiscard]] bool is_out_regular(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    return util::is_constant(hypergraph.out_degree_map());
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given bf-directed hypergraph is in-$k$-regular (all vertices have an in-degree of $k$).
[[nodiscard]] bool is_in_regular(
    const traits::c_bf_directed_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.in_degree_map(), k);
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given bf-directed hypergraph is in-regular (all vertices have the same in-degree).
[[nodiscard]] bool is_in_regular(const traits::c_bf_directed_hypergraph auto& hypergraph) noexcept {
    return util::is_constant(hypergraph.in_degree_map());
}

// --- uniformity ---

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given hypergraph is $k$-uniform (all hyperedges have a size of $k$).
[[nodiscard]] bool is_uniform(
    const traits::c_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.hyperedge_size_map(), k);
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given hypergraph is structurally uniform (all hyperedges have the exact same size).
[[nodiscard]] bool is_uniform(const traits::c_hypergraph auto& hypergraph) noexcept {
    return util::is_constant(hypergraph.hyperedge_size_map());
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given bf-directed hypergraph is tail-$k$-uniform (all hyperedges have a tail size of $k$).
[[nodiscard]] bool is_tail_uniform(
    const traits::c_bf_directed_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.tail_size_map(), k);
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given bf-directed hypergraph is tail-uniform (all hyperedges have the exact same tail size).
[[nodiscard]] bool is_tail_uniform(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    return util::is_constant(hypergraph.tail_size_map());
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given bf-directed hypergraph is head-$k$-uniform (all hyperedges have a head size of $k$).
[[nodiscard]] bool is_head_uniform(
    const traits::c_bf_directed_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.head_size_map(), k);
}

/// @ingroup HGL-Algorithm
/// @brief Evaluates whether the given bf-directed hypergraph is head-uniform (all hyperedges have the exact same head size).
[[nodiscard]] bool is_head_uniform(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    return util::is_constant(hypergraph.head_size_map());
}

} // namespace hgl
