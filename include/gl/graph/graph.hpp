#ifndef CPP_GL_GRAPH
#define CPP_GL_GRAPH

#include <numeric>
#include <stdexcept>

#include <gl/graph/graph_traits.hpp>
#include <gl/vertex.hpp>



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

private:
    container_type _adjacency_list;

    using container_iterator = gl::container_traits<container_s, vertex_type>::iterator;
    using container_const_iterator = gl::container_traits<container_s, vertex_type>::const_iterator;
    std::function<void(container_type&, const vertex_type&)> _container_insert = 
        container_traits<container_s, vertex_type>::insert;
    static constexpr bool _container_resizable = gl::container_traits<container_s, vertex_type>::resizable;

public:
    graph() = default;
    ~graph() = default;

    [[nodiscard]] inline std::size_t num_vertices() const override;
    [[nodiscard]] std::size_t num_edges() const override;
    [[nodiscard]] inline std::size_t size() const override;
    [[nodiscard]] inline bool empty() const override;
    [[nodiscard]] inline bool has_vertex(const vertex_key_type& key) const override;

    [[nodiscard]] inline vertex_type& operator[](const std::size_t& idx) const override;
    [[nodiscard]] inline vertex_type& at(const std::size_t& idx) const override;
    [[nodiscard]] inline const container_type& vertices() const override;

    inline void add_vertex() override;
    void add_vertices(const vertex_key_type& num_new_vertices) override;
    inline void add_edge(const edge_type& edge) override;
    void add_edges(const vertex_type::container_type& edges) override;

private:
    inline bool _index_in_range(const std::size_t& idx) const; 

    template <bool directed = DIRECTED> requires(directed)
    void _add_edge(const edge_type& edge);
    
    template <bool directed = DIRECTED> requires(!directed)
    void _add_edge(const edge_type& edge);    
};



// public member functions
template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
std::size_t gl::graph<DIRECTED, vertex_t, container_s>::num_vertices() const {
    return this->_adjacency_list.size();
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
std::size_t gl::graph<DIRECTED, vertex_t, container_s>::num_edges() const {
    std::size_t edges = 0;
    for (const vertex_type& vertex : this->_adjacency_list)
        edges += vertex.degree();
    return edges;
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
std::size_t gl::graph<DIRECTED, vertex_t, container_s>::size() const {
    return this->num_vertices() + this->num_edges();
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
bool gl::graph<DIRECTED, vertex_t, container_s>::empty() const {
    return this->num_vertices() == 0;
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
bool gl::graph<DIRECTED, vertex_t, container_s>::has_vertex(const vertex_key_type& key) const {
    return this->_index_in_range(key);
}


template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
gl::graph<DIRECTED, vertex_t, container_s>::vertex_type&
              gl::graph<DIRECTED, vertex_t, container_s>::operator[](const std::size_t& idx) const {
    return const_cast<vertex_type&>(this->_adjacency_list[idx]);
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
gl::graph<DIRECTED, vertex_t, container_s>::vertex_type& 
gl::graph<DIRECTED, vertex_t, container_s>::at(const std::size_t& idx) const  {
    return const_cast<vertex_type&>(this->_adjacency_list.at(idx));
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
const gl::graph<DIRECTED, vertex_t, container_s>::container_type& 
      gl::graph<DIRECTED, vertex_t, container_s>::vertices() const {
    return const_cast<container_type&>(this->_adjacency_list);
}



template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
void gl::graph<DIRECTED, vertex_t, container_s>::add_vertex() {
    this->_container_insert(this->_adjacency_list, vertex_type(this->num_vertices()));
};

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
void gl::graph<DIRECTED, vertex_t, container_s>::add_vertices(const vertex_key_type& num_new_vertices) {
    // TODO: check type overflow
    std::size_t new_size = this->num_vertices() + num_new_vertices;
    for (vertex_key_type key = this->num_vertices(); key < new_size; key++)
        this->_container_insert(this->_adjacency_list, vertex_type(key));
};

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
void gl::graph<DIRECTED, vertex_t, container_s>::add_edge(const edge_type& edge) {
    this->_add_edge<>(edge);
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
void gl::graph<DIRECTED, vertex_t, container_s>::add_edges(const vertex_type::container_type& edges) {
    for (const auto& edge : edges) 
        this->_add_edge<>(edge);
}



// private member functions
template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
bool gl::graph<DIRECTED, vertex_t, container_s>::_index_in_range(const std::size_t& idx) const {
    return idx < this->num_vertices();
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
template <bool directed> requires(directed)
void gl::graph<DIRECTED, vertex_t, container_s>::_add_edge(const edge_type &edge) {
    // TODO: check for multiedges
    if (!(this->_index_in_range(edge.source) && this->_index_in_range(edge.destination))) 
        return;

    this->_adjacency_list[edge.source].add_edge(edge);
    this->_adjacency_list[edge.destination]._in_deg++;
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
template <bool directed> requires(!directed)
void gl::graph<DIRECTED, vertex_t, container_s>::_add_edge(const edge_type& edge) {
    if (!(this->_index_in_range(edge.source) && this->_index_in_range(edge.destination))) 
        return;
    
    this->_adjacency_list[edge.source].add_edge(edge);
    this->_adjacency_list[edge.destination].add_edge(edge.reverse());
}

} // namespace gl

#endif // CPP_GL_GRAPH