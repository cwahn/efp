#include <cstdlib>
#include <iostream>
#include <vector>
#include <array>

#include "prelude.hpp"
#include "numeric.hpp"

constexpr size_t n = 10000;

int main(int argc, char const *argv[])
{
    std::cout << "efp, array, n = " << n << " iter: " << 1 << std::endl;

    // int xs[n];
    // std::vector<int> xs(n);
    std::array<int, n> xs;

    // std::iota(std::begin(xs), std::end(xs), 0);

    for (int i = 0; i < n; ++i)
    {
        xs[i] = std::rand();
    }

    // 5
     auto ys = map(sqrt<double>, map(square<double>, map(sqrt<double>, map(square<double>, map(sqrt<double>, map(square<double>, map(sqrt<double>, map(square<double>, map(sqrt<double>, map(square<double>, xs))))))))));
    // 1
    // auto ys = map(sqrt<double>, map(square<double>, xs));

    double result = mean(ys);

    std::cout << "Result: " << result << std::endl;

    return 0;
}
