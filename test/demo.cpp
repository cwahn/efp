#include <iostream>
#include "efp.hpp"

using namespace efp;

constexpr size_t n = 10;

int main()
{
    auto negate = [](int x)
    { return -x; };

    auto is_even = [](int x)
    { return x % 2 == 0; };

    auto stdout = [&](int x)
    { std::cout << x << " "; };

    auto as = from_function(StaticSizeT<n>{}, id<int>);

    auto minus_square = compose(negate, square<int>);

    auto bs = map(minus_square, as);

    auto cs = filter(is_even, bs);

    for_each(stdout, cs); // 0 -4 -16 -36 -64

    return 0;
}