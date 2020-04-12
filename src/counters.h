#pragma once

namespace pene {
  struct counters
  {
    using int_type = unsigned long long;
    int_type add_float_scalar;
    int_type add_float_simd;
    int_type add_double_scalar;
    int_type add_double_simd;
    int_type mul_float_scalar;
    int_type mul_float_simd;
    int_type mul_double_scalar;
    int_type mul_double_simd;
    int_type div_float_scalar;
    int_type div_float_simd;
    int_type div_double_scalar;
    int_type div_double_simd;
    counters();

    void print() const;
  };
}