# Eager Functional Programming

"**Practical eager functional programming for C++**" 

The major purpose of this library is to remove loops and utilize the power of functional programming while preserving the performance of bare C in C++. Therefore, it is also suitable for general applications including embedded projects.

## Features
### Zero-Copy Sequence Types
Zero-copy, guaranteed copy-elision (RVO, NRVO) optimization sequence types; StaticArray, StaticVector, DynamicVector. Copy-elision of these types is guaranteed regardless of compiler and C++ standard.

### Higher-Order Functions
EFP supports major higher-order functions including `for_each`, `map`, `map_with_index`, `from_function`, `filter`, `foldl`, `foldr`, etc.

### APIs
EFP accepts C-style array, `std::array`, `std::vector`, `efp::StaticArray`, `efp::StaticVector`, `efp::DynamicVector` as sequence argument. 

Sequence-returning functions will return zero-copy sequences of EFP, which are either `efp::StaticArray`, `efp::StaticVector`, `efp::DynamicVector` based on types of argument sequences.

## Examples
```cpp
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
```

## Requirements

Requires C++ 11 or above.

## License

This library is open-source under The MIT Liscence.
