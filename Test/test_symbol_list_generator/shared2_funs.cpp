#include "shared_funs.h"
#undef LIBRARY_API
#define LIBRARY_EXPORTS
#include "shared2_funs.h"

LIBRARY_API void func2_1() { func1(); }
LIBRARY_API void func2_2() {}
LIBRARY_API void func2_3() {}