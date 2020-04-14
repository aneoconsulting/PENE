cmake_minimum_required (VERSION 3.14)

project(test_symbol_list_generator)

add_library(shared_funs SHARED "shared_funs.cpp")
set_property(TARGET test_counters_executable PROPERTY CXX_STANDARD 17)

add_library(shared2_funs SHARED "shared2_funs.cpp")
set_property(TARGET test_counters_executable PROPERTY CXX_STANDARD 17)
target_link_libraries(shared2_funs PRIVATE shared_funs)

add_executable(executable "executable.cpp")
set_property(TARGET executable PROPERTY CXX_STANDARD 17)
target_link_libraries(executable PRIVATE shared2_funs)


	