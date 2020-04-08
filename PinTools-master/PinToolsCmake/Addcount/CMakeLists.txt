cmake_minimum_required (VERSION 3.14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Set source files
set(SOURCES "Addcount.cpp" "version_module.cpp" )
set(PIN_TOOL Addcount)

# Add target
add_library (${PIN_TOOL} SHARED ${SOURCES})
set_property(TARGET ${PIN_TOOL} PROPERTY CXX_STANDARD 17)

target_link_libraries(${PIN_TOOL} ${PIN_LIBRARIES})
target_link_options(${PIN_TOOL} PUBLIC ${PIN_LDFLAGS}) 

install(TARGETS ${PIN_TOOL})

# Add tests
add_test(NAME TestVersion
		 COMMAND $<TARGET_FILE:pin> -t $<TARGET_FILE:${PIN_TOOL}> -version 1 -- "C:/PinTools-master/sampleprog/sampleprog/x64/Debug/sampleprog.exe")

add_test(NAME TestAddcount
		 COMMAND $<TARGET_FILE:pin> -t $<TARGET_FILE:${PIN_TOOL}> -- "C:/PinTools-master/sampleprog/sampleprog/x64/Debug/sampleprog.exe")