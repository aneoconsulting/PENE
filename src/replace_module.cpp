#include <pin.H>
#include <iostream>
#include <cassert>
#include "replace_module.h"


namespace pene {

  replace_module::replace_module() : module(), knob_replace(KNOB_MODE_WRITEONCE, "pintool",
    "replace", "0", "switch add and multiply") {
  }

  void product_sse(const PIN_REGISTER* ra, const PIN_REGISTER* rb, PIN_REGISTER* rc) {
    rc->flt[0] = ra->flt[0] * rb->flt[0];
  }
  void sum_sse(const PIN_REGISTER* ra, const PIN_REGISTER* rb, PIN_REGISTER* rc) {
    rc->flt[0] = ra->flt[0] + rb->flt[0];
  }

  void product_avx(const PIN_REGISTER* ra, const PIN_REGISTER* rb, PIN_REGISTER* rc) {
    rc->flt[0] = ra->flt[0] * rb->flt[0];
    rc->flt[1] = ra->flt[1];
    rc->flt[2] = ra->flt[2];
    rc->flt[3] = ra->flt[3];
    rc->flt[4] = 0;
    rc->flt[5] = 0;
    rc->flt[6] = 0;
    rc->flt[7] = 0;
    rc->flt[8] = 0;
    rc->flt[9] = 0;
    rc->flt[10] = 0;
    rc->flt[11] = 0;
    rc->flt[12] = 0;
    rc->flt[13] = 0;
    rc->flt[14] = 0;
    rc->flt[15] = 0;

  }
  void sum_avx(const PIN_REGISTER* ra, const PIN_REGISTER* rb, PIN_REGISTER* rc) {
    rc->flt[0] = ra->flt[0] + rb->flt[0];
    rc->flt[1] = ra->flt[1];
    rc->flt[2] = ra->flt[2];
    rc->flt[3] = ra->flt[3];
    rc->flt[4] = 0;
    rc->flt[5] = 0;
    rc->flt[6] = 0;
    rc->flt[7] = 0;
    rc->flt[8] = 0;
    rc->flt[9] = 0;
    rc->flt[10] = 0;
    rc->flt[11] = 0;
    rc->flt[12] = 0;
    rc->flt[13] = 0;
    rc->flt[14] = 0;
    rc->flt[15] = 0;
  }


  void Switch_add_multiply(INS ins, VOID* v) {
    //if (INS_IsOriginal(ins)) 
    {
        auto oc = INS_Opcode(ins);

        switch (oc) {
        //case XED_ICLASS_ADDSD:
        //case XED_ICLASS_VADDSD:
        case XED_ICLASS_VADDSS:
            std::cerr << "vaddss case" << std::endl;
            if (INS_OperandIsMemory(ins, 2))
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)product_avx, IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0), IARG_REG_REFERENCE, INS_OperandReg(ins, 1), IARG_MEMORYREAD_EA, IARG_END);
            else
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)product_avx, IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0), IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1), IARG_REG_REFERENCE, INS_OperandReg(ins, 2), IARG_END);
            INS_Delete(ins);
            break;
        case XED_ICLASS_ADDSS:
            std::cerr << "addss case" << std::endl;
            if (INS_OperandIsMemory(ins, 1))
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)product_sse, IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0), IARG_MEMORYREAD_EA, IARG_REG_REFERENCE, INS_OperandReg(ins, 0), IARG_END);
            else
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)product_sse, IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0), IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1), IARG_REG_REFERENCE, INS_OperandReg(ins, 0), IARG_END);
            INS_Delete(ins);
            break;
        //case XED_ICLASS_MULSD:
        //case XED_ICLASS_VMULSD:
        case XED_ICLASS_VMULSS:
            std::cerr << "vmul case" << std::endl;

            if (INS_OperandIsMemory(ins, 2))
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)sum_avx, IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0), IARG_REG_REFERENCE, INS_OperandReg(ins, 1), IARG_MEMORYREAD_EA, IARG_END);
            else
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)sum_avx, IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0), IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1), IARG_REG_REFERENCE, INS_OperandReg(ins, 2), IARG_END);
            INS_Delete(ins);
            break;
        case XED_ICLASS_MULSS:
            std::cerr << "mul case" << std::endl;

            if (INS_OperandIsMemory(ins, 1))
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)sum_sse, IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0), IARG_MEMORYREAD_EA, IARG_REG_REFERENCE, INS_OperandReg(ins, 0), IARG_END);
            else
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)sum_sse, IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0), IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1), IARG_REG_REFERENCE, INS_OperandReg(ins, 0), IARG_END);
            INS_Delete(ins);
            break;
        }
    }
  }

  void replace_module::init() {
    std::cerr << "Initialization: replace module." << std::endl;
    auto mode = knob_replace.Value();
    switch (mode) {
    case 1:
      std::cerr << "switching add and multiply" << std::endl;
      INS_AddInstrumentFunction(Switch_add_multiply, nullptr);
      std::cerr << "Switch compelet." << std::endl;
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