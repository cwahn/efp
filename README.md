# Eager Functional Programming

This library is for eager functional programming with C++. 

The major purpose of this library is to remove loop iterations, preserving the performance of bear C. Therefore, it is also suitable for embedded project.

## Features
### Zero-Copy Sequence Types
Zero-copy, guaranteed copy-elision (RVO, NRVO) optimization sequence types; StaticArray, StaticVector, DynamicVector.

### Higher Order Functions
Efp supports major HOFs including `for_each`, `map`, `map_with_index`, `from_function`, `filter`, `foldl`, `foldr`, etc.

## Examples
```
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
```

## Requirements

Requires C++ 11 or above.

## License

This library is open-source under The MIT Liscence.
