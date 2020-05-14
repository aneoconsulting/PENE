#include "version_module.h"

#include <iostream>

#include "version.h"

namespace pene {
  version_module::version_module() : module(), KnobVersion(KNOB_MODE_WRITEONCE, "pintool",
    "version", "0", "display the actual version of this pin tool")
  {}

  void version_module::init()
  {
    if (KnobVersion.Value())
    {
      std::cerr << "This execution is launched with PENE version " << pene_version << std::endl;
      std::cerr << "Execution will stop now." <<  std::endl;
      PIN_ExitProcess(1);
    }
  }

  const std::string& version_module::name() 
  {
    static const std::string name_("version_module");
    return name_;
  }
}
