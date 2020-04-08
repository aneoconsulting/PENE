cmake_minimum_required (VERSION 3.14)

project ("PENE")

cmake_policy(SET CMP0074 NEW)

# Reset default compiler flags for avoiding conflicts
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS "")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules" CACHE STRING "")

include(CommonConfig)

# Download Pin toolkit
configure_file(${CMAKE_SOURCE_DIR}/cmake/resources/CMakeLists.txt.Pin ${CMAKE_SOURCE_DIR}/Pin/CMakeLists.txt)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/Pin/build)
execute_process(COMMAND				${CMAKE_COMMAND}  -G "${CMAKE_GENERATOR}" ..
				RESULT_VARIABLE		PIN_DOWNLOAD_STATUS
				WORKING_DIRECTORY	${CMAKE_SOURCE_DIR}/Pin/build)

execute_process(COMMAND				${CMAKE_COMMAND}  --build .
				RESULT_VARIABLE		PIN_DOWNLOAD_STATUS
				WORKING_DIRECTORY	${CMAKE_SOURCE_DIR}/Pin/build)

find_package(PIN)
if (PIN_FOUND)
	include_directories(${PIN_INCLUDE_DIRS})
	include_directories(SYSTEM ${PIN_CRT_INCLUDE_DIRS})
	add_compile_options(${PIN_CXXFLAGS})
else()
    message(FATAL_ERROR "Could not find PIN.")
endif()

add_executable(pin IMPORTED GLOBAL)
set_property(TARGET pin PROPERTY IMPORTED_LOCATION ${PIN_ROOT}/pin)

# RPATH setup. Guidelines from https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling
# set installation directory for pin tools
set(CMAKE_INSTALL_PREFIX ${PIN_TOOLS_INSTALL_PREFIX})

# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" ${PIN_LIBRARY_DIRS})

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" ${PIN_LIBRARY_DIRS})
endif("${isSystemDir}" STREQUAL "-1")

enable_testing()

# Include sub-projects.
add_subdirectory ("SamplePinTool")
add_subdirectory ("Addcount")

