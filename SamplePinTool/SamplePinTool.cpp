#include "pin.H"
#include <iostream>
#include "instlib.H"
#include "version.h"

#include "includes/defs.h"
#include "includes/Filters.h"
#include "backends/ieee.h"
#include "backends/opposite_ieee.h"
#include "interflop_wrappers.h"

/* ================================================================== */
// Global Variables
/* ================================================================== */


/* ================================================================== */
// Backend Variables and initializations
/* ================================================================== */
typedef interflop::backends::ieee::backend ieee_backend_t;
ieee_backend_t ieee_backend;
ieee_backend_t& get_ieee_backend() { return ieee_backend; }
void setup_ieee_backend()
{
    //ieee_backend.init<interflop::backends::ieee::interflop_init>();
    INS_AddInstrumentFunction(interflop::instrument<ieee_backend_t, get_ieee_backend>, nullptr);
    std::cerr << "Using ieee rounding mode." << std::endl;
}

typedef interflop::interflop_generic_backend opposite_ieee_backend_t;
opposite_ieee_backend_t opposite_ieee_backend;
opposite_ieee_backend_t& get_opposite_ieee_backend() { return opposite_ieee_backend; }
void setup_oposite_ieee_backend()
{
    opposite_ieee_backend.init<interflop::backends::opposite_ieee::interflop_init>();
    INS_AddInstrumentFunction(interflop::instrument<opposite_ieee_backend_t, get_opposite_ieee_backend>, nullptr);
    std::cerr << "Using opposite-ieee rounding mode. This mode can be used to debug the instrumentation." << std::endl;
}

/* ===================================================================== */
// Utilities Command line switches
/* ===================================================================== */

KNOB<bool> KnobVersion(KNOB_MODE_WRITEONCE, "pintool",
    "version", "0", "display the actual version of this pin tool");

KNOB<string> KnobRoundingMode(KNOB_MODE_WRITEONCE, "pintool",
    "rounding-mode", "none", "Defines the backend choosen to change the rounding mode. Available options: \"none\", \"ieee\" and \"opposite-ieee\" (for debug purpose only)");
VOID set_rounding_mode()
{
    if (KnobRoundingMode.Value() == "none")
    {
        return;
    }
    if (KnobRoundingMode.Value() == "ieee")
    {
        setup_ieee_backend();
        return;
    }
    if (KnobRoundingMode.Value() == "opposite-ieee")
    {
        setup_oposite_ieee_backend();
        return;
    }
    std::cerr << "Rounding mode " << KnobRoundingMode.Value() << " does not exist." << std::endl;
}


/*!
 *  Print out help message.
 */
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
    pene::Filter& filter = pene::Filter::Instance();

    if (PIN_Init(argc, argv))
    {
        return Usage();
    }

    if (KnobVersion.Value())
    {
        std::cout << "Current version for SamplePinTool : ";
        std::cout << pene_version << std::endl;
        return 0;
    }

    PIN_InitSymbols();

    filter.Activate();

    set_rounding_mode();

    PIN_StartProgram();

    getchar();

    return 0;
}