#pragma once

struct counters
{
    unsigned long long add;
    counters() :add(0) {}

    void print() const
    {
        std::cout << "counters : ";
        std::cout << add << std::endl;
    }
};