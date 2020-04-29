#include "counters.h"

#include <iostream>
#include <iomanip>

#define PRINT(prec, op, mode)                                                               \
std::cerr                                                                                   \
          << std::setw(17) << std::left << #op                                            \
          << std::setw(17) << std::left << #prec                                              \
          << std::setw(17) << std::left << #mode                                            \
          << std::setw(17) << std::right << (*this)[counter_type::##op##_##prec##_##mode]   \
          << std::endl


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
    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "Operation        Precision        Vectorization    Instruction count" << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    PRINT(float, add, scalar);
    PRINT(float, add, simd);
    PRINT(float, mul, scalar);
    PRINT(float, mul, simd);
    PRINT(float, div, scalar);
    PRINT(float, div, simd);
    PRINT(float, fma, scalar);
    PRINT(float, fma, simd);
    PRINT(double, add, scalar);
    PRINT(double, add, simd);
    PRINT(double, mul, scalar);
    PRINT(double, mul, simd);
    PRINT(double, div, scalar);
    PRINT(double, div, simd);
    PRINT(double, fma, scalar);
    PRINT(double, fma, simd);
    std::cout << "---------------------------------------------------------------------" << std::endl;
  }
}