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
    #include "prelude.hpp"
    #include "signal.hpp"
    
    constexpr size_t n = 100;

    double a = 2;
    double b = 100;

    auto xs = from_function<StaticSizeT<n>{}, id<int>>;

    auto f = [&](double x)
    {
        return sin(x * 2 * M_PI / n) + a * x + b;
    };

    auto ys = map(f, xs);

    double a_hat, b_hat;
    std::tie(a_hat, b_hat) = linear_regression(xs, ys);

```

## Requirements

Requires C++ 11 or above.

## License

This library is open-source under The MIT Liscence.
