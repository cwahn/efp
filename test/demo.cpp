#include <iostream>

#include "prelude.hpp"
#include "numeric.hpp"

constexpr size_t n = 10;

int main()
{
    auto as = from_function(StaticSizeT<n>{}, id<int>);

    auto bs = map(square<int>, as);

    auto is_even = [](int x)
    {
        return x % 2 == 0;
    };

    auto cs = filter(is_even, bs);

    auto stdout = [&] (int x)
    {
        std::cout << x << " ";
    };

    for_each(stdout, cs); // 0 4 16 36 64 

    return 0;
}