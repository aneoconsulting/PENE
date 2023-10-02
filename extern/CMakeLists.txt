project("Interflop")

# fmaqApprox.c was removed because of quad implementation depending on compiler runtime.
# PinCRT doesn't support this part of GCC C library
set (INTERFLOP_STDLIB_SRC
    # FMA
    "interflop/fma/fmaqApproxM.c"
    "interflop/fma/interflop_fma.cxx"
    # # HASHMAP
    "interflop/hashmap/vfc_hashmap.c"
    # # IOSTREAM
    "interflop/iostream/logger.c"
    # PRNG
    "interflop/prng/tinymt64.c"
    "interflop/prng/xoshiro.cxx"
    # # RNG
    "interflop/rng/splitmix64.c"
    "interflop/rng/vfc_rng.c"
    "interflop/rng/xoroshiro128.c"
    "interflop/interflop_stdlib.c"
)

set (INTERFLOP_IEEE_SRC
    "interflop-backend-ieee/interflop_ieee.c"
    "interflop-backend-ieee/common/printf_specifier.c"
)

set (INTERFLOP_VIEEE_SRC
    "interflop-backend-ieee/x86_64/interflop_vector_ieee.c"
)

set (INTERFLOP_VERROU_SRC
    "interflop-backend-verrou/interflop_verrou.cxx"
)

set (INTERFLOP_VPREC_SRC
    "interflop-backend-vprec/common/vprec_tools.c"
    "interflop-backend-vprec/interflop_vprec.c"
    "interflop-backend-vprec/interflop_vprec_function_instrumentation.c"
)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/interflop")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-mfma" COMPILER_SUPPORTS_ARCH_MFMA)
if(COMPILER_SUPPORTS_ARCH_MFMA)
  set(FMA_FLAGS "-mfma")
  set(HAVE_FMA_INTRINSIC "-DHAVE_FMA_INTRINSIC ")
endif()

set(CRT_COMPILE_DEFINITIONS 
  # Preprocessor Options
  "-D__PIN__=1 -DPIN_CRT=1 "
  "-DTARGET_IA32E -DHOST_IA32E "
  "-DTARGET_LINUX "
  ${HAVE_FMA_INTRINSIC}
  "-DHAVE_FENV_H "
  "-D__PENE_FRONTEND__ "
)
set(CRT_COMPILE_OPTIONS
  # Compilateur (middle)
  "-funwind-tables"              # Code Gen Options
  "-fasynchronous-unwind-tables" # Code Gen Option
  "-fomit-frame-pointer"         # Code Optimize Options
  "-fno-strict-aliasing"         # Code Optimize Options
  "-fno-stack-protector"         # Instrumentation Options
  ${FMA_FLAGS}
  "-fPIC"
)
set(CRT_PREPROCESS_OPTIONS
  # Preprocesseur (front)
  "-isystem ${PIN_ROOT}/extras/stlport/include "
  "-isystem ${PIN_ROOT}/extras/libstdc++/include "
  "-isystem ${PIN_ROOT}/extras/crt/include "
  "-isystem ${PIN_ROOT}/extras/crt/include/arch-x86_64 "
  "-isystem ${PIN_ROOT}/extras/crt/include/kernel/uapi "
  "-isystem ${PIN_ROOT}/extras/crt/include/kernel/uapi/asm-x86 "
  "-I${PIN_ROOT}/source/include/pin "
  "-I${PIN_ROOT}/source/include/pin/gen "
  "-I${PIN_ROOT}/extras/components/include "
  "-I${PIN_ROOT}/extras/xed-intel64/include/xed "
)
set(CRT_CXX_COMPILE_OPTIONS
  "-fno-exceptions"
  "-fno-rtti"
  "-fPIC"
  "-faligned-new"
)
set(CRT_LINK_OPTIONS
  "-fPIC"
  "-Wl,--hash-style=sysv"
)
  
set(CRT_LINK_LIBRARIES
  "-L${PIN_ROOT}/intel64/runtime/pincrt"
  "-L${PIN_ROOT}/intel64/lib-ext"
  "-nostdlib"  "c-dynamic" "m-dynamic" "stlport-dynamic" "pin3dwarf" "dl-dynamic"
  # "c-dynamic" "m-dynamic" "stlport-dynamic" "pin3dwarf" "dl-dynamic"
)
add_subdirectory ("interflop")
add_subdirectory ("interflop-backend-verrou")
add_subdirectory ("interflop-backend-vprec")
add_subdirectory ("interflop-backend-ieee")

get_directory_property(CRT_COMPILE_DEFINITIONS DIRECTORY interflop VARIABLES CRT_COMPILE_DEFINITIONS
                                               DIRECTORY interflop-backend-verrou VARIABLES CRT_COMPILE_DEFINITIONS
                                               DIRECTORY interflop-backend-vprec VARIABLES CRT_COMPILE_DEFINITIONS
                                               DIRECTORY interflop-backend-ieee VARIABLES CRT_COMPILE_DEFINITIONS
)
get_directory_property(CRT_COMPILE_OPTIONS DIRECTORY interflop VARIABLES CRT_COMPILE_OPTIONS
                                           DIRECTORY interflop-backend-verrou VARIABLES CRT_COMPILE_OPTIONS
                                           DIRECTORY interflop-backend-vprec VARIABLES CRT_COMPILE_OPTIONS
                                           DIRECTORY interflop-backend-ieee VARIABLES CRT_COMPILE_OPTIONS
)
get_directory_property(CRT_CXX_COMPILE_OPTIONS DIRECTORY interflop VARIABLES CRT_CXX_COMPILE_OPTIONS
                                               DIRECTORY interflop-backend-verrou VARIABLES CRT_CXX_COMPILE_OPTIONS
                                               DIRECTORY interflop-backend-vprec VARIABLES CRT_CXX_COMPILE_OPTIONS
                                               DIRECTORY interflop-backend-ieee VARIABLES CRT_CXX_COMPILE_OPTIONS
)
get_directory_property(CRT_PREPROCESS_OPTIONS DIRECTORY interflop VARIABLES CRT_PREPROCESS_OPTIONS
                                              DIRECTORY interflop-backend-verrou VARIABLES CRT_PREPROCESS_OPTIONS
                                              DIRECTORY interflop-backend-vprec VARIABLES CRT_PREPROCESS_OPTIONS
                                              DIRECTORY interflop-backend-ieee VARIABLES CRT_PREPROCESS_OPTIONS
)
get_directory_property(CRT_LINK_OPTIONS DIRECTORY interflop VARIABLES CRT_LINK_OPTIONS
                                        DIRECTORY interflop VARIABLES CRT_LINK_OPTIONS
                                        DIRECTORY interflop-backend-verrou VARIABLES CRT_LINK_OPTIONS
                                        DIRECTORY interflop-backend-vprec VARIABLES CRT_LINK_OPTIONS
                                        DIRECTORY interflop-backend-ieee VARIABLES CRT_LINK_OPTIONS
)
get_directory_property(CRT_LINK_LIBRARIES DIRECTORY interflop VARIABLES CRT_LINK_LIBRARIES
                                          DIRECTORY interflop-backend-verrou VARIABLES CRT_LINK_LIBRARIES
                                          DIRECTORY interflop-backend-vprec VARIABLES CRT_LINK_LIBRARIES
                                          DIRECTORY interflop-backend-ieee VARIABLES CRT_LINK_LIBRARIES
)