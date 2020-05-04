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
  REG tmp_reg;

  template <class OPERATION_IMPL>
  VOID overlaod_fp_operations(INS ins, VOID*)
  {
    auto xed = INS_XedDec(ins); // TODO check that pointer does not need to be freed
    auto iform = xed_decoded_inst_get_iform_enum(xed);

    switch (INS_Category(ins))
    {
    case xed_category_enum_t::XED_CATEGORY_SSE:
      replace::wrappers::sse::instrument<OPERATION_IMPL>(ins, xed, iform, tmp_reg);
      break;
    case xed_category_enum_t::XED_CATEGORY_AVX:
      replace::wrappers::avx::instrument<OPERATION_IMPL>(ins, xed, iform, tmp_reg);
      break;
    case xed_category_enum_t::XED_CATEGORY_AVX512:
      replace::wrappers::avx512::instrument<OPERATION_IMPL>(ins, xed, iform, tmp_reg);
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

      tmp_reg = PIN_ClaimToolRegister();

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
