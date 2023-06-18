#ifndef CPP_GL_CONCEPTS
#define CPP_GL_CONCEPTS

#include <concepts>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>

#include <types.hpp>
#include <edges.hpp>



namespace gl {

// adjacent vertices container type
template <typename container_t, typename = void> 
struct adjacent_container_trait : std::false_type {};

template <typename T, typename allocator_t> 
struct adjacent_container_trait <gl::vect<T, allocator_t>> : std::true_type {};

template <typename T, typename allocator_t> 
struct adjacent_container_trait <gl::list<T, allocator_t>> : std::true_type {};

template <typename T, typename allocator_t> 
struct adjacent_container_trait <gl::set<T, allocator_t>> : std::true_type {};

template <typename T, typename allocator_t> 
struct adjacent_container_trait <gl::hash_set<T, allocator_t>> : std::true_type {};

template <typename container_t>
concept adjacent_container_t = adjacent_container_trait<container_t>::value;



// edge container type
template <typename descriptor_t, typename = void>
struct edge_descriptor_trait : std::false_type {};

template <index_t key_t>
struct edge_descriptor_trait <edge <key_t>> : std::true_type {};

template <index_t key_t, numerical_t weight_t>
struct edge_descriptor_trait <weighted_edge <key_t, weight_t>> : std::true_type {};

template <index_t key_t, numerical_t flow_t>
struct edge_descriptor_trait <flow_edge <key_t, flow_t>> : std::true_type {};

template <index_t key_t, numerical_t weight_t, numerical_t flow_t>
struct edge_descriptor_trait <weighted_flow_edge <key_t, weight_t, flow_t>> : std::true_type {};

template <typename descriptor_t>
concept edge_descriptor_t = edge_descriptor_trait<descriptor_t>::value;

} // namespace gl

#endif // CPP_GL_CONCEPTS