#pragma once

namespace pene
{
  namespace replace {
    namespace backend {
      struct ieee
      {
        // the void* arg is for the interflop backen context.
        static void add_float(float a, float b, float* cptr, void*) noexcept { *cptr = a + b; }
        static void sub_float(float a, float b, float* cptr, void*) noexcept { *cptr = a - b; }
        static void mul_float(float a, float b, float* cptr, void*) noexcept { *cptr = a * b; }
        static void div_float(float a, float b, float* cptr, void*) noexcept { *cptr = a / b; }

        static void add_double(double a, double b, double* cptr, void*) noexcept { *cptr = a + b; }
        static void sub_double(double a, double b, double* cptr, void*) noexcept { *cptr = a - b; }
        static void mul_double(double a, double b, double* cptr, void*) noexcept { *cptr = a * b; }
        static void div_double(double a, double b, double* cptr, void*) noexcept { *cptr = a / b; }
        static void* init() noexcept { return nullptr; }
      };



    }
  }
}
