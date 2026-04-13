// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/io/graph_fio.hpp"
#include "gl/io/options.hpp"
#include "gl/io/options_manip.hpp"
#include "hgl/hypergraph.hpp"

#include <filesystem>
#include <fstream>
#include <initializer_list>

namespace hgl::io {

using gl::io::append;
using gl::io::write;

namespace detail {

using gl::io::detail::open_infile;
using gl::io::detail::open_outfile;

} // namespace detail

template <traits::c_hypergraph HypergraphType, traits::c_io_save_mode Mode = write>
void save(
    const HypergraphType& hypergraph,
    const std::filesystem::path& path = "hypergraph.hgsf",
    const std::initializer_list<gl::io::options_manip>& options = {}
) {
    std::ofstream file = detail::open_outfile<Mode>(path);

    file << gl::io::spec_fmt;
    for (auto option : options)
        file << option;

    file << hypergraph;
}

template <traits::c_hypergraph HypergraphType>
[[nodiscard]] HypergraphType load(const std::filesystem::path& path = "hypergraph.hgsf") {
    std::ifstream file = detail::open_infile(path);

    file >> gl::io::spec_fmt;

    HypergraphType hypergraph;
    file >> hypergraph;

    return hypergraph;
}

} // namespace hgl::io
