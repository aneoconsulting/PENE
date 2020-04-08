#include "ieee.h"
namespace interflop
{
  namespace backends {
    namespace ieee {
      constexpr float add_float(float a, float b) noexcept { return a + b; }
      constexpr float sub_float(float a, float b) noexcept { return a - b; }
      constexpr float mul_float(float a, float b) noexcept { return a * b; }
      constexpr float div_float(float a, float b) noexcept { return a / b; }
      constexpr double add_double(double a, double b) noexcept { return a + b; }
      constexpr double sub_double(double a, double b) noexcept { return a - b; }
      constexpr double mul_double(double a, double b) noexcept { return a * b; }
      constexpr double div_double(double a, double b) noexcept { return a / b; }

      void add_float(float a, float b, float* cptr, void*) noexcept { *cptr = add_float(a, b); }
      void sub_float(float a, float b, float* cptr, void*) noexcept { *cptr = sub_float(a, b); }
      void mul_float(float a, float b, float* cptr, void*) noexcept { *cptr = mul_float(a, b); }
      void div_float(float a, float b, float* cptr, void*) noexcept { *cptr = div_float(a, b); }
      void add_double(double a, double b, double* cptr, void*) noexcept { *cptr = add_double(a, b); }
      void sub_double(double a, double b, double* cptr, void*) noexcept { *cptr = sub_double(a, b); }
      void mul_double(double a, double b, double* cptr, void*) noexcept { *cptr = mul_double(a, b); }
      void div_double(double a, double b, double* cptr, void*) noexcept { *cptr = div_double(a, b); }

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

      void backend::add_float(float a, float b, float* cptr) { *cptr = ieee::add_float(a, b); }
      void backend::sub_float(float a, float b, float* cptr) { *cptr = ieee::sub_float(a, b); }
      void backend::mul_float(float a, float b, float* cptr) { *cptr = ieee::mul_float(a, b); }
      void backend::div_float(float a, float b, float* cptr) { *cptr = ieee::div_float(a, b); }
      void backend::add_double(double a, double b, double* cptr) { *cptr = ieee::add_double(a, b); }
      void backend::sub_double(double a, double b, double* cptr) { *cptr = ieee::sub_double(a, b); }
      void backend::mul_double(double a, double b, double* cptr) { *cptr = ieee::mul_double(a, b); }
      void backend::div_double(double a, double b, double* cptr) { *cptr = ieee::div_double(a, b); }
    }
  }
}