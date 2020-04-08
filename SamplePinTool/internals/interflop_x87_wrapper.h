#pragma once

#include "pin.H"
#include "interflop_functions_wrapper.h"
#include "xed-iclass-enum.h"

namespace interflop
{
  namespace internals
  {

    template <typename ops>
    bool insert_wrapper_function_x87(INS ins)
    {
      if (INS_Category(ins) != XED_CATEGORY_X87_ALU)
      {
        return false;
      }
      auto oc = INS_Opcode(ins);
      AFUNPTR func = nullptr;
      switch (oc)
      {
      case XED_ICLASS_FADD:
      case XED_ICLASS_FIADD:
      case XED_ICLASS_FADDP:
      case XED_ICLASS_FSCALE:
      case XED_ICLASS_FMUL:
      case XED_ICLASS_FMULP:
      case XED_ICLASS_FSUB:
      case XED_ICLASS_FSUBP:
      case XED_ICLASS_FSUBR:
      case XED_ICLASS_FSUBRP:
      case XED_ICLASS_FISUB:
      case XED_ICLASS_FISUBR:
      case XED_ICLASS_FIMUL:
      case XED_ICLASS_FDIV:
      case XED_ICLASS_FDIVP:
      case XED_ICLASS_FIDIV:
      case XED_ICLASS_FDIVR:
      case XED_ICLASS_FIDIVR:
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
          std::cerr << ")";
        }
        std::cerr << "Warning: x87 instructions are not instrumented. "
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
      }
      default:
        return false;
      }
    }
  }
}
