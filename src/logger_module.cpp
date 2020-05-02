#include "logger_module.h"
#include <iostream>
#include <ostream>

namespace pene 
{
  namespace logger_module_internals 
  {

    class nullStream final : private std::streambuf, public std::ostream
    {
      using int_type = typename std::streambuf::int_type;

    public:
      nullStream():std::ostream(this){}
      virtual ~nullStream() noexcept {}

    protected:
      int_type overflow(int_type ch)
      {
        return int_type(ch);
      }
    };

  }

  logger::logger(){}

  std::ostream& logger::get(const level&)
  {
    return std::cerr;
  }
}