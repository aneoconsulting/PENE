#include <string>

#define xstr(s) str(s)
#define str(s) #s

#define PENE_VERSION_MAJOR 0
#define PENE_VERSION_MINOR 3
#define PENE_VERSION "v" xstr(PENE_VERSION_MAJOR) "." xstr(PENE_VERSION_MINOR)

constexpr char pene_version[] = PENE_VERSION;