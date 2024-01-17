
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

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-mfma" COMPILER_SUPPORTS_ARCH_MFMA)
if(COMPILER_SUPPORTS_ARCH_MFMA)
  set(CMAKE_CXX_FLAGS "-mfma")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Set source files
set(SOURCES "pene.cpp" "module.cpp" "version_module.cpp"  "utils/match.cpp"
			"counters.cpp" "counters_module.cpp"  "update_counters.cpp" 
			"symbol_list_generator_module.cpp" "replace_module.cpp" "interflop_module.cpp"
			"pin_utils/instrumenter.cpp" "pin_utils/filter.cpp" 
			"pin_utils/element_instrumenter.cpp" "pin_utils/symbol_filter.cpp" 
			"pin_utils/source_filter.cpp" 
			"replace/backend/backend_verrou/interflop_verrou.cpp" "replace/backend/backend_verrou/tinymt64.cpp"
			"${CMAKE_SOURCE_DIR}/extern/interflop/interflop_stdlib.c"
			"${CMAKE_SOURCE_DIR}/extern/interflop/iostream/logger.c"
)

set(PIN_TOOL pene)


# Add target
add_library (${PIN_TOOL} SHARED ${SOURCES})
set_property(TARGET ${PIN_TOOL} PROPERTY CXX_STANDARD 17)


target_compile_definitions(${PIN_TOOL} PRIVATE  "-DHAVE_FMA_INTRINSIC")
target_link_libraries(${PIN_TOOL} ${PIN_LIBRARIES})
target_link_libraries(${PIN_TOOL} instr argp interflop_stdlib)
target_link_options(${PIN_TOOL} PUBLIC ${PIN_LDFLAGS}) 

install(TARGETS ${PIN_TOOL})
