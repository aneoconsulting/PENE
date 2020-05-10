#pragma once
#include <pin.H>

#include "module.h"
#include "pin_utils/instrumenter.h"
#include <string>

namespace pene {
  class replace_module final : public module
  {
    KNOB<int> knob_replace_mode;
    KNOB<std::string> knob_exl_symbols;
    KNOB<std::string> knob_incl_source_lines;

    pin_utils::instrumenter* data;
  public:
    replace_module();
    ~replace_module();
    virtual void init() override;
    virtual bool validate() override;
    virtual const std::string&& name() override;
  };
}