cmake_minimum_required(VERSION 3.14)

include(FindPackageHandleStandardArgs)


set(PIN_INCLUDE_DIRS ${TOOL_INCLUDES})
set(PIN_CRT_INCLUDE_DIRS ${TOOL_CRT_INCLUDES})

##################################
# Set version strings
##################################
macro(_pin_check_version)
    find_file(PIN_VERSION_FILE "pin_version.h" PATHS ${PIN_INCLUDE_DIRS})
    file(READ ${PIN_VERSION_FILE} _pin_version_header)

    string(REGEX MATCH "define[ \t]+PIN_PRODUCT_VERSION_MAJOR[ \t]+([0-9]+)" _pin_major_version_match "${_pin_version_header}")
    set(PIN_MAJOR_VERSION "${CMAKE_MATCH_1}")
    string(REGEX MATCH "define[ \t]+PIN_PRODUCT_VERSION_MINOR[ \t]+([0-9]+)" _pin_minor_version_match "${_pin_version_header}")
    set(PIN_MINOR_VERSION "${CMAKE_MATCH_1}")
    string(REGEX MATCH "define[ \t]+PIN_BUILD_NUMBER[ \t]+([0-9]+)" _pin_build_number_match "${_pin_version_header}")
    set(PIN_BUILD_NUMBER "${CMAKE_MATCH_1}")

    set(PIN_VERSION ${PIN_MAJOR_VERSION}.${PIN_MINOR_VERSION}.${PIN_BUILD_NUMBER})
endmacro(_pin_check_version)

if(PIN_INCLUDE_DIRS)
    _pin_check_version()
endif()


##################################
  # Find PIN components
##################################

set(PIN_SEARCH_COMPOMPONENTS ${TOOL_LIBS})
foreach(_comp ${PIN_SEARCH_COMPOMPONENTS})
    # Search for the libraries

    set(_OPT "")
    if (NOT _comp MATCHES "kernel32")
        set(_OPT NO_DEFAULT_PATH)
    endif()

    find_library(PIN_${_comp}_LIBRARY_RELEASE ${_comp} PATHS ${TOOL_LPATHS} ${_OPT})
    if(PIN_${_comp}_LIBRARY_RELEASE)
        list(APPEND PIN_LIBRARIES_RELEASE "${PIN_${_comp}_LIBRARY_RELEASE}")
    endif()

    # PIN kit contains only release libraries
    set(PIN_LIBRARIES_DEBUG ${PIN_LIBRARIES_RELEASE})
    set(PIN_LIBRARIES_RELWITHDEBINFO ${PIN_LIBRARIES_RELEASE})
    set(PIN_LIBRARIES_MINISIZEREL ${PIN_LIBRARIES_RELEASE})

    set(PIN_LIBRARIES ${PIN_LIBRARIES_RELEASE})
endforeach()

set(PIN_CXXFLAGS ${TOOL_CXXFLAGS})
set(PIN_LDFLAGS ${TOOL_LDFLAGS})
set(PIN_LIBRARY_DIRS ${TOOL_LPATHS})

find_package_handle_standard_args(PIN 
                                  REQUIRED_VARS PIN_INCLUDE_DIRS PIN_LIBRARIES
                                  HANDLE_COMPONENTS
                                  VERSION_VAR PIN_VERSION)

if (PIN_VERSION)
    find_package_message(PIN
        "Found PIN: VERSION ${PIN_VERSION}
            PIN_INCLUDE_DIRS      = [${PIN_INCLUDE_DIRS}]
            PIN_LIBRARY_DIRS      = [${PIN_LIBRARY_DIRS}]
            PIN_LIBRARIES         = [${PIN_LIBRARIES}]
            PIN_CXXFLAGS          = [${PIN_CXXFLAGS}]
            PIN_LDFLAGS           = [${PIN_LDFLAGS}]
            PIN_EXE               = [${PIN_EXE}]"
          "[[${PIN_INCLUDE_DIRS}][${PIN_LIBRARY_DIRS}][${PIN_LIBRARIES}]${PIN_EXE}]")
else()
    message(FATAL_ERROR "Could not find PIN.")
endif()
