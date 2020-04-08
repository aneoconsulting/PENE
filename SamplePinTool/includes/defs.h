#pragma once

#include "includes/interflop.h"

namespace interflop {
  template <typename Derived>
  class backend
  {
  private:
    Derived& derived() { return static_cast<Derived&>(*this); }
  public:
    void add_float(float a, float b, float* cptr) { derived().add_float(a, b, cptr); }
    void sub_float(float a, float b, float* cptr) { derived().sub_float(a, b, cptr); }
    void mul_float(float a, float b, float* cptr) { derived().mul_float(a, b, cptr); }
    void div_float(float a, float b, float* cptr) { derived().div_float(a, b, cptr); }

    void add_double(double a, double b, double* cptr) { derived().add_double(a, b, cptr); }
    void sub_double(double a, double b, double* cptr) { derived().sub_double(a, b, cptr); }
    void mul_double(double a, double b, double* cptr) { derived().mul_double(a, b, cptr); }
    void div_double(double a, double b, double* cptr) { derived().div_double(a, b, cptr); }
  };

  class interflop_generic_backend : public backend<interflop_generic_backend>
  {
  private:
    interflop_backend_interface_t interface;
    void* context;
  public:
    interflop_generic_backend() {}
    template <interflop_backend_interface_t(*INITIATOR)(void**)>
    void init() { interface = INITIATOR(&context); }
    void add_float(float a, float b, float* cptr) { interface.interflop_add_float(a, b, cptr, context); }
    void sub_float(float a, float b, float* cptr) { interface.interflop_sub_float(a, b, cptr, context); }
    void mul_float(float a, float b, float* cptr) { interface.interflop_mul_float(a, b, cptr, context); }
    void div_float(float a, float b, float* cptr) { interface.interflop_div_float(a, b, cptr, context); }

    void add_double(double a, double b, double* cptr) { interface.interflop_add_double(a, b, cptr, context); }
    void sub_double(double a, double b, double* cptr) { interface.interflop_sub_double(a, b, cptr, context); }
    void mul_double(double a, double b, double* cptr) { interface.interflop_mul_double(a, b, cptr, context); }
    void div_double(double a, double b, double* cptr) { interface.interflop_div_double(a, b, cptr, context); }
  };
}
