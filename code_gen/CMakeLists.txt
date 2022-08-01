cmake_minimum_required (VERSION 3.14)


set(TARGET_OS "${CMAKE_HOST_SYSTEM_NAME}")

if (TARGET_OS STREQUAL "Windows")
    set(ENUM_FILE_PATH "${CMAKE_SOURCE_DIR}/Pin/Windows/extras/xed-intel64/include/xed/xed-iform-enum.h")
elseif(TARGET_OS STREQUAL "Linux")
    set(ENUM_FILE_PATH "${CMAKE_SOURCE_DIR}/Pin/Linux/extras/xed-intel64/include/xed/xed-iform-enum.h")
endif()



set(TEMPLATE_FILE "sse_avx_template.txt")
set(TEMPLATE_FOLDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/templates/")

set(OUTPUT_PATH  "${CMAKE_SOURCE_DIR}/include/replace/wrappers")



set(args_list 
     "${ENUM_FILE_PATH}"
     "${TEMPLATE_FILE}"
     "${OUTPUT_PATH}"
)
#for debugging purpose only
foreach(item ${args_list})
  message(STATUS "A list item: ${item}")
endforeach()

find_package(Python REQUIRED Interpreter)
if (NOT ${Python_FOUND})
    message(FATAL_ERROR "Python is needed to generate sources.")
endif ()

execute_process(COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/generator.py ${args_list})




