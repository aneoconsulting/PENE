#include "pin_utils/instrumenter.h"

namespace pene
{
  namespace pin_utils
  {
    void instrumenter::TRACE_AddInstrumentFunction()
    {
      ::TRACE_AddInstrumentFunction([](TRACE trace, void* voided_instrumenter) { reinterpret_cast<instrumenter*>(voided_instrumenter)->instrument_callback(trace); }, this);
    }

    instrumenter::instrumenter(element_instrumenter* then_i, filter* f)
      : el_instrumenter(then_i)
      , filter_(f)
    {}

    instrumenter::~instrumenter()
    {
      if (filter_ != nullptr)
      {
        delete filter_;
        filter_ = nullptr;
      }
    }

    void instrumenter::instrument_callback(TRACE trace)
    {
      if (!filter_->check_trace() || filter_->is_instrumented(trace))
      {
        el_instrumenter->init_instrument(trace);
        for (auto bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
        {
          if (!filter_->check_bbl() || filter_->is_instrumented(bbl))
          {
            el_instrumenter->init_instrument(bbl);
            for (auto ins = BBL_InsHead(bbl); INS_Valid(ins); ins = INS_Next(ins))
            {
              if (!filter_->check_ins() || filter_->is_instrumented(ins))
              {
                el_instrumenter->instrument(ins);
              }
            }
            el_instrumenter->end_instrument(bbl);
          }
        }
        el_instrumenter->end_instrument(trace);
      }
    }
  }
}