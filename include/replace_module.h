#pragma once
#include <pin.H>

#include "module.h"
#include "pin_utils/instrumenter.h"

namespace pene {
  class replace_module : public module
  {
    KNOB<int> knob_replace;
    pin_utils::instrumenter* data;
  public:
    replace_module();
    ~replace_module();
    virtual void init() override;
    virtual bool validate() override;
    virtual const std::string& name() override;
  };
}