#pragma once
#include <pin.H>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <fstream>

#include "filter.h"

namespace pene
{
  namespace pin_utils
  {
    class source_filter_base : public virtual filter
    {
    public:
      source_filter_base() = delete;
      virtual check_status check_trace() const override;
      virtual check_status check_bbl() const override;
      virtual check_status check_ins() const override;
      virtual check_status is_instrumented(TRACE) const override;
      virtual check_status is_instrumented(BBL) const override;

    protected:
      source_filter_base(const std::string& source_lis_filename);
      BOOL is_in_list(ADDRINT) const;
      BOOL is_in_list(const std::string & filename, INT32 line_number) const;

    private:
      void load(std::istream& istream);
      using line_list_t = std::tr1::unordered_set<int>;
      using line_list_by_obj_name_t = std::tr1::unordered_map<std::string, line_list_t>;
      // line_list[obj_name] => line_list
      line_list_by_obj_name_t line_list;
    };

    class source_exclude_filter final : public virtual source_filter_base
    {
    public:
      source_exclude_filter(const std::string& source_lis_filename);
      virtual check_status is_instrumented(ADDRINT) const override;
    };

    class source_include_filter final : public virtual source_filter_base
    {
    public:
      source_include_filter(const std::string& source_lis_filename);
      virtual check_status is_instrumented(ADDRINT) const override;
    };




  }
}