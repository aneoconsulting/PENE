#pragma once

#include "interflop_functions_wrapper.h"
#include "pin.H"
#include "xed-iclass-enum.h"

namespace interflop 
{
  namespace internals 
  {

    template <typename ops>
    bool insert_wrapper_function_SSE(INS ins)
    {
      if (INS_Category(ins) != XED_CATEGORY_SSE)
      {
        return false;
      }

      auto oc = INS_Opcode(ins);
      AFUNPTR func = nullptr;
      switch (oc)
      {
      case XED_ICLASS_ADDSS:
        func = (AFUNPTR)simd_op<float, ops::add_float, true, 16>;
        break;
      case XED_ICLASS_ADDPS:
        func = (AFUNPTR)simd_op<float, ops::add_float, false, 16>;
        break;

      case XED_ICLASS_SUBSS:
        func = (AFUNPTR)simd_op<float, ops::sub_float, true, 16>;
        break;
      case XED_ICLASS_SUBPS:
        func = (AFUNPTR)simd_op<float, ops::sub_float, false, 16>;
        break;

      case XED_ICLASS_MULSS:
        func = (AFUNPTR)simd_op<float, ops::mul_float, true, 16>;
        break;
      case XED_ICLASS_MULPS:
        func = (AFUNPTR)simd_op<float, ops::mul_float, false, 16>;
        break;

      case XED_ICLASS_DIVSS:
        func = (AFUNPTR)simd_op<float, ops::div_float, true, 16>;
        break;
      case XED_ICLASS_DIVPS:
        func = (AFUNPTR)simd_op<float, ops::div_float, false, 16>;
        break;


      case XED_ICLASS_ADDSD:
        func = (AFUNPTR)simd_op<double, ops::add_double, true, 16>;
        break;
      case XED_ICLASS_ADDPD:
        func = (AFUNPTR)simd_op<double, ops::add_double, false, 16>;
        break;

      case XED_ICLASS_SUBSD:
        func = (AFUNPTR)simd_op<double, ops::sub_double, true, 16>;
        break;
      case XED_ICLASS_SUBPD:
        func = (AFUNPTR)simd_op<double, ops::sub_double, false, 16>;
        break;

      case XED_ICLASS_MULSD:
        func = (AFUNPTR)simd_op<double, ops::mul_double, true, 16>;
        break;
      case XED_ICLASS_MULPD:
        func = (AFUNPTR)simd_op<double, ops::mul_double, false, 16>;
        break;

      case XED_ICLASS_DIVSD:
        func = (AFUNPTR)simd_op<double, ops::div_double, true, 16>;
        break;
      case XED_ICLASS_DIVPD:
        func = (AFUNPTR)simd_op<double, ops::div_double, false, 16>;
        break;

      default:
        return false;;
      }
      if (INS_OperandIsMemory(ins, 1))
      {
        INS_InsertCall(ins, IPOINT_BEFORE, func,
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
          IARG_MEMORYREAD_EA,
          IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
          IARG_END);
      }
      else
      {
        INS_InsertCall(ins, IPOINT_BEFORE, func,
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
          IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
          IARG_END);
      }
      return true;
    }
  }
}
