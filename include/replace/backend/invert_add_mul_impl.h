#pragma once

namespace pene
{
  namespace replace {
    namespace backend {
      struct invert_add_mul_impl
      {
        // the void* arg is for the interflop backen context.
        static constexpr void add_float(float a, float b, float* cptr, void*) noexcept { *cptr = a * b; }// replace + with *
        static constexpr void sub_float(float a, float b, float* cptr, void*) noexcept { *cptr = a - b; }
        static constexpr void mul_float(float a, float b, float* cptr, void*) noexcept { *cptr = a + b; }// replace * with +
        static constexpr void div_float(float a, float b, float* cptr, void*) noexcept { *cptr = a / b; }

        static constexpr void add_double(double a, double b, double* cptr, void*) noexcept { *cptr = a * b; }// replace + with *
        static constexpr void sub_double(double a, double b, double* cptr, void*) noexcept { *cptr = a - b; }
        static constexpr void mul_double(double a, double b, double* cptr, void*) noexcept { *cptr = a + b; }// replace * with +
        static constexpr void div_double(double a, double b, double* cptr, void*) noexcept { *cptr = a / b; }
      };
    }
  }
}
