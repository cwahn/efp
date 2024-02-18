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