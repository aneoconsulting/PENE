#include <pin.H>
#include <instlib.H>


#include "version_module.h"
#include "counters_module.h"
#include "symbol_list_generator_module.h"
#include "replace_module.h"
#include "extern_file.h"
#include "interflop_module.h"

using namespace pene;
std::ofstream OutFile;
KNOB< string > KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "debug.out", "specify output file name");


INT32 Usage()
{
  std::cerr << "===============================================" << std::endl;
  std::cerr << "    PENE: command line options                 " << std::endl;
  std::cerr << "===============================================" << std::endl;
  std::cerr << "This tool instruments the execution with an Interflop backend\n" << std::endl;
  std::cerr << KNOB_BASE::StringKnobSummary() << std::endl;
  PIN_ExitProcess(1);
}



VOID Fini(INT32 code, VOID*)
{
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.setf(std::ios::showbase);
    OutFile << "End of execution. Code returned " << code << "." << std::endl;
    OutFile.close();
    std::cerr << "End of execution. Code returned " << code << "." << std::endl;
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
    replace_module replacemodule{};
    interflop_module interflopmodule{};

    OutFile.open(KnobOutputFile.Value().c_str());

    if (PIN_Init(argc, argv) || !module::validate_all())
    {
        return Usage();
    }

    PIN_AddFiniFunction(Fini, nullptr);

    module::init_all();

    PIN_StartProgram();

    return 0;
}
