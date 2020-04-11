cmake_minimum_required (VERSION 3.14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Set source files
set(SOURCES "sampleprog.cpp")
set(TARGET_EXE sampleprog)

# Add target
add_executable (${TARGET_EXE} ${SOURCES})
set_property(TARGET ${TARGET_EXE} PROPERTY CXX_STANDARD 17)

