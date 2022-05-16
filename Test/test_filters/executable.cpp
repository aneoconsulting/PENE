#include <iostream>
#include <cassert>
#include <string>
#include "header.h"

extern "C" float shared_func_f(float, float);
extern "C" double shared_func_d(double, double);

extern "C" float func_f(float a, float b)
{
  auto a2 = shared_func_f(a, b);
  std::cout << "shared_func_f(" << a << ", " << b << ") = " << a2 << std::endl;

  auto t1 = compute1(a, b);
  std::cout << "func_f::compute1(" << a << ", " << b << ") = " << t1 << std::endl;
  auto t2 = compute2(t1, b);
  std::cout << "func_f::compute2(" << a << ", " << b << ") = " << t2 << std::endl;
  auto a3 = (t1 + t2) / 2;
  std::cout << "func_f(" << t1 << " + " << t2 << ")/2 = " << a3 << std::endl;
  return a3 - a2;
}
extern "C" double func_d(double a, double b)
{
  auto a2 = shared_func_d(a, b);
  std::cout << "shared_func_d(" << a << ", " << b << ") = " << a2 << std::endl;

  auto t1 = compute1(a, b);
  std::cout << "func_d::compute1(" << a << ", " << b << ") = " << t1 << std::endl;
  auto t2 = compute2(t1, b);
  std::cout << "func_d::compute2(" << a << ", " << b << ") = " << t2 << std::endl;
  auto a3 = (t1 + t2) / 2;
  std::cout << "func_d(" << t1 << " + " << t2 << ")/2 = " << a3 << std::endl;
  return a3 - a2;
}

int main(int argc, char* argv[])
{
  std::cout << "This program is used for tests purposes only. "
    << std::endl;
  if (argc == 1) {
    argv = new char* [3]{ "main", "5", "10" };
  }
  float a = std::stof(argv[1]);
  float b = std::stof(argv[2]);

  std::cout << "float" << std::endl;
  auto r1 = func_f(a, b);
  std::cout << "double" << std::endl;
  auto r2 = func_d(a, b);
  return 0;
}

