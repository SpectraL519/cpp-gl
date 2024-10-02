// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

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
