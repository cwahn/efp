#ifndef CRTP_PRELUDE_TEST_HPP_
#define CRTP_PRELUDE_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "crtp_prelude.hpp"

using namespace efp;

TEST_CASE("for_each")
{
    Arr<int, 3> crtp_array{1, 2, 3};

    int res_1 = 0;
    auto add_to_res_1 = [&](int x)
    { res_1 += x; };

    for_each(add_to_res_1, crtp_array);
    CHECK(res_1 == 6);
}

TEST_CASE("map")
{
    Arr<int, 3> crtp_array{1, 2, 3};

    auto times_2 = [](int x)
    { return 2 * x; };

    CHECK(map(times_2, crtp_array) == Arr<int, 3>{2, 4, 6});
}

TEST_CASE("filter")
{
    Arr<int, 3> crtp_array{1, 2, 3};

    auto is_even = [](int x)
    {
        return x % 2 == 0 ? true : false;
    };

    ArrayVec<int, 3> ref{};
    ref.resize(1);
    ref[0] = 2;

    CHECK(filter(is_even, crtp_array) == ref);
}

TEST_CASE("foldl")
{
    ArrayVec<int, 5> crtp_arrayvec{1, 2, 3, 4};
    auto plus = [](int lhs, int rhs)
    { return lhs + rhs; };

    CHECK(foldl(plus, 0, crtp_arrayvec) == 10);
}

TEST_CASE("foldr")
{
    ArrayVec<int, 5> crtp_arrayvec{1, 2, 3, 4};
    auto plus = [](int lhs, int rhs)
    { return lhs + rhs; };

    CHECK(foldr(plus, 0, crtp_arrayvec) == 10);
}

#endif