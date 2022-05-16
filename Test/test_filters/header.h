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

#define compute1(a, b) ((((a * a) - (b * b)) / (a + b)) + b)
#define compute2(a, b) ((((a * a) - (b * b)) / (a - b)) - b)