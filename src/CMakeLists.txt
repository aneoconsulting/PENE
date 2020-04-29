cmake_minimum_required (VERSION 3.14)

if (PIN_FOUND)
	include_directories(${PIN_INCLUDE_DIRS})
	include_directories(SYSTEM ${PIN_CRT_INCLUDE_DIRS})

# Reset default compiler flags for avoiding conflicts
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "")
	set(CMAKE_CXX_FLAGS_DEBUG "")
	set(CMAKE_CXX_FLAGS "")
	add_compile_options(${PIN_CXXFLAGS})
else()
    message(FATAL_ERROR "Could not find PIN.")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Set source files
set(SOURCES "pene.cpp" "module.cpp" "version_module.cpp" 
			"counters.cpp" "counters_module.cpp"  "update_counters.cpp" 
			"symbol_list_generator_module.cpp"
			"replace_module.cpp")
set(PIN_TOOL pene)

# Add target
add_library (${PIN_TOOL} SHARED ${SOURCES})
set_property(TARGET ${PIN_TOOL} PROPERTY CXX_STANDARD 17)

target_link_libraries(${PIN_TOOL} ${PIN_LIBRARIES})
target_link_options(${PIN_TOOL} PUBLIC ${PIN_LDFLAGS}) 

install(TARGETS ${PIN_TOOL})