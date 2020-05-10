#include "header.h"

extern "C" float shared_func(float a, float b)
{
  auto t1 = compute1(a, b);
  std::cout << "shared_func::compute1(" << a << "(, " << b << ") = " << t1 << std::endl;
  auto t2 = compute2(t1, b);
  std::cout << "shared_func::compute1(" << a << "(, " << b << ") = " << t2 << std::endl;
  auto out = (t1 + t2) / 2;
  std::cout << "shared_func::(" << t1 << " + " << t2 << ") = " << out << std::endl;
  return out;
}

extern "C" double shared_func(double a, double b)
{
  auto t1 = compute1(a, b);
  std::cout << "shared_func::compute1(" << a << "(, " << b << ") = " << t1 << std::endl;
  auto t2 = compute2(t1, b);
  std::cout << "shared_func::compute1(" << a << "(, " << b << ") = " << t2 << std::endl;
  auto out = (t1 + t2) / 2;
  std::cout << "shared_func::(" << t1 << " + " << t2 << ") = " << out << std::endl;
  return out;
}