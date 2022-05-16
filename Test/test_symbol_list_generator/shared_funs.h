#pragma once

#ifndef LIBRARY_API
#  ifdef _WIN32
#      ifdef LIBRARY_EXPORTS
#          define LIBRARY_API extern "C" __declspec(dllexport)
#      else
#          define LIBRARY_API extern "C" __declspec(dllimport)
#      endif
#  elif
#      define LIBRARY_API extern "C"
#  endif
#endif

LIBRARY_API float func1(float a);
LIBRARY_API float func2(float a);
LIBRARY_API float func3(float a);
