#pragma once

#if defined(__clang__)
#define gl_attr_force_inline [[clang::always_inline]] inline
#elif defined(__GNUC__)
#define gl_attr_force_inline [[gnu::always_inline]] inline
#elif defined(_MSC_VER)
#define gl_attr_force_inline [[msvc::forceinline]] inline
#else
#define gl_attr_force_inline inline
#endif
