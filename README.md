# EFP

**"Expressive High-Performance Eager Functional Programming Library for C++"**

This is a header-only library for zero-cost functional programming in C++. It is highly influenced by the philosophy and style of Haskell and Rust, providing primitives leveraging the safety and expressive power of functional programming.

Two pillars of this library are the `Sequence` trait and the `Enum` container. `Sequence` traits effectively incarnate the concise sequential data manipulation of FP. `Enum` container is a disjoint union type like `std::variant` but based on C++ 11, and comes with Rust-like pattern matching. 

Support freestanding(no-STL) C++ with some limitations.

## Features

### Sequence Trait

Generalization of contiguous sequential containers

EFP offers no-STL contiguous sequence types and immutable view types implementing the Sequence trait: `Array`, `ArrVec`, `Vector`, `ArrayView`, `ArrVecView`, and `VectorView`

STL containers like `std::vector`, `std::array`, `std::string` also implement the `Sequence` trait. Therefore they could be used whenever sequence type is expected.

Also, the trait is not 'sealed'. Therefore one could implement `Sequence` trait for external types as well.

#### Higher Order Functions
EFP offers higher-order functions including   

- Composition and Utility Functions: `compose`, `execute_pack`
- Element Access and Manipulation: `head`, `last`, `tail`, `init`
- Mapping Functions: `map`, `map_with_index`, `cartesian_map`
- Folding Functions: `foldl`, `foldr`
- Filtering Functions: `filter`, `take_while`, `drop_while`
- Iteration Functions: `for_each`, `for_each_mut`, `for_each_with_index`, `for_each_with_index_mut`, `cartesian_for_each`, `cartesian_for_each_mut`, `for_index`, `cartesian_for_index`
- Sequence Operations: `append`, `slice`, `slice_unsafe`, `take`, `take_unsafe`, `drop`, `drop_unsafe`
- Element Search and Indexing: `elem`, `elem_index`, `elem_indices`, `find`, `find_index`, `find_indices`

and many more.

#### Automatic Allocation Avoidance
HOFs of EFP often expect arguments implementing the `Sequence` trait and return another `Sequence`. By taking advantage of the type-level function, EFP automatically determines the resulting type in a manner that minimizes allocation in compile-time. The adequate type will be determined based on whether the capacity and length of the result are known in compile time from argument types.

Sequence-returning functions will return either `Array`, `ArrVec`, or `Vector`.

- Static capacity && static length
  - Array (analog of `std::array`)
- Static capacity && dynamic length
  - ArrVec (fixed capacity, no-allocation, on-stack variant of `std::vector`)
- Dynamic capacity
  - Vector (analog of `std::vector`)

#### String and formatting
Just like in Haskell, `String` is `Vector<char>` in EFP (with minor difference on template argument). This enables string data manipulation with the same HOF used for all the other sequencial types. 

EFP also supports powerful formatting (and printing) features based on [fmtlib/fmt](https://github.com/fmtlib/fmt).

### Sum Type with Pattern Matching

EFP offers `Enum`, which is a sum type(or disjoint union type) similar to `std::variant` with rust-style pattern matching. 

- Exhaustive: Non-exhaustive branches will fail to be compiled.
- Performance: Compiles to switch-case statement on the runtime tag. 
- Coherent: The matching rule is the same as that of the overloaded function.
  
One drawback of `Enum` is memory usage. Like all the other tagged-union base implementations, `Enum` requires extra space to store the runtime tag of the stored variant. The tag itself is only 1 byte (hence supports 256 variants at maximum for now), but because of the memory alignment, the size of the resultant `Enum` tends to be larger than the size of the largest variant.

EFP provides `Maybe<A>` which is inheriting `Enum<Nothing, A>`. 

## Examples
### General
```cpp
#include <iostream>
#include "efp.hpp"

using namespace efp;

constexpr int n = 10;

int main() {
    const auto negate = [](int x) { return -x; };

    const auto is_even = [](int x) { return x % 2 == 0; };

    // Create a statically sized sequence, which is efp::Array<int, n> with identity function
    const auto as = from_function(Int<n> {}, id<int>);
    // Function composition
    const auto minus_square = compose(negate, square<int>);
    const auto bs = map(minus_square, as);
    const auto cs = filter(is_even, bs);

    println("Result is: {}", cs);  // Result is: 0 -4 -16 -36 -64

    const auto cout = [](int x) { std::cout << x << " "; };
    for_each(cout, cs);  // 0 -4 -16 -36 -64

    return 0;
}
```

### Enum and Maybe with Pattern Matching
```cpp
const Enum<bool, int, double> x = 2;  // Example with int

const int y = x.match(
    [](int x) { return x * 2; },  // Specific branch for int
    []() { return -1; }           // Wildcard branch
);
CHECK(y == 4);

// Maybe<A> is a specialization of Enum<Nothing, A>
const Maybe<int> maybe_42 = 42;
const Maybe<int> no_value = nothing;

// Using value() and isEmpty() for direct value access and checking
CHECK(maybe_42.value() == 42);
CHECK(no_value.is_nothing() == true);

const int result = maybe_42.match(
    [](int x) { return x; },  // Executes if there's a value
    []() { return 0; }        // Executes if empty (wildcard)
);
CHECK(result == 42);
```

### String and Formatting
```cpp
const String name_0 = "Lighthouse";
const String name_1 = "Tinker Bell";

println("{}", format("{} loves {}.", name_0, name_1));
``` 

### File IO
```c++
auto file = File::open("test", "w+").move();

file.write("Hello, world!\nThis is a test file.\nEnd of test.");
file.close();
```

## Benchmarks
WIP

## Testing

EFP undergoes extensive testing with Catch2, leveraging pedantic warnings and Address Sanitizer to guarantee superior code quality and reliability.

### Compatibility
- **Compilers:** Clang 15, GCC 13
- **C++ Standards:** C++11, C++14, C++17, C++20, C++23

### Continuous Integration
We are in the process of setting up CI to automate testing and ensure consistent quality.

### Test Suite Results
Our comprehensive tests affirm EFP's robustness:

```
===============================================================================
All tests passed (1055 assertions in 107 test cases)
```

## Requirements

- Requires C++ 11 or later.
- Also supporte freestanding env. with some limitations.

## Acknowledgement
- Formatting
  - The formatting functionality of EFP is based on the header-only version of the excellent [fmtlib/fmt](https://github.com/fmtlib/fmt).
  - There have been modifications to the internal implementation 
    - Now internal implementation is based on `efp::BasicString`.
    - APIs based on `efp::String`.
  
  
## License

Drafting of this library is started in July of 2023 by Ahn, Chanwoo for an embedded project. 

This library is open-source under The MIT Liscence.
