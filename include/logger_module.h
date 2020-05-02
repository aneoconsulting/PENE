#include "module.h"
#include <ostream>

namespace pene
{
  class logger
  {
  public:
    enum level {
      error,
      warning,
      info,
      debug,
      trace,
      default = info,
      max = debug
    };
    logger();
    std::ostream& get(const level &);

  };

}