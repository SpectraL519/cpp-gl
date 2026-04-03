#pragma once

#if defined(__clang__) || defined(__GNUC__)

#define SUPPRESS_PRAGMA(x) _Pragma(#x)

#define SUPPRESS_WARNING_BEGIN(w)        \
    SUPPRESS_PRAGMA(GCC diagnostic push) \
    SUPPRESS_PRAGMA(GCC diagnostic ignored w)

#define SUPPRESS_WARNING_END SUPPRESS_PRAGMA(GCC diagnostic pop)

#else

#define SUPPRESS_WARNING_BEGIN(w)
#define SUPPRESS_WARNING_END

#endif
