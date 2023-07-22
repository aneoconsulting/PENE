#include "symbol_list_generator_module.h"
#include <cassert>

#include <unordered_set>
#include <unordered_map>
#include "counters.h"
#include "counters_utils.h"


namespace pene{
  namespace symbol_list_generator_module_internals {
    static std::ofstream sym_list_stream{};

    static size_t img_name_max_size = 0;

    using rtn_name_list_t = std::tr1::unordered_set<std::string>;
    using rtn_name_list_by_img_name_t = std::tr1::unordered_map<std::string, rtn_name_list_t>;
    // sym_list[img_name] => rtn_name_list
    rtn_name_list_by_img_name_t sym_list;


    static VOID store_loaded_symbols(IMG img, void*)
    {
      std::string img_name = IMG_Name(img);
      img_name_max_size = std::max(img_name.length(), img_name_max_size);
      for (SEC sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec))
      {
        for (RTN rtn = SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn))
        {
          counters c;
          bool has_fp_inst = false;
          RTN_Open(rtn);
          for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins))
          {
            update_counters(ins, c);
          }

          for (size_t i = 0; i < counters::size && !has_fp_inst; ++i)
          {
            has_fp_inst |= c.array[i] > 0;
          }
          if (has_fp_inst)
          {
            sym_list[img_name].insert(RTN_Name(rtn));
          }
          RTN_Close(rtn);
        }
      }
    }

    static VOID store_executed_traces(TRACE trace, void*)
    {
      const auto& addr = TRACE_Address(trace);
      counters c;
      bool has_fp_inst = false;

      for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
      {
        for (INS ins = BBL_InsHead(bbl); INS_Valid(ins); ins = INS_Next(ins))
        {
          update_counters(ins, c);
        }
      }

      for (size_t i = 0; i < counters::size && !has_fp_inst; ++i)
      {
        has_fp_inst |= c.array[i] > 0;
      }
      if (has_fp_inst)
      {
        const auto& img = IMG_FindByAddress(addr);
        const auto& name = IMG_Valid(img) ? IMG_Name(img) : "Unknown_IMG";
        img_name_max_size = std::max(name.length(), img_name_max_size);
        sym_list[name].insert(RTN_FindNameByAddress(addr));
      }
    }



    using is_executed_by_sym_name_t = std::tr1::unordered_map<std::string, bool>;
    using is_executed_by_sym_name_by_img_name_t = std::tr1::unordered_map<std::string, is_executed_by_sym_name_t>;
    // sym_list[img_name] => rtn_name_list
    is_executed_by_sym_name_by_img_name_t executed_sym_list;

    static VOID set_true(bool* bool_ptr)
    {
      *bool_ptr = true;
    }

    static VOID store_executed_bbl(TRACE trace, void*)
    {
      for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
      {
        auto addr = BBL_Address(bbl);
        counters c;
        bool has_fp_inst = false;
        for (INS ins = BBL_InsHead(bbl); INS_Valid(ins); ins = INS_Next(ins))
        {
          update_counters(ins, c);
        }
        for (size_t i = 0; i < counters::size && !has_fp_inst; ++i)
        {
          has_fp_inst |= c.array[i] > 0;
        }

        if (has_fp_inst)
        {
          const auto& img = IMG_FindByAddress(addr);
          const auto& name = IMG_Valid(img) ? IMG_Name(img) : "Unknown_IMG";
          img_name_max_size = std::max(name.length(), img_name_max_size);

          auto& executed_sym = executed_sym_list[name][RTN_FindNameByAddress(addr)];

          INS_InsertCall(BBL_InsHead(bbl), IPOINT_BEFORE, (AFUNPTR)set_true, IARG_PTR, &executed_sym, IARG_END);
        }
      }


    }
  }

  using namespace symbol_list_generator_module_internals;

  symbol_list_generator_module::symbol_list_generator_module()
    : module(true)
    , knob_exclist_gen{ KNOB_MODE_WRITEONCE, "pintool", "gen-sym-list", "",
      "Save the list of symbols with fp instructions in given file." }
    , knob_exclist_mode{ KNOB_MODE_WRITEONCE, "pintool", "gen-sym-mode", "0",
      "Sets the mode for symbols generations. "
      "0: lists all symbols with fp instruction that have been loaded during execution. "
      "1: lists only symbols executed which contain fp instructions"
      "2: lists only symbols with fp instructions that have been executed (slow)." }
  {
  }

  bool symbol_list_generator_module::validate()
  {
    std::cerr << "Checking configuration: symbol list generation - ";
    const auto& filename = knob_exclist_gen.Value();

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

    const auto& mode = knob_exclist_mode.Value();
    if (mode < 0 || mode > 2)
    {
      std::cerr << " KO: -gen-sym-mode should be either 0, 1 or 2." << std::endl;
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
      if (knob_exclist_mode.Value() == 2)
      {

        TRACE_AddInstrumentFunction(store_executed_bbl, nullptr);
        PIN_AddFiniFunction([](INT32, void*)
          {
            for (const auto& img_name_it : executed_sym_list)
            {
              const auto& img_name = img_name_it.first;

              for (const auto& rtn_name_it : img_name_it.second)
              {
                if (rtn_name_it.second)
                {
                  const auto& rtn_name = rtn_name_it.first;
                  sym_list_stream << std::left << std::setw(int(img_name_max_size) + 4) << img_name << rtn_name << "\n";
                }
              }
            }
            sym_list_stream.flush();
            sym_list_stream.close();
          }, nullptr);
      }
      else
      {
        if (knob_exclist_mode.Value() == 0)
        {
          IMG_AddInstrumentFunction(store_loaded_symbols, nullptr);
        }
        else
        {
          TRACE_AddInstrumentFunction(store_executed_traces, nullptr);
        }
        PIN_AddFiniFunction([](INT32, void*)
          {
            for (const auto& img_name_it : sym_list)
            {
              const auto& img_name = img_name_it.first;

              for (const auto& rtn_name : img_name_it.second)
              {
                sym_list_stream << std::left << std::setw(int(img_name_max_size) + 4) << img_name << rtn_name << "\n";
              }
            }
            sym_list_stream.flush();
            sym_list_stream.close();
          }, nullptr);
      }
    }
  }

  const std::string& symbol_list_generator_module::name()
  {
    static const std::string name_("symbol_list_generator_module");
    return name_;
  }
}

