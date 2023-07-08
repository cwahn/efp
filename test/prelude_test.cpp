#include "catch2/catch_test_macros.hpp"

#include "prelude.hpp"

int c_array_3[3] = {1, 2, 3};
int c_array_5[5] = {1, 2, 3, 4, 5};

std::array<double, 3> std_array_3 = {1., 2., 3.};
std::array<double, 5> std_array_5 = {1., 2., 3., 4., 5.};

std::vector<double> std_vector_0 = {};
std::vector<double> std_vector_3 = {1., 2., 3.};
std::vector<double> std_vector_5 = {1., 2., 3., 4., 5.};

TEST_CASE("IsArray")
{
    REQUIRE(IsArray<int[3]>::value == true);
    REQUIRE(IsArray<std::array<int, 3>>::value == true);
    REQUIRE(IsArray<std::vector<int>>::value == false);
}

TEST_CASE("AreAllArrayss")
{
    REQUIRE(AreAllArrays<int[3]>::value == true);
    REQUIRE(AreAllArrays<std::array<int, 3>>::value == true);
    REQUIRE(AreAllArrays<int[3], std::array<int, 3>>::value == true);
    REQUIRE(AreAllArrays<std::vector<int>>::value == false);
    REQUIRE(AreAllArrays<std::array<int, 3>, std::vector<int>>::value == false);
}

TEST_CASE("for_each")
{
    int res_1 = 0;
    auto add_1 = [&](int x)
    {
        res_1 += x;
    };

    for_each(add_1, c_array_3);
    REQUIRE(res_1 == 6);

    double res_2 = 0.;
    auto add_2 = [&](double x)
    {
        res_2 += x;
    };

    for_each(add_2, std_array_3);
    REQUIRE(res_2 == 6.);

    double res_3 = 0.;
    auto add_3 = [&](double x)
    {
        res_3 += x;
    };

    for_each(add_3, std_vector_3);
    REQUIRE(res_3 == 6.);

    double res_4 = 0.;
    auto add_product = [&](double x1, double x2)
    {
        res_4 += x1 * x2;
    };

    for_each(add_product, std_vector_3, c_array_3);
    REQUIRE(res_4 == 14.);
}

TEST_CASE("fmap")
{
    SECTION("c style array")
    {
        auto square = [](int x)
        {
            return x * x;
        };

        REQUIRE(fmap(square, c_array_3) == std::array<int, 3>{1, 4, 9});
    }

    SECTION("std::vector")
    {
        auto square = [](int x)
        {
            return x * x;
        };

        REQUIRE(fmap(square, std_vector_3) == std::vector<int>{1, 4, 9});
    }

    SECTION("binary of std::array, std::vecor")
    {
        auto plus = [](int lhs, int rhs)
        {
            return lhs + rhs;
        };
        std::vector<int> ref = {2, 4, 6};

        REQUIRE(fmap(plus, std_array_3, std_vector_3) == std::vector<int>{2, 4, 6});
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

        REQUIRE(filter(is_even, c_array_3) == std::vector<int>{2});
    }

    SECTION("odd std::vector")
    {
        auto is_odd = [](int x)
        {
            return x % 2 != 0;
        };

        REQUIRE(filter(is_odd, std_vector_3) == std::vector<double>{1, 3});
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
        REQUIRE(foldl(plus, 0, c_array_5) == 15);
    }

    SECTION("times on std::vectors arary")
    {
        auto times = [](double lhs, double rhs)
        {
            return lhs * rhs;
        };
        REQUIRE(foldl(times, 1., std_vector_3) == 6.);
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
        REQUIRE(foldr(plus, 0, c_array_5) == 15);
    }

    SECTION("times on std::vectors arary")
    {
        auto times = [](double lhs, double rhs)
        {
            return lhs * rhs;
        };
        REQUIRE(foldr(times, 1., std_vector_3) == 6.);
    }
}