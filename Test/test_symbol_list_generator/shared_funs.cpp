#define LIBRARY_EXPORTS
#include "shared_funs.h"

LIBRARY_API float func1(float a) {
	int ai = *((int*)(&a));
	if (ai & 0xFF)
	{
		ai = ai ^ 0xFF;
		ai = ai / 2;
		return (*((float*)(&ai)));
	}
	return a+1;
}
LIBRARY_API float func2(float a) {
	int ai = *((int*)(&a));
	if (ai & 0xFF)
	{
		ai = ai ^ 0xFF;
		ai = ai / 2;
		return (*((float*)(&ai)));
	}
	return a + 2;
}
LIBRARY_API float func3(float a) {
	int ai = *((int*)(&a));
	if (ai & 0xFF)
	{
		ai = ai ^ 0xFF;
		ai = ai / 2;
		return (*((float*)(&ai)));
	}
	return a + 3;
}