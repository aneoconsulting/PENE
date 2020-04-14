#include <pin.H>
#include <instlib.H>

#include "version_module.h"
#include "counters_module.h"
#include "symbol_list_generator_module.h"

using namespace pene;

INT32 Usage()
{
  std::cerr << "===============================================" << std::endl;
  std::cerr << "    PENE: command line options                 " << std::endl;
  std::cerr << "===============================================" << std::endl;
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
    std::cerr << "===============================================" << std::endl;
    std::cerr << "    PENE: Pin Enabled Numerical Exploration    " << std::endl;
    std::cerr << "===============================================" << std::endl;

    // Modules have to be loaded before calling PIN_Init.
    version_module versionModule{};
    counters_module countersModule{};
    symbol_list_generator_module symlistgenModule{};

    if (PIN_Init(argc, argv) || !module::validate_all())
    {
        return Usage();
    }

    PIN_AddFiniFunction([](INT32 code, void*) {std::cerr << "End of execution. Code returned " << code << "." << std::endl; }, nullptr);

    module::init_all();

    PIN_StartProgram();

    return 0;
}