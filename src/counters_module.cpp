#include "counters_module.h"
#include "counters_utils.h"

#include <pin.H>
#include <iostream>

namespace pene {
  void Add(counters::int_type* a, UINT32 b)
  {
    *a += b;
  }

  void INS_CountersInstrumentation(INS ins, void* void_counters)
  {
    auto counters_ = (counters*)void_counters;
    auto tmp_counters = counters{};
    auto oc = INS_Opcode(ins);
    update_counters(oc, tmp_counters);

    for (auto i = 0; i < counters::size; ++i)
    {
      if (tmp_counters.array[i] > 0) 
      {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_PTR, &counters_->array[i], IARG_UINT32, (UINT32)tmp_counters.array[i], IARG_END);
      }
    }
  }

  VOID TRACE_CountersInstrumentation(TRACE trace, VOID* void_counters)
  {
    auto counters_ = (counters*)void_counters;

    for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
    {
      auto tmp_counters = counters{};

      for (INS ins = BBL_InsHead(bbl); INS_Valid(ins); ins = INS_Next(ins))
      {
        auto oc = INS_Opcode(ins);
        update_counters(oc, tmp_counters);
      }    

      for (auto i = 0; i < counters::size; ++i)
      {
        if (tmp_counters.array[i] > 0)
        {
          BBL_InsertCall(bbl, IPOINT_ANYWHERE, (AFUNPTR)Add, IARG_PTR, &counters_->array[i], IARG_UINT32, (UINT32)tmp_counters.array[i], IARG_END);
        }
      }
    }
  }

  counters_module::counters_module() :module(), c(), knob_counter(KNOB_MODE_WRITEONCE, "pintool",
    "counter-mode", "1", "Activate floating point instruction counting. 0: no counter, 1: fast counter, 2: slow counter (for debug purpose).") {}

  bool counters_module::validate() 
  {
    std::cerr << "Checking configuration: FP instructions count." << std::endl;

    auto mode = knob_counter.Value();
    if (mode < 0 && mode > 2)
    {
      PIN_WriteErrorMessage("-counter-mode option only accepts values 0, 1 or 2.", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_NONFATAL, 0);
      return false;
    }
    return true;
  }

  void counters_module::init()
  {
    std::cerr << "Initialization: FP instructions count." << std::endl;
    auto mode = knob_counter.Value();
    switch (mode)
    {
    case 1:
      std::cerr << "Set counters to \"trace\" instrumentation mode." << std::endl;
      TRACE_AddInstrumentFunction(TRACE_CountersInstrumentation, &c);
      PIN_AddFiniFunction([](INT32, void* void_counters) {((counters*)void_counters)->print(); }, &c);
      break;
    case 2:
      std::cerr << "Set counters to \"instructions\" instrumentation mode." << std::endl;
      INS_AddInstrumentFunction(INS_CountersInstrumentation, &c);
      PIN_AddFiniFunction([](INT32, void* void_counters) {((counters*)void_counters)->print(); }, &c);
      break;
    default: // case 0
      std::cerr << "Set counters to no instrumentation mode" << std::endl;
      break;
    }
  }
}