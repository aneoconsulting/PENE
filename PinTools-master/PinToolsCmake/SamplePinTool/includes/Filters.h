#pragma once
#include <unordered_set>
#include <unordered_map>
#include <string>
#include "pin.H"

namespace pene {

  class Filter
  {
  public:
    static Filter& Instance();
    VOID Activate();
    BOOL SelectRtn(RTN rtn) const;
    BOOL SelectTrace(TRACE trace) const;
    BOOL SelectBbl(BBL bbl) const;
    BOOL SelectIns(INS ins) const;
    BOOL SelectAddr(ADDRINT addr) const;
    ~Filter();

  private:
    Filter();
    Filter(const Filter&) = delete;
    Filter(const Filter&&) = delete;

    VOID ActivateExclusionList();
    VOID EndExclusionList();
    VOID ActivateExclusionListGeneration();
    VOID EndExclusionListGeneration();
    VOID ActivateSourceInstrumentList();
    VOID EndSourceInstrumentList();

    KNOB<std::string> KnobSymExludeList;

    KNOB<std::string> KnobGenExclusionList;

    KNOB<std::string> KnobSourceInstrumentList;

    typedef std::tr1::unordered_set<std::string> name_list_t;
    typedef std::tr1::unordered_map<std::string, name_list_t> name_list_by_obj_name_t;
    // sym_exlude_list[obj_name] => func_name_list
    name_list_by_obj_name_t sym_exlude_list;

    typedef std::tr1::unordered_set<INT32> line_list_t;
    typedef std::tr1::unordered_map<std::string, line_list_t> line_list_by_file_t;
    // sym_gen_list[obj_name] => func_name_list
    line_list_by_file_t sym_gen_list;
  };
}

