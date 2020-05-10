#include "counters_module.h"
#include "counters_utils.h"

#include <pin.H>
#include <iostream>
#include <cassert>
#include "pin_utils/insert_call.h"
#include "pin_utils/instrumenter.h"
#include "pin_utils/tls_reg.h"
#include "utils/move.h"

namespace pene {
  using namespace pin_utils;
  namespace counter_module_internals {

    class counters_element_instrumenters final : public element_instrumenter
    {
      class tls_life_cycle_manager : public tls<counters>::life_cycle_manager {
        counters& c;
      public:
        tls_life_cycle_manager(counters& c) :c(c) {}
        virtual counters* create() override
        {
          return new counters();
        }
        virtual void destroy(counters* local_counters) override
        {
          for (int i = 0; i < counters::size; ++i)
          {
            c.array[i] += local_counters->array[i];
          }
          delete local_counters;
        }
      };

      TRACE trace;
      BBL bbl;
      INS ins;
      counters c;
      tls_life_cycle_manager tlcm;
      pin_utils::tls_reg<counters> * tls;
      bool use_tls;
      counters tmp_counters;

    public:
      counters_element_instrumenters()
        : element_instrumenter()
        , trace(nullptr)
        , bbl(MAKE_BBL(0))
        , ins(MAKE_INS(0))
        , c(), tlcm(c), tls(nullptr), use_tls(false)
        , tmp_counters()
      {
        PIN_AddFiniFunction([](INT32, void* void_counters) {((counters*)void_counters)->print(); }, &c);
      }

      ~counters_element_instrumenters()
      {
        if (tls)
          delete tls;
        tls = nullptr;
      }

      void activate_tls()
      {
        if (tls == nullptr)
        {
          tls = new pin_utils::tls_reg<counters>(tlcm);
        }
        use_tls = true;
        std::cerr << "TLS has been activated for counters." << std::endl;
      }

      void init_instrument(TRACE) { }

      void init_instrument(BBL bbl_) 
      {
        bbl = bbl_;
        tmp_counters = counters{};
      }

      void instrument(INS ins_)
      {
        ins = ins_;

        if (INS_IsOriginal(ins))
        {
          auto oc = INS_Opcode(ins);
          update_counters(oc, tmp_counters);
        }
      }

      void end_instrument(BBL) 
      { 
        for (UINT32 i = 0; i < counters::size; ++i)
        {
          if (tmp_counters.array[i] > 0)
          {
            if (!use_tls)
            {
              dispatch_insertCall_bbl_ins((AFUNPTR)Add, IARG_FAST_ANALYSIS_CALL, IARG_PTR, &(c.array[i]), IARG_UINT32, (UINT32)tmp_counters.array[i], IARG_END);
            }
            else
            {
              add_counters_tls(i);
            }
          }
        }
      }

      void end_instrument(TRACE) {}

    private:
      template<class ... Args>
      VOID dispatch_insertCall_bbl_ins(Args... args)
      {
        if (trace != nullptr)
        {
          insertCall(trace, IPOINT_ANYWHERE, args...);
        }
        else if (bbl.is_valid())
        {
          insertCall(bbl, IPOINT_ANYWHERE, args...);
        }
        else
        {
          insertCall(ins, IPOINT_BEFORE, args...);
        }
      }

      template <int N = 0>
      void add_counters_tls(UINT32 index)
      {
        if (index < N)
        {
          std::cerr << "ERROR: index(" << index << ")<N(" << N << ")\n";
          PIN_WriteErrorMessage("ERROR: index<N)", 1000, PIN_ERR_FATAL, 0);
          PIN_ExitApplication(-1);
        }
        if constexpr (N < counters::size)
        {
          if (index > N)
          {
            add_counters_tls<N + 1>(index);
            return;
          }
        }
        dispatch_insertCall_bbl_ins((AFUNPTR)Addi<N>, IARG_FAST_ANALYSIS_CALL, IARG_REG_VALUE, tls->get_reg(), IARG_UINT32, (UINT32)tmp_counters.array[index], IARG_END);
      }

      // cannot be lighter than this. avoids all unnecessary adress offset calculations
      static void PIN_FAST_ANALYSIS_CALL Add(void* a_, UINT32 b)
      {
        volatile auto a = reinterpret_cast<counters::int_type*>(a_);
        *a += b;
      }

      // I could be a function argument. This approach ends up using less registers for function call
      // Hopefully, pin will be able to perform a less expensive code insertion
      template<int I>
      static void PIN_FAST_ANALYSIS_CALL Addi(void* voided_counters, UINT32 b)
      {
        volatile auto c = static_cast<counters*>(voided_counters);
        Add(c->array + I, b);
      }
    };
  }

  using namespace counter_module_internals;

  counters_module::counters_module()
    : module(), knob_counter_instrumentation_mode(KNOB_MODE_WRITEONCE, "pintool"
    , "counter-mode", "1", "Activate floating point instruction counting. 0: no counter, 1: fast counter, 2: slow counter (for debug purpose).")
    , knob_counters_use_tls(KNOB_MODE_WRITEONCE, "pintool", "counter-tls", "0", "Activate use of the Thread Local Storage (TLS) for the counters")
    , data(nullptr)
  {}

  counters_module::~counters_module()
  {
    if (data != nullptr)
      delete reinterpret_cast<counters_element_instrumenters*>(data);
    data = 0;
  }

  bool counters_module::validate() 
  {
    auto mode = knob_counter_instrumentation_mode.Value();
    std::cerr << "Checking configuration: FP instructions count with counter-mode=" << mode << std::endl;
    if (mode > 2)
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
    counters_element_instrumenters* cei = nullptr;
    switch (mode)
    {
    case 1:
      cei = new counters_element_instrumenters();
      data = new instrumenter(cei);
      std::cerr << "Set counters to \"trace\" instrumentation mode." << std::endl;
      data->TRACE_AddInstrumentFunction();
      break;
    case 2:
      cei = new counters_element_instrumenters();
      data = new instrumenter(cei);
      std::cerr << "Set counters to \"instructions\" instrumentation mode." << std::endl;
      data->INS_AddInstrumentFunction();
      break;
    default: // case 0
      std::cerr << "Set counters to no instrumentation mode" << std::endl;
      return;
    }

    if (cei!=nullptr && knob_counters_use_tls.Value())
    {
      cei->activate_tls();
    }
  }

  const std::string&& counters_module::name() { 
    return tr1::move(std::string("counters_module"));
  }
}