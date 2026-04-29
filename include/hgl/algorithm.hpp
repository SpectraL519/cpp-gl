// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm.hpp
/// @brief Includes all algorithm-related headers, providing a comprehensive set of hypergraph algorithms for various hypergraph types and configurations.

#pragma once

// clang-format off

#include "hgl/algorithm/core.hpp"
#include "hgl/algorithm/util.hpp"

#include "hgl/algorithm/templates/bfs.hpp"
#include "hgl/algorithm/templates/dfs.hpp"

#include "hgl/algorithm/traversal/breadth_first_search.hpp"
#include "hgl/algorithm/traversal/depth_first_search.hpp"
#include "hgl/algorithm/traversal/backward_search.hpp"
#include "hgl/algorithm/traversal/forward_search.hpp"

// clang-format on
