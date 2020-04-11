#include "pin.H"
#include "instlib.H"
#include "version.h"
#include "counters.h"
#include "module.h"
#include "version_module.h"

counters c{};
void DoCountAdd() { c.add++; }
/// TODO
void AddCountInstrumentation(INS ins, void*)
{
    auto oc = INS_Opcode(ins);
    switch (oc)
    {
    case XED_ICLASS_ADDSS:
    case XED_ICLASS_SUBSS:
    case XED_ICLASS_VADDSS:
    case XED_ICLASS_VSUBSS:
        INS_InsertCall(ins, IPOINT_BEFORE, DoCountAdd, IARG_END);
        break;
    default:
        break;
    }
}




INT32 Usage()
{
    std::cerr << "This tool instruments the execution with an Interflop backend\n" << std::endl;
    std::cerr << KNOB_BASE::StringKnobSummary() << std::endl;
    return -1;
}

/*!
 * The main procedure of the tool.
 * This function is called when the application image is loaded but not yet started.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments,
 *                              including pin -t <toolname> -- ...
 */
int main(int argc, char* argv[])
{
    std::cout << "===============================================" << std::endl;
    std::cout << "    PENE: Pin Enabled Numerical Exploration    " << std::endl;
    std::cout << "===============================================" << std::endl;

    // Modules have to be loaded before calling PIN_Init.
    version_module versionModule{};

    if (PIN_Init(argc, argv))
    {
        return Usage();
    }

    versionModule.init();

    PIN_InitSymbols();

    INS_AddInstrumentFunction(AddCountInstrumentation, nullptr);

    PIN_AddFiniFunction([](INT32, void*) {c.print(); }, nullptr);

    PIN_AddFiniFunction([](INT32, void* module_ptr) { ((version_module*)module_ptr)->end(); }, &versionModule);

    PIN_StartProgram();

    return 0;
}