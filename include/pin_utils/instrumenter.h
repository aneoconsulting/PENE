#pragma once

#include <pin.H>

namespace pene {
  namespace pin_utils {

    class base_instrumenter
    {
    public:
      virtual void INS_AddInstrumentFunction() = 0;
      virtual void TRACE_AddInstrumentFunction() = 0;
    };

    template <class BASE_INSTRUMENTER>
    class instrumenter : public base_instrumenter
    {
    public:
      using base = BASE_INSTRUMENTER;

      void init_instrument(TRACE trace) { derived().init_instrument(trace); }
      void init_instrument(BBL bbl) { derived().init_instrument(bbl); }
      void instrument(INS ins) { derived().instrument(ins); }
      void end_instrument(BBL bbl) { derived().end_instrument(bbl); }
      void end_instrument(TRACE trace) { derived().end_instrument(trace); }

      void instrument_callback(TRACE trace_)
      {
        init_instrument(trace_);
        for (auto bbl = TRACE_BblHead(trace_); BBL_Valid(bbl); bbl = BBL_Next(bbl))
        {
          init_instrument(bbl);
          for (auto ins = BBL_InsHead(bbl); INS_Valid(ins); ins = INS_Next(ins))
          {
            instrument(ins);
          }
          end_instrument(bbl);
        }
        end_instrument(trace_);
      }

      void instrument_callback(INS ins_)
      {
        init_instrument(nullptr);
        init_instrument(MAKE_BBL(0));
        instrument(ins_);
        end_instrument(MAKE_BBL(0));
        end_instrument(nullptr);
      }

      virtual void INS_AddInstrumentFunction() override
      {
        ::INS_AddInstrumentFunction([](INS ins_, void* voided_instrumenter) { reinterpret_cast<instrumenter*>(voided_instrumenter)->instrument_callback(ins_); }, this);
      }

      virtual void TRACE_AddInstrumentFunction() override
      {
        ::TRACE_AddInstrumentFunction([](TRACE trace_, void* voided_instrumenter) { reinterpret_cast<instrumenter*>(voided_instrumenter)->instrument_callback(trace_); }, this);
      }

    protected:
      base& derived()
      { 
        return static_cast<base&>(*this); 
      }

      TRACE trace;
      BBL bbl;
      INS ins;

      instrumenter()
        : trace(nullptr)
        , bbl(MAKE_BBL(0))
        , ins(MAKE_INS(0))
      {}

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
