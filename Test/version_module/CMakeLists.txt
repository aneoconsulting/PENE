cmake_minimum_required (VERSION 3.14)

project(version_module)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test_version_executable "executable.cpp")
set_property(TARGET test_version_executable PROPERTY CXX_STANDARD 17)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_version_module.py
  COMMAND ${CMAKE_COMMAND} 
          -D INFILE=${CMAKE_CURRENT_SOURCE_DIR}/test_version_module.py.in
          -D OUTFILE=${CMAKE_CURRENT_BINARY_DIR}/test_version_module.py
          -D FLAG_TESTS_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
          -D FLAG_TESTS_ECEXUTABLE=$<TARGET_FILE:test_version_executable>
          -D FLAG_TESTS_PINTOOL=$<TARGET_FILE:Addcount>
          -D PIN_EXECUTABLE=$<TARGET_FILE:pin>
          -P ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
  DEPENDS
    ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/test_version_module.py.in
  COMMENT "Configuring test_version_module.py"
  VERBATIM
)

set(TEST_SCRIPTS test_version_module.py)
set(REFERENCE_FILES reference_active.txt reference_inactive.txt)
set(TEST_LIST test_version_module.flag_tests.test_active test_version_module.flag_tests.test_inactive)



function(copy_file FILENAME)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
                ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
        COMMENT "Copying ${FILENAME}"
        VERBATIM
    )
endfunction(copy_file)

foreach(reference_file ${REFERENCE_FILES})
    copy_file(${reference_file})
endforeach()

list(TRANSFORM REFERENCE_FILES PREPEND ${CMAKE_CURRENT_BINARY_DIR}/)
list(TRANSFORM TEST_SCRIPTS PREPEND ${CMAKE_CURRENT_BINARY_DIR}/)
add_custom_target(${PROJECT_NAME} ALL DEPENDS 
    ${TEST_SCRIPTS}
    ${REFERENCE_FILES}
)

find_package(Python REQUIRED Interpreter)
foreach(unittest ${TEST_LIST})
	add_test(NAME ${unittest}
			COMMAND ${Python_EXECUTABLE} -m unittest ${unittest})
endforeach()

	