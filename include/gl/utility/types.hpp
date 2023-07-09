#ifndef CPP_GL_TYPES
#define CPP_GL_TYPES



namespace gl {

// numerical type concepts
template <typename T>
concept index_t = std::unsigned_integral<T>;

template <typename T>
concept arithmetic_t = std::is_arithmetic_v<T>;


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