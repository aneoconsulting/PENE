#pragma once

#include <pin.H>

#include "module.h"
#include "pin_utils/instrumenter.h"

#define MAX_BACKENDS 16
#define MAX_ARGS 256

namespace pene {
  class interflop_module : public module
  {
    KNOB<bool> KnobInterflop;
    KNOB<bool> KnobVectorInterflop;
    pin_utils::instrumenter* data;

  public:

    interflop_module();
    void init() override;
    // virtual bool validate() override;
    const std::string& name() override;



    static void panic (const char *msg);

    static void _interflop_err(int eval, const char *fmt, ...);    
  private:
  };
}
