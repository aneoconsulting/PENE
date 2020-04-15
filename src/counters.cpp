#include "counters.h"

#include <iostream>


#define SCRUTE14(a) \
std::cout << #a << "                                             " << named.##a << std::endl
#define SCRUTE15(a) \
std::cout << #a << "                                            " << named.##a << std::endl
#define SCRUTE16(a) \
std::cout << #a << "                                           " << named.##a << std::endl
#define SCRUTE17(a) \
std::cout << #a << "                                          " << named.##a << std::endl

namespace pene {
  counters::counters() : 
    array{}
  {}

  void counters::print() const
  {
    std::cout << "Displaying counters' information : " << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "Operation_Precision_Vectorization                   Instruction count" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    SCRUTE16(add_float_scalar);
    SCRUTE16(mul_float_scalar);
    SCRUTE16(div_float_scalar);
    SCRUTE16(fma_float_scalar);
    SCRUTE17(add_double_scalar);
    SCRUTE17(mul_double_scalar);
    SCRUTE17(div_double_scalar);
    SCRUTE17(fma_double_scalar);
    SCRUTE14(add_float_simd);
    SCRUTE14(mul_float_simd);
    SCRUTE14(div_float_simd);
    SCRUTE14(fma_float_simd);
    SCRUTE15(add_double_simd);
    SCRUTE15(mul_double_simd);
    SCRUTE15(div_double_simd);
    SCRUTE15(fma_double_simd);
    std::cout << "---------------------------------------------------------------------" << std::endl;
  }
}