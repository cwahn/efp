#include <iostream>
#include "efp.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>

using namespace efp;

int main()
{
#define TEST(correct_str, ...) \
    assert(!strcmp(AUTOFORMAT(__VA_ARGS__), correct_str))

    TEST("", "");

    TEST("%%", "%%");
    TEST("%d %f", "{} %f", 123, 1.23f);
    TEST("%f %d", "%f {}", 1.23f, 123);

    TEST(" { ", " \\{ ");
    TEST("{}", "\\{}");
    TEST(" { %d } ", " \\{ {} } ", 123);

    TEST("%p", "{}", nullptr);
    TEST("%p", "{}", reinterpret_cast<void *>(0));

    // For safety reasons:
    // Only print strings as strings, if the user also writes {s}
    TEST("%p", "{}", "str");
    // ! Not working as expected
    // TEST("%s", "{s}", "str");

    TEST("%c", "{}", static_cast<char>(123));

    TEST("%d", "{}", static_cast<short>(123));
    TEST("%d", "{}", 123);
    TEST("%ld", "{}", 123l);
    TEST("%lld", "{}", 123ll);

    TEST("%u", "{}", 123u);
    TEST("%lu", "{}", 123ul);
    TEST("%llu", "{}", 123ull);

    // TEST("%x", "{x}", 123u);
    // TEST("%lx", "{x}", 123ul);
    // TEST("%llx", "{x}", 123ull);

    TEST("%d", "{}", true);

    TEST("%f", "{}", 1.0f);
    TEST("%lf", "{}", 1.0);

    // TEST("%10d", "{10}", 123);
    // TEST("%10x", "{10x}", 123u);
    // TEST("%#10x", "{#10x}", 123u);

    // Give the user hex if asked for explicitly.
    // TEST("%x", "{x}", 123);
    // TEST("%lx", "{x}", 123l);
    // TEST("%llx", "{x}", 123ll);

    // puts("Green, green, green! All tests passed.\n");

    // pprintf("{s} {} {1} {} {} {} {} {} {} {} {} {} {} {} {} {#x}\n", "1", 2u, 3.0,
    //         4.0f, 5ull, '6', 7ul, 8, 9, 10, 11, 12, 13, 14, 15, 16u);

    pprintf("Hello, world!\n");
    const int a = 42;
    pprintf("a: {}\n", a);
    // ! String formating is not working correctly
    pprintf("s: {s}\n", "This is string");

    const String str0 = "This is 42";
    std::cout << str0 << std::endl;

    const auto str = format("This is {}", 42);
    std::cout << str << std::endl;

    return 0;
}