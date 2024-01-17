project("argp")

cmake_minimum_required(VERSION 3.20)

set (ARGP_C_SRC
    "argp-parse.c"
)

add_library(argp_c OBJECT ${ARGP_C_SRC})
target_compile_definitions(argp_c PRIVATE ${CRT_COMPILE_DEFINITIONS})
target_compile_options (argp_c PRIVATE ${CRT_PREPROCESS_OPTIONS} ${CRT_COMPILE_OPTIONS} "-fPIC")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
add_library (argp SHARED $<TARGET_OBJECTS:argp_c>)
target_link_options (argp PRIVATE ${CRT_LINK_OPTIONS})
target_link_libraries (argp ${CRT_LINK_LIBRARIES})

