#pragma once
#include <pin.H>

#include "module.h"
#include "pin_utils/instrumenter.h"

namespace pene {
  class counters_module : public module
  {
    KNOB<UINT> knob_counter_instrumentation_mode;
    KNOB<bool> knob_counters_use_tls;
    pin_utils::base_instrumenter* data;
  public:
    counters_module();
    ~counters_module();

    bool validate() override;
    void init() override;
    const std::string& name() override;
  };
}