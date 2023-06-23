#ifndef CPP_GL_ADJACENT_CONTAINER
#define CPP_GL_ADJACENT_CONTAINER

#include <utility/types.hpp>



namespace gl {

template <typename container_t> 
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

} // namesapce gl

#endif // CPP_GL_ADJACENT_CONTAINER