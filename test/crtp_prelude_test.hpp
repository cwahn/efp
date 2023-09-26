#ifndef CRTP_PRELUDE_TEST_HPP_
#define CRTP_PRELUDE_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "crtp_prelude.hpp"

using namespace efp;

TEST_CASE("for_each")
{
    Array<int, 3> crtp_array{1, 2, 3};

    int res_1 = 0;
    auto add_to_res_1 = [&](int x)
    { res_1 += x; };

    for_each(add_to_res_1, crtp_array);
    CHECK(res_1 == 6);
}

TEST_CASE("map")
{
    Array<int, 3> crtp_array{1, 2, 3};

    auto times_2 = [](int x)
    { return 2 * x; };

    CHECK(map(times_2, crtp_array) == Array<int, 3>{2, 4, 6});
}

TEST_CASE("filter")
{
    Array<int, 3> crtp_array{1, 2, 3};

    auto is_even = [](int x)
    {
        return x % 2 == 0 ? true : false;
    };

    ArrVec<int, 3> ref{};
    ref.resize(1);
    ref[0] = 2;

    CHECK(filter(is_even, crtp_array) == ref);
}

TEST_CASE("foldl")
{
    ArrVec<int, 5> crtp_arrayvec{1, 2, 3, 4};
    auto plus = [](int lhs, int rhs)
    { return lhs + rhs; };

    CHECK(foldl(plus, 0, crtp_arrayvec) == 10);
}

TEST_CASE("foldr")
{
    ArrVec<int, 5> crtp_arrayvec{1, 2, 3, 4};
    auto plus = [](int lhs, int rhs)
    { return lhs + rhs; };

    CHECK(foldr(plus, 0, crtp_arrayvec) == 10);
}

TEST_CASE("from_function")
{
    auto plus_one = [](int i)
    {
        return i + 1;
    };

    SECTION("Array")
    {
        CHECK(from_function(IntegralConst<int, 3>{}, plus_one) == Array<int, 3>{1, 2, 3});
    }

    SECTION("Vector")
    {
        CHECK(from_function(3, plus_one) == Vector<int>{1, 2, 3});
    }
}

TEST_CASE("for_index")
{
    int res = 0;

    auto add_to_res = [&](int i)
    {
        res += i;
    };

    for_index(add_to_res, 0);
    CHECK(res == 0);
    for_index(add_to_res, 3);
    CHECK(res == 3);
    for_index(add_to_res, -1);
    CHECK(res == 3);
}

TEST_CASE("for_each_with_index")
{
    Array<int, 3> res;

    auto product = [&](int i, int x0, int x1)
    {
        res[i] = x0 * x1;
    };

    for_each_with_index(product, array_3, vector_5);
    CHECK(res == Array<int, 3>{1, 4, 9});
}

TEST_CASE("cartesian_for_each")
{
    Vector<int> res{};
    auto as = Array<int, 2>{1, 2};
    auto bs = Array<int, 2>{1, 3};

    auto push_res = [&](int x0, int x1)
    {
        res.push_back(10 * x0 + x1);
    };

    cartesian_for_each(push_res, as, bs);
    CHECK(res == Vector<int>{11, 13, 21, 23});
}

TEST_CASE("map_with_index")
{
    SECTION("array")
    {
        auto times = [](int lhs, int rhs)
        {
            return lhs * rhs;
        };

        CHECK(map_with_index(times, array_3) == Array<int, 3>{0, 2, 6});
    }

    SECTION("vector")
    {
        auto times = [](double lhs, double rhs)
        {
            return lhs * rhs;
        };

        CHECK(map_with_index(times, vector_3) == Vector<double>{0., 2., 6.});
    }

    SECTION("binary hetero")
    {
        auto product = [](int i, double a, double b)
        {
            return i * a * b;
        };

        CHECK(map_with_index(product, array_3, vector_3) == Vector<double>{0., 4., 18.});
    }
}

#endif