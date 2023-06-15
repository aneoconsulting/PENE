cmake_minimum_required (VERSION 3.20)
include(GoogleTest)

project(cpp-unit)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Add target
add_executable(match_test "utils/match_test.cpp" "${CMAKE_SOURCE_DIR}/src/utils/match.cpp")

foreach(target match_test)
    target_link_libraries(${target} gtest_main)

    gtest_discover_tests(${target} EXTRA_ARGS "--gtest_output=xml" TEST_PREFIX "cpp-unit::")
endforeach()