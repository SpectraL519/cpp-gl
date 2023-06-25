#ifndef CPP_GL_TYPES
#define CPP_GL_TYPES

#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <unordered_map>



namespace gl {

// container types
template <typename T, typename allocator_t = std::vector<T>::allocator_type>
using vect = std::vector <T, allocator_t>;

template <typename T, typename allocator_t = std::vector<T>::allocator_type>
using list = std::list <T, allocator_t>;

template <typename T, typename allocator_t = std::vector<T>::allocator_type>
using set = std::set <T, allocator_t>;

template <typename T, typename allocator_t = std::vector<T>::allocator_type>
using hash_set = std::unordered_set <T, allocator_t>;

template <typename T, typename allocator_t = std::vector<T>::allocator_type>
using hash_map = std::unordered_map <T, allocator_t>;

} // namespace gl

#endif // CPP_GL_TYPES