// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/io/hypergraph_fio.hpp
/// @brief File I/O utilities for safely saving and loading hypergraphs to and from files.

#pragma once

#include "gl/io/graph_fio.hpp"
#include "gl/io/options.hpp"
#include "gl/io/options_manip.hpp"
#include "hgl/hypergraph.hpp"

#include <filesystem>
#include <fstream>
#include <initializer_list>

namespace hgl::io {

/// @ingroup HGL-IO
/// @copybrief gl::io::append
/// @copydetails gl::io::append
using append = gl::io::append;

/// @ingroup HGL-IO
/// @copybrief gl::io::write
/// @copydetails gl::io::write
using write = gl::io::write;

namespace detail {

using gl::io::detail::open_infile;
using gl::io::detail::open_outfile;

} // namespace detail

/// @ingroup HGL-IO
/// @brief Serializes and saves a hypergraph to a file.
///
/// Writes the topology and optionally the properties of the hypergraph to the specified file using the Hypergraph Specification Format (HGSF). By default,
/// The function strictly respects the @ref hgl::io::write "write" and @ref hgl::io::append "append" safety guards.
///
/// @tparam HypergraphType The concrete type of the hypergraph being saved. Must satisfy [**c_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph).
/// @tparam Mode The file access mode (e.g., @ref hgl::io::write "write" or @ref hgl::io::append "append").
/// @param hypergraph The hypergraph instance to serialize.
/// @param path The filesystem path where the hypergraph will be saved. Defaults to `"hypergraph.hgsf"`.
/// @param options An optional initializer list of stream manipulators to configure the output (e.g., `{hgl::io::with_properties}`).
/// @throws std::filesystem::filesystem_error If file safety checks fail (e.g., overwriting an existing file in `write` mode).
/// @throws std::ios_base::failure If the underlying file stream cannot be opened.
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

/// @ingroup HGL-IO
/// @brief Deserializes and loads a hypergraph from a file.
///
/// Instantiates a new hypergraph populated with the topology and properties read from the target HGSF file.
///
/// @tparam HypergraphType The target hypergraph type to construct. Must match the directional nature of the saved hypergraph.
/// @param path The filesystem path from which to load the hypergraph.
/// @return A newly constructed hypergraph populated with the file's data.
///
/// @throws std::filesystem::filesystem_error If the file does not exist or is not a regular file.
/// @throws std::ios_base::failure If the file cannot be opened or if the HGSF directional discriminator mismatches `HypergraphType`.
template <traits::c_hypergraph HypergraphType>
[[nodiscard]] HypergraphType load(const std::filesystem::path& path) {
    std::ifstream file = detail::open_infile(path);

    file >> gl::io::spec_fmt;

    HypergraphType hypergraph;
    file >> hypergraph;

    return hypergraph;
}

} // namespace hgl::io
