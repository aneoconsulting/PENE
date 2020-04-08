#include "opposite_ieee.h"
namespace interflop
{
  namespace backends {
    namespace opposite_ieee {
      void add_float(float a, float b, float* c, void*) { *c = -(a + b); }
      void sub_float(float a, float b, float* c, void*) { *c = -(a - b); }
      void mul_float(float a, float b, float* c, void*) { *c = -(a * b); }
      void div_float(float a, float b, float* c, void*) { *c = -(a / b); }

      void add_double(double a, double b, double* c, void*) { *c = -(a + b); }
      void sub_double(double a, double b, double* c, void*) { *c = -(a - b); }
      void mul_double(double a, double b, double* c, void*) { *c = -(a * b); }
      void div_double(double a, double b, double* c, void*) { *c = -(a / b); }

      interflop_backend_interface_t interface = {
        add_float,
        sub_float,
        mul_float,
        div_float,
        add_double,
        sub_double,
        mul_double,
        div_double
      };

      struct interflop_backend_interface_t interflop_init(void** context)
      {
        *context = nullptr;
        return interface;
      }
    }
  }
}