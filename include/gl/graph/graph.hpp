#ifndef CPP_GL_GRAPH
#define CPP_GL_GRAPH

#include <numeric>

#include <gl/graph/graph_traits.hpp>



namespace gl {

template <
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
    using adj_container_type = vertex_t::container_type;

private:
    container_type _adjacency_list;

public:
    graph() = default;
    ~graph() = default;

    [[nodiscard]] inline std::size_t num_vertices () const override {
        return this->_adjacency_list.size();
    }

    [[nodiscard]] std::size_t num_edges () const override {
        std::size_t edges = 0;
        for (const vertex_type& vertex : this->_adjacency_list)
            edges += vertex.degree();
        return edges;
    }

    [[nodiscard]] inline bool empty () const override {
        return this->_adjacency_list.size() == 0;
    }

    void add_vertex (const vertex_key_type& vertex_key) override {
        // TODO: insert vertex to adjacency list
        // TODO: add an insert method to container_traits
    };

    void add_edge (const edge_type& edge) {
        this->_add_edge<>(edge);
    }

private:
    template <bool directed = DIRECTED>
    std::enable_if_t<directed> _add_edge (const edge_type& edge) {
        std::cout << "todo: directed graph" << std::endl;
    }

    template <bool directed = DIRECTED>
    std::enable_if_t<!directed> _add_edge (const edge_type& edge) {
        std::cout << "todo: undirected graph" << std::endl;
    }
};

} // namespace gl

#endif // CPP_GL_GRAPH