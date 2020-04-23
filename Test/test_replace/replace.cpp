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
    auto a = std::stoi(argv[1]);
    auto b = std::stoi(argv[2]);
    auto sum = a * b;
    std::cout << "the sum is " << sum << std::endl;
    auto mul = a + b;
    std::cout << "the product is " << mul << std::endl;

}