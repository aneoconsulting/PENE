#pragma once

#include <pin.H>

#include "module.h"

namespace pene {
  class version_module : public module
  {
    KNOB<bool> KnobVersion;
  public:
    version_module();
    void init();
    void end();
  };
}