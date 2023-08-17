cmake_minimum_required(VERSION 3.21)

project(
    cpp_gl
    VERSION 1.0
    DESCRIPTION "A general purpose C++20 header only graph library"
    HOMEPAGE_URL "https://github.com/SpectraL519/cpp-gl"
    LANGUAGES CXX
)

include(GNUInstallDirs)

if (NOT PROJECT_IS_TOP_LEVEL)
    message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
    message(STATUS "This project is a top-level one")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(cpp_gl INTERFACE)
target_compile_features(cpp_gl INTERFACE cxx_std_20)
target_include_directories(
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if(DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX is not set\n"
        "Default value: ${CMAKE_INSTALL_PREFIX}\n"
        "Will set it to ${CMAKE_SOURCE_DIR}/install"
    )
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_SOURCE_DIR}/install"
        CACHE PATH "Where the library will be installed to" FORCE
    )
else()
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX was already set\n"
        "Current value: ${CMAKE_INSTALL_PREFIX}"
    )
endif()

install(
    TARGETS cpp_gl
    EXPORT cpp_glTargets
)
install(
    EXPORT cpp_glTargets
    NAMESPACE gl::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/gl
)
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
install(
    FILES ${HEADER_FILES}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gl
)
