#pragma once
#include <vector>
#include <string>

namespace pene {
  struct module
  {
    module(bool require_pin_symbols_initialization = false);
    module(const module&) = delete;
    module(const module&&) = delete;
    module& operator=(const module&) = delete;
    module& operator=(const module&&) = delete;
    virtual ~module();
    virtual bool validate();
    virtual void init() = 0;
    virtual const std::string&& name() = 0;
    static bool validate_all();
    static void init_all();
  protected:
    static std::vector<module*> modules;
    static bool pin_symbols_initalized;
  };
}
