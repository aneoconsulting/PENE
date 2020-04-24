#include <pin.H>
#include <iostream>
#include <cassert>
#include "replace_module.h"


namespace pene {

    replace_module::replace_module() : module(), knob_replace(KNOB_MODE_WRITEONCE, "pintool",
        "replace", "0", "switch add and multiply"){
    }

    void product(const PIN_REGISTER* ra, const PIN_REGISTER* rb, PIN_REGISTER* rc) {
        std::cerr << (*ra).dbl[1] << std::endl;
        std::cerr << (*rb).dbl[1] << std::endl;
        std::cerr << (*rc).dbl[1] << std::endl;
    
    }
    void sum(const PIN_REGISTER* ra, const PIN_REGISTER* rb, PIN_REGISTER* rc) {
        std::cerr << (*ra).dbl[1] << std::endl;
        std::cerr << (*rb).dbl[1] << std::endl;
        std::cerr << (*rc).dbl[1] << std::endl;

    }


    void Switch_add_multiply(INS ins, VOID* v) {
        if (INS_IsOriginal(ins)){
            auto oc = INS_Opcode(ins);

            switch (oc) {
            case XED_ICLASS_ADDSD:
            case XED_ICLASS_VADDSD:
            case XED_ICLASS_ADDSS:
            case XED_ICLASS_VADDSS:
                std::cerr << "add case" << std::endl;
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)product, IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0), IARG_MEMORYREAD_EA, IARG_REG_REFERENCE, INS_OperandReg(ins, 0), IARG_END);
                break;
            case XED_ICLASS_MULSD:
            case XED_ICLASS_VMULSD:
            case XED_ICLASS_MULSS:
            case XED_ICLASS_VMULSS:
                std::cerr << "mul case" << std::endl;
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)sum, IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0), IARG_MEMORYREAD_EA, IARG_REG_REFERENCE, INS_OperandReg(ins, 0), IARG_END);
            }
        }
        //INS_Delete(ins);
    }

    void replace_module::init() {
        std::cerr << "Initialization: replace module." << std::endl;
        auto mode = knob_replace.Value();
        switch(mode){
        case 1 :
            std::cerr << "switching add and multiply" << std::endl;
            INS_AddInstrumentFunction(Switch_add_multiply, 0);
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