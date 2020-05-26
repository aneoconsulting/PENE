#pragma once
#include <pin.H>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <fstream>

namespace pene
{
  namespace pin_utils
  {
    class filter
    {
    public:
      enum check_status
      {
        IGNORE,
        INSTRUMENT,
        CHECK,
        CHECK_NEST
      };

      virtual ~filter();
      virtual check_status is_instrumented(TRACE) const;
      virtual check_status is_instrumented(BBL) const;
      virtual check_status is_instrumented(INS) const;
      virtual check_status check_trace() const=0;
      virtual check_status check_bbl() const=0;
      virtual check_status check_ins() const=0;
    protected:
      virtual check_status is_instrumented(ADDRINT) const=0;
    };

    class null_filter final : public filter
    {
    public:
      virtual check_status check_trace() const override;
      virtual check_status check_bbl() const override ;
      virtual check_status check_ins() const override ;
    protected:
      virtual check_status is_instrumented(ADDRINT) const override;
    };

  }
}