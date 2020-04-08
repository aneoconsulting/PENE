#pragma once

#include "includes/Filters.h"
#include "pin.H"
#include "internals/interflop_functions_wrapper.h"
#include "internals/interflop_x87_wrapper.h"
#include "internals/interflop_sse_wrapper.h"
#include "internals/interflop_avx_wrapper.h"
#include "internals/interflop_avx512_wrapper.h"

namespace interflop {

  template <typename backend_type, backend_type &(*back)()>
  VOID instrument(INS ins, VOID*)
  {
    struct ops {
      static void add_float(float a, float b, float* c) { back().add_float(a, b, c); }
      static void sub_float(float a, float b, float* c) { back().sub_float(a, b, c); }
      static void mul_float(float a, float b, float* c) { back().mul_float(a, b, c); }
      static void div_float(float a, float b, float* c) { back().div_float(a, b, c); }
      static void add_double(double a, double b, double* c) { back().add_double(a, b, c); }
      static void sub_double(double a, double b, double* c) { back().sub_double(a, b, c); }
      static void mul_double(double a, double b, double* c) { back().mul_double(a, b, c); }
      static void div_double(double a, double b, double* c) { back().div_double(a, b, c); }
    };
    if (pene::Filter::Instance().SelectIns(ins))
    {
      bool replaced =  (AFUNPTR)internals::insert_wrapper_function_x87<ops>(ins)
                    || (AFUNPTR)internals::insert_wrapper_function_SSE<ops>(ins)
                    || (AFUNPTR)internals::insert_wrapper_function_AVX<ops>(ins)
                    || (AFUNPTR)internals::insert_wrapper_function_AVX512<ops>(ins)
                    ;
      if (replaced)
      {
        INS_Delete(ins);
      }
    }
  }
}
