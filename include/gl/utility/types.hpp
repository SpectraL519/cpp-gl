#pragma once



namespace gl {

// container specifier types
struct vector             {};
struct deque              {};
struct linked_list        {};
struct doubly_linked_list {};
struct set                {}; // TODO: add Compare param
struct multiset           {}; // TODO: add Compare param


// graph specifier types
using directed_specifier_type = bool;

} // namespace gl
