// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "graph_utility.hpp"

#include <filesystem>
#include <fstream>

namespace gl::io {

struct write {};

struct append {};

namespace detail {

template <typename T>
concept c_io_save_mode = type_traits::c_one_of<T, write, append>;

template <c_io_save_mode Mode>
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

template <c_io_save_mode Mode>
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

template <type_traits::c_graph GraphType, detail::c_io_save_mode Mode = write>
void save(
    const GraphType& graph,
    const std::filesystem::path& path = "graph.gsf",
    const std::initializer_list<stream_options_manipulator>& options = {}
) {
    std::ofstream file = detail::open_outfile<Mode>(path);

    file << enable_gsf;
    for (const auto& option : options)
        file << option;

    file << graph;
}

template <type_traits::c_graph GraphType>
[[nodiscard]] GraphType load(const std::filesystem::path& path = "graph.gsf") {
    std::ifstream file = detail::open_infile(path);

    file >> enable_gsf;

    GraphType graph;
    file >> graph;

    return graph;
}

} // namespace gl::io
