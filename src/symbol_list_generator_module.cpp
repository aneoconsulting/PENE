#include "symbol_list_generator_module.h"
#include <cassert>


namespace pene{
  namespace symbol_list_generator_module_internals {
    static VOID write_loaded_symbols(IMG img, void* voided_stream)
    {
      auto & ofstream = *reinterpret_cast<std::ofstream*>(voided_stream);

      std::string img_name = IMG_Name(img);
      for (SEC sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec))
      {
        for (RTN rtn = SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn))
        {
          ofstream << img_name << "\t" << RTN_Name(rtn) << "\n";
        }
      }
    }
  }



  symbol_list_generator_module::symbol_list_generator_module():
    module(true),
    knob_exclist_gen(KNOB_MODE_WRITEONCE, "pintool", "gen-sym-list", "",
      "Save the list of all symbols loaded durong the exceution in given file."),
    sym_list_stream()
  {
  }

  bool symbol_list_generator_module::validate()
  {
    std::cerr << "Checking configuration: symbol list generation - ";
    auto filename = knob_exclist_gen.Value();

    if (filename.empty())
    {
      std::cerr << "no symbol file will be generated" << filename << std::endl;
      return true;
    }

    std::cerr << "opening file " << filename;
    sym_list_stream.open(filename.c_str());
    if (!sym_list_stream.is_open())
    {
      std::cerr << " KO" << std::endl;
      std::string error_mess = "ERROR: Could not create file ";
      error_mess.append(filename);
      std::cerr << error_mess << std::endl;
      PIN_WriteErrorMessage(error_mess.c_str(), 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_NONFATAL, 0);
      return false;
    }
    std::cerr << " OK" << std::endl;
    return true;
  }

  void symbol_list_generator_module::init()
  {
    std::cerr << "Initialization: symbol list generation." << std::endl;
    if (!knob_exclist_gen.Value().empty() && sym_list_stream.good())
    {
      IMG_AddInstrumentFunction(symbol_list_generator_module_internals::write_loaded_symbols, &sym_list_stream);
      PIN_AddFiniFunction([](INT32, void* voided_stream) 
        {
          auto& ofstream = *(std::ofstream*)voided_stream;
          ofstream.flush();
          ofstream.close();
        }, &sym_list_stream);
    }
  }

  const std::string& symbol_list_generator_module::name() {
    static const std::string name_{ "symbol_list_generator_module" };
    return name_;
  }
}

