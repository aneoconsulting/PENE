#include <pin.H>
#include <iostream>
#include <cassert>
#include "replace_module.h"

namespace pene {

    replace_module::replace_module() : module(), knob_replace(KNOB_MODE_WRITEONCE, "pintool",
        "replace", "1", "swith add and multiply"){
    }

    void TODO(){}

    void Switch_add_multiply(INS ins, VOID* v) {
        if (INS_IsOriginal(ins)){
            auto oc = INS_Opcode(ins);
            switch (oc) {
            case XED_ICLASS_ADDSS:
            case XED_ICLASS_VADDSS:
            case XED_ICLASS_MULSS:
            case XED_ICLASS_VMULSS:
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)TODO, IARG_END);
            }
        }
        INS_Delete(ins);
    }

    void replace_module::init() {
        std::cerr << "Initialization: FP instructions count." << std::endl;
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