cmake_minimum_required(VERSION 3.14)

include(CommonVars)

###### PIN URL ######

set(PIN_URL ${PIN_URL_WINDOWS})
set(PIN_TOOLKIT ${PIN_TOOLKIT_WINDOWS})

###### Operating system ###### 

# Identify the operating system
if (NOT "${TARGET_OS}" STREQUAL "Windows")
    set(TARGET_OS "Windows")
endif()

###### Compiler and linker definitions ######

# COMPILER may have already been defined during the build so don't override it with the hardcoded defaults below.
if (NOT ${COMPILER})
    set(COMPILER "MSVC")
endif()

set(EXE_SUFFIX ".exe")
set(DLL_SUFFIX ".dll")

# Set destination directory for PIN tools. 
# The default behavior of cmake on DLL platforms (Windows) is to install DLLs in <prefix>/bin.
# https://cmake.org/cmake/help/v3.8/command/install.html#id3
set(PIN_TOOL_INSTALL_DIR ${PIN_TOOLS_INSTALL_PREFIX}/bin)

