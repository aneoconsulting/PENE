cmake_minimum_required(VERSION 3.14)

include(CommonVars)

###### PIN URL ######

set(PIN_URL ${PIN_URL_LINUX})
set(PIN_TOOLKIT ${PIN_TOOLKIT_LINUX})

###### Operating system ###### 

# Identify the operating system
if (NOT "${TARGET_OS}" STREQUAL "Linux")
    set(TARGET_OS "Linux")
endif()

###### Program definitions ######

# COMPILER may have already been defined during the build so don't override it with the hardcoded defaults below.
#if (ICCPIN)
#    if (NOT COMPILER)
#        set(COMPILER icc)
#    endif()
#endif
if (NOT COMPILER)
    set(COMPILER gcc)
endif()

#ifeq ($(ICC),1)
#    # This makefile assumes that the iccvars script has been run. So if the user didn't
#    # specify a full path to CC and CXX, we override them now and let the PATH set by the
#    # iccvars script to find the appropriate compiler.
#    ifeq ($(origin CC), default)
#        CC := icc
#        CXX := icpc
#    endif
#endif

set(EXE_SUFFIX "")
set(DLL_SUFFIX ".so")

###### Function pre/suffixes ######

if ("${TARGET_OS}" STREQUAL "mac")
    # The function naming convention on mac is to have all public functions start with '_'
    set(GLOBALFUN_PREFIX "_")
else()
    set(GLOBALFUN_PREFIX "")
endif()

# Set destination directory for PIN tools. 
set(PIN_TOOL_INSTALL_DIR ${PIN_TOOLS_INSTALL_PREFIX}/lib)
