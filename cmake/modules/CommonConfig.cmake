set(TARGET_OS "${CMAKE_SYSTEM_NAME}")

###### Architecture ######

# Identify the architecture of the host
if (NOT HOST_ARCH)
    if ("${PROCESSOR_ARCHITEW6432}" STREQUAL "AMD64")
        set(HOST_ARCH "intel64")
    else()
        if ("${CMAKE_HOST_SYSTEM_PROCESSOR}" MATCHES "(x86_64)|(AMD64)")
            set(HOST_ARCH "intel64")
        elseif ("${CMAKE_HOST_SYSTEM_PROCESSOR}" MATCHES "(x86)|(i386)|(i686)")
            set(HOST_ARCH "ia32")
        endif()
    endif()
endif()

# Verify known host
if (NOT HOST_ARCH)
    message(FATAL_ERROR "Could not detect the host architecture. Please define HOST_ARCH from the command line")
endif()

# Define the architecture of the target
set(TARGET "${HOST_ARCH}")
if ("${TARGET}" STREQUAL "ia32")
    set(BITS "32")
else()
    set(BITS "64")
endif()

set(BIONIC_ARCH "x86")
if (NOT "${TARGET}" STREQUAL "ia32")
    set(BIONIC_ARCH "x86_64")
endif()

set(PIN_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/Pin)
set(PIN_ROOT ${PIN_INSTALL_PREFIX}/${CMAKE_SYSTEM_NAME})
set(PIN_TOOLS_INSTALL_PREFIX ${PIN_INSTALL_PREFIX}/Install)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include(WindowsVars)
    include(WindowsConfig)
else()
    include(UnixVars)
    include(UnixConfig)
endif()

