#include "counters_module.h"
#include "counters_utils.h"

#include <pin.H>
#include <iostream>
#include <cassert>
#include "utils.h"

namespace pene {
  using namespace utils;
  namespace counter_module_internals {

    static TLS_KEY tls_counters_key = INVALID_TLS_KEY;
    static REG tls_counter_reg;

    void init_tls() {
      tls_counters_key = PIN_CreateThreadDataKey(NULL);
      if (tls_counters_key == INVALID_TLS_KEY)
      {
        std::cerr << "number of already allocated keys reached the MAX_CLIENT_TLS_KEYS limit(" << MAX_CLIENT_TLS_KEYS << ")" << std::endl;
        PIN_WriteErrorMessage("number of already allocated keys reached the MAX_CLIENT_TLS_KEYS limit", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
        PIN_ExitProcess(1);
      }
      tls_counter_reg = PIN_ClaimToolRegister();
      if (!REG_valid_for_iarg_reg_value(tls_counter_reg))
      {
        std::cerr << "Cannot allocate tls register for counters" << std::endl;
        PIN_WriteErrorMessage("Cannot allocate tls register for counters", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
        PIN_ExitProcess(1);
      }
    }

    static VOID ThreadStart(THREADID threadid, CONTEXT*, INT32, VOID*)
    {
      if (tls_counters_key != INVALID_TLS_KEY)
      {
        counters* tdata = new counters;
        if (PIN_SetThreadData(tls_counters_key, tdata, threadid) == FALSE)
        {
          std::cerr << "PIN_SetThreadData failed" << std::endl;
          PIN_WriteErrorMessage("PIN_SetThreadData failed", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
          PIN_ExitProcess(1);
        }
      }
    }

    static VOID ThreadFini(THREADID threadIndex, const CONTEXT*, INT32, VOID* voided_counters)
    {
      if (tls_counters_key != INVALID_TLS_KEY)
      {
        counters* local_counters = static_cast<counters*>(PIN_GetThreadData(tls_counters_key, threadIndex));
        counters* global_counters = static_cast<counters*>(voided_counters);
        for (int i = 0; i < counters::size; ++i)
        {
          global_counters->array[i] += local_counters->array[i];
        }
        delete local_counters;
      }
    }

    static void PIN_FAST_ANALYSIS_CALL Add(void* a_, UINT32 b)
    {
      volatile auto a = reinterpret_cast<counters::int_type*>(a_);
      *a += b;
    }

    static counters* PIN_FAST_ANALYSIS_CALL get_counters_tls(THREADID threadid)
    {
      return static_cast<counters*>(PIN_GetThreadData(tls_counters_key, threadid));
    }

    template<int I>
    static void PIN_FAST_ANALYSIS_CALL Addi(void* voided_counters, UINT32 b)
    {
      volatile auto c = static_cast<counters*>(voided_counters);
      Add(c->array + I, b);
    }

    template <class LOC, int N=0>
    void add_counters_tls(LOC loc, IPOINT ipoint, UINT32 index, UINT32 value)
    {
      if (index < N)
      {
        std::cerr << "ERROR: index(" << index << ")<N(" << N << ")\n";
        PIN_WriteErrorMessage("ERROR: index<N)", 1000, PIN_ERR_FATAL, 0);
        PIN_ExitApplication(-1);
      }
      if constexpr(N < counters::size)
      {
        if (index > N)
        {
          add_counters_tls<LOC, N + 1>(loc, ipoint, index, value);
          return;
        }
      }
      insertCall(loc, ipoint, (AFUNPTR)Addi<N>, IARG_FAST_ANALYSIS_CALL, IARG_REG_VALUE, tls_counter_reg, IARG_UINT32, value, IARG_END);
    }

    template <class LOC>
    static void counters_instrumentation(LOC loc, IPOINT ipoint, counters& c, const counters& tmp)
    {
      if (tls_counters_key == INVALID_TLS_KEY)
      {
        for (UINT32 i = 0; i < counters::size; ++i)
        {
          if (tmp.array[i] > 0)
          {
            insertCall(loc, ipoint, (AFUNPTR)Add, IARG_FAST_ANALYSIS_CALL, IARG_PTR, &(c.array[i]), IARG_UINT32, (UINT32)tmp.array[i], IARG_END);
          }
        }
      }
      else
      {
        bool is_init = false;
        for (UINT32 i = 0; i < counters::size; ++i)
        {
          if (tmp.array[i] > 0)
          {
            if (!is_init)
            {
              insertCall(loc, ipoint, (AFUNPTR)get_counters_tls, IARG_FAST_ANALYSIS_CALL, IARG_THREAD_ID, IARG_RETURN_REGS, tls_counter_reg, IARG_END);
            }
            add_counters_tls(loc, ipoint, i, (UINT32)tmp.array[i]);
          }
        }
      }
    }

    static void INS_CountersInstrumentation(INS ins, void* void_counters)
    {
      if (INS_IsOriginal(ins))
      {
        auto& counters_ = *(counters*)void_counters;
        auto tmp_counters = counters();
        auto oc = INS_Opcode(ins);
        update_counters(oc, tmp_counters);

        counters_instrumentation(ins, IPOINT_BEFORE, counters_, tmp_counters);
      }
    }

    static VOID TRACE_CountersInstrumentation(TRACE trace, VOID* void_counters)
    {
      auto& counters_ = *(counters*)void_counters;

      for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
      {
        auto tmp_counters = counters{};

        for (INS ins = BBL_InsHead(bbl); INS_Valid(ins); ins = INS_Next(ins))
        {
          if (INS_IsOriginal(ins))
          {
            auto oc = INS_Opcode(ins);
            update_counters(oc, tmp_counters);
          }
        }

        counters_instrumentation(bbl, IPOINT_ANYWHERE, counters_, tmp_counters);
      }
    }
  }

  using namespace counter_module_internals;

  counters_module::counters_module() :module(), c(), knob_counter_instrumentation_mode(KNOB_MODE_WRITEONCE, "pintool",
    "counter-mode", "1", "Activate floating point instruction counting. 0: no counter, 1: fast counter, 2: slow counter (for debug purpose)."),
    knob_counters_use_tls(KNOB_MODE_WRITEONCE, "pintool", "counter-tls", "0", "Activate use of the Thread Local Storage (TLS) for the counters")
  {
  }

  bool counters_module::validate() 
  {
    auto mode = knob_counter_instrumentation_mode.Value();
    std::cerr << "Checking configuration: FP instructions count with counter-mode=" << mode << std::endl;
    if (mode < 0 || mode > 2)
    {
      std::cerr << "ERROR: -counter-mode option only accepts values 0, 1 or 2." << std::endl;
      PIN_WriteErrorMessage("-counter-mode option only accepts values 0, 1 or 2.", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_NONFATAL, 0);
      return false;
    }
    std::cerr << "Checking configuration: FP instructions count with counter-tls=" << knob_counters_use_tls.Value() << std::endl;
    return true;
  }

  void counters_module::init()
  {
    std::cerr << "Initialization: FP instructions count." << std::endl;
    auto mode = knob_counter_instrumentation_mode.Value();
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
      return;
    }
    if (knob_counters_use_tls.Value())
    {
      init_tls();
      if (tls_counters_key != INVALID_TLS_KEY)
      {
        std::cerr << "TLS has been activated for counters." << std::endl;
        PIN_AddThreadStartFunction(ThreadStart, 0);
        PIN_AddThreadFiniFunction(ThreadFini, &c);
      }
    }
  }
  const std::string& counters_module::name() { 
    static const std::string name_{ "counters_module" };
    return name_;
  }
}