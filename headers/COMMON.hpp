#pragma once

#define PRINT(x) std::cout << x << std::endl
#define SQ(x) std::pow(x, 2)

template <typename T> int sgnum(T val) 
{
    return (T(0) < val) - (val < T(0));
}