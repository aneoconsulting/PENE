#pragma once

struct counters
{
    unsigned long long add;
    counters() :add(0) {}

    static void print(counters c)
    {
        std::cout << "counters : ";
        std::cout << c.add << std::endl;
    }
};