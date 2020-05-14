#include "pin_utils/symbol_filters.h"
#include "utils/safe_get_line.h"
#include "utils/match.h"
#include <pin.H>
#include <iostream>

namespace pene
{
  namespace pin_utils
  {
    BOOL symbol_filter_base::is_in_list(ADDRINT addr) const
    {
      IMG img = IMG_FindByAddress(addr);
      std::string img_name(IMG_Name(img));
      std::string rtn_name(RTN_FindNameByAddress(addr));
      return is_in_list(img_name, rtn_name);
    }

    BOOL symbol_filter_base::is_in_list(const std::string & img_name, const std::string & rtn_name) const
    {
      if (sym_list.empty() && wildcarded_sym_list.empty()) return false;
      auto it = sym_list.find(img_name);
      if (it != sym_list.end())
      {
        if (it->second.find("*") != it->second.end())
        {
          //std::cerr << "mapped symbols in list: " << img_name << "\t" << rtn_name << std::endl;
          return true;
        }
        return it->second.find(rtn_name) != it->second.end();
      }
      
      if (unmatched_img_names.find(img_name) != unmatched_img_names.end())
      {
        return false;
      }
      
      for each (auto wilded_it in wildcarded_sym_list)
      {
        if (utils::match(wilded_it.first, img_name))
        {
          sym_list[img_name].insert(wilded_it.second.begin(), wilded_it.second.end());
          bool res = false
            || wilded_it.second.find("*") != wilded_it.second.end() 
            || wilded_it.second.find(rtn_name) != wilded_it.second.end()
            ;
          if (res)
          {
            //std::cerr << "mapped symbols in list: " << img_name << "\t" << rtn_name << "\n";
            return true;
          }
        }
      }
      unmatched_img_names.insert(img_name);
      
      return false;
    }

    symbol_filter_base::symbol_filter_base(const std::string& sym_lis_filename)
      : sym_list()
      , wildcarded_sym_list()
      , unmatched_img_names()
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

      load(inputFileStream);
      inputFileStream.close();
    }

    VOID symbol_filter_base::load(std::istream& inputFileStream)
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
          std::string second = utils::trim(line.substr(pos + 1));

          std::cerr << "Loaded from file :" << second << " in " << first << std::endl;
          if (filename.find('*') == std::string::npos)
          {
            sym_list[first].insert(second);
          }
          else
          {
            wildcarded_sym_list[first].insert(second);
          }
        }
      }
    }

    BOOL symbol_filter_base::check_trace() const { return true; }
    BOOL symbol_filter_base::check_bbl() const { return false; }
    BOOL symbol_filter_base::check_ins() const { return false; }

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
