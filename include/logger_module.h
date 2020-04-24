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
      default = info
    };
    logger();
    std::ostream& get(const level &);

  };

}