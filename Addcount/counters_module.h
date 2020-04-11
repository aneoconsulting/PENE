#pragma once
#include "module.h"
#include "counters.h"

namespace pene {
  class counters_module : public module
  {
    counters c;
  public:
    counters_module();
    void init();
    void end();
  };

}