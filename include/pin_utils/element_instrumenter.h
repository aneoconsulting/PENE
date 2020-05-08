#pragma once

#include <pin.H>
#include "filter.h"

namespace pene {
  namespace pin_utils {

    class element_instrumenter
    {
    public:
      virtual VOID init_instrument(TRACE);
      virtual VOID init_instrument(BBL);
      virtual VOID instrument(INS) = 0;
      virtual VOID end_instrument(BBL);
      virtual VOID end_instrument(TRACE);
    };

    class null_element_instrumenter final : public element_instrumenter
    {
    public:
      virtual VOID instrument(INS) override;
    };
  }
}
