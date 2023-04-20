cmake_minimum_required (VERSION 3.14)

project(test_symbol_list_generator)


add_library(shared_funs SHARED "shared_funs.cpp")
set_property(TARGET shared_funs PROPERTY CXX_STANDARD 17)

add_library(shared2_funs SHARED "shared2_funs.cpp")
set_property(TARGET shared2_funs PROPERTY CXX_STANDARD 17)
target_link_libraries(shared2_funs PRIVATE shared_funs)

add_executable(executable "executable.cpp")
set_property(TARGET executable PROPERTY CXX_STANDARD 17)
target_link_libraries(executable PRIVATE shared2_funs)

set(TEST_SCRIPTS test_symbol_list_generator.py)
set(REFERENCE_FILES )
set(TEST_LIST test_symbol_list_generator.symbols_list_generation_tests.test_exec_and_shared)


foreach(test_script ${TEST_SCRIPTS})
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${test_script}
      COMMAND ${CMAKE_COMMAND} 
              -D INFILE=${CMAKE_CURRENT_SOURCE_DIR}/${test_script}
              -D OUTFILE=${CMAKE_CURRENT_BINARY_DIR}/${test_script}
              -D FLAG_TESTS_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
              -D FLAG_TESTS_ECEXUTABLE=$<TARGET_FILE:executable>
              -D EXECUTABLE=$<TARGET_FILE:executable>
              -D FLAG_TESTS_PINTOOL=$<TARGET_FILE:pene>
              -D PINTOOL=$<TARGET_FILE:pene>
              -D PIN_EXECUTABLE=$<TARGET_FILE:pin>
              -D PIN=$<TARGET_FILE:pin>
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

list(TRANSFORM REFERENCE_FILES PREPEND ${CMAKE_CURRENT_BINARY_DIR}/)
list(TRANSFORM TEST_SCRIPTS PREPEND ${CMAKE_CURRENT_BINARY_DIR}/)
add_custom_target(${PROJECT_NAME} ALL DEPENDS 
    ${TEST_SCRIPTS}
    ${REFERENCE_FILES}
    ${CMAKE_CURRENT_BINARY_DIR}/stderr2stdout.py
)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
foreach(unittest ${TEST_LIST})
	add_test(NAME ${unittest}
			COMMAND ${Python3_EXECUTABLE} stderr2stdout.py ${Python3_EXECUTABLE} -m unittest ${unittest})
endforeach()
	