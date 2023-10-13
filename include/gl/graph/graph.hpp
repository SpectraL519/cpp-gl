#pragma once

#include <numeric>
#include <optional>
#include <stdexcept>

#include "gl/graph/detail/abstract_graph_base.hpp"
#include "gl/graph/graph_traits.hpp"
#include "gl/vertex.hpp"



namespace gl {

template <
    directed_specifier_type Directed,
    vertex_descriptor_c VertexDescriptor,
    graph_container_c Container
>
class mutable_graph;



template <
    directed_specifier_type Directed = directed,
    vertex_descriptor_c VertexDescriptor = gl::vertex_descriptor<>,
    graph_container_c Container = gl::vector
>
class graph : public detail::abstract_graph_base<Directed, VertexDescriptor, Container> {
public:
    using vertex_type = VertexDescriptor;
    using vertex_ptr_type = std::unique_ptr<vertex_type>;
    using vertex_key_type = vertex_type::key_type;

    using edge_type = vertex_type::edge_type;

    using container_type = gl::container_traits_t<Container, vertex_ptr_type>;
    using container_specifier =
        typename gl::container_traits<Container, vertex_ptr_type>::container_specifier;


    graph(const graph&) = delete;
    graph(graph&&) = delete;

    graph() = default;
    ~graph() = default;
    // ? TODO: initializer list constructor


    [[nodiscard]] inline vertex_key_type num_vertices() const override {
        return (vertex_key_type)this->_adjacency_list.size();
    }

    [[nodiscard]] std::size_t num_edges() const override {
        // ? keep edge count as a private member variable
        std::size_t edges = 0;
        for (auto& vertex : this->_adjacency_list)
            edges += vertex->out_degree();
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
        // TODO: return std::make_optional<vertex_ptr_type>(this->at(key));
        return this->at(key);
    }

    [[nodiscard]] inline const container_type& vertices() override {
        return this->_adjacency_list;
    }


    inline void add_vertex() override {
        this->_insert(this->_adjacency_list, std::make_unique<vertex_type>(this->num_vertices()));
    }

    void add_vertices(vertex_key_type num_new_vertices) override {
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

    void add_edge(vertex_key_type source_key, vertex_key_type destination_key) override {
        if (!(this->_index_in_range(source_key) && this->_index_in_range(destination_key)))
            return;

        vertex_ptr_type& source = this->_at(this->_adjacency_list, source_key);
        vertex_ptr_type& destination = this->_at(this->_adjacency_list, destination_key);

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

        vertex_ptr_type& source = this->_at(this->_adjacency_list, edge.source);
        vertex_ptr_type& destination = this->_at(this->_adjacency_list, edge.destination);

        if constexpr (this->is_directed()) {
            source->_add_edge(std::move(edge));
            destination->_in_deg++;
        }
        else {
            destination->_add_edge(edge.reverse());
            source->_add_edge(std::move(edge));
        }
    }


private:
    vertex_key_type _max_key = std::numeric_limits<vertex_key_type>::max();
    container_type _adjacency_list;


    using _container_traits = gl::container_traits<Container, vertex_ptr_type>;

    std::function<void(container_type&, vertex_ptr_type&&)> _insert = _container_traits::insert;
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


    friend class mutable_graph<Directed, VertexDescriptor, Container>;
};

} // namespace gl
