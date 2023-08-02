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

public:
    graph() = default;
    ~graph() = default;

    [[nodiscard]] inline std::size_t num_vertices () const override;
    [[nodiscard]] std::size_t num_edges () const override;
    [[nodiscard]] inline bool empty () const override;
    [[nodiscard]] inline bool has_vertex (const vertex_key_type& vertex_key) const override;

    [[nodiscard]] inline vertex_type& operator [] (const std::size_t& index) const override;
    [[nodiscard]] vertex_type& at (const std::size_t& index) const override;
    [[nodiscard]] std::optional<std::reference_wrapper<vertex_type>> get_vertex (const vertex_key_type& vertex_key) override; 
    [[nodiscard]] inline const container_type& vertices () const override;

    void add_vertex (const vertex_type& vertex) override;
    void add_vertex (const vertex_key_type& vertex_key) override;
    bool add_edge (const edge_type& edge) override;   

private:
    std::optional<std::size_t> _index_of (const vertex_key_type& vertex_key) const;

    template <bool directed = DIRECTED> requires (directed)
    bool _add_edge (const edge_type& edge);
    
    template <bool directed = DIRECTED> requires (!directed)
    bool _add_edge (const edge_type& edge);    
};



// public member functions
template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
[[nodiscard]] inline std::size_t gl::graph<DIRECTED, vertex_t, container_s>::num_vertices () const {
    return this->_adjacency_list.size();
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
[[nodiscard]] std::size_t gl::graph<DIRECTED, vertex_t, container_s>::num_edges () const {
    std::size_t edges = 0;
    for (const vertex_type& vertex : this->_adjacency_list)
        edges += vertex.degree();
    return edges;
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
[[nodiscard]] inline bool gl::graph<DIRECTED, vertex_t, container_s>::empty () const {
    return this->num_vertices() == 0;
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
[[nodiscard]] inline bool gl::graph<DIRECTED, vertex_t, container_s>::has_vertex (const vertex_key_type& vertex_key) const {
    return (this->_index_of(vertex_key) != std::nullopt);
}


template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
[[nodiscard]] inline gl::graph<DIRECTED, vertex_t, container_s>::vertex_type&
              gl::graph<DIRECTED, vertex_t, container_s>::operator[] (const std::size_t& index) const {
    return const_cast<vertex_type&>(this->_adjacency_list[index]);
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
[[nodiscard]] gl::graph<DIRECTED, vertex_t, container_s>::vertex_type&
              gl::graph<DIRECTED, vertex_t, container_s>::at (const std::size_t& index) const  {
    if (index >= this->num_vertices())
        throw std::out_of_range(
            "index (" + std::to_string(index) + ") >= this->num_vertices() (" + std::to_string(this->num_vertices()) + ")"
        );

    return const_cast<vertex_type&>(this->_adjacency_list[index]);
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
[[nodiscard]] std::optional<std::reference_wrapper<typename gl::graph<DIRECTED, vertex_t, container_s>::vertex_type>> 
              gl::graph<DIRECTED, vertex_t, container_s>::get_vertex (const vertex_key_type& vertex_key) {
    std::optional<std::size_t> idx = this->_index_of(vertex_key);
    if (!idx) 
        return std::nullopt;

    return std::ref(this->_adjacency_list[idx.value()]);
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
[[nodiscard]] inline const gl::graph<DIRECTED, vertex_t, container_s>::container_type& 
              gl::graph<DIRECTED, vertex_t, container_s>::vertices () const {
    return const_cast<container_type&>(this->_adjacency_list);
}


template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
void gl::graph<DIRECTED, vertex_t, container_s>::add_vertex (const vertex_key_type& vertex_key) {
    this->_container_insert(this->_adjacency_list, vertex_key);
};

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
void gl::graph<DIRECTED, vertex_t, container_s>::add_vertex (const vertex_type& vertex) {
    this->_container_insert(this->_adjacency_list, vertex);
};

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
bool gl::graph<DIRECTED, vertex_t, container_s>::add_edge (const edge_type& edge) {
    return this->_add_edge<>(edge);
}



// private member functions
template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
std::optional<std::size_t> gl::graph<DIRECTED, vertex_t, container_s>::_index_of (const vertex_key_type& vertex_key) const {
    container_const_iterator it = std::find_if(
        this->_adjacency_list.cbegin(), this->_adjacency_list.cend(), 
        [&vertex_key] (const vertex_type& vertex) { return vertex.key == vertex_key; }
    );

    if (it == this->_adjacency_list.cend())
        return std::nullopt;
    return std::distance(this->_adjacency_list.cbegin(), it);
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
template <bool directed> requires (directed)
bool gl::graph<DIRECTED, vertex_t, container_s>::_add_edge (const edge_type& edge) {
    std::optional<std::size_t> src_idx = this->_index_of(edge.source);
    std::optional<std::size_t> dst_idx = this->_index_of(edge.destination);
    if (!(src_idx && dst_idx)) return false;

    const bool correctly_added = this->_adjacency_list[src_idx.value()].add_edge(edge);
    if (correctly_added) 
        this->_adjacency_list[dst_idx.value()]._in_deg++;

    return correctly_added;
}

template <bool DIRECTED, gl::vertex_descriptor_t vertex_t, gl::graph_container_s container_s>
template <bool directed> requires (!directed)
bool gl::graph<DIRECTED, vertex_t, container_s>::_add_edge (const edge_type& edge) {
    std::optional<std::size_t> src_idx = this->_index_of(edge.source);
    std::optional<std::size_t> dst_idx = this->_index_of(edge.destination);
    if (!(src_idx && dst_idx)) return false;
    
    this->_adjacency_list[src_idx.value()].add_edge(edge);
    this->_adjacency_list[dst_idx.value()].add_edge(edge.reverse());
    return true;
}

} // namespace gl

#endif // CPP_GL_GRAPH