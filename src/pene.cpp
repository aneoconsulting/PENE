#include <pin.H>
#include <instlib.H>

#include "version_module.h"
#include "counters_module.h"

using namespace pene;

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
    pene::counters_module countersModule{};

    if (PIN_Init(argc, argv))
    {
        return Usage();
    }

    versionModule.init();
    countersModule.init();

    PIN_InitSymbols();

    PIN_AddFiniFunction([](INT32 code, void*) {std::cout << "code returned " << code << std::endl; }, nullptr);

    versionModule.end();
    countersModule.end();

    PIN_StartProgram();

    return 0;
}