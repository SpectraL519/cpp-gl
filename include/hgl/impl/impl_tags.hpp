// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/impl/impl_tags.hpp
/// @brief Defines implementation tags for different hypergraph representations.

#pragma once

#include "hgl/decl/impl_tags.hpp"
#include "hgl/impl/flat_incidence_list.hpp"
#include "hgl/impl/flat_incidence_matrix.hpp"
#include "hgl/impl/incidence_list.hpp"
#include "hgl/impl/incidence_matrix.hpp"
#include "hgl/impl/layout_tags.hpp"

namespace hgl::impl {

/// @ingroup HGL-Core
/// @headerfile hgl/impl/impl_tags.hpp
/// @brief Tag struct for the standard incidence list hypergraph implementation.
///
/// ### Layout Implications
/// The chosen layout significantly impacts memory usage and query performance:
///
/// - @ref hgl::impl::bidirectional_t "bidirectional_t" (Default): Maintains two internal lists (vertex-to-hyperedges and hyperedge-to-vertices). Provides optimal $O(1)$ degree/size lookups and fast traversals in both directions at the cost of doubled memory consumption.
/// - @ref hgl::impl::vertex_major_t "vertex_major_t": Maintains only a vertex-to-hyperedges list. Highly memory efficient and fast for querying vertex degrees or incident hyperedge sets, but querying hyperedge sizes or incident vertex setss requires expensive full-hypergraph scans.
/// - @ref hgl::impl::hyperedge_major_t "hyperedge_major_t": Maintains only a hyperedge-to-vertices list. Memory efficient and fast for hyperedge-centric queries, but querying vertex degrees or incident hyperedge sets requires full-hypergraph scans.
///
/// @tparam LayoutTag Specifies the memory layout orientation for the underlying data structure.
/// @tparam IdType The underlying integer type used for identifiers.
template <traits::c_hypergraph_layout_tag LayoutTag, traits::c_id_type IdType>
struct list_t {
    /// @brief Self type alias.
    using type = list_t<LayoutTag, IdType>;

    /// @brief The configured layout tag.
    using layout_tag = LayoutTag;
    /// @brief The configured identifier type.
    using id_type = IdType;

    /// @brief Type alias for the underlying incidence list hypergraph implementation based on the provided directional tag.
    /// @tparam DirectionalTag The directional tag (e.g., undirected or bf_directed).
    template <traits::c_hypergraph_directional_tag DirectionalTag>
    using implementation_type = incidence_list<DirectionalTag, type>;
};

/// @ingroup HGL-Core
/// @headerfile hgl/impl/impl_tags.hpp
/// @brief Tag struct for the flattened incidence list hypergraph implementation.
///
/// ### Layout Implications
/// The chosen layout significantly impacts memory usage and query performance:
///
/// - @ref hgl::impl::bidirectional_t "bidirectional_t" (Default): Maintains two internal flattened lists (vertex-to-hyperedges and hyperedge-to-vertices). Provides optimal $O(1)$ degree/size lookups and fast traversals in both directions at the cost of doubled memory consumption.
/// - @ref hgl::impl::vertex_major_t "vertex_major_t": Maintains only a vertex-to-hyperedges flattened list. Highly memory efficient and fast for querying vertex degrees or incident hyperedge sets, but querying hyperedge sizes or incident vertex sets requires expensive full-graph scans.
/// - @ref hgl::impl::hyperedge_major_t "hyperedge_major_t": Maintains only a hyperedge-to-vertices flattened list. Memory efficient and fast for hyperedge-centric queries, but querying vertex degrees or incident hyperedge sets requires full-graph scans.
///
/// @tparam LayoutTag Specifies the memory layout orientation for the underlying data structure.
/// @tparam IdType The underlying integer type used for identifiers.
/// ### See Also
/// - @ref gl::flat_jagged_vector "flat_jagged_vector" for the data structure used for the underlying model implementation.
template <traits::c_hypergraph_layout_tag LayoutTag, traits::c_id_type IdType>
struct flat_list_t {
    /// @brief Self type alias.
    using type = flat_list_t<LayoutTag, IdType>;

    /// @brief The configured layout tag.
    using layout_tag = LayoutTag;
    /// @brief The configured identifier type.
    using id_type = IdType;

    /// @brief Type alias for the underlying flattened incidence list hypergraph implementation based on the provided directional tag.
    /// @tparam DirectionalTag The directional tag (e.g., undirected or bf_directed).
    template <traits::c_hypergraph_directional_tag DirectionalTag>
    using implementation_type = flat_incidence_list<DirectionalTag, type>;
};

/// @ingroup HGL-Core
/// @headerfile hgl/impl/impl_tags.hpp
/// @brief Tag struct for the standard incidence matrix hypergraph implementation.
///
/// ### Layout Implications
/// Matrix implementations strictly require an asymmetric layout tag to define the row and column dimensions of the underlying matrix:
///
/// - @ref hgl::impl::hyperedge_major_t "hyperedge_major_t" (Default): Stores a \f$\vert E \vert \times \vert V \vert\f$ matrix, where hyperedges are mapped to rows and vertices to columns. Retrieving the vertices incident to a specific hyperedge translates to a fast, cache-friendly contiguous memory read across a single row.
/// - @ref hgl::impl::vertex_major_t "vertex_major_t": Stores a \f$\vert V \vert \times \vert E \vert\f$ matrix, where vertices are mapped to rows and hyperedges to columns. Retrieving the hyperedges incident to a specific vertex translates to a fast, contiguous memory read.
///
/// @tparam LayoutTag Specifies the memory layout orientation for the underlying data structure (must be asymmetric).
/// @tparam IdType The underlying integer type used for identifiers.
template <traits::c_hypergraph_asymmetric_layout_tag LayoutTag, traits::c_id_type IdType>
struct matrix_t {
    /// @brief Self type alias.
    using type = matrix_t<LayoutTag, IdType>;

    /// @brief The configured asymmetric layout tag.
    using layout_tag = LayoutTag;
    /// @brief The configured identifier type.
    using id_type = IdType;

    /// @brief Type alias for the underlying incidence matrix hypergraph implementation based on the provided directional tag.
    /// @tparam DirectionalTag The directional tag (e.g., undirected or bf_directed).
    template <traits::c_hypergraph_directional_tag DirectionalTag>
    using implementation_type = incidence_matrix<DirectionalTag, type>;
};

/// @ingroup HGL-Core
/// @headerfile hgl/impl/impl_tags.hpp
/// @brief Tag struct for the flattened incidence matrix hypergraph implementation.
///
/// ### Layout Implications
/// Matrix implementations strictly require an asymmetric layout tag to define the row and column dimensions of the underlying matrix:
///
/// - @ref hgl::impl::hyperedge_major_t "hyperedge_major_t" (Default): Stores a \f$\vert E \vert \times \vert V \vert\f$ flat matrix, where hyperedges are mapped to rows and vertices to columns. Retrieving the vertices incident to a specific hyperedge translates to a fast, cache-friendly contiguous memory read across a single row.
/// - @ref hgl::impl::vertex_major_t "vertex_major_t": Stores a \f$\vert V \vert \times \vert E \vert\f$ flat matrix, where vertices are mapped to rows and hyperedges to columns. Retrieving the hyperedges incident to a specific vertex translates to a fast, contiguous memory read.
///
/// @tparam LayoutTag Specifies the memory layout orientation for the underlying data structure (must be asymmetric).
/// @tparam IdType The underlying integer type used for identifiers.
/// ### See Also
/// - @ref gl::flat_matrix "flat_matrix" for the data structure used for the underlying model implementation.
template <traits::c_hypergraph_asymmetric_layout_tag LayoutTag, traits::c_id_type IdType>
struct flat_matrix_t {
    /// @brief Self type alias.
    using type = flat_matrix_t<LayoutTag, IdType>;

    /// @brief The configured asymmetric layout tag.
    using layout_tag = LayoutTag;
    /// @brief The configured identifier type.
    using id_type = IdType;

    /// @brief Type alias for the underlying flattened incidence matrix hypergraph implementation based on the provided directional tag.
    /// @tparam DirectionalTag The directional tag (e.g., undirected or bf_directed).
    template <traits::c_hypergraph_directional_tag DirectionalTag>
    using implementation_type = flat_incidence_matrix<DirectionalTag, type>;
};

} // namespace hgl::impl
