
project(code_generator LANGUAGES CXX)

set(TARGET_OS "${CMAKE_HOST_SYSTEM_NAME}")

if (TARGET_OS STREQUAL "Windows")
    set(ENUM_FILE_PATH "${CMAKE_SOURCE_DIR}/Pin/Windows/extras/xed-intel64/include/xed/xed-iform-enum.h")
elseif(TARGET_OS STREQUAL "Linux")
    set(ENUM_FILE_PATH "${CMAKE_SOURCE_DIR}/Pin/Linux/extras/xed-intel64/include/xed/xed-iform-enum.h")
endif()



set(TEMPLATE_FILE "wrapper_template.txt")
set(TEMPLATE_FOLDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/templates/")
set(OUTPUT_PATH  "${CMAKE_SOURCE_DIR}/include/replace/")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/templates/wrapper_template.txt" "${CMAKE_CURRENT_BINARY_DIR}/templates/wrapper_template.txt" COPYONLY)

file(TOUCH just_for_cmake.cpp)

set(args_list 
     "${ENUM_FILE_PATH}"
     "${TEMPLATE_FILE}"
     "${OUTPUT_PATH}"
)

find_package(Python3 REQUIRED Interpreter)
if (NOT ${Python3_FOUND})
    message(FATAL_ERROR "Python is needed to generate sources.")
endif ()

execute_process(
    COMMAND pip show Jinja2
    RESULT_VARIABLE EXIT_CODE
    OUTPUT_QUIET
)

if (NOT ${EXIT_CODE} EQUAL 0)
    message(
            FATAL_ERROR
            "The \"Jinja2\" Python3 package is not installed. Please install it using the following command: \"pip3 install Jinja2\"."
    )
endif()

add_custom_command(OUTPUT "${OUTPUT_PATH}/sse.h" "${OUTPUT_PATH}/avx.h" "${OUTPUT_PATH}/avx512.h" "${OUTPUT_PATH}/fma.h"
                   COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/generator.py" ${args_list}
                   DEPENDS  "${CMAKE_CURRENT_SOURCE_DIR}/generator.py" ${ENUM_FILE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/templates/wrapper_template.txt" 
                   COMMENT "Generating files"
                   VERBATIM)
add_library(instr STATIC "${OUTPUT_PATH}/sse.h" "${OUTPUT_PATH}/avx.h" "${OUTPUT_PATH}/avx512.h" "${OUTPUT_PATH}/fma.h" "${CMAKE_CURRENT_SOURCE_DIR}/just_for_cmake.cpp" )