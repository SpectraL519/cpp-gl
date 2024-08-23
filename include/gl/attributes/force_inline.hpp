#pragma once

#ifndef gl_attr_force_inline

#if defined(__clang__)
#define gl_attr_force_inline [[clang::always_inline]] inline
#elif defined(__GNUC__)
#define gl_attr_force_inline [[gnu::always_inline]] inline
#else
#define gl_attr_force_inline inline
#endif

#endif
