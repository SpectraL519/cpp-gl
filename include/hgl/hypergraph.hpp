// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "constants.hpp"
#include "hypergraph_traits.hpp"
#include "util.hpp"

#include <memory>
#include <set>
#include <type_traits>
#include <vector>

namespace hgl {

template <type_traits::c_instantiation_of<hypergraph_traits> HypergraphTraits = hypergraph_traits<>>
class hypergraph final {
public:
    using traits_type = HypergraphTraits;
    using implementation_tag = typename traits_type::implementation_tag;

    using vertex_type = typename traits_type::vertex_type;
    using vertex_properties_type = typename traits_type::vertex_properties_type;
    using vertex_properties_map_type = std::conditional_t<
        type_traits::c_empty_properties<vertex_properties_type>,
        types::empty_properties_map,
        std::vector<std::unique_ptr<vertex_properties_type>>>;

    using hyperedge_type = typename traits_type::hyperedge_type;
    using hyperedge_properties_type = typename traits_type::hyperedge_properties_type;
    using hyperedge_properties_map_type = std::conditional_t<
        type_traits::c_empty_properties<hyperedge_properties_type>,
        types::empty_properties_map,
        std::vector<std::unique_ptr<hyperedge_properties_type>>>;

    hypergraph(const hypergraph&) = delete;
    hypergraph& operator=(const hypergraph&) = delete;

    hypergraph() = default;

    hypergraph(const types::size_type n_vertices) : _n_vertices(n_vertices) {
        if constexpr (type_traits::c_non_empty_properties<vertex_properties_type>) {
            this->_vertex_properties.reserve(n_vertices);
            for (const auto _ : this->vertex_ids())
                this->_vertex_properties.push_back(std::make_unique<vertex_properties_type>());
        }
    }

    // --- general methods ---

    [[nodiscard]] gl_attr_force_inline types::size_type n_vertices() const noexcept {
        return this->_n_vertices;
    }

    [[nodiscard]] gl_attr_force_inline types::size_type n_hyperedges() const noexcept {
        return this->_n_hyperedges;
    }

    // --- vertex methods ---

    [[nodiscard]] gl_attr_force_inline auto vertices() const
    requires(type_traits::c_empty_properties<vertex_properties_type>)
    {
        return this->vertex_ids()
             | std::views::transform([](const types::id_type id) { return vertex_type{id}; });
    }

    [[nodiscard]] gl_attr_force_inline auto vertices() const
    requires(type_traits::c_non_empty_properties<vertex_properties_type>)
    {
        return this->_vertex_properties | std::views::enumerate
             | std::views::transform([](const auto& x) {
                   const auto& [id, ptr] = x;
                   return vertex_type{static_cast<types::id_type>(id), *ptr};
               });
    }

    [[nodiscard]] gl_attr_force_inline auto vertex_ids() const noexcept {
        return std::views::iota(constants::initial_id, this->_n_vertices);
    }

    [[nodiscard]] vertex_type get_vertex(const types::id_type vertex_id) const {
        this->_verify_vertex_id(vertex_id);
        if constexpr (type_traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_type{vertex_id, *this->_vertex_properties[vertex_id]};
        else
            return vertex_type{vertex_id};
    }

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const types::id_type vertex_id) const {
        return vertex_id < this->_n_vertices;
    }

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const vertex_type& vertex) const {
        return this->has_vertex(vertex.id());
    }

    vertex_type add_vertex() {
        // this->_impl.add_vertex();
        const auto new_vertex_id = this->_n_vertices++;

        if constexpr (type_traits::c_non_empty_properties<vertex_properties_type>)
            return vertex_type{
                new_vertex_id,
                *this->_vertex_properties.emplace_back(std::make_unique<vertex_properties_type>())
            };
        else
            return vertex_type{new_vertex_id};
    }

    vertex_type add_vertex_with(vertex_properties_type properties)
    requires(type_traits::c_non_empty_properties<vertex_properties_type>)
    {
        // this->_impl.add_vertex();
        this->_vertex_properties.push_back(
            std::make_unique<vertex_properties_type>(std::move(properties))
        );
        return vertex_type{this->_n_vertices++, *this->_vertex_properties.back()};
    }

    void add_vertices(const types::size_type n) {
        // this->_impl.add_vertices(n);
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

        // this->_impl.add_vertices(n);
        this->_n_vertices += n;

        if constexpr (type_traits::c_non_empty_properties<vertex_properties_type>) {
            for (auto& properties : properties_range) {
                this->_vertex_properties.emplace_back(
                    std::make_unique<vertex_properties_type>(properties)
                );
            }
        }
    }

    void remove_vertex(const types::size_type vertex_id) {
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
        // sort the ids in a descending order and removes duplicate ids
        std::set<vertex_type, std::greater<vertex_type>> vertex_set(
            std::ranges::begin(vertex_range), std::ranges::end(vertex_range)
        );

        // TODO: optimize
        for (const auto& vertex : vertex_set)
            this->_remove_vertex_impl(vertex.id());
    }

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

private:
    // --- vertex methods ---

    gl_attr_force_inline void _verify_vertex_id(const types::id_type vertex_id) const {
        if (not this->has_vertex(vertex_id))
            throw std::out_of_range(std::format("Got invalid vertex id [{}]", vertex_id));
    }

    void _remove_vertex_impl(const types::id_type vertex_id) {
        // const auto removed_edge_ids = this->_impl.remove_vertex(vertex_id);
        this->_n_vertices--;
        // this->_n_edges -= removed_edge_ids.size();

        if constexpr (type_traits::c_non_empty_properties<vertex_properties_type>)
            this->_vertex_properties.erase(this->_vertex_properties.begin() + vertex_id);

        // if constexpr (type_traits::c_non_empty_properties<edge_properties_type>) {
        //     // IDs are sorted and do not contain duplicates
        //     for (const auto& edge_id : std::views::reverse(removed_edge_ids))
        //         this->_edge_properties.erase(this->_edge_properties.begin() + edge_id);
        // }
    }

    types::size_type _n_vertices = 0uz;
    types::size_type _n_hyperedges = 0uz;

    [[no_unique_address]] vertex_properties_map_type _vertex_properties{};
    [[no_unique_address]] hyperedge_properties_map_type _hyperedge_properties{};
};

} // namespace hgl
