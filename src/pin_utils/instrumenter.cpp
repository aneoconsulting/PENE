#include "pin_utils/instrumenter.h"

namespace pene
{
  namespace pin_utils
  {
    void base_instrumenter::init_instrument(TRACE trace_) {}
    void base_instrumenter::init_instrument(BBL bbl_) {}
    void base_instrumenter::instrument(INS ins_) {}
    void base_instrumenter::end_instrument(BBL bbl_) {}
    void base_instrumenter::end_instrument(TRACE trace_) {}

    void base_instrumenter::instrument_callback(TRACE trace)
    {
      init_instrument(trace);
      for (auto bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
      {
        init_instrument(bbl);
        for (auto ins = BBL_InsHead(bbl); INS_Valid(ins); ins = INS_Next(ins))
        {
          instrument(ins);
        }
        end_instrument(bbl);
      }
      end_instrument(trace);
    }

    void base_instrumenter::instrument_callback(INS ins_)
    {
      init_instrument(nullptr);
      init_instrument(MAKE_BBL(0));
      instrument(ins_);
      end_instrument(MAKE_BBL(0));
      end_instrument(nullptr);
    }

    void base_instrumenter::INS_AddInstrumentFunction()
    {
      ::INS_AddInstrumentFunction([](INS ins, void* voided_instrumenter) { reinterpret_cast<base_instrumenter*>(voided_instrumenter)->instrument_callback(ins); }, this);
    }

    void base_instrumenter::TRACE_AddInstrumentFunction()
    {
      ::TRACE_AddInstrumentFunction([](TRACE trace, void* voided_instrumenter) { reinterpret_cast<base_instrumenter*>(voided_instrumenter)->instrument_callback(trace); }, this);
    }
  }
}