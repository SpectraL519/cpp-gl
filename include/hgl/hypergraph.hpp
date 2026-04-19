// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

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
#include <memory>
#include <set>
#include <type_traits>
#include <vector>

namespace hgl {

template <traits::c_instantiation_of<hypergraph_traits> HypergraphTraits = hypergraph_traits<>>
class hypergraph;

// --- general hypergraph utility ---

namespace traits {

template <typename H>
concept c_hypergraph = c_instantiation_of<H, hypergraph>;

template <typename H>
concept c_undirected_hypergraph =
    c_hypergraph<H> and std::same_as<typename H::directional_tag, undirected_t>;

template <typename H>
concept c_bf_directed_hypergraph =
    c_hypergraph<H> and std::same_as<typename H::directional_tag, bf_directed_t>;
template <typename H>
concept c_list_hypergraph =
    c_hypergraph<H> and c_hypergraph_list_impl<typename H::implementation_tag>;

template <typename H>
concept c_flat_list_hypergraph =
    c_hypergraph<H> and c_hypergraph_flat_list_impl<typename H::implementation_tag>;

template <typename H>
concept c_incidence_list_hypergraph =
    c_hypergraph<H> and c_hypergraph_incidence_list_impl<typename H::implementation_tag>;

template <typename H>
concept c_matrix_hypergraph =
    c_hypergraph<H> and c_hypergraph_matrix_impl<typename H::implementation_tag>;

template <typename H>
concept c_incidence_matrix_hypergraph =
    c_hypergraph<H> and c_hypergraph_incidence_matrix_impl<typename H::implementation_tag>;

} // namespace traits

template <traits::c_hypergraph Hypergraph>
[[nodiscard]] Hypergraph clone(const Hypergraph& source);

template <traits::c_hypergraph_impl_tag TargetImplTag, traits::c_hypergraph Hypergraph>
[[nodiscard]] auto to(Hypergraph&& source);

namespace detail {

template <traits::c_hypergraph_impl_tag TargetImplTag, traits::c_hypergraph_impl_tag SourceImplTag>
struct to_impl;

} // namespace detail

template <traits::c_instantiation_of<hypergraph_traits> HypergraphTraits>
class hypergraph final {
public:
    using traits_type = HypergraphTraits;
    using directional_tag = typename traits_type::directional_tag;
    using implementation_tag = typename traits_type::implementation_tag;
    using implementation_type =
        typename implementation_tag::template implementation_type<directional_tag>;
    using id_type = typename traits_type::id_type;

    using vertex_type = typename traits_type::vertex_type;
    using vertex_properties_type = typename traits_type::vertex_properties_type;
    using vertex_properties_map_type = std::conditional_t<
        traits::c_empty_properties<vertex_properties_type>,
        empty_properties_map,
        std::vector<std::unique_ptr<vertex_properties_type>>>;

    using hyperedge_type = typename traits_type::hyperedge_type;
    using hyperedge_properties_type = typename traits_type::hyperedge_properties_type;
    using hyperedge_properties_map_type = std::conditional_t<
        traits::c_empty_properties<hyperedge_properties_type>,
        empty_properties_map,
        std::vector<std::unique_ptr<hyperedge_properties_type>>>;

    hypergraph& operator=(const hypergraph&) = delete;

    explicit hypergraph(const size_type n_vertices = 0uz, const size_type n_hyperedges = 0uz)
    : _n_vertices(n_vertices), _n_hyperedges(n_hyperedges), _impl(n_vertices, n_hyperedges) {
        if constexpr (traits::c_non_empty_properties<vertex_properties_type>) {
            this->_vertex_properties.reserve(n_vertices);
            for (const auto _ : this->vertex_ids())
                this->_vertex_properties.push_back(std::make_unique<vertex_properties_type>());
        }

        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>) {
            this->_hyperedge_properties.reserve(n_hyperedges);
            for (const auto _ : this->hyperedge_ids())
                this->_hyperedge_properties.push_back(std::make_unique<hyperedge_properties_type>()
                );
        }
    }

    hypergraph(hypergraph&&) noexcept = default;
    hypergraph& operator=(hypergraph&&) noexcept = default;

    ~hypergraph() = default;

    // --- size methods ---

    [[nodiscard]] gl_attr_force_inline size_type n_vertices() const noexcept {
        return this->_n_vertices;
    }

    [[nodiscard]] gl_attr_force_inline size_type n_hyperedges() const noexcept {
        return this->_n_hyperedges;
    }

    // --- vertex modifiers ---

    vertex_type add_vertex() {
        this->_impl.add_vertices(1uz);
        const auto new_vertex_id = static_cast<id_type>(this->_n_vertices++);

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_type{
                new_vertex_id,
                *this->_vertex_properties.emplace_back(std::make_unique<vertex_properties_type>())
            };
        else
            return vertex_type{new_vertex_id};
    }

    vertex_type add_vertex_with(vertex_properties_type properties)
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        this->_impl.add_vertices(1uz);
        return vertex_type{
            static_cast<id_type>(this->_n_vertices++),
            *this->_vertex_properties.emplace_back(
                std::make_unique<vertex_properties_type>(std::move(properties))
            )
        };
    }

    void add_vertices(const size_type n) {
        this->_impl.add_vertices(n);
        this->_n_vertices += n;

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>) {
            const auto old_size = this->_vertex_properties.size();
            this->_vertex_properties.reserve(this->_n_vertices);
            for (size_type i = old_size; i < this->_n_vertices; ++i)
                this->_vertex_properties.push_back(std::make_unique<vertex_properties_type>());
        }
    }

    void add_vertices_with(
        const traits::c_sized_range_of<vertex_properties_type> auto& properties_rng
    )
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        const auto n = std::ranges::size(properties_rng);

        this->_impl.add_vertices(n);
        this->_n_vertices += n;

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>) {
            for (auto& properties : properties_rng) {
                this->_vertex_properties.emplace_back(
                    std::make_unique<vertex_properties_type>(properties)
                );
            }
        }
    }

    gl_attr_force_inline void remove_vertex(const id_type vertex_id) {
        this->_remove_vertex_impl(vertex_id);
    }

    gl_attr_force_inline void remove_vertex(const vertex_type& vertex) {
        this->remove_vertex(vertex.id());
    }

    void remove_vertices_from(const traits::c_forward_range_of<id_type> auto& vertex_id_rng) {
        // sorts ids in a descending n_vertices and removes duplicate ids
        std::set<id_type, std::greater<id_type>> vertex_id_set(
            std::ranges::begin(vertex_id_rng), std::ranges::end(vertex_id_rng)
        );

        // TODO: optimize
        for (const auto vertex_id : vertex_id_set)
            this->_remove_vertex_impl(vertex_id);
    }

    void remove_vertices_from(const traits::c_sized_range_of<vertex_type> auto& vertex_rng) {
        // sort vertices in a descending n_vertices (by id) and removes duplicate ids
        std::set<vertex_type, std::greater<vertex_type>> vertex_set(
            std::ranges::begin(vertex_rng), std::ranges::end(vertex_rng)
        );

        // TODO: optimize
        for (const auto& vertex : vertex_set)
            this->_remove_vertex_impl(vertex.id());
    }

    // --- vertex getters ---

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const id_type vertex_id) const {
        return vertex_id < this->_n_vertices;
    }

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const vertex_type& vertex) const {
        return this->has_vertex(vertex.id());
    }

    [[nodiscard]] vertex_type vertex(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->vertex_unchecked(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline vertex_type vertex_unchecked(const id_type vertex_id) const {
        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_type{vertex_id, *this->_vertex_properties[vertex_id]};
        else
            return vertex_type{vertex_id};
    }

    [[nodiscard]] gl_attr_force_inline auto vertices() const noexcept {
        return this->vertex_ids() | std::views::transform(this->_create_vertex_descriptor());
    }

    [[nodiscard]] gl_attr_force_inline auto vertex_ids() const noexcept {
        return std::views::iota(initial_id_v<id_type>, this->_n_vertices);
    }

    [[nodiscard]] gl_attr_force_inline auto vertex_properties_map() const noexcept
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        return util::deref_view(this->_vertex_properties);
    }

    [[nodiscard]] gl_attr_force_inline vertex_properties_type& vertex_properties(
        const id_type vertex_id
    ) const
    requires(traits::c_non_empty_properties<vertex_properties_type>)
    {
        this->_verify_vertex_id(vertex_id);
        return *this->_vertex_properties[vertex_id];
    }

    // --- hyperedge modifiers ---

    hyperedge_type add_hyperedge() {
        this->_impl.add_hyperedges(1uz);
        const auto new_hyperedge_id = static_cast<id_type>(this->_n_hyperedges++);

        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>)
            return hyperedge_type{
                new_hyperedge_id,
                *this->_hyperedge_properties.emplace_back(
                    std::make_unique<hyperedge_properties_type>()
                )
            };
        else
            return hyperedge_type{new_hyperedge_id};
    }

    hyperedge_type add_hyperedge_with(hyperedge_properties_type properties)
    requires(traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        this->_impl.add_hyperedges(1uz);
        return hyperedge_type{
            static_cast<id_type>(this->_n_hyperedges++),
            *this->_hyperedge_properties.emplace_back(
                std::make_unique<hyperedge_properties_type>(std::move(properties))
            )
        };
    }

    hyperedge_type add_hyperedge(const traits::c_forward_range_of<id_type> auto& vertex_id_rng)
    requires std::same_as<directional_tag, undirected_t>
    {
        auto he = this->add_hyperedge();
        this->bind(vertex_id_rng, he.id());
        return he;
    }

    gl_attr_force_inline hyperedge_type add_hyperedge(std::initializer_list<id_type> vertex_ids)
    requires std::same_as<directional_tag, undirected_t>
    {
        return this->add_hyperedge(std::views::all(vertex_ids));
    }

    gl_attr_force_inline hyperedge_type
    add_hyperedge(const traits::c_forward_range_of<vertex_type> auto& vertex_rng)
    requires std::same_as<directional_tag, undirected_t>
    {
        return this->add_hyperedge(vertex_rng | std::views::transform(&vertex_type::id));
    }

    gl_attr_force_inline hyperedge_type add_hyperedge(std::initializer_list<vertex_type> vertices)
    requires std::same_as<directional_tag, undirected_t>
    {
        return this->add_hyperedge(std::views::all(vertices));
    }

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

    hyperedge_type add_hyperedge_with(
        std::initializer_list<id_type> vertex_ids, hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, undirected_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return this->add_hyperedge_with(std::views::all(vertex_ids), std::move(properties));
    }

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

    hyperedge_type add_hyperedge_with(
        std::initializer_list<vertex_type> vertices, hyperedge_properties_type properties
    )
    requires(std::same_as<directional_tag, undirected_t> and traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return this->add_hyperedge_with(std::views::all(vertices), std::move(properties));
    }

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

    gl_attr_force_inline hyperedge_type
    add_hyperedge(std::initializer_list<id_type> tail_ids, std::initializer_list<id_type> head_ids)
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->add_hyperedge(std::views::all(tail_ids), std::views::all(head_ids));
    }

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

    gl_attr_force_inline hyperedge_type
    add_hyperedge(std::initializer_list<vertex_type> tail, std::initializer_list<vertex_type> head)
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->add_hyperedge(std::views::all(tail), std::views::all(head));
    }

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

    void add_hyperedges(const size_type n) {
        this->_impl.add_hyperedges(n);
        this->_n_hyperedges += n;

        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>) {
            const auto old_size = this->_hyperedge_properties.size();
            this->_hyperedge_properties.reserve(this->_n_hyperedges);
            for (auto i = old_size; i < this->_n_hyperedges; ++i)
                this->_hyperedge_properties.push_back(std::make_unique<hyperedge_properties_type>()
                );
        }
    }

    void add_hyperedges_with(
        const traits::c_sized_range_of<hyperedge_properties_type> auto& properties_rng
    )
    requires(traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        const auto n = std::ranges::size(properties_rng);

        this->_impl.add_hyperedges(n);
        this->_n_hyperedges += n;

        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>) {
            for (auto& properties : properties_rng) {
                this->_hyperedge_properties.emplace_back(
                    std::make_unique<hyperedge_properties_type>(properties)
                );
            }
        }
    }

    gl_attr_force_inline void remove_hyperedge(const id_type hyperedge_id) {
        this->_remove_hyperedge_impl(hyperedge_id);
    }

    gl_attr_force_inline void remove_hyperedge(const hyperedge_type& hyperedge) {
        this->remove_hyperedge(hyperedge.id());
    }

    void remove_hyperedges_from(const traits::c_forward_range_of<id_type> auto& hyperedge_id_rng) {
        // sorts ids in a descending n_vertices and removes duplicate ids
        std::set<id_type, std::greater<id_type>> hyperedge_id_set(
            std::ranges::begin(hyperedge_id_rng), std::ranges::end(hyperedge_id_rng)
        );

        // TODO: optimize
        for (const auto hyperedge_id : hyperedge_id_set)
            this->_remove_hyperedge_impl(hyperedge_id);
    }

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

    [[nodiscard]] gl_attr_force_inline bool has_hyperedge(const id_type hyperedge_id) const {
        return hyperedge_id < this->_n_hyperedges;
    }

    [[nodiscard]] gl_attr_force_inline bool has_hyperedge(const hyperedge_type& hyperedge) const {
        return this->has_hyperedge(hyperedge.id());
    }

    [[nodiscard]] hyperedge_type hyperedge(const id_type hyperedge_id) const {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->hyperedge_unchecked(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline hyperedge_type hyperedge_unchecked(const id_type hyperedge_id
    ) const {
        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>)
            return hyperedge_type{hyperedge_id, *this->_hyperedge_properties[hyperedge_id]};
        else
            return hyperedge_type{hyperedge_id};
    }

    [[nodiscard]] gl_attr_force_inline auto hyperedges() const noexcept {
        return this->hyperedge_ids() | std::views::transform(this->_create_hyperedge_descriptor());
    }

    [[nodiscard]] gl_attr_force_inline auto hyperedge_ids() const noexcept {
        return std::views::iota(initial_id_v<id_type>, this->_n_hyperedges);
    }

    [[nodiscard]] gl_attr_force_inline auto hyperedge_properties_map() const noexcept
    requires(traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        return util::deref_view(this->_hyperedge_properties);
    }

    [[nodiscard]] gl_attr_force_inline hyperedge_properties_type& hyperedge_properties(
        const id_type id
    ) const
    requires(traits::c_non_empty_properties<hyperedge_properties_type>)
    {
        this->_verify_hyperedge_id(id);
        return *this->_hyperedge_properties[id];
    }

    // --- incidence modifiers ---

    void bind(const id_type vertex_id, const id_type hyperedge_id)
    requires std::same_as<directional_tag, undirected_t>
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        this->_impl.bind(vertex_id, hyperedge_id);
    }

    gl_attr_force_inline void bind(const vertex_type& vertex, const hyperedge_type& hyperedge)
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex.id(), hyperedge.id());
    }

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

    gl_attr_force_inline void bind(
        std::initializer_list<id_type> vertex_ids, const id_type hyperedge_id
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(std::views::all(vertex_ids), hyperedge_id);
    }

    gl_attr_force_inline void bind(
        const traits::c_forward_range_of<vertex_type> auto& vertex_rng,
        const hyperedge_type& hyperedge
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex_rng | std::views::transform(&vertex_type::id), hyperedge.id());
    }

    gl_attr_force_inline void bind(
        std::initializer_list<vertex_type> vertices, const hyperedge_type& hyperedge
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(std::views::all(vertices), hyperedge);
    }

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

    gl_attr_force_inline void bind(
        const id_type vertex_id, std::initializer_list<id_type> hyperedge_ids
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex_id, std::views::all(hyperedge_ids));
    }

    gl_attr_force_inline void bind(
        const vertex_type& vertex,
        const traits::c_forward_range_of<hyperedge_type> auto& hyperedge_rng
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex.id(), hyperedge_rng | std::views::transform(&hyperedge_type::id));
    }

    gl_attr_force_inline void bind(
        const vertex_type& vertex, std::initializer_list<hyperedge_type> hyperedges
    )
    requires std::same_as<directional_tag, undirected_t>
    {
        this->bind(vertex, std::views::all(hyperedges));
    }

    void bind_tail(const id_type vertex_id, const id_type hyperedge_id)
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        this->_impl.bind_tail(vertex_id, hyperedge_id);
    }

    gl_attr_force_inline void bind_tail(const vertex_type& vertex, const hyperedge_type& hyperedge)
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(vertex.id(), hyperedge.id());
    }

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

    gl_attr_force_inline void bind_tail(
        std::initializer_list<id_type> vertex_ids, const id_type hyperedge_id
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(std::views::all(vertex_ids), hyperedge_id);
    }

    gl_attr_force_inline void bind_tail(
        const traits::c_forward_range_of<vertex_type> auto& vertex_rng,
        const hyperedge_type& hyperedge
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(vertex_rng | std::views::transform(&vertex_type::id), hyperedge.id());
    }

    gl_attr_force_inline void bind_tail(
        std::initializer_list<vertex_type> vertices, const hyperedge_type& hyperedge
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(std::views::all(vertices), hyperedge);
    }

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

    gl_attr_force_inline void bind_tail(
        const id_type vertex_id, std::initializer_list<id_type> hyperedge_ids
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(vertex_id, std::views::all(hyperedge_ids));
    }

    gl_attr_force_inline void bind_tail(
        const vertex_type& vertex,
        const traits::c_forward_range_of<hyperedge_type> auto& hyperedge_rng
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(vertex.id(), hyperedge_rng | std::views::transform(&hyperedge_type::id));
    }

    gl_attr_force_inline void bind_tail(
        const vertex_type& vertex, std::initializer_list<hyperedge_type> hyperedges
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_tail(vertex, std::views::all(hyperedges));
    }

    void bind_head(const id_type vertex_id, const id_type hyperedge_id)
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        this->_impl.bind_head(vertex_id, hyperedge_id);
    }

    gl_attr_force_inline void bind_head(const vertex_type& vertex, const hyperedge_type& hyperedge)
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(vertex.id(), hyperedge.id());
    }

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

    gl_attr_force_inline void bind_head(
        std::initializer_list<id_type> vertex_ids, const id_type hyperedge_id
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(std::views::all(vertex_ids), hyperedge_id);
    }

    gl_attr_force_inline void bind_head(
        const traits::c_forward_range_of<vertex_type> auto& vertex_rng,
        const hyperedge_type& hyperedge
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(vertex_rng | std::views::transform(&vertex_type::id), hyperedge.id());
    }

    gl_attr_force_inline void bind_head(
        std::initializer_list<vertex_type> vertices, const hyperedge_type& hyperedge
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(std::views::all(vertices), hyperedge);
    }

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

    gl_attr_force_inline void bind_head(
        const id_type vertex_id, std::initializer_list<id_type> hyperedge_ids
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(vertex_id, std::views::all(hyperedge_ids));
    }

    gl_attr_force_inline void bind_head(
        const vertex_type& vertex,
        const traits::c_forward_range_of<hyperedge_type> auto& hyperedge_rng
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(vertex.id(), hyperedge_rng | std::views::transform(&hyperedge_type::id));
    }

    gl_attr_force_inline void bind_head(
        const vertex_type& vertex, std::initializer_list<hyperedge_type> hyperedges
    )
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->bind_head(vertex, std::views::all(hyperedges));
    }

    void unbind(const id_type vertex_id, const id_type hyperedge_id) {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        this->_impl.unbind(vertex_id, hyperedge_id);
    }

    gl_attr_force_inline void unbind(const vertex_type& vertex, const hyperedge_type& hyperedge) {
        this->unbind(vertex.id(), hyperedge.id());
    }

    // --- incidence validators ---

    [[nodiscard]] bool are_incident(const id_type vertex_id, const id_type hyperedge_id) const {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.are_bound(vertex_id, hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline bool are_incident(
        const vertex_type& vertex, const hyperedge_type& hyperedge
    ) const {
        return this->are_incident(vertex.id(), hyperedge.id());
    }

    [[nodiscard]] bool is_tail(const id_type vertex_id, const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.is_tail(vertex_id, hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline bool is_tail(
        const vertex_type& vertex, const hyperedge_type& hyperedge
    ) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->is_tail(vertex.id(), hyperedge.id());
    }

    [[nodiscard]] bool is_head(const id_type vertex_id, const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.is_head(vertex_id, hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline bool is_head(
        const vertex_type& vertex, const hyperedge_type& hyperedge
    ) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->is_head(vertex.id(), hyperedge.id());
    }

    // --- incidence getters ---

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const id_type vertex_id) {
        return this->incident_hyperedge_ids(vertex_id)
             | std::views::transform(this->_create_hyperedge_descriptor());
    }

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const vertex_type& vertex) {
        return this->incident_hyperedges(vertex.id());
    }

    [[nodiscard]] auto incident_hyperedge_ids(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.incident_hyperedges(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedge_ids(const vertex_type& vertex
    ) const {
        return this->incident_hyperedge_ids(vertex.id());
    }

    [[nodiscard]] size_type degree(const id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.degree(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type degree(const vertex_type& vertex) const {
        return this->degree(vertex.id());
    }

    [[nodiscard]] std::vector<size_type> degree_map() const {
        return this->_impl.degree_map(this->_n_vertices);
    }

    [[nodiscard]] gl_attr_force_inline auto out_hyperedges(const id_type vertex_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->out_hyperedge_ids(vertex_id)
             | std::views::transform(this->_create_hyperedge_descriptor());
    }

    [[nodiscard]] gl_attr_force_inline auto out_hyperedges(const vertex_type& vertex) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->out_hyperedges(vertex.id());
    }

    [[nodiscard]] auto out_hyperedge_ids(const id_type vertex_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.out_hyperedges(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto out_hyperedge_ids(const vertex_type& vertex) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->out_hyperedge_ids(vertex.id());
    }

    [[nodiscard]] size_type out_degree(const id_type vertex_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.out_degree(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type out_degree(const vertex_type& vertex) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->out_degree(vertex.id());
    }

    [[nodiscard]] std::vector<size_type> out_degree_map() const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->_impl.out_degree_map(this->_n_vertices);
    }

    [[nodiscard]] gl_attr_force_inline auto in_hyperedges(const id_type vertex_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->in_hyperedge_ids(vertex_id)
             | std::views::transform(this->_create_hyperedge_descriptor());
    }

    [[nodiscard]] gl_attr_force_inline auto in_hyperedges(const vertex_type& vertex) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->in_hyperedges(vertex.id());
    }

    [[nodiscard]] auto in_hyperedge_ids(const id_type vertex_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.in_hyperedges(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto in_hyperedge_ids(const vertex_type& vertex) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->in_hyperedge_ids(vertex.id());
    }

    [[nodiscard]] size_type in_degree(const id_type vertex_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.in_degree(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type in_degree(const vertex_type& vertex) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->in_degree(vertex.id());
    }

    [[nodiscard]] std::vector<size_type> in_degree_map() const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->_impl.in_degree_map(this->_n_vertices);
    }

    [[nodiscard]] auto incident_vertices(const id_type hyperedge_id) const {
        return this->incident_vertex_ids(hyperedge_id)
             | std::views::transform(this->_create_vertex_descriptor());
    }

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const hyperedge_type& hyperedge
    ) const {
        return this->incident_vertices(hyperedge.id());
    }

    [[nodiscard]] auto incident_vertex_ids(const id_type hyperedge_id) const {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.incident_vertices(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_vertex_ids(const hyperedge_type& hyperedge
    ) const {
        return this->incident_vertex_ids(hyperedge.id());
    }

    [[nodiscard]] size_type hyperedge_size(const id_type hyperedge_id) const {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.hyperedge_size(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type hyperedge_size(const hyperedge_type& hyperedge
    ) const {
        return this->hyperedge_size(hyperedge.id());
    }

    [[nodiscard]] std::vector<size_type> hyperedge_size_map() const {
        return this->_impl.hyperedge_size_map(this->_n_hyperedges);
    }

    [[nodiscard]] gl_attr_force_inline auto tail(const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->tail_ids(hyperedge_id)
             | std::views::transform(this->_create_vertex_descriptor());
    }

    [[nodiscard]] gl_attr_force_inline auto tail(const hyperedge_type& hyperedge) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->tail(hyperedge.id());
    }

    [[nodiscard]] auto tail_ids(const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.tail(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline auto tail_ids(const hyperedge_type& hyperedge) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->tail_ids(hyperedge.id());
    }

    [[nodiscard]] size_type tail_size(const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.tail_size(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type tail_size(const hyperedge_type& hyperedge) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->tail_size(hyperedge.id());
    }

    [[nodiscard]] std::vector<size_type> tail_size_map() const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->_impl.tail_size_map(this->_n_hyperedges);
    }

    [[nodiscard]] gl_attr_force_inline auto head(const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->head_ids(hyperedge_id)
             | std::views::transform(this->_create_vertex_descriptor());
    }

    [[nodiscard]] gl_attr_force_inline auto head(const hyperedge_type& hyperedge) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->head(hyperedge.id());
    }

    [[nodiscard]] auto head_ids(const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.head(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline auto head_ids(const hyperedge_type& hyperedge) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->head_ids(hyperedge.id());
    }

    [[nodiscard]] size_type head_size(const id_type hyperedge_id) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        this->_verify_hyperedge_id(hyperedge_id);
        return this->_impl.head_size(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type head_size(const hyperedge_type& hyperedge) const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->head_size(hyperedge.id());
    }

    [[nodiscard]] std::vector<size_type> head_size_map() const
    requires std::same_as<directional_tag, bf_directed_t>
    {
        return this->_impl.head_size_map(this->_n_hyperedges);
    }

    // --- comparison ---

    [[nodiscard]] friend bool operator==(const hypergraph& lhs, const hypergraph& rhs) noexcept {
        constexpr auto val_eq = [](const auto& ptr_a, const auto& ptr_b) {
            return *ptr_a == *ptr_b;
        };

        if (lhs._n_vertices != rhs._n_vertices or lhs._n_hyperedges != rhs._n_hyperedges)
            return false;

        if constexpr (traits::c_non_empty_properties<vertex_properties_type>)
            if (not std::ranges::equal(lhs._vertex_properties, rhs._vertex_properties, val_eq))
                return false;

        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>)
            if (not std::ranges::equal(
                    lhs._hyperedge_properties, rhs._hyperedge_properties, val_eq
                ))
                return false;

        return lhs._impl == rhs._impl;
    }

    // --- I/O utility ---

    struct hyperedge_formatter {
    public:
        const hypergraph& hg;
        const hyperedge_type hyperedge;

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

    [[nodiscard]] hyperedge_formatter display(const hyperedge_type& hyperedge) const {
        return hyperedge_formatter{*this, hyperedge};
    }

    friend std::ostream& operator<<(std::ostream& os, const hypergraph& hg) {
        using enum io::detail::option_bit;

        if (gl::io::is_option_set(os, spec_fmt))
            return hg._hgsf_write(os);

        if (gl::io::is_option_set(os, verbose))
            return hg._verbose_write(os);

        return hg._concise_write(os);
    }

    friend gl_attr_force_inline std::istream& operator>>(std::istream& is, hypergraph& hg) {
        return hg._hgsf_read(is);
    }

    // --- friend declarations ---

    template <traits::c_hypergraph Hypergraph>
    friend Hypergraph clone(const Hypergraph& source);

    template <traits::c_hypergraph_impl_tag TargetImplTag, traits::c_hypergraph Hypergraph>
    friend auto to(Hypergraph&& source);

    template <
        traits::c_hypergraph_impl_tag TargetImplTag,
        traits::c_hypergraph_impl_tag SourceImplTag>
    friend struct detail::to_impl;

private:
    hypergraph(const hypergraph& other)
    : _n_vertices{other._n_vertices}, _n_hyperedges{other._n_hyperedges}, _impl{other._impl} {
        // Deep copy vertex properties
        if constexpr (traits::c_non_empty_properties<vertex_properties_type>) {
            this->_vertex_properties.reserve(other._vertex_properties.size());
            for (const auto& property : other._vertex_properties)
                this->_vertex_properties.push_back(
                    std::make_unique<vertex_properties_type>(*property)
                );
        }

        // Deep copy hyperedge properties
        if constexpr (traits::c_non_empty_properties<hyperedge_properties_type>) {
            this->_hyperedge_properties.reserve(other._hyperedge_properties.size());
            for (const auto& property : other._hyperedge_properties)
                this->_hyperedge_properties.push_back(
                    std::make_unique<hyperedge_properties_type>(*property)
                );
        }
    }

    // --- vertex methods ---

    gl_attr_force_inline void _verify_vertex_id(const id_type vertex_id) const {
        if (not this->has_vertex(vertex_id))
            throw std::out_of_range(std::format("Got invalid vertex id [{}]", vertex_id));
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
            return vertex_type{id, *pmap[to_idx(id)]};
        };
    }

    // --- hyperedge methods ---

    gl_attr_force_inline void _verify_hyperedge_id(const id_type hyperedge_id) const {
        if (not this->has_hyperedge(hyperedge_id))
            throw std::out_of_range(std::format("Got invalid hyperedge id [{}]", hyperedge_id));
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
            return hyperedge_type{id, *pmap[to_idx(id)]};
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

    size_type _n_vertices = 0uz;
    size_type _n_hyperedges = 0uz;

    implementation_type _impl{};

    [[no_unique_address]] vertex_properties_map_type _vertex_properties{};
    [[no_unique_address]] hyperedge_properties_map_type _hyperedge_properties{};
};

// --- general hypergraph utility ---

template <traits::c_hypergraph Hypergraph>
[[nodiscard]] Hypergraph clone(const Hypergraph& source) {
    return Hypergraph(source);
}

template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_hypergraph_impl_tag ImplTag = impl::list_t<>>
using undirected_hypergraph =
    hypergraph<undirected_hypergraph_traits<VertexProperties, HyperedgeProperties, ImplTag>>;

template <
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_hypergraph_impl_tag ImplTag = impl::list_t<>>
using bf_directed_hypergraph =
    hypergraph<bf_directed_hypergraph_traits<VertexProperties, HyperedgeProperties, ImplTag>>;

template <
    traits::c_hypergraph_layout_tag LayoutTag = impl::bidirectional_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using list_hypergraph = hypergraph<
    list_hypergraph_traits<LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType>>;

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

template <
    traits::c_hypergraph_layout_tag LayoutTag = impl::bidirectional_t,
    traits::c_hypergraph_directional_tag DirectionalTag = undirected_t,
    traits::c_properties VertexProperties = empty_properties,
    traits::c_properties HyperedgeProperties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using matrix_hypergraph = hypergraph<
    matrix_hypergraph_traits<LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType>>;

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

[[nodiscard]] size_type max_degree(const traits::c_hypergraph auto& hypergraph) noexcept {
    const auto degrees = hypergraph.degree_map();
    return degrees.empty() ? 0uz : *std::ranges::max_element(degrees);
}

[[nodiscard]] size_type min_degree(const traits::c_hypergraph auto& hypergraph) noexcept {
    const auto degrees = hypergraph.degree_map();
    return degrees.empty() ? 0uz : *std::ranges::min_element(degrees);
}

[[nodiscard]] size_type max_out_degree(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto degrees = hypergraph.out_degree_map();
    return degrees.empty() ? 0uz : *std::ranges::max_element(degrees);
}

[[nodiscard]] size_type min_out_degree(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto degrees = hypergraph.out_degree_map();
    return degrees.empty() ? 0uz : *std::ranges::min_element(degrees);
}

[[nodiscard]] size_type max_in_degree(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto degrees = hypergraph.in_degree_map();
    return degrees.empty() ? 0uz : *std::ranges::max_element(degrees);
}

[[nodiscard]] size_type min_in_degree(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto degrees = hypergraph.in_degree_map();
    return degrees.empty() ? 0uz : *std::ranges::min_element(degrees);
}

// --- hyperedge size bounds ---

[[nodiscard]] size_type rank(const traits::c_hypergraph auto& hypergraph) noexcept {
    const auto sizes = hypergraph.hyperedge_size_map();
    return sizes.empty() ? 0uz : *std::ranges::max_element(sizes);
}

[[nodiscard]] size_type corank(const traits::c_hypergraph auto& hypergraph) noexcept {
    const auto sizes = hypergraph.hyperedge_size_map();
    return sizes.empty() ? 0uz : *std::ranges::min_element(sizes);
}

[[nodiscard]] size_type max_tail_size(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto sizes = hypergraph.tail_size_map();
    return sizes.empty() ? 0uz : *std::ranges::max_element(sizes);
}

[[nodiscard]] size_type min_tail_size(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto sizes = hypergraph.tail_size_map();
    return sizes.empty() ? 0uz : *std::ranges::min_element(sizes);
}

[[nodiscard]] size_type max_head_size(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto sizes = hypergraph.head_size_map();
    return sizes.empty() ? 0uz : *std::ranges::max_element(sizes);
}

[[nodiscard]] size_type min_head_size(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    const auto sizes = hypergraph.head_size_map();
    return sizes.empty() ? 0uz : *std::ranges::min_element(sizes);
}

// --- regularity ---

[[nodiscard]] bool is_regular(
    const traits::c_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.degree_map(), k);
}

[[nodiscard]] bool is_regular(const traits::c_hypergraph auto& hypergraph) noexcept {
    return util::is_constant(hypergraph.degree_map());
}

[[nodiscard]] bool is_out_regular(
    const traits::c_bf_directed_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.out_degree_map(), k);
}

[[nodiscard]] bool is_out_regular(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    return util::is_constant(hypergraph.out_degree_map());
}

[[nodiscard]] bool is_in_regular(
    const traits::c_bf_directed_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.in_degree_map(), k);
}

[[nodiscard]] bool is_in_regular(const traits::c_bf_directed_hypergraph auto& hypergraph) noexcept {
    return util::is_constant(hypergraph.in_degree_map());
}

// --- uniformity ---

[[nodiscard]] bool is_uniform(
    const traits::c_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.hyperedge_size_map(), k);
}

[[nodiscard]] bool is_uniform(const traits::c_hypergraph auto& hypergraph) noexcept {
    return util::is_constant(hypergraph.hyperedge_size_map());
}

[[nodiscard]] bool is_tail_uniform(
    const traits::c_bf_directed_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.tail_size_map(), k);
}

[[nodiscard]] bool is_tail_uniform(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    return util::is_constant(hypergraph.tail_size_map());
}

[[nodiscard]] bool is_head_uniform(
    const traits::c_bf_directed_hypergraph auto& hypergraph, const size_type k
) noexcept {
    return util::all_equal(hypergraph.head_size_map(), k);
}

[[nodiscard]] bool is_head_uniform(const traits::c_bf_directed_hypergraph auto& hypergraph
) noexcept {
    return util::is_constant(hypergraph.head_size_map());
}

} // namespace hgl
