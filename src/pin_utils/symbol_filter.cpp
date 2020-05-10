#include "pin_utils/symbol_filters.h"
#include "utils/safe_get_line.h"
#include <pin.H>
#include <iostream>

namespace pene
{
  namespace pin_utils
  {
    BOOL symbol_filter_base::is_in_list(ADDRINT addr) const
    {
      if (sym_list.empty()) return true;
      IMG img = IMG_FindByAddress(addr);
      name_list_by_obj_name_t::const_iterator it;
      it = sym_list.find(IMG_Name(img));
      if (it != sym_list.end())
      {
        return it->second.find("*") == it->second.end() && it->second.find(RTN_FindNameByAddress(addr)) == it->second.end();
      }
      it = sym_list.find("*");
      return it == sym_list.end() || it->second.find(RTN_FindNameByAddress(addr)) == it->second.end();
    }

    symbol_filter_base::symbol_filter_base(const std::string& sym_lis_filename)
    {
      std::ifstream inputFileStream(sym_lis_filename.c_str(), std::ifstream::in);
      if (!inputFileStream.is_open())
      {
        std::string error_mess = "Could not open file ";
        error_mess.append(sym_lis_filename);

        std::cerr << error_mess << std::endl;
        PIN_WriteErrorMessage(error_mess.c_str(), 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
        exit(1);
      }

      size_t line_nb = 0;
      while (!inputFileStream.eof())
      {
        line_nb++;
        std::string line;
        utils::safeGetline(inputFileStream, line);

        if (line != "" && line[0] != '#')
        {

          std::string::size_type pos;
          pos = line.find(' ', 0);

          if (pos == std::string::npos)
          {
            pos = line.find('\t', 0);
            if (pos == std::string::npos)
            {
              std::string error_mess = "Error in file ";
              error_mess.append(sym_lis_filename);
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
          std::string second = utils::trim(line.substr(pos + 1));

          sym_list[first].insert(second);
        }
      }
      inputFileStream.close();
    }

    symbol_include_filter::symbol_include_filter(const std::string& sym_lis_filename)
      : symbol_filter_base(sym_lis_filename)
    {}

    BOOL symbol_include_filter::is_instrumented(ADDRINT addr) const
    {
      return is_in_list(addr);
    }

    symbol_exclude_filter::symbol_exclude_filter(const std::string& sym_lis_filename)
      : symbol_filter_base(sym_lis_filename)
    {}

    BOOL symbol_exclude_filter::is_instrumented(ADDRINT addr) const
    {
      return !is_in_list(addr);
    }
  }
}
