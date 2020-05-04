#pragma once

#include "interflop_functions_wrapper.h"
#include <pin.H>
#include <xed-iclass-enum.h>
#include <xed-decoded-inst.h>
#include <xed-decoded-inst-api.h>
#include <iostream>

namespace interflop
{
  namespace internals
  {

    void print_error_details(INS ins, xed_iform_enum_t iform)
    {
      INT32 line, column;
      string file;
      PIN_GetSourceLocation(INS_Address(ins), &column, &line, &file);
      if (file != "")
      {
        std::cerr << file << "(" << decstr(line);
        if (column > 0)
        {
          std::cerr << "," << decstr(column);
        }
        std::cerr << "): ";
      }
      std::cerr << "WARNING: AVX512 instructions are not incorrectly instrumented. "
                << xed_iform_enum_t2str(iform);
    }

    REG get_zmm_register(REG reg)
    {
      switch (reg)
      {
      case REG::REG_PIN_XMM0:
      case REG::REG_PIN_YMM0:
      case REG::REG_PIN_ZMM0:
        return REG::REG_PIN_ZMM0;

      case REG::REG_PIN_XMM1:
      case REG::REG_PIN_YMM1:
      case REG::REG_PIN_ZMM1:
        return REG::REG_PIN_ZMM1;

      case REG::REG_PIN_XMM2:
      case REG::REG_PIN_YMM2:
      case REG::REG_PIN_ZMM2:
        return REG::REG_PIN_ZMM2;

      case REG::REG_PIN_XMM3:
      case REG::REG_PIN_YMM3:
      case REG::REG_PIN_ZMM3:
        return REG::REG_PIN_ZMM3;

      case REG::REG_PIN_XMM4:
      case REG::REG_PIN_YMM4:
      case REG::REG_PIN_ZMM4:
        return REG::REG_PIN_ZMM4;

      case REG::REG_PIN_XMM5:
      case REG::REG_PIN_YMM5:
      case REG::REG_PIN_ZMM5:
        return REG::REG_PIN_ZMM5;

      case REG::REG_PIN_XMM6:
      case REG::REG_PIN_YMM6:
      case REG::REG_PIN_ZMM6:
        return REG::REG_PIN_ZMM6;

      case REG::REG_PIN_XMM7:
      case REG::REG_PIN_YMM7:
      case REG::REG_PIN_ZMM7:
        return REG::REG_PIN_ZMM7;

      case REG::REG_PIN_XMM8:
      case REG::REG_PIN_YMM8:
      case REG::REG_PIN_ZMM8:
        return REG::REG_PIN_ZMM8;

      case REG::REG_PIN_XMM9:
      case REG::REG_PIN_YMM9:
      case REG::REG_PIN_ZMM9:
        return REG::REG_PIN_ZMM9;

      case REG::REG_PIN_XMM10:
      case REG::REG_PIN_YMM10:
      case REG::REG_PIN_ZMM10:
        return REG::REG_PIN_ZMM10;

      case REG::REG_PIN_XMM11:
      case REG::REG_PIN_YMM11:
      case REG::REG_PIN_ZMM11:
        return REG::REG_PIN_ZMM11;

      case REG::REG_PIN_XMM12:
      case REG::REG_PIN_YMM12:
      case REG::REG_PIN_ZMM12:
        return REG::REG_PIN_ZMM12;

      case REG::REG_PIN_XMM13:
      case REG::REG_PIN_YMM13:
      case REG::REG_PIN_ZMM13:
        return REG::REG_PIN_ZMM13;

      case REG::REG_PIN_XMM14:
      case REG::REG_PIN_YMM14:
      case REG::REG_PIN_ZMM14:
        return REG::REG_PIN_ZMM14;

      case REG::REG_PIN_XMM15:
      case REG::REG_PIN_YMM15:
      case REG::REG_PIN_ZMM15:
        return REG::REG_PIN_ZMM15;

      case REG::REG_PIN_XMM16:
      case REG::REG_PIN_YMM16:
      case REG::REG_PIN_ZMM16:
        return REG::REG_PIN_ZMM16;

      case REG::REG_PIN_XMM17:
      case REG::REG_PIN_YMM17:
      case REG::REG_PIN_ZMM17:
        return REG::REG_PIN_ZMM17;

      case REG::REG_PIN_XMM18:
      case REG::REG_PIN_YMM18:
      case REG::REG_PIN_ZMM18:
        return REG::REG_PIN_ZMM18;

      case REG::REG_PIN_XMM19:
      case REG::REG_PIN_YMM19:
      case REG::REG_PIN_ZMM19:
        return REG::REG_PIN_ZMM19;

      case REG::REG_PIN_XMM20:
      case REG::REG_PIN_YMM20:
      case REG::REG_PIN_ZMM20:
        return REG::REG_PIN_ZMM20;

      case REG::REG_PIN_XMM21:
      case REG::REG_PIN_YMM21:
      case REG::REG_PIN_ZMM21:
        return REG::REG_PIN_ZMM21;

      case REG::REG_PIN_XMM22:
      case REG::REG_PIN_YMM22:
      case REG::REG_PIN_ZMM22:
        return REG::REG_PIN_ZMM22;

      case REG::REG_PIN_XMM23:
      case REG::REG_PIN_YMM23:
      case REG::REG_PIN_ZMM23:
        return REG::REG_PIN_ZMM23;

      case REG::REG_PIN_XMM24:
      case REG::REG_PIN_YMM24:
      case REG::REG_PIN_ZMM24:
        return REG::REG_PIN_ZMM24;

      case REG::REG_PIN_XMM25:
      case REG::REG_PIN_YMM25:
      case REG::REG_PIN_ZMM25:
        return REG::REG_PIN_ZMM25;

      case REG::REG_PIN_XMM26:
      case REG::REG_PIN_YMM26:
      case REG::REG_PIN_ZMM26:
        return REG::REG_PIN_ZMM26;

      case REG::REG_PIN_XMM27:
      case REG::REG_PIN_YMM27:
      case REG::REG_PIN_ZMM27:
        return REG::REG_PIN_ZMM27;

      case REG::REG_PIN_XMM28:
      case REG::REG_PIN_YMM28:
      case REG::REG_PIN_ZMM28:
        return REG::REG_PIN_ZMM28;

      case REG::REG_PIN_XMM29:
      case REG::REG_PIN_YMM29:
      case REG::REG_PIN_ZMM29:
        return REG::REG_PIN_ZMM29;

      case REG::REG_PIN_XMM30:
      case REG::REG_PIN_YMM30:
      case REG::REG_PIN_ZMM30:
        return REG::REG_PIN_ZMM30;

      case REG::REG_PIN_XMM31:
      case REG::REG_PIN_YMM31:
      case REG::REG_PIN_ZMM31:
        return REG::REG_PIN_ZMM31;

      default:
        return REG::REG_INVALID_;
      }
    }

    REG get_ymm_register(REG reg)
    {
      switch (reg)
      {
      case REG::REG_PIN_XMM0:
      case REG::REG_PIN_YMM0:
        return REG::REG_PIN_YMM0;

      case REG::REG_PIN_XMM1:
      case REG::REG_PIN_YMM1:
        return REG::REG_PIN_YMM1;

      case REG::REG_PIN_XMM2:
      case REG::REG_PIN_YMM2:
        return REG::REG_PIN_YMM2;

      case REG::REG_PIN_XMM3:
      case REG::REG_PIN_YMM3:
        return REG::REG_PIN_YMM3;

      case REG::REG_PIN_XMM4:
      case REG::REG_PIN_YMM4:
        return REG::REG_PIN_YMM4;

      case REG::REG_PIN_XMM5:
      case REG::REG_PIN_YMM5:
        return REG::REG_PIN_YMM5;

      case REG::REG_PIN_XMM6:
      case REG::REG_PIN_YMM6:
        return REG::REG_PIN_YMM6;

      case REG::REG_PIN_XMM7:
      case REG::REG_PIN_YMM7:
        return REG::REG_PIN_YMM7;

      case REG::REG_PIN_XMM8:
      case REG::REG_PIN_YMM8:
        return REG::REG_PIN_YMM8;

      case REG::REG_PIN_XMM9:
      case REG::REG_PIN_YMM9:
        return REG::REG_PIN_YMM9;

      case REG::REG_PIN_XMM10:
      case REG::REG_PIN_YMM10:
        return REG::REG_PIN_YMM10;

      case REG::REG_PIN_XMM11:
      case REG::REG_PIN_YMM11:
        return REG::REG_PIN_YMM11;

      case REG::REG_PIN_XMM12:
      case REG::REG_PIN_YMM12:
        return REG::REG_PIN_YMM12;

      case REG::REG_PIN_XMM13:
      case REG::REG_PIN_YMM13:
        return REG::REG_PIN_YMM13;

      case REG::REG_PIN_XMM14:
      case REG::REG_PIN_YMM14:
        return REG::REG_PIN_YMM14;

      case REG::REG_PIN_XMM15:
      case REG::REG_PIN_YMM15:
        return REG::REG_PIN_YMM15;

      case REG::REG_PIN_XMM16:
      case REG::REG_PIN_YMM16:
        return REG::REG_PIN_YMM16;

      case REG::REG_PIN_XMM17:
      case REG::REG_PIN_YMM17:
        return REG::REG_PIN_YMM17;

      case REG::REG_PIN_XMM18:
      case REG::REG_PIN_YMM18:
        return REG::REG_PIN_YMM18;

      case REG::REG_PIN_XMM19:
      case REG::REG_PIN_YMM19:
        return REG::REG_PIN_YMM19;

      case REG::REG_PIN_XMM20:
      case REG::REG_PIN_YMM20:
        return REG::REG_PIN_YMM20;

      case REG::REG_PIN_XMM21:
      case REG::REG_PIN_YMM21:
        return REG::REG_PIN_YMM21;

      case REG::REG_PIN_XMM22:
      case REG::REG_PIN_YMM22:
        return REG::REG_PIN_YMM22;

      case REG::REG_PIN_XMM23:
      case REG::REG_PIN_YMM23:
        return REG::REG_PIN_YMM23;

      case REG::REG_PIN_XMM24:
      case REG::REG_PIN_YMM24:
        return REG::REG_PIN_YMM24;

      case REG::REG_PIN_XMM25:
      case REG::REG_PIN_YMM25:
        return REG::REG_PIN_YMM25;

      case REG::REG_PIN_XMM26:
      case REG::REG_PIN_YMM26:
        return REG::REG_PIN_YMM26;

      case REG::REG_PIN_XMM27:
      case REG::REG_PIN_YMM27:
        return REG::REG_PIN_YMM27;

      case REG::REG_PIN_XMM28:
      case REG::REG_PIN_YMM28:
        return REG::REG_PIN_YMM28;

      case REG::REG_PIN_XMM29:
      case REG::REG_PIN_YMM29:
        return REG::REG_PIN_YMM29;

      case REG::REG_PIN_XMM30:
      case REG::REG_PIN_YMM30:
        return REG::REG_PIN_YMM30;

      case REG::REG_PIN_XMM31:
      case REG::REG_PIN_YMM31:
        return REG::REG_PIN_YMM31;

      default:
        return REG::REG_INVALID_;
      }
    }

    VOID instrument_simd_instruction(INS ins, AFUNPTR func) 
    {
      if (INS_OperandIsMemory(ins, 3))
      {
        INS_InsertCall(ins, IPOINT_BEFORE, func,
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 2),
          IARG_MEMORYREAD_EA,
          IARG_REG_REFERENCE, get_zmm_register(INS_OperandReg(ins, 0)),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
          IARG_END);
      }
      else
      {
        INS_InsertCall(ins, IPOINT_BEFORE, func,
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 2),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 3),
          IARG_REG_REFERENCE, get_zmm_register(INS_OperandReg(ins, 0)),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
          IARG_END);
      }
    }

    template <typename ops>
    bool insert_wrapper_function_AVX512(INS ins)
    {
      if (INS_Category(ins) != XED_CATEGORY_AVX512)
      {
        return false;
      }

      auto xed = INS_XedDec(ins);
      auto iform = xed_decoded_inst_get_iform_enum(xed);
      bool zeroing = xed_decoded_inst_zeroing(xed);
      free(xed);
      bool has_mem_operand = false;
      constexpr int xmm = 16;
      constexpr int ymm = 32;
      constexpr int zmm = 64;
      AFUNPTR func = nullptr;

      switch (iform)
      {
      case xed_iform_enum_t::XED_IFORM_V4FMADDPS_ZMMf32_MASKmskw_ZMMf32_MEMf32_AVX512:
      case xed_iform_enum_t::XED_IFORM_V4FMADDSS_XMMf32_MASKmskw_XMMf32_MEMf32_AVX512:
      case xed_iform_enum_t::XED_IFORM_V4FNMADDPS_ZMMf32_MASKmskw_ZMMf32_MEMf32_AVX512:
      case xed_iform_enum_t::XED_IFORM_V4FNMADDSS_XMMf32_MASKmskw_XMMf32_MEMf32_AVX512:
        print_error_details(ins, iform);
        return false;

      case xed_iform_enum_t::XED_IFORM_VADDSS_XMMf32_MASKmskw_XMMf32_MEMf32_AVX512:
      case xed_iform_enum_t::XED_IFORM_VADDSS_XMMf32_MASKmskw_XMMf32_XMMf32_AVX512:
        if (zeroing)
          func = (AFUNPTR)simd_op<float, ops::add_float, true, xmm, zmm, false, true>;
        else
          func = (AFUNPTR)simd_op<float, ops::add_float, true, xmm, zmm, false, false>;
        instrument_simd_instruction(ins, func);
        return true;

      case xed_iform_enum_t::XED_IFORM_VADDPS_XMMf32_MASKmskw_XMMf32_MEMf32_AVX512:
      case xed_iform_enum_t::XED_IFORM_VADDPS_XMMf32_MASKmskw_XMMf32_XMMf32_AVX512:
        if (zeroing)
          func = (AFUNPTR)simd_op<float, ops::add_float, false, xmm, zmm, false, true>;
        else
          func = (AFUNPTR)simd_op<float, ops::add_float, false, xmm, zmm, false, false>;
        instrument_simd_instruction(ins, func);
        return true;

      case xed_iform_enum_t::XED_IFORM_VADDPS_YMMf32_MASKmskw_YMMf32_MEMf32_AVX512:
      case xed_iform_enum_t::XED_IFORM_VADDPS_YMMf32_MASKmskw_YMMf32_YMMf32_AVX512:
        if (zeroing)
          func = (AFUNPTR)simd_op<float, ops::add_float, false, ymm, zmm, false, true>;
        else
          func = (AFUNPTR)simd_op<float, ops::add_float, false, ymm, zmm, false, false>;
        instrument_simd_instruction(ins, func);
        return true;

      case xed_iform_enum_t::XED_IFORM_VADDPS_ZMMf32_MASKmskw_ZMMf32_MEMf32_AVX512:
      case xed_iform_enum_t::XED_IFORM_VADDPS_ZMMf32_MASKmskw_ZMMf32_ZMMf32_AVX512:
        if (zeroing)
          func = (AFUNPTR)simd_op<float, ops::add_float, false, zmm, zmm, false, true>;
        else
          func = (AFUNPTR)simd_op<float, ops::add_float, false, zmm, zmm, false, false>;
        instrument_simd_instruction(ins, func);
        return true;



      default:
        break;
      }


      auto oc = INS_Opcode(ins);
      switch (oc)
      {
       case XED_ICLASS_VADDSS:
        func = (AFUNPTR)simd_op<float, ops::add_float, true, 64, false>;
        break;
      case XED_ICLASS_VADDPS:
        func = (AFUNPTR)simd_op<float, ops::add_float, false, 64, false>;
        break;

      case XED_ICLASS_VSUBSS:
        func = (AFUNPTR)simd_op<float, ops::sub_float, true, 64, false>;
        break;
      case XED_ICLASS_VSUBPS:
        func = (AFUNPTR)simd_op<float, ops::sub_float, false, 64, false>;
        break;

      case XED_ICLASS_VMULSS:
        func = (AFUNPTR)simd_op<float, ops::mul_float, true, 64, false>;
        break;
      case XED_ICLASS_VMULPS:
        func = (AFUNPTR)simd_op<float, ops::mul_float, false, 64, false>;
        break;

      case XED_ICLASS_VDIVSS:
        func = (AFUNPTR)simd_op<float, ops::div_float, true, 64, false>;
        break;
      case XED_ICLASS_VDIVPS:
        func = (AFUNPTR)simd_op<float, ops::div_float, false, 64, false>;
        break;


      case XED_ICLASS_VADDSD:
        func = (AFUNPTR)simd_op<double, ops::add_double, true, 64, false>;
        break;
      case XED_ICLASS_VADDPD:
        func = (AFUNPTR)simd_op<double, ops::add_double, false, 64, false>;
        break;

      case XED_ICLASS_VSUBSD:
        func = (AFUNPTR)simd_op<double, ops::sub_double, true, 64, false>;
        break;
      case XED_ICLASS_VSUBPD:
        func = (AFUNPTR)simd_op<double, ops::sub_double, false, 64, false>;
        break;

      case XED_ICLASS_VMULSD:
        func = (AFUNPTR)simd_op<double, ops::mul_double, true, 64, false>;
        break;
      case XED_ICLASS_VMULPD:
        func = (AFUNPTR)simd_op<double, ops::mul_double, false, 64, false>;
        break;

      case XED_ICLASS_VDIVSD:
        func = (AFUNPTR)simd_op<double, ops::div_double, true, 64, false>;
        break;
      case XED_ICLASS_VDIVPD:
        func = (AFUNPTR)simd_op<double, ops::div_double, false, 64, false>;
        break;


      default:
        return false;;
      }

      INT32 line, column;
      string file;
      PIN_GetSourceLocation(INS_Address(ins), &column, &line, &file);
      if (file != "")
      {
        std::cerr << file << "(" << decstr(line);
        if (column > 0)
        {
          std::cerr << "," << decstr(column);
        }
        std::cerr << "): ";
      }
      std::cerr << "WARNING: AVX512 instructions might be incorrectly instrumented. " 
                << xed_iclass_enum_t2str((xed_iclass_enum_t)oc)
                ;
        
      for (int i = 0; i < INS_OperandCount(ins); ++i)
      {
        if (INS_OperandIsReg(ins, i))
        {
          std::cerr << " " << REG_StringShort(INS_OperandReg(ins, i));
        }
        else if (INS_OperandIsMemory(ins, i))
        {
          std::cerr << " mem";
        }
      }
      std::cerr << std::endl;
           
      if (INS_OperandIsMemory(ins, 2))
      {
        INS_InsertCall(ins, IPOINT_BEFORE, func,
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
          IARG_MEMORYREAD_EA,
          IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 2),
          IARG_END);
      }
      else
      {
        INS_InsertCall(ins, IPOINT_BEFORE, func,
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 3),
          IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 2),
          IARG_END);
      }
      return true;
    }
  }
}
