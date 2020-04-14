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

LIBRARY_API void func1();
LIBRARY_API void func2();
LIBRARY_API void func3();
