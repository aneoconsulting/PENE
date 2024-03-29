
project(test_backend)

add_executable(test_backend_executable1 "Apps/test_backend_executable1.cpp")
add_executable(test_backend_executable2 "Apps/test_backend_executable2.cpp")
add_executable(test_backend_executable3 "Apps/test_backend_executable3.cpp")

set_property(TARGET test_backend_executable1 test_backend_executable2 test_backend_executable3 PROPERTY CXX_STANDARD 17)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("/arch:AVX" COMPILER_SUPPORTS_ARCH_AVX)
if(COMPILER_SUPPORTS_ARCH_AVX)
  message("/arch:AVX is supported by the compiler")
  target_compile_options(test_backend_executable3 PUBLIC "/arch:AVX")
else()
  CHECK_CXX_COMPILER_FLAG("-mavx" COMPILER_SUPPORTS_MAVX)
  if(COMPILER_SUPPORTS_MAVX)
    message("-mavx is supported by the compiler")
    target_compile_options(test_backend_executable3 PUBLIC "-mavx")
   endif()
endif()


add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_backend.py
  COMMAND ${CMAKE_COMMAND} 
          -D INFILE=${CMAKE_CURRENT_SOURCE_DIR}/test_backend.py
          -D OUTFILE=${CMAKE_CURRENT_BINARY_DIR}/test_backend.py
          -D FLAG_TESTS_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
          -D EXECUTABLE1=$<TARGET_FILE:test_backend_executable1>
          -D EXECUTABLE2=$<TARGET_FILE:test_backend_executable2>
          -D EXECUTABLE3=$<TARGET_FILE:test_backend_executable3>
          -D FLAG_TESTS_PINTOOL=$<TARGET_FILE:pene>
          -D PIN_EXECUTABLE=$<TARGET_FILE:pin>
          -P ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
  DEPENDS
    ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
  COMMENT "Configuring test_backend.py"
  VERBATIM
)
set(TEST_SCRIPTS test_backend.py)




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

find_package(Python3 REQUIRED Interpreter)

execute_process(
    COMMAND pip show pytest
    RESULT_VARIABLE EXIT_CODE
    OUTPUT_QUIET
)

if (NOT ${EXIT_CODE} EQUAL 0)
    message(
            FATAL_ERROR
            "The \"pytest\" Python3 package is not installed. Please install it using the following command: \"pip3 install pytest\"."
    )
endif()

foreach(script ${TEST_SCRIPTS})
	add_test(NAME ${script}
			COMMAND ${Python3_EXECUTABLE} stderr2stdout.py ${Python3_EXECUTABLE} -m pytest ${script})
endforeach()

