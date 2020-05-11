#include "counters.h"

#include <iostream>
#include <iomanip>

#define PRINT(prec, op, mode,ins) ((*this)[counter_type::##op##_##prec##_##mode##_##ins] ? \
std::cerr                                                                                   \
          << std::setw(17) << std::left << #prec                                            \
          << std::setw(17) << std::left << #op                                              \
          << std::setw(17) << std::left << #mode                                            \
          << std::setw(17) << std::left << #ins                                            \
          << std::setw(17) << std::right << (*this)[counter_type::##op##_##prec##_##mode##_##ins]   \
          << std::endl : std::cerr << "")


namespace pene {
  counters::counters() : 
    array{0}
  {}

  counters::int_type& counters::operator[](const counter_type & ct)
  {
    return array[(UINT)ct];
  }
  const counters::int_type& counters::operator[](const counter_type & ct) const 
  {
    return array[(UINT)ct];
  }

  void counters::print() const
  {
    std::cout << "Displaying counters' information : " << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Precision        Operation        Vectorization        Instruction        Instruction count" << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    PRINT(float, add, scalar, sse);
    PRINT(float, add, simd, sse);
    PRINT(float, mul, scalar, sse);
    PRINT(float, mul, simd, sse);
    PRINT(float, div, scalar, sse);
    PRINT(float, div, simd, sse);
    PRINT(double, add, scalar, sse);
    PRINT(double, add, simd, sse);
    PRINT(double, mul, scalar, sse);
    PRINT(double, mul, simd, sse);
    PRINT(double, div, scalar, sse);
    PRINT(double, div, simd, sse);
    PRINT(float, add, scalar, avx);
    PRINT(float, add, simd, avx);
    PRINT(float, mul, scalar, avx);
    PRINT(float, mul, simd, avx);
    PRINT(float, div, scalar, avx);
    PRINT(float, div, simd, avx);
    PRINT(float, fma, scalar, avx);
    PRINT(float, fma, simd, avx);
    PRINT(double, add, scalar, avx);
    PRINT(double, add, simd, avx);
    PRINT(double, mul, scalar, avx);
    PRINT(double, mul, simd, avx);
    PRINT(double, div, scalar, avx);
    PRINT(double, div, simd, avx);
    PRINT(double, fma, scalar, avx);
    PRINT(double, fma, simd, avx);
    PRINT(float, add, scalar, avx512);
    PRINT(float, add, simd, avx512);
    PRINT(float, mul, scalar, avx512);
    PRINT(float, mul, simd, avx512);
    PRINT(float, div, scalar, avx512);
    PRINT(float, div, simd, avx512);
    PRINT(float, fma, scalar, avx512);
    PRINT(float, fma, simd, avx512);
    PRINT(double, add, scalar, avx512);
    PRINT(double, add, simd, avx512);
    PRINT(double, mul, scalar, avx512);
    PRINT(double, mul, simd, avx512);
    PRINT(double, div, scalar, avx512);
    PRINT(double, div, simd, avx512);
    PRINT(double, fma, scalar, avx512);
    PRINT(double, fma, simd, avx512);
    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
  }
}