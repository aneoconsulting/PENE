#include "pin.H"
#include "instlib.H"

#include "version_module.h"
#include "version.h"

version_module::version_module() : module(), KnobVersion(KNOB_MODE_WRITEONCE, "pintool",
    "version", "0", "display the actual version of this pin tool")
{}

void version_module::Init()
{
    if (KnobVersion.Value())
    {
        std::cout << "Current version for Addcount : ";
        std::cout << pene_version << std::endl;
        exit(0);
    }
}

void version_module::End() {}
//