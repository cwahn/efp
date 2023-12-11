# Eager Functional Programming

"**Practical eager functional programming for C++**" 

The major purpose of this header-only library is to leverage the power of functional programming while preserving the performance of bare C. 

It is a general-purpose library. The usage of no-STL, resource-tight embedded applications are also considered. (no-STL is WIP)

## Features
### Higher-Order Functions
EFP supports major higher-order functions including   

**Composition and Utility Functions:** `compose`, `execute_pack`

**Element Access and Manipulation:** `head`, `last`, `tail`, `init`

**Mapping Functions:** `map`, `map_with_index`, `cartesian_map`

**Folding Functions:** `foldl`, `foldr`

**Filtering Functions:** `filter`, `take_while`, `drop_while`

**Iteration Functions:** `for_each`, `for_each_mut`, `for_each_with_index`, `for_each_with_index_mut`, `cartesian_for_each`, `cartesian_for_each_mut`, `for_index`, `cartesian_for_index`

**Sequence Operations:** `append`, `slice`, `slice_unsafe`, `take`, `take_unsafe`, `drop`, `drop_unsafe`

**Element Search and Indexing:** `elem`, `elem_index`, `elem_indices`, `find`, `find_index`, `find_indices`

and many more.

### Contegious Sequence Types
EFP offeres no-STL contegious sequence types and immutable conteguous view types:  `efp::Array`, `efp::ArrVec`, `efp::Vector`, `efp::ArrayView`, `efp::ArrVecView`, and `efp::VectorView`

### Compile-Time Polymorphism
HOFs of EFP acceptsequence arguments. APIs are generic on these types, which means there is (almost)no need to care about sequence container type.  

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

### String and formatting
EFP provides a `String` class tailored for efficient manipulation, which is basically `Vector<char>` with some specialization, supporting operations without explicit null-termination. The `format` function enables embedded variable formatting, harnessing the library's type-safe templates for concise and expressive string composition at compile-time. 

## Examples
### General
```cpp
#include <iostream>
#include "efp.hpp"

using namespace efp;

constexpr int n = 10;

int main()
{
    auto negate = [](int x)
    { return -x; };

    auto is_even = [](int x)
    { return x % 2 == 0; };

    auto stdout = [&](int x)
    { std::cout << x << " "; };

    auto as = from_function(Int<n>{}, id<int>);
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
    [&]()                       
    { return b * 1; }); // Empty branch indicates wildcard

CHECK(b == 42);
```

### File IO
```c++
file = File::open("test", "w+").move();
file.write("Hello, world!\nThis is a test file.\nEnd of test.");
file.close();
```

<!-- ### String and formatting
```cpp
String name = "Alice";
Vector<char> hobby = "coding";

String result = join(", ", format("Name: {}", name), format("Hobby: {}", hobby));

std::cout << result << std::endl; // Output: "Name: Alice, Hobby: coding"
```  -->

## Benchmarks
WIP

## Tests
EFP is tested with Catch2 with GCC and Clang.
```
===============================================================================
All tests passed (779 assertions in 99 test cases)
```

## Requirements

Requires C++ 11 or later.

## Acknowledgement
- format
  - The formatting functionality of the library is heavily influenced by [tfc/ppprint](https://github.com/tfc/pprintpp.git). The great idea of compile-time formatting with C++11 has an excellent fit with SFINAE tools of EFP and rewritten in favor of EFP's language and some additional support for EFP String.
  - 
## License

Drafting of this library is started in the July of 2023 by Ahn, Chanwoo for an embedded project. 

This library is open-source under The MIT Liscence.
# Eager Functional Programming

"**Practical eager functional programming for C++**" 

The major purpose of this header-only library is to leverage the power of functional programming while preserving the performance of bare C. 

It is a general-purpose library. The usage of no-STL, resource-tight embedded applications are also considered. (no-STL is WIP)

## Features
### Higher-Order Functions
EFP supports major higher-order functions including   

**Composition and Utility Functions:** `compose`, `execute_pack`

**Element Access and Manipulation:** `head`, `last`, `tail`, `init`

**Mapping Functions:** `map`, `map_with_index`, `cartesian_map`

**Folding Functions:** `foldl`, `foldr`

**Filtering Functions:** `filter`, `take_while`, `drop_while`

**Iteration Functions:** `for_each`, `for_each_mut`, `for_each_with_index`, `for_each_with_index_mut`, `cartesian_for_each`, `cartesian_for_each_mut`, `for_index`, `cartesian_for_index`

**Sequence Operations:** `append`, `slice`, `slice_unsafe`, `take`, `take_unsafe`, `drop`, `drop_unsafe`

**Element Search and Indexing:** `elem`, `elem_index`, `elem_indices`, `find`, `find_index`, `find_indices`

and many more.

### Contegious Sequence Types
EFP offeres no-STL contegious sequence types and immutable conteguous view types:  `efp::Array`, `efp::ArrVec`, `efp::Vector`, `efp::ArrayView`, `efp::ArrVecView`, and `efp::VectorView`

### Compile-Time Polymorphism
HOFs of EFP acceptsequence arguments. APIs are generic on these types, which means there is (almost)no need to care about sequence container type.  

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

### String and formatting
EFP provides a `String` class tailored for efficient manipulation, which is basically `Vector<char>` with some specialization, supporting operations without explicit null-termination. The `format` function enables embedded variable formatting, harnessing the library's type-safe templates for concise and expressive string composition at compile-time. 

## Examples
### General
```cpp
#include <iostream>
#include "efp.hpp"

using namespace efp;

constexpr int n = 10;

int main()
{
    auto negate = [](int x)
    { return -x; };

    auto is_even = [](int x)
    { return x % 2 == 0; };

    auto stdout = [&](int x)
    { std::cout << x << " "; };

    auto as = from_function(Int<n>{}, id<int>);
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
    [&]()                       
    { return b * 1; }); // Empty branch indicates wildcard

CHECK(b == 42);
```

### File IO
```c++
file = File::open("test", "w+").move();
file.write("Hello, world!\nThis is a test file.\nEnd of test.");
file.close();
```

<!-- ### String and formatting
```cpp
String name = "Alice";
Vector<char> hobby = "coding";

String result = join(", ", format("Name: {}", name), format("Hobby: {}", hobby));

std::cout << result << std::endl; // Output: "Name: Alice, Hobby: coding"
```  -->

## Benchmarks
WIP

## Tests
EFP is tested with Catch2 with GCC and Clang.
```
===============================================================================
All tests passed (779 assertions in 99 test cases)
```

## Requirements

Requires C++ 11 or later.

## Acknowledgement
- format
  - The formatting functionality of the library is heavily influenced by [tfc/ppprint](https://github.com/tfc/pprintpp.git). The great idea of compile-time formatting with C++11 has an excellent fit with SFINAE tools of EFP and rewritten in favor of EFP's language and some additional support for EFP String.
  - 
## License

Drafting of this library is started in the July of 2023 by Ahn, Chanwoo for an embedded project. 

This library is open-source under The MIT Liscence.
