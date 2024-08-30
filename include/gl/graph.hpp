#pragma once

#include "gl/attributes/force_inline.hpp"
#include "graph_traits.hpp"
#include "impl/impl_tags.hpp"
#include "types/formatter.hpp"
#include "types/iterator_range.hpp"
#include "types/type_traits.hpp"
#include "types/types.hpp"

#include <algorithm>

#ifdef GL_TESTING

namespace gl_testing {
template <typename TraitsType>
struct test_graph;
} // namespace gl_testing

#endif

namespace gl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits = graph_traits<>>
class graph {
public:
    using traits_type = GraphTraits;

    using implementation_tag = typename traits_type::implementation_tag;
    using implementation_type = typename implementation_tag::template type<traits_type>;

    using vertex_type = typename traits_type::vertex_type;
    using vertex_properties_type = typename traits_type::vertex_properties_type;

    using vertex_set_type = std::vector<vertex_type>;
    using vertex_iterator_type = typename vertex_set_type::const_iterator;

    // TODO: reverese iterators should be available for bidirectional ranges

    using edge_type = typename traits_type::edge_type;
    using edge_directional_tag = typename traits_type::edge_directional_tag;
    using edge_properties_type = typename traits_type::edge_properties_type;

    using edge_set_type = typename implementation_type::edge_set_type;
    using edge_iterator_type = typename implementation_type::edge_iterator_type;

#ifdef GL_TESTING
    friend struct ::gl_testing::test_graph<traits_type>;
#endif

    graph(const graph&) = delete;
    graph& operator=(const graph&) = delete;

    graph() = default;

    graph(const types::size_type n_vertices) : _impl(n_vertices) {
        this->_vertices.reserve(n_vertices);
        for (auto vertex_id = 0ull; vertex_id < n_vertices; vertex_id++)
            this->_vertices.emplace_back(vertex_id);
    }

    graph(graph&&) = default;
    graph& operator=(graph&&) = default;

    ~graph() = default;

    // --- general methods ---

    [[nodiscard]] gl_attr_force_inline types::size_type n_vertices() const {
        return this->_vertices.size();
    }

    [[nodiscard]] gl_attr_force_inline types::size_type n_unique_edges() const {
        return this->_impl.n_unique_edges();
    }

    // --- vertex methods ---

    inline const vertex_type& add_vertex() {
        this->_impl.add_vertex();
        return this->_vertices.emplace_back(this->n_vertices());
    }

    inline const vertex_type& add_vertex(const vertex_properties_type& properties)
    requires(not type_traits::is_default_properties_type_v<vertex_properties_type>)
    {
        this->_impl.add_vertex();
        return this->_vertices.emplace_back(this->n_vertices(), properties);
    }

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const types::id_type vertex_id) const {
        return vertex_id < this->n_vertices();
    }

    [[nodiscard]] gl_attr_force_inline bool has_vertex(const vertex_type& vertex) const {
        return this->has_vertex(vertex.id()) and &vertex == &this->_vertices[vertex.id()];
    }

    // clang-format off
    // gl_attr_force_inline misplacement

    [[nodiscard]] gl_attr_force_inline const vertex_type& get_vertex(
        const types::id_type vertex_id
    ) const {
        return this->_vertices.at(vertex_id);
    }

    // clang-format on

    gl_attr_force_inline void remove_vertex(const types::size_type vertex_id) {
        this->_remove_vertex_impl(this->get_vertex(vertex_id));
    }

    inline void remove_vertex(const vertex_type& vertex) {
        this->_verify_vertex(vertex);
        this->_remove_vertex_impl(vertex);
    }

    [[nodiscard]] gl_attr_force_inline types::iterator_range<vertex_iterator_type> vertices(
    ) const {
        return make_const_iterator_range(this->_vertices);
    }

    // --- edge methods ---

    // clang-format off
    // gl_attr_force_inline misplacement

    gl_attr_force_inline const edge_type& add_edge(
        const types::id_type first_id, const types::id_type second_id
    ) {
        return this->_impl.add_edge(
            make_edge<edge_type>(this->get_vertex(first_id), this->get_vertex(second_id))
        );
    }

    gl_attr_force_inline const edge_type& add_edge(
        const types::id_type first_id,
        const types::id_type second_id,
        const edge_properties_type& properties
    )
    requires(not type_traits::is_default_properties_type_v<edge_properties_type>)
    {
        return this->_impl.add_edge(make_edge<edge_type>(
            this->get_vertex(first_id), this->get_vertex(second_id), properties
        ));
    }

    // clang-format on

    const edge_type& add_edge(const vertex_type& first, const vertex_type& second) {
        this->_verify_vertex(first);
        this->_verify_vertex(second);
        return this->_impl.add_edge(make_edge<edge_type>(first, second));
    }

    const edge_type& add_edge(
        const vertex_type& first, const vertex_type& second, const edge_properties_type& properties
    )
    requires(not type_traits::is_default_properties_type_v<edge_properties_type>)
    {
        this->_verify_vertex(first);
        this->_verify_vertex(second);
        return this->_impl.add_edge(make_edge<edge_type>(first, second, properties));
    }

    [[nodiscard]] bool has_edge(const types::id_type first_id, const types::id_type second_id)
        const {
        return this->_impl.has_edge(first_id, second_id);
    }

    [[nodiscard]] bool has_edge(const vertex_type& first, const vertex_type& second) const {
        this->_verify_vertex(first);
        this->_verify_vertex(second);
        return this->has_edge(first.id(), second.id());
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(const edge_type& edge) const {
        return this->_impl.has_edge(edge);
    }

    [[nodiscard]] gl_attr_force_inline types::optional_ref<const edge_type> get_edge(
        const types::id_type first_id, const types::id_type second_id
    ) const {
        return this->_impl.get_edge(first_id, second_id);
    }

    [[nodiscard]] types::optional_ref<const edge_type> get_edge(
        const vertex_type& first, const vertex_type& second
    ) const {
        if (not (this->has_vertex(first) and this->has_vertex(second)))
            return std::nullopt;

        // TODO: optimize this so that the vertex ids are not checked twice
        return this->_impl.get_edge(first.id(), second.id());
    }

    [[nodiscard]] inline std::vector<std::reference_wrapper<const edge_type>> get_edges(
        const types::id_type first_id, const types::id_type second_id
    ) const {
        using edge_ref_set = std::vector<std::reference_wrapper<const edge_type>>;

        if constexpr (std::same_as<implementation_tag, impl::list_t>) {
            return this->_impl.get_edges(first_id, second_id);
        }
        else {
            const auto edge_opt = this->_impl.get_edge(first_id, second_id);
            return edge_opt.has_value() ? edge_ref_set{edge_opt.value()} : edge_ref_set{};
        }
    }

    [[nodiscard]] std::vector<std::reference_wrapper<const edge_type>> get_edges(
        const vertex_type& first, const vertex_type& second
    ) const {
        this->_verify_vertex(first);
        this->_verify_vertex(second);
        return this->get_edges(first.id(), second.id());
    }

    gl_attr_force_inline void remove_edge(const edge_type& edge) {
        this->_impl.remove_edge(edge);
    }

    [[nodiscard]] inline types::iterator_range<edge_iterator_type> adjacent_edges(
        const types::id_type vertex_id
    ) const {
        this->_verify_vertex_id(vertex_id);
        return this->_impl.adjacent_edges(vertex_id);
    }

    [[nodiscard]] inline types::iterator_range<edge_iterator_type> adjacent_edges(
        const vertex_type& vertex
    ) const {
        this->_verify_vertex(vertex);
        return this->_impl.adjacent_edges(vertex.id());
    }

    // --- incidence methods ---

    [[nodiscard]] bool are_incident(const types::id_type first_id, const types::id_type second_id)
        const {
        this->_verify_vertex_id(first_id);

        if (first_id == second_id)
            return true;

        this->_verify_vertex_id(second_id);

        return this->has_edge(first_id, second_id);
    }

    [[nodiscard]] bool are_incident(const vertex_type& first, const vertex_type& second) const {
        this->_verify_vertex(first);
        this->_verify_vertex(second);
        return first == second or this->has_edge(first.id(), second.id());
    }

    [[nodiscard]] bool are_incident(const vertex_type& vertex, const edge_type& edge) const {
        this->_verify_vertex(vertex);
        this->_verify_edge(edge);
        return edge.is_incident_with(vertex);
    }

    [[nodiscard]] bool are_incident(const edge_type& edge, const vertex_type& vertex) const {
        this->_verify_vertex(vertex);
        this->_verify_edge(edge);
        return edge.is_incident_with(vertex);
    }

    [[nodiscard]] bool are_incident(const edge_type& edge_1, const edge_type& edge_2) const {
        this->_verify_edge(edge_1);
        this->_verify_edge(edge_2);
        return edge_1.is_incident_with(edge_2.first()) or edge_1.is_incident_with(edge_2.second());
    }

private:
    gl_attr_force_inline void _verify_vertex_id(const types::id_type vertex_id) const {
        if (not this->has_vertex(vertex_id))
            throw std::invalid_argument(std::format("Got invalid vertex id [{}]", vertex_id));
    }

    void _verify_vertex(const vertex_type& vertex) const {
        const auto vertex_id = vertex.id();
        const auto& self_vertex = this->get_vertex(vertex_id);

        if (&vertex != &self_vertex)
            throw std::invalid_argument(std::format(
                "Got invalid vertex [id = {} | expected addr = {} | actual addr = {}]",
                vertex_id,
                types::formatter(&self_vertex),
                types::formatter(&vertex)
            ));
    }

    void _verify_edge(const edge_type& edge) const {
        if (not this->has_edge(edge))
            throw std::invalid_argument(std::format(
                "Got invalid edge [vertices = ({}, {}) | addr = {}]",
                edge.first().id(),
                edge.second().id(),
                types::formatter(&edge)
            ));
    }

    void _remove_vertex_impl(const vertex_type& vertex) {
        const auto vertex_id = vertex.id();
        this->_impl.remove_vertex(vertex);
        this->_vertices.erase(std::next(std::begin(this->_vertices), vertex_id));

        // align ids of remainig vertices
        std::for_each(
            std::next(std::begin(this->_vertices), vertex_id),
            this->_vertices.end(),
            [](auto& v) { v._id--; }
        );
    }

    vertex_set_type _vertices{};
    implementation_type _impl{};
};

} // namespace gl
