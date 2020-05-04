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
    float a = std::stof(argv[1]);
    float b = std::stof(argv[2]);
    float sum = a * b;
    float mul = a + b;
    
    std::cout << "following lines will be OK only if the addss and mulss operations are swapped by the pintool" << std::endl;
    std::cout << a << "+" << b << "=" << std::defaultfloat << sum << std::endl;
    std::cout << a << "*" << b << "=" << std::defaultfloat << mul << std::endl;

    auto c = sum-b;
    std::cout << c << std::endl;
}