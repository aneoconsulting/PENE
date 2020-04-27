#include "counters_module.h"
#include "counters_utils.h"

#include <pin.H>
#include <iostream>
#include <cassert>

namespace pene {
  namespace counter_module_internals {
    static  TLS_KEY tls_counters_key = INVALID_TLS_KEY;

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

    template<class ... Types> 
    struct addn {};

    template<class ... Types> 
    struct addn<UINT32, UINT32, Types...> {
      inline static void apply(counters* c, UINT32 idx, UINT32 b, Types ... values)
      {
        c->array[idx] += b;
        addn<Types...>::apply(c, values...);
      }
    };

    template<> 
    struct addn<UINT32, UINT32> {
      inline static void apply(counters* c, UINT32 idx, UINT32 b)
      {
        c->array[idx] += b;
      }
    };

    template<class ... Types> 
    void PIN_FAST_ANALYSIS_CALL Addn_tls(THREADID threadid, Types ... values)
    {
      counters* c = static_cast<counters*>(PIN_GetThreadData(tls_counters_key, threadid));
      addn<Types...>::apply(c, values...);
    }

    template <class A, class B> struct is_same { static constexpr bool value = false; };
    template <class A> struct is_same<A, A> { static constexpr bool value = true; };

    template<class LOC, int N> struct counters_tls_instrumenter
    {
      template <class ... Types> struct  helper
      {
        template<class ... Args>
        static void apply(LOC loc, UINT32* indexes, UINT32* values, Args... args)
        {
          if constexpr (N == 0)
          {
            indexes; //suppress C4100 warning with VS
            values; //suppress C4100 warning with VS
            if constexpr (is_same<LOC, BBL>::value)
            {
              BBL_InsertCall(loc, IPOINT_ANYWHERE, (AFUNPTR)Addn_tls<Types...>, IARG_FAST_ANALYSIS_CALL, IARG_THREAD_ID, args..., IARG_END);
              return;
            }
            else if constexpr (is_same<LOC, INS>::value)
            {
              INS_InsertCall(loc, IPOINT_BEFORE, (AFUNPTR)Addn_tls<Types...>, IARG_FAST_ANALYSIS_CALL, IARG_THREAD_ID, args..., IARG_END);
              return;
            }
            else static_assert(false);
          }
          else if constexpr (N > 0)
          {
            counters_tls_instrumenter<LOC, N - 1>::helper<UINT32, UINT32, Types...>::apply(
              loc, indexes, values, IARG_UINT32, indexes[N - 1], IARG_UINT32, values[N - 1], args...);
          }
          else static_assert(false);
        }
      };

      template <class LOC>
      static void apply(LOC loc, UINT32* indexes, UINT32* values)
      {
        static_assert(N > 0);
        counters_tls_instrumenter<LOC, N - 1>::helper<UINT32, UINT32>::apply(
          loc, indexes, values, IARG_UINT32, indexes[N - 1], IARG_UINT32, values[N - 1]);
      }
    };

    template <class LOC, int N=1>
    int add_counters_tls(LOC loc, UINT32* indexes, UINT32* values, UINT32 nb_elts)
    {
      if (nb_elts < N)
      {
        std::cerr << "ERROR: nb_elts(" << nb_elts << ")<N(" << N << ")\n";
        PIN_WriteErrorMessage("ERROR: nb_elts<N)", 1000, PIN_ERR_FATAL, 0);
        PIN_ExitApplication(-1);
      }
      if constexpr(N < counters::size)
      {
        if (nb_elts > N)
        {
          return add_counters_tls<LOC, N + 1>(loc, indexes, values, nb_elts);
        }
      }
      counters_tls_instrumenter<LOC, N>::apply(loc, indexes, values);
      return N;
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
          if (nb_elts>0)
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
          if (nb_elts > 0)
          {
            nb_elts -= add_counters_tls<BBL>(bbl, indexes, values, nb_elts);
          }
        }
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