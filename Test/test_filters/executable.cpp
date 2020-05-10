#include <iostream>
#include "header.h"

extern "C" float shared_func(float, float);
extern "C" double shared_func(double, double);

extern "C" float func(float a, float b)
{
  auto a2 = func(a, b);
  std::cout << "func(" << a << "(, " << b << ") = " << a2 << std::endl;
  auto b2 = func(b, a);
  std::cout << "func(" << b << "(, " << a << ") = " << b2 << std::endl;

  auto t1 = compute1(a, b);
  std::cout << "func::compute1(" << a << "(, " << b << ") = " << t1 << std::endl;
  auto t2 = compute2(t1, b);
  std::cout << "func::compute1(" << a << "(, " << b << ") = " << t2 << std::endl;
  auto a3 = (t1 + t2) / 2;
  std::cout << "func::(" << t1 << " + " << t2 << ") = " << a3 << std::endl;
}
extern "C" double func(double a, double b)
{
  auto a2 = func(a, b);
  std::cout << "func(" << a << "(, " << b << ") = " << a2 << std::endl;
  auto b2 = func(b, a);
  std::cout << "func(" << b << "(, " << a << ") = " << b2 << std::endl;

  auto t1 = compute1(a, b);
  std::cout << "func::compute1(" << a << "(, " << b << ") = " << t1 << std::endl;
  auto t2 = compute2(t1, b);
  std::cout << "func::compute1(" << a << "(, " << b << ") = " << t2 << std::endl;
  auto a3 = (t1 + t2) / 2;
  std::cout << "func::(" << t1 << " + " << t2 << ") = " << a3 << std::endl;
}

void main()
{
  func(5.f, 10.f);
  func(5., 10.);
}

