#include "counters.h"

#include <iostream>
#include <iomanip>

#define PRINT(prec, op, mode) (((*this)[counter_type::##op##_##prec##_##mode##_sse] || (*this)[counter_type::##op##_##prec##_##mode##_avx] || (*this)[counter_type::##op##_##prec##_##mode##_avx512]) ? \
std::cerr                                                                                   \
          << std::setw(17) << std::left << #prec                                            \
          << std::setw(17) << std::left << #op                                              \
          << std::setw(21) << std::left << #mode                                            \
          << std::setw(17) << std::right << (*this)[counter_type::##op##_##prec##_##mode##_sse] + (*this)[counter_type::##op##_##prec##_##mode##_avx] + (*this)[counter_type::##op##_##prec##_##mode##_avx512]  \
<< " (" << (*this)[counter_type::##op##_##prec##_##mode##_sse] << "/" << (*this)[counter_type::##op##_##prec##_##mode##_avx] << "/" << (*this)[counter_type::##op##_##prec##_##mode##_avx512] << ")"     \
          << std::endl : std::cerr << "")
#define NO_SSE_PRINT(prec, op, mode) (((*this)[counter_type::##op##_##prec##_##mode##_avx] || (*this)[counter_type::##op##_##prec##_##mode##_avx512]) ? \
std::cerr                                                                                   \
          << std::setw(17) << std::left << #prec                                            \
          << std::setw(17) << std::left << #op                                              \
          << std::setw(21) << std::left << #mode                                            \
          << std::setw(17) << std::right << (*this)[counter_type::##op##_##prec##_##mode##_avx] + (*this)[counter_type::##op##_##prec##_##mode##_avx512]  \
<< " (0/" << (*this)[counter_type::##op##_##prec##_##mode##_avx] << "/" << (*this)[counter_type::##op##_##prec##_##mode##_avx512] << ")"     \
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
    std::cout << "--------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Precision        Operation        Vectorization           Instruction count (sse/avx/avx512)" << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------" << std::endl;
    PRINT(float, add, scalar);
    PRINT(float, add, simd);
    PRINT(float, mul, scalar);
    PRINT(float, mul, simd);
    PRINT(float, div, scalar);
    PRINT(float, div, simd);
    PRINT(double, add, scalar);
    PRINT(double, add, simd);
    PRINT(double, mul, scalar);
    PRINT(double, mul, simd);
    PRINT(double, div, scalar);
    PRINT(double, div, simd);
    NO_SSE_PRINT(float, fma, scalar);
    NO_SSE_PRINT(float, fma, simd);
    NO_SSE_PRINT(double, fma, scalar);
    NO_SSE_PRINT(double, fma, simd);
    PRINT(i2f, cvt, scalar);
    PRINT(i2d, cvt, scalar);
    PRINT(f2i, cvt, scalar);
    PRINT(f2d, cvt, scalar);
    PRINT(d2i, cvt, scalar);
    PRINT(d2f, cvt, scalar);
    PRINT(i2f, cvt, simd);
    PRINT(i2d, cvt, simd);
    PRINT(f2i, cvt, simd);
    PRINT(f2d, cvt, simd);
    PRINT(d2i, cvt, simd);
    PRINT(d2f, cvt, simd);
    NO_SSE_PRINT(h2f, cvt, simd);
    NO_SSE_PRINT(f2h, cvt, simd);
    std::cout << "--------------------------------------------------------------------------------------------" << std::endl;
  }
}