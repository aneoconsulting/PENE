#pragma once

#include <pin.H>

namespace pene {
  namespace pin_utils {

    class base_instrumenter
    {
    public:
      virtual void INS_AddInstrumentFunction();
      virtual void TRACE_AddInstrumentFunction();

    protected:
      virtual void init_instrument(TRACE);
      virtual void init_instrument(BBL);
      virtual void instrument(INS);
      virtual void end_instrument(BBL);
      virtual void end_instrument(TRACE);

      virtual void instrument_callback(TRACE);
      virtual void instrument_callback(INS);
    };

    template <class BASE_INSTRUMENTER>
    class instrumenter : public base_instrumenter
    {
    protected:
      // TODO move to counter_instrumenter
      TRACE trace;
      // TODO move to counter_instrumenter
      BBL bbl;
      // TODO move to counter_instrumenter
      INS ins;

      instrumenter()
        : trace(nullptr)
        , bbl(MAKE_BBL(0))
        , ins(MAKE_INS(0))
      {}

      // TODO move to counter_instrumenter
      template<class ... Args>
      void dispatch_insertCall_bbl_ins(Args... args)
      {
        if (trace != nullptr)
        {
          insertCall(trace, IPOINT_ANYWHERE, args...);
        }
        else if (bbl.is_valid())
        {
          insertCall(bbl, IPOINT_ANYWHERE, args...);
        }
        else
        {
          insertCall(ins, IPOINT_BEFORE, args...);
        }
      }
    };
  }
}
