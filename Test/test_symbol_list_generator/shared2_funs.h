#pragma once

#ifndef LIBRARY_API
#  ifdef _WIN32
#      ifdef LIBRARY_EXPORTS
#          define LIBRARY_API extern "C" __declspec(dllexport)
#      else
#          define LIBRARY_API extern "C" __declspec(dllimport)
#      endif
#  else
#      define LIBRARY_API extern "C"
#  endif
#endif

LIBRARY_API float func2_1(float);
LIBRARY_API float func2_2(float);
LIBRARY_API float func2_3(float);
