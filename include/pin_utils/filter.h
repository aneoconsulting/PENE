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
      virtual ~filter();
      virtual BOOL is_instrumented(TRACE) const;
      virtual BOOL is_instrumented(BBL) const;
      virtual BOOL is_instrumented(INS) const;
      virtual BOOL is_instrumented(ADDRINT) const=0;
      virtual BOOL check_trace() const=0;
      virtual BOOL check_bbl() const=0;
      virtual BOOL check_ins() const=0;
    };

    class null_filter final : public filter
    {
    public:
      virtual BOOL is_instrumented(ADDRINT) const override;
      virtual BOOL check_trace() const override;
      virtual BOOL check_bbl() const override ;
      virtual BOOL check_ins() const override ;
    };

  }
}