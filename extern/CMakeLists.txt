project("Interflop")


include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/interflop")


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
)

set(OUTPUT_DIRECTORY "${PIN_ROOT}/intel64/lib-ext")

add_subdirectory ("argp")
add_subdirectory ("interflop")
add_subdirectory ("interflop-backend-verrou")
add_subdirectory ("interflop-backend-vprec")
add_subdirectory ("interflop-backend-ieee")

get_directory_property(CRT_COMPILE_DEFINITIONS DIRECTORY interflop VARIABLES CRT_COMPILE_DEFINITIONS
                                               DIRECTORY interflop-backend-verrou VARIABLES CRT_COMPILE_DEFINITIONS
                                               DIRECTORY interflop-backend-vprec VARIABLES CRT_COMPILE_DEFINITIONS
                                               DIRECTORY interflop-backend-ieee VARIABLES CRT_COMPILE_DEFINITIONS
                                               DIRECTORY argp VARIABLES CRT_COMPILE_DEFINITIONS
)
get_directory_property(CRT_COMPILE_OPTIONS DIRECTORY interflop VARIABLES CRT_COMPILE_OPTIONS
                                           DIRECTORY interflop-backend-verrou VARIABLES CRT_COMPILE_OPTIONS
                                           DIRECTORY interflop-backend-vprec VARIABLES CRT_COMPILE_OPTIONS
                                           DIRECTORY interflop-backend-ieee VARIABLES CRT_COMPILE_OPTIONS
                                           DIRECTORY argp VARIABLES CRT_COMPILE_OPTIONS
)
get_directory_property(CRT_CXX_COMPILE_OPTIONS DIRECTORY interflop VARIABLES CRT_CXX_COMPILE_OPTIONS
                                               DIRECTORY interflop-backend-verrou VARIABLES CRT_CXX_COMPILE_OPTIONS
                                               DIRECTORY interflop-backend-vprec VARIABLES CRT_CXX_COMPILE_OPTIONS
                                               DIRECTORY interflop-backend-ieee VARIABLES CRT_CXX_COMPILE_OPTIONS
                                               DIRECTORY argp VARIABLES CRT_CXX_COMPILE_OPTIONS
)
get_directory_property(CRT_PREPROCESS_OPTIONS DIRECTORY interflop VARIABLES CRT_PREPROCESS_OPTIONS
                                              DIRECTORY interflop-backend-verrou VARIABLES CRT_PREPROCESS_OPTIONS
                                              DIRECTORY interflop-backend-vprec VARIABLES CRT_PREPROCESS_OPTIONS
                                              DIRECTORY interflop-backend-ieee VARIABLES CRT_PREPROCESS_OPTIONS
                                              DIRECTORY argp VARIABLES CRT_PREPROCESS_OPTIONS
)
get_directory_property(CRT_LINK_OPTIONS DIRECTORY interflop VARIABLES CRT_LINK_OPTIONS
                                        DIRECTORY interflop VARIABLES CRT_LINK_OPTIONS
                                        DIRECTORY interflop-backend-verrou VARIABLES CRT_LINK_OPTIONS
                                        DIRECTORY interflop-backend-vprec VARIABLES CRT_LINK_OPTIONS
                                        DIRECTORY interflop-backend-ieee VARIABLES CRT_LINK_OPTIONS
                                        DIRECTORY argp VARIABLES CRT_LINK_OPTIONS
)
get_directory_property(CRT_LINK_LIBRARIES DIRECTORY interflop VARIABLES CRT_LINK_LIBRARIES
                                          DIRECTORY interflop-backend-verrou VARIABLES CRT_LINK_LIBRARIES
                                          DIRECTORY interflop-backend-vprec VARIABLES CRT_LINK_LIBRARIES
                                          DIRECTORY interflop-backend-ieee VARIABLES CRT_LINK_LIBRARIES
                                          DIRECTORY argp VARIABLES CRT_LINK_LIBRARIES
)

get_directory_property( OUTPUT_DIRECTORY  DIRECTORY interflop VARIABLES OUTPUT_DIRECTORY
                                          DIRECTORY interflop-backend-verrou VARIABLES OUTPUT_DIRECTORY
                                          DIRECTORY interflop-backend-vprec VARIABLES OUTPUT_DIRECTORY
                                          DIRECTORY interflop-backend-ieee VARIABLES OUTPUT_DIRECTORY
                                          DIRECTORY argp VARIABLES OUTPUT_DIRECTORY
)
