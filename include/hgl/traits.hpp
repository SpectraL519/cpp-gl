// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/traits.hpp
/// @brief Contains C++20 concepts and type traits used to constrain hypergraph library templates.

#pragma once

#include "gl/traits.hpp"

namespace hgl {

/// @ingroup HGL-Traits
/// @brief Traits and concepts for the HGL module.
///
/// This namespace contains the definitions of all hypergraph-specifc concepts and type traits
/// used to constrain library types and pulls in all standard graph traits, concept checkers,
/// and metaprogramming utilities from `gl::traits`. Because hypergraphs share the same underlying
/// implementation design and mechanisms as standard graphs, they seamlessly reuse the same
/// fundamental C++20 concepts.
namespace traits {

using namespace gl::traits;

} // namespace traits
} // namespace hgl
