#include "pin_utils/filter.h"


namespace pene
{
  namespace pin_utils
  {
    filter::~filter() {};

    BOOL filter::is_instrumented(TRACE trace) const
    {
      return is_instrumented(TRACE_Address(trace));
    }
    BOOL filter::is_instrumented(BBL bbl) const
    {
      return is_instrumented(BBL_Address(bbl));
    }
    BOOL filter::is_instrumented(INS ins) const
    {
      return is_instrumented(INS_Address(ins));
    }

    BOOL null_filter::is_instrumented(ADDRINT) const
    {
      return true;
    }
    BOOL null_filter::check_trace() const { return false; }
    BOOL null_filter::check_bbl() const { return false; }
    BOOL null_filter::check_ins() const { return false; }
  }
}
