cmake_minimum_required(VERSION 3.5)

set(JSON_STREAM_GEN_SRC
        include/json_stream_gen.h
        src/json_stream_gen.c
)

set(JSON_STREAM_GEN_INC
        include
)

if(${ESP_PLATFORM})

    idf_component_register(
        SRCS
            ${JSON_STREAM_GEN_SRC}
        INCLUDE_DIRS
            ${JSON_STREAM_GEN_INC}
    )
    target_compile_options(__idf_ruuvi.json_stream_gen.c PRIVATE -Wall -Werror -Wextra -Wno-error=nonnull-compare)

else()

    project(json_stream_gen)
    set(ProjectId json_stream_gen)

    add_library(${ProjectId} STATIC ${JSON_STREAM_GEN_SRC})

    target_include_directories(${ProjectId} PUBLIC ${JSON_STREAM_GEN_INC})

    set_target_properties(${ProjectId} PROPERTIES
            C_STANDARD 11
            CXX_STANDARD 14
    )

    target_compile_options(${ProjectId} PRIVATE -Wall -Werror -Wextra -Wno-error=nonnull-compare)

endif()
