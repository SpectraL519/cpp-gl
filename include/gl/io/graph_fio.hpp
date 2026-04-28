// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/io/graph_fio.hpp
/// @brief File I/O utilities for safely saving and loading graphs to and from files.

#pragma once

#include "gl/graph.hpp"

#include <filesystem>
#include <fstream>

namespace gl {
namespace io {

/// @ingroup GL GL-IO
/// @brief Tag type specifying that a file should be opened in strictly write (truncate) mode.
///
/// When using this mode, the operation will intentionally throw an exception if the target file already exists
/// to prevent accidental data overwrites.
struct write {};

/// @ingroup GL GL-IO
/// @brief Tag type specifying that a file should be opened in append mode.
///
/// When using this mode, the operation will append data to an existing file. It will throw an exception
/// if the target file does not already exist.
struct append {};

} // namespace io

namespace traits {

/// @ingroup GL GL-Traits
/// @brief Concept checking if a provided type is a valid file I/O save mode.
/// @see gl::io::write "write"
/// @see gl::io::append "append"
template <typename T>
concept c_io_save_mode = traits::c_one_of<T, io::write, io::append>;

} // namespace traits

namespace io {
namespace detail {

template <traits::c_io_save_mode Mode>
requires(std::same_as<Mode, write>)
[[nodiscard]] std::ofstream open_outfile(const std::filesystem::path& path) {
    if (std::filesystem::exists(path))
        throw std::filesystem::filesystem_error(
            "Cannot save to file", path, std::make_error_code(std::errc::file_exists)
        );

    std::ofstream file;
    file.open(path, std::ios::trunc);

    if (not file.is_open())
        throw std::ios_base::failure(std::format("Could not open file `{}`", path.string()));

    return file;
}

template <traits::c_io_save_mode Mode>
requires(std::same_as<Mode, append>)
[[nodiscard]] std::ofstream open_outfile(const std::filesystem::path& path) {
    if (not std::filesystem::exists(path))
        throw std::filesystem::filesystem_error(
            "Cannot save to file", path, std::make_error_code(std::errc::no_such_file_or_directory)
        );

    if (not std::filesystem::is_regular_file(path))
        throw std::filesystem::filesystem_error(
            "Cannot save to file (invalid file type - should be a regular file",
            path,
            std::make_error_code(std::errc::invalid_argument)
        );

    std::ofstream file;
    file.open(path, std::ios::app);

    if (not file.is_open())
        throw std::ios_base::failure(std::format("Could not open file `{}`", path.string()));

    return file;
}

[[nodiscard]] inline std::ifstream open_infile(const std::filesystem::path& path) {
    if (not std::filesystem::exists(path))
        throw std::filesystem::filesystem_error(
            "Cannot load from file",
            path,
            std::make_error_code(std::errc::no_such_file_or_directory)
        );

    if (not std::filesystem::is_regular_file(path))
        throw std::filesystem::filesystem_error(
            "Cannot load from file (invalid file type - should be a regular file",
            path,
            std::make_error_code(std::errc::invalid_argument)
        );

    std::ifstream file;
    file.open(path);

    if (not file.is_open())
        throw std::ios_base::failure(std::format("Could not open file `{}`", path.string()));

    return file;
}

} // namespace detail

/// @ingroup GL GL-IO
/// @brief Serializes and saves a graph to a file.
///
/// Saves the graph topology and optionally its properties using the Graph Specification Format (GSF).
/// The function strictly respects the @ref gl::io::write "write" and @ref gl::io::append "append" safety guards.
///
/// @tparam GraphType The concrete type of the graph being saved. Must satisfy [**c_graph**](gl_concepts.md#hgl-traits-c-graph).
/// @tparam Mode The save behavior tag (@ref gl::io::write "write" or @ref gl::io::append "append"). Defaults to `write`.
/// @param graph The graph instance to serialize.
/// @param path The filesystem path where the graph will be saved. Defaults to `"graph.gsf"`.
/// @param options An initializer list of @ref gl::io::options_manip "options_manip" to configure the serialization (e.g., `gl::io::with_properties`).
///
/// @throws std::filesystem::filesystem_error If file safety checks fail (e.g., overwriting an existing file in `write` mode).
/// @throws std::ios_base::failure If the underlying file stream cannot be opened.
template <traits::c_graph GraphType, traits::c_io_save_mode Mode = write>
void save(
    const GraphType& graph,
    const std::filesystem::path& path = "graph.gsf",
    const std::initializer_list<options_manip>& options = {}
) {
    std::ofstream file = detail::open_outfile<Mode>(path);

    file << spec_fmt;
    for (const auto& option : options)
        file << option;

    file << graph;
}

/// @ingroup GL GL-IO
/// @brief Deserializes and loads a graph from a file.
///
/// Instantiates a new graph populated with the topology and properties read from the target GSF file.
///
/// @tparam GraphType The target graph type to construct. Must match the directional nature of the saved graph.
/// @param path The filesystem path from which to load the graph.
/// @return A newly constructed graph populated with the file's data.
///
/// @throws std::filesystem::filesystem_error If the file does not exist or is not a standard file.
/// @throws std::ios_base::failure If the file cannot be opened or if the GSF directional discriminator mismatches `GraphType`.
template <traits::c_graph GraphType>
[[nodiscard]] GraphType load(const std::filesystem::path& path) {
    std::ifstream file = detail::open_infile(path);

    file >> spec_fmt;

    GraphType graph;
    file >> graph;

    return graph;
}

} // namespace io
} // namespace gl
