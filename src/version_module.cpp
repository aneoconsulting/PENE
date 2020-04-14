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
      std::cout << "This execution is launched with PENE version " << pene_version << std::endl;
      std::cout << "Execution will stop now." <<  std::endl;
      exit(0);
    }
  }
}
