cmake_minimum_required(VERSION 3.12)

# Check if cpp-gl is a top level project
if (NOT DEFINED PROJECT_NAME)
    set(CPP_GL_IS_TOP_LEVEL_PROJECT ON)
else()
    set(CPP_GL_IS_TOP_LEVEL_PROJECT OFF)
endif()

project(cpp-gl
    VERSION 1.0.1
    DESCRIPTION "General purpose header-only template graph library for C++20"
    HOMEPAGE_URL "https://github.com/SpectraL519/cpp-gl"
    LANGUAGES CXX
)

option(BUILD_TESTS "Build project tests" ON)

# Set the proper cxx standard
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
elseif(CMAKE_CXX_STANDARD LESS 20)
    message(WARNING "The defined C++ standard (${CMAKE_CXX_STANDARD}) is too low - overriding!")
    set(CMAKE_CXX_STANDARD 20)
endif()
message(STATUS "The C++ standard is set to ${CMAKE_CXX_STANDARD}")

# Define the library target
add_library(cpp-gl INTERFACE)
target_include_directories(cpp-gl INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
set_target_properties(cpp-gl PROPERTIES
    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Installation configuration
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install the headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install the library target
install(TARGETS cpp-gl EXPORT cpp-gl-targets)

# Create a Config file for find_package
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cpp-gl-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cpp-gl-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp-gl
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cpp-gl-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY ExactVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cpp-gl-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cpp-gl-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp-gl
)

install(EXPORT cpp-gl-targets
    FILE cpp-gl-targets.cmake
    NAMESPACE cpp-gl::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp-gl
)

# Exporting from the build tree
export(EXPORT cpp-gl-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/cpp-gl-targets.cmake
    NAMESPACE cpp-gl::
)
export(PACKAGE cpp-gl)

# Include test directory if cpp-gl is a top level project
if (CPP_GL_IS_TOP_LEVEL_PROJECT AND BUILD_TESTS)
    add_subdirectory(tests)
endif()
