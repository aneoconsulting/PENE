#include "counters_module.h"
#include "counters_utils.h"

#include <pin.H>
#include <iostream>
#include <cassert>

namespace pene {
  namespace counter_module_internals {
    static  TLS_KEY tls_counters_key = INVALID_TLS_KEY;
    static REG tls_counter_reg;


    void init_tls() {
      tls_counters_key = PIN_CreateThreadDataKey(NULL);
      if (tls_counters_key == INVALID_TLS_KEY)
      {
        std::cerr << "number of already allocated keys reached the MAX_CLIENT_TLS_KEYS limit(" << MAX_CLIENT_TLS_KEYS << ")" << std::endl;
        PIN_WriteErrorMessage("number of already allocated keys reached the MAX_CLIENT_TLS_KEYS limit", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
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

    template<class ... Types> struct addn {};
    template<class ... Types> struct addn<UINT32, UINT32, Types...> {
      static void PIN_FAST_ANALYSIS_CALL apply(counters* c, UINT32 idx, UINT32 b, Types ... values)
      {
        c->array[idx] += b;
        addn<Types...>::apply(c, values...);
      }
    };

    template<> struct addn<UINT32, UINT32> {
      static void PIN_FAST_ANALYSIS_CALL apply(counters* c, UINT32 idx, UINT32 b)
      {
        c->array[idx] += b;
      }
    };

    template<class ... Types> void PIN_FAST_ANALYSIS_CALL Addn_tls(THREADID threadid, Types ... values)
    {
      counters* c = static_cast<counters*>(PIN_GetThreadData(tls_counters_key, threadid));
      addn<Types...>::apply(c, values...);
    }


    template<class LOC, int N> struct counters_tls_instrumenter
    {
      template <class ... Types> struct  helper
      {
        template<class ... Args>
        static void apply(LOC loc, UINT32* indexes, UINT32* values, Args... args)
        {
          counters_tls_instrumenter<LOC, N - 1>::helper<UINT32, UINT32, Types...>::apply(
            loc, indexes, values, IARG_UINT32, indexes[N - 1], IARG_UINT32, values[N - 1], args...);
        }
      };

      template <class LOC>
      static void apply(LOC loc, UINT32* indexes, UINT32* values)
      {
        counters_tls_instrumenter<LOC, N - 1>::helper<UINT32, UINT32>::apply(
          loc, indexes, values, IARG_UINT32, indexes[N - 1], IARG_UINT32, values[N - 1], IARG_END);
      }
    };

    template <> struct counters_tls_instrumenter<INS, 0>
    {
      template <class ... Types> struct  helper
      {
        template<class ... Args>
        static void apply(INS ins, UINT32*, UINT32*, Args... args)
        {
          INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Addn_tls<Types...>, IARG_FAST_ANALYSIS_CALL, IARG_THREAD_ID, args...);
        }
      };
    };

    template <> struct counters_tls_instrumenter<BBL, 0>
    {
      template <class ... Types> struct  helper
      {
        template<class ... Args>
        static void apply(BBL bbl, UINT32*, UINT32*, Args... args)
        {
          BBL_InsertCall(bbl, IPOINT_ANYWHERE, (AFUNPTR)Addn_tls<Types...>, IARG_FAST_ANALYSIS_CALL, IARG_THREAD_ID, args...);
        }
      };
    };

    template <class LOC>
    int add_counters_tls(LOC loc, UINT32* indexes, UINT32* values, UINT32 nb_elts)
    {
      switch (nb_elts)
      {
      case 1:
        counters_tls_instrumenter<LOC, 1>::apply(loc, indexes, values);
        return 1;
      case 2:
        counters_tls_instrumenter<LOC, 2>::apply(loc, indexes, values);
        return 2;
      case 3:
        counters_tls_instrumenter<LOC, 3>::apply(loc, indexes, values);
        return 3;
      case 4:
        counters_tls_instrumenter<LOC, 4>::apply(loc, indexes, values);
        return 4;
      case 5:
        counters_tls_instrumenter<LOC, 5>::apply(loc, indexes, values);
        return 5;
      case 6:
        counters_tls_instrumenter<LOC, 6>::apply(loc, indexes, values);
        return 6;
      case 7:
        counters_tls_instrumenter<LOC, 7>::apply(loc, indexes, values);
        return 7;
      case 8:
        counters_tls_instrumenter<LOC, 8>::apply(loc, indexes, values);
        return 8;
      case 9:
        counters_tls_instrumenter<LOC, 9>::apply(loc, indexes, values);
        return 9;
      default:
        counters_tls_instrumenter<LOC, 10>::apply(loc, indexes, values);
        return 10;
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
        if (tls_counters_key == INVALID_TLS_KEY)
        {
          for (UINT i = 0; i < counters::size; ++i)
          {
            if (tmp_counters.array[i] > 0)
            {
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_FAST_ANALYSIS_CALL, IARG_PTR, &(counters_.array[i]), IARG_UINT32, (UINT32)tmp_counters.array[i], IARG_END);
            }
          }
        }
        else
        {
          UINT32 indexes[counters::size];
          UINT32 values[counters::size];
          UINT32 nb_elts = 0;
          for (UINT i = 0; i < counters::size; ++i)
          {
            if (tmp_counters.array[i] > 0)
            {
              indexes[nb_elts] = i;
              values[nb_elts] = (UINT32)tmp_counters.array[i];
              ++nb_elts;
            }
          }
          while (nb_elts>0)
          {
            nb_elts -= add_counters_tls<INS>(ins, indexes, values, nb_elts);
          }
        }
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

        if (tls_counters_key == INVALID_TLS_KEY)
        {
          for (UINT i = 0; i < counters::size; ++i)
          {
            if (tmp_counters.array[i] > 0)
            {
              BBL_InsertCall(bbl, IPOINT_ANYWHERE, (AFUNPTR)Add, IARG_FAST_ANALYSIS_CALL, IARG_PTR, &(counters_.array[i]), IARG_UINT32, (UINT32)tmp_counters.array[i], IARG_END);
            }
          }
        }
        else
        {
          UINT32 indexes[counters::size];
          UINT32 values[counters::size];
          UINT32 nb_elts = 0;
          for (UINT i = 0; i < counters::size; ++i)
          {
            if (tmp_counters.array[i] > 0)
            {
              indexes[nb_elts] = i;
              values[nb_elts] = (UINT32)tmp_counters.array[i];
              ++nb_elts;
            }
          }
          while (nb_elts > 0)
          {
            nb_elts -= add_counters_tls<BBL>(bbl, indexes, values, nb_elts);
          }
        }
      }
    }
  }

  using namespace counter_module_internals;

  counters_module::counters_module() :module(), c(), knob_counter(KNOB_MODE_WRITEONCE, "pintool",
    "counter-mode", "1", "Activate floating point instruction counting. 0: no counter, 1: fast counter, 2: slow counter (for debug purpose).")
  {
  }

  bool counters_module::validate() 
  {
    auto mode = knob_counter.Value();
    std::cerr << "Checking configuration: FP instructions count with counter-mode=" << mode << std::endl;
    if (mode < 0 || mode > 2)
    {
      std::cerr << "ERROR: -counter-mode option only accepts values 0, 1 or 2." << std::endl;
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
  const std::string& counters_module::name() { 
    static const std::string name_{ "counters_module" };
    return name_;
  }
}