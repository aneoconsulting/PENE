#pragma once

namespace pene {
  union counters
  {
    using int_type = unsigned long long;
    static constexpr int size = 16;
    struct named_t {
      int_type add_float_scalar;
      int_type mul_float_scalar;
      int_type div_float_scalar;
      int_type fma_float_scalar;
      int_type add_double_scalar;
      int_type mul_double_scalar;
      int_type div_double_scalar;
      int_type fma_double_scalar;
      int_type add_float_simd;
      int_type mul_float_simd;
      int_type div_float_simd;
      int_type fma_float_simd;
      int_type add_double_simd;
      int_type mul_double_simd;
      int_type div_double_simd;
      int_type fma_double_simd;
    };
    named_t named;
    int_type array[size];

    static_assert(sizeof(named_t) / sizeof(int_type) == size);

    counters();

    void print() const;
  };
}