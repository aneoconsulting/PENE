#pragma once

#include "traits.h"

namespace pene {
  namespace pin_utils {

    template<class LOC, class ... ARGS> void insertCall(LOC loc, ARGS... args) {
      if constexpr (is_same<LOC, TRACE>::value)
      {
        TRACE_InsertCall(loc, args...);
      }
      else if constexpr (is_same<LOC, RTN>::value)
      {
        RTN_InsertCall(loc, args...);
      }
      else if constexpr (is_same<LOC, BBL>::value)
      {
        BBL_InsertCall(loc, args...);
      }
      else if constexpr (is_same<LOC, INS>::value)
      {
        INS_InsertCall(loc, args...);
      }
      else
      {
        static_assert(sizeof(LOC)==0);
      }
    }
  }
}
