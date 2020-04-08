#include "includes/Filters.h"
#include <iostream>
#include <fstream>
#include <string>

namespace pene {
  using namespace std;
  using namespace std::tr1;

  Filter::Filter() :
    KnobSymExludeList(KNOB_MODE_WRITEONCE, "pintool", "exclude", "none",
      "Exclude the symbols listed in the file from instrumentation."),
    KnobGenExclusionList(KNOB_MODE_WRITEONCE, "pintool", "gen-exclude", "none",
      "Save the list of all symbols loaded durong the exceution in given file."),
    KnobSourceInstrumentList(KNOB_MODE_WRITEONCE, "pintool", "source", "none",
      "When this option is present, only instructions coming from source code lines listed in the file are instrumented. [NOT WORKING YET]"),
    sym_exlude_list(),
    sym_gen_list()
  {}

  Filter& Filter::Instance()
  {
    static Filter instance;
    return instance;
  }

  Filter::~Filter()
  {
    EndExclusionList();
    EndExclusionListGeneration();
    EndSourceInstrumentList();
  }

  // same as std::getline but also handling \r to get windows/linux/macos portability.
  static std::istream& safeGetline(std::istream& is, std::string& t)
  {
    t.clear();
    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.
    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();
    while (true)
    {
      int c = sb->sbumpc();
      if (c == '\n') return is;
      if (c == '\r')
      {
        if (sb->sgetc() == '\n')
          sb->sbumpc();
        return is;
      }
      if (c == std::streambuf::traits_type::eof())
      {
        // Also handle the case when the last line has no line ending
        if (t.empty())
          is.setstate(std::ios::eofbit);
        return is;
      }
      t += (char)c;
    }
  }

  static std::string trim(const std::string& line)
  {
    const char* WhiteSpace = " \t\v\r\n";
    std::size_t start = line.find_first_not_of(WhiteSpace);
    std::size_t end = line.find_last_not_of(WhiteSpace);
    return start == end ? std::string() : line.substr(start, end - start + 1);
  }

  VOID Filter::Activate()
  {
    ActivateExclusionList();
    ActivateExclusionListGeneration();
    ActivateSourceInstrumentList();
  }

  VOID Filter::ActivateSourceInstrumentList()
  {
    if (KnobSourceInstrumentList.Value() != "none")
    {
      std::cerr << "The -source flag is not working yet. The execution will continue, ignoring the flag." << std::endl;
      PIN_WriteErrorMessage("The -source flag is not working yet. The execution will continue, ignoring the flag.", PIN_ERR_PARSING, PIN_ERR_SEVERITY_TYPE::PIN_ERR_NONFATAL, 0);
    }
  }

  VOID Filter::EndSourceInstrumentList()
  {
    if (KnobSourceInstrumentList.Value() != "none")
    {
      std::cerr << "The -source flag is not working yet. The flag has been ignored." << std::endl;
      PIN_WriteErrorMessage("The -source flag is not working yet. The flag has been ignored.", PIN_ERR_PARSING, PIN_ERR_SEVERITY_TYPE::PIN_ERR_NONFATAL, 0);
    }
  }

  ofstream exlusion_list_gen_stream;

  static VOID WriteExclusionList(IMG img, void*)
  {
    std::string img_name = IMG_Name(img);
    for (SEC sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec))
    {
      for (RTN rtn = SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn))
      {
        exlusion_list_gen_stream << img_name << "\t" << RTN_Name(rtn) << "\n";
      }
    }
  }



  VOID Filter::ActivateExclusionListGeneration()
  {
    std::string exlcusion_list_out_name = KnobGenExclusionList.Value();
    if (exlcusion_list_out_name != "none")
    {
      exlusion_list_gen_stream.open(exlcusion_list_out_name.c_str(), ofstream::out);
      if (!exlusion_list_gen_stream.is_open())
      {
        std::string error_mess = "Could not open file ";
        error_mess.append(exlcusion_list_out_name);

        std::cerr << error_mess << std::endl;
        PIN_WriteErrorMessage(error_mess.c_str(), 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
        exit(1);
      }
      IMG_AddInstrumentFunction(WriteExclusionList, nullptr);
    }
  }

  VOID Filter::EndExclusionListGeneration()
  {
    exlusion_list_gen_stream.close();
  }

  VOID Filter::ActivateExclusionList()
  {
    std::string sym_exlude_list_str = KnobSymExludeList.Value();
    if (sym_exlude_list_str != "none")
    {
      ifstream inputFileStream(sym_exlude_list_str.c_str(), ifstream::in);
      if (!inputFileStream.is_open())
      {
        std::string error_mess = "Could not open file ";
        error_mess.append(sym_exlude_list_str);

        std::cerr << error_mess << std::endl;
        PIN_WriteErrorMessage(error_mess.c_str(), 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
        exit(1);
      }

      size_t line_nb = 0;
      while (!inputFileStream.eof())
      {
        line_nb++;
        std::string line;
        safeGetline(inputFileStream, line);

        if (line != "" && line[0] != '#')
        {

          string::size_type pos;
          pos = line.find(' ', 0);

          if (pos == std::string::npos)
          {
            pos = line.find('\t', 0);
            if (pos == std::string::npos)
            {
              std::string error_mess = "Error in file ";
              error_mess.append(sym_exlude_list_str);
              error_mess.append(": line ");
              error_mess.append(decstr(line_nb));
              error_mess.append("contains no space or tab. Line content: \"");
              error_mess.append(line);
              error_mess.append("\".");
              std::cerr << error_mess << std::endl;
              PIN_WriteErrorMessage(error_mess.c_str(), 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
              exit(1);
            }
          }

          std::string first = line.substr(0, pos);
          std::string second = trim(line.substr(pos + 1));

          sym_exlude_list[first].insert(second);
        }
      }
    }
  }
  VOID Filter::EndExclusionList() {}

  BOOL Filter::SelectRtn(RTN rtn) const
  {
    if (sym_exlude_list.empty()) return true;
    ADDRINT addr = RTN_Address(rtn);
    return SelectAddr(addr);
  }
  BOOL Filter::SelectTrace(TRACE trace) const
  {
    if (sym_exlude_list.empty()) return true;
    ADDRINT addr = TRACE_Address(trace);
    return SelectAddr(addr);
  }
  BOOL Filter::SelectBbl(BBL bbl) const
  {
    if (sym_exlude_list.empty()) return true;
    ADDRINT addr = BBL_Address(bbl);
    return SelectAddr(addr);
  }
  BOOL Filter::SelectIns(INS ins) const 
  {
    if (sym_exlude_list.empty()) return true;
    ADDRINT addr = INS_Address(ins);
    return SelectAddr(addr);
  }

  BOOL Filter::SelectAddr(ADDRINT addr) const
  {
    if (sym_exlude_list.empty()) return true;
    IMG img = IMG_FindByAddress(addr);
    name_list_by_obj_name_t::const_iterator it;
    it = sym_exlude_list.find(IMG_Name(img));
    if (it != sym_exlude_list.end())
    {
      return it->second.find("*") == it->second.end() && it->second.find(RTN_FindNameByAddress(addr)) == it->second.end();
    }
    it = sym_exlude_list.find("*");
    return it == sym_exlude_list.end() || it->second.find(RTN_FindNameByAddress(addr)) == it->second.end();
  }
}
