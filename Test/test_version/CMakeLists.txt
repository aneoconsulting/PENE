cmake_minimum_required (VERSION 3.14)

project(version_module)

add_executable(test_version_executable "test_version.cpp")
set_property(TARGET test_version_executable PROPERTY CXX_STANDARD 17)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_version.py
  COMMAND ${CMAKE_COMMAND} 
          -D INFILE=${CMAKE_CURRENT_SOURCE_DIR}/test_version.py.in
          -D OUTFILE=${CMAKE_CURRENT_BINARY_DIR}/test_version.py
          -D FLAG_TESTS_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
          -D FLAG_TESTS_ECEXUTABLE=$<TARGET_FILE:test_version_executable>
          -D FLAG_TESTS_PINTOOL=$<TARGET_FILE:pene>
          -D PIN_EXECUTABLE=$<TARGET_FILE:pin>
          -P ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
  DEPENDS
    ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/test_version.py.in
  COMMENT "Configuring test_version.py"
  VERBATIM
)

set(TEST_SCRIPTS test_version.py)
set(REFERENCE_FILES test_version_reference_inactive.txt test_version_reference_active.txt)
set(TEST_LIST test_version.flag_tests.test_active test_version.flag_tests.test_inactive)



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

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/stderr2stdout.py
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_TOOLS_PATH}/stderr2stdout.py
            ${CMAKE_CURRENT_BINARY_DIR}/stderr2stdout.py
    DEPENDS
        ${CMAKE_TOOLS_PATH}/stderr2stdout.py
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

find_package(Python REQUIRED Interpreter)
foreach(unittest ${TEST_LIST})
	add_test(NAME ${unittest}
			COMMAND ${Python_EXECUTABLE} stderr2stdout.py ${Python_EXECUTABLE} -m unittest ${unittest})
endforeach()

	