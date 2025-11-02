// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

namespace gl::views {

inline constexpr auto deref = std::views::transform([](auto&& p) -> decltype(auto) { return *p; });

} // namespace gl::views
