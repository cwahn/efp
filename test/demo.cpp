#include <iostream>

#include "efp.hpp"

using namespace efp;

constexpr size_t n = 10;

int main()
{
    auto is_even = [](int x)
    {
        return x % 2 == 0;
    };

    auto stdout = [&] (int x)
    {
        std::cout << x << " ";
    };

    auto as = from_function(StaticSizeT<n>{}, id<int>);

    auto bs = map(square<int>, as);

    auto cs = filter(is_even, bs);

    for_each(stdout, cs); // 0 4 16 36 64 

    return 0;
}