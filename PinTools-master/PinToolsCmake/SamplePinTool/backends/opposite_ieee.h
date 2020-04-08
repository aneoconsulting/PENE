#pragma once
#include "includes/interflop.h"
namespace interflop
{
  namespace backends {
    namespace opposite_ieee {
      struct interflop_backend_interface_t interflop_init(void** context);
    }
  }
}