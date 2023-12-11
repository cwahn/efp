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
    std::cout << "efp, array, n = " << n << std::endl;

    const auto xs = from_function(Size<n>{}, id<int>);

    auto square_mut = [&](double &x)
    { x = square<double>(x); };

    auto sqrt_mut = [&](double &x)
    { x = sqrt<double>(x); };

    auto ys = map(square<double>, xs);
    for_eachi(sqrt_mut, ys);

    for_eachi(square_mut, ys);
    for_eachi(sqrt_mut, ys);

    for_eachi(square_mut, ys);
    for_eachi(sqrt_mut, ys);

    for_eachi(square_mut, ys);
    for_eachi(sqrt_mut, ys);

    for_eachi(square_mut, ys);
    for_eachi(sqrt_mut, ys);

    // 5
    //  auto ys = map(sqrt<double>, map(square<double>, map(sqrt<double>, map(square<double>, map(sqrt<double>, map(square<double>, map(sqrt<double>, map(square<double>, map(sqrt<double>, map(square<double>, xs))))))))));
    // 1
    // auto ys = map(sqrt<double>, map(square<double>, xs));

    double result = mean<double>(ys);

    std::cout << "Result: " << result << std::endl;

    return 0;
}
