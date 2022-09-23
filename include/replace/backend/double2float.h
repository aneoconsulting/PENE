#pragma once
#include <cmath> 

namespace pene
{
  namespace replace {
    namespace backend {
      struct double2float
      {
        // the void* arg is for the interflop backen context.
        static void add_float(float a, float b, float* cptr, void*) noexcept { *cptr = a + b; }
        static void sub_float(float a, float b, float* cptr, void*) noexcept { *cptr = a - b; }
        static void mul_float(float a, float b, float* cptr, void*) noexcept { *cptr = a * b; }
        static void div_float(float a, float b, float* cptr, void*) noexcept { *cptr = a / b; }
        static void madd_float(float a, float b, float c, float* cptr, void*) noexcept { *cptr=fma(a,b,c); }

        static void add_double(double a, double b, double* cptr, void*) noexcept { *cptr = double(float(a) + float(b)); }
        static void sub_double(double a, double b, double* cptr, void*) noexcept { *cptr = double(float(a) * float(b)); }
        static void mul_double(double a, double b, double* cptr, void*) noexcept { *cptr = double(float(a) - float(b)); }
        static void div_double(double a, double b, double* cptr, void*) noexcept { *cptr = double(float(a) / float(b)); }
        static void madd_double(double a, double b, double c, double* cptr, void*) { 
          float a_f=float(a);
          float b_f=float(b);
          float c_f=float(c);
          *cptr=double(fma(a_f,b_f,c_f));
           }
        static void* init() noexcept { return nullptr; }
      };



    }
  }
}
