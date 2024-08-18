#pragma once

#include "adjacency_list.hpp"
#include "adjacency_matrix.hpp"
#include "impl_tags_decl.hpp"

namespace gl {

namespace impl {

struct list_t {
    template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
    requires(std::same_as<typename GraphTraits::implementation_tag, list_t>)
    using type = adjacency_list<GraphTraits>;
};

struct matrix_t {
    template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
    requires(std::same_as<typename GraphTraits::implementation_tag, matrix_t>)
    using type = adjacency_matrix<GraphTraits>;
};

} // namespace impl

} // namespace gl
