#pragma once
#include "includes/interflop.h"
#include "includes/defs.h"

namespace interflop
{
  namespace backends {
    namespace ieee {
      struct interflop_backend_interface_t interflop_init(void** context);

      class backend: public ::interflop::backend<backend>
      {
      public:
        void add_float(float a, float b, float* cptr);
        void sub_float(float a, float b, float* cptr);
        void mul_float(float a, float b, float* cptr);
        void div_float(float a, float b, float* cptr);

        void add_double(double a, double b, double* cptr);
        void sub_double(double a, double b, double* cptr);
        void mul_double(double a, double b, double* cptr);
        void div_double(double a, double b, double* cptr);
      };
    }
  }
}