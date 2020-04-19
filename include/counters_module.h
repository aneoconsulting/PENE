#pragma once
#include <pin.H>

#include "module.h"
#include "counters.h"

namespace pene {
  class counters_module : public module
  {
    counters c;
    KNOB<int> knob_counter;
  public:
    counters_module();
    bool validate() override;
    void init() override;
    const std::string& name() override;
  };
}