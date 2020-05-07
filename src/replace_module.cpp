//#include <pin.H>
#include <iostream>
#include <cassert>

#include "replace_module.h"

#include "replace/backend/invert_add_mul_impl.h"
#include "replace/wrappers/sse.h"
#include "replace/wrappers/avx.h"
#include "replace/wrappers/avx512.h"

#include "pin_utils/insert_call.h"
#include "pin_utils/instrumenter.h"



namespace pene {
  using namespace pin_utils;
  namespace replace_module_internals
  {
    enum replace_modes : int
    {
      NONE = 0,
      IEEE,
      DOUBLE2FLOAT,
      RANDOM_ROUNDING,
      END_PUBLIC,
      DEBUG = 100,
      DEBUG_FLOAT_ADD_MULL_SWAP,
      DEBUG_ADD_MULL_SWAP,
      DEBUG_END
    };

    template<class OPERATION_IMPL, class BASE_INSTRUMENTER = base_instrumenter>
    class replace_instrumenters : public BASE_INSTRUMENTER
    {
      REG tmp_reg1;
      REG tmp_reg2;
      void* backend_ctx;

    public:
      replace_instrumenters()
        : BASE_INSTRUMENTER()
        , tmp_reg1(PIN_ClaimToolRegister())
        , tmp_reg2(PIN_ClaimToolRegister())
        , backend_ctx(OPERATION_IMPL::init())
      {
        if (!REG_valid(tmp_reg1) || !REG_valid(tmp_reg2))
        {
          std::cerr << "Cannot allocate a scratch register.\n";
          std::cerr << std::flush;
          PIN_ExitApplication(1);
        }
      }
      void instrument(INS ins) {
        switch (INS_Category(ins))
        {
        case xed_category_enum_t::XED_CATEGORY_SSE:
          replace::wrappers::sse::instrument<OPERATION_IMPL>(backend_ctx, ins);
          break;
        case xed_category_enum_t::XED_CATEGORY_AVX:
          replace::wrappers::avx::instrument<OPERATION_IMPL>(backend_ctx, ins, tmp_reg1, tmp_reg2);
          break;
        case xed_category_enum_t::XED_CATEGORY_AVX512:
          replace::wrappers::avx512::instrument<OPERATION_IMPL>(backend_ctx, ins, tmp_reg1, tmp_reg2);
          break;
        default:
          break;
        }
      }
    };
  }



  replace_module::replace_module() 
    : module()
    , knob_replace(KNOB_MODE_WRITEONCE, "pintool", "fp-replace", "0", "Enables fp operation replacement.\n\t0: disabled\n\t1: ieee\n\t3: double2float\n\t4: random rounding")
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
    auto mode = knob_replace.Value();
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
    auto mode = (replace_module_internals::replace_modes)knob_replace.Value();
    switch (mode) {
    case replace_module_internals::replace_modes::NONE:
      std::cerr << "fp-replace None mode : no instrumenation" << std::endl;
      break;
    case replace_module_internals::replace_modes::IEEE:
      std::cerr << "fp-replace ieee mode : instrumentation should not change anything" << std::endl;
      std::cerr << "ieee mode not working yet. Exiting now" << std::endl;
      PIN_WriteErrorMessage("ieee mode not working yet. Exiting now", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
      PIN_ExitApplication(1);
      break;
    case replace_module_internals::replace_modes::DOUBLE2FLOAT:
      std::cerr << "fp-replace double2float mode : double precision instructions are replaced by single precision ones" << std::endl;
      std::cerr << "double2float mode not working yet. Exiting now" << std::endl;
      PIN_WriteErrorMessage("double2float mode not working yet. Exiting now", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
      PIN_ExitApplication(1);
      break;
    case replace_module_internals::replace_modes::RANDOM_ROUNDING:
      std::cerr << "fp-replace random rounding mode : rounding mode is randomly changed for each instruction" << std::endl;
      std::cerr << "random rounding mode not working yet. Exiting now" << std::endl;
      PIN_WriteErrorMessage("random rounding mode not working yet. Exiting now", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
      PIN_ExitApplication(1);
      break;
    case replace_module_internals::replace_modes::DEBUG_FLOAT_ADD_MULL_SWAP:
      std::cerr << "fp-replace single precision debug mode - swapping single precision additions and multiplications" << std::endl;
      data = new replace_module_internals::replace_instrumenters< replace::backend::invert_add_mul_impl, base_instrumenter>();
      data->INS_AddInstrumentFunction();
      break;
    case replace_module_internals::replace_modes::DEBUG_ADD_MULL_SWAP:
      std::cerr << "fp-replace fp debug mode - swapping fp dditions and multiplications" << std::endl;
      std::cerr << "fp debug mode not working yet. Exiting now" << std::endl;
      PIN_WriteErrorMessage("fp debug mode not working yet. Exiting now", 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
      PIN_ExitApplication(1);
      break;
    default: // case 0
      std::cerr << "no replacement mode" << std::endl;
      break;
    }
  }

  const std::string& replace_module::name() {
    static const std::string name_{ "replace_module" };
    return name_;
  }
}
