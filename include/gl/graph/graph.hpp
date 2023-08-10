#ifndef CPP_GL_GRAPH
#define CPP_GL_GRAPH

#include <numeric>
#include <stdexcept>

#include <gl/graph/graph_traits.hpp>
#include <gl/vertex.hpp>



namespace gl {

template<
    bool DIRECTED = true,
    vertex_descriptor_t vertex_t = gl::vertex_descriptor<>,
    graph_container_s container_s = gl::vect_s
>
class graph : public igraph<DIRECTED, vertex_t, container_s> {
public:
    using vertex_key_type = vertex_t::key_type;
    using vertex_type = vertex_t;
    using edge_type = vertex_t::edge_type;
    using container_type = gl::container_traits_t<container_s, vertex_type>;

private:
    container_type _adjacency_list;

public:
    graph() = default;
    ~graph() = default;
    // TODO: copy constructor


    // class feature getters
    [[nodiscard]] inline std::size_t num_vertices() const override {
        return this->_adjacency_list.size();
    }

    [[nodiscard]] std::size_t num_edges() const override {
        // ??? keep edge count as a private member variable
        std::size_t edges = 0;
        for (const vertex_type& vertex : this->_adjacency_list)
            edges += vertex.degree();
        return edges;
    }

    [[nodiscard]] inline std::size_t size() const override {
        return this->num_vertices() + this->num_edges();
    }

    [[nodiscard]] inline bool empty() const override {
        return this->num_vertices() == 0;
    }

    [[nodiscard]] inline bool has_vertex(const vertex_key_type& key) const override {
        return this->_index_in_range(key);
    }


    // getters
    [[nodiscard]] inline vertex_type& operator[](const std::size_t& idx) const override {
        return const_cast<vertex_type&>(this->_adjacency_list[idx]);
    }

    [[nodiscard]] inline vertex_type& at(const std::size_t& idx) const override {
        return const_cast<vertex_type&>(this->_adjacency_list.at(idx));
    }

    [[nodiscard]] inline const container_type& vertices() const override {
        return const_cast<container_type&>(this->_adjacency_list);
    }


    // modifiers
    inline void add_vertex() override {
        this->_container_insert(this->_adjacency_list, vertex_type(this->num_vertices()));
    }

    void add_vertices(const vertex_key_type& num_new_vertices) override {
        // TODO: check type overflow
        std::size_t new_size = this->num_vertices() + num_new_vertices;
        for (vertex_key_type key = this->num_vertices(); key < new_size; key++)
            this->_container_insert(this->_adjacency_list, vertex_type(key));
    }

    inline void add_edge(const edge_type& edge) override {
        this->_add_edge<>(edge);
    }

    void add_edges(const vertex_type::container_type& edges) override {
        for (const auto& edge : edges)
            this->_add_edge<>(edge);
    }


private:
    using container_iterator = gl::container_traits<container_s, vertex_type>::iterator;
    using container_const_iterator = gl::container_traits<container_s, vertex_type>::const_iterator;
    std::function<void(container_type&, const vertex_type&)> _container_insert =
        container_traits<container_s, vertex_type>::insert;


    inline bool _index_in_range(const std::size_t& idx) const {
        return idx < this->num_vertices();
    }

    template<bool directed = DIRECTED> requires(directed)
    void _add_edge(const edge_type& edge) {
        // TODO: check for multiedges
        if (!(this->_index_in_range(edge.source) && this->_index_in_range(edge.destination)))
            return;

        this->_adjacency_list[edge.source].add_edge(edge);
        this->_adjacency_list[edge.destination]._in_deg++;
    }

    template<bool directed = DIRECTED> requires(!directed)
    void _add_edge(const edge_type& edge) {
        if (!(this->_index_in_range(edge.source) && this->_index_in_range(edge.destination)))
        return;

        this->_adjacency_list[edge.source].add_edge(edge);
        this->_adjacency_list[edge.destination].add_edge(edge.reverse());
    }
};

} // namespace gl

#endif // CPP_GL_GRAPH
