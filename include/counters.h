#pragma once
#include <pin.H>

namespace pene {
  enum counter_type : UINT {
    add_double_scalar,
    add_double_simd,
    add_float_scalar,
    add_float_simd,
    div_double_scalar,
    div_double_simd,
    div_float_scalar,
    div_float_simd,
    fma_double_scalar,
    fma_double_simd,
    fma_float_scalar,
    fma_float_simd,
    mul_double_scalar,
    mul_double_simd,
    mul_float_scalar,
    mul_float_simd,
    cvt_double_scalar,
    cvt_double_simd,
    cvt_float_scalar,
    cvt_float_simd,
    oth_double_scalar,
    oth_double_simd,
    oth_float_scalar,
    oth_float_simd,
    size
  };

  struct counters
  {
    /// TODO: Provide a thread safe int_type similar to std::atomic<>
    using int_type = UINT64;
    static constexpr UINT size = counter_type::size;
    int_type array[size];
    counters();
    int_type& operator[](const counter_type &);
    const int_type& operator[](const counter_type &) const;
    void print() const;
  };
}