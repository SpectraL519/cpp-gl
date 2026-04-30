// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/impl/impl_tags.hpp
/// @brief Defines implementation tags for different graph representations.

#pragma once

#include "gl/decl/impl_tags.hpp"
#include "gl/impl/adjacency_list.hpp"
#include "gl/impl/adjacency_matrix.hpp"

namespace gl::impl {

/// @ingroup GL-Core
/// @headerfile gl/impl/impl_tags.hpp
/// @brief Tag struct for the standard adjacency list graph implementation.
struct list_t {
    /// @brief Type alias for the adjacency list graph implementation based on the provided graph traits.
    /// @tparam GraphTraits The graph traits for which to define the adjacency list type.
    ///         Must be an instantiation of @ref gl::graph_traits "graph_traits" and have `list_t` as its implementation tag.
    template <traits::c_instantiation_of<graph_traits> GraphTraits>
    requires(std::same_as<typename GraphTraits::implementation_tag, list_t>)
    using type = adjacency_list<GraphTraits>;
};

/// @ingroup GL-Core
/// @headerfile gl/impl/impl_tags.hpp
/// @brief Tag struct for the flattened adjacency list graph implementation.
/// ### See Also
/// - @ref gl::flat_jagged_vector "flat_jagged_vector" : For the data structure used for the underlying model implementation.
struct flat_list_t {
    /// @brief Type alias for the flattened adjacency list graph implementation based on the provided graph traits.
    /// @tparam GraphTraits The graph traits for which to define the flattened adjacency list type.
    ///         Must be an instantiation of @ref gl::graph_traits "graph_traits" and have `flat_list_t` as its implementation tag.
    template <traits::c_instantiation_of<graph_traits> GraphTraits>
    requires(std::same_as<typename GraphTraits::implementation_tag, flat_list_t>)
    using type = adjacency_list<GraphTraits>;
};

/// @ingroup GL-Core
/// @headerfile gl/impl/impl_tags.hpp
/// @brief Tag struct for the standard adjacency matrix graph implementation.
struct matrix_t {
    /// @brief Type alias for the adjacency matrix graph implementation based on the provided graph traits.
    /// @tparam GraphTraits The graph traits for which to define the adjacency matrix type.
    ///         Must be an instantiation of @ref gl::graph_traits "graph_traits" and have `matrix_t` as its implementation tag.
    template <traits::c_instantiation_of<graph_traits> GraphTraits>
    requires(std::same_as<typename GraphTraits::implementation_tag, matrix_t>)
    using type = adjacency_matrix<GraphTraits>;
};

/// @ingroup GL-Core
/// @headerfile gl/impl/impl_tags.hpp
/// @brief Tag struct for the flattened adjacency matrix graph implementation.
/// ### See Also
/// - @ref gl::flat_matrix "flat_matrix" : For the data structure used for the underlying model implementation.
struct flat_matrix_t {
    /// @brief Type alias for the flattened adjacency matrix graph implementation based on the provided graph traits.
    /// @tparam GraphTraits The graph traits for which to define the flattened adjacency matrix type.
    ///         Must be an instantiation of @ref gl::graph_traits "graph_traits" and have `flat_matrix_t` as its implementation tag.
    template <traits::c_instantiation_of<graph_traits> GraphTraits>
    requires(std::same_as<typename GraphTraits::implementation_tag, flat_matrix_t>)
    using type = adjacency_matrix<GraphTraits>;
};

} // namespace gl::impl
