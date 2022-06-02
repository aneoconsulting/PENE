cmake_minimum_required(VERSION 3.14)

###### Common definitions ######

# In this section we place all the generic flags. Later sections provide
# additional flags depending on architecutre, compiler etc.

## General flags

# If we are testing Pin, add some internal checks. Don't do this by default
# since it may incur a performance penatly.
set(PIN_TESTFLAGS "")
if ("${PIN_TESTING}" STREQUAL "1")
    list(APPEND PIN_TESTFLAGS "-slow_asserts")
endif()

## Special compilation/linkage directives

set(NO_ASLR "")
set(NO_STDLIBS "/NODEFAULTLIB")
set(EXPORT "/EXPORT:")


## Include paths

# COMPONENT_INCLUDES will be defined later in the directory structure section.
set(TOOL_INCLUDES "")

## Library paths

set(TOOL_LPATHS "")
set(SATOOL_LPATHS "")

## Libraries to link
set(TOOL_CRT_LIBS pincrt.lib)
set(TOOL_STATIC_CRT_LIBS stlport-static.lib m-static.lib c-static.lib os-apis.lib)


set(TOOL_LIBS pin.lib xed.lib pinvm.lib ${TOOL_CRT_LIBS} ntdll-${BITS}.lib kernel32.lib)
string(REPLACE "os-apis.lib" "os-apis-kernel32.lib" SATOOL_LIBS ${TOOL_STATIC_CRT_LIBS})
set(SATOOL_LIBS ${NO_STDLIBS} ${SATOOL_LIBS} ntdll-${BITS}.lib kernel32.lib)

## Compiler flags

# Throughout this file, we only fill in the NOOPT versions of these variables.
# At the bottom of the file, the full versions are built from the NOOPT and
# the relevant optimization and debug flags (see below).
# e.g. APP_CXXFLAGS := $(APP_CXXFLAGS_NOOPT) $(APP_OPT_CXX) $(DBG_INFO_CXX)

# /EHs- /EHa-	Disable exception handling in the tool, otherwise it could interfere in
#				the apllication's exception handling.
# /wd4530              Turn off the warning about not having /EHsc turned on
set(TOOL_CXXFLAGS_NOOPT /MD /EHs- /EHa- /wd4640 /wd4530 /wd4505 /wd5045 /wd4266 
                        /wd4626 /wd4625 /wd4820 /wd4191 /wd4577 /wd5026 /wd5027 
                        /wd4435 /wd4061 /wd4514 /wd4555 /DTARGET_WINDOWS /nologo /Gy)
set(DLL_CXXFLAGS "")
set(ENABLE_DEPRECATED /DPIN_DEPRECATED_WARNINGS=0)


set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} /GR- /GS- /EHs- /EHa- /fp:strict /Oi- /D__PIN__=1 /DPIN_CRT=1 /DXED_DLL /Wall )
if ("${TARGET}" STREQUAL "ia32")
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} /D__i386__)
else()
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} /D__LP64__)
endif()

set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} /D_STLP_IMPORT_IOSTREAMS)

# no additional flags are required to support Restricted Transactional Memory on MSVC
set(RTM_FLAGS "")

## Linker flags

# Throughout this file, we only fill in the NOOPT versions of these variables.
# At the bottom of the file, the full versions are built from the NOOPT and
# the relevant optimization and debug flags (see below).
# e.g. TOOL_LDFLAGS := $(TOOL_LDFLAGS_NOOPT) $(TOOL_OPT_LD) $(DBG_INFO_LD)

# /EXPORT:main	Main function of the tool has to be exported.
# /NODEFAULTLIB	Do not link and use any system library except for those listed explicitly
#				to avoid conflicts with an application using the same library.
set(TOOL_LDFLAGS_NOOPT /DLL /EXPORT:main ${NO_STDLIBS} /NOLOGO /INCREMENTAL:NO /IGNORE:4210 /IGNORE:4049 /IGNORE:4281)
# Assume options /DYNAMICBASE and /NXCOMPAT are accepted by all supported Windows linkers.
set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} /DYNAMICBASE /NXCOMPAT)
set(SATOOL_LDFLAGS_NOOPT "")
set(DLL_LDFLAGS /DLL)

###### Debugging and optimizations ######

## Utility variables "internal" to this file

# If debug mode, no optimizations will be applied and debug
# information will be generated. Otherwise, full optimization will be
# performed and debug information will not be generated.
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(TOOL_OPT_CXX /Od)
    set(TOOL_OPT_LD "")
    # Note: DBG_INFO_CXX is intentionally defined with deferred expansion.
    set(DBG_INFO_CXX /Z7 /FC )
    set(DBG_INFO_LD /DEBUG:FULL)
else()
    set(TOOL_OPT_CXX /O2 /GL /Gw)
    set(TOOL_OPT_LD /LTCG /OPT:REF)
    set(DBG_INFO_CXX /Z7 /Wall)
    set(DBG_INFO_LD /DEBUG:FULL)
endif()


# The following variables do not depend on the user-defined DEBUG
# variable. When they are used, debug information will always be
# generated.
# Note: DBG_INFO_CXX_ALWAYS is intentionally defined with deferred expansion.
set(DBG_INFO_CXX_ALWAYS /Z7 /Fd)
set(DBG_INFO_LD_ALWAYS /DEBUG)

###### Additional flags ######

set(XED_ROOT ${PIN_ROOT}/extras/xed-${TARGET})
set(TOOLS_ROOT ${PIN_ROOT}/source/tools)
set(COMPONENT_INCLUDES ${PIN_ROOT}/extras/components/include)

set(TOOL_INCLUDES ${PIN_ROOT}/source/include/pin 
                  ${PIN_ROOT}/source/include/pin/gen 
                  ${PIN_ROOT}/extras/stlport/include 
                  ${PIN_ROOT}/extras 
                  ${PIN_ROOT}/extras/libstdc++/include 
                  ${PIN_ROOT}/extras/crt/include 
                  ${PIN_ROOT}/extras/crt 
                  ${PIN_ROOT}/extras/crt/include/arch-${BIONIC_ARCH} 
                  ${PIN_ROOT}/extras/crt/include/kernel/uapi 
                  ${PIN_ROOT}/extras/crt/include/kernel/uapi/asm-x86
                  ${COMPONENT_INCLUDES} ${XED_ROOT}/include/xed ${TOOLS_ROOT}/Utils ${TOOLS_ROOT}/InstLib)
                         
set(TOOL_LPATHS ${PIN_ROOT}/${TARGET}/lib ${PIN_ROOT}/${TARGET}/lib-ext ${PIN_ROOT}/${TARGET}/runtime/pincrt ${XED_ROOT}/lib)
set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} ${PIN_ROOT}/${TARGET}/runtime/pincrt/crtbeginS.obj)

set(SATOOL_LPATHS ${PIN_ROOT}/${TARGET}/lib ${PIN_ROOT}/${TARGET}/lib-ext ${PIN_ROOT}/${TARGET}/runtime/pincrt ${XED_ROOT}/lib)
set(SATOOL_LDFLAGS_NOOPT ${SATOOL_LDFLAGS_NOOPT} /IGNORE:4049 ${PIN_ROOT}/${TARGET}/runtime/pincrt/crtbegin.obj)
        
set(PIN ${PIN_ROOT}/pin)
set(PINBIN ${PIN_ROOT}/${TARGET}/bin/pin${EXE_SUFFIX})
set(PIN32 ${PIN_ROOT}/ia32/bin/pin${EXE_SUFFIX})
set(PIN64 ${PIN_ROOT}/intel64/bin/pin${EXE_SUFFIX})

###### Additional flags depending on the compiler ######
# TODO handle Intel Compiler
#ifeq ($(ICC),1)
#    CC := icl
#    CXX := icl
#    LINKER = xilink
#    ifneq ($(CCOV),)
#        TOOL_LIBS += libmmt.lib libipgo.lib libirc.lib svml_disp.lib
#    endif
#endif

if (${MSVC})
	if (${MSVC_TOOLSET_VERSION} VERSION_GREATER_EQUAL "140")
        # Suppress thread-safe local static initialization feature of C++ 11 using VS2015/2017/2019 compilers
        set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} /Zc:threadSafeInit- /Zc:sizedDealloc-)
	endif()
	if (${MSVC_TOOLSET_VERSION} VERSION_GREATER_EQUAL "141")
        set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} /experimental:external /external:W0 /external:anglebrackets /external:I${PIN_ROOT})
	endif()
endif()

###### Additional flags depending on architecture ######

if ("${TARGET}" STREQUAL "ia32")
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} /DTARGET_IA32 /DHOST_IA32)
    set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} /MACHINE:x86 /ENTRY:Ptrace_DllMainCRTStartup@12 /BASE:0x55000000)
    set(SSE2 /arch:SSE2)
    if ("${ICC}" STREQUAL "1")
        set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} /arch:IA32)
    endif()
endif()

if ("${TARGET}" STREQUAL "intel64")
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} /DTARGET_IA32E /DHOST_IA32E)
    set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} /MACHINE:x64 /ENTRY:Ptrace_DllMainCRTStartup /BASE:0xC5000000)
    set(NO_ASLR /HIGHENTROPYVA:NO)
endif()

###### Finalize flags ######

## Compiler and linker flags

# Add the include paths to the compilation flags.
set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT})

# Define the versions containing the full options
set(TOOL_CXXFLAGS /FIinclude/msvc_compat.h ${TOOL_CXXFLAGS_NOOPT} ${TOOL_OPT_CXX} ${DBG_INFO_CXX})
string(REPLACE "/GR-" "" TOOL_CFLAGS ${TOOL_CXXFLAGS})
set(TOOL_LDFLAGS ${TOOL_LDFLAGS_NOOPT} ${TOOL_OPT_LD} ${DBG_INFO_LD})
set(SATOOL_LDFLAGS ${SATOOL_LDFLAGS_NOOPT} ${TOOL_OPT_LD} ${DBG_INFO_LD})
set(EXPORT_MAIN /EXPORT:main)
string(REPLACE "${EXPORT_MAIN}" "" TOOL_LIBRARY_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT})

# Add debugging flags to the NOOPT versions and disable optimizations
set(TOOL_CXXFLAGS_NOOPT /NOSTDLIB /FIinclude/msvc_compat.h ${TOOL_CXXFLAGS_NOOPT} /Od ${DBG_INFO_CXX})
set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} /OPT:NOREF ${DBG_INFO_LD})

## Pin program finalization:

# Add any additional flags that the user specified to the Pin command line.
set(PIN_EXE "${PINBIN} ${PIN_TESTFLAGS} ${PINFLAGS}")

# Extracts the major and minor Windows version ID and build number (e.g. for XP 64 bit it's 5.2.3790 --> 5, 2, 3790)
string(REPLACE "." ";" VERSION_LIST ${CMAKE_SYSTEM_VERSION})
list(GET VERSION_LIST 0 WIN_VER_MAJOR)
list(GET VERSION_LIST 1 WIN_VER_MINOR)
list(GET VERSION_LIST 2 WIN_BUILD_NUMBER)


