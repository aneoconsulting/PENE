#include <pin.H>
#include <iostream>
#include <cassert>
#include <xed-decoded-inst-api.h>

#include "replace_module.h"

#include "replace/backend/invert_add_mul_impl.h"
#include "replace/wrappers/sse.h"
#include "replace/wrappers/avx.h"
#include "replace/wrappers/avx512.h"



namespace pene {
  REG tmp_reg1;
  REG tmp_reg2;

  template <class OPERATION_IMPL>
  VOID overlaod_fp_operations(INS ins, VOID*)
  {


    switch (INS_Category(ins))
    {
    case xed_category_enum_t::XED_CATEGORY_SSE:
      replace::wrappers::sse::instrument<OPERATION_IMPL>(ins);
      break;
    case xed_category_enum_t::XED_CATEGORY_AVX:
      replace::wrappers::avx::instrument<OPERATION_IMPL>(ins, tmp_reg1, tmp_reg2);
      break;
    case xed_category_enum_t::XED_CATEGORY_AVX512:
      replace::wrappers::avx512::instrument<OPERATION_IMPL>(ins, tmp_reg1);
      break;
    default:
      break;
    }
  }


  replace_module::replace_module() : module(), knob_replace(KNOB_MODE_WRITEONCE, "pintool",
    "replace", "0", "switch add and multiply") 
  {}

  void replace_module::init() {
    std::cerr << "Initialization: replace module." << std::endl;
    auto mode = knob_replace.Value();
    switch (mode) {
    case 1:
      std::cerr << "switching add and multiply" << std::endl;
      INS_AddInstrumentFunction(overlaod_fp_operations<replace::backend::invert_add_mul_impl>, nullptr);
      std::cerr << "Switch compelet." << std::endl;

      tmp_reg1 = PIN_ClaimToolRegister();
      if (!REG_valid(tmp_reg1))
      {
        std::cerr << "Cannot allocate a scratch register.\n";
        std::cerr << std::flush;
        PIN_ExitApplication(1);
      }
      tmp_reg2 = PIN_ClaimToolRegister();
      if (!REG_valid(tmp_reg2))
      {
        std::cerr << "Cannot allocate a scratch register.\n";
        std::cerr << std::flush;
        PIN_ExitApplication(1);
      }

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
