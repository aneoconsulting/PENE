#pragma once

#include "module.h"
#include "pin.H"

class version_module : public module
{
    KNOB<bool> KnobVersion;
public:
    version_module();
    void Init();
    void End();
};