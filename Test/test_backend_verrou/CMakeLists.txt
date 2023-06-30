
project(test_backend_verrou)

add_executable(test_verrou_executable "test_verrou_executable.cpp")

set_property(TARGET test_verrou_executable PROPERTY CXX_STANDARD 17)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_verrou.py
  COMMAND ${CMAKE_COMMAND} 
          -D INFILE=${CMAKE_CURRENT_SOURCE_DIR}/test_verrou.py
          -D OUTFILE=${CMAKE_CURRENT_BINARY_DIR}/test_verrou.py
          -D FLAG_TESTS_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
          -D EXECUTABLE=$<TARGET_FILE:test_verrou_executable>
          -D FLAG_TESTS_PINTOOL=$<TARGET_FILE:pene>
          -D PIN_EXECUTABLE=$<TARGET_FILE:pin>
          -P ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
  DEPENDS
    ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
  COMMENT "Configuring test_verrou.py"
  VERBATIM
)

set(TEST_SCRIPTS test_verrou.py)




function(copy_file FILENAME)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
                ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
		
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

    COMMENT "Copying stderr2stdout.py"
    VERBATIM
)


list(TRANSFORM TEST_SCRIPTS PREPEND ${CMAKE_CURRENT_BINARY_DIR}/)
add_custom_target(${PROJECT_NAME} ALL DEPENDS 
    ${TEST_SCRIPTS}
    ${CMAKE_CURRENT_BINARY_DIR}/stderr2stdout.py
)

find_package(Python REQUIRED Interpreter)
foreach(script ${TEST_SCRIPTS})
	add_test(NAME ${script}
			COMMAND ${Python_EXECUTABLE} stderr2stdout.py ${Python_EXECUTABLE} -m pytest ${script})
endforeach()
