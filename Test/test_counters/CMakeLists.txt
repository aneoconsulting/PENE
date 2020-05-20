cmake_minimum_required (VERSION 3.14)

project(counters_module)

add_executable(test_counters_executable "test_counters_executable.cpp")
set_property(TARGET test_counters_executable PROPERTY CXX_STANDARD 17)

set(TEST_SCRIPTS test_flags.py testCounterGenerator.py 
    test_trace_counters.py test_trace_counters_tls.py 
    test_cvt_counters.py)

set(TEST_LIST test_flags.instruction_count_flag_tests.test_disable_instruction_count 
              test_flags.instruction_count_flag_tests.wrong_instruction_count_flag_tests
              )

foreach(precision float double)
    foreach(operation add sub mul div fma mix)
        foreach(mode scalar simd)
            list(APPEND TEST_LIST test_trace_counters.${precision}_${operation}_${mode}_tests.test_result 
                                  test_trace_counters.${precision}_${operation}_${mode}_tests.test_count
                                  test_trace_counters_tls.${precision}_${operation}_${mode}_tests.test_result 
                                  test_trace_counters_tls.${precision}_${operation}_${mode}_tests.test_count
                                  )
        endforeach()
    endforeach()
endforeach()

foreach(precision i2d i2f f2i f2d d2i d2f)
    foreach(operation cvt)
        foreach(mode scalar simd)
            list(APPEND TEST_LIST test_cvt_counters.${precision}_${operation}_${mode}_tests.test_count)
        endforeach()
    endforeach()
endforeach()

               
function(configure_file FILENAME)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
      COMMAND ${CMAKE_COMMAND} 
              -D INFILE=${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
              -D OUTFILE=${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
              -D COUNTERS_TESTS_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
              -D COUNTERS_TESTS_ECEXUTABLE=$<TARGET_FILE:test_counters_executable>
              -D COUNTERS_TESTS_PINTOOL=$<TARGET_FILE:pene>
              -D PIN_EXECUTABLE=$<TARGET_FILE:pin>
              -D EXECUTABLE=$<TARGET_FILE:test_counters_executable>
              -D PINTOOL=$<TARGET_FILE:pene>
              -D PIN=$<TARGET_FILE:pin>
              -P ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
      DEPENDS
        ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
      COMMENT "Configuring FILENAME"
      VERBATIM)
endfunction(configure_file)

foreach(test_script ${TEST_SCRIPTS})
    configure_file(${test_script})
endforeach()


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
list(SORT TEST_LIST)
foreach(unittest ${TEST_LIST})
	add_test(NAME ${unittest}
			COMMAND ${Python_EXECUTABLE} stderr2stdout.py ${Python_EXECUTABLE} -m unittest ${unittest})
    set_property(TEST ${unittest} PROPERTY REQUIRED_FILES 
        ${TEST_SCRIPTS}
        ${REFERENCE_FILES}
        ${CMAKE_CURRENT_BINARY_DIR}/stderr2stdout.py
        )    
    set_property(TEST ${unittest} PROPERTY NAME test_counters)
endforeach()
	