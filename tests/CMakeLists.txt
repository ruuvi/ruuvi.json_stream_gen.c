cmake_minimum_required(VERSION 3.5)

project(test-json_stream_gen)
set(ProjectId test-json_stream_gen)

add_definitions(
        -DJSON_STREAM_GEN_TESTS=1
)

enable_testing()
add_subdirectory(googletest)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(SRC ${ROOT_DIR}/src)
set(INC ${ROOT_DIR}/include)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -Wno-error=nonnull-compare")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wno-error=nonnull-compare -Wno-missing-field-initializers")

add_executable(${ProjectId}
        test_json_stream_gen_create_delete.cpp
        test_json_stream_gen_unformatted.cpp
        test_json_stream_gen_formatted.cpp
        json_stream_gen_wrapper.h
        ${SRC}/json_stream_gen.c
        ${INC}/json_stream_gen.h
        )

set_target_properties(${ProjectId} PROPERTIES
        C_STANDARD 11
        CXX_STANDARD 14
        )

target_include_directories(${ProjectId} PUBLIC
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
        ${INC}
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_compile_definitions(${ProjectId} PUBLIC
        TEST_JSON_STREAM_GEN=1
        )

target_compile_options(${ProjectId} PUBLIC
        -g3
        -ggdb
        -fprofile-arcs
        -ftest-coverage
        --coverage
        )

target_link_libraries(${ProjectId}
        gtest
        gtest_main
        gcov
        --coverage
        )

add_test(NAME test-json_stream_gen
        COMMAND test-json_stream_gen
        --gtest_output=xml:$<TARGET_FILE_DIR:test-json_stream_gen>/gtestresults.xml
        )
