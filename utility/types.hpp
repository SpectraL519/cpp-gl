#ifndef CPP_GL_TYPES
#define CPP_GL_TYPES

#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>



namespace gl {

// numerical type concepts
template <typename T>
concept index_t = std::unsigned_integral<T>;

template <typename T>
concept numerical_t = std::is_arithmetic_v<T>;



// container types
namespace container {

struct vect_s     {};
struct list_s     {};
struct set_s      {};
struct hash_set_s {};
struct map_s      {};
struct hash_map_s {};

} // namespace container

} // namespace gl

#endif // CPP_GL_TYPES