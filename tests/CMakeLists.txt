cmake_minimum_required(VERSION 3.12)
project(cpp-gl-test)

# Common structure
set(SOURCE_DIRS "app")
set(INCLUDE_DIRS "include" "external")
set(EXECUTABLE_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Test target generation
function(add_test_target BASE_NAME SOURCE_DIRS DATA_DIR TESTING_DEF)
    set(TARGET_NAME "cpp-gl-tests-${BASE_NAME}")

    set(TARGET_SOURCES "")
    foreach(SOURCE_DIR ${SOURCE_DIRS})
        file(GLOB_RECURSE CURRENT_SOURCES "${SOURCE_DIR}/*.cpp")
        list(APPEND TARGET_SOURCES ${CURRENT_SOURCES})
    endforeach()

    add_executable(${TARGET_NAME} ${TARGET_SOURCES})

    set_target_properties(${TARGET_NAME} PROPERTIES
        OUTPUT_NAME "${BASE_NAME}"
        RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_DIR}"
        CXX_STANDARD ${CMAKE_CXX_STANDARD}
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )

    target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIRS})
    target_compile_options(${TARGET_NAME} PRIVATE
        -Werror
        -Wall
        -Wextra
        -Wcast-align
        -Wconversion
        -Wsign-conversion
        -Wunreachable-code
        -Wuninitialized
        -Wunused
        -pedantic
        -g
        -O2
    )
    target_compile_definitions(${TARGET_NAME} PRIVATE
        ${TESTING_DEF}
        TEST_DATA_PATH="${DATA_DIR}"
    )
    target_link_libraries(${TARGET_NAME} PRIVATE cpp-gl)
endfunction()

# GL target
set(GL_SOURCE_DIRS "source/gl" ${SOURCE_DIRS})
set(GL_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data/gl")
add_test_target(gl "${GL_SOURCE_DIRS}" "${GL_DATA_DIR}" GL_TESTING)

# HGL target
set(HGL_SOURCE_DIRS "source/hgl" ${SOURCE_DIRS})
set(HGL_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data/hgl")
add_test_target(hgl "${HGL_SOURCE_DIRS}" "${HGL_DATA_DIR}" HGL_TESTING)
