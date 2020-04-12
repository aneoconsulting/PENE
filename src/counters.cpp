#include "counters.h"

#include <iostream>

#define SCRUTE(a) \
std::cout << #a << " " << named.##a << std::endl

namespace pene {
  counters::counters() : array{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} {}

  void counters::print() const
  {
    std::cout << "counters : " << std::endl;
    SCRUTE(add_float_scalar);
    SCRUTE(mul_float_scalar);
    SCRUTE(div_float_scalar);
    SCRUTE(fma_float_scalar);
    SCRUTE(add_double_scalar);
    SCRUTE(mul_double_scalar);
    SCRUTE(div_double_scalar);
    SCRUTE(fma_double_scalar);
    SCRUTE(add_float_simd);
    SCRUTE(mul_float_simd);
    SCRUTE(div_float_simd);
    SCRUTE(fma_float_simd);
    SCRUTE(add_double_simd);
    SCRUTE(mul_double_simd);
    SCRUTE(div_double_simd);
    SCRUTE(fma_double_simd);
  }
}