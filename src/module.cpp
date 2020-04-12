#include "module.h"

#include <vector>
#include <algorithm>

namespace pene {
  namespace module_internals {
    static std::vector<module*> modules{};


  }
  module::module() { module_internals::modules.push_back(this); }
  module::~module() {
    auto & vec = module_internals::modules;
    vec.erase(std::remove(vec.begin(), vec.end(), this), vec.end());
  }
  bool module::validate() { return true; }
  bool module::validate_all()
  {
    for each (auto module_ptr in module_internals::modules)
    {
      if (!module_ptr->validate())
      {
        return false;
      }
    }
    return true;
  }
  void module::init_all()
  {
    for each (auto module_ptr in module_internals::modules)
    {
      module_ptr->init();
    }
  }
}