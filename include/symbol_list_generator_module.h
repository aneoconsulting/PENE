#pragma once
#include <pin.H>
#include <fstream>
#include <iostream>
#include <string>
#include "module.h"
#include "counters.h"

namespace pene {
  class symbol_list_generator_module : public module
  { 
  public:
    symbol_list_generator_module();
    bool validate() override;
    void init() override;
    const std::string& name() override;
  };
}