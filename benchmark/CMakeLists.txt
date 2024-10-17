cmake_minimum_required(VERSION 3.12)

project(gl_benchmark LANGUAGES CXX)

set(SRC_DIR source)
set(INC_DIR include)
set(GL_INC_DIR ../include)

# Include FetchContent module
include(FetchContent)

# Fetch the CPP-AP library
FetchContent_Declare(
    cpp-ap
    GIT_REPOSITORY https://github.com/SpectraL519/cpp-ap.git
    GIT_TAG v1.1 # here you can specify the desired tag or branch
)

FetchContent_MakeAvailable(cpp-ap)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -03")

# Define the CPP-GL executable (queue)
add_executable(is_bipartite ${SRC_DIR}/is_bipartite.cpp)
target_link_libraries(is_bipartite PRIVATE cpp-ap)

# Common
include_directories(${INC_DIR} ${GL_INC_DIR})
