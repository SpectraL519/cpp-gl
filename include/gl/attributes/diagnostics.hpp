// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#define GL_PRAGMA(x) _Pragma(#x)

#if defined(__clang__)

#define GL_SUPPRESS_WARNING_BEGIN(w) \
    GL_PRAGMA(clang diagnostic push) \
    GL_PRAGMA(clang diagnostic ignored w)

#define GL_SUPPRESS_WARNING_END GL_PRAGMA(clang diagnostic pop)

#define GL_SUPPRESS_CLANG_WARNING_BEGIN(w) GL_SUPPRESS_WARNING_BEGIN(w)

// GCC-only warning: Push on Clang to keep the END macro balanced, but don't ignore
#define GL_SUPPRESS_GCC_WARNING_BEGIN(w) GL_PRAGMA(clang diagnostic push)

#elif defined(__GNUC__)

#define GL_SUPPRESS_WARNING_BEGIN(w) \
    GL_PRAGMA(GCC diagnostic push)   \
    GL_PRAGMA(GCC diagnostic ignored w)

#define GL_SUPPRESS_WARNING_END GL_PRAGMA(GCC diagnostic pop)

#define GL_SUPPRESS_GCC_WARNING_BEGIN(w) GL_SUPPRESS_WARNING_BEGIN(w)

// Clang-only warning: Push on GCC to keep the END macro balanced, but don't ignore
#define GL_SUPPRESS_CLANG_WARNING_BEGIN(w) GL_PRAGMA(GCC diagnostic push)

#else

#define GL_SUPPRESS_WARNING_BEGIN(w)
#define GL_SUPPRESS_WARNING_END
#define GL_SUPPRESS_GCC_WARNING_BEGIN(w)
#define GL_SUPPRESS_CLANG_WARNING_BEGIN(w)

#endif
