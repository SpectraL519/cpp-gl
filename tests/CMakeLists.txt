cmake_minimum_required(VERSION 3.12)
project(cpp-gl-test)

# Set compile options
if(NOT DEFINED CMAKE_CXX_FLAGS)
    set(
        CMAKE_CXX_FLAGS
        "-Wall -Wextra -Wcast-align -Wconversion -Wunreachable-code -Wuninitialized -Wunused -pedantic -g -O3"
        CACHE STRING "Default C++ compile flags" FORCE
    )
endif()

# Common structure
set(SOURCE_DIRS "app")
set(INCLUDE_DIRS "include" "external")
set(EXECUTABLE_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Target-specific structure
set(GL_SOURCE_DIRS "source/gl" ${SOURCE_DIRS})
set(GL_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data/gl")

set(HGL_SOURCE_DIRS "source/hgl" ${SOURCE_DIRS})
set(HGL_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data/hgl")

# GL: Source files
set(GL_SOURCES "")
foreach(SOURCE_DIR ${GL_SOURCE_DIRS})
    file(GLOB_RECURSE CURRENT_SOURCES ${SOURCE_DIR}/*.cpp)
    list(APPEND GL_SOURCES ${CURRENT_SOURCES})
endforeach()

# GL: Executable
add_executable(gl ${GL_SOURCES})
set_target_properties(gl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_DIR}"
    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_include_directories(gl PRIVATE ${INCLUDE_DIRS})
target_compile_options(gl PRIVATE ${CMAKE_CXX_FLAGS})
target_compile_definitions(gl PRIVATE GL_TESTING TEST_DATA_PATH="${GL_DATA_DIR}")
target_link_libraries(gl PRIVATE cpp-gl)

# HGL: Source files
set(HGL_SOURCES "")
foreach(SOURCE_DIR ${HGL_SOURCE_DIRS})
    file(GLOB_RECURSE CURRENT_SOURCES ${SOURCE_DIR}/*.cpp)
    list(APPEND HGL_SOURCES ${CURRENT_SOURCES})
endforeach()

# GL: Executable
add_executable(hgl ${HGL_SOURCES})
set_target_properties(hgl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_DIR}"
    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_include_directories(hgl PRIVATE ${INCLUDE_DIRS})
target_compile_options(hgl PRIVATE ${CMAKE_CXX_FLAGS})
target_compile_definitions(hgl PRIVATE GL_TESTING TEST_DATA_PATH="${HGL_DATA_DIR}")
target_link_libraries(hgl PRIVATE cpp-gl)
