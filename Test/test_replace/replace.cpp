#include <iostream>
#include <immintrin.h>
#include <string>

void main(int argc, char* argv[])
{
    std::cout << "This program is used for tests purposes only. "
        << std::endl;
    if (argc == 1) {
        argv = new char* [3]{ "main", "4", "6" };
    }
    auto a = std::stof(argv[1]);
    auto b = std::stof(argv[2]);
    auto sum = a * b;
    std::cout << a << "+" << b << "=" << sum << std::endl;
    auto mul = a + b;
    std::cout << a << "*" << b << "=" << mul << std::endl;

}