#pragma once

#include "interflop_functions_wrapper.h"
#include "pin.H"
#include "xed-iclass-enum.h"

namespace interflop
{
  namespace internals
  {

    template <typename ops>
    bool insert_wrapper_function_AVX(INS ins)
    {
      if (INS_Category(ins) != XED_CATEGORY_AVX)
      {
        return false;
      }
      auto oc = INS_Opcode(ins);
      AFUNPTR func = nullptr;
      switch (oc)
      {
      case XED_ICLASS_VADDSS:
        func = (AFUNPTR)simd_op<float, ops::add_float, true, 32>;
        break;
      case XED_ICLASS_VADDPS:
        func = (AFUNPTR)simd_op<float, ops::add_float, false, 32>;
        break;

      case XED_ICLASS_VSUBSS:
        func = (AFUNPTR)simd_op<float, ops::sub_float, true, 32>;
        break;
      case XED_ICLASS_VSUBPS:
        func = (AFUNPTR)simd_op<float, ops::sub_float, false, 32>;
        break;

      case XED_ICLASS_VMULSS:
        func = (AFUNPTR)simd_op<float, ops::mul_float, true, 32>;
        break;
      case XED_ICLASS_VMULPS:
        func = (AFUNPTR)simd_op<float, ops::mul_float, false, 32>;
        break;

      case XED_ICLASS_VDIVSS:
        func = (AFUNPTR)simd_op<float, ops::div_float, true, 32>;
        break;
      case XED_ICLASS_VDIVPS:
        func = (AFUNPTR)simd_op<float, ops::div_float, false, 32>;
        break;


      case XED_ICLASS_VADDSD:
        func = (AFUNPTR)simd_op<double, ops::add_double, true, 32>;
        break;
      case XED_ICLASS_VADDPD:
        func = (AFUNPTR)simd_op<double, ops::add_double, false, 32>;
        break;

      case XED_ICLASS_VSUBSD:
        func = (AFUNPTR)simd_op<double, ops::sub_double, true, 32>;
        break;
      case XED_ICLASS_VSUBPD:
        func = (AFUNPTR)simd_op<double, ops::sub_double, false, 32>;
        break;

      case XED_ICLASS_VMULSD:
        func = (AFUNPTR)simd_op<double, ops::mul_double, true, 32>;
        break;
      case XED_ICLASS_VMULPD:
        func = (AFUNPTR)simd_op<double, ops::mul_double, false, 32>;
        break;

      case XED_ICLASS_VDIVSD:
        func = (AFUNPTR)simd_op<double, ops::div_double, true, 32>;
        break;
      case XED_ICLASS_VDIVPD:
        func = (AFUNPTR)simd_op<double, ops::div_double, false, 32>;
        break;

      default:
        return false;;
      }
      if (INS_OperandIsMemory(ins, 2))
      {
        INS_InsertCall(ins, IPOINT_BEFORE, func,
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
          IARG_MEMORYREAD_EA,
          IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
          IARG_END);
      }
      else
      {
        INS_InsertCall(ins, IPOINT_BEFORE, func,
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 2),
          IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
          IARG_END);
      }
      return true;
    }
  }
}
