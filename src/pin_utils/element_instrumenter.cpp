#include "pin_utils/element_instrumenter.h"

namespace pene
{
  namespace pin_utils
  {
    void element_instrumenter::init_instrument(TRACE) {}
    void element_instrumenter::init_instrument(BBL) {}
    void element_instrumenter::end_instrument(BBL) {}
    void element_instrumenter::end_instrument(TRACE) {}
    void null_element_instrumenter::instrument(INS){}
  }
}