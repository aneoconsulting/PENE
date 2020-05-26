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
      auto trace_status = filter_->check_trace();
      if (trace_status == filter::CHECK)
      {
        trace_status = filter_->is_instrumented(trace);
      }

      if (trace_status != filter::IGNORE)
      {
        el_instrumenter->init_instrument(trace);
        for (auto bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
        {
          auto bbl_status = trace_status;
          if (bbl_status == filter::CHECK || bbl_status == filter::CHECK_NEST)
          {
            bbl_status = filter_->check_bbl();
            if (bbl_status == filter::CHECK)
            {
              bbl_status = filter_->is_instrumented(bbl);
            }
          }

          if (bbl_status != filter::IGNORE)
          {
            el_instrumenter->init_instrument(bbl);
            for (auto ins = BBL_InsHead(bbl); INS_Valid(ins); ins = INS_Next(ins))
            {
              auto ins_status = bbl_status;
              if (ins_status == filter::CHECK || ins_status == filter::CHECK_NEST)
              {
                ins_status = filter_->check_ins();
                if (ins_status == filter::CHECK)
                {
                  ins_status = filter_->is_instrumented(ins);
                }
              }
              if (ins_status != filter::IGNORE)
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