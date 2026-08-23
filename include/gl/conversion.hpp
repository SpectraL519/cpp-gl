// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/conversion.hpp
/// @brief Defines the graph representation model conversion utility.

#pragma once

#include "gl/decl/repr_tags.hpp"
#include "gl/graph.hpp"
#include "gl/repr_tags.hpp"
#include "gl/traits.hpp"
#include "gl/types/core.hpp"

#include <utility>

namespace gl {

namespace traits {

/// @ingroup GL-Traits
/// @brief Utility trait type used to swap the representation tag of a graph traits or graph type.
/// ### See Also:
/// - @ref gl::to "to" : For the function that utilizes this trait to perform graph conversions between different representations.
template <typename GT, traits::c_graph_repr_tag NewReprTag>
requires c_graph<GT> or c_instantiation_of<GT, graph_traits>
struct swap_repr_tag;

/// @ingroup GL-Traits
/// @brief Specialization of @ref gl::traits::swap_repr_tag "swap_repr_tag" for the @ref gl::graph_traits "graph_traits" type.
template <
    traits::c_graph_directional_tag Dir,
    traits::c_properties VP,
    traits::c_properties EP,
    traits::c_graph_repr_tag OldReprTag,
    traits::c_graph_repr_tag NewReprTag,
    traits::c_api_policy_tag ApiPolicyTag,
    traits::c_id_type IdType>
struct swap_repr_tag<graph_traits<Dir, VP, EP, OldReprTag, ApiPolicyTag, IdType>, NewReprTag> {
    using type = graph_traits<Dir, VP, EP, NewReprTag, ApiPolicyTag, IdType>;
};

/// @ingroup GL-Traits
/// @brief Specialization of @ref gl::traits::swap_repr_tag "swap_repr_tag" for the @ref gl::graph "graph" class.
template <
    traits::c_graph_directional_tag Dir,
    traits::c_properties VP,
    traits::c_properties EP,
    traits::c_graph_repr_tag OldReprTag,
    traits::c_graph_repr_tag NewReprTag,
    traits::c_api_policy_tag ApiPolicyTag,
    traits::c_id_type IdType>
struct swap_repr_tag<graph<graph_traits<Dir, VP, EP, OldReprTag, ApiPolicyTag, IdType>>, NewReprTag> {
    using type = graph<graph_traits<Dir, VP, EP, NewReprTag, ApiPolicyTag, IdType>>;
};

/// @ingroup GL-Traits
/// @brief Alias template for easier usage of the @ref gl::traits::swap_repr_tag "swap_repr_tag" trait to resolve the swapped type directly.
/// ### See Also:
/// - @ref gl::to "to" : For the function that utilizes this trait to perform graph conversions between different representations.
template <typename GT, traits::c_graph_repr_tag NewReprTag>
requires c_graph<GT> or c_instantiation_of<GT, graph_traits>
using swap_repr_tag_t = typename swap_repr_tag<GT, NewReprTag>::type;

} // namespace traits

namespace detail {

template <traits::c_graph_repr_tag TargetReprTag, traits::c_graph_repr_tag SourceReprTag>
struct to_impl {
    template <typename TargetGraph, typename SourceGraph>
    static void convert(TargetGraph& target, SourceGraph& source) {
        target._impl.add_vertices(source.n_vertices());

        for (const auto u : source.vertex_ids()) {
            for (const auto& edge : source.out_edges(u)) {
                if constexpr (traits::c_undirected_graph<SourceGraph>)
                    if (edge.source() > edge.target())
                        continue; // prevent double insertion

                target._impl.add_edge(edge.id(), edge.source(), edge.target());
            }
        }
    }
};

// Conversion: identity
template <traits::c_graph_repr_tag ReprTag>
struct to_impl<ReprTag, ReprTag> {
    template <typename TargetGraph, typename SourceGraph>
    static void convert(TargetGraph& target, SourceGraph& source) {
        target._impl = std::move(source._impl);
    }
};

// Conversion: list -> flat list
template <>
struct to_impl<repr::flat_list_t, repr::list_t> {
    template <typename TargetGraph, typename SourceGraph>
    static void convert(TargetGraph& target, SourceGraph& source) {
        auto& target_list = target._impl._list;
        auto& source_list = source._impl._list;

        std::size_t total_items = 0uz;
        for (const auto& inc : source_list)
            total_items += inc.size();

        target_list.reserve_segments(source_list.size());
        target_list.reserve_data(total_items);

        for (auto& inc : source_list)
            target_list.push_back(std::move(inc));
    }
};

// Conversion: flat list -> list
template <>
struct to_impl<repr::list_t, repr::flat_list_t> {
    template <typename TargetGraph, typename SourceGraph>
    static void convert(TargetGraph& target, SourceGraph& source) {
        auto& target_list = target._impl._list;
        auto& source_list = source._impl._list;

        target_list.reserve(source_list.size());
        for (auto inc : source_list)
            target_list.emplace_back(inc.begin(), inc.end());
    }
};

// Conversion: matrix -> flat matrix
template <>
struct to_impl<repr::flat_matrix_t, repr::matrix_t> {
    template <typename TargetGraph, typename SourceGraph>
    static void convert(TargetGraph& target, SourceGraph& source) {
        auto& target_matrix = target._impl._matrix;
        auto& source_matrix = source._impl._matrix;

        const auto n_vertices = source_matrix.size();
        target_matrix.resize(n_vertices, n_vertices);

        auto* target_ptr = target_matrix.data_ptr();
        for (auto& row : source_matrix)
            target_ptr = std::ranges::move(row, target_ptr).out;
    }
};

// Conversion: flat matrix -> matrix
template <>
struct to_impl<repr::matrix_t, repr::flat_matrix_t> {
    template <typename TargetGraph, typename SourceGraph>
    static void convert(TargetGraph& target, SourceGraph& source) {
        auto& target_matrix = target._impl._matrix;
        auto& source_matrix = source._impl._matrix;

        target_matrix.reserve(source_matrix.n_rows());
        for (auto row : source_matrix.rows())
            target_matrix.emplace_back(row.begin(), row.end());
    }
};

} // namespace detail

/// @ingroup GL-Core
/// @headerfile gl/conversion.hpp
/// @brief Converts a graph from one representation model to another.
///
/// This function efficiently transforms a graph's underlying memory representation (e.g., from a standard adjacency list to a flattened adjacency list) while preserving its exact topology, properties, and identifiers.
///
/// ### Template Parameters
/// | Parameter     | Description | Constraints |
/// | :------------ | :---------- | :---------- |
/// | TargetReprTag | The representation tag of the desired target representation (e.g., `gl::repr::flat_list_t`) | [**c_graph_repr_tag**](gl_concepts.md#gl-traits-c-graph-repr-tag) |
/// | Graph         | The type of the source graph, which will be automatically deduced from the function argument. | [**c_graph**](gl_concepts.md#gl-traits-c-graph) and must **NOT** be an Lvalue reference |
///
/// @param source The graph to convert. After the operation it will be left in a valid, empty state.
/// @return A new graph containing the moved data, structured according to `TargetReprTag`.
///
/// ### See Also
/// - @ref gl::traits::swap_repr_tag "swap_repr_tag" : For the trait used to resolve the target graph type with the swapped representation tag.
template <traits::c_graph_repr_tag TargetReprTag, traits::c_graph Graph>
requires(not std::is_lvalue_reference_v<Graph>)
[[nodiscard]] auto to(Graph&& source) {
    using source_traits = typename Graph::traits_type;
    using source_impl_tag = typename source_traits::representation_tag;

    using target_traits = traits::swap_repr_tag_t<source_traits, TargetReprTag>;
    using target_graph = graph<target_traits>;

    target_graph target;

    detail::to_impl<TargetReprTag, source_impl_tag>::convert(target, source);

    target._n_vertices = std::exchange(source._n_vertices, 0uz);
    target._n_edges = std::exchange(source._n_edges, 0uz);
    target._vertex_properties = std::move(source._vertex_properties);
    target._edge_properties = std::move(source._edge_properties);
    source._impl = typename Graph::implementation_type{};

    return target;
}

} // namespace gl
