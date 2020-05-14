#pragma once

#include <pin.H>
#include "filter.h"
#include "element_instrumenter.h"

namespace pene {
  namespace pin_utils {

    class instrumenter
    {
    public:
      void TRACE_AddInstrumentFunction();
      instrumenter() = delete;
      instrumenter(element_instrumenter* then_i, filter* f = new null_filter());
      virtual ~instrumenter();

    protected:

      virtual VOID instrument_callback(TRACE);
      element_instrumenter* el_instrumenter;
      filter* filter_;
    };
  }
}
