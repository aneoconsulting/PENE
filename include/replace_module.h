#pragma once
#include <pin.H>

#include "module.h"

namespace pene {
    class replace_module : public module
    {
        KNOB<int> knob_replace;
    public:
        replace_module();
        void init() override;
        const std::string& name() override;
    };
}