#pragma once

#include <numeric>
#include <stdexcept>
#include <optional>

#include "gl/graph/graph_traits.hpp"
#include "gl/vertex.hpp"



namespace gl {

template <
    directed_specifier directed_v = directed,
    vertex_descriptor_t vertex_t = gl::vertex_descriptor<>,
    graph_container_t container_t = gl::vector
>
class graph : public igraph<directed_v, vertex_t, container_t> {
public:
    using vertex_type = vertex_t;
    using vertex_ptr = std::unique_ptr<vertex_type>;
    using vertex_key_type = vertex_type::key_type;
    using edge_type = vertex_type::edge_type;
    using container_type = gl::container_traits_t<container_t, vertex_ptr>;


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


    [[nodiscard]] inline const vertex_ptr& at(std::size_t idx) override {
        return this->_adjacency_list.at(idx);
    }

    [[nodiscard]] inline const vertex_ptr& get_vertex(vertex_key_type key) override {
        // TODO: return std::make_optional<vertex_ptr>(this->at(key));
        return this->at(key);
    }

    [[nodiscard]] inline const container_type& vertices() override {
        return this->_adjacency_list;
    }


    inline void add_vertex() override {
        this->_container_insert(this->_adjacency_list, std::make_unique<vertex_type>(this->num_vertices()));
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
            this->_container_insert(this->_adjacency_list, std::make_unique<vertex_type>(key));
    }

    void add_edge(vertex_key_type source_key, vertex_key_type destination_key) override {
        if (!(this->_index_in_range(source_key) && this->_index_in_range(destination_key)))
            return;

        vertex_ptr& source = this->_container_at(this->_adjacency_list, source_key);
        vertex_ptr& destination = this->_container_at(this->_adjacency_list, destination_key);

        if constexpr (directed_v) {
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

        vertex_ptr& source = this->_container_at(this->_adjacency_list, edge.source);
        vertex_ptr& destination = this->_container_at(this->_adjacency_list, edge.destination);

        if constexpr (directed_v) {
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


    using _container_traits = gl::container_traits<container_t, vertex_ptr>;

    std::function<void(container_type&, vertex_ptr&&)> _container_insert = _container_traits::insert;
    std::function<vertex_ptr&(container_type&, std::size_t)> _container_at = _container_traits::at;


    inline bool _index_in_range(const std::size_t& idx) const {
        return idx < this->num_vertices();
    }

    inline std::optional<vertex_key_type> _add_with_overflow_check(
        const vertex_key_type& a,
        const vertex_key_type& b
    ) {
        return (a < this->_max_key - b) ? std::make_optional<vertex_key_type>(a + b) : std::nullopt;
    }
};

} // namespace gl
