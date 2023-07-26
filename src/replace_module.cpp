//#include <pin.H>
#include <iostream>
#include <cassert>
#include <time.h>



#include "replace_module.h"
#include "extern_file.h"

#include "replace/backend/invert_add_mul_impl.h"
#include "replace/backend/ieee.h"
#include "replace/backend/double2float.h"
#include "replace/backend/test_backend.h"
#include "replace/backend/backend_verrou/interflop_verrou.h"
#include "replace/wrappers/sse.h"
#include "replace/wrappers/avx.h"
#include "replace/wrappers/avx512.h"
#include "replace/wrappers/fma.h"

#include "pin_utils/insert_call.h"
#include "pin_utils/instrumenter.h" 
#include "pin_utils/symbol_filters.h"



namespace pene {
  using namespace pin_utils;
  
  
  namespace replace_module_internals
  {

    
    enum replace_modes : int
    {
      NONE = 0,
      IEEE,
      DOUBLE2FLOAT,
      VERROU,
      TEST,
      END_PUBLIC,
      DEBUG = 100,
      DEBUG_FLOAT_ADD_MULL_SWAP,
      DEBUG_ADD_MULL_SWAP,
      DEBUG_END
    };
    enum rounding_modes : int{
      NOTSPECIFIED = 0,
      NEAREST,
      UPWARD,
      DOWNWARD,
      ZERO,
      RANDOM, 
      RANDOM_DET,
      RANDOM_COMDET,
      AVERAGE,
      AVERAGE_DET,
      AVERAGE_COMDET,
      FARTHEST,
      FLOAT,
      NATIVE,
      FTZ
    };

    template<class OPERATION_IMPL>
    class replace_inst_instrumenters final : public element_instrumenter
    {
      REG tmp_reg1;
      REG tmp_reg2;
      REG tmp_reg_output;
  
    public:
      void* backend_ctx;
      replace_inst_instrumenters()
        : element_instrumenter()
        , tmp_reg1(PIN_ClaimToolRegister())
        , tmp_reg2(PIN_ClaimToolRegister())
        , tmp_reg_output(PIN_ClaimToolRegister())
        //, backend_ctx(OPERATION_IMPL::init())
      {
           //struct pene::replace::backend::verrou ifverrou=pene::replace::backend::init(&backend_ctx);
        if (!REG_valid(tmp_reg1) || !REG_valid(tmp_reg2) || !REG_valid(tmp_reg_output))
        {
          std::cerr << "Cannot allocate a scratch register.\n";
          std::cerr << std::flush;
          PIN_ExitApplication(1);
        }
      }
      virtual void instrument(INS ins) override {
        switch (INS_Category(ins))
        {
        case xed_category_enum_t::XED_CATEGORY_SSE:
          replace::wrappers::sse::instrument<OPERATION_IMPL>(backend_ctx, ins);
          break;
        case xed_category_enum_t::XED_CATEGORY_AVX:
          replace::wrappers::avx::instrument<OPERATION_IMPL>(backend_ctx, ins);
          break;
        case xed_category_enum_t::XED_CATEGORY_AVX512:
          replace::wrappers::avx512::instrument<OPERATION_IMPL>(backend_ctx, ins);
          break;
        case xed_category_enum_t::XED_CATEGORY_VFMA:
          replace::wrappers::fma::instrument<OPERATION_IMPL>(backend_ctx, ins);
          break;
        default:
          break;
        }
      }
    };
  }



  replace_module::replace_module() 
    : module(true)
    , knob_replace_mode(KNOB_MODE_WRITEONCE, "pintool", "fp-replace", "0", "Enables fp operation replacement.\n\t0: disabled\n\t1: ieee\n\t2: double2float\n\t3: random rounding")
    , knob_exl_symbols(KNOB_MODE_WRITEONCE, "pintool", "exclude", "", "When this option is present, symbols listed in file will be left uninstrumented")
    , knob_incl_source_lines(KNOB_MODE_WRITEONCE, "pintool", "source", "", "When this option is present, only instructions coming from source code lines listed in file are instrumented. [NOT WORKING YET]")
    , knob_rounding_mode(KNOB_MODE_WRITEONCE, "pintool","rounding-mode", "0", "specify rouding mode for verrou backend")
    , data(nullptr)
  {}

  replace_module::~replace_module()
  {
    if (data != nullptr)
    {
      delete data;
    }
  }

  bool replace_module::validate() {
    {
      auto filename = knob_exl_symbols.Value();

      if (!filename.empty())
      {
        std::ifstream is(filename.c_str());
        if (!is.is_open())
        {
          std::string error_mess = "ERROR: Could not create file ";
          error_mess.append(filename);
          std::cerr << error_mess << std::endl;
          PIN_WriteErrorMessage(error_mess.c_str(), 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_NONFATAL, 0);
          return false;
        }
        std::cerr << " OK" << std::endl;
      }
    }
    {
      auto filename = knob_incl_source_lines.Value();

      if (!filename.empty())
      {
        std::ifstream is(filename.c_str());
        if (!is.is_open())
        {
          std::string error_mess = "ERROR: Could not create file ";
          error_mess.append(filename);
          std::cerr << error_mess << std::endl;
          PIN_WriteErrorMessage(error_mess.c_str(), 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_NONFATAL, 0);
          return false;
        }
        std::cerr << " OK" << std::endl;
      }
    }

    auto mode = knob_replace_mode.Value();
    std::cerr << "Checking configuration: instruction replacement mode with fp-replace=" << mode << std::endl;
    if (mode >=0 && mode < replace_module_internals::replace_modes::END_PUBLIC)
    {
      return true;
    }
    if (mode > replace_module_internals::replace_modes::DEBUG && mode < replace_module_internals::replace_modes::DEBUG_END)
    {
      return true;
    }
    // option 100 shall not be declared in the command help
    std::cerr << "ERROR: -fp-replace option only accepts values 0" << std::endl;
    PIN_WriteErrorMessage("ERROR: -fp-replace option only accepts values 0", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_NONFATAL, 0);
    return false;
  }

  void replace_module::init() {
    std::cerr << "Initialization: replace module." << std::endl;
    filter* filter = nullptr;
    if (!knob_exl_symbols.Value().empty())
    {
      std::cerr << "Instructions corresponding to symbols in file will not be instrumented." << std::endl;
      filter = new symbol_exclude_filter(knob_exl_symbols.Value());
    }

    if (!knob_incl_source_lines.Value().empty())
    {
      std::cerr << "Only instructions corresponding to source lines in file will be instrumented." << std::endl;
      std::cerr << "source line based instrumentation is not implemented yet. Exiting now." << std::endl;
      PIN_WriteErrorMessage("source line based instrumentation is not implemented yet. Exiting now."
        , 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
      PIN_ExitApplication(1);
    }

    if (filter == nullptr)
    {
      filter = new null_filter();
    }



    auto mode = (replace_module_internals::replace_modes)knob_replace_mode.Value();
    auto rounding_mode = (replace_module_internals::rounding_modes)knob_rounding_mode.Value();
    switch (mode) {
    case replace_module_internals::replace_modes::NONE:
      {
      std::cerr << "fp-replace None mode : no instrumentation" << std::endl;
      break;
      }
    case replace_module_internals::replace_modes::IEEE:
      {
      std::cerr << "fp-replace ieee mode : instrumentation should not change anything" << std::endl;
      data = new instrumenter(new replace_module_internals::replace_inst_instrumenters<replace::backend::ieee>(), filter);
      data->TRACE_AddInstrumentFunction();
      break;
      }
    case replace_module_internals::replace_modes::DOUBLE2FLOAT:
      {
      std::cerr << "fp-replace double2float mode : double precision instructions are replaced by single precision ones" << std::endl;
      data = new instrumenter(new replace_module_internals::replace_inst_instrumenters<replace::backend::double2float>(), filter);
      data->TRACE_AddInstrumentFunction();
      break;
      }
    case replace_module_internals::replace_modes::VERROU:
      {
      auto verrou_replace_instrmenter = new replace_module_internals::replace_inst_instrumenters<typename replace::backend::verrou>();
      srand((uint)time(nullptr));
      uint seed=(uint)rand();
      pene::replace::backend::verrou_set_seed (seed);
      if (rounding_mode == replace_module_internals::rounding_modes::NOTSPECIFIED)
        {
          std::cerr << "fp-replace mode : verrou, but rounding mode was not specified, the default is nearest" << std::endl;
          pene::replace::backend::configure(pene::replace::backend::VR_NEAREST, verrou_replace_instrmenter->backend_ctx);
        }
      else 
        if(rounding_mode == replace_module_internals::rounding_modes::NEAREST)
         {
          std::cerr << "fp-replace mode : verrou with nearest rounding mode " << std::endl;
          pene::replace::backend::configure(pene::replace::backend::VR_NEAREST, verrou_replace_instrmenter->backend_ctx);
        }
      else
        if(rounding_mode == replace_module_internals::rounding_modes::UPWARD)
        {
          std::cerr << "fp-replace mode : verrou with upward rounding mode " << std::endl;
          pene::replace::backend::configure(pene::replace::backend::VR_UPWARD, verrou_replace_instrmenter->backend_ctx);
        }
      else
        if(rounding_mode == replace_module_internals::rounding_modes::DOWNWARD)
        {
          std::cerr << "fp-replace mode : verrou with downward rounding mode " << std::endl;
          pene::replace::backend::configure(pene::replace::backend::VR_DOWNWARD, verrou_replace_instrmenter->backend_ctx);
        }
      else
        if(rounding_mode == replace_module_internals::rounding_modes::DOWNWARD)
        {
          std::cerr << "fp-replace mode : verrou with downward rounding mode " << std::endl;
          pene::replace::backend::configure(pene::replace::backend::VR_DOWNWARD, verrou_replace_instrmenter->backend_ctx);
        }
      else
        if(rounding_mode == replace_module_internals::rounding_modes::ZERO)
        {
          std::cerr << "fp-replace mode : verrou with downward rounding mode " << std::endl;
          pene::replace::backend::configure(pene::replace::backend::VR_ZERO, verrou_replace_instrmenter->backend_ctx);
        }
      else
        if(rounding_mode == replace_module_internals::rounding_modes::RANDOM)
        {
          std::cerr << "fp-replace mode : verrou with random rounding mode " << std::endl;
          std::cerr << "Seed " << seed << std::endl;
          pene::replace::backend::configure(pene::replace::backend::VR_RANDOM, verrou_replace_instrmenter->backend_ctx);
        }
        
      
      //pene::replace::backend::configure(pene::replace::backend::VR_NEAREST,context);
      data = new instrumenter(verrou_replace_instrmenter, filter);
      data->TRACE_AddInstrumentFunction();

      //PIN_WriteErrorMessage("random rounding mode not working yet. Exiting now", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
      //PIN_ExitApplication(1);
      break;
      }
    case replace_module_internals::replace_modes::DEBUG_FLOAT_ADD_MULL_SWAP:
      {
      std::cerr << "fp-replace single precision debug mode - swapping single precision additions and multiplications" << std::endl;
      OutFile << "fp-replace single precision debug mode - swapping single precision additions and multiplications" << std::endl;
      data = new instrumenter(new replace_module_internals::replace_inst_instrumenters<replace::backend::invert_add_mul_float_impl>(), filter);
      data->TRACE_AddInstrumentFunction();
      break;
      }
    case replace_module_internals::replace_modes::DEBUG_ADD_MULL_SWAP:
      {
      std::cerr << "fp-replace fp debug mode - swapping fp additions and multiplications" << std::endl;
      data = new instrumenter(new replace_module_internals::replace_inst_instrumenters<replace::backend::invert_add_mul_impl>(), filter);
      data->TRACE_AddInstrumentFunction();
      break;
      }
    case replace_module_internals::replace_modes::TEST:
       {
       std::cerr << "fp-replace fp test mode -  using test backend" << std::endl;
       data = new instrumenter(new replace_module_internals::replace_inst_instrumenters<replace::backend::test_backend>(), filter);
       data->TRACE_AddInstrumentFunction();
       break;
       }
    default:
      std::cerr << "no replacement mode" << std::endl;
      break;
    }
  }

  const std::string& replace_module::name()
  {
    static const std::string name_("replace_module");
    return name_;
  }
}
