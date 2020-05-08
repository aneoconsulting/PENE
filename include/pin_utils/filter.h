#pragma once
#include <pin.H>

namespace pene
{
  namespace pin_utils
  {
    class filter
    {
    public:
      virtual BOOL is_instrumented(TRACE) const;
      virtual BOOL is_instrumented(BBL) const;
      virtual BOOL is_instrumented(INS) const;
      virtual BOOL is_instrumented(ADDRINT) const=0;
    };

    class null_filter final : public virtual filter
    {
    public:
      virtual BOOL is_instrumented(ADDRINT) const override;
    };




  }
}