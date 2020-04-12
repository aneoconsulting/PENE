#include "counters_module.h"

#include <pin.H>

namespace pene {
  void Add(counters::int_type* a, UINT32 b)
  {
    *a += b;
  }

  void AddCountersInstrumentation(INS ins, void* void_counters)
  {
    auto counters_ = (counters*)void_counters;
    auto oc = INS_Opcode(ins);

    switch (oc)
    {
    case XED_ICLASS_ADDSS:
    case XED_ICLASS_SUBSS:
    case XED_ICLASS_VADDSS:
    case XED_ICLASS_VSUBSS:
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_PTR, &counters_->add_float_scalar, IARG_UINT32, (UINT32)1, IARG_END);
      break;
    case XED_ICLASS_ADDPS:
    case XED_ICLASS_SUBPS:
    case XED_ICLASS_VADDPS:
    case XED_ICLASS_VSUBPS:
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_PTR, &counters_->add_float_simd, IARG_UINT32, (UINT32)1, IARG_END);
      break;
    case XED_ICLASS_ADDSD:
    case XED_ICLASS_SUBSD:
    case XED_ICLASS_VADDSD:
    case XED_ICLASS_VSUBSD:
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_PTR, &counters_->add_double_scalar, IARG_UINT32, (UINT32)1, IARG_END);
      break;
    case XED_ICLASS_ADDPD:
    case XED_ICLASS_SUBPD:
    case XED_ICLASS_VADDPD:
    case XED_ICLASS_VSUBPD:
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_PTR, &counters_->add_double_simd, IARG_UINT32, (UINT32)1, IARG_END);
      break;
    case XED_ICLASS_MULSS:
    case XED_ICLASS_VMULSS:
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_PTR, &counters_->mul_float_scalar, IARG_UINT32, (UINT32)1, IARG_END);
      break;
    case XED_ICLASS_MULPS:
    case XED_ICLASS_VMULPS:
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_PTR, &counters_->mul_float_simd, IARG_UINT32, (UINT32)1, IARG_END);
      break;
    case XED_ICLASS_MULSD:
    case XED_ICLASS_VMULSD:
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_PTR, &counters_->mul_double_scalar, IARG_UINT32, (UINT32)1, IARG_END);
      break;
    case XED_ICLASS_MULPD:
    case XED_ICLASS_VMULPD:
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_PTR, &counters_->mul_double_simd, IARG_UINT32, (UINT32)1, IARG_END);
      break;
    case XED_ICLASS_DIVSS:
    case XED_ICLASS_VDIVSS:
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_PTR, &counters_->div_float_scalar, IARG_UINT32, (UINT32)1, IARG_END);
      break;
    case XED_ICLASS_DIVPS:
    case XED_ICLASS_VDIVPS:
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_PTR, &counters_->div_float_simd, IARG_UINT32, (UINT32)1, IARG_END);
      break;
    case XED_ICLASS_DIVSD:
    case XED_ICLASS_VDIVSD:
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_PTR, &counters_->div_double_scalar, IARG_UINT32, (UINT32)1, IARG_END);
      break;
    case XED_ICLASS_DIVPD:
    case XED_ICLASS_VDIVPD:
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Add, IARG_PTR, &counters_->div_double_simd, IARG_UINT32, (UINT32)1, IARG_END);
      break;
    default:
      break;
    }
  }

  counters_module::counters_module() :module(), c() {}

  void counters_module::init()
  {
    INS_AddInstrumentFunction(AddCountersInstrumentation, &c);
  }

  void counters_module::end()
  {
    PIN_AddFiniFunction([](INT32, void* void_counters) {((counters*)void_counters)->print(); }, &c);
  }
}