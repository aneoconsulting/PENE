cmake_minimum_required(VERSION 3.14)

###### Common definitions ######

# In this section we place all the generic flags. Later sections provide
# additional flags depending on architecutre, compiler etc.

## General flags

## Output control

## Special compilation/linkage directives

if ("${TARGET_OS}" STREQUAL "mac")
    set(STATIC "")
else()
    set(STATIC -static)
endif()
set(NO_RANDOM "")
set(PIC -fPIC)
# There's no concept of compiling a file to be specifically PIE in windows.
set(NO_PIC -fno-PIC -fno-pic)
set(PIE -pie)
set(SSE2 -msse2)
set(FP387 -mfpmath=387 -mno-sse)
set(ASMCPP -x assembler-with-cpp)
set(NO_STDLIBS -nostartfiles -nodefaultlibs -nostdlib)
if ("${TARGET_OS}" STREQUAL "mac")
    set(EXPORT_ALL -Wl,-export_dynamic)
    set(EXPORT -Wl,-exported_symbol,${GLOBALFUN_PREFIX})
else()
    set(EXPORT_ALL -Wl,--export-dynamic)
    # The following is essentially a no-op for compatibility. It has no effect on the resulting linked file.
    set(EXPORT -Wl,--trace-symbol=)
endif()

# Flags to suppress errors when the linker has trouble solving relocations but they are still valid.
set(SUPPRESS_RELOCS "")


## Include paths

# COMPONENT_INCLUDES will be defined later in the directory structure section.
# APP_INCLUDES and TOOL_INCLUDES are internal utilities for this file.
set(TOOL_INCLUDES "")
set(TOOL_CRT_INCLUDES "")

## Library paths

set(TOOL_LPATHS "")
set(SATOOL_LPATHS "")
set(CXX_LPATHS "")

## Libraries to link

if ("${ICC}" STREQUAL "1")
    set(LIBPIN_SUFFIX -icc)
else()
    set(LIBPIN_SUFFIX "")
endif()
set(DL_LIB )
set(TOOL_LIBS libpin${LIBPIN_SUFFIX}.a libxed.so)
set(SATOOL_LIBS libsapin${LIBPIN_SUFFIX}.a libxed.so)
set(CXX_LIBS "")

## Compiler flags

# Throughout this file, we only fill in the NOOPT versions of these variables.
# At the bottom of the file, the full versions are built from the NOOPT and
# the relevant optimization and debug flags (see below).
# e.g. APP_CXXFLAGS := $(APP_CXXFLAGS_NOOPT) $(APP_OPT_CXX) $(DBG_INFO_CXX)

# On Unix, ASM_FLAGS is identical to APP_CXXFLAGS, therefore it is built at
# the end of this file, same as APP_CXXFLAGS.

set(TOOL_CXXFLAGS_NOOPT -Wall -Wno-unknown-pragmas -D__PIN__=1 -DPIN_CRT=1 -DXED_DLL -D_STLP_IMPORT_IOSTREAMS) # TODO Is it required to add -Werror ??
set(DLL_CXXFLAGS "")
set(ENABLE_DEPRECATED -DPIN_DEPRECATED_WARNINGS=0)
set(CPP11FLAGS -std=c++11)
set(CPP11LIBS -Wl,--no-as-needed -lpthread)
set(RTM_FLAGS -mrtm)

## Linker flags

# Throughout this file, we only fill in the NOOPT versions of these variables.
# At the bottom of the file, the full versions are built from the NOOPT and
# the relevant optimization and debug flags (see below).
# e.g. TOOL_LDFLAGS := $(TOOL_LDFLAGS_NOOPT) $(TOOL_OPT_LD) $(DBG_INFO_LD)

set(TOOL_LDFLAGS_NOOPT -shared)
set(SATOOL_LDFLAGS_NOOPT "")
set(DLL_LDFLAGS -shared)


###### Debugging and optimizations ######

## Utility variables "internal" to this file

# The following variables depend on the user-defined DEBUG variable.
# If DEBUG=1 was specified, no optimizations will be applied and debug
# information will be generated. Otherwise, full optimization will be
# performed and debug information will not be generated.

# On Unix platforms we don't apply any link-time optimizations. The
# variables are defined here for readability.

# On Unix platforms the debug info flags for the compiler and linker
# are the same, we support two different variables for compatibility
# with Windows. This will incur two instances of the "-g" flag when
# compiling most applications with debug information. It's ugly but
# it doesn't do any harm.

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(TOOL_OPT_CXX -O0)
    set(TOOL_OPT_LD "")
    set(DBG_INFO_CXX -g)
    set(DBG_INFO_LD -g)
else()
    set(TOOL_OPT_CXX -O3 -fomit-frame-pointer -fno-strict-aliasing)
    set(TOOL_OPT_LD "")
    set(DBG_INFO_CXX "")
    set(DBG_INFO_LD "")
endif()

## Debugging flags to be used in any makefile.rules file

# The following variables do not depend on the user-defined DEBUG
# variable. When they are used, debug information will always be
# generated.

set(DBG_INFO_CXX_ALWAYS -g)
set(DBG_INFO_LD_ALWAYS -g)

# This allows us to acquire information about the compiler
execute_process(COMMAND         ${CMAKE_CXX_COMPILER} -v --help
                OUTPUT_VARIABLE HELPOUT_OUT
                ERROR_VARIABLE  HELPOUT_ERR)
set(HELPOUT "${HELPOUT_ERR} ${HELPOUT_OUT}")

# GLIBC version 2.4 implements the function __stack_chk_fail used by new GCC
# versions when stack-protector is on. Therefore, disable this option (if supported)
if ("${HELPOUT}" MATCHES "stack-protector")
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} -fno-stack-protector)
endif()

if (("${HELPOUT}" MATCHES "-fexceptions") OR ("${HELPOUT}" MATCHES "-f[no-]exceptions"))
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} -fno-exceptions)
endif()

if ("${HELPOUT}" MATCHES "(-funwind-tables)|(Clang)")
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} -funwind-tables)
endif()

if ("${HELPOUT}" MATCHES "-fasynchronous-unwind-tables")
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} -fasynchronous-unwind-tables)
endif()

if ("${HELPOUT}" MATCHES "(-fno-rtti)|(-frtti)")
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} -fno-rtti)
endif()

if ("${HELPOUT}" MATCHES "Clang")
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} -Wtypedef-redefinition)
endif()

# Pin-probe runtime doesn't support the new GNU_HASH style
# First check if the linker used to build the tools support the flag --hash-style.
# In this case set the hash-style to be the old (SYSV) style
if ("${HELPOUT}" MATCHES "--hash-style")
    set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} -Wl,--hash-style=sysv)
    set(SATOOL_LDFLAGS_NOOPT ${SATOOL_LDFLAGS_NOOPT} -Wl,--hash-style=sysv)
endif()

if ("${ICC}" STREQUAL "1")
    # ICC requires some system utilites, this directs it to the right ones.
    # Also, override CXXPATH with the full path to g++. This is correct because the CXXPATH variable
    # is used to determine whether or not to use Pin's cpp libraries (e.g. libstdc++) which depend
    # on the g++ version, not the icpc version.
    if (NOT GCCVER)
        set(ICC_CXXFLAGS -gcc-name=/usr/bin/gcc -gxx-name=/usr/bin/g++)
        set(ICC_LDFLAGS -Qlocation,ld,/usr/bin -gcc-name=/usr/bin/gcc -gxx-name=/usr/bin/g++)
        set(CXXPATH /usr/bin/g++)
    else()
	    set(ICC_CXXFLAGS -gcc-name=/usr/intel/pkgs/gcc/${GCCVER}/bin/gcc -gxx-name=/usr/intel/pkgs/gcc/${GCCVER}/bin/g++)
	    set(ICC_LDFLAGS -Wl,-rpath=/usr/intel/pkgs/gcc/${GCCVER}/lib -Qlocation,gld,/usr/intel/pkgs/gcc/${GCCVER}/bin)
        set(CXXPATH /usr/intel/pkgs/gcc/${GCCVER}/bin/g++)
    endif()

    # Enable ICC optimizations
    # ICC splits the called function into 2 different funcs - the actual func that using nonconventional
    # calling standard (args passed in regs), and a func which handle standard calling convention (pass
    # args to regs). Pin is trying to change the last func. To avoid this we disable inter-procedural
    # optimizations. Maybe in ICC 12 we could use -opt-args-in-reg=none
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} -inline-level=1 -no-ip ${ICC_CXXFLAGS} -fno-builtin)
   
    # Add ICC link flags to all linkage flags
    set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} ${ICC_LDFLAGS})

    # Disable warnings
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} -wd1418 -wd1419 -wd981 -wd383 -wd869 -wd593 -wd266 -wd279 -wd444 -wd168 -wd810 -wd810 -wd181 -wd1195 -wd168 -wd193 -wd584)

    if (${CCOV} STREQUAL "1")
        # code coverage is on
        if (${ICCDIR} MATCHES "cc/10.")
            # icc version >= 11 
            set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} -prof-gen=srcpos)
        else()
            # icc version 10
            set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} -prof-genx)
        endif()
        if (NOT CCOVDIR)
            set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} -prof-dir ${CCOVDIR})
        endif()
    endif()
    set(CRT_INC_DIR -I)
else()
    set(CRT_INC_DIR -isystem)
endif()


###### Additional flags depending on directory structure ######

# In the kit tree, the default Pin root is the kit root.
# However, don't overwrite a user-defined PIN_ROOT definition (if it exists)

set(XED_ROOT ${PIN_ROOT}/extras/xed-${TARGET})
set(TOOLS_ROOT ${PIN_ROOT}/source/tools)
set(COMPONENT_INCLUDES ${PIN_ROOT}/extras/components/include)

set(TOOL_INCLUDES       ${TOOL_INCLUDES} 
                        ${COMPONENT_INCLUDES}
                        ${XED_ROOT}/include/xed ${TOOLS_ROOT}/Utils 
                        ${TOOLS_ROOT}/InstLib
                        ${PIN_ROOT}/source/include/pin
                        ${PIN_ROOT}/source/include/pin/gen)

set(TOOL_CRT_INCLUDES   ${TOOL_CRT_INCLUDES}
                        ${PIN_ROOT}/extras/stlport/include 
                        ${PIN_ROOT}/extras/libstdc++/include 
                        ${PIN_ROOT}/extras/crt/include 
                        ${PIN_ROOT}/extras/crt/include/arch-${BIONIC_ARCH} 
                        ${PIN_ROOT}/extras/crt/include/kernel/uapi 
                        ${PIN_ROOT}/extras/crt/include/kernel/uapi/asm-x86)

set(TOOL_LPATHS         ${TOOL_LPATHS} 
                        ${XED_ROOT}/lib
                        ${PIN_ROOT}/${TARGET}/runtime/pincrt
                        ${PIN_ROOT}/${TARGET}/lib
                        ${PIN_ROOT}/${TARGET}/lib-ext
                        ${PIN_ROOT}/extras/xed-${TARGET}/lib)
set(SATOOL_LPATHS       ${SATOOL_LPATHS} 
                        ${XED_ROOT}/lib
                        ${PIN_ROOT}/${TARGET}/runtime/pincrt
                        ${PIN_ROOT}/${TARGET}/lib
                        ${PIN_ROOT}/${TARGET}/lib-ext)

set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} ${PIN_ROOT}/${TARGET}/runtime/pincrt/crtbeginS.o)
set(SATOOL_LDFLAGS_NOOPT ${SATOOL_LDFLAGS_NOOPT} ${PIN_ROOT}/${TARGET}/runtime/pincrt/crtbegin.o)
if (${TARGET_OS} STREQUAL "Linux")
    set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} ${PIN_ROOT}/${TARGET}/runtime/pincrt/crtendS.o)
    set(SATOOL_LDFLAGS_NOOPT ${SATOOL_LDFLAGS_NOOPT} ${PIN_ROOT}/${TARGET}/runtime/pincrt/crtend.o)
    set(SATOOL_LDFLAGS_NOOPT ${SATOOL_LDFLAGS_NOOPT} -Wl,-pie)
else()
    if (${TARGET_OS} STREQUAL "mac")
        set(SATOOL_LDFLAGS_NOOPT ${SATOOL_LDFLAGS_NOOPT} -Wl,-no_new_main)
    endif()
endif()
    
# If Pin was compiled with icc, tools need to be linked with the libraries below.
if (${COMPILER} STREQUAL "icc")
    set(TOOL_LIBS ${TOOL_LIBS} libimf.a libintlc.a libirng.a libsvml.a)
    set(SATOOL_LIBS ${SATOOL_LIBS} libimf.a libintlc.a libirng.a libsvml.a)
endif()

set(PIN ${PIN_ROOT}/pin)
set(PINBIN ${PIN_ROOT}/${TARGET}/bin/pinbin)
set(PIN32 ${PIN_ROOT}/ia32/bin/pinbin)
set(PIN64 ${PIN_ROOT}/intel64/bin/pinbin)
    
set(VSCRIPT_DIR ${PIN_ROOT}/source/include/pin)
    
    
set(ADDITIONAL_CRT_DLL_PATH ${ADDITIONAL_CRT_DLL_PATH} ${PIN_ROOT}/${TARGET}/runtime/pincrt)
# TODO fix these additional variables for building a SA tool
#if (${TARGET_OS} STREQUAL "mac")
#    # macOS* has a different variable for notifying the loader where to look for libraries
#    set(LOADER_LIBRARY_PATH DYLD_LIBRARY_PATH)
#    set(SET_DLL_PATH := export $(LOADER_LIBRARY_PATH)=$(OBJDIR):${PIN_ROOT}/${TARGET}/lib-ext:${PIN_ROOT}/extras/xed-${TARGET}/lib:$(ADDITIONAL_CRT_DLL_PATH):.:$$$(LOADER_LIBRARY_PATH);
#else()
#    # Command to set the search path for libraries required by a stand-alone tool.
#    # Note that some OSs don't look for SOs in the current directory automatically so we need to add "."
#    # to the search path.
#    set(LOADER_LIBRARY_PATH LD_LIBRARY_PATH)
#    SET_DLL_PATH := export $(LOADER_LIBRARY_PATH)=$(OBJDIR):${PIN_ROOT}/${TARGET}/lib-ext:${PIN_ROOT}/extras/xed-${TARGET}/lib:$(ADDITIONAL_CRT_DLL_PATH):.:$$$(LOADER_LIBRARY_PATH);
#endif()



set(DWARF_LIBS ${DWARF_LIBS} libpin3dwarf.so)

###### Additional flags depending on architecture ######

if (${TARGET} STREQUAL "ia32")
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} -DTARGET_IA32 -DHOST_IA32)
    
    # cross compilation
    if (${HOST_ARCH} STREQUAL "intel64")
        set(COMP_OBJ ${COMP_OBJ} -m32)
        set(COMP_EXE ${COMP_EXE} -m32)
        set(LINK_EXE ${LINK_EXE} -m32)
    endif()
endif()

if (${TARGET} STREQUAL "intel64")
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} -DTARGET_IA32E -DHOST_IA32E -fPIC)
    set(DLL_CXXFLAGS ${DLL_CXXFLAGS} -fPIC)
endif()

###### Additional flags depending on OS ######

if (${TARGET_OS} STREQUAL "Linux")
    # TODO fix detection of PIE flag
    #set(NO_PIE -no-pie)
    #if ($(shell $(CC) -v 2>&1 | $(QGREP) " --enable-default-pie " && echo default-pie),default-pie)
    #    LDFLAGS_DEFAULT_NO_PIE := $(NO_PIE)
    #endif()
    set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} -DTARGET_LINUX -fabi-version=2)
    set(DL_LIB ${DL_LIB} libdl-dynamic.so)
    set(TOOL_LIBS ${TOOL_LIBS} ${DWARF_LIBS} ${DL_LIB})
    set(SATOOL_LIBS ${SATOOL_LIBS} ${DWARF_LIBS} ${DL_LIB})
    set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} -nostdlib -Wl,-Bsymbolic -Wl,--version-script=${VSCRIPT_DIR}/pintool.ver -fabi-version=2)
    set(SATOOL_LDFLAGS_NOOPT ${SATOOL_LDFLAGS_NOOPT} -nostdlib -Wl,-Bsymbolic -fabi-version=2)
    set(DWARF4 ${DWARF4} -gdwarf-4)

    # Suppress warning -Waligned-new that warns  about a new-expression of a type that requires greater 
    # alignment than the alignof(std::max_align_t) but uses a standard new that is aligned for void*. 
    # This warrning should be suppressed for types that do not require an extended alignment.
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER 7.0)
        # GCC version >= 7.0
        set(SUPPRESS_WARNING_ALIGNED_NEW -Wno-aligned-new)
    endif()

    file(STRINGS /etc/os-release LINUX_DISTRIBUTION REGEX "^ID=")
    string(REGEX REPLACE "ID=\"(.*)\"" "\\1" LINUX_DISTRIBUTION "${LINUX_DISTRIBUTION}")

    file(STRINGS /etc/os-release LINUX_DISTRIBUTION_VERSION REGEX "^VERSION_ID=")
    string(REGEX REPLACE "VERSION_ID=\"(.*)\"" "\\1" LINUX_DISTRIBUTION_VERSION "${LINUX_DISTRIBUTION_VERSION}")

    # On Fedora version 28 and up, the RPC Interfaces have been removed from glibc.
    # An additional package is installed to provide the RPC interface.
    # The include path and library from the additional package are specified by the 
    # RPC_INCLUDES and RPC_LIBS variables
    set(RPC_INCLUDES "")
    set(RPC_LIBS "") 
    if (${LINUX_DISTRIBUTION} STREQUAL "fedora")
        if (${LINUX_DISTRIBUTION_VERSION} VERSION_GREATER 28)
            set(RPC_INCLUDES ${RPC_INCLUDES} /usr/include/tirpc)
            set(RPC_LIBS ${RPC_LIBS} libtirpc.so)
        endif()
    endif()
endif()

set(TOOL_LIBS ${TOOL_LIBS} libstlport-dynamic.so libm-dynamic.so libc-dynamic.so libunwind-dynamic.so)
set(SATOOL_LIBS ${SATOOL_LIBS} libstlport-dynamic.so libm-dynamic.so libc-dynamic.so libunwind-dynamic.so)

###### Finalize flags ######


# Define the versions containing the full options
set(TOOL_CXXFLAGS ${TOOL_CXXFLAGS_NOOPT} ${TOOL_OPT_CXX} ${DBG_INFO_CXX})

string(REPLACE "-fno-rtti" "" TOOL_CFLAGS ${TOOL_CXXFLAGS})
string(REPLACE "-fno-exception" "" TOOL_CFLAGS ${TOOL_CFLAGS})

set(TOOL_LDFLAGS ${TOOL_LDFLAGS_NOOPT} ${TOOL_OPT_LD} ${DBG_INFO_LD})
set(SATOOL_LDFLAGS ${SATOOL_LDFLAGS_NOOPT} ${TOOL_OPT_LD} ${DBG_INFO_LD})
if (${TARGET_OS} STREQUAL "Linux")
    set(VERSION_SCRIPT_PATTERN -Wl,--version-script=%)
    string(REPLACE "${VERSION_SCRIPT_PATTERN}" "" TOOL_LIBRARY_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT})
endif()


# Add debugging flags to the NOOPT versions and disable optimizations
set(TOOL_CXXFLAGS_NOOPT ${TOOL_CXXFLAGS_NOOPT} -O0 ${DBG_INFO_CXX})
set(TOOL_LDFLAGS_NOOPT ${TOOL_LDFLAGS_NOOPT} ${DBG_INFO_LD})

## Pin program finalization:

# Add any additional flags that the user specified to the Pin command line.
set(PIN_EXE "${PIN} ${PIN_TESTFLAGS} ${PINFLAGS}")
