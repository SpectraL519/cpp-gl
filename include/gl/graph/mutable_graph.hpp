#pragma once

#include "gl/graph/graph_traits.hpp"
#include "gl/vertex.hpp"



namespace gl {

template <
    bool DIRECTED = true,
    vertex_descriptor_t vertex_t = gl::vertex_descriptor<>,
    graph_container_t container_t = gl::vector
>
class mutable_graph : public igraph<DIRECTED, vertex_t, container_t> {
public:
    using vertex_type = vertex_t;
    using vertex_ptr = std::unique_ptr<vertex_type>;
    using vertex_key_type = vertex_type::key_type;
    using edge_type = vertex_type::edge_type;
    using container_type = gl::container_traits_t<container_t, vertex_ptr>;

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


    [[nodiscard]] inline const vertex_ptr& at(std::size_t idx) override {
        return this->_adjacency_list.at(idx);
    }

    [[nodiscard]] inline const vertex_ptr& get_vertex(vertex_key_type key) override {
        return *std::find_if(
            this->_adjacency_list.begin(), this->_adjacency_list.end(),
            [key](const vertex_ptr& vertex) {
                return vertex->key == key;
            }
        );
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

    void remove_vertex(vertex_key_type key) {
        auto vertex_it = std::find_if(
            std::begin(this->_adjacency_list),
            std::end(this->_adjacency_list),
            [key](const vertex_ptr& vertex) {
                return vertex->key == key;
            }
        );

        this->_container_remove(this->_adjacency_list, vertex_it);
        this->_remove_stale_edges(key);
    }

    void add_edge(vertex_key_type source_key, vertex_key_type destination_key) override {
        if (!(this->_index_in_range(source_key) && this->_index_in_range(destination_key)))
            return;

        const auto& source = this->get_vertex(source_key);
        const auto& destination = this->get_vertex(destination_key);

        if constexpr (DIRECTED) {
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

        if constexpr (DIRECTED) {
            source->_add_edge(std::move(edge));
            destination->_in_deg++;
        }
        else {
            destination->_add_edge(edge.reverse());
            source->_add_edge(std::move(edge));
        }
    }

    void remove_edge(const vertex_type::edge_ptr& edge) {
        auto& vertex = get_vertex(edge->source);
        vertex->_remove_edge(edge->destination);
    }

    void remove_edge(vertex_key_type source, vertex_key_type destination) {
        auto& vertex = get_vertex(source);
        vertex->_remove_edge(destination);
    }

private:
    vertex_key_type _max_key = std::numeric_limits<vertex_key_type>::max();
    container_type _adjacency_list;


    using _container_traits = gl::container_traits<container_t, vertex_ptr>;
    using _container_iterator = _container_traits::iterator;
    using _container_const_iterator = _container_traits::const_iterator;

    std::function<void(container_type&, vertex_ptr&&)> _container_insert = _container_traits::insert;
    std::function<void(container_type&, _container_iterator)> _container_remove = _container_traits::remove;
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

    void _remove_stale_edges(vertex_key_type key) {
        auto key_equal_predicate =
            [key](const vertex_type::edge_ptr& edge) {
                return edge->destination == key;
            };

        _container_traits::remove_if<decltype(key_equal_predicate)>(
            this->_adjacency_list, key_equal_predicate
        );
    }
};

} // namespace gl
