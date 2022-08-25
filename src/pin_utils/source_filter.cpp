#include "pin_utils/source_filter.h"
#include "utils/safe_get_line.h"
#include "utils/match.h"
#include <pin.H>
#include <iostream>

namespace pene
{
  namespace pin_utils
  {
    BOOL source_filter_base::is_in_list(ADDRINT addr) const
    { 

      INT32 line_number, column;
      std::string filename;
      PIN_GetSourceLocation(addr, &column, &line_number, &filename);
      return is_in_list(filename, line_number);
    }

    BOOL source_filter_base::is_in_list(const std::string& filename, INT32 line_number) const
    {
      if (line_list.empty()) return false;

      auto it = line_list.find(filename);
      return it != line_list.end() && (it->second.find(line_number) != it->second.end());
    }

    source_filter_base::source_filter_base(const std::string& source_lis_filename)
      : line_list()
    {
      std::ifstream inputFileStream(source_lis_filename.c_str(), std::ifstream::in);
      if (!inputFileStream.is_open())
      {
        std::string error_mess = "Could not open file ";
        error_mess.append(source_lis_filename);

        std::cerr << error_mess << std::endl;
        PIN_WriteErrorMessage(error_mess.c_str(), 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
        exit(1);
      }

      load(inputFileStream);
      inputFileStream.close();
    }

    VOID source_filter_base::load(std::istream& inputFileStream)
    {
      size_t line_nb = 0;
      while (!inputFileStream.eof())
      {
        line_nb++;
        std::string line;
        utils::safeGetline(inputFileStream, line);

        if (line.length() > 0 && line[0] != '#')
        {
          std::string::size_type pos;
          pos = line.find(' ', 0);

          if (pos == std::string::npos)
          {
            pos = line.find('\t', 0);
            if (pos == std::string::npos)
            {
              std::string error_mess = "Error in file: line contains no space or tab. Line content: \"";
              error_mess.append(line);
              error_mess.append("\".");
              std::cerr << error_mess << std::endl;
              PIN_WriteErrorMessage(error_mess.c_str(), 1000, PIN_ERR_SEVERITY_TYPE::PIN_ERR_FATAL, 0);
              exit(1);
            }
          }
          auto filename = line.substr(0, pos);
          std::replace(filename.begin(), filename.end(), '/', '\\');
          std::string first = filename;
          INT32 second = std::atoi(line.substr(pos + 1).c_str());

          std::cerr << "Loaded from file :" << second << " in " << first << std::endl;

          line_list[first].insert(second);

        }
      }
    }

    filter::check_status source_filter_base::check_trace() const { return filter::CHECK_NEST; }
    filter::check_status source_filter_base::check_bbl() const { return filter::CHECK_NEST; }
    filter::check_status source_filter_base::check_ins() const { return filter::CHECK_NEST; }
    filter::check_status source_filter_base::is_instrumented(TRACE) const { return filter::CHECK_NEST; }
    filter::check_status source_filter_base::is_instrumented(BBL) const { return filter::CHECK_NEST; }

    source_include_filter::source_include_filter(const std::string& sym_lis_filename)
      : source_filter_base(sym_lis_filename)
    {}

    filter::check_status source_include_filter::is_instrumented(ADDRINT addr) const
    {
      return is_in_list(addr) ? filter::INSTRUMENT : filter::IGNORE;
    }

    source_exclude_filter::source_exclude_filter(const std::string& sym_lis_filename)
      : source_filter_base(sym_lis_filename)
    {}

    filter::check_status source_exclude_filter::is_instrumented(ADDRINT addr) const
    {
      return is_in_list(addr) ? filter::IGNORE : filter::INSTRUMENT;
    }
  }
}
