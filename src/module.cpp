#include "module.h"
#include <pin.H>
#include <vector>
#include <iostream>
#include <algorithm>

namespace pene {

  std::vector<module*> module::modules{};
  bool module::pin_symbols_initalized = false;

  module::module(bool require_pin_symbols_initialization) 
  { 
    module::modules.push_back(this);
    if (require_pin_symbols_initialization && !module::pin_symbols_initalized)
    {
      module::pin_symbols_initalized = true;
      PIN_InitSymbols();
    }
  }
  module::~module() {
    auto & vec = module::modules;
    vec.erase(std::remove(vec.begin(), vec.end(), this), vec.end());
  }
  bool module::validate() { return true; }
  bool module::validate_all()
  {
    for (auto module_ptr : module::modules)
    {
      std::cerr << "Validating inputs for " << module_ptr->name() << std::endl;
      if (!module_ptr->validate())
      {
        return false;
      }
    }
    KNOB<bool> * interflop = static_cast<KNOB<bool>*>(KNOB_BASE::FindKnob("interflop"));
    KNOB<int> *  replace   = static_cast<KNOB<int>*>(KNOB_BASE::FindKnob("fp-replace"));
    if (interflop && replace)
    {
      if (interflop->Value() && replace->Value())
      {
        std::cerr << "error : Only one mode between -interflop or -fp-replace should be used" << std::endl;
        return false;
      }
    }
    return true;
  }
  void module::init_all()
  {
    std::cerr << "Modules initialization." << std::endl;
    for (auto module_ptr : module::modules)
    {
      module_ptr->init();
    }    
    std::cerr << "Modules initialized." << std::endl;
  }
}