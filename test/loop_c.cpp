#include <cstdlib>
#include <iostream>
#include <vector>
#include <array>
#include <numeric>

#include "efp.hpp"

using namespace efp;

constexpr int n = 10000;

int main(int argc, char const *argv[])
{
    std::cout << "c, array, n = " << n << std::endl;

    // int xs[n];
    // std::vector<int> xs(n);
    std::array<int, n> xs;

    std::iota(std::begin(xs), std::end(xs), 0);

    // for (size_t i = 0; i < n; ++i)
    // {
    //     xs[i] = std::rand();
    // }

    // Vector<int> ys(n);
    Array<int, n> ys{};

    for (size_t i = 0; i < n; ++i)
    {
        ys[i] = square(xs[i]);
    }
    for (size_t i = 0; i < n; ++i)
    {
        ys[i] = sqrt<double>(ys[i]);
    }

    for (size_t i = 0; i < n; ++i)
    {
        ys[i] = square<double>(ys[i]);
    }
    for (size_t i = 0; i < n; ++i)
    {
        ys[i] = sqrt<double>(ys[i]);
    }

    for (size_t i = 0; i < n; ++i)
    {
        ys[i] = square<double>(ys[i]);
    }
    for (size_t i = 0; i < n; ++i)
    {
        ys[i] = sqrt<double>(ys[i]);
    }

    for (size_t i = 0; i < n; ++i)
    {
        ys[i] = square<double>(ys[i]);
    }
    for (size_t i = 0; i < n; ++i)
    {
        ys[i] = sqrt<double>(ys[i]);
    }

    for (size_t i = 0; i < n; ++i)
    {
        ys[i] = square<double>(ys[i]);
    }
    for (size_t i = 0; i < n; ++i)
    {
        ys[i] = sqrt<double>(ys[i]);
    }

    double sum = 0;

    for (size_t i = 0; i < n; ++i)
    {
        sum += ys[i];
    }

    std::cout << "Result: " << sum / double(n) << std::endl;

    return 0;
}
