#include "shared_funs.h"
#undef LIBRARY_API
#define LIBRARY_EXPORTS
#include "shared2_funs.h"

LIBRARY_API float func2_1(float a) { return func1(a + 1); }
LIBRARY_API float func2_2(float a) { return func1(a + 2); }
LIBRARY_API float func2_3(float a) { return func1(a + 3); }