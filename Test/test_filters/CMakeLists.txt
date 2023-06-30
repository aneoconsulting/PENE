
project(test_filters)

set(TARGET_OS "${CMAKE_HOST_SYSTEM_NAME}")

add_library(filter_shared_funs SHARED "shared_fun.cpp")
set_property(TARGET filter_shared_funs PROPERTY CXX_STANDARD 17)

add_executable(filter_executable "executable.cpp")
set_property(TARGET filter_executable PROPERTY CXX_STANDARD 17)
target_link_libraries(filter_executable PRIVATE filter_shared_funs)

set(TEST_SCRIPTS test_filters.py 
                 excl_all.lst
                 excl_double.lst
                 excl_shared.lst
                 excl_shared_all.lst
                 excl_shared_double.lst
                 test_excl_tree_func_f.lst
                 )
set(TEST_LIST 
              test_filters.filters_tests.test_excl_all
              test_filters.filters_tests.test_excl_double
              test_filters.filters_tests.test_excl_shared
              test_filters.filters_tests.test_excl_shared_all
              test_filters.filters_tests.test_excl_shared_double
              test_filters.filters_tests.test_excl_tree_func_f
)

if (TARGET_OS STREQUAL "Windows")
    set(SYS_LIB_PATH "C:\\Windows\\SYSTEM32\\*")
elseif(TARGET_OS STREQUAL "Linux")
    set(SYS_LIB_PATH "/lib/x86_64-linux-gnu/libc.so*")
endif()

foreach(test_script ${TEST_SCRIPTS})
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${test_script}
      COMMAND ${CMAKE_COMMAND} 
              -D INFILE=${CMAKE_CURRENT_SOURCE_DIR}/${test_script}
              -D OUTFILE=${CMAKE_CURRENT_BINARY_DIR}/${test_script}
              -D EXECUTABLE=$<TARGET_FILE:filter_executable>
              -D SHARED_LIB=$<TARGET_FILE:filter_shared_funs>
              -D PINTOOL=$<TARGET_FILE:pene>
              -D PIN_EXECUTABLE=$<TARGET_FILE:pin>
              -D PIN=$<TARGET_FILE:pin>
              -D SYSTEM_LIBRARIES_PATH=${SYS_LIB_PATH}
              -P ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
      DEPENDS
        ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
        #${CMAKE_CURRENT_SOURCE_DIR}/${test_script}
      COMMENT "Configuring ${test_script}"
      VERBATIM
    )
endforeach()

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/stderr2stdout.py
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_TOOLS_PATH}/stderr2stdout.py
            ${CMAKE_CURRENT_BINARY_DIR}/stderr2stdout.py
    #DEPENDS
        #${CMAKE_TOOLS_PATH}/stderr2stdout.py
    COMMENT "Copying stderr2stdout.py"
    VERBATIM
)

list(TRANSFORM TEST_SCRIPTS PREPEND ${CMAKE_CURRENT_BINARY_DIR}/)
add_custom_target(${PROJECT_NAME} ALL DEPENDS 
    ${TEST_SCRIPTS}
    ${CMAKE_CURRENT_BINARY_DIR}/stderr2stdout.py
)

find_package(Python REQUIRED Interpreter)
foreach(unittest ${TEST_LIST})
	add_test(NAME ${unittest}
			COMMAND ${Python_EXECUTABLE} stderr2stdout.py ${Python_EXECUTABLE} -m unittest ${unittest})
endforeach()
	