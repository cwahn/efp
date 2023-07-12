#include <cstdlib>
#include <iostream>
#include <vector>
#include <array>

#include "prelude.hpp"
#include "numeric.hpp"

constexpr size_t n = 10;

int main(int argc, char const *argv[])
{
    std::cout << "c, array, n = " << n << std::endl;

    // int xs[n];
    // std::vector<int> xs(n);
    std::array<int, n> xs;

    // std::iota(std::begin(xs), std::end(xs), 0);

    for (int i = 0; i < n; ++i)
    {
        xs[i] = std::rand();
    }

    // DynamicVector<int> ys(n);
    StaticArray<int, n> ys;

    for (int i = 0; i < n; ++i)
    {
        ys[i] = square<double>(xs[i]);
    }
    for (int i = 0; i < n; ++i)
    {
        ys[i] = sqrt<double>(ys[i]);
    }

    for (int i = 0; i < n; ++i)
    {
        ys[i] = square<double>(ys[i]);
    }
    for (int i = 0; i < n; ++i)
    {
        ys[i] = sqrt<double>(ys[i]);
    }

    for (int i = 0; i < n; ++i)
    {
        ys[i] = square<double>(ys[i]);
    }
    for (int i = 0; i < n; ++i)
    {
        ys[i] = sqrt<double>(ys[i]);
    }

    for (int i = 0; i < n; ++i)
    {
        ys[i] = square<double>(ys[i]);
    }
    for (int i = 0; i < n; ++i)
    {
        ys[i] = sqrt<double>(ys[i]);
    }

    for (int i = 0; i < n; ++i)
    {
        ys[i] = square<double>(ys[i]);
    }
    for (int i = 0; i < n; ++i)
    {
        ys[i] = sqrt<double>(ys[i]);
    }

    double sum = 0;

    for (int i = 0; i < n; ++i)
    {
        sum += ys[i];
    }

    std::cout << "Result: " << sum / double(n) << std::endl;

    return 0;
}
