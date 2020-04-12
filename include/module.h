#pragma once

namespace pene {
  struct module
  {
    module();
    module(const module&) = delete;
    module(const module&&) = delete;
    module& operator=(const module&) = delete;
    module& operator=(const module&&) = delete;
    virtual ~module();
    virtual bool validate();
    virtual void init() = 0;
    static bool validate_all();
    static void init_all();
  };
}
