#include <iostream>
#include "efp.hpp"

// #include <cassert>
// #include <cstdio>
// #include <cstring>

using namespace efp;

int main()
{
    print("Hello, world!\n");
    const int a = 42;
    print("a: {}\n", a);
    print("s: {s}\n", "This is string");

    const String str0 = "This is 42";
    std::cout << str0 << std::endl;

    const auto str = format("This is {}", 42);
    std::cout << str << std::endl;

    const auto fmt0 = auto_format("a: {}", a);
    const auto fmt1 = auto_format("s: {s}", "This is string");

    std::cout << fmt0 << std::endl;
    std::cout << fmt1 << std::endl;

    return 0;
}