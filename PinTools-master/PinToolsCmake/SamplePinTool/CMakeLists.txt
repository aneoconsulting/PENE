cmake_minimum_required (VERSION 3.14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Set source files
set(SOURCES "SamplePinTool.cpp" "Filters.cpp" "backends/opposite_ieee.cpp" "backends/ieee.cpp")
set(PIN_TOOL SamplePinTool)

# Add target
add_library (${PIN_TOOL} SHARED ${SOURCES})
set_property(TARGET ${PIN_TOOL} PROPERTY CXX_STANDARD 17)

target_link_libraries(${PIN_TOOL} ${PIN_LIBRARIES})
target_link_options(${PIN_TOOL} PUBLIC ${PIN_LDFLAGS}) 

install(TARGETS ${PIN_TOOL})

# Add tests
#add_test(NAME TestSamplePinTool COMMAND $<TARGET_FILE:pin> -t $<TARGET_FILE:${PIN_TOOL}> -- "C:/PinTools-master/sampleprog/sampleprog/x64/Debug/sampleprog.exe")
