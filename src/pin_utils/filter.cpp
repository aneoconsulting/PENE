#include "pin_utils/filter.h"


namespace pene
{
  namespace pin_utils
  {
    filter::~filter() {};

    filter::check_status filter::is_instrumented(TRACE trace) const
    {
      return is_instrumented(TRACE_Address(trace));
    }
    filter::check_status filter::is_instrumented(BBL bbl) const
    {
      return is_instrumented(BBL_Address(bbl));
    }
    filter::check_status filter::is_instrumented(INS ins) const
    {
      return is_instrumented(INS_Address(ins));
    }

    filter::check_status null_filter::is_instrumented(ADDRINT) const
    {
      return filter::INSTRUMENT;
    }
    filter::check_status null_filter::check_trace() const { return filter::INSTRUMENT; }
    filter::check_status null_filter::check_bbl() const { return filter::INSTRUMENT; }
    filter::check_status null_filter::check_ins() const { return filter::INSTRUMENT; }
  }
}
