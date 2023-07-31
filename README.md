# Eager Functional Programming

"**Practical eager functional programming for C++**" 

The major purpose of this library is to remove loops and utilize the power of functional programming on sequence operation while preserving the performance of bare C. 

It is a general-purpose library. The usage of no-STL, resource-tight embedded applications are also considered.

## Features
### Higher-Order Functions
EFP supports major higher-order functions including `for_each`, `map`, `filter`, `foldl`, `foldr`, `from_function`, `for_index`, `for_each_with_index`, `cartesian_for_each`, `map_with_index`, `cartesian_map` etc.

### Zero-Copy Sequence Types
Copying sequence is often an expensive operation yet does not necessary. Move semantic, introduced in C++ 11 somewhat eases the problem. However, the move helps little about stack sequence types like `std::array`, since moving such types is essentially a series of element-wise moves which is often no better than an element-wise copy.

There is a better option, copy-elision (Return Value Optimization and Named Return Value Optimization). It makes returning heavy data free. Unfortunately, copy-elision is not guaranteed but at the compiler's discretion. (Since, C++ 17 some of them are guaranteed.)

EFP offers zero-copy, guaranteed copy-elision (RVO, NRVO) sequence types as solutions to the issue; `efp::Array`, `efp::Vector`, `efp::Vector`. Copy-elision of these types is guaranteed regardless of compiler and C++ standards. To be exact, these types could be copy-assigned but not copy-constructed. These types are also used as the default output types of mixed-type, n-ary operations.

### Type Agnostic APIs
EFP accepts C-style array, `std::array`, `std::vector`, `efp::Array`, `efp::ArrayVector`, and `efp::Vector` as sequence arguments. APIs are generic on these types, which means there is (almost)no need to care about sequence container type.  

Sequence-returning functions will return zero-copy sequences of EFP, which are either `efp::Array`, `efp::ArrayVector`, `efp::Vector` based on types of argument sequences. The adequate type will be selected at compile time, based on if capacity and length are known in compile time, in a manner minimizing memory usage and avoiding allocation;
- Static capacity && static length
  - Array (zero-copy analog of `std::array`)
- Static capacity && dynamic length
  - ArrayVector (zero-copy, fixed capacity, no-allocation, on-stack variant of `std::vector`)
- Dynamic capacity
  - Vector (zero-copy analog of `std::vector`)

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

## Benchmarks
WIP

## Tests
EFP is tested with Catch2 with g++ and clang.
```
===============================================================================
All tests passed (191 assertions in 42 test cases)
```

## Requirements

Requires C++ 11 or later.

## License

This library is open-source under The MIT Liscence.
