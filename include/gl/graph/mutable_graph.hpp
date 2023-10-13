#pragma once

#include "gl/graph/detail/abstract_graph_base.hpp"
#include "gl/graph/graph.hpp"
#include "gl/graph/graph_traits.hpp"
#include "gl/vertex.hpp"



namespace gl {

template <
    directed_specifier_type Directed = directed,
    vertex_descriptor_c VertexDescriptor = gl::vertex_descriptor<>,
    graph_container_c Container = gl::vector
>
class mutable_graph : public detail::abstract_graph_base<Directed, VertexDescriptor, Container> {
public:
    using vertex_type = VertexDescriptor;
    using vertex_ptr_type = std::unique_ptr<vertex_type>;
    using vertex_key_type = vertex_type::key_type;

    using edge_type = vertex_type::edge_type;

    using container_type = gl::container_traits_t<Container, vertex_ptr_type>;
    using container_specifier =
        typename gl::container_traits<Container, vertex_ptr_type>::container_specifier;

    mutable_graph(const mutable_graph&) = delete;
    mutable_graph(mutable_graph&&) = delete;

    mutable_graph() = default;
    ~mutable_graph() = default;
    // ? TODO: initializer list constructor


    [[nodiscard]] inline vertex_key_type num_vertices() const override {
        return (vertex_key_type)this->_adjacency_list.size();
    }

    [[nodiscard]] std::size_t num_edges() const override {
        // ? keep edge count as a private member variable
        std::size_t edges = 0;
        for (auto& vertex : this->_adjacency_list)
            edges += vertex->degree();
        return edges;
    }

    [[nodiscard]] inline std::size_t size() const override {
        return this->num_vertices() + this->num_edges();
    }

    [[nodiscard]] inline bool empty() const override {
        return this->num_vertices() == 0;
    }

    [[nodiscard]] inline bool has_vertex(vertex_key_type key) const override {
        return this->_index_in_range(key);
    }


    [[nodiscard]] inline const vertex_ptr_type& at(std::size_t idx) override {
        return this->_adjacency_list.at(idx);
    }

    [[nodiscard]] inline const vertex_ptr_type& get_vertex(vertex_key_type key) override {
        if (this->_consecutive_vertices)
            // TODO: return std::make_optional<vertex_ptr_type>(this->at(key));
            return this->at(key);

        // TODO: optional get_vertex -> return nullopt if vertex is not found
        return *std::find_if(
            this->_adjacency_list.begin(), this->_adjacency_list.end(),
            [key](const vertex_ptr_type& vertex) {
                return vertex->key == key;
            }
        );
    }

    [[nodiscard]] inline const container_type& vertices() override {
        return this->_adjacency_list;
    }


    inline void add_vertex() override {
        // TODO: account for removed vertices
        // ? keep a queue of indices which have been removed from the graph
        this->_insert(this->_adjacency_list, std::make_unique<vertex_type>(this->num_vertices()));
    }

    void add_vertices(vertex_key_type num_new_vertices) override {
        // TODO: account for removed vertices
        // ? keep a queue of indices which have been removed from the graph
        auto new_size_opt = this->_add_with_overflow_check(this->num_vertices(), num_new_vertices);
        if (!new_size_opt)
            throw std::out_of_range(
                std::string("type overflow (") + typeid(vertex_key_type()).name() + "): cannot add "
                + std::to_string(num_new_vertices) + " vertices"
                + "\n\tcurrent vertex count: " + std::to_string(this->num_vertices())
                + "\n\tmaximum type value: " + std::to_string(std::numeric_limits<vertex_key_type>::max())
            );

        auto new_size = new_size_opt.value();
        for (vertex_key_type key = this->num_vertices(); key < new_size; key++)
            this->_insert(this->_adjacency_list, std::make_unique<vertex_type>(key));
    }

    void remove_vertex(vertex_key_type key) {
        auto vertex_it = std::find_if(
            std::begin(this->_adjacency_list),
            std::end(this->_adjacency_list),
            [key](const vertex_ptr_type& vertex) {
                return vertex->key == key;
            }
        );

        this->_remove(this->_adjacency_list, vertex_it);
        this->_remove_stale_edges(key);
        this->_consecutive_vertices = false;
    }

    void add_edge(vertex_key_type source_key, vertex_key_type destination_key) override {
        if (!(this->_index_in_range(source_key) && this->_index_in_range(destination_key)))
            return;

        const auto& source = this->get_vertex(source_key);
        const auto& destination = this->get_vertex(destination_key);

        if constexpr (this->is_directed()) {
            source->_add_edge(destination_key);
            destination->_in_deg++;
        }
        else {
            source->_add_edge(destination_key);
            destination->_add_edge(source_key);
        }
    }

    void add_edge(edge_type&& edge) override {
        if (!(this->_index_in_range(edge.source) && this->_index_in_range(edge.destination)))
            return;

        const auto& source = this->get_vertex(edge.source);
        const auto& destination = this->get_vertex(edge.destination);

        if constexpr (this->is_directed()) {
            source->_add_edge(std::move(edge));
            destination->_in_deg++;
        }
        else {
            destination->_add_edge(edge.reverse());
            source->_add_edge(std::move(edge));
        }
    }

    void remove_edge(const vertex_type::edge_ptr_type& edge) {
        auto& vertex = get_vertex(edge->source);
        vertex->_remove_edge(edge->destination);
    }

    void remove_edge(vertex_key_type source, vertex_key_type destination) {
        auto& vertex = get_vertex(source);
        vertex->_remove_edge(destination);
    }

    // TODO: shrink[_to_size]

private:
    vertex_key_type _max_key = std::numeric_limits<vertex_key_type>::max();
    container_type _adjacency_list;
    bool _consecutive_vertices = true;


    using _container_traits = gl::container_traits<Container, vertex_ptr_type>;
    using _container_iterator = _container_traits::iterator;
    using _container_const_iterator = _container_traits::const_iterator;

    std::function<void(container_type&, vertex_ptr_type&&)> _insert = _container_traits::insert;
    std::function<void(container_type&, _container_iterator)> _remove = _container_traits::remove;
    std::function<vertex_ptr_type&(container_type&, std::size_t)> _at = _container_traits::at;


    inline bool _index_in_range(const std::size_t& idx) const {
        return idx < this->num_vertices();
    }

    inline std::optional<vertex_key_type> _add_with_overflow_check(
        const vertex_key_type& a, const vertex_key_type& b
    ) {
        return (a < this->_max_key - b) ? std::make_optional<vertex_key_type>(a + b)
                                        : std::nullopt;
    }

    void _remove_stale_edges(vertex_key_type key) {
        auto key_destination_predicate =
            [key](const vertex_type::edge_ptr_type& edge) {
                return edge->destination == key;
            };

        for (auto& vertex : this->_adjacency_list) {
            vertex_type::_container_traits::remove_if(
                vertex->_adjacent, key_destination_predicate
            );
        }
    }


    mutable_graph(graph<Directed, VertexDescriptor, Container>&& graph)
    : _adjacency_list(std::move(graph._adjacency_list)) {}

    friend mutable_graph make_mutable<Directed, VertexDescriptor, Container>(
        graph<Directed, VertexDescriptor, Container>&& graph
    );
};



template <
    directed_specifier_type Directed,
    vertex_descriptor_c VertexDescriptor,
    graph_container_c Container
>
mutable_graph<Directed, VertexDescriptor, Container> make_mutable(
    graph<Directed, VertexDescriptor, Container>&& graph
) {
    return mutable_graph<Directed, VertexDescriptor, Container>(std::move(graph));
}

} // namespace gl
