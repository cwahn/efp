# Eager Functional Programming

"**Practical eager functional programming for C++**" 

The major purpose of this header-only library is to leverage the power of functional programming while preserving the performance of bare C. 

It is a general-purpose library. The usage of no-STL, resource-tight embedded applications are also considered. (no-STL is WIP)

## Features
### Higher-Order Functions
EFP supports major higher-order functions including `for_each`, `map`, `filter`, `foldl`, `foldr`, `from_function`, `for_index`, `for_each_with_index`, `cartesian_for_each`, `map_with_index`, `cartesian_map` and many more.

### Zero-Copy Sequence Types
Copying sequence is often an expensive operation yet does not necessary. Move semantic, introduced in C++ 11 somewhat eases the problem. However, the move helps little about stack sequence types like `std::array`, since moving such types is essentially a series of element-wise moves which is often no better than an element-wise copy.

There is a better option, copy-elision (Return Value Optimization and Named Return Value Optimization). It makes returning heavy data free. Unfortunately, copy-elision is not guaranteed but at the compiler's discretion. (Since, C++ 17 some of them are guaranteed.)

EFP offers zero-copy, guaranteed copy-elision (RVO, NRVO) sequence type as solutions to the issue; `efp::Sequence`, or `efp::Array`, `efp::ArrVec`, `efp::Vector` as aliased name. Copy-elision of these types is guaranteed regardless of compiler and C++ standards. To be exact, these types could be copy-assigned but not copy-constructed. These types are also used as the default output types of mixed-type, n-ary operations.

### Compile-Time Polymorphism
HOFs of EFP accept `efp::Sequence` as sequence arguments. APIs are generic on these types, which means there is (almost)no need to care about sequence container type.  

Sequence-returning functions will return zero-copy sequences of EFP, which are either `efp::Array`, `efp::ArrVec`, or `efp::Vector` based on the types of argument sequences. The adequate type will be selected at compile time, based on if capacity and length are known in compile time, in a manner minimizing memory usage and avoiding allocation;
- Static capacity && static length
  - Array (zero-copy analog of `std::array`)
- Static capacity && dynamic length
  - ArrVec (zero-copy, fixed capacity, no-allocation, on-stack variant of `std::vector`)
- Dynamic capacity
  - Vector (zero-copy analog of `std::vector`)

### Sum Type (Enum) with Pattern Matching

EFP supports sum-type similar to `std::variant` with pattern matching at C++ 11. The matching is 
- Coherent: The matching rule is the same as that of the overloaded function.
- Exhaustive: Non-exhaustive branches will fail to be compiled.
- Effective: Internally changes to switch-case statement.

One drawback of `efp::Enum` is memory usage. Like `std::variant` Enum needs extra space to store the runtime tag of the stored variant. The tag itself is only 1 byte (hence support 256 variant at max for now), but because of the memory alignment, the size of `efp::Enum` tends to be twice the size of the largest variant.

### Maybe
EFP provides sum-type `efp::Maybe` which is also Functor, Applicative, and Monad.  

WIP

## Examples
### General
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

### Sum Type Matching
```cpp
Enum<bool, int, double> a = 0;
int b = 42;

int c = a.match(
    [&](bool x)
    { return b * 0; },
    [&](int x)
    { return b * 1; },
    [&](double x)
    { return b * 2; });

CHECK(b == 42);
```

## Benchmarks
WIP

## Tests
EFP is tested with Catch2 with g++ and clang.
```
===============================================================================
All tests passed (344 assertions in 70 test cases)
```

## Requirements

Requires C++ 11 or later.

## License

Drafting of this library is started in the July of 2023 by Ahn, Chanwoo for an embedded project. 

This library is open-source under The MIT Liscence.
