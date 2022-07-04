cmake_minimum_required(VERSION 3.14)

set(PIN_URL_WINDOWS https://software.intel.com/sites/landingpage/pintool/downloads/pin-3.23-98579-gb15ab7903-msvc-windows.zip)
set(PIN_URL_LINUX	https://software.intel.com/sites/landingpage/pintool/downloads/pin-3.23-98579-gb15ab7903-gcc-linux.tar.gz)

# Extract windows toolkit name from archive (.zip)
get_filename_component(PIN_TOOLKIT_WINDOWS	${PIN_URL_WINDOWS}		NAME)
get_filename_component(PIN_TOOLKIT_WINDOWS	${PIN_URL_WINDOWS}		NAME_WLE)

# TODO Since PIN version 3.13, the Linux package does not have the same hierarchy as for Windows. We need to
# handle this situation properly.
## Extract linux toolkit name from archive (.tar.gz)
#get_filename_component(PIN_TOOLKIT_LINUX	${PIN_URL_LINUX}		NAME)
#get_filename_component(PIN_TOOLKIT_LINUX	${PIN_TOOLKIT_LINUX}	NAME_WLE)
#get_filename_component(PIN_TOOLKIT_LINUX	${PIN_TOOLKIT_LINUX}	NAME_WLE)
