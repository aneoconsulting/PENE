#include <iostream>
#define LIBRARY_EXPORTS
#include "header.h"

LIBRARY_API float shared_func_f(float a, float b)
{
  auto t1 = compute1(a, b);
  std::cout << "shared_func_f::compute1(" << a << ", " << b << ") = " << t1 << std::endl;
  auto t2 = compute2(t1, b);
  std::cout << "shared_func_f::compute2(" << a << ", " << b << ") = " << t2 << std::endl;
  auto out = (t1 + t2) / 2;
  std::cout << "shared_func_f::(" << t1 << " + " << t2 << ")/2 = " << out << std::endl;
  return out;
}

LIBRARY_API double shared_func_d(double a, double b)
{
  auto t1 = compute1(a, b);
  std::cout << "shared_func_d::compute1(" << a << ", " << b << ") = " << t1 << std::endl;
  auto t2 = compute2(t1, b);
  std::cout << "shared_func_d::compute2(" << a << ", " << b << ") = " << t2 << std::endl;
  auto out = (t1 + t2) / 2;
  std::cout << "shared_func_d::(" << t1 << " + " << t2 << ")/2 = " << out << std::endl;
  return out;
}