#ifndef PRELUDE_TEST_HPP_
#define PRELUDE_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "test_common.hpp"
#include "prelude.hpp"

TEST_CASE("IsStaticCapacity")
{
    CHECK(IsStaticCapacity<int[3]>::value == true);
    CHECK(IsStaticCapacity<std::array<int, 3>>::value == true);
    CHECK(IsStaticCapacity<std::vector<int>>::value == false);
}

TEST_CASE("AreAllStaticCapacity")
{
    CHECK(AreAllStaticCapacity<int[3]>::value == true);
    CHECK(AreAllStaticCapacity<std::array<int, 3>>::value == true);
    CHECK(AreAllStaticCapacity<int[3], std::array<int, 3>>::value == true);
    CHECK(AreAllStaticCapacity<std::vector<int>>::value == false);
    CHECK(AreAllStaticCapacity<std::array<int, 3>, std::vector<int>>::value == false);
}

TEST_CASE("for_each")
{
    int res_1 = 0;
    auto add_1 = [&](int x)
    {
        res_1 += x;
    };

    for_each(add_1, c_array_3);
    CHECK(res_1 == 6);

    double res_2 = 0.;
    auto add_2 = [&](double x)
    {
        res_2 += x;
    };

    for_each(add_2, std_array_3);
    CHECK(res_2 == 6.);

    double res_3 = 0.;
    auto add_3 = [&](double x)
    {
        res_3 += x;
    };

    for_each(add_3, std_vector_3);
    CHECK(res_3 == 6.);

    double res_4 = 0.;
    auto add_product = [&](double x1, double x2)
    {
        res_4 += x1 * x2;
    };

    for_each(add_product, std_vector_3, c_array_3);
    CHECK(res_4 == 14.);
}

TEST_CASE("map")
{
    SECTION("c style array")
    {
        auto square = [](int x)
        {
            return x * x;
        };

        CHECK(map(square, c_array_3) == std::array<int, 3>{1, 4, 9});
    }

    SECTION("std::vector")
    {
        auto square = [](int x)
        {
            return x * x;
        };

        CHECK(map(square, std_vector_3) == std::vector<int>{1, 4, 9});
    }

    SECTION("binary of std::array, std::vecor")
    {
        auto plus = [](int lhs, int rhs)
        {
            return lhs + rhs;
        };
        std::vector<int> ref = {2, 4, 6};

        CHECK(map(plus, std_array_3, std_vector_3) == std::vector<int>{2, 4, 6});
    }
}

TEST_CASE("map_with_index")
{
    SECTION("c style array")
    {
        auto times = [](int lhs, int rhs)
        {
            return lhs * rhs;
        };

        CHECK(map_with_index(times, c_array_3) == std::array<int, 3>{0, 2, 6});
    }

    SECTION("std::vector")
    {
        auto times = [](int lhs, int rhs)
        {
            return lhs * rhs;
        };

        CHECK(map_with_index(times, std_vector_3) == DynamicVector<int>{0, 2, 6});
    }

    SECTION("binary of std::array, std::vecor")
    {
        auto product = [](int i, int a, int b)
        {
            return i * a * b;
        };

        CHECK(map_with_index(product, std_array_3, std_vector_3) == DynamicVector<int>{0, 4, 18});
    }
}

TEST_CASE("filter")
{
    SECTION("even c style array")
    {
        auto is_even = [](int x)
        {
            return x % 2 == 0;
        };

        CHECK(filter(is_even, c_array_3) == std::vector<int>{2});
    }

    SECTION("odd std::vector")
    {
        auto is_odd = [](int x)
        {
            return x % 2 != 0;
        };

        CHECK(filter(is_odd, std_vector_3) == std::vector<double>{1, 3});
    }
}

TEST_CASE("foldl")
{
    SECTION("plus on c style arary")
    {
        auto plus = [](int lhs, int rhs)
        {
            return lhs + rhs;
        };
        CHECK(foldl(plus, 0, c_array_5) == 15);
    }

    SECTION("times on std::vectors arary")
    {
        auto times = [](double lhs, double rhs)
        {
            return lhs * rhs;
        };
        CHECK(foldl(times, 1., std_vector_3) == 6.);
    }
}

TEST_CASE("foldr")
{
    SECTION("plus on c style arary")
    {
        auto plus = [](int lhs, int rhs)
        {
            return lhs + rhs;
        };
        CHECK(foldr(plus, 0, c_array_5) == 15);
    }

    SECTION("times on std::vectors arary")
    {
        auto times = [](double lhs, double rhs)
        {
            return lhs * rhs;
        };
        CHECK(foldr(times, 1., std_vector_3) == 6.);
    }
}

TEST_CASE("from_function")
{
    SECTION("StaticArray")
    {
        auto plus_one = [](int i)
        {
            return i + 1;
        };

        CHECK(from_function(StaticSizeT<3>{}, plus_one) == StaticArray<int, 3>{1, 2, 3});
    }

    SECTION("DynamicArray")
    {
        auto plus_one = [](int i)
        {
            return i + 1;
        };

        CHECK(from_function(3, plus_one) == DynamicVector<int>{1, 2, 3});
    }
}

#endif