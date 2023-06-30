
project(counters_replace_avx)

add_executable(test_replace_avx_executable "replace.cpp")
set_property(TARGET test_replace_avx_executable PROPERTY CXX_STANDARD 17)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("/arch:AVX" COMPILER_SUPPORTS_ARCH_AVX)
if(COMPILER_SUPPORTS_ARCH_AVX)
  message("/arch:AVX is supported by the compiler")
  target_compile_options(test_replace_avx_executable PUBLIC "/arch:AVX")
else()
  CHECK_CXX_COMPILER_FLAG("-mavx" COMPILER_SUPPORTS_MAVX)
  if(COMPILER_SUPPORTS_MAVX)
    message("-mavx is supported by the compiler")
    target_compile_options(test_replace_avx_executable PUBLIC "-mavx")
   endif()
endif()



add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_replace_avx.py
  COMMAND ${CMAKE_COMMAND} 
          -D INFILE=${CMAKE_CURRENT_SOURCE_DIR}/test_replace_avx.py
          -D OUTFILE=${CMAKE_CURRENT_BINARY_DIR}/test_replace_avx.py
          -D REPLACE_TESTS_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
          -D REPLACE_TESTS_EXECUTABLE=$<TARGET_FILE:test_replace_avx_executable>
          -D REPLACE_TESTS_PINTOOL=$<TARGET_FILE:pene>
          -D PIN_EXECUTABLE=$<TARGET_FILE:pin>
          -P ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
  DEPENDS
    ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
    #${CMAKE_CURRENT_SOURCE_DIR}/test_replace_avx.py
  COMMENT "Configuring test_replace_avx.py"
  VERBATIM
)

set(TEST_SCRIPTS test_replace_avx.py)
set(TEST_LIST test_replace_avx.replace_tests.test_replace test_replace_avx.replace_tests.test_replace_inactive)

function(copy_file FILENAME)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
                ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
        #DEPENDS
            #${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
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

find_package(Python3 REQUIRED Interpreter)
foreach(unittest ${TEST_LIST})
	add_test(NAME ${unittest}
			COMMAND ${Python3_EXECUTABLE} stderr2stdout.py ${Python3_EXECUTABLE} -m unittest ${unittest})
endforeach()
